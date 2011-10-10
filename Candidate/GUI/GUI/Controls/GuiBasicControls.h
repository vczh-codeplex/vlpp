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

			class IGuiStyleController : public Interface
			{
			public:
				virtual elements::GuiBoundsComposition*		GetBoundsComposition()=0;
				virtual elements::GuiGraphicsComposition*	GetContainerComposition()=0;
			};

			class GuiControl : public Object
			{
				friend class elements::GuiGraphicsComposition;
				typedef collections::List<GuiControl*>		ControlList;
			protected:
				Ptr<IGuiStyleController>				styleController;
				elements::GuiBoundsComposition*			boundsComposition;
				elements::GuiGraphicsComposition*		eventComposition;
				elements::GuiGraphicsComposition*		containerComposition;
				elements::GuiGraphicsEventReceiver*		eventReceiver;

				bool									isEnabled;
				bool									isVisible;

				GuiControl*								parent;
				ControlList								children;

				void									OnChildInserted(GuiControl* control);
				void									OnChildRemoved(GuiControl* control);
			public:
				GuiControl(Ptr<IGuiStyleController> _styleController);
				~GuiControl();

				elements::GuiNotifyEvent				VisibleChanged;
				elements::GuiNotifyEvent				EnabledChanged;

				elements::GuiEventArgs					GetNotifyEventArguments();
				IGuiStyleController*					GetStyleController();
				elements::GuiBoundsComposition*			GetBoundsComposition();
				elements::GuiGraphicsComposition*		GetContainerComposition();
				elements::GuiGraphicsEventReceiver*		GetEventReceiver();
				GuiControl*								GetParent();

				virtual bool							GetEnabled();
				virtual void							SetEnabled(bool value);
				virtual bool							GetVisible();
				virtual void							SetVisible(bool value);
			};

			class GuiControlHost : public GuiControl
			{
			protected:
				class Style : public Object, public IGuiStyleController
				{
				protected:
					elements::GuiGraphicsHost*			host;
				public:
					Style();
					~Style();

					elements::GuiBoundsComposition*		GetBoundsComposition();
					elements::GuiGraphicsComposition*	GetContainerComposition();
					elements::GuiGraphicsHost*			GetHost();
				};

				elements::GuiGraphicsHost*				host;
			public:
				GuiControlHost();
				~GuiControlHost();

				elements::GuiGraphicsHost*				GetGraphicsHost();
				INativeWindow*							GetNativeWindow();
				void									SetNativeWindow(INativeWindow* window);
				void									Render();
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
					Disabled,
				};

				class IStyleController : public IGuiStyleController
				{
				public:
					virtual void						Transfer(ControlStyle value)=0;
					virtual void						SetText(const WString& value)=0;
				};
			protected:
				IStyleController*						styleController;
				bool									mousePressing;
				bool									mouseHoving;
				ControlStyle							controlStyle;
				WString									text;
				
				void									UpdateControlStyle();
				void									OnEnabledChanged(elements::GuiGraphicsComposition* sender, elements::GuiEventArgs& arguments);
				void									OnLeftButtonDown(elements::GuiGraphicsComposition* sender, elements::GuiMouseEventArgs& arguments);
				void									OnLeftButtonUp(elements::GuiGraphicsComposition* sender, elements::GuiMouseEventArgs& arguments);
				void									OnMouseEnter(elements::GuiGraphicsComposition* sender, elements::GuiEventArgs& arguments);
				void									OnMouseLeave(elements::GuiGraphicsComposition* sender, elements::GuiEventArgs& arguments);
			public:
				GuiButton(IStyleController* _styleController);
				~GuiButton();

				elements::GuiNotifyEvent				TextChanged;
				elements::GuiNotifyEvent				Clicked;

				virtual const WString&					GetText();
				virtual void							SetText(const WString& value);
			};
		}
	}
}

#endif