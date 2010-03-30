/***********************************************************************
Vczh Library++ 3.0
Developer: ³Âè÷å«(vczh)
GUI::Windows Platform

Interfaces:
***********************************************************************/

#ifndef VCZH_PRESENTATION_WINDOWSCORE_WINNATIVECANVAS
#define VCZH_PRESENTATION_WINDOWSCORE_WINNATIVECANVAS

#include "..\Core\GuiNativeWindow.h"
#include <windows.h>

namespace vl
{
	namespace presentation
	{
		namespace windows
		{

/***********************************************************************
Windows Platform GDI Graphics
***********************************************************************/

			class IGdiCanvas : private Interface
			{
			public:
				HDC							GetDeviceContext();
			};

			extern INativeGraphics*			CreateWindowsGdiGraphics();
			extern IGdiCanvas*				GetGdiCanvas(INativeCanvas* canvas);
			extern void						DestroyWindowsGdiGraphics(INativeGraphics* graphics);
		}
	}
}

#endif