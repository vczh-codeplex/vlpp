#include "GuiComboControls.h"

namespace vl
{
	namespace presentation
	{
		namespace controls
		{

/***********************************************************************
GuiComboBoxBase::CommandExecutor
***********************************************************************/

			GuiComboBoxBase::CommandExecutor::CommandExecutor(GuiComboBoxBase* _combo)
				:combo(_combo)
			{
			}

			GuiComboBoxBase::CommandExecutor::~CommandExecutor()
			{
			}

			void GuiComboBoxBase::CommandExecutor::ShowPopup()
			{
				combo->ShowPopup();
			}

			void GuiComboBoxBase::CommandExecutor::SelectItem()
			{
				combo->SelectItem();
			}

/***********************************************************************
GuiComboBoxBase
***********************************************************************/

			void GuiComboBoxBase::SelectItem()
			{
				ItemSelecting.Execute(GetNotifyEventArguments());
				styleController->OnItemSelected();
				ItemSelected.Execute(GetNotifyEventArguments());
			}

			void GuiComboBoxBase::OnClicked(elements::GuiGraphicsComposition* sender, elements::GuiEventArgs& arguments)
			{
				styleController->OnClicked();
			}

			void GuiComboBoxBase::OnPopupOpened(elements::GuiGraphicsComposition* sender, elements::GuiEventArgs& arguments)
			{
				styleController->OnPopupOpened();
				PopupOpened.Execute(arguments);
			}

			void GuiComboBoxBase::OnPopupClosed(elements::GuiGraphicsComposition* sender, elements::GuiEventArgs& arguments)
			{
				styleController->OnPopupClosed();
				PopupClosed.Execute(arguments);
			}

			GuiComboBoxBase::GuiComboBoxBase(IStyleController* _styleController)
				:GuiButton(_styleController)
			{
				commandExecutor=new CommandExecutor(this);
				styleController=dynamic_cast<IStyleController*>(GetStyleController());
				styleController->SetCommandExecutor(commandExecutor.Obj());
				popup=new GuiPopup(styleController->CreatePopupStyle());

				PopupOpened.SetAssociatedComposition(boundsComposition);
				PopupClosed.SetAssociatedComposition(boundsComposition);
				ItemSelecting.SetAssociatedComposition(boundsComposition);
				ItemSelected.SetAssociatedComposition(boundsComposition);

				Clicked.AttachMethod(this, &GuiComboBoxBase::OnClicked);
				popup->WindowOpened.AttachMethod(this, &GuiComboBoxBase::OnPopupOpened);
				popup->WindowClosed.AttachMethod(this, &GuiComboBoxBase::OnPopupClosed);
			}

			GuiComboBoxBase::~GuiComboBoxBase()
			{
				delete popup;
			}

			void GuiComboBoxBase::ShowPopup()
			{
				Size size=popup->GetBoundsComposition()->GetPreferredMinSize();
				size.x=GetBoundsComposition()->GetBounds().Width();
				if(size.y<GetFont().size)
				{
					size.y=GetFont().size;
				}
				popup->GetBoundsComposition()->SetPreferredMinSize(size);
				popup->ShowPopup(this, true);
			}

			GuiPopup* GuiComboBoxBase::GetPopup()
			{
				return popup;
			}

/***********************************************************************
GuiComboBoxListControl
***********************************************************************/

			void GuiComboBoxListControl::DisplaySelectedContent(int itemIndex)
			{
				if(primaryTextView)
				{
					WString text=itemIndex==-1?L"":primaryTextView->GetPrimaryTextViewText(itemIndex);
					SetText(text);
					popup->Hide();
				}
			}

			void GuiComboBoxListControl::OnListControlSelectionChanged(elements::GuiGraphicsComposition* sender, elements::GuiEventArgs& arguments)
			{
				if(containedListControl->GetSelectedItems().Count()==1)
				{
					DisplaySelectedContent(containedListControl->GetSelectedItems()[0]);
				}
				else
				{
					DisplaySelectedContent(-1);
				}
			}

			GuiComboBoxListControl::GuiComboBoxListControl(IStyleController* _styleController, GuiSelectableListControl* _containedListControl)
				:GuiComboBoxBase(_styleController)
				,containedListControl(_containedListControl)
			{
				containedListControl->SetMultiSelect(false);
				containedListControl->SelectionChanged.AttachMethod(this, &GuiComboBoxListControl::OnListControlSelectionChanged);
				primaryTextView=dynamic_cast<GuiListControl::IItemPrimaryTextView*>(containedListControl->GetItemProvider()->RequestView(GuiListControl::IItemPrimaryTextView::Identifier));

				containedListControl->GetBoundsComposition()->SetAlignmentToParent(Margin(0, 0, 0, 0));
				popup->GetBoundsComposition()->AddChild(containedListControl->GetBoundsComposition());
				SetFont(GetFont());
			}

			GuiComboBoxListControl::~GuiComboBoxListControl()
			{
				if(primaryTextView)
				{
					containedListControl->GetItemProvider()->ReleaseView(primaryTextView);
				}
			}

			void GuiComboBoxListControl::SetFont(const FontProperties& value)
			{
				GuiComboBoxBase::SetFont(value);
				Size size=popup->GetBoundsComposition()->GetPreferredMinSize();
				size.y=20*value.size;
				popup->GetBoundsComposition()->SetPreferredMinSize(size);
			}

			GuiSelectableListControl* GuiComboBoxListControl::GetContainedListControl()
			{
				return containedListControl;
			}
		}
	}
}