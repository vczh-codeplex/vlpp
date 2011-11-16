/***********************************************************************
Vczh Library++ 3.0
Developer: ³Âè÷å«(vczh)
GacUI::Control Styles::Windows7 Styles

Clases:
	GuiControl::IStyleController
		Win7WindowStyle
		Win7MenuStyle
		Win7MenuSplitterStyle
		Win7MenuBarStyle
		Win7GroupBoxStyle
	GuiSelectableButton::IStyleController
		Win7ButtonStyle(vertical|horizontal)
		Win7SelectableItemStyle
		Win7ToolstripButtonStyle
		Win7CheckBoxStyle(check|radio)
	GuiMenuButton::IStyleController
		Win7MenuBarButtonStyle
		Win7MenuItemButtonStyle
	GuiScroll::IStyleController
		Win7ScrollStyle
		Win7TrackStyle
	GuiScrollView::IStyleProvider
		Win7ScrollViewProvider
		Win7MultilineTextBoxProvider
	GuiSinglelineTextBox::IStyleProvider
		Win7SinglelineTextBoxProvider
	list::TextItemStyleProvider::ITextItemStyleProvider
		Win7TextListProvider
		Win7CheckTextListProvider
		Win7RadioTextListProvider
***********************************************************************/

#ifndef VCZH_PRESENTATION_CONTROLS_GUIWIN7STYLES
#define VCZH_PRESENTATION_CONTROLS_GUIWIN7STYLES

#include "GuiCommonStyles.h"

namespace vl
{
	namespace presentation
	{
		namespace win7
		{

/***********************************************************************
Button Configuration
***********************************************************************/
			
			struct Win7ButtonColors
			{
				Color										borderColor;
				Color										backgroundColor;
				Color										g1;
				Color										g2;
				Color										g3;
				Color										g4;
				Color										textColor;
				Color										bulletLight;
				Color										bulletDark;

				bool operator==(const Win7ButtonColors& colors)
				{
					return
						borderColor == colors.borderColor &&
						backgroundColor == colors.backgroundColor &&
						g1 == colors.g1 &&
						g2 == colors.g2 &&
						g3 == colors.g3 &&
						g4 == colors.g4 &&
						textColor == colors.textColor &&
						bulletLight == colors.bulletLight &&
						bulletDark == colors.bulletDark;
				}

				bool operator!=(const Win7ButtonColors& colors)
				{
					return !(*this==colors);
				}

				void										SetAlphaWithoutText(unsigned char a);

				static Win7ButtonColors						Blend(const Win7ButtonColors& c1, const Win7ButtonColors& c2, int ratio, int total);

				static Win7ButtonColors						ButtonNormal();
				static Win7ButtonColors						ButtonActive();
				static Win7ButtonColors						ButtonPressed();
				static Win7ButtonColors						ButtonDisabled();
				
				static Win7ButtonColors						ItemNormal();
				static Win7ButtonColors						ItemActive();
				static Win7ButtonColors						ItemSelected();
				static Win7ButtonColors						ItemDisabled();
				
				static Win7ButtonColors						CheckedNormal(bool selected);
				static Win7ButtonColors						CheckedActive(bool selected);
				static Win7ButtonColors						CheckedPressed(bool selected);
				static Win7ButtonColors						CheckedDisabled(bool selected);

				static Win7ButtonColors						ToolstripButtonNormal();
				static Win7ButtonColors						ToolstripButtonActive();
				static Win7ButtonColors						ToolstripButtonPressed();
				static Win7ButtonColors						ToolstripButtonDisabled();

				static Win7ButtonColors						MenuBarButtonNormal();
				static Win7ButtonColors						MenuBarButtonActive();
				static Win7ButtonColors						MenuBarButtonPressed();
				static Win7ButtonColors						MenuBarButtonDisabled();

				static Win7ButtonColors						MenuItemButtonNormal();
				static Win7ButtonColors						MenuItemButtonNormalActive();
				static Win7ButtonColors						MenuItemButtonDisabled();
				static Win7ButtonColors						MenuItemButtonDisabledActive();

