#include "com_func_hook.h"
#include <uxtheme.h>
#include <commoncontrols.h>
#include "MinHook/MinHook.h"
#include "functions.h"
#include "explorer_vars.h"
#include "pointer_redirection.h"
#include "labels.h"

static BOOL HookTaskBtnGroupFunctions();
static void UnhookTaskBtnGroupFunctions();
static LONG_PTR __stdcall RenderHook(LONG_PTR *button_group, LONG_PTR var2, LONG_PTR var3, LONG_PTR var4, LONG_PTR var5);
static LONG_PTR __stdcall RenderHook2(LONG_PTR *button_group, LONG_PTR var2, LONG_PTR var3, LONG_PTR var4, LONG_PTR var5, LONG_PTR var6);
static BOOL RenderHookPreOperation(LONG_PTR *button_group);
static void RenderHookPostOperation();
static LONG_PTR __stdcall TaskbarImageListDrawHook(LONG_PTR var1, IMAGELISTDRAWPARAMS *pimldp);
static int ButtonGroupFirstButtonIndex(LONG_PTR lpMMTaskListLongPtr, LONG_PTR *button_group);
static void TaskbarImageListDrawText(HTHEME hTheme, IMAGELISTDRAWPARAMS *pimldp, WCHAR *pText, BOOL bSmallIcons, HBRUSH fillBrush);

// superglobals
extern DWORD dwTaskbarThreadId;
extern HWND hTaskbarWnd, hTaskSwWnd, hTaskListWnd;
extern LONG_PTR lpTaskbarLongPtr, lpTaskSwLongPtr, lpTaskListLongPtr;

extern BOOL showPIDs;

// hooks
static void **ppRender;
static void *pRender;
POINTER_REDIRECTION_VAR(static POINTER_REDIRECTION prRender);
static void **ppTaskbarImageListDraw;
static void *pTaskbarImageListDraw;
POINTER_REDIRECTION_VAR(static POINTER_REDIRECTION prTaskbarImageListDraw);
static void **ppTrayToolbarImageListDraw;
static void *pTrayToolbarImageListDraw;
POINTER_REDIRECTION_VAR(static POINTER_REDIRECTION prTrayToolbarImageListDraw);
static BOOL bTaskBtnGroupFunctionsHooked;
volatile static int hook_proc_call_counter;

static HTHEME hTaskListTheme;
static int nButtonNumber;
static DWORD currentButtonPid;
static BOOL bButtonIsSmall;
static HWND hPaintingToolbar;
static BOOL bButtonIsPinned;

// Hooks
static BOOL CreateEnableHook(void** ppTarget, void* const pDetour, void** ppOriginal, POINTER_REDIRECTION *ppr);
static BOOL DisableHook(void** ppTarget, POINTER_REDIRECTION *ppr);

BOOL ComFuncHook_Init()
{
	// TaskListWnd functions hook
	if(HookTaskBtnGroupFunctions())
		bTaskBtnGroupFunctionsHooked = TRUE;

	return TRUE;
}

void ComFuncHook_Exit()
{
	if(bTaskBtnGroupFunctionsHooked)
		UnhookTaskBtnGroupFunctions();
}

void ComFuncHook_WaitTillDone()
{
	while(hook_proc_call_counter > 0)
		Sleep(10);
}

HWND GetTaskItemWnd(LONG_PTR* task_item)
{
	LONG_PTR this_ptr;
	LONG_PTR* plp;

	this_ptr = (LONG_PTR)task_item;
	plp = *(LONG_PTR**)this_ptr;

	// CWindowTaskItem::GetWindow(this)
	return FUNC_CWindowTaskItem_GetWindow(plp)(this_ptr);
}

DWORD GetButtonGroupPID(LONG_PTR* button_group)
{
	LONG_PTR* plp = FUNC_BUTTONGROUP_BUTTONS(button_group);
	if (!plp)
		return;

	int buttons_count = (int)plp[0];
	LONG_PTR** buttons = (LONG_PTR**)plp[1];

	if (buttons_count > 0)
	{
		LONG_PTR* button = buttons[0];

		LONG_PTR* task_item = FUNC_BUTTON_TASKITEM(button);
		HWND hwnd = GetTaskItemWnd(task_item);

		DWORD pid;
		GetWindowThreadProcessId(hwnd, &pid);

		return pid;
	}

	return 0;
}

