#include "LayoutHost.h"

namespace vl
{
	namespace presentation
	{
		using namespace collections;

		const int LayoutMaxSize=65536;

/***********************************************************************
LayoutHost
***********************************************************************/

		void LayoutHost::CalculateBound(int min, int max, int start, int length, int marginStart, int marginEnd, int relativeStart, int relativeEnd, int& resultStart, int& resultEnd)
		{
			int s=start;
			int l=length;

			if(marginStart!=-1)
			{
				s=relativeStart+marginStart;
				if(marginEnd!=-1)
				{
					l=relativeEnd-marginEnd-s;
				}
			}
			else
			{
				if(marginEnd!=-1)
				{
					s=relativeEnd-marginEnd-l;
				}
			}

			if(l<min)
			{
				l=min;
			}
			else if(l>max)
			{
				l=max;
			}

			resultStart=s;
			resultEnd=s+l;
		}

		void LayoutHost::GetBound(int marginStart, int marginEnd, int min, int& bound)
		{
			bound=min+(marginStart==-1?0:marginStart)+(marginEnd==-1?0:marginEnd);
		}

		void LayoutHost::UpdateRealBounds()
		{
			CalculateBound(minSize.x, maxSize.x, bounds.x1, bounds.x2, margin.left, margin.right, relativeBounds.x1, relativeBounds.x2, realBounds.x1, realBounds.x2);
			CalculateBound(minSize.y, maxSize.y, bounds.y1, bounds.y2, margin.top, margin.bottom, relativeBounds.y1, relativeBounds.y2, realBounds.y1, realBounds.y2);
		}

		LayoutHost::LayoutHost()
			:minSize(0, 0)
			,maxSize(LayoutMaxSize, LayoutMaxSize)
			,margin(-1, -1, -1, -1)
			,bounds(0, 0, 0, 0)
			,relativeBounds(0, 0, 0, 0)
			,realBounds(0, 0, 0, 0)
		{
		}

		LayoutHost::~LayoutHost()
		{
		}

		Size LayoutHost::GetMinSize()
		{
			return minSize;
		}

		Size LayoutHost::GetMaxSize()
		{
			return maxSize;
		}

		Margin LayoutHost::GetMargin()
		{
			return margin;
		}

		Point LayoutHost::GetPosition()
		{
			return bounds.LeftTop();
		}

		Size LayoutHost::GetSize()
		{
			return bounds.GetSize();
		}

		Rect LayoutHost::GetBounds()
		{
			return bounds;
		}

		Rect LayoutHost::GetMarginRelativeBounds()
		{
			return relativeBounds;
		}

		Rect LayoutHost::GetRealBounds()
		{
			return realBounds;
		}

		Size LayoutHost::GetMinBounds()
		{
			Size size;
			GetBound(margin.left, margin.right, minSize.x, size.x);
			GetBound(margin.top, margin.bottom, minSize.y, size.y);
			return size;
		}

		Size LayoutHost::GetMaxBounds()
		{
			Size size;
			GetBound(margin.left, margin.right, maxSize.x, size.x);
			GetBound(margin.top, margin.bottom, maxSize.y, size.y);
			return size;
		}

		void LayoutHost::SetMinSize(Size _size)
		{
			if(minSize!=_size)
			{
				minSize=_size;
				UpdateRealBounds();
			}
		}

		void LayoutHost::SetMaxSize(Size _size)
		{
			if(maxSize!=_size)
			{
				maxSize=_size;
				UpdateRealBounds();
			}
		}

		void LayoutHost::SetMargin(Margin _margin)
		{
			if(margin!=_margin)
			{
				margin=_margin;
				UpdateRealBounds();
			}
		}

		void LayoutHost::SetPosition(Point _position)
		{
			if(bounds.LeftTop()!=_position)
			{
				bounds.x1=_position.x;
				bounds.x2=_position.y;
				UpdateRealBounds();
			}
		}

		void LayoutHost::SetSize(Size _size)
		{
			if(bounds.GetSize()!=_size)
			{
				bounds.x2=bounds.x1+_size.x;
				bounds.x2=bounds.y1+_size.y;
				UpdateRealBounds();
			}
		}

		void LayoutHost::SetBounds(Rect _bounds)
		{
			if(bounds!=_bounds)
			{
				bounds=_bounds;
				UpdateRealBounds();
			}
		}

		void LayoutHost::SetMarginRelativeBounds(Rect _bounds)
		{
			if(relativeBounds!=_bounds)
			{
				relativeBounds=_bounds;
				UpdateRealBounds();
			}
		}

/***********************************************************************
LayoutBase
***********************************************************************/

		LayoutBase::LayoutBase()
		{
			maxSize=Size(LayoutMaxSize, LayoutMaxSize);
		}

		LayoutBase::~LayoutBase()
		{
		}

		Size LayoutBase::GetSize()
		{
			return size;
		}

		void LayoutBase::SetSize(Size _size)
		{
			size=_size;
		}

		Size LayoutBase::GetMinSize()
		{
			return minSize;
		}

		Size LayoutBase::GetMaxSize()
		{
			return maxSize;
		}

/***********************************************************************
FreeLayout
***********************************************************************/

		FreeLayout::FreeLayout()
		{
		}

		FreeLayout::~FreeLayout()
		{
		}

		void FreeLayout::AdjustHosts()
		{
			Rect bounds(Point(), size);
			for(int i=0;i<hosts.Count();i++)
			{
				hosts[i]->SetMarginRelativeBounds(bounds);
			}
		}

		void FreeLayout::CalculateMinMax()
		{
			Size minResult(0, 0);
			Size maxResult(LayoutMaxSize, LayoutMaxSize);
			for(int i=0;i<hosts.Count();i++)
			{
				Size minHostSize=hosts[i]->GetMinBounds();
				if(minResult.x<minHostSize.x)minResult.x=minHostSize.x;
				if(minResult.y<minHostSize.y)minResult.y=minHostSize.y;

				Size maxHostSize=hosts[i]->GetMaxBounds();
				if(maxResult.x>maxHostSize.x)maxResult.x=maxHostSize.x;
				if(maxResult.y>maxHostSize.y)maxResult.y=maxHostSize.y;
			}
			minSize=minResult;
			maxSize=maxResult;
		}

		int FreeLayout::GetHostCount()
		{
			return hosts.Count();
		}

		LayoutHost* FreeLayout::GetHost(int index)
		{
			return hosts[index];
		}

		collections::IList<LayoutHost*>& FreeLayout::Hosts()
		{
			return hosts.Wrap();
		}
	}
}