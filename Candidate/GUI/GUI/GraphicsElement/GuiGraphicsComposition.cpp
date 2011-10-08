#include "GuiGraphicsComposition.h"

namespace vl
{
	namespace presentation
	{
		namespace elements
		{
			using namespace collections;

/***********************************************************************
GuiGraphicsComposition
***********************************************************************/

			void GuiGraphicsComposition::OnChildInserted(GuiGraphicsComposition* child)
			{
			}

			void GuiGraphicsComposition::OnChildRemoved(GuiGraphicsComposition* child)
			{
			}

			void GuiGraphicsComposition::OnParentChanged(GuiGraphicsComposition* oldParent, GuiGraphicsComposition* newParent)
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
					minSize.x+=margin.left+margin.right;
					minSize.y+=margin.top+margin.bottom;
				}
				{
					int w=expectedBounds.Width();
					int h=expectedBounds.Height();
					if(minSize.x<w) minSize.x=w;
					if(minSize.y<h) minSize.y=h;
				}
				if(limitation==GuiGraphicsComposition::LimitToElementAndChildren)
				{
					int childCount=Children().Count();
					for(int i=0;i<childCount;i++)
					{
						Rect childBounds=Children()[i]->GetBounds();
						if(minSize.x<childBounds.x2) minSize.x=childBounds.x2;
						if(minSize.y<childBounds.y2) minSize.y=childBounds.y2;
					}
				}

				return Rect(expectedBounds.LeftTop(), minSize);
			}

			GuiGraphicsComposition::GuiGraphicsComposition()
				:parent(0)
				,visible(true)
				,minSizeLimitation(NoLimit)
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
				return GetBoundsInternal(bounds, GetMinSizeLimitation());
			}

			void GuiBoundsComposition::SetBounds(Rect value)
			{
				bounds=value;
			}

/***********************************************************************
GuiTableComposition
***********************************************************************/

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
				int (*getSpan)(GuiCellComposition*),
				int (*getRow)(int, int),
				int (*getCol)(int, int)
				)
			{
				for(int i=0;i<this->*dim1;i++)
				{
					GuiCellOption option=dimOptions[i];
					dimSizes[i]=0;
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
								if(cell && getSpan(cell)==1)
								{
									int size=getSize(cell->GetMinSize());
									if(dimSizes[i]<size)
									{
										dimSizes[i]=size;
									}
								}
							}
						}
						break;
					}
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
							totalPercentage=option.percentage;
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

			void GuiTableComposition::UpdateCellBoundsOffsets(
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
				if(max>right)
				{
					sizes[last]+=max-right;
				}
			}

			GuiTableComposition::GuiTableComposition()
				:rows(0)
				,columns(0)
				,cellPadding(0)
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
				Rect bounds(Point(0, 0), GetBounds().GetSize());
				bounds.x1+=margin.left+internalMargin.left+cellPadding;
				bounds.y1+=margin.top+internalMargin.top+cellPadding;
				bounds.x2-=margin.right+internalMargin.right+cellPadding;
				bounds.y2-=margin.bottom+internalMargin.bottom+cellPadding;
				if(bounds.x2<bounds.x1) bounds.x2=bounds.x1;
				if(bounds.y2<bounds.y1) bounds.y2=bounds.y1;
				return bounds;
			}

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

			void GuiTableComposition::UpdateCellBounds()
			{
				Array<int> rowOffsets, columnOffsets, rowSizes, columnSizes;
				int rowTotal=(rows-1)*cellPadding;
				int columnTotal=(columns-1)*cellPadding;

				rowOffsets.Resize(rows);
				rowSizes.Resize(rows);
				columnOffsets.Resize(columns);
				columnSizes.Resize(columns);

				UpdateCellBoundsInternal(
					rowSizes,
					rowTotal,
					rowOptions,
					&GuiTableComposition::rows,
					&GuiTableComposition::columns,
					&Y,
					&RS,
					&First,
					&Second
					);
				UpdateCellBoundsInternal(
					columnSizes,
					columnTotal,
					columnOptions,
					&GuiTableComposition::columns,
					&GuiTableComposition::rows,
					&X,
					&CS,
					&Second,
					&First
					);

				Rect area=GetCellArea();
				UpdateCellBoundsPercentages(rowSizes, rowTotal, area.Height(), rowOptions);
				UpdateCellBoundsPercentages(columnSizes, columnTotal, area.Width(), columnOptions);
				UpdateCellBoundsOffsets(rowOffsets, rowSizes, cellPadding, cellPadding+area.Height());
				UpdateCellBoundsOffsets(columnOffsets, columnSizes, cellPadding, cellPadding+area.Width());

				for(int i=0;i<rows;i++)
				{
					for(int j=0;j<columns;j++)
					{
						int index=GetSiteIndex(rows, columns, i, j);
						cellBounds[index]=Rect(Point(columnOffsets[j], rowOffsets[i]), Size(columnSizes[j], rowSizes[i]));
					}
				}
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
					}
					return Rect(bounds1.x1, bounds1.y1, bounds2.x2, bounds2.y2);
				}
				else
				{
					return Rect();
				}
			}

			void GuiCellComposition::SetBounds(Rect value)
			{
				value=bounds;
			}
		}
	}
}