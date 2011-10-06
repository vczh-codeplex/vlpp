#include "..\..\GUI\NativeWindow\Windows\GDI\WinGDIApplication.h"

using namespace vl;
using namespace vl::presentation;

void GuiMain()
{
	INativeWindow* window=GetCurrentController()->CreateNativeWindow();
	window->SetSizeBox(false);
	window->SetMaximizedBox(false);
	window->SetMinimizedBox(false);
	window->SetClientSize(Size(800, 600));
	window->SetTitle(L"Vczh GUI Demo");

	INativeScreen* screen=GetCurrentController()->GetScreen(0);
	Rect windowBounds=window->GetBounds();
	Rect screenBounds=screen->GetClientBounds();
	window->SetBounds(Rect(
		Point(
			screenBounds.Left()+(screenBounds.Width()-windowBounds.Width())/2,
			screenBounds.Top()+(screenBounds.Height()-windowBounds.Height())/2
			),
		windowBounds.GetSize()
		));
	window->Show();

	GetCurrentController()->Run(window);
}