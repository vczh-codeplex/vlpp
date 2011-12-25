/***********************************************************************
Vczh Library++ 3.0
Developer: �����(vczh)
GacUI::Native Window::Direct2D Provider for Windows Implementation::Renderer

Interfaces:
***********************************************************************/

#ifndef VCZH_PRESENTATION_ELEMENTS_GUIGRAPHICSRENDERERSWINDOWSDIRECT2D
#define VCZH_PRESENTATION_ELEMENTS_GUIGRAPHICSRENDERERSWINDOWSDIRECT2D

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
				void					Render(Rect bounds)override;\
				void					OnElementStateChanged()override;\

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

				void					Render(Rect bounds)override;
				void					OnElementStateChanged()override;
			};

			class Gui3DSplitterElementRenderer : public Object, public IGuiGraphicsRenderer
			{
				DEFINE_GUI_GRAPHICS_RENDERER(Gui3DSplitterElement, Gui3DSplitterElementRenderer, IWindowsDirect2DRenderTarget)
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
				Gui3DSplitterElementRenderer();

				void					Render(Rect bounds)override;
				void					OnElementStateChanged()override;
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
				Color							oldColor;
				FontProperties					oldFont;
				WString							oldText;
				ID2D1SolidColorBrush*			brush;
				Direct2DTextFormatPackage*		textFormat;

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

				void					Render(Rect bounds)override;
				void					OnElementStateChanged()override;
			};

			class GuiImageFrameElementRenderer : public Object, public IGuiGraphicsRenderer
			{
				DEFINE_GUI_GRAPHICS_RENDERER(GuiImageFrameElement, GuiImageFrameElementRenderer, IWindowsDirect2DRenderTarget)
			protected:
				ComPtr<ID2D1Bitmap>		bitmap;

				void					UpdateBitmap(IWindowsDirect2DRenderTarget* renderTarget);

				void					InitializeInternal();
				void					FinalizeInternal();
				void					RenderTargetChangedInternal(IWindowsDirect2DRenderTarget* oldRenderTarget, IWindowsDirect2DRenderTarget* newRenderTarget);
			public:
				GuiImageFrameElementRenderer();

				void					Render(Rect bounds)override;
				void					OnElementStateChanged()override;
			};

			class GuiPolygonElementRenderer : public Object, public IGuiGraphicsRenderer
			{
				DEFINE_GUI_GRAPHICS_RENDERER(GuiPolygonElement, GuiPolygonElementRenderer, IWindowsDirect2DRenderTarget)
			protected:
				Color							oldBorderColor;
				Color							oldBackgroundColor;
				ID2D1SolidColorBrush*			borderBrush;
				ID2D1SolidColorBrush*			backgroundBrush;
				collections::Array<Point>		oldPoints;
				ComPtr<ID2D1PathGeometry>		geometry;

				void							CreateGeometry();
				void							DestroyGeometry();
				void							RecreateResource(IWindowsDirect2DRenderTarget* oldRenderTarget, IWindowsDirect2DRenderTarget* newRenderTarget);
				void							InitializeInternal();
				void							FinalizeInternal();
				void							RenderTargetChangedInternal(IWindowsDirect2DRenderTarget* oldRenderTarget, IWindowsDirect2DRenderTarget* newRenderTarget);
			public:
				GuiPolygonElementRenderer();

				void							Render(Rect bounds)override;
				void							OnElementStateChanged()override;
			};

			class GuiColorizedTextElementRenderer : public Object, public IGuiGraphicsRenderer, protected GuiColorizedTextElement::ICallback
			{
				DEFINE_GUI_GRAPHICS_RENDERER(GuiColorizedTextElement, GuiColorizedTextElementRenderer, IWindowsDirect2DRenderTarget)

			public:
				struct ColorItemResource
				{
					Color						text;
					ID2D1SolidColorBrush*		textBrush;
					Color						background;
					ID2D1SolidColorBrush*		backgroundBrush;
				};

				struct ColorEntryResource
				{
					ColorItemResource			normal;
					ColorItemResource			selectedFocused;
					ColorItemResource			selectedUnfocused;

					bool						operator==(const ColorEntryResource& value){return false;}
					bool						operator!=(const ColorEntryResource& value){return true;}
				};

				typedef collections::Array<ColorEntryResource>			ColorArray;
			protected:
				FontProperties					oldFont;
				Direct2DTextFormatPackage*		textFormat;
				ColorArray						colors;
				Color							oldCaretColor;
				ID2D1SolidColorBrush*			caretBrush;
				
				void					CreateTextBrush(IWindowsDirect2DRenderTarget* _renderTarget);
				void					DestroyTextBrush(IWindowsDirect2DRenderTarget* _renderTarget);
				void					CreateCaretBrush(IWindowsDirect2DRenderTarget* _renderTarget);
				void					DestroyCaretBrush(IWindowsDirect2DRenderTarget* _renderTarget);

				void					ColorChanged();
				void					FontChanged();
				text::CharMeasurer*		GetCharMeasurer();

				void					InitializeInternal();
				void					FinalizeInternal();
				void					RenderTargetChangedInternal(IWindowsDirect2DRenderTarget* oldRenderTarget, IWindowsDirect2DRenderTarget* newRenderTarget);
			public:
				void					Render(Rect bounds)override;
				void					OnElementStateChanged()override;
			};
		}
	}
}

#endif