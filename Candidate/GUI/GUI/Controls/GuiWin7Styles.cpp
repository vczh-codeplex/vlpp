#include "GuiWin7Styles.h"

namespace vl
{
	namespace presentation
	{
		namespace win7
		{

			unsigned char IntToColor(int color)
			{
				return color<0?0:color>255?255:(unsigned char)color;
			}

			Color BlendColor(Color c1, Color c2, int currentPosition, int totalLength)
			{
				return Color(
					(unsigned char)IntToColor((c2.r*currentPosition+c1.r*(totalLength-currentPosition))/totalLength),
					(unsigned char)IntToColor((c2.g*currentPosition+c1.g*(totalLength-currentPosition))/totalLength),
					(unsigned char)IntToColor((c2.b*currentPosition+c1.b*(totalLength-currentPosition))/totalLength),
					(unsigned char)IntToColor((c2.a*currentPosition+c1.a*(totalLength-currentPosition))/totalLength)
					);
			}

/***********************************************************************
Win7ItemColors
***********************************************************************/

			Win7ItemColors Win7ItemColors::Blend(const Win7ItemColors c1, const Win7ItemColors c2, int ratio, int total)
			{
				if(ratio<0) ratio=0;
				else if(ratio>total) ratio=total;

				Win7ItemColors result;
				result.borderColor=BlendColor(c1.borderColor, c2.borderColor, ratio, total);
				result.backgroundColor=BlendColor(c1.backgroundColor, c2.backgroundColor, ratio, total);
				result.g1=BlendColor(c1.g1, c2.g1, ratio, total);
				result.g2=BlendColor(c1.g2, c2.g2, ratio, total);
				result.g3=BlendColor(c1.g3, c2.g3, ratio, total);
				result.g4=BlendColor(c1.g4, c2.g4, ratio, total);
				result.textColor=BlendColor(c1.textColor, c2.textColor, ratio, total);
				return result;
			}

			Win7ItemColors Win7ItemColors::Normal()
			{
				Win7ItemColors colors=
				{
					Color(122, 122, 122),
					Color(251, 251, 251),
					Color(250, 250, 250),
					Color(235, 235, 235),
					Color(221, 221, 221),
					Color(207, 207, 207),
					Color(0, 0, 0),
				};
				return colors;
			}

			Win7ItemColors Win7ItemColors::ItemActive()
			{
				Win7ItemColors colors=
				{
					Color(204, 240, 255),
					Color(220, 244, 254),
					Color(246, 252, 255),
					Color(240, 250, 255),
					Color(225, 245, 254),
					Color(215, 245, 254),
					Color(0, 0, 0),
				};
				return colors;
			}

			Win7ItemColors Win7ItemColors::ItemSelected()
			{
				Win7ItemColors colors=
				{
					Color(128, 190, 247),
					Color(232, 248, 255),
					Color(225, 246, 255),
					Color(204, 239, 254),
					Color(181, 231, 253),
					Color(164, 225, 251),
					Color(0, 0, 0),
				};
				return colors;
			}

			Win7ItemColors Win7ItemColors::ButtonActive()
			{
				Win7ItemColors colors=
				{
					Color(60, 127, 177),
					Color(220, 244, 254),
					Color(246, 252, 255),
					Color(240, 250, 255),
					Color(225, 245, 254),
					Color(215, 245, 254),
					Color(0, 0, 0),
				};
				return colors;
			}

			Win7ItemColors Win7ItemColors::ButtonPressed()
			{
				Win7ItemColors colors=
				{
					Color(44, 98, 139),
					Color(158, 176, 186),
					Color(225, 246, 255),
					Color(204, 239, 254),
					Color(181, 231, 253),
					Color(164, 225, 251),
					Color(0, 0, 0),
				};
				return colors;
			}

			Win7ItemColors Win7ItemColors::Disabled()
			{
				Win7ItemColors colors=
				{
					Color(173, 178, 181),
					Color(252, 252, 252),
					Color(244, 244, 244),
					Color(244, 244, 244),
					Color(244, 244, 244),
					Color(244, 244, 244),
					Color(131, 131, 131),
				};
				return colors;
			}

			int Win7ItemColors::GetColorAnimationLength()
			{
				return 120;
			}

			Color Win7ItemColors::GetSystemWindowColor()
			{
				return Color(240, 240, 240);
			}

/***********************************************************************
Win7ButtonElements
***********************************************************************/

