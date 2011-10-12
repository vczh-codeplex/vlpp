/***********************************************************************
Vczh Library++ 3.0
Developer: ³Âè÷å«(vczh)
GUI::Windows Platform

Interfaces:
***********************************************************************/

#ifndef VCZH_PRESENTATION_CONTROLS_GUIBASICCONTROLS
#define VCZH_PRESENTATION_CONTROLS_GUIBASICCONTROLS

#include "..\GraphicsElement\GuiGraphicsHost.h"

namespace vl
{
	namespace presentation
	{
		namespace controls
		{

/***********************************************************************
Basic Construction
***********************************************************************/

			class GuiControl : public Object
			{
				friend class elements::GuiGraphicsComposition;
				typedef collections::List<GuiControl*>		ControlList;
			public:
				class IStyleController : public Interface
				{
				public:
					virtual elements::GuiBoundsComposition*		GetBoundsComposition()=0;
					virtual elements::GuiGraphicsComposition*	GetContainerComposition()=0;
					virtual void								SetText(const WString& value)=0;
					virtual void								SetFont(const FontProperties& value)=0;
					virtual void								SetVisuallyEnabled(bool value)=0;
				};
			protected:
				Ptr<IStyleController>					styleController;
				elements::GuiBoundsComposition*			boundsComposition;
				elements::GuiGraphicsComposition*		eventComposition;
				elements::GuiGraphicsComposition*		containerComposition;
				elements::GuiGraphicsEventReceiver*		eventReceiver;

				bool									isEnabled;
				bool									isVisuallyEnabled;
				bool									isVisible;
				WString									text;
				FontProperties							font;

				GuiControl*								parent;
				ControlList								children;

				void									OnChildInserted(GuiControl* control);
				void									OnChildRemoved(GuiControl* control);
				void									UpdateVisuallyEnabled();
			public:
				GuiControl(Ptr<IStyleController> _styleController);
				~GuiControl();

				elements::GuiNotifyEvent				VisibleChanged;
				elements::GuiNotifyEvent				EnabledChanged;
				elements::GuiNotifyEvent				VisuallyEnabledChanged;
				elements::GuiNotifyEvent				TextChanged;
				elements::GuiNotifyEvent				FontChanged;

				elements::GuiEventArgs					GetNotifyEventArguments();
				IStyleController*						GetStyleController();
				elements::GuiBoundsComposition*			GetBoundsComposition();
				elements::GuiGraphicsComposition*		GetContainerComposition();
				elements::GuiGraphicsEventReceiver*		GetEventReceiver();
				GuiControl*								GetParent();

				virtual bool							GetVisuallyEnabled();
				virtual bool							GetEnabled();
				virtual void							SetEnabled(bool value);
				virtual bool							GetVisible();
				virtual void							SetVisible(bool value);
				virtual const WString&					GetText();
				virtual void							SetText(const WString& value);
				virtual const FontProperties&			GetFont();
				virtual void							SetFont(const FontProperties& value);
			};

			class GuiControlHost : public GuiControl
			{
			protected:
				class Style : public Object, public GuiControl::IStyleController
				{
				protected:
					elements::GuiGraphicsHost*			host;
				public:
					Style();
					~Style();

					elements::GuiBoundsComposition*		GetBoundsComposition();
					elements::GuiGraphicsComposition*	GetContainerComposition();
					elements::GuiGraphicsHost*			GetHost();

					void								SetText(const WString& value);
					void								SetFont(const FontProperties& value);
					void								SetVisuallyEnabled(bool value);
				};

				elements::GuiGraphicsHost*				host;
			public:
				GuiControlHost();
				~GuiControlHost();

				elements::GuiGraphicsHost*				GetGraphicsHost();
				elements::GuiGraphicsComposition*		GetMainComposition();
				INativeWindow*							GetNativeWindow();
				void									SetNativeWindow(INativeWindow* window);
				void									Render();

				void									Show();
				void									ShowRestored();
				void									ShowMaximized();
				void									ShowMinimized();
				void									Hide();
				void									Close();
			};

/***********************************************************************
Controls
***********************************************************************/

			class GuiButton : public GuiControl
			{
			public:
				enum ControlStyle
				{
					Normal,
					Active,
					Pressed,
				};

				class IStyleController : public GuiControl::IStyleController
				{
				public:
					virtual void						Transfer(ControlStyle value)=0;
				};
			protected:
				IStyleController*						styleController;
				bool									mousePressing;
				bool									mouseHoving;
				ControlStyle							controlStyle;
				
				void									UpdateControlStyle();
				void									OnLeftButtonDown(elements::GuiGraphicsComposition* sender, elements::GuiMouseEventArgs& arguments);
				void									OnLeftButtonUp(elements::GuiGraphicsComposition* sender, elements::GuiMouseEventArgs& arguments);
				void									OnMouseEnter(elements::GuiGraphicsComposition* sender, elements::GuiEventArgs& arguments);
				void									OnMouseLeave(elements::GuiGraphicsComposition* sender, elements::GuiEventArgs& arguments);
			public:
				GuiButton(IStyleController* _styleController);
				~GuiButton();

				elements::GuiNotifyEvent				Clicked;
			};
		}
	}
}

#endif