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
				friend class list::ListViewItemStyleProviderBase;
			public:
				class IStyleProvider : public virtual GuiSelectableListControl::IStyleProvider
				{
				public:
					virtual GuiSelectableButton::IStyleController*		CreateItemBackground()=0;
				};

			protected:
				IStyleProvider*									styleProvider;

			public:
				GuiListViewBase(IStyleProvider* _styleProvider, GuiListControl::IItemProvider* _itemProvider);
				~GuiListViewBase();
				
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
						virtual void							Install(IListViewItemView* view, int itemIndex)=0;
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
						ItemContent(const FontProperties& font);
						~ItemContent();

						elements::GuiBoundsComposition*					GetContentComposition()override;
						void											Install(ListViewItemStyleProvider::IListViewItemView* view, int itemIndex)override;
					};
				public:
					ListViewBigIconContentProvider();
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
						ItemContent(const FontProperties& font);
						~ItemContent();

						elements::GuiBoundsComposition*					GetContentComposition()override;
						void											Install(ListViewItemStyleProvider::IListViewItemView* view, int itemIndex)override;
					};
				public:
					ListViewSmallIconContentProvider();
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
						ItemContent(const FontProperties& font);
						~ItemContent();

						elements::GuiBoundsComposition*					GetContentComposition()override;
						void											Install(ListViewItemStyleProvider::IListViewItemView* view, int itemIndex)override;
					};
				public:
					ListViewListContentProvider();
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
						ItemContent(const FontProperties& font);
						~ItemContent();

						elements::GuiBoundsComposition*					GetContentComposition()override;
						void											Install(ListViewItemStyleProvider::IListViewItemView* view, int itemIndex)override;
					};
				public:
					ListViewDetailContentProvider();
					~ListViewDetailContentProvider();

					GuiListControl::IItemArranger*						CreatePreferredArranger()override;
					ListViewItemStyleProvider::IListViewItemContent*	CreateItemContent(const FontProperties& font)override;
				};
				
				class ListViewTileContentProvider : public Object, public virtual ListViewItemStyleProvider::IListViewItemContentProvider
				{
				protected:
					class ItemContent : public Object, public virtual ListViewItemStyleProvider::IListViewItemContent
					{
					protected:
						elements::GuiBoundsComposition*					contentComposition;

					public:
						ItemContent(const FontProperties& font);
						~ItemContent();

						elements::GuiBoundsComposition*					GetContentComposition()override;
						void											Install(ListViewItemStyleProvider::IListViewItemView* view, int itemIndex)override;
					};
				public:
					ListViewTileContentProvider();
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
						ItemContent(const FontProperties& font);
						~ItemContent();

						elements::GuiBoundsComposition*					GetContentComposition()override;
						void											Install(ListViewItemStyleProvider::IListViewItemView* view, int itemIndex)override;
					};
				public:
					ListViewInformationContentProvider();
					~ListViewInformationContentProvider();

					GuiListControl::IItemArranger*						CreatePreferredArranger()override;
					ListViewItemStyleProvider::IListViewItemContent*	CreateItemContent(const FontProperties& font)override;
				};
			}
		}
	}
}

#endif