#ifdef GUI_GRAPHICS_RENDERER_GDI

#include "GuiGraphicsRenderersWindowsGDI.h"
#include "..\..\..\..\..\Library\Collections\OperationCopyFrom.h"

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

			void GuiSolidBorderElementRenderer::FinalizeInternal()
			{
				IWindowsGDIResourceManager* resourceManager=GetWindowsGDIResourceManager();
				resourceManager->DestroyGdiPen(oldColor);
				resourceManager->DestroyGdiBrush(Color(0, 0, 0, 0));
			}

			void GuiSolidBorderElementRenderer::RenderTargetChangedInternal(IWindowsGDIRenderTarget* oldRenderTarget, IWindowsGDIRenderTarget* newRenderTarget)
			{
			}

			void GuiSolidBorderElementRenderer::Render(Rect bounds)
			{
				if(oldColor.a>0)
				{
					renderTarget->GetDC()->SetBrush(brush);
					renderTarget->GetDC()->SetPen(pen);
					switch(element->GetShape())
					{
					case ElementShape::Rectangle:
						renderTarget->GetDC()->Rectangle(bounds.Left(), bounds.Top(), bounds.Right()-1, bounds.Bottom()-1);
						break;
					case ElementShape::Ellipse:
						renderTarget->GetDC()->Ellipse(bounds.Left(), bounds.Top(), bounds.Right()-1, bounds.Bottom()-1);
						break;
					}
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

			void GuiRoundBorderElementRenderer::FinalizeInternal()
			{
				IWindowsGDIResourceManager* resourceManager=GetWindowsGDIResourceManager();
				resourceManager->DestroyGdiPen(oldColor);
				resourceManager->DestroyGdiBrush(Color(0, 0, 0, 0));
			}

			void GuiRoundBorderElementRenderer::RenderTargetChangedInternal(IWindowsGDIRenderTarget* oldRenderTarget, IWindowsGDIRenderTarget* newRenderTarget)
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
Gui3DBorderElementRenderer
***********************************************************************/

			void Gui3DBorderElementRenderer::InitializeInternal()
			{
				IWindowsGDIResourceManager* resourceManager=GetWindowsGDIResourceManager();
				oldColor1=element->GetColor1();
				oldColor2=element->GetColor2();
				pen1=resourceManager->CreateGdiPen(oldColor1);
				pen2=resourceManager->CreateGdiPen(oldColor2);
			}

			void Gui3DBorderElementRenderer::FinalizeInternal()
			{
				IWindowsGDIResourceManager* resourceManager=GetWindowsGDIResourceManager();
				resourceManager->DestroyGdiPen(oldColor1);
				resourceManager->DestroyGdiPen(oldColor2);
			}

			void Gui3DBorderElementRenderer::RenderTargetChangedInternal(IWindowsGDIRenderTarget* oldRenderTarget, IWindowsGDIRenderTarget* newRenderTarget)
			{
			}

			void Gui3DBorderElementRenderer::Render(Rect bounds)
			{
				if(oldColor1.a>0)
				{
					renderTarget->GetDC()->SetPen(pen1);
					renderTarget->GetDC()->MoveTo(bounds.x1, bounds.y1);
					renderTarget->GetDC()->LineTo(bounds.x2, bounds.y1);
					renderTarget->GetDC()->MoveTo(bounds.x1, bounds.y1);
					renderTarget->GetDC()->LineTo(bounds.x1, bounds.y2);
				}
				if(oldColor2.a>0)
				{
					renderTarget->GetDC()->SetPen(pen2);
					renderTarget->GetDC()->MoveTo(bounds.x2-1, bounds.y2-1);
					renderTarget->GetDC()->LineTo(bounds.x1, bounds.y2-1);
					renderTarget->GetDC()->MoveTo(bounds.x2-1, bounds.y2-1);
					renderTarget->GetDC()->LineTo(bounds.x2-1, bounds.y1);
				}
			}

			void Gui3DBorderElementRenderer::OnElementStateChanged()
			{
				Color color1=element->GetColor1();
				if(oldColor1!=color1)
				{
					IWindowsGDIResourceManager* resourceManager=GetWindowsGDIResourceManager();
					resourceManager->DestroyGdiPen(oldColor1);
					oldColor1=color1;
					pen1=resourceManager->CreateGdiPen(oldColor1);
				}

				Color color2=element->GetColor2();
				if(oldColor2!=color2)
				{
					IWindowsGDIResourceManager* resourceManager=GetWindowsGDIResourceManager();
					resourceManager->DestroyGdiPen(oldColor2);
					oldColor2=color2;
					pen2=resourceManager->CreateGdiPen(oldColor2);
				}
			}

/***********************************************************************
GuiSolidBackgroundElementRenderer
***********************************************************************/

			void GuiSolidBackgroundElementRenderer::InitializeInternal()
			{
				IWindowsGDIResourceManager* resourceManager=GetWindowsGDIResourceManager();
				oldColor=element->GetColor();
				pen=resourceManager->CreateGdiPen(oldColor);
				brush=resourceManager->CreateGdiBrush(oldColor);
			}

			void GuiSolidBackgroundElementRenderer::FinalizeInternal()
			{
				IWindowsGDIResourceManager* resourceManager=GetWindowsGDIResourceManager();
				resourceManager->DestroyGdiPen(oldColor);
				resourceManager->DestroyGdiBrush(oldColor);
			}

			void GuiSolidBackgroundElementRenderer::RenderTargetChangedInternal(IWindowsGDIRenderTarget* oldRenderTarget, IWindowsGDIRenderTarget* newRenderTarget)
			{
			}

			void GuiSolidBackgroundElementRenderer::Render(Rect bounds)
			{
				if(oldColor.a>0)
				{
					renderTarget->GetDC()->SetPen(pen);
					renderTarget->GetDC()->SetBrush(brush);
					switch(element->GetShape())
					{
					case ElementShape::Rectangle:
						renderTarget->GetDC()->FillRect(bounds.Left(), bounds.Top(), bounds.Right(), bounds.Bottom());
						break;
					case ElementShape::Ellipse:
						renderTarget->GetDC()->Ellipse(bounds.Left(), bounds.Top(), bounds.Right()-1, bounds.Bottom()-1);
						break;
					}
				}
			}

			void GuiSolidBackgroundElementRenderer::OnElementStateChanged()
			{
				Color color=element->GetColor();
				if(oldColor!=color)
				{
					IWindowsGDIResourceManager* resourceManager=GetWindowsGDIResourceManager();
					resourceManager->DestroyGdiPen(oldColor);
					resourceManager->DestroyGdiBrush(oldColor);
					oldColor=color;
					pen=resourceManager->CreateGdiPen(oldColor);
					brush=resourceManager->CreateGdiBrush(oldColor);
				}
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

			void GuiGradientBackgroundElementRenderer::RenderTargetChangedInternal(IWindowsGDIRenderTarget* oldRenderTarget, IWindowsGDIRenderTarget* newRenderTarget)
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
					
					switch(element->GetShape())
					{
					case ElementShape::Rectangle:
						{
							renderTarget->GetDC()->GradientTriangle(vertices, 6, triangles, 2);
						}
						break;
					case ElementShape::Ellipse:
						{
							Ptr<WinRegion> ellipseRegion=new WinRegion(bounds.x1, bounds.y1, bounds.x2+1, bounds.y2+1, false);
							Ptr<WinRegion> oldRegion=renderTarget->GetDC()->GetClipRegion();
							Ptr<WinRegion> newRegion=new WinRegion(oldRegion, ellipseRegion, RGN_AND);
							renderTarget->GetDC()->ClipRegion(newRegion);
							renderTarget->GetDC()->GradientTriangle(vertices, 6, triangles, 2);
							renderTarget->GetDC()->ClipRegion(oldRegion);
						}
						break;
					}
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
					const WString& text=element->GetText();
					SIZE size=text.Length()==0
						?renderTarget->GetDC()->MeasureBuffer(L" ")
						:renderTarget->GetDC()->MeasureString(text)
						;
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

			void GuiSolidLabelElementRenderer::FinalizeInternal()
			{
				IWindowsGDIResourceManager* resourceManager=GetWindowsGDIResourceManager();
				resourceManager->DestroyGdiFont(oldFont);
			}

			void GuiSolidLabelElementRenderer::RenderTargetChangedInternal(IWindowsGDIRenderTarget* oldRenderTarget, IWindowsGDIRenderTarget* newRenderTarget)
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

/***********************************************************************
GuiColorizedTextElementRenderer
***********************************************************************/

			void GuiColorizedTextElementRenderer::DestroyColors()
			{
				IWindowsGDIResourceManager* resourceManager=GetWindowsGDIResourceManager();
				for(int i=0;i<colors.Count();i++)
				{
					resourceManager->DestroyGdiBrush(colors[i].normal.background);
					resourceManager->DestroyGdiBrush(colors[i].selectedFocused.background);
					resourceManager->DestroyGdiBrush(colors[i].selectedUnfocused.background);
				}
			}

			void GuiColorizedTextElementRenderer::ColorChanged()
			{
				IWindowsGDIResourceManager* resourceManager=GetWindowsGDIResourceManager();
				ColorArray newColors;
				newColors.Resize(element->GetColors().Count());
				for(int i=0;i<newColors.Count();i++)
				{
					text::ColorEntry entry=element->GetColors()[i];
					ColorEntryResource newEntry;

					newEntry.normal.text=entry.normal.text;
					newEntry.normal.background=entry.normal.background;
					newEntry.normal.backgroundBrush=resourceManager->CreateGdiBrush(newEntry.normal.background);
					newEntry.selectedFocused.text=entry.selectedFocused.text;
					newEntry.selectedFocused.background=entry.selectedFocused.background;
					newEntry.selectedFocused.backgroundBrush=resourceManager->CreateGdiBrush(newEntry.selectedFocused.background);
					newEntry.selectedUnfocused.text=entry.selectedUnfocused.text;
					newEntry.selectedUnfocused.background=entry.selectedUnfocused.background;
					newEntry.selectedUnfocused.backgroundBrush=resourceManager->CreateGdiBrush(newEntry.selectedUnfocused.background);
					newColors[i]=newEntry;
				}

				DestroyColors();
				CopyFrom(colors.Wrap(), newColors.Wrap());
			}

			void GuiColorizedTextElementRenderer::FontChanged()
			{
				IWindowsGDIResourceManager* resourceManager=GetWindowsGDIResourceManager();
				if(font)
				{
					element->lines.SetCharMeasurer(0);
					resourceManager->DestroyGdiFont(oldFont);
					resourceManager->DestroyCharMeasurer(oldFont);
					font=0;
				}
				oldFont=element->GetFont();
				font=resourceManager->CreateGdiFont(oldFont);
				element->lines.SetCharMeasurer(resourceManager->CreateCharMeasurer(oldFont).Obj());
			}

			void GuiColorizedTextElementRenderer::InitializeInternal()
			{
				IWindowsGDIResourceManager* resourceManager=GetWindowsGDIResourceManager();
				element->SetCallback(this);
				oldCaretColor=element->GetCaretColor();
				caretPen=resourceManager->CreateGdiPen(oldCaretColor);
			}

			void GuiColorizedTextElementRenderer::FinalizeInternal()
			{
				IWindowsGDIResourceManager* resourceManager=GetWindowsGDIResourceManager();
				if(font)
				{
					resourceManager->DestroyGdiFont(oldFont);
					resourceManager->DestroyCharMeasurer(oldFont);
				}
				resourceManager->DestroyGdiPen(oldCaretColor);
				DestroyColors();
			}

			void GuiColorizedTextElementRenderer::RenderTargetChangedInternal(IWindowsGDIRenderTarget* oldRenderTarget, IWindowsGDIRenderTarget* newRenderTarget)
			{
				element->lines.SetRenderTarget(newRenderTarget);
			}

			void GuiColorizedTextElementRenderer::Render(Rect bounds)
			{
				if(renderTarget)
				{
					WinDC* dc=renderTarget->GetDC();
					dc->SetFont(font);

					Point viewPosition=element->GetViewPosition();
					Rect viewBounds(viewPosition, bounds.GetSize());
					int startRow=element->lines.GetTextPosFromPoint(Point(viewBounds.x1, viewBounds.y1)).row;
					int endRow=element->lines.GetTextPosFromPoint(Point(viewBounds.x2, viewBounds.y2)).row;
					TextPos selectionBegin=element->GetCaretBegin()<element->GetCaretEnd()?element->GetCaretBegin():element->GetCaretEnd();
					TextPos selectionEnd=element->GetCaretBegin()>element->GetCaretEnd()?element->GetCaretBegin():element->GetCaretEnd();
					bool focused=element->GetFocused();
					Ptr<windows::WinBrush> lastBrush=0;

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
							
							if(color.background.a)
							{
								if(lastBrush!=color.backgroundBrush)
								{
									lastBrush=color.backgroundBrush;
									dc->SetBrush(lastBrush);
								}
								dc->FillRect(tx, ty, tx+(x2-x), ty+startRect.Height());
							}
							if(!crlf)
							{
								if(color.text.a)
								{
									dc->SetTextColor(RGB(color.text.r, color.text.g, color.text.b));
									dc->DrawBuffer(tx, ty, &line.text[column], 1);
								}
							}
							x=x2;
						}
					}

					if(element->GetCaretVisible() && element->lines.IsAvailable(element->GetCaretEnd()))
					{
						Point caretPoint=element->lines.GetPointFromTextPos(element->GetCaretEnd());
						int height=element->lines.GetRowHeight();
						dc->SetPen(caretPen);
						dc->MoveTo(caretPoint.x-viewPosition.x+bounds.x1, caretPoint.y-viewPosition.y+bounds.y1+1);
						dc->LineTo(caretPoint.x-viewPosition.x+bounds.x1, caretPoint.y+height-viewPosition.y+bounds.y1-1);
						dc->MoveTo(caretPoint.x-1-viewPosition.x+bounds.x1, caretPoint.y-viewPosition.y+bounds.y1+1);
						dc->LineTo(caretPoint.x-1-viewPosition.x+bounds.x1, caretPoint.y+height-viewPosition.y+bounds.y1-1);
					}
				}
			}

			void GuiColorizedTextElementRenderer::OnElementStateChanged()
			{
				Color caretColor=element->GetCaretColor();
				if(oldCaretColor!=caretColor)
				{
					IWindowsGDIResourceManager* resourceManager=GetWindowsGDIResourceManager();
					resourceManager->DestroyGdiPen(oldCaretColor);
					oldCaretColor=caretColor;
					caretPen=resourceManager->CreateGdiPen(oldCaretColor);
				}
			}
		}
	}
}

#endif