			Win7ButtonElements Win7ButtonElements::Create()
			{
				Win7ButtonElements button;
				{
					button.mainComposition=new GuiBoundsComposition;
					button.mainComposition->SetMinSizeLimitation(GuiGraphicsComposition::LimitToElementAndChildren);
				}
				{
					GuiSolidBackgroundElement* element=GuiSolidBackgroundElement::Create();
					button.backgroundElement=element;

					GuiBoundsComposition* composition=new GuiBoundsComposition;
					button.mainComposition->AddChild(composition);
					composition->SetAlignmentToParent(Margin(1, 1, 1, 1));
					composition->SetOwnedElement(element);
				}
				{
					GuiRoundBorderElement* element=GuiRoundBorderElement::Create();
					button.borderElement=element;
					element->SetRadius(2);

					GuiBoundsComposition* composition=new GuiBoundsComposition;
					button.mainComposition->AddChild(composition);
					composition->SetAlignmentToParent(Margin(0, 0, 0, 0));
					composition->SetOwnedElement(element);
				}
				{
					GuiTableComposition* table=new GuiTableComposition;
					table->SetAlignmentToParent(Margin(2, 2, 2, 2));
					button.mainComposition->AddChild(table);
					table->SetRowsAndColumns(2, 1);
					table->SetRowOption(0, GuiCellOption::PercentageOption(0.5));
					table->SetRowOption(1, GuiCellOption::PercentageOption(0.5));
					table->SetColumnOption(0, GuiCellOption::PercentageOption(1.0));
					{
						GuiGradientBackgroundElement* element=GuiGradientBackgroundElement::Create();
						button.topGradientElement=element;
						element->SetDirection(GuiGradientBackgroundElement::Vertical);

						GuiCellComposition* cell=new GuiCellComposition;
						table->AddChild(cell);
						cell->SetSite(0, 0, 1, 1);
						cell->SetOwnedElement(element);
					}
					{
						GuiGradientBackgroundElement* element=GuiGradientBackgroundElement::Create();
						button.bottomGradientElement=element;
						element->SetDirection(GuiGradientBackgroundElement::Vertical);

						GuiCellComposition* cell=new GuiCellComposition;
						table->AddChild(cell);
						cell->SetSite(1, 0, 1, 1);
						cell->SetOwnedElement(element);
					}
				}
				{
					Win7CreateSolidLabelElement(button.textElement, button.textComposition);
					
					button.textElement->SetAlignments(Alignment::Center, Alignment::Center);
					button.mainComposition->AddChild(button.textComposition);
				}
				return button;
			}

			void Win7ButtonElements::Apply(Win7ItemColors colors)
			{
				borderElement->SetColor(colors.borderColor);
				backgroundElement->SetColor(colors.backgroundColor);
				topGradientElement->SetColors(colors.g1, colors.g2);
				bottomGradientElement->SetColors(colors.g3, colors.g4);
				textElement->SetColor(colors.textColor);
			}

/***********************************************************************
Helpers
***********************************************************************/

			void Win7SetFont(GuiSolidLabelElement* element, GuiBoundsComposition* composition, const FontProperties& fontProperties)
			{
				FontProperties font=GetCurrentController()->GetDefaultFont();
				int margin=font.size/2;
				element->SetFont(font);
				composition->SetMargin(Margin(margin, margin, margin, margin));
			}

			void Win7CreateSolidLabelElement(GuiSolidLabelElement*& element, GuiBoundsComposition*& composition)
			{
				element=GuiSolidLabelElement::Create();
				element->SetAlignments(Alignment::Center, Alignment::Center);

				composition=new GuiBoundsComposition;
				composition->SetOwnedElement(element);
				composition->SetMargin(Margin(0, 0, 0, 0));
				composition->SetMinSizeLimitation(GuiGraphicsComposition::LimitToElement);
				composition->SetAlignmentToParent(Margin(0, 0, 0, 0));
			}

/***********************************************************************
Win7WindowStyle
***********************************************************************/

			Win7WindowStyle::Win7WindowStyle()
			{
				GuiSolidBackgroundElement* element=GuiSolidBackgroundElement::Create();
				element->SetColor(Win7ItemColors::GetSystemWindowColor());
				
				boundsComposition=new GuiBoundsComposition;
				boundsComposition->SetOwnedElement(element);
			}

			Win7WindowStyle::~Win7WindowStyle()
			{
			}

			elements::GuiBoundsComposition* Win7WindowStyle::GetBoundsComposition()
			{
				return boundsComposition;
			}

