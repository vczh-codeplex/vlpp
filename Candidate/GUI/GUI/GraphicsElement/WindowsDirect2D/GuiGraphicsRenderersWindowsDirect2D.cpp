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
			void TRENDERER::Render(Rect bounds)\

#define IMPLEMENT_BRUSH_ELEMENT_RENDERER_SOLID_COLOR_BRUSH(TRENDERER)\
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
			void TRENDERER::OnElementStateChanged()\
			{\
				if(renderTarget)\
				{\
					Color color=element->GetColor();\
					if(oldColor!=color)\
					{\
						DestroyBrush(renderTarget);\
						CreateBrush(renderTarget);\
					}\
				}\
			}\

#define IMPLEMENT_BRUSH_ELEMENT_RENDERER_LINEAR_GRADIENT_BRUSH(TRENDERER)\
			void TRENDERER::CreateBrush(IWindowsDirect2DRenderTarget* _renderTarget)\
			{\
				if(_renderTarget)\
				{\
					oldColor=Pair<Color, Color>(element->GetColor1(), element->GetColor2());\
					brush=_renderTarget->CreateDirect2DLinearBrush(oldColor.key, oldColor.value);\
				}\
			}\
			void TRENDERER::DestroyBrush(IWindowsDirect2DRenderTarget* _renderTarget)\
			{\
				if(_renderTarget && brush)\
				{\
					_renderTarget->DestroyDirect2DLinearBrush(oldColor.key, oldColor.value);\
					brush=0;\
				}\
			}\
			void TRENDERER::OnElementStateChanged()\
			{\
				if(renderTarget)\
				{\
					Pair<Color, Color> color=Pair<Color, Color>(element->GetColor1(), element->GetColor2());\
					if(oldColor!=color)\
					{\
						DestroyBrush(renderTarget);\
						CreateBrush(renderTarget);\
					}\
				}\
			}\

/***********************************************************************
GuiSolidBorderElementRenderer
***********************************************************************/

			IMPLEMENT_BRUSH_ELEMENT_RENDERER_SOLID_COLOR_BRUSH(GuiSolidBorderElementRenderer)
			IMPLEMENT_BRUSH_ELEMENT_RENDERER(GuiSolidBorderElementRenderer)
			{
				ID2D1RenderTarget* d2dRenderTarget=renderTarget->GetDirect2DRenderTarget();
				switch(element->GetShape())
				{
				case ElementShape::Rectangle:
					d2dRenderTarget->DrawRectangle(
						D2D1::RectF((FLOAT)bounds.x1+0.5f, (FLOAT)bounds.y1+0.5f, (FLOAT)bounds.x2-0.5f, (FLOAT)bounds.y2-0.5f),
						brush
						);
					break;
				case ElementShape::Ellipse:
					d2dRenderTarget->DrawEllipse(
						D2D1::Ellipse(D2D1::Point2F((bounds.x1+bounds.x2)/2.0f, (bounds.y1+bounds.y2)/2.0f), bounds.Width()/2.0f, bounds.Height()/2.0f),
						brush
						);
					break;
				}
			}

