#include "..\Presentation\WindowsCore\WinGdiApplication.h"

using namespace vl;
using namespace vl::presentation;

void DrawText(INativeWindow* window)
{
	INativeGraphics* graphics=GetCurrentApplication()->GetGraphics();
	INativeBrush* backgroundBrush=graphics->CreateBrush(Color(255, 255, 255));
	INativeBrush* textBrush=graphics->CreateBrush(Color(128, 0, 255));
	INativeFont* textFont=graphics->CreateFont(L"ËÎÌו", 64, false, false, false);

	WString string=L"Vczh Is Genius!";
	INativeCanvas* canvas=GetCurrentApplication()->LockWindow(window);
	Size clientSize=window->GetClientSize();
	Size textSize=canvas->MeasureString(string, textFont);
	Point location((clientSize.x-textSize.x)/2, (clientSize.y-textSize.y)/2);

	canvas->DrawRectangle(Rect(Point(0, 0), clientSize), 0, backgroundBrush);
	canvas->DrawString(string, location, textFont, textBrush);

	GetCurrentApplication()->UnlockWindow(window, canvas);
	graphics->Destroy(textFont);
	graphics->Destroy(textBrush);
	graphics->Destroy(backgroundBrush);
}

void TwoWindowsMain()
{
	INativeApplication* application=windows::CreateGdiApplication();

	INativeWindow* mainWindow=application->CreateNativeWindow();
	mainWindow->SetTitle(L"Vczh Window");
	mainWindow->SetClientSize(Size(800, 600));
	DrawText(mainWindow);
	mainWindow->Show();

	INativeWindow* sideWindow=application->CreateNativeWindow();
	sideWindow->SetTitle(L"Side Window");
	sideWindow->SetMaximizedBox(false);
	sideWindow->SetMinimizedBox(false);
	sideWindow->SetSizeBox(false);
	sideWindow->SetClientSize(Size(400, 300));
	sideWindow->HideInTaskBar();
	DrawText(sideWindow);
	sideWindow->Show();
	sideWindow->Hide();
	sideWindow->Show();

	application->Run(mainWindow);
	windows::DestroyGdiApplication(application);
}