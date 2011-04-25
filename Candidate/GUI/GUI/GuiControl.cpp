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

		Rect GuiControl::GetBoundsForSkin()
		{
			return bounds;
		}

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
					if(skin)
					{
						skin->SetBounds(GetBoundsForSkin());
					}
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
			bounds=value;
			if(skin)
			{
				skin->SetBounds(GetBoundsForSkin());
			}
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
		}
		
		Size GuiControl::GetClientSize()
		{
			return GetBounds().GetSize();
		}

		void GuiControl::SetClientSize(Size value)
		{
			SetBounds(Rect(GetBounds().LeftTop(), value));
		}

/***********************************************************************
GuiWindowBase
***********************************************************************/

		void GuiWindowBase::Moving(Rect& value)
		{
			NotifyMoving(value);
		}

		void GuiWindowBase::Moved()
		{
			NotifyMoved(GetBounds());
		}

		void GuiWindowBase::Destroying()
		{
			if(nativeWindow)
			{
				INativeWindow* window=nativeWindow;
				nativeWindow=0;
				if(!destructorInvoked)
				{
					delete this;
				}
			}
		}

		//----------------------------------------------------------------------

		void GuiWindowBase::InitializeWindow()
		{
			NotifyAttachedToWindow(this);
		}

		Rect GuiWindowBase::GetBoundsForSkin()
		{
			return Rect(Point(0, 0), GetContainingNativeWindow()->GetClientSize());
		}

		GuiWindowBase::GuiWindowBase()
			:nativeWindow(0)
			,destructorInvoked(false)
		{
			nativeWindow=GetCurrentController()->CreateNativeWindow();
			nativeWindow->InstallListener(this);
			GetCurrentApplication()->RegisterWindow(this);
		}

		GuiWindowBase::~GuiWindowBase()
		{
			destructorInvoked=true;
			if(nativeWindow)
			{
				GetCurrentController()->DestroyNativeWindow(nativeWindow);
			}
			GetCurrentApplication()->UnregisterWindow(this);
		}

		INativeWindow* GuiWindowBase::GetContainingNativeWindow()
		{
			return nativeWindow;
		}

/***********************************************************************
GuiApplication
***********************************************************************/

		void GuiApplication::RegisterWindow(GuiWindowBase* window)
		{
			int index=windowBases.IndexOf(window);
			if(index==-1)
			{
				windowBases.Add(window);
			}
		}

		void GuiApplication::UnregisterWindow(GuiWindowBase* window)
		{
			int index=windowBases.IndexOf(window);
			if(index!=-1)
			{
				windowBases.RemoveAt(index);
			}
		}

		GuiApplication::GuiApplication()
			:mainWindow(0)
		{
		}

		GuiApplication::~GuiApplication()
		{
			while(windowBases.Count()>0)
			{
				GuiWindowBase* window=windowBases[windowBases.Count()-1];
				delete window;
			}
		}

		IGuiSkinBuilder* GuiApplication::GetSkinBuilder(const WString& name)
		{
			int index=skinBuilders.Keys().IndexOf(name);
			return index==-1?0:skinBuilders.Values()[index].Obj();
		}

		void GuiApplication::SetSkinBuilder(const WString& name, IGuiSkinBuilder* builder)
		{
			skinBuilders.Set(name, builder);
		}
		
		void GuiApplication::Run(GuiWindow* window)
		{
			if(!mainWindow)
			{
				mainWindow=window;
				GetCurrentController()->Run(window->GetContainingNativeWindow());
			}
		}

		GuiWindow* GuiApplication::GetMainWindow()
		{
			return mainWindow;
		}

		GuiApplication* currentApplication=0;

		GuiApplication* GetCurrentApplication()
		{
			return currentApplication;
		}

/***********************************************************************
GuiWindow
***********************************************************************/

		WString GuiWindow::GetSkinBuilderName()
		{
			return L"vl::presentation::GuiWindow";
		}

		GuiWindow::GuiWindow()
		{
			InitializeWindow();
		}

		GuiWindow::~GuiWindow()
		{
		}

		WString GuiWindow::GetTitle()
		{
			return GetContainingNativeWindow()->GetTitle();
		}

		void GuiWindow::SetTitle(const WString& value)
		{
			GetContainingNativeWindow()->SetTitle(value);
		}

		void GuiWindow::MoveToScreenCenter()
		{
			INativeWindow* window=GetContainingNativeWindow();
			INativeScreen* screen=GetCurrentController()->GetScreen(window);
			if(screen)
			{
				Rect client=screen->GetClientBounds();
				Rect bounds=window->GetBounds();
				int x=(client.Width()-bounds.Width())/2+client.Left();
				int y=(client.Height()-bounds.Height())/2+client.Top();
				window->SetBounds(Rect(Point(x, y), bounds.GetSize()));
			}
		}

		void GuiWindow::Show()
		{
			GetContainingNativeWindow()->Show();
		}

		void GuiWindow::Close()
		{
			GetContainingNativeWindow()->Hide();
		}

		Rect GuiWindow::GetBounds()
		{
			return GetContainingNativeWindow()->GetBounds();
		}

		void GuiWindow::SetBounds(Rect value)
		{
			GetContainingNativeWindow()->SetBounds(value);
		}
		
		Size GuiWindow::GetClientSize()
		{
			return GetContainingNativeWindow()->GetClientSize();
		}

		void GuiWindow::SetClientSize(Size value)
		{
			GetContainingNativeWindow()->SetClientSize(value);
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