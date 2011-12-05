#include "..\..\GUI\GacUI.h"

/***********************************************************************
SetupWindow
***********************************************************************/

void SetupMainPanel(GuiControlHost* controlHost, GuiControl* container, GuiCellComposition* cell)
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
			GuiSelectableButton* checkBox=new GuiSelectableButton(new win7::Win7SelectableItemStyle());
			groupBox->GetContainerComposition()->AddChild(checkBox->GetBoundsComposition());
			checkBox->GetBoundsComposition()->SetAlignmentToParent(Margin(10, 40, 10, -1));
			checkBox->SetText(L"CheckBox 2");
		}
		{
			GuiSelectableButton::MutexGroupController* controller=new GuiSelectableButton::MutexGroupController;
			controlHost->AddComponent(controller);

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
			textBox->GetBoundsComposition()->SetBounds(Rect(Point(210, 245), Size(200, 27)));
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

void SetupWindow(GuiControlHost* controlHost, GuiControl* container)
{
	container->GetBoundsComposition()->SetMinSizeLimitation(GuiGraphicsComposition::LimitToElementAndChildren);
	GuiTableComposition* table=new GuiTableComposition;
	table->SetAlignmentToParent(Margin(0, 0, 0, 0));
	container->GetContainerComposition()->AddChild(table);
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
				SetupMainPanel(controlHost, container, cell);
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
		cell->SetPreferredMinSize(Size(100, 0));

		GuiButton* button=new GuiButton(new win7::Win7ButtonStyle());
		cell->AddChild(button->GetBoundsComposition());
		button->GetBoundsComposition()->SetAlignmentToParent(Margin(0, 0, 0, 0));
		button->SetText(buttonTexts[i]);

		button->Clicked.AttachLambda([controlHost](GuiGraphicsComposition* sender, GuiEventArgs& arguments)
		{
			GuiButton* button=dynamic_cast<GuiButton*>(sender->GetAssociatedControl());
			if(button->GetText()==L"Close")
			{
				controlHost->Close();
			}
		});
	}
}

/***********************************************************************
SetupTextBoxWindow
***********************************************************************/

int textBoxTextChangedCounter=0;
int textBoxSelectionChangedCounter=0;

void SetupTextBoxWindow(GuiControlHost* controlHost, GuiControl* container)
{
	container->GetBoundsComposition()->SetMinSizeLimitation(GuiGraphicsComposition::LimitToElementAndChildren);
	GuiMultilineTextBox* textBox=new GuiMultilineTextBox(new win7::Win7MultilineTextBoxProvider);
	textBox->GetBoundsComposition()->SetAlignmentToParent(Margin(0, 0, 0, 0));
	textBox->GetBoundsComposition()->SetBounds(Rect(0, 0, 300, 200));
	container->GetBoundsComposition()->AddChild(textBox->GetBoundsComposition());

	textBox->TextChanged.AttachLambda([controlHost](GuiGraphicsComposition* sender, GuiEventArgs& arguments)
	{
		textBoxTextChangedCounter++;
		controlHost->GetNativeWindow()->SetTitle(L"TextChanged["+itow(textBoxTextChangedCounter)+L"], SelectionChanged["+itow(textBoxSelectionChangedCounter)+L"]");
	});

	textBox->SelectionChanged.AttachLambda([controlHost](GuiGraphicsComposition* sender, GuiEventArgs& arguments)
	{
		textBoxSelectionChangedCounter++;
		controlHost->GetNativeWindow()->SetTitle(L"TextChanged["+itow(textBoxTextChangedCounter)+L"], SelectionChanged["+itow(textBoxSelectionChangedCounter)+L"]");
	});
}

/***********************************************************************
SetupListControlWindow
***********************************************************************/

