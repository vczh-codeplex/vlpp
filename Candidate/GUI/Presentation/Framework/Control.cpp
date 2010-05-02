#include "Control.h"

namespace vl
{
	namespace presentation
	{

/***********************************************************************
Control
***********************************************************************/

		void Control::RequestRefresh()
		{
		}

		void Control::UpdateGrid()
		{
			if(grid && !updatingGrid)
			{
				updatingGrid=true;
				grid->UpdateMinMax();
				if(autoSizeMode)
				{
					SetMinSize(grid->GetMinSize());
					SetMaxSize(grid->GetMaxSize());
				}
				grid->SetSize(GetVisualSize());
				grid->ApplyLayout();
				updatingGrid=false;
			}
		}

		void Control::UpdateHost()
		{
			Point location=layoutHost.GetRealBounds().LeftTop();
			Size size=layoutHost.GetRealBounds().GetSize();
			if(location!=visualLocation)
			{
				visualLocation=location;
				VisualLocationChanged(this, NotifyEventArgs());
			}
			if(size!=visualSize)
			{
				visualSize=size;
				if(!updatingGrid)
				{
					grid->SetSize(GetVisualSize());
					grid->ApplyLayout();
				}
				VisualSizeChanged(this, NotifyEventArgs());
			}
		}

		bool Control::RequestFocus()
		{
			return IsVisuallyVisible() && IsVisuallyEnabled();
		}
		
		Control::Control()
			:parent(0)
			,grid(0)
			,isVisible(true)
			,isEnabled(true)
			,isFocused(false)
			,isMouseOver(false)
			,isTabStopEnabled(false)
			,isConsumeTab(false)
			,altShortcutKey(-1)
			,updatingGrid(false)
		{
		}

		Control::~Control()
		{
		}

		void Control::ProcessMessage(int message, void* arguments)
		{
		}

		Control* Control::GetParent()
		{
			return parent;
		}

		Grid* Control::GetGrid()
		{
			return grid;
		}

		void Control::SetGrid(Grid* value)
		{
			if(grid==value)return;
			if(value && value->GetHostControl())
			{
				return;
			}
			if(grid)
			{
				for(int i=0;i<grid->GetControls().Count();i++)
				{
					Control* child=grid->GetControls()[i];
					child->parent=0;
					child->ParentChanged(child, NotifyEventArgs());
				}
				grid->hostControl=0;
			}
			grid=value;
			if(grid)
			{
				grid->hostControl=this;
				for(int i=0;i<grid->GetControls().Count();i++)
				{
					Control* child=grid->GetControls()[i];
					child->parent=this;
					child->ParentChanged(child, NotifyEventArgs());
				}
			}
			UpdateGrid();
			GridChanged(this, NotifyEventArgs());
		}

		WString Control::GetText()
		{
			return text;
		}

		void Control::SetText(const WString& value)
		{
			if(text==value)return;
			text=value;
			RequestRefresh();
			TextChanged(this, NotifyEventArgs());
		}

		WString Control::GetName()
		{
			return name;
		}

		void Control::SetName(const WString& value)
		{
			if(name==value)return;
			name=value;
			NameChanged(this, NotifyEventArgs());
		}

		Size Control::GetMinSize()
		{
			return layoutHost.GetMinSize();
		}

		void Control::SetMinSize(Size value)
		{
			if(layoutHost.GetMinSize()==value)return;
			layoutHost.SetMinSize(value);
			UpdateHost();
			MinSizeChanged(this, NotifyEventArgs());
		}

		Size Control::GetMaxSize()
		{
			return layoutHost.GetMaxSize();
		}

		void Control::SetMaxSize(Size value)
		{
			if(layoutHost.GetMaxSize()==value)return;
			layoutHost.SetMaxSize(value);
			UpdateHost();
			MaxSizeChanged(this, NotifyEventArgs());
		}

		Point Control::GetLocation()
		{
			return layoutHost.GetBounds().LeftTop();
		}

		void Control::SetLocation(Point value)
		{
			if(layoutHost.GetBounds().LeftTop()==value)return;
			layoutHost.SetBounds(Rect(value, layoutHost.GetBounds().GetSize()));
			UpdateHost();
			LocationChanged(this, NotifyEventArgs());
		}

		Size Control::GetSize()
		{
			return layoutHost.GetBounds().GetSize();
		}

		void Control::SetSize(Size value)
		{
			if(layoutHost.GetBounds().GetSize()==value)return;
			layoutHost.SetBounds(Rect(layoutHost.GetBounds().LeftTop(), value));
			UpdateHost();
			SizeChanged(this, NotifyEventArgs());
		}

