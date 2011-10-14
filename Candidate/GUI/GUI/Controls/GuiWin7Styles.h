/***********************************************************************
Vczh Library++ 3.0
Developer: ³Âè÷å«(vczh)
GUI::Windows Platform

Interfaces:
***********************************************************************/

#ifndef VCZH_PRESENTATION_CONTROLS_GUIWIN7STYLES
#define VCZH_PRESENTATION_CONTROLS_GUIWIN7STYLES

#include "GuiBasicControls.h"

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

				static Win7ButtonColors						Blend(const Win7ButtonColors c1, const Win7ButtonColors c2, int ratio, int total);

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

				static Win7ButtonElements					Create();
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
				elements::GuiBoundsComposition*				textComposition;
				elements::GuiBoundsComposition*				mainComposition;

				static Win7CheckedButtonElements			Create(elements::ElementShape::Type shape);
				void										Apply(const Win7ButtonColors& colors);
			};

/***********************************************************************
Helpers
***********************************************************************/
			
			extern int										Win7GetColorAnimationLength();
			extern Color									Win7GetSystemWindowColor();
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
Button
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
				void										SetText(const WString& value);
				void										SetFont(const FontProperties& value);
				void										SetVisuallyEnabled(bool value);
			};

			class Win7ButtonStyle : public Object, public controls::GuiButton::IStyleController
			{
			protected:
				DEFINE_TRANSFERRING_ANIMATION(Win7ButtonColors, Win7ButtonStyle)

				Win7ButtonElements							elements;
				Ptr<TransferringAnimation>					transferringAnimation;
				controls::GuiButton::ControlState			controlStyle;
				bool										isVisuallyEnabled;

				void										TransferInternal(controls::GuiButton::ControlState value, bool enabled);
			public:
				Win7ButtonStyle();
				~Win7ButtonStyle();

				elements::GuiBoundsComposition*				GetBoundsComposition();
				elements::GuiGraphicsComposition*			GetContainerComposition();
				void										SetText(const WString& value);
				void										SetFont(const FontProperties& value);
				void										SetVisuallyEnabled(bool value);
				void										Transfer(controls::GuiButton::ControlState value);
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
				void										SetText(const WString& value);
				void										SetFont(const FontProperties& value);
				void										SetVisuallyEnabled(bool value);
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
				void										SetText(const WString& value);
				void										SetFont(const FontProperties& value);
				void										SetVisuallyEnabled(bool value);
				void										SetSelected(bool value);
				void										Transfer(controls::GuiButton::ControlState value);
			};
		}
	}
}

#endif