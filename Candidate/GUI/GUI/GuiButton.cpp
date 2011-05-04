#include "GuiButton.h"

namespace vl
{
	namespace presentation
	{
		using namespace eventargs;

/***********************************************************************
GuiButtonBase
***********************************************************************/

		GuiControl* GuiButtonBase::NotifyMouseDown(eventargs::MouseButton button, const eventargs::MouseInfo& info)
		{
			GuiControl* eventRaiser=GuiControl::NotifyMouseDown(button, info);
			if(IsVisuallyEnabled())
			{
				if(button==eventargs::LeftButton)
				{
					RequireTracking();
					RequireFocus();
					pressing=true;
					buttonState=Pressed;
					NotifyButtonStateChanged();
				}
			}
			return eventRaiser;
		}

		GuiControl* GuiButtonBase::NotifyMouseUp(eventargs::MouseButton button, const eventargs::MouseInfo& info)
		{
			GuiControl* eventRaiser=GuiControl::NotifyMouseUp(button, info);
			if(IsVisuallyEnabled())
			{
				if(button==eventargs::LeftButton)
				{
					ReleaseTracking();
					pressing=false;
					if(entering && buttonState==Pressed)
					{
						NotifyExecuted();
					}
					buttonState=(entering?Active:Normal);
					NotifyButtonStateChanged();
				}
			}
			return eventRaiser;
		}

		void GuiButtonBase::NotifyMouseEntered()
		{
			GuiControl::NotifyMouseEntered();
			if(IsVisuallyEnabled())
			{
				buttonState=(pressing?Pressed:Active);
				entering=true;
			}
			NotifyButtonStateChanged();
		}

		void GuiButtonBase::NotifyMouseLeaved()
		{
			GuiControl::NotifyMouseLeaved();
			if(IsVisuallyEnabled())
			{
				buttonState=(pressing?Active:Normal);
				entering=false;
			}
			NotifyButtonStateChanged();
		}

		void GuiButtonBase::NotifyVisuallyEnabled()
		{
			GuiControl::NotifyVisuallyEnabled();
			if(buttonState==Disabled)
			{
				buttonState=Normal;
			}
			NotifyButtonStateChanged();
		}

		void GuiButtonBase::NotifyVisuallyDisabled()
		{
			GuiControl::NotifyVisuallyDisabled();
			buttonState=Disabled;
			entering=false;
			pressing=false;
			NotifyButtonStateChanged();
		}

		void GuiButtonBase::NotifyButtonStateChanged()
		{
		}

		void GuiButtonBase::NotifyExecuted()
		{
			NotifyEventArgs e;
			e.sender=this;
			e.raiser=this;
			OnExecuted(e);
		}

		GuiButtonBase::GuiButtonBase()
			:buttonState(Normal)
			,pressing(false)
			,entering(false)
		{
		}

		GuiButtonBase::~GuiButtonBase()
		{
		}

/***********************************************************************
GuiTextButton
***********************************************************************/

		const wchar_t* const GuiTextButton::SkinBuilderName = L"vl::presentation::GuiTextButton";

		WString GuiTextButton::GetSkinBuilderName()
		{
			return SkinBuilderName;
		}

		void GuiTextButton::NotifySkinChanged()
		{
			GuiButtonBase::NotifySkinChanged();
			Ptr<IGuiTextButtonSkin> skin=GetSkin().Cast<IGuiTextButtonSkin>();
			if(skin)
			{
				skin->SetState(buttonState);
				skin->SetText(text);
				skin->SetFocus(IsFocusing());
			}
		}

		void GuiTextButton::NotifyButtonStateChanged()
		{
			GuiButtonBase::NotifyButtonStateChanged();
			Ptr<IGuiTextButtonSkin> skin=GetSkin().Cast<IGuiTextButtonSkin>();
			if(skin)
			{
				skin->SetState(buttonState);
			}
		}

		GuiControl* GuiTextButton::NotifyGotFocus()
		{
			GuiControl* actionRaiser=GuiButtonBase::NotifyGotFocus();
			Ptr<IGuiTextButtonSkin> skin=GetSkin().Cast<IGuiTextButtonSkin>();
			if(skin)
			{
				skin->SetFocus(true);
			}
			return actionRaiser;
		}

		GuiControl* GuiTextButton::NotifyLostFocus()
		{
			GuiControl* actionRaiser=GuiButtonBase::NotifyLostFocus();
			Ptr<IGuiTextButtonSkin> skin=GetSkin().Cast<IGuiTextButtonSkin>();
			if(skin)
			{
				skin->SetFocus(false);
			}
			return actionRaiser;
		}

		GuiTextButton::GuiTextButton()
		{
		}

		GuiTextButton::~GuiTextButton()
		{
		}

		WString& GuiTextButton::GetText()
		{
			return text;
		}

		void GuiTextButton::SetText(const WString& value)
		{
			text=value;
			Ptr<IGuiTextButtonSkin> skin=GetSkin().Cast<IGuiTextButtonSkin>();
			if(skin)
			{
				skin->SetText(text);
			}
		}
	}
}