/***********************************************************************
Vczh Library++ 3.0
Developer: ³Âè÷å«(vczh)
GUI::Layout

Classes:
***********************************************************************/

#ifndef VCZH_PRESENTATION_LAYOUT_FREELAYOUT
#define VCZH_PRESENTATION_LAYOUT_FREELAYOUT

#include "LayoutHost.h"

namespace vl
{
	namespace presentation
	{
		class FreeLayout : public LayoutBase
		{
		protected:
			collections::List<LayoutHost*>				hosts;

		public:
			FreeLayout();
			~FreeLayout();

			void										AdjustHosts();
			void										CalculateMinMax();
			int											GetHostCount();
			LayoutHost*									GetHost(int index);

			collections::IList<LayoutHost*>&			Hosts();
		};
	}
}

#endif