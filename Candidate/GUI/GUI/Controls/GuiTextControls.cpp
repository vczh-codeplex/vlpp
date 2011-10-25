#include "GuiTextControls.h"
#include <math.h>

namespace vl
{
	namespace presentation
	{
		namespace controls
		{
			using namespace elements;

/***********************************************************************
GuiTextElementOperator::DefaultCallback
***********************************************************************/

			GuiTextElementOperator::DefaultCallback::DefaultCallback(elements::GuiColorizedTextElement* _textElement, elements::GuiBoundsComposition* _textComposition)
				:textElement(_textElement)
				,textComposition(_textComposition)
			{
			}

			GuiTextElementOperator::DefaultCallback::~DefaultCallback()
			{
			}

			TextPos GuiTextElementOperator::DefaultCallback::GetLeftWord(TextPos pos)
			{
				return pos;
			}

			TextPos GuiTextElementOperator::DefaultCallback::GetRightWord(TextPos pos)
			{
				return pos;
			}

			void GuiTextElementOperator::DefaultCallback::GetWord(TextPos pos, TextPos& begin, TextPos& end)
			{
				begin=pos;
				end=pos;
			}

			int GuiTextElementOperator::DefaultCallback::GetPageRows()
			{
				return textComposition->GetBounds().Height()/textElement->lines.GetRowHeight();
			}

			bool GuiTextElementOperator::DefaultCallback::BeforeModify(TextPos& start, TextPos& end, const WString& originalText, WString& inputText)
			{
				return true;
			}

/***********************************************************************
GuiTextElementOperator
***********************************************************************/

			void GuiTextElementOperator::UpdateCaretPoint()
			{
				GuiGraphicsHost* host=textComposition->GetRelatedGraphicsHost();
				if(host)
				{
					Rect caret=textElement->lines.GetRectFromTextPos(textElement->GetCaretEnd());
					Point view=textElement->GetViewPosition();
					int textMargin=callback->GetTextMargin();
					int x=caret.x1-view.x;
					int y=caret.y2-view.y;
					host->SetCaretPoint(Point(x, y), textComposition);
				}
			}

			void GuiTextElementOperator::Move(TextPos pos, bool shift)
			{
				pos=textElement->lines.Normalize(pos);
				if(!shift)
				{
					textElement->SetCaretBegin(pos);
				}
				textElement->SetCaretEnd(pos);
				textElement->SetCaretVisible(true);

				Rect bounds=textElement->lines.GetRectFromTextPos(pos);
				Rect view=Rect(textElement->GetViewPosition(), textComposition->GetBounds().GetSize());
				Point viewPoint=view.LeftTop();
				int offsetX=textElement->lines.GetRowHeight()*5;

				if(bounds.x1<view.x1)
				{
					viewPoint.x=bounds.x1-offsetX;
				}
				else if(bounds.x2>view.x2)
				{
					viewPoint.x=bounds.x2-view.Width()+offsetX;
				}
				if(bounds.y1<view.y1)
				{
					viewPoint.y=bounds.y1;
				}
				else if(bounds.y2>view.y2)
				{
					viewPoint.y=bounds.y2-view.Height();
				}

				callback->ScrollToView(viewPoint);
				UpdateCaretPoint();
			}

			void GuiTextElementOperator::Modify(TextPos start, TextPos end, const WString& input)
			{
				if(start>end)
				{
					TextPos temp=start;
					start=end;
					end=temp;
				}
				TextPos originalStart=start;
				TextPos originalEnd=end;
				WString originalText=textElement->lines.GetText(start, end);
				WString inputText=input;
				if(callback->BeforeModify(start, end, originalText, inputText))
				{
					end=textElement->lines.Modify(start, end, inputText);
					callback->AfterModify(originalStart, originalEnd, originalText, start, end, inputText);
					Move(end, false);
				}
			}

