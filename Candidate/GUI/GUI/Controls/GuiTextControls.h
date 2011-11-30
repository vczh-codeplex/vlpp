/***********************************************************************
Vczh Library++ 3.0
Developer: ³Âè÷å«(vczh)
GacUI::Control System

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

/***********************************************************************
Common Operations
***********************************************************************/

			class GuiTextBoxCommonInterface;

			class GuiTextElementOperator : public Object
			{
			public:
				class ICallback : public Interface
				{
				public:
					virtual TextPos							GetLeftWord(TextPos pos)=0;
					virtual TextPos							GetRightWord(TextPos pos)=0;
					virtual void							GetWord(TextPos pos, TextPos& begin, TextPos& end)=0;
					virtual int								GetPageRows()=0;
					virtual bool							BeforeModify(TextPos& start, TextPos& end, const WString& originalText, WString& inputText)=0;
					virtual void							AfterModify(TextPos originalStart, TextPos originalEnd, const WString& originalText, TextPos inputStart, TextPos inputEnd, const WString& inputText)=0;
					virtual void							ScrollToView(Point point)=0;
					virtual int								GetTextMargin()=0;
				};

				class DefaultCallback : public Object, public ICallback
				{
				protected:
					elements::GuiColorizedTextElement*		textElement;
					elements::GuiGraphicsComposition*		textComposition;
					bool									readonly;
				public:
					DefaultCallback(elements::GuiColorizedTextElement* _textElement, elements::GuiGraphicsComposition* _textComposition);
					~DefaultCallback();

					TextPos									GetLeftWord(TextPos pos)override;
					TextPos									GetRightWord(TextPos pos)override;
					void									GetWord(TextPos pos, TextPos& begin, TextPos& end)override;
					int										GetPageRows()override;
					bool									BeforeModify(TextPos& start, TextPos& end, const WString& originalText, WString& inputText)override;
				};
			protected:
				elements::GuiColorizedTextElement*			textElement;
				elements::GuiGraphicsComposition*			textComposition;
				GuiControl*									textControl;
				GuiTextBoxCommonInterface*					textBoxCommonInterface;
				ICallback*									callback;
				bool										dragging;

				void										UpdateCaretPoint();
				void										Move(TextPos pos, bool shift);
				void										Modify(TextPos start, TextPos end, const WString& input);
				void										ProcessKey(int code, bool shift, bool ctrl);
					
				void										OnGotFocus(elements::GuiGraphicsComposition* sender, elements::GuiEventArgs& arguments);
				void										OnLostFocus(elements::GuiGraphicsComposition* sender, elements::GuiEventArgs& arguments);
				void										OnCaretNotify(elements::GuiGraphicsComposition* sender, elements::GuiEventArgs& arguments);

				void										OnLeftButtonDown(elements::GuiGraphicsComposition* sender, elements::GuiMouseEventArgs& arguments);
				void										OnLeftButtonUp(elements::GuiGraphicsComposition* sender, elements::GuiMouseEventArgs& arguments);
				void										OnMouseMove(elements::GuiGraphicsComposition* sender, elements::GuiMouseEventArgs& arguments);
				void										OnKeyDown(elements::GuiGraphicsComposition* sender, elements::GuiKeyEventArgs& arguments);
				void										OnCharInput(elements::GuiGraphicsComposition* sender, elements::GuiCharEventArgs& arguments);
			public:
				GuiTextElementOperator();
				~GuiTextElementOperator();

				void										Install(elements::GuiColorizedTextElement* _textElement, elements::GuiGraphicsComposition* _textComposition, GuiControl* _textControl);
				ICallback*									GetCallback();
				void										SetCallback(ICallback* value);
				GuiTextBoxCommonInterface*					GetTextBoxCommonInterface();
				void										SetTextBoxCommonInterface(GuiTextBoxCommonInterface* value);

				elements::GuiColorizedTextElement*			GetTextElement();
				elements::GuiGraphicsComposition*			GetTextComposition();
				TextPos										GetNearestTextPos(Point point);
				void										Select(TextPos begin, TextPos end);
				WString										GetSelectionText();
				void										SetSelectionText(const WString& value);

				bool										CanCut();
				bool										CanCopy();
				bool										CanPaste();
				void										SelectAll();
				bool										Cut();
				bool										Copy();
				bool										Paste();
			};

			class GuiTextBoxCommonInterface
			{
				friend class GuiTextElementOperator;
			protected:
				GuiTextElementOperator*						textElementOperator;
				GuiControl*									textControl;

				void										RaiseTextChanged();
				void										RaiseSelectionChanged();
				void										InitializeCommonInterface(GuiControl* _textControl, GuiTextElementOperator* _textElementOperator);
			public:
				GuiTextBoxCommonInterface();
				~GuiTextBoxCommonInterface();

				elements::GuiNotifyEvent					SelectionChanged;

				elements::GuiGraphicsComposition*			GetTextComposition();

				bool										CanCut();
				bool										CanCopy();
				bool										CanPaste();
				void										SelectAll();
				bool										Cut();
				bool										Copy();
				bool										Paste();
				
				WString										GetRowText(int row);
				WString										GetFragmentText(TextPos start, TextPos end);
				int											GetRowWidth(int row);
				int											GetRowHeight();
				int											GetMaxWidth();
				int											GetMaxHeight();
				TextPos										GetTextPosFromPoint(Point point);
				Point										GetPointFromTextPos(TextPos pos);
				Rect										GetRectFromTextPos(TextPos pos);
				TextPos										GetNearestTextPos(Point point);

				TextPos										GetCaretBegin();
				TextPos										GetCaretEnd();
				TextPos										GetCaretSmall();
				TextPos										GetCaretLarge();
				void										Select(TextPos begin, TextPos end);

				WString										GetSelectionText();
				void										SetSelectionText(const WString& value);
			};