				static Win7ButtonColors						TabPageHeaderNormal();
				static Win7ButtonColors						TabPageHeaderActive();
				static Win7ButtonColors						TabPageHeaderSelected();
			};

			struct Win7ButtonElements
			{
				elements::GuiSolidBorderElement*			rectBorderElement;
				elements::GuiRoundBorderElement*			roundBorderElement;
				elements::GuiSolidBackgroundElement*		backgroundElement;
				elements::GuiGradientBackgroundElement*		topGradientElement;
				elements::GuiGradientBackgroundElement*		bottomGradientElement;
				elements::GuiSolidLabelElement*				textElement;
				elements::GuiBoundsComposition*				textComposition;
				elements::GuiBoundsComposition*				mainComposition;
				elements::GuiBoundsComposition*				backgroundComposition;
				elements::GuiTableComposition*				gradientComposition;

				static Win7ButtonElements					Create(bool verticalGradient, bool roundBorder, Alignment::Type horizontal=Alignment::Center, Alignment::Type vertical=Alignment::Center);
				void										Apply(const Win7ButtonColors& colors);
			};

			struct Win7CheckedButtonElements
			{
				elements::GuiSolidBorderElement*			borderElement;
				elements::GuiSolidBackgroundElement*		backgroundElement;
				elements::GuiGradientBackgroundElement*		outerGradientElement;
				elements::GuiGradientBackgroundElement*		innerGradientElement;
				elements::GuiSolidLabelElement*				textElement;
				elements::GuiSolidLabelElement*				bulletTextElement;
				elements::GuiSolidBackgroundElement*		bulletBackgroundElement;
				elements::GuiBoundsComposition*				textComposition;
				elements::GuiBoundsComposition*				mainComposition;

				static Win7CheckedButtonElements			Create(elements::ElementShape::Type shape, bool backgroundVisible);
				void										Apply(const Win7ButtonColors& colors);
			};

			struct Win7MenuItemButtonElements
			{
				elements::GuiRoundBorderElement*			borderElement;
				elements::GuiSolidBackgroundElement*		backgroundElement;
				elements::GuiGradientBackgroundElement*		gradientElement;
				elements::Gui3DSplitterElement*				splitterElement;
				elements::GuiCellComposition*				splitterComposition;
				elements::GuiSolidLabelElement*				textElement;
				elements::GuiBoundsComposition*				textComposition;
				elements::GuiSolidLabelElement*				subMenuTextElement;
				elements::GuiGraphicsComposition*			subMenuTextComposition;
				elements::GuiBoundsComposition*				mainComposition;

				static Win7MenuItemButtonElements			Create();
				void										Apply(const Win7ButtonColors& colors);
				void										SetActive(bool value);
				void										SetSubMenuExisting(bool value);
			};

			struct Win7TextBoxColors
			{
				Color										borderColor;
				Color										backgroundColor;

				bool operator==(const Win7TextBoxColors& colors)
				{
					return
						borderColor == colors.borderColor &&
						backgroundColor == colors.backgroundColor;
				}

				bool operator!=(const Win7TextBoxColors& colors)
				{
					return !(*this==colors);
				}

				static Win7TextBoxColors					Blend(const Win7TextBoxColors& c1, const Win7TextBoxColors& c2, int ratio, int total);

				static Win7TextBoxColors					Normal();
				static Win7TextBoxColors					Active();
				static Win7TextBoxColors					Focused();
				static Win7TextBoxColors					Disabled();
			};

/***********************************************************************
Helper Functions
***********************************************************************/
			
