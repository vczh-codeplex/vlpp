/***********************************************************************
Vczh Library++ 3.0
Developer: ³Âè÷å«(vczh)
GUI::Windows Platform

Interfaces:
***********************************************************************/

#ifndef VCZH_PRESENTATION_WINDOWSCORE_WINNATIVEWINDOW
#define VCZH_PRESENTATION_WINDOWSCORE_WINNATIVEWINDOW

#include "..\Core\GuiNativeWindow.h"

namespace vl
{
	namespace presentation
	{
		namespace windows
		{

/***********************************************************************
Windows Platform Native Controller
***********************************************************************/

			extern INativeController*		CreateWindowsNativeController(HINSTANCE hInstance);
			extern void						DestroyWindowsNativeController(INativeController* controller);
		}
	}
}

#endif