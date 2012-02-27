/***********************************************************************
Vczh Library++ 3.0
Developer: ³Âè÷å«(vczh)
GacUI::Application Framework

Interfaces:
***********************************************************************/

#ifndef VCZH_PRESENTATION_CONTROLS_GUIAPPLICATION
#define VCZH_PRESENTATION_CONTROLS_GUIAPPLICATION

#include "GuiWindowControls.h"

namespace vl
{
	namespace presentation
	{
		namespace controls
		{
			class GuiApplication : public Object, private INativeControllerListener
			{
				friend void GuiApplicationInitialize();
				friend class GuiWindow;
				friend class Ptr<GuiApplication>;
			private:
				void											LeftButtonDown(Point position)override;
				void											LeftButtonUp(Point position)override;
				void											RightButtonDown(Point position)override;
				void											RightButtonUp(Point position)override;
			protected:
				GuiWindow*										mainWindow;
				collections::List<GuiWindow*>					windows;

				GuiApplication();
				~GuiApplication();

				void											RegisterWindow(GuiWindow* window);
				void											UnregisterWindow(GuiWindow* window);
				void											OnMouseDown(Point location);
			public:
				void											Run(GuiWindow* _mainWindow);
				const collections::IReadonlyList<GuiWindow*>&	GetWindows();
				GuiWindow*										GetWindow(Point location);

				bool											IsInMainThread();
				void											InvokeInMainThread(INativeAsyncService::AsyncTaskProc* proc, void* argument);
				bool											InvokeInMainThreadAndWait(INativeAsyncService::AsyncTaskProc* proc, void* argument, int milliseconds=-1);
				void											InvokeInMainThread(const Func<void()>& proc);
				bool											InvokeInMainThreadAndWait(const Func<void()>& proc, int milliseconds=-1);

				template<typename T>
				void InvokeLambdaInMainThread(const T& proc)
				{
					InvokeInMainThread(Func<void()>(proc));
				}
				
				template<typename T>
				bool InvokeLambdaInMainThreadAndWait(const T& proc, int milliseconds=-1)
				{
					return InvokeInMainThreadAndWait(Func<void()>(proc));
				}
			};

			extern GuiApplication*								GetApplication();
		}
	}
}

extern void GuiApplicationMain();

#endif