			extern int										Win7GetColorAnimationLength();
			extern Color									Win7GetSystemWindowColor();
			extern Color									Win7GetSystemTabContentColor();
			extern Color									Win7GetSystemBorderColor();
			extern Color									Win7GetSystemTextColor(bool enabled);
			extern void										Win7SetFont(elements::GuiSolidLabelElement* element, elements::GuiBoundsComposition* composition, const FontProperties& fontProperties);
			extern void										Win7CreateSolidLabelElement(elements::GuiSolidLabelElement*& element, elements::GuiBoundsComposition*& composition, Alignment::Type horizontal, Alignment::Type vertical);

/***********************************************************************
Animation
***********************************************************************/

#define DEFINE_TRANSFERRING_ANIMATION(TSTATE, TSTYLECONTROLLER)\
				class TransferringAnimation : public elements::GuiTimeBasedAnimation\
				{\
				protected:\
					TSTATE									colorBegin;\
					TSTATE									colorEnd;\
					TSTATE									colorCurrent;\
					TSTYLECONTROLLER*						style;\
					bool									stopped;\
					bool									enableAnimation;\
				public:\
					TransferringAnimation(TSTYLECONTROLLER* _style, const TSTATE& begin);\
					void									Play(int currentPosition, int totalLength)override;\
					void									Stop()override;\
					bool									GetEnableAnimation();\
					void									SetEnableAnimation(bool value);\
					void									Transfer(const TSTATE& end);\
				};\

/***********************************************************************
Container
***********************************************************************/

			class Win7EmptyStyle : public Object, public controls::GuiControl::IStyleController
			{
			protected:
				elements::GuiBoundsComposition*				boundsComposition;
			public:
				Win7EmptyStyle(Color color);
				~Win7EmptyStyle();

				elements::GuiBoundsComposition*				GetBoundsComposition()override;
				elements::GuiGraphicsComposition*			GetContainerComposition()override;
				void										SetFocusableComposition(elements::GuiGraphicsComposition* value)override;
				void										SetText(const WString& value)override;
				void										SetFont(const FontProperties& value)override;
				void										SetVisuallyEnabled(bool value)override;
			};

			class Win7WindowStyle : public Win7EmptyStyle
			{
			public:
				Win7WindowStyle();
				~Win7WindowStyle();
			};

			class Win7MenuStyle : public Object, public controls::GuiControl::IStyleController
			{
			protected:
				elements::GuiBoundsComposition*				boundsComposition;
				elements::GuiBoundsComposition*				containerComposition;
			public:
				Win7MenuStyle();
				~Win7MenuStyle();

				elements::GuiBoundsComposition*				GetBoundsComposition()override;
				elements::GuiGraphicsComposition*			GetContainerComposition()override;
				void										SetFocusableComposition(elements::GuiGraphicsComposition* value)override;
				void										SetText(const WString& value)override;
				void										SetFont(const FontProperties& value)override;
				void										SetVisuallyEnabled(bool value)override;
			};

			class Win7MenuBarStyle : public Object, public controls::GuiControl::IStyleController
			{
			protected:
				elements::GuiBoundsComposition*				boundsComposition;
			public:
				Win7MenuBarStyle();
				~Win7MenuBarStyle();

				elements::GuiBoundsComposition*				GetBoundsComposition()override;
				elements::GuiGraphicsComposition*			GetContainerComposition()override;
				void										SetFocusableComposition(elements::GuiGraphicsComposition* value)override;
				void										SetText(const WString& value)override;
				void										SetFont(const FontProperties& value)override;
				void										SetVisuallyEnabled(bool value)override;
			};

			class Win7GroupBoxStyle : public Object, public controls::GuiControl::IStyleController
			{
			protected:
				DEFINE_TRANSFERRING_ANIMATION(Color, Win7GroupBoxStyle)

				elements::GuiBoundsComposition*				boundsComposition;
				elements::GuiBoundsComposition*				sinkBorderComposition;
				elements::GuiBoundsComposition*				raisedBorderComposition;
				elements::GuiBoundsComposition*				textComposition;
				elements::GuiBoundsComposition*				textBackgroundComposition;
				elements::GuiBoundsComposition*				containerComposition;
				elements::GuiSolidLabelElement*				textElement;
				Ptr<TransferringAnimation>					transferringAnimation;

