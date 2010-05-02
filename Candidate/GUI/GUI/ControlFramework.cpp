#include "..\Presentation\WindowsCore\WinGdiApplication.h"
#include "..\Presentation\Framework\Window.h"

using namespace vl;
using namespace vl::presentation;

extern Rect GuiGetWorkArea();

class MainWindow : public Window
{
public:
	MainWindow()
	{
		ExpectedLocationChanged.Add(this, &MainWindow::Window_ExpectedLocationChanged);
		ExpectedSizeChanged.Add(this, &MainWindow::Window_ExpectedSizeChanged);
	}

	~MainWindow()
	{
	}
protected:

	WString ToString(Point s)
	{
		return L"("+itow(s.x)+L"-"+itow(s.y)+L")";
	}

	WString ToString(Size s)
	{
		return L"("+itow(s.x)+L"-"+itow(s.y)+L")";
	}

	void UpdateText()
	{
		WString text=WString(L"vczh")
			+L", location="
			+ToString(GetLocation())
			+L", size="
			+ToString(GetSize())
			+L", client_size="
			+ToString(GetClientSize())
			+L", offset="
			+ToString(GetSize()-GetClientSize())
			;
		SetText(text);
	}
	
	void Window_ExpectedLocationChanged(Control*, NotifyEventArgs)
	{
		UpdateText();
	}
	
	void Window_ExpectedSizeChanged(Control*, NotifyEventArgs)
	{
		UpdateText();
	}
};

void ControlFrameworkMain()
{
	INativeApplication* application=windows::CreateGdiApplication();
	WindowManager* manager=new WindowManager();
	Rect area=GuiGetWorkArea();

	Window* mainWindow=new MainWindow();
	mainWindow->SetExpectedClientSize(Size(640, 480));
	mainWindow->SetExpectedLocation(area.LeftTop() + (area.GetSize()-mainWindow->GetSize())/2);

	manager->Run(mainWindow);
	delete manager;
	windows::DestroyGdiApplication(application);
}