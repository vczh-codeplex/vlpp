#include "GuiListViewControls.h"

namespace vl
{
	namespace presentation
	{
		namespace controls
		{
			using namespace elements;

			namespace list
			{

/***********************************************************************
ListViewItemStyleProviderBase::TextItemStyleController
***********************************************************************/

				ListViewItemStyleProviderBase::ListViewItemStyleController::ListViewItemStyleController(ListViewItemStyleProviderBase* provider)
					:ItemStyleControllerBase(provider, 0)
					,backgroundButton(0)
					,listViewItemStyleProvider(provider)
				{
					backgroundButton=new GuiSelectableButton(listViewItemStyleProvider->listControl->styleProvider->CreateItemBackground());
					backgroundButton->SetAutoSelection(false);
					Initialize(backgroundButton->GetBoundsComposition(), backgroundButton);
				}

				ListViewItemStyleProviderBase::ListViewItemStyleController::~ListViewItemStyleController()
				{
				}

				bool ListViewItemStyleProviderBase::ListViewItemStyleController::GetSelected()
				{
					return backgroundButton->GetSelected();
				}

				void ListViewItemStyleProviderBase::ListViewItemStyleController::SetSelected(bool value)
				{
					backgroundButton->SetSelected(value);
				}

/***********************************************************************
ListViewItemStyleProviderBase
***********************************************************************/

				ListViewItemStyleProviderBase::ListViewItemStyleProviderBase()
					:listControl(0)
				{
				}

				ListViewItemStyleProviderBase::~ListViewItemStyleProviderBase()
				{
				}

				void ListViewItemStyleProviderBase::AttachListControl(GuiListControl* value)
				{
					listControl=dynamic_cast<GuiListViewBase*>(value);
				}

				void ListViewItemStyleProviderBase::DetachListControl()
				{
					listControl=0;
				}

				int ListViewItemStyleProviderBase::GetItemStyleId(int itemIndex)
				{
					return 0;
				}

				void ListViewItemStyleProviderBase::DestroyItemStyle(GuiListControl::IItemStyleController* style)
				{
					delete dynamic_cast<ListViewItemStyleController*>(style);
				}

				void ListViewItemStyleProviderBase::SetStyleSelected(GuiListControl::IItemStyleController* style, bool value)
				{
					ListViewItemStyleController* textStyle=dynamic_cast<ListViewItemStyleController*>(style);
					textStyle->SetSelected(value);
				}
			}

/***********************************************************************
GuiListViewBase
***********************************************************************/

			GuiListViewBase::GuiListViewBase(IStyleProvider* _styleProvider, GuiListControl::IItemProvider* _itemProvider)
				:GuiSelectableListControl(_styleProvider, _itemProvider)
				,styleProvider(0)
			{
				styleProvider=dynamic_cast<IStyleProvider*>(styleController->GetStyleProvider());
			}

			GuiListViewBase::~GuiListViewBase()
			{
			}

			Ptr<GuiListControl::IItemStyleProvider> GuiListViewBase::SetStyleProvider(Ptr<GuiListControl::IItemStyleProvider> value)
			{
				if(value.Cast<list::ListViewItemStyleProvider>())
				{
					return GuiSelectableListControl::SetStyleProvider(value);
				}
				else
				{
					return 0;
				}
			}

			namespace list
			{

/***********************************************************************
ListViewItemStyleProvider::ListViewContentItemStyleController
***********************************************************************/

				ListViewItemStyleProvider::ListViewContentItemStyleController::ListViewContentItemStyleController(ListViewItemStyleProvider* provider)
					:ListViewItemStyleController(provider)
					,listViewItemStyleProvider(provider)
				{
					content=listViewItemStyleProvider->listViewItemContentProvider->CreateItemContent(backgroundButton->GetFont());
					GuiBoundsComposition* composition=content->GetContentComposition();
					composition->SetAlignmentToParent(Margin(0, 0, 0, 0));
					backgroundButton->GetContainerComposition()->AddChild(composition);
				}

				ListViewItemStyleProvider::ListViewContentItemStyleController::~ListViewContentItemStyleController()
				{
				}

