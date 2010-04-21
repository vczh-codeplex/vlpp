#include "..\Presentation\WindowsCore\WinGdiApplication.h"
#include "..\Presentation\Layout\LayoutHost.h"

using namespace vl;
using namespace vl::presentation;

class LayoutVisualizer : public Object, public INativeWindowListener
{
private:
	INativeApplication*			application;
	INativeWindow*				mainWindow;
	LayoutHost					host;

	INativeBrush*				background;
	INativePen*					hostPen;
	INativeBrush*				hostBrush;

public:
	LayoutVisualizer(INativeApplication* _application, INativeWindow* _mainWindow)
		:application(_application)
		,mainWindow(_mainWindow)
	{
		mainWindow->InstallListener(this);
		host.SetMinSize(Size(200, 200));
		host.SetMaxSize(Size(800, 600));
		host.SetMargin(Margin(3, 3, 3, 3));

		background=application->GetGraphics()->CreateBrush(Color(255, 255, 255));
		hostPen=application->GetGraphics()->CreatePen(Color(255, 0, 0));
		hostBrush=application->GetGraphics()->CreateBrush(Color(192, 192, 192));
	}

	~LayoutVisualizer()
	{
		application->GetGraphics()->Destroy(background);
		application->GetGraphics()->Destroy(hostPen);
		application->GetGraphics()->Destroy(hostBrush);
	}

	void Moved()
	{
		host.SetMarginRelativeBounds(Rect(Point(), mainWindow->GetClientSize()));
		INativeCanvas* canvas=application->LockWindow(mainWindow);
		canvas->DrawRectangle(Rect(Point(), mainWindow->GetClientSize()), hostPen, background);
		canvas->DrawRectangle(host.GetRealBounds(), hostPen, hostBrush);
		application->UnlockWindow(mainWindow, canvas);
	}
};

extern Rect GuiGetWorkArea();

void LayoutVisualizerMain()
{
	INativeApplication* application=windows::CreateGdiApplication();
	{
		INativeWindow* mainWindow=application->CreateNativeWindow();
		LayoutVisualizer listener(application, mainWindow);
		Rect area=GuiGetWorkArea();

		mainWindow->SetMaximizedBox(false);
		mainWindow->SetMinimizedBox(false);
		mainWindow->SetClientSize(Size(640, 480));
		mainWindow->SetBounds(Rect(
			Point(
				area.Left() + (area.Width()-mainWindow->GetBounds().Width())/2,
				area.Top() + (area.Height()-mainWindow->GetBounds().Height())/2
			), mainWindow->GetBounds().GetSize()
		));
		mainWindow->SetTitle(L"Layout Visualizer");
		mainWindow->Show();

		application->Run(mainWindow);
	}
	windows::DestroyGdiApplication(application);
}