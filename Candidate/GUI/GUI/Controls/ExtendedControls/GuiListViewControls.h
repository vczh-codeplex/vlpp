/***********************************************************************
Vczh Library++ 3.0
Developer: ³Âè÷å«(vczh)
GacUI::Control System

Interfaces:
***********************************************************************/

#ifndef VCZH_PRESENTATION_CONTROLS_GUILISTVIEWCONTROLS
#define VCZH_PRESENTATION_CONTROLS_GUILISTVIEWCONTROLS

#include "..\GuiListControls.h"

namespace vl
{
	namespace presentation
	{
		namespace controls
		{
			class GuiListViewBase;

			namespace list
			{

/***********************************************************************
ListView Base
***********************************************************************/

				class ListViewItemStyleProviderBase: public Object, public GuiSelectableListControl::IItemStyleProvider
				{
				protected:
					class ListViewItemStyleController : public ItemStyleControllerBase
					{
					protected:
						GuiSelectableButton*					backgroundButton;
						ListViewItemStyleProviderBase*			listViewItemStyleProvider;

					public:
						ListViewItemStyleController(ListViewItemStyleProviderBase* provider);
						~ListViewItemStyleController();

						bool									GetSelected();
						void									SetSelected(bool value);
					};

					GuiListViewBase*							listControl;

				public:
					ListViewItemStyleProviderBase();
					~ListViewItemStyleProviderBase();

					void										AttachListControl(GuiListControl* value)override;
					void										DetachListControl()override;
					int											GetItemStyleId(int itemIndex)override;
					void										SetStyleSelected(GuiListControl::IItemStyleController* style, bool value)override;
				};
			}

			class GuiListViewBase : public GuiSelectableListControl
			{
			public:
				class IStyleProvider : public virtual GuiSelectableListControl::IStyleProvider
				{
				public:
					virtual GuiSelectableButton::IStyleController*		CreateItemBackground()=0;
					virtual GuiSelectableButton::IStyleController*		CreateColumnStyle()=0;
					virtual Color										GetPrimaryTextColor()=0;
					virtual Color										GetSecondaryTextColor()=0;
					virtual Color										GetItemSeparatorColor()=0;
				};

			protected:
				IStyleProvider*									styleProvider;

			public:
				GuiListViewBase(IStyleProvider* _styleProvider, GuiListControl::IItemProvider* _itemProvider);
				~GuiListViewBase();
				
				IStyleProvider*									GetListViewStyleProvider();
				Ptr<GuiListControl::IItemStyleProvider>			SetStyleProvider(Ptr<GuiListControl::IItemStyleProvider> value);
			};

/***********************************************************************
ListView ItemStyleProvider
***********************************************************************/

			namespace list
			{
				class ListViewItemStyleProvider : public ListViewItemStyleProviderBase
				{
				public:
					class IListViewItemView : public virtual GuiListControl::IItemPrimaryTextView
					{
					public:
						static const wchar_t*					Identifier;

						virtual Ptr<GuiImageData>				GetSmallImage(int itemIndex)=0;
						virtual Ptr<GuiImageData>				GetLargeImage(int itemIndex)=0;
						virtual WString							GetText(int itemIndex)=0;
						virtual WString							GetSubItem(int itemIndex, int index)=0;

						virtual int								GetDataColumnCount()=0;
						virtual int								GetDataColumn(int index)=0;

						virtual int								GetColumnCount()=0;
						virtual WString							GetColumnText(int index)=0;
					};

					class IListViewItemContent : public virtual Interface
					{
					public:
						virtual elements::GuiBoundsComposition*	GetContentComposition()=0;
						virtual elements::GuiBoundsComposition*	GetBackgroundDecorator()=0;
						virtual void							Install(GuiListViewBase::IStyleProvider* styleProvider, IListViewItemView* view, int itemIndex)=0;
					};

					class IListViewItemContentProvider : public virtual Interface
					{
					public:
						virtual GuiListControl::IItemCoordinateTransformer*		CreatePreferredCoordinateTransformer()=0;
						virtual GuiListControl::IItemArranger*					CreatePreferredArranger()=0;
						virtual IListViewItemContent*							CreateItemContent(const FontProperties& font)=0;
						virtual void											AttachListControl(GuiListControl* value)=0;
						virtual void											DetachListControl()=0;
					};

