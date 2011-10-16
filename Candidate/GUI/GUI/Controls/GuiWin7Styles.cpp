#include "GuiWin7Styles.h"

namespace vl
{
	namespace presentation
	{
		namespace win7
		{
			using namespace elements;
			using namespace controls;

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

			Win7ButtonColors Win7ButtonColors::Blend(const Win7ButtonColors c1, const Win7ButtonColors c2, int ratio, int total)
			{
				if(ratio<0) ratio=0;
				else if(ratio>total) ratio=total;

				Win7ButtonColors result;
				result.borderColor=BlendColor(c1.borderColor, c2.borderColor, ratio, total);
				result.backgroundColor=BlendColor(c1.backgroundColor, c2.backgroundColor, ratio, total);
				result.g1=BlendColor(c1.g1, c2.g1, ratio, total);
				result.g2=BlendColor(c1.g2, c2.g2, ratio, total);
				result.g3=BlendColor(c1.g3, c2.g3, ratio, total);
				result.g4=BlendColor(c1.g4, c2.g4, ratio, total);
				result.textColor=BlendColor(c1.textColor, c2.textColor, ratio, total);
				result.bulletLight=BlendColor(c1.bulletLight, c2.bulletLight, ratio, total);
				result.bulletDark=BlendColor(c1.bulletDark, c2.bulletDark, ratio, total);
				return result;
			}

			Win7ButtonColors Win7ButtonColors::Normal()
			{
				Win7ButtonColors colors=
				{
					Color(122, 122, 122),
					Color(251, 251, 251),
					Color(250, 250, 250),
					Color(235, 235, 235),
					Color(221, 221, 221),
					Color(207, 207, 207),
					Win7GetSystemTextColor(true),
				};
				return colors;
			}

			Win7ButtonColors Win7ButtonColors::ItemActive()
			{
				Win7ButtonColors colors=
				{
					Color(204, 240, 255),
					Color(220, 244, 254),
					Color(246, 252, 255),
					Color(240, 250, 255),
					Color(225, 245, 254),
					Color(215, 245, 254),
					Win7GetSystemTextColor(true),
				};
				return colors;
			}

			Win7ButtonColors Win7ButtonColors::ItemSelected()
			{
				Win7ButtonColors colors=
				{
					Color(128, 190, 247),
					Color(232, 248, 255),
					Color(225, 246, 255),
					Color(204, 239, 254),
					Color(181, 231, 253),
					Color(164, 225, 251),
					Win7GetSystemTextColor(true),
				};
				return colors;
			}

			Win7ButtonColors Win7ButtonColors::ButtonActive()
			{
				Win7ButtonColors colors=
				{
					Color(60, 127, 177),
					Color(220, 244, 254),
					Color(246, 252, 255),
					Color(240, 250, 255),
					Color(225, 245, 254),
					Color(215, 245, 254),
					Win7GetSystemTextColor(true),
				};
				return colors;
			}

			Win7ButtonColors Win7ButtonColors::ButtonPressed()
			{
				Win7ButtonColors colors=
				{
					Color(44, 98, 139),
					Color(158, 176, 186),
					Color(225, 246, 255),
					Color(204, 239, 254),
					Color(181, 231, 253),
					Color(164, 225, 251),
					Win7GetSystemTextColor(true),
				};
				return colors;
			}

			Win7ButtonColors Win7ButtonColors::Disabled()
			{
				Win7ButtonColors colors=
				{
					Color(173, 178, 181),
					Color(252, 252, 252),
					Color(244, 244, 244),
					Color(244, 244, 244),
					Color(244, 244, 244),
					Color(244, 244, 244),
					Win7GetSystemTextColor(false),
				};
				return colors;
			}

