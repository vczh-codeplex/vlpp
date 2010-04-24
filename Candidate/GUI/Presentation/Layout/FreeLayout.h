/***********************************************************************
Vczh Library++ 3.0
Developer: �����(vczh)
GUI::Layout

Classes:
  LayoutHost								���Ű����
  LayoutBase								���Ű����

  FreeLayout								�������Ű�
  TableLayout								������Ű�
  DockLayout								��ͣ���Ű�
  StackLayout								����ջ�Ű�
  WrapLayout								�������Ű�
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