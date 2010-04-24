/***********************************************************************
Vczh Library++ 3.0
Developer: ≥¬Ë˜Â´(vczh)
GUI::Layout

Classes:
  LayoutHost								£∫≈≈∞Ê∂‘œÛ
  LayoutBase								£∫≈≈∞Êª˘¿‡

  FreeLayout								£∫◊‘”…≈≈∞Ê
  TableLayout								£∫±Ì∏Ò≈≈∞Ê
  DockLayout								£∫Õ£øø≈≈∞Ê
  StackLayout								£∫∂—’ª≈≈∞Ê
  WrapLayout								£∫ªª––≈≈∞Ê
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