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

			GuiMenu::GuiMenu(GuiControl::IStyleController* _styleController)
				:GuiPopup(_styleController)
			{
			}

			GuiMenu::~GuiMenu()
			{
			}

/***********************************************************************
GuiMenuBar
***********************************************************************/

			GuiMenuBar::GuiMenuBar(GuiControl::IStyleController* _styleController)
				:GuiControl(_styleController)
			{
			}

			GuiMenuBar::~GuiMenuBar()
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