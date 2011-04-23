#include "..\..\GUI\NativeWindow\Windows\GDI\WinGDIApplication.h"
#include "..\..\GUI\NativeWindow\Windows\GDI\WinGDISimpleElement.h"

using namespace vl;
using namespace vl::presentation;
using namespace vl::presentation::windows;
using namespace vl::presentation::elements;

class ThickBorder : public WinGDIElement
{
public:
	Ptr<WinBrush>				emptyBrush;
	Ptr<WinBrush>				brush;
	Ptr<WinPen>					pen;
	Rect						bounds;

	ThickBorder(COLORREF brushColor, COLORREF penColor, WinGDIElementEnvironment* _environment)
		:WinGDIElement(_environment)
	{
		emptyBrush=new WinBrush();
		brush=new WinBrush(brushColor);
		pen=new WinPen(PS_SOLID, 1, penColor);
	}

	void Paint(Size offset, windows::WinDC* dc)
	{
		int x=bounds.Left()+offset.x;
		int y=bounds.Top()+offset.y;
		int w=bounds.Width();
		int h=bounds.Height();

		dc->SetBrush(brush);
		dc->FillRect(x, y, x+w-1, y+h-1);
		dc->SetBrush(emptyBrush);
		dc->SetPen(pen);
		for(int i=0;i<10;i++)
		{
			dc->Rectangle(x+i, y+i, x+w-1-i, y+h-1-i);
		}
	}
};

class WindowPainter : public Object, public INativeWindowListener
{
protected:
	INativeWindow*				window;
	WinGDIElementEnvironment*	env;

	Ptr<WinGDIClipElement>		container;
	Ptr<ThickBorder>			background;
	Ptr<WinGDIClipElement>		subContainer;
	Ptr<ThickBorder>			subLeft;
	Ptr<ThickBorder>			subRight;
public:
	WindowPainter(INativeWindow* _window)
		:window(_window)
		,env(GetNativeWindowGDIElementEnvironment(_window))
	{
		background=new ThickBorder(RGB(0, 0, 0), RGB(255, 0, 128), env);
		subLeft=new ThickBorder(RGB(128, 128, 255), RGB(0, 255, 128), env);
		subRight=new ThickBorder(RGB(128, 128, 255), RGB(0, 255, 128), env);
		container=new WinGDIClipElement(env);
		subContainer=new WinGDIClipElement(env);

		container->Children().Add(background);
		container->Children().Add(subContainer);
		subContainer->Children().Add(subLeft);
		subContainer->Children().Add(subRight);

		env->SetRootElement(container);
	}

	void Moved()
	{
		container->SetBounds(Rect(Point(0, 0), window->GetClientSize()));
		background->bounds=Rect(Point(0, 0), window->GetClientSize());
		subContainer->SetBounds(Rect(Point(20, 20), window->GetClientSize()-Size(40, 40)));

		Size c=subContainer->GetBounds().GetSize();
		subLeft->bounds=Rect(
			Point(-9, -9),
			Size(c.x/2, c.y+18)
			);
		subRight->bounds=Rect(
			Point(c.x-c.x/2+9, -9),
			Size(c.x/2, c.y+18)
			);
		env->Paint();
	}
};

void NativeMain()
{
	WinGDISimpleResources::RegisterAutoInstall();
	INativeController* controller=GetCurrentController();
	INativeWindow* window=controller->CreateNativeWindow();
	WindowPainter painter(window);
	window->InstallListener(&painter);
	window->SetTitle(L"Vczh Library++ 3.0 Native Window");
	window->SetClientSize(Size(800, 600));

	INativeScreen* screen=controller->GetScreen(window);
	if(screen)
	{
		Rect client=screen->GetClientBounds();
		Rect bounds=window->GetBounds();
		int x=(client.Width()-bounds.Width())/2+client.Left();
		int y=(client.Height()-bounds.Height())/2+client.Top();
		window->SetBounds(Rect(Point(x, y), bounds.GetSize()));
	}

	controller->Run(window);
	WinGDISimpleResources::UnregisterAutoInstall();
}