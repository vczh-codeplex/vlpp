#include "GuiWindowControls.h"

namespace vl
{
	namespace presentation
	{
		namespace controls
		{
			using namespace elements;

/***********************************************************************
GuiControlHost
***********************************************************************/

			void GuiControlHost::Enabled()
			{
				GuiControl::SetEnabled(true);
			}

			void GuiControlHost::Disabled()
			{
				GuiControl::SetEnabled(false);
			}

			void GuiControlHost::GotFocus()
			{
				WindowGotFocus.Execute(GetNotifyEventArguments());
			}

			void GuiControlHost::LostFocus()
			{
				WindowLostFocus.Execute(GetNotifyEventArguments());
			}

			void GuiControlHost::Activated()
			{
				WindowActivated.Execute(GetNotifyEventArguments());
			}

			void GuiControlHost::Deactivated()
			{
				WindowDeactivated.Execute(GetNotifyEventArguments());
			}

			void GuiControlHost::Opened()
			{
				WindowOpened.Execute(GetNotifyEventArguments());
			}

			void GuiControlHost::Closing(bool& cancel)
			{
				GuiRequestEventArgs arguments;
				arguments.compositionSource=GetStyleController()->GetBoundsComposition();
				arguments.eventSource=GetStyleController()->GetBoundsComposition();
				arguments.cancel=cancel;
				WindowClosing.Execute(arguments);
				if(!arguments.handled)
				{
					cancel=arguments.cancel;
				}
			}

			void GuiControlHost::Closed()
			{
				WindowClosed.Execute(GetNotifyEventArguments());
			}

			void GuiControlHost::Destroying()
			{
				WindowDestroying.Execute(GetNotifyEventArguments());
				SetNativeWindow(0);
			}

			GuiControlHost::GuiControlHost(GuiControl::IStyleController* _styleController)
				:GuiControl(_styleController)
			{
				GetStyleController()->GetBoundsComposition()->SetAlignmentToParent(Margin(0, 0, 0, 0));
				
				WindowGotFocus.SetAssociatedComposition(GetStyleController()->GetBoundsComposition());
				WindowLostFocus.SetAssociatedComposition(GetStyleController()->GetBoundsComposition());
				WindowActivated.SetAssociatedComposition(GetStyleController()->GetBoundsComposition());
				WindowDeactivated.SetAssociatedComposition(GetStyleController()->GetBoundsComposition());
				WindowOpened.SetAssociatedComposition(GetStyleController()->GetBoundsComposition());
				WindowClosing.SetAssociatedComposition(GetStyleController()->GetBoundsComposition());
				WindowClosed.SetAssociatedComposition(GetStyleController()->GetBoundsComposition());
				WindowDestroying.SetAssociatedComposition(GetStyleController()->GetBoundsComposition());

				host=new GuiGraphicsHost;
				host->GetMainComposition()->AddChild(GetStyleController()->GetBoundsComposition());
			}

			GuiControlHost::~GuiControlHost()
			{
				styleController=0;
				for(int i=0;i<components.Count();i++)
				{
					delete components[i];
				}
				delete host;
			}

			elements::GuiGraphicsHost* GuiControlHost::GetGraphicsHost()
			{
				return host;
			}

			elements::GuiGraphicsComposition* GuiControlHost::GetMainComposition()
			{
				return host->GetMainComposition();
			}

			INativeWindow* GuiControlHost::GetNativeWindow()
			{
				return host->GetNativeWindow();
			}

			void GuiControlHost::SetNativeWindow(INativeWindow* window)
			{
				if(host->GetNativeWindow())
				{
					host->GetNativeWindow()->UninstallListener(this);
				}
				host->SetNativeWindow(window);
				if(host->GetNativeWindow())
				{
					host->GetNativeWindow()->InstallListener(this);
				}
			}

			void GuiControlHost::Render()
			{
				host->Render();
			}

