#pragma once

#ifndef __COMMANDS_H__
#define __COMMANDS_H__

#include <Windows.h>

#define MAX_LABEL_TEXT_LENGTH 64 // Must match with labels.h in inject dll

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

#endif // __COMMANDS_H__