				void ListViewItemStyleProvider::ListViewContentItemStyleController::Install(IListViewItemView* view, int itemIndex)
				{
					content->Install(view, itemIndex);
				}

/***********************************************************************
ListViewItemStyleProvider
***********************************************************************/

				const wchar_t* ListViewItemStyleProvider::IListViewItemView::Identifier = L"vl::presentation::controls::list::ListViewItemStyleProvider::IListViewItemView";

				ListViewItemStyleProvider::ListViewItemStyleProvider(IListViewItemContentProvider* itemContentProvider)
					:listViewItemView(0)
					,listViewItemContentProvider(itemContentProvider)
				{
				}

				ListViewItemStyleProvider::~ListViewItemStyleProvider()
				{
				}

				void ListViewItemStyleProvider::AttachListControl(GuiListControl* value)
				{
					ListViewItemStyleProviderBase::AttachListControl(value);
					listViewItemView=dynamic_cast<IListViewItemView*>(value->GetItemProvider()->RequestView(IListViewItemView::Identifier));
				}

				void ListViewItemStyleProvider::DetachListControl()
				{
					listControl->GetItemProvider()->ReleaseView(listViewItemView);
					listViewItemView=0;
					ListViewItemStyleProviderBase::DetachListControl();
				}

				GuiListControl::IItemStyleController* ListViewItemStyleProvider::CreateItemStyle(int styleId)
				{
					return new ListViewContentItemStyleController(this);
				}

				void ListViewItemStyleProvider::Install(GuiListControl::IItemStyleController* style, int itemIndex)
				{
					ListViewContentItemStyleController* itemStyle=dynamic_cast<ListViewContentItemStyleController*>(style);
					itemStyle->Install(listViewItemView, itemIndex);
				}

/***********************************************************************
ListViewItemProvider
***********************************************************************/

				Ptr<GuiImageData> ListViewItemProvider::GetSmallImage(int itemIndex)
				{
					return Get(itemIndex)->smallImage;
				}

				Ptr<GuiImageData> ListViewItemProvider::GetLargeImage(int itemIndex)
				{
					return Get(itemIndex)->largeImage;
				}

				WString ListViewItemProvider::GetText(int itemIndex)
				{
					return Get(itemIndex)->text;
				}

				WString ListViewItemProvider::GetSubItem(int itemIndex, int index)
				{
					Ptr<ListViewItem> item=Get(itemIndex);
					if(index<0 || index>=item->subItems.Count())
					{
						return L"";
					}
					else
					{
						return item->subItems[index];
					}
				}

				int ListViewItemProvider::GetDataColumnCount()
				{
					return dataColumns.Count();
				}

				int ListViewItemProvider::GetDataColumn(int index)
				{
					return dataColumns[index];
				}

				ListViewItemProvider::ListViewItemProvider()
				{
				}

				ListViewItemProvider::~ListViewItemProvider()
				{
				}

				Interface* ListViewItemProvider::RequestView(const WString& identifier)
				{
					if(identifier==ListViewItemStyleProvider::IListViewItemView::Identifier)
					{
						return (ListViewItemStyleProvider::IListViewItemView*)this;
					}
					else
					{
						return 0;
					}
				}

				void ListViewItemProvider::ReleaseView(Interface* view)
				{
				}

				collections::IList<int>& ListViewItemProvider::GetDataColumns()
				{
					return dataColumns.Wrap();
				}
			}

/***********************************************************************
GuiListView
***********************************************************************/

			GuiListView::GuiListView(IStyleProvider* _styleProvider)
				:GuiListViewBase(_styleProvider, new list::ListViewItemProvider)
				,items(0)
			{
				items=dynamic_cast<list::ListViewItemProvider*>(itemProvider.Obj());
				ChangeItemStyle(new list::ListViewBigIconContentProvider);
			}

			GuiListView::~GuiListView()
			{
			}

			list::ListViewItemProvider& GuiListView::GetItems()
			{
				return *items;
			}

			void GuiListView::ChangeItemStyle(list::ListViewItemStyleProvider::IListViewItemContentProvider* contentProvider)
			{
				SetStyleProvider(new list::ListViewItemStyleProvider(contentProvider));
				SetArranger(contentProvider->CreatePreferredArranger());
			}

			namespace list
			{
				
/***********************************************************************
ListViewBigIconContentProvider
***********************************************************************/

