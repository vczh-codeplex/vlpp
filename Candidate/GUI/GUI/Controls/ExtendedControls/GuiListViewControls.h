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
					void										DestroyItemStyle(GuiListControl::IItemStyleController* style)override;
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
					virtual Color										GetPrimaryTextColor()=0;
					virtual Color										GetSecondaryTextColor()=0;
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
ListView
***********************************************************************/

			namespace list
			{
				class ListViewItemStyleProvider : public ListViewItemStyleProviderBase
				{
				public:
					class IListViewItemView : public virtual Interface
					{
					public:
						static const wchar_t*					Identifier;

						virtual Ptr<GuiImageData>				GetSmallImage(int itemIndex)=0;
						virtual Ptr<GuiImageData>				GetLargeImage(int itemIndex)=0;
						virtual WString							GetText(int itemIndex)=0;
						virtual WString							GetSubItem(int itemIndex, int index)=0;

						virtual int								GetDataColumnCount()=0;
						virtual int								GetDataColumn(int index)=0;
					};

					class IListViewItemContent : public virtual Interface
					{
					public:
						virtual elements::GuiBoundsComposition*	GetContentComposition()=0;
						virtual void							Install(GuiListViewBase::IStyleProvider* styleProvider, IListViewItemView* view, int itemIndex)=0;
					};

					class IListViewItemContentProvider : public virtual Interface
					{
					public:
						virtual GuiListControl::IItemArranger*	CreatePreferredArranger()=0;
						virtual IListViewItemContent*			CreateItemContent(const FontProperties& font)=0;
					};
				protected:

					class ListViewContentItemStyleController : public ListViewItemStyleController
					{
					protected:
						ListViewItemStyleProvider*				listViewItemStyleProvider;
						Ptr<IListViewItemContent>				content;
					public:
						ListViewContentItemStyleController(ListViewItemStyleProvider* provider);
						~ListViewContentItemStyleController();

						void									Install(IListViewItemView* view, int itemIndex);
					};

					IListViewItemView*							listViewItemView;
					Ptr<IListViewItemContentProvider>			listViewItemContentProvider;

				public:
					ListViewItemStyleProvider(IListViewItemContentProvider* itemContentProvider);
					~ListViewItemStyleProvider();

					void										AttachListControl(GuiListControl* value)override;
					void										DetachListControl()override;
					GuiListControl::IItemStyleController*		CreateItemStyle(int styleId)override;
					void										Install(GuiListControl::IItemStyleController* style, int itemIndex)override;
				};

				class ListViewItem
				{
				public:
					Ptr<GuiImageData>							smallImage;
					Ptr<GuiImageData>							largeImage;
					WString										text;
					collections::List<WString>					subItems;
				};

				class ListViewItemProvider : public ListProvider<Ptr<ListViewItem>>, protected virtual ListViewItemStyleProvider::IListViewItemView
				{
				protected:
					collections::List<int>						dataColumns;

					Ptr<GuiImageData>							GetSmallImage(int itemIndex)override;
					Ptr<GuiImageData>							GetLargeImage(int itemIndex)override;
					WString										GetText(int itemIndex)override;
					WString										GetSubItem(int itemIndex, int index)override;
					int											GetDataColumnCount()override;
					int											GetDataColumn(int index)override;
				public:
					ListViewItemProvider();
					~ListViewItemProvider();

					Interface*									RequestView(const WString& identifier)override;
					void										ReleaseView(Interface* view)override;
					collections::IList<int>&					GetDataColumns();
				};
			}

			class GuiListView : public GuiListViewBase
			{
			protected:
				list::ListViewItemProvider*						items;
			public:
				GuiListView(IStyleProvider* _styleProvider);
				~GuiListView();
				
				list::ListViewItemProvider&						GetItems();
				void											ChangeItemStyle(list::ListViewItemStyleProvider::IListViewItemContentProvider* contentProvider);
			};

