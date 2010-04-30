/***********************************************************************
Vczh Library++ 3.0
Developer: ³Âè÷å«(vczh)
GUI::Layout

Classes:
***********************************************************************/

#ifndef VCZH_PRESENTATION_LAYOUT_TABLELAYOUT
#define VCZH_PRESENTATION_LAYOUT_TABLELAYOUT

#include "LayoutHost.h"

namespace vl
{
	namespace presentation
	{
		class TableLayout : public LayoutBase
		{
		public:
			struct HostPlacement
			{
				LayoutHost*								host;
				int										columnSpan;
				int										rowSpan;
				int										column;
				int										row;

				HostPlacement();
				HostPlacement(LayoutHost* _host, int _column, int _row, int _columnSpan=1, int _rowSpan=1);

				bool									operator==(const HostPlacement& hostPlacement)const;
				bool									operator!=(const HostPlacement& hostPlacement)const;
			};

			enum SizingType
			{
				AutoSize,
				Absolute,
				Percentage
			};

			struct SizingSettings
			{
				SizingType								sizingType;
				int										length;
				double									percentage;

				SizingSettings();

				bool									operator==(const SizingSettings& sizingSettings)const;
				bool									operator!=(const SizingSettings& sizingSettings)const;
			};
		protected:
			collections::List<HostPlacement>			hosts;
			collections::Array<SizingSettings>			rows;
			collections::Array<SizingSettings>			columns;

			int											FindHost(LayoutHost* host);
		public:
			TableLayout();
			~TableLayout();

			void										AdjustHosts();
			void										CalculateMinMax();
			int											GetHostCount();
			LayoutHost*									GetHost(int index);

			int											GetRowCount();
			int											GetColumnCount();
			void										SetRowCount(int value);
			void										SetColumnCount(int value);
			SizingSettings&								Row(int index);
			SizingSettings&								Columm(int index);

			void										DockHost(const HostPlacement& hostPlacement);
			void										UndockHost(LayoutHost* host);
			HostPlacement								GetDock(LayoutHost* host);
			void										ClearHosts();
		};
	}
}

#endif