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
			class GuiMultilineTextBox : public GuiScrollView
			{
			public:
				static const int						TextMargin=3;

				class StyleController : public GuiScrollView::StyleController
				{
				protected:
					elements::GuiColorizedTextElement*	textElement;
					elements::GuiBoundsComposition*		textComposition;
					bool								dragging;
					
					void								OnGotFocus(elements::GuiGraphicsComposition* sender, elements::GuiEventArgs& arguments);
					void								OnLostFocus(elements::GuiGraphicsComposition* sender, elements::GuiEventArgs& arguments);
					void								OnCaretNotify(elements::GuiGraphicsComposition* sender, elements::GuiEventArgs& arguments);

					void								OnLeftButtonDown(elements::GuiGraphicsComposition* sender, elements::GuiMouseEventArgs& arguments);
					void								OnLeftButtonUp(elements::GuiGraphicsComposition* sender, elements::GuiMouseEventArgs& arguments);
					void								OnMouseMove(elements::GuiGraphicsComposition* sender, elements::GuiMouseEventArgs& arguments);
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

				void									OnRenderTargetChanged(elements::IGuiGraphicsRenderTarget* renderTarget);
				Size									QueryFullSize();
				void									UpdateView(Rect viewBounds);
				void									OnBoundsMouseButtonDown(elements::GuiGraphicsComposition* sender, elements::GuiMouseEventArgs& arguments);
			public:
				GuiMultilineTextBox(GuiScrollView::IStyleProvider* styleProvider);
				~GuiMultilineTextBox();

				elements::GuiGraphicsComposition*		GetFocusableComposition();

				const WString&							GetText();
				void									SetText(const WString& value);
			};
		}
	}
}

#endif