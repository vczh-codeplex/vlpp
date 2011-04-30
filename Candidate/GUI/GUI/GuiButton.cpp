#include "GuiButton.h"

namespace vl
{
	namespace presentation
	{

/***********************************************************************
GuiButtonBase
***********************************************************************/

		GuiControl* GuiButtonBase::NotifyMouseDown(eventargs::MouseButton button, const eventargs::MouseInfo& info)
		{
			GuiControl* eventRaiser=GuiControl::NotifyMouseDown(button, info);
			RequireTracking();
			RequireFocus();
			pressing=true;
			buttonState=Pressed;
			NotifyButtonStateChanged();
			return eventRaiser;
		}

		GuiControl* GuiButtonBase::NotifyMouseUp(eventargs::MouseButton button, const eventargs::MouseInfo& info)
		{
			GuiControl* eventRaiser=GuiControl::NotifyMouseUp(button, info);
			ReleaseTracking();
			pressing=false;
			buttonState=(entering?Active:Normal);
			NotifyButtonStateChanged();
			return eventRaiser;
		}

		void GuiButtonBase::NotifyMouseEntered()
		{
			GuiControl::NotifyMouseEntered();
			buttonState=(pressing?Pressed:Active);
			entering=true;
			NotifyButtonStateChanged();
		}

		void GuiButtonBase::NotifyMouseLeaved()
		{
			GuiControl::NotifyMouseLeaved();
			buttonState=(pressing?Active:Normal);
			entering=false;
			NotifyButtonStateChanged();
		}

		void GuiButtonBase::NotifyButtonStateChanged()
		{
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

		void GuiTextButton::NotifyGotFocus()
		{
			GuiButtonBase::NotifyGotFocus();
			Ptr<IGuiTextButtonSkin> skin=GetSkin().Cast<IGuiTextButtonSkin>();
			if(skin)
			{
				skin->SetFocus(true);
			}
		}

		void GuiTextButton::NotifyLostFocus()
		{
			GuiButtonBase::NotifyLostFocus();
			Ptr<IGuiTextButtonSkin> skin=GetSkin().Cast<IGuiTextButtonSkin>();
			if(skin)
			{
				skin->SetFocus(false);
			}
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