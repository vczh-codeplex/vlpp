/***********************************************************************
Vczh Library++ 3.0
Developer: ³Âè÷å«(vczh)
GUI::Windows Platform

Interfaces:
***********************************************************************/

#ifndef VCZH_PRESENTATION_WINDOWS_GUIGRAPHICSCOMPOSITION
#define VCZH_PRESENTATION_WINDOWS_GUIGRAPHICSCOMPOSITION

#include "GuiGraphicsElement.h"
#include "GuiGraphicsEventReceiver.h"

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
			public:
				enum MinSizeLimitation
				{
					NoLimit,
					LimitToElement,
					LimitToElementAndChildren,
				};
			protected:
				CompositionList						children;
				GuiGraphicsComposition*				parent;
				Ptr<IGuiGraphicsElement>			ownedElement;
				bool								visible;
				MinSizeLimitation					minSizeLimitation;
				IGuiGraphicsRenderTarget*			renderTarget;
				Margin								margin, internalMargin;
				Ptr<GuiGraphicsEventReceiver>		eventReceiver;

				virtual void						OnChildInserted(GuiGraphicsComposition* child);
				virtual void						OnChildRemoved(GuiGraphicsComposition* child);
				virtual void						OnParentChanged(GuiGraphicsComposition* oldParent, GuiGraphicsComposition* newParent);
				virtual Rect						GetBoundsInternal(Rect expectedBounds, MinSizeLimitation limitation);
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
				MinSizeLimitation					GetMinSizeLimitation();
				void								SetMinSizeLimitation(MinSizeLimitation value);
				IGuiGraphicsRenderTarget*			GetRenderTarget();
				void								SetRenderTarget(IGuiGraphicsRenderTarget* value);

				void								Render(Size offset);
				GuiGraphicsEventReceiver*			GetEventReceiver();

				virtual Margin						GetMargin();
				virtual void						SetMargin(Margin value);
				virtual Margin						GetInternalMargin();
				virtual void						SetInternalMargin(Margin value);
				virtual Rect						GetClientArea();
				virtual Rect						GetMinNecessaryBounds();
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
				Rect								compositionBounds;
				Margin								alignmentToParent;
				
				virtual Rect						GetUnalignedBoundsForMinNecessaryBounds();
			public:
				GuiBoundsComposition();
				~GuiBoundsComposition();

				Rect								GetMinNecessaryBounds();
				Rect								GetBounds();
				void								SetBounds(Rect value);

				void								ClearAlignmentToParent();
				Margin								GetAlignmentToParent();
				void								SetAlignmentToParent(Margin value);
				bool								IsAlignedToParent();
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

				static GuiCellOption AbsoluteOption(int value)
				{
					GuiCellOption option;
					option.composeType=Absolute;
					option.absolute=value;
					return option;
				}

				static GuiCellOption PercentageOption(double value)
				{
					GuiCellOption option;
					option.composeType=Percentage;
					option.percentage=value;
					return option;
				}

				static GuiCellOption MinSizeOption()
				{
					GuiCellOption option;
					option.composeType=MinSize;
					return option;
				}
			};

			class GuiTableComposition : public GuiBoundsComposition
			{
				friend class GuiCellComposition;
			protected:
				int											rows;
				int											columns;
				int											cellPadding;
				int											rowExtending;
				int											columnExtending;
				collections::Array<GuiCellOption>			rowOptions;
				collections::Array<GuiCellOption>			columnOptions;
				collections::Array<GuiCellComposition*>		cellCompositions;
				collections::Array<Rect>					cellBounds;
				Rect										previousGetBoundsResult;
				Size										tableContentMinSize;

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
				int									UpdateCellBoundsOffsets(
														collections::Array<int>& offsets,
														collections::Array<int>& sizes,
														int start,
														int max
														);

				Rect								GetUnalignedBoundsForMinNecessaryBounds();
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
				
				Rect								GetBounds();
				void								SetBounds(Rect value);
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