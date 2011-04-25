#include "..\..\GUI\GuiControl.h"

using namespace vl;
using namespace vl::presentation;

void Main_GuiApplication_Window()
{
	GuiWindow* window=new GuiWindow();
	window->SetTitle(L"Vczh Library++ 3.0 Gui Window");
	window->SetClientSize(Size(800, 600));
	window->MoveToScreenCenter();
	window->Show();

	GuiWindow* another=new GuiWindow();
	another->SetTitle(L"This is another window");
	another->SetClientSize(Size(300, 200));
	another->MoveToScreenCenter();
	another->Show();

	GetCurrentApplication()->Run(window);
}