#include "..\..\GUI\Controls\GuiTextControls.h"
#include "..\..\GUI\Controls\GuiWin7Styles.h"

using namespace vl;
using namespace vl::presentation;
using namespace vl::presentation::elements;
using namespace vl::presentation::controls;

void SetupMainPanel(GuiControlHost* host, GuiCellComposition* cell)
{
	{
		GuiControl* groupBox=new GuiControl(new win7::Win7GroupBoxStyle);
		cell->AddChild(groupBox->GetBoundsComposition());
		groupBox->GetBoundsComposition()->SetBounds(Rect(Point(0, 0), Size(200, 200)));
		groupBox->SetText(L"GroupBox1");

		GuiButton* buttonBig=new GuiButton(new win7::Win7ButtonStyle());
		groupBox->GetContainerComposition()->AddChild(buttonBig->GetBoundsComposition());
		buttonBig->GetBoundsComposition()->SetAlignmentToParent(Margin(10, 10, 10, 10));
		buttonBig->SetText(L"I am a big button!");
		{
			GuiButton* buttonEnabling=new GuiButton(new win7::Win7ButtonStyle());
			cell->AddChild(buttonEnabling->GetBoundsComposition());
			buttonEnabling->GetBoundsComposition()->SetBounds(Rect(Point(0, 210), Size(200, 25)));
			buttonEnabling->SetText(L"Disable GroupBox1");

			buttonEnabling->Clicked.AttachLambda([groupBox, buttonEnabling](GuiGraphicsComposition* sender, GuiEventArgs& arguments)
			{
				if(groupBox->GetEnabled())
				{
					buttonEnabling->SetText(L"Enable GroupBox1");
				}
				else
				{
					buttonEnabling->SetText(L"Disable GroupBox1");
				}
				groupBox->SetEnabled(!groupBox->GetEnabled());
			});
		}
		{
			GuiButton* buttonEnabling=new GuiButton(new win7::Win7ButtonStyle());
			cell->AddChild(buttonEnabling->GetBoundsComposition());
			buttonEnabling->GetBoundsComposition()->SetBounds(Rect(Point(0, 245), Size(200, 25)));
			buttonEnabling->SetText(L"Disable big button");

			buttonEnabling->Clicked.AttachLambda([buttonBig, buttonEnabling](GuiGraphicsComposition* sender, GuiEventArgs& arguments)
			{
				if(buttonBig->GetEnabled())
				{
					buttonEnabling->SetText(L"Enable big button");
				}
				else
				{
					buttonEnabling->SetText(L"Disable big button");
				}
				buttonBig->SetEnabled(!buttonBig->GetEnabled());
			});
		}
		{
			GuiMultilineTextBox* textBox=new GuiMultilineTextBox(new win7::Win7MultilineTextBoxProvider);
			cell->AddChild(textBox->GetBoundsComposition());
			textBox->GetBoundsComposition()->SetBounds(Rect(Point(0, 280), Size(200, 180)));

			const wchar_t* text=
				L"#include <iostream>"											L"\r\n"
				L"using namespace std;"											L"\r\n"
				L""																L"\r\n"
				L"int main()"													L"\r\n"
				L"{"															L"\r\n"
				L"    cout<<\"Hello, world! This is Vczh GUI Demo.\"<<endl;"	L"\r\n"
				L"}"															L"\r\n"
				;
			textBox->SetText(text);

			GuiButton* buttonEnabling=new GuiButton(new win7::Win7ButtonStyle());
			cell->AddChild(buttonEnabling->GetBoundsComposition());
			buttonEnabling->GetBoundsComposition()->SetBounds(Rect(Point(0, 470), Size(200, 25)));
			buttonEnabling->SetText(L"Disable TextBox");

			buttonEnabling->Clicked.AttachLambda([textBox, buttonEnabling](GuiGraphicsComposition* sender, GuiEventArgs& arguments)
			{
				if(textBox->GetEnabled())
				{
					buttonEnabling->SetText(L"Enable TextBox");
				}
				else
				{
					buttonEnabling->SetText(L"Disable TextBox");
				}
				textBox->SetEnabled(!textBox->GetEnabled());
			});
		}
	}
	{
		GuiControl* groupBox=new GuiControl(new win7::Win7GroupBoxStyle);
		cell->AddChild(groupBox->GetBoundsComposition());
		groupBox->GetBoundsComposition()->SetBounds(Rect(Point(210, 0), Size(200, 200)));
		groupBox->SetText(L"GroupBox2");
		{
			GuiSelectableButton* checkBox=new GuiSelectableButton(new win7::Win7CheckBoxStyle(win7::Win7CheckBoxStyle::CheckBox));
			groupBox->GetContainerComposition()->AddChild(checkBox->GetBoundsComposition());
			checkBox->GetBoundsComposition()->SetAlignmentToParent(Margin(10, 5, 10, -1));
			checkBox->SetText(L"CheckBox 1");
		}
		{
			GuiButton* checkBox=new GuiButton(new win7::Win7ButtonStyle());
			groupBox->GetContainerComposition()->AddChild(checkBox->GetBoundsComposition());
			checkBox->GetBoundsComposition()->SetAlignmentToParent(Margin(10, 40, 10, -1));
			checkBox->SetText(L"CheckBox 2");
		}
		{
			GuiSelectableButton::MutexGroupController* controller=new GuiSelectableButton::MutexGroupController;
			host->AddComponent(controller);

			for(int i=0;i<3;i++)
			{
				GuiSelectableButton* radioButton=new GuiSelectableButton(new win7::Win7CheckBoxStyle(win7::Win7CheckBoxStyle::RadioButton));
				groupBox->GetContainerComposition()->AddChild(radioButton->GetBoundsComposition());
				radioButton->GetBoundsComposition()->SetBounds(Rect(Point(10, 75+i*35), Size(200, 25)));
				radioButton->SetText(L"RadioButton "+itow(i+1));
				radioButton->SetGroupController(controller);
			}
		}
		{
			GuiButton* buttonEnabling=new GuiButton(new win7::Win7ButtonStyle());
			cell->AddChild(buttonEnabling->GetBoundsComposition());
			buttonEnabling->GetBoundsComposition()->SetBounds(Rect(Point(210, 210), Size(200, 25)));
			buttonEnabling->SetText(L"Disable GroupBox2");

			buttonEnabling->Clicked.AttachLambda([groupBox, buttonEnabling](GuiGraphicsComposition* sender, GuiEventArgs& arguments)
			{
				if(groupBox->GetEnabled())
				{
					buttonEnabling->SetText(L"Enable GroupBox2");
				}
				else
				{
					buttonEnabling->SetText(L"Disable GroupBox2");
				}
				groupBox->SetEnabled(!groupBox->GetEnabled());
			});
		}
		{
			GuiSinglelineTextBox* textBox=new GuiSinglelineTextBox(new win7::Win7SinglelineTextBoxProvider);
			cell->AddChild(textBox->GetBoundsComposition());
			textBox->GetBoundsComposition()->SetBounds(Rect(Point(210, 245), Size(200, 25)));
			textBox->SetText(L"Singleline TextBox");
		}
	}
	{
		GuiScrollContainer* scrollView=new GuiScrollContainer(new win7::Win7ScrollViewProvider);
		cell->AddChild(scrollView->GetBoundsComposition());
		scrollView->GetBoundsComposition()->SetBounds(Rect(Point(420, 0), Size(200, 200)));
		{
			GuiButton* buttonAdd=new GuiButton(new win7::Win7ButtonStyle());
			cell->AddChild(buttonAdd->GetBoundsComposition());
			buttonAdd->GetBoundsComposition()->SetBounds(Rect(Point(420, 210), Size(200, 25)));
			buttonAdd->SetText(L"Add Button");

			buttonAdd->Clicked.AttachLambda([scrollView](GuiGraphicsComposition* sender, GuiEventArgs& arguments)
			{
				int count=0;
				for(int i=0;i<scrollView->GetChildrenCount();i++)
				{
					if(dynamic_cast<GuiButton*>(scrollView->GetChild(i)))
					{
						count++;
					}
				}
				GuiButton* child=new GuiButton(new win7::Win7ButtonStyle);
				child->GetBoundsComposition()->SetBounds(Rect(Point(0, count*40), Size(200, 30)));
				child->SetText(L"Button "+itow(count+1));
				scrollView->GetContainerComposition()->AddChild(child->GetBoundsComposition());
			});
		}
		{
			GuiButton* buttonClear=new GuiButton(new win7::Win7ButtonStyle());
			cell->AddChild(buttonClear->GetBoundsComposition());
			buttonClear->GetBoundsComposition()->SetBounds(Rect(Point(420, 245), Size(200, 25)));
			buttonClear->SetText(L"Clear Buttons");

			buttonClear->Clicked.AttachLambda([scrollView](GuiGraphicsComposition* sender, GuiEventArgs& arguments)
			{
				for(int i=scrollView->GetChildrenCount()-1;i>=0;i--)
				{
					GuiControl* child=scrollView->GetChild(i);
					if(dynamic_cast<GuiButton*>(child))
					{
						scrollView->GetContainerComposition()->RemoveChild(child->GetBoundsComposition());
						delete child;
					}
				}
			});
		}
		{
			GuiButton* buttonHorizontal=new GuiButton(new win7::Win7ButtonStyle());
			cell->AddChild(buttonHorizontal->GetBoundsComposition());
			buttonHorizontal->GetBoundsComposition()->SetBounds(Rect(Point(420, 280), Size(200, 25)));
			buttonHorizontal->SetText(L"Horizontal Scroll Bar");

			buttonHorizontal->Clicked.AttachLambda([scrollView](GuiGraphicsComposition* sender, GuiEventArgs& arguments)
			{
				scrollView->SetHorizontalAlwaysVisible(!scrollView->GetHorizontalAlwaysVisible());
			});
		}
		{
			GuiButton* buttonVertical=new GuiButton(new win7::Win7ButtonStyle());
			cell->AddChild(buttonVertical->GetBoundsComposition());
			buttonVertical->GetBoundsComposition()->SetBounds(Rect(Point(420, 315), Size(200, 25)));
			buttonVertical->SetText(L"Vertical Scroll Bar");

			buttonVertical->Clicked.AttachLambda([scrollView](GuiGraphicsComposition* sender, GuiEventArgs& arguments)
			{
				scrollView->SetVerticalAlwaysVisible(!scrollView->GetVerticalAlwaysVisible());
			});
		}
	}
}

