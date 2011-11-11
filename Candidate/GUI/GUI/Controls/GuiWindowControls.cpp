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

			bool GuiControlHost::GetOpening()
			{
				INativeWindow* window=host->GetNativeWindow();
				if(window)
				{
					return window->IsVisible();
				}
				return false;
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

/***********************************************************************
GuiPopup
***********************************************************************/

			GuiPopup::GuiPopup(GuiControl::IStyleController* _styleController)
				:GuiWindow(_styleController)
			{
				SetMinimizedBox(false);
				SetMaximizedBox(false);
				SetSizeBox(false);
				GetBoundsComposition()->SetMinSizeLimitation(GuiGraphicsComposition::LimitToElementAndChildren);
			}

			GuiPopup::~GuiPopup()
			{
			}

			bool GuiPopup::IsClippedByScreen(Point location)
			{
				SetBounds(Rect(location, GetBounds().GetSize()));
				INativeWindow* window=GetNativeWindow();
				if(window)
				{
					INativeScreen* screen=GetCurrentController()->GetScreen(window);
					if(screen)
					{
						Rect screenBounds=screen->GetClientBounds();
						Rect windowBounds=window->GetBounds();
						return !screenBounds.Contains(windowBounds.LeftTop()) || !screenBounds.Contains(windowBounds.RightBottom());
					}
				}
				return true;
			}

			void GuiPopup::ShowPopup(Point location)
			{
				INativeWindow* window=GetNativeWindow();
				if(window)
				{
					INativeScreen* screen=GetCurrentController()->GetScreen(window);
					if(screen)
					{
						Rect screenBounds=screen->GetClientBounds();
						Size size=window->GetBounds().GetSize();

						if(location.x<screenBounds.x1)
						{
							location.x=screenBounds.x1;
						}
						else if(location.x+size.x>screenBounds.x2)
						{
							location.x=screenBounds.x2-size.x;
						}

						if(location.y<screenBounds.y1)
						{
							location.y=screenBounds.y1;
						}
						else if(location.y+size.y>screenBounds.y2)
						{
							location.y=screenBounds.y2-size.y;
						}
					}
					SetBounds(Rect(location, GetBounds().GetSize()));
					Show();
				}
			}

			void GuiPopup::ShowPopup(GuiControl* control, bool preferredTopBottomSide)
			{
				INativeWindow* window=GetNativeWindow();
				if(window)
				{
					Point locations[4];
					Size size=window->GetBounds().GetSize();
					Rect controlBounds=control->GetBoundsComposition()->GetGlobalBounds();

					GuiControlHost* controlHost=control->GetBoundsComposition()->GetRelatedControlHost();
					if(controlHost)
					{
						INativeWindow* controlWindow=controlHost->GetNativeWindow();
						if(controlWindow)
						{
							Point controlClientOffset=controlWindow->GetClientBoundsInScreen().LeftTop();
							controlBounds.x1+=controlClientOffset.x;
							controlBounds.x2+=controlClientOffset.x;
							controlBounds.y1+=controlClientOffset.y;
							controlBounds.y2+=controlClientOffset.y;

							if(preferredTopBottomSide)
							{
								locations[0]=Point(controlBounds.x1, controlBounds.y2);
								locations[1]=Point(controlBounds.x2-size.x, controlBounds.y2);
								locations[2]=Point(controlBounds.x1, controlBounds.y1-size.y);
								locations[3]=Point(controlBounds.x2-size.x, controlBounds.y1-size.y);
							}
							else
							{
								locations[0]=Point(controlBounds.x1-size.x, controlBounds.y1);
								locations[1]=Point(controlBounds.x1-size.x, controlBounds.y2-size.y);
								locations[0]=Point(controlBounds.x2, controlBounds.y1);
								locations[1]=Point(controlBounds.x2, controlBounds.y2-size.y);
							}

							for(int i=0;i<4;i++)
							{
								if(!IsClippedByScreen(locations[i]))
								{
									ShowPopup(locations[i]);
									return;
								}
							}
							ShowPopup(locations[0]);
						}
					}
				}
			}

/***********************************************************************
GuiWindow
***********************************************************************/

			GuiMenu::GuiMenu(GuiControl::IStyleController* _styleController)
				:GuiPopup(_styleController)
			{
			}

			GuiMenu::~GuiMenu()
			{
			}

/***********************************************************************
GuiMenuButton
***********************************************************************/

			void GuiMenuButton::OnSubMenuWindowOpened(elements::GuiGraphicsComposition* sender, elements::GuiEventArgs& arguments)
			{
				SubMenuOpeningChanged.Execute(GetNotifyEventArguments());
				styleController->SetSubMenuOpening(true);
			}

			void GuiMenuButton::OnSubMenuWindowClosed(elements::GuiGraphicsComposition* sender, elements::GuiEventArgs& arguments)
			{
				SubMenuOpeningChanged.Execute(GetNotifyEventArguments());
				styleController->SetSubMenuOpening(false);
			}

			void GuiMenuButton::OnClicked(elements::GuiGraphicsComposition* sender, elements::GuiEventArgs& arguments)
			{
				if(!GetSubMenuOpening())
				{
					SetSubMenuOpening(true);
				}
			}

			GuiMenuButton::GuiMenuButton(IStyleController* _styleController)
				:GuiButton(_styleController)
				,styleController(_styleController)
				,subMenu(0)
			{
				SubMenuOpeningChanged.SetAssociatedComposition(boundsComposition);
				Clicked.AttachMethod(this, &GuiMenuButton::OnClicked);
			}

			GuiMenuButton::~GuiMenuButton()
			{
				if(subMenu)
				{
					delete subMenu;
				}
			}

			bool GuiMenuButton::IsSubMenuExists()
			{
				return subMenu!=0;
			}

			GuiMenu* GuiMenuButton::GetSubMenu()
			{
				return subMenu;
			}

			void GuiMenuButton::CreateSubMenu(GuiMenu::IStyleController* subMenuStyleController)
			{
				if(!subMenu)
				{
					subMenu=new GuiMenu(subMenuStyleController?subMenuStyleController:styleController->CreateSubMenuStyleController());
					subMenu->WindowOpened.AttachMethod(this, &GuiMenuButton::OnSubMenuWindowOpened);
					subMenu->WindowClosed.AttachMethod(this, &GuiMenuButton::OnSubMenuWindowClosed);
				}
			}

			void GuiMenuButton::DestroySubMenu()
			{
				if(subMenu)
				{
					delete subMenu;
					subMenu=0;
				}
			}

			bool GuiMenuButton::GetSubMenuOpening()
			{
				if(subMenu)
				{
					return subMenu->GetOpening();
				}
				else
				{
					return false;
				}
			}

			void GuiMenuButton::SetSubMenuOpening(bool value)
			{
				if(subMenu)
				{
					if(value)
					{
						subMenu->SetClientSize(preferredMenuClientSize);
						subMenu->ShowPopup(this, true);
					}
					else
					{
						subMenu->Close();
					}
				}
			}

			Size GuiMenuButton::GetPreferredMenuClientSize()
			{
				return preferredMenuClientSize;
			}

			void GuiMenuButton::SetPreferredMenuClientSize(Size value)
			{
				preferredMenuClientSize=value;
			}
		}
	}
}