static BOOL HookTaskBtnGroupFunctions()
{
	LONG_PTR *plp;
	SECONDARY_TASK_LIST_GET secondary_task_list_get;
	LONG_PTR lpSecondaryTaskListLongPtr;

	plp = (LONG_PTR *)*EV_MM_TASKLIST_BUTTON_GROUPS_HDPA(lpTaskListLongPtr);
	if(!plp || (int)plp[0] == 0)
	{
		lpSecondaryTaskListLongPtr = SecondaryTaskListGetFirstLongPtr(&secondary_task_list_get);
		while(lpSecondaryTaskListLongPtr)
		{
			plp = (LONG_PTR *)*EV_MM_TASKLIST_BUTTON_GROUPS_HDPA(lpSecondaryTaskListLongPtr);
			if(!plp || (int)plp[0] == 0)
				lpSecondaryTaskListLongPtr = SecondaryTaskListGetNextLongPtr(&secondary_task_list_get);
			else
				break;
		}

		if(!lpSecondaryTaskListLongPtr)
			return FALSE;
	}

	plp = (LONG_PTR *)plp[1]; // CTaskBtnGroup DPA array
	plp = (LONG_PTR *)plp[0]; // First array item
	plp = (LONG_PTR *)plp[0]; // COM functions list

	void *pRenderHook;
	if(nWinVersion >= WIN_VERSION_10_R4)
		pRenderHook = RenderHook2;
	else
		pRenderHook = RenderHook;

	ppRender = (void **)&plp[DO5(18, 20, , , 21)];
	if(!CreateEnableHook(ppRender, pRenderHook, &pRender, &prRender))
	{
		UnhookTaskBtnGroupFunctions();
		return FALSE;
	}

	bTaskBtnGroupFunctionsHooked = TRUE;
	return TRUE;
}

static void UnhookTaskBtnGroupFunctions()
{
	DisableHook(ppRender, &prRender);
}

static LONG_PTR __stdcall RenderHook(LONG_PTR *button_group, LONG_PTR var2, LONG_PTR var3, LONG_PTR var4, LONG_PTR var5)
{
	BOOL bProcessed;
	LONG_PTR lpRet;

	InterlockedIncrement((long *)&hook_proc_call_counter);

	bProcessed = RenderHookPreOperation(button_group);

	lpRet = ((LONG_PTR(__stdcall *)(LONG_PTR *, LONG_PTR, LONG_PTR, LONG_PTR, LONG_PTR))pRender)
		(button_group, var2, var3, var4, var5);

	if(bProcessed)
		RenderHookPostOperation();

	InterlockedDecrement((long *)&hook_proc_call_counter);

	return lpRet;
}

static LONG_PTR __stdcall RenderHook2(LONG_PTR *button_group, LONG_PTR var2, LONG_PTR var3, LONG_PTR var4, LONG_PTR var5, LONG_PTR var6)
{
	BOOL bProcessed;
	LONG_PTR lpRet;

	InterlockedIncrement((long *)&hook_proc_call_counter);

	bProcessed = RenderHookPreOperation(button_group);

	lpRet = ((LONG_PTR(__stdcall *)(LONG_PTR *, LONG_PTR, LONG_PTR, LONG_PTR, LONG_PTR, LONG_PTR))pRender)
		(button_group, var2, var3, var4, var5, var6);

	if(bProcessed)
		RenderHookPostOperation();

	InterlockedDecrement((long *)&hook_proc_call_counter);

	return lpRet;
}

