/***********************************************************************
Vczh Library++ 3.0
Developer: ³Âè÷å«(vczh)
GacUI::Control System

Interfaces:
***********************************************************************/

#ifndef VCZH_PRESENTATION_CONTROLS_GUICOMBOCONTROLS
#define VCZH_PRESENTATION_CONTROLS_GUICOMBOCONTROLS

#include "..\GuiWindowControls.h"

namespace vl
{
	namespace presentation
	{
		namespace controls
		{
			class GuiComboBoxBase : public GuiControl
			{
			public:
				class ICommandExecutor : public virtual Interface
				{
				public:
					virtual void							ShowPopup()=0;
					virtual void							SelectItem()=0;
				};

				class IStyleController : public virtual GuiControl::IStyleController
				{
				public:
					virtual void							SetCommandExecutor(ICommandExecutor* value)=0;
					virtual void							OnPopupOpened()=0;
					virtual void							OnPopupClosed()=0;
					virtual void							OnItemSelected()=0;
					virtual GuiControl::IStyleController*	CreatePopupStyle()=0;
				};
			protected:

				class CommandExecutor : public Object, public virtual ICommandExecutor
				{
				protected:
					GuiComboBoxBase*						combo;

				public:
					CommandExecutor(GuiComboBoxBase* _combo);
					~CommandExecutor();

					void									ShowPopup()override;
					void									SelectItem()override;
				};

				Ptr<CommandExecutor>						commandExecutor;
				IStyleController*							styleController;
				GuiPopup*									popup;

				virtual void								SelectItem();
				void										OnPopupOpened(elements::GuiGraphicsComposition* sender, elements::GuiEventArgs& arguments);
				void										OnPopupClosed(elements::GuiGraphicsComposition* sender, elements::GuiEventArgs& arguments);
			public:
				GuiComboBoxBase(IStyleController* _styleController);
				~GuiComboBoxBase();

				elements::GuiNotifyEvent					PopupOpened;
				elements::GuiNotifyEvent					PopupClosed;
				elements::GuiNotifyEvent					ItemSelecting;
				elements::GuiNotifyEvent					ItemSelected;

				void										ShowPopup();
				GuiPopup*									GetPopup();
			};
		}
	}
}

#endif