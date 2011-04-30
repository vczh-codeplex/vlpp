#include "GuiControl.h"
#include "GuiWindow.h"

namespace vl
{
	namespace presentation
	{

/***********************************************************************
GuiControl::Grid
***********************************************************************/

		void GuiControl::Grid::NotifyChildEntering(GuiControl* value)
		{
			if(ownedControl)
			{
				ownedControl->NotifyChildEntering(value);
			}
		}

		void GuiControl::Grid::NotifyChildEntered(GuiControl* value)
		{
			if(ownedControl)
			{
				ownedControl->NotifyChildEntered(value);
			}
		}

		void GuiControl::Grid::NotifyChildLeaving(GuiControl* value)
		{
			if(ownedControl)
			{
				ownedControl->NotifyChildLeaving(value);
			}
		}

		void GuiControl::Grid::NotifyChildLeaved(GuiControl* value)
		{
			if(ownedControl)
			{
				ownedControl->NotifyChildLeaved(value);
			}
		}

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
			if(!ownedControl == !control) return false;
			ownedControl=control;
			return true;
		}

/***********************************************************************
GuiControl
***********************************************************************/

		void GuiControl::TrackChild(GuiControl* child)
		{
			if(child)
			{
				RequireTracking();
			}
			else
			{
				ReleaseTracking();
			}
			trackingControl=child;
		}

		bool GuiControl::FocusChild(GuiControl* child)
		{
			GuiControl* oldFocusedControl=focusedControl;
			focusedControl=child;
			if(oldFocusedControl)
			{
				oldFocusedControl->NotifyLostFocus();
			}
			if(IsFocusing())
			{
				focusedControl->NotifyGotFocus();
				return true;
			}
			if(RequireFocus())
			{
				return true;
			}
			else
			{
				return false;
			}
		}

		Rect GuiControl::GetBoundsForSkin()
		{
			return bounds;
		}

		Ptr<IGuiSkin> GuiControl::GetSkin()
		{
			return skin;
		}

		eventargs::MouseInfo GuiControl::Offset(GuiControl* child, const eventargs::MouseInfo& info)
		{
			eventargs::MouseInfo r=info;
			Point offset=child->GetBounds().LeftTop();
			r.x-=offset.x;
			r.y-=offset.y;
			return r;
		}

