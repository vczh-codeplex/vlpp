/***********************************************************************
Vczh Library++ 3.0
Developer: �����(vczh)
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

#define DEFINE_BRUSH_ELEMENT_RENDERER(TELEMENT, TRENDERER, TBRUSH, TBRUSHPROPERTY)\
				DEFINE_GUI_GRAPHICS_RENDERER(TELEMENT, TRENDERER, IWindowsDirect2DRenderTarget)\
			protected:\
				TBRUSHPROPERTY			oldColor;\
				TBRUSH*					brush;\
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
				DEFINE_BRUSH_ELEMENT_RENDERER(GuiSolidBorderElement, GuiSolidBorderElementRenderer, ID2D1SolidColorBrush, Color)
			};

			class GuiRoundBorderElementRenderer : public Object, public IGuiGraphicsRenderer
			{
				DEFINE_BRUSH_ELEMENT_RENDERER(GuiRoundBorderElement, GuiRoundBorderElementRenderer, ID2D1SolidColorBrush, Color)
			};

			class Gui3DBorderElementRenderer : public Object, public IGuiGraphicsRenderer
			{
				DEFINE_GUI_GRAPHICS_RENDERER(Gui3DBorderElement, Gui3DBorderElementRenderer, IWindowsDirect2DRenderTarget)
			protected:
				Color					oldColor1;
				Color					oldColor2;
				ID2D1SolidColorBrush*	brush1;
				ID2D1SolidColorBrush*	brush2;

				void					CreateBrush(IWindowsDirect2DRenderTarget* _renderTarget);
				void					DestroyBrush(IWindowsDirect2DRenderTarget* _renderTarget);
				void					InitializeInternal();
				void					FinalizeInternal();
				void					RenderTargetChangedInternal(IWindowsDirect2DRenderTarget* oldRenderTarget, IWindowsDirect2DRenderTarget* newRenderTarget);
			public:
				Gui3DBorderElementRenderer();

				void					Render(Rect bounds);
				void					OnElementStateChanged();
			};

			class GuiSolidBackgroundElementRenderer : public Object, public IGuiGraphicsRenderer
			{
				DEFINE_BRUSH_ELEMENT_RENDERER(GuiSolidBackgroundElement, GuiSolidBackgroundElementRenderer, ID2D1SolidColorBrush, Color)
			};

			class GuiGradientBackgroundElementRenderer : public Object, public IGuiGraphicsRenderer
			{
				typedef collections::Pair<Color, Color> ColorPair;
				DEFINE_BRUSH_ELEMENT_RENDERER(GuiGradientBackgroundElement, GuiGradientBackgroundElementRenderer, ID2D1LinearGradientBrush, ColorPair)
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