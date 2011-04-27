/***********************************************************************
Vczh Library++ 3.0
Developer: ³Âè÷å«(vczh)
GUI::Control Library

Interfaces:
***********************************************************************/

#ifndef VCZH_PRESENTATION_GUIGRID
#define VCZH_PRESENTATION_GUIGRID

#include "GuiControl.h"

namespace vl
{
	namespace presentation
	{

/***********************************************************************
×ÔÓÉÅÅ°æ
***********************************************************************/

		class GuiFreeGrid : public GuiControl::Grid
		{
		protected:
			collections::List<GuiControl*>		children;
		public:
			GuiFreeGrid();
			~GuiFreeGrid();

			int									GetChildCount();
			GuiControl*							GetChild(int index);
			int									GetChildIndex(GuiControl* child);
			void								InsertControl(int index, GuiControl* child);
			void								RemoveControl(GuiControl* child);
		};
	}
}

#endif