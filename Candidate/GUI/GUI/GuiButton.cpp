#include "GuiButton.h"

namespace vl
{
	namespace presentation
	{

/***********************************************************************
GuiButtonBase
***********************************************************************/

		GuiButtonBase::GuiButtonBase()
			:buttonState(Normal)
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
			}
		}

		void GuiTextButton::NotifyButtonStateChanged()
		{
			Ptr<IGuiTextButtonSkin> skin=GetSkin().Cast<IGuiTextButtonSkin>();
			if(skin)
			{
				skin->SetState(buttonState);
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