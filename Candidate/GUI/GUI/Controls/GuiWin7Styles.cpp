#include <math.h>
#include "GuiWin7Styles.h"

namespace vl
{
	namespace presentation
	{
		namespace win7
		{
			using namespace collections;
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
Win7ButtonColors
***********************************************************************/

			void Win7ButtonColors::SetAlphaWithoutText(unsigned char a)
			{
				borderColor.a=a;
				backgroundColor.a=a;
				g1.a=a;
				g2.a=a;
				g3.a=a;
				g4.a=a;
			}

			Win7ButtonColors Win7ButtonColors::Blend(const Win7ButtonColors& c1, const Win7ButtonColors& c2, int ratio, int total)
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

			Win7ButtonElements Win7ButtonElements::Create(bool verticalGradient)
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
					if(verticalGradient)
					{
						table->SetRowsAndColumns(2, 1);
						table->SetRowOption(0, GuiCellOption::PercentageOption(0.5));
						table->SetRowOption(1, GuiCellOption::PercentageOption(0.5));
						table->SetColumnOption(0, GuiCellOption::PercentageOption(1.0));
					}
					else
					{
						table->SetRowsAndColumns(1, 2);
						table->SetRowOption(0, GuiCellOption::PercentageOption(1.0));
						table->SetColumnOption(0, GuiCellOption::PercentageOption(0.5));
						table->SetColumnOption(1, GuiCellOption::PercentageOption(0.5));
					}
					{
						GuiGradientBackgroundElement* element=GuiGradientBackgroundElement::Create();
						button.topGradientElement=element;
						element->SetDirection(verticalGradient?GuiGradientBackgroundElement::Vertical:GuiGradientBackgroundElement::Horizontal);

						GuiCellComposition* cell=new GuiCellComposition;
						table->AddChild(cell);
						cell->SetSite(0, 0, 1, 1);
						cell->SetOwnedElement(element);
					}
					{
						GuiGradientBackgroundElement* element=GuiGradientBackgroundElement::Create();
						button.bottomGradientElement=element;
						element->SetDirection(verticalGradient?GuiGradientBackgroundElement::Vertical:GuiGradientBackgroundElement::Horizontal);

						GuiCellComposition* cell=new GuiCellComposition;
						table->AddChild(cell);
						if(verticalGradient)
						{
							cell->SetSite(1, 0, 1, 1);
						}
						else
						{
							cell->SetSite(0, 1, 1, 1);
						}
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
Win7TextBoxColors
***********************************************************************/

			Win7TextBoxColors Win7TextBoxColors::Blend(const Win7TextBoxColors& c1, const Win7TextBoxColors& c2, int ratio, int total)
			{
				if(ratio<0) ratio=0;
				else if(ratio>total) ratio=total;

				Win7TextBoxColors result;
				result.borderColor=BlendColor(c1.borderColor, c2.borderColor, ratio, total);
				result.backgroundColor=BlendColor(c1.backgroundColor, c2.backgroundColor, ratio, total);
				return result;
			}
			
			Win7TextBoxColors Win7TextBoxColors::Normal()
			{
				Win7TextBoxColors result=
				{
					Color(197, 197, 197),
					Color(255, 255, 255),
				};
				return result;
			}

			Win7TextBoxColors Win7TextBoxColors::Active()
			{
				Win7TextBoxColors result=
				{
					Color(197, 218, 237),
					Color(255, 255, 255),
				};
				return result;
			}

			Win7TextBoxColors Win7TextBoxColors::Focused()
			{
				Win7TextBoxColors result=
				{
					Color(61, 123, 173),
					Color(255, 255, 255),
				};
				return result;
			}

			Win7TextBoxColors Win7TextBoxColors::Disabled()
			{
				Win7TextBoxColors result=
				{
					Color(175, 175, 175),
					Win7GetSystemWindowColor(),
				};
				return result;
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

			Color Win7GetSystemBorderColor()
			{
				return Color(100, 100, 100);
			}

			Color Win7GetSystemTextColor(bool enabled)
			{
				return enabled?Color(0, 0, 0):Color(131, 131, 131);
			}

			void Win7SetFont(GuiSolidLabelElement* element, GuiBoundsComposition* composition, const FontProperties& fontProperties)
			{
				int margin=fontProperties.size/2;
				element->SetFont(fontProperties);
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

#define DEFAULT_TRANSFERRING_ANIMATION_HOST_GETTER(STYLE) (STYLE->GetBoundsComposition()->GetRelatedGraphicsHost())

#define IMPLEMENT_TRANSFERRING_ANIMATION_BASE(TSTATE, TSTYLECONTROLLER, HOST_GETTER)\
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
					GuiGraphicsHost* host=HOST_GETTER(style);\
					if(host)\
					{\
						Restart(Win7GetColorAnimationLength());\
						if(stopped)\
						{\
							colorBegin=colorEnd;\
							colorEnd=end;\
							host->GetAnimationManager()->AddAnimation(style->transferringAnimation);\
							stopped=false;\
						}\
						else\
						{\
							colorBegin=colorCurrent;\
							colorEnd=end;\
						}\
					}\
					else\
					{\
						colorBegin=end;\
						colorEnd=end;\
						colorCurrent=end;\
						Play(1, 1);\
					}\
				}\
			}\
			void TSTYLECONTROLLER::TransferringAnimation::Play(int currentPosition, int totalLength)\

#define IMPLEMENT_TRANSFERRING_ANIMATION(TSTATE, TSTYLECONTROLLER)\
	IMPLEMENT_TRANSFERRING_ANIMATION_BASE(TSTATE, TSTYLECONTROLLER, DEFAULT_TRANSFERRING_ANIMATION_HOST_GETTER)

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

			void Win7WindowStyle::SetFocusableComposition(elements::GuiGraphicsComposition* value)
			{
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
				Win7ButtonColors targetColor;
				if(enabled)
				{
					switch(value)
					{
					case GuiButton::Normal:
						targetColor=Win7ButtonColors::Normal();
						if(transparentWhenInactive)
						{
							targetColor.SetAlphaWithoutText(0);
						}
						break;
					case GuiButton::Active:
						targetColor=Win7ButtonColors::ButtonActive();
						break;
					case GuiButton::Pressed:
						targetColor=Win7ButtonColors::ButtonPressed();
						break;
					}
				}
				else
				{
					targetColor=Win7ButtonColors::Disabled();
					if(transparentWhenDisabled)
					{
						targetColor.SetAlphaWithoutText(0);
					}
				}
				transferringAnimation->Transfer(targetColor);
			}

			Win7ButtonStyle::Win7ButtonStyle(bool verticalGradient)
				:controlStyle(GuiButton::Normal)
				,isVisuallyEnabled(true)
				,transparentWhenInactive(false)
				,transparentWhenDisabled(false)
			{
				Win7ButtonColors initialColor=Win7ButtonColors::Normal();
				elements=Win7ButtonElements::Create(verticalGradient);
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

			void Win7ButtonStyle::SetFocusableComposition(elements::GuiGraphicsComposition* value)
			{
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

			bool Win7ButtonStyle::GetTransparentWhenInactive()
			{
				return transparentWhenInactive;
			}

			void Win7ButtonStyle::SetTransparentWhenInactive(bool value)
			{
				transparentWhenInactive=value;
				TransferInternal(controlStyle, isVisuallyEnabled);
			}

			bool Win7ButtonStyle::GetTransparentWhenDisabled()
			{
				return transparentWhenDisabled;
			}

			void Win7ButtonStyle::SetTransparentWhenDisabled(bool value)
			{
				transparentWhenDisabled=value;
				TransferInternal(controlStyle, isVisuallyEnabled);
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

			void Win7GroupBoxStyle::SetFocusableComposition(elements::GuiGraphicsComposition* value)
			{
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

			void Win7CheckBoxStyle::SetFocusableComposition(elements::GuiGraphicsComposition* value)
			{
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

			controls::GuiButton::IStyleController* Win7ScrollStyle::CreateDecreaseButtonStyle(Direction direction)
			{
				Win7ButtonStyle* decreaseButtonStyle=new Win7ButtonStyle(direction==Horizontal);
				decreaseButtonStyle->SetTransparentWhenInactive(true);
				decreaseButtonStyle->SetTransparentWhenDisabled(true);
				return decreaseButtonStyle;
			}

			controls::GuiButton::IStyleController* Win7ScrollStyle::CreateIncreaseButtonStyle(Direction direction)
			{
				Win7ButtonStyle* increaseButtonStyle=new Win7ButtonStyle(direction==Horizontal);
				increaseButtonStyle->SetTransparentWhenInactive(true);
				increaseButtonStyle->SetTransparentWhenDisabled(true);
				return increaseButtonStyle;
			}

			controls::GuiButton::IStyleController* Win7ScrollStyle::CreateHandleButtonStyle(Direction direction)
			{
				Win7ButtonStyle* handleButtonStyle=new Win7ButtonStyle(direction==Horizontal);
				handleButtonStyle->SetTransparentWhenDisabled(true);
				return handleButtonStyle;
			}

			void Win7ScrollStyle::InstallBackground(elements::GuiGraphicsComposition* boundsComposition, Direction direction)
			{
				Color sinkColor(227, 227, 227);
				GuiSolidBackgroundElement* element=GuiSolidBackgroundElement::Create();
				element->SetColor(Win7GetSystemWindowColor());
				boundsComposition->SetOwnedElement(element);

				{
					GuiSideAlignedComposition* composition=new GuiSideAlignedComposition;
					composition->SetMaxLength(DefaultSize);
					composition->SetMaxRatio(0.2);
					boundsComposition->AddChild(composition);

					GuiGradientBackgroundElement* gradient=GuiGradientBackgroundElement::Create();
					gradient->SetColors(sinkColor, Win7GetSystemWindowColor());
					composition->SetOwnedElement(gradient);

					switch(direction)
					{
					case Horizontal:
						composition->SetDirection(GuiSideAlignedComposition::Top);
						gradient->SetDirection(GuiGradientBackgroundElement::Vertical);
						break;
					case Vertical:
						composition->SetDirection(GuiSideAlignedComposition::Left);
						gradient->SetDirection(GuiGradientBackgroundElement::Horizontal);
						break;
					}
				}
				{
					GuiSideAlignedComposition* composition=new GuiSideAlignedComposition;
					composition->SetMaxLength(DefaultSize);
					composition->SetMaxRatio(0.2);
					boundsComposition->AddChild(composition);

					GuiGradientBackgroundElement* gradient=GuiGradientBackgroundElement::Create();
					gradient->SetColors(Win7GetSystemWindowColor(), sinkColor);
					composition->SetOwnedElement(gradient);

					switch(direction)
					{
					case Horizontal:
						composition->SetDirection(GuiSideAlignedComposition::Bottom);
						gradient->SetDirection(GuiGradientBackgroundElement::Vertical);
						break;
					case Vertical:
						composition->SetDirection(GuiSideAlignedComposition::Right);
						gradient->SetDirection(GuiGradientBackgroundElement::Horizontal);
						break;
					}
				}
				{
					GuiSolidBorderElement* element=GuiSolidBorderElement::Create();
					element->SetColor(sinkColor);

					GuiBoundsComposition* composition=new GuiBoundsComposition;
					composition->SetAlignmentToParent(Margin(0, 0, 0, 0));
					composition->SetOwnedElement(element);
					boundsComposition->AddChild(composition);
				}
			}

			Win7ScrollStyle::Win7ScrollStyle(Direction _direction)
				:CommonScrollStyle(_direction)
			{
				BuildStyle(DefaultSize, ArrowSize);
			}

			Win7ScrollStyle::~Win7ScrollStyle()
			{
			}

/***********************************************************************
Win7TrackStyle
***********************************************************************/

			controls::GuiButton::IStyleController* Win7TrackStyle::CreateHandleButtonStyle(Direction direction)
			{
				Win7ButtonStyle* handleButtonStyle=new Win7ButtonStyle(direction==Horizontal);
				return handleButtonStyle;
			}

			void Win7TrackStyle::InstallBackground(elements::GuiGraphicsComposition* boundsComposition, Direction direction)
			{
				GuiSolidBackgroundElement* element=GuiSolidBackgroundElement::Create();
				element->SetColor(Win7GetSystemWindowColor());
				boundsComposition->SetOwnedElement(element);
			}

			void Win7TrackStyle::InstallTrack(elements::GuiGraphicsComposition* trackComposition, Direction direction)
			{
				Gui3DBorderElement* element=Gui3DBorderElement::Create();
				element->SetColors(Color(176, 176, 176), Color(252, 252, 252));
				trackComposition->SetOwnedElement(element);
			}

			Win7TrackStyle::Win7TrackStyle(Direction _direction)
				:CommonTrackStyle(_direction)
			{
				BuildStyle(TrackThickness, TrackPadding, HandleLong, HandleShort);
			}

			Win7TrackStyle::~Win7TrackStyle()
			{
			}

/***********************************************************************
Win7ScrollViewProvider
***********************************************************************/

			void Win7ScrollViewProvider::AssociateStyleController(controls::GuiControl::IStyleController* controller)
			{
			}
			
			void Win7ScrollViewProvider::SetFocusableComposition(elements::GuiGraphicsComposition* value)
			{
			}

			void Win7ScrollViewProvider::SetText(const WString& value)
			{
			}

			void Win7ScrollViewProvider::SetFont(const FontProperties& value)
			{
			}

			void Win7ScrollViewProvider::SetVisuallyEnabled(bool value)
			{
			}

			controls::GuiScroll::IStyleController* Win7ScrollViewProvider::CreateHorizontalScrollStyle()
			{
				return new Win7ScrollStyle(Win7ScrollStyle::Horizontal);
			}

			controls::GuiScroll::IStyleController* Win7ScrollViewProvider::CreateVerticalScrollStyle()
			{
				return new Win7ScrollStyle(Win7ScrollStyle::Vertical);
			}

			int Win7ScrollViewProvider::GetDefaultScrollSize()
			{
				return Win7ScrollStyle::DefaultSize;
			}

			elements::GuiGraphicsComposition* Win7ScrollViewProvider::InstallBackground(elements::GuiBoundsComposition* boundsComposition)
			{
				GuiSolidBorderElement* border=GuiSolidBorderElement::Create();
				border->SetColor(Win7GetSystemBorderColor());
				boundsComposition->SetOwnedElement(border);
				boundsComposition->SetInternalMargin(Margin(1, 1, 1, 1));
				
				GuiSolidBackgroundElement* background=GuiSolidBackgroundElement::Create();
				background->SetColor(Win7GetSystemWindowColor());

				GuiBoundsComposition* backgroundComposition=new GuiBoundsComposition;
				boundsComposition->AddChild(backgroundComposition);
				backgroundComposition->SetAlignmentToParent(Margin(0, 0, 0, 0));
				backgroundComposition->SetOwnedElement(background);

				return boundsComposition;
			}

/***********************************************************************
Win7TextBoxBackground
***********************************************************************/

#define HOST_GETTER_BY_FOCUSABLE_COMPOSITION(STYLE) (style->focusableComposition->GetRelatedGraphicsHost())

			IMPLEMENT_TRANSFERRING_ANIMATION_BASE(Win7TextBoxColors, Win7TextBoxBackground, HOST_GETTER_BY_FOCUSABLE_COMPOSITION)
			{
				colorCurrent=Win7TextBoxColors::Blend(colorBegin, colorEnd, currentPosition, totalLength);
				style->Apply(colorCurrent);
			}

			void Win7TextBoxBackground::UpdateStyle()
			{
				if(!isVisuallyEnabled)
				{
					transferringAnimation->Transfer(Win7TextBoxColors::Disabled());
				}
				else if(isFocused)
				{
					transferringAnimation->Transfer(Win7TextBoxColors::Focused());
				}
				else if(isMouseEnter)
				{
					transferringAnimation->Transfer(Win7TextBoxColors::Active());
				}
				else
				{
					transferringAnimation->Transfer(Win7TextBoxColors::Normal());
				}
			}

			void Win7TextBoxBackground::Apply(const Win7TextBoxColors& colors)
			{
				borderElement->SetColor(colors.borderColor);
				backgroundElement->SetColor(colors.backgroundColor);
			}

			void Win7TextBoxBackground::OnBoundsMouseEnter(elements::GuiGraphicsComposition* sender, elements::GuiEventArgs& arguments)
			{
				isMouseEnter=true;
				UpdateStyle();
			}

			void Win7TextBoxBackground::OnBoundsMouseLeave(elements::GuiGraphicsComposition* sender, elements::GuiEventArgs& arguments)
			{
				isMouseEnter=false;
				UpdateStyle();
			}

			void Win7TextBoxBackground::OnBoundsGotFocus(elements::GuiGraphicsComposition* sender, elements::GuiEventArgs& arguments)
			{
				isFocused=true;
				UpdateStyle();
			}

			void Win7TextBoxBackground::OnBoundsLostFocus(elements::GuiGraphicsComposition* sender, elements::GuiEventArgs& arguments)
			{
				isFocused=false;
				UpdateStyle();
			}

			Win7TextBoxBackground::Win7TextBoxBackground()
				:backgroundElement(0)
				,borderElement(0)
				,focusableComposition(0)
				,isMouseEnter(false)
				,isFocused(false)
				,isVisuallyEnabled(false)
				,styleController(0)
				,textElement(0)
			{
				transferringAnimation=new TransferringAnimation(this, Win7TextBoxColors::Normal());
			}

			Win7TextBoxBackground::~Win7TextBoxBackground()
			{
			}

			void Win7TextBoxBackground::AssociateStyleController(controls::GuiControl::IStyleController* controller)
			{
				styleController=controller;
			}
			
			void Win7TextBoxBackground::SetFocusableComposition(elements::GuiGraphicsComposition* value)
			{
				focusableComposition=value;
				focusableComposition->GetEventReceiver()->mouseEnter.AttachMethod(this, &Win7TextBoxBackground::OnBoundsMouseEnter);
				focusableComposition->GetEventReceiver()->mouseLeave.AttachMethod(this, &Win7TextBoxBackground::OnBoundsMouseLeave);
				focusableComposition->GetEventReceiver()->gotFocus.AttachMethod(this, &Win7TextBoxBackground::OnBoundsGotFocus);
				focusableComposition->GetEventReceiver()->lostFocus.AttachMethod(this, &Win7TextBoxBackground::OnBoundsLostFocus);
			}

			void Win7TextBoxBackground::SetVisuallyEnabled(bool value)
			{
				isVisuallyEnabled=value;
				UpdateStyle();
			}

			elements::GuiGraphicsComposition* Win7TextBoxBackground::InstallBackground(elements::GuiBoundsComposition* boundsComposition)
			{
				{
					GuiSolidBackgroundElement* background=GuiSolidBackgroundElement::Create();
					background->SetColor(Color(255, 255, 255));

					GuiBoundsComposition* backgroundComposition=new GuiBoundsComposition;
					boundsComposition->AddChild(backgroundComposition);
					backgroundComposition->SetAlignmentToParent(Margin(1, 1, 1, 1));
					backgroundComposition->SetOwnedElement(background);
					backgroundElement=background;
				}
				{
					GuiRoundBorderElement* border=GuiRoundBorderElement::Create();
					border->SetColor(Win7GetSystemBorderColor());
					border->SetRadius(2);
					borderElement=border;

					GuiBoundsComposition* borderComposition=new GuiBoundsComposition;
					boundsComposition->AddChild(borderComposition);
					borderComposition->SetAlignmentToParent(Margin(0, 0, 0, 0));
					borderComposition->SetOwnedElement(border);
				}
				Apply(Win7TextBoxColors::Normal());
				{
					GuiBoundsComposition* containerComposition=new GuiBoundsComposition;
					boundsComposition->AddChild(containerComposition);
					containerComposition->SetAlignmentToParent(Margin(2, 2, 2, 2));
					return containerComposition;
				}
			}

			void Win7TextBoxBackground::InitializeTextElement(elements::GuiColorizedTextElement* _textElement)
			{
				textElement=_textElement;

				Array<text::ColorEntry> colors;
				colors.Resize(1);
				{
					text::ColorEntry entry;
					entry.normal.text=Color(0, 0, 0);
					entry.normal.background=Color(0, 0, 0, 0);
					entry.selectedFocused.text=Color(255, 255, 255);
					entry.selectedFocused.background=Color(51, 153, 255);
					entry.selectedUnfocused.text=Color(255, 255, 255);
					entry.selectedUnfocused.background=Color(51, 153, 255);
					colors[0]=entry;
				}
				textElement->SetColors(colors);
				textElement->SetCaretColor(Color(0, 0, 0));
			}

/***********************************************************************
Win7MultilineTextBoxProvider
***********************************************************************/

			Win7MultilineTextBoxProvider::Win7MultilineTextBoxProvider()
				:styleController(0)
			{
			}

			Win7MultilineTextBoxProvider::~Win7MultilineTextBoxProvider()
			{
			}

			void Win7MultilineTextBoxProvider::AssociateStyleController(controls::GuiControl::IStyleController* controller)
			{
				styleController=controller;
				background.AssociateStyleController(controller);
			}
			
			void Win7MultilineTextBoxProvider::SetFocusableComposition(elements::GuiGraphicsComposition* value)
			{
				background.SetFocusableComposition(value);
				GuiMultilineTextBox::StyleController* textBoxController=dynamic_cast<GuiMultilineTextBox::StyleController*>(styleController);
				background.InitializeTextElement(textBoxController->GetTextElement());
			}

			void Win7MultilineTextBoxProvider::SetVisuallyEnabled(bool value)
			{
				background.SetVisuallyEnabled(value);
			}

			elements::GuiGraphicsComposition* Win7MultilineTextBoxProvider::InstallBackground(elements::GuiBoundsComposition* boundsComposition)
			{
				return background.InstallBackground(boundsComposition);
			}

/***********************************************************************
Win7SinglelineTextBoxProvider
***********************************************************************/

			Win7SinglelineTextBoxProvider::Win7SinglelineTextBoxProvider()
				:styleController(0)
			{
			}

			Win7SinglelineTextBoxProvider::~Win7SinglelineTextBoxProvider()
			{
			}

			void Win7SinglelineTextBoxProvider::AssociateStyleController(controls::GuiControl::IStyleController* controller)
			{
				styleController=controller;
				background.AssociateStyleController(controller);
			}
			
			void Win7SinglelineTextBoxProvider::SetFocusableComposition(elements::GuiGraphicsComposition* value)
			{
				background.SetFocusableComposition(value);
				GuiSinglelineTextBox::StyleController* textBoxController=dynamic_cast<GuiSinglelineTextBox::StyleController*>(styleController);
				background.InitializeTextElement(textBoxController->GetTextElement());
			}

			void Win7SinglelineTextBoxProvider::SetText(const WString& value)
			{
			}

			void Win7SinglelineTextBoxProvider::SetFont(const FontProperties& value)
			{
			}

			void Win7SinglelineTextBoxProvider::SetVisuallyEnabled(bool value)
			{
				background.SetVisuallyEnabled(value);
			}

			elements::GuiGraphicsComposition* Win7SinglelineTextBoxProvider::InstallBackground(elements::GuiBoundsComposition* boundsComposition)
			{
				return background.InstallBackground(boundsComposition);
			}
		}
	}
}