#include "GuiBasicControls.h"

namespace vl
{
	namespace presentation
	{
		namespace controls
		{
			using namespace elements;

/***********************************************************************
GuiControl
***********************************************************************/

			void GuiControl::OnChildInserted(GuiControl* control)
			{
				children.Add(control);
				control->parent=this;
				control->UpdateVisuallyEnabled();
			}

			void GuiControl::OnChildRemoved(GuiControl* control)
			{
				control->parent=0;
				children.Remove(control);
			}

			void GuiControl::UpdateVisuallyEnabled()
			{
				bool newValue=isEnabled && (parent==0?true:parent->GetVisuallyEnabled());
				if(isVisuallyEnabled!=newValue)
				{
					isVisuallyEnabled=newValue;
					styleController->SetVisuallyEnabled(isVisuallyEnabled);
					VisuallyEnabledChanged.Execute(GetNotifyEventArguments());

					for(int i=0;i<children.Count();i++)
					{
						children[i]->UpdateVisuallyEnabled();
					}
				}
			}

			GuiControl::GuiControl(IStyleController* _styleController)
				:styleController(_styleController)
				,boundsComposition(_styleController->GetBoundsComposition())
				,containerComposition(_styleController->GetContainerComposition())
				,eventReceiver(_styleController->GetBoundsComposition()->GetEventReceiver())
				,isEnabled(true)
				,isVisuallyEnabled(true)
				,isVisible(true)
				,parent(0)
			{
				boundsComposition->SetAssociatedControl(this);
				VisibleChanged.SetAssociatedComposition(boundsComposition);
				EnabledChanged.SetAssociatedComposition(boundsComposition);
				VisuallyEnabledChanged.SetAssociatedComposition(boundsComposition);
				TextChanged.SetAssociatedComposition(boundsComposition);
				FontChanged.SetAssociatedComposition(boundsComposition);

				font=GetCurrentController()->GetDefaultFont();
				styleController->SetFont(font);
				styleController->SetText(text);
				styleController->SetVisuallyEnabled(isVisuallyEnabled);
			}

			GuiControl::~GuiControl()
			{
				for(int i=0;i<children.Count();i++)
				{
					delete children[i];
				}
			}

			elements::GuiEventArgs GuiControl::GetNotifyEventArguments()
			{
				return GuiEventArgs(boundsComposition);
			}

			GuiControl::IStyleController* GuiControl::GetStyleController()
			{
				return styleController.Obj();
			}

			elements::GuiBoundsComposition* GuiControl::GetBoundsComposition()
			{
				return boundsComposition;
			}

			elements::GuiGraphicsComposition* GuiControl::GetContainerComposition()
			{
				return containerComposition;
			}

			elements::GuiGraphicsEventReceiver* GuiControl::GetEventReceiver()
			{
				return eventReceiver;
			}

			GuiControl* GuiControl::GetParent()
			{
				return parent;
			}

			bool GuiControl::GetVisuallyEnabled()
			{
				return isVisuallyEnabled;
			}

			bool GuiControl::GetEnabled()
			{
				return isEnabled;
			}

			void GuiControl::SetEnabled(bool value)
			{
				if(isEnabled!=value)
				{
					isEnabled=value;
					EnabledChanged.Execute(GetNotifyEventArguments());
					UpdateVisuallyEnabled();
				}
			}

			bool GuiControl::GetVisible()
			{
				return isVisible;
			}

			void GuiControl::SetVisible(bool value)
			{
				boundsComposition->SetVisible(value);
				if(isVisible!=value)
				{
					isVisible=value;
					VisibleChanged.Execute(GetNotifyEventArguments());
				}
			}

			const WString& GuiControl::GetText()
			{
				return text;
			}

			void GuiControl::SetText(const WString& value)
			{
				if(text!=value)
				{
					text=value;
					styleController->SetText(text);
					TextChanged.Execute(GetNotifyEventArguments());
				}
			}

			const FontProperties& GuiControl::GetFont()
			{
				return font;
			}

			void GuiControl::SetFont(const FontProperties& value)
			{
				if(font!=value)
				{
					font=value;
					styleController->SetFont(font);
					FontChanged.Execute(GetNotifyEventArguments());
				}
			}

/***********************************************************************
GuiComponent
***********************************************************************/
			
			GuiComponent::GuiComponent()
				:controlHost(0)
			{
			}

			GuiComponent::~GuiComponent()
			{
			}

			GuiControlHost* GuiComponent::GetControlHost()
			{
				return controlHost;
			}

			void GuiComponent::SetControlHost(GuiControlHost* value)
			{
				controlHost=value;
			}

/***********************************************************************
GuiControlHost
***********************************************************************/

			GuiControlHost::GuiControlHost(GuiControl::IStyleController* _styleController)
				:GuiControl(_styleController)
			{
				GetStyleController()->GetBoundsComposition()->SetAlignmentToParent(Margin(0, 0, 0, 0));

				host=new GuiGraphicsHost;
				host->GetMainComposition()->AddChild(GetStyleController()->GetBoundsComposition());
			}

			GuiControlHost::~GuiControlHost()
			{
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
				host->SetNativeWindow(window);
			}

