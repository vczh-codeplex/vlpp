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
					VisuallyEnabledChanged.Execute(GetNotifyEventArguments());

					for(int i=0;i<children.Count();i++)
					{
						children[i]->UpdateVisuallyEnabled();
					}
				}
			}

			GuiControl::GuiControl(Ptr<IGuiStyleController> _styleController)
				:styleController(_styleController)
				,boundsComposition(_styleController->GetBoundsComposition())
				,eventComposition(GetEventComposition(_styleController))
				,containerComposition(_styleController->GetContainerComposition())
				,eventReceiver(GetEventComposition(_styleController)->GetEventReceiver())
				,isEnabled(true)
				,isVisuallyEnabled(true)
				,isVisible(true)
				,parent(0)
			{
				eventComposition->SetAssociatedControl(this);
				VisibleChanged.SetAssociatedComposition(eventComposition);
				EnabledChanged.SetAssociatedComposition(eventComposition);
				VisuallyEnabledChanged.SetAssociatedComposition(eventComposition);
				TextChanged.SetAssociatedComposition(eventComposition);
				FontChanged.SetAssociatedComposition(eventComposition);

				font=GetCurrentController()->GetDefaultFont();
				styleController->SetFont(font);
				styleController->SetText(text);
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

			void GuiControlHost::Style::SetText(const WString& value)
			{
			}

			void GuiControlHost::Style::SetFont(const FontProperties& value)
			{
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

			void GuiButton::OnVisuallyEnabledChanged(elements::GuiGraphicsComposition* sender, elements::GuiEventArgs& arguments)
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
				Clicked.SetAssociatedComposition(eventComposition);

				VisuallyEnabledChanged.AttachMethod(this, &GuiButton::OnVisuallyEnabledChanged);
				GetEventReceiver()->leftButtonDown.AttachMethod(this, &GuiButton::OnLeftButtonDown);
				GetEventReceiver()->leftButtonUp.AttachMethod(this, &GuiButton::OnLeftButtonUp);
				GetEventReceiver()->mouseEnter.AttachMethod(this, &GuiButton::OnMouseEnter);
				GetEventReceiver()->mouseLeave.AttachMethod(this, &GuiButton::OnMouseLeave);
			}

			GuiButton::~GuiButton()
			{
			}
		}
	}
}