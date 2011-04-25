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

			virtual const WString&			GetSkinBuilderName()=0;
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

			Grid*							GetContainer();
			void							SetContainer(Grid* value);

			Rect							GetBounds();
			void							SetBounds(Rect value);
		};

		class GuiWindowBase : public GuiControl
		{
		protected:
		public:
			virtual INativeWindow*			GetContainingNativeWindow()=0;
		};

/***********************************************************************
Ó¦ÓÃ³ÌÐò
***********************************************************************/

		class GuiApplication : public Object
		{
			typedef collections::Dictionary<WString, Ptr<IGuiSkinBuilder>>		SkinBuilderMap;
		protected:
			SkinBuilderMap					skinBuilders;
		public:
			GuiApplication();
			~GuiApplication();

			void							SetSkinBuilder(const WString& name, IGuiSkinBuilder* builder);
			IGuiSkinBuilder*				GetSkinBuilder(const WString& name);
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
		public:
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