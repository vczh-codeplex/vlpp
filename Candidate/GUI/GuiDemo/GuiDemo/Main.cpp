#include "..\..\GUI\NativeWindow\WinNativeWindow.h"

using namespace vl;
using namespace vl::presentation;

void NativeMain()
{
	INativeController* controller=GetCurrentController();
	INativeWindow* window=controller->CreateNativeWindow();
	window->SetTitle(L"Vczh Library++ 3.0 Native Window");
	window->SetClientSize(Size(800, 600));
	controller->Run(window);
}