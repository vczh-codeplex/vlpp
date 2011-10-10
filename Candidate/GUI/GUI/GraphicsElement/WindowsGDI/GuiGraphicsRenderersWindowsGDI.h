/***********************************************************************
Vczh Library++ 3.0
Developer: ³Âè÷å«(vczh)
GUI::Windows Platform

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
				void					FinalizeInternal();
				void					RenderTargetChangedInternal(IWindowsGDIRenderTarget* oldRenderTarget, IWindowsGDIRenderTarget* newRenderTarget);
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
				void					FinalizeInternal();
				void					RenderTargetChangedInternal(IWindowsGDIRenderTarget* oldRenderTarget, IWindowsGDIRenderTarget* newRenderTarget);
			public:
				void					Render(Rect bounds);
				void					OnElementStateChanged();
			};

			class GuiGradientBackgroundElementRenderer : public Object, public IGuiGraphicsRenderer
			{
				DEFINE_GUI_GRAPHICS_RENDERER(GuiGradientBackgroundElement, GuiGradientBackgroundElementRenderer, IWindowsGDIRenderTarget)
			protected:
				void					InitializeInternal();
				void					FinalizeInternal();
				void					RenderTargetChangedInternal(IWindowsGDIRenderTarget* oldRenderTarget, IWindowsGDIRenderTarget* newRenderTarget);
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
				void					FinalizeInternal();
				void					RenderTargetChangedInternal(IWindowsGDIRenderTarget* oldRenderTarget, IWindowsGDIRenderTarget* newRenderTarget);
			public:
				void					Render(Rect bounds);
				void					OnElementStateChanged();
			};
		}
	}
}

#endif
#endif