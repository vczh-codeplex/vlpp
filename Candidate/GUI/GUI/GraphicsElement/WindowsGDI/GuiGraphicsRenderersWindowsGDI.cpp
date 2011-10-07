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
					renderTarget->GetDC()->DrawString(bounds.Left(), bounds.Top(), element->GetText());
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