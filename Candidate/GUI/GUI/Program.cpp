#define _CRTDBG_MAP_ALLOC
#include <stdlib.h>
#include <crtdbg.h>
#include "..\Presentation\WindowsCore\WinGdiApplication.h"

using namespace vl;
using namespace vl::presentation;

void NativeMain()
{
	INativeApplication* application=windows::CreateGdiApplication();

	INativeWindow* mainWindow=application->CreateNativeWindow();
	mainWindow->SetTitle(L"Vczh Window");
	mainWindow->SetClientSize(Size(800, 600));
	mainWindow->Show();

	INativeWindow* sideWindow=application->CreateNativeWindow();
	sideWindow->SetTitle(L"Side Window");
	sideWindow->SetMaximizedBox(false);
	sideWindow->SetMinimizedBox(false);
	sideWindow->SetSizeBox(false);
	sideWindow->SetClientSize(Size(400, 300));
	sideWindow->HideInTaskBar();
	sideWindow->Show();
	sideWindow->Hide();
	sideWindow->Show();

	application->Run(mainWindow);
	windows::DestroyGdiApplication(application);
	_CrtDumpMemoryLeaks();
}