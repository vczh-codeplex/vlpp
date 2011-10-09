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

			GuiControl::GuiControl(Ptr<IGuiStyleController> _styleController)
				:styleController(_styleController)
				,boundsComposition(_styleController->GetBoundsComposition())
				,containerComposition(_styleController->GetContainerComposition())
				,eventReceiver(_styleController->GetBoundsComposition()->GetEventReceiver())
				,isEnabled(true)
				,isVisible(true)
			{
				boundsComposition->SetAssociatedControl(this);
				VisibleChanged.SetAssociatedComposition(GetBoundsComposition());
				EnabledChanged.SetAssociatedComposition(GetBoundsComposition());
			}

			GuiControl::~GuiControl()
			{
			}

			elements::GuiEventArgs GuiControl::GetNotifyEventArguments()
			{
				return GuiEventArgs(boundsComposition);
			}

			IGuiStyleController* GuiControl::GetStyleController()
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

/***********************************************************************
GuiButton
***********************************************************************/

			void GuiButton::UpdateControlStyle()
			{
				if(mousePressing)
				{
					if(mouseHoving)
					{
						controlStyle=Pressed;
					}
					else
					{
						controlStyle=Active;
					}
				}
				else
				{
					if(mouseHoving)
					{
						controlStyle=Active;
					}
					else
					{
						controlStyle=Normal;
					}
				}
			}

			void GuiButton::OnEnabledChanged(elements::GuiGraphicsComposition* sender, elements::GuiEventArgs& arguments)
			{
				if(GetEnabled())
				{
					styleController->Transfer(controlStyle);
				}
				else
				{
					styleController->Transfer(Disabled);
				}
			}

			void GuiButton::OnLeftButtonDown(elements::GuiGraphicsComposition* sender, elements::GuiMouseEventArgs& arguments)
			{
				if(arguments.eventSource==GetBoundsComposition())
				{
					mousePressing=true;
					UpdateControlStyle();
				}
				if(GetEnabled())
				{
					styleController->Transfer(controlStyle);
				}
			}

			void GuiButton::OnLeftButtonUp(elements::GuiGraphicsComposition* sender, elements::GuiMouseEventArgs& arguments)
			{
				if(arguments.eventSource==GetBoundsComposition())
				{
					mousePressing=false;
					UpdateControlStyle();
				}
				if(GetEnabled())
				{
					styleController->Transfer(controlStyle);
					if(mouseHoving)
					{
						Clicked.Execute(GetNotifyEventArguments());
					}
				}
			}

			void GuiButton::OnMouseEnter(elements::GuiGraphicsComposition* sender, elements::GuiMouseEventArgs& arguments)
			{
				if(arguments.eventSource==GetBoundsComposition())
				{
					mouseHoving=true;
					UpdateControlStyle();
				}
				if(GetEnabled())
				{
					styleController->Transfer(controlStyle);
				}
			}

			void GuiButton::OnMouseLeave(elements::GuiGraphicsComposition* sender, elements::GuiMouseEventArgs& arguments)
			{
				if(arguments.eventSource==GetBoundsComposition())
				{
					mouseHoving=false;
					UpdateControlStyle();
				}
				if(GetEnabled())
				{
					styleController->Transfer(controlStyle);
				}
			}

			GuiButton::GuiButton(IStyleController* _styleController)
				:GuiControl(_styleController)
				,styleController(_styleController)
				,mousePressing(false)
				,mouseHoving(false)
				,controlStyle(Normal)
			{
				TextChanged.SetAssociatedComposition(GetBoundsComposition());
				Clicked.SetAssociatedComposition(GetBoundsComposition());
			}

			GuiButton::~GuiButton()
			{
			}

			const WString& GuiButton::GetText()
			{
				return text;
			}

			void GuiButton::SetText(const WString& value)
			{
				if(text!=value)
				{
					text=value;
					styleController->SetText(text);
					TextChanged.Execute(GetNotifyEventArguments());
				}
			}
		}
	}
}