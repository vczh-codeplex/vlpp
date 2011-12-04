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

			//---------------------------------------------------------

			Win7ButtonColors Win7ButtonColors::ButtonNormal()
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

			Win7ButtonColors Win7ButtonColors::ButtonDisabled()
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

			//---------------------------------------------------------

			Win7ButtonColors Win7ButtonColors::ItemNormal()
			{
				Win7ButtonColors colors=
				{
					Color(184, 214, 251, 0),
					Color(252, 253, 254, 0),
					Color(250, 251, 253, 0),
					Color(242, 247, 253, 0),
					Color(242, 247, 253, 0),
					Color(235, 243, 253, 0),
					Win7GetSystemTextColor(true),
				};
				return colors;
			}

			Win7ButtonColors Win7ButtonColors::ItemActive()
			{
				Win7ButtonColors colors=
				{
					Color(184, 214, 251),
					Color(252, 253, 254),
					Color(250, 251, 253),
					Color(242, 247, 253),
					Color(242, 247, 253),
					Color(235, 243, 253),
					Win7GetSystemTextColor(true),
				};
				return colors;
			}

			Win7ButtonColors Win7ButtonColors::ItemSelected()
			{
				Win7ButtonColors colors=
				{
					Color(118, 158, 206),
					Color(235, 244, 253),
					Color(220, 235, 252),
					Color(208, 227, 252),
					Color(208, 227, 252),
					Color(193, 219, 252),
					Win7GetSystemTextColor(true),
				};
				return colors;
			}

			Win7ButtonColors Win7ButtonColors::ItemDisabled()
			{
				Win7ButtonColors colors=
				{
					Color(184, 214, 251, 0),
					Color(252, 253, 254, 0),
					Color(250, 251, 253, 0),
					Color(242, 247, 253, 0),
					Color(242, 247, 253, 0),
					Color(235, 243, 253, 0),
					Win7GetSystemTextColor(false),
				};
				return colors;
			}

			//---------------------------------------------------------

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

			//---------------------------------------------------------

			Win7ButtonColors Win7ButtonColors::ToolstripButtonNormal()
			{
				Win7ButtonColors colors=
				{
					Win7GetSystemWindowColor(),
					Win7GetSystemWindowColor(),
					Win7GetSystemWindowColor(),
					Win7GetSystemWindowColor(),
					Win7GetSystemWindowColor(),
					Win7GetSystemWindowColor(),
					Win7GetSystemTextColor(true),
				};
				return colors;
			}

			Win7ButtonColors Win7ButtonColors::ToolstripButtonActive()
			{
				Win7ButtonColors colors=
				{
					Color(168, 168, 168),
					Color(248, 248, 248),
					Color(250, 250, 250),
					Color(232, 232, 232),
					Color(218, 218, 218),
					Color(240, 240, 240),
					Win7GetSystemTextColor(true),
				};
				return colors;
			}

			Win7ButtonColors Win7ButtonColors::ToolstripButtonPressed()
			{
				Win7ButtonColors colors=
				{
					Color(84, 84, 84),
					Color(156, 156, 156),
					Color(240, 240, 240),
					Color(228, 228, 228),
					Color(222, 222, 222),
					Color(230, 230, 230),
					Win7GetSystemTextColor(true),
				};
				return colors;
			}

			Win7ButtonColors Win7ButtonColors::ToolstripButtonDisabled()
			{
				Win7ButtonColors colors=
				{
					Win7GetSystemWindowColor(),
					Win7GetSystemWindowColor(),
					Win7GetSystemWindowColor(),
					Win7GetSystemWindowColor(),
					Win7GetSystemWindowColor(),
					Win7GetSystemWindowColor(),
					Win7GetSystemTextColor(false),
				};
				return colors;
			}

			//---------------------------------------------------------

			Win7ButtonColors Win7ButtonColors::MenuBarButtonNormal()
			{
				Win7ButtonColors colors=
				{
					Color(176, 176, 176),
					Color(234, 238, 247),
					Color(246, 248, 250),
					Color(211, 217, 231),
					Color(199, 204, 218),
					Color(224, 228, 243),
					Win7GetSystemTextColor(true),
				};
				return colors;
			}

			Win7ButtonColors Win7ButtonColors::MenuBarButtonActive()
			{
				Win7ButtonColors colors=
				{
					Color(176, 176, 176),
					Color(234, 238, 247),
					Color(246, 248, 250),
					Color(211, 217, 231),
					Color(199, 204, 218),
					Color(224, 228, 243),
					Win7GetSystemTextColor(true),
				};
				return colors;
			}

			Win7ButtonColors Win7ButtonColors::MenuBarButtonPressed()
			{
				Win7ButtonColors colors=
				{
					Color(88, 88, 89),
					Color(159, 160, 162),
					Color(200, 202, 208),
					Color(205, 209, 219),
					Color(197, 204, 221),
					Color(213, 218, 233),
					Win7GetSystemTextColor(true),
				};
				return colors;
			}

			Win7ButtonColors Win7ButtonColors::MenuBarButtonDisabled()
			{
				Win7ButtonColors colors=
				{
					Color(176, 176, 176),
					Color(234, 238, 247),
					Color(246, 248, 250),
					Color(211, 217, 231),
					Color(199, 204, 218),
					Color(224, 228, 243),
					Win7GetSystemTextColor(false),
				};
				return colors;
			}

			//---------------------------------------------------------

			Win7ButtonColors Win7ButtonColors::MenuItemButtonNormal()
			{
				Win7ButtonColors colors=
				{
					Color(0, 0, 0, 0),
					Color(0, 0, 0, 0),
					Color(0, 0, 0, 0),
					Color(0, 0, 0, 0),
					Color(227, 227, 227),
					Color(255, 255, 255),
					Win7GetSystemTextColor(true),
				};
				return colors;
			}

			Win7ButtonColors Win7ButtonColors::MenuItemButtonNormalActive()
			{
				Win7ButtonColors colors=
				{
					Color(175, 208, 247),
					Color(248, 248, 250),
					Color(243, 245, 247),
					Color(231, 238, 247),
					Color(229, 233, 238),
					Color(245, 249, 255),
					Win7GetSystemTextColor(true),
				};
				return colors;
			}

			Win7ButtonColors Win7ButtonColors::MenuItemButtonDisabled()
			{
				Win7ButtonColors colors=
				{
					Color(0, 0, 0, 0),
					Color(0, 0, 0, 0),
					Color(0, 0, 0, 0),
					Color(0, 0, 0, 0),
					Color(227, 227, 227),
					Color(255, 255, 255),
					Win7GetSystemTextColor(false),
				};
				return colors;
			}

			Win7ButtonColors Win7ButtonColors::MenuItemButtonDisabledActive()
			{
				Win7ButtonColors colors=
				{
					Color(212, 212, 212),
					Color(244, 244, 244),
					Color(243, 243, 243),
					Color(229, 229, 229),
					Color(232, 232, 232),
					Color(239, 239, 239),
					Win7GetSystemTextColor(false),
				};
				return colors;
			}

			//---------------------------------------------------------

			Win7ButtonColors Win7ButtonColors::TabPageHeaderNormal()
			{
				Win7ButtonColors colors=
				{
					Color(137, 140, 149),
					Color(243, 243, 243),
					Color(242, 242, 242),
					Color(235, 235, 235),
					Color(219, 219, 219),
					Color(207, 207, 207),
					Win7GetSystemTextColor(true),
				};
				return colors;
			}

			Win7ButtonColors Win7ButtonColors::TabPageHeaderActive()
			{
				Win7ButtonColors colors=
				{
					Color(60, 127, 177),
					Color(233, 245, 252),
					Color(234, 246, 253),
					Color(217, 240, 252),
					Color(188, 229, 252),
					Color(167, 217, 245),
					Win7GetSystemTextColor(true),
				};
				return colors;
			}

			Win7ButtonColors Win7ButtonColors::TabPageHeaderSelected()
			{
				Win7ButtonColors colors=
				{
					Color(137, 140, 149),
					Win7GetSystemTabContentColor(),
					Win7GetSystemTabContentColor(),
					Win7GetSystemTabContentColor(),
					Win7GetSystemTabContentColor(),
					Win7GetSystemTabContentColor(),
					Win7GetSystemTextColor(true),
				};
				return colors;
			}

