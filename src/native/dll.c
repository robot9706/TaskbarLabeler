#include <windows.h>
#include "explorer_inject.h"
#include "errors.h"
#include "commands.h"

#define EXPORT __declspec(dllexport)

// uPrivateMsg commands
#define MSG_DLL_SETSHOWPIDS		(-1)

// WM_COPYDATA commands
#define MSG_DLL_COPYDATA_CMD_REMOVE (-1)
#define MSG_DLL_COPYDATA_CMD_ADD (-2)
#define MSG_DLL_COPYDATA_CMD_BUTTONS (-3)

struct enum_window_data {
    unsigned long PID;
    HWND resultWindow;
};

static UINT uPrivateMsg;

static BOOL CompareWindowsVersion(DWORD dwMajorVersion, DWORD dwMinorVersion)
{
    OSVERSIONINFOEX ver;
    DWORDLONG dwlConditionMask = 0;

    ZeroMemory(&ver, sizeof(OSVERSIONINFOEX));
    ver.dwOSVersionInfoSize = sizeof(OSVERSIONINFOEX);
    ver.dwMajorVersion = dwMajorVersion;
    ver.dwMinorVersion = dwMinorVersion;

    VER_SET_CONDITION(dwlConditionMask, VER_MAJORVERSION, VER_EQUAL);
    VER_SET_CONDITION(dwlConditionMask, VER_MINORVERSION, VER_EQUAL);

    return VerifyVersionInfo(&ver, VER_MAJORVERSION | VER_MINORVERSION, dwlConditionMask);
}

static void SendDataCommand(ULONG_PTR cmd, DWORD dataSize, PVOID data)
{
    COPYDATASTRUCT cds;
    cds.dwData = cmd;
    cds.cbData = dataSize;
    cds.lpData = data;

    SendMessage(ExplorerGetTaskbarWnd(), WM_COPYDATA, (WPARAM)cmd, (LPARAM)&cds);
}

BOOL CALLBACK EnumWindowsFilter(HWND handle, LPARAM lParam)
{
    struct enum_window_data* data = (struct enum_window_data*)lParam;
    
    DWORD pid = 0;
    GetWindowThreadProcessId(handle, &pid);

    BOOL mainWindow = (GetWindow(handle, GW_OWNER) == (HWND)0 && IsWindowVisible(handle));

    if (data->PID != pid || !mainWindow)
        return TRUE; // Continue

    data->resultWindow = handle; // Found
    return FALSE;
}

// Exported functions
EXPORT INT32 __cdecl native_init(HANDLE owner)
{
    // Check windows version
    if (
        !CompareWindowsVersion(6, 1) &&
        !CompareWindowsVersion(6, 2) &&
        !CompareWindowsVersion(6, 3) &&
        !CompareWindowsVersion(6, 4) &&
        !CompareWindowsVersion(10, 0)
        )
    {
        return INIT_ERROR_OS_VERSION;
    }

    // Check bitness
#ifndef _WIN64
    BOOL bWow64Process;
    if (IsWow64Process(GetCurrentProcess(), &bWow64Process) && bWow64Process)
    {
        return INIT_ERROR_BITNESS;
    }
#endif

    // Setup
    uPrivateMsg = RegisterWindowMessage(L"TaskbarLabeler");

    // Inject
    return ExplorerInject(owner);
}

EXPORT BOOL __cdecl native_exit() 
{
    ExplorerCleanup();

    return TRUE;
}

EXPORT BOOL __cdecl native_is_private_message(UINT message) 
{
    return (message == uPrivateMsg);
}

EXPORT void __cdecl native_set_showpids(BOOL showPIDs)
{
    SendMessage(ExplorerGetTaskbarWnd(), uPrivateMsg, showPIDs ? 1 : 0, MSG_DLL_SETSHOWPIDS);
}

EXPORT void __cdecl native_remove_label(DWORD pid)
{
    struct WM_COPYDATA_COMMAND_REMOVE cmd = {
        .magic = WM_COPYDATA_COMMAND_MAGIC,
        .pid = pid,
    };

    SendDataCommand(MSG_DLL_COPYDATA_CMD_REMOVE, sizeof(struct WM_COPYDATA_COMMAND_REMOVE), &cmd);

    return TRUE;
}

EXPORT BOOL __cdecl native_add_label(DWORD pid, DWORD color, WCHAR *text) 
{
    struct WM_COPYDATA_COMMAND_ADD cmd = {
        .magic = WM_COPYDATA_COMMAND_MAGIC,
        .pid = pid,
        .fillColor = color
    };
    wcscpy_s(cmd.text, MAX_LABEL_TEXT_LENGTH, text);

    SendDataCommand(MSG_DLL_COPYDATA_CMD_ADD, sizeof(struct WM_COPYDATA_COMMAND_ADD), &cmd);

    return TRUE;
}

EXPORT BOOL __cdecl native_get_button_pids(LPARAM lParam, DWORD* ar, int* count)
{
    COPYDATASTRUCT* pcds = (COPYDATASTRUCT*)lParam;
    if (pcds->dwData != MSG_DLL_COPYDATA_CMD_BUTTONS)
    {
        return FALSE;
    }

    DWORD* data = (DWORD*)pcds->lpData;
    if (data[0] != WM_COPYDATA_COMMAND_MAGIC)
    {
        return FALSE;
    }

    *count = (pcds->cbData / sizeof(DWORD)) - 1;

    memcpy(ar, &data[1], sizeof(DWORD) * (*count));

    return TRUE;
}

EXPORT DWORD __cdecl native_get_window_title(DWORD pid, WCHAR* title, int maxLength)
{
   struct enum_window_data data;
   data.PID = pid;
   data.resultWindow = 0;
   EnumWindows(EnumWindowsFilter, (LPARAM)&data);

   if (data.resultWindow == 0) 
   {
       return -1;
   }

   return GetWindowText(data.resultWindow, title, maxLength);
}

// DLL entry
BOOL APIENTRY DllMain(HMODULE hModule, DWORD  ul_reason_for_call, LPVOID lpReserved)
{
    return TRUE;
}