				void										SetMargins(int fontSize);
			public:
				Win7GroupBoxStyle();
				~Win7GroupBoxStyle();

				elements::GuiBoundsComposition*				GetBoundsComposition()override;
				elements::GuiGraphicsComposition*			GetContainerComposition()override;
				void										SetFocusableComposition(elements::GuiGraphicsComposition* value)override;
				void										SetText(const WString& value)override;
				void										SetFont(const FontProperties& value)override;
				void										SetVisuallyEnabled(bool value)override;
			};

			class Win7TabStyle : public Object, public controls::GuiTab::IStyleController
			{
			protected:
				elements::GuiTableComposition*				boundsComposition;
				elements::GuiBoundsComposition*				containerComposition;
				elements::GuiStackComposition*				tabHeaderComposition;
				elements::GuiBoundsComposition*				tabContentTopLineComposition;
				FontProperties								headerFont;
				controls::GuiTab::ICommandExecutor*			commandExecutor;

				Ptr<controls::GuiSelectableButton::MutexGroupController>	headerController;
				collections::List<controls::GuiSelectableButton*>			headerButtons;

				void										OnHeaderButtonClicked(elements::GuiGraphicsComposition* sender, elements::GuiEventArgs& arguments);
				void										UpdateHeaderZOrder();
			public:
				Win7TabStyle();
				~Win7TabStyle();

				elements::GuiBoundsComposition*				GetBoundsComposition();
				elements::GuiGraphicsComposition*			GetContainerComposition();
				void										SetFocusableComposition(elements::GuiGraphicsComposition* value);
				void										SetText(const WString& value);
				void										SetFont(const FontProperties& value);
				void										SetVisuallyEnabled(bool value);

				void										SetCommandExecutor(controls::GuiTab::ICommandExecutor* value);
				void										InsertTab(int index);
				void										SetTabText(int index, const WString& value);
				void										RemoveTab(int index);
				void										SetSelectedTab(int index);
				controls::GuiControl::IStyleController*		CreateTabPageStyleController();
			};

/***********************************************************************
Button
***********************************************************************/

			class Win7ButtonStyleBase : public Object, public controls::GuiSelectableButton::IStyleController
			{
			protected:
				DEFINE_TRANSFERRING_ANIMATION(Win7ButtonColors, Win7ButtonStyleBase)

				Win7ButtonElements							elements;
				Ptr<TransferringAnimation>					transferringAnimation;
				controls::GuiButton::ControlState			controlStyle;
				bool										isVisuallyEnabled;
				bool										isSelected;
				bool										transparentWhenInactive;
				bool										transparentWhenDisabled;

				virtual void								TransferInternal(controls::GuiButton::ControlState value, bool enabled, bool selected)=0;
			public:
				Win7ButtonStyleBase(bool verticalGradient, bool roundBorder, const Win7ButtonColors& initialColor, Alignment::Type horizontal, Alignment::Type vertical);
				~Win7ButtonStyleBase();

				elements::GuiBoundsComposition*				GetBoundsComposition()override;
				elements::GuiGraphicsComposition*			GetContainerComposition()override;
				void										SetFocusableComposition(elements::GuiGraphicsComposition* value)override;
				void										SetText(const WString& value)override;
				void										SetFont(const FontProperties& value)override;
				void										SetVisuallyEnabled(bool value)override;
				void										SetSelected(bool value)override;
				void										Transfer(controls::GuiButton::ControlState value)override;

				bool										GetTransparentWhenInactive();
				void										SetTransparentWhenInactive(bool value);
				bool										GetTransparentWhenDisabled();
				void										SetTransparentWhenDisabled(bool value);
				bool										GetAutoSizeForText();
				void										SetAutoSizeForText(bool value);
			};

