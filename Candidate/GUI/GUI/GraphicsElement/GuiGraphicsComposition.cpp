#include "GuiGraphicsComposition.h"
#include "..\Controls\GuiBasicControls.h"
#include "..\..\..\..\Library\Collections\OperationCopyFrom.h"

namespace vl
{
	namespace presentation
	{
		namespace elements
		{
			using namespace collections;
			using namespace controls;

/***********************************************************************
GuiGraphicsComposition
***********************************************************************/

			void GuiGraphicsComposition::OnControlParentChanged(controls::GuiControl* control)
			{
				if(associatedControl && associatedControl!=control)
				{
					if(associatedControl->GetParent())
					{
						associatedControl->GetParent()->OnChildRemoved(associatedControl);
					}
					if(control)
					{
						control->OnChildInserted(associatedControl);
					}
				}
				else
				{
					for(int i=0;i<children.Count();i++)
					{
						children[i]->OnControlParentChanged(control);
					}
				}
			}

			void GuiGraphicsComposition::OnChildInserted(GuiGraphicsComposition* child)
			{
				child->OnControlParentChanged(GetRelatedControl());
			}

			void GuiGraphicsComposition::OnChildRemoved(GuiGraphicsComposition* child)
			{
				child->OnControlParentChanged(0);
			}

			void GuiGraphicsComposition::OnParentChanged(GuiGraphicsComposition* oldParent, GuiGraphicsComposition* newParent)
			{
			}

			void GuiGraphicsComposition::OnRenderTargetChanged()
			{
			}

			Rect GuiGraphicsComposition::GetBoundsInternal(Rect expectedBounds, MinSizeLimitation limitation)
			{
				Size minSize;
				if(limitation!=GuiGraphicsComposition::NoLimit)
				{
					if(ownedElement)
					{
						minSize=ownedElement->GetRenderer()->GetMinSize();
					}
				}
				if(limitation==GuiGraphicsComposition::LimitToElementAndChildren)
				{
					int childCount=Children().Count();
					for(int i=0;i<childCount;i++)
					{
						Rect childBounds=Children()[i]->GetMinNecessaryBounds();
						if(minSize.x<childBounds.x2) minSize.x=childBounds.x2;
						if(minSize.y<childBounds.y2) minSize.y=childBounds.y2;
					}
				}
				
				minSize.x+=margin.left+margin.right+internalMargin.left+internalMargin.right;
				minSize.y+=margin.top+margin.bottom+internalMargin.top+internalMargin.bottom;
				int w=expectedBounds.Width();
				int h=expectedBounds.Height();
				if(minSize.x<w) minSize.x=w;
				if(minSize.y<h) minSize.y=h;
				return Rect(expectedBounds.LeftTop(), minSize);
			}

			GuiGraphicsComposition::GuiGraphicsComposition()
				:parent(0)
				,visible(true)
				,minSizeLimitation(NoLimit)
				,renderTarget(0)
				,associatedControl(0)
				,associatedHost(0)
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
				if(!child) return false;
				if(child->GetParent()) return false;
				children.Insert(index, child);
				child->parent=this;
				child->SetRenderTarget(renderTarget);
				OnChildInserted(child);
				child->OnParentChanged(0, child->parent);
				return true;
			}

			bool GuiGraphicsComposition::RemoveChild(GuiGraphicsComposition* child)
			{
				if(!child) return false;
				int index=children.IndexOf(child);
				if(index==-1) return false;
				child->OnParentChanged(child->parent, 0);
				OnChildRemoved(child);
				child->SetRenderTarget(0);
				child->parent=0;
				children.RemoveAt(index);
				return true;
			}

