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
					content=listViewItemStyleProvider->listViewItemContentProvider->CreateItemContent();
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

				ListViewBigIconContentProvider::ItemContent::ItemContent()
					:contentComposition(0)
				{
					contentComposition=new GuiBoundsComposition;
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
				}

				ListViewBigIconContentProvider::ListViewBigIconContentProvider()
				{
				}

				ListViewBigIconContentProvider::~ListViewBigIconContentProvider()
				{
				}

				GuiListControl::IItemArranger* ListViewBigIconContentProvider::CreatePreferredArranger()
				{
					return new FixedHeightItemArranger;
				}

				ListViewItemStyleProvider::IListViewItemContent* ListViewBigIconContentProvider::CreateItemContent()
				{
					return new ItemContent;
				}
			}
		}
	}
}