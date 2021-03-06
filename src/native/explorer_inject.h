#ifndef _EXPLORER_INJECT_H_
#define _EXPLORER_INJECT_H_

#include <windows.h>
#include <stddef.h>

#ifndef _WIN64
#define ASM_CODE_LOAD \
	"\xEF\xBE\xAD\xDE\xEF\xBE\xAD\xDE\xEF\xBE\xAD\xDE\x57\x56\x55\x53" \
	"\xE8\x00\x00\x00\x00\x5F\x8D\x5F\xEB\x83\xC7\x63\x57\xFF\x13\x85" \
	"\xC0\x89\x03\x74\x35\x89\xC6\x8B\x6F\x90\x83\xC7\xFB\x57\x50\xFF" \
	"\x57\x91\x85\xC0\x74\x2D\x87\x47\x91\x83\xC7\xFB\x57\x56\xFF\xD0" \
	"\x85\xC0\x74\x1F\x83\xC7\x0A\x89\xF9\x83\xC1\x02\x66\x83\x79\xFE" \
	"\x00\x75\xF6\x51\xFF\xD0\x85\xC0\x74\x09\x5B\x5D\x5E\x5F\x31\xC0" \
	"\xC2\x04\x00\xC7\x03\x00\x00\x00\x00\x56\xFF\xD5\xEB\xEC\x49\x6E" \
	"\x69\x74\x00\x45\x78\x69\x74\x00"
#define ASM_CODE_WAITANDFREE \
	"\xEF\xBE\xAD\xDE\xEF\xBE\xAD\xDE\xEF\xBE\xAD\xDE\xEF\xBE\xAD\xDE" \
	"\xEF\xBE\xAD\xDE\xEF\xBE\xAD\xDE\xEF\xBE\xAD\xDE\xEF\xBE\xAD\xDE" \
	"\x57\xE8\x00\x00\x00\x00\x5F\x6A\xFF\x6A\x00\x83\xC7\xDA\x57\x6A" \
	"\x02\xFF\x57\x08\xFF\x37\xFF\x57\x0C\xFF\x77\x04\xFF\x57\x0C\xFF" \
	"\x57\x10\xFF\x77\x14\xFF\x57\x18\x89\xF8\x5F\x59\x5A\x68\x00\x80" \
	"\x00\x00\x6A\x00\x50\x51\xFF\x60\x1C"
#else
#define ASM_CODE_LOAD \
	"\xEF\xBE\xAD\xEE\xEE\xEE\xEE\xDE\xEF\xBE\xAD\xEE\xEE\xEE\xEE\xDE" \
	"\xEF\xBE\xAD\xEE\xEE\xEE\xEE\xDE\x53\x48\x83\xEC\x20\x48\x8D\x0D" \
	"\x83\x00\x00\x00\xFF\x15\xD6\xFF\xFF\xFF\x48\x85\xC0\x48\x89\x05" \
	"\xCC\xFF\xFF\xFF\x74\x49\x48\x89\xC3\x48\x89\xC1\x48\x8D\x15\x5F" \
	"\x00\x00\x00\xFF\x15\xBF\xFF\xFF\xFF\x48\x85\xC0\x74\x39\x48\x87" \
	"\x05\xB3\xFF\xFF\xFF\x48\x89\xD9\x48\x8D\x15\x3E\x00\x00\x00\xFF" \
	"\xD0\x48\x85\xC0\x74\x21\x48\x8D\x0D\x3A\x00\x00\x00\x48\x83\xC1" \
	"\x02\x66\x83\x79\xFE\x00\x75\xF5\xFF\xD0\x48\x85\xC0\x74\x08\x48" \
	"\x83\xC4\x20\x5B\x31\xC0\xC3\x48\xC7\x05\x6E\xFF\xFF\xFF\x00\x00" \
	"\x00\x00\x48\x89\xD9\xFF\x15\x75\xFF\xFF\xFF\xEB\xE2\x49\x6E\x69" \
	"\x74\x00\x45\x78\x69\x74\x00"
#define ASM_CODE_WAITANDFREE \
	"\xEF\xBE\xAD\xEE\xEE\xEE\xEE\xDE\xEF\xBE\xAD\xEE\xEE\xEE\xEE\xDE" \
	"\xEF\xBE\xAD\xEE\xEE\xEE\xEE\xDE\xEF\xBE\xAD\xEE\xEE\xEE\xEE\xDE" \
	"\xEF\xBE\xAD\xEE\xEE\xEE\xEE\xDE\xEF\xBE\xAD\xEE\xEE\xEE\xEE\xDE" \
	"\xEF\xBE\xAD\xEE\xEE\xEE\xEE\xDE\xEF\xBE\xAD\xEE\xEE\xEE\xEE\xDE" \
	"\x48\x83\xEC\x28\x41\x83\xC9\xFF\x45\x31\xC0\x48\x8D\x15\xAE\xFF" \
	"\xFF\xFF\xB9\x02\x00\x00\x00\xFF\x15\xB3\xFF\xFF\xFF\x48\x8B\x0D" \
	"\x9C\xFF\xFF\xFF\xFF\x15\xAE\xFF\xFF\xFF\x48\x8B\x0D\x97\xFF\xFF" \
	"\xFF\xFF\x15\xA1\xFF\xFF\xFF\xFF\x15\xA3\xFF\xFF\xFF\x48\x8B\x0D" \
	"\xA4\xFF\xFF\xFF\xFF\x15\xA6\xFF\xFF\xFF\x48\x83\xC4\x28\x41\xB8" \
	"\x00\x80\x00\x00\x31\xD2\x48\x8D\x0D\x63\xFF\xFF\xFF\xFF\x25\x95" \
	"\xFF\xFF\xFF"
#endif

INT ExplorerInject(HANDLE ownerProcess);
static HMODULE LoadLibraryInExplorer(HANDLE hProcess, WCHAR *pDllName, HANDLE hEvent, void **ppExitProc, HANDLE ownerProcess);
static HANDLE InjectCleanCodeInExplorer(HANDLE hProcess, HMODULE hDllModule, HANDLE hEvent, HANDLE hCurrentProcess, void *pExitProc);
static DWORD WINAPI WaitThread(LPVOID lpParameter);
static BOOL MyReadProcessMemory(HANDLE hProcess, LPCVOID lpBaseAddress, LPVOID lpBuffer, SIZE_T nSize);
static BOOL MyWriteProcessMemory(HANDLE hProcess, LPVOID lpBaseAddress, LPVOID lpBuffer, SIZE_T nSize);
BOOL ExplorerIsInjected();
HWND ExplorerGetTaskbarWnd();
void ExplorerCleanup();

#endif // _EXPLORER_INJECT_H_