				ListViewBigIconContentProvider::ItemContent::ItemContent(const FontProperties& font)
					:contentComposition(0)
				{
					contentComposition=new GuiBoundsComposition;
					contentComposition->SetMinSizeLimitation(GuiGraphicsComposition::LimitToElementAndChildren);

					GuiTableComposition* table=new GuiTableComposition;
					contentComposition->AddChild(table);
					table->SetRowsAndColumns(2, 3);
					table->SetRowOption(0, GuiCellOption::MinSizeOption());
					table->SetRowOption(1, GuiCellOption::MinSizeOption());
					table->SetColumnOption(0, GuiCellOption::PercentageOption(0.5));
					table->SetColumnOption(1, GuiCellOption::MinSizeOption());
					table->SetColumnOption(2, GuiCellOption::PercentageOption(0.5));
					table->SetAlignmentToParent(Margin(0, 0, 0, 0));
					table->SetCellPadding(5);
					{
						GuiCellComposition* cell=new GuiCellComposition;
						table->AddChild(cell);
						cell->SetSite(0, 1, 1, 1);
						cell->SetPreferredMinSize(Size(32, 32));

						image=GuiImageFrameElement::Create();
						image->SetStretch(true);
						cell->SetOwnedElement(image);
					}
					{
						GuiCellComposition* cell=new GuiCellComposition;
						table->AddChild(cell);
						cell->SetMinSizeLimitation(GuiGraphicsComposition::NoLimit);
						cell->SetSite(1, 0, 1, 3);
						cell->SetPreferredMinSize(Size(64, 40));

						text=GuiSolidLabelElement::Create();
						text->SetAlignments(Alignment::Center, Alignment::Top);
						text->SetFont(font);
						text->SetWrapLine(true);
						text->SetEllipse(true);
						cell->SetOwnedElement(text);
					}
				}

				ListViewBigIconContentProvider::ItemContent::~ItemContent()
				{
				}

				elements::GuiBoundsComposition* ListViewBigIconContentProvider::ItemContent::GetContentComposition()
				{
					return contentComposition;
				}

				void ListViewBigIconContentProvider::ItemContent::Install(ListViewItemStyleProvider::IListViewItemView* view, int itemIndex)
				{
					Ptr<GuiImageData> imageData=view->GetLargeImage(itemIndex);
					if(imageData)
					{
						image->SetImage(imageData->GetImage(), imageData->GetFrameIndex());
					}
					else
					{
						image->SetImage(0);
					}
					text->SetText(view->GetText(itemIndex));
				}

				ListViewBigIconContentProvider::ListViewBigIconContentProvider()
				{
				}

				ListViewBigIconContentProvider::~ListViewBigIconContentProvider()
				{
				}

				GuiListControl::IItemArranger* ListViewBigIconContentProvider::CreatePreferredArranger()
				{
					return new FixedSizeMultiColumnItemArranger;
				}

				ListViewItemStyleProvider::IListViewItemContent* ListViewBigIconContentProvider::CreateItemContent(const FontProperties& font)
				{
					return new ItemContent(font);
				}
				
/***********************************************************************
ListViewSmallIconContentProvider
***********************************************************************/

				ListViewSmallIconContentProvider::ItemContent::ItemContent(const FontProperties& font)
					:contentComposition(0)
				{
					contentComposition=new GuiBoundsComposition;
					contentComposition->SetMinSizeLimitation(GuiGraphicsComposition::LimitToElementAndChildren);

					GuiTableComposition* table=new GuiTableComposition;
					contentComposition->AddChild(table);
					table->SetRowsAndColumns(3, 2);
					table->SetRowOption(0, GuiCellOption::PercentageOption(0.5));
					table->SetRowOption(1, GuiCellOption::MinSizeOption());
					table->SetRowOption(2, GuiCellOption::PercentageOption(0.5));
					table->SetColumnOption(0, GuiCellOption::MinSizeOption());
					table->SetColumnOption(1, GuiCellOption::MinSizeOption());
					table->SetAlignmentToParent(Margin(0, 0, 0, 0));
					table->SetCellPadding(2);
					{
						GuiCellComposition* cell=new GuiCellComposition;
						table->AddChild(cell);
						cell->SetSite(1, 0, 1, 1);
						cell->SetPreferredMinSize(Size(16, 16));

						image=GuiImageFrameElement::Create();
						image->SetStretch(true);
						cell->SetOwnedElement(image);
					}
					{
						GuiCellComposition* cell=new GuiCellComposition;
						table->AddChild(cell);
						cell->SetMinSizeLimitation(GuiGraphicsComposition::NoLimit);
						cell->SetSite(0, 1, 3, 1);
						cell->SetPreferredMinSize(Size(192, 16));

						text=GuiSolidLabelElement::Create();
						text->SetAlignments(Alignment::Left, Alignment::Center);
						text->SetFont(font);
						text->SetEllipse(true);
						cell->SetOwnedElement(text);
					}
				}

