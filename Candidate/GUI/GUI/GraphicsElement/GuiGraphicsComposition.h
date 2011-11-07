/***********************************************************************
Vczh Library++ 3.0
Developer: �����(vczh)
GacUI::Composition System

Interfaces:
***********************************************************************/

#ifndef VCZH_PRESENTATION_ELEMENTS_GUIGRAPHICSCOMPOSITION
#define VCZH_PRESENTATION_ELEMENTS_GUIGRAPHICSCOMPOSITION

#include "GuiGraphicsElement.h"
#include "GuiGraphicsTextElement.h"
#include "GuiGraphicsEventReceiver.h"

namespace vl
{
	namespace presentation
	{
		namespace controls
		{
			class IGuiStyleController;
			class GuiControl;
			class GuiControlHost;
		}

		namespace elements
		{
			class GuiGraphicsHost;

/***********************************************************************
Basic Construction
***********************************************************************/

			class GuiGraphicsComposition : public Object
			{
				typedef collections::IReadonlyList<GuiGraphicsComposition*> ICompositionList;
				typedef collections::List<GuiGraphicsComposition*> CompositionList;

				friend class controls::GuiControl;
				friend class GuiGraphicsHost;
			public:
				enum MinSizeLimitation
				{
					NoLimit,
					LimitToElement,
					LimitToElementAndChildren,
				};

				enum ParentSizeAffection
				{
					NotAffectedByParent,
					AffectedByParent,
					TotallyDecidedByParent,
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
				controls::GuiControl*				associatedControl;
				GuiGraphicsHost*					associatedHost;
				INativeCursor*						associatedCursor;

				virtual void						OnControlParentChanged(controls::GuiControl* control);
				virtual void						OnChildInserted(GuiGraphicsComposition* child);
				virtual void						OnChildRemoved(GuiGraphicsComposition* child);
				virtual void						OnParentChanged(GuiGraphicsComposition* oldParent, GuiGraphicsComposition* newParent);
				virtual void						OnRenderTargetChanged();
				
				virtual void						SetAssociatedControl(controls::GuiControl* control);
				virtual void						SetAssociatedHost(GuiGraphicsHost* host);
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
				bool								HasEventReceiver();
				GuiGraphicsComposition*				FindComposition(Point location);
				Rect								GetGlobalBounds();

				controls::GuiControl*				GetAssociatedControl();
				GuiGraphicsHost*					GetAssociatedHost();
				INativeCursor*						GetAssociatedCursor();
				void								SetAssociatedCursor(INativeCursor* cursor);

				controls::GuiControl*				GetRelatedControl();
				GuiGraphicsHost*					GetRelatedGraphicsHost();
				controls::GuiControlHost*			GetRelatedControlHost();
				INativeCursor*						GetRelatedCursor();

				virtual Margin						GetMargin();
				virtual void						SetMargin(Margin value);
				virtual Margin						GetInternalMargin();
				virtual void						SetInternalMargin(Margin value);
				virtual Rect						GetClientArea();
				
				virtual ParentSizeAffection			GetAffectionFromParent()=0;
				virtual bool						IsSizeAffectParent()=0;
				virtual Size						GetMinPreferredClientSize()=0;
				virtual Rect						GetPreferredBounds()=0;
				virtual Rect						GetBounds()=0;
			};

			class GuiGraphicsSite : public GuiGraphicsComposition
			{
			protected:

				virtual Rect						GetBoundsInternal(Rect expectedBounds);
			public:
				GuiGraphicsSite();
				~GuiGraphicsSite();
				
				ParentSizeAffection					GetAffectionFromParent();
				bool								IsSizeAffectParent();
				Size								GetMinPreferredClientSize();
				Rect								GetPreferredBounds();
			};

/***********************************************************************
Basic Compositions
***********************************************************************/

			class GuiWindowComposition : public GuiGraphicsSite
			{
			protected:
				INativeWindow*						attachedWindow;
			public:
				GuiWindowComposition();
				~GuiWindowComposition();

				INativeWindow*						GetAttachedWindow();
				void								SetAttachedWindow(INativeWindow* window);

				Rect								GetBounds();
				void								SetMargin(Margin value);
			};