			bool GuiControlHost::GetEnabled()
			{
				if(host->GetNativeWindow())
				{
					return host->GetNativeWindow()->IsEnabled();
				}
				else
				{
					return false;
				}
			}

			void GuiControlHost::SetEnabled(bool value)
			{
				if(host->GetNativeWindow())
				{
					if(value)
					{
						host->GetNativeWindow()->Enable();
					}
					else
					{
						host->GetNativeWindow()->Disable();
					}
				}
			}

			bool GuiControlHost::GetFocused()
			{
				if(host->GetNativeWindow())
				{
					return host->GetNativeWindow()->IsFocused();
				}
				else
				{
					return false;
				}
			}

			void GuiControlHost::SetFocused()
			{
				if(host->GetNativeWindow())
				{
					host->GetNativeWindow()->SetFocus();
				}
			}

			bool GuiControlHost::GetActivated()
			{
				if(host->GetNativeWindow())
				{
					return host->GetNativeWindow()->IsActivated();
				}
				else
				{
					return false;
				}
			}

			void GuiControlHost::SetActivated()
			{
				if(host->GetNativeWindow())
				{
					host->GetNativeWindow()->SetActivate();
				}
			}

			bool GuiControlHost::GetShowInTaskBar()
			{
				if(host->GetNativeWindow())
				{
					return host->GetNativeWindow()->IsAppearedInTaskBar();
				}
				else
				{
					return false;
				}
			}

			void GuiControlHost::SetShowInTaskBar(bool value)
			{
				if(host->GetNativeWindow())
				{
					if(value)
					{
						host->GetNativeWindow()->ShowInTaskBar();
					}
					else
					{
						host->GetNativeWindow()->HideInTaskBar();
					}
				}
			}

			bool GuiControlHost::GetMaximizedBox()
			{
				if(host->GetNativeWindow())
				{
					return host->GetNativeWindow()->GetMaximizedBox();
				}
				else
				{
					return false;
				}
			}

			void GuiControlHost::SetMaximizedBox(bool visible)
			{
				if(host->GetNativeWindow())
				{
					host->GetNativeWindow()->SetMaximizedBox(visible);
				}
			}

			bool GuiControlHost::GetMinimizedBox()
			{
				if(host->GetNativeWindow())
				{
					return host->GetNativeWindow()->GetMinimizedBox();
				}
				else
				{
					return false;
				}
			}

			void GuiControlHost::SetMinimizedBox(bool visible)
			{
				if(host->GetNativeWindow())
				{
					host->GetNativeWindow()->SetMinimizedBox(visible);
				}
			}

			bool GuiControlHost::GetBorder()
			{
				if(host->GetNativeWindow())
				{
					return host->GetNativeWindow()->GetBorder();
				}
				else
				{
					return false;
				}
			}

			void GuiControlHost::SetBorder(bool visible)
			{
				if(host->GetNativeWindow())
				{
					host->GetNativeWindow()->SetBorder(visible);
				}
			}

			bool GuiControlHost::GetSizeBox()
			{
				if(host->GetNativeWindow())
				{
					return host->GetNativeWindow()->GetSizeBox();
				}
				else
				{
					return false;
				}
			}

			void GuiControlHost::SetSizeBox(bool visible)
			{
				if(host->GetNativeWindow())
				{
					host->GetNativeWindow()->SetSizeBox(visible);
				}
			}

			bool GuiControlHost::GetIconVisible()
			{
				if(host->GetNativeWindow())
				{
					return host->GetNativeWindow()->GetIconVisible();
				}
				else
				{
					return false;
				}
			}

			void GuiControlHost::SetIconVisible(bool visible)
			{
				if(host->GetNativeWindow())
				{
					host->GetNativeWindow()->SetIconVisible(visible);
				}
			}

			bool GuiControlHost::GetTitleBar()
			{
				if(host->GetNativeWindow())
				{
					return host->GetNativeWindow()->GetTitleBar();
				}
				else
				{
					return false;
				}
			}

