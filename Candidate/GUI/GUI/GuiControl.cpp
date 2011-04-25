#include "GuiControl.h"

namespace vl
{
	namespace presentation
	{

/***********************************************************************
GuiControl::Grid
***********************************************************************/

		GuiControl::Grid::Grid()
			:ownedControl(0)
		{
		}

		GuiControl::Grid::~Grid()
		{
		}

		GuiControl* GuiControl::Grid::GetOwnedControl()
		{
			return ownedControl;
		}

		bool GuiControl::Grid::SetOwnedControl(GuiControl* control)
		{
			if(ownedControl)
			{
				ownedControl=control;
				return true;
			}
			else
			{
				return false;
			}
		}

/***********************************************************************
GuiControl
***********************************************************************/

		void GuiControl::NotifySetParent(GuiControl* value)
		{
			if(parent)
			{
				GuiWindowBase* parentWindow=dynamic_cast<GuiWindowBase*>(parent);
				if(parentWindow)
				{
					NotifyAttachedToWindow(0);
				}
			}
			parent=value;
			if(parent)
			{
				GuiWindowBase* parentWindow=dynamic_cast<GuiWindowBase*>(parent);
				if(parentWindow)
				{
					NotifyAttachedToWindow(parentWindow);
				}
			}
		}

		void GuiControl::NotifyAttachedToWindow(GuiWindowBase* window)
		{
			skin=0;
			if(window)
			{
				IGuiSkinBuilder* builder=GetCurrentApplication()->GetSkinBuilder(GetSkinBuilderName());
				if(builder)
				{
					skin=builder->Build(window->GetContainingNativeWindow());
					skin->SetBounds(bounds);
				}
			}
			if(container)
			{
				int count=container->GetChildCount();
				for(int i=0;i<count;i++)
				{
					GuiControl* child=container->GetChild(i);
					child->NotifyAttachedToWindow(window);
				}
			}
		}

		void GuiControl::NotifyChildEntering(GuiControl* value)
		{
		}

		void GuiControl::NotifyChildEntered(GuiControl* value)
		{
			if(skin)
			{
				skin->InsertChild(container->GetChildIndex(value), value->skin.Obj());
			}
		}

		void GuiControl::NotifyChildLeaving(GuiControl* value)
		{
			if(skin)
			{
				skin->RemoveChild(value->skin.Obj());
			}
		}

		void GuiControl::NotifyChildLeaved(GuiControl* value)
		{
		}

		void GuiControl::NotifyMoving(Rect& value)
		{
		}

		void GuiControl::NotifyMoved(Rect value)
		{
		}

		GuiControl::GuiControl()
			:parent(0)
		{
		}

		GuiControl::~GuiControl()
		{
		}

		GuiControl* GuiControl::GetParent()
		{
			return parent;
		}

		const WString& GuiControl::GetControlName()
		{
			return name;
		}

		void GuiControl::SetControlName(const WString& value)
		{
			name=value;
		}

		GuiControl::Grid* GuiControl::GetContainer()
		{
			return container.Obj();
		}

		void GuiControl::SetContainer(Grid* value)
		{
			if(value->SetOwnedControl(this))
			{
				if(container)
				{
					int count=container->GetChildCount();
					for(int i=0;i<count;i++)
					{
						GuiControl* child=container->GetChild(i);
						NotifyChildLeaving(child);
						child->NotifySetParent(0);
						NotifyChildLeaved(child);
					}
				}
				container=value;
				if(container)
				{
					int count=container->GetChildCount();
					for(int i=0;i<count;i++)
					{
						GuiControl* child=container->GetChild(i);
						NotifyChildEntering(child);
						child->NotifySetParent(this);
						NotifyChildEntered(child);
					}
				}
			}
		}

		Rect GuiControl::GetBounds()
		{
			return bounds;
		}

		void GuiControl::SetBounds(Rect value)
		{
			NotifyMoving(value);
			NotifyMoved(value);
			bounds=value;
			if(skin)
			{
				skin->SetBounds(bounds);
			}
		}

/***********************************************************************
GuiApplication
***********************************************************************/

		GuiApplication::GuiApplication()
		{
		}

		GuiApplication::~GuiApplication()
		{
		}

		void GuiApplication::SetSkinBuilder(const WString& name, IGuiSkinBuilder* builder)
		{
			skinBuilders.Set(name, builder);
		}

		IGuiSkinBuilder* GuiApplication::GetSkinBuilder(const WString& name)
		{
			int index=skinBuilders.Keys().IndexOf(name);
			return index==-1?0:skinBuilders.Values()[index].Obj();
		}

		GuiApplication* currentApplication=0;

		GuiApplication* GetCurrentApplication()
		{
			return currentApplication;
		}
	}
}

/***********************************************************************
NativeMain
***********************************************************************/

using namespace vl;
using namespace vl::presentation;

void NativeMain()
{
	GuiApplication application;
	currentApplication=&application;
	GuiMain();
}