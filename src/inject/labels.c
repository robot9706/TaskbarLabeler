#include "labels.h"

static struct label_entry label_entries[MAX_LABEL_ENTRIES];

void LabelsInit()
{
	memset(&label_entries, 0, sizeof(struct label_entry) * MAX_LABEL_ENTRIES);
}

struct label_entry* GetLabelEntry(DWORD pid)
{
	for (int x = 0; x < MAX_LABEL_ENTRIES; x++)
	{
		if (label_entries[x].pid == pid) 
		{
			return &label_entries[x];
		}
	}

	return NULL;
}

BOOL AddLabel(DWORD pid, DWORD color, WCHAR* text)
{
	// Find an empty spot
	for (int x = 0; x < MAX_LABEL_ENTRIES; x++)
	{
		// Is empty or the label already exists
		if (label_entries[x].pid == 0 || label_entries[x].pid == pid)
		{
			label_entries[x].pid = pid;
			wcscpy_s(label_entries[x].text, MAX_LABEL_TEXT_LENGTH, text);
			label_entries[x].brush = CreateSolidBrush(color);

			return TRUE;
		}
	}

	return FALSE; // No empty spot
}

void RemoveLabel(DWORD pid)
{
	// Find the label
	for (int x = 0; x < MAX_LABEL_ENTRIES; x++)
	{
		if (label_entries[x].pid == pid)
		{
			// Delete the brush
			DeleteObject(label_entries[x].brush);

			// Set pid to zero so the label can be reused
			label_entries[x].pid = 0;
		}
	}
}