void SetupListControlWindow(GuiControlHost* controlHost, GuiControl* container)
{
	container->GetBoundsComposition()->SetMinSizeLimitation(GuiGraphicsComposition::LimitToElementAndChildren);
	GuiTextList* listControl=0;
	{
		listControl=new GuiTextList(new win7::Win7MultilineTextBoxProvider, new win7::Win7TextListProvider);
		listControl->GetBoundsComposition()->SetAlignmentToParent(Margin(200, 5, 5, 5));
		listControl->GetBoundsComposition()->SetBounds(Rect(0, 0, 300, 200));
		listControl->SetHorizontalAlwaysVisible(false);
		container->GetBoundsComposition()->AddChild(listControl->GetBoundsComposition());

		for(int i=0;i<30;i++)
		{
			listControl->GetItems().Add(L"Text Item "+itow(i+1));
			listControl->GetItems().SetChecked(i, i%2==0);
		}
	}
	{
		GuiTextList* typeList=new GuiTextList(new win7::Win7MultilineTextBoxProvider, new win7::Win7TextListProvider);
		typeList->GetBoundsComposition()->SetAlignmentToParent(Margin(5, 5, -1, 5));
		typeList->GetBoundsComposition()->SetBounds(Rect(0, 0, 190, 200));
		typeList->SetHorizontalAlwaysVisible(false);
		container->GetBoundsComposition()->AddChild(typeList->GetBoundsComposition());

		typeList->GetItems().Add(L"Text List");
		typeList->GetItems().Add(L"Check List");
		typeList->GetItems().Add(L"Radio List");
		typeList->SetSelected(0, true);

		typeList->SelectionChanged.AttachLambda([listControl, typeList](GuiGraphicsComposition* sender, GuiEventArgs& arguments)
		{
			if(typeList->GetSelectedItems().Count()>0)
			{
				switch(typeList->GetSelectedItems()[0])
				{
				case 0:
					listControl->ChangeItemStyle(new win7::Win7TextListProvider);
					break;
				case 1:
					listControl->ChangeItemStyle(new win7::Win7CheckTextListProvider);
					break;
				case 2:
					listControl->ChangeItemStyle(new win7::Win7RadioTextListProvider);
					break;
				}
			}
		});
	}
}

/***********************************************************************
SetupListviewWindow
***********************************************************************/

void SetupListviewWindow(GuiControlHost* controlHost, GuiControl* container)
{
	container->GetBoundsComposition()->SetMinSizeLimitation(GuiGraphicsComposition::LimitToElementAndChildren);
	GuiListView* listControl=0;
	{
		listControl=new GuiListView(new win7::Win7ListViewProvider);
		listControl->GetBoundsComposition()->SetAlignmentToParent(Margin(200, 5, 5, 5));
		listControl->GetBoundsComposition()->SetBounds(Rect(0, 0, 300, 200));
		listControl->SetHorizontalAlwaysVisible(false);
		container->GetBoundsComposition()->AddChild(listControl->GetBoundsComposition());
		
		INativeImageProvider* imageProvider=GetCurrentController()->GetImageProvider();
		Ptr<INativeImage> largeImage=imageProvider->CreateImageFromFile(L"Resources\\New.png");
		Ptr<GuiImageData> largeImageData=new GuiImageData(largeImage, 0);
		Ptr<INativeImage> smallImage=imageProvider->CreateImageFromFile(L"Resources\\NewSmall.png");
		Ptr<GuiImageData> smallImageData=new GuiImageData(smallImage, 0);

		for(int i=0;i<100;i++)
		{
			Ptr<list::ListViewItem> item=new list::ListViewItem;
			if(i<10)
			{
				item->text=L"This is a very long List View Item "+itow(i+1);
			}
			else
			{
				item->text=L"List View Item "+itow(i+1);
			}
			item->largeImage=largeImageData;
			item->smallImage=smallImageData;
			item->subItems.Add(L"Description of "+item->text);
			item->subItems.Add(itow(i+1));
			item->subItems.Add(i<10?L"Long":L"Short");
			listControl->GetItems().Add(item);
		}
		listControl->GetItems().GetColumns().Add(L"Name");
		listControl->GetItems().GetColumns().Add(L"Description");
		listControl->GetItems().GetColumns().Add(L"Index");
		listControl->GetItems().GetColumns().Add(L"Type");
	}
	{
		GuiTextList* typeList=new GuiTextList(new win7::Win7MultilineTextBoxProvider, new win7::Win7TextListProvider);
		typeList->GetBoundsComposition()->SetAlignmentToParent(Margin(5, 5, -1, 5));
		typeList->GetBoundsComposition()->SetBounds(Rect(0, 0, 190, 200));
		typeList->SetHorizontalAlwaysVisible(false);
		container->GetBoundsComposition()->AddChild(typeList->GetBoundsComposition());

		typeList->GetItems().Add(L"Big Icon");
		typeList->GetItems().Add(L"Small Icon");
		typeList->GetItems().Add(L"List");
		typeList->GetItems().Add(L"Detail (not completed)");
		typeList->GetItems().Add(L"Tile");
		typeList->GetItems().Add(L"Information");
		typeList->SetSelected(0, true);

		typeList->SelectionChanged.AttachLambda([listControl, typeList](GuiGraphicsComposition* sender, GuiEventArgs& arguments)
		{
			if(typeList->GetSelectedItems().Count()>0)
			{
				listControl->GetItems().GetDataColumns().Clear();
				switch(typeList->GetSelectedItems()[0])
				{
				case 0:
					listControl->ChangeItemStyle(new list::ListViewBigIconContentProvider);
					break;
				case 1:
					listControl->ChangeItemStyle(new list::ListViewSmallIconContentProvider);
					break;
				case 2:
					listControl->ChangeItemStyle(new list::ListViewListContentProvider);
					break;
				case 3:
					listControl->ChangeItemStyle(new list::ListViewDetailContentProvider);
					break;
				case 4:
					listControl->GetItems().GetDataColumns().Add(0);
					listControl->GetItems().GetDataColumns().Add(2);
					listControl->ChangeItemStyle(new list::ListViewTileContentProvider);
					break;
				case 5:
					listControl->GetItems().GetDataColumns().Add(1);
					listControl->GetItems().GetDataColumns().Add(2);
					listControl->ChangeItemStyle(new list::ListViewInformationContentProvider);
					break;
				}
			}
		});
	}
}