			void GuiTextElementOperator::ProcessKey(int code, bool shift, bool ctrl)
			{
				if(!shift && ctrl)
				{
					switch(code)
					{
					case L'A':
						SelectAll();
						return;
					case L'X':
						Cut();
						return;
					case L'C':
						Copy();
						return;
					case L'V':
						Paste();
						return;
					}
				}
				TextPos begin=textElement->GetCaretBegin();
				TextPos end=textElement->GetCaretEnd();
				switch(code)
				{
				case VKEY_UP:
					{
						end.row--;
						Move(end, shift);
					}
					break;
				case VKEY_DOWN:
					{
						end.row++;
						Move(end, shift);
					}
					break;
				case VKEY_LEFT:
					{
						if(ctrl)
						{
							Move(callback->GetLeftWord(end), shift);
						}
						else
						{
							if(end.column==0)
							{
								end.row--;
								end=textElement->lines.Normalize(end);
								end.column=textElement->lines.GetLine(end.row).dataLength;
							}
							else
							{
								end.column--;
							}
							Move(end, shift);
						}
					}
					break;
				case VKEY_RIGHT:
					{
						if(ctrl)
						{
							Move(callback->GetRightWord(end), shift);
						}
						else
						{
							if(end.column==textElement->lines.GetLine(end.row).dataLength)
							{
								end.row++;
								end.column=0;
							}
							else
							{
								end.column++;
							}
							Move(end, shift);
						}
					}
					break;
				case VKEY_HOME:
					{
						if(ctrl)
						{
							Move(TextPos(0, 0), shift);
						}
						else
						{
							end.column=0;
							Move(end, shift);
						}
					}
					break;
				case VKEY_END:
					{
						if(ctrl)
						{
							end.row=textElement->lines.GetCount()-1;
						}
						end.column=textElement->lines.GetLine(end.row).dataLength;
						Move(end, shift);
					}
					break;
				case VKEY_PRIOR:
					{
						end.row-=callback->GetPageRows();
						Move(end, shift);
					}
					break;
				case VKEY_NEXT:
					{
						end.row+=callback->GetPageRows();
						Move(end, shift);
					}
					break;
				case VKEY_BACK:
					{
						if(ctrl && !shift)
						{
							ProcessKey(VKEY_LEFT, true, true);
							ProcessKey(VKEY_BACK, false, false);
						}
						else if(!ctrl && shift)
						{
							ProcessKey(VKEY_UP, true, false);
							ProcessKey(VKEY_BACK, false, false);
						}
						else
						{
							if(begin==end)
							{
								ProcessKey(VKEY_LEFT, true, false);
							}
							SetSelectionText(L"");
						}
					}
					break;
				case VKEY_DELETE:
					{
						if(ctrl && !shift)
						{
							ProcessKey(VKEY_RIGHT, true, true);
							ProcessKey(VKEY_DELETE, false, false);
						}
						else if(!ctrl && shift)
						{
							ProcessKey(VKEY_DOWN, true, false);
							ProcessKey(VKEY_DELETE, false, false);
						}
						else
						{
							if(begin==end)
							{
								ProcessKey(VKEY_RIGHT, true, false);
							}
							SetSelectionText(L"");
						}
					}
					break;
				}
			}

			void GuiTextElementOperator::OnGotFocus(elements::GuiGraphicsComposition* sender, elements::GuiEventArgs& arguments)
			{
				textElement->SetFocused(true);
				textElement->SetCaretVisible(true);
				UpdateCaretPoint();
			}

			void GuiTextElementOperator::OnLostFocus(elements::GuiGraphicsComposition* sender, elements::GuiEventArgs& arguments)
			{
				textElement->SetFocused(false);
				textElement->SetCaretVisible(false);
			}

			void GuiTextElementOperator::OnCaretNotify(elements::GuiGraphicsComposition* sender, elements::GuiEventArgs& arguments)
			{
				textElement->SetCaretVisible(!textElement->GetCaretVisible());
			}

			void GuiTextElementOperator::OnLeftButtonDown(elements::GuiGraphicsComposition* sender, elements::GuiMouseEventArgs& arguments)
			{
				if(textControl->GetVisuallyEnabled() && arguments.compositionSource==arguments.eventSource)
				{
					dragging=true;
					TextPos pos=GetNearestTextPos(Point(arguments.x, arguments.y));
					Move(pos, arguments.shift);
				}
			}

			void GuiTextElementOperator::OnLeftButtonUp(elements::GuiGraphicsComposition* sender, elements::GuiMouseEventArgs& arguments)
			{
				if(textControl->GetVisuallyEnabled() && arguments.compositionSource==arguments.eventSource)
				{
					dragging=false;
				}
			}

			void GuiTextElementOperator::OnMouseMove(elements::GuiGraphicsComposition* sender, elements::GuiMouseEventArgs& arguments)
			{
				if(textControl->GetVisuallyEnabled() && arguments.compositionSource==arguments.eventSource)
				{
					if(dragging)
					{
						TextPos pos=GetNearestTextPos(Point(arguments.x, arguments.y));
						Move(pos, true);
					}
				}
			}

