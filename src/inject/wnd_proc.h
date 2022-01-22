#ifndef _WND_PROC_H_
#define _WND_PROC_H_

#include <windows.h>
#include "labels.h"

BOOL WndProcInit(void);
void WndProcExit(void);
void WndProcWaitTillDone(void);

#define WM_COPYDATA_COMMAND_MAGIC (0xC0FEBABE)

struct WM_COPYDATA_COMMAND_REMOVE {
	DWORD magic;
	DWORD pid;
};

struct WM_COPYDATA_COMMAND_ADD {
	DWORD magic;
	DWORD pid;
	WCHAR text[MAX_LABEL_TEXT_LENGTH];
	DWORD fillColor;
};

#endif // _WND_PROC_H_