			void GuiControlHost::Render()
			{
				host->Render();
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
GuiButton
***********************************************************************/

			void GuiButton::UpdateControlState()
			{
				ControlState newControlState=Normal;
				if(mousePressing)
				{
					if(mouseHoving)
					{
						newControlState=Pressed;
					}
					else
					{
						newControlState=Active;
					}
				}
				else
				{
					if(mouseHoving)
					{
						newControlState=Active;
					}
					else
					{
						newControlState=Normal;
					}
				}
				if(controlState!=newControlState)
				{
					controlState=newControlState;
					styleController->Transfer(controlState);
				}
			}

			void GuiButton::OnLeftButtonDown(elements::GuiGraphicsComposition* sender, elements::GuiMouseEventArgs& arguments)
			{
				if(arguments.eventSource==GetBoundsComposition())
				{
					mousePressing=true;
					UpdateControlState();
				}
			}

			void GuiButton::OnLeftButtonUp(elements::GuiGraphicsComposition* sender, elements::GuiMouseEventArgs& arguments)
			{
				if(arguments.eventSource==GetBoundsComposition())
				{
					mousePressing=false;
					UpdateControlState();
				}
				if(GetVisuallyEnabled())
				{
					if(mouseHoving)
					{
						Clicked.Execute(GetNotifyEventArguments());
					}
				}
			}

			void GuiButton::OnMouseEnter(elements::GuiGraphicsComposition* sender, elements::GuiEventArgs& arguments)
			{
				if(arguments.eventSource==GetBoundsComposition())
				{
					mouseHoving=true;
					UpdateControlState();
				}
			}

			void GuiButton::OnMouseLeave(elements::GuiGraphicsComposition* sender, elements::GuiEventArgs& arguments)
			{
				if(arguments.eventSource==GetBoundsComposition())
				{
					mouseHoving=false;
					UpdateControlState();
				}
			}

			GuiButton::GuiButton(IStyleController* _styleController)
				:GuiControl(_styleController)
				,styleController(_styleController)
				,mousePressing(false)
				,mouseHoving(false)
				,controlState(Normal)
			{
				Clicked.SetAssociatedComposition(boundsComposition);

				GetEventReceiver()->leftButtonDown.AttachMethod(this, &GuiButton::OnLeftButtonDown);
				GetEventReceiver()->leftButtonUp.AttachMethod(this, &GuiButton::OnLeftButtonUp);
				GetEventReceiver()->mouseEnter.AttachMethod(this, &GuiButton::OnMouseEnter);
				GetEventReceiver()->mouseLeave.AttachMethod(this, &GuiButton::OnMouseLeave);
			}

			GuiButton::~GuiButton()
			{
			}

/***********************************************************************
GuiSelectableButton::GroupController
***********************************************************************/

			GuiSelectableButton::GroupController::GroupController()
			{
			}

			GuiSelectableButton::GroupController::~GroupController()
			{
			}

			void GuiSelectableButton::GroupController::Attach(GuiSelectableButton* button)
			{
				if(!buttons.Contains(button))
				{
					buttons.Add(button);
				}
			}

			void GuiSelectableButton::GroupController::Detach(GuiSelectableButton* button)
			{
				buttons.Remove(button);
			}

			void GuiSelectableButton::OnClicked(elements::GuiGraphicsComposition* sender, elements::GuiEventArgs& arguments)
			{
				SetSelected(!GetSelected());
			}

/***********************************************************************
GuiSelectableButton::MutexGroupController
***********************************************************************/

			void GuiSelectableButton::MutexGroupController::OnSelectedChanged(GuiSelectableButton* button)
			{
				if(button->GetSelected())
				{
					for(int i=0;i<buttons.Count();i++)
					{
						if(buttons[i]!=button)
						{
							buttons[i]->SetSelected(false);
						}
					}
				}
			}

/***********************************************************************
GuiSelectableButton
***********************************************************************/

			GuiSelectableButton::GuiSelectableButton(IStyleController* _styleController)
				:GuiButton(_styleController)
				,styleController(_styleController)
				,groupController(0)
				,isSelected(false)
			{
				SelectedChanged.SetAssociatedComposition(boundsComposition);

				Clicked.AttachMethod(this, &GuiSelectableButton::OnClicked);
				styleController->SetSelected(isSelected);
			}
			
			GuiSelectableButton::~GuiSelectableButton()
			{
			}

			GuiSelectableButton::GroupController* GuiSelectableButton::GetGroupController()
			{
				return groupController;
			}

			void GuiSelectableButton::SetGroupController(GroupController* value)
			{
				if(groupController)
				{
					groupController->Detach(this);
				}
				groupController=value;
				if(groupController)
				{
					groupController->Attach(this);
				}
			}

			bool GuiSelectableButton::GetSelected()
			{
				return isSelected;
			}

			void GuiSelectableButton::SetSelected(bool value)
			{
				if(isSelected!=value)
				{
					isSelected=value;
					styleController->SetSelected(isSelected);
					if(groupController)
					{
						groupController->OnSelectedChanged(this);
					}
					SelectedChanged.Execute(GetNotifyEventArguments());
				}
			}
		}
	}
}