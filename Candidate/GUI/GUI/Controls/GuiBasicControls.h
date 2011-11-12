/***********************************************************************
Vczh Library++ 3.0
Developer: ³Âè÷å«(vczh)
GacUI::Control System

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
					virtual void								SetFocusableComposition(elements::GuiGraphicsComposition* value)=0;
					virtual void								SetText(const WString& value)=0;
					virtual void								SetFont(const FontProperties& value)=0;
					virtual void								SetVisuallyEnabled(bool value)=0;
				};

				class IStyleProvider : public Interface
				{
				public:
					virtual void								AssociateStyleController(IStyleController* controller)=0;
					virtual void								SetFocusableComposition(elements::GuiGraphicsComposition* value)=0;
					virtual void								SetText(const WString& value)=0;
					virtual void								SetFont(const FontProperties& value)=0;
					virtual void								SetVisuallyEnabled(bool value)=0;
				};
			protected:
				Ptr<IStyleController>					styleController;
				elements::GuiBoundsComposition*			boundsComposition;
				elements::GuiGraphicsComposition*		focusableComposition;
				elements::GuiGraphicsEventReceiver*		eventReceiver;

				bool									isEnabled;
				bool									isVisuallyEnabled;
				bool									isVisible;
				WString									text;
				FontProperties							font;

				GuiControl*								parent;
				ControlList								children;

				virtual void							OnChildInserted(GuiControl* control);
				virtual void							OnChildRemoved(GuiControl* control);
				virtual void							OnParentChanged(GuiControl* oldParent, GuiControl* newParent);
				virtual void							OnParentLineChanged();
				virtual void							OnRenderTargetChanged(elements::IGuiGraphicsRenderTarget* renderTarget);
				virtual void							UpdateVisuallyEnabled();
				void									SetFocusableComposition(elements::GuiGraphicsComposition* value);
			public:
				GuiControl(IStyleController* _styleController);
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
				elements::GuiGraphicsComposition*		GetFocusableComposition();
				elements::GuiGraphicsEventReceiver*		GetEventReceiver();
				GuiControl*								GetParent();
				int										GetChildrenCount();
				GuiControl*								GetChild(int index);

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

			class GuiComponent : public Object
			{
				friend class GuiControlHost;
			private:
				GuiControlHost*							controlHost;

				void									SetControlHost(GuiControlHost* value);
			public:
				GuiComponent();
				~GuiComponent();

				GuiControlHost*							GetControlHost();
			};

/***********************************************************************
Buttons
***********************************************************************/

			class GuiButton : public GuiControl
			{
			public:
				enum ControlState
				{
					Normal,
					Active,
					Pressed,
				};

				class IStyleController : public GuiControl::IStyleController
				{
				public:
					virtual void						Transfer(ControlState value)=0;
				};
			protected:
				IStyleController*						styleController;
				bool									mousePressing;
				bool									mouseHoving;
				ControlState							controlState;
				
				void									UpdateControlState();
				void									OnLeftButtonDown(elements::GuiGraphicsComposition* sender, elements::GuiMouseEventArgs& arguments);
				void									OnLeftButtonUp(elements::GuiGraphicsComposition* sender, elements::GuiMouseEventArgs& arguments);
				void									OnMouseEnter(elements::GuiGraphicsComposition* sender, elements::GuiEventArgs& arguments);
				void									OnMouseLeave(elements::GuiGraphicsComposition* sender, elements::GuiEventArgs& arguments);
			public:
				GuiButton(IStyleController* _styleController);
				~GuiButton();

				elements::GuiNotifyEvent				Clicked;
			};

			class GuiSelectableButton : public GuiButton
			{
			public:
				class IStyleController : public GuiButton::IStyleController
				{
				public:
					virtual void						SetSelected(bool value)=0;
				};

				class GroupController : public GuiComponent
				{
				protected:
					collections::List<GuiSelectableButton*>	buttons;
				public:
					GroupController();
					~GroupController();

					virtual void						Attach(GuiSelectableButton* button);
					virtual void						Detach(GuiSelectableButton* button);
					virtual void						OnSelectedChanged(GuiSelectableButton* button)=0;
				};

				class MutexGroupController : public GroupController
				{
				protected:
					bool								suppress;
				public:
					MutexGroupController();
					~MutexGroupController();

					void								OnSelectedChanged(GuiSelectableButton* button)override;
				};

			protected:
				IStyleController*						styleController;
				GroupController*						groupController;
				bool									autoSelection;
				bool									isSelected;

				void									OnClicked(elements::GuiGraphicsComposition* sender, elements::GuiEventArgs& arguments);
			public:
				GuiSelectableButton(IStyleController* _styleController);
				~GuiSelectableButton();

				elements::GuiNotifyEvent				SelectedChanged;

				virtual GroupController*				GetGroupController();
				virtual void							SetGroupController(GroupController* value);

				virtual bool							GetAutoSelection();
				virtual void							SetAutoSelection(bool value);

				virtual bool							GetSelected();
				virtual void							SetSelected(bool value);
			};