			void GuiTextElementOperator::OnKeyDown(elements::GuiGraphicsComposition* sender, elements::GuiKeyEventArgs& arguments)
			{
				if(textControl->GetVisuallyEnabled() && arguments.compositionSource==arguments.eventSource)
				{
					ProcessKey(arguments.code, arguments.shift, arguments.ctrl);
				}
			}

			void GuiTextElementOperator::OnCharInput(elements::GuiGraphicsComposition* sender, elements::GuiCharEventArgs& arguments)
			{
				if(textControl->GetVisuallyEnabled() && arguments.compositionSource==arguments.eventSource)
				{
					if(arguments.code!=VKEY_ESCAPE && arguments.code!=VKEY_BACK && !arguments.ctrl)
					{
						SetSelectionText(WString(arguments.code));
					}
				}
			}

			GuiTextElementOperator::GuiTextElementOperator()
				:textElement(0)
				,textComposition(0)
				,textControl(0)
				,callback(0)
				,dragging(false)
			{
			}

			GuiTextElementOperator::~GuiTextElementOperator()
			{
			}

			void GuiTextElementOperator::Install(elements::GuiColorizedTextElement* _textElement, elements::GuiBoundsComposition* _textComposition, GuiControl* _textControl)
			{
				textElement=_textElement;
				textComposition=_textComposition;
				textControl=_textControl;
				textComposition->SetAssociatedCursor(GetCurrentController()->GetSystemCursor(INativeCursor::IBeam));

				GuiGraphicsComposition* focusableComposition=textControl->GetFocusableComposition();
				focusableComposition->GetEventReceiver()->gotFocus.AttachMethod(this, &GuiTextElementOperator::OnGotFocus);
				focusableComposition->GetEventReceiver()->lostFocus.AttachMethod(this, &GuiTextElementOperator::OnLostFocus);
				focusableComposition->GetEventReceiver()->caretNotify.AttachMethod(this, &GuiTextElementOperator::OnCaretNotify);
				textComposition->GetEventReceiver()->leftButtonDown.AttachMethod(this, &GuiTextElementOperator::OnLeftButtonDown);
				textComposition->GetEventReceiver()->leftButtonUp.AttachMethod(this, &GuiTextElementOperator::OnLeftButtonUp);
				textComposition->GetEventReceiver()->mouseMove.AttachMethod(this, &GuiTextElementOperator::OnMouseMove);
				focusableComposition->GetEventReceiver()->keyDown.AttachMethod(this, &GuiTextElementOperator::OnKeyDown);
				focusableComposition->GetEventReceiver()->charInput.AttachMethod(this, &GuiTextElementOperator::OnCharInput);
			}
			
			GuiTextElementOperator::ICallback* GuiTextElementOperator::GetCallback()
			{
				return callback;
			}

			void GuiTextElementOperator::SetCallback(ICallback* value)
			{
				callback=value;
			}

			TextPos GuiTextElementOperator::GetNearestTextPos(Point point)
			{
				Point viewPosition=textElement->GetViewPosition();
				Point mousePosition=Point(point.x+viewPosition.x, point.y+viewPosition.y);
				TextPos pos=textElement->lines.GetTextPosFromPoint(mousePosition);
				if(pos.column<textElement->lines.GetLine(pos.row).dataLength)
				{
					Rect rect=textElement->lines.GetRectFromTextPos(pos);
					if(abs(rect.x1-mousePosition.x)>=abs(rect.x2-1-mousePosition.x))
					{
						pos.column++;
					}
				}
				return pos;
			}

			WString GuiTextElementOperator::GetSelectionText()
			{
				TextPos selectionBegin=textElement->GetCaretBegin()<textElement->GetCaretEnd()?textElement->GetCaretBegin():textElement->GetCaretEnd();
				TextPos selectionEnd=textElement->GetCaretBegin()>textElement->GetCaretEnd()?textElement->GetCaretBegin():textElement->GetCaretEnd();
				return textElement->lines.GetText(selectionBegin, selectionEnd);
			}

			void GuiTextElementOperator::SetSelectionText(const WString& value)
			{
				Modify(textElement->GetCaretBegin(), textElement->GetCaretEnd(), value);
			}

			bool GuiTextElementOperator::CanCut()
			{
				return textElement->GetCaretBegin()!=textElement->GetCaretEnd();
			}

