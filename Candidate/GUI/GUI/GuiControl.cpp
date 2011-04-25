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

		class GuiWindowBaseListener : public Object, public IGuiSkinListener, public INativeWindowListener
		{
		protected:
			bool						needToRedraw;
			INativeWindow*				window;

			void RedrawIfRequired()
			{
				if(needToRedraw)
				{
					window->RedrawContent();
				}
			}
		public:
			GuiWindowBaseListener(INativeWindow* _window)
				:needToRedraw(false)
				,window(_window)
			{
			}

			~GuiWindowBaseListener()
			{
			}

			void RequireRedraw()
			{
				needToRedraw=true;
			}

			void Moving(Rect& bounds)
			{
				RedrawIfRequired();
			}

			void Moved()
			{
				RedrawIfRequired();
			}

			void Enabled()
			{
				RedrawIfRequired();
			}

			void Disabled()
			{
				RedrawIfRequired();
			}

			void GotFocus()
			{
				RedrawIfRequired();
			}

			void LostFocus()
			{
				RedrawIfRequired();
			}

			void Activated()
			{
				RedrawIfRequired();
			}

			void Deactivated()
			{
				RedrawIfRequired();
			}

			void Opened()
			{
				RedrawIfRequired();
			}

			void LeftButtonDown(const NativeWindowMouseInfo& info)
			{
				RedrawIfRequired();
			}

			void LeftButtonUp(const NativeWindowMouseInfo& info)
			{
				RedrawIfRequired();
			}

			void LeftButtonDoubleClick(const NativeWindowMouseInfo& info)
			{
				RedrawIfRequired();
			}

			void RightButtonDown(const NativeWindowMouseInfo& info)
			{
				RedrawIfRequired();
			}

			void RightButtonUp(const NativeWindowMouseInfo& info)
			{
				RedrawIfRequired();
			}

			void RightButtonDoubleClick(const NativeWindowMouseInfo& info)
			{
				RedrawIfRequired();
			}

			void MiddleButtonDown(const NativeWindowMouseInfo& info)
			{
				RedrawIfRequired();
			}

			void MiddleButtonUp(const NativeWindowMouseInfo& info)
			{
				RedrawIfRequired();
			}

			void MiddleButtonDoubleClick(const NativeWindowMouseInfo& info)
			{
				RedrawIfRequired();
			}

			void HorizontalWheel(const NativeWindowMouseInfo& info)
			{
				RedrawIfRequired();
			}

			void VerticalWheel(const NativeWindowMouseInfo& info)
			{
				RedrawIfRequired();
			}

			void MouseMoving(const NativeWindowMouseInfo& info)
			{
				RedrawIfRequired();
			}

			void MouseEntered()
			{
				RedrawIfRequired();
			}

			void MouseLeaved()
			{
				RedrawIfRequired();
			}

			void KeyDown(int code, bool alt)
			{
				RedrawIfRequired();
			}

			void KeyUp(int code, bool alt)
			{
				RedrawIfRequired();
			}

			void SysKeyDown(int code, bool alt)
			{
				RedrawIfRequired();
			}

			void SysKeyUp(int code, bool alt)
			{
				RedrawIfRequired();
			}

			void Char(wchar_t keyChar)
			{
				RedrawIfRequired();
			}
		};

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
				nativeWindow->UninstallListener(skinListener.Cast<INativeWindowListener>().Obj());
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
			,skinListener(0)
		{
			nativeWindow=GetCurrentController()->CreateNativeWindow();
			nativeWindow->InstallListener(this);

			Ptr<GuiWindowBaseListener> listener=new GuiWindowBaseListener(nativeWindow);
			nativeWindow->InstallListener(listener.Obj());
			skinListener=listener;

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
			return skinListener.Obj();
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