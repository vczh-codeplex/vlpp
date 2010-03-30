/***********************************************************************
Vczh Library++ 3.0
Developer: �����(vczh)
GUI::Windows Platform

Interfaces:
***********************************************************************/

#ifndef VCZH_PRESENTATION_WINDOWSCORE_WINNATIVECANVAS
#define VCZH_PRESENTATION_WINDOWSCORE_WINNATIVECANVAS

#include "..\Core\GuiNativeWindow.h"

namespace vl
{
	namespace presentation
	{
		namespace windows
		{
			extern INativeGraphics*			CreateWindowsGdiGraphics();
			extern void						DestroyWindowsGdiGraphics(INativeGraphics* graphics);
		}
	}
}

#endif