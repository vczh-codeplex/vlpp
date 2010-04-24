#include "TableLayout.h"

namespace vl
{
	namespace presentation
	{

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

		void TableLayout::CalculateMinMax()
		{
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