					class ListViewContentItemStyleController : public ListViewItemStyleController
					{
					protected:
						ListViewItemStyleProvider*				listViewItemStyleProvider;
						Ptr<IListViewItemContent>				content;
					public:
						ListViewContentItemStyleController(ListViewItemStyleProvider* provider);
						~ListViewContentItemStyleController();

						IListViewItemContent*					GetItemContent();
						void									Install(IListViewItemView* view, int itemIndex);
					};

				protected:

					typedef collections::List<GuiListControl::IItemStyleController*>				ItemStyleList;
					typedef collections::IReadonlyList<GuiListControl::IItemStyleController*>		IItemStyleList;

					IListViewItemView*							listViewItemView;
					Ptr<IListViewItemContentProvider>			listViewItemContentProvider;
					ItemStyleList								itemStyles;
				public:
					ListViewItemStyleProvider(IListViewItemContentProvider* itemContentProvider);
					~ListViewItemStyleProvider();

					void										AttachListControl(GuiListControl* value)override;
					void										DetachListControl()override;
					GuiListControl::IItemStyleController*		CreateItemStyle(int styleId)override;
					void										DestroyItemStyle(GuiListControl::IItemStyleController* style)override;
					void										Install(GuiListControl::IItemStyleController* style, int itemIndex)override;

					const IItemStyleList&						GetCreatedItemStyles();
					bool										IsItemStyleAttachedToListView(GuiListControl::IItemStyleController* itemStyle);
				};
			}

/***********************************************************************
ListView ItemContentProvider
***********************************************************************/

			namespace list
			{
				class ListViewBigIconContentProvider : public Object, public virtual ListViewItemStyleProvider::IListViewItemContentProvider
				{
				protected:
					class ItemContent : public Object, public virtual ListViewItemStyleProvider::IListViewItemContent
					{
					protected:
						elements::GuiBoundsComposition*					contentComposition;
						elements::GuiImageFrameElement*					image;
						elements::GuiSolidLabelElement*					text;

					public:
						ItemContent(Size iconSize, const FontProperties& font);
						~ItemContent();

						elements::GuiBoundsComposition*					GetContentComposition()override;
						elements::GuiBoundsComposition*					GetBackgroundDecorator()override;
						void											Install(GuiListViewBase::IStyleProvider* styleProvider, ListViewItemStyleProvider::IListViewItemView* view, int itemIndex)override;
					};

					Size												iconSize;
				public:
					ListViewBigIconContentProvider(Size _iconSize=Size(32, 32));
					~ListViewBigIconContentProvider();

					GuiListControl::IItemCoordinateTransformer*			CreatePreferredCoordinateTransformer()override;
					GuiListControl::IItemArranger*						CreatePreferredArranger()override;
					ListViewItemStyleProvider::IListViewItemContent*	CreateItemContent(const FontProperties& font)override;
					void												AttachListControl(GuiListControl* value)override;
					void												DetachListControl()override;
				};
				
				class ListViewSmallIconContentProvider : public Object, public virtual ListViewItemStyleProvider::IListViewItemContentProvider
				{
				protected:
					class ItemContent : public Object, public virtual ListViewItemStyleProvider::IListViewItemContent
					{
					protected:
						elements::GuiBoundsComposition*					contentComposition;
						elements::GuiImageFrameElement*					image;
						elements::GuiSolidLabelElement*					text;

					public:
						ItemContent(Size iconSize, const FontProperties& font);
						~ItemContent();

						elements::GuiBoundsComposition*					GetContentComposition()override;
						elements::GuiBoundsComposition*					GetBackgroundDecorator()override;
						void											Install(GuiListViewBase::IStyleProvider* styleProvider, ListViewItemStyleProvider::IListViewItemView* view, int itemIndex)override;
					};

					Size												iconSize;
				public:
					ListViewSmallIconContentProvider(Size _iconSize=Size(16, 16));
					~ListViewSmallIconContentProvider();
					
					GuiListControl::IItemCoordinateTransformer*			CreatePreferredCoordinateTransformer()override;
					GuiListControl::IItemArranger*						CreatePreferredArranger()override;
					ListViewItemStyleProvider::IListViewItemContent*	CreateItemContent(const FontProperties& font)override;
					void												AttachListControl(GuiListControl* value)override;
					void												DetachListControl()override;
				};
				