/***********************************************************************
SetupTreeviewWindow
***********************************************************************/

void SetupTreeviewWindow(GuiControlHost* controlHost, GuiControl* container)
{
}

/***********************************************************************
SetupToolstripWindow
***********************************************************************/

GuiBoundsComposition* CreateImageFrame(Ptr<INativeImage> image, int frameIndex=0)
{
	GuiImageFrameElement* element=GuiImageFrameElement::Create();
	element->SetImage(image, frameIndex);

	GuiBoundsComposition* composition=new GuiBoundsComposition;
	composition->SetOwnedElement(element);
	composition->SetMinSizeLimitation(GuiGraphicsComposition::LimitToElement);
	return composition;
}

class GifAnimation : public Object, public IGuiGraphicsAnimation
{
protected:
	unsigned __int64			startTime;
	GuiImageFrameElement*		element;
public:
	GifAnimation(GuiImageFrameElement* _element)
		:element(_element)
		,startTime(DateTime::LocalTime().totalMilliseconds)
	{
	}

	int GetTotalLength()
	{
		return 1;
	}

	int GetCurrentPosition()
	{
		return 0;
	}

	void Play(int currentPosition, int totalLength)
	{
		unsigned __int64 ms=DateTime::LocalTime().totalMilliseconds-startTime;
		int frameIndex=(ms/100)%element->GetImage()->GetFrameCount();
		element->SetImage(element->GetImage(), frameIndex);
	}

	void Stop()
	{
	}
};

template<int count>
GuiStackComposition* CreateSubMenu(const wchar_t* (&menuText)[count])
{
	GuiStackComposition* menuStack=new GuiStackComposition;
	menuStack->SetMinSizeLimitation(GuiGraphicsComposition::LimitToElementAndChildren);
	menuStack->SetDirection(GuiStackComposition::Vertical);
	menuStack->SetAlignmentToParent(Margin(0, 0, 0, 0));

	for(int i=0;i<count;i++)
	{
		GuiControl* menuItem=0;
		if(wcscmp(menuText[i], L"-")==0)
		{
			GuiControl* splitter=new GuiControl(new win7::Win7MenuSplitterStyle);
			menuItem=splitter;
		}
		else
		{
			GuiMenuButton* button=new GuiMenuButton(new win7::Win7MenuItemButtonStyle);
			button->SetText(menuText[i]);
			button->GetBoundsComposition()->SetAlignmentToParent(Margin(0, 0, 0, 0));
			button->SetEnabled(i<4);
			menuItem=button;
		}
		GuiStackItemComposition* item=new GuiStackItemComposition;
		item->AddChild(menuItem->GetBoundsComposition());
		menuStack->AddChild(item);
	}
	return menuStack;
}

void CreateSubMenu(GuiMenu* menu, int index, GuiStackComposition* subMenu)
{
	GuiStackComposition* menuStack=dynamic_cast<GuiStackComposition*>(menu->GetContainerComposition()->Children()[0]);
	GuiMenuButton* button=dynamic_cast<GuiMenuButton*>(menuStack->GetStackItems()[index]->Children()[0]->GetAssociatedControl());
	if(!button->GetSubMenu())
	{
		button->CreateSubMenu();
		button->GetSubMenu()->GetContainerComposition()->AddChild(subMenu);
	}
}

