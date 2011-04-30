/***********************************************************************
Vczh Library++ 3.0
Developer: ³Âè÷å«(vczh)
GUI::Control Library

Interfaces:
***********************************************************************/

#ifndef VCZH_PRESENTATION_GUICONTROL
#define VCZH_PRESENTATION_GUICONTROL

#include "..\..\..\Library\Pointer.h"
#include "..\..\..\Library\String.h"
#include "..\..\..\Library\Event.h"
#include "..\..\..\Library\Collections\List.h"
#include "..\..\..\Library\Collections\Dictionary.h"
#include "NativeWindow\GuiNativeWindow.h"

extern void GuiMain();

namespace vl
{
	namespace presentation
	{
		class IGuiSkin;
		class IGuiSkinBuilder;
		class GuiControl;
		class GuiWindowBase;
		class GuiApplication;
		class GuiWindow;

/***********************************************************************
Æ¤·ô
***********************************************************************/

		class IGuiSkinListener : public Interface
		{
		public:
			virtual void					RequireRedraw()=0;
		};

		class IGuiSkin : public Interface
		{
		public:
			virtual IGuiSkinListener*		GetListener()=0;
			virtual void					AttachListener(IGuiSkinListener* listener)=0;
			virtual void					SetBounds(Rect value)=0;
			virtual bool					ContainsPoint(Point value)=0;
			virtual void					RemoveChild(IGuiSkin* child)=0;
			virtual void					InsertChild(int index, IGuiSkin* child)=0;
			virtual IGuiSkin*				GetChild(int index)=0;
			virtual int						ChildCount()=0;
		};

		class IGuiSkinBuilder : public Interface
		{
		public:
			virtual Ptr<IGuiSkin>			Build(INativeWindow* window)=0;
		};

/***********************************************************************
¿Ø¼þ
***********************************************************************/

		namespace eventargs
		{
			enum MouseButton
			{
				LeftButton,
				MiddleButton,
				RightButton,
			};

			struct MouseInfo
			{
				bool						ctrl, shift;
				bool						left, middle, right;
				int							x, y, wheel;
			};
		}

		class GuiControl : public Object
		{
		public:
			class Grid : public Object
			{
			protected:
				GuiControl*					ownedControl;
				
				void						NotifyChildEntering(GuiControl* value);
				void						NotifyChildEntered(GuiControl* value);
				void						NotifyChildLeaving(GuiControl* value);
				void						NotifyChildLeaved(GuiControl* value);
			public:
				Grid();
				~Grid();

				virtual GuiControl*			GetOwnedControl();
				virtual bool				SetOwnedControl(GuiControl* control);

				virtual int					GetChildCount()=0;
				virtual GuiControl*			GetChild(int index)=0;
				virtual int					GetChildIndex(GuiControl* child)=0;
			};
		private:
			GuiControl*						parent;
			Ptr<IGuiSkin>					skin;
			WString							name;
			Ptr<Grid>						container;
			Rect							bounds;
			bool							tabStop;

			GuiControl*						focusedControl;
			GuiControl*						enteredControl;
			GuiControl*						trackingControl;

			void							TrackChild(GuiControl* child);
			bool							FocusChild(GuiControl* child);
		protected:

			virtual WString					GetSkinBuilderName()=0;
			virtual Rect					GetBoundsForSkin();
			Ptr<IGuiSkin>					GetSkin();
			eventargs::MouseInfo			Offset(GuiControl* child, const eventargs::MouseInfo& info);

			virtual void					NotifySetParent(GuiControl* value);
			virtual void					NotifyAttachedToWindow(GuiWindowBase* window);
			virtual void					NotifySkinChanged();
			virtual void					NotifyChildEntering(GuiControl* value);
			virtual void					NotifyChildEntered(GuiControl* value);
			virtual void					NotifyChildLeaving(GuiControl* value);
			virtual void					NotifyChildLeaved(GuiControl* value);
			virtual void					NotifyMoving(Rect& value);
			virtual void					NotifyMoved(Rect value);

			virtual GuiControl*				NotifyMouseDown(eventargs::MouseButton button, const eventargs::MouseInfo& info);
			virtual GuiControl*				NotifyMouseMove(const eventargs::MouseInfo& info);
			virtual GuiControl*				NotifyMouseUp(eventargs::MouseButton button, const eventargs::MouseInfo& info);
			virtual GuiControl*				NotifyMouseDoubleClick(eventargs::MouseButton button, const eventargs::MouseInfo& info);
			virtual GuiControl*				NotifyMouseHorizontalWheel(const eventargs::MouseInfo& info);
			virtual GuiControl*				NotifyMouseVerticalWheel(const eventargs::MouseInfo& info);

