/***********************************************************************
Vczh Library++ 3.0
Developer: 陈梓瀚(vczh)
GUI::Native Window

Interfaces:
  INativeWindow							：窗口适配器
  INativeWindowListener					：窗口事件监听器
  INativeController						：全局控制器
  INativeControllerListener				：全局事件监听器
***********************************************************************/

#ifndef VCZH_PRESENTATION_GUINATIVEWINDOW
#define VCZH_PRESENTATION_GUINATIVEWINDOW

#include "GuiTypes.h"

namespace vl
{
	namespace presentation
	{
		class INativeWindow;
		class INativeWindowListener;
		class INativeController;
		class INativeControllerListener;

/***********************************************************************
Native Window
***********************************************************************/

		class INativeWindow : public Interface
		{
		public:
			virtual Rect				GetBounds()=0;
			virtual void				SetBounds(const Rect& bounds)=0;
			virtual Size				GetClientSize()=0;
			virtual void				SetClientSize(Size size)=0;
			virtual WString				GetTitle()=0;
			virtual void				SetTitle(WString title)=0;

			virtual void				Show()=0;
			virtual void				ShowRestored()=0;
			virtual void				ShowMaximized()=0;
			virtual void				ShowMinimized()=0;
			virtual void				Hide()=0;
			virtual bool				IsVisible()=0;
			virtual void				Enable()=0;
			virtual void				Disable()=0;
			virtual bool				IsEnabled()=0;
			virtual void				SetFocus()=0;
			virtual bool				IsFocused()=0;
			virtual void				SetActivate()=0;
			virtual bool				IsActivated()=0;
			virtual void				ShowInTaskBar()=0;
			virtual void				HideInTaskBar()=0;
			virtual bool				IsAppearedInTaskBar()=0;
			virtual bool				RequireCapture()=0;
			virtual bool				ReleaseCapture()=0;

			virtual bool				GetMaximizedBox()=0;
			virtual void				SetMaximizedBox(bool visible)=0;
			virtual bool				GetMinimizedBox()=0;
			virtual void				SetMinimizedBox(bool visible)=0;
			virtual bool				GetBorder()=0;
			virtual void				SetBorder(bool visible)=0;
			virtual bool				GetSizeBox()=0;
			virtual void				SetSizeBox(bool visible)=0;
			virtual bool				GetIconVisible()=0;
			virtual void				SetIconVisible(bool visible)=0;
			virtual bool				GetTitleBar()=0;
			virtual void				SetTitleBar(bool visible)=0;

			virtual bool				InstallListener(INativeWindowListener* listener)=0;
			virtual bool				UninstallListener(INativeWindowListener* listener)=0;
			virtual void				RedrawContent()=0;
		};

		struct NativeWindowMouseInfo
		{
			bool						ctrl;
			bool						shift;
			bool						left;
			bool						middle;
			bool						right;
			int							x;
			int							y;
			int							wheel;
		};

		class INativeWindowListener : public Interface
		{
		public:
			virtual void				Moving(Rect& bounds);
			virtual void				Moved();
			virtual void				Enabled();
			virtual void				Disabled();
			virtual void				GotFocus();
			virtual void				LostFocus();
			virtual void				Activated();
			virtual void				Deactivated();
			virtual void				Opened();
			virtual void				Closing(bool& cancel);
			virtual void				Closed();
			virtual void				Paint();
			virtual void				Destroying();

			virtual void				LeftButtonDown(const NativeWindowMouseInfo& info);
			virtual void				LeftButtonUp(const NativeWindowMouseInfo& info);
			virtual void				LeftButtonDoubleClick(const NativeWindowMouseInfo& info);
			virtual void				RightButtonDown(const NativeWindowMouseInfo& info);
			virtual void				RightButtonUp(const NativeWindowMouseInfo& info);
			virtual void				RightButtonDoubleClick(const NativeWindowMouseInfo& info);
			virtual void				MiddleButtonDown(const NativeWindowMouseInfo& info);
			virtual void				MiddleButtonUp(const NativeWindowMouseInfo& info);
			virtual void				MiddleButtonDoubleClick(const NativeWindowMouseInfo& info);
			virtual void				HorizontalWheel(const NativeWindowMouseInfo& info);
			virtual void				VerticalWheel(const NativeWindowMouseInfo& info);
			virtual void				MouseMoving(const NativeWindowMouseInfo& info);
			virtual void				MouseEntered();
			virtual void				MouseLeaved();

			virtual void				KeyDown(int code, bool alt);
			virtual void				KeyUp(int code, bool alt);
			virtual void				SysKeyDown(int code, bool alt);
			virtual void				SysKeyUp(int code, bool alt);
			virtual void				Char(wchar_t keyChar);
		};

/***********************************************************************
Native Window Provider
***********************************************************************/

		class INativeScreen : public Interface
		{
		public:
			virtual Rect				GetBounds()=0;
			virtual Rect				GetClientBounds()=0;
			virtual WString				GetName()=0;
			virtual bool				IsPrimary()=0;
		};

		class INativeController : public Interface
		{
		public:
			virtual INativeWindow*		CreateNativeWindow()=0;
			virtual void				DestroyNativeWindow(INativeWindow* window)=0;
			virtual INativeWindow*		GetMainWindow()=0;
			virtual void				Run(INativeWindow* window)=0;

			virtual bool				InstallListener(INativeControllerListener* listener)=0;
			virtual bool				UninstallListener(INativeControllerListener* listener)=0;
			
			virtual int					GetScreenCount()=0;
			virtual INativeScreen*		GetScreen(int index)=0;
			virtual INativeScreen*		GetScreen(INativeWindow* window)=0;
		};

		class INativeControllerListener : public Interface
		{
		public:
			virtual void				LeftButtonDown(Point position);
			virtual void				LeftButtonUp(Point position);
			virtual void				RightButtonDown(Point position);
			virtual void				RightButtonUp(Point position);
			virtual void				MouseMoving(Point position);
			virtual void				GlobalTimer();
			virtual void				NativeWindowCreated(INativeWindow* window);
			virtual void				NativeWindowDestroying(INativeWindow* window);
		};

		extern							INativeController* GetCurrentController();
		extern void						SetCurrentController(INativeController* controller);
	}
}

extern void NativeMain();

#endif