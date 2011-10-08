/***********************************************************************
Vczh Library++ 3.0
Developer: �����(vczh)
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
				DEFINE_GUI_GRAPHICS_RENDERER(GuiSolidBorderElement, GuiSolidBorderElementRenderer, IWindowsGDIRenderTarget)
			protected:
				Color					oldColor;
				Ptr<windows::WinPen>	pen;
				Ptr<windows::WinBrush>	brush;

				void					InitializeInternal();
				void					RenderTargetChangedInternal();
			public:
				void					Render(Rect bounds);
				void					OnElementStateChanged();
			};

			class GuiRoundBorderElementRenderer : public Object, public IGuiGraphicsRenderer
			{
				DEFINE_GUI_GRAPHICS_RENDERER(GuiRoundBorderElement, GuiRoundBorderElementRenderer, IWindowsGDIRenderTarget)
			protected:
				Color					oldColor;
				Ptr<windows::WinPen>	pen;
				Ptr<windows::WinBrush>	brush;

				void					InitializeInternal();
				void					RenderTargetChangedInternal();
			public:
				void					Render(Rect bounds);
				void					OnElementStateChanged();
			};

			class GuiSolidBackgroundElementRenderer : public Object, public IGuiGraphicsRenderer
			{
				DEFINE_GUI_GRAPHICS_RENDERER(GuiSolidBackgroundElement, GuiSolidBackgroundElementRenderer, IWindowsGDIRenderTarget)
			protected:
				Color					oldColor;
				Ptr<windows::WinBrush>	brush;

				void					InitializeInternal();
				void					RenderTargetChangedInternal();
			public:
				void					Render(Rect bounds);
				void					OnElementStateChanged();
			};

			class GuiGradientBackgroundElementRenderer : public Object, public IGuiGraphicsRenderer
			{
				DEFINE_GUI_GRAPHICS_RENDERER(GuiGradientBackgroundElement, GuiGradientBackgroundElementRenderer, IWindowsGDIRenderTarget)
			protected:
				void					InitializeInternal();
				void					RenderTargetChangedInternal();
			public:
				void					Render(Rect bounds);
				void					OnElementStateChanged();
			};

			class GuiSolidLabelElementRenderer : public Object, public IGuiGraphicsRenderer
			{
				DEFINE_GUI_GRAPHICS_RENDERER(GuiSolidLabelElement, GuiSolidLabelElementRenderer, IWindowsGDIRenderTarget)
			protected:
				FontProperties			oldFont;
				Ptr<windows::WinFont>	font;

				void					UpdateMinSize();

				void					InitializeInternal();
				void					RenderTargetChangedInternal();
			public:
				void					Render(Rect bounds);
				void					OnElementStateChanged();
			};
		}
	}
}

#endif