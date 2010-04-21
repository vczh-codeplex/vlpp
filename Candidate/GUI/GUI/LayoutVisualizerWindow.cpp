#include "..\Presentation\WindowsCore\WinGdiApplication.h"

using namespace vl;
using namespace vl::presentation;

extern Rect GuiGetWorkArea();

void LayoutVisualizerMain()
{
	INativeApplication* application=windows::CreateGdiApplication();
	Rect area=GuiGetWorkArea();

	INativeWindow* mainWindow=application->CreateNativeWindow();
	mainWindow->SetSizeBox(false);
	mainWindow->SetMaximizedBox(false);
	mainWindow->SetMinimizedBox(false);
	mainWindow->SetClientSize(Size(800, 600));
	mainWindow->SetBounds(Rect(
		Point(
			area.Left() + (area.Width()-mainWindow->GetBounds().Width())/2,
			area.Top() + (area.Height()-mainWindow->GetBounds().Height())/2
		), mainWindow->GetBounds().GetSize()
	));
	mainWindow->SetTitle(L"Layout Visualizer");
	mainWindow->Show();

	application->Run(mainWindow);
	windows::DestroyGdiApplication(application);
}