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
GuiTextElementOperator
***********************************************************************/

			void GuiTextElementOperator::OnGotFocus(elements::GuiGraphicsComposition* sender, elements::GuiEventArgs& arguments)
			{
				textElement->SetFocused(true);
				textElement->SetCaretVisible(true);
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
					if(!arguments.shift)
					{
						textElement->SetCaretBegin(pos);
					}
					textElement->SetCaretEnd(pos);
					textElement->SetCaretVisible(true);
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
						textElement->SetCaretEnd(pos);
						textElement->SetCaretVisible(true);
					}
				}
			}

			GuiTextElementOperator::GuiTextElementOperator()
				:textElement(0)
				,textComposition(0)
				,textControl(0)
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

			void GuiMultilineTextBox::StyleController::SetViewPosition(Point value)
			{
				textElement->SetViewPosition(value);
			}

			WString GuiMultilineTextBox::StyleController::GetText()
			{
				return textElement->lines.GetText();
			}

			void GuiMultilineTextBox::StyleController::SetFocusableComposition(elements::GuiGraphicsComposition* value)
			{
				GuiScrollView::StyleController::SetFocusableComposition(value);
				textElementOperator.Install(textElement, textComposition, scrollView);
			}

			void GuiMultilineTextBox::StyleController::SetText(const WString& value)
			{
				textElement->lines.SetText(value);
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