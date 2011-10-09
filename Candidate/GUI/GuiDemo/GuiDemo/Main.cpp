#include "..\..\GUI\GraphicsElement\GuiGraphicsHost.h"

using namespace vl;
using namespace vl::presentation;
using namespace vl::presentation::elements;

GuiBoundsComposition* CreateBorder(GuiGraphicsComposition* parent, Color color, Rect bounds, bool internalMargin)
{
	GuiSolidBorderElement* element=GuiSolidBorderElement::Create();
	element->SetColor(color);

	GuiBoundsComposition* composition=new GuiBoundsComposition;
	composition->SetOwnedElement(element);
	composition->SetBounds(bounds);
	if(internalMargin)
	{
		composition->SetInternalMargin(Margin(1, 1, 1, 1));
	}
	parent->AddChild(composition);

	return composition;
}

GuiBoundsComposition* CreateLabel(GuiGraphicsComposition* parent, Color color, Point location, const WString& text)
{
	GuiSolidLabelElement* element=GuiSolidLabelElement::Create();
	{
		FontProperties font;
		font.size=16;
		font.fontFamily=L"Lucida Console";
		element->SetFont(font);
	}
	element->SetText(text);

	GuiBoundsComposition* composition=new GuiBoundsComposition;
	parent->AddChild(composition);
	composition->SetOwnedElement(element);
	composition->SetBounds(Rect(location, element->GetRenderer()->GetMinSize()));

	return composition;
}

GuiBoundsComposition* CreateLabel(GuiCellComposition* cell, Color color, const WString& text)
{
	GuiSolidLabelElement* element=GuiSolidLabelElement::Create();
	{
		FontProperties font;
		font.size=16;
		font.fontFamily=L"Lucida Console";
		element->SetFont(font);
	}
	element->SetText(text);
	element->SetColor(color);

	GuiBoundsComposition* composition=new GuiBoundsComposition;
	cell->AddChild(composition);
	composition->SetOwnedElement(element);
	composition->SetMargin(Margin(10, 10, 10, 10));
	composition->SetMinSizeLimitation(GuiGraphicsComposition::LimitToElement);
	return composition;
}

GuiBoundsComposition* CreateCenteredLabel(GuiCellComposition* cell, Color color, const WString& text)
{
	GuiBoundsComposition* composition=CreateLabel(cell, color, text);
	composition->SetAlignmentToParent(Margin(0, 0, 0, 0));
	composition->GetOwnedElement().Cast<GuiSolidLabelElement>()->SetAlignments(Alignment::Center, Alignment::Center);
	return composition;
}

struct ButtonColors
{
	Color		borderColor;
	Color		backgroundColor;
	Color		g1;
	Color		g2;
	Color		g3;
	Color		g4;
	Color		textColor;

