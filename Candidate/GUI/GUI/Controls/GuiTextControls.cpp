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

			GuiMultilineTextBox::GuiMultilineTextBox(GuiScrollView::IStyleProvider* styleProvider)
				:GuiScrollView(new StyleController(styleProvider))
			{
			}

			GuiMultilineTextBox::~GuiMultilineTextBox()
			{
			}
		}
	}
}