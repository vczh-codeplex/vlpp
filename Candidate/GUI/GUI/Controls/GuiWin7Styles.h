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
			using namespace elements;
			using namespace controls;

/***********************************************************************
Common Configurations
***********************************************************************/
			
			struct Win7ItemColors
			{
				Color									borderColor;
				Color									backgroundColor;
				Color									g1;
				Color									g2;
				Color									g3;
				Color									g4;
				Color									textColor;

				bool operator==(const Win7ItemColors& colors)
				{
					return
						borderColor == colors.borderColor &&
						backgroundColor == colors.backgroundColor &&
						g1 == colors.g1 &&
						g2 == colors.g2 &&
						g3 == colors.g3 &&
						g4 == colors.g4 &&
						textColor == colors.textColor;
				}

				bool operator!=(const Win7ItemColors& colors)
				{
					return !(*this==colors);
				}

				static Win7ItemColors					Blend(const Win7ItemColors c1, const Win7ItemColors c2, int ratio, int total);

				static Win7ItemColors					Normal();
				static Win7ItemColors					ItemActive();
				static Win7ItemColors					ItemSelected();
				static Win7ItemColors					ButtonActive();
				static Win7ItemColors					ButtonPressed();
				static Win7ItemColors					Disabled();

				static int								GetColorAnimationLength();
				static Color							GetSystemWindowColor();
			};

			struct Win7ButtonElements
			{
				GuiRoundBorderElement*					borderElement;
				GuiSolidBackgroundElement*				backgroundElement;
				GuiGradientBackgroundElement*			topGradientElement;
				GuiGradientBackgroundElement*			bottomGradientElement;
				GuiSolidLabelElement*					textElement;
				GuiBoundsComposition*					textComposition;
				GuiBoundsComposition*					mainComposition;

				static Win7ButtonElements				Create();
				void									Apply(Win7ItemColors colors);
			};

			extern void									Win7SetFont(GuiSolidLabelElement* element, GuiBoundsComposition* composition, const FontProperties& fontProperties);
			extern void									Win7CreateSolidLabelElement(GuiSolidLabelElement*& element, GuiBoundsComposition*& composition);

/***********************************************************************
Button
***********************************************************************/

			class Win7WindowStyle : public Object, public GuiControl::IStyleController
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

			class Win7ButtonStyle : public Object, public GuiButton::IStyleController
			{
			protected:
				class TransferringAnimation : public GuiTimeBasedAnimation
				{
				protected:
					Win7ItemColors							colorBegin;
					Win7ItemColors							colorEnd;
					Win7ItemColors							colorCurrent;
					Win7ButtonStyle*						style;
					bool									stopped;
				public:
					TransferringAnimation(Win7ButtonStyle* _style, const Win7ItemColors& begin);

					void									Play(int currentPosition, int totalLength);
					void									Stop();
					void									Transfer(const Win7ItemColors& end);
				};

				Win7ButtonElements							elements;
				Ptr<TransferringAnimation>					transferringAnimation;
				GuiButton::ControlStyle						controlStyle;
				bool										isVisuallyEnabled;

				void										TransferInternal(GuiButton::ControlStyle value, bool enabled);
			public:
				Win7ButtonStyle();
				~Win7ButtonStyle();

				elements::GuiBoundsComposition*				GetBoundsComposition();
				elements::GuiGraphicsComposition*			GetContainerComposition();
				void										SetText(const WString& value);
				void										SetFont(const FontProperties& value);
				void										SetVisuallyEnabled(bool value);
				void										Transfer(GuiButton::ControlStyle value);
			};

			class Win7GroupBoxStyle : public Object, public GuiControl::IStyleController
			{
			protected:
				class TransferringAnimation : public GuiTimeBasedAnimation
				{
				protected:
					Color									colorBegin;
					Color									colorEnd;
					Color									colorCurrent;
					Win7GroupBoxStyle*						style;
					bool									stopped;
				public:
					TransferringAnimation(Win7GroupBoxStyle* _style, const Color& begin);

					void									Play(int currentPosition, int totalLength);
					void									Stop();
					void									Transfer(const Color& end);
				};

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
		}
	}
}

#endif