			bool GuiGraphicsComposition::MoveChild(GuiGraphicsComposition* child, int newIndex)
			{
				if(!child) return false;
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

			GuiGraphicsComposition::MinSizeLimitation GuiGraphicsComposition::GetMinSizeLimitation()
			{
				return minSizeLimitation;
			}

			void GuiGraphicsComposition::SetMinSizeLimitation(MinSizeLimitation value)
			{
				minSizeLimitation=value;
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
				OnRenderTargetChanged();
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

			GuiGraphicsEventReceiver* GuiGraphicsComposition::GetEventReceiver()
			{
				if(!eventReceiver)
				{
					eventReceiver=new GuiGraphicsEventReceiver(this);
				}
				return eventReceiver.Obj();
			}

			bool GuiGraphicsComposition::HasEventReceiver()
			{
				return eventReceiver;
			}

			GuiGraphicsComposition* GuiGraphicsComposition::FindComposition(Point location)
			{
				if(!visible) return 0;
				Rect bounds=GetBounds();
				Rect relativeBounds=Rect(Point(0, 0), bounds.GetSize());
				if(relativeBounds.Contains(location))
				{
					Rect clientArea=GetClientArea();
					for(int i=children.Count()-1;i>=0;i--)
					{
						GuiGraphicsComposition* child=children[i];
						Rect childBounds=child->GetBounds();
						int offsetX=childBounds.x1+(clientArea.x1-bounds.x1);
						int offsetY=childBounds.y1+(clientArea.y1-bounds.y1);
						Point newLocation=location-Size(offsetX, offsetY);
						GuiGraphicsComposition* childResult=child->FindComposition(newLocation);
						if(childResult)
						{
							return childResult;
						}
					}
					return this;
				}
				else
				{
					return 0;
				}
			}

			Rect GuiGraphicsComposition::GetGlobalBounds()
			{
				Rect bounds=GetBounds();
				GuiGraphicsComposition* composition=parent;
				while(composition)
				{
					Rect clientArea=composition->GetClientArea();
					Rect parentBounds=composition->GetBounds();
					bounds.x1+=clientArea.x1;
					bounds.x2+=clientArea.x1;
					bounds.y1+=clientArea.y1;
					bounds.y2+=clientArea.y1;
					composition=composition->parent;
				}
				return bounds;
			}

			controls::GuiControl* GuiGraphicsComposition::GetAssociatedControl()
			{
				return associatedControl;
			}

			void GuiGraphicsComposition::SetAssociatedControl(controls::GuiControl* control)
			{
				if(associatedControl)
				{
					for(int i=0;i<children.Count();i++)
					{
						children[i]->OnControlParentChanged(0);
					}
				}
				associatedControl=control;
				if(associatedControl)
				{
					for(int i=0;i<children.Count();i++)
					{
						children[i]->OnControlParentChanged(associatedControl);
					}
				}
			}

			GuiGraphicsHost* GuiGraphicsComposition::GetAssociatedHost()
			{
				return associatedHost;
			}

			void GuiGraphicsComposition::SetAssociatedHost(GuiGraphicsHost* host)
			{
				associatedHost=host;
			}

			controls::GuiControl* GuiGraphicsComposition::GetRelatedControl()
			{
				GuiGraphicsComposition* composition=this;
				while(composition)
				{
					if(composition->GetAssociatedControl())
					{
						return composition->GetAssociatedControl();
					}
					else
					{
						composition=composition->GetParent();
					}
				}
				return 0;
			}

			GuiGraphicsHost* GuiGraphicsComposition::GetRelatedGraphicsHost()
			{
				GuiGraphicsComposition* composition=this;
				while(composition)
				{
					if(composition->GetAssociatedHost())
					{
						return composition->GetAssociatedHost();
					}
					else
					{
						composition=composition->GetParent();
					}
				}
				return 0;
			}

			controls::GuiControlHost* GuiGraphicsComposition::GetRelatedControlHost()
			{
				GuiGraphicsComposition* composition=this;
				while(composition)
				{
					if(composition->GetParent()==0)
					{
						return dynamic_cast<GuiControlHost*>(composition->GetAssociatedControl());
					}
				}
				return 0;
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

			Rect GuiGraphicsComposition::GetClientArea()
			{
				Rect bounds=GetBounds();
				bounds.x1+=margin.left+internalMargin.left;
				bounds.y1+=margin.top+internalMargin.top;
				bounds.x2-=margin.right+internalMargin.right;
				bounds.y2-=margin.bottom+internalMargin.bottom;
				return bounds;
			}

			Rect GuiGraphicsComposition::GetMinNecessaryBounds()
			{
				return GetBoundsInternal(Rect(), GetMinSizeLimitation());
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

			void GuiBoundsComposition::OnBoundsChanged()
			{
			}

			Rect GuiBoundsComposition::GetUnalignedBoundsForMinNecessaryBounds()
			{
				return GetBoundsInternal(compositionBounds, GetMinSizeLimitation());
			}

			GuiBoundsComposition::GuiBoundsComposition()
			{
				ClearAlignmentToParent();
			}

			GuiBoundsComposition::~GuiBoundsComposition()
			{
			}

			Rect GuiBoundsComposition::GetMinNecessaryBounds()
			{
				Rect result=GetUnalignedBoundsForMinNecessaryBounds();
				if(GetParent() && IsAlignedToParent())
				{
					if(alignmentToParent.left>=0)
					{
						int offset=alignmentToParent.left-result.x1;
						result.x1+=offset;
						result.x2+=offset;
					}
					if(alignmentToParent.top>=0)
					{
						int offset=alignmentToParent.top-result.y1;
						result.y1+=offset;
						result.y2+=offset;
					}
					if(alignmentToParent.right>=0)
					{
						result.x2+=alignmentToParent.right;
					}
					if(alignmentToParent.bottom>=0)
					{
						result.y2+=alignmentToParent.bottom;
					}
				}
				return result;
			}

			Rect GuiBoundsComposition::GetBounds()
			{
				Rect result=GetBoundsInternal(compositionBounds, GetMinSizeLimitation());
				if(GetParent() && IsAlignedToParent())
				{
					Size clientSize=GetParent()->GetClientArea().GetSize();
					if(alignmentToParent.left>=0 && alignmentToParent.right>=0)
					{
						result.x1=alignmentToParent.left;
						result.x2=clientSize.x-alignmentToParent.right;
					}
					else if(alignmentToParent.left>=0)
					{
						int width=result.Width();
						result.x1=alignmentToParent.left;
						result.x2=result.x1+width;
					}
					else if(alignmentToParent.right>=0)
					{
						int width=result.Width();
						result.x2=clientSize.x-alignmentToParent.right;
						result.x1=result.x2-width;
					}

					if(alignmentToParent.top>=0 && alignmentToParent.bottom>=0)
					{
						result.y1=alignmentToParent.top;
						result.y2=clientSize.y-alignmentToParent.bottom;
					}
					else if(alignmentToParent.top>=0)
					{
						int height=result.Height();
						result.y1=alignmentToParent.top;
						result.y2=result.y1+height;
					}
					else if(alignmentToParent.bottom>=0)
					{
						int height=result.Height();
						result.y2=clientSize.x-alignmentToParent.bottom;
						result.y1=result.y2-height;
					}
				}
				if(previousBounds!=result)
				{
					previousBounds=result;
					OnBoundsChanged();
				}
				return result;
			}

			void GuiBoundsComposition::SetBounds(Rect value)
			{
				compositionBounds=value;
			}

			void GuiBoundsComposition::ClearAlignmentToParent()
			{
				alignmentToParent=Margin(-1, -1, -1, -1);
			}

			Margin GuiBoundsComposition::GetAlignmentToParent()
			{
				return alignmentToParent;
			}

			void GuiBoundsComposition::SetAlignmentToParent(Margin value)
			{
				alignmentToParent=value;
			}

			bool GuiBoundsComposition::IsAlignedToParent()
			{
				return alignmentToParent!=Margin(-1, -1, -1, -1);
			}

/***********************************************************************
GuiTableComposition
***********************************************************************/

			namespace update_cell_bounds_helpers
			{
				int First(int a, int b)
				{
					return a;
				}

				int Second(int a, int b)
				{
					return b;
				}

				int X(Size s)
				{
					return s.x;
				}

				int Y(Size s)
				{
					return s.y;
				}

				int RL(GuiCellComposition* cell)
				{
					return cell->GetRow();
				}

				int CL(GuiCellComposition* cell)
				{
					return cell->GetColumn();
				}

				int RS(GuiCellComposition* cell)
				{
					return cell->GetRowSpan();
				}

				int CS(GuiCellComposition* cell)
				{
					return cell->GetColumnSpan();
				}
			}
			using namespace update_cell_bounds_helpers;

			int GuiTableComposition::GetSiteIndex(int _rows, int _columns, int _row, int _column)
			{
				return _row*_columns+_column;
			}

			void GuiTableComposition::SetSitedCell(int _row, int _column, GuiCellComposition* cell)
			{
				cellCompositions[GetSiteIndex(rows, columns, _row, _column)]=cell;
			}

			void GuiTableComposition::UpdateCellBoundsInternal(
				collections::Array<int>& dimSizes,
				int& dimSize,
				collections::Array<GuiCellOption>& dimOptions,
				int GuiTableComposition::* dim1,
				int GuiTableComposition::* dim2,
				int (*getSize)(Size),
				int (*getLocation)(GuiCellComposition*),
				int (*getSpan)(GuiCellComposition*),
				int (*getRow)(int, int),
				int (*getCol)(int, int),
				int maxPass
				)
			{
				for(int pass=0;pass<maxPass;pass++)
				{
					for(int i=0;i<this->*dim1;i++)
					{
						GuiCellOption option=dimOptions[i];
						if(pass==0)
						{
							dimSizes[i]=0;
						}
						switch(option.composeType)
						{
						case GuiCellOption::Absolute:
							{
								dimSizes[i]=option.absolute;
							}
							break;
						case GuiCellOption::MinSize:
							{
								for(int j=0;j<this->*dim2;j++)
								{
									GuiCellComposition* cell=GetSitedCell(getRow(i, j), getCol(i, j));
									if(cell)
									{
										bool accept=false;
										if(pass==0)
										{
											accept=getSpan(cell)==1;
										}
										else
										{
											accept=getLocation(cell)+getSpan(cell)==i+1;
										}
										if(accept)
										{
											int size=getSize(cell->GetMinSize());
											int span=getSpan(cell);
											for(int k=1;k<span;k++)
											{
												size-=dimSizes[i-k]+cellPadding;
											}
											if(dimSizes[i]<size)
											{
												dimSizes[i]=size;
											}
										}
									}
								}
							}
							break;
						}
					}
				}
				for(int i=0;i<this->*dim1;i++)
				{
					dimSize+=dimSizes[i];
				}
			}

			void GuiTableComposition::UpdateCellBoundsPercentages(
				collections::Array<int>& dimSizes,
				int dimSize,
				int maxDimSize,
				collections::Array<GuiCellOption>& dimOptions
				)
			{
				if(maxDimSize>dimSize)
				{
					double totalPercentage=0;
					int percentageCount=0;
					for(int i=0;i<dimOptions.Count();i++)
					{
						GuiCellOption option=dimOptions[i];
						if(option.composeType==GuiCellOption::Percentage)
						{
							totalPercentage+=option.percentage;
							percentageCount++;
						}
					}
					if(percentageCount>0 && totalPercentage>0.001)
					{
						for(int i=0;i<dimOptions.Count();i++)
						{
							GuiCellOption option=dimOptions[i];
							if(option.composeType==GuiCellOption::Percentage)
							{
								dimSizes[i]=(int)((maxDimSize-dimSize)*option.percentage/totalPercentage);
							}
						}
					}
				}
			}

			int GuiTableComposition::UpdateCellBoundsOffsets(
				collections::Array<int>& offsets,
				collections::Array<int>& sizes,
				int start,
				int max
				)
			{
				offsets[0]=start;
				for(int i=1;i<offsets.Count();i++)
				{
					start+=cellPadding+sizes[i-1];
					offsets[i]=start;
				}

				int last=offsets.Count()-1;
				int right=offsets[last]+sizes[last];
				return max-right;
			}

			Rect GuiTableComposition::GetUnalignedBoundsForMinNecessaryBounds()
			{
				Rect bounds=GuiBoundsComposition::GetUnalignedBoundsForMinNecessaryBounds();
				int w=margin.left+margin.right+internalMargin.left+internalMargin.right+tableContentMinSize.x;
				int h=margin.top+margin.bottom+internalMargin.top+internalMargin.bottom+tableContentMinSize.y;
				return Rect(bounds.LeftTop(), Size(w, h));
			}

			void GuiTableComposition::UpdateCellBoundsInternal()
			{
				Array<int> rowOffsets, columnOffsets, rowSizes, columnSizes;
				rowOffsets.Resize(rows);
				rowSizes.Resize(rows);
				columnOffsets.Resize(columns);
				columnSizes.Resize(columns);
				{
					int rowTotal=(rows-1)*cellPadding;
					int columnTotal=(columns-1)*cellPadding;

					UpdateCellBoundsInternal(
						rowSizes,
						rowTotal,
						rowOptions,
						&GuiTableComposition::rows,
						&GuiTableComposition::columns,
						&Y,
						&RL,
						&RS,
						&First,
						&Second,
						1
						);
					UpdateCellBoundsInternal(
						columnSizes,
						columnTotal,
						columnOptions,
						&GuiTableComposition::columns,
						&GuiTableComposition::rows,
						&X,
						&CL,
						&CS,
						&Second,
						&First,
						1
						);

					Rect area=GetCellArea();
					UpdateCellBoundsPercentages(rowSizes, rowTotal, area.Height(), rowOptions);
					UpdateCellBoundsPercentages(columnSizes, columnTotal, area.Width(), columnOptions);
					rowExtending=UpdateCellBoundsOffsets(rowOffsets, rowSizes, cellPadding, cellPadding+area.Height());
					columnExtending=UpdateCellBoundsOffsets(columnOffsets, columnSizes, cellPadding, cellPadding+area.Width());

					for(int i=0;i<rows;i++)
					{
						for(int j=0;j<columns;j++)
						{
							int index=GetSiteIndex(rows, columns, i, j);
							cellBounds[index]=Rect(Point(columnOffsets[j], rowOffsets[i]), Size(columnSizes[j], rowSizes[i]));
						}
					}
				}
			}

			void GuiTableComposition::UpdateTableContentMinSize()
			{
				Array<int> rowSizes, columnSizes;
				rowSizes.Resize(rows);
				columnSizes.Resize(columns);
				{
					int rowTotal=(rows+1)*cellPadding;
					int columnTotal=(columns+1)*cellPadding;

					Array<GuiCellOption> rowOptionsMin, columnOptionsMin;
					CopyFrom(rowOptionsMin.Wrap(), rowOptions.Wrap());
					CopyFrom(columnOptionsMin.Wrap(), columnOptions.Wrap());
					for(int i=0;i<rowOptionsMin.Count();i++)
					{
						if(rowOptionsMin[i].composeType==GuiCellOption::Percentage)
						{
							rowOptionsMin[i]=GuiCellOption::MinSizeOption();
						}
					}
					for(int i=0;i<columnOptionsMin.Count();i++)
					{
						if(columnOptionsMin[i].composeType==GuiCellOption::Percentage)
						{
							columnOptionsMin[i]=GuiCellOption::MinSizeOption();
						}
					}

					UpdateCellBoundsInternal(
						rowSizes,
						rowTotal,
						rowOptionsMin,
						&GuiTableComposition::rows,
						&GuiTableComposition::columns,
						&Y,
						&RL,
						&RS,
						&First,
						&Second,
						2
						);
					UpdateCellBoundsInternal(
						columnSizes,
						columnTotal,
						columnOptionsMin,
						&GuiTableComposition::columns,
						&GuiTableComposition::rows,
						&X,
						&CL,
						&CS,
						&Second,
						&First,
						2
						);
					tableContentMinSize=Size(columnTotal, rowTotal);
				}
				if(previousContentMinSize!=tableContentMinSize)
				{
					previousContentMinSize=tableContentMinSize;
					UpdateCellBoundsInternal();
				}
			}

			void GuiTableComposition::OnRenderTargetChanged()
			{
				if(GetRenderTarget())
				{
					UpdateTableContentMinSize();
				}
			}

			GuiTableComposition::GuiTableComposition()
				:rows(0)
				,columns(0)
				,cellPadding(0)
				,rowExtending(0)
				,columnExtending(0)
			{
				SetRowsAndColumns(1, 1);
			}

			GuiTableComposition::~GuiTableComposition()
			{
			}

			int GuiTableComposition::GetRows()
			{
				return rows;
			}

			int GuiTableComposition::GetColumns()
			{
				return columns;
			}

			bool GuiTableComposition::SetRowsAndColumns(int _rows, int _columns)
			{
				if(_rows<=0 || _columns<=0) return false;
				rowOptions.Resize(_rows);
				columnOptions.Resize(_columns);
				cellCompositions.Resize(_rows*_columns);
				cellBounds.Resize(_rows*_columns);
				for(int i=0;i<_rows*_columns;i++)
				{
					cellCompositions[i]=0;
					cellBounds[i]=Rect();
				}
				rows=_rows;
				columns=_columns;
				int childCount=Children().Count();
				for(int i=0;i<childCount;i++)
				{
					GuiCellComposition* cell=dynamic_cast<GuiCellComposition*>(Children()[i]);
					if(cell)
					{
						cell->OnTableRowsAndColumnsChanged();
					}
				}
				UpdateCellBounds();
				return true;
			}

			GuiCellComposition* GuiTableComposition::GetSitedCell(int _row, int _column)
			{
				return cellCompositions[GetSiteIndex(rows, columns, _row, _column)];
			}

			GuiCellOption GuiTableComposition::GetRowOption(int _row)
			{
				return rowOptions[_row];
			}

			void GuiTableComposition::SetRowOption(int _row, GuiCellOption option)
			{
				rowOptions[_row]=option;
			}

			GuiCellOption GuiTableComposition::GetColumnOption(int _column)
			{
				return columnOptions[_column];
			}

			void GuiTableComposition::SetColumnOption(int _column, GuiCellOption option)
			{
				columnOptions[_column]=option;
			}

			int GuiTableComposition::GetCellPadding()
			{
				return cellPadding;
			}

			void GuiTableComposition::SetCellPadding(int value)
			{
				if(value<0) value=0;
				cellPadding=value;
			}

			Rect GuiTableComposition::GetCellArea()
			{
				Rect bounds(Point(0, 0), GuiBoundsComposition::GetBounds().GetSize());
				bounds.x1+=margin.left+internalMargin.left+cellPadding;
				bounds.y1+=margin.top+internalMargin.top+cellPadding;
				bounds.x2-=margin.right+internalMargin.right+cellPadding;
				bounds.y2-=margin.bottom+internalMargin.bottom+cellPadding;
				if(bounds.x2<bounds.x1) bounds.x2=bounds.x1;
				if(bounds.y2<bounds.y1) bounds.y2=bounds.y1;
				return bounds;
			}

			void GuiTableComposition::UpdateCellBounds()
			{
				UpdateCellBoundsInternal();
				UpdateTableContentMinSize();
			}

			Rect GuiTableComposition::GetBounds()
			{
				Rect result;
				if(!IsAlignedToParent() && GetMinSizeLimitation()!=GuiGraphicsComposition::NoLimit)
				{
					result=Rect(compositionBounds.LeftTop(), compositionBounds.GetSize()-Size(columnExtending, rowExtending));
				}
				else
				{
					result=GuiBoundsComposition::GetBounds();
				}
				if(previousBounds!=result)
				{
					previousBounds=result;
					UpdateCellBounds();
				}
				return result;
			}

			void GuiTableComposition::SetBounds(Rect value)
			{
				GuiBoundsComposition::SetBounds(value);
			}

/***********************************************************************
GuiCellComposition
***********************************************************************/

			void GuiCellComposition::ClearSitedCells(GuiTableComposition* table)
			{
				if(row!=-1 && column!=-1)
				{
					for(int r=0;r<rowSpan;r++)
					{
						for(int c=0;c<columnSpan;c++)
						{
							table->SetSitedCell(row+r, column+c, 0);
						}
					}
				}
			}

			void GuiCellComposition::SetSitedCells(GuiTableComposition* table)
			{
				for(int r=0;r<rowSpan;r++)
				{
					for(int c=0;c<columnSpan;c++)
					{
						table->SetSitedCell(row+r, column+c, this);
					}
				}
			}

			void GuiCellComposition::ResetSiteInternal()
			{
				row=-1;
				column=-1;
				rowSpan=1;
				columnSpan=1;
			}

			bool GuiCellComposition::SetSiteInternal(int _row, int _column, int _rowSpan, int _columnSpan)
			{
				if(!tableParent) return false;
				if(_row<0 || _row>=tableParent->rows || _column<0 || _column>=tableParent->columns) return false;
				if(_rowSpan<1 || _row+_rowSpan>tableParent->rows || _columnSpan<1 || _column+_columnSpan>tableParent->columns) return false;

				for(int r=0;r<_rowSpan;r++)
				{
					for(int c=0;c<_columnSpan;c++)
					{
						GuiCellComposition* cell=tableParent->GetSitedCell(_row+r, _column+c);
						if(cell && cell!=this)
						{
							return false;
						}
					}
				}
				ClearSitedCells(tableParent);
				row=_row;
				column=_column;
				rowSpan=_rowSpan;
				columnSpan=_columnSpan;
				SetSitedCells(tableParent);
				return true;
			}

			void GuiCellComposition::OnParentChanged(GuiGraphicsComposition* oldParent, GuiGraphicsComposition* newParent)
			{
				if(tableParent)
				{
					ClearSitedCells(tableParent);
				}
				tableParent=dynamic_cast<GuiTableComposition*>(newParent);
				if(!tableParent || !SetSiteInternal(row, column, rowSpan, columnSpan))
				{
					ResetSiteInternal();
				}
				if(tableParent)
				{
					tableParent->UpdateCellBounds();
				}
			}

			void GuiCellComposition::OnTableRowsAndColumnsChanged()
			{
				if(!SetSiteInternal(row, column, rowSpan, columnSpan))
				{
					ResetSiteInternal();
				}
			}

			Size GuiCellComposition::GetMinSize()
			{
				return GetBoundsInternal(bounds, GetMinSizeLimitation()).GetSize();
			}

			GuiCellComposition::GuiCellComposition()
				:row(-1)
				,column(-1)
				,rowSpan(1)
				,columnSpan(1)
				,tableParent(0)
			{
				SetMinSizeLimitation(GuiGraphicsComposition::LimitToElementAndChildren);
			}

			GuiCellComposition::~GuiCellComposition()
			{
			}

			GuiTableComposition* GuiCellComposition::GetTableParent()
			{
				return tableParent;
			}

			int GuiCellComposition::GetRow()
			{
				return row;
			}

			int GuiCellComposition::GetRowSpan()
			{
				return rowSpan;
			}

			int GuiCellComposition::GetColumn()
			{
				return column;
			}

			int GuiCellComposition::GetColumnSpan()
			{
				return columnSpan;
			}

			bool GuiCellComposition::SetSite(int _row, int _column, int _rowSpan, int _columnSpan)
			{
				if(SetSiteInternal(_row, _column, _rowSpan, _columnSpan))
				{
					tableParent->UpdateCellBounds();
					return true;
				}
				else
				{
					return false;
				}
			}

			Rect GuiCellComposition::GetBounds()
			{
				if(tableParent && row!=-1 && column!=-1)
				{
					Rect bounds1, bounds2;
					{
						int index=tableParent->GetSiteIndex(tableParent->rows, tableParent->columns, row, column);
						bounds1=tableParent->cellBounds[index];
					}
					{
						int index=tableParent->GetSiteIndex(tableParent->rows, tableParent->columns, row+rowSpan-1, column+columnSpan-1);
						bounds2=tableParent->cellBounds[index];
						if(tableParent->GetMinSizeLimitation()==GuiGraphicsComposition::NoLimit)
						{
							if(row+rowSpan==tableParent->rows)
							{
								bounds2.y2+=tableParent->rowExtending;
							}
							if(column+columnSpan==tableParent->columns)
							{
								bounds2.x2+=tableParent->columnExtending;
							}
						}
					}
					return Rect(bounds1.x1, bounds1.y1, bounds2.x2, bounds2.y2);
				}
				else
				{
					return bounds;
				}
			}

			void GuiCellComposition::SetBounds(Rect value)
			{
				bounds=value;
			}

/***********************************************************************
GuiSideAlignedComposition
***********************************************************************/

			GuiSideAlignedComposition::GuiSideAlignedComposition()
				:direction(Top)
				,maxLength(10)
				,maxRatio(1.0)
			{
			}

			GuiSideAlignedComposition::~GuiSideAlignedComposition()
			{
			}

			GuiSideAlignedComposition::Direction GuiSideAlignedComposition::GetDirection()
			{
				return direction;
			}

			void GuiSideAlignedComposition::SetDirection(Direction value)
			{
				direction=value;
			}

			int GuiSideAlignedComposition::GetMaxLength()
			{
				return maxLength;
			}

			void GuiSideAlignedComposition::SetMaxLength(int value)
			{
				if(value<0) value=0;
				maxLength=value;
			}

			double GuiSideAlignedComposition::GetMaxRatio()
			{
				return maxRatio;
			}

			void GuiSideAlignedComposition::SetMaxRatio(double value)
			{
				maxRatio=
					value<0?0:
					value>1?1:
					value;
			}

			Rect GuiSideAlignedComposition::GetBounds()
			{
				GuiGraphicsComposition* parent=GetParent();
				if(parent)
				{
					Rect bounds=parent->GetBounds();
					int w=(int)(bounds.Width()*maxRatio);
					int h=(int)(bounds.Height()*maxRatio);
					if(w>maxLength) w=maxLength;
					if(h>maxLength) h=maxLength;
					switch(direction)
					{
					case Left:
						{
							bounds.x2=bounds.x1+w;
						}
						break;
					case Top:
						{
							bounds.y2=bounds.y1+h;
						}
						break;
					case Right:
						{
							bounds.x1=bounds.x2-w;
						}
						break;
					case Bottom:
						{
							bounds.y1=bounds.y2-h;
						}
						break;
					}
					return bounds;
				}
				return Rect();
			}

			void GuiSideAlignedComposition::SetBounds(Rect value)
			{
			}

/***********************************************************************
GuiPartialViewComposition
***********************************************************************/

			GuiPartialViewComposition::GuiPartialViewComposition()
				:wRatio(0.0)
				,wPageSize(1.0)
				,hRatio(0.0)
				,hPageSize(1.0)
			{
			}

			GuiPartialViewComposition::~GuiPartialViewComposition()
			{
			}

			double GuiPartialViewComposition::GetWidthRatio()
			{
				return wRatio;
			}

			double GuiPartialViewComposition::GetWidthPageSize()
			{
				return wPageSize;
			}

			double GuiPartialViewComposition::GetHeightRatio()
			{
				return hRatio;
			}

			double GuiPartialViewComposition::GetHeightPageSize()
			{
				return hPageSize;
			}

			void GuiPartialViewComposition::SetWidthRatio(double value)
			{
				wRatio=value;
			}

			void GuiPartialViewComposition::SetWidthPageSize(double value)
			{
				wPageSize=value;
			}

			void GuiPartialViewComposition::SetHeightRatio(double value)
			{
				hRatio=value;
			}

			void GuiPartialViewComposition::SetHeightPageSize(double value)
			{
				hPageSize=value;
			}

			Rect GuiPartialViewComposition::GetBounds()
			{
				GuiGraphicsComposition* parent=GetParent();
				if(parent)
				{
					Rect bounds=parent->GetBounds();
					int w=bounds.Width();
					int h=bounds.Height();
					int pw=(int)(wPageSize*w);
					int ph=(int)(hPageSize*h);
					return Rect(Point((int)(wRatio*w), (int)(hRatio*h)), Size(pw, ph));
				}
				return Rect();
			}

			void GuiPartialViewComposition::SetBounds(Rect value)
			{
			}
		}
	}
}