#if _DEBUG
#define _CRTDBG_MAP_ALLOC
#include <stdlib.h>
#include <crtdbg.h>
#endif

#include"WindowSetup.h"		

/***********************************************************************
Window
***********************************************************************/

extern LRESULT CALLBACK DirectXProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam, bool& callDefWindowProc);
extern void CALLBACK DirectXProcIdle();

LRESULT CALLBACK WndProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	bool callDefWindowProc=true;
	switch(uMsg)
	{
	case WM_DESTROY:
		PostQuitMessage(0);
		break;
	case WM_ERASEBKGND:
		callDefWindowProc=false;
		break;
	}

	LRESULT result=DirectXProc(hwnd, uMsg, wParam, lParam, callDefWindowProc);
	if(!callDefWindowProc)
	{
		return result;
	}
	return DefWindowProc(hwnd, uMsg, wParam, lParam);
}

BOOL CALLBACK PrimaryMonitorSelector(
	HMONITOR hMonitor,
	HDC hdcMonitor,
	LPRECT lprcMonitor,
	LPARAM dwData
)
{
	MONITORINFOEX info;
	info.cbSize=sizeof(MONITORINFOEX);
	GetMonitorInfo(hMonitor, &info);
	if(info.dwFlags==MONITORINFOF_PRIMARY)
	{
		HMONITOR* pMonitor=(HMONITOR*)dwData;
		*pMonitor=hMonitor;
		return FALSE;
	}
	else
	{
		return TRUE;
	}
}

void RunWindow(HINSTANCE hInstance, int clientWidth, int clientHeight)
{
	//=========================================
	// global variables
	WNDCLASSEX windowClass;
	HWND mainWindowHandle;

	//=========================================
	// create class
	{
		windowClass.cbSize=sizeof(windowClass);
		windowClass.style=CS_HREDRAW | CS_VREDRAW | CS_DBLCLKS | CS_OWNDC;
		windowClass.lpfnWndProc=WndProc;
		windowClass.cbClsExtra=0;
		windowClass.cbWndExtra=0;
		windowClass.hInstance=hInstance;
		windowClass.hIcon=LoadIcon(NULL,IDI_APPLICATION);
		windowClass.hCursor=LoadCursor(NULL,IDC_ARROW);
		windowClass.hbrBackground=GetSysColorBrush(COLOR_BTNFACE);
		windowClass.lpszMenuName=NULL;
		windowClass.lpszClassName=L"VczhDirectXWindowClass";
		windowClass.hIconSm=NULL;
		RegisterClassEx(&windowClass);
	}

	//=========================================
	// create window
	{
		DWORD exStyle=WS_EX_APPWINDOW | WS_EX_CONTROLPARENT;
		DWORD style=WS_BORDER | WS_CAPTION | WS_SIZEBOX | WS_SYSMENU | WS_POPUP | WS_CLIPSIBLINGS | WS_CLIPCHILDREN | WS_MAXIMIZEBOX | WS_MINIMIZEBOX;
		mainWindowHandle=CreateWindowEx(exStyle, windowClass.lpszClassName, L"Vczh DirectX Window", style, CW_USEDEFAULT, CW_USEDEFAULT, CW_USEDEFAULT, CW_USEDEFAULT, NULL, NULL, hInstance, NULL);
	}
	
	//=========================================
	// calculate position
	{
		HMONITOR primaryMonitor=0;
		EnumDisplayMonitors(NULL, NULL, PrimaryMonitorSelector, (LPARAM)(&primaryMonitor));
		MONITORINFOEX monitorInfo;
		monitorInfo.cbSize=sizeof(MONITORINFOEX);
		GetMonitorInfo(primaryMonitor, &monitorInfo);
	
		RECT required={0, 0, clientWidth, clientHeight};
		AdjustWindowRect(&required, GetWindowLongPtr(mainWindowHandle, GWL_STYLE), FALSE);

		int width=required.right-required.left;
		int height=required.bottom-required.top;
		int left=monitorInfo.rcWork.left+(monitorInfo.rcWork.right-monitorInfo.rcWork.left-width)/2;
		int top=monitorInfo.rcWork.top+(monitorInfo.rcWork.bottom-monitorInfo.rcWork.top-height)/2;
		MoveWindow(mainWindowHandle, left, top, width, height, TRUE);
	}
	
	//=========================================
	// run
	{
		ShowWindow(mainWindowHandle, SW_SHOWNORMAL);
		MSG message;
		while(true)
		{
			if(PeekMessage(&message, NULL, 0, 0, PM_REMOVE))
			{
				if(message.message==WM_QUIT)
				{
					break;
				}
				else
				{
					TranslateMessage(&message);
					DispatchMessage(&message);
				}
			}
			DirectXProcIdle();
		}
	}
	
	//=========================================
	// exit
	DestroyWindow(mainWindowHandle);
}

/***********************************************************************
WinMain
***********************************************************************/

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow)
{
	RunWindow(hInstance, 800, 600);
#if _DEBUG
	_CrtDumpMemoryLeaks();
#endif
	return 0;
}

namespace vl
{
	namespace directx
	{
/***********************************************************************
WindowMessageHelper
***********************************************************************/

		WindowMouseInfo::WindowMouseInfo(WPARAM wParam, LPARAM lParam, bool wheelMessage)
		{
			if(wheelMessage)
			{
				this->wheel=GET_WHEEL_DELTA_WPARAM(wParam);
				wParam=GET_KEYSTATE_WPARAM(wParam);
			}
			else
			{
				this->wheel=0;
			}
			this->ctrl=(wParam & MK_CONTROL)!=0;
			this->shift=(wParam & MK_SHIFT)!=0;
			this->left=(wParam & MK_LBUTTON)!=0;
			this->middle=(wParam & MK_MBUTTON)!=0;
			this->right=(wParam & MK_RBUTTON)!=0;
			POINTS Point=MAKEPOINTS(lParam);
			this->x=Point.x;
			this->y=Point.y;
		}

		WindowKeyInfo::WindowKeyInfo(WPARAM wParam, LPARAM lParam)
		{
			this->key=wParam;
			this->alt=(lParam>>29)%1==1;
		}

		void WindowTrackMouse(HWND handle, bool enable)
		{
			TRACKMOUSEEVENT trackMouseEvent;
			trackMouseEvent.cbSize=sizeof(trackMouseEvent);
			trackMouseEvent.hwndTrack=handle;
			trackMouseEvent.dwFlags=(enable?0:TME_CANCEL) | TME_HOVER | TME_LEAVE;
			trackMouseEvent.dwHoverTime=HOVER_DEFAULT;
			TrackMouseEvent(&trackMouseEvent);
		}

		SIZE WindowGetClient(HWND handle)
		{
			RECT required={0,0,0,0};
			RECT bounds;
			GetWindowRect(handle, &bounds);
			AdjustWindowRect(&required, GetWindowLongPtr(handle, GWL_STYLE), FALSE);
			int width=(bounds.right-bounds.left)-(required.right-required.left);
			int height=(bounds.bottom-bounds.top)-(required.bottom-required.top);

			SIZE size;
			size.cx=width;
			size.cy=height;
			return size;
		}
	}
}