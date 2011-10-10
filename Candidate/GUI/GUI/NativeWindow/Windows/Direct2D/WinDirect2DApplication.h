/***********************************************************************
Vczh Library++ 3.0
Developer: ³Âè÷å«(vczh)
GUI::Windows GDI Provider

Interfaces:
***********************************************************************/
#ifndef VCZH_PRESENTATION_WINDOWS_GDI_WINDIRECT2DAPPLICATION
#define VCZH_PRESENTATION_WINDOWS_GDI_WINDIRECT2DAPPLICATION
#ifdef GUI_GRAPHICS_RENDERER_DIRECT2D

#include <D2D1.h>
#include <DWrite.h>
#include "..\WinNativeWindow.h"

namespace vl
{
	namespace presentation
	{
		namespace windows
		{
			extern ID2D1RenderTarget*					GetNativeWindowDirect2DRenderTarget(INativeWindow* window);
			extern ID2D1Factory*						GetDirect2DFactory();
			extern IDWriteFactory*						GetDirectWriteFactory();
		}
	}
}

#endif
#endif