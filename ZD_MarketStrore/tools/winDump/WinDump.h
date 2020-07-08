#pragma

#pragma comment(lib, "dbghelp.lib")
#include <Windows.h>
#define    WIN32_LEAN_AND_MEAN
#include <Windows.h>
#include <tchar.h>
#include <DbgHelp.h>

LONG __stdcall ExceptionFilter(LPEXCEPTION_POINTERS lpExceptionInfo);
int GenerateMiniDump(PEXCEPTION_POINTERS pExceptionPointers);

class WinDumpTool 
{
public:
	WinDumpTool();
	static void setExceptionFilter();
		
};