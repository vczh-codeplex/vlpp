#include "DockLayout.h"

namespace vl
{
	namespace presentation
	{
		using namespace collections;

/***********************************************************************
DockLayout
***********************************************************************/

		bool DockLayout::DockedHost::operator==(const DockedHost& dockedHost)const
		{
			return dockType==dockedHost.dockType && host==dockedHost.host;
		}

		bool DockLayout::DockedHost::operator!=(const DockedHost& dockedHost)const
		{
			return dockType!=dockedHost.dockType || host!=dockedHost.host;
		}

		int DockLayout::FindHost(LayoutHost* host)
		{
			for(int i=0;i<dockedHosts.Count();i++)
			{
				if(dockedHosts[i].host==host)
				{
					return i;
				}
			}
			return -1;
		}

		DockLayout::DockLayout()
		{
		}

		DockLayout::~DockLayout()
		{
		}

		void DockLayout::AdjustHosts()
		{
			Rect bounds(Point(), size);
			for(int i=0;i<dockedHosts.Count();i++)
			{
				DockedHost dockedHost=dockedHosts[i];
				Size suggestedBounds=dockedHost.host->GetSuggestedBounds();
				switch(dockedHost.dockType)
				{
				case Left:
					{
						Rect relativeBounds(
							bounds.x1,
							bounds.y1,
							bounds.x1+suggestedBounds.x,
							bounds.y2);
						dockedHost.host->SetMarginRelativeBounds(relativeBounds);
						bounds.x1+=suggestedBounds.x;
					}
					break;
				case Right:
					{
						Rect relativeBounds(
							bounds.x2-suggestedBounds.x,
							bounds.y1,
							bounds.x2,
							bounds.y2);
						dockedHost.host->SetMarginRelativeBounds(relativeBounds);
						bounds.x2-=suggestedBounds.x;
					}
					break;
				case Top:
					{
						Rect relativeBounds(
							bounds.x1,
							bounds.y1,
							bounds.x2,
							bounds.y1+suggestedBounds.y);
						dockedHost.host->SetMarginRelativeBounds(relativeBounds);
						bounds.y1+=suggestedBounds.y;
					}
					break;
				case Bottom:
					{
						Rect relativeBounds(
							bounds.x1,
							bounds.y2-suggestedBounds.y,
							bounds.x2,
							bounds.y2);
						dockedHost.host->SetMarginRelativeBounds(relativeBounds);
						bounds.y2-=suggestedBounds.y;
					}
					break;
				case None:
					dockedHost.host->SetMarginRelativeBounds(bounds);
					break;
				}
			}
		}

		void DockLayout::CalculateMinMax()
		{
			Size minOffset;
			Size minResult;
			Size maxResult(LayoutMaxSize, LayoutMaxSize);
			for(int i=0;i<dockedHosts.Count();i++)
			{
				DockedHost dockedHost=dockedHosts[i];
				Size hostSize=dockedHost.host->GetBounds().GetSize();
				Size minHostSize=hostSize;
				Size maxHostSize=hostSize;
				switch(dockedHost.dockType)
				{
				case Left:case Right:
					minHostSize.y=dockedHost.host->GetMinBounds().y;
					maxHostSize.y=dockedHost.host->GetMaxBounds().y;
					break;
				case Top:case Bottom:
					minHostSize.x=dockedHost.host->GetMinBounds().x;
					maxHostSize.x=dockedHost.host->GetMaxBounds().x;
					break;
				case None:
					minHostSize=dockedHost.host->GetMinBounds();
					maxHostSize=dockedHost.host->GetMaxBounds();
					break;
				}
				minHostSize+=minOffset;
				maxHostSize+=minOffset;

				switch(dockedHost.dockType)
				{
				case Left:case Right:
					{
						if(minResult.y<minHostSize.y)minResult.y=minHostSize.y;
						if(maxResult.y>maxHostSize.y)maxResult.y=maxHostSize.y;
						int offset=dockedHost.host->GetSuggestedBounds().x;
						minOffset.x+=offset;
						minSize.x+=offset;
					}
					break;
				case Top:case Bottom:
					{
						if(minResult.x<minHostSize.x)minResult.x=minHostSize.x;
						if(maxResult.x>maxHostSize.x)maxResult.x=maxHostSize.x;
						int offset=dockedHost.host->GetSuggestedBounds().y;
						minOffset.y+=offset;
						minSize.y+=offset;
					}
					break;
				case None:
					{
						if(minResult.x<minHostSize.x)minResult.x=minHostSize.x;
						if(minResult.y<minHostSize.y)minResult.y=minHostSize.y;
						if(maxResult.x>maxHostSize.x)maxResult.x=maxHostSize.x;
						if(maxResult.y>maxHostSize.y)maxResult.y=maxHostSize.y;
					}
					break;
				}
			}
			minSize=minResult;
			maxSize=maxResult;
		}

		int DockLayout::GetHostCount()
		{
			return dockedHosts.Count();
		}

		LayoutHost* DockLayout::GetHost(int index)
		{
			return dockedHosts[index].host;
		}

		void DockLayout::DockHost(DockType dockType, LayoutHost* host)
		{
			UndockHost(host);
			DockedHost dockedHost;
			dockedHost.dockType=dockType;
			dockedHost.host=host;
			dockedHosts.Add(dockedHost);
		}

		void DockLayout::UndockHost(LayoutHost* host)
		{
			int index=FindHost(host);
			if(index!=-1)
			{
				dockedHosts.RemoveAt(index);
			}
		}

		DockLayout::DockType DockLayout::GetDockType(LayoutHost* host)
		{
			return dockedHosts[FindHost(host)].dockType;
		}

		void DockLayout::ClearHosts()
		{
			dockedHosts.Clear();
		}
	}
}