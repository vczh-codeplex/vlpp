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
			protected:
				class StyleController : public GuiScrollView::StyleController
				{
				public:
					StyleController(GuiScrollView::IStyleProvider* styleProvider);
					~StyleController();
				};

				Size									QueryFullSize();
				void									UpdateView(Rect viewBounds);
			public:
				GuiMultilineTextBox(GuiScrollView::IStyleProvider* styleProvider);
				~GuiMultilineTextBox();
			};
		}
	}
}

#endif