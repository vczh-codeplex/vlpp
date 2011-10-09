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

				static Win7ItemColors					Blend(const Win7ItemColors c1, const Win7ItemColors c2, int ratio, int total);

				static Win7ItemColors					Normal();
				static Win7ItemColors					ItemActive();
				static Win7ItemColors					ItemSelected();
				static Win7ItemColors					ButtonActive();
				static Win7ItemColors					ButtonPressed();
				static Win7ItemColors					Disabled();
			};

			struct Win7ButtonElements
			{
				GuiRoundBorderElement*					borderElement;
				GuiSolidBackgroundElement*				backgroundElement;
				GuiGradientBackgroundElement*			topGradientElement;
				GuiGradientBackgroundElement*			bottomGradientElement;
				GuiSolidLabelElement*					textElement;
				GuiBoundsComposition*					mainComposition;

				static Win7ButtonElements				Create();
				void									Apply(Win7ItemColors colors);
			};

/***********************************************************************
Button
***********************************************************************/

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
			public:
				Win7ButtonStyle();
				~Win7ButtonStyle();

				elements::GuiBoundsComposition*				GetBoundsComposition();
				elements::GuiGraphicsComposition*			GetContainerComposition();
				void										Transfer(GuiButton::ControlStyle value);
				void										SetText(const WString& value);
			};
		}
	}
}

#endif