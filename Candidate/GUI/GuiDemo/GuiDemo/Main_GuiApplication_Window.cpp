#include "..\..\GUI\GuiApplication.h"
#include "..\..\GUI\NativeWindow\Windows\GDI\WinGDISimpleElement.h"

using namespace vl;
using namespace vl::presentation;
using namespace vl::presentation::elements;

class MainWindow : public GuiWindow
{
public:
	GuiTextButton*					button;

	MainWindow()
	{
		SetTitle(L"Vczh Library++ 3.0 Gui Window");
		SetClientSize(Size(800, 600));
		MoveToScreenCenter();

		GuiFreeGrid* grid=new GuiFreeGrid;
		SetContainer(grid);

		button=new GuiTextButton;
		button->SetText(L"Press me!");
		button->SetBounds(Rect(Point(10, 10), Size(100, 30)));
		grid->InsertControl(0, button);
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