	static ButtonColors Normal()
	{
		ButtonColors colors=
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

	static ButtonColors Active()
	{
		ButtonColors colors=
		{
			Color(60, 127, 177),//Color(204, 240, 255),
			Color(220, 244, 254),
			Color(246, 252, 255),
			Color(240, 250, 255),
			Color(225, 245, 254),
			Color(215, 245, 254),
			Color(0, 0, 0),
		};
		return colors;
	}

	static ButtonColors Selected()
	{
		ButtonColors colors=
		{
			Color(60, 127, 177),//Color(128, 190, 247),
			Color(232, 248, 255),
			Color(225, 246, 255),
			Color(204, 239, 254),
			Color(181, 231, 253),
			Color(164, 225, 251),
			Color(0, 0, 0),
		};
		return colors;
	}

	static ButtonColors Pressed()
	{
		ButtonColors colors=
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

	static ButtonColors Disabled()
	{
		ButtonColors colors=
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
};

struct ButtonComposition
{
	GuiRoundBorderElement*					borderElement;
	GuiSolidBackgroundElement*				backgroundElement;
	GuiGradientBackgroundElement*			topGradientElement;
	GuiGradientBackgroundElement*			bottomGradientElement;
	GuiSolidLabelElement*					textElement;
	GuiBoundsComposition*					mainComposition;

	static ButtonComposition Create(GuiGraphicsComposition* parent, const WString& text)
	{
		ButtonComposition button;
		{
			button.mainComposition=new GuiBoundsComposition;
			parent->AddChild(button.mainComposition);
			button.mainComposition->SetAlignmentToParent(Margin(0, 0, 0, 0));
			button.mainComposition->SetMinSizeLimitation(GuiGraphicsComposition::LimitToElementAndChildren);
		}
		{
			GuiRoundBorderElement* element=GuiRoundBorderElement::Create();
			element->SetRadius(2);
			button.borderElement=element;

			GuiBoundsComposition* composition=new GuiBoundsComposition;
			button.mainComposition->AddChild(composition);
			composition->SetAlignmentToParent(Margin(0, 0, 0, 0));
			composition->SetOwnedElement(element);
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
			GuiTableComposition* table=new GuiTableComposition;
			table->SetAlignmentToParent(Margin(2, 2, 2, 2));
			button.mainComposition->AddChild(table);
			table->SetRowsAndColumns(2, 1);
			table->SetRowOption(0, GuiCellOption::PercentageOption(0.5));
			table->SetRowOption(1, GuiCellOption::PercentageOption(0.5));
			table->SetColumnOption(0, GuiCellOption::PercentageOption(1.0));
			{
				GuiGradientBackgroundElement* element=GuiGradientBackgroundElement::Create();
				element->SetDirection(GuiGradientBackgroundElement::Vertical);
				button.topGradientElement=element;

				GuiCellComposition* cell=new GuiCellComposition;
				table->AddChild(cell);
				cell->SetSite(0, 0, 1, 1);
				cell->SetOwnedElement(element);
			}
			{
				GuiGradientBackgroundElement* element=GuiGradientBackgroundElement::Create();
				element->SetDirection(GuiGradientBackgroundElement::Vertical);
				button.bottomGradientElement=element;

				GuiCellComposition* cell=new GuiCellComposition;
				table->AddChild(cell);
				cell->SetSite(1, 0, 1, 1);
				cell->SetOwnedElement(element);
			}
			table->UpdateCellBounds();
		}
		{
			GuiSolidLabelElement* element=GuiSolidLabelElement::Create();
			{
				FontProperties font;
				font.size=16;
				font.fontFamily=L"Lucida Console";
				element->SetFont(font);
				element->SetText(text);
				button.textElement=element;
			}

			GuiBoundsComposition* composition=new GuiBoundsComposition;
			button.mainComposition->AddChild(composition);
			composition->SetOwnedElement(element);
			composition->SetMargin(Margin(10, 10, 10, 10));
			composition->SetMinSizeLimitation(GuiGraphicsComposition::LimitToElement);
			composition->SetAlignmentToParent(Margin(0, 0, 0, 0));
			composition->GetOwnedElement().Cast<GuiSolidLabelElement>()->SetAlignments(Alignment::Center, Alignment::Center);
		}
		return button;
	}

	void Apply(ButtonColors colors)
	{
		borderElement->SetColor(colors.borderColor);
		backgroundElement->SetColor(colors.backgroundColor);
		topGradientElement->SetColors(colors.g1, colors.g2);
		bottomGradientElement->SetColors(colors.g3, colors.g4);
		textElement->SetColor(colors.textColor);
	}
};

void SetupWindow(GuiGraphicsHost* host)
{
	{
		GuiSolidBackgroundElement* background=GuiSolidBackgroundElement::Create();
		background->SetColor(Color(255, 255, 255));
		host->GetMainComposition()->SetOwnedElement(background);
	}
	{
		GuiTableComposition* table=new GuiTableComposition;
		table->SetAlignmentToParent(Margin(0, 0, 0, 0));
		host->GetMainComposition()->AddChild(table);
		table->SetRowsAndColumns(3, 6);
		table->SetCellPadding(2);
		table->SetMinSizeLimitation(GuiGraphicsComposition::LimitToElementAndChildren);

		{
			table->SetRowOption(0, GuiCellOption::MinSizeOption());
			table->SetRowOption(1, GuiCellOption::PercentageOption(1.0));
			table->SetRowOption(2, GuiCellOption::MinSizeOption());

			table->SetColumnOption(0, GuiCellOption::PercentageOption(1.0));
			table->SetColumnOption(1, GuiCellOption::MinSizeOption());
			table->SetColumnOption(2, GuiCellOption::MinSizeOption());
			table->SetColumnOption(3, GuiCellOption::MinSizeOption());
			table->SetColumnOption(4, GuiCellOption::MinSizeOption());
			table->SetColumnOption(5, GuiCellOption::MinSizeOption());
		}

		{
			GuiCellComposition* cell=new GuiCellComposition;
			table->AddChild(cell);
			cell->SetSite(0, 0, 1, 6);
			CreateLabel(cell, Color(0, 0, 0), L"This is the title");
		}
		{
			GuiCellComposition* cell=new GuiCellComposition;
			table->AddChild(cell);
			cell->SetSite(1, 0, 1, 6);
			{
				GuiSolidBorderElement* element=GuiSolidBorderElement::Create();
				element->SetColor(Color(0, 128, 255));
				cell->SetOwnedElement(element);
				cell->SetInternalMargin(Margin(1, 1, 1, 1));
			}
			CreateCenteredLabel(cell, Color(0, 0, 0), L"This is the content");

			ButtonComposition button=ButtonComposition::Create(cell, L"This is the button with animation");
			button.Apply(ButtonColors::Normal());
			button.mainComposition->SetAlignmentToParent(Margin(-1, 10, 10, -1));
		}

		const wchar_t* buttonTexts[]={L"Normal", L"Active", L"Selected", L"Pressed", L"Disabled"};
		ButtonColors buttonColors[]={ButtonColors::Normal(), ButtonColors::Active(), ButtonColors::Selected(), ButtonColors::Pressed(), ButtonColors::Disabled()};
		for(int i=0;i<5;i++)
		{
			GuiCellComposition* cell=new GuiCellComposition;
			table->AddChild(cell);
			cell->SetSite(2, i+1, 1, 1);
			cell->SetBounds(Rect(Point(0, 0), Size(100, 0)));

			ButtonComposition button=ButtonComposition::Create(cell, buttonTexts[i]);
			button.Apply(buttonColors[i]);
		}
		table->UpdateCellBounds();
	}
}

void GuiMain()
{
	INativeWindow* window=GetCurrentController()->CreateNativeWindow();
	window->SetTitle(L"Vczh GUI Demo");
	window->SetClientSize(Size(800, 600));

	INativeScreen* screen=GetCurrentController()->GetScreen(window);
	Rect windowBounds=window->GetBounds();
	Rect screenBounds=screen->GetClientBounds();
	window->SetBounds(Rect(
		Point(
			screenBounds.Left()+(screenBounds.Width()-windowBounds.Width())/2,
			screenBounds.Top()+(screenBounds.Height()-windowBounds.Height())/2
			),
		windowBounds.GetSize()
		));

	GetCurrentController()->StartTimer();
	GuiGraphicsHost host;
	SetupWindow(&host);
	host.SetNativeWindow(window);

	GetCurrentController()->Run(window);
}