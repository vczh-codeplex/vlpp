#include "GuiTextControls.h"

namespace vl
{
	namespace presentation
	{
		namespace controls
		{
			using namespace elements;

/***********************************************************************
GuiMultilineTextBox::StyleController
***********************************************************************/

			GuiMultilineTextBox::StyleController::StyleController(GuiScrollView::IStyleProvider* styleProvider)
				:GuiScrollView::StyleController(styleProvider)
			{
			}

			GuiMultilineTextBox::StyleController::~StyleController()
			{
			}

/***********************************************************************
GuiMultilineTextBox
***********************************************************************/

			Size GuiMultilineTextBox::QueryFullSize()
			{
				return Size(0, 0);
			}

			void GuiMultilineTextBox::UpdateView(Rect viewBounds)
			{
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
				boundsComposition->GetEventReceiver()->leftButtonDown.AttachMethod(this, &GuiMultilineTextBox::OnBoundsMouseButtonDown);
				boundsComposition->GetEventReceiver()->middleButtonDown.AttachMethod(this, &GuiMultilineTextBox::OnBoundsMouseButtonDown);
				boundsComposition->GetEventReceiver()->rightButtonDown.AttachMethod(this, &GuiMultilineTextBox::OnBoundsMouseButtonDown);
				styleController->SetFocusableComposition(boundsComposition);
			}

			GuiMultilineTextBox::~GuiMultilineTextBox()
			{
			}

			elements::GuiGraphicsComposition* GuiMultilineTextBox::GetFocusableComposition()
			{
				return boundsComposition;
			}
		}
	}
}