static BOOL RenderHookPreOperation(LONG_PTR *button_group)
{
	LONG_PTR lpMMTaskListLongPtr;
	LONG_PTR lp;
	int nNumber;
	HIMAGELIST hImageList;
	IImageList *imageList;
	HRESULT hr;

	// multimonitor environment
	if(nWinVersion >= WIN_VERSION_8)
		lpMMTaskListLongPtr = button_group[3];
	else
		lpMMTaskListLongPtr = lpTaskListLongPtr;

	nNumber = -1;

	int nIndex = ButtonGroupFirstButtonIndex(lpMMTaskListLongPtr, button_group);
	if (nIndex >= 0)
		nNumber = 1 + nIndex;

	int button_group_type = FUNC_BUTTONGROUP_TYPE(button_group);
	bButtonIsPinned = (button_group_type == 2);

	if(nNumber >= 0)
	{
		nButtonNumber = nNumber;
		bButtonIsSmall = ((TaskbarGetPreference(lpMMTaskListLongPtr) & 0x10) != 0);
		currentButtonPid = GetButtonGroupPID(button_group);

		// DEF3264: CTaskListWnd::_HandlePaint -or- CTaskListWnd::_HandleThemeChanged
		hTaskListTheme = *(HTHEME *)(lpTaskListLongPtr + DO8_3264(0x3C, 0x78, 0x40, 0x80, ,, ,, ,, ,, 0x34, 0x68, 0x38, 0x70));
		if(hTaskListTheme)
		{
			lp = *EV_MM_TASKLIST_TASK_BAND_REF(lpTaskListLongPtr);

			if(nWinVersion <= WIN_VERSION_811)
			{
				// DEF3264: CTaskBand::GetImageList
				hImageList = *(HIMAGELIST *)(lp + DO4_3264(0x40, 0x60, 0x34, 0x50, 0x38, 0x58, 0x44, 0x68));
			}
			else // if(nWinVersion >= WIN_VERSION_10)
			{
				// DEF3264: CTaskBand::GetImageList
				lp = *(LONG_PTR *)(lp + DO11_3264(0, 0, ,, ,, ,,
					0x50 - 0x04, 0x80 - 0x08,
					0x54 - 0x04, 0x88 - 0x08,
					0x5C - 0x04, 0xA0 - 0x08,
					0x60 - 0x04, 0xA0 - 0x08,
					,,
					,,
					0x58 - 0x04, 0x98 - 0x08));

				// DEF3264: IconContainer::GetReadOnlyImageListForDPI
				hImageList = **(HIMAGELIST **)(lp + DEF3264(0x20, 0x38));
			}

			// CImageList::Draw
			hr = HIMAGELIST_QueryInterface(hImageList, &IID_IImageList, (void **)&imageList);
			if(SUCCEEDED(hr))
			{
				ppTaskbarImageListDraw = (void **)&imageList->lpVtbl->Draw;

				PointerRedirectionAdd(ppTaskbarImageListDraw, TaskbarImageListDrawHook, &prTaskbarImageListDraw);

				imageList->lpVtbl->Release(imageList);

				return TRUE;
			}
		}
	}

	return FALSE;
}

static void RenderHookPostOperation()
{
	PointerRedirectionRemove(ppTaskbarImageListDraw, &prTaskbarImageListDraw);
}

static LONG_PTR __stdcall TaskbarImageListDrawHook(LONG_PTR var1, IMAGELISTDRAWPARAMS *pimldp)
{
	LONG_PTR lpRet;

	InterlockedIncrement((long *)&hook_proc_call_counter);

	// Render the original taskbar item
	lpRet = ((LONG_PTR(__stdcall *)(LONG_PTR, IMAGELISTDRAWPARAMS *))prTaskbarImageListDraw.pOriginalAddress)(var1, pimldp);

	if(GetCurrentThreadId() == dwTaskbarThreadId)
	{
		if (pimldp->himl)
		{
			if (!bButtonIsPinned) // If the button is pinned the PID is 0, so skip it
			{
				// Custom rendering
				if (showPIDs)
				{
					// Show PIDs on taskbar items
					if (currentButtonPid != 0) // 0 is a pinned placeholder item
					{
						WCHAR pidText[32];
						wsprintf(pidText, L"%ld", currentButtonPid);

						TaskbarImageListDrawText(hTaskListTheme, pimldp, pidText, bButtonIsSmall, (HBRUSH)GetStockObject(BLACK_BRUSH));
					}
				}
				else
				{
					// Render custom labels...
					struct label_entry* entry = GetLabelEntry(currentButtonPid);

					if (entry != NULL) // ...if theres one
					{
						TaskbarImageListDrawText(hTaskListTheme, pimldp, entry->text, bButtonIsSmall, entry->brush);
					}
				}
			}

			nButtonNumber++;
		}
	}

	InterlockedDecrement((long *)&hook_proc_call_counter);

	return lpRet;
}

