/***********************************************************************
Vczh Library++ 3.0
Developer: ³Âè÷å«(vczh)
GacUI::Control Styles::Windows7 Styles

Interfaces:
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

				static Win7ButtonColors						Normal();
				static Win7ButtonColors						ItemActive();
				static Win7ButtonColors						ItemSelected();
				static Win7ButtonColors						ButtonActive();
				static Win7ButtonColors						ButtonPressed();
				static Win7ButtonColors						Disabled();
				
				static Win7ButtonColors						CheckedNormal(bool selected);
				static Win7ButtonColors						CheckedActive(bool selected);
				static Win7ButtonColors						CheckedPressed(bool selected);
				static Win7ButtonColors						CheckedDisabled(bool selected);
			};

			struct Win7ButtonElements
			{
				elements::GuiRoundBorderElement*			borderElement;
				elements::GuiSolidBackgroundElement*		backgroundElement;
				elements::GuiGradientBackgroundElement*		topGradientElement;
				elements::GuiGradientBackgroundElement*		bottomGradientElement;
				elements::GuiSolidLabelElement*				textElement;
				elements::GuiBoundsComposition*				textComposition;
				elements::GuiBoundsComposition*				mainComposition;

				static Win7ButtonElements					Create(bool verticalGradient);
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

				static Win7CheckedButtonElements			Create(elements::ElementShape::Type shape);
				void										Apply(const Win7ButtonColors& colors);
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
Helpers
***********************************************************************/
			
			extern int										Win7GetColorAnimationLength();
			extern Color									Win7GetSystemWindowColor();
			extern Color									Win7GetSystemBorderColor();
			extern Color									Win7GetSystemTextColor(bool enabled);
			extern void										Win7SetFont(elements::GuiSolidLabelElement* element, elements::GuiBoundsComposition* composition, const FontProperties& fontProperties);
			extern void										Win7CreateSolidLabelElement(elements::GuiSolidLabelElement*& element, elements::GuiBoundsComposition*& composition);

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
				public:\
					TransferringAnimation(TSTYLECONTROLLER* _style, const TSTATE& begin);\
					void									Play(int currentPosition, int totalLength);\
					void									Stop();\
					void									Transfer(const TSTATE& end);\
				};\

/***********************************************************************
Containers
***********************************************************************/

			class Win7WindowStyle : public Object, public controls::GuiControl::IStyleController
			{
			protected:
				elements::GuiBoundsComposition*				boundsComposition;
			public:
				Win7WindowStyle();
				~Win7WindowStyle();

				elements::GuiBoundsComposition*				GetBoundsComposition();
				elements::GuiGraphicsComposition*			GetContainerComposition();
				void										SetFocusableComposition(elements::GuiGraphicsComposition* value);
				void										SetText(const WString& value);
				void										SetFont(const FontProperties& value);
				void										SetVisuallyEnabled(bool value);
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

				elements::GuiBoundsComposition*				GetBoundsComposition();
				elements::GuiGraphicsComposition*			GetContainerComposition();
				void										SetFocusableComposition(elements::GuiGraphicsComposition* value);
				void										SetText(const WString& value);
				void										SetFont(const FontProperties& value);
				void										SetVisuallyEnabled(bool value);
			};

/***********************************************************************
Buttons
***********************************************************************/

			class Win7ButtonStyle : public Object, public controls::GuiButton::IStyleController
			{
			protected:
				DEFINE_TRANSFERRING_ANIMATION(Win7ButtonColors, Win7ButtonStyle)

				Win7ButtonElements							elements;
				Ptr<TransferringAnimation>					transferringAnimation;
				controls::GuiButton::ControlState			controlStyle;
				bool										isVisuallyEnabled;
				bool										transparentWhenInactive;
				bool										transparentWhenDisabled;

				void										TransferInternal(controls::GuiButton::ControlState value, bool enabled);
			public:
				Win7ButtonStyle(bool verticalGradient=true);
				~Win7ButtonStyle();

				elements::GuiBoundsComposition*				GetBoundsComposition();
				elements::GuiGraphicsComposition*			GetContainerComposition();
				void										SetFocusableComposition(elements::GuiGraphicsComposition* value);
				void										SetText(const WString& value);
				void										SetFont(const FontProperties& value);
				void										SetVisuallyEnabled(bool value);
				void										Transfer(controls::GuiButton::ControlState value);

				bool										GetTransparentWhenInactive();
				void										SetTransparentWhenInactive(bool value);
				bool										GetTransparentWhenDisabled();
				void										SetTransparentWhenDisabled(bool value);
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
				Win7CheckBoxStyle(BulletStyle bulletStyle);
				~Win7CheckBoxStyle();

				elements::GuiBoundsComposition*				GetBoundsComposition();
				elements::GuiGraphicsComposition*			GetContainerComposition();
				void										SetFocusableComposition(elements::GuiGraphicsComposition* value);
				void										SetText(const WString& value);
				void										SetFont(const FontProperties& value);
				void										SetVisuallyEnabled(bool value);
				void										SetSelected(bool value);
				void										Transfer(controls::GuiButton::ControlState value);
			};

/***********************************************************************
Scrolls
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
				void										InstallBackground(elements::GuiGraphicsComposition* boundsComposition, Direction direction);
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
				controls::GuiButton::IStyleController*		CreateHandleButtonStyle(Direction direction);
				void										InstallBackground(elements::GuiGraphicsComposition* boundsComposition, Direction direction);
				void										InstallTrack(elements::GuiGraphicsComposition* trackComposition, Direction direction);
			public:
				Win7TrackStyle(Direction _direction);
				~Win7TrackStyle();
			};

			class Win7ScrollViewProvider : public Object, public controls::GuiScrollView::IStyleProvider
			{
			public:
				void										AssociateStyleController(controls::GuiControl::IStyleController* controller);
				void										SetFocusableComposition(elements::GuiGraphicsComposition* value);
				void										SetText(const WString& value);
				void										SetFont(const FontProperties& value);
				void										SetVisuallyEnabled(bool value);

				controls::GuiScroll::IStyleController*		CreateHorizontalScrollStyle();
				controls::GuiScroll::IStyleController*		CreateVerticalScrollStyle();
				int											GetDefaultScrollSize();
				elements::GuiGraphicsComposition*			InstallBackground(elements::GuiBoundsComposition* boundsComposition);
			};

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
				
				void										AssociateStyleController(controls::GuiControl::IStyleController* controller);
				void										SetFocusableComposition(elements::GuiGraphicsComposition* value);
				void										SetVisuallyEnabled(bool value);
				elements::GuiGraphicsComposition*			InstallBackground(elements::GuiBoundsComposition* boundsComposition);
			};

			class Win7SinglelineTextBoxProvider : public Object, public controls::GuiSinglelineTextBox::IStyleProvider
			{
			protected:
				Win7TextBoxBackground						background;
				controls::GuiControl::IStyleController*		styleController;
			public:
				Win7SinglelineTextBoxProvider();
				~Win7SinglelineTextBoxProvider();

				void										AssociateStyleController(controls::GuiControl::IStyleController* controller);
				void										SetFocusableComposition(elements::GuiGraphicsComposition* value);
				void										SetText(const WString& value);
				void										SetFont(const FontProperties& value);
				void										SetVisuallyEnabled(bool value);
				elements::GuiGraphicsComposition*			InstallBackground(elements::GuiBoundsComposition* boundsComposition);
			};
		}
	}
}

#endif