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
				bool								minSizeLimitated;
				IGuiGraphicsRenderTarget*			renderTarget;
				Margin								margin, internalMargin;

				virtual void						OnChildInserted(GuiGraphicsComposition* child);
				virtual void						OnChildRemoved(GuiGraphicsComposition* child);
				virtual void						OnParentChanged(GuiGraphicsComposition* oldParent, GuiGraphicsComposition* newParent);
				Rect								GetBoundsInternal(Rect expectedBounds);
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
				bool								GetMinSizeLimited();
				void								SetMinSizeLimited(bool value);
				IGuiGraphicsRenderTarget*			GetRenderTarget();
				void								SetRenderTarget(IGuiGraphicsRenderTarget* value);

				void								Render(Size offset);

				virtual Margin						GetMargin();
				virtual void						SetMargin(Margin value);
				virtual Margin						GetInternalMargin();
				virtual void						SetInternalMargin(Margin value);
				virtual Rect						GetBounds()=0;
				virtual void						SetBounds(Rect value)=0;
			};

/***********************************************************************
Basic Compositions
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

/***********************************************************************
Table Compositions
***********************************************************************/

			class GuiTableComposition;
			class GuiCellComposition;

			struct GuiCellOption
			{
				enum ComposeType
				{
					Absolute,
					Percentage,
					MinSize,
				};

				ComposeType		composeType;
				int				absolute;
				double			percentage;

				GuiCellOption()
					:composeType(Absolute)
					,absolute(20)
					,percentage(0)
				{
				}

				bool operator==(const GuiCellOption& value){return false;}
				bool operator!=(const GuiCellOption& value){return true;}
			};

			class GuiTableComposition : public GuiBoundsComposition
			{
				friend class GuiCellComposition;
			protected:
				int											rows;
				int											columns;
				int											cellPadding;
				collections::Array<GuiCellOption>			rowOptions;
				collections::Array<GuiCellOption>			columnOptions;
				collections::Array<GuiCellComposition*>		cellCompositions;
				collections::Array<Rect>					cellBounds;

				int									GetSiteIndex(int _rows, int _columns, int _row, int _column);
				void								SetSitedCell(int _row, int _column, GuiCellComposition* cell);

				void								UpdateCellBoundsInternal(
														collections::Array<int>& dimSizes,
														int& dimSize, 
														collections::Array<GuiCellOption>& dimOptions,
														int GuiTableComposition::* dim1,
														int GuiTableComposition::* dim2,
														int (*getSize)(Size),
														int (*getSpan)(GuiCellComposition*),
														int (*getRow)(int, int),
														int (*getCol)(int, int)
														);
				void								UpdateCellBoundsPercentages(
														collections::Array<int>& dimSizes,
														int dimSize,
														int maxDimSize,
														collections::Array<GuiCellOption>& dimOptions
														);
				void								UpdateCellBoundsOffsets(
														collections::Array<int>& offsets,
														collections::Array<int>& sizes,
														int start,
														int max
														);
			public:
				GuiTableComposition();
				~GuiTableComposition();

				int									GetRows();
				int									GetColumns();
				bool								SetRowsAndColumns(int _rows, int _columns);
				GuiCellComposition*					GetSitedCell(int _row, int _column);

				GuiCellOption						GetRowOption(int _row);
				void								SetRowOption(int _row, GuiCellOption option);
				GuiCellOption						GetColumnOption(int _column);
				void								SetColumnOption(int _column, GuiCellOption option);

				int									GetCellPadding();
				void								SetCellPadding(int value);
				Rect								GetCellArea();
				void								UpdateCellBounds();
			};

			class GuiCellComposition : public GuiGraphicsComposition
			{
				friend class GuiTableComposition;
			protected:
				int									row;
				int									rowSpan;
				int									column;
				int									columnSpan;
				GuiTableComposition*				tableParent;
				Rect								bounds;
				
				void								ClearSitedCells(GuiTableComposition* table);
				void								SetSitedCells(GuiTableComposition* table);
				void								ResetSiteInternal();
				bool								SetSiteInternal(int _row, int _column, int _rowSpan, int _columnSpan);
				void								OnParentChanged(GuiGraphicsComposition* oldParent, GuiGraphicsComposition* newParent);
				void								OnTableRowsAndColumnsChanged();
				Size								GetMinSize();
			public:
				GuiCellComposition();
				~GuiCellComposition();

				GuiTableComposition*				GetTableParent();

				int									GetRow();
				int									GetRowSpan();
				int									GetColumn();
				int									GetColumnSpan();
				bool								SetSite(int _row, int _column, int _rowSpan, int _columnSpan);

				Rect								GetBounds();
				void								SetBounds(Rect value);
			};
		}
	}
}

#endif