			class Win7ButtonStyle : public Win7ButtonStyleBase
			{
			protected:
				void										TransferInternal(controls::GuiButton::ControlState value, bool enabled, bool selected)override;
			public:
				Win7ButtonStyle(bool verticalGradient=true);
				~Win7ButtonStyle();
			};

			class Win7SelectableItemStyle : public Win7ButtonStyleBase
			{
			protected:
				void										TransferInternal(controls::GuiButton::ControlState value, bool enabled, bool selected)override;
			public:
				Win7SelectableItemStyle();
				~Win7SelectableItemStyle();
			};

			class Win7TabPageHeaderStyle : public Win7ButtonStyleBase
			{
			protected:
				void										TransferInternal(controls::GuiButton::ControlState value, bool enabled, bool selected)override;
			public:
				Win7TabPageHeaderStyle();
				~Win7TabPageHeaderStyle();

				void										SetFont(const FontProperties& value)override;
			};

			class Win7ToolstripButtonStyle : public Win7ButtonStyleBase
			{
			protected:
				void										TransferInternal(controls::GuiButton::ControlState value, bool enabled, bool selected)override;
			public:
				Win7ToolstripButtonStyle(bool transparent);
				~Win7ToolstripButtonStyle();
			};

			class Win7CheckBoxStyle : public Object, public controls::GuiSelectableButton::IStyleController
			{
			public:
				enum BulletStyle
				{
					CheckBox,
					RadioButton,
				};
			protected:
				DEFINE_TRANSFERRING_ANIMATION(Win7ButtonColors, Win7CheckBoxStyle)

				Win7CheckedButtonElements					elements;
				Ptr<TransferringAnimation>					transferringAnimation;
				controls::GuiButton::ControlState			controlStyle;
				bool										isVisuallyEnabled;
				bool										isSelected;

				void										TransferInternal(controls::GuiButton::ControlState value, bool enabled, bool selected);
			public:
				Win7CheckBoxStyle(BulletStyle bulletStyle, bool backgroundVisible=true);
				~Win7CheckBoxStyle();

				elements::GuiBoundsComposition*				GetBoundsComposition()override;
				elements::GuiGraphicsComposition*			GetContainerComposition()override;
				void										SetFocusableComposition(elements::GuiGraphicsComposition* value)override;
				void										SetText(const WString& value)override;
				void										SetFont(const FontProperties& value)override;
				void										SetVisuallyEnabled(bool value)override;
				void										SetSelected(bool value)override;
				void										Transfer(controls::GuiButton::ControlState value)override;
			};

/***********************************************************************
MenuButton
***********************************************************************/

			class Win7MenuBarButtonStyle : public Object, public controls::GuiMenuButton::IStyleController
			{
			protected:
				Win7ButtonElements							elements;
				controls::GuiButton::ControlState			controlStyle;
				bool										isVisuallyEnabled;
				bool										isOpening;

				void										TransferInternal(controls::GuiButton::ControlState value, bool enabled, bool opening);
			public:
				Win7MenuBarButtonStyle();
				~Win7MenuBarButtonStyle();

				elements::GuiBoundsComposition*				GetBoundsComposition()override;
				elements::GuiGraphicsComposition*			GetContainerComposition()override;
				void										SetFocusableComposition(elements::GuiGraphicsComposition* value)override;
				void										SetText(const WString& value)override;
				void										SetFont(const FontProperties& value)override;
				void										SetVisuallyEnabled(bool value)override;
				controls::GuiMenu::IStyleController*		CreateSubMenuStyleController()override;
				void										SetSubMenuExisting(bool value)override;
				void										SetSubMenuOpening(bool value)override;
				void										Transfer(controls::GuiButton::ControlState value)override;
			};

			class Win7MenuItemButtonStyle : public Object, public controls::GuiMenuButton::IStyleController
			{
			protected:
				Win7MenuItemButtonElements					elements;
				controls::GuiButton::ControlState			controlStyle;
				bool										isVisuallyEnabled;
				bool										isOpening;