			class GuiBoundsComposition : public GuiGraphicsSite
			{
			protected:
				Rect								compositionBounds;
				Rect								previousBounds;
				Margin								alignmentToParent;
				
			public:
				GuiBoundsComposition();
				~GuiBoundsComposition();

				GuiNotifyEvent						BoundsChanged;
				
				ParentSizeAffection					GetAffectionFromParent();
				Rect								GetPreferredBounds();
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
				Rect										previousBounds;
				Size										previousContentMinSize;
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
														int (*getLocation)(GuiCellComposition*),
														int (*getSpan)(GuiCellComposition*),
														int (*getRow)(int, int),
														int (*getCol)(int, int),
														int maxPass
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
				
				void								UpdateCellBoundsInternal();
				void								UpdateTableContentMinSize();
				void								OnRenderTargetChanged();
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
				
				Size								GetMinPreferredClientSize();
				Rect								GetBounds();
			};

			class GuiCellComposition : public GuiGraphicsSite
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

/***********************************************************************
Stack Compositions
***********************************************************************/

			class GuiStackComposition;
			class GuiStackItemComposition;

			class GuiStackComposition : public GuiBoundsComposition
			{
				friend class GuiStackItemComposition;

				typedef collections::List<GuiStackItemComposition*>				ItemCompositionList;
				typedef collections::IReadonlyList<GuiStackItemComposition*>	IItemCompositionList;
			public:
				enum Direction
				{
					Horizontal,
					Vertical,
				};
			protected:
				Direction							direction;
				ItemCompositionList					stackItems;
				int									padding;

				void								OnChildInserted(GuiGraphicsComposition* child);
				void								OnChildRemoved(GuiGraphicsComposition* child);
			public:
				GuiStackComposition();
				~GuiStackComposition();

				const IItemCompositionList&			GetStackItems();
				bool								InsertStackItem(int index, GuiStackItemComposition* item);

				Direction							GetDirection();
				void								SetDirection(Direction value);
				int									GetPadding();
				void								SetPadding(int value);

				Size								GetMinPreferredClientSize();
			};

			class GuiStackItemComposition : public GuiGraphicsSite
			{
				friend class GuiStackComposition;
			protected:
				GuiStackComposition*				stackParent;
				Rect								bounds;

				void								OnParentChanged(GuiGraphicsComposition* oldParent, GuiGraphicsComposition* newParent);
				Size								GetMinSize();
			public:
				GuiStackItemComposition();
				~GuiStackItemComposition();
				Rect								GetBounds();
				void								SetBounds(Rect value);
			};

/***********************************************************************
Specialized Compositions
***********************************************************************/

			class GuiSideAlignedComposition : public GuiGraphicsSite
			{
			public:
				enum Direction
				{
					Left,
					Top,
					Right,
					Bottom,
				};
			protected:
				Direction							direction;
				int									maxLength;
				double								maxRatio;
			public:
				GuiSideAlignedComposition();
				~GuiSideAlignedComposition();

				Direction							GetDirection();
				void								SetDirection(Direction value);
				int									GetMaxLength();
				void								SetMaxLength(int value);
				double								GetMaxRatio();
				void								SetMaxRatio(double value);
				
				ParentSizeAffection					GetAffectionFromParent();
				bool								IsSizeAffectParent();
				Rect								GetBounds();
			};

			class GuiPartialViewComposition : public GuiGraphicsSite
			{
			protected:
				double								wRatio;
				double								wPageSize;
				double								hRatio;
				double								hPageSize;
				Size								minSize;

			public:
				GuiPartialViewComposition();
				~GuiPartialViewComposition();

				double								GetWidthRatio();
				double								GetWidthPageSize();
				double								GetHeightRatio();
				double								GetHeightPageSize();
				void								SetWidthRatio(double value);
				void								SetWidthPageSize(double value);
				void								SetHeightRatio(double value);
				void								SetHeightPageSize(double value);
				
				ParentSizeAffection					GetAffectionFromParent();
				bool								IsSizeAffectParent();
				Rect								GetBounds();
				Size								GetMinSize();
				void								SetMinSize(Size value);
			};
		}
	}
}

#endif