				class ListViewListContentProvider : public Object, public virtual ListViewItemStyleProvider::IListViewItemContentProvider
				{
				protected:
					class ItemContent : public Object, public virtual ListViewItemStyleProvider::IListViewItemContent
					{
					protected:
						elements::GuiBoundsComposition*					contentComposition;
						elements::GuiImageFrameElement*					image;
						elements::GuiSolidLabelElement*					text;

					public:
						ItemContent(Size iconSize, const FontProperties& font);
						~ItemContent();

						elements::GuiBoundsComposition*					GetContentComposition()override;
						elements::GuiBoundsComposition*					GetBackgroundDecorator()override;
						void											Install(GuiListViewBase::IStyleProvider* styleProvider, ListViewItemStyleProvider::IListViewItemView* view, int itemIndex)override;
					};

					Size												iconSize;
				public:
					ListViewListContentProvider(Size _iconSize=Size(16, 16));
					~ListViewListContentProvider();
					
					GuiListControl::IItemCoordinateTransformer*			CreatePreferredCoordinateTransformer()override;
					GuiListControl::IItemArranger*						CreatePreferredArranger()override;
					ListViewItemStyleProvider::IListViewItemContent*	CreateItemContent(const FontProperties& font)override;
					void												AttachListControl(GuiListControl* value)override;
					void												DetachListControl()override;
				};
				
				class ListViewTileContentProvider : public Object, public virtual ListViewItemStyleProvider::IListViewItemContentProvider
				{
				protected:
					class ItemContent : public Object, public virtual ListViewItemStyleProvider::IListViewItemContent
					{
						typedef collections::Array<elements::GuiSolidLabelElement*>		DataTextElementArray;
					protected:
						elements::GuiBoundsComposition*					contentComposition;
						elements::GuiImageFrameElement*					image;
						elements::GuiSolidLabelElement*					text;
						elements::GuiTableComposition*					textTable;
						DataTextElementArray							dataTexts;

						void											RemoveTextElement(int textRow);
						elements::GuiSolidLabelElement*					CreateTextElement(int textRow, const FontProperties& font);
						void											ResetTextTable(int textRows);
					public:
						ItemContent(Size iconSize, const FontProperties& font);
						~ItemContent();

						elements::GuiBoundsComposition*					GetContentComposition()override;
						elements::GuiBoundsComposition*					GetBackgroundDecorator()override;
						void											Install(GuiListViewBase::IStyleProvider* styleProvider, ListViewItemStyleProvider::IListViewItemView* view, int itemIndex)override;
					};

					Size												iconSize;
				public:
					ListViewTileContentProvider(Size _iconSize=Size(32, 32));
					~ListViewTileContentProvider();
					
					GuiListControl::IItemCoordinateTransformer*			CreatePreferredCoordinateTransformer()override;
					GuiListControl::IItemArranger*						CreatePreferredArranger()override;
					ListViewItemStyleProvider::IListViewItemContent*	CreateItemContent(const FontProperties& font)override;
					void												AttachListControl(GuiListControl* value)override;
					void												DetachListControl()override;
				};
				
				class ListViewInformationContentProvider : public Object, public virtual ListViewItemStyleProvider::IListViewItemContentProvider
				{
				protected:
					class ItemContent : public Object, public virtual ListViewItemStyleProvider::IListViewItemContent
					{
						typedef collections::Array<elements::GuiSolidLabelElement*>		DataTextElementArray;
					protected:
						FontProperties									baselineFont;
						elements::GuiBoundsComposition*					contentComposition;
						elements::GuiImageFrameElement*					image;
						elements::GuiSolidLabelElement*					text;
						elements::GuiTableComposition*					textTable;
						DataTextElementArray							dataTexts;

						elements::GuiSolidBackgroundElement*			bottomLine;
						elements::GuiBoundsComposition*					bottomLineComposition;

					public:
						ItemContent(Size iconSize, const FontProperties& font);
						~ItemContent();

						elements::GuiBoundsComposition*					GetContentComposition()override;
						elements::GuiBoundsComposition*					GetBackgroundDecorator()override;
						void											Install(GuiListViewBase::IStyleProvider* styleProvider, ListViewItemStyleProvider::IListViewItemView* view, int itemIndex)override;
					};

