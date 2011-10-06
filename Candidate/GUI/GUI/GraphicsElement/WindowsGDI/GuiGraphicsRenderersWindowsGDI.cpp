#include "GuiGraphicsRenderersWindowsGDI.h"

namespace vl
{
	namespace presentation
	{
		namespace elements
		{
			using namespace windows;
			using namespace elements;
			using namespace collections;

/***********************************************************************
GuiSolidBorderElementRenderer
***********************************************************************/

			void GuiSolidBorderElementRenderer::Render(Rect bounds)
			{
				Color color=element->GetColor();
				if(color.a>0)
				{
					// TODO: cache gdi resource
					Ptr<WinBrush> brush=new WinBrush;
					Ptr<WinPen> pen=new WinPen(PS_SOLID, 1, RGB(color.r, color.g, color.b));
					renderTarget->GetDC()->SetBrush(brush);
					renderTarget->GetDC()->SetPen(pen);
					renderTarget->GetDC()->Rectangle(bounds.Left(), bounds.Top(), bounds.Right()-1, bounds.Bottom()-1);
				}
			}

			void GuiSolidBorderElementRenderer::OnElementStateChanged()
			{
			}

/***********************************************************************
GuiSolidBackgroundElementRenderer
***********************************************************************/

			void GuiSolidBackgroundElementRenderer::Render(Rect bounds)
			{
				Color color=element->GetColor();
				if(color.a>0)
				{
					// TODO: cache gdi resource
					Ptr<WinBrush> brush=new WinBrush(RGB(color.r, color.g, color.b));
					renderTarget->GetDC()->SetBrush(brush);
					renderTarget->GetDC()->FillRect(bounds.Left(), bounds.Top(), bounds.Right(), bounds.Bottom());
				}
			}

			void GuiSolidBackgroundElementRenderer::OnElementStateChanged()
			{
			}
		}
	}
}