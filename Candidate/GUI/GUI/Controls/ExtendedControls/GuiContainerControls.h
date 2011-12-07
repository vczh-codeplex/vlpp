/***********************************************************************
Vczh Library++ 3.0
Developer: ³Âè÷å«(vczh)
GacUI::Control System

Interfaces:
***********************************************************************/

#ifndef VCZH_PRESENTATION_CONTROLS_GUICONTAINERCONTROLS
#define VCZH_PRESENTATION_CONTROLS_GUICONTAINERCONTROLS

#include "..\GuiBasicControls.h"

namespace vl
{
	namespace presentation
	{
		namespace controls
		{
/***********************************************************************
Tab Control
***********************************************************************/

			class GuiTab;

			class GuiTabPage : public Object
			{
				friend class GuiTab;
				friend class Ptr<GuiTabPage>;
			protected:
				GuiControl*										container;
				GuiTab*											owner;
				WString											text;
				
				GuiTabPage();
				~GuiTabPage();

				bool											AssociateTab(GuiTab* _owner, GuiControl::IStyleController* _styleController);
			public:
				elements::GuiNotifyEvent						TextChanged;

				GuiControl*										GetContainer();
				GuiTab*											GetOwnerTab();
				const WString&									GetText();
				void											SetText(const WString& value);
				bool											GetSelected();
			};

			class GuiTab : public GuiControl
			{
				friend class GuiTabPage;
			public:
				class ICommandExecutor : public Interface
				{
				public:
					virtual void								ShowTab(int index)=0;
				};

				class IStyleController : public virtual GuiControl::IStyleController
				{
				public:
					virtual void								SetCommandExecutor(ICommandExecutor* value)=0;
					virtual void								InsertTab(int index)=0;
					virtual void								SetTabText(int index, const WString& value)=0;
					virtual void								RemoveTab(int index)=0;
					virtual void								MoveTab(int oldIndex, int newIndex)=0;
					virtual void								SetSelectedTab(int index)=0;
					virtual GuiControl::IStyleController*		CreateTabPageStyleController()=0;
				};
			protected:
				class CommandExecutor : public Object, public ICommandExecutor
				{
				protected:
					GuiTab*										tab;
				public:
					CommandExecutor(GuiTab* _tab);
					~CommandExecutor();

					void										ShowTab(int index)override;
				};

				Ptr<CommandExecutor>							commandExecutor;
				IStyleController*								styleController;
				collections::List<GuiTabPage*>					tabPages;
				GuiTabPage*										selectedPage;
			public:
				GuiTab(IStyleController* _styleController);
				~GuiTab();

				elements::GuiNotifyEvent						SelectedPageChanged;

				GuiTabPage*										CreatePage(int index=-1);
				bool											CreatePage(GuiTabPage* page, int index=-1);
				bool											RemovePage(GuiTabPage* value);
				bool											MovePage(GuiTabPage* page, int newIndex);
				const collections::IReadonlyList<GuiTabPage*>&	GetPages();

				GuiTabPage*										GetSelectedPage();
				bool											SetSelectedPage(GuiTabPage* value);
			};
		}
	}
}

#endif