			void GuiControlHost::SetTitleBar(bool visible)
			{
				if(host->GetNativeWindow())
				{
					host->GetNativeWindow()->SetTitleBar(visible);
				}
			}

			bool GuiControlHost::AddComponent(GuiComponent* component)
			{
				if(components.Contains(component))
				{
					return false;
				}
				else
				{
					components.Add(component);
					return true;
				}
			}

			bool GuiControlHost::RemoveComponent(GuiComponent* component)
			{
				return components.Remove(component);
			}

			bool GuiControlHost::ContainsComponent(GuiComponent* component)
			{
				return components.Contains(component);
			}

			Size GuiControlHost::GetClientSize()
			{
				if(host->GetNativeWindow())
				{
					return host->GetNativeWindow()->GetClientSize();
				}
				else
				{
					return Size(0, 0);
				}
			}

			void GuiControlHost::SetClientSize(Size value)
			{
				if(host->GetNativeWindow())
				{
					host->GetNativeWindow()->SetClientSize(value);
				}
			}

			Rect GuiControlHost::GetBounds()
			{
				if(host->GetNativeWindow())
				{
					return host->GetNativeWindow()->GetBounds();
				}
				else
				{
					return Rect();
				}
			}

			void GuiControlHost::SetBounds(Rect value)
			{
				if(host->GetNativeWindow())
				{
					host->GetNativeWindow()->SetBounds(value);
				}
			}

			const WString& GuiControlHost::GetText()
			{
				WString result;
				if(host->GetNativeWindow())
				{
					result=host->GetNativeWindow()->GetTitle();
				}
				if(result!=GuiControl::GetText())
				{
					GuiControl::SetText(result);
				}
				return GuiControl::GetText();
			}

			void GuiControlHost::SetText(const WString& value)
			{
				if(host->GetNativeWindow())
				{
					host->GetNativeWindow()->SetTitle(value);
					GuiControl::SetText(value);
				}
			}

			INativeScreen* GuiControlHost::GetRelatedScreen()
			{
				if(host->GetNativeWindow())
				{
					return GetCurrentController()->GetScreen(host->GetNativeWindow());
				}
				else
				{
					return 0;
				}
			}

			void GuiControlHost::Show()
			{
				if(host->GetNativeWindow())
				{
					host->GetNativeWindow()->Show();
				}
			}

			void GuiControlHost::ShowRestored()
			{
				if(host->GetNativeWindow())
				{
					host->GetNativeWindow()->ShowRestored();
				}
			}

			void GuiControlHost::ShowMaximized()
			{
				if(host->GetNativeWindow())
				{
					host->GetNativeWindow()->ShowMaximized();
				}
			}

			void GuiControlHost::ShowMinimized()
			{
				if(host->GetNativeWindow())
				{
					host->GetNativeWindow()->ShowMinimized();
				}
			}

			void GuiControlHost::Hide()
			{
				if(host->GetNativeWindow())
				{
					host->GetNativeWindow()->Hide();
				}
			}

			void GuiControlHost::Close()
			{
				INativeWindow* window=host->GetNativeWindow();
				if(window)
				{
					SetNativeWindow(0);
					if(GetCurrentController()->GetMainWindow()==window)
					{
						window->Hide();
					}
					else
					{
						GetCurrentController()->DestroyNativeWindow(window);
					}
				}
			}

/***********************************************************************
GuiWindow
***********************************************************************/

			GuiWindow::GuiWindow(GuiControl::IStyleController* _styleController)
				:GuiControlHost(_styleController)
			{
				INativeWindow* window=GetCurrentController()->CreateNativeWindow();
				SetNativeWindow(window);
			}

			GuiWindow::~GuiWindow()
			{
				INativeWindow* window=host->GetNativeWindow();
				if(window)
				{
					SetNativeWindow(0);
					GetCurrentController()->DestroyNativeWindow(window);
				}
			}
		}
	}
}