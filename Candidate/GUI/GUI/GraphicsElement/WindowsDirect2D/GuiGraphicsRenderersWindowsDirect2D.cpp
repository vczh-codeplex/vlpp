#ifdef GUI_GRAPHICS_RENDERER_DIRECT2D

#include "GuiGraphicsRenderersWindowsDirect2D.h"

namespace vl
{
	namespace presentation
	{
		namespace elements_windows_d2d
		{
			using namespace windows;
			using namespace collections;

/***********************************************************************
GuiSolidBorderElementRenderer
***********************************************************************/

			void GuiSolidBorderElementRenderer::InitializeInternal()
			{
			}

			void GuiSolidBorderElementRenderer::FinalizeInternal()
			{
			}

			void GuiSolidBorderElementRenderer::RenderTargetChangedInternal()
			{
			}

			void GuiSolidBorderElementRenderer::Render(Rect bounds)
			{
			}

			void GuiSolidBorderElementRenderer::OnElementStateChanged()
			{
			}

/***********************************************************************
GuiRoundBorderElementRenderer
***********************************************************************/

			void GuiRoundBorderElementRenderer::InitializeInternal()
			{
			}

			void GuiRoundBorderElementRenderer::FinalizeInternal()
			{
			}

			void GuiRoundBorderElementRenderer::RenderTargetChangedInternal()
			{
			}

			void GuiRoundBorderElementRenderer::Render(Rect bounds)
			{
			}

			void GuiRoundBorderElementRenderer::OnElementStateChanged()
			{
			}

/***********************************************************************
GuiSolidBackgroundElementRenderer
***********************************************************************/

			void GuiSolidBackgroundElementRenderer::InitializeInternal()
			{
			}

			void GuiSolidBackgroundElementRenderer::FinalizeInternal()
			{
			}

			void GuiSolidBackgroundElementRenderer::RenderTargetChangedInternal()
			{
			}

			void GuiSolidBackgroundElementRenderer::Render(Rect bounds)
			{
			}

			void GuiSolidBackgroundElementRenderer::OnElementStateChanged()
			{
			}

/***********************************************************************
GuiGradientBackgroundElementRenderer
***********************************************************************/

			void GuiGradientBackgroundElementRenderer::InitializeInternal()
			{
			}

			void GuiGradientBackgroundElementRenderer::FinalizeInternal()
			{
			}

			void GuiGradientBackgroundElementRenderer::RenderTargetChangedInternal()
			{
			}

			void GuiGradientBackgroundElementRenderer::Render(Rect bounds)
			{
			}

			void GuiGradientBackgroundElementRenderer::OnElementStateChanged()
			{
			}

/***********************************************************************
GuiSolidLabelElementRenderer
***********************************************************************/

			void GuiSolidLabelElementRenderer::UpdateMinSize()
			{
			}

			void GuiSolidLabelElementRenderer::InitializeInternal()
			{
			}

			void GuiSolidLabelElementRenderer::FinalizeInternal()
			{
			}

			void GuiSolidLabelElementRenderer::RenderTargetChangedInternal()
			{
				UpdateMinSize();
			}

			void GuiSolidLabelElementRenderer::Render(Rect bounds)
			{
			}

			void GuiSolidLabelElementRenderer::OnElementStateChanged()
			{
				UpdateMinSize();
			}
		}
	}
}

#endif