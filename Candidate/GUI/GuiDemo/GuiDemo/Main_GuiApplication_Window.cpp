#include "..\..\GUI\GuiApplication.h"
#include "..\..\GUI\NativeWindow\Windows\GDI\WinGDISimpleElement.h"

using namespace vl;
using namespace vl::presentation;
using namespace vl::presentation::elements;

void Main_GuiApplication_Window()
{
	gdi_simple::Resources::RegisterAutoInstall();

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