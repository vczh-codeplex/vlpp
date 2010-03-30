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

#ifndef VCZH_PRESENTATION_CORE_GUINATIVEWINDOW
#define VCZH_PRESENTATION_CORE_GUINATIVEWINDOW

#include "GuiTypes.h"

namespace vl
{
	namespace presentation
	{
		class INativePen;
		class INativeBrush;
		class INativeFont;
		class INativeString;
		class INativeBitmap;
		class INativeCanvas;

		class INativeWindow;
		class INativeWindowListener;
		class INativeController;
		class INativeControllerListener;

/***********************************************************************
Native Painter
***********************************************************************/

		class INativePen : private Interface
		{
		public:
			virtual Color				GetColor()=0;
		};

		class INativeBrush : private Interface
		{
		public:
			virtual Color				GetColor()=0;
		};

		class INativeFont : private Interface
		{
		public:
			virtual WString				GetName()=0;
			virtual bool				GetBold()=0;
			virtual bool				GetItalic()=0;
			virtual bool				GetUnderline()=0;
			virtual int					GetSize()=0;
		};

		class INativeString : private Interface
		{
		public:
			virtual INativeFont*		GetFont()=0;
			virtual INativeBrush*		GetBrush()=0;
			virtual const WString&		GetString()=0;
		};

		class INativeBitmap : private Interface
		{
		public:
			virtual int					GetWidth()=0;
			virtual int					GetHeight()=0;
			virtual INativeCanvas*		Lock()=0;
			virtual void				Unlock()=0;
		};

		class INativeCanvas : private Interface
		{
		public:
			virtual void				DrawLine(Point a, Point b, INativePen* pen)=0;
			virtual void				DrawRectangle(Rect bounds, INativePen* pen, INativeBrush* brush)=0;
			virtual void				DrawEllipse(Rect bounds, INativePen* pen, INativeBrush* brush)=0;
			virtual void				DrawRoundRectangle(Rect bounds, Size ellipseSize, INativePen* pen, INativeBrush* brush)=0;
			virtual void				DrawString(const WString& string, Point position, INativeFont* font, INativeBrush* brush)=0;
			virtual void				DrawString(INativeString* string, Point position)=0;
			virtual void				DrawBitmap(INativeBitmap* bitmap, Rect dstBounds, Rect srcBounds)=0;
			virtual Size				MeasureString(const WString& string, INativeFont* font)=0;
			virtual Size				MeasureString(INativeString* string)=0;
		};

		class INativeGraphics : private Interface
		{
		public:
			virtual INativePen*			CreatePen(Color color)=0;
			virtual INativeBrush*		CreateBrush(Color color)=0;
			virtual INativeFont*		CreateFont(const WString& name, int size, bool bold, bool italic, bool underline)=0;
			virtual INativeString*		CreateString(const WString& string, INativeFont* font, INativeBrush* brush)=0;
			virtual INativeBitmap*		CreateBitmap(const WString& path)=0;
			virtual INativeBitmap*		CreateBitmap(int width, int height)=0;
			
			virtual void				Destroy(INativePen* pen)=0;
			virtual void				Destroy(INativeBrush* brush)=0;
			virtual void				Destroy(INativeFont* font)=0;
			virtual void				Destroy(INativeString* string)=0;
			virtual void				Destroy(INativeBitmap* bitmap)=0;
		};

/***********************************************************************
Native Window
***********************************************************************/

		class INativeWindow : private Interface
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

		class INativeWindowListener : private Interface
		{
		public:
			virtual void				Moving(Rect& bounds)=0;
			virtual void				Moved()=0;
			virtual void				Enabled()=0;
			virtual void				Disabled()=0;
			virtual void				GotFocus()=0;
			virtual void				LostFocus()=0;
			virtual void				Activated()=0;
			virtual void				Deactivated()=0;
			virtual void				Opened()=0;
			virtual void				Closing(bool& cancel)=0;
			virtual void				Closed()=0;
			virtual void				Paint()=0;

			virtual void				LeftButtonDown(const NativeWindowMouseInfo& info)=0;
			virtual void				LeftButtonUp(const NativeWindowMouseInfo& info)=0;
			virtual void				LeftButtonDoubleClick(const NativeWindowMouseInfo& info)=0;
			virtual void				RightButtonDown(const NativeWindowMouseInfo& info)=0;
			virtual void				RightButtonUp(const NativeWindowMouseInfo& info)=0;
			virtual void				RightButtonDoubleClick(const NativeWindowMouseInfo& info)=0;
			virtual void				MiddleButtonDown(const NativeWindowMouseInfo& info)=0;
			virtual void				MiddleButtonUp(const NativeWindowMouseInfo& info)=0;
			virtual void				MiddleButtonDoubleClick(const NativeWindowMouseInfo& info)=0;
			virtual void				HorizontalWheel(const NativeWindowMouseInfo& info)=0;
			virtual void				VerticalWheel(const NativeWindowMouseInfo& info)=0;
			virtual void				MouseMoving(const NativeWindowMouseInfo& info)=0;
			virtual void				MouseEntered()=0;
			virtual void				MouseLeaved()=0;

			virtual void				KeyDown(int code, bool alt)=0;
			virtual void				KeyUp(int code, bool alt)=0;
			virtual void				SysKeyDown(int code, bool alt)=0;
			virtual void				SysKeyUp(int code, bool alt)=0;
			virtual void				Char(wchar_t keyChar)=0;
		};

/***********************************************************************
Native Window Provider
***********************************************************************/

		class INativeController : private Interface
		{
		public:
			virtual INativeWindow*		CreateNativeWindow()=0;
			virtual void				DestroyNativeWindow(INativeWindow* window)=0;
			virtual INativeWindow*		GetMainWindow()=0;
			virtual void				SetMainWindow(INativeWindow* window)=0;

			virtual bool				InstallListener(INativeControllerListener* listener)=0;
			virtual bool				UninstallListener(INativeControllerListener* listener)=0;
		};

		class INativeControllerListener : private Interface
		{
		public:
			virtual void				LeftButtonDown(Point position)=0;
			virtual void				LeftButtonUp(Point position)=0;
			virtual void				RightButtonDown(Point position)=0;
			virtual void				RightButtonUp(Point position)=0;
			virtual void				MouseMoving(Point position)=0;
			virtual void				GlobalTimer()=0;
		};

		extern INativeController*		GetCurrentController();
		extern void						SetCurrentController(INativeController* controller);

/***********************************************************************
Native Application
***********************************************************************/

		class INativeApplication : private Interface
		{
		public:
			virtual INativeWindow*		CreateNativeWindow()=0;
			virtual void				DestroyNativeWindow(INativeWindow* window)=0;
			virtual bool				InstallListener(INativeControllerListener* listener)=0;
			virtual bool				UninstallListener(INativeControllerListener* listener)=0;
			virtual INativeGraphics*	GetGraphics()=0;
			virtual INativeCanvas*		LockWindow(INativeWindow* window)=0;
			virtual void				UnlockWindow()=0;
			virtual void				Run(INativeWindow* window)=0;
		};
	}
}

extern void NativeMain();

#endif