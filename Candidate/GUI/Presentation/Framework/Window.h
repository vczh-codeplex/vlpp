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

		class Window : public Control
		{
			friend class WindowManager;
		protected:
			INativeWindow*						nativeWindow;
		public:
			Window();
			~Window();
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
		};

		extern WindowManager*					GetCurrentWindowManager();
		extern void								SetCurrentWindowManager(WindowManager* manager);
	}
}

#endif