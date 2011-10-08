#include "GuiGraphicsRenderersWindowsGDI.h"

namespace vl
{
	namespace presentation
	{
		namespace elements_windows_gdi
		{
			using namespace windows;
			using namespace collections;

/***********************************************************************
GuiSolidBorderElementRenderer
***********************************************************************/

			void GuiSolidBorderElementRenderer::InitializeInternal()
			{
				IWindowsGDIResourceManager* resourceManager=GetWindowsGDIResourceManager();
				oldColor=element->GetColor();
				pen=resourceManager->CreateGdiPen(oldColor);
				brush=resourceManager->CreateGdiBrush(Color(0, 0, 0, 0));
			}

			void GuiSolidBorderElementRenderer::RenderTargetChangedInternal()
			{
			}

			void GuiSolidBorderElementRenderer::Render(Rect bounds)
			{
				if(oldColor.a>0)
				{
					renderTarget->GetDC()->SetBrush(brush);
					renderTarget->GetDC()->SetPen(pen);
					renderTarget->GetDC()->Rectangle(bounds.Left(), bounds.Top(), bounds.Right()-1, bounds.Bottom()-1);
				}
			}

			void GuiSolidBorderElementRenderer::OnElementStateChanged()
			{
				Color color=element->GetColor();
				if(oldColor!=color)
				{
					IWindowsGDIResourceManager* resourceManager=GetWindowsGDIResourceManager();
					resourceManager->DestroyGdiPen(oldColor);
					oldColor=color;
					pen=resourceManager->CreateGdiPen(oldColor);
				}
			}

/***********************************************************************
GuiRoundBorderElementRenderer
***********************************************************************/

			void GuiRoundBorderElementRenderer::InitializeInternal()
			{
				IWindowsGDIResourceManager* resourceManager=GetWindowsGDIResourceManager();
				oldColor=element->GetColor();
				pen=resourceManager->CreateGdiPen(oldColor);
				brush=resourceManager->CreateGdiBrush(Color(0, 0, 0, 0));
			}

			void GuiRoundBorderElementRenderer::RenderTargetChangedInternal()
			{
			}

			void GuiRoundBorderElementRenderer::Render(Rect bounds)
			{
				if(oldColor.a>0)
				{
					int ellipse=element->GetRadius()*2;
					renderTarget->GetDC()->SetBrush(brush);
					renderTarget->GetDC()->SetPen(pen);
					renderTarget->GetDC()->RoundRect(bounds.Left(), bounds.Top(), bounds.Right()-1, bounds.Bottom()-1, ellipse, ellipse);
				}
			}

			void GuiRoundBorderElementRenderer::OnElementStateChanged()
			{
				Color color=element->GetColor();
				if(oldColor!=color)
				{
					IWindowsGDIResourceManager* resourceManager=GetWindowsGDIResourceManager();
					resourceManager->DestroyGdiPen(oldColor);
					oldColor=color;
					pen=resourceManager->CreateGdiPen(oldColor);
				}
			}

/***********************************************************************
GuiSolidBackgroundElementRenderer
***********************************************************************/

			void GuiSolidBackgroundElementRenderer::InitializeInternal()
			{
				IWindowsGDIResourceManager* resourceManager=GetWindowsGDIResourceManager();
				oldColor=element->GetColor();
				brush=resourceManager->CreateGdiBrush(oldColor);
			}

			void GuiSolidBackgroundElementRenderer::RenderTargetChangedInternal()
			{
			}

			void GuiSolidBackgroundElementRenderer::Render(Rect bounds)
			{
				if(oldColor.a>0)
				{
					renderTarget->GetDC()->SetBrush(brush);
					renderTarget->GetDC()->FillRect(bounds.Left(), bounds.Top(), bounds.Right(), bounds.Bottom());
				}
			}

			void GuiSolidBackgroundElementRenderer::OnElementStateChanged()
			{
				Color color=element->GetColor();
				if(oldColor!=color)
				{
					IWindowsGDIResourceManager* resourceManager=GetWindowsGDIResourceManager();
					resourceManager->DestroyGdiBrush(oldColor);
					oldColor=color;
					brush=resourceManager->CreateGdiBrush(oldColor);
				}
			}

/***********************************************************************
GuiGradientBackgroundElementRenderer
***********************************************************************/

			void GuiGradientBackgroundElementRenderer::InitializeInternal()
			{
			}

			void GuiGradientBackgroundElementRenderer::RenderTargetChangedInternal()
			{
			}

