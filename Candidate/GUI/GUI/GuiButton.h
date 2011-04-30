/***********************************************************************
Vczh Library++ 3.0
Developer: 陈梓瀚(vczh)
GUI::Control Library

Interfaces:
***********************************************************************/

#ifndef VCZH_PRESENTATION_GUIBUTTON
#define VCZH_PRESENTATION_GUIBUTTON

#include "GuiControl.h"

namespace vl
{
	namespace presentation
	{

/***********************************************************************
按钮基类
***********************************************************************/

		class GuiButtonBase : public GuiControl
		{
		public:
			enum ButtonState
			{
				Normal,
				Active,
				Pressed,
			};
		protected:
			ButtonState							buttonState;
			bool								pressing;
			bool								entering;
			
			GuiControl*							NotifyMouseDown(eventargs::MouseButton button, const eventargs::MouseInfo& info);
			GuiControl*							NotifyMouseUp(eventargs::MouseButton button, const eventargs::MouseInfo& info);

			void								NotifyMouseEntered();
			void								NotifyMouseLeaved();
			virtual void						NotifyButtonStateChanged();
		public:
			GuiButtonBase();
			~GuiButtonBase();
		};

/***********************************************************************
文字按钮
***********************************************************************/

		class GuiTextButton : public GuiButtonBase
		{
		public:
			static const wchar_t* const		SkinBuilderName;

			class IGuiTextButtonSkin : public virtual IGuiSkin
			{
			public:
				virtual void					SetState(ButtonState style)=0;
				virtual void					SetText(const WString& text)=0;
				virtual void					SetFocus(bool focusing)=0;
			};
		protected:
			WString								text;

			WString								GetSkinBuilderName();
			void								NotifySkinChanged();
			void								NotifyButtonStateChanged();

			GuiControl*							NotifyGotFocus();
			GuiControl*							NotifyLostFocus();
		public:
			GuiTextButton();
			~GuiTextButton();

			WString&							GetText();
			void								SetText(const WString& value);
		};
	}
}

#endif