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
		namespace elements
		{

/***********************************************************************
Renderers
***********************************************************************/

			class GuiSolidBorderElementRenderer
				: public GuiGraphicsRenderer<GuiSolidBorderElement, GuiSolidBorderElementRenderer, IWindowsGDIRenderTarget>
			{
			public:
				void				Render(Rect bounds);
				void				OnElementStateChanged();
			};

			class GuiSolidBackgroundElementRenderer
				: public GuiGraphicsRenderer<GuiSolidBackgroundElement, GuiSolidBackgroundElementRenderer, IWindowsGDIRenderTarget>
			{
			public:
				void				Render(Rect bounds);
				void				OnElementStateChanged();
			};
		}
	}
}

#endif