		void GuiControl::NotifySetParent(GuiControl* value)
		{
			if(parent)
			{
				GuiWindowBase* parentWindow=parent->GetAttachedWindow();
				if(parentWindow)
				{
					NotifyAttachedToWindow(0);
				}
			}
			parent=value;
			if(parent)
			{
				GuiWindowBase* parentWindow=parent->GetAttachedWindow();
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
			attachedWindow=window;
			NotifySkinChanged();
		}

		void GuiControl::NotifySkinChanged()
		{
			if(skin)
			{
				skin->SetBounds(GetBoundsForSkin());
			}
		}

		void GuiControl::NotifyChildEntering(GuiControl* value)
		{
		}

		void GuiControl::NotifyChildEntered(GuiControl* value)
		{
			if(skin)
			{
				value->NotifySetParent(this);
				skin->InsertChild(container->GetChildIndex(value), value->skin.Obj());
				skin->GetListener()->RequireRedraw();
			}
		}

		void GuiControl::NotifyChildLeaving(GuiControl* value)
		{
			if(skin)
			{
				value->NotifySetParent(0);
				skin->RemoveChild(value->skin.Obj());
				skin->GetListener()->RequireRedraw();
			}
			if(focusedControl==value) focusedControl=0;
			if(enteredControl==value) enteredControl=0;
			if(trackingControl==value) trackingControl=0;
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

		GuiControl* GuiControl::NotifyMouseDown(eventargs::MouseButton button, const eventargs::MouseInfo& info)
		{
			GuiControl* child=GetChildFromPoint(Point(info.x, info.y));
			if(child)
			{
				child->NotifyMouseDown(button, Offset(child, info));
				return child;
			}
			else
			{
				return this;
			}
		}

		GuiControl* GuiControl::NotifyMouseMove(const eventargs::MouseInfo& info)
		{
			GuiControl* child=GetChildFromPoint(Point(info.x, info.y));
			if(enteredControl!=child)
			{
				if(enteredControl)
				{
					enteredControl->NotifyMouseLeaved();
				}
				enteredControl=child;
				if(enteredControl)
				{
					enteredControl->NotifyMouseEntered();
				}
			}
			if(child || trackingControl)
			{
				GuiControl* control=trackingControl?trackingControl:child;
				control->NotifyMouseMove(Offset(control, info));
			}
			return child?child:this;
		}

		GuiControl* GuiControl::NotifyMouseUp(eventargs::MouseButton button, const eventargs::MouseInfo& info)
		{
			GuiControl* child=trackingControl?trackingControl:GetChildFromPoint(Point(info.x, info.y));
			if(child)
			{
				child->NotifyMouseUp(button, Offset(child, info));
				return child;
			}
			else
			{
				return this;
			}
		}

		GuiControl* GuiControl::NotifyMouseDoubleClick(eventargs::MouseButton button, const eventargs::MouseInfo& info)
		{
			GuiControl* child=trackingControl?trackingControl:GetChildFromPoint(Point(info.x, info.y));
			if(child)
			{
				child->NotifyMouseDoubleClick(button, Offset(child, info));
				return child;
			}
			else
			{
				return this;
			}
		}

		GuiControl* GuiControl::NotifyMouseHorizontalWheel(const eventargs::MouseInfo& info)
		{
			if(focusedControl)
			{
				focusedControl->NotifyMouseHorizontalWheel(Offset(focusedControl, info));
				return focusedControl;
			}
			else
			{
				return this;
			}
		}

		GuiControl* GuiControl::NotifyMouseVerticalWheel(const eventargs::MouseInfo& info)
		{
			if(focusedControl)
			{
				focusedControl->NotifyMouseVerticalWheel(Offset(focusedControl, info));
				return focusedControl;
			}
			else
			{
				return this;
			}
		}

		GuiControl* GuiControl::NotifyKeyDown(int code, bool alt)
		{
			if(IsFocusing() && IsTabAwayEnabled())
			{
				if(attachedWindow && code==VKEY_TAB && !alt)
				{
					if(GetCurrentController()->IsKeyPressing(VKEY_SHIFT))
					{
						attachedWindow->FocusPreviousControl();
					}
					else
					{
						attachedWindow->FocusNextControl();
					}
				}
				return this;
			}
			if(focusedControl)
			{
				focusedControl->NotifyKeyDown(code, alt);
				return focusedControl;
			}
			else
			{
				return this;
			}
		}

		GuiControl* GuiControl::NotifyKeyUp(int code, bool alt)
		{
			if(focusedControl)
			{
				focusedControl->NotifyKeyUp(code, alt);
				return focusedControl;
			}
			else
			{
				return this;
			}
		}

		GuiControl* GuiControl::NotifySysKeyDown(int code, bool alt)
		{
			if(focusedControl)
			{
				focusedControl->NotifySysKeyDown(code, alt);
				return focusedControl;
			}
			else
			{
				return this;
			}
		}

		GuiControl* GuiControl::NotifySysKeyUp(int code, bool alt)
		{
			if(focusedControl)
			{
				focusedControl->NotifySysKeyUp(code, alt);
				return focusedControl;
			}
			else
			{
				return this;
			}
		}

		GuiControl* GuiControl::NotifyChar(wchar_t keyChar)
		{
			if(focusedControl)
			{
				focusedControl->NotifyChar(keyChar);
				return focusedControl;
			}
			else
			{
				return this;
			}
		}

		void GuiControl::NotifyMouseEntered()
		{
		}

		void GuiControl::NotifyMouseLeaved()
		{
			if(enteredControl)
			{
				enteredControl->NotifyMouseLeaved();
				enteredControl=0;
			}
		}

		GuiControl* GuiControl::NotifyGotFocus()
		{
			return focusedControl?focusedControl->NotifyGotFocus():this;
		}

		GuiControl* GuiControl::NotifyLostFocus()
		{
			return focusedControl?focusedControl->NotifyLostFocus():this;
		}

		GuiControl::GuiControl()
			:parent(0)
			,attachedWindow(0)
			,tabStop(true)
			,tabAway(true)
			,focusedControl(0)
			,enteredControl(0)
			,trackingControl(0)
		{
		}

		GuiControl::~GuiControl()
		{
		}

		GuiControl* GuiControl::GetParent()
		{
			return parent;
		}

		GuiWindowBase* GuiControl::GetAttachedWindow()
		{
			return attachedWindow;
		}

		GuiControl* GuiControl::GetChildFromPoint(Point value)
		{
			if(skin && container)
			{
				int count=container->GetChildCount();
				for(int i=0;i<count;i++)
				{
					GuiControl* child=container->GetChild(i);
					Ptr<IGuiSkin> childSkin=child->GetSkin();
					if(childSkin && childSkin->ContainsPoint(value))
					{
						return child;
					}
				}
			}
			return 0;
		}

		const WString& GuiControl::GetControlName()
		{
			return name;
		}

		void GuiControl::SetControlName(const WString& value)
		{
			name=value;
		}

		void GuiControl::RequireTracking()
		{
			if(parent)
			{
				parent->TrackChild(this);
			}
		}

		void GuiControl::ReleaseTracking()
		{
			if(parent)
			{
				parent->TrackChild(0);
			}
		}

		bool GuiControl::IsTracking()
		{
			return parent&&parent->trackingControl==this;
		}

		bool GuiControl::RequireFocus()
		{
			if(parent)
			{
				return parent->FocusChild(this);
			}
			else
			{
				return false;
			}
		}

		bool GuiControl::IsFocusing()
		{
			return parent&&parent->IsFocusing()&&parent->focusedControl==this;
		}

		GuiControl* GuiControl::GetPreviousFocusControl()
		{
			if(focusedControl)
			{
				GuiControl* control=focusedControl->GetPreviousFocusControl();
				if(control) return control;
				control=GetLastFocusControl(focusedControl);
				if(control) return control;
			}
			return parent?0:GetLastFocusControl(0);
		}

		GuiControl* GuiControl::GetNextFocusControl()
		{
			if(focusedControl)
			{
				GuiControl* control=focusedControl->GetNextFocusControl();
				if(control) return control;
				control=GetFirstFocusControl(focusedControl);
				if(control) return control;
			}
			return parent?0:GetFirstFocusControl(0);
		}

		GuiControl* GuiControl::GetFirstFocusControl(GuiControl* after)
		{
			if(IsTabStopEnabled())
			{
				return this;
			}
			if(container)
			{
				int current=container->GetChildIndex(after);
				int count=container->GetChildCount();
				while(++current<count)
				{
					GuiControl* control=container->GetChild(current)->GetFirstFocusControl(0);
					if(control)
					{
						return control;
					}
				}
			}
			return 0;
		}

		GuiControl* GuiControl::GetLastFocusControl(GuiControl* before)
		{
			if(IsTabStopEnabled())
			{
				return this;
			}
			if(container)
			{
				int current=container->GetChildIndex(before);
				if(current==-1)
				{
					current=container->GetChildCount();
				}
				while(--current>=0)
				{
					GuiControl* control=container->GetChild(current)->GetLastFocusControl(0);
					if(control)
					{
						return control;
					}
				}
			}
			return 0;
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
			Size value=GetBounds().GetSize();
			if(skin)
			{
				value=skin->GetClientSizeFromBoundsSize(value);
			}
			return value;
		}

		void GuiControl::SetClientSize(Size value)
		{
			if(skin)
			{
				value=skin->GetBoundsSizeFromClientSize(value);
			}
			SetBounds(Rect(GetBounds().LeftTop(), value));
		}

		bool GuiControl::GetTabStop()
		{
			return tabStop;
		}

		void GuiControl::SetTabStop(bool value)
		{
			tabStop=value;
		}

		bool GuiControl::IsTabStopEnabled()
		{
			return tabStop && (!container || container->GetChildCount()==0);
		}

		bool GuiControl::GetTabAway()
		{
			return tabAway;
		}

		void GuiControl::SetTabAway(bool value)
		{
			tabAway=value;
		}

		bool GuiControl::IsTabAwayEnabled()
		{
			return tabAway && (!container || container->GetChildCount()==0);
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
			NotifyGotFocus();
			RedrawIfRequired();
		}

		void GuiWindowBase::LostFocus()
		{
			NotifyLostFocus();
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

		eventargs::MouseInfo Convert(const NativeWindowMouseInfo& info)
		{
			eventargs::MouseInfo r;
			r.ctrl=info.ctrl;
			r.shift=info.shift;
			r.left=info.left;
			r.middle=info.middle;
			r.right=info.right;
			r.x=info.x;
			r.y=info.y;
			r.wheel=info.wheel;
			return r;
		}

		void GuiWindowBase::LeftButtonDown(const NativeWindowMouseInfo& info)
		{
			NotifyMouseDown(eventargs::LeftButton, Convert(info));
			RedrawIfRequired();
		}

		void GuiWindowBase::LeftButtonUp(const NativeWindowMouseInfo& info)
		{
			NotifyMouseUp(eventargs::LeftButton, Convert(info));
			RedrawIfRequired();
		}

		void GuiWindowBase::LeftButtonDoubleClick(const NativeWindowMouseInfo& info)
		{
			NotifyMouseDoubleClick(eventargs::LeftButton, Convert(info));
			RedrawIfRequired();
		}

		void GuiWindowBase::RightButtonDown(const NativeWindowMouseInfo& info)
		{
			NotifyMouseDown(eventargs::RightButton, Convert(info));
			RedrawIfRequired();
		}

		void GuiWindowBase::RightButtonUp(const NativeWindowMouseInfo& info)
		{
			NotifyMouseUp(eventargs::RightButton, Convert(info));
			RedrawIfRequired();
		}

		void GuiWindowBase::RightButtonDoubleClick(const NativeWindowMouseInfo& info)
		{
			NotifyMouseDoubleClick(eventargs::RightButton, Convert(info));
			RedrawIfRequired();
		}

		void GuiWindowBase::MiddleButtonDown(const NativeWindowMouseInfo& info)
		{
			NotifyMouseDown(eventargs::MiddleButton, Convert(info));
			RedrawIfRequired();
		}

		void GuiWindowBase::MiddleButtonUp(const NativeWindowMouseInfo& info)
		{
			NotifyMouseUp(eventargs::MiddleButton, Convert(info));
			RedrawIfRequired();
		}

		void GuiWindowBase::MiddleButtonDoubleClick(const NativeWindowMouseInfo& info)
		{
			NotifyMouseDoubleClick(eventargs::MiddleButton, Convert(info));
			RedrawIfRequired();
		}

		void GuiWindowBase::HorizontalWheel(const NativeWindowMouseInfo& info)
		{
			NotifyMouseHorizontalWheel(Convert(info));
			RedrawIfRequired();
		}

		void GuiWindowBase::VerticalWheel(const NativeWindowMouseInfo& info)
		{
			NotifyMouseVerticalWheel(Convert(info));
			RedrawIfRequired();
		}

		void GuiWindowBase::MouseMoving(const NativeWindowMouseInfo& info)
		{
			NotifyMouseMove(Convert(info));
			RedrawIfRequired();
		}

		void GuiWindowBase::MouseEntered()
		{
			NotifyMouseEntered();
			RedrawIfRequired();
		}

		void GuiWindowBase::MouseLeaved()
		{
			NotifyMouseLeaved();
			RedrawIfRequired();
		}

		void GuiWindowBase::KeyDown(int code, bool alt)
		{
			NotifyKeyDown(code, alt);
			RedrawIfRequired();
		}

		void GuiWindowBase::KeyUp(int code, bool alt)
		{
			NotifyKeyUp(code, alt);
			RedrawIfRequired();
		}

		void GuiWindowBase::SysKeyDown(int code, bool alt)
		{
			NotifySysKeyDown(code, alt);
			RedrawIfRequired();
		}

		void GuiWindowBase::SysKeyUp(int code, bool alt)
		{
			NotifySysKeyUp(code, alt);
			RedrawIfRequired();
		}

		void GuiWindowBase::Char(wchar_t keyChar)
		{
			NotifyChar(keyChar);
			RedrawIfRequired();
		}

		//----------------------------------------------------------------------

		void GuiWindowBase::InitializeWindow()
		{
			NotifyAttachedToWindow(this);
		}

		void GuiWindowBase::FinalizeWindow()
		{
			NotifyAttachedToWindow(0);
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

		void GuiWindowBase::RequireTracking()
		{
			nativeWindow->RequireCapture();
		}

		void GuiWindowBase::ReleaseTracking()
		{
			nativeWindow->ReleaseCapture();
		}

		bool GuiWindowBase::RequireFocus()
		{
			nativeWindow->SetFocus();
			return nativeWindow->IsFocused();
		}

		bool GuiWindowBase::IsFocusing()
		{
			return nativeWindow->IsFocused();
		}

		void GuiWindowBase::FocusPreviousControl()
		{
			GuiControl* control=GetPreviousFocusControl();
			if(control)
			{
				control->RequireFocus();
			}
		}

		void GuiWindowBase::FocusNextControl()
		{
			GuiControl* control=GetNextFocusControl();
			if(control)
			{
				control->RequireFocus();
			}
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