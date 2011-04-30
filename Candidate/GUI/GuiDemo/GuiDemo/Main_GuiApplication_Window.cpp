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

	MainWindow()
	{
		SetTitle(L"Vczh Library++ 3.0 Gui Window");
		SetClientSize(Size(800, 600));
		MoveToScreenCenter();

		GuiFreeGrid* grid=new GuiFreeGrid;
		SetContainer(grid);

		button1=new GuiTextButton;
		button1->SetText(L"Press me 1!");
		button1->SetBounds(Rect(Point(10, 10), Size(100, 30)));
		grid->InsertControl(0, button1);

		button2=new GuiTextButton;
		button2->SetText(L"Press me 2!");
		button2->SetBounds(Rect(Point(120, 10), Size(100, 30)));
		grid->InsertControl(0, button2);

		button3=new GuiTextButton;
		button3->SetText(L"Press me 3!");
		button3->SetBounds(Rect(Point(230, 10), Size(100, 30)));
		grid->InsertControl(0, button3);
	}
};

class SmallWindow : public GuiWindow
{
public:
	SmallWindow()
	{
		SetTitle(L"This is another window");
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