				void										TransferInternal(controls::GuiButton::ControlState value, bool enabled, bool opening);
			public:
				Win7MenuItemButtonStyle();
				~Win7MenuItemButtonStyle();

				elements::GuiBoundsComposition*				GetBoundsComposition()override;
				elements::GuiGraphicsComposition*			GetContainerComposition()override;
				void										SetFocusableComposition(elements::GuiGraphicsComposition* value)override;
				void										SetText(const WString& value)override;
				void										SetFont(const FontProperties& value)override;
				void										SetVisuallyEnabled(bool value)override;
				controls::GuiMenu::IStyleController*		CreateSubMenuStyleController()override;
				void										SetSubMenuExisting(bool value)override;
				void										SetSubMenuOpening(bool value)override;
				void										Transfer(controls::GuiButton::ControlState value)override;
			};

			class Win7MenuSplitterStyle : public Object, public controls::GuiControl::IStyleController
			{
			protected:
				elements::GuiBoundsComposition*				boundsComposition;
			public:
				Win7MenuSplitterStyle();
				~Win7MenuSplitterStyle();

				elements::GuiBoundsComposition*				GetBoundsComposition()override;
				elements::GuiGraphicsComposition*			GetContainerComposition()override;
				void										SetFocusableComposition(elements::GuiGraphicsComposition* value)override;
				void										SetText(const WString& value)override;
				void										SetFont(const FontProperties& value)override;
				void										SetVisuallyEnabled(bool value)override;
			};

/***********************************************************************
Scroll
***********************************************************************/

			class Win7ScrollStyle : public common_styles::CommonScrollStyle
			{
			public:
				static const int							DefaultSize=18;
				static const int							ArrowSize=10;
			protected:
				controls::GuiButton::IStyleController*		CreateDecreaseButtonStyle(Direction direction);
				controls::GuiButton::IStyleController*		CreateIncreaseButtonStyle(Direction direction);
				controls::GuiButton::IStyleController*		CreateHandleButtonStyle(Direction direction);
				void										InstallBackground(elements::GuiGraphicsComposition* boundsComposition, Direction direction)override;
			public:
				Win7ScrollStyle(Direction _direction);
				~Win7ScrollStyle();
			};

			class Win7TrackStyle : public common_styles::CommonTrackStyle
			{
			public:
				static const int							TrackThickness=4;
				static const int							TrackPadding=8;
				static const int							HandleLong=21;
				static const int							HandleShort=10;

			protected:
				controls::GuiButton::IStyleController*		CreateHandleButtonStyle(Direction direction)override;
				void										InstallBackground(elements::GuiGraphicsComposition* boundsComposition, Direction direction)override;
				void										InstallTrack(elements::GuiGraphicsComposition* trackComposition, Direction direction)override;
			public:
				Win7TrackStyle(Direction _direction);
				~Win7TrackStyle();
			};

/***********************************************************************
ScrollView
***********************************************************************/

			class Win7ScrollViewProvider : public Object, public controls::GuiScrollView::IStyleProvider
			{
			public:
				void										AssociateStyleController(controls::GuiControl::IStyleController* controller)override;
				void										SetFocusableComposition(elements::GuiGraphicsComposition* value)override;
				void										SetText(const WString& value)override;
				void										SetFont(const FontProperties& value)override;
				void										SetVisuallyEnabled(bool value)override;

				controls::GuiScroll::IStyleController*		CreateHorizontalScrollStyle()override;
				controls::GuiScroll::IStyleController*		CreateVerticalScrollStyle()override;
				int											GetDefaultScrollSize()override;
				elements::GuiGraphicsComposition*			InstallBackground(elements::GuiBoundsComposition* boundsComposition)override;
			};

/***********************************************************************
TextBox
***********************************************************************/

			class Win7TextBoxBackground : public Object
			{
			protected:
				DEFINE_TRANSFERRING_ANIMATION(Win7TextBoxColors, Win7TextBoxBackground)
					
