/***********************************************************************
Vczh Library++ 3.0
Developer: ³Âè÷å«(vczh)
GUI::Layout

Classes:
***********************************************************************/

#ifndef VCZH_PRESENTATION_LAYOUT_STACKLAYOUT
#define VCZH_PRESENTATION_LAYOUT_STACKLAYOUT

#include "LayoutHost.h"

namespace vl
{
	namespace presentation
	{
		class StackLayout : public LayoutBase
		{
		public:
			enum Direction
			{
				LeftToRight,
				RightToLeft,
				TopToBottom,
				BottomToTop,
			};
		protected:
			collections::List<LayoutHost*>				hosts;
			Direction									direction;
			int											availableCount;

		public:
			StackLayout();
			~StackLayout();

			void										AdjustHosts();
			void										CalculateMinMax();
			int											GetHostCount();
			LayoutHost*									GetHost(int index);

			int											GetAvailableCount();
			Direction									GetDirection();
			void										SetDirection(Direction _direction);
			collections::IList<LayoutHost*>&			Hosts();
		};
	}
}

#endif