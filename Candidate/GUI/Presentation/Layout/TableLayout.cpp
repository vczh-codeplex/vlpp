#include <math.h>
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

		// AutoHosts-CalculateMinMax Helper Functions Begin

		void Zero(Array<int>& nums)
		{
			for(int i=0;i<nums.Count();i++)
			{
				nums[i]=0;
			}
		}

		int Sum(Array<int>& nums, int start, int end)
		{
			int result=0;
			for(int i=start;i<=end;i++)
			{
				result+=nums[i];
			}
			return result;
		}

		int Sum(Array<int>& nums, Array<TableLayout::SizingSettings>& settings, int start, int end)
		{
			int result=0;
			for(int i=start;i<=end;i++)
			{
				if(settings[i].sizingType==TableLayout::Percentage)
				{
					return -1;
				}
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

		void UpdateSizes(Array<int>& sizes, Array<TableLayout::SizingSettings>& settings, int size)
		{
			int usedSize=Sum(sizes, 0, sizes.Count()-1);
			int remainSize=size-usedSize;
			double percentages=0;
			int percentageCount=0;
			int calculatedPercentageCount=0;

			for(int i=0;i<settings.Count();i++)
			{
				if(settings[i].sizingType==TableLayout::Percentage)
				{
					percentages+=settings[i].percentage;
					percentageCount++;
				}
			}

			for(int i=0;i<settings.Count();i++)
			{
				if(settings[i].sizingType==TableLayout::Percentage)
				{
					if(calculatedPercentageCount==percentageCount-1)
					{
						sizes[i]=remainSize;
					}
					else
					{
						int percentageSize=0;
						if(percentages==0)
						{
							percentageSize=remainSize/(percentageCount-calculatedPercentageCount);
						}
						else
						{
							percentageSize=(int)floor(remainSize*settings[i].percentage/percentages);
						}
						sizes[i]=percentageSize;
						remainSize-=percentageSize;
					}
					calculatedPercentageCount++;
				}
			}
		}

		// AdjustHosts-CalculateMinMax Helper Functions End

		void TableLayout::AdjustHosts()
		{
			Array<int> colSizes(columns.Count());
			Array<int> rowSizes(rows.Count());
			Zero(colSizes);
			Zero(rowSizes);

			for(int i=0;i<columns.Count();i++)
			{
				SizingSettings& settings=columns[i];
				switch(settings.sizingType)
				{
				case TableLayout::Absolute:
					colSizes[i]=settings.length;
					break;
				case TableLayout::AutoSize:
					{
						int minAutoSize=0;
						for(int j=0;j<hosts.Count();j++)
						{
							HostPlacement& host=hosts[j];
							if(host.column+host.columnSpan-1==i)
							{
								int usedSize=Sum(colSizes, columns, host.column, i-1);
								if(usedSize!=-1)
								{
									minAutoSize=Max(minAutoSize, host.host->GetSuggestedBounds().x-usedSize);
								}
							}
						}
						colSizes[i]=minAutoSize;
					}
					break;
				}
			}

			for(int i=0;i<rows.Count();i++)
			{
				SizingSettings& settings=rows[i];
				switch(settings.sizingType)
				{
				case TableLayout::Absolute:
					rowSizes[i]=settings.length;
					break;
				case TableLayout::AutoSize:
					{
						int minAutoSize=0;
						for(int j=0;j<hosts.Count();j++)
						{
							HostPlacement& host=hosts[j];
							if(host.row+host.rowSpan-1==i)
							{
								int usedSize=Sum(rowSizes, rows, host.row, i-1);
								if(usedSize!=-1)
								{
									minAutoSize=Max(minAutoSize, host.host->GetSuggestedBounds().y-usedSize);
								}
							}
						}
						rowSizes[i]=minAutoSize;
					}
					break;
				}
			}

			UpdateSizes(colSizes, columns, size.x);
			UpdateSizes(rowSizes, rows, size.y);

			for(int i=0;i<hosts.Count();i++)
			{
				HostPlacement& host=hosts[i];
				int x=Sum(colSizes, 0, host.column-1);
				int y=Sum(rowSizes, 0, host.row-1);
				int w=Sum(colSizes, host.column, host.column+host.columnSpan-1);
				int h=Sum(rowSizes, host.row, host.row+host.rowSpan-1);
				host.host->SetMarginRelativeBounds(Rect(Point(x, y), Size(w, h)));
			}
		}

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
				if(columns[i].sizingType==TableLayout::Absolute)
				{
					min=columns[i].length;
					max=columns[i].length;
				}
				else
				{
					for(int j=0;j<hosts.Count();j++)
					{
						HostPlacement& host=hosts[j];
						if(host.column+host.columnSpan-1==i)
						{
							int minx=0;
							int maxx=0;

							if(columns[i].sizingType==TableLayout::AutoSize)
							{
								if(host.columnSpan==1)
								{
									minx=maxx=host.host->GetSuggestedBounds().x;
								}
								else
								{
									continue;
								}
							}
							else
							{
								minx=host.host->GetMinBounds().x;
								maxx=host.host->GetMaxBounds().x;
							}

							min=Max(min, minx-Sum(colMins, host.column, i-1));
							max=Min(max, maxx-Sum(colMaxs, host.column, i-1)); 
						}
					}
				}
				colMins[i]=min;
				colMaxs[i]=max;
			}

			for(int i=0;i<rows.Count();i++)
			{
				int min=0;
				int max=LayoutMaxSize;
				if(rows[i].sizingType==TableLayout::Absolute)
				{
					min=rows[i].length;
					max=rows[i].length;
				}
				else
				{
					for(int j=0;j<hosts.Count();j++)
					{
						HostPlacement& host=hosts[j];
						if(host.row+host.rowSpan-1==i)
						{
							int miny=0;
							int maxy=0;

							if(rows[i].sizingType==TableLayout::AutoSize)
							{
								if(host.rowSpan==1)
								{
									miny=maxy=host.host->GetSuggestedBounds().y;
								}
								else
								{
									continue;
								}
							}
							else
							{
								miny=host.host->GetMinBounds().y;
								maxy=host.host->GetMaxBounds().y;
							}

							min=Max(min, miny-Sum(rowMins, host.row, i-1));
							max=Min(max, maxy-Sum(rowMaxs, host.row, i-1));
						}
					}
				}
				rowMins[i]=min;
				rowMaxs[i]=max;
			}

			minSize.x=Max(0, Sum(colMins, 0, columns.Count()-1));
			minSize.y=Max(0, Sum(rowMins, 0, rows.Count()-1));
			maxSize.x=Min(LayoutMaxSize, Sum(colMaxs, 0, columns.Count()-1));
			maxSize.y=Min(LayoutMaxSize, Sum(rowMaxs, 0, rows.Count()-1));
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

		void TableLayout::DockHost(const HostPlacement& hostPlacement)
		{
			UndockHost(hostPlacement.host);
			hosts.Add(hostPlacement);
		}

		void TableLayout::UndockHost(LayoutHost* host)
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