				ListViewSmallIconContentProvider::ItemContent::~ItemContent()
				{
				}

				elements::GuiBoundsComposition* ListViewSmallIconContentProvider::ItemContent::GetContentComposition()
				{
					return contentComposition;
				}

				void ListViewSmallIconContentProvider::ItemContent::Install(ListViewItemStyleProvider::IListViewItemView* view, int itemIndex)
				{
					Ptr<GuiImageData> imageData=view->GetSmallImage(itemIndex);
					if(imageData)
					{
						image->SetImage(imageData->GetImage(), imageData->GetFrameIndex());
					}
					else
					{
						image->SetImage(0);
					}
					text->SetText(view->GetText(itemIndex));
				}

				ListViewSmallIconContentProvider::ListViewSmallIconContentProvider()
				{
				}

				ListViewSmallIconContentProvider::~ListViewSmallIconContentProvider()
				{
				}

				GuiListControl::IItemArranger* ListViewSmallIconContentProvider::CreatePreferredArranger()
				{
					return new FixedSizeMultiColumnItemArranger;
				}

				ListViewItemStyleProvider::IListViewItemContent* ListViewSmallIconContentProvider::CreateItemContent(const FontProperties& font)
				{
					return new ItemContent(font);
				}
				
/***********************************************************************
ListViewListContentProvider
***********************************************************************/

				ListViewListContentProvider::ItemContent::ItemContent(const FontProperties& font)
					:contentComposition(0)
				{
					contentComposition=new GuiBoundsComposition;
					contentComposition->SetMinSizeLimitation(GuiGraphicsComposition::LimitToElementAndChildren);

					GuiTableComposition* table=new GuiTableComposition;
					contentComposition->AddChild(table);
					table->SetRowsAndColumns(3, 2);
					table->SetRowOption(0, GuiCellOption::PercentageOption(0.5));
					table->SetRowOption(1, GuiCellOption::MinSizeOption());
					table->SetRowOption(2, GuiCellOption::PercentageOption(0.5));
					table->SetColumnOption(0, GuiCellOption::MinSizeOption());
					table->SetColumnOption(1, GuiCellOption::MinSizeOption());
					table->SetAlignmentToParent(Margin(0, 0, 0, 0));
					table->SetCellPadding(2);
					{
						GuiCellComposition* cell=new GuiCellComposition;
						table->AddChild(cell);
						cell->SetSite(1, 0, 1, 1);
						cell->SetPreferredMinSize(Size(16, 16));

						image=GuiImageFrameElement::Create();
						image->SetStretch(true);
						cell->SetOwnedElement(image);
					}
					{
						GuiCellComposition* cell=new GuiCellComposition;
						table->AddChild(cell);
						cell->SetSite(0, 1, 3, 1);
						cell->SetPreferredMinSize(Size(192, 16));

						text=GuiSolidLabelElement::Create();
						text->SetAlignments(Alignment::Left, Alignment::Center);
						text->SetFont(font);
						cell->SetOwnedElement(text);
					}
				}

				ListViewListContentProvider::ItemContent::~ItemContent()
				{
				}

				elements::GuiBoundsComposition* ListViewListContentProvider::ItemContent::GetContentComposition()
				{
					return contentComposition;
				}

