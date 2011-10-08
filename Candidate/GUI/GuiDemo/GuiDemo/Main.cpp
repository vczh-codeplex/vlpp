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
	CreateCenteredLabel(cell, textColor, text);
}

class WindowListener : public Object, public INativeWindowListener
{
protected:
	INativeWindow*			window;
	GuiWindowComposition*	windowComposition;
	Size					previousClientSize;
	Size					minSize;
public:
	WindowListener(INativeWindow* _window)
	{
		window=_window;
		previousClientSize=window->GetClientSize();

		windowComposition=new GuiWindowComposition;
		windowComposition->SetAttachedWindow(window);
		windowComposition->SetMinSizeLimitation(GuiGraphicsComposition::LimitToElementAndChildren);
		{
			GuiSolidBackgroundElement* background=GuiSolidBackgroundElement::Create();
			background->SetColor(Color(255, 255, 255));
			windowComposition->SetOwnedElement(background);
		}
		{
			GuiTableComposition* table=new GuiTableComposition;
			table->SetAlignmentToParent(Margin(0, 0, 0, 0));
			windowComposition->AddChild(table);
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
			}

			for(int i=0;i<5;i++)
			{
				GuiCellComposition* cell=new GuiCellComposition;
				table->AddChild(cell);
				cell->SetSite(2, i+1, 1, 1);
				cell->SetBounds(Rect(Point(0, 0), Size(100, 0)));
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
		minSize=windowComposition->GetMinNecessaryBounds().GetSize();
		Render();
	}

	~WindowListener()
	{
		delete windowComposition;
	}

	void Render()
	{
		windowComposition->GetRenderTarget()->StartRendering();
		windowComposition->Render(Size());
		windowComposition->GetRenderTarget()->StopRendering();
	}

	void Moving(Rect& bounds)
	{
		Rect oldBounds=window->GetBounds();
		Size minWindowSize=minSize+(oldBounds.GetSize()-window->GetClientSize());
		if(bounds.Width()<minWindowSize.x)
		{
			if(oldBounds.x1!=bounds.x1)
			{
				bounds.x1=oldBounds.x2-minWindowSize.x;
			}
			else if(oldBounds.x2!=bounds.x2)
			{
				bounds.x2=oldBounds.x1+minWindowSize.x;
			}
		}
		if(bounds.Height()<minWindowSize.y)
		{
			if(oldBounds.y1!=bounds.y1)
			{
				bounds.y1=oldBounds.y2-minWindowSize.y;
			}
			else if(oldBounds.y2!=bounds.y2)
			{
				bounds.y2=oldBounds.y1+minWindowSize.y;
			}
		}
	}

	void Moved()
	{
		Size size=window->GetClientSize();
		if(previousClientSize!=size)
		{
			previousClientSize=size;
			minSize=windowComposition->GetMinNecessaryBounds().GetSize();
			Render();
		}
	}
};

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

	WindowListener listener(window);
	window->InstallListener(&listener);

	GetCurrentController()->Run(window);
}