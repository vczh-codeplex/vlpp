#include "GuiBasicControls.h"

namespace vl
{
	namespace presentation
	{
		namespace controls
		{
			using namespace elements;

			GuiGraphicsComposition* GetEventComposition(Ptr<IGuiStyleController> styleController)
			{
				if(styleController->GetBoundsComposition())
				{
					return styleController->GetBoundsComposition();
				}
				else
				{
					return styleController->GetContainerComposition();
				}
			}

/***********************************************************************
GuiControl
***********************************************************************/

			void GuiControl::OnChildInserted(GuiControl* control)
			{
				children.Add(control);
				control->parent=this;
			}

			void GuiControl::OnChildRemoved(GuiControl* control)
			{
				control->parent=0;
				children.Remove(control);
			}

			GuiControl::GuiControl(Ptr<IGuiStyleController> _styleController)
				:styleController(_styleController)
				,boundsComposition(_styleController->GetBoundsComposition())
				,eventComposition(GetEventComposition(_styleController))
				,containerComposition(_styleController->GetContainerComposition())
				,eventReceiver(GetEventComposition(_styleController)->GetEventReceiver())
				,isEnabled(true)
				,isVisible(true)
				,parent(0)
			{
				eventComposition->SetAssociatedControl(this);
				VisibleChanged.SetAssociatedComposition(eventComposition);
				EnabledChanged.SetAssociatedComposition(eventComposition);
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
				return GuiEventArgs(eventComposition);
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

			GuiControl* GuiControl::GetParent()
			{
				return parent;
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
GuiControlHost
***********************************************************************/

			GuiControlHost::Style::Style()
				:host(new GuiGraphicsHost)
			{
			}

			GuiControlHost::Style::~Style()
			{
				delete host;
			}

			elements::GuiBoundsComposition* GuiControlHost::Style::GetBoundsComposition()
			{
				return 0;
			}

			elements::GuiGraphicsComposition* GuiControlHost::Style::GetContainerComposition()
			{
				return host->GetMainComposition();
			}

			elements::GuiGraphicsHost* GuiControlHost::Style::GetHost()
			{
				return host;
			}

			GuiControlHost::GuiControlHost()
				:GuiControl(new Style)
			{
				host=dynamic_cast<Style*>(GetStyleController())->GetHost();
			}

			GuiControlHost::~GuiControlHost()
			{
			}

			elements::GuiGraphicsHost* GuiControlHost::GetGraphicsHost()
			{
				return host;
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

			void GuiButton::OnMouseEnter(elements::GuiGraphicsComposition* sender, elements::GuiEventArgs& arguments)
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

			void GuiButton::OnMouseLeave(elements::GuiGraphicsComposition* sender, elements::GuiEventArgs& arguments)
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

				GetEventReceiver()->leftButtonDown.Attach(new GuiMouseEvent::FunctionHandler(GuiMouseEvent::FunctionType(this, &GuiButton::OnLeftButtonDown)));
				GetEventReceiver()->leftButtonUp.Attach(new GuiMouseEvent::FunctionHandler(GuiMouseEvent::FunctionType(this, &GuiButton::OnLeftButtonUp)));
				GetEventReceiver()->mouseEnter.Attach(new GuiNotifyEvent::FunctionHandler(GuiNotifyEvent::FunctionType(this, &GuiButton::OnMouseEnter)));
				GetEventReceiver()->mouseLeave.Attach(new GuiNotifyEvent::FunctionHandler(GuiNotifyEvent::FunctionType(this, &GuiButton::OnMouseLeave)));
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