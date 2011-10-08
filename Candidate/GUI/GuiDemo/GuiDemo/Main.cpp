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
		font.size=24;
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
		font.size=24;
		font.fontFamily=L"Lucida Console";
		element->SetFont(font);
	}
	element->SetText(text);

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
				table->SetRowsAndColumns(3, 3);
				table->SetCellPadding(5);
				table->SetInternalMargin(Margin(1, 1, 1, 1));

				{
					table->SetRowOption(0, GuiCellOption::MinSizeOption());
					table->SetRowOption(1, GuiCellOption::PercentageOption(1.0));
					table->SetRowOption(2, GuiCellOption::MinSizeOption());

					table->SetColumnOption(0, GuiCellOption::PercentageOption(1.0));
					table->SetColumnOption(1, GuiCellOption::MinSizeOption());
					table->SetColumnOption(2, GuiCellOption::MinSizeOption());
				}

				{
					GuiCellComposition* cell=new GuiCellComposition;
					table->AddChild(cell);
					cell->SetSite(0, 0, 1, 3);
					CreateLabel(cell, Color(0, 0, 0), L"This is the title");
				}
				{
					GuiCellComposition* cell=new GuiCellComposition;
					table->AddChild(cell);
					cell->SetSite(1, 0, 1, 3);
					cell->SetMinSizeLimitation(GuiGraphicsComposition::NoLimit);
					{
						GuiSolidBorderElement* element=GuiSolidBorderElement::Create();
						element->SetColor(Color(0, 128, 255));
						cell->SetOwnedElement(element);
						cell->SetInternalMargin(Margin(1, 1, 1, 1));
					}
					CreateCenteredLabel(cell, Color(0, 0, 0), L"This is the content");
				}
				const wchar_t* buttons[]={L"OK", L"Cancel"};
				for(int i=0;i<2;i++)
				{
					GuiCellComposition* cell=new GuiCellComposition;
					table->AddChild(cell);
					cell->SetSite(2, i+1, 1, 1);
					{
						GuiSolidBorderElement* element=GuiSolidBorderElement::Create();
						element->SetColor(Color(255, 128, 128));
						cell->SetOwnedElement(element);
						cell->SetInternalMargin(Margin(1, 1, 1, 1));
					}
					CreateLabel(cell, Color(0, 0, 0),buttons[i]);
				}
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