/***********************************************************************
Win7ButtonElements
***********************************************************************/

			Win7ButtonElements Win7ButtonElements::Create(bool verticalGradient, bool roundBorder, Alignment::Type horizontal, Alignment::Type vertical)
			{
				Win7ButtonElements button;
				button.rectBorderElement=0;
				button.roundBorderElement=0;
				{
					button.mainComposition=new GuiBoundsComposition;
					button.mainComposition->SetMinSizeLimitation(GuiGraphicsComposition::LimitToElementAndChildren);
				}
				if(roundBorder)
				{
					{
						GuiSolidBackgroundElement* element=GuiSolidBackgroundElement::Create();
						button.backgroundElement=element;

						GuiBoundsComposition* composition=new GuiBoundsComposition;
						button.backgroundComposition=composition;
						button.mainComposition->AddChild(composition);
						composition->SetAlignmentToParent(Margin(1, 1, 1, 1));
						composition->SetOwnedElement(element);
					}
					{
						GuiRoundBorderElement* element=GuiRoundBorderElement::Create();
						button.roundBorderElement=element;
						element->SetRadius(2);

						GuiBoundsComposition* composition=new GuiBoundsComposition;
						button.mainComposition->AddChild(composition);
						composition->SetAlignmentToParent(Margin(0, 0, 0, 0));
						composition->SetOwnedElement(element);
					}
				}
				else
				{
					{
						GuiSolidBorderElement* element=GuiSolidBorderElement::Create();
						button.rectBorderElement=element;

						GuiBoundsComposition* composition=new GuiBoundsComposition;
						button.mainComposition->AddChild(composition);
						composition->SetAlignmentToParent(Margin(0, 0, 0, 0));
						composition->SetOwnedElement(element);
					}
					{
						GuiSolidBackgroundElement* element=GuiSolidBackgroundElement::Create();
						button.backgroundElement=element;

						GuiBoundsComposition* composition=new GuiBoundsComposition;
						button.backgroundComposition=composition;
						button.mainComposition->AddChild(composition);
						composition->SetAlignmentToParent(Margin(1, 1, 1, 1));
						composition->SetOwnedElement(element);
					}
				}
				{
					GuiTableComposition* table=new GuiTableComposition;
					button.gradientComposition=table;
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
					Win7CreateSolidLabelElement(button.textElement, button.textComposition, horizontal, vertical);
					button.mainComposition->AddChild(button.textComposition);
				}
				return button;
			}

			void Win7ButtonElements::Apply(const Win7ButtonColors& colors)
			{
				if(rectBorderElement)
				{
					rectBorderElement->SetColor(colors.borderColor);
				}
				if(roundBorderElement)
				{
					roundBorderElement->SetColor(colors.borderColor);
				}
				backgroundElement->SetColor(colors.backgroundColor);
				topGradientElement->SetColors(colors.g1, colors.g2);
				bottomGradientElement->SetColors(colors.g3, colors.g4);
				textElement->SetColor(colors.textColor);
			}

