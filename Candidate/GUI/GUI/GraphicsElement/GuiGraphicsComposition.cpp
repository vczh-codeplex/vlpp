#include "GuiGraphicsComposition.h"

namespace vl
{
	namespace presentation
	{
		namespace elements
		{

/***********************************************************************
GuiGraphicsComposition
***********************************************************************/

			void GuiGraphicsComposition::OnChildInserted(GuiGraphicsComposition* child)
			{
			}

			void GuiGraphicsComposition::OnChildRemoved(GuiGraphicsComposition* child)
			{
			}

			Rect GuiGraphicsComposition::GetBoundsInternal(Rect expectedBounds)
			{
				Size minSize;
				if(ownedElement)
				{
					minSize=ownedElement->GetRenderer()->GetMinSize();
				}
				minSize.x+=margin.left+margin.right;
				minSize.y+=margin.top+margin.bottom;

				int w=expectedBounds.Width();
				int h=expectedBounds.Height();
				if(w<minSize.x) w=minSize.x;
				if(h<minSize.y) w=minSize.y;

				return Rect(expectedBounds.LeftTop(), Size(w, h));
			}

			GuiGraphicsComposition::GuiGraphicsComposition()
				:parent(0)
				,visible(true)
				,minSizeLimitated(false)
				,renderTarget(0)
			{
			}

			GuiGraphicsComposition::~GuiGraphicsComposition()
			{
				for(int i=0;i<children.Count();i++)
				{
					delete children[i];
				}
			}

			GuiGraphicsComposition* GuiGraphicsComposition::GetParent()
			{
				return parent;
			}

			const GuiGraphicsComposition::ICompositionList& GuiGraphicsComposition::Children()
			{
				return children.Wrap();
			}

			bool GuiGraphicsComposition::AddChild(GuiGraphicsComposition* child)
			{
				return InsertChild(children.Count(), child);
			}

			bool GuiGraphicsComposition::InsertChild(int index, GuiGraphicsComposition* child)
			{
				if(child->GetParent()) return false;
				children.Insert(index, child);
				child->parent=this;
				child->SetRenderTarget(renderTarget);
				OnChildInserted(child);
				return true;
			}

			bool GuiGraphicsComposition::RemoveChild(GuiGraphicsComposition* child)
			{
				int index=children.IndexOf(child);
				if(index==-1) return false;
				OnChildRemoved(child);
				child->SetRenderTarget(0);
				child->parent=0;
				children.RemoveAt(index);
				return true;
			}

			bool GuiGraphicsComposition::MoveChild(GuiGraphicsComposition* child, int newIndex)
			{
				int index=children.IndexOf(child);
				if(index==-1) return false;
				children.RemoveAt(index);
				children.Insert(newIndex, child);
				return true;
			}

			Ptr<IGuiGraphicsElement> GuiGraphicsComposition::GetOwnedElement()
			{
				return ownedElement;
			}

			void GuiGraphicsComposition::SetOwnedElement(Ptr<IGuiGraphicsElement> element)
			{
				if(ownedElement)
				{
					ownedElement->GetRenderer()->SetRenderTarget(0);
				}
				ownedElement=element;
				if(ownedElement)
				{
					ownedElement->GetRenderer()->SetRenderTarget(renderTarget);
				}
			}

			bool GuiGraphicsComposition::GetVisible()
			{
				return visible;
			}

			void GuiGraphicsComposition::SetVisible(bool value)
			{
				visible=value;
			}

			bool GuiGraphicsComposition::GetMinSizeLimited()
			{
				return minSizeLimitated;
			}

			void GuiGraphicsComposition::SetMinSizeLimited(bool value)
			{
				minSizeLimitated=value;
			}

			IGuiGraphicsRenderTarget* GuiGraphicsComposition::GetRenderTarget()
			{
				return renderTarget;
			}

			void GuiGraphicsComposition::SetRenderTarget(IGuiGraphicsRenderTarget* value)
			{
				renderTarget=value;
				if(ownedElement)
				{
					ownedElement->GetRenderer()->SetRenderTarget(renderTarget);
				}
				for(int i=0;i<children.Count();i++)
				{
					children[i]->SetRenderTarget(renderTarget);
				}
			}

			void GuiGraphicsComposition::Render(Size offset)
			{
				if(visible && renderTarget && !renderTarget->IsClipperCoverWholeTarget())
				{
					Rect bounds=GetBounds();
					bounds.x1+=margin.left;
					bounds.y1+=margin.top;
					bounds.x2-=margin.right;
					bounds.y2-=margin.bottom;

					if(bounds.x1<=bounds.x2 && bounds.y1<=bounds.y2)
					{
						bounds.x1+=offset.x;
						bounds.x2+=offset.x;
						bounds.y1+=offset.y;
						bounds.y2+=offset.y;

						if(ownedElement)
						{
							ownedElement->GetRenderer()->Render(bounds);
						}
						if(children.Count()>0)
						{
							bounds.x1+=internalMargin.left;
							bounds.y1+=internalMargin.top;
							bounds.x2-=internalMargin.right;
							bounds.y2-=internalMargin.bottom;
							if(bounds.x1<=bounds.x2 && bounds.y1<=bounds.y2)
							{
								offset=bounds.GetSize();
								renderTarget->PushClipper(bounds);
								if(!renderTarget->IsClipperCoverWholeTarget())
								{
									for(int i=0;i<children.Count();i++)
									{
										children[i]->Render(Size(bounds.x1, bounds.y1));
									}
								}
								renderTarget->PopClipper();
							}
						}
					}
				}
			}

			Margin GuiGraphicsComposition::GetMargin()
			{
				return margin;
			}

			void GuiGraphicsComposition::SetMargin(Margin value)
			{
				margin=value;
			}

			Margin GuiGraphicsComposition::GetInternalMargin()
			{
				return internalMargin;
			}

			void GuiGraphicsComposition::SetInternalMargin(Margin value)
			{
				internalMargin=value;
			}

/***********************************************************************
GuiWindowComposition
***********************************************************************/

			GuiWindowComposition::GuiWindowComposition()
				:attachedWindow(0)
			{
			}

			GuiWindowComposition::~GuiWindowComposition()
			{
			}

			INativeWindow* GuiWindowComposition::GetAttachedWindow()
			{
				return attachedWindow;
			}

			void GuiWindowComposition::SetAttachedWindow(INativeWindow* window)
			{
				attachedWindow=window;
				SetRenderTarget(attachedWindow?GetGuiGraphicsResourceManager()->GetRenderTarget(attachedWindow):0);
			}

			Rect GuiWindowComposition::GetBounds()
			{
				return attachedWindow?Rect(Point(0, 0), attachedWindow->GetClientSize()):Rect();
			}

			void GuiWindowComposition::SetBounds(Rect value)
			{
				if(attachedWindow)
				{
					attachedWindow->SetClientSize(value.GetSize());
				}
			}

			void GuiWindowComposition::SetMargin(Margin value)
			{
			}

/***********************************************************************
GuiBoundsComposition
***********************************************************************/

			GuiBoundsComposition::GuiBoundsComposition()
			{
			}

			GuiBoundsComposition::~GuiBoundsComposition()
			{
			}

			Rect GuiBoundsComposition::GetBounds()
			{
				return GetBoundsInternal(bounds);
			}

			void GuiBoundsComposition::SetBounds(Rect value)
			{
				bounds=value;
			}
		}
	}
}