static int ButtonGroupFirstButtonIndex(LONG_PTR lpMMTaskListLongPtr, LONG_PTR *button_group)
{
	LONG_PTR *plp;
	int button_groups_count;
	LONG_PTR **button_groups;
	int button_group_type;
	int buttons_count;
	LONG_PTR **buttons;
	int index;
	int i;

	if(button_group)
	{
		plp = (LONG_PTR *)*EV_MM_TASKLIST_BUTTON_GROUPS_HDPA(lpMMTaskListLongPtr);
		if(plp)
		{
			index = 0;

			button_groups_count = (int)plp[0];
			button_groups = (LONG_PTR **)plp[1];

			for(i = 0; i < button_groups_count; i++)
			{
				if(button_group == button_groups[i])
					return index;

				button_group_type = FUNC_BUTTONGROUP_TYPE(button_groups[i]);
				if(button_group_type == 1)
				{
					plp = (LONG_PTR *)button_groups[i][DO2(5, 7)];

					buttons_count = (int)plp[0];
					buttons = (LONG_PTR **)plp[1];

					index += buttons_count;
				}
				else
					index++;
			}
		}
	}

	return -1;
}

static void TaskbarImageListDrawText(HTHEME hTheme, IMAGELISTDRAWPARAMS *pimldp, WCHAR *pText, BOOL bSmallIcons, HBRUSH fillBrush)
{
	int cx, cy;
	RECT rc;
	DTTOPTS dtt_options;

	// Rect init
	ImageList_GetIconSize(pimldp->himl, &cx, &cy);

	rc.left = pimldp->x - 10;
	rc.top = pimldp->y - 8;
	rc.right = pimldp->x + cx + 10;
	rc.bottom = pimldp->y + cy + 6;

	// Rect calc
	ZeroMemory(&dtt_options, sizeof(DTTOPTS));
	dtt_options.dwSize = sizeof(DTTOPTS);
	dtt_options.dwFlags = DTT_COMPOSITED;

	FillRect(pimldp->hdcDst, &rc, fillBrush);
	FrameRect(pimldp->hdcDst, &rc, (HBRUSH)GetStockObject(WHITE_BRUSH));

	rc.left += 2;
	rc.right -= 2;
	rc.bottom -= 2;

	dtt_options.dwFlags |= (DTT_SHADOWTYPE & DTT_SHADOWOFFSET);
	dtt_options.iTextShadowType = 1;
	dtt_options.ptShadowOffset.x = 2;
	dtt_options.ptShadowOffset.y = 2;

	DrawThemeTextEx(hTheme, pimldp->hdcDst, 0, 1, pText, -1, DT_NOPREFIX | DT_END_ELLIPSIS | DT_WORDBREAK, &rc, &dtt_options);
}

void ComFuncButtonCreatedOrPinnedItemChange()
{
	BOOL bApplyQueuedHooks = FALSE;

	if(!bTaskBtnGroupFunctionsHooked)
		if(HookTaskBtnGroupFunctions() && bTaskBtnGroupFunctionsHooked)
			bApplyQueuedHooks = TRUE;

	if(bApplyQueuedHooks)
		MH_ApplyQueued();
}

// Hooks
static BOOL CreateEnableHook(void** ppTarget, void* const pDetour, void** ppOriginal, POINTER_REDIRECTION *ppr)
{
	void* pTarget;
	MH_STATUS status;

	pTarget = PointerRedirectionGetOriginalPtr(ppTarget);

	status = MH_CreateHook(pTarget, pDetour, ppOriginal);
	if(status == MH_OK)
	{
		status = MH_QueueEnableHook(pTarget);
		if(status == MH_OK)
		{
			PointerRedirectionAdd(ppTarget, pDetour, ppr);
			return TRUE;
		}

		MH_RemoveHook(pTarget);
	}

	*ppOriginal = NULL;
	return FALSE;
}

static BOOL DisableHook(void** ppTarget, POINTER_REDIRECTION *ppr)
{
	if(ppr->pOriginalAddress)
	{
		PointerRedirectionRemove(ppTarget, ppr);
	}

	return TRUE;
}
