#include "StackLayout.h"

namespace vl
{
	namespace presentation
	{
		using namespace collections;

/***********************************************************************
FreeLayout
***********************************************************************/

		StackLayout::StackLayout()
			:direction(LeftToRight)
			,availableCount(0)
		{
		}

		StackLayout::~StackLayout()
		{
		}

		void StackLayout::AdjustHosts()
		{
			availableCount=0;
			switch(direction)
			{
			case LeftToRight:
				{
					int offset=0;
					for(int i=0;i<hosts.Count();i++)
					{
						if(offset>=size.x)
						{
							break;
						}
						LayoutHost* host=hosts[i];
						int length=host->GetSuggestedBounds().x;
						Rect bounds(Point(offset, 0), Size(length, size.y));
						host->SetMarginRelativeBounds(bounds);
						availableCount++;
						offset+=length;
					}
				}
				break;
			case RightToLeft:
				{
					int offset=0;
					for(int i=0;i<hosts.Count();i++)
					{
						if(offset>=size.x)
						{
							break;
						}
						LayoutHost* host=hosts[i];
						int length=host->GetSuggestedBounds().x;
						offset+=length;
						Rect bounds(Point(size.x-offset, 0), Size(length, size.y));
						host->SetMarginRelativeBounds(bounds);
						availableCount++;
					}
				}
				break;
			case TopToBottom:
				{
					int offset=0;
					for(int i=0;i<hosts.Count();i++)
					{
						if(offset>=size.y)
						{
							break;
						}
						LayoutHost* host=hosts[i];
						int length=host->GetSuggestedBounds().y;
						Rect bounds(Point(0, offset), Size(size.x, length));
						host->SetMarginRelativeBounds(bounds);
						availableCount++;
						offset+=length;
					}
				}
				break;
			case BottomToTop:
				{
					int offset=0;
					for(int i=0;i<hosts.Count();i++)
					{
						if(offset>=size.y)
						{
							break;
						}
						LayoutHost* host=hosts[i];
						int length=host->GetSuggestedBounds().y;
						offset+=length;
						Rect bounds(Point(0, size.y-offset), Size(size.x, length));
						host->SetMarginRelativeBounds(bounds);
						availableCount++;
					}
				}
				break;
			}
		}

		void StackLayout::CalculateMinMax()
		{
			switch(direction)
			{
			case LeftToRight:
			case RightToLeft:
				{
					int min=0;
					int max=LayoutMaxSize;
					for(int i=0;i<hosts.Count();i++)
					{
						int minHost=hosts[i]->GetMinBounds().y;
						int maxHost=hosts[i]->GetMaxBounds().y;

						min=min>minHost?min:minHost;
						max=max<maxHost?max:maxHost;
					}
					minSize=Size(0, min);
					maxSize=Size(LayoutMaxSize, max);
				}
				break;
			case TopToBottom:
			case BottomToTop:
				{
					int min=0;
					int max=LayoutMaxSize;
					for(int i=0;i<hosts.Count();i++)
					{
						int minHost=hosts[i]->GetMinBounds().x;
						int maxHost=hosts[i]->GetMaxBounds().x;

						min=min>minHost?min:minHost;
						max=max<maxHost?max:maxHost;
					}
					minSize=Size(min, 0);
					maxSize=Size(max, LayoutMaxSize);
				}
				break;
			}
		}

		int StackLayout::GetHostCount()
		{
			return hosts.Count();
		}

		LayoutHost* StackLayout::GetHost(int index)
		{
			return hosts[index];
		}

		int StackLayout::GetAvailableCount()
		{
			return availableCount;
		}

		StackLayout::Direction StackLayout::GetDirection()
		{
			return direction;
		}

		void StackLayout::SetDirection(Direction _direction)
		{
			direction=_direction;
		}

		collections::IList<LayoutHost*>& StackLayout::Hosts()
		{
			return hosts.Wrap();
		}
	}
}