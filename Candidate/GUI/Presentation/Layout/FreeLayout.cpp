#include "FreeLayout.h"

namespace vl
{
	namespace presentation
	{
		using namespace collections;

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