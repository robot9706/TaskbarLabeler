#pragma once

#ifndef __LABELS_H__
#define __LABELS_H__

#include <Windows.h>

#define MAX_LABEL_ENTRIES 64
#define MAX_LABEL_TEXT_LENGTH 64

struct label_entry {
	DWORD pid;
	HBRUSH brush;
	WCHAR text[MAX_LABEL_TEXT_LENGTH];
};

void LabelsInit();
struct label_entry* GetLabelEntry(DWORD pid);

BOOL AddLabel(DWORD pid, DWORD color, WCHAR* text);
void RemoveLabel(DWORD pid);

#endif // __LABELS_H__