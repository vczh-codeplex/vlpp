#include "Window.h"

namespace vl
{
	namespace presentation
	{

/***********************************************************************
Window
***********************************************************************/

		void LimitMovingBound(int olda, int oldb, int& newa, int& newb, int min, int max)
		{
			int newl=newb-newa;
			int limited=newl;
			if(newl<min)
			{
				limited=min;
			}
			else if(newl>max)
			{
				limited=max;
			}
			if(newl!=limited)
			{
				if(olda!=newa)
				{
					newa=newb-limited;
				}
				else
				{
					newb=newa+limited;
				}
			}
		}

		void Window::Moving(Rect& bounds)
		{
			Rect oldBounds=nativeWindow->GetBounds();
			Size min=GetMinSize();
			Size max=GetMaxSize();
			LimitMovingBound(oldBounds.x1, oldBounds.x2, bounds.x1, bounds.x2, min.x, max.x);
			LimitMovingBound(oldBounds.y1, oldBounds.y2, bounds.y1, bounds.y2, min.y, max.y);
		}

		void Window::Moved()
		{
			if(!updating)
			{
				updating=true;
				ParentClass::SetExpectedLocation(nativeWindow->GetBounds().LeftTop());
				ParentClass::SetExpectedSize(nativeWindow->GetBounds().GetSize());
				updating=false;
			}
		}

		void Window::Enabled()
		{
			ParentClass::SetEnabled(true);
		}

		void Window::Disabled()
		{
			ParentClass::SetEnabled(false);
		}

		void Window::GotFocus()
		{
			ParentClass::SetFocused(true);
		}

		void Window::LostFocus()
		{
			ParentClass::SetFocused(false);
		}

		void Window::Activated()
		{
			ParentClass::SetFocused(nativeWindow->IsFocused());
		}

		void Window::Deactivated()
		{
			ParentClass::SetFocused(nativeWindow->IsFocused());
		}

		void Window::Opened()
		{
			ParentClass::SetVisible(true);
		}

		void Window::Closing(bool& cancel)
		{
		}

		void Window::Closed()
		{
			ParentClass::SetVisible(false);
		}

		void Window::Destroying()
		{
			nativeWindow=0;
			delete this;
		}

		void Window::UpdateHost()
		{
			if(!updating)
			{
				updating=true;
				nativeWindow->SetBounds(Rect(GetLocation(), GetSize()));
				updating=false;
			}
		}

		Window::Window()
			:updating(false)
		{
			nativeWindow=GetCurrentApplication()->CreateNativeWindow();
			ParentClass::SetText(nativeWindow->GetTitle());
			ParentClass::SetExpectedLocation(nativeWindow->GetBounds().LeftTop());
			ParentClass::SetExpectedSize(nativeWindow->GetBounds().GetSize());
			ParentClass::SetVisible(nativeWindow->IsVisible());
			ParentClass::SetEnabled(nativeWindow->IsEnabled());
			ParentClass::SetFocused(nativeWindow->IsFocused());
		}

		Window::~Window()
		{
			if(nativeWindow)
			{
				GetCurrentApplication()->DestroyNativeWindow(nativeWindow);
			}
		}

		void Window::SetText(const WString& value)
		{
			nativeWindow->SetTitle(value);
			ParentClass::SetText(nativeWindow->GetTitle());
		}

		Size Window::GetExpectedClientSize()
		{
			return GetExpectedSize()-(nativeWindow->GetBounds().GetSize()-nativeWindow->GetClientSize());
		}

		void Window::SetMargin(Margin margin)
		{
		}

		bool Window::IsVisuallyVisible()
		{
			return nativeWindow->IsVisible();
		}

		void Window::SetVisible(bool value)
		{
			if(value)
			{
				nativeWindow->Show();
			}
			else
			{
				nativeWindow->Hide();
			}
		}

		void Window::SetEnabled(bool value)
		{
			if(value)
			{
				nativeWindow->Enable();
			}
			else
			{
				nativeWindow->Disable();
			}
		}

		void Window::SetFocused(bool value)
		{
			if(value)
			{
				nativeWindow->SetFocus();
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