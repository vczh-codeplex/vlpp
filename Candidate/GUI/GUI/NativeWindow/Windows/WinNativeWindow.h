/***********************************************************************
Vczh Library++ 3.0
Developer: ³Âè÷å«(vczh)
GacUI::Native Window::Windows Implementation

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

			class IWindowsFormGraphicsHandler : public Interface
			{
			public:
				virtual void								RedrawContent()=0;
			};

			class IWindowsForm : public Interface
			{
			public:
				virtual HWND								GetWindowHandle()=0;
				virtual IWindowsFormGraphicsHandler*		GetGraphicsHandler()=0;
				virtual void								SetGraphicsHandler(IWindowsFormGraphicsHandler* handler)=0;
			};

			extern INativeController*						CreateWindowsNativeController(HINSTANCE hInstance);
			extern IWindowsForm*							GetWindowsForm(INativeWindow* window);
			extern void										DestroyWindowsNativeController(INativeController* controller);
		}
	}
}

#endif