			bool GuiTextElementOperator::CanCopy()
			{
				return textElement->GetCaretBegin()!=textElement->GetCaretEnd();
			}

			bool GuiTextElementOperator::CanPaste()
			{
				return GetCurrentController()->GetClipboard()->ContainsText();
			}

			void GuiTextElementOperator::SelectAll()
			{
				int row=textElement->lines.GetCount()-1;
				textElement->SetCaretBegin(TextPos(0, 0));
				textElement->SetCaretEnd(TextPos(row, textElement->lines.GetLine(row).dataLength));
			}

			bool GuiTextElementOperator::Cut()
			{
				if(CanCut())
				{
					GetCurrentController()->GetClipboard()->SetText(GetSelectionText());
					SetSelectionText(L"");
					return true;
				}
				else
				{
					return false;
				}
			}

			bool GuiTextElementOperator::Copy()
			{
				if(CanCopy())
				{
					GetCurrentController()->GetClipboard()->SetText(GetSelectionText());
					return true;
				}
				else
				{
					return false;
				}
			}

			bool GuiTextElementOperator::Paste()
			{
				if(CanPaste())
				{
					SetSelectionText(GetCurrentController()->GetClipboard()->GetText());
					return true;
				}
				else
				{
					return false;
				}
			}

/***********************************************************************
GuiMultilineTextBox::StyleController
***********************************************************************/

			GuiMultilineTextBox::StyleController::StyleController(GuiScrollView::IStyleProvider* styleProvider)
				:GuiScrollView::StyleController(styleProvider)
				,textElement(0)
				,textComposition(0)
			{
				textElement=GuiColorizedTextElement::Create();

				textComposition=new GuiBoundsComposition;
				textComposition->SetAlignmentToParent(Margin(0, 0, 0, 0));
				textComposition->SetOwnedElement(textElement);

				GetInternalContainerComposition()->AddChild(textComposition);
			}

			GuiMultilineTextBox::StyleController::~StyleController()
			{
			}

			elements::GuiColorizedTextElement* GuiMultilineTextBox::StyleController::GetTextElement()
			{
				return textElement;
			}

			elements::GuiBoundsComposition* GuiMultilineTextBox::StyleController::GetTextComposition()
			{
				return textComposition;
			}

			GuiTextElementOperator* GuiMultilineTextBox::StyleController::GetTextElementOperator()
			{
				return &textElementOperator;
			}

			void GuiMultilineTextBox::StyleController::SetViewPosition(Point value)
			{
				textElement->SetViewPosition(value);
			}

			void GuiMultilineTextBox::StyleController::SetFocusableComposition(elements::GuiGraphicsComposition* value)
			{
				GuiScrollView::StyleController::SetFocusableComposition(value);
				textElementOperator.Install(textElement, textComposition, scrollView);
				if(!textElementOperator.GetCallback())
				{
					if(!defaultCallback)
					{
						defaultCallback=new DefaultTextElementOperatorCallback(dynamic_cast<GuiMultilineTextBox*>(scrollView));
					}
					textElementOperator.SetCallback(defaultCallback.Obj());
				}
			}

			WString GuiMultilineTextBox::StyleController::GetText()
			{
				return textElement->lines.GetText();
			}

			void GuiMultilineTextBox::StyleController::SetText(const WString& value)
			{
				textElement->lines.SetText(value);
				textElement->SetCaretBegin(TextPos(0, 0));
				textElement->SetCaretEnd(TextPos(0, 0));
				GuiScrollView::StyleController::SetText(value);
			}

			void GuiMultilineTextBox::StyleController::SetFont(const FontProperties& value)
			{
				textElement->SetFont(value);
				GuiScrollView::StyleController::SetFont(value);
			}

			void GuiMultilineTextBox::StyleController::SetVisuallyEnabled(bool value)
			{
				textElement->SetVisuallyEnabled(value);
				GuiScrollView::StyleController::SetVisuallyEnabled(value);
			}

/***********************************************************************
GuiMultilineTextBox::DefaultTextElementOperatorCallback
***********************************************************************/

			GuiMultilineTextBox::DefaultTextElementOperatorCallback::DefaultTextElementOperatorCallback(GuiMultilineTextBox* _textControl)
				:GuiTextElementOperator::DefaultCallback(
					dynamic_cast<StyleController*>(_textControl->GetStyleController())->GetTextElement(),
					dynamic_cast<StyleController*>(_textControl->GetStyleController())->GetTextComposition()
					)
				,textControl(_textControl)
				,textController(dynamic_cast<StyleController*>(_textControl->GetStyleController()))
			{
			}

