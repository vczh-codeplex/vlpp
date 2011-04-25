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

		class IGuiSkin : public Interface
		{
		public:
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

			virtual void					NotifySetParent(GuiControl* value);
			virtual void					NotifyAttachedToWindow(GuiWindowBase* window);
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

		class GuiWindowBase : public GuiControl, private INativeWindowListener
		{
		private:
			INativeWindow*					nativeWindow;
			bool							destructorInvoked;

		private:

			void							Moving(Rect& value);
			void							Moved();
			void							Destroying();
		protected:

			virtual void					InitializeWindow();
			Rect							GetBoundsForSkin();
		public:
			GuiWindowBase();
			~GuiWindowBase();

			INativeWindow*					GetContainingNativeWindow();
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
¶¥²ã´°¿Ú
***********************************************************************/

		class GuiWindow : public GuiWindowBase
		{
		protected:

			WString							GetSkinBuilderName();
		public:
			GuiWindow();
			~GuiWindow();

			WString							GetTitle();
			void							SetTitle(const WString& value);
			void							MoveToScreenCenter();
			void							Show();
			void							Close();

			Rect							GetBounds();
			void							SetBounds(Rect value);
			Size							GetClientSize();
			void							SetClientSize(Size value);
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