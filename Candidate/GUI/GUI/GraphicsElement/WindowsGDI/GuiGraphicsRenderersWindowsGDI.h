/***********************************************************************
Vczh Library++ 3.0
Developer: ³Âè÷å«(vczh)
GacUI::Native Window::GDI Provider for Windows Implementation::Renderer

Interfaces:
***********************************************************************/

#ifndef VCZH_PRESENTATION_ELEMENTS_GUIGRAPHICSRENDERERSWINDOWSGDI
#define VCZH_PRESENTATION_ELEMENTS_GUIGRAPHICSRENDERERSWINDOWSGDI
#ifdef GUI_GRAPHICS_RENDERER_GDI

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
				DEFINE_GUI_GRAPHICS_RENDERER(GuiSolidBorderElement, GuiSolidBorderElementRenderer, IWindowsGDIRenderTarget)
			protected:
				Color					oldColor;
				Ptr<windows::WinPen>	pen;
				Ptr<windows::WinBrush>	brush;

				void					InitializeInternal();
				void					FinalizeInternal();
				void					RenderTargetChangedInternal(IWindowsGDIRenderTarget* oldRenderTarget, IWindowsGDIRenderTarget* newRenderTarget);
			public:
				void					Render(Rect bounds)override;
				void					OnElementStateChanged()override;
			};

			class GuiRoundBorderElementRenderer : public Object, public IGuiGraphicsRenderer
			{
				DEFINE_GUI_GRAPHICS_RENDERER(GuiRoundBorderElement, GuiRoundBorderElementRenderer, IWindowsGDIRenderTarget)
			protected:
				Color					oldColor;
				Ptr<windows::WinPen>	pen;
				Ptr<windows::WinBrush>	brush;

				void					InitializeInternal();
				void					FinalizeInternal();
				void					RenderTargetChangedInternal(IWindowsGDIRenderTarget* oldRenderTarget, IWindowsGDIRenderTarget* newRenderTarget);
			public:
				void					Render(Rect bounds)override;
				void					OnElementStateChanged()override;
			};

			class Gui3DBorderElementRenderer : public Object, public IGuiGraphicsRenderer
			{
				DEFINE_GUI_GRAPHICS_RENDERER(Gui3DBorderElement, Gui3DBorderElementRenderer, IWindowsGDIRenderTarget)
			protected:
				Color					oldColor1;
				Color					oldColor2;
				Ptr<windows::WinPen>	pen1;
				Ptr<windows::WinPen>	pen2;

				void					InitializeInternal();
				void					FinalizeInternal();
				void					RenderTargetChangedInternal(IWindowsGDIRenderTarget* oldRenderTarget, IWindowsGDIRenderTarget* newRenderTarget);
			public:
				void					Render(Rect bounds)override;
				void					OnElementStateChanged()override;
			};

			class Gui3DSplitterElementRenderer : public Object, public IGuiGraphicsRenderer
			{
				DEFINE_GUI_GRAPHICS_RENDERER(Gui3DSplitterElement, Gui3DSplitterElementRenderer, IWindowsGDIRenderTarget)
			protected:
				Color					oldColor1;
				Color					oldColor2;
				Ptr<windows::WinPen>	pen1;
				Ptr<windows::WinPen>	pen2;

				void					InitializeInternal();
				void					FinalizeInternal();
				void					RenderTargetChangedInternal(IWindowsGDIRenderTarget* oldRenderTarget, IWindowsGDIRenderTarget* newRenderTarget);
			public:
				void					Render(Rect bounds)override;
				void					OnElementStateChanged()override;
			};

			class GuiSolidBackgroundElementRenderer : public Object, public IGuiGraphicsRenderer
			{
				DEFINE_GUI_GRAPHICS_RENDERER(GuiSolidBackgroundElement, GuiSolidBackgroundElementRenderer, IWindowsGDIRenderTarget)
			protected:
				Color					oldColor;
				Ptr<windows::WinPen>	pen;
				Ptr<windows::WinBrush>	brush;

				void					InitializeInternal();
				void					FinalizeInternal();
				void					RenderTargetChangedInternal(IWindowsGDIRenderTarget* oldRenderTarget, IWindowsGDIRenderTarget* newRenderTarget);
			public:
				void					Render(Rect bounds)override;
				void					OnElementStateChanged()override;
			};

			class GuiGradientBackgroundElementRenderer : public Object, public IGuiGraphicsRenderer
			{
				DEFINE_GUI_GRAPHICS_RENDERER(GuiGradientBackgroundElement, GuiGradientBackgroundElementRenderer, IWindowsGDIRenderTarget)
			protected:
				void					InitializeInternal();
				void					FinalizeInternal();
				void					RenderTargetChangedInternal(IWindowsGDIRenderTarget* oldRenderTarget, IWindowsGDIRenderTarget* newRenderTarget);
			public:
				void					Render(Rect bounds)override;
				void					OnElementStateChanged()override;
			};

			class GuiSolidLabelElementRenderer : public Object, public IGuiGraphicsRenderer
			{
				DEFINE_GUI_GRAPHICS_RENDERER(GuiSolidLabelElement, GuiSolidLabelElementRenderer, IWindowsGDIRenderTarget)
			protected:
				FontProperties			oldFont;
				Ptr<windows::WinFont>	font;

				void					UpdateMinSize();

				void					InitializeInternal();
				void					FinalizeInternal();
				void					RenderTargetChangedInternal(IWindowsGDIRenderTarget* oldRenderTarget, IWindowsGDIRenderTarget* newRenderTarget);
			public:
				void					Render(Rect bounds)override;
				void					OnElementStateChanged()override;
			};

			class GuiImageFrameElementRenderer : public Object, public IGuiGraphicsRenderer
			{
				DEFINE_GUI_GRAPHICS_RENDERER(GuiImageFrameElement, GuiImageFrameElementRenderer, IWindowsGDIRenderTarget)
			protected:
				Ptr<windows::WinBitmap>		bitmap;

				void					UpdateBitmap();

				void					InitializeInternal();
				void					FinalizeInternal();
				void					RenderTargetChangedInternal(IWindowsGDIRenderTarget* oldRenderTarget, IWindowsGDIRenderTarget* newRenderTarget);
			public:
				GuiImageFrameElementRenderer();

				void					Render(Rect bounds)override;
				void					OnElementStateChanged()override;
			};

			class GuiColorizedTextElementRenderer : public Object, public IGuiGraphicsRenderer, protected GuiColorizedTextElement::ICallback
			{
				DEFINE_GUI_GRAPHICS_RENDERER(GuiColorizedTextElement, GuiColorizedTextElementRenderer, IWindowsGDIRenderTarget)

			public:
				struct ColorItemResource
				{
					Color						text;
					Color						background;
					Ptr<windows::WinBrush>		backgroundBrush;
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
				FontProperties			oldFont;
				Ptr<windows::WinFont>	font;
				ColorArray				colors;
				Color					oldCaretColor;
				Ptr<windows::WinPen>	caretPen;

				void					DestroyColors();
				void					ColorChanged();
				void					FontChanged();

				void					InitializeInternal();
				void					FinalizeInternal();
				void					RenderTargetChangedInternal(IWindowsGDIRenderTarget* oldRenderTarget, IWindowsGDIRenderTarget* newRenderTarget);
			public:
				void					Render(Rect bounds)override;
				void					OnElementStateChanged()override;
			};
		}
	}
}

#endif
#endif