				elements::GuiRoundBorderElement*			borderElement;
				elements::GuiSolidBackgroundElement*		backgroundElement;
				elements::GuiGraphicsComposition*			focusableComposition;
				bool										isMouseEnter;
				bool										isFocused;
				bool										isVisuallyEnabled;
				Ptr<TransferringAnimation>					transferringAnimation;
				controls::GuiControl::IStyleController*		styleController;
				elements::GuiColorizedTextElement*			textElement;

				void										UpdateStyle();
				void										Apply(const Win7TextBoxColors& colors);

				void										OnBoundsMouseEnter(elements::GuiGraphicsComposition* sender, elements::GuiEventArgs& arguments);
				void										OnBoundsMouseLeave(elements::GuiGraphicsComposition* sender, elements::GuiEventArgs& arguments);
				void										OnBoundsGotFocus(elements::GuiGraphicsComposition* sender, elements::GuiEventArgs& arguments);
				void										OnBoundsLostFocus(elements::GuiGraphicsComposition* sender, elements::GuiEventArgs& arguments);
			public:
				Win7TextBoxBackground();
				~Win7TextBoxBackground();
				
				void										AssociateStyleController(controls::GuiControl::IStyleController* controller);
				void										SetFocusableComposition(elements::GuiGraphicsComposition* value);
				void										SetVisuallyEnabled(bool value);
				elements::GuiGraphicsComposition*			InstallBackground(elements::GuiBoundsComposition* boundsComposition);
				void										InitializeTextElement(elements::GuiColorizedTextElement* _textElement);
			};

			class Win7MultilineTextBoxProvider : public Win7ScrollViewProvider
			{
			protected:
				Win7TextBoxBackground						background;
				controls::GuiControl::IStyleController*		styleController;
			public:
				Win7MultilineTextBoxProvider();
				~Win7MultilineTextBoxProvider();
				
				void										AssociateStyleController(controls::GuiControl::IStyleController* controller)override;
				void										SetFocusableComposition(elements::GuiGraphicsComposition* value)override;
				void										SetVisuallyEnabled(bool value)override;
				elements::GuiGraphicsComposition*			InstallBackground(elements::GuiBoundsComposition* boundsComposition)override;
			};

			class Win7SinglelineTextBoxProvider : public Object, public controls::GuiSinglelineTextBox::IStyleProvider
			{
			protected:
				Win7TextBoxBackground						background;
				controls::GuiControl::IStyleController*		styleController;
			public:
				Win7SinglelineTextBoxProvider();
				~Win7SinglelineTextBoxProvider();

				void										AssociateStyleController(controls::GuiControl::IStyleController* controller)override;
				void										SetFocusableComposition(elements::GuiGraphicsComposition* value)override;
				void										SetText(const WString& value)override;
				void										SetFont(const FontProperties& value)override;
				void										SetVisuallyEnabled(bool value)override;
				elements::GuiGraphicsComposition*			InstallBackground(elements::GuiBoundsComposition* boundsComposition)override;
			};

/***********************************************************************
List
***********************************************************************/

			class Win7TextListProvider : public Object, public controls::list::TextItemStyleProvider::ITextItemStyleProvider
			{
			public:
				Win7TextListProvider();
				~Win7TextListProvider();

				controls::GuiSelectableButton::IStyleController*		CreateBackgroundStyleController()override;
				controls::GuiSelectableButton::IStyleController*		CreateBulletStyleController()override;
			};

			class Win7CheckTextListProvider : public Win7TextListProvider
			{
			public:
				Win7CheckTextListProvider();
				~Win7CheckTextListProvider();

				controls::GuiSelectableButton::IStyleController*		CreateBulletStyleController()override;
			};

			class Win7RadioTextListProvider : public Win7TextListProvider
			{
			public:
				Win7RadioTextListProvider();
				~Win7RadioTextListProvider();

				controls::GuiSelectableButton::IStyleController*		CreateBulletStyleController()override;
			};
		}
	}
}

#endif