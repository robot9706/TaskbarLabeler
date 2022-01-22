#include "wnd_proc.h"
#include <shlobj.h>
#include "MinHook/MinHook.h"
#include "functions.h"
#include "explorer_vars.h"
#include "pointer_redirection.h"
#include "com_func_hook.h"
#include "labels.h"

#include <stdint.h>

// External
#define MSG_DLL_SETSHOWPIDS		(-1)

// External WM_COPYDATA commands
#define MSG_DLL_COPYDATA_CMD_REMOVE (-1)
#define MSG_DLL_COPYDATA_CMD_ADD (-2)
#define MSG_DLL_COPYDATA_CMD_BUTTONS (-3)

// Internal
#define MSG_DLL_INIT           (-2)
#define MSG_DLL_UNSUBCLASS     (-3)
#define MSG_DLL_CALLFUNC       (-4)

static LRESULT THISCALL_C InitTaskbarProc(THISCALL_C_THIS_ARG(LONG_PTR *this_ptr), HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam);
static void SubclassExplorerWindows(void);
static void UnsubclassExplorerWindows(void);
static BOOL InitFromExplorerThread(void);
static void UnintializeTweakerComponents(void);
static LONG_PTR ExitFromExplorerThread(void);
static LRESULT CALLBACK NewTaskbarProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam, UINT_PTR uIdSubclass, DWORD_PTR dwRefData);
static LRESULT CALLBACK NewTaskSwProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam, UINT_PTR uIdSubclass, DWORD_PTR dwRefData);
static void InvalidateSecondaryTaskListWndRect();
static void SendButtonInfo();

// superglobals
extern UINT uPrivateMsg;
extern DWORD dwTaskbarThreadId;
extern HWND hTaskbarWnd, hTaskSwWnd, hTaskListWnd;
extern LONG_PTR lpTaskbarLongPtr, lpTaskSwLongPtr, lpTaskListLongPtr;

extern BOOL showPIDs;
extern HWND ownerHandle;

// subclasses
static WNDPROC pOldTaskbarProc;
static volatile int wnd_proc_call_counter;

// hooks
static void **ppTaskbarSubWndProc;
POINTER_REDIRECTION_VAR(static POINTER_REDIRECTION prTaskbarSubWndProc);

static BOOL bUnintializeStarted;

BOOL WndProcInit(void)
{
	HANDLE hExplorerIsShellMutex;
	DWORD dwError;

	// Set some globals
	uPrivateMsg = RegisterWindowMessage(L"TaskbarLabeler");
	showPIDs = FALSE;

	PostMessage(ownerHandle, uPrivateMsg, (WPARAM)0xAB, (LPARAM)0xCD);

	// Wait until explorer shell is created
	hExplorerIsShellMutex = OpenMutex(SYNCHRONIZE, FALSE, L"Local\\ExplorerIsShellMutex");
	if(hExplorerIsShellMutex)
	{
		switch(WaitForSingleObject(hExplorerIsShellMutex, INFINITE))
		{
		case WAIT_OBJECT_0:
		case WAIT_ABANDONED:
			ReleaseMutex(hExplorerIsShellMutex);
			break;
		}

		CloseHandle(hExplorerIsShellMutex);
	}

	// Find our windows, and get their LongPtr's
	hTaskbarWnd = FindWindow(L"Shell_TrayWnd", NULL);
	if(!hTaskbarWnd)
		return FALSE;

	lpTaskbarLongPtr = GetWindowLongPtr(hTaskbarWnd, 0);
	hTaskSwWnd = (HWND)GetProp(hTaskbarWnd, L"TaskbandHWND");
	if(!hTaskSwWnd)
		return FALSE;

	lpTaskSwLongPtr = GetWindowLongPtr(hTaskSwWnd, 0);

	hTaskListWnd = FindWindowEx(hTaskSwWnd, NULL, L"MSTaskListWClass", NULL);
	if(!hTaskListWnd)
		return FALSE;
	lpTaskListLongPtr = GetWindowLongPtr(hTaskListWnd, 0);

	// Init labels
	LabelsInit();

	// Init other stuff
	ppTaskbarSubWndProc = &((*(void ***)lpTaskbarLongPtr)[2]);
	PointerRedirectionAdd(ppTaskbarSubWndProc, InitTaskbarProc, &prTaskbarSubWndProc);

	dwError = (DWORD)SendMessage(hTaskbarWnd, uPrivateMsg, 0, MSG_DLL_INIT);
	if(dwError == 0)
	{
		PointerRedirectionRemove(ppTaskbarSubWndProc, &prTaskbarSubWndProc);
		return FALSE;
	}

	dwError -= 1;
	if(dwError)
	{
		SendMessage(hTaskbarWnd, uPrivateMsg, 0, MSG_DLL_UNSUBCLASS);
		while(wnd_proc_call_counter > 0)
			Sleep(10);

		return FALSE;
	}

	return TRUE;
}

