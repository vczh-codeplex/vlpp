#include "..\..\GUI\GuiApplication.h"
#include "..\..\GUI\NativeWindow\Windows\GDI\WinGDISimpleElement.h"

using namespace vl;
using namespace vl::presentation;
using namespace vl::presentation::elements;

class MainWindow : public GuiWindow
{
public:
	GuiTextButton*					button1;
	GuiTextButton*					button2;
	GuiTextButton*					button3;
	GuiTextButton*					button4;
	GuiPanel*						panel;

	GuiTextButton*					disableButton1;
	GuiTextButton*					disableButton2;
	GuiTextButton*					disableButton3;
	GuiTextButton*					disableButton4;
	GuiTextButton*					disablePanel;
	GuiPanel*						disablePanelButtons;

	void DisableButton_Executed(eventargs::NotifyEventArgs& e)
	{
		GuiControl* control=e.sender->GetTag().Cast<ObjectBox<GuiControl*>>()->Unbox();
		control->SetEnabled(!control->GetEnabled());
	}

	MainWindow()
	{
		SetTitle(L"Vczh Library++ 3.0 Gui Window");
		SetClientSize(Size(800, 600));
		MoveToScreenCenter();

		GuiFreeGrid* windowGrid=new GuiFreeGrid;
		SetContainer(windowGrid);

		button1=new GuiTextButton;
		windowGrid->AddControl(button1);
		button1->SetText(L"Press me 1!");
		button1->SetBounds(Rect(Point(10, 10), Size(100, 30)));

		{
			panel=new GuiPanel;
			windowGrid->AddControl(panel);
			panel->SetClientSize(Size(230, 50));
			panel->SetBounds(Rect(Point(10, 50), panel->GetBounds().GetSize()));
			GuiFreeGrid* panelGrid=new GuiFreeGrid;
			panel->SetContainer(panelGrid);

			button2=new GuiTextButton;
			panelGrid->AddControl(button2);
			button2->SetText(L"Press me 2!");
			button2->SetBounds(Rect(Point(10, 10), Size(100, 30)));

			button3=new GuiTextButton;
			panelGrid->AddControl(button3);
			button3->SetText(L"Press me 3!");
			button3->SetBounds(Rect(Point(120, 10), Size(100, 30)));
		}

		button4=new GuiTextButton;
		windowGrid->AddControl(button4);
		button4->SetText(L"Press me 4!");
		button4->SetBounds(Rect(Point(10, 112), Size(100, 30)));

		{
			disablePanelButtons=new GuiPanel;
			windowGrid->AddControl(disablePanelButtons);
			disablePanelButtons->SetClientSize(Size(230, 130));
			disablePanelButtons->SetBounds(Rect(Point(10, 152), disablePanelButtons->GetBounds().GetSize()));
			GuiFreeGrid* panelGrid=new GuiFreeGrid;
			disablePanelButtons->SetContainer(panelGrid);

			disableButton1=new GuiTextButton;
			panelGrid->AddControl(disableButton1);
			disableButton1->SetText(L"Disable Button 1");
			disableButton1->SetBounds(Rect(Point(10, 10), Size(100, 30)));
			disableButton1->SetTag(new ObjectBox<GuiControl*>(button1));
			disableButton1->OnExecuted.Add(this, &MainWindow::DisableButton_Executed);

			disableButton2=new GuiTextButton;
			panelGrid->AddControl(disableButton2);
			disableButton2->SetText(L"Disable Button 2");
			disableButton2->SetBounds(Rect(Point(120, 10), Size(100, 30)));
			disableButton2->SetTag(new ObjectBox<GuiControl*>(button2));
			disableButton2->OnExecuted.Add(this, &MainWindow::DisableButton_Executed);

			disableButton3=new GuiTextButton;
			panelGrid->AddControl(disableButton3);
			disableButton3->SetText(L"Disable Button 3");
			disableButton3->SetBounds(Rect(Point(10, 50), Size(100, 30)));
			disableButton3->SetTag(new ObjectBox<GuiControl*>(button3));
			disableButton3->OnExecuted.Add(this, &MainWindow::DisableButton_Executed);

			disableButton4=new GuiTextButton;
			panelGrid->AddControl(disableButton4);
			disableButton4->SetText(L"Disable Button 4");
			disableButton4->SetBounds(Rect(Point(120, 50), Size(100, 30)));
			disableButton4->SetTag(new ObjectBox<GuiControl*>(button4));
			disableButton4->OnExecuted.Add(this, &MainWindow::DisableButton_Executed);

			disablePanel=new GuiTextButton;
			panelGrid->AddControl(disablePanel);
			disablePanel->SetText(L"Disable Panel");
			disablePanel->SetBounds(Rect(Point(10, 90), Size(100, 30)));
			disablePanel->SetTag(new ObjectBox<GuiControl*>(panel));
			disablePanel->OnExecuted.Add(this, &MainWindow::DisableButton_Executed);
		}
	}
};

class SmallWindow : public GuiWindow
{
protected:

	void UpdateTitle()
	{
		bool capslockPressing=GetCurrentController()->IsKeyPressing(VKEY_CAPITAL);
		bool capslockToggled=GetCurrentController()->IsKeyToggled(VKEY_CAPITAL);
		if(capslockPressing)
		{
			if(capslockToggled)
			{
				SetTitle(L"CAPSLOCK pressing and toggled");
			}
			else
			{
				SetTitle(L"CAPSLOCK pressing");
			}
		}
		else
		{
			if(capslockToggled)
			{
				SetTitle(L"CAPSLOCK toggled");
			}
			else
			{
				SetTitle(L"CAPSLOCK");
			}
		}
	}

	GuiControl* NotifyKeyDown(int code, bool alt)
	{
		GuiControl* actionRaiser=GuiWindow::NotifyKeyDown(code, alt);
		if(code==VKEY_CAPITAL)
		{
			UpdateTitle();
		}
		return actionRaiser;
	}

	GuiControl* NotifyKeyUp(int code, bool alt)
	{
		GuiControl* actionRaiser=GuiWindow::NotifyKeyUp(code, alt);
		if(code==VKEY_CAPITAL)
		{
			UpdateTitle();
		}
		return actionRaiser;
	}
public:
	SmallWindow()
	{
		UpdateTitle();
		SetClientSize(Size(300, 200));
		MoveToScreenCenter();
	}
};

void Main_GuiApplication_Window()
{
	gdi_simple::Resources::RegisterAutoInstall();

	MainWindow* window=new MainWindow;
	window->Show();

	SmallWindow* another=new SmallWindow;
	another->Show();

	GetCurrentApplication()->Run(window);
}