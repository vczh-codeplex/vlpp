#include "GuiControl.h"
#include "GuiWindow.h"

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

		Ptr<IGuiSkin> GuiControl::GetSkin()
		{
			return skin;
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
						skin->AttachListener(window->GetSkinListener());
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
			NotifySkinChanged();
		}

		void GuiControl::NotifySkinChanged()
		{
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

		void GuiWindowBase::RedrawIfRequired()
		{
			if(needToRedraw)
			{
				nativeWindow->RedrawContent();
				needToRedraw=false;
			}
		}

		void GuiWindowBase::RequireRedraw()
		{
			needToRedraw=true;
		}

		void GuiWindowBase::Moving(Rect& value)
		{
			NotifyMoving(value);
			RedrawIfRequired();
		}

		void GuiWindowBase::Moved()
		{
			NotifyMoved(GetBounds());
			RedrawIfRequired();
		}

		void GuiWindowBase::Enabled()
		{
			RedrawIfRequired();
		}

		void GuiWindowBase::Disabled()
		{
			RedrawIfRequired();
		}

		void GuiWindowBase::GotFocus()
		{
			RedrawIfRequired();
		}

		void GuiWindowBase::LostFocus()
		{
			RedrawIfRequired();
		}

		void GuiWindowBase::Activated()
		{
			RedrawIfRequired();
		}

		void GuiWindowBase::Deactivated()
		{
			RedrawIfRequired();
		}

		void GuiWindowBase::Opened()
		{
			RedrawIfRequired();
		}

		void GuiWindowBase::Closing(bool& cancel)
		{
		}

		void GuiWindowBase::Closed()
		{
		}

		void GuiWindowBase::Paint()
		{
		}

		void GuiWindowBase::Destroying()
		{
			if(nativeWindow)
			{
				nativeWindow=0;
				if(!destructorInvoked)
				{
					delete this;
				}
			}
		}

		void GuiWindowBase::LeftButtonDown(const NativeWindowMouseInfo& info)
		{
			RedrawIfRequired();
		}

		void GuiWindowBase::LeftButtonUp(const NativeWindowMouseInfo& info)
		{
			RedrawIfRequired();
		}

		void GuiWindowBase::LeftButtonDoubleClick(const NativeWindowMouseInfo& info)
		{
			RedrawIfRequired();
		}

		void GuiWindowBase::RightButtonDown(const NativeWindowMouseInfo& info)
		{
			RedrawIfRequired();
		}

		void GuiWindowBase::RightButtonUp(const NativeWindowMouseInfo& info)
		{
			RedrawIfRequired();
		}

		void GuiWindowBase::RightButtonDoubleClick(const NativeWindowMouseInfo& info)
		{
			RedrawIfRequired();
		}

		void GuiWindowBase::MiddleButtonDown(const NativeWindowMouseInfo& info)
		{
			RedrawIfRequired();
		}

		void GuiWindowBase::MiddleButtonUp(const NativeWindowMouseInfo& info)
		{
			RedrawIfRequired();
		}

		void GuiWindowBase::MiddleButtonDoubleClick(const NativeWindowMouseInfo& info)
		{
			RedrawIfRequired();
		}

		void GuiWindowBase::HorizontalWheel(const NativeWindowMouseInfo& info)
		{
			RedrawIfRequired();
		}

		void GuiWindowBase::VerticalWheel(const NativeWindowMouseInfo& info)
		{
			RedrawIfRequired();
		}

		void GuiWindowBase::MouseMoving(const NativeWindowMouseInfo& info)
		{
			RedrawIfRequired();
		}

		void GuiWindowBase::MouseEntered()
		{
			RedrawIfRequired();
		}

		void GuiWindowBase::MouseLeaved()
		{
			RedrawIfRequired();
		}

		void GuiWindowBase::KeyDown(int code, bool alt)
		{
			RedrawIfRequired();
		}

		void GuiWindowBase::KeyUp(int code, bool alt)
		{
			RedrawIfRequired();
		}

		void GuiWindowBase::SysKeyDown(int code, bool alt)
		{
			RedrawIfRequired();
		}

		void GuiWindowBase::SysKeyUp(int code, bool alt)
		{
			RedrawIfRequired();
		}

		void GuiWindowBase::Char(wchar_t keyChar)
		{
			RedrawIfRequired();
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
			,needToRedraw(false)
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

		IGuiSkinListener* GuiWindowBase::GetSkinListener()
		{
			return this;
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