void SetupWindow(GuiControlHost* host)
{
	host->GetBoundsComposition()->SetMinSizeLimitation(GuiGraphicsComposition::LimitToElementAndChildren);
	GuiTableComposition* table=new GuiTableComposition;
	table->SetAlignmentToParent(Margin(0, 0, 0, 0));
	host->GetContainerComposition()->AddChild(table);
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
			GuiTableComposition* mainTable=new GuiTableComposition;
			cell->AddChild(mainTable);
			mainTable->SetAlignmentToParent(Margin(0, 0, 0, 0));
			mainTable->SetRowsAndColumns(3, 3);
			mainTable->SetCellPadding(0);
			mainTable->SetMinSizeLimitation(GuiGraphicsComposition::LimitToElementAndChildren);

			mainTable->SetRowOption(0, GuiCellOption::PercentageOption(1.0));
			mainTable->SetRowOption(1, GuiCellOption::AbsoluteOption(win7::Win7TrackStyle::HandleLong));
			mainTable->SetRowOption(2, GuiCellOption::AbsoluteOption(win7::Win7ScrollStyle::DefaultSize));
			mainTable->SetColumnOption(0, GuiCellOption::PercentageOption(1.0));
			mainTable->SetColumnOption(1, GuiCellOption::AbsoluteOption(win7::Win7TrackStyle::HandleLong));
			mainTable->SetColumnOption(2, GuiCellOption::AbsoluteOption(win7::Win7ScrollStyle::DefaultSize));
			{
				GuiCellComposition* cell=new GuiCellComposition;
				mainTable->AddChild(cell);
				cell->SetSite(0, 0, 1, 1);
				SetupMainPanel(host, cell);
			}
			{
				GuiCellComposition* cell=new GuiCellComposition;
				mainTable->AddChild(cell);
				cell->SetSite(0, 1, 1, 1);

				GuiScroll* scroll=new GuiScroll(new win7::Win7TrackStyle(win7::Win7TrackStyle::Vertical));
				scroll->GetBoundsComposition()->SetAlignmentToParent(Margin(0, 0, 0, 0));
				scroll->SetTotalSize(10);
				cell->AddChild(scroll->GetBoundsComposition());
			}
			{
				GuiCellComposition* cell=new GuiCellComposition;
				mainTable->AddChild(cell);
				cell->SetSite(1, 0, 1, 1);

				GuiScroll* scroll=new GuiScroll(new win7::Win7TrackStyle(win7::Win7TrackStyle::Horizontal));
				scroll->GetBoundsComposition()->SetAlignmentToParent(Margin(0, 0, 0, 0));
				scroll->SetTotalSize(10);
				scroll->SetPageSize(8);
				cell->AddChild(scroll->GetBoundsComposition());
			}
			{
				GuiCellComposition* cell=new GuiCellComposition;
				mainTable->AddChild(cell);
				cell->SetSite(0, 2, 2, 1);

				GuiScroll* scroll=new GuiScroll(new win7::Win7ScrollStyle(win7::Win7ScrollStyle::Vertical));
				scroll->GetBoundsComposition()->SetAlignmentToParent(Margin(0, 0, 0, 0));
				scroll->SetTotalSize(10);
				scroll->SetPageSize(6);
				scroll->SetBigMove(2);
				scroll->SetSmallMove(1);
				cell->AddChild(scroll->GetBoundsComposition());
			}
			{
				GuiCellComposition* cell=new GuiCellComposition;
				mainTable->AddChild(cell);
				cell->SetSite(2, 0, 1, 2);

				GuiScroll* scroll=new GuiScroll(new win7::Win7ScrollStyle(win7::Win7ScrollStyle::Horizontal));
				scroll->GetBoundsComposition()->SetAlignmentToParent(Margin(0, 0, 0, 0));
				scroll->SetTotalSize(10);
				scroll->SetPageSize(6);
				scroll->SetBigMove(2);
				scroll->SetSmallMove(1);
				cell->AddChild(scroll->GetBoundsComposition());
			}
		}
	}

	const wchar_t* buttonTexts[]={L"Not Reactive", L"Close"};
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

		button->Clicked.AttachLambda([host](GuiGraphicsComposition* sender, GuiEventArgs& arguments)
		{
			GuiButton* button=dynamic_cast<GuiButton*>(sender->GetAssociatedControl());
			if(button->GetText()==L"Close")
			{
				host->Close();
			}
		});
	}
}

void SetupTextBoxWindow(GuiControlHost* host)
{
	host->GetBoundsComposition()->SetMinSizeLimitation(GuiGraphicsComposition::LimitToElementAndChildren);
	GuiMultilineTextBox* textBox=new GuiMultilineTextBox(new win7::Win7MultilineTextBoxProvider);
	textBox->GetBoundsComposition()->SetAlignmentToParent(Margin(0, 0, 0, 0));
	textBox->GetBoundsComposition()->SetBounds(Rect(0, 0, 300, 200));
	host->GetBoundsComposition()->AddChild(textBox->GetBoundsComposition());
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

	GuiControlHost host(new win7::Win7WindowStyle);
	SetupWindow(&host);
	host.SetNativeWindow(window);

	GetCurrentController()->Run(window);
}