static LRESULT THISCALL_C InitTaskbarProc(THISCALL_C_THIS_ARG(LONG_PTR *this_ptr), HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	DWORD dwError;

	if(uMsg == uPrivateMsg && lParam == MSG_DLL_INIT)
	{
		PointerRedirectionRemove(ppTaskbarSubWndProc, &prTaskbarSubWndProc);

		SubclassExplorerWindows();

		dwError = 0;

		if(!InitFromExplorerThread())
			dwError = 1;

		return 1 + dwError;
	}

	return ((LRESULT(THISCALL_C *)(THISCALL_C_THIS_TYPE(LONG_PTR *), HWND, UINT, WPARAM, LPARAM))prTaskbarSubWndProc.pOriginalAddress)(THISCALL_C_THIS_VAL(this_ptr), hWnd, uMsg, wParam, lParam);
}

static void SubclassExplorerWindows(void)
{
	SetWindowSubclass(hTaskbarWnd, NewTaskbarProc, 0, 0);
	SetWindowSubclass(hTaskSwWnd, NewTaskSwProc, 0, 0);
}

static void UnsubclassExplorerWindows(void)
{
	RemoveWindowSubclass(hTaskbarWnd, NewTaskbarProc, 0);
	RemoveWindowSubclass(hTaskSwWnd, NewTaskSwProc, 0);
}

static BOOL InitFromExplorerThread(void)
{
	dwTaskbarThreadId = GetCurrentThreadId();

	if(MH_Initialize() == MH_OK)
	{
		if(ComFuncHook_Init())
		{
			if(MH_ApplyQueued() == MH_OK)
			{
				InvalidateRect(hTaskListWnd, NULL, FALSE);

				InvalidateSecondaryTaskListWndRect();

				return TRUE;
			}
		}

		MH_Uninitialize();
	}

	return FALSE;
}

void WndProcExit(void)
{
	LRESULT lExited = SendMessage(hTaskbarWnd, uPrivateMsg, (WPARAM)ExitFromExplorerThread, MSG_DLL_CALLFUNC);
	if(lExited)
	{
		ComFuncHook_WaitTillDone();

		SendMessage(hTaskbarWnd, uPrivateMsg, 0, MSG_DLL_UNSUBCLASS);
	}
}

void WndProcWaitTillDone(void)
{
	while(wnd_proc_call_counter > 0)
		Sleep(10);
}

static LONG_PTR ExitFromExplorerThread(void)
{
	if(bUnintializeStarted)
		return 0;

	bUnintializeStarted = TRUE;

	InvalidateRect(hTaskListWnd, NULL, FALSE);

	InvalidateSecondaryTaskListWndRect();

	UnintializeTweakerComponents();

	return 1;
}

static void UnintializeTweakerComponents(void)
{
	ComFuncHook_Exit();
	MH_Uninitialize();
}

static LRESULT CALLBACK NewTaskbarProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam, UINT_PTR uIdSubclass, DWORD_PTR dwRefData)
{
	int nOldOpt, nNewOpt;
	LRESULT result;

	wnd_proc_call_counter++;

	switch(uMsg)
	{
	case WM_DESTROY:
		if(!bUnintializeStarted)
		{
			bUnintializeStarted = TRUE;

			result = DefSubclassProc(hWnd, uMsg, wParam, lParam);

			UnintializeTweakerComponents();
			UnsubclassExplorerWindows();
		}
		else
			result = DefSubclassProc(hWnd, uMsg, wParam, lParam);
		break;

	case WM_WTSSESSION_CHANGE:
		result = DefSubclassProc(hWnd, uMsg, wParam, lParam);
		break;

	case WM_COPYDATA:
	{
		BOOL messageProcessed = FALSE;

		COPYDATASTRUCT* pcds = (COPYDATASTRUCT*)lParam;
		if (wParam == pcds->dwData) // wParam and the dwData should match
		{
			switch (pcds->dwData) // Let's see if this is one of our commands
			{
			case MSG_DLL_COPYDATA_CMD_REMOVE:
			{
				struct WM_COPYDATA_COMMAND_REMOVE* removeCommand = (struct WM_COPYDATA_COMMAND_REMOVE*)pcds->lpData;
				if (removeCommand->magic == WM_COPYDATA_COMMAND_MAGIC) // Make sure the dwData doesn't accidentally match
				{
					RemoveLabel(removeCommand->pid);

					messageProcessed = TRUE;
				}
				break;
			}
			case MSG_DLL_COPYDATA_CMD_ADD:
			{
				struct WM_COPYDATA_COMMAND_ADD* addCommand = (struct WM_COPYDATA_COMMAND_ADD*)pcds->lpData;
				if (addCommand->magic == WM_COPYDATA_COMMAND_MAGIC)
				{
					AddLabel(addCommand->pid, addCommand->fillColor, &addCommand->text);

					messageProcessed = TRUE;
				}
				break;
			}
			}
		}

		if (messageProcessed)
		{
			// If the message is processed, redraw
			InvalidateRect(hTaskListWnd, NULL, FALSE);
			InvalidateSecondaryTaskListWndRect();
		}
		else
		{
			// Otherwise pass it forward
			result = DefSubclassProc(hWnd, uMsg, wParam, lParam);
		}
		break;
	}

	default:
		if(uMsg == uPrivateMsg)
		{
			result = 0;

			switch(lParam)
			{
			case MSG_DLL_UNSUBCLASS:
				UnsubclassExplorerWindows();
				break;

			case MSG_DLL_CALLFUNC:
				result = ((LONG_PTR(*)())wParam)();
				break;

			case MSG_DLL_SETSHOWPIDS:
			{
				showPIDs = (wParam == 1);

				InvalidateRect(hTaskListWnd, NULL, FALSE);
				InvalidateSecondaryTaskListWndRect();
				break;
			}
			}
		}
		else
			result = DefSubclassProc(hWnd, uMsg, wParam, lParam);
		break;
	}

	wnd_proc_call_counter--;

	return result;
}

