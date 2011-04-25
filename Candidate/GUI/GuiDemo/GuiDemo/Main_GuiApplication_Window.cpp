#include "..\..\GUI\GuiControl.h"

using namespace vl;
using namespace vl::presentation;

void Main_GuiApplication_Window()
{
	GuiWindow* window=new GuiWindow();
	window->SetTitle(L"Vczh Library++ 3.0 Gui Window");
	window->SetClientSize(Size(800, 600));
	window->MoveToScreenCenter();
	GetCurrentApplication()->Run(window);
}