/***********************************************************************
Win7CheckedButtonElements
***********************************************************************/

			Win7CheckedButtonElements Win7CheckedButtonElements::Create(elements::ElementShape::Type shape, bool backgroundVisible)
			{
				const int checkSize=13;
				const int checkPadding=2;

				Win7CheckedButtonElements button;
				{
					button.mainComposition=new GuiBoundsComposition;
					button.mainComposition->SetMinSizeLimitation(GuiGraphicsComposition::LimitToElementAndChildren);
				}
				{
					GuiTableComposition* mainTable=new GuiTableComposition;
					button.mainComposition->AddChild(mainTable);
					if(backgroundVisible)
					{
						GuiSolidBackgroundElement* element=GuiSolidBackgroundElement::Create();
						element->SetColor(Win7GetSystemWindowColor());
						mainTable->SetOwnedElement(element);
					}
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
						GuiCellComposition* textCell=new GuiCellComposition;
						mainTable->AddChild(textCell);
						textCell->SetSite(0, 1, 1, 1);
						{
							Win7CreateSolidLabelElement(button.textElement, button.textComposition, Alignment::Left, Alignment::Center);
							textCell->AddChild(button.textComposition);
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
Win7MenuItemButtonElements
***********************************************************************/

			Win7MenuItemButtonElements Win7MenuItemButtonElements::Create()
			{
				Win7MenuItemButtonElements button;
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
					GuiGradientBackgroundElement* element=GuiGradientBackgroundElement::Create();
					button.gradientElement=element;
					element->SetDirection(GuiGradientBackgroundElement::Vertical);

					GuiBoundsComposition* composition=new GuiBoundsComposition;
					button.mainComposition->AddChild(composition);
					composition->SetAlignmentToParent(Margin(2, 2, 2, 2));
					composition->SetOwnedElement(element);
				}
				{
					GuiTableComposition* table=new GuiTableComposition;
					button.mainComposition->AddChild(table);
					table->SetAlignmentToParent(Margin(2, 0, 2, 0));
					table->SetRowsAndColumns(1, 4);

					table->SetRowOption(0, GuiCellOption::PercentageOption(1.0));
					table->SetColumnOption(0, GuiCellOption::AbsoluteOption(24));
					table->SetColumnOption(1, GuiCellOption::AbsoluteOption(2));
					table->SetColumnOption(2, GuiCellOption::PercentageOption(1.0));
					table->SetColumnOption(3, GuiCellOption::AbsoluteOption(10));

					{
						GuiCellComposition* cell=new GuiCellComposition;
						table->AddChild(cell);
						cell->SetSite(0, 1, 1, 1);
						button.splitterComposition=cell;

						Gui3DSplitterElement* element=Gui3DSplitterElement::Create();
						button.splitterElement=element;
						element->SetDirection(Gui3DSplitterElement::Vertical);
						cell->SetOwnedElement(element);
					}
					{
						GuiCellComposition* cell=new GuiCellComposition;
						table->AddChild(cell);
						cell->SetSite(0, 2, 1, 1);

						Win7CreateSolidLabelElement(button.textElement, button.textComposition, Alignment::Left, Alignment::Center);
						cell->AddChild(button.textComposition);
					}
					{
						GuiSolidLabelElement* element=GuiSolidLabelElement::Create();
						button.subMenuTextElement=element;
						element->SetAlignments(Alignment::Center, Alignment::Center);
						{
							FontProperties font;
							font.fontFamily=L"Wingdings 3";
							font.size=10;
							element->SetFont(font);
						}
						element->SetText((wchar_t)0x7D);

						GuiCellComposition* cell=new GuiCellComposition;
						button.subMenuTextComposition=cell;
						cell->SetMinSizeLimitation(GuiGraphicsComposition::LimitToElement);
						table->AddChild(cell);
						cell->SetSite(0, 3, 1, 1);
						cell->SetOwnedElement(element);
						cell->SetVisible(false);
					}
				}
				return button;
			}

			void Win7MenuItemButtonElements::Apply(const Win7ButtonColors& colors)
			{
				borderElement->SetColor(colors.borderColor);
				backgroundElement->SetColor(colors.backgroundColor);
				gradientElement->SetColors(colors.g1, colors.g2);
				splitterElement->SetColors(colors.g3, colors.g4);
				textElement->SetColor(colors.textColor);
				subMenuTextElement->SetColor(colors.textColor);
			}

			void Win7MenuItemButtonElements::SetActive(bool value)
			{
				if(value)
				{
					splitterComposition->SetMargin(Margin(0, 1, 0, 2));
				}
				else
				{
					splitterComposition->SetMargin(Margin(0, 0, 0, 0));
				}
			}

			void Win7MenuItemButtonElements::SetSubMenuExisting(bool value)
			{
				subMenuTextComposition->SetVisible(value);
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

			Color Win7GetSystemTabContentColor()
			{
				return Color(255, 255, 255);
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
				int margin=3;
				element->SetFont(fontProperties);
				composition->SetMargin(Margin(margin, margin, margin, margin));
			}

			void Win7CreateSolidLabelElement(GuiSolidLabelElement*& element, GuiBoundsComposition*& composition, Alignment::Type horizontal, Alignment::Type vertical)
			{
				element=GuiSolidLabelElement::Create();
				element->SetAlignments(horizontal, vertical);

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
				,enableAnimation(true)\
			{\
			}\
			void TSTYLECONTROLLER::TransferringAnimation::Stop()\
			{\
				stopped=true;\
			}\
			bool TSTYLECONTROLLER::TransferringAnimation::GetEnableAnimation()\
			{\
				return enableAnimation;\
			}\
			void TSTYLECONTROLLER::TransferringAnimation::SetEnableAnimation(bool value)\
			{\
				enableAnimation=value;\
			}\
			void TSTYLECONTROLLER::TransferringAnimation::Transfer(const TSTATE& end)\
			{\
				if(colorEnd!=end)\
				{\
					GuiGraphicsHost* host=HOST_GETTER(style);\
					if(enableAnimation && host)\
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
Win7EmptyStyle
***********************************************************************/

			Win7EmptyStyle::Win7EmptyStyle(Color color)
			{
				GuiSolidBackgroundElement* element=GuiSolidBackgroundElement::Create();
				element->SetColor(color);
				
				boundsComposition=new GuiBoundsComposition;
				boundsComposition->SetOwnedElement(element);
			}

			Win7EmptyStyle::~Win7EmptyStyle()
			{
			}

			elements::GuiBoundsComposition* Win7EmptyStyle::GetBoundsComposition()
			{
				return boundsComposition;
			}

			elements::GuiGraphicsComposition* Win7EmptyStyle::GetContainerComposition()
			{
				return boundsComposition;
			}

			void Win7EmptyStyle::SetFocusableComposition(elements::GuiGraphicsComposition* value)
			{
			}

			void Win7EmptyStyle::SetText(const WString& value)
			{
			}

			void Win7EmptyStyle::SetFont(const FontProperties& value)
			{
			}

			void Win7EmptyStyle::SetVisuallyEnabled(bool value)
			{
			}

/***********************************************************************
Win7WindowStyle
***********************************************************************/

			Win7WindowStyle::Win7WindowStyle()
				:Win7EmptyStyle(Win7GetSystemWindowColor())
			{
			}

			Win7WindowStyle::~Win7WindowStyle()
			{
			}

/***********************************************************************
Win7MenuStyle
***********************************************************************/

			Win7MenuStyle::Win7MenuStyle()
			{
				{
					GuiSolidBorderElement* element=GuiSolidBorderElement::Create();
					element->SetColor(Color(151, 151, 151));
					boundsComposition=new GuiBoundsComposition;
					boundsComposition->SetOwnedElement(element);
				}
				{
					GuiSolidBackgroundElement* element=GuiSolidBackgroundElement::Create();
					element->SetColor(Color(245, 245, 245));
					GuiBoundsComposition* subBorder=new GuiBoundsComposition;
					subBorder->SetOwnedElement(element);
					subBorder->SetAlignmentToParent(Margin(1, 1, 1, 1));
					boundsComposition->AddChild(subBorder);
				}
				{
					GuiSolidBackgroundElement* element=GuiSolidBackgroundElement::Create();
					element->SetColor(Win7GetSystemWindowColor());
					containerComposition=new GuiBoundsComposition;
					containerComposition->SetOwnedElement(element);
					containerComposition->SetAlignmentToParent(Margin(3, 3, 3, 3));
					containerComposition->SetMinSizeLimitation(GuiGraphicsComposition::LimitToElementAndChildren);
					boundsComposition->AddChild(containerComposition);
				}
			}

			Win7MenuStyle::~Win7MenuStyle()
			{
			}

			elements::GuiBoundsComposition* Win7MenuStyle::GetBoundsComposition()
			{
				return boundsComposition;
			}

			elements::GuiGraphicsComposition* Win7MenuStyle::GetContainerComposition()
			{
				return containerComposition;
			}

			void Win7MenuStyle::SetFocusableComposition(elements::GuiGraphicsComposition* value)
			{
			}

			void Win7MenuStyle::SetText(const WString& value)
			{
			}

			void Win7MenuStyle::SetFont(const FontProperties& value)
			{
			}

			void Win7MenuStyle::SetVisuallyEnabled(bool value)
			{
			}

/***********************************************************************
Win7MenuBarStyle
***********************************************************************/

			Win7MenuBarStyle::Win7MenuBarStyle()
			{
				GuiTableComposition* table=new GuiTableComposition;
				table->SetAlignmentToParent(Margin(0, 0, 0, 1));
				table->SetRowsAndColumns(2, 1);
				table->SetRowOption(0, GuiCellOption::PercentageOption(0.5));
				table->SetRowOption(1, GuiCellOption::PercentageOption(0.5));
				table->SetColumnOption(0, GuiCellOption::PercentageOption(1.0));
				{
					GuiGradientBackgroundElement* gradient=GuiGradientBackgroundElement::Create();
					gradient->SetDirection(GuiGradientBackgroundElement::Vertical);
					gradient->SetColors(Color(252, 253, 254), Color(229, 234, 245));

					GuiCellComposition* cell=new GuiCellComposition;
					table->AddChild(cell);
					cell->SetSite(0, 0, 1, 1);
					cell->SetOwnedElement(gradient);
				}
				{
					GuiGradientBackgroundElement* gradient=GuiGradientBackgroundElement::Create();
					gradient->SetDirection(GuiGradientBackgroundElement::Vertical);
					gradient->SetColors(Color(212, 219, 237), Color(225, 230, 246));

					GuiCellComposition* cell=new GuiCellComposition;
					table->AddChild(cell);
					cell->SetSite(1, 0, 1, 1);
					cell->SetOwnedElement(gradient);
				}

				boundsComposition=new GuiBoundsComposition;
				{
					GuiSolidBackgroundElement* solid=GuiSolidBackgroundElement::Create();
					solid->SetColor(Color(182, 188, 204));
					boundsComposition->SetOwnedElement(solid);
				}
				boundsComposition->AddChild(table);
			}

			Win7MenuBarStyle::~Win7MenuBarStyle()
			{
			}

			elements::GuiBoundsComposition* Win7MenuBarStyle::GetBoundsComposition()
			{
				return boundsComposition;
			}

			elements::GuiGraphicsComposition* Win7MenuBarStyle::GetContainerComposition()
			{
				return boundsComposition;
			}

			void Win7MenuBarStyle::SetFocusableComposition(elements::GuiGraphicsComposition* value)
			{
			}

			void Win7MenuBarStyle::SetText(const WString& value)
			{
			}

			void Win7MenuBarStyle::SetFont(const FontProperties& value)
			{
			}

			void Win7MenuBarStyle::SetVisuallyEnabled(bool value)
			{
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
Win7TabStyle
***********************************************************************/

			void Win7TabStyle::OnHeaderButtonClicked(elements::GuiGraphicsComposition* sender, elements::GuiEventArgs& arguments)
			{
				if(commandExecutor)
				{
					int index=headerButtons.IndexOf(dynamic_cast<GuiSelectableButton*>(sender->GetAssociatedControl()));
					if(index!=-1)
					{
						commandExecutor->ShowTab(index);
					}
				}
			}

			void Win7TabStyle::UpdateHeaderZOrder()
			{
				int itemCount=tabHeaderComposition->GetStackItems().Count();
				int childCount=tabHeaderComposition->Children().Count();
				for(int i=0;i<itemCount;i++)
				{
					GuiStackItemComposition* item=tabHeaderComposition->GetStackItems()[i];
					if(headerButtons[i]->GetSelected())
					{
						tabHeaderComposition->MoveChild(item, childCount-1);
						item->SetExtraMargin(Margin(2, 2, 2, 0));
					}
					else
					{
						item->SetExtraMargin(Margin(0, 0, 0, 0));
					}
				}
				tabHeaderComposition->MoveChild(tabContentTopLineComposition, childCount-2);
			}

			Win7TabStyle::Win7TabStyle()
				:commandExecutor(0)
			{
				boundsComposition=new GuiTableComposition;
				boundsComposition->SetRowsAndColumns(2, 1);
				boundsComposition->SetRowOption(0, GuiCellOption::MinSizeOption());
				boundsComposition->SetRowOption(1, GuiCellOption::PercentageOption(1.0));
				boundsComposition->SetColumnOption(0, GuiCellOption::PercentageOption(1.0));
				{
					GuiCellComposition* cell=new GuiCellComposition;
					boundsComposition->AddChild(cell);
					cell->SetSite(0, 0, 1, 1);

					tabHeaderComposition=new GuiStackComposition;
					tabHeaderComposition->SetExtraMargin(Margin(2, 2, 2, 0));
					tabHeaderComposition->SetAlignmentToParent(Margin(0, 0, 0, 0));
					tabHeaderComposition->SetMinSizeLimitation(GuiGraphicsComposition::LimitToElementAndChildren);
					cell->AddChild(tabHeaderComposition);
				}
				{
					GuiSolidBackgroundElement* element=GuiSolidBackgroundElement::Create();
					element->SetColor(Win7ButtonColors::TabPageHeaderNormal().borderColor);

					tabContentTopLineComposition=new GuiBoundsComposition;
					tabContentTopLineComposition->SetOwnedElement(element);
					tabContentTopLineComposition->SetAlignmentToParent(Margin(0, -1, 0, 0));
					tabContentTopLineComposition->SetPreferredMinSize(Size(0, 1));
					tabHeaderComposition->AddChild(tabContentTopLineComposition);
				}
				{
					GuiCellComposition* cell=new GuiCellComposition;
					boundsComposition->AddChild(cell);
					cell->SetSite(1, 0, 1, 1);

					containerComposition=new GuiBoundsComposition;
					containerComposition->SetAlignmentToParent(Margin(1, 0, 1, 1));
					containerComposition->SetMinSizeLimitation(GuiGraphicsComposition::LimitToElementAndChildren);
					cell->AddChild(containerComposition);

					{
						GuiSolidBorderElement* element=GuiSolidBorderElement::Create();
						element->SetColor(Win7ButtonColors::TabPageHeaderNormal().borderColor);
						cell->SetOwnedElement(element);
					}
					{
						GuiSolidBackgroundElement* element=GuiSolidBackgroundElement::Create();
						element->SetColor(Win7GetSystemTabContentColor());
						containerComposition->SetOwnedElement(element);
					}
				}

				headerController=new GuiSelectableButton::MutexGroupController;
			}

			Win7TabStyle::~Win7TabStyle()
			{
			}

			elements::GuiBoundsComposition* Win7TabStyle::GetBoundsComposition()
			{
				return boundsComposition;
			}

			elements::GuiGraphicsComposition* Win7TabStyle::GetContainerComposition()
			{
				return containerComposition;
			}

			void Win7TabStyle::SetFocusableComposition(elements::GuiGraphicsComposition* value)
			{
			}

			void Win7TabStyle::SetText(const WString& value)
			{
			}

			void Win7TabStyle::SetFont(const FontProperties& value)
			{
				headerFont=value;
			}

			void Win7TabStyle::SetVisuallyEnabled(bool value)
			{
			}

			void Win7TabStyle::SetCommandExecutor(controls::GuiTab::ICommandExecutor* value)
			{
				commandExecutor=value;
			}

			void Win7TabStyle::InsertTab(int index)
			{
				GuiSelectableButton* button=new GuiSelectableButton(new Win7TabPageHeaderStyle);
				button->SetAutoSelection(false);
				button->SetFont(headerFont);
				button->GetBoundsComposition()->SetAlignmentToParent(Margin(0, 0, 0, 0));
				button->SetGroupController(headerController.Obj());
				button->Clicked.AttachMethod(this, &Win7TabStyle::OnHeaderButtonClicked);

				GuiStackItemComposition* item=new GuiStackItemComposition;
				item->AddChild(button->GetBoundsComposition());
				tabHeaderComposition->InsertStackItem(index, item);
				headerButtons.Insert(index, button);
				UpdateHeaderZOrder();
			}

			void Win7TabStyle::SetTabText(int index, const WString& value)
			{
				headerButtons[index]->SetText(value);
			}

			void Win7TabStyle::RemoveTab(int index)
			{
				GuiStackItemComposition* item=tabHeaderComposition->GetStackItems()[index];
				GuiSelectableButton* button=headerButtons[index];

				tabHeaderComposition->RemoveChild(item);
				item->RemoveChild(button->GetBoundsComposition());
				headerButtons.RemoveAt(index);

				delete item;
				delete button;
			}

			void Win7TabStyle::SetSelectedTab(int index)
			{
				headerButtons[index]->SetSelected(true);
				UpdateHeaderZOrder();
			}

			controls::GuiControl::IStyleController* Win7TabStyle::CreateTabPageStyleController()
			{
				GuiControl::IStyleController* style=new Win7EmptyStyle(Win7GetSystemTabContentColor());
				style->GetBoundsComposition()->SetAlignmentToParent(Margin(2, 2, 2, 2));
				return style;
			}

/***********************************************************************
Win7ButtonStyleBase
***********************************************************************/

			IMPLEMENT_TRANSFERRING_ANIMATION(Win7ButtonColors, Win7ButtonStyleBase)
			{
				colorCurrent=Win7ButtonColors::Blend(colorBegin, colorEnd, currentPosition, totalLength);
				style->elements.Apply(colorCurrent);
			}

			Win7ButtonStyleBase::Win7ButtonStyleBase(bool verticalGradient, bool roundBorder, const Win7ButtonColors& initialColor, Alignment::Type horizontal, Alignment::Type vertical)
				:controlStyle(GuiButton::Normal)
				,isVisuallyEnabled(true)
				,isSelected(false)
				,transparentWhenInactive(false)
				,transparentWhenDisabled(false)
			{
				elements=Win7ButtonElements::Create(verticalGradient, roundBorder, horizontal, vertical);
				elements.Apply(initialColor);
				transferringAnimation=new TransferringAnimation(this, initialColor);
			}

			Win7ButtonStyleBase::~Win7ButtonStyleBase()
			{
			}

			elements::GuiBoundsComposition* Win7ButtonStyleBase::GetBoundsComposition()
			{
				return elements.mainComposition;
			}

			elements::GuiGraphicsComposition* Win7ButtonStyleBase::GetContainerComposition()
			{
				return elements.mainComposition;
			}

			void Win7ButtonStyleBase::SetFocusableComposition(elements::GuiGraphicsComposition* value)
			{
			}

			void Win7ButtonStyleBase::SetText(const WString& value)
			{
				elements.textElement->SetText(value);
			}

			void Win7ButtonStyleBase::SetFont(const FontProperties& value)
			{
				Win7SetFont(elements.textElement, elements.textComposition, value);
			}

			void Win7ButtonStyleBase::SetVisuallyEnabled(bool value)
			{
				if(isVisuallyEnabled!=value)
				{
					isVisuallyEnabled=value;
					TransferInternal(controlStyle, isVisuallyEnabled, isSelected);
				}
			}

			void Win7ButtonStyleBase::SetSelected(bool value)
			{
				if(isSelected!=value)
				{
					isSelected=value;
					TransferInternal(controlStyle, isVisuallyEnabled, isSelected);
				}
			}

			void Win7ButtonStyleBase::Transfer(GuiButton::ControlState value)
			{
				if(controlStyle!=value)
				{
					controlStyle=value;
					TransferInternal(controlStyle, isVisuallyEnabled, isSelected);
				}
			}

			bool Win7ButtonStyleBase::GetTransparentWhenInactive()
			{
				return transparentWhenInactive;
			}

			void Win7ButtonStyleBase::SetTransparentWhenInactive(bool value)
			{
				transparentWhenInactive=value;
				TransferInternal(controlStyle, isVisuallyEnabled, isSelected);
			}

			bool Win7ButtonStyleBase::GetTransparentWhenDisabled()
			{
				return transparentWhenDisabled;
			}

			void Win7ButtonStyleBase::SetTransparentWhenDisabled(bool value)
			{
				transparentWhenDisabled=value;
				TransferInternal(controlStyle, isVisuallyEnabled, isSelected);
			}

			bool Win7ButtonStyleBase::GetAutoSizeForText()
			{
				return elements.textComposition->GetMinSizeLimitation()!=GuiGraphicsComposition::NoLimit;
			}

			void Win7ButtonStyleBase::SetAutoSizeForText(bool value)
			{
				if(value)
				{
					elements.textComposition->SetMinSizeLimitation(GuiGraphicsComposition::LimitToElement);
				}
				else
				{
					elements.textComposition->SetMinSizeLimitation(GuiGraphicsComposition::NoLimit);
				}
			}

/***********************************************************************
Win7ButtonStyle
***********************************************************************/

			void Win7ButtonStyle::TransferInternal(GuiButton::ControlState value, bool enabled, bool selected)
			{
				Win7ButtonColors targetColor;
				if(enabled)
				{
					switch(value)
					{
					case GuiButton::Normal:
						targetColor=Win7ButtonColors::ButtonNormal();
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
					targetColor=Win7ButtonColors::ButtonDisabled();
					if(transparentWhenDisabled)
					{
						targetColor.SetAlphaWithoutText(0);
					}
				}
				transferringAnimation->Transfer(targetColor);
			}

			Win7ButtonStyle::Win7ButtonStyle(bool verticalGradient)
				:Win7ButtonStyleBase(verticalGradient, true, Win7ButtonColors::ButtonNormal(), Alignment::Center, Alignment::Center)
			{
			}

			Win7ButtonStyle::~Win7ButtonStyle()
			{
			}

/***********************************************************************
Win7SelectableItemStyle
***********************************************************************/

			void Win7SelectableItemStyle::TransferInternal(GuiButton::ControlState value, bool enabled, bool selected)
			{
				if(!enabled)
				{
					transferringAnimation->Transfer(Win7ButtonColors::ItemDisabled());
				}
				else if(selected)
				{
					transferringAnimation->Transfer(Win7ButtonColors::ItemSelected());
				}
				else
				{
					switch(value)
					{
					case GuiButton::Normal:
						transferringAnimation->Transfer(Win7ButtonColors::ItemNormal());
						break;
					case GuiButton::Active:
						transferringAnimation->Transfer(Win7ButtonColors::ItemActive());
						break;
					case GuiButton::Pressed:
						transferringAnimation->Transfer(Win7ButtonColors::ItemSelected());
						break;
					}
				}
			}

			Win7SelectableItemStyle::Win7SelectableItemStyle()
				:Win7ButtonStyleBase(true, true, Win7ButtonColors::ItemNormal(), Alignment::Left, Alignment::Center)
			{
				transferringAnimation->SetEnableAnimation(false);
			}

			Win7SelectableItemStyle::~Win7SelectableItemStyle()
			{
			}

/***********************************************************************
Win7TabPageHeaderStyle
***********************************************************************/

			void Win7TabPageHeaderStyle::TransferInternal(GuiButton::ControlState value, bool enabled, bool selected)
			{
				if(selected)
				{
					transferringAnimation->Transfer(Win7ButtonColors::TabPageHeaderSelected());
				}
				else
				{
					switch(value)
					{
					case GuiButton::Normal:
						transferringAnimation->Transfer(Win7ButtonColors::TabPageHeaderNormal());
						break;
					case GuiButton::Active:
					case GuiButton::Pressed:
						transferringAnimation->Transfer(Win7ButtonColors::TabPageHeaderActive());
						break;
					}
				}
			}

			Win7TabPageHeaderStyle::Win7TabPageHeaderStyle()
				:Win7ButtonStyleBase(true, false, Win7ButtonColors::TabPageHeaderNormal(), Alignment::Left, Alignment::Center)
			{
				transferringAnimation->SetEnableAnimation(false);
				{
					Margin margin=elements.backgroundComposition->GetAlignmentToParent();
					margin.bottom=0;
					elements.backgroundComposition->SetAlignmentToParent(margin);
				}
				{
					Margin margin=elements.gradientComposition->GetAlignmentToParent();
					margin.bottom=0;
					elements.gradientComposition->SetAlignmentToParent(margin);
				}
			}

			Win7TabPageHeaderStyle::~Win7TabPageHeaderStyle()
			{
			}

			void Win7TabPageHeaderStyle::SetFont(const FontProperties& value)
			{
				Win7ButtonStyleBase::SetFont(value);
				Margin margin=elements.textComposition->GetMargin();
				margin.left*=2;
				margin.right*=2;
				elements.textComposition->SetMargin(margin);
			}

/***********************************************************************
Win7ToolstripButtonStyle
***********************************************************************/

			void Win7ToolstripButtonStyle::TransferInternal(GuiButton::ControlState value, bool enabled, bool selected)
			{
				Win7ButtonColors targetColor;
				if(enabled)
				{
					switch(value)
					{
					case GuiButton::Normal:
						targetColor=Win7ButtonColors::ToolstripButtonNormal();
						if(transparentWhenInactive)
						{
							targetColor.SetAlphaWithoutText(0);
						}
						break;
					case GuiButton::Active:
						targetColor=Win7ButtonColors::ToolstripButtonActive();
						break;
					case GuiButton::Pressed:
						targetColor=Win7ButtonColors::ToolstripButtonPressed();
						break;
					}
				}
				else
				{
					targetColor=Win7ButtonColors::ToolstripButtonDisabled();
					if(transparentWhenDisabled)
					{
						targetColor.SetAlphaWithoutText(0);
					}
				}
				transferringAnimation->Transfer(targetColor);
			}

			Win7ToolstripButtonStyle::Win7ToolstripButtonStyle(bool transparent)
				:Win7ButtonStyleBase(true, true, Win7ButtonColors::ToolstripButtonNormal(), Alignment::Center, Alignment::Center)
			{
				SetAutoSizeForText(false);
				SetTransparentWhenInactive(transparent);
				SetTransparentWhenDisabled(transparent);
			}

			Win7ToolstripButtonStyle::~Win7ToolstripButtonStyle()
			{
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

			Win7CheckBoxStyle::Win7CheckBoxStyle(BulletStyle bulletStyle, bool backgroundVisible)
				:controlStyle(GuiButton::Normal)
				,isVisuallyEnabled(true)
				,isSelected(false)
			{
				Win7ButtonColors initialColor=Win7ButtonColors::CheckedNormal(isSelected);
				elements=Win7CheckedButtonElements::Create(bulletStyle==CheckBox?ElementShape::Rectangle:ElementShape::Ellipse, backgroundVisible);
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
Win7MenuBarButtonStyle
***********************************************************************/

			void Win7MenuBarButtonStyle::TransferInternal(GuiButton::ControlState value, bool enabled, bool opening)
			{
				Win7ButtonColors targetColor;
				if(!enabled)
				{
					targetColor=Win7ButtonColors::MenuBarButtonDisabled();
					targetColor.SetAlphaWithoutText(0);
				}
				else if(opening)
				{
					targetColor=Win7ButtonColors::MenuBarButtonPressed();
				}
				else
				{
					switch(value)
					{
					case GuiButton::Normal:
						targetColor=Win7ButtonColors::MenuBarButtonNormal();
						targetColor.SetAlphaWithoutText(0);
						break;
					case GuiButton::Active:
						targetColor=Win7ButtonColors::MenuBarButtonActive();
						break;
					case GuiButton::Pressed:
						targetColor=Win7ButtonColors::MenuBarButtonPressed();
						break;
					}
				}
				elements.Apply(targetColor);
			}

			Win7MenuBarButtonStyle::Win7MenuBarButtonStyle()
				:controlStyle(GuiButton::Normal)
				,isVisuallyEnabled(true)
				,isOpening(false)
			{
				Win7ButtonColors initialColor=Win7ButtonColors::MenuBarButtonNormal();
				initialColor.SetAlphaWithoutText(0);

				elements=Win7ButtonElements::Create(true, true, Alignment::Center, Alignment::Center);
				elements.Apply(initialColor);
			}

			Win7MenuBarButtonStyle::~Win7MenuBarButtonStyle()
			{
			}

			elements::GuiBoundsComposition* Win7MenuBarButtonStyle::GetBoundsComposition()
			{
				return elements.mainComposition;
			}

			elements::GuiGraphicsComposition* Win7MenuBarButtonStyle::GetContainerComposition()
			{
				return elements.mainComposition;
			}

			void Win7MenuBarButtonStyle::SetFocusableComposition(elements::GuiGraphicsComposition* value)
			{
			}

			void Win7MenuBarButtonStyle::SetText(const WString& value)
			{
				elements.textElement->SetText(value);
			}

			void Win7MenuBarButtonStyle::SetFont(const FontProperties& value)
			{
				Win7SetFont(elements.textElement, elements.textComposition, value);
				Margin margin=elements.textComposition->GetMargin();
				margin.left*=3;
				margin.right*=3;
				elements.textComposition->SetMargin(margin);
			}

			void Win7MenuBarButtonStyle::SetVisuallyEnabled(bool value)
			{
				if(isVisuallyEnabled!=value)
				{
					isVisuallyEnabled=value;
					TransferInternal(controlStyle, isVisuallyEnabled, isOpening);
				}
			}

			controls::GuiMenu::IStyleController* Win7MenuBarButtonStyle::CreateSubMenuStyleController()
			{
				return new Win7MenuStyle;
			}

			void Win7MenuBarButtonStyle::SetSubMenuExisting(bool value)
			{
			}

			void Win7MenuBarButtonStyle::SetSubMenuOpening(bool value)
			{
				if(isOpening!=value)
				{
					isOpening=value;
					TransferInternal(controlStyle, isVisuallyEnabled, isOpening);
				}
			}

			void Win7MenuBarButtonStyle::Transfer(GuiButton::ControlState value)
			{
				if(controlStyle!=value)
				{
					controlStyle=value;
					TransferInternal(controlStyle, isVisuallyEnabled, isOpening);
				}
			}

/***********************************************************************
Win7MenuItemButtonStyle
***********************************************************************/

			void Win7MenuItemButtonStyle::TransferInternal(GuiButton::ControlState value, bool enabled, bool opening)
			{
				Win7ButtonColors targetColor;
				bool active=false;
				if(enabled)
				{
					if(opening)
					{
						targetColor=Win7ButtonColors::MenuItemButtonNormalActive();
						active=true;
					}
					else
					{
						switch(value)
						{
						case GuiButton::Normal:
							targetColor=Win7ButtonColors::MenuItemButtonNormal();
							break;
						case GuiButton::Active:
						case GuiButton::Pressed:
							targetColor=Win7ButtonColors::MenuItemButtonNormalActive();
							active=true;
							break;
						}
					}
				}
				else
				{
					switch(value)
					{
					case GuiButton::Normal:
						targetColor=Win7ButtonColors::MenuItemButtonDisabled();
						break;
					case GuiButton::Active:
					case GuiButton::Pressed:
						targetColor=Win7ButtonColors::MenuItemButtonDisabledActive();
						active=true;
						break;
					}
				}
				elements.Apply(targetColor);
				elements.SetActive(active);
			}

			Win7MenuItemButtonStyle::Win7MenuItemButtonStyle()
				:controlStyle(GuiButton::Normal)
				,isVisuallyEnabled(true)
				,isOpening(false)
			{
				elements=Win7MenuItemButtonElements::Create();
				elements.Apply(Win7ButtonColors::MenuItemButtonNormal());
			}

			Win7MenuItemButtonStyle::~Win7MenuItemButtonStyle()
			{
			}

			elements::GuiBoundsComposition* Win7MenuItemButtonStyle::GetBoundsComposition()
			{
				return elements.mainComposition;
			}

			elements::GuiGraphicsComposition* Win7MenuItemButtonStyle::GetContainerComposition()
			{
				return elements.mainComposition;
			}

			void Win7MenuItemButtonStyle::SetFocusableComposition(elements::GuiGraphicsComposition* value)
			{
			}

			void Win7MenuItemButtonStyle::SetText(const WString& value)
			{
				elements.textElement->SetText(value);
			}

			void Win7MenuItemButtonStyle::SetFont(const FontProperties& value)
			{
				Win7SetFont(elements.textElement, elements.textComposition, value);
			}

			void Win7MenuItemButtonStyle::SetVisuallyEnabled(bool value)
			{
				if(isVisuallyEnabled!=value)
				{
					isVisuallyEnabled=value;
					TransferInternal(controlStyle, isVisuallyEnabled, isOpening);
				}
			}

			controls::GuiMenu::IStyleController* Win7MenuItemButtonStyle::CreateSubMenuStyleController()
			{
				return new Win7MenuStyle;
			}

			void Win7MenuItemButtonStyle::SetSubMenuExisting(bool value)
			{
				elements.SetSubMenuExisting(value);
			}

			void Win7MenuItemButtonStyle::SetSubMenuOpening(bool value)
			{
				if(isOpening!=value)
				{
					isOpening=value;
					TransferInternal(controlStyle, isVisuallyEnabled, isOpening);
				}
			}

			void Win7MenuItemButtonStyle::Transfer(GuiButton::ControlState value)
			{
				if(controlStyle!=value)
				{
					controlStyle=value;
					TransferInternal(controlStyle, isVisuallyEnabled, isOpening);
				}
			}

/***********************************************************************
Win7MenuSplitterStyle
***********************************************************************/

			Win7MenuSplitterStyle::Win7MenuSplitterStyle()
			{
				Color dark=Win7ButtonColors::MenuItemButtonNormal().g3;
				Color bright=Win7ButtonColors::MenuItemButtonNormal().g4;

				GuiTableComposition* table=new GuiTableComposition;
				table->SetAlignmentToParent(Margin(0, 0, 0, 0));
				table->SetRowsAndColumns(1, 3);
				table->SetPreferredMinSize(Size(0, 6));

				table->SetRowOption(0, GuiCellOption::PercentageOption(1.0));
				table->SetColumnOption(0, GuiCellOption::AbsoluteOption(26));
				table->SetColumnOption(1, GuiCellOption::AbsoluteOption(2));
				table->SetColumnOption(2, GuiCellOption::MinSizeOption());

				{
					GuiCellComposition* cell=new GuiCellComposition;
					table->AddChild(cell);
					cell->SetSite(0, 1, 1, 1);

					Gui3DSplitterElement* element=Gui3DSplitterElement::Create();
					element->SetDirection(Gui3DSplitterElement::Vertical);
					element->SetColors(dark, bright);
					cell->SetOwnedElement(element);
				}
				{
					GuiCellComposition* cell=new GuiCellComposition;
					table->AddChild(cell);
					cell->SetSite(0, 2, 1, 1);

					Gui3DSplitterElement* element=Gui3DSplitterElement::Create();
					element->SetDirection(Gui3DSplitterElement::Horizontal);
					element->SetColors(dark, bright);
					cell->SetOwnedElement(element);
				}
				boundsComposition=table;
			}

			Win7MenuSplitterStyle::~Win7MenuSplitterStyle()
			{
			}

			elements::GuiBoundsComposition* Win7MenuSplitterStyle::GetBoundsComposition()
			{
				return boundsComposition;
			}

			elements::GuiGraphicsComposition* Win7MenuSplitterStyle::GetContainerComposition()
			{
				return boundsComposition;
			}

			void Win7MenuSplitterStyle::SetFocusableComposition(elements::GuiGraphicsComposition* value)
			{
			}

			void Win7MenuSplitterStyle::SetText(const WString& value)
			{
			}

			void Win7MenuSplitterStyle::SetFont(const FontProperties& value)
			{
			}

			void Win7MenuSplitterStyle::SetVisuallyEnabled(bool value)
			{
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
				if(textBoxController)
				{
					background.InitializeTextElement(textBoxController->GetTextElement());
				}
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

/***********************************************************************
Win7TextListProvider
***********************************************************************/
			
			Win7TextListProvider::Win7TextListProvider()
			{
			}

			Win7TextListProvider::~Win7TextListProvider()
			{
			}

			controls::GuiSelectableButton::IStyleController* Win7TextListProvider::CreateBackgroundStyleController()
			{
				return new Win7SelectableItemStyle;
			}

			controls::GuiSelectableButton::IStyleController* Win7TextListProvider::CreateBulletStyleController()
			{
				return 0;
			}

/***********************************************************************
Win7CheckTextListProvider
***********************************************************************/

			Win7CheckTextListProvider::Win7CheckTextListProvider()
			{
			}

			Win7CheckTextListProvider::~Win7CheckTextListProvider()
			{
			}

			controls::GuiSelectableButton::IStyleController* Win7CheckTextListProvider::CreateBulletStyleController()
			{
				return new Win7CheckBoxStyle(Win7CheckBoxStyle::CheckBox, false);
			}

/***********************************************************************
Win7RadioTextListProvider
***********************************************************************/

			Win7RadioTextListProvider::Win7RadioTextListProvider()
			{
			}

			Win7RadioTextListProvider::~Win7RadioTextListProvider()
			{
			}

			controls::GuiSelectableButton::IStyleController* Win7RadioTextListProvider::CreateBulletStyleController()
			{
				return new Win7CheckBoxStyle(Win7CheckBoxStyle::RadioButton, false);
			}

/***********************************************************************
Win7SinglelineTextBoxProvider
***********************************************************************/

			Win7ListViewProvider::Win7ListViewProvider()
			{
			}

			Win7ListViewProvider::~Win7ListViewProvider()
			{
			}

			controls::GuiSelectableButton::IStyleController* Win7ListViewProvider::CreateItemBackground()
			{
				return new Win7SelectableItemStyle;
			}

			Color Win7ListViewProvider::GetPrimaryTextColor()
			{
				return Win7GetSystemTextColor(true);
			}

			Color Win7ListViewProvider::GetSecondaryTextColor()
			{
				return Win7GetSystemTextColor(false);
			}

			Color Win7ListViewProvider::GetItemSeparatorColor()
			{
				return Color(220, 220, 220);
			}
		}
	}
}