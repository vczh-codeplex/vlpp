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
Menu Service
***********************************************************************/

			class GuiMenu;

			class IGuiMenuService : public Interface
			{
			public:
				static const wchar_t*					Identifier;

				enum Direction
				{
					Horizontal,
					Vertical,
				};
			protected:
				GuiMenu*								openingMenu;
			public:
				IGuiMenuService();

				virtual IGuiMenuService*				GetParent()=0;
				virtual Direction						GetPreferredDirection()=0;
				virtual bool							IsActiveState()=0;

				virtual void							MenuItemExecuted();
				virtual GuiMenu*						GetOpeningMenu();
				virtual void							MenuOpened(GuiMenu* menu);
				virtual void							MenuClosed(GuiMenu* menu);
			};

/***********************************************************************
Menu
***********************************************************************/

			class GuiMenu : public GuiPopup, private IGuiMenuService
			{
			private:
				IGuiMenuService*						parentMenuService;

				IGuiMenuService*						GetParent();
				Direction								GetPreferredDirection();
				bool									IsActiveState();
			protected:
				GuiControl*								owner;

				void									OnWindowOpened(elements::GuiGraphicsComposition* sender, elements::GuiEventArgs& arguments);
				void									OnWindowClosed(elements::GuiGraphicsComposition* sender, elements::GuiEventArgs& arguments);
			public:
				GuiMenu(GuiControl::IStyleController* _styleController, GuiControl* _owner);
				~GuiMenu();

				void									UpdateMenuService();
				vl::Interface*							QueryService(const WString& identifier)override;
			};

			class GuiMenuBar : public GuiControl, private IGuiMenuService
			{
			private:
				IGuiMenuService*						GetParent();
				Direction								GetPreferredDirection();
				bool									IsActiveState();
			public:
				GuiMenuBar(GuiControl::IStyleController* _styleController);
				~GuiMenuBar();
				
				Interface*								QueryService(const WString& identifier)override;
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
					virtual void						SetSubMenuExisting(bool value)=0;
					virtual void						SetSubMenuOpening(bool value)=0;
				};
			protected:
				IStyleController*						styleController;
				GuiMenu*								subMenu;
				Size									preferredMenuClientSize;
				IGuiMenuService*						ownerMenuService;

				void									OpenSubMenuInternal();
				void									OnParentLineChanged()override;
				void									OnSubMenuWindowOpened(elements::GuiGraphicsComposition* sender, elements::GuiEventArgs& arguments);
				void									OnSubMenuWindowClosed(elements::GuiGraphicsComposition* sender, elements::GuiEventArgs& arguments);
				void									OnLeftButtonDown(elements::GuiGraphicsComposition* sender, elements::GuiMouseEventArgs& arguments);
				void									OnMouseEnter(elements::GuiGraphicsComposition* sender, elements::GuiEventArgs& arguments);
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