			virtual GuiControl*				NotifyKeyDown(int code, bool alt);
			virtual GuiControl*				NotifyKeyUp(int code, bool alt);
			virtual GuiControl*				NotifySysKeyDown(int code, bool alt);
			virtual GuiControl*				NotifySysKeyUp(int code, bool alt);
			virtual GuiControl*				NotifyChar(wchar_t keyChar);

			virtual void					NotifyMouseEntered();
			virtual void					NotifyMouseLeaved();
			virtual void					NotifyGotFocus();
			virtual void					NotifyLostFocus();
		public:
			GuiControl();
			~GuiControl();

			GuiControl*						GetParent();
			GuiControl*						GetChildFromPoint(Point value);

			const WString&					GetControlName();
			void							SetControlName(const WString& value);

			virtual void					RequireTracking();
			virtual void					ReleaseTracking();
			virtual bool					IsTracking();
			virtual bool					RequireFocus();
			virtual bool					IsFocusing();
			virtual void					FocusNextControl();
			virtual void					FocusPreviousControl();

			virtual Grid*					GetContainer();
			virtual void					SetContainer(Grid* value);

			virtual Rect					GetBounds();
			virtual void					SetBounds(Rect value);
			
			virtual Size					GetClientSize();
			virtual void					SetClientSize(Size value);

			virtual bool					GetTabStop();
			virtual void					SetTabStop(bool value);
			virtual bool					IsTabStopEnabled();
		};

		class GuiWindowBase : public GuiControl, protected INativeWindowListener, private IGuiSkinListener
		{
		private:
			INativeWindow*					nativeWindow;
			bool							destructorInvoked;
			bool							needToRedraw;

			void							RedrawIfRequired();
			void							RequireRedraw();
		protected:
			void							RedrawContent();
			void							Moving(Rect& bounds);
			void							Moved();
			void							Enabled();
			void							Disabled();
			void							GotFocus();
			void							LostFocus();
			void							Activated();
			void							Deactivated();
			void							Opened();
			void							Closing(bool& cancel);
			void							Closed();
			void							Paint();
			void							Destroying();

			void							LeftButtonDown(const NativeWindowMouseInfo& info);
			void							LeftButtonUp(const NativeWindowMouseInfo& info);
			void							LeftButtonDoubleClick(const NativeWindowMouseInfo& info);
			void							RightButtonDown(const NativeWindowMouseInfo& info);
			void							RightButtonUp(const NativeWindowMouseInfo& info);
			void							RightButtonDoubleClick(const NativeWindowMouseInfo& info);
			void							MiddleButtonDown(const NativeWindowMouseInfo& info);
			void							MiddleButtonUp(const NativeWindowMouseInfo& info);
			void							MiddleButtonDoubleClick(const NativeWindowMouseInfo& info);
			void							HorizontalWheel(const NativeWindowMouseInfo& info);
			void							VerticalWheel(const NativeWindowMouseInfo& info);
			void							MouseMoving(const NativeWindowMouseInfo& info);
			void							MouseEntered();
			void							MouseLeaved();

			void							KeyDown(int code, bool alt);
			void							KeyUp(int code, bool alt);
			void							SysKeyDown(int code, bool alt);
			void							SysKeyUp(int code, bool alt);
			void							Char(wchar_t keyChar);
		protected:

			virtual void					InitializeWindow();
			virtual void					FinalizeWindow();
			Rect							GetBoundsForSkin();
		public:
			GuiWindowBase();
			~GuiWindowBase();

			void							RequireTracking();
			void							ReleaseTracking();
			bool							RequireFocus();
			bool							IsFocusing();

			INativeWindow*					GetContainingNativeWindow();
			IGuiSkinListener*				GetSkinListener();
		};

/***********************************************************************
Ó¦ÓÃ³ÌÐò
***********************************************************************/

		class GuiApplication : public Object
		{
			friend class GuiWindowBase;

			typedef collections::Dictionary<WString, Ptr<IGuiSkinBuilder>>		SkinBuilderMap;
			typedef collections::List<GuiWindowBase*>							WindowBaseList;
		protected:
			SkinBuilderMap					skinBuilders;
			WindowBaseList					windowBases;
			GuiWindow*						mainWindow;

			void							RegisterWindow(GuiWindowBase* window);
			void							UnregisterWindow(GuiWindowBase* window);
		public:
			GuiApplication();
			~GuiApplication();

			IGuiSkinBuilder*				GetSkinBuilder(const WString& name);
			void							SetSkinBuilder(const WString& name, IGuiSkinBuilder* builder);

			void							Run(GuiWindow* window);
			GuiWindow*						GetMainWindow();
		};

		extern GuiApplication*				GetCurrentApplication();

/***********************************************************************
ÅÅ°æ
***********************************************************************/

		class GuiNormalGrid : public GuiControl::Grid
		{
		};
	}
}

#endif