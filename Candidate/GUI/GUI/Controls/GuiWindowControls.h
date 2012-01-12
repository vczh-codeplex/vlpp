/***********************************************************************
Vczh Library++ 3.0
Developer: ³Âè÷å«(vczh)
GacUI::Control System

Interfaces:
***********************************************************************/

#ifndef VCZH_PRESENTATION_CONTROLS_GUIWINDOWCONTROLS
#define VCZH_PRESENTATION_CONTROLS_GUIWINDOWCONTROLS

#include "GuiBasicControls.h"

namespace vl
{
	namespace presentation
	{
		namespace controls
		{

/***********************************************************************
Control Host
***********************************************************************/

			class GuiControlHost : public GuiControl, private INativeWindowListener, public Description<GuiControlHost>
			{
			protected:
				elements::GuiGraphicsHost*				host;
				collections::List<GuiComponent*>		components;

			private:
				void									Enabled()override;
				void									Disabled()override;
				void									GotFocus()override;
				void									LostFocus()override;
				void									Activated()override;
				void									Deactivated()override;
				void									Opened()override;
				void									Closing(bool& cancel)override;
				void									Closed()override;
				void									Destroying()override;
			public:
				GuiControlHost(GuiControl::IStyleController* _styleController);
				~GuiControlHost();
				
				elements::GuiNotifyEvent				WindowGotFocus;
				elements::GuiNotifyEvent				WindowLostFocus;
				elements::GuiNotifyEvent				WindowActivated;
				elements::GuiNotifyEvent				WindowDeactivated;
				elements::GuiNotifyEvent				WindowOpened;
				elements::GuiRequestEvent				WindowClosing;
				elements::GuiNotifyEvent				WindowClosed;
				elements::GuiNotifyEvent				WindowDestroying;

				elements::GuiGraphicsHost*				GetGraphicsHost();
				elements::GuiGraphicsComposition*		GetMainComposition();
				INativeWindow*							GetNativeWindow();
				void									SetNativeWindow(INativeWindow* window);
				void									Render();

				bool									GetEnabled()override;
				void									SetEnabled(bool value)override;
				bool									GetFocused();
				void									SetFocused();
				bool									GetActivated();
				void									SetActivated();
				bool									GetShowInTaskBar();
				void									SetShowInTaskBar(bool value);
				bool									GetEnabledActivate();
				void									SetEnabledActivate(bool value);

				bool									GetMaximizedBox();
				void									SetMaximizedBox(bool visible);
				bool									GetMinimizedBox();
				void									SetMinimizedBox(bool visible);
				bool									GetBorder();
				void									SetBorder(bool visible);
				bool									GetSizeBox();
				void									SetSizeBox(bool visible);
				bool									GetIconVisible();
				void									SetIconVisible(bool visible);
				bool									GetTitleBar();
				void									SetTitleBar(bool visible);
				bool									GetTopMost();
				void									SetTopMost(bool topmost);

				bool									AddComponent(GuiComponent* component);
				bool									RemoveComponent(GuiComponent* component);
				bool									ContainsComponent(GuiComponent* component);

				Size									GetClientSize();
				void									SetClientSize(Size value);
				Rect									GetBounds();
				void									SetBounds(Rect value);
				GuiControlHost*							GetRelatedControlHost()override;
				const WString&							GetText()override;
				void									SetText(const WString& value)override;

				INativeScreen*							GetRelatedScreen();
				void									Show();
				void									ShowDeactivated();
				void									ShowRestored();
				void									ShowMaximized();
				void									ShowMinimized();
				void									Hide();
				void									Close();
				bool									GetOpening();
			};

/***********************************************************************
Window
***********************************************************************/

			class GuiWindow : public GuiControlHost, public Description<GuiWindow>
			{
				friend class GuiApplication;
			protected:
				virtual void							MouseClickedOnOtherWindow(GuiWindow* window);
			public:
				GuiWindow(GuiControl::IStyleController* _styleController);
				~GuiWindow();
			};

			class GuiPopup : public GuiWindow, public Description<GuiPopup>
			{
			protected:
				void									MouseClickedOnOtherWindow(GuiWindow* window)override;
			public:
				GuiPopup(GuiControl::IStyleController* _styleController);
				~GuiPopup();

				bool									IsClippedByScreen(Point location);
				void									ShowPopup(Point location);
				void									ShowPopup(GuiControl* control, bool preferredTopBottomSide);
			};
		}
	}
}

#endif