/***********************************************************************
ListView ItemStyles
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
						void											Install(GuiListViewBase::IStyleProvider* styleProvider, ListViewItemStyleProvider::IListViewItemView* view, int itemIndex)override;
					};

					Size												iconSize;
				public:
					ListViewBigIconContentProvider(Size _iconSize=Size(32, 32));
					~ListViewBigIconContentProvider();

					GuiListControl::IItemArranger*						CreatePreferredArranger()override;
					ListViewItemStyleProvider::IListViewItemContent*	CreateItemContent(const FontProperties& font)override;
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
						void											Install(GuiListViewBase::IStyleProvider* styleProvider, ListViewItemStyleProvider::IListViewItemView* view, int itemIndex)override;
					};

					Size												iconSize;
				public:
					ListViewSmallIconContentProvider(Size _iconSize=Size(16, 16));
					~ListViewSmallIconContentProvider();

					GuiListControl::IItemArranger*						CreatePreferredArranger()override;
					ListViewItemStyleProvider::IListViewItemContent*	CreateItemContent(const FontProperties& font)override;
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
						void											Install(GuiListViewBase::IStyleProvider* styleProvider, ListViewItemStyleProvider::IListViewItemView* view, int itemIndex)override;
					};

					Size												iconSize;
				public:
					ListViewListContentProvider(Size _iconSize=Size(16, 16));
					~ListViewListContentProvider();

					GuiListControl::IItemArranger*						CreatePreferredArranger()override;
					ListViewItemStyleProvider::IListViewItemContent*	CreateItemContent(const FontProperties& font)override;
				};
				
				class ListViewDetailContentProvider : public Object, public virtual ListViewItemStyleProvider::IListViewItemContentProvider
				{
				protected:
					class ItemContent : public Object, public virtual ListViewItemStyleProvider::IListViewItemContent
					{
					protected:
						elements::GuiBoundsComposition*					contentComposition;

					public:
						ItemContent(Size iconSize, const FontProperties& font);
						~ItemContent();

						elements::GuiBoundsComposition*					GetContentComposition()override;
						void											Install(GuiListViewBase::IStyleProvider* styleProvider, ListViewItemStyleProvider::IListViewItemView* view, int itemIndex)override;
					};

					Size												iconSize;
				public:
					ListViewDetailContentProvider(Size _iconSize=Size(16, 16));
					~ListViewDetailContentProvider();

					GuiListControl::IItemArranger*						CreatePreferredArranger()override;
					ListViewItemStyleProvider::IListViewItemContent*	CreateItemContent(const FontProperties& font)override;
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
						void											Install(GuiListViewBase::IStyleProvider* styleProvider, ListViewItemStyleProvider::IListViewItemView* view, int itemIndex)override;
					};

					Size												iconSize;
				public:
					ListViewTileContentProvider(Size _iconSize=Size(32, 32));
					~ListViewTileContentProvider();

					GuiListControl::IItemArranger*						CreatePreferredArranger()override;
					ListViewItemStyleProvider::IListViewItemContent*	CreateItemContent(const FontProperties& font)override;
				};
				
				class ListViewInformationContentProvider : public Object, public virtual ListViewItemStyleProvider::IListViewItemContentProvider
				{
				protected:
					class ItemContent : public Object, public virtual ListViewItemStyleProvider::IListViewItemContent
					{
					protected:
						elements::GuiBoundsComposition*					contentComposition;

					public:
						ItemContent(Size iconSize, const FontProperties& font);
						~ItemContent();

						elements::GuiBoundsComposition*					GetContentComposition()override;
						void											Install(GuiListViewBase::IStyleProvider* styleProvider, ListViewItemStyleProvider::IListViewItemView* view, int itemIndex)override;
					};

					Size												iconSize;
				public:
					ListViewInformationContentProvider(Size _iconSize=Size(32, 32));
					~ListViewInformationContentProvider();

					GuiListControl::IItemArranger*						CreatePreferredArranger()override;
					ListViewItemStyleProvider::IListViewItemContent*	CreateItemContent(const FontProperties& font)override;
				};
			}
		}
	}
}

#endif