/***********************************************************************
TextBox
***********************************************************************/

			class GuiMultilineTextBox : public GuiScrollView, public GuiTextBoxCommonInterface
			{
			public:
				static const int							TextMargin=3;

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

					elements::GuiColorizedTextElement*		GetTextElement();
					elements::GuiGraphicsComposition*		GetTextComposition();
					GuiTextElementOperator*					GetTextElementOperator();
					void									SetViewPosition(Point value);
					void									SetFocusableComposition(elements::GuiGraphicsComposition* value)override;

					WString									GetText();
					void									SetText(const WString& value)override;
					void									SetFont(const FontProperties& value)override;
					void									SetVisuallyEnabled(bool value)override;
				};

				class TextElementOperatorCallback : public GuiTextElementOperator::DefaultCallback
				{
				protected:
					GuiMultilineTextBox*					textControl;
					StyleController*						textController;
				public:
					TextElementOperatorCallback(GuiMultilineTextBox* _textControl);

					void									AfterModify(TextPos originalStart, TextPos originalEnd, const WString& originalText, TextPos inputStart, TextPos inputEnd, const WString& inputText)override;
					void									ScrollToView(Point point)override;
					int										GetTextMargin()override;
				};

			protected:
				StyleController*							styleController;

				void										CalculateViewAndSetScroll();
				void										OnRenderTargetChanged(elements::IGuiGraphicsRenderTarget* renderTarget)override;
				Size										QueryFullSize()override;
				void										UpdateView(Rect viewBounds)override;
				void										OnBoundsMouseButtonDown(elements::GuiGraphicsComposition* sender, elements::GuiMouseEventArgs& arguments);
			public:
				GuiMultilineTextBox(GuiMultilineTextBox::IStyleProvider* styleProvider);
				~GuiMultilineTextBox();

				const WString&								GetText()override;
				void										SetText(const WString& value)override;
				void										SetFont(const FontProperties& value)override;
			};

			class GuiSinglelineTextBox : public GuiControl, public GuiTextBoxCommonInterface
			{
			public:
				static const int							TextMargin=3;

				class IStyleProvider : public virtual GuiControl::IStyleProvider
				{
				public:
					virtual elements::GuiGraphicsComposition*		InstallBackground(elements::GuiBoundsComposition* background)=0;
				};

				class StyleController : public Object, public GuiControl::IStyleController
				{
				protected:
					Ptr<IStyleProvider>						styleProvider;
					elements::GuiBoundsComposition*			boundsComposition;
					elements::GuiGraphicsComposition*		containerComposition;

					GuiSinglelineTextBox*					textBox;
					elements::GuiColorizedTextElement*		textElement;
					elements::GuiTableComposition*			textCompositionTable;
					elements::GuiCellComposition*			textComposition;
					GuiTextElementOperator					textElementOperator;
					Ptr<GuiTextElementOperator::ICallback>	defaultCallback;

				public:
					StyleController(IStyleProvider* _styleProvider);
					~StyleController();

					void									SetTextBox(GuiSinglelineTextBox* value);
					void									RearrangeTextElement();
					elements::GuiBoundsComposition*			GetBoundsComposition();
					elements::GuiGraphicsComposition*		GetContainerComposition();
					void									SetFocusableComposition(elements::GuiGraphicsComposition* value);

					WString									GetText();
					void									SetText(const WString& value);
					void									SetFont(const FontProperties& value);
					void									SetVisuallyEnabled(bool value);

					elements::GuiColorizedTextElement*		GetTextElement();
					elements::GuiGraphicsComposition*		GetTextComposition();
					GuiTextElementOperator*					GetTextElementOperator();
					void									SetViewPosition(Point value);
				};

				class TextElementOperatorCallback : public GuiTextElementOperator::DefaultCallback
				{
				protected:
					GuiSinglelineTextBox*					textControl;
					StyleController*						textController;
				public:
					TextElementOperatorCallback(GuiSinglelineTextBox* _textControl);

					bool									BeforeModify(TextPos& start, TextPos& end, const WString& originalText, WString& inputText)override;
					void									AfterModify(TextPos originalStart, TextPos originalEnd, const WString& originalText, TextPos inputStart, TextPos inputEnd, const WString& inputText)override;
					void									ScrollToView(Point point)override;
					int										GetTextMargin()override;
				};
			protected:
				StyleController*							styleController;
				
				void										OnRenderTargetChanged(elements::IGuiGraphicsRenderTarget* renderTarget)override;
				void										OnBoundsMouseButtonDown(elements::GuiGraphicsComposition* sender, elements::GuiMouseEventArgs& arguments);
			public:
				GuiSinglelineTextBox(GuiSinglelineTextBox::IStyleProvider* styleProvider);
				~GuiSinglelineTextBox();

				const WString&								GetText()override;
				void										SetText(const WString& value)override;
				void										SetFont(const FontProperties& value)override;
			};
		}
	}
}

#endif