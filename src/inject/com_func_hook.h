#ifndef _COM_FUNC_HOOK_H_
#define _COM_FUNC_HOOK_H_

#include <windows.h>

BOOL ComFuncHook_Init();
void ComFuncHook_Exit();
void ComFuncHook_WaitTillDone();
void ComFuncButtonCreatedOrPinnedItemChange();
DWORD GetButtonGroupPID(LONG_PTR* button_group);

#endif // _COM_FUNC_HOOK_H_
