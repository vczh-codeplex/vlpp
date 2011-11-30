#include "GuiListViewControls.h"

namespace vl
{
	namespace presentation
	{
		namespace controls
		{
			namespace list
			{

/***********************************************************************
ListViewItemStyleProvider::TextItemStyleController
***********************************************************************/

				ListViewItemStyleProvider::ListViewItemStyleController::ListViewItemStyleController(ListViewItemStyleProvider* provider)
					:ItemStyleControllerBase(provider, 0)
					,backgroundButton(0)
					,listViewItemStyleProvider(provider)
				{
					backgroundButton=new GuiSelectableButton(listViewItemStyleProvider->listControl->styleProvider->CreateItemBackground());
					backgroundButton->SetAutoSelection(false);
					Initialize(backgroundButton->GetBoundsComposition(), backgroundButton);
				}

				ListViewItemStyleProvider::ListViewItemStyleController::~ListViewItemStyleController()
				{
				}

				bool ListViewItemStyleProvider::ListViewItemStyleController::GetSelected()
				{
					return backgroundButton->GetSelected();
				}

				void ListViewItemStyleProvider::ListViewItemStyleController::SetSelected(bool value)
				{
					backgroundButton->SetSelected(value);
				}

/***********************************************************************
ListViewItemStyleProvider
***********************************************************************/

				const wchar_t* ListViewItemStyleProvider::IListViewItemView::Identifier = L"vl::presentation::controls::list::ListViewItemStyleProvider::IListViewItemView";

				ListViewItemStyleProvider::ListViewItemStyleProvider()
					:listViewItemView(0)
					,listControl(0)
				{
				}

				ListViewItemStyleProvider::~ListViewItemStyleProvider()
				{
				}

				void ListViewItemStyleProvider::AttachListControl(GuiListControl* value)
				{
					listControl=dynamic_cast<GuiListView*>(value);
					listViewItemView=dynamic_cast<IListViewItemView*>(value->GetItemProvider()->RequestView(IListViewItemView::Identifier));
				}

				void ListViewItemStyleProvider::DetachListControl()
				{
					listControl->GetItemProvider()->ReleaseView(listViewItemView);
					listViewItemView=0;
					listControl=0;
				}

				int ListViewItemStyleProvider::GetItemStyleId(int itemIndex)
				{
					return 0;
				}

				GuiListControl::IItemStyleController* ListViewItemStyleProvider::CreateItemStyle(int styleId)
				{
					return new ListViewItemStyleController(this);
				}

				void ListViewItemStyleProvider::DestroyItemStyle(GuiListControl::IItemStyleController* style)
				{
					delete dynamic_cast<ListViewItemStyleController*>(style);
				}

				void ListViewItemStyleProvider::Install(GuiListControl::IItemStyleController* style, int itemIndex)
				{
					ListViewItemStyleController* textStyle=dynamic_cast<ListViewItemStyleController*>(style);
				}

				void ListViewItemStyleProvider::SetStyleSelected(GuiListControl::IItemStyleController* style, bool value)
				{
					ListViewItemStyleController* textStyle=dynamic_cast<ListViewItemStyleController*>(style);
					textStyle->SetSelected(value);
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
				:GuiSelectableListControl(_styleProvider, new list::ListViewItemProvider)
				,items(0)
				,styleProvider(0)
			{
				items=dynamic_cast<list::ListViewItemProvider*>(itemProvider.Obj());
				styleProvider=dynamic_cast<IStyleProvider*>(styleController->GetStyleProvider());
				SetStyleProvider(new list::ListViewItemStyleProvider);
				SetArranger(new list::FixedHeightItemArranger);
			}

			GuiListView::~GuiListView()
			{
			}

			Ptr<GuiListControl::IItemStyleProvider> GuiListView::SetStyleProvider(Ptr<GuiListControl::IItemStyleProvider> value)
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

			list::ListViewItemProvider& GuiListView::GetItems()
			{
				return *items;
			}
		}
	}
}