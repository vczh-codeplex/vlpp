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
IMPLEMENT_BRUSH_ELEMENT_RENDERER
***********************************************************************/

#define IMPLEMENT_BRUSH_ELEMENT_RENDERER(TRENDERER)\
			void TRENDERER::CreateBrush(IWindowsDirect2DRenderTarget* _renderTarget)\
			{\
				if(_renderTarget)\
				{\
					oldColor=element->GetColor();\
					brush=_renderTarget->CreateDirect2DBrush(oldColor);\
				}\
			}\
			void TRENDERER::DestroyBrush(IWindowsDirect2DRenderTarget* _renderTarget)\
			{\
				if(_renderTarget && brush)\
				{\
					_renderTarget->DestroyDirect2DBrush(oldColor);\
					brush=0;\
				}\
			}\
			void TRENDERER::InitializeInternal()\
			{\
			}\
			void TRENDERER::FinalizeInternal()\
			{\
				DestroyBrush(renderTarget);\
			}\
			void TRENDERER::RenderTargetChangedInternal(IWindowsDirect2DRenderTarget* oldRenderTarget, IWindowsDirect2DRenderTarget* newRenderTarget)\
			{\
				DestroyBrush(oldRenderTarget);\
				CreateBrush(newRenderTarget);\
			}\
			void TRENDERER::OnElementStateChanged()\
			{\
				if(renderTarget)\
				{\
					Color color=element->GetColor();\
					if(oldColor!=color)\
					{\
						DestroyBrush(renderTarget);\
						oldColor=color;\
						CreateBrush(renderTarget);\
					}\
				}\
			}\
			void TRENDERER::Render(Rect bounds)\

/***********************************************************************
GuiSolidBorderElementRenderer
***********************************************************************/

			IMPLEMENT_BRUSH_ELEMENT_RENDERER(GuiSolidBorderElementRenderer)
			{
				ID2D1RenderTarget* d2dRenderTarget=renderTarget->GetDirect2DRenderTarget();
				d2dRenderTarget->DrawRectangle(
					D2D1::RectF((FLOAT)bounds.x1, (FLOAT)bounds.y1, (FLOAT)bounds.x2, (FLOAT)bounds.y2),
					brush
					);
			}

/***********************************************************************
GuiRoundBorderElementRenderer
***********************************************************************/

			IMPLEMENT_BRUSH_ELEMENT_RENDERER(GuiRoundBorderElementRenderer)
			{
				ID2D1RenderTarget* d2dRenderTarget=renderTarget->GetDirect2DRenderTarget();
				d2dRenderTarget->DrawRoundedRectangle(
					D2D1::RoundedRect(
						D2D1::RectF((FLOAT)bounds.x1, (FLOAT)bounds.y1, (FLOAT)bounds.x2, (FLOAT)bounds.y2),
						(FLOAT)element->GetRadius(),
						(FLOAT)element->GetRadius()
						),
					brush
					);
			}

/***********************************************************************
GuiSolidBackgroundElementRenderer
***********************************************************************/

			IMPLEMENT_BRUSH_ELEMENT_RENDERER(GuiSolidBackgroundElementRenderer)
			{
				ID2D1RenderTarget* d2dRenderTarget=renderTarget->GetDirect2DRenderTarget();
				d2dRenderTarget->FillRectangle(
					D2D1::RectF((FLOAT)bounds.x1, (FLOAT)bounds.y1, (FLOAT)bounds.x2, (FLOAT)bounds.y2),
					brush
					);
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

			void GuiGradientBackgroundElementRenderer::RenderTargetChangedInternal(IWindowsDirect2DRenderTarget* oldRenderTarget, IWindowsDirect2DRenderTarget* newRenderTarget)
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

			void GuiSolidLabelElementRenderer::RenderTargetChangedInternal(IWindowsDirect2DRenderTarget* oldRenderTarget, IWindowsDirect2DRenderTarget* newRenderTarget)
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