void SetupToolstripWindow(GuiControlHost* controlHost, GuiControl* container)
{
	container->GetBoundsComposition()->SetMinSizeLimitation(GuiGraphicsComposition::LimitToElementAndChildren);
	INativeImageProvider* imageProvider=GetCurrentController()->GetImageProvider();

	const wchar_t* fileMenuText[]={L"New", L"Open", L"Save", L"Save As...", L"-", L"Page Setting...", L"Print...", L"-", L"Exit"};
	const wchar_t* fileNewMenuText[]={L"Project...", L"Web Site...", L"Team Project...", L"File...", L"Project From Existing Code..."};
	const wchar_t* fileOpenMenuText[]={L"Project/Solution...", L"Web Site...", L"Team Project...", L"File...", L"Convert..."};
	const wchar_t* filePrintMenuText[]={L"Print", L"Don't Print"};
	const wchar_t* editMenuText[]={L"Undo", L"-", L"Cut", L"Copy", L"Paste", L"Delete", L"-", L"Find...", L"Find Next", L"Replace...", L"Go to...", L"-", L"Select All", L"Time/Date"};
	const wchar_t* formatMenuText[]={L"Wrap Text", L"Font..."};
	const wchar_t* viewMenuText[]={L"Status Bar"};
	const wchar_t* helpMenuText[]={L"View Help", L"About Notepad"};

	GuiStackComposition* menuStack=new GuiStackComposition;
	{
		menuStack->SetMinSizeLimitation(GuiGraphicsComposition::LimitToElementAndChildren);
		const wchar_t* menuText[]={L"File", L"Edit", L"Format", L"View", L"Help"};
		for(int i=0;i<sizeof(menuText)/sizeof(*menuText);i++)
		{
			GuiMenuButton* button=new GuiMenuButton(new win7::Win7MenuBarButtonStyle);
			button->SetText(menuText[i]);
			button->GetBoundsComposition()->SetAlignmentToParent(Margin(0, 0, 0, 0));

			GuiStackItemComposition* item=new GuiStackItemComposition;
			item->AddChild(button->GetBoundsComposition());
			menuStack->AddChild(item);
			
			button->CreateSubMenu();
			switch(i)
			{
			case 0:
				button->GetSubMenu()->GetContainerComposition()->AddChild(CreateSubMenu(fileMenuText));
				CreateSubMenu(button->GetSubMenu(), 0, CreateSubMenu(fileNewMenuText));
				CreateSubMenu(button->GetSubMenu(), 1, CreateSubMenu(fileOpenMenuText));
				CreateSubMenu(button->GetSubMenu(), 6, CreateSubMenu(filePrintMenuText));
				break;
			case 1:
				button->GetSubMenu()->GetContainerComposition()->AddChild(CreateSubMenu(editMenuText));
				break;
			case 2:
				button->GetSubMenu()->GetContainerComposition()->AddChild(CreateSubMenu(formatMenuText));
				break;
			case 3:
				button->GetSubMenu()->GetContainerComposition()->AddChild(CreateSubMenu(viewMenuText));
				break;
			case 4:
				button->GetSubMenu()->GetContainerComposition()->AddChild(CreateSubMenu(helpMenuText));
				break;
			}
		}
	}

	GuiStackComposition* toolStack=new GuiStackComposition;
	{
		toolStack->SetMinSizeLimitation(GuiGraphicsComposition::LimitToElementAndChildren);
		Ptr<INativeImage> imageButtons[]=
		{
			imageProvider->CreateImageFromFile(L"Resources\\New.png"),
			imageProvider->CreateImageFromFile(L"Resources\\Open.png"),
			imageProvider->CreateImageFromFile(L"Resources\\Save.png"),
		};
		for(int i=0;i<sizeof(imageButtons)/sizeof(*imageButtons);i++)
		{
			GuiBoundsComposition* imageComposition=CreateImageFrame(imageButtons[i]);
			imageComposition->SetAlignmentToParent(Margin(3, 3, 3, 3));
			GuiButton* button=new GuiButton(new win7::Win7ToolstripButtonStyle(false));
			button->GetContainerComposition()->AddChild(imageComposition);

			GuiStackItemComposition* item=new GuiStackItemComposition;
			item->AddChild(button->GetBoundsComposition());
			toolStack->AddChild(item);
		}
	}
	
	GuiBoundsComposition* picGif=0;
	{
		Ptr<INativeImage> imageGif=imageProvider->CreateImageFromFile(L"Resources\\Gif.gif");
		picGif=CreateImageFrame(imageGif);
		Ptr<GifAnimation> animation=new GifAnimation(picGif->GetOwnedElement().Cast<GuiImageFrameElement>().Obj());
		controlHost->GetGraphicsHost()->GetAnimationManager()->AddAnimation(animation);
	}

	GuiStackComposition* windowStack=new GuiStackComposition;
	windowStack->SetMinSizeLimitation(GuiGraphicsComposition::LimitToElementAndChildren);
	windowStack->SetDirection(GuiStackComposition::Vertical);
	windowStack->SetAlignmentToParent(Margin(0, 0, 0, 0));
	{
		GuiMenuBar* menuBar=new GuiMenuBar(new win7::Win7MenuBarStyle);
		menuBar->GetBoundsComposition()->SetMinSizeLimitation(GuiGraphicsComposition::LimitToElementAndChildren);
		menuBar->GetBoundsComposition()->SetAlignmentToParent(Margin(0, 0, 0, 0));
		menuBar->GetContainerComposition()->AddChild(menuStack);

		GuiStackItemComposition* item=new GuiStackItemComposition;
		item->AddChild(menuBar->GetBoundsComposition());
		windowStack->AddChild(item);
	}
	{
		GuiStackItemComposition* item=new GuiStackItemComposition;
		item->AddChild(toolStack);
		windowStack->AddChild(item);
	}
	{
		GuiStackItemComposition* item=new GuiStackItemComposition;
		item->AddChild(picGif);
		windowStack->AddChild(item);
	}
	container->GetContainerComposition()->AddChild(windowStack);
}

