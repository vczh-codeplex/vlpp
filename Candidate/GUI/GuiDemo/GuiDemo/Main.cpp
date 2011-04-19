#include "..\..\GUI\NativeWindow\WinNativeWindow.h"

using namespace vl;
using namespace vl::presentation;

void NativeMain()
{
	INativeController* controller=GetCurrentController();
	INativeWindow* window=controller->CreateNativeWindow();
	window->SetTitle(L"Vczh Library++ 3.0 Native Window");
	window->SetClientSize(Size(800, 600));

	INativeScreen* screen=controller->GetScreen(window);
	if(screen)
	{
		Rect client=screen->GetClientBounds();
		Rect bounds=window->GetBounds();
		vint x=(client.Width()-bounds.Width())/2+client.Left();
		vint y=(client.Height()-bounds.Height())/2+client.Top();
		window->SetBounds(Rect(Point(x, y), Size(bounds.Width(), bounds.Height())));
	}

	controller->Run(window);
}