				void ListViewListContentProvider::ItemContent::Install(ListViewItemStyleProvider::IListViewItemView* view, int itemIndex)
				{
					Ptr<GuiImageData> imageData=view->GetSmallImage(itemIndex);
					if(imageData)
					{
						image->SetImage(imageData->GetImage(), imageData->GetFrameIndex());
					}
					else
					{
						image->SetImage(0);
					}
					text->SetText(view->GetText(itemIndex));
				}

				ListViewListContentProvider::ListViewListContentProvider()
				{
				}

				ListViewListContentProvider::~ListViewListContentProvider()
				{
				}

				GuiListControl::IItemArranger* ListViewListContentProvider::CreatePreferredArranger()
				{
					return new FixedHeightItemArranger;
				}

				ListViewItemStyleProvider::IListViewItemContent* ListViewListContentProvider::CreateItemContent(const FontProperties& font)
				{
					return new ItemContent(font);
				}
				
/***********************************************************************
ListViewDetailContentProvider
***********************************************************************/

				ListViewDetailContentProvider::ItemContent::ItemContent(const FontProperties& font)
					:contentComposition(0)
				{
					contentComposition=new GuiBoundsComposition;
				}

				ListViewDetailContentProvider::ItemContent::~ItemContent()
				{
				}

				elements::GuiBoundsComposition* ListViewDetailContentProvider::ItemContent::GetContentComposition()
				{
					return contentComposition;
				}

				void ListViewDetailContentProvider::ItemContent::Install(ListViewItemStyleProvider::IListViewItemView* view, int itemIndex)
				{
				}

				ListViewDetailContentProvider::ListViewDetailContentProvider()
				{
				}

				ListViewDetailContentProvider::~ListViewDetailContentProvider()
				{
				}

				GuiListControl::IItemArranger* ListViewDetailContentProvider::CreatePreferredArranger()
				{
					return new FixedHeightItemArranger;
				}

				ListViewItemStyleProvider::IListViewItemContent* ListViewDetailContentProvider::CreateItemContent(const FontProperties& font)
				{
					return new ItemContent(font);
				}
				
/***********************************************************************
ListViewTileContentProvider
***********************************************************************/

				ListViewTileContentProvider::ItemContent::ItemContent(const FontProperties& font)
					:contentComposition(0)
				{
					contentComposition=new GuiBoundsComposition;
				}

				ListViewTileContentProvider::ItemContent::~ItemContent()
				{
				}

				elements::GuiBoundsComposition* ListViewTileContentProvider::ItemContent::GetContentComposition()
				{
					return contentComposition;
				}

				void ListViewTileContentProvider::ItemContent::Install(ListViewItemStyleProvider::IListViewItemView* view, int itemIndex)
				{
				}

				ListViewTileContentProvider::ListViewTileContentProvider()
				{
				}

				ListViewTileContentProvider::~ListViewTileContentProvider()
				{
				}

				GuiListControl::IItemArranger* ListViewTileContentProvider::CreatePreferredArranger()
				{
					return new FixedSizeMultiColumnItemArranger;
				}

				ListViewItemStyleProvider::IListViewItemContent* ListViewTileContentProvider::CreateItemContent(const FontProperties& font)
				{
					return new ItemContent(font);
				}
				
/***********************************************************************
ListViewInformationContentProvider
***********************************************************************/

				ListViewInformationContentProvider::ItemContent::ItemContent(const FontProperties& font)
					:contentComposition(0)
				{
					contentComposition=new GuiBoundsComposition;
				}

				ListViewInformationContentProvider::ItemContent::~ItemContent()
				{
				}

				elements::GuiBoundsComposition* ListViewInformationContentProvider::ItemContent::GetContentComposition()
				{
					return contentComposition;
				}

				void ListViewInformationContentProvider::ItemContent::Install(ListViewItemStyleProvider::IListViewItemView* view, int itemIndex)
				{
				}

				ListViewInformationContentProvider::ListViewInformationContentProvider()
				{
				}

				ListViewInformationContentProvider::~ListViewInformationContentProvider()
				{
				}

				GuiListControl::IItemArranger* ListViewInformationContentProvider::CreatePreferredArranger()
				{
					return new FixedHeightItemArranger;
				}

				ListViewItemStyleProvider::IListViewItemContent* ListViewInformationContentProvider::CreateItemContent(const FontProperties& font)
				{
					return new ItemContent(font);
				}
			}
		}
	}
}