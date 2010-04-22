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

	void LimitMovingBound(int olda, int oldb, int& newa, int& newb, int min, int max)
	{
		int newl=newb-newa;
		int limited=newl;
		if(newl<min)
		{
			limited=min;
		}
		else if(newl>max)
		{
			limited=max;
		}
		if(newl!=limited)
		{
			if(olda!=newa)
			{
				newa=newb-limited;
			}
			else
			{
				newb=newa+limited;
			}
		}
	}
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

	void Moving(Rect& bounds)
	{
		Rect oldBounds=mainWindow->GetBounds();
		Size oldSize=oldBounds.GetSize();
		Size clientSize=mainWindow->GetClientSize();
		Size min=host.GetMinBounds()+(oldSize-clientSize);
		Size max=host.GetMaxBounds()+(oldSize-clientSize);
		LimitMovingBound(oldBounds.x1, oldBounds.x2, bounds.x1, bounds.x2, min.x, max.x);
		LimitMovingBound(oldBounds.y1, oldBounds.y2, bounds.y1, bounds.y2, min.y, max.y);
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