#include "TableLayout.h"

namespace vl
{
	namespace presentation
	{
		using namespace collections;

/***********************************************************************
DockLayout
***********************************************************************/

		TableLayout::HostPlacement::HostPlacement()
			:host(0)
			,column(0)
			,row(0)
			,columnSpan(1)
			,rowSpan(1)
		{
		}

		TableLayout::HostPlacement::HostPlacement(LayoutHost* _host, int _column, int _row, int _columnSpan, int _rowSpan)
			:host(_host)
			,column(_column)
			,row(_row)
			,columnSpan(_columnSpan)
			,rowSpan(_rowSpan)
		{
		}

		bool TableLayout::HostPlacement::operator==(const HostPlacement& hostPlacement)const
		{
			return
				host==hostPlacement.host &&
				column==hostPlacement.column &&
				row==hostPlacement.row &&
				columnSpan==hostPlacement.columnSpan &&
				rowSpan==hostPlacement.rowSpan;
		}

		bool TableLayout::HostPlacement::operator!=(const HostPlacement& hostPlacement)const
		{
			return
				host!=hostPlacement.host ||
				column!=hostPlacement.column ||
				row!=hostPlacement.row ||
				columnSpan!=hostPlacement.columnSpan ||
				rowSpan!=hostPlacement.rowSpan;
		}

		TableLayout::SizingSettings::SizingSettings()
			:sizingType(AutoSize)
			,length(20)
			,percentage(0.5)
		{
		}

		bool TableLayout::SizingSettings::operator==(const SizingSettings& sizingSettings)const
		{
			return
				sizingType==sizingSettings.sizingType &&
				length==sizingSettings.length &&
				percentage==sizingSettings.percentage;
		}

		bool TableLayout::SizingSettings::operator!=(const SizingSettings& sizingSettings)const
		{
			return
				sizingType!=sizingSettings.sizingType ||
				length!=sizingSettings.length ||
				percentage!=sizingSettings.percentage;
		}

		int TableLayout::FindHost(LayoutHost* host)
		{
			for(int i=0;i<hosts.Count();i++)
			{
				if(hosts[i].host==host)
				{
					return i;
				}
			}
			return -1;
		}

		TableLayout::TableLayout()
		{
		}

		TableLayout::~TableLayout()
		{
		}

		void TableLayout::AdjustHosts()
		{
		}

		// CalculateMinMax Helper Functions Begin

		int Sum(Array<int>& nums, int start, int end)
		{
			int result=0;
			for(int i=start;i<=end;i++)
			{
				result+=nums[i];
			}
			return result;
		}

		int Truncate(int i)
		{
			if(i<0)return 0;
			else if(i>LayoutMaxSize)return LayoutMaxSize;
			else return i;
		}

		int Min(int a, int b)
		{
			return Truncate(a<b?a:b);
		}

		int Max(int a, int b)
		{
			return Truncate(a>b?a:b);
		}

		// CalculateMinMax Helper Functions End

		void TableLayout::CalculateMinMax()
		{
			Array<int> colMins(columns.Count());
			Array<int> colMaxs(columns.Count());
			Array<int> rowMins(rows.Count());
			Array<int> rowMaxs(rows.Count());

			for(int i=0;i<columns.Count();i++)
			{
				int min=0;
				int max=LayoutMaxSize;
				for(int j=0;j<hosts.Count();j++)
				{
					HostPlacement& host=hosts[j];
					if(host.column+host.columnSpan-1==i)
					{
						int minx=host.host->GetMinBounds().x;
						int maxx=host.host->GetMaxBounds().x;

						min=Max(min, minx-Sum(colMins, host.column, i-1));
						max=Min(max, maxx-Sum(colMaxs, host.column, i-1));
					}
				}
				colMins[i]=min;
				colMaxs[i]=max;
			}

			for(int i=0;i<rows.Count();i++)
			{
				int min=0;
				int max=LayoutMaxSize;
				for(int j=0;j<hosts.Count();j++)
				{
					HostPlacement& host=hosts[j];
					if(host.row+host.rowSpan-1==i)
					{
						int miny=host.host->GetMinBounds().y;
						int maxy=host.host->GetMaxBounds().y;

						min=Max(min, miny-Sum(rowMins, host.column, i-1));
						max=Min(max, maxy-Sum(rowMaxs, host.column, i-1));
					}
				}
				rowMins[i]=min;
				rowMaxs[i]=max;
			}

			int colMin=0;
			int rowMin=0;

			for(int i=0;i<columns.Count();i++)
			{
				if(columns[i].sizingType==TableLayout::Absolute)
				{
					colMin+=columns[i].length;
				}
			}

			for(int i=0;i<rows.Count();i++)
			{
				if(rows[i].sizingType==TableLayout::Absolute)
				{
					rowMin+=rows[i].length;
				}
			}

			minSize.x=Max(colMin, Sum(colMins, 0, columns.Count()-1));
			minSize.y=Max(rowMin, Sum(rowMins, 0, rows.Count()-1));
			maxSize.x=Max(colMin, Sum(colMaxs, 0, columns.Count()-1));
			maxSize.y=Max(rowMin, Sum(rowMaxs, 0, rows.Count()-1));
		}

		int TableLayout::GetHostCount()
		{
			return hosts.Count();
		}

		LayoutHost* TableLayout::GetHost(int index)
		{
			return hosts[index].host;
		}

		int TableLayout::GetRowCount()
		{
			return rows.Count();
		}

		int TableLayout::GetColumnCount()
		{
			return columns.Count();
		}

		void TableLayout::SetRowCount(int value)
		{
			rows.Resize(value);
		}

		void TableLayout::SetColumnCount(int value)
		{
			columns.Resize(value);
		}

		TableLayout::SizingSettings& TableLayout::Row(int index)
		{
			return rows[index];
		}

		TableLayout::SizingSettings& TableLayout::Columm(int index)
		{
			return columns[index];
		}

		void TableLayout::Dock(const HostPlacement& hostPlacement)
		{
			Undock(hostPlacement.host);
			hosts.Add(hostPlacement);
		}

		void TableLayout::Undock(LayoutHost* host)
		{
			int index=FindHost(host);
			if(index!=-1)
			{
				hosts.RemoveAt(index);
			}
		}

		TableLayout::HostPlacement TableLayout::GetDock(LayoutHost* host)
		{
			return hosts[FindHost(host)];
		}

		void TableLayout::ClearHosts()
		{
			hosts.Count();
		}
	}
}