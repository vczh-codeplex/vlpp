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
				HWND							handle;
				WString							title;
				List<INativeWindowListener*>	listeners;
				int								mouseLastX;
				int								mouseLastY;
				int								mouseHoving;
				
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

				NativeWindowMouseInfo ConvertMouse(WPARAM wParam, LPARAM lParam, bool wheelMessage)
				{
					NativeWindowMouseInfo info;
					if(wheelMessage)
					{
						info.wheel=GET_WHEEL_DELTA_WPARAM(wParam);
						wParam=GET_KEYSTATE_WPARAM(wParam);
					}
					else
					{
						info.wheel=0;
					}
					info.ctrl=(wParam & MK_CONTROL)!=0;
					info.shift=(wParam & MK_SHIFT)!=0;
					info.left=(wParam & MK_LBUTTON)!=0;
					info.middle=(wParam & MK_MBUTTON)!=0;
					info.right=(wParam & MK_RBUTTON)!=0;
					POINTS Point=MAKEPOINTS(lParam);
					info.x=Point.x;
					info.y=Point.y;
					return info;
				}

				void TrackMouse(bool enable)
				{
					TRACKMOUSEEVENT trackMouseEvent;
					trackMouseEvent.cbSize=sizeof(trackMouseEvent);
					trackMouseEvent.hwndTrack=handle;
					trackMouseEvent.dwFlags=(enable?0:TME_CANCEL) | TME_HOVER | TME_LEAVE;
					trackMouseEvent.dwHoverTime=HOVER_DEFAULT;
					TrackMouseEvent(&trackMouseEvent);
				}

			public:
				WindowsForm(HWND parent, WString className, HINSTANCE hInstance)
					:mouseLastX(-1)
					,mouseLastY(-1)
					,mouseHoving(false)
				{
					DWORD exStyle=WS_EX_APPWINDOW | WS_EX_CONTROLPARENT;
					DWORD style=WS_BORDER | WS_CAPTION | WS_SIZEBOX | WS_SYSMENU | WS_POPUP | WS_CLIPSIBLINGS | WS_CLIPCHILDREN | WS_MAXIMIZEBOX | WS_MINIMIZEBOX;
					handle=CreateWindowEx(exStyle, className.Buffer(), L"", style, CW_USEDEFAULT, CW_USEDEFAULT, CW_USEDEFAULT, CW_USEDEFAULT, parent, NULL, hInstance, NULL);
				}

				~WindowsForm()
				{
					DestroyWindow(handle);
				}

				bool HandleMessage(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam, LRESULT& result)
				{
					switch(uMsg)
					{
					case WM_MOVING:case WM_SIZING:
						{
							LPRECT rawBounds=(LPRECT)lParam;
							Rect bounds(rawBounds->left, rawBounds->top, rawBounds->right, rawBounds->bottom);
							for(int i=0;i<listeners.Count();i++)
							{
								listeners[i]->Moving(bounds);
							}
							if(		rawBounds->left!=bounds.Left()
								||	rawBounds->top!=bounds.Top()
								||	rawBounds->right!=bounds.Right()
								||	rawBounds->bottom!=bounds.Bottom())
							{
								rawBounds->left=bounds.Left();
								rawBounds->top=bounds.Top();
								rawBounds->right=bounds.Right();
								rawBounds->bottom=bounds.Bottom();
								result=TRUE;
							}
						}
						break;
					case WM_MOVE:case WM_SIZE:
						{
							for(int i=0;i<listeners.Count();i++)
							{
								listeners[i]->Moved();
							}
						}
						break;
					case WM_ENABLE:
						{
							for(int i=0;i<listeners.Count();i++)
							{
								if(wParam==TRUE)
								{
									listeners[i]->Enabled();
								}
								else
								{
									listeners[i]->Disabled();
								}
							}
						}
						break;
					case WM_SETFOCUS:
						{
							for(int i=0;i<listeners.Count();i++)
							{
								listeners[i]->GotFocus();
							}
						}
						break;
					case WM_KILLFOCUS:
						{
							for(int i=0;i<listeners.Count();i++)
							{
								listeners[i]->LostFocus();
							}
						}
						break;
					case WM_ACTIVATE:
						{
							for(int i=0;i<listeners.Count();i++)
							{
								if(wParam==WA_ACTIVE || wParam==WA_CLICKACTIVE)
								{
									listeners[i]->Activated();
								}
								else
								{
									listeners[i]->Deactivated();
								}
							}
						}
						break;
					case WM_SHOWWINDOW:
						{
							if(wParam==TRUE)
							{
								for(int i=0;i<listeners.Count();i++)
								{
									listeners[i]->Opened();
								}
							}
							else
							{
								for(int i=0;i<listeners.Count();i++)
								{
									listeners[i]->Closed();
								}
							}
						}
						break;
					case WM_CLOSE:
						{
							bool cancel=false;
							for(int i=0;i<listeners.Count();i++)
							{
								listeners[i]->Closing(cancel);
							}
							return cancel;
						}
						break;
					case WM_LBUTTONDOWN:
						{
							NativeWindowMouseInfo info=ConvertMouse(wParam, lParam, false);
							for(int i=0;i<listeners.Count();i++)
							{
								listeners[i]->LeftButtonDown(info);
							}
						}
						break;
					case WM_LBUTTONUP:
						{
							NativeWindowMouseInfo info=ConvertMouse(wParam, lParam, false);
							for(int i=0;i<listeners.Count();i++)
							{
								listeners[i]->LeftButtonUp(info);
							}
						}
						break;
					case WM_LBUTTONDBLCLK:
						{
							NativeWindowMouseInfo info=ConvertMouse(wParam, lParam, false);
							for(int i=0;i<listeners.Count();i++)
							{
								listeners[i]->LeftButtonDoubleClick(info);
							}
						}
						break;
					case WM_RBUTTONDOWN:
						{
							NativeWindowMouseInfo info=ConvertMouse(wParam, lParam, false);
							for(int i=0;i<listeners.Count();i++)
							{
								listeners[i]->RightButtonDown(info);
							}
						}
						break;
					case WM_RBUTTONUP:
						{
							NativeWindowMouseInfo info=ConvertMouse(wParam, lParam, false);
							for(int i=0;i<listeners.Count();i++)
							{
								listeners[i]->RightButtonUp(info);
							}
						}
						break;
					case WM_RBUTTONDBLCLK:
						{
							NativeWindowMouseInfo info=ConvertMouse(wParam, lParam, false);
							for(int i=0;i<listeners.Count();i++)
							{
								listeners[i]->RightButtonDoubleClick(info);
							}
						}
						break;
					case WM_MBUTTONDOWN:
						{
							NativeWindowMouseInfo info=ConvertMouse(wParam, lParam, false);
							for(int i=0;i<listeners.Count();i++)
							{
								listeners[i]->MiddleButtonDown(info);
							}
						}
						break;
					case WM_MBUTTONUP:
						{
							NativeWindowMouseInfo info=ConvertMouse(wParam, lParam, false);
							for(int i=0;i<listeners.Count();i++)
							{
								listeners[i]->MiddleButtonUp(info);
							}
						}
						break;
					case WM_MBUTTONDBLCLK:
						{
							NativeWindowMouseInfo info=ConvertMouse(wParam, lParam, false);
							for(int i=0;i<listeners.Count();i++)
							{
								listeners[i]->MiddleButtonDoubleClick(info);
							}
						}
						break;
					case WM_MOUSEHWHEEL:
						{
							NativeWindowMouseInfo info=ConvertMouse(wParam, lParam, true);
							for(int i=0;i<listeners.Count();i++)
							{
								listeners[i]->HorizontalWheel(info);
							}
						}
						break;
					case WM_MOUSEWHEEL:
						{
							NativeWindowMouseInfo info=ConvertMouse(wParam, lParam, true);
							for(int i=0;i<listeners.Count();i++)
							{
								listeners[i]->VerticalWheel(info);
							}
						}
						break;
					case WM_MOUSEMOVE:
						{
							NativeWindowMouseInfo info=ConvertMouse(wParam, lParam, false);
							if(info.x!=mouseLastX || info.y!=mouseLastY)
							{
								if(!mouseHoving)
								{
									mouseHoving=true;
									for(int i=0;i<listeners.Count();i++)
									{
										listeners[i]->MouseEntered();
									}
									TrackMouse(true);
								}
								for(int i=0;i<listeners.Count();i++)
								{
									listeners[i]->MouseMoving(info);
								}
							}
						}
						break;
					case WM_MOUSELEAVE:
						{
							mouseLastX=-1;
							mouseLastY=-1;
							mouseHoving=false;
							for(int i=0;i<listeners.Count();i++)
							{
								listeners[i]->MouseLeaved();
							}
						}
						break;
					case WM_MOUSEHOVER:
						{
							TrackMouse(true);
						}
						break;
					case WM_KEYUP:
						{
							for(int i=0;i<listeners.Count();i++)
							{
								listeners[i]->KeyUp(wParam, (lParam>>29)%1==1);
							}
						}
						break;
					case WM_KEYDOWN:
						{
							for(int i=0;i<listeners.Count();i++)
							{
								listeners[i]->KeyDown(wParam, (lParam>>29)%1==1);
							}
						}
						break;
					case WM_SYSKEYUP:
						{
							for(int i=0;i<listeners.Count();i++)
							{
								listeners[i]->SysKeyUp(wParam, (lParam>>29)%1==1);
							}
						}
						break;
					case WM_SYSKEYDOWN:
						{
							for(int i=0;i<listeners.Count();i++)
							{
								listeners[i]->SysKeyDown(wParam, (lParam>>29)%1==1);
							}
						}
						break;
					case WM_CHAR:
						{
							for(int i=0;i<listeners.Count();i++)
							{
								listeners[i]->Char(wParam);
							}
						}
						break;
					}
					return false;
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
					if(listeners.Contains(listener))
					{
						return false;
					}
					else
					{
						listeners.Add(listener);
						return true;
					}
				}

				bool UninstallListener(INativeWindowListener* listener)
				{
					if(listeners.Contains(listener))
					{
						listeners.Remove(listener);
						return true;
					}
					else
					{
						return false;
					}
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
				WinClass							windowClass;
				WinClass							godClass;
				HINSTANCE							hInstance;
				HWND								godWindow;
				Dictionary<HWND, WindowsForm*>		windows;
				List<INativeControllerListener*>	listeners;
				INativeWindow*						mainWindow;

			public:
				WindowsController(HINSTANCE _hInstance)
					:hInstance(_hInstance)
					,windowClass(L"VczhWindow", false, false, WndProc, _hInstance)
					,godClass(L"GodWindow", false, false, GodProc, _hInstance)
					,mainWindow(0)
				{
					godWindow=CreateWindowEx(WS_EX_CONTROLPARENT, godClass.GetName().Buffer(), L"GodWindow", WS_OVERLAPPEDWINDOW, 0, 0, 0, 0, NULL, NULL, hInstance, NULL);
				}

				~WindowsController()
				{
					DestroyWindow(godWindow);
				}

				bool HandleMessage(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam, LRESULT& result)
				{
					bool skipDefaultProcedure=false;
					int index=windows.Keys().IndexOf(hwnd);
					if(index!=-1)
					{
						WindowsForm* window=windows.Values()[index];
						skipDefaultProcedure=window->HandleMessage(hwnd, uMsg, wParam, lParam, result);
						switch(uMsg)
						{
						case WM_CLOSE:
							if(!skipDefaultProcedure)
							{
								ShowWindow(window->GetHandle(), SW_HIDE);
								if(window!=mainWindow)
								{
									skipDefaultProcedure=true;
								}
							}
							break;
						case WM_DESTROY:
							DestroyNativeWindow(window);
							if(window==mainWindow)
							{
								for(int i=0;i<windows.Count();i++)
								{
									if(windows.Values()[i]->IsVisible())
									{
										windows.Values()[i]->Hide();
									}
								}
								PostQuitMessage(0);
							}
							break;
						}
					}
					return skipDefaultProcedure;
				}

				INativeWindow* CreateNativeWindow()
				{
					WindowsForm* window=new WindowsForm(godWindow, windowClass.GetName(), hInstance);
					windows.Add(window->GetHandle(), window);
					return window;
				}

				void DestroyNativeWindow(INativeWindow* window)
				{
					WindowsForm* windowsForm=dynamic_cast<WindowsForm*>(window);
					if(windowsForm!=0 && windows.Keys().Contains(windowsForm->GetHandle()))
					{
						windows.Remove(windowsForm->GetHandle());
						delete windowsForm;
					}
				}

				INativeWindow* GetMainWindow()
				{
					return mainWindow;
				}

				void SetMainWindow(INativeWindow* window)
				{
					mainWindow=window;
				}

				bool InstallListener(INativeControllerListener* listener)
				{
					if(listeners.Contains(listener))
					{
						return false;
					}
					else
					{
						listeners.Add(listener);
						return true;
					}
				}

				bool UninstallListener(INativeControllerListener* listener)
				{
					if(listeners.Contains(listener))
					{
						listeners.Remove(listener);
						return true;
					}
					else
					{
						return false;
					}
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
					LRESULT result=0;
					if(controller->HandleMessage(hwnd, uMsg, wParam, lParam, result))
					{
						return result;
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