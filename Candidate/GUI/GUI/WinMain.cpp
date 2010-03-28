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

	INativeWindow* mainWindow=controller->CreateNativeWindow();
	controller->SetMainWindow(mainWindow);
	mainWindow->SetTitle(L"Vczh Window");
	mainWindow->SetClientSize(Size(800, 600));
	mainWindow->Show();

	INativeWindow* sideWindow=controller->CreateNativeWindow();
	sideWindow->SetTitle(L"Side Window");
	sideWindow->SetMaximizedBox(false);
	sideWindow->SetMinimizedBox(false);
	sideWindow->SetSizeBox(false);
	sideWindow->SetClientSize(Size(400, 300));
	sideWindow->HideInTaskBar();
	sideWindow->Show();
	sideWindow->Hide();
	sideWindow->Show();

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