#define _CRTDBG_MAP_ALLOC
#include <stdlib.h>
#include <crtdbg.h>
#include <windows.h>
#include "..\Presentation\WindowsCore\WinNativeWindow.h"

using namespace vl;
using namespace vl::presentation;

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow)
{
	INativeController* controller=windows::CreateWindowsNativeController(hInstance);
	SetCurrentController(controller);

	INativeWindow* window=controller->CreateNativeWindow();
	window->SetTitle(L"Vczh Window");
	window->SetClientSize(Size(800, 600));
	window->Show();

	MSG message;
	while(GetMessage(&message, NULL, 0, 0))
	{
		TranslateMessage(&message);
		DispatchMessage(&message);
	}

	windows::DestroyWindowsNativeController(controller);
	_CrtDumpMemoryLeaks();
	return 0;
}