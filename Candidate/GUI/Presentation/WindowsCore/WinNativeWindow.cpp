#include <windows.h>
#include "WinNativeWindow.h"
#include "..\..\..\..\Library\Collections\Dictionary.h"

namespace vl
{
	namespace presentation
	{
		namespace windows
		{
			using namespace collections;

/***********************************************************************
WindowsClass
***********************************************************************/

			class WinClass : public Object
			{
			protected:
				WString					name;
				WNDCLASSEX				windowClass;
				ATOM					windowAtom;
			public:
				WinClass(WString _name, bool shadow, bool ownDC, WNDPROC procedure, HINSTANCE hInstance)
				{
					name=_name;
					windowClass.cbSize=sizeof(windowClass);
					windowClass.style=CS_HREDRAW | CS_VREDRAW | CS_DBLCLKS | (shadow?CS_DROPSHADOW:0) | (ownDC?CS_OWNDC:0);
					windowClass.lpfnWndProc=procedure;
					windowClass.cbClsExtra=0;
					windowClass.cbWndExtra=0;
					windowClass.hInstance=hInstance;
					windowClass.hIcon=LoadIcon(NULL,IDI_APPLICATION);
					windowClass.hCursor=LoadCursor(NULL,IDC_ARROW);
					windowClass.hbrBackground=GetSysColorBrush(COLOR_BTNFACE);
					windowClass.lpszMenuName=NULL;
					windowClass.lpszClassName=name.Buffer();
					windowClass.hIconSm=NULL;
					windowAtom=RegisterClassEx(&windowClass);
				}

				bool IsAvailable()
				{
					return windowAtom!=0;
				}

				WString GetName()
				{
					return name;
				}

				ATOM GetClassAtom()
				{
					return windowAtom;
				}
			};

/***********************************************************************
WindowsForm
***********************************************************************/

			class WindowsForm : public Object, public INativeWindow
			{
			protected:
				HWND			handle;
				WString			title;
				
				DWORD InternalGetExStyle()
				{
					return GetWindowLongPtr(handle,GWL_EXSTYLE);
				}

				void InternalSetExStyle(DWORD exStyle)
				{
					SetWindowLongPtr(handle,GWL_EXSTYLE,exStyle);
					SetWindowPos(handle,0,0,0,0,0,SWP_NOSIZE | SWP_NOMOVE | SWP_NOZORDER | SWP_FRAMECHANGED);
				}

				bool GetExStyle(DWORD exStyle)
				{
					LONG_PTR Long=InternalGetExStyle();
					return (Long & exStyle) != 0;
				}

				void SetExStyle(DWORD exStyle, bool available)
				{
					LONG_PTR Long=InternalGetExStyle();
					if(available)
					{
						Long|=exStyle;
					}
					else
					{
						Long&=~exStyle;
					}
					InternalSetExStyle(Long);
				}

				bool GetStyle(DWORD style)
				{
					LONG_PTR Long=GetWindowLongPtr(handle,GWL_STYLE);
					return (Long & style) != 0;
				}

				void SetStyle(DWORD style, bool available)
				{
					LONG_PTR Long=GetWindowLongPtr(handle,GWL_STYLE);
					if(available)
					{
						Long|=style;
					}
					else
					{
						Long&=~style;
					}
					SetWindowLongPtr(handle,GWL_STYLE,Long);
					SetWindowPos(handle,0,0,0,0,0,SWP_NOSIZE | SWP_NOMOVE | SWP_NOZORDER | SWP_FRAMECHANGED);
				}
			public:
				WindowsForm(HWND parent, WString className, HINSTANCE hInstance)
				{
					DWORD exStyle=WS_EX_APPWINDOW | WS_EX_CONTROLPARENT;
					DWORD style=WS_BORDER | WS_CAPTION | WS_SIZEBOX | WS_SYSMENU | WS_POPUP | WS_CLIPSIBLINGS | WS_CLIPCHILDREN | WS_MAXIMIZEBOX | WS_MINIMIZEBOX;
					handle=CreateWindowEx(exStyle, className.Buffer(), L"", style, CW_USEDEFAULT, CW_USEDEFAULT, CW_USEDEFAULT, CW_USEDEFAULT, parent, NULL, hInstance, NULL);
				}

				~WindowsForm()
				{
					DestroyWindow(handle);
				}

				HWND GetHandle()
				{
					return handle;
				}

				Rect GetBounds()
				{
					RECT rect;
					GetWindowRect(handle, &rect);
					return Rect(rect.left, rect.top, rect.right, rect.bottom);
				}

				void SetBounds(const Rect& bounds)
				{
					MoveWindow(handle, bounds.Left(), bounds.Top(), bounds.Width(), bounds.Height(), TRUE);
				}

				Size GetClientSize()
				{
					RECT required={0,0,0,0};
					RECT bounds;
					GetWindowRect(handle, &bounds);
					AdjustWindowRect(&required, GetWindowLongPtr(handle, GWL_STYLE), FALSE);
					return Size(
						(bounds.right-bounds.left)-(required.right-required.left)
						,(bounds.bottom-bounds.top)-(required.bottom-required.top)
						);
				}

				void SetClientSize(Size size)
				{
					RECT required={0,0,size.x,size.y};
					RECT bounds;
					GetWindowRect(handle, &bounds);
					AdjustWindowRect(&required, GetWindowLongPtr(handle, GWL_STYLE), FALSE);
					MoveWindow(handle, bounds.left, bounds.top, required.right-required.left, required.bottom-required.top, TRUE);
				}

				WString GetTitle()
				{
					return title;
				}

