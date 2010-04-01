#include "LayoutHost.h"

namespace vl
{
	namespace presentation
	{

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
			}
			if(marginEnd!=-1)
			{
				l=relativeEnd-marginEnd-s;
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

		void LayoutHost::UpdateRealBounds()
		{
			CalculateBound(minSize.x, maxSize.x, bounds.x1, bounds.x2, margin.left, margin.right, relativeBounds.x1, relativeBounds.x2, realBounds.x1, realBounds.x2);
			CalculateBound(minSize.y, maxSize.y, bounds.y1, bounds.y2, margin.top, margin.bottom, relativeBounds.y1, relativeBounds.y2, realBounds.y1, realBounds.y2);
		}

		LayoutHost::LayoutHost()
			:minSize(0, 0)
			,maxSize(65536, 65536)
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
	}
}