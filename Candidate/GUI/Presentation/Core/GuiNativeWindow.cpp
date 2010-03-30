#include "GuiNativeWindow.h"

namespace vl
{
	namespace presentation
	{

/***********************************************************************
INativeWindowListener
***********************************************************************/

		void INativeWindowListener::Moving(Rect& bounds)
		{
		}

		void INativeWindowListener::Moved()
		{
		}

		void INativeWindowListener::Enabled()
		{
		}

		void INativeWindowListener::Disabled()
		{
		}

		void INativeWindowListener::GotFocus()
		{
		}

		void INativeWindowListener::LostFocus()
		{
		}

		void INativeWindowListener::Activated()
		{
		}

		void INativeWindowListener::Deactivated()
		{
		}

		void INativeWindowListener::Opened()
		{
		}

		void INativeWindowListener::Closing(bool& cancel)
		{
		}

		void INativeWindowListener::Closed()
		{
		}

		void INativeWindowListener::Paint()
		{
		}

		void INativeWindowListener::LeftButtonDown(const NativeWindowMouseInfo& info)
		{
		}

		void INativeWindowListener::LeftButtonUp(const NativeWindowMouseInfo& info)
		{
		}

		void INativeWindowListener::LeftButtonDoubleClick(const NativeWindowMouseInfo& info)
		{
		}

		void INativeWindowListener::RightButtonDown(const NativeWindowMouseInfo& info)
		{
		}

		void INativeWindowListener::RightButtonUp(const NativeWindowMouseInfo& info)
		{
		}

		void INativeWindowListener::RightButtonDoubleClick(const NativeWindowMouseInfo& info)
		{
		}

		void INativeWindowListener::MiddleButtonDown(const NativeWindowMouseInfo& info)
		{
		}

		void INativeWindowListener::MiddleButtonUp(const NativeWindowMouseInfo& info)
		{
		}

		void INativeWindowListener::MiddleButtonDoubleClick(const NativeWindowMouseInfo& info)
		{
		}

		void INativeWindowListener::HorizontalWheel(const NativeWindowMouseInfo& info)
		{
		}

		void INativeWindowListener::VerticalWheel(const NativeWindowMouseInfo& info)
		{
		}

		void INativeWindowListener::MouseMoving(const NativeWindowMouseInfo& info)
		{
		}

		void INativeWindowListener::MouseEntered()
		{
		}

		void INativeWindowListener::MouseLeaved()
		{
		}

		void INativeWindowListener::KeyDown(int code, bool alt)
		{
		}

		void INativeWindowListener::KeyUp(int code, bool alt)
		{
		}

		void INativeWindowListener::SysKeyDown(int code, bool alt)
		{
		}

		void INativeWindowListener::SysKeyUp(int code, bool alt)
		{
		}

		void INativeWindowListener::Char(wchar_t keyChar)
		{
		}

/***********************************************************************
INativeControllerListener
***********************************************************************/

		void INativeControllerListener::LeftButtonDown(Point position)
		{
		}

		void INativeControllerListener::LeftButtonUp(Point position)
		{
		}

		void INativeControllerListener::RightButtonDown(Point position)
		{
		}

		void INativeControllerListener::RightButtonUp(Point position)
		{
		}

		void INativeControllerListener::MouseMoving(Point position)
		{
		}

		void INativeControllerListener::GlobalTimer()
		{
		}

/***********************************************************************
Native Window Provider
***********************************************************************/

		INativeController* currentController=0;

		INativeController* GetCurrentController()
		{
			return currentController;
		}

		void SetCurrentController(INativeController* controller)
		{
			currentController=controller;
		}
	}
}