			elements::GuiGraphicsComposition* Win7WindowStyle::GetContainerComposition()
			{
				return boundsComposition;
			}

			void Win7WindowStyle::SetText(const WString& value)
			{
			}

			void Win7WindowStyle::SetFont(const FontProperties& value)
			{
			}

			void Win7WindowStyle::SetVisuallyEnabled(bool value)
			{
			}

/***********************************************************************
Win7ButtonStyle
***********************************************************************/

			Win7ButtonStyle::TransferringAnimation::TransferringAnimation(Win7ButtonStyle* _style, const Win7ItemColors& begin)
				:GuiTimeBasedAnimation(0)
				,colorBegin(begin)
				,colorEnd(begin)
				,colorCurrent(begin)
				,style(_style)
				,stopped(true)
			{
			}

			void Win7ButtonStyle::TransferringAnimation::Play(int currentPosition, int totalLength)
			{
				colorCurrent=Win7ItemColors::Blend(colorBegin, colorEnd, currentPosition, totalLength);
				style->elements.Apply(colorCurrent);
			}

			void Win7ButtonStyle::TransferringAnimation::Stop()
			{
				stopped=true;
			}

			void Win7ButtonStyle::TransferringAnimation::Transfer(const Win7ItemColors& end)
			{
				if(colorEnd!=end)
				{
					Restart(Win7ItemColors::GetColorAnimationLength());
					if(stopped)
					{
						colorBegin=colorEnd;
						colorEnd=end;
						style->GetBoundsComposition()->GetRelatedGraphicsHost()->GetAnimationManager()->AddAnimation(style->transferringAnimation);
						stopped=false;
					}
					else
					{
						colorBegin=colorCurrent;
						colorEnd=end;
					}
				}
			}

			void Win7ButtonStyle::TransferInternal(GuiButton::ControlStyle value, bool enabled)
			{
				if(enabled)
				{
					switch(value)
					{
					case GuiButton::Normal:
						transferringAnimation->Transfer(Win7ItemColors::Normal());
						break;
					case GuiButton::Active:
						transferringAnimation->Transfer(Win7ItemColors::ButtonActive());
						break;
					case GuiButton::Pressed:
						transferringAnimation->Transfer(Win7ItemColors::ButtonPressed());
						break;
					}
				}
				else
				{
					transferringAnimation->Transfer(Win7ItemColors::Disabled());
				}
			}

			Win7ButtonStyle::Win7ButtonStyle()
				:controlStyle(GuiButton::Normal)
				,isVisuallyEnabled(true)
			{
				Win7ItemColors initialColor=Win7ItemColors::Normal();
				elements=Win7ButtonElements::Create();
				elements.Apply(initialColor);
				transferringAnimation=new TransferringAnimation(this, initialColor);
			}

			Win7ButtonStyle::~Win7ButtonStyle()
			{
			}

			elements::GuiBoundsComposition* Win7ButtonStyle::GetBoundsComposition()
			{
				return elements.mainComposition;
			}

			elements::GuiGraphicsComposition* Win7ButtonStyle::GetContainerComposition()
			{
				return elements.mainComposition;
			}

			void Win7ButtonStyle::SetText(const WString& value)
			{
				elements.textElement->SetText(value);
			}

			void Win7ButtonStyle::SetFont(const FontProperties& value)
			{
				Win7SetFont(elements.textElement, elements.textComposition, value);
			}

			void Win7ButtonStyle::SetVisuallyEnabled(bool value)
			{
				if(isVisuallyEnabled!=value)
				{
					isVisuallyEnabled=value;
					TransferInternal(controlStyle, isVisuallyEnabled);
				}
			}

			void Win7ButtonStyle::Transfer(GuiButton::ControlStyle value)
			{
				if(controlStyle!=value)
				{
					controlStyle=value;
					TransferInternal(controlStyle, isVisuallyEnabled);
				}
			}

/***********************************************************************
Win7GroupBoxStyle
***********************************************************************/

			Win7GroupBoxStyle::TransferringAnimation::TransferringAnimation(Win7GroupBoxStyle* _style, const Color& begin)
				:GuiTimeBasedAnimation(0)
				,colorBegin(begin)
				,colorEnd(begin)
				,colorCurrent(begin)
				,style(_style)
				,stopped(true)
			{
			}

			void Win7GroupBoxStyle::TransferringAnimation::Play(int currentPosition, int totalLength)
			{
				colorCurrent=BlendColor(colorBegin, colorEnd, currentPosition, totalLength);
				style->textElement->SetColor(colorCurrent);
			}

