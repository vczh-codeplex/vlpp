#include "GuiApplication.h"

extern void GuiMain();

namespace vl
{
	namespace presentation
	{
		namespace controls
		{
			using namespace collections;

/***********************************************************************
GuiApplication
***********************************************************************/

			void GuiApplication::LeftButtonDown(Point position)
			{
				OnMouseDown(position);
			}

			void GuiApplication::LeftButtonUp(Point position)
			{
			}

			void GuiApplication::RightButtonDown(Point position)
			{
				OnMouseDown(position);
			}

			void GuiApplication::RightButtonUp(Point position)
			{
			}

			GuiApplication::GuiApplication()
				:mainWindow(0)
			{
				GetCurrentController()->InstallListener(this);
			}

			GuiApplication::~GuiApplication()
			{
				GetCurrentController()->UninstallListener(this);
			}

			void GuiApplication::RegisterWindow(GuiWindow* window)
			{
				windows.Add(window);
			}

			void GuiApplication::UnregisterWindow(GuiWindow* window)
			{
				windows.Remove(window);
			}

			void GuiApplication::OnMouseDown(Point location)
			{
				GuiWindow* window=GetWindow(location);
				for(int i=0;i<windows.Count();i++)
				{
					if(windows[i]!=window)
					{
						windows[i]->MouseClickedOnOtherWindow(window);
					}
				}
			}

			void GuiApplication::Run(GuiWindow* _mainWindow)
			{
				if(!mainWindow)
				{
					mainWindow=_mainWindow;
					GetCurrentController()->Run(mainWindow->GetNativeWindow());
					mainWindow=0;
				}
			}

			const collections::IReadonlyList<GuiWindow*>& GuiApplication::GetWindows()
			{
				return windows.Wrap();
			}

			GuiWindow* GuiApplication::GetWindow(Point location)
			{
				INativeWindow* nativeWindow=GetCurrentController()->GetWindow(location);
				if(nativeWindow)
				{
					for(int i=0;i<windows.Count();i++)
					{
						GuiWindow* window=windows[i];
						if(window->GetNativeWindow()==nativeWindow)
						{
							return window;
						}
					}
				}
				return 0;
			}

			bool GuiApplication::IsInMainThread()
			{
				return GetCurrentController()->IsInMainThread();
			}

			void GuiApplication::InvokeInMainThread(INativeController::AsyncTaskProc* proc, void* argument)
			{
				GetCurrentController()->InvokeInMainThread(proc, argument);
			}

			bool GuiApplication::InvokeInMainThreadAndWait(INativeController::AsyncTaskProc* proc, void* argument, int milliseconds)
			{
				return GetCurrentController()->InvokeInMainThreadAndWait(proc, argument, milliseconds);
			}

			void InvokeInMainThreadProc(void* argument)
			{
				Func<void()>* proc=(Func<void()>*)argument;
				(*proc)();
				delete proc;
			}

			void GuiApplication::InvokeInMainThread(const Func<void()>& proc)
			{
				InvokeInMainThread(&InvokeInMainThreadProc, new Func<void()>(proc));
			}

			bool GuiApplication::InvokeInMainThreadAndWait(const Func<void()>& proc, int milliseconds)
			{
				return InvokeInMainThreadAndWait(&InvokeInMainThreadProc, new Func<void()>(proc));
			}

/***********************************************************************
Helpers
***********************************************************************/

			GuiApplication* application=0;

			GuiApplication* GetApplication()
			{
				return application;
			}

			void GuiApplicationInitialize()
			{
				GetCurrentController()->StartTimer();
				GetCurrentController()->StartHookMouse();
				GuiApplication app;
				application=&app;
				GuiMain();
			}
		}
	}
}

void GuiApplicationMain()
{
	vl::presentation::controls::GuiApplicationInitialize();
}