		Point Control::GetClientLocation()
		{
			return GetLocation();
		}

		Size Control::GetClientSize()
		{
			return GetSize();
		}

		void Control::SetClientSize(Size value)
		{
			if(GetClientSize()==value)return;
			SetSize(value);
		}

		Margin Control::GetMargin()
		{
			return layoutHost.GetMargin();
		}

		void Control::SetMargin(Margin value)
		{
			if(layoutHost.GetMargin()==value)return;
			layoutHost.SetMargin(value);
			UpdateHost();
			MarginChanged(this, NotifyEventArgs());
		}

		bool Control::GetAutoSizeMode()
		{
			return autoSizeMode;
		}

		void Control::SetAutoSizeMode(bool value)
		{
			if(autoSizeMode==value)return;
			autoSizeMode=value;
			AutoSizeModeChanged(this, NotifyEventArgs());
		}

		Point Control::GetVisualLocation()
		{
			return layoutHost.GetRealBounds().LeftTop();
		}

		Size Control::GetVisualSize()
		{
			return layoutHost.GetRealBounds().GetSize();
		}

		Size Control::GetVisualClientSize()
		{
			return GetVisualSize();
		}

		bool Control::GetVisible()
		{
			return isVisible;
		}

		void Control::SetVisible(bool value)
		{
			if(isVisible==value)return;
			isVisible=value;
			RequestRefresh();
			VisibleChanged(this, NotifyEventArgs());
		}

		bool Control::GetEnabled()
		{
			return isEnabled;
		}

		void Control::SetEnabled(bool value)
		{
			if(isEnabled==value)return;
			isEnabled=value;
			RequestRefresh();
			EnabledChanged(this, NotifyEventArgs());
		}

		bool Control::GetFocused()
		{
			return isFocused;
		}

		void Control::SetFocused(bool value)
		{
			if(isFocused==value)return;
			if(value && !RequestFocus())return;
			isFocused=value;
			RequestRefresh();
			FocusedChanged(this, NotifyEventArgs());
		}

		bool Control::GetTabStopEnabled()
		{
			return isTabStopEnabled;
		}

		void Control::SetTabStopEnabled(bool value)
		{
			if(isTabStopEnabled==value)return;
			isTabStopEnabled=value;
			TabStopEnabledChanged(this, NotifyEventArgs());
		}

		bool Control::GetConsumeTab()
		{
			return isConsumeTab;
		}

		void Control::SetConsumeTab(bool value)
		{
			if(isConsumeTab==value)return;
			isConsumeTab=value;
			ConsumeTabChanged(this, NotifyEventArgs());
		}

		int Control::GetAltShortcutKey()
		{
			return altShortcutKey;
		}

		void Control::SetAltShortcutKey(int value)
		{
			if(altShortcutKey==value)return;
			altShortcutKey=value;
			AltShortcutKeyChanged(this, NotifyEventArgs());
		}

		bool Control::IsMouseOver()
		{
			return isMouseOver;
		}

		bool Control::IsVisuallyVisible()
		{
			if(parent)
			{
				return parent->IsVisuallyVisible() && GetVisible();
			}
			else
			{
				return false;
			}
		}

		bool Control::IsVisuallyEnabled()
		{
			if(parent)
			{
				return parent->IsVisuallyEnabled() && GetEnabled();
			}
			else
			{
				return true;
			}
		}

/***********************************************************************
Grid
***********************************************************************/
		
		LayoutHost* Grid::GetControlLayoutHost(Control* control)
		{
			return &control->layoutHost;
		}

		Grid::Grid()
			:layout(0)
			,hostControl(0)
		{
		}

		Grid::~Grid()
		{
		}

		Control* Grid::GetHostControl()
		{
			return hostControl;
		}

		const IReadonlyList<Control*>& Grid::GetControls()
		{
			return children.Wrap();
		}

		Size Grid::GetMinSize()
		{
			return layout->GetMinSize();
		}

		Size Grid::GetMaxSize()
		{
			return layout->GetMaxSize();
		}

		Size Grid::GetSize()
		{
			return layout->GetSize();
		}

		void Grid::SetSize(Size value)
		{
			layout->SetSize(value);
		}

		void Grid::UpdateMinMax()
		{
			layout->CalculateMinMax();
		}

		void Grid::ApplyLayout()
		{
			layout->AdjustHosts();
			for(int i=0;i<children.Count();i++)
			{
				children[i]->UpdateHost();
			}
		}
	}
}