static LRESULT CALLBACK NewTaskSwProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam, UINT_PTR uIdSubclass, DWORD_PTR dwRefData)
{
	HANDLE hChange;
	PIDLIST_ABSOLUTE *ppidl;
	long lEvent;
	LRESULT result;

	wnd_proc_call_counter++;

	switch(uMsg)
	{
	case 0x044A: // Button create
		result = DefSubclassProc(hWnd, uMsg, wParam, lParam);

		ComFuncButtonCreatedOrPinnedItemChange();

		SendButtonInfo();
		break;

	case 0x454: // Button destroyed
		SendButtonInfo();

		result = DefSubclassProc(hWnd, uMsg, wParam, lParam);
		break;

	case 0x043A: // Registered with SHChangeNotifyRegister
		hChange = SHChangeNotification_Lock((HANDLE)wParam, (DWORD)lParam, &ppidl, &lEvent);
		if(hChange)
		{
			if(lEvent == SHCNE_EXTENDED_EVENT)
			{
				if(ppidl && ppidl[0] && *(DWORD *)&ppidl[0]->mkid.abID == 0x0D) // Pinned item change (add or remove)
					ComFuncButtonCreatedOrPinnedItemChange();
			}

			SHChangeNotification_Unlock(hChange);
		}

		result = DefSubclassProc(hWnd, uMsg, wParam, lParam);
		break;

	default:
		result = DefSubclassProc(hWnd, uMsg, wParam, lParam);
		break;
	}

	wnd_proc_call_counter--;

	return result;
}

void InvalidateSecondaryTaskListWndRect()
{
	SECONDARY_TASK_LIST_GET secondary_task_list_get;
	LONG_PTR lpSecondaryTaskListLongPtr;
	HWND hSecondaryTaskListWnd;

	lpSecondaryTaskListLongPtr = SecondaryTaskListGetFirstLongPtr(&secondary_task_list_get);
	while(lpSecondaryTaskListLongPtr)
	{
		hSecondaryTaskListWnd = *(HWND *)(lpSecondaryTaskListLongPtr + DEF3264(0x04, 0x08));
		InvalidateRect(hSecondaryTaskListWnd, NULL, FALSE);

		lpSecondaryTaskListLongPtr = SecondaryTaskListGetNextLongPtr(&secondary_task_list_get);
	}
}

void SendButtonInfo()
{
	int head = 1;
	DWORD pids[129] = { 0 };
	pids[0] = WM_COPYDATA_COMMAND_MAGIC;

	LONG_PTR* plp = (LONG_PTR*)*EV_MM_TASKLIST_BUTTON_GROUPS_HDPA(lpTaskListLongPtr);
	if (plp)
	{
		int button_groups_count = (int)plp[0];
		LONG_PTR** button_groups = (LONG_PTR**)plp[1];

		for (int i = 0; i < button_groups_count; i++)
		{
			int button_group_type = (int)button_groups[i][DO2(6, 8)];
			if (button_group_type < 0 || button_group_type > 4)
				break; // 4 is the hack of removing pinned items gap in the original code

			if (button_group_type == 1 || button_group_type == 3)
			{
				DWORD pid = GetButtonGroupPID(button_groups[i]);

				pids[head] = pid;
				head++;

				if (head >= 129) 
				{
					break;
				}
			}
		}
	}

	// Send info
	COPYDATASTRUCT cds;
	cds.dwData = MSG_DLL_COPYDATA_CMD_BUTTONS;
	cds.cbData = sizeof(DWORD) * head;
	cds.lpData = pids;

	SendMessage(ownerHandle, WM_COPYDATA, (WPARAM)MSG_DLL_COPYDATA_CMD_BUTTONS, (LPARAM)&cds);
}