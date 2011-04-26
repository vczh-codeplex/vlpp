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
			virtual void					AttachListener(IGuiSkinListener* listener)=0;
			virtual void					SetBounds(Rect value)=0;
			virtual void					RemoveChild(IGuiSkin* child)=0;
			virtual void					InsertChild(int index, IGuiSkin* child)=0;
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

		class GuiControl : public Object
		{
		public:
			class Grid : public Object
			{
			private:
				GuiControl*					ownedControl;
			protected:
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

		protected:

			virtual WString					GetSkinBuilderName()=0;
			virtual Rect					GetBoundsForSkin();
			Ptr<IGuiSkin>					GetSkin();

			virtual void					NotifySetParent(GuiControl* value);
			virtual void					NotifyAttachedToWindow(GuiWindowBase* window);
			virtual void					NotifySkinChanged();
			virtual void					NotifyChildEntering(GuiControl* value);
			virtual void					NotifyChildEntered(GuiControl* value);
			virtual void					NotifyChildLeaving(GuiControl* value);
			virtual void					NotifyChildLeaved(GuiControl* value);
			virtual void					NotifyMoving(Rect& value);
			virtual void					NotifyMoved(Rect value);
		public:
			GuiControl();
			~GuiControl();

			GuiControl*						GetParent();

			const WString&					GetControlName();
			void							SetControlName(const WString& value);

			virtual Grid*					GetContainer();
			virtual void					SetContainer(Grid* value);

			virtual Rect					GetBounds();
			virtual void					SetBounds(Rect value);
			
			virtual Size					GetClientSize();
			virtual void					SetClientSize(Size value);
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
			Rect							GetBoundsForSkin();
		public:
			GuiWindowBase();
			~GuiWindowBase();

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

/***********************************************************************
¿Ø¼þ¿â
***********************************************************************/

		class GuiLabel : public GuiControl
		{
		protected:
		public:
		};

		class GuiButtonBase : public GuiControl
		{
		protected:
		public:
		};

		class GuiButton : public GuiButtonBase
		{
		protected:
		public:
		};
	}
}

#endif