/***********************************************************************
Vczh Library++ 3.0
Developer: ³Âè÷å«(vczh)
GUI::Windows Platform

Interfaces:
***********************************************************************/

#ifndef VCZH_PRESENTATION_CONTROLS_GUITEXTCONTROLS
#define VCZH_PRESENTATION_CONTROLS_GUITEXTCONTROLS

#include "GuiBasicControls.h"

namespace vl
{
	namespace presentation
	{
		namespace controls
		{
			class GuiTextElementOperator : public Object
			{
			protected:
				elements::GuiColorizedTextElement*		textElement;
				elements::GuiBoundsComposition*			textComposition;
				GuiControl*								textControl;
				bool									dragging;
					
				void									OnGotFocus(elements::GuiGraphicsComposition* sender, elements::GuiEventArgs& arguments);
				void									OnLostFocus(elements::GuiGraphicsComposition* sender, elements::GuiEventArgs& arguments);
				void									OnCaretNotify(elements::GuiGraphicsComposition* sender, elements::GuiEventArgs& arguments);

				void									OnLeftButtonDown(elements::GuiGraphicsComposition* sender, elements::GuiMouseEventArgs& arguments);
				void									OnLeftButtonUp(elements::GuiGraphicsComposition* sender, elements::GuiMouseEventArgs& arguments);
				void									OnMouseMove(elements::GuiGraphicsComposition* sender, elements::GuiMouseEventArgs& arguments);
			public:
				GuiTextElementOperator();
				~GuiTextElementOperator();

				void									Install(elements::GuiColorizedTextElement* _textElement, elements::GuiBoundsComposition* _textComposition, GuiControl* _textControl);
				TextPos									GetNearestTextPos(Point point);
			};

			class GuiMultilineTextBox : public GuiScrollView
			{
			public:
				static const int						TextMargin=3;

				class StyleController : public GuiScrollView::StyleController
				{
				protected:
					elements::GuiColorizedTextElement*	textElement;
					elements::GuiBoundsComposition*		textComposition;
					GuiTextElementOperator				textElementOperator;

				public:
					StyleController(GuiScrollView::IStyleProvider* styleProvider);
					~StyleController();

					elements::GuiColorizedTextElement*	GetTextElement();
					void								SetViewPosition(Point value);
					WString								GetText();

					void								SetFocusableComposition(elements::GuiGraphicsComposition* value);
					void								SetText(const WString& value);
					void								SetFont(const FontProperties& value);
					void								SetVisuallyEnabled(bool value);
				};

			protected:
				StyleController*						styleController;

				void									CalculateViewAndSetScroll();
				void									OnRenderTargetChanged(elements::IGuiGraphicsRenderTarget* renderTarget);
				Size									QueryFullSize();
				void									UpdateView(Rect viewBounds);
				void									OnBoundsMouseButtonDown(elements::GuiGraphicsComposition* sender, elements::GuiMouseEventArgs& arguments);
			public:
				GuiMultilineTextBox(GuiScrollView::IStyleProvider* styleProvider);
				~GuiMultilineTextBox();

				const WString&							GetText();
				void									SetText(const WString& value);
				void									SetFont(const FontProperties& value);
			};
		}
	}
}

#endif