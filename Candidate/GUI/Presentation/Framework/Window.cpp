#include "Window.h"

namespace vl
{
	namespace presentation
	{

/***********************************************************************
Window
***********************************************************************/

		Window::Window()
		{
			nativeWindow=GetCurrentApplication()->CreateNativeWindow();
		}

		Window::~Window()
		{
			if(nativeWindow)
			{
				GetCurrentApplication()->DestroyNativeWindow(nativeWindow);
			}
		}

/***********************************************************************
WindowManager
***********************************************************************/

		void WindowManager::RegisterWindow(Window* window)
		{
			windows.Add(window);
		}

		void WindowManager::UnregisterWindow(Window* window)
		{
			windows.Remove(window);
		}

		WindowManager::WindowManager()
		{
			CHECK_ERROR(GetCurrentWindowManager()==0, L"WindowManager只能有一个实例。");
			SetCurrentWindowManager(this);
		}

		WindowManager::~WindowManager()
		{
			while(windows.Count()>0)
			{
				windows[0]->nativeWindow=0;
				delete windows[0];
			}
			SetCurrentWindowManager(0);
		}

/***********************************************************************
辅助函数
***********************************************************************/

		WindowManager* currentWindowManager=0;

		WindowManager* GetCurrentWindowManager()
		{
			return currentWindowManager;
		}

		void SetCurrentWindowManager(WindowManager* manager)
		{
			currentWindowManager=manager;
		}
	}
}