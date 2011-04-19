#if _DEBUG
#define _CRTDBG_MAP_ALLOC
#include <stdlib.h>
#include <crtdbg.h>
#endif

#include "..\WinNativeWindow.h"
#include "WinGDI.h"

namespace vl
{
	namespace presentation
	{
		namespace windows
		{
		}
	}
}

using namespace vl;
using namespace vl::presentation;
using namespace vl::presentation::windows;

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow)
{
	INativeController* controller=CreateWindowsNativeController(hInstance);
	SetCurrentController(controller);
	NativeMain();
	DestroyWindowsNativeController(controller);
#if _DEBUG
	_CrtDumpMemoryLeaks();
#endif
	return 0;
}