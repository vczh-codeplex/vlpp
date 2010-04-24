/***********************************************************************
Vczh Library++ 3.0
Developer: ³Âè÷å«(vczh)
GUI::Layout

Classes:
***********************************************************************/

#ifndef VCZH_PRESENTATION_LAYOUT_DOCKLAYOUT
#define VCZH_PRESENTATION_LAYOUT_DOCKLAYOUT

#include "LayoutHost.h"

namespace vl
{
	namespace presentation
	{
		class DockLayout : public LayoutBase
		{
		public:
			enum DockType
			{
				Left,
				Top,
				Right,
				Bottom,
				None
			};
		protected:
			struct DockedHost
			{
				DockType								dockType;
				LayoutHost*								host;

				bool									operator==(const DockedHost& dockedHost)const;
				bool									operator!=(const DockedHost& dockedHost)const;
			};
			friend class collections::ReadonlyListEnumerator<DockedHost>;

			collections::List<DockedHost>				dockedHosts;

			int											FindHost(LayoutHost* host);
		public:
			DockLayout();
			~DockLayout();

			void										AdjustHosts();
			void										CalculateMinMax();
			int											GetHostCount();
			LayoutHost*									GetHost(int index);

			void										DockHost(DockType dockType, LayoutHost* host);
			void										UndockHost(LayoutHost* host);
			DockType									GetDockType(LayoutHost* host);
			void										ClearHosts();
		};
	}
}

#endif