/***********************************************************************
Vczh Library++ 3.0
Developer: ³Âè÷å«(vczh)
GUI::Windows Platform

Interfaces:
***********************************************************************/

#ifndef VCZH_PRESENTATION_ELEMENTS_GUIGRAPHICSWINDOWSDIRECT2D
#define VCZH_PRESENTATION_ELEMENTS_GUIGRAPHICSWINDOWSDIRECT2D
#ifdef GUI_GRAPHICS_RENDERER_DIRECT2D

#include "..\..\NativeWindow\Windows\Direct2D\WinDirect2DApplication.h"
#include "..\GuiGraphicsElement.h"
#include "..\GuiGraphicsComposition.h"

namespace vl
{
	namespace presentation
	{
		namespace elements
		{
			class IWindowsDirect2DRenderTarget : public IGuiGraphicsRenderTarget
			{
			public:
				virtual ID2D1RenderTarget*					GetDirect2DRenderTarget()=0;
			};

			class IWindowsDirect2DResourceManager : public Interface
			{
			public:
			};

			extern IWindowsDirect2DResourceManager*			GetWindowsDirect2DResourceManager();
		}
	}
}

#endif
#endif