/***********************************************************************
Vczh Library++ 3.0
Developer: ³Âè÷å«(vczh)
GUI::Windows Platform

Interfaces:
***********************************************************************/

#ifndef VCZH_PRESENTATION_WINDOWS_GUIGRAPHICSCOMPOSITION
#define VCZH_PRESENTATION_WINDOWS_GUIGRAPHICSCOMPOSITION

#include "GuiGraphicsElement.h"

namespace vl
{
	namespace presentation
	{
		namespace elements
		{

/***********************************************************************
Basic Construction
***********************************************************************/

			class GuiGraphicsComposition : public Object
			{
				typedef collections::IReadonlyList<GuiGraphicsComposition*> ICompositionList;
				typedef collections::List<GuiGraphicsComposition*> CompositionList;
			protected:
				CompositionList						children;
				GuiGraphicsComposition*				parent;
				Ptr<IGuiGraphicsElement>			ownedElement;
				bool								visible;
				IGuiGraphicsRenderTarget*			renderTarget;
				Margin								margin;

				virtual void						OnChildInserted(GuiGraphicsComposition* child);
				virtual void						OnChildRemoved(GuiGraphicsComposition* child);
			public:
				GuiGraphicsComposition();
				~GuiGraphicsComposition();

				GuiGraphicsComposition*				GetParent();
				const ICompositionList&				Children();
				bool								AddChild(GuiGraphicsComposition* child);
				bool								InsertChild(int index, GuiGraphicsComposition* child);
				bool								RemoveChild(GuiGraphicsComposition* child);
				bool								MoveChild(GuiGraphicsComposition* child, int newIndex);

				Ptr<IGuiGraphicsElement>			GetOwnedElement();
				void								SetOwnedElement(Ptr<IGuiGraphicsElement> element);
				bool								GetVisible();
				void								SetVisible(bool value);
				IGuiGraphicsRenderTarget*			GetRenderTarget();
				void								SetRenderTarget(IGuiGraphicsRenderTarget* value);

				void								Render(Size offset);

				virtual Margin						GetMargin();
				virtual void						SetMargin(Margin value);
				virtual Rect						GetBounds()=0;
				virtual void						SetBounds(Rect value)=0;
			};

/***********************************************************************
Compositions
***********************************************************************/

			class GuiWindowComposition : public GuiGraphicsComposition
			{
			protected:
				INativeWindow*						attachedWindow;
			public:
				GuiWindowComposition();
				~GuiWindowComposition();

				INativeWindow*						GetAttachedWindow();
				void								SetAttachedWindow(INativeWindow* window);

				Rect								GetBounds();
				void								SetBounds(Rect value);
				void								SetMargin(Margin value);
			};

			class GuiBoundsComposition : public GuiGraphicsComposition
			{
			protected:
				Rect								bounds;
			public:
				GuiBoundsComposition();
				~GuiBoundsComposition();

				Rect								GetBounds();
				void								SetBounds(Rect value);
			};
		}
	}
}

#endif