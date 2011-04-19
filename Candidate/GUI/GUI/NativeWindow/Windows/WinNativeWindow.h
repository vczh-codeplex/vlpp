/***********************************************************************
Vczh Library++ 3.0
Developer: ³Âè÷å«(vczh)
GUI::Windows Platform

Interfaces:
***********************************************************************/

#ifndef VCZH_PRESENTATION_WINDOWS_WINNATIVEWINDOW
#define VCZH_PRESENTATION_WINDOWS_WINNATIVEWINDOW

#include "..\GuiNativeWindow.h"
#include <windows.h>

namespace vl
{
	namespace presentation
	{
		namespace windows
		{

/***********************************************************************
Windows Platform Native Controller
***********************************************************************/

			class IWindowsForm : private Interface
			{
			public:
				virtual HWND				GetWindowHandle()=0;
			};

			extern INativeController*		CreateWindowsNativeController(HINSTANCE hInstance);
			extern IWindowsForm*			GetWindowsForm(INativeWindow* window);
			extern void						DestroyWindowsNativeController(INativeController* controller);
		}
	}
}

#endif