					Size												iconSize;
				public:
					ListViewInformationContentProvider(Size _iconSize=Size(32, 32));
					~ListViewInformationContentProvider();
					
					GuiListControl::IItemCoordinateTransformer*			CreatePreferredCoordinateTransformer()override;
					GuiListControl::IItemArranger*						CreatePreferredArranger()override;
					ListViewItemStyleProvider::IListViewItemContent*	CreateItemContent(const FontProperties& font)override;
					void												AttachListControl(GuiListControl* value)override;
					void												DetachListControl()override;
				};

/***********************************************************************
ListView ItemContentProvider(Detailed)
***********************************************************************/

				class ListViewColumnItemArranger : public FixedHeightItemArranger
				{
					typedef collections::List<GuiButton*>							ColumnHeaderButtonList;
					typedef collections::List<elements::GuiBoundsComposition*>		ColumnHeaderSplitterList;
				public:
					static const int							SplitterWidth=8;

					class IColumnItemViewCallback : public virtual Interface
					{
					public:
						virtual void							OnColumnChanged()=0;
						virtual void							OnColumnSizeChanged(int index)=0;
					};

					class IColumnItemView : public virtual Interface
					{
					public:
						static const wchar_t*					Identifier;
						
						virtual bool							AttachCallback(IColumnItemViewCallback* value)=0;
						virtual bool							DetachCallback(IColumnItemViewCallback* value)=0;
						virtual int								GetColumnCount()=0;
						virtual WString							GetColumnText(int index)=0;
						virtual int								GetColumnSize(int index)=0;
						virtual void							SetColumnSize(int index, int value)=0;
					};
				protected:
					class ColumnItemViewCallback : public Object, public virtual IColumnItemViewCallback
					{
					protected:
						ListViewColumnItemArranger*				arranger;
					public:
						ColumnItemViewCallback(ListViewColumnItemArranger* _arranger);
						~ColumnItemViewCallback();

						void									OnColumnChanged();
						void									OnColumnSizeChanged(int index);
					};

					GuiListViewBase*							listView;
					GuiListViewBase::IStyleProvider*			styleProvider;
					IColumnItemView*							columnItemView;
					Ptr<ColumnItemViewCallback>					columnItemViewCallback;
					elements::GuiStackComposition*				columnHeaders;
					ColumnHeaderButtonList						columnHeaderButtons;
					ColumnHeaderSplitterList					columnHeaderSplitters;
					bool										splitterDragging;
					int											splitterLatestX;

					void										ColumnHeaderSplitterLeftButtonDown(elements::GuiGraphicsComposition* sender, elements::GuiMouseEventArgs& arguments);
					void										ColumnHeaderSplitterLeftButtonUp(elements::GuiGraphicsComposition* sender, elements::GuiMouseEventArgs& arguments);
					void										ColumnHeaderSplitterMouseMove(elements::GuiGraphicsComposition* sender, elements::GuiMouseEventArgs& arguments);

					void										RearrangeItemBounds()override;
					int											GetWidth()override;
					int											GetYOffset()override;
					Size										OnCalculateTotalSize()override;
					void										DeleteColumnButtons();
					void										RebuildColumns();
					void										UpdateColumnSize(int index);
				public:
					ListViewColumnItemArranger();
					~ListViewColumnItemArranger();

					void										AttachListControl(GuiListControl* value)override;
					void										DetachListControl()override;
				};
				