/***********************************************************************
GuiRoundBorderElementRenderer
***********************************************************************/
				
			IMPLEMENT_BRUSH_ELEMENT_RENDERER_SOLID_COLOR_BRUSH(GuiRoundBorderElementRenderer)
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
Gui3DBorderElementRenderer
***********************************************************************/

			void Gui3DBorderElementRenderer::CreateBrush(IWindowsDirect2DRenderTarget* _renderTarget)
			{
				if(_renderTarget)
				{
					oldColor1=element->GetColor1();
					oldColor2=element->GetColor2();
					brush1=_renderTarget->CreateDirect2DBrush(oldColor1);
					brush2=_renderTarget->CreateDirect2DBrush(oldColor2);
				}
			}

			void Gui3DBorderElementRenderer::DestroyBrush(IWindowsDirect2DRenderTarget* _renderTarget)
			{
				if(_renderTarget)
				{
					if(brush1)
					{
						_renderTarget->DestroyDirect2DBrush(oldColor1);
						brush1=0;
					}
					if(brush2)
					{
						_renderTarget->DestroyDirect2DBrush(oldColor2);
						brush2=0;
					}
				}
			}

			void Gui3DBorderElementRenderer::InitializeInternal()
			{
			}

			void Gui3DBorderElementRenderer::FinalizeInternal()
			{
				DestroyBrush(renderTarget);
			}

			void Gui3DBorderElementRenderer::RenderTargetChangedInternal(IWindowsDirect2DRenderTarget* oldRenderTarget, IWindowsDirect2DRenderTarget* newRenderTarget)
			{
				DestroyBrush(oldRenderTarget);
				CreateBrush(newRenderTarget);
			}

			Gui3DBorderElementRenderer::Gui3DBorderElementRenderer()
				:brush1(0)
				,brush2(0)
			{
			}

			void Gui3DBorderElementRenderer::Render(Rect bounds)
			{
				D2D1_RECT_F rect=D2D1::RectF((FLOAT)bounds.x1+0.5f, (FLOAT)bounds.y1+0.5f, (FLOAT)bounds.x2-0.5f, (FLOAT)bounds.y2-0.5f);
				ID2D1RenderTarget* d2dRenderTarget=renderTarget->GetDirect2DRenderTarget();

				d2dRenderTarget->DrawLine(D2D1::Point2F(rect.left, rect.top), D2D1::Point2F(rect.right, rect.top), brush1);
				d2dRenderTarget->DrawLine(D2D1::Point2F(rect.left, rect.top), D2D1::Point2F(rect.left, rect.bottom), brush1);
				d2dRenderTarget->DrawLine(D2D1::Point2F(rect.right, rect.bottom), D2D1::Point2F(rect.left, rect.bottom), brush2);
				d2dRenderTarget->DrawLine(D2D1::Point2F(rect.right, rect.bottom), D2D1::Point2F(rect.right, rect.top), brush2);
			}

			void Gui3DBorderElementRenderer::OnElementStateChanged()
			{
				if(renderTarget)
				{
					Color color1=element->GetColor1();
					Color color2=element->GetColor2();
					if(oldColor1!=color1 || oldColor2!=color2)
					{
						DestroyBrush(renderTarget);
						CreateBrush(renderTarget);
					}
				}
			}

/***********************************************************************
Gui3DSplitterElementRenderer
***********************************************************************/

			void Gui3DSplitterElementRenderer::CreateBrush(IWindowsDirect2DRenderTarget* _renderTarget)
			{
				if(_renderTarget)
				{
					oldColor1=element->GetColor1();
					oldColor2=element->GetColor2();
					brush1=_renderTarget->CreateDirect2DBrush(oldColor1);
					brush2=_renderTarget->CreateDirect2DBrush(oldColor2);
				}
			}

			void Gui3DSplitterElementRenderer::DestroyBrush(IWindowsDirect2DRenderTarget* _renderTarget)
			{
				if(_renderTarget)
				{
					if(brush1)
					{
						_renderTarget->DestroyDirect2DBrush(oldColor1);
						brush1=0;
					}
					if(brush2)
					{
						_renderTarget->DestroyDirect2DBrush(oldColor2);
						brush2=0;
					}
				}
			}

			void Gui3DSplitterElementRenderer::InitializeInternal()
			{
			}

			void Gui3DSplitterElementRenderer::FinalizeInternal()
			{
				DestroyBrush(renderTarget);
			}

			void Gui3DSplitterElementRenderer::RenderTargetChangedInternal(IWindowsDirect2DRenderTarget* oldRenderTarget, IWindowsDirect2DRenderTarget* newRenderTarget)
			{
				DestroyBrush(oldRenderTarget);
				CreateBrush(newRenderTarget);
			}

			Gui3DSplitterElementRenderer::Gui3DSplitterElementRenderer()
				:brush1(0)
				,brush2(0)
			{
			}

			void Gui3DSplitterElementRenderer::Render(Rect bounds)
			{
				D2D1_POINT_2F p11, p12, p21, p22;
				switch(element->GetDirection())
				{
				case Gui3DSplitterElement::Horizontal:
					{
						int y=bounds.y1+bounds.Height()/2-1;
						p11=D2D1::Point2F((FLOAT)bounds.x1, (FLOAT)y+0.5f);
						p12=D2D1::Point2F((FLOAT)bounds.x2, (FLOAT)y+0.5f);
						p21=D2D1::Point2F((FLOAT)bounds.x1, (FLOAT)y+1.5f);
						p22=D2D1::Point2F((FLOAT)bounds.x2, (FLOAT)y+1.5f);
					}
					break;
				case Gui3DSplitterElement::Vertical:
					{
						int x=bounds.x1+bounds.Width()/2-1;
						p11=D2D1::Point2F((FLOAT)x+0.5f, (FLOAT)bounds.y1-0.0f);
						p12=D2D1::Point2F((FLOAT)x+0.5f, (FLOAT)bounds.y2+0.0f);
						p21=D2D1::Point2F((FLOAT)x+1.5f, (FLOAT)bounds.y1-0.0f);
						p22=D2D1::Point2F((FLOAT)x+1.5f, (FLOAT)bounds.y2+0.0f);
					}
					break;
				}
				ID2D1RenderTarget* d2dRenderTarget=renderTarget->GetDirect2DRenderTarget();

				d2dRenderTarget->DrawLine(p11, p12, brush1);
				d2dRenderTarget->DrawLine(p21, p22, brush2);
			}

			void Gui3DSplitterElementRenderer::OnElementStateChanged()
			{
				if(renderTarget)
				{
					Color color1=element->GetColor1();
					Color color2=element->GetColor2();
					if(oldColor1!=color1 || oldColor2!=color2)
					{
						DestroyBrush(renderTarget);
						CreateBrush(renderTarget);
					}
				}
			}

