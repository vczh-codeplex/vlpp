#include "..\Presentation\WindowsCore\WinGdiApplication.h"
#include "..\Presentation\Layout\LayoutHost.h"

using namespace vl;
using namespace vl::presentation;

class LayoutVisualizerBase : public Object, public INativeWindowListener
{
protected:
	INativeApplication*			application;
	INativeWindow*				mainWindow;
	LayoutBase*					layout;

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
	LayoutVisualizerBase(INativeApplication* _application, INativeWindow* _mainWindow)
		:application(_application)
		,mainWindow(_mainWindow)
	{
		mainWindow->InstallListener(this);

		background=application->GetGraphics()->CreateBrush(Color(255, 255, 255));
		hostPen=application->GetGraphics()->CreatePen(Color(255, 0, 0));
		hostBrush=application->GetGraphics()->CreateBrush(Color(192, 192, 192));
	}

	~LayoutVisualizerBase()
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
		Size min=layout->GetMinSize()+(oldSize-clientSize)+Size(2, 2);
		Size max=layout->GetMaxSize()+(oldSize-clientSize)+Size(2, 2);
		LimitMovingBound(oldBounds.x1, oldBounds.x2, bounds.x1, bounds.x2, min.x, max.x);
		LimitMovingBound(oldBounds.y1, oldBounds.y2, bounds.y1, bounds.y2, min.y, max.y);
	}

	void Moved()
	{
		layout->SetSize(mainWindow->GetClientSize()-Size(2, 2));
		layout->AdjustHosts();
		layout->CalculateMinMax();
		INativeCanvas* canvas=application->LockWindow(mainWindow);
		canvas->DrawRectangle(Rect(Point(), mainWindow->GetClientSize()), hostPen, background);
		int count=layout->GetHostCount();
		for(int i=0;i<count;i++)
		{
			LayoutHost* host=layout->GetHost(i);
			Rect bounds=host->GetRealBounds();
			bounds.x1+=1;
			bounds.x2+=1;
			bounds.y1+=1;
			bounds.y2+=1;
			canvas->DrawRectangle(bounds, hostPen, hostBrush);
		}
		application->UnlockWindow(mainWindow, canvas);
	}
};

class FreeLayoutVisualizer : public LayoutVisualizerBase
{
protected:
	LayoutHost					hosts[5];
	FreeLayout					freeLayout;
public:
	FreeLayoutVisualizer(INativeApplication* _application, INativeWindow* _mainWindow)
		:LayoutVisualizerBase(_application, _mainWindow)
	{
		layout=&freeLayout;

		hosts[0].SetBounds(Rect(Point(), Size(0, 50)));
		hosts[0].SetMargin(Margin(3, 3, 3, -1));

		hosts[1].SetBounds(Rect(Point(), Size(0, 50)));
		hosts[1].SetMargin(Margin(3, -1, 3, 3));

		hosts[2].SetBounds(Rect(Point(), Size(50, 0)));
		hosts[2].SetMargin(Margin(3, 56, -1, 56));

		hosts[3].SetBounds(Rect(Point(), Size(50, 0)));
		hosts[3].SetMargin(Margin(-1, 56, 3, 56));

		hosts[4].SetMargin(Margin(56, 56, 56, 56));
		hosts[4].SetMinSize(Size(200, 100));
		hosts[4].SetMaxSize(Size(800, 600));

		for(int i=0;i<sizeof(hosts)/sizeof(*hosts);i++)
		{
			freeLayout.Hosts().Add(&hosts[i]);
		}
	}
};

extern Rect GuiGetWorkArea();

void LayoutVisualizerMain()
{
	INativeApplication* application=windows::CreateGdiApplication();
	{
		INativeWindow* mainWindow=application->CreateNativeWindow();
		FreeLayoutVisualizer listener(application, mainWindow);
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