/***********************************************************************
SetupRibbonWindow
***********************************************************************/

void SetupRibbonWindow(GuiControlHost* controlHost, GuiControl* container)
{
}

/***********************************************************************
SetupTabPageWindow
***********************************************************************/

void SetupTabPageWindow(GuiControlHost* controlHost, GuiControl* container)
{
	container->GetBoundsComposition()->SetMinSizeLimitation(GuiGraphicsComposition::LimitToElementAndChildren);
	GuiTab* tab=new GuiTab(new win7::Win7TabStyle);
	tab->GetBoundsComposition()->SetAlignmentToParent(Margin(6, 6, 6, 6));
	{
		GuiTabPage* page=tab->CreatePage();
		page->SetText(L"Basic");
		SetupWindow(controlHost, page->GetContainer());
	}
	{
		GuiTabPage* page=tab->CreatePage();
		page->SetText(L"Text Box");
		SetupTextBoxWindow(controlHost, page->GetContainer());
	}
	{
		GuiTabPage* page=tab->CreatePage();
		page->SetText(L"List Control");
		SetupListControlWindow(controlHost, page->GetContainer());
	}
	{
		GuiTabPage* page=tab->CreatePage();
		page->SetText(L"Toolstrip");
		SetupToolstripWindow(controlHost, page->GetContainer());
	}
	{
		GuiTabPage* page=tab->CreatePage();
		page->SetText(L"List View (not completed)");
		SetupListviewWindow(controlHost, page->GetContainer());
	}
	{
		GuiTabPage* page=tab->CreatePage();
		page->SetText(L"Tree View (not completed)");
		SetupTreeviewWindow(controlHost, page->GetContainer());
	}
	{
		GuiTabPage* page=tab->CreatePage();
		page->SetText(L"Ribbon (not completed)");
		SetupRibbonWindow(controlHost, page->GetContainer());
	}
	container->GetContainerComposition()->AddChild(tab->GetBoundsComposition());
}

/***********************************************************************
GuiMain

ListView
TreeView
Ribbon
Misc

develop a tool to merge those files under Library\ to
  vlpp_common.h/cpp
  vlpp_gacui.h/cpp
  vlpp_fs.h/cpp
  vlpp_gacui_fs_plugin.h/cpp
***********************************************************************/

void GuiMain()
{
	GuiWindow window(new win7::Win7WindowStyle);
#ifdef GUI_GRAPHICS_RENDERER_GDI
	window.SetText(L"Vczh GUI Demo (GDI)");
#endif
#ifdef GUI_GRAPHICS_RENDERER_DIRECT2D
	window.SetText(L"Vczh GUI Demo (Direct2D)");
#endif
	SetupTabPageWindow(&window, &window);

	window.SetClientSize(Size(800, 600));
	INativeScreen* screen=window.GetRelatedScreen();
	Rect windowBounds=window.GetBounds();
	Rect screenBounds=screen->GetClientBounds();
	window.SetBounds(Rect(
		Point(
			screenBounds.Left()+(screenBounds.Width()-windowBounds.Width())/2,
			screenBounds.Top()+(screenBounds.Height()-windowBounds.Height())/2
			),
		windowBounds.GetSize()
		));

	GetApplication()->Run(&window);
}