#include "..\..\GUI\GraphicsElement\GuiGraphicsElement.h"
#include "..\..\GUI\GraphicsElement\GuiGraphicsComposition.h"

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

void CreateButton(
	GuiCellComposition* cell,
	Color borderColor,
	Color backgroundColor,
	Color g1,
	Color g2,
	Color g3,
	Color g4,
	const WString& text,
	Color textColor
	)
{
	{
		GuiRoundBorderElement* element=GuiRoundBorderElement::Create();
		element->SetColor(borderColor);
		element->SetRadius(2);

		GuiBoundsComposition* composition=new GuiBoundsComposition;
		cell->AddChild(composition);
		composition->SetAlignmentToParent(Margin(0, 0, 0, 0));
		composition->SetOwnedElement(element);
	}
	{
		GuiSolidBackgroundElement* element=GuiSolidBackgroundElement::Create();
		element->SetColor(backgroundColor);

		GuiBoundsComposition* composition=new GuiBoundsComposition;
		cell->AddChild(composition);
		composition->SetAlignmentToParent(Margin(1, 1, 1, 1));
		composition->SetOwnedElement(element);
	}
	{
		GuiTableComposition* table=new GuiTableComposition;
		table->SetAlignmentToParent(Margin(2, 2, 2, 2));
		cell->AddChild(table);
		table->SetRowsAndColumns(2, 1);
		table->SetRowOption(0, GuiCellOption::PercentageOption(0.5));
		table->SetRowOption(1, GuiCellOption::PercentageOption(0.5));
		table->SetColumnOption(0, GuiCellOption::PercentageOption(1.0));
		{
			GuiGradientBackgroundElement* element=GuiGradientBackgroundElement::Create();
			element->SetDirection(GuiGradientBackgroundElement::Vertical);
			element->SetColors(g1, g2);

			GuiCellComposition* cell=new GuiCellComposition;
			table->AddChild(cell);
			cell->SetSite(0, 0, 1, 1);
			cell->SetOwnedElement(element);
		}
		{
			GuiGradientBackgroundElement* element=GuiGradientBackgroundElement::Create();
			element->SetDirection(GuiGradientBackgroundElement::Vertical);
			element->SetColors(g3, g4);

			GuiCellComposition* cell=new GuiCellComposition;
			table->AddChild(cell);
			cell->SetSite(1, 0, 1, 1);
			cell->SetOwnedElement(element);
		}
		table->UpdateCellBounds();
	}
	CreateLabel(cell, textColor, text);
}

