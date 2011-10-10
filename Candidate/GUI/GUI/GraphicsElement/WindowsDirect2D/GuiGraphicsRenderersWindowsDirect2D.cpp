#ifdef GUI_GRAPHICS_RENDERER_DIRECT2D

#include <math.h>
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
			TRENDERER::TRENDERER()\
				:brush(0)\
			{\
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
					D2D1::RectF((FLOAT)bounds.x1+0.5f, (FLOAT)bounds.y1+0.5f, (FLOAT)bounds.x2-0.5f, (FLOAT)bounds.y2-0.5f),
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
						D2D1::RectF((FLOAT)bounds.x1+0.5f, (FLOAT)bounds.y1+0.5f, (FLOAT)bounds.x2-0.5f, (FLOAT)bounds.y2-0.5f),
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

			void GuiSolidLabelElementRenderer::CreateBrush(IWindowsDirect2DRenderTarget* _renderTarget)
			{
				if(_renderTarget)
				{
					oldColor=element->GetColor();
					brush=_renderTarget->CreateDirect2DBrush(oldColor);
				}
			}

			void GuiSolidLabelElementRenderer::DestroyBrush(IWindowsDirect2DRenderTarget* _renderTarget)
			{
				if(_renderTarget && brush)
				{
					_renderTarget->DestroyDirect2DBrush(oldColor);
					brush=0;
				}
			}

			void GuiSolidLabelElementRenderer::CreateTextFormat(IWindowsDirect2DRenderTarget* _renderTarget)
			{
				if(_renderTarget)
				{
					IWindowsDirect2DResourceManager* resourceManager=GetWindowsDirect2DResourceManager();
					oldFont=element->GetFont();
					textFormat=resourceManager->CreateDirect2DTextFormat(oldFont);
				}
			}

			void GuiSolidLabelElementRenderer::DestroyTextFormat(IWindowsDirect2DRenderTarget* _renderTarget)
			{
				if(_renderTarget && textFormat)
				{
					IWindowsDirect2DResourceManager* resourceManager=GetWindowsDirect2DResourceManager();
					resourceManager->DestroyDirect2DTextFormat(oldFont);
					textFormat=0;
				}
			}

			void GuiSolidLabelElementRenderer::UpdateMinSize()
			{
				if(renderTarget)
				{
					IDWriteTextLayout* textLayout=0;
					HRESULT hr=GetDirectWriteFactory()->CreateTextLayout(
						oldText.Buffer(),
						oldText.Length(),
						textFormat,
						0,
						0,
						&textLayout);
					if(!FAILED(hr))
					{
						DWRITE_TEXT_METRICS metrics;
						hr=textLayout->GetMetrics(&metrics);
						if(!FAILED(hr))
						{
							minSize=Size((int)ceil(metrics.widthIncludingTrailingWhitespace), (int)ceil(metrics.height));
						}
						textLayout->Release();
						return;
					}
				}
				minSize=Size();
			}

			void GuiSolidLabelElementRenderer::InitializeInternal()
			{
			}

			void GuiSolidLabelElementRenderer::FinalizeInternal()
			{
				DestroyBrush(renderTarget);
				DestroyTextFormat(renderTarget);
			}

			void GuiSolidLabelElementRenderer::RenderTargetChangedInternal(IWindowsDirect2DRenderTarget* oldRenderTarget, IWindowsDirect2DRenderTarget* newRenderTarget)
			{
				DestroyBrush(oldRenderTarget);
				DestroyTextFormat(oldRenderTarget);
				CreateBrush(newRenderTarget);
				CreateTextFormat(newRenderTarget);
				UpdateMinSize();
			}

			GuiSolidLabelElementRenderer::GuiSolidLabelElementRenderer()
				:brush(0)
				,textFormat(0)
			{
			}

			void GuiSolidLabelElementRenderer::Render(Rect bounds)
			{
				int x=0;
				int y=0;
				switch(element->GetHorizontalAlignment())
				{
				case Alignment::Left:
					x=bounds.Left();
					break;
				case Alignment::Center:
					x=bounds.Left()+(bounds.Width()-minSize.x)/2;
					break;
				case Alignment::Right:
					x=bounds.Right()-minSize.x;
					break;
				}
				switch(element->GetVerticalAlignment())
				{
				case Alignment::Top:
					y=bounds.Top();
					break;
				case Alignment::Center:
					y=bounds.Top()+(bounds.Height()-minSize.y)/2;
					break;
				case Alignment::Bottom:
					y=bounds.Bottom()-minSize.y;
					break;
				}

				ID2D1RenderTarget* d2dRenderTarget=renderTarget->GetDirect2DRenderTarget();
				d2dRenderTarget->DrawText(
					oldText.Buffer(),
					oldText.Length(),
					textFormat,
					D2D1::RectF((FLOAT)x, (FLOAT)y, (FLOAT)x+1, (FLOAT)y+1),
					brush,
					D2D1_DRAW_TEXT_OPTIONS_NO_SNAP,
					DWRITE_MEASURING_MODE_GDI_NATURAL
					);
			}

			void GuiSolidLabelElementRenderer::OnElementStateChanged()
			{
				bool fontChanged=false;
				bool textChanged=false;
				if(renderTarget)
				{
					Color color=element->GetColor();
					if(oldColor!=color)
					{
						DestroyBrush(renderTarget);
						oldColor=color;
						CreateBrush(renderTarget);
					}

					FontProperties font=element->GetFont();
					if(oldFont!=font)
					{
						DestroyTextFormat(renderTarget);
						oldFont=font;
						CreateTextFormat(renderTarget);
						fontChanged=true;
					}
				}

				if(oldText!=element->GetText())
				{
					oldText=element->GetText();
					textChanged=true;
				}
				if(fontChanged || textChanged)
				{
					UpdateMinSize();
				}
			}
		}
	}
}

#endif