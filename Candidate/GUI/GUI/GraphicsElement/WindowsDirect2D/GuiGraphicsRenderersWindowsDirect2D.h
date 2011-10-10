/***********************************************************************
Vczh Library++ 3.0
Developer: ³Âè÷å«(vczh)
GUI::Windows Platform

Interfaces:
***********************************************************************/

#ifndef VCZH_PRESENTATION_ELEMENTS_GUIGRAPHICSRENDERERSWINDOWSDIRECT2D
#define VCZH_PRESENTATION_ELEMENTS_GUIGRAPHICSRENDERERSWINDOWSDIRECT2D
#ifdef GUI_GRAPHICS_RENDERER_DIRECT2D

#include "GuiGraphicsWindowsDirect2D.h"

namespace vl
{
	namespace presentation
	{
		namespace elements_windows_d2d
		{
			using namespace elements;

/***********************************************************************
Renderers
***********************************************************************/

			class GuiSolidBorderElementRenderer : public Object, public IGuiGraphicsRenderer
			{
				DEFINE_GUI_GRAPHICS_RENDERER(GuiSolidBorderElement, GuiSolidBorderElementRenderer, IWindowsDirect2DRenderTarget)
			protected:

				void					InitializeInternal();
				void					FinalizeInternal();
				void					RenderTargetChangedInternal();
			public:
				void					Render(Rect bounds);
				void					OnElementStateChanged();
			};

			class GuiRoundBorderElementRenderer : public Object, public IGuiGraphicsRenderer
			{
				DEFINE_GUI_GRAPHICS_RENDERER(GuiRoundBorderElement, GuiRoundBorderElementRenderer, IWindowsDirect2DRenderTarget)
			protected:

				void					InitializeInternal();
				void					FinalizeInternal();
				void					RenderTargetChangedInternal();
			public:
				void					Render(Rect bounds);
				void					OnElementStateChanged();
			};

			class GuiSolidBackgroundElementRenderer : public Object, public IGuiGraphicsRenderer
			{
				DEFINE_GUI_GRAPHICS_RENDERER(GuiSolidBackgroundElement, GuiSolidBackgroundElementRenderer, IWindowsDirect2DRenderTarget)
			protected:

				void					InitializeInternal();
				void					FinalizeInternal();
				void					RenderTargetChangedInternal();
			public:
				void					Render(Rect bounds);
				void					OnElementStateChanged();
			};

			class GuiGradientBackgroundElementRenderer : public Object, public IGuiGraphicsRenderer
			{
				DEFINE_GUI_GRAPHICS_RENDERER(GuiGradientBackgroundElement, GuiGradientBackgroundElementRenderer, IWindowsDirect2DRenderTarget)
			protected:
				void					InitializeInternal();
				void					FinalizeInternal();
				void					RenderTargetChangedInternal();
			public:
				void					Render(Rect bounds);
				void					OnElementStateChanged();
			};

			class GuiSolidLabelElementRenderer : public Object, public IGuiGraphicsRenderer
			{
				DEFINE_GUI_GRAPHICS_RENDERER(GuiSolidLabelElement, GuiSolidLabelElementRenderer, IWindowsDirect2DRenderTarget)
			protected:

				void					UpdateMinSize();

				void					InitializeInternal();
				void					FinalizeInternal();
				void					RenderTargetChangedInternal();
			public:
				void					Render(Rect bounds);
				void					OnElementStateChanged();
			};
		}
	}
}

#endif
#endif