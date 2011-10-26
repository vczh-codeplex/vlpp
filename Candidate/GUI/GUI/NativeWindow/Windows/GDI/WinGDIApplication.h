/***********************************************************************
Vczh Library++ 3.0
Developer: ³Âè÷å«(vczh)
GacUI::Native Window::GDI Provider for Windows Implementation

Interfaces:
***********************************************************************/
#ifndef VCZH_PRESENTATION_WINDOWS_GDI_WINGDIAPPLICATION
#define VCZH_PRESENTATION_WINDOWS_GDI_WINGDIAPPLICATION
#ifdef GUI_GRAPHICS_RENDERER_GDI

#include "WinGDI.h"
#include "..\WinNativeWindow.h"

namespace vl
{
	namespace presentation
	{
		namespace windows
		{
			extern WinDC*									GetNativeWindowDC(INativeWindow* window);
		}
	}
}

#endif
#endif