void GuiMain()
{
	INativeWindow* window=GetCurrentController()->CreateNativeWindow();
	window->SetSizeBox(false);
	window->SetMaximizedBox(false);
	window->SetMinimizedBox(false);
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

	{
		GuiWindowComposition windowComposition;
		windowComposition.SetAttachedWindow(window);
		{
			GuiSolidBackgroundElement* background=GuiSolidBackgroundElement::Create();
			background->SetColor(Color(255, 255, 255));
			windowComposition.SetOwnedElement(background);
		}
		{
			GuiBoundsComposition* redBorder=CreateBorder(&windowComposition, Color(255, 0, 0), windowComposition.GetBounds(), true);
			{
				GuiBoundsComposition* label=CreateLabel(redBorder, Color(0, 0, 0), Point(10, 10), L"Vczh GUI Demo");
				GuiBoundsComposition* border1=CreateBorder(redBorder, Color(255, 0, 0), Rect(Point(50, 200), Size(100, 100)), false);
				GuiBoundsComposition* border2=CreateBorder(border1, Color(0, 255, 0), Rect(Point(0, 20), Size(100, 60)), false);
				GuiBoundsComposition* border3=CreateBorder(border1, Color(0, 0, 255), Rect(Point(-1, 21), Size(102, 58)), false);
			}
			Rect tableBounds;
			{
				GuiTableComposition* table=new GuiTableComposition;
				table->SetBounds(Rect(Point(200, 50), Size(550, 500)));
				redBorder->AddChild(table);
				{
					GuiSolidBorderElement* element=GuiSolidBorderElement::Create();
					element->SetColor(Color(0, 0, 255));
					table->SetOwnedElement(element);
				}
				table->SetRowsAndColumns(5, 3);
				table->SetCellPadding(5);
				table->SetInternalMargin(Margin(1, 1, 1, 1));
				table->SetMinSizeLimitation(GuiGraphicsComposition::LimitToElement);

				const wchar_t* rows[]={L"C++", L"C#", L"VB.NET", L"F#"};
				const wchar_t* cols[]={L"Name", L"Properties"};
				const wchar_t* contents[]={L"C Plus Plus", L"C Sharp", L"VB Dot NET", L"F Sharp"};
				for(int i=0;i<table->GetRows();i++)
				{
					GuiCellOption option;
					option.composeType=GuiCellOption::MinSize;
					table->SetRowOption(i, option);
				}
				for(int j=0;j<table->GetColumns();j++)
				{
					GuiCellOption option;
					option.composeType=GuiCellOption::MinSize;
					table->SetColumnOption(j, option);
				}
				for(int i=0;i<table->GetRows();i++)
				{
					for(int j=0;j<table->GetColumns();j++)
					{
						GuiCellComposition* cell=new GuiCellComposition;
						table->AddChild(cell);
						cell->SetSite(i, j, 1, 1);
						{
							GuiSolidBorderElement* element=GuiSolidBorderElement::Create();
							element->SetColor(Color(0, 0, 255));
							cell->SetOwnedElement(element);
							cell->SetInternalMargin(Margin(1, 1, 1, 1));
						}
					}
				}
				for(int i=1;i<table->GetRows();i++)
				{
					CreateLabel(table->GetSitedCell(i, 0), Color(0, 0, 0), rows[i-1]);
					CreateLabel(table->GetSitedCell(i, 1), Color(0, 0, 0), contents[i-1]);
				}
				for(int j=1;j<table->GetColumns();j++)
				{
					CreateLabel(table->GetSitedCell(0, j), Color(0, 0, 0), cols[j-1]);
				}
				table->UpdateCellBounds();
				tableBounds=table->GetBounds();
			}
			{
				GuiTableComposition* table=new GuiTableComposition;
				table->SetBounds(Rect(Point(200, 20+tableBounds.Bottom()), Size(550, 250)));
				redBorder->AddChild(table);
				{
					GuiSolidBorderElement* element=GuiSolidBorderElement::Create();
					element->SetColor(Color(0, 128, 255));
					table->SetOwnedElement(element);
				}
				table->SetRowsAndColumns(3, 6);
				table->SetCellPadding(2);
				table->SetInternalMargin(Margin(1, 1, 1, 1));

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
					cell->SetMinSizeLimitation(GuiGraphicsComposition::NoLimit);
					{
						GuiSolidBorderElement* element=GuiSolidBorderElement::Create();
						element->SetColor(Color(0, 128, 255));
						cell->SetOwnedElement(element);
						cell->SetInternalMargin(Margin(1, 1, 1, 1));
					}
					CreateCenteredLabel(cell, Color(0, 0, 0), L"This is the content");
				}

				for(int i=0;i<5;i++)
				{
					GuiCellComposition* cell=new GuiCellComposition;
					table->AddChild(cell);
					cell->SetSite(2, i+1, 1, 1);
				}
				CreateButton(
					table->GetSitedCell(2, 1),
					Color(122, 122, 122),
					Color(251, 251, 251),
					Color(250, 250, 250),
					Color(235, 235, 235),
					Color(221, 221, 221),
					Color(207, 207, 207),
					L"Normal",
					Color(0, 0, 0)
					);
				CreateButton(
					table->GetSitedCell(2, 2),
					Color(60, 127, 177),//Color(204, 240, 255),
					Color(220, 244, 254),
					Color(246, 252, 255),
					Color(240, 250, 255),
					Color(225, 245, 254),
					Color(215, 245, 254),
					L"Active",
					Color(0, 0, 0)
					);
				CreateButton(
					table->GetSitedCell(2, 3),
					Color(60, 127, 177),//Color(128, 190, 247),
					Color(232, 248, 255),
					Color(225, 246, 255),
					Color(204, 239, 254),
					Color(181, 231, 253),
					Color(164, 225, 251),
					L"Selected",
					Color(0, 0, 0)
					);
				CreateButton(
					table->GetSitedCell(2, 4),
					Color(44, 98, 139),
					Color(158, 176, 186),
					Color(225, 246, 255),
					Color(204, 239, 254),
					Color(181, 231, 253),
					Color(164, 225, 251),
					L"Pressed",
					Color(0, 0, 0)
					);
				CreateButton(
					table->GetSitedCell(2, 5),
					Color(173, 178, 181),
					Color(252, 252, 252),
					Color(244, 244, 244),
					Color(244, 244, 244),
					Color(244, 244, 244),
					Color(244, 244, 244),
					L"Disabled",
					Color(131, 131, 131)
					);
				table->UpdateCellBounds();
			}
		}
		{
			windowComposition.GetRenderTarget()->StartRendering();
			windowComposition.Render(Size());
			windowComposition.GetRenderTarget()->StopRendering();
		}

		//GuiSolidBorderElement* border=GuiSolidBorderElement::Create();
		//border->SetColor(Color(255, 0, 0));

	}

	GetCurrentController()->Run(window);
}