/***********************************************************************
Vczh Library++ 3.0
Developer: ³Âè÷å«(vczh)
GUI::Windows Platform

Interfaces:
***********************************************************************/

#ifndef VCZH_PRESENTATION_WINDOWSCORE_WINGDIAPPLICATION
#define VCZH_PRESENTATION_WINDOWSCORE_WINGDIAPPLICATION

#include "..\Core\GuiNativeWindow.h"

namespace vl
{
	namespace presentation
	{
		namespace windows
		{

/***********************************************************************
Windows GDI Application
***********************************************************************/

			extern INativeApplication*			CreateGdiApplication();
			extern void							DestroyGdiApplication(INativeApplication* application);
		}
	}
}

#endif