/***********************************************************************
Vczh Library++ 3.0
Developer: ³Âè÷å«(vczh)
GUI::Windows Platform

Interfaces:
***********************************************************************/

#ifndef VCZH_PRESENTATION_WINDOWS_GUIGRAPHICSRENDERERSWINDOWSGDI
#define VCZH_PRESENTATION_WINDOWS_GUIGRAPHICSRENDERERSWINDOWSGDI

#include "GuiGraphicsWindowsGDI.h"

namespace vl
{
	namespace presentation
	{
		namespace elements_windows_gdi
		{
			using namespace elements;

/***********************************************************************
Renderers
***********************************************************************/

			class GuiSolidBorderElementRenderer : public Object, public IGuiGraphicsRenderer
			{
				DEFINE_GUI_GRAPHICS_ELEMENT(GuiSolidBorderElement, GuiSolidBorderElementRenderer, IWindowsGDIRenderTarget)
			protected:
				Color					oldColor;
				Ptr<windows::WinPen>	pen;
				Ptr<windows::WinBrush>	brush;

				void					InitializeInternal();
			public:
				void					Render(Rect bounds);
				void					OnElementStateChanged();
			};

			class GuiSolidBackgroundElementRenderer : public Object, public IGuiGraphicsRenderer
			{
				DEFINE_GUI_GRAPHICS_ELEMENT(GuiSolidBackgroundElement, GuiSolidBackgroundElementRenderer, IWindowsGDIRenderTarget)
			protected:
				Color					oldColor;
				Ptr<windows::WinBrush>	brush;

				void					InitializeInternal();
			public:
				void					Render(Rect bounds);
				void					OnElementStateChanged();
			};
		}
	}
}

#endif