			Win7ButtonColors Win7ButtonColors::CheckedNormal(bool selected)
			{
				Win7ButtonColors colors=
				{
					Color(142, 143, 143),
					Color(244, 244, 244),
					Color(174, 179, 185),
					Color(233, 233, 234),
					Color(203, 207, 213),
					Color(235, 235, 236),
					Win7GetSystemTextColor(true),
					Color(76, 97, 152),
					Color(76, 97, 152),
				};
				if(!selected)
				{
					colors.bulletLight.a=0;
					colors.bulletDark.a=0;
				}
				return colors;
			}

			Win7ButtonColors Win7ButtonColors::CheckedActive(bool selected)
			{
				Win7ButtonColors colors=
				{
					Color(85, 134, 163),
					Color(222, 249, 250),
					Color(121, 198, 249),
					Color(207, 236, 253),
					Color(177, 233, 253),
					Color(231, 247, 254),
					Win7GetSystemTextColor(true),
					Color(4, 34, 113),
					Color(4, 34, 113),
				};
				if(!selected)
				{
					colors.bulletLight.a=0;
					colors.bulletDark.a=0;
				}
				return colors;
			}

			Win7ButtonColors Win7ButtonColors::CheckedPressed(bool selected)
			{
				Win7ButtonColors colors=
				{
					Color(44, 98, 139),
					Color(194, 228, 254),
					Color(94, 182, 247),
					Color(193, 230, 252),
					Color(157, 213, 252),
					Color(224, 244, 254),
					Win7GetSystemTextColor(true),
					Color(63, 93, 153),
					Color(63, 93, 153),
				};
				if(!selected)
				{
					colors.bulletLight.a=0;
					colors.bulletDark.a=0;
				}
				return colors;
			}

