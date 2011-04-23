/***********************************************************************
Vczh Library++ 3.0
Developer: ³Âè÷å«(vczh)
GUI::Windows GDI Provider

Interfaces:
***********************************************************************/
#ifndef VCZH_PRESENTATION_WINDOWS_GDI_WINGDIAPPLICATION
#define VCZH_PRESENTATION_WINDOWS_GDI_WINGDIAPPLICATION

#include "WinGDIElement.h"

namespace vl
{
	namespace presentation
	{
		namespace windows
		{
			extern WinDC*									GetNativeWindowDC(INativeWindow* window);
			extern elements::WinGDIElementEnvironment*		GetNativeWindowGDIElementEnvironment(INativeWindow* window);
		}
	}
}

#endif