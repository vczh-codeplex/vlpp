#include "..\..\GUI\Controls\GuiBasicControls.h"
#include "..\..\GUI\Controls\GuiWin7Styles.h"

using namespace vl;
using namespace vl::presentation;
using namespace vl::presentation::elements;
using namespace vl::presentation::controls;

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
		table->SetRowsAndColumns(2, 3);
		table->SetCellPadding(5);
		table->SetMinSizeLimitation(GuiGraphicsComposition::LimitToElementAndChildren);
		{
			table->SetRowOption(0, GuiCellOption::PercentageOption(1.0));
			table->SetRowOption(1, GuiCellOption::MinSizeOption());

			table->SetColumnOption(0, GuiCellOption::PercentageOption(1.0));
			table->SetColumnOption(1, GuiCellOption::MinSizeOption());
			table->SetColumnOption(2, GuiCellOption::MinSizeOption());
		}
		{
			GuiCellComposition* cell=new GuiCellComposition;
			table->AddChild(cell);
			cell->SetSite(0, 0, 1, 3);
			{
				GuiSolidBorderElement* element=GuiSolidBorderElement::Create();
				element->SetColor(Color(0, 128, 255));
				cell->SetOwnedElement(element);
				cell->SetInternalMargin(Margin(1, 1, 1, 1));
			}
		}

		const wchar_t* buttonTexts[]={L"OK", L"Cancel"};
		for(int i=0;i<2;i++)
		{
			GuiCellComposition* cell=new GuiCellComposition;
			table->AddChild(cell);
			cell->SetSite(1, i+1, 1, 1);
			cell->SetBounds(Rect(Point(0, 0), Size(100, 0)));

			GuiButton* button=new GuiButton(new win7::Win7ButtonStyle());
			cell->AddChild(button->GetBoundsComposition());
			button->GetBoundsComposition()->SetAlignmentToParent(Margin(0, 0, 0, 0));
			button->SetText(buttonTexts[i]);
		}
	}
}

void GuiMain()
{
	INativeWindow* window=GetCurrentController()->CreateNativeWindow();
#ifdef GUI_GRAPHICS_RENDERER_GDI
	window->SetTitle(L"Vczh GUI Demo (GDI)");
#endif
#ifdef GUI_GRAPHICS_RENDERER_DIRECT2D
	window->SetTitle(L"Vczh GUI Demo (Direct2D)");
#endif
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

	GuiControlHost host;
	SetupWindow(host.GetGraphicsHost());
	host.SetNativeWindow(window);

	GetCurrentController()->Run(window);
}