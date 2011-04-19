/***********************************************************************
Vczh Library++ 3.0
Developer: ³Âè÷å«(vczh)
GUI::Windows Platform

Interfaces:
***********************************************************************/
#ifndef VCZH_PRESENTATION_WINDOWS_GDI_WINGDIAPPLICATION
#define VCZH_PRESENTATION_WINDOWS_GDI_WINGDIAPPLICATION

#include "..\WinNativeWindow.h"
#include "WinGDI.h"

namespace vl
{
	namespace presentation
	{
		namespace windows
		{
			extern vl::windows::WinDC*				GetNativeWindowDC(INativeWindow* window);
		}
	}
}

#endif