				class ListViewDetailContentProvider
					: public Object
					, public virtual ListViewItemStyleProvider::IListViewItemContentProvider
					, protected virtual ListViewColumnItemArranger::IColumnItemViewCallback
				{
				protected:
					class ItemContent : public Object, public virtual ListViewItemStyleProvider::IListViewItemContent
					{
						typedef collections::List<elements::GuiSolidLabelElement*>		SubItemList;
					protected:
						elements::GuiBoundsComposition*					contentComposition;
						elements::GuiImageFrameElement*					image;
						elements::GuiSolidLabelElement*					text;
						elements::GuiTableComposition*					textTable;
						SubItemList										subItems;

						GuiListControl::IItemProvider*					itemProvider;
						ListViewColumnItemArranger::IColumnItemView*	columnItemView;

					public:
						ItemContent(Size iconSize, const FontProperties& font, GuiListControl::IItemProvider* _itemProvider);
						~ItemContent();

						elements::GuiBoundsComposition*					GetContentComposition()override;
						elements::GuiBoundsComposition*					GetBackgroundDecorator()override;
						void											UpdateSubItemSize();
						void											Install(GuiListViewBase::IStyleProvider* styleProvider, ListViewItemStyleProvider::IListViewItemView* view, int itemIndex)override;
					};

					Size												iconSize;
					GuiListControl::IItemProvider*						itemProvider;
					ListViewColumnItemArranger::IColumnItemView*		columnItemView;
					ListViewItemStyleProvider*							listViewItemStyleProvider;

					void												OnColumnChanged()override;
					void												OnColumnSizeChanged(int index)override;
				public:
					ListViewDetailContentProvider(Size _iconSize=Size(16, 16));
					~ListViewDetailContentProvider();
					
					GuiListControl::IItemCoordinateTransformer*			CreatePreferredCoordinateTransformer()override;
					GuiListControl::IItemArranger*						CreatePreferredArranger()override;
					ListViewItemStyleProvider::IListViewItemContent*	CreateItemContent(const FontProperties& font)override;
					void												AttachListControl(GuiListControl* value)override;
					void												DetachListControl()override;
				};
			}

/***********************************************************************
ListView
***********************************************************************/

			namespace list
			{
				class ListViewItem
				{
				public:
					Ptr<GuiImageData>							smallImage;
					Ptr<GuiImageData>							largeImage;
					WString										text;
					collections::List<WString>					subItems;
				};

				class ListViewColumn
				{
				public:
					WString										text;
					int											size;

					ListViewColumn(const WString& _text=L"", int _size=160);
				};

				class ListViewItemProvider
					: public ListProvider<Ptr<ListViewItem>>
					, protected virtual ListViewItemStyleProvider::IListViewItemView
					, protected virtual ListViewColumnItemArranger::IColumnItemView
				{
					typedef collections::List<ListViewColumnItemArranger::IColumnItemViewCallback*>		ColumnItemViewCallbackList;
				protected:
					collections::List<int>						dataColumns;
					collections::List<Ptr<ListViewColumn>>		columns;
					ColumnItemViewCallbackList					columnItemViewCallbacks;

					bool										ContainsPrimaryText(int itemIndex)override;
					WString										GetPrimaryTextViewText(int itemIndex)override;
					Ptr<GuiImageData>							GetSmallImage(int itemIndex)override;
					Ptr<GuiImageData>							GetLargeImage(int itemIndex)override;
					WString										GetText(int itemIndex)override;
					WString										GetSubItem(int itemIndex, int index)override;
					int											GetDataColumnCount()override;
					int											GetDataColumn(int index)override;

					bool										AttachCallback(ListViewColumnItemArranger::IColumnItemViewCallback* value)override;
					bool										DetachCallback(ListViewColumnItemArranger::IColumnItemViewCallback* value)override;
					int											GetColumnCount()override;
					WString										GetColumnText(int index)override;
					int											GetColumnSize(int index)override;
					void										SetColumnSize(int index, int value)override;
				public:
					ListViewItemProvider();
					~ListViewItemProvider();

					Interface*									RequestView(const WString& identifier)override;
					void										ReleaseView(Interface* view)override;

					collections::IList<int>&					GetDataColumns();
					void										NotifyDataColumnsUpdated();
					collections::IList<Ptr<ListViewColumn>>&	GetColumns();
					void										NotifyColumnsUpdated();
				};
			}

			class GuiVirtualListView : public GuiListViewBase
			{
			public:
				GuiVirtualListView(IStyleProvider* _styleProvider, GuiListControl::IItemProvider* _itemProvider);
				~GuiVirtualListView();
				
				void											ChangeItemStyle(list::ListViewItemStyleProvider::IListViewItemContentProvider* contentProvider);
			};

			class GuiListView : public GuiVirtualListView
			{
			protected:
				list::ListViewItemProvider*						items;
			public:
				GuiListView(IStyleProvider* _styleProvider);
				~GuiListView();
				
				list::ListViewItemProvider&						GetItems();
			};
		}
	}
}

#endif