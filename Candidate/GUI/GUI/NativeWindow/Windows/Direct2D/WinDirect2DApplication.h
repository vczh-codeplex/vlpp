/***********************************************************************
Vczh Library++ 3.0
Developer: �����(vczh)
GacUI::Native Window::Direct2D Provider for Windows Implementation

Interfaces:
***********************************************************************/
#ifndef VCZH_PRESENTATION_WINDOWS_GDI_WINDIRECT2DAPPLICATION
#define VCZH_PRESENTATION_WINDOWS_GDI_WINDIRECT2DAPPLICATION

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

extern int WinMainDirect2D(HINSTANCE hInstance, void(*RendererMain)());

#endif