/***********************************************************************
Vczh Library++ 3.0
Developer: ³Âè÷å«(vczh)
GUI::Framework

Classes:
  Window								£º´°¿Ú»ùÀà
***********************************************************************/

#ifndef VCZH_PRESENTATION_FRAMEWORK_WINDOW
#define VCZH_PRESENTATION_FRAMEWORK_WINDOW

#include "..\Core\GuiNativeWindow.h"
#include "Control.h"

namespace vl
{
	namespace presentation
	{
		class Window;
		class WindowManager;

/***********************************************************************
Window
***********************************************************************/

		class Window : public Control, private INativeWindowListener
		{
			friend class WindowManager;
			typedef Control						ParentClass;
		protected:
			INativeWindow*						nativeWindow;
			bool								updating;

			void								UpdateHost();
		private:
			void								Moving(Rect& bounds);
			void								Moved();
			void								Enabled();
			void								Disabled();
			void								GotFocus();
			void								LostFocus();
			void								Activated();
			void								Deactivated();
			void								Opened();
			void								Closing(bool& cancel);
			void								Closed();
			void								Destroying();
		public:
			Window();
			~Window();

			void								SetText(const WString& value);

			Size								GetExpectedClientSize();
			void								SetMargin(Margin margin);
			bool								IsVisuallyVisible();
			void								SetVisible(bool value);
			void								SetEnabled(bool value);
			void								SetFocused(bool value);

			void								Show();
			void								Close();
		};

/***********************************************************************
WindowManager
***********************************************************************/

		class WindowManager : public Object
		{
			friend class Window;
		protected:
			List<Window*>						windows;

			void								RegisterWindow(Window* window);
			void								UnregisterWindow(Window* window);
		public:
			WindowManager();
			~WindowManager();

			void								Run(Window* window);
		};

		extern WindowManager*					GetCurrentWindowManager();
		extern void								SetCurrentWindowManager(WindowManager* manager);
	}
}

#endif