			void Win7GroupBoxStyle::TransferringAnimation::Stop()
			{
				stopped=true;
			}

			void Win7GroupBoxStyle::TransferringAnimation::Transfer(const Color& end)
			{
				if(colorEnd!=end)
				{
					Restart(Win7ItemColors::GetColorAnimationLength());
					if(stopped)
					{
						colorBegin=colorEnd;
						colorEnd=end;
						style->GetBoundsComposition()->GetRelatedGraphicsHost()->GetAnimationManager()->AddAnimation(style->transferringAnimation);
						stopped=false;
					}
					else
					{
						colorBegin=colorCurrent;
						colorEnd=end;
					}
				}
			}

			void Win7GroupBoxStyle::SetMargins(int fontSize)
			{
				fontSize+=4;
				int half=fontSize/2;
				sinkBorderComposition->SetAlignmentToParent(Margin(0, half, 1, 1));
				raisedBorderComposition->SetAlignmentToParent(Margin(1, half+1, 0, 0));
				containerComposition->SetAlignmentToParent(Margin(2, fontSize, 2, 2));
				textBackgroundComposition->SetAlignmentToParent(Margin(half, 2, -1, -1));
			}

			Win7GroupBoxStyle::Win7GroupBoxStyle()
			{
				boundsComposition=new GuiBoundsComposition;
				{
					GuiSolidBackgroundElement* element=GuiSolidBackgroundElement::Create();
					element->SetColor(Win7ItemColors::GetSystemWindowColor());

					boundsComposition->SetOwnedElement(element);
				}

				sinkBorderComposition=new GuiBoundsComposition;
				{
					GuiRoundBorderElement* element=GuiRoundBorderElement::Create();
					element->SetRadius(3);
					element->SetColor(Color(213, 223, 229));

					sinkBorderComposition->SetOwnedElement(element);
					boundsComposition->AddChild(sinkBorderComposition);
				}

				raisedBorderComposition=new GuiBoundsComposition;
				{
					GuiRoundBorderElement* element=GuiRoundBorderElement::Create();
					element->SetRadius(3);
					element->SetColor(Color(255, 255, 255));

					raisedBorderComposition->SetOwnedElement(element);
					boundsComposition->AddChild(raisedBorderComposition);
				}

				textBackgroundComposition=new GuiBoundsComposition;
				{
					GuiSolidBackgroundElement* element=GuiSolidBackgroundElement::Create();
					element->SetColor(Win7ItemColors::GetSystemWindowColor());

					textBackgroundComposition->SetOwnedElement(element);
					textBackgroundComposition->SetMinSizeLimitation(GuiGraphicsComposition::LimitToElementAndChildren);
					boundsComposition->AddChild(textBackgroundComposition);
				}

				textComposition=new GuiBoundsComposition;
				{
					GuiSolidLabelElement* element=GuiSolidLabelElement::Create();
					element->SetColor(Win7ItemColors::Normal().textColor);
					textElement=element;

					textComposition->SetOwnedElement(element);
					textComposition->SetAlignmentToParent(Margin(0, 0, 0, 0));
					textComposition->SetMinSizeLimitation(GuiGraphicsComposition::LimitToElement);
					textBackgroundComposition->AddChild(textComposition);
				}

				containerComposition=new GuiBoundsComposition;
				{
					boundsComposition->AddChild(containerComposition);
				}

				SetMargins(0);
				transferringAnimation=new TransferringAnimation(this, Win7ItemColors::Normal().textColor);
			}

			Win7GroupBoxStyle::~Win7GroupBoxStyle()
			{
			}

			elements::GuiBoundsComposition* Win7GroupBoxStyle::GetBoundsComposition()
			{
				return boundsComposition;
			}

			elements::GuiGraphicsComposition* Win7GroupBoxStyle::GetContainerComposition()
			{
				return containerComposition;
			}

			void Win7GroupBoxStyle::SetText(const WString& value)
			{
				textElement->SetText(value);
			}

			void Win7GroupBoxStyle::SetFont(const FontProperties& value)
			{
				textElement->SetFont(value);
				SetMargins(value.size);
			}

			void Win7GroupBoxStyle::SetVisuallyEnabled(bool value)
			{
				if(value)
				{
					transferringAnimation->Transfer(Win7ItemColors::Normal().textColor);
				}
				else
				{
					transferringAnimation->Transfer(Win7ItemColors::Disabled().textColor);
				}
			}
		}
	}
}