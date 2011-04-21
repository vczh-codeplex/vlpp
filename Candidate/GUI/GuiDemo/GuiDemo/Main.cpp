#include "..\..\GUI\NativeWindow\Windows\GDI\WinGDIApplication.h"

using namespace vl;
using namespace vl::presentation;
using namespace vl::presentation::windows;

class WindowPainter : public Object, public INativeWindowListener
{
protected:
	INativeWindow*				window;
	WinDC*						bufferDC;
	Ptr<WinBrush>				backgroundBrush;
	Ptr<WinPen>					foregroundPen;
public:
	WindowPainter(INativeWindow* _window)
		:window(_window)
		,bufferDC(0)
	{
		backgroundBrush=new WinBrush(RGB(0, 0, 0));
		foregroundPen=new WinPen(PS_SOLID, 1, RGB(255, 0, 0));
	}

	void Moved()
	{
		WinDC* dc=GetNativeWindowDC(window);
		dc->SetBrush(backgroundBrush);
		dc->SetPen(foregroundPen);
		Size size=window->GetClientSize();
		dc->Rectangle(0, 0, size.x-1, size.y-1);
	}
};

void NativeMain()
{
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
		vint x=(client.Width()-bounds.Width())/2+client.Left();
		vint y=(client.Height()-bounds.Height())/2+client.Top();
		window->SetBounds(Rect(Point(x, y), bounds.GetSize()));
	}

	controller->Run(window);
}