			void GuiMultilineTextBox::DefaultTextElementOperatorCallback::AfterModify(TextPos originalStart, TextPos originalEnd, const WString& originalText, TextPos inputStart, TextPos inputEnd, const WString& inputText)
			{
				textControl->CalculateView();
			}
			
			void GuiMultilineTextBox::DefaultTextElementOperatorCallback::ScrollToView(Point point)
			{
				point.x+=TextMargin;
				point.y+=TextMargin;
				Point oldPoint(textControl->GetHorizontalScroll()->GetPosition(), textControl->GetVerticalScroll()->GetPosition());
				int marginX=0;
				int marginY=0;
				if(oldPoint.x<point.x)
				{
					marginX=TextMargin;
				}
				else if(oldPoint.x>point.x)
				{
					marginX=-TextMargin;
				}
				if(oldPoint.y<point.y)
				{
					marginY=TextMargin;
				}
				else if(oldPoint.y>point.y)
				{
					marginY=-TextMargin;
				}
				textControl->GetHorizontalScroll()->SetPosition(point.x+marginX);
				textControl->GetVerticalScroll()->SetPosition(point.y+marginY);
			}

			int GuiMultilineTextBox::DefaultTextElementOperatorCallback::GetTextMargin()
			{
				return TextMargin;
			}

/***********************************************************************
GuiMultilineTextBox
***********************************************************************/

			void GuiMultilineTextBox::CalculateViewAndSetScroll()
			{
				CalculateView();
				int smallMove=styleController->GetTextElement()->lines.GetRowHeight();
				int bigMove=smallMove*5;
				styleController->GetHorizontalScroll()->SetSmallMove(smallMove);
				styleController->GetHorizontalScroll()->SetBigMove(bigMove);
				styleController->GetVerticalScroll()->SetSmallMove(smallMove);
				styleController->GetVerticalScroll()->SetBigMove(bigMove);
			}

			void GuiMultilineTextBox::OnRenderTargetChanged(elements::IGuiGraphicsRenderTarget* renderTarget)
			{
				CalculateViewAndSetScroll();
			}

			Size GuiMultilineTextBox::QueryFullSize()
			{
				text::TextLines& lines=styleController->GetTextElement()->lines;
				return Size(lines.GetMaxWidth()+TextMargin*2, lines.GetMaxHeight()+TextMargin*2);
			}

			void GuiMultilineTextBox::UpdateView(Rect viewBounds)
			{
				styleController->SetViewPosition(viewBounds.LeftTop()-Size(TextMargin, TextMargin));
			}

			void GuiMultilineTextBox::OnBoundsMouseButtonDown(elements::GuiGraphicsComposition* sender, elements::GuiMouseEventArgs& arguments)
			{
				if(GetVisuallyEnabled())
				{
					boundsComposition->GetRelatedGraphicsHost()->SetFocus(boundsComposition);
				}
			}

			GuiMultilineTextBox::GuiMultilineTextBox(GuiScrollView::IStyleProvider* styleProvider)
				:GuiScrollView(new StyleController(styleProvider))
			{
				styleController=dynamic_cast<StyleController*>(GetStyleController());

				boundsComposition->GetEventReceiver()->leftButtonDown.AttachMethod(this, &GuiMultilineTextBox::OnBoundsMouseButtonDown);
				boundsComposition->GetEventReceiver()->middleButtonDown.AttachMethod(this, &GuiMultilineTextBox::OnBoundsMouseButtonDown);
				boundsComposition->GetEventReceiver()->rightButtonDown.AttachMethod(this, &GuiMultilineTextBox::OnBoundsMouseButtonDown);
				SetFocusableComposition(boundsComposition);
			}

			GuiMultilineTextBox::~GuiMultilineTextBox()
			{
			}

			const WString& GuiMultilineTextBox::GetText()
			{
				text=styleController->GetText();
				return text;
			}

			void GuiMultilineTextBox::SetText(const WString& value)
			{
				styleController->SetText(value);
				GuiScrollView::SetText(value);
				CalculateView();
			}

			void GuiMultilineTextBox::SetFont(const FontProperties& value)
			{
				GuiScrollView::SetFont(value);
				CalculateViewAndSetScroll();
			}
		}
	}
}