/***********************************************************************
Scrolls
***********************************************************************/

			class GuiScroll : public GuiControl
			{
			public:
				class ICommandExecutor : public Interface
				{
				public:
					virtual void						SmallDecrease()=0;
					virtual void						SmallIncrease()=0;
					virtual void						BigDecrease()=0;
					virtual void						BigIncrease()=0;

					virtual void						SetTotalSize(int value)=0;
					virtual void						SetPageSize(int value)=0;
					virtual void						SetPosition(int value)=0;
				};

				class IStyleController : public GuiControl::IStyleController
				{
				public:
					virtual void						SetCommandExecutor(ICommandExecutor* value)=0;
					virtual void						SetTotalSize(int value)=0;
					virtual void						SetPageSize(int value)=0;
					virtual void						SetPosition(int value)=0;
				};
			protected:
				class CommandExecutor : public Object, public ICommandExecutor
				{
				protected:
					GuiScroll*							scroll;
				public:
					CommandExecutor(GuiScroll* _scroll);
					~CommandExecutor();

					void								SmallDecrease()override;
					void								SmallIncrease()override;
					void								BigDecrease()override;
					void								BigIncrease()override;

					void								SetTotalSize(int value)override;
					void								SetPageSize(int value)override;
					void								SetPosition(int value)override;
				};

				IStyleController*						styleController;
				Ptr<CommandExecutor>					commandExecutor;
				int										totalSize;
				int										pageSize;
				int										position;
				int										smallMove;
				int										bigMove;
			public:
				GuiScroll(IStyleController* _styleController);
				~GuiScroll();

				elements::GuiNotifyEvent				PositionChanged;

				virtual int								GetTotalSize();
				virtual void							SetTotalSize(int value);
				virtual int								GetPageSize();
				virtual void							SetPageSize(int value);
				virtual int								GetPosition();
				virtual void							SetPosition(int value);
				virtual int								GetSmallMove();
				virtual void							SetSmallMove(int value);
				virtual int								GetBigMove();
				virtual void							SetBigMove(int value);

				int										GetMinPosition();
				int										GetMaxPosition();
			};

			class GuiScrollView : public GuiControl
			{
			public:
				class IStyleProvider : public GuiControl::IStyleProvider
				{
				public:
					virtual GuiScroll::IStyleController*		CreateHorizontalScrollStyle()=0;
					virtual GuiScroll::IStyleController*		CreateVerticalScrollStyle()=0;
					virtual int									GetDefaultScrollSize()=0;
					virtual elements::GuiGraphicsComposition*	InstallBackground(elements::GuiBoundsComposition* boundsComposition)=0;
				};

				class StyleController : public Object, public GuiControl::IStyleController
				{
				protected:
					Ptr<IStyleProvider>					styleProvider;
					GuiScrollView*						scrollView;
					GuiScroll*							horizontalScroll;
					GuiScroll*							verticalScroll;
					elements::GuiBoundsComposition*		boundsComposition;
					elements::GuiTableComposition*		tableComposition;
					elements::GuiCellComposition*		containerCellComposition;
					elements::GuiBoundsComposition*		containerComposition;
					bool								horizontalAlwaysVisible;
					bool								verticalAlwaysVisible;

					void								UpdateTable();
				public:
					StyleController(IStyleProvider* _styleProvider);
					~StyleController();

					void								SetScrollView(GuiScrollView* _scrollView);
					void								AdjustView(Size fullSize);

					GuiScroll*							GetHorizontalScroll();
					GuiScroll*							GetVerticalScroll();
					elements::GuiTableComposition*		GetInternalTableComposition();
					elements::GuiBoundsComposition*		GetInternalContainerComposition();

					bool								GetHorizontalAlwaysVisible();
					void								SetHorizontalAlwaysVisible(bool value);
					bool								GetVerticalAlwaysVisible();
					void								SetVerticalAlwaysVisible(bool value);

					elements::GuiBoundsComposition*		GetBoundsComposition()override;
					elements::GuiGraphicsComposition*	GetContainerComposition()override;
					void								SetFocusableComposition(elements::GuiGraphicsComposition* value)override;
					void								SetText(const WString& value)override;
					void								SetFont(const FontProperties& value)override;
					void								SetVisuallyEnabled(bool value)override;
				};
			protected:

				StyleController*						styleController;
				bool									supressScrolling;

				void									OnContainerBoundsChanged(elements::GuiGraphicsComposition* sender, elements::GuiEventArgs& arguments);
				void									OnHorizontalScroll(elements::GuiGraphicsComposition* sender, elements::GuiEventArgs& arguments);
				void									OnVerticalScroll(elements::GuiGraphicsComposition* sender, elements::GuiEventArgs& arguments);
				void									CallUpdateView();
				void									Initialize();

				virtual Size							QueryFullSize()=0;
				virtual void							UpdateView(Rect viewBounds)=0;

				GuiScrollView(StyleController* _styleController);
			public:
				GuiScrollView(IStyleProvider* styleProvider);
				~GuiScrollView();

				void									CalculateView();

				GuiScroll*								GetHorizontalScroll();
				GuiScroll*								GetVerticalScroll();
				bool									GetHorizontalAlwaysVisible();
				void									SetHorizontalAlwaysVisible(bool value);
				bool									GetVerticalAlwaysVisible();
				void									SetVerticalAlwaysVisible(bool value);
			};

			class GuiScrollContainer : public GuiScrollView
			{
			public:
				class StyleController : public GuiScrollView::StyleController
				{
				protected:
					elements::GuiBoundsComposition*		controlContainerComposition;
				public:
					StyleController(GuiScrollView::IStyleProvider* styleProvider);
					~StyleController();

					elements::GuiGraphicsComposition*	GetContainerComposition()override;
					void								MoveContainer(Point leftTop);
				};

			protected:
				StyleController*						styleController;

				void									OnControlContainerBoundsChanged(elements::GuiGraphicsComposition* sender, elements::GuiEventArgs& arguments);
				Size									QueryFullSize()override;
				void									UpdateView(Rect viewBounds)override;
			public:
				GuiScrollContainer(GuiScrollContainer::IStyleProvider* styleProvider);
				~GuiScrollContainer();
			};
		}
	}
}

#endif