#include "GuiGrid.h"

namespace vl
{
	namespace presentation
	{

/***********************************************************************
GuiFreeGrid
***********************************************************************/

		GuiFreeGrid::GuiFreeGrid()
		{
		}

		GuiFreeGrid::~GuiFreeGrid()
		{
			while(children.Count()>0)
			{
				GuiControl* control=children[0];
				RemoveControl(control);
				delete control;
			}
		}

		int GuiFreeGrid::GetChildCount()
		{
			return children.Count();
		}

		GuiControl* GuiFreeGrid::GetChild(int index)
		{
			return children[index];
		}

		int GuiFreeGrid::GetChildIndex(GuiControl* child)
		{
			return children.IndexOf(child);
		}

		void GuiFreeGrid::AddControl(GuiControl* child)
		{
			InsertControl(children.Count(), child);
		}

		void GuiFreeGrid::InsertControl(int index, GuiControl* child)
		{
			if(!children.Contains(child))
			{
				NotifyChildEntering(child);
				children.Insert(index, child);
				NotifyChildEntered(child);
			}
		}

		void GuiFreeGrid::RemoveControl(GuiControl* child)
		{
			int index=children.IndexOf(child);
			if(index!=-1)
			{
				NotifyChildLeaving(child);
				children.RemoveAt(index);
				NotifyChildLeaved(child);
			}
		}
	}
}