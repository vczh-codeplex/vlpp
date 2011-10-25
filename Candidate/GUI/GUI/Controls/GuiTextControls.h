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
			public:
				class ICallback : public Interface
				{
				public:
					virtual TextPos						GetLeftWord(TextPos pos)=0;
					virtual TextPos						GetRightWord(TextPos pos)=0;
					virtual void						GetWord(TextPos pos, TextPos& begin, TextPos& end)=0;
					virtual int							GetPageRows()=0;
					virtual bool						BeforeModify(TextPos& start, TextPos& end, const WString& originalText, WString& inputText)=0;
					virtual void						AfterModify(TextPos originalStart, TextPos originalEnd, const WString& originalText, TextPos inputStart, TextPos inputEnd, const WString& inputText)=0;
					virtual void						ScrollToView(Point point)=0;
					virtual int							GetTextMargin()=0;
				};

				class DefaultCallback : public Object, public ICallback
				{
				protected:
					elements::GuiColorizedTextElement*	textElement;
					elements::GuiBoundsComposition*		textComposition;
					bool								readonly;
				public:
					DefaultCallback(elements::GuiColorizedTextElement* _textElement, elements::GuiBoundsComposition* _textComposition);
					~DefaultCallback();

					TextPos								GetLeftWord(TextPos pos);
					TextPos								GetRightWord(TextPos pos);
					void								GetWord(TextPos pos, TextPos& begin, TextPos& end);
					int									GetPageRows();
					bool								BeforeModify(TextPos& start, TextPos& end, const WString& originalText, WString& inputText);
				};
			protected:
				elements::GuiColorizedTextElement*		textElement;
				elements::GuiBoundsComposition*			textComposition;
				GuiControl*								textControl;
				ICallback*								callback;
				bool									dragging;

				void									UpdateCaretPoint();
				void									Move(TextPos pos, bool shift);
				void									Modify(TextPos start, TextPos end, const WString& input);
				void									ProcessKey(int code, bool shift, bool ctrl);
					
				void									OnGotFocus(elements::GuiGraphicsComposition* sender, elements::GuiEventArgs& arguments);
				void									OnLostFocus(elements::GuiGraphicsComposition* sender, elements::GuiEventArgs& arguments);
				void									OnCaretNotify(elements::GuiGraphicsComposition* sender, elements::GuiEventArgs& arguments);

				void									OnLeftButtonDown(elements::GuiGraphicsComposition* sender, elements::GuiMouseEventArgs& arguments);
				void									OnLeftButtonUp(elements::GuiGraphicsComposition* sender, elements::GuiMouseEventArgs& arguments);
				void									OnMouseMove(elements::GuiGraphicsComposition* sender, elements::GuiMouseEventArgs& arguments);
				void									OnKeyDown(elements::GuiGraphicsComposition* sender, elements::GuiKeyEventArgs& arguments);
				void									OnCharInput(elements::GuiGraphicsComposition* sender, elements::GuiCharEventArgs& arguments);
			public:
				GuiTextElementOperator();
				~GuiTextElementOperator();

				void									Install(elements::GuiColorizedTextElement* _textElement, elements::GuiBoundsComposition* _textComposition, GuiControl* _textControl);
				ICallback*								GetCallback();
				void									SetCallback(ICallback* value);
				TextPos									GetNearestTextPos(Point point);

				WString									GetSelectionText();
				void									SetSelectionText(const WString& value);

				bool									CanCut();
				bool									CanCopy();
				bool									CanPaste();
				void									SelectAll();
				bool									Cut();
				bool									Copy();
				bool									Paste();
			};

			class GuiMultilineTextBox : public GuiScrollView
			{
			public:
				static const int						TextMargin=3;

				class StyleController : public GuiScrollView::StyleController
				{
				protected:
					elements::GuiColorizedTextElement*		textElement;
					elements::GuiBoundsComposition*			textComposition;
					GuiTextElementOperator					textElementOperator;
					Ptr<GuiTextElementOperator::ICallback>	defaultCallback;

				public:
					StyleController(GuiScrollView::IStyleProvider* styleProvider);
					~StyleController();

					elements::GuiColorizedTextElement*	GetTextElement();
					elements::GuiBoundsComposition*		GetTextComposition();
					GuiTextElementOperator*				GetTextElementOperator();
					void								SetViewPosition(Point value);
					void								SetFocusableComposition(elements::GuiGraphicsComposition* value);

					WString								GetText();
					void								SetText(const WString& value);
					void								SetFont(const FontProperties& value);
					void								SetVisuallyEnabled(bool value);
				};

				class DefaultTextElementOperatorCallback : public GuiTextElementOperator::DefaultCallback
				{
				protected:
					GuiMultilineTextBox*				textControl;
					StyleController*					textController;
				public:
					DefaultTextElementOperatorCallback(GuiMultilineTextBox* _textControl);

					void								AfterModify(TextPos originalStart, TextPos originalEnd, const WString& originalText, TextPos inputStart, TextPos inputEnd, const WString& inputText);
					void								ScrollToView(Point point);
					int									GetTextMargin();
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