			void GuiGradientBackgroundElementRenderer::Render(Rect bounds)
			{
				Color color1=element->GetColor1();
				Color color2=element->GetColor2();
				if(color1.a>0 || color2.a>0)
				{
					TRIVERTEX vertices[4];
					GRADIENT_TRIANGLE triangles[2];

					vertices[0].x=bounds.x1;
					vertices[0].y=bounds.y1;
					vertices[1].x=bounds.x1;
					vertices[1].y=bounds.y2;
					vertices[2].x=bounds.x2;
					vertices[2].y=bounds.y2;
					vertices[3].x=bounds.x2;
					vertices[3].y=bounds.y1;

					triangles[0].Vertex1=0;
					triangles[0].Vertex2=1;
					triangles[0].Vertex3=2;
					triangles[1].Vertex1=0;
					triangles[1].Vertex2=2;
					triangles[1].Vertex3=3;

					if(element->GetDirection()==GuiGradientBackgroundElement::Horizontal)
					{
						vertices[0].Red=color1.r<<8;
						vertices[0].Green=color1.g<<8;
						vertices[0].Blue=color1.b<<8;
						vertices[0].Alpha=0xFF00;
						
						vertices[1].Red=color1.r<<8;
						vertices[1].Green=color1.g<<8;
						vertices[1].Blue=color1.b<<8;
						vertices[1].Alpha=0xFF00;
						
						vertices[2].Red=color2.r<<8;
						vertices[2].Green=color2.g<<8;
						vertices[2].Blue=color2.b<<8;
						vertices[2].Alpha=0xFF00;
						
						vertices[3].Red=color2.r<<8;
						vertices[3].Green=color2.g<<8;
						vertices[3].Blue=color2.b<<8;
						vertices[3].Alpha=0xFF00;
					}
					else
					{
						vertices[0].Red=color1.r<<8;
						vertices[0].Green=color1.g<<8;
						vertices[0].Blue=color1.b<<8;
						vertices[0].Alpha=0xFF00;
						
						vertices[1].Red=color2.r<<8;
						vertices[1].Green=color2.g<<8;
						vertices[1].Blue=color2.b<<8;
						vertices[1].Alpha=0xFF00;
						
						vertices[2].Red=color2.r<<8;
						vertices[2].Green=color2.g<<8;
						vertices[2].Blue=color2.b<<8;
						vertices[2].Alpha=0xFF00;
						
						vertices[3].Red=color1.r<<8;
						vertices[3].Green=color1.g<<8;
						vertices[3].Blue=color1.b<<8;
						vertices[3].Alpha=0xFF00;
					}

					renderTarget->GetDC()->GradientTriangle(vertices, 6, triangles, 2);
				}
			}

			void GuiGradientBackgroundElementRenderer::OnElementStateChanged()
			{
			}

/***********************************************************************
GuiSolidLabelElementRenderer
***********************************************************************/

			void GuiSolidLabelElementRenderer::UpdateMinSize()
			{
				if(renderTarget)
				{
					renderTarget->GetDC()->SetFont(font);
					SIZE size=renderTarget->GetDC()->MeasureString(element->GetText());
					minSize=Size(size.cx, size.cy);
				}
				else
				{
					minSize=Size();
				}
			}

			void GuiSolidLabelElementRenderer::InitializeInternal()
			{
				IWindowsGDIResourceManager* resourceManager=GetWindowsGDIResourceManager();
				oldFont=element->GetFont();
				font=resourceManager->CreateGdiFont(oldFont);
			}

			void GuiSolidLabelElementRenderer::RenderTargetChangedInternal()
			{
				UpdateMinSize();
			}

			void GuiSolidLabelElementRenderer::Render(Rect bounds)
			{
				Color color=element->GetColor();
				if(color.a>0)
				{
					renderTarget->GetDC()->SetFont(font);
					renderTarget->GetDC()->SetTextColor(RGB(color.r, color.g, color.b));
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
					renderTarget->GetDC()->DrawString(x, y, element->GetText());
				}
			}

			void GuiSolidLabelElementRenderer::OnElementStateChanged()
			{
				FontProperties fontProperties=element->GetFont();
				if(oldFont!=fontProperties)
				{
					IWindowsGDIResourceManager* resourceManager=GetWindowsGDIResourceManager();
					resourceManager->DestroyGdiFont(oldFont);
					oldFont=fontProperties;
					font=resourceManager->CreateGdiFont(oldFont);
				}
				UpdateMinSize();
			}
		}
	}
}