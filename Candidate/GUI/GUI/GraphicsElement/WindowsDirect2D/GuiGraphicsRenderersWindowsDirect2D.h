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

#define DEFINE_BRUSH_ELEMENT_RENDERER(TELEMENT, TRENDERER)\
				DEFINE_GUI_GRAPHICS_RENDERER(TELEMENT, TRENDERER, IWindowsDirect2DRenderTarget)\
			protected:\
				Color					oldColor;\
				ID2D1SolidColorBrush*	brush;\
				void					CreateBrush(IWindowsDirect2DRenderTarget* _renderTarget);\
				void					DestroyBrush(IWindowsDirect2DRenderTarget* _renderTarget);\
				void					InitializeInternal();\
				void					FinalizeInternal();\
				void					RenderTargetChangedInternal(IWindowsDirect2DRenderTarget* oldRenderTarget, IWindowsDirect2DRenderTarget* newRenderTarget);\
			public:\
				TRENDERER();\
				void					Render(Rect bounds);\
				void					OnElementStateChanged();\

/***********************************************************************
Renderers
***********************************************************************/


			class GuiSolidBorderElementRenderer : public Object, public IGuiGraphicsRenderer
			{
				DEFINE_BRUSH_ELEMENT_RENDERER(GuiSolidBorderElement, GuiSolidBorderElementRenderer)
			};

			class GuiRoundBorderElementRenderer : public Object, public IGuiGraphicsRenderer
			{
				DEFINE_BRUSH_ELEMENT_RENDERER(GuiRoundBorderElement, GuiRoundBorderElementRenderer)
			};

			class GuiSolidBackgroundElementRenderer : public Object, public IGuiGraphicsRenderer
			{
				DEFINE_BRUSH_ELEMENT_RENDERER(GuiSolidBackgroundElement, GuiSolidBackgroundElementRenderer)
			};

			class GuiGradientBackgroundElementRenderer : public Object, public IGuiGraphicsRenderer
			{
				DEFINE_GUI_GRAPHICS_RENDERER(GuiGradientBackgroundElement, GuiGradientBackgroundElementRenderer, IWindowsDirect2DRenderTarget)
			protected:
				void					InitializeInternal();
				void					FinalizeInternal();
				void					RenderTargetChangedInternal(IWindowsDirect2DRenderTarget* oldRenderTarget, IWindowsDirect2DRenderTarget* newRenderTarget);
			public:
				void					Render(Rect bounds);
				void					OnElementStateChanged();
			};

			class GuiSolidLabelElementRenderer : public Object, public IGuiGraphicsRenderer
			{
				DEFINE_GUI_GRAPHICS_RENDERER(GuiSolidLabelElement, GuiSolidLabelElementRenderer, IWindowsDirect2DRenderTarget)
			protected:
				Color					oldColor;
				FontProperties			oldFont;
				WString					oldText;
				ID2D1SolidColorBrush*	brush;
				IDWriteTextFormat*		textFormat;

				void					CreateBrush(IWindowsDirect2DRenderTarget* _renderTarget);
				void					DestroyBrush(IWindowsDirect2DRenderTarget* _renderTarget);
				void					CreateTextFormat(IWindowsDirect2DRenderTarget* _renderTarget);
				void					DestroyTextFormat(IWindowsDirect2DRenderTarget* _renderTarget);
				void					UpdateMinSize();

				void					InitializeInternal();
				void					FinalizeInternal();
				void					RenderTargetChangedInternal(IWindowsDirect2DRenderTarget* oldRenderTarget, IWindowsDirect2DRenderTarget* newRenderTarget);
			public:
				GuiSolidLabelElementRenderer();

				void					Render(Rect bounds);
				void					OnElementStateChanged();
			};
		}
	}
}

#endif
#endif