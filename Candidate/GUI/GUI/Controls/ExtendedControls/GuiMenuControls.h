/***********************************************************************
Vczh Library++ 3.0
Developer: ³Âè÷å«(vczh)
GacUI::Control System

Interfaces:
***********************************************************************/

#ifndef VCZH_PRESENTATION_CONTROLS_GUIMENUCONTROLS
#define VCZH_PRESENTATION_CONTROLS_GUIMENUCONTROLS

#include "..\GuiWindowControls.h"

namespace vl
{
	namespace presentation
	{
		namespace controls
		{

/***********************************************************************
Menu
***********************************************************************/

			class GuiMenu : public GuiPopup
			{
			public:
				GuiMenu(GuiControl::IStyleController* _styleController);
				~GuiMenu();
			};

			class GuiMenuBar : public GuiControl
			{
			public:
				GuiMenuBar(GuiControl::IStyleController* _styleController);
				~GuiMenuBar();
			};

/***********************************************************************
MenuButton
***********************************************************************/

			class GuiMenuButton : public GuiButton
			{
			public:
				class IStyleController : public GuiButton::IStyleController
				{
				public:
					virtual GuiMenu::IStyleController*	CreateSubMenuStyleController()=0;
					virtual void						SetSubMenuOpening(bool value)=0;
				};
			protected:
				IStyleController*						styleController;
				GuiMenu*								subMenu;
				Size									preferredMenuClientSize;

				void									OnSubMenuWindowOpened(elements::GuiGraphicsComposition* sender, elements::GuiEventArgs& arguments);
				void									OnSubMenuWindowClosed(elements::GuiGraphicsComposition* sender, elements::GuiEventArgs& arguments);
				void									OnClicked(elements::GuiGraphicsComposition* sender, elements::GuiEventArgs& arguments);
			public:
				GuiMenuButton(IStyleController* _styleController);
				~GuiMenuButton();

				elements::GuiNotifyEvent				SubMenuOpeningChanged;

				bool									IsSubMenuExists();
				GuiMenu*								GetSubMenu();
				void									CreateSubMenu(GuiMenu::IStyleController* subMenuStyleController=0);
				void									DestroySubMenu();

				bool									GetSubMenuOpening();
				void									SetSubMenuOpening(bool value);

				Size									GetPreferredMenuClientSize();
				void									SetPreferredMenuClientSize(Size value);
			};
		}
	}
}

#endif