#define _CRTDBG_MAP_ALLOC
#include <stdlib.h>
#include <crtdbg.h>
#include <windows.h>
#include "..\..\Library\String.h"
#include "..\..\Library\GlobalStorage.h"

using namespace vl;

WString GetPath()
{
	wchar_t buffer[65536];
	GetModuleFileName(NULL, buffer, sizeof(buffer)/sizeof(*buffer));
	int pos=-1;
	int index=0;
	while(buffer[index])
	{
		if(buffer[index]==L'\\')
		{
			pos=index;
		}
		index++;
	}
	return WString(buffer, pos+1);
}

int wmain(int argc , wchar_t* args[])
{
	FinalizeGlobalStorage();
	_CrtDumpMemoryLeaks();
	return 0;
}