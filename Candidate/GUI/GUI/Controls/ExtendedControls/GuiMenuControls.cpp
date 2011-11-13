#include "GuiMenuControls.h"

namespace vl
{
	namespace presentation
	{
		namespace controls
		{

/***********************************************************************
GuiWindow
***********************************************************************/

			const wchar_t* IGuiMenuService::Identifier = L"vl::presentation::controls::IGuiMenuService";

			IGuiMenuService::IGuiMenuService()
				:openingMenu(0)
			{
			}

			void IGuiMenuService::MenuItemExecuted()
			{
				if(openingMenu)
				{
					openingMenu->Hide();
				}
				if(GetParent())
				{
					GetParent()->MenuItemExecuted();
				}
			}

			GuiMenu* IGuiMenuService::GetOpeningMenu()
			{
				return openingMenu;
			}

			void IGuiMenuService::MenuOpened(GuiMenu* menu)
			{
				if(openingMenu!=menu && openingMenu)
				{
					openingMenu->Hide();
				}
				openingMenu=menu;
			}

			void IGuiMenuService::MenuClosed(GuiMenu* menu)
			{
				if(openingMenu==menu)
				{
					openingMenu=0;
				}
			}

/***********************************************************************
GuiWindow
***********************************************************************/

			IGuiMenuService* GuiMenu::GetParent()
			{
				return parentMenuService;
			}

			IGuiMenuService::Direction GuiMenu::GetPreferredDirection()
			{
				return IGuiMenuService::Vertical;
			}

			bool GuiMenu::IsActiveState()
			{
				return true;
			}

			void GuiMenu::OnWindowOpened(elements::GuiGraphicsComposition* sender, elements::GuiEventArgs& arguments)
			{
				if(parentMenuService)
				{
					parentMenuService->MenuOpened(this);
				}
			}

			void GuiMenu::OnWindowClosed(elements::GuiGraphicsComposition* sender, elements::GuiEventArgs& arguments)
			{
				if(parentMenuService)
				{
					parentMenuService->MenuClosed(this);
					GuiMenu* openingSubMenu=GetOpeningMenu();
					if(openingSubMenu)
					{
						openingSubMenu->Hide();
					}
				}
			}

			GuiMenu::GuiMenu(GuiControl::IStyleController* _styleController, GuiControl* _owner)
				:GuiPopup(_styleController)
				,owner(_owner)
				,parentMenuService(0)
			{
				UpdateMenuService();
				WindowOpened.AttachMethod(this, &GuiMenu::OnWindowOpened);
				WindowClosed.AttachMethod(this, &GuiMenu::OnWindowClosed);
			}

			GuiMenu::~GuiMenu()
			{
			}

			void GuiMenu::UpdateMenuService()
			{
				if(owner)
				{
					parentMenuService=owner->QueryService<IGuiMenuService>();
				}
			}

			Interface* GuiMenu::QueryService(const WString& identifier)
			{
				if(identifier==IGuiMenuService::Identifier)
				{
					return (IGuiMenuService*)this;
				}
				else
				{
					return GuiPopup::QueryService(identifier);
				}
			}

/***********************************************************************
GuiMenuBar
***********************************************************************/

			IGuiMenuService* GuiMenuBar::GetParent()
			{
				return 0;
			}

			IGuiMenuService::Direction GuiMenuBar::GetPreferredDirection()
			{
				return IGuiMenuService::Horizontal;
			}

			bool GuiMenuBar::IsActiveState()
			{
				return GetOpeningMenu()!=0;
			}

			GuiMenuBar::GuiMenuBar(GuiControl::IStyleController* _styleController)
				:GuiControl(_styleController)
			{
			}

			GuiMenuBar::~GuiMenuBar()
			{
			}

			Interface* GuiMenuBar::QueryService(const WString& identifier)
			{
				if(identifier==IGuiMenuService::Identifier)
				{
					return (IGuiMenuService*)this;
				}
				else
				{
					return GuiControl::QueryService(identifier);
				}
			}

/***********************************************************************
GuiMenuButton
***********************************************************************/

			void GuiMenuButton::OpenSubMenuInternal()
			{
				if(!GetSubMenuOpening())
				{
					if(ownerMenuService)
					{
						GuiMenu* openingSiblingMenu=ownerMenuService->GetOpeningMenu();
						if(openingSiblingMenu)
						{
							openingSiblingMenu->Hide();
						}
					}
					SetSubMenuOpening(true);
				}
			}

			void GuiMenuButton::OnParentLineChanged()
			{
				GuiButton::OnParentLineChanged();
				ownerMenuService=QueryService<IGuiMenuService>();
				if(subMenu)
				{
					subMenu->UpdateMenuService();
				}
			}

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

			void GuiMenuButton::OnMouseEnter(elements::GuiGraphicsComposition* sender, elements::GuiEventArgs& arguments)
			{
				if(GetVisuallyEnabled())
				{
					if(ownerMenuService && ownerMenuService->IsActiveState())
					{
						OpenSubMenuInternal();
					}
				}
			}

			void GuiMenuButton::OnClicked(elements::GuiGraphicsComposition* sender, elements::GuiEventArgs& arguments)
			{
				if(GetSubMenu())
				{
					OpenSubMenuInternal();
				}
				else if(ownerMenuService)
				{
					ownerMenuService->MenuItemExecuted();
				}
			}

			GuiMenuButton::GuiMenuButton(IStyleController* _styleController)
				:GuiButton(_styleController)
				,styleController(_styleController)
				,subMenu(0)
				,ownerMenuService(0)
			{
				SubMenuOpeningChanged.SetAssociatedComposition(boundsComposition);
				Clicked.AttachMethod(this, &GuiMenuButton::OnClicked);
				GetEventReceiver()->mouseEnter.AttachMethod(this, &GuiMenuButton::OnMouseEnter);
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
					subMenu=new GuiMenu(subMenuStyleController?subMenuStyleController:styleController->CreateSubMenuStyleController(), this);
					subMenu->WindowOpened.AttachMethod(this, &GuiMenuButton::OnSubMenuWindowOpened);
					subMenu->WindowClosed.AttachMethod(this, &GuiMenuButton::OnSubMenuWindowClosed);
					styleController->SetSubMenuExisting(true);
				}
			}

			void GuiMenuButton::DestroySubMenu()
			{
				if(subMenu)
				{
					delete subMenu;
					subMenu=0;
					styleController->SetSubMenuExisting(false);
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
						IGuiMenuService::Direction direction=ownerMenuService?ownerMenuService->GetPreferredDirection():IGuiMenuService::Horizontal;
						subMenu->ShowPopup(this, direction==IGuiMenuService::Horizontal);
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