/***********************************************************************
GuiSolidBackgroundElementRenderer
***********************************************************************/
			
			IMPLEMENT_BRUSH_ELEMENT_RENDERER_SOLID_COLOR_BRUSH(GuiSolidBackgroundElementRenderer)
			IMPLEMENT_BRUSH_ELEMENT_RENDERER(GuiSolidBackgroundElementRenderer)
			{
				ID2D1RenderTarget* d2dRenderTarget=renderTarget->GetDirect2DRenderTarget();
				switch(element->GetShape())
				{
				case ElementShape::Rectangle:
					d2dRenderTarget->FillRectangle(
						D2D1::RectF((FLOAT)bounds.x1, (FLOAT)bounds.y1, (FLOAT)bounds.x2, (FLOAT)bounds.y2),
						brush
						);
					break;
				case ElementShape::Ellipse:
					d2dRenderTarget->FillEllipse(
						D2D1::Ellipse(D2D1::Point2F((bounds.x1+bounds.x2)/2.0f, (bounds.y1+bounds.y2)/2.0f), bounds.Width()/2.0f, bounds.Height()/2.0f),
						brush
						);
					break;
				}
			}

/***********************************************************************
GuiGradientBackgroundElementRenderer
***********************************************************************/

			IMPLEMENT_BRUSH_ELEMENT_RENDERER_LINEAR_GRADIENT_BRUSH(GuiGradientBackgroundElementRenderer)
			IMPLEMENT_BRUSH_ELEMENT_RENDERER(GuiGradientBackgroundElementRenderer)
			{
				D2D1_POINT_2F points[2];
				switch(element->GetDirection())
				{
				case GuiGradientBackgroundElement::Horizontal:
					{
						points[0].x=(FLOAT)bounds.x1;
						points[0].y=(FLOAT)bounds.y1;
						points[1].x=(FLOAT)bounds.x2;
						points[1].y=(FLOAT)bounds.y1;
					}
					break;
				case GuiGradientBackgroundElement::Vertical:
					{
						points[0].x=(FLOAT)bounds.x1;
						points[0].y=(FLOAT)bounds.y1;
						points[1].x=(FLOAT)bounds.x1;
						points[1].y=(FLOAT)bounds.y2;
					}
					break;
				case GuiGradientBackgroundElement::Slash:
					{
						points[0].x=(FLOAT)bounds.x2;
						points[0].y=(FLOAT)bounds.y1;
						points[1].x=(FLOAT)bounds.x1;
						points[1].y=(FLOAT)bounds.y2;
					}
					break;
				case GuiGradientBackgroundElement::Backslash:
					{
						points[0].x=(FLOAT)bounds.x1;
						points[0].y=(FLOAT)bounds.y1;
						points[1].x=(FLOAT)bounds.x2;
						points[1].y=(FLOAT)bounds.y2;
					}
					break;
				}

				brush->SetStartPoint(points[0]);
				brush->SetEndPoint(points[1]);
				
				ID2D1RenderTarget* d2dRenderTarget=renderTarget->GetDirect2DRenderTarget();
				switch(element->GetShape())
				{
				case ElementShape::Rectangle:
					d2dRenderTarget->FillRectangle(
						D2D1::RectF((FLOAT)bounds.x1, (FLOAT)bounds.y1, (FLOAT)bounds.x2, (FLOAT)bounds.y2),
						brush
						);
					break;
				case ElementShape::Ellipse:
					d2dRenderTarget->FillEllipse(
						D2D1::Ellipse(D2D1::Point2F((bounds.x1+bounds.x2)/2.0f, (bounds.y1+bounds.y2)/2.0f), bounds.Width()/2.0f, bounds.Height()/2.0f),
						brush
						);
					break;
				}
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
				if(renderTarget && !element->GetMultiline() && !element->GetWrapLine())
				{
					IDWriteTextLayout* textLayout=0;
					HRESULT hr=GetDirectWriteFactory()->CreateTextLayout(
						oldText.Buffer(),
						oldText.Length(),
						textFormat->textFormat.Obj(),
						0,
						0,
						&textLayout);
					if(!FAILED(hr))
					{
						DWRITE_TEXT_METRICS metrics;
						hr=textLayout->GetMetrics(&metrics);
						if(!FAILED(hr))
						{
							minSize=Size((element->GetEllipse()?0:(int)ceil(metrics.widthIncludingTrailingWhitespace)), (int)ceil(metrics.height));
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
				if(!element->GetEllipse() && !element->GetMultiline() && !element->GetWrapLine())
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
						textFormat->textFormat.Obj(),
						D2D1::RectF((FLOAT)x, (FLOAT)y, (FLOAT)x+1, (FLOAT)y+1),
						brush,
						D2D1_DRAW_TEXT_OPTIONS_NO_SNAP,
						DWRITE_MEASURING_MODE_GDI_NATURAL
						);
				}
				else
				{
					IDWriteFactory* dwriteFactory=GetDirectWriteFactory();
					DWRITE_WORD_WRAPPING wrapping=textFormat->textFormat->GetWordWrapping();
					DWRITE_TEXT_ALIGNMENT textAlignment=textFormat->textFormat->GetTextAlignment();
					DWRITE_PARAGRAPH_ALIGNMENT paragraphAlignment=textFormat->textFormat->GetParagraphAlignment();
					DWRITE_TRIMMING trimming;
					IDWriteInlineObject* inlineObject;
					textFormat->textFormat->GetTrimming(&trimming, &inlineObject);

					if(element->GetWrapLine())
					{
						textFormat->textFormat->SetWordWrapping(DWRITE_WORD_WRAPPING_WRAP);
					}
					if(element->GetEllipse())
					{
						textFormat->textFormat->SetTrimming(&textFormat->trimming, textFormat->ellipseInlineObject.Obj());
					}
					switch(element->GetHorizontalAlignment())
					{
					case Alignment::Left:
						textFormat->textFormat->SetTextAlignment(DWRITE_TEXT_ALIGNMENT_LEADING);
						break;
					case Alignment::Center:
						textFormat->textFormat->SetTextAlignment(DWRITE_TEXT_ALIGNMENT_CENTER);
						break;
					case Alignment::Right:
						textFormat->textFormat->SetTextAlignment(DWRITE_TEXT_ALIGNMENT_TRAILING);
						break;
					}
					if(!element->GetMultiline() && !element->GetWrapLine())
					{
						switch(element->GetVerticalAlignment())
						{
						case Alignment::Top:
							textFormat->textFormat->SetParagraphAlignment(DWRITE_PARAGRAPH_ALIGNMENT_NEAR);
							break;
						case Alignment::Center:
							textFormat->textFormat->SetParagraphAlignment(DWRITE_PARAGRAPH_ALIGNMENT_CENTER);
							break;
						case Alignment::Bottom:
							textFormat->textFormat->SetParagraphAlignment(DWRITE_PARAGRAPH_ALIGNMENT_FAR);
							break;
						}
					}

					ID2D1RenderTarget* d2dRenderTarget=renderTarget->GetDirect2DRenderTarget();
					d2dRenderTarget->DrawText(
						oldText.Buffer(),
						oldText.Length(),
						textFormat->textFormat.Obj(),
						D2D1::RectF((FLOAT)bounds.Left(), (FLOAT)bounds.Top(), (FLOAT)bounds.Right(), (FLOAT)bounds.Bottom()),
						brush,
						D2D1_DRAW_TEXT_OPTIONS_NO_SNAP,
						DWRITE_MEASURING_MODE_GDI_NATURAL
						);

					textFormat->textFormat->SetWordWrapping(wrapping);
					textFormat->textFormat->SetTextAlignment(textAlignment);
					textFormat->textFormat->SetParagraphAlignment(paragraphAlignment);
					textFormat->textFormat->SetTrimming(&trimming, inlineObject);
				}
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
						CreateBrush(renderTarget);
					}

					FontProperties font=element->GetFont();
					if(oldFont!=font)
					{
						DestroyTextFormat(renderTarget);
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

/***********************************************************************
GuiImageFrameElementRenderer
***********************************************************************/

			void GuiImageFrameElementRenderer::UpdateBitmap(IWindowsDirect2DRenderTarget* renderTarget)
			{
				if(renderTarget && element->GetImage())
				{
					INativeImageFrame* frame=element->GetImage()->GetFrame(element->GetFrameIndex());
					bitmap=renderTarget->GetBitmap(frame);
					minSize=frame->GetSize();
				}
				else
				{
					bitmap=0;
					minSize=Size(0, 0);
				}
			}

			void GuiImageFrameElementRenderer::InitializeInternal()
			{
			}

			void GuiImageFrameElementRenderer::FinalizeInternal()
			{
			}

			void GuiImageFrameElementRenderer::RenderTargetChangedInternal(IWindowsDirect2DRenderTarget* oldRenderTarget, IWindowsDirect2DRenderTarget* newRenderTarget)
			{
				UpdateBitmap(newRenderTarget);
			}

			GuiImageFrameElementRenderer::GuiImageFrameElementRenderer()
			{
			}

			void GuiImageFrameElementRenderer::Render(Rect bounds)
			{
				if(bitmap)
				{
					ID2D1RenderTarget* d2dRenderTarget=renderTarget->GetDirect2DRenderTarget();
					D2D1_RECT_F source=D2D1::RectF(0, 0, (FLOAT)minSize.x, (FLOAT)minSize.y);
					D2D1_RECT_F destination;
					if(element->GetStretch())
					{
						destination=D2D1::RectF((FLOAT)bounds.x1, (FLOAT)bounds.y1, (FLOAT)bounds.x2, (FLOAT)bounds.y2);
					}
					else
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
						destination=D2D1::RectF((FLOAT)x, (FLOAT)y, (FLOAT)(x+minSize.x), (FLOAT)(y+minSize.y));
					}
					if(element->GetImage()->GetFormat()==INativeImage::Gif &&  element->GetFrameIndex()>0)
					{
						int max=element->GetFrameIndex();
						for(int i=0;i<=max;i++)
						{
							ComPtr<ID2D1Bitmap> frameBitmap=renderTarget->GetBitmap(element->GetImage()->GetFrame(i));
							d2dRenderTarget->DrawBitmap(frameBitmap.Obj(), destination, 1.0f, D2D1_BITMAP_INTERPOLATION_MODE_LINEAR, source);
						}
					}
					else
					{
						d2dRenderTarget->DrawBitmap(bitmap.Obj(), destination, 1.0f, D2D1_BITMAP_INTERPOLATION_MODE_LINEAR, source);
					}
				}
			}

			void GuiImageFrameElementRenderer::OnElementStateChanged()
			{
				UpdateBitmap(renderTarget);
			}

/***********************************************************************
GuiColorizedTextElementRenderer
***********************************************************************/

			void GuiColorizedTextElementRenderer::CreateTextBrush(IWindowsDirect2DRenderTarget* _renderTarget)
			{
				if(_renderTarget)
				{
					colors.Resize(element->GetColors().Count());
					for(int i=0;i<colors.Count();i++)
					{
						text::ColorEntry entry=element->GetColors()[i];
						ColorEntryResource newEntry;

						newEntry.normal.text=entry.normal.text;
						newEntry.normal.textBrush=_renderTarget->CreateDirect2DBrush(newEntry.normal.text);
						newEntry.normal.background=entry.normal.background;
						newEntry.normal.backgroundBrush=_renderTarget->CreateDirect2DBrush(newEntry.normal.background);
						newEntry.selectedFocused.text=entry.selectedFocused.text;
						newEntry.selectedFocused.textBrush=_renderTarget->CreateDirect2DBrush(newEntry.selectedFocused.text);
						newEntry.selectedFocused.background=entry.selectedFocused.background;
						newEntry.selectedFocused.backgroundBrush=_renderTarget->CreateDirect2DBrush(newEntry.selectedFocused.background);
						newEntry.selectedUnfocused.text=entry.selectedUnfocused.text;
						newEntry.selectedUnfocused.textBrush=_renderTarget->CreateDirect2DBrush(newEntry.selectedUnfocused.text);
						newEntry.selectedUnfocused.background=entry.selectedUnfocused.background;
						newEntry.selectedUnfocused.backgroundBrush=_renderTarget->CreateDirect2DBrush(newEntry.selectedUnfocused.background);
						colors[i]=newEntry;
					}
				}
			}

			void GuiColorizedTextElementRenderer::DestroyTextBrush(IWindowsDirect2DRenderTarget* _renderTarget)
			{
				if(_renderTarget)
				{
					for(int i=0;i<colors.Count();i++)
					{
						_renderTarget->DestroyDirect2DBrush(colors[i].normal.text);
						_renderTarget->DestroyDirect2DBrush(colors[i].normal.background);
						_renderTarget->DestroyDirect2DBrush(colors[i].selectedFocused.text);
						_renderTarget->DestroyDirect2DBrush(colors[i].selectedFocused.background);
						_renderTarget->DestroyDirect2DBrush(colors[i].selectedUnfocused.text);
						_renderTarget->DestroyDirect2DBrush(colors[i].selectedUnfocused.background);
					}
					colors.Resize(0);
				}
			}

			void GuiColorizedTextElementRenderer::CreateCaretBrush(IWindowsDirect2DRenderTarget* _renderTarget)
			{
				if(_renderTarget)
				{
					oldCaretColor=element->GetCaretColor();
					caretBrush=_renderTarget->CreateDirect2DBrush(oldCaretColor);
				}
			}

			void GuiColorizedTextElementRenderer::DestroyCaretBrush(IWindowsDirect2DRenderTarget* _renderTarget)
			{
				if(_renderTarget)
				{
					if(caretBrush)
					{
						_renderTarget->DestroyDirect2DBrush(oldCaretColor);
						caretBrush=0;
					}
				}
			}

			void GuiColorizedTextElementRenderer::ColorChanged()
			{
				DestroyTextBrush(renderTarget);
				CreateTextBrush(renderTarget);
			}

			void GuiColorizedTextElementRenderer::FontChanged()
			{
				IWindowsDirect2DResourceManager* resourceManager=GetWindowsDirect2DResourceManager();
				if(textFormat)
				{
					resourceManager->DestroyDirect2DTextFormat(oldFont);
					resourceManager->DestroyDirect2DCharMeasurer(oldFont);
				}
				oldFont=element->GetFont();
				textFormat=resourceManager->CreateDirect2DTextFormat(oldFont);
				element->lines.SetCharMeasurer(resourceManager->CreateDirect2DCharMeasurer(oldFont).Obj());
			}

			text::CharMeasurer* GuiColorizedTextElementRenderer::GetCharMeasurer()
			{
				return 0;
			}

			void GuiColorizedTextElementRenderer::InitializeInternal()
			{
				textFormat=0;
				caretBrush=0;
				element->SetCallback(this);
			}

			void GuiColorizedTextElementRenderer::FinalizeInternal()
			{
				DestroyTextBrush(renderTarget);
				DestroyCaretBrush(renderTarget);

				IWindowsDirect2DResourceManager* resourceManager=GetWindowsDirect2DResourceManager();
				if(textFormat)
				{
					resourceManager->DestroyDirect2DTextFormat(oldFont);
					resourceManager->DestroyDirect2DCharMeasurer(oldFont);
				}
			}

			void GuiColorizedTextElementRenderer::RenderTargetChangedInternal(IWindowsDirect2DRenderTarget* oldRenderTarget, IWindowsDirect2DRenderTarget* newRenderTarget)
			{
				DestroyTextBrush(oldRenderTarget);
				DestroyCaretBrush(oldRenderTarget);
				CreateTextBrush(newRenderTarget);
				CreateCaretBrush(newRenderTarget);
				element->lines.SetRenderTarget(newRenderTarget);
			}

			void GuiColorizedTextElementRenderer::Render(Rect bounds)
			{
				if(renderTarget)
				{
					ID2D1RenderTarget* d2dRenderTarget=renderTarget->GetDirect2DRenderTarget();
					Point viewPosition=element->GetViewPosition();
					Rect viewBounds(viewPosition, bounds.GetSize());
					int startRow=element->lines.GetTextPosFromPoint(Point(viewBounds.x1, viewBounds.y1)).row;
					int endRow=element->lines.GetTextPosFromPoint(Point(viewBounds.x2, viewBounds.y2)).row;
					TextPos selectionBegin=element->GetCaretBegin()<element->GetCaretEnd()?element->GetCaretBegin():element->GetCaretEnd();
					TextPos selectionEnd=element->GetCaretBegin()>element->GetCaretEnd()?element->GetCaretBegin():element->GetCaretEnd();
					bool focused=element->GetFocused();

					for(int row=startRow;row<=endRow;row++)
					{
						Rect startRect=element->lines.GetRectFromTextPos(TextPos(row, 0));
						Point startPoint=startRect.LeftTop();
						int startColumn=element->lines.GetTextPosFromPoint(Point(viewBounds.x1, startPoint.y)).column;
						int endColumn=element->lines.GetTextPosFromPoint(Point(viewBounds.x2, startPoint.y)).column;
						text::TextLine& line=element->lines.GetLine(row);

						int x=startColumn==0?0:line.att[startColumn-1].rightOffset;
						for(int column=startColumn; column<=endColumn; column++)
						{
							bool inSelection=false;
							if(selectionBegin.row==selectionEnd.row)
							{
								inSelection=(row==selectionBegin.row && selectionBegin.column<=column && column<selectionEnd.column);
							}
							else if(row==selectionBegin.row)
							{
								inSelection=selectionBegin.column<=column;
							}
							else if(row==selectionEnd.row)
							{
								inSelection=column<selectionEnd.column;
							}
							else
							{
								inSelection=selectionBegin.row<row && row<selectionEnd.row;
							}
							
							bool crlf=column==line.dataLength;
							int colorIndex=crlf?0:line.att[column].colorIndex;
							ColorItemResource& color=
								!inSelection?colors[colorIndex].normal:
								focused?colors[colorIndex].selectedFocused:
								colors[colorIndex].selectedUnfocused;
							int x2=crlf?x+startRect.Height()/2:line.att[column].rightOffset;
							int tx=x-viewPosition.x+bounds.x1;
							int ty=startPoint.y-viewPosition.y+bounds.y1;
							
							if(color.background.a>0)
							{
								d2dRenderTarget->FillRectangle(D2D1::RectF((FLOAT)tx, (FLOAT)ty, (FLOAT)(tx+(x2-x)), (FLOAT)(ty+startRect.Height())), color.backgroundBrush);
							}
							if(!crlf)
							{
								d2dRenderTarget->DrawText(
									&line.text[column],
									1,
									textFormat->textFormat.Obj(),
									D2D1::RectF((FLOAT)tx, (FLOAT)ty, (FLOAT)tx+1, (FLOAT)ty+1),
									color.textBrush,
									D2D1_DRAW_TEXT_OPTIONS_NO_SNAP,
									DWRITE_MEASURING_MODE_GDI_NATURAL
									);
							}
							x=x2;
						}
					}

					if(element->GetCaretVisible() && element->lines.IsAvailable(element->GetCaretEnd()))
					{
						Point caretPoint=element->lines.GetPointFromTextPos(element->GetCaretEnd());
						int height=element->lines.GetRowHeight();
						Point p1(caretPoint.x-viewPosition.x+bounds.x1, caretPoint.y-viewPosition.y+bounds.y1+1);
						Point p2(caretPoint.x-viewPosition.x+bounds.x1, caretPoint.y+height-viewPosition.y+bounds.y1-1);
						d2dRenderTarget->DrawLine(
							D2D1::Point2F((FLOAT)p1.x+0.5f, (FLOAT)p1.y),
							D2D1::Point2F((FLOAT)p2.x+0.5f, (FLOAT)p2.y),
							caretBrush
							);
						d2dRenderTarget->DrawLine(
							D2D1::Point2F((FLOAT)p1.x-0.5f, (FLOAT)p1.y),
							D2D1::Point2F((FLOAT)p2.x-0.5f, (FLOAT)p2.y),
							caretBrush
							);
					}
				}
			}

			void GuiColorizedTextElementRenderer::OnElementStateChanged()
			{
				if(renderTarget)
				{
					Color caretColor=element->GetCaretColor();
					if(oldCaretColor!=caretColor)
					{
						DestroyCaretBrush(renderTarget);
						CreateCaretBrush(renderTarget);
					}
				}
			}
		}
	}
}