				void SetTitle(WString _title)
				{
					title=_title;
					SetWindowText(handle, title.Buffer());
				}

				void Show()
				{
					ShowWindow(handle, SW_SHOWNORMAL);
				}

				void ShowRestored()
				{
					ShowWindow(handle, SW_RESTORE);
				}

				void ShowMaximized()
				{
					ShowWindow(handle, SW_SHOWMAXIMIZED);
				}

				void ShowMinimized()
				{
					ShowWindow(handle, SW_SHOWMINIMIZED);
				}

				void Hide()
				{
					SendMessage(handle, WM_CLOSE, NULL, NULL);
				}

				bool IsVisible()
				{
					return IsWindowVisible(handle)!=0;
				}

				void Enable()
				{
					EnableWindow(handle, TRUE);
				}

				void Disable()
				{
					EnableWindow(handle, FALSE);
				}

				bool IsEnabled()
				{
					return IsWindowEnabled(handle)!=0;
				}

				void SetFocus()
				{
					::SetFocus(handle);
				}

				bool IsFocused()
				{
					return GetFocus()==handle;
				}

				void SetActivate()
				{
					SetActiveWindow(handle);
				}

				bool IsActivated()
				{
					return GetActiveWindow()==handle;
				}

				void ShowInTaskBar()
				{
					SetExStyle(WS_EX_APPWINDOW, true);
				}

				void HideInTaskBar()
				{
					SetExStyle(WS_EX_APPWINDOW, false);
				}

				bool IsAppearedInTaskBar()
				{
					return GetExStyle(WS_EX_APPWINDOW);
				}

				bool GetMaximizedBox()
				{
					return GetStyle(WS_MAXIMIZEBOX);
				}

				void SetMaximizedBox(bool visible)
				{
					SetStyle(WS_MAXIMIZEBOX, visible);
				}

				bool GetMinimizedBox()
				{
					return GetStyle(WS_MINIMIZEBOX);
				}

				void SetMinimizedBox(bool visible)
				{
					SetStyle(WS_MINIMIZEBOX, visible);
				}

				bool GetBorder()
				{
					return GetStyle(WS_BORDER);
				}

				void SetBorder(bool visible)
				{
					SetStyle(WS_BORDER, visible);
				}

				bool GetSizeBox()
				{
					return GetStyle(WS_SIZEBOX);
				}

				void SetSizeBox(bool visible)
				{
					SetStyle(WS_SIZEBOX, visible);
				}

				bool GetIconVisible()
				{
					return GetStyle(WS_SYSMENU);
				}

				void SetIconVisible(bool visible)
				{
					SetStyle(WS_SYSMENU, visible);
				}

				bool GetTitleBar()
				{
					return GetStyle(WS_CAPTION);
				}

				void SetTitleBar(bool visible)
				{
					SetStyle(WS_CAPTION, visible);
				}

				bool InstallListener(INativeWindowListener* listener)
				{
					return false;
				}

				bool UninstallListener(INativeWindowListener* listener)
				{
					return false;
				}

			};

/***********************************************************************
WindowsController
***********************************************************************/

			LRESULT CALLBACK WndProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam);
			LRESULT CALLBACK GodProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam);

			class WindowsController : public Object, public INativeController
			{
			protected:
				WinClass						windowClass;
				WinClass						godClass;
				HINSTANCE						hInstance;
				HWND							godWindow;
				Dictionary<HWND, WindowsForm*>	windows;
			public:
				WindowsController(HINSTANCE _hInstance)
					:hInstance(_hInstance)
					,windowClass(L"VczhWindow", false, false, WndProc, _hInstance)
					,godClass(L"GodWindow", false, false, GodProc, _hInstance)
				{
					godWindow=CreateWindowEx(WS_EX_CONTROLPARENT, godClass.GetName().Buffer(), L"GodWindow", WS_OVERLAPPEDWINDOW, 0, 0, 0, 0, NULL, NULL, hInstance, NULL);
				}

				~WindowsController()
				{
					DestroyWindow(godWindow);
				}

				bool HandleMessage(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
				{
					switch(uMsg)
					{
					case WM_CLOSE:
						DestroyNativeWindow(windows[hwnd]);
						break;
					case WM_DESTROY:
						PostQuitMessage(0);
						break;
					}
					return false;
				}

				INativeWindow* CreateNativeWindow()
				{
					WindowsForm* window=new WindowsForm(godWindow, windowClass.GetName(), hInstance);
					windows.Add(window->GetHandle(), window);
					return window;
				}

				void DestroyNativeWindow(INativeWindow* window)
				{
					delete window;
				}

				bool InstallListener(INativeControllerListener* listener)
				{
					return false;
				}

				bool UninstallListener(INativeControllerListener* listener)
				{
					return false;
				}
			};

/***********************************************************************
Windows Procedure
***********************************************************************/

			LRESULT CALLBACK WndProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
			{
				WindowsController* controller=dynamic_cast<WindowsController*>(GetCurrentController());
				if(controller)
				{
					if(controller->HandleMessage(hwnd, uMsg, wParam, lParam))
					{
						return 0;
					}
				}
				return DefWindowProc(hwnd, uMsg, wParam, lParam);
			}

			LRESULT CALLBACK GodProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
			{
				return DefWindowProc(hwnd, uMsg, wParam, lParam);
			}

/***********************************************************************
Windows Platform Native Controller
***********************************************************************/

			INativeController* CreateWindowsNativeController(HINSTANCE hInstance)
			{
				return new WindowsController(hInstance);
			}

			void DestroyWindowsNativeController(INativeController* controller)
			{
				delete controller;
			}
		}
	}
}