			Win7ButtonColors Win7ButtonColors::CheckedDisabled(bool selected)
			{
				Win7ButtonColors colors=
				{
					Color(177, 177, 177),
					Color(240, 240, 240),
					Color(240, 240, 240),
					Color(240, 240, 240),
					Color(240, 240, 240),
					Color(240, 240, 240),
					Win7GetSystemTextColor(false),
					Color(177, 177, 177),
					Color(177, 177, 177),
				};
				if(!selected)
				{
					colors.bulletLight.a=0;
					colors.bulletDark.a=0;
				}
				return colors;
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

			void Win7ButtonElements::Apply(const Win7ButtonColors& colors)
			{
				borderElement->SetColor(colors.borderColor);
				backgroundElement->SetColor(colors.backgroundColor);
				topGradientElement->SetColors(colors.g1, colors.g2);
				bottomGradientElement->SetColors(colors.g3, colors.g4);
				textElement->SetColor(colors.textColor);
			}

/***********************************************************************
Win7CheckedButtonElements
***********************************************************************/

			Win7CheckedButtonElements Win7CheckedButtonElements::Create(elements::ElementShape::Type shape)
			{
				const int checkSize=13;
				const int checkPadding=2;

				Win7CheckedButtonElements button;
				{
					button.mainComposition=new GuiBoundsComposition;
					button.mainComposition->SetMinSizeLimitation(GuiGraphicsComposition::LimitToElementAndChildren);
				}
				{
					GuiSolidBackgroundElement* element=GuiSolidBackgroundElement::Create();
					element->SetColor(Win7GetSystemWindowColor());

					GuiTableComposition* mainTable=new GuiTableComposition;
					button.mainComposition->AddChild(mainTable);
					mainTable->SetOwnedElement(element);
					mainTable->SetRowsAndColumns(1, 2);
					mainTable->SetAlignmentToParent(Margin(0, 0, 0, 0));
					mainTable->SetRowOption(0, GuiCellOption::PercentageOption(1.0));
					mainTable->SetColumnOption(0, GuiCellOption::AbsoluteOption(checkSize+2*checkPadding));
					mainTable->SetColumnOption(1, GuiCellOption::PercentageOption(1.0));
					
					{
						GuiCellComposition* cell=new GuiCellComposition;
						mainTable->AddChild(cell);
						cell->SetSite(0, 0, 1, 1);

						GuiTableComposition* table=new GuiTableComposition;
						cell->AddChild(table);
						table->SetRowsAndColumns(3, 1);
						table->SetAlignmentToParent(Margin(0, 0, 0, 0));
						table->SetRowOption(0, GuiCellOption::PercentageOption(0.5));
						table->SetRowOption(1, GuiCellOption::MinSizeOption());
						table->SetRowOption(2, GuiCellOption::PercentageOption(0.5));

						{
							GuiCellComposition* checkCell=new GuiCellComposition;
							table->AddChild(checkCell);
							checkCell->SetSite(1, 0, 1, 1);
							{
								GuiSolidBackgroundElement* element=GuiSolidBackgroundElement::Create();
								button.backgroundElement=element;
								element->SetShape(shape);

								GuiBoundsComposition* borderBounds=new GuiBoundsComposition;
								checkCell->AddChild(borderBounds);
								borderBounds->SetOwnedElement(element);
								borderBounds->SetAlignmentToParent(Margin(checkPadding, -1, checkPadding, -1));
								borderBounds->SetMinSizeLimitation(GuiGraphicsComposition::LimitToElementAndChildren);
								{
									GuiSolidBorderElement* element=GuiSolidBorderElement::Create();
									button.borderElement=element;
									element->SetShape(shape);

									GuiBoundsComposition* bounds=new GuiBoundsComposition;
									borderBounds->AddChild(bounds);
									bounds->SetOwnedElement(element);
									bounds->SetAlignmentToParent(Margin(0, 0, 0, 0));
									bounds->SetBounds(Rect(Point(0, 0), Size(checkSize, checkSize)));
								}
								{
									GuiGradientBackgroundElement* element=GuiGradientBackgroundElement::Create();
									button.outerGradientElement=element;
									element->SetShape(shape);
									element->SetDirection(GuiGradientBackgroundElement::Backslash);

									GuiBoundsComposition* bounds=new GuiBoundsComposition;
									borderBounds->AddChild(bounds);
									bounds->SetOwnedElement(element);
									bounds->SetAlignmentToParent(Margin(2, 2, 2, 2));
								}
								{
									GuiGradientBackgroundElement* element=GuiGradientBackgroundElement::Create();
									button.innerGradientElement=element;
									element->SetDirection(GuiGradientBackgroundElement::Backslash);
									element->SetShape(shape);

									GuiBoundsComposition* bounds=new GuiBoundsComposition;
									borderBounds->AddChild(bounds);
									bounds->SetOwnedElement(element);
									bounds->SetAlignmentToParent(Margin(3, 3, 3, 3));
								}
							}

							button.bulletTextElement=0;
							button.bulletBackgroundElement=0;
							if(shape==ElementShape::Rectangle)
							{
								button.bulletTextElement=GuiSolidLabelElement::Create();
								{
									FontProperties font;
									font.fontFamily=L"Wingdings 2";
									font.size=16;
									font.bold=true;
									button.bulletTextElement->SetFont(font);
								}
								button.bulletTextElement->SetText(L"P");
								button.bulletTextElement->SetAlignments(Alignment::Center, Alignment::Center);

								GuiBoundsComposition* composition=new GuiBoundsComposition;
								composition->SetOwnedElement(button.bulletTextElement);
								composition->SetAlignmentToParent(Margin(0, 0, 0, 0));
								checkCell->AddChild(composition);
							}
							else
							{
								button.bulletBackgroundElement=GuiSolidBackgroundElement::Create();
								button.bulletBackgroundElement->SetShape(ElementShape::Ellipse);

								GuiBoundsComposition* composition=new GuiBoundsComposition;
								composition->SetOwnedElement(button.bulletBackgroundElement);
								composition->SetAlignmentToParent(Margin(checkPadding+3, 3, checkPadding+3, 3));
								checkCell->AddChild(composition);
							}
						}
					}

					{
						GuiCellComposition* cell=new GuiCellComposition;
						mainTable->AddChild(cell);
						cell->SetSite(0, 1, 1, 1);

						GuiTableComposition* table=new GuiTableComposition;
						cell->AddChild(table);
						table->SetRowsAndColumns(3, 1);
						table->SetAlignmentToParent(Margin(0, 0, 0, 0));
						table->SetRowOption(0, GuiCellOption::PercentageOption(0.5));
						table->SetRowOption(1, GuiCellOption::MinSizeOption());
						table->SetRowOption(2, GuiCellOption::PercentageOption(0.5));

						{
							GuiCellComposition* textCell=new GuiCellComposition;
							table->AddChild(textCell);
							textCell->SetSite(1, 0, 1, 1);
							{
								Win7CreateSolidLabelElement(button.textElement, button.textComposition);
					
								button.textElement->SetAlignments(Alignment::Left, Alignment::Center);
								textCell->AddChild(button.textComposition);
							}
						}
					}
				}
				return button;
			}

			void Win7CheckedButtonElements::Apply(const Win7ButtonColors& colors)
			{
				borderElement->SetColor(colors.borderColor);
				backgroundElement->SetColor(colors.backgroundColor);
				outerGradientElement->SetColors(colors.g1, colors.g2);
				innerGradientElement->SetColors(colors.g3, colors.g4);
				textElement->SetColor(colors.textColor);
				if(bulletTextElement)
				{
					bulletTextElement->SetColor(colors.bulletDark);
				}
				if(bulletBackgroundElement)
				{
					bulletBackgroundElement->SetColor(colors.bulletDark);
				}
			}

/***********************************************************************
Helpers
***********************************************************************/

			int Win7GetColorAnimationLength()
			{
				return 120;
			}

			Color Win7GetSystemWindowColor()
			{
				return Color(240, 240, 240);
			}

			Color Win7GetSystemTextColor(bool enabled)
			{
				return enabled?Color(0, 0, 0):Color(131, 131, 131);
			}

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
Animation
***********************************************************************/

#define IMPLEMENT_TRANSFERRING_ANIMATION(TSTATE, TSTYLECONTROLLER)\
			TSTYLECONTROLLER::TransferringAnimation::TransferringAnimation(TSTYLECONTROLLER* _style, const TSTATE& begin)\
				:GuiTimeBasedAnimation(0)\
				,colorBegin(begin)\
				,colorEnd(begin)\
				,colorCurrent(begin)\
				,style(_style)\
				,stopped(true)\
			{\
			}\
			void TSTYLECONTROLLER::TransferringAnimation::Stop()\
			{\
				stopped=true;\
			}\
			void TSTYLECONTROLLER::TransferringAnimation::Transfer(const TSTATE& end)\
			{\
				if(colorEnd!=end)\
				{\
					Restart(Win7GetColorAnimationLength());\
					if(stopped)\
					{\
						colorBegin=colorEnd;\
						colorEnd=end;\
						style->GetBoundsComposition()->GetRelatedGraphicsHost()->GetAnimationManager()->AddAnimation(style->transferringAnimation);\
						stopped=false;\
					}\
					else\
					{\
						colorBegin=colorCurrent;\
						colorEnd=end;\
					}\
				}\
			}\
			void TSTYLECONTROLLER::TransferringAnimation::Play(int currentPosition, int totalLength)\

/***********************************************************************
Win7WindowStyle
***********************************************************************/

			Win7WindowStyle::Win7WindowStyle()
			{
				GuiSolidBackgroundElement* element=GuiSolidBackgroundElement::Create();
				element->SetColor(Win7GetSystemWindowColor());
				
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

			IMPLEMENT_TRANSFERRING_ANIMATION(Win7ButtonColors, Win7ButtonStyle)
			{
				colorCurrent=Win7ButtonColors::Blend(colorBegin, colorEnd, currentPosition, totalLength);
				style->elements.Apply(colorCurrent);
			}

			void Win7ButtonStyle::TransferInternal(GuiButton::ControlState value, bool enabled)
			{
				if(enabled)
				{
					switch(value)
					{
					case GuiButton::Normal:
						transferringAnimation->Transfer(Win7ButtonColors::Normal());
						break;
					case GuiButton::Active:
						transferringAnimation->Transfer(Win7ButtonColors::ButtonActive());
						break;
					case GuiButton::Pressed:
						transferringAnimation->Transfer(Win7ButtonColors::ButtonPressed());
						break;
					}
				}
				else
				{
					transferringAnimation->Transfer(Win7ButtonColors::Disabled());
				}
			}

			Win7ButtonStyle::Win7ButtonStyle()
				:controlStyle(GuiButton::Normal)
				,isVisuallyEnabled(true)
			{
				Win7ButtonColors initialColor=Win7ButtonColors::Normal();
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

			void Win7ButtonStyle::Transfer(GuiButton::ControlState value)
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
			
			IMPLEMENT_TRANSFERRING_ANIMATION(Color, Win7GroupBoxStyle)
			{
				colorCurrent=BlendColor(colorBegin, colorEnd, currentPosition, totalLength);
				style->textElement->SetColor(colorCurrent);
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
					element->SetColor(Win7GetSystemWindowColor());

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
					element->SetColor(Win7GetSystemWindowColor());

					textBackgroundComposition->SetOwnedElement(element);
					textBackgroundComposition->SetMinSizeLimitation(GuiGraphicsComposition::LimitToElementAndChildren);
					boundsComposition->AddChild(textBackgroundComposition);
				}

				textComposition=new GuiBoundsComposition;
				{
					GuiSolidLabelElement* element=GuiSolidLabelElement::Create();
					element->SetColor(Win7GetSystemTextColor(true));
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
				transferringAnimation=new TransferringAnimation(this, Win7GetSystemTextColor(true));
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
					transferringAnimation->Transfer(Win7GetSystemTextColor(true));
				}
				else
				{
					transferringAnimation->Transfer(Win7GetSystemTextColor(false));
				}
			}

/***********************************************************************
Win7CheckBoxStyle
***********************************************************************/

			IMPLEMENT_TRANSFERRING_ANIMATION(Win7ButtonColors, Win7CheckBoxStyle)
			{
				colorCurrent=Win7ButtonColors::Blend(colorBegin, colorEnd, currentPosition, totalLength);
				style->elements.Apply(colorCurrent);
			}

			void Win7CheckBoxStyle::TransferInternal(GuiButton::ControlState value, bool enabled, bool selected)
			{
				if(enabled)
				{
					switch(value)
					{
					case GuiButton::Normal:
						transferringAnimation->Transfer(Win7ButtonColors::CheckedNormal(selected));
						break;
					case GuiButton::Active:
						transferringAnimation->Transfer(Win7ButtonColors::CheckedActive(selected));
						break;
					case GuiButton::Pressed:
						transferringAnimation->Transfer(Win7ButtonColors::CheckedPressed(selected));
						break;
					}
				}
				else
				{
					transferringAnimation->Transfer(Win7ButtonColors::CheckedDisabled(selected));
				}
			}

			Win7CheckBoxStyle::Win7CheckBoxStyle(BulletStyle bulletStyle)
				:controlStyle(GuiButton::Normal)
				,isVisuallyEnabled(true)
				,isSelected(false)
			{
				Win7ButtonColors initialColor=Win7ButtonColors::CheckedNormal(isSelected);
				elements=Win7CheckedButtonElements::Create(bulletStyle==CheckBox?ElementShape::Rectangle:ElementShape::Ellipse);
				elements.Apply(initialColor);
				transferringAnimation=new TransferringAnimation(this, initialColor);
			}

			Win7CheckBoxStyle::~Win7CheckBoxStyle()
			{
			}

			elements::GuiBoundsComposition* Win7CheckBoxStyle::GetBoundsComposition()
			{
				return elements.mainComposition;
			}

			elements::GuiGraphicsComposition* Win7CheckBoxStyle::GetContainerComposition()
			{
				return elements.mainComposition;
			}

			void Win7CheckBoxStyle::SetText(const WString& value)
			{
				elements.textElement->SetText(value);
			}

			void Win7CheckBoxStyle::SetFont(const FontProperties& value)
			{
				Win7SetFont(elements.textElement, elements.textComposition, value);
			}

			void Win7CheckBoxStyle::SetVisuallyEnabled(bool value)
			{
				if(isVisuallyEnabled!=value)
				{
					isVisuallyEnabled=value;
					TransferInternal(controlStyle, isVisuallyEnabled, isSelected);
				}
			}

			void Win7CheckBoxStyle::SetSelected(bool value)
			{
				if(isSelected!=value)
				{
					isSelected=value;
					TransferInternal(controlStyle, isVisuallyEnabled, isSelected);
				}
			}

			void Win7CheckBoxStyle::Transfer(GuiButton::ControlState value)
			{
				if(controlStyle!=value)
				{
					controlStyle=value;
					TransferInternal(controlStyle, isVisuallyEnabled, isSelected);
				}
			}

/***********************************************************************
Win7ScrollStyle
***********************************************************************/

			Win7ScrollStyle::Win7ScrollStyle(Direction direction)
				:commandExecutor(0)
				,decreaseButton(0)
				,increaseButton(0)
				,boundsComposition(0)
			{
				boundsComposition=new GuiBoundsComposition;
				{
					decreaseButton=new GuiButton(new Win7ButtonStyle);
					decreaseButton->GetBoundsComposition()->SetAlignmentToParent(Margin(0, 0, 0, 0));
					increaseButton=new GuiButton(new Win7ButtonStyle);
					increaseButton->GetBoundsComposition()->SetAlignmentToParent(Margin(0, 0, 0, 0));
				}
				{
					GuiSideAlignedComposition* decreaseComposition=new GuiSideAlignedComposition;
					decreaseComposition->SetMaxLength(DefaultSize);
					decreaseComposition->SetMaxRatio(0.5);
					decreaseComposition->AddChild(decreaseButton->GetBoundsComposition());
					boundsComposition->AddChild(decreaseComposition);

					GuiSideAlignedComposition* increaseComposition=new GuiSideAlignedComposition;
					increaseComposition->SetMaxLength(DefaultSize);
					increaseComposition->SetMaxRatio(0.5);
					increaseComposition->AddChild(increaseButton->GetBoundsComposition());
					boundsComposition->AddChild(increaseComposition);

					switch(direction)
					{
					case Horizontal:
						{
							decreaseComposition->SetDirection(GuiSideAlignedComposition::Left);
							increaseComposition->SetDirection(GuiSideAlignedComposition::Right);
						}
						break;
					case Vertical:
						{
							decreaseComposition->SetDirection(GuiSideAlignedComposition::Top);
							increaseComposition->SetDirection(GuiSideAlignedComposition::Bottom);
						}
						break;
					}
				}
			}

			Win7ScrollStyle::~Win7ScrollStyle()
			{
			}

			elements::GuiBoundsComposition* Win7ScrollStyle::GetBoundsComposition()
			{
				return boundsComposition;
			}

			elements::GuiGraphicsComposition* Win7ScrollStyle::GetContainerComposition()
			{
				return boundsComposition;
			}

			void Win7ScrollStyle::SetText(const WString& value)
			{
			}

			void Win7ScrollStyle::SetFont(const FontProperties& value)
			{
			}

			void Win7ScrollStyle::SetVisuallyEnabled(bool value)
			{
			}

			void Win7ScrollStyle::SetCommandExecutor(controls::GuiScroll::ICommandExecutor* value)
			{
				commandExecutor=value;
			}

			void Win7ScrollStyle::SetTotalSize(int value)
			{
			}

			void Win7ScrollStyle::SetPageSize(int value)
			{
			}

			void Win7ScrollStyle::SetPosition(int value)
			{
			}
		}
	}
}