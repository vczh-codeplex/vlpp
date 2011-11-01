#include "GuiListControls.h"
#include "..\..\..\..\Library\Collections\OperationCopyFrom.h"

namespace vl
{
	namespace presentation
	{
		namespace controls
		{
			using namespace collections;
			using namespace elements;

/***********************************************************************
GuiListControl::ItemCallback
***********************************************************************/

			GuiListControl::ItemCallback::ItemCallback(GuiListControl* _listControl)
				:listControl(_listControl)
			{
			}

			GuiListControl::ItemCallback::~ItemCallback()
			{
				ClearCache();
			}

			void GuiListControl::ItemCallback::ClearCache()
			{
				for(int i=0;i<cachedStyles.Count();i++)
				{
					listControl->itemStyleProvider->DestroyItemStyle(cachedStyles[i]);
				}
				for(int i=0;i<installedStyles.Count();i++)
				{
					listControl->itemStyleProvider->DestroyItemStyle(installedStyles[i]);
				}
				cachedStyles.Clear();
				installedStyles.Clear();
			}

			void GuiListControl::ItemCallback::OnAttached(IItemProvider* provider)
			{
			}

			void GuiListControl::ItemCallback::OnItemModified(int start, int count, int newCount)
			{
				listControl->OnItemModified(start, count, newCount);
			}

			GuiListControl::IItemStyleController* GuiListControl::ItemCallback::RequestItem(int itemIndex)
			{
				int id=listControl->itemStyleProvider->GetItemStyleId(itemIndex);
				IItemStyleController* style=0;
				for(int i=0;i<cachedStyles.Count();i++)
				{
					IItemStyleController* cachedStyle=cachedStyles[i];
					if(cachedStyle->GetItemStyleId()==id)
					{
						style=cachedStyle;
						cachedStyles.RemoveAt(i);
						break;
					}
				}
				if(!style)
				{
					style=listControl->itemStyleProvider->CreateItemStyle(id);
				}
				listControl->itemStyleProvider->Install(style, itemIndex);
				style->OnInstalled();
				installedStyles.Add(style);
				listControl->OnStyleInstalled(style);
				return style;
			}

			void GuiListControl::ItemCallback::ReleaseItem(IItemStyleController* style)
			{
				int index=installedStyles.IndexOf(style);
				if(index!=-1)
				{
					listControl->OnStyleUninstalled(style);
					installedStyles.RemoveAt(index);
					style->OnUninstalled();
					if(style->IsCacheable())
					{
						cachedStyles.Add(style);
					}
					else
					{
						listControl->itemStyleProvider->DestroyItemStyle(style);
					}
				}
			}

			void GuiListControl::ItemCallback::SetViewLocation(Point value)
			{
				listControl->GetHorizontalScroll()->SetPosition(value.x);
				listControl->GetVerticalScroll()->SetPosition(value.y);
			}

			elements::GuiGraphicsComposition* GuiListControl::ItemCallback::GetContainerComposition()
			{
				return listControl->GetContainerComposition();
			}

			void GuiListControl::ItemCallback::OnTotalSizeChanged()
			{
				listControl->CalculateView();
			}

/***********************************************************************
GuiListControl
***********************************************************************/

			void GuiListControl::OnItemModified(int start, int count, int newCount)
			{
			}

			void GuiListControl::OnStyleInstalled(IItemStyleController* style)
			{
			}

			void GuiListControl::OnStyleUninstalled(IItemStyleController* style)
			{
			}

			void GuiListControl::OnRenderTargetChanged(elements::IGuiGraphicsRenderTarget* renderTarget)
			{
				if(itemArranger)
				{
					itemArranger->OnItemModified(0, itemProvider->Count(), itemProvider->Count());
				}
				GuiScrollView::OnRenderTargetChanged(renderTarget);
			}

			Size GuiListControl::QueryFullSize()
			{
				return itemArranger?itemArranger->GetTotalSize():Size(0, 0);
			}

			void GuiListControl::UpdateView(Rect viewBounds)
			{
				if(itemArranger)
				{
					itemArranger->OnViewChanged(viewBounds);
				}
			}

			GuiListControl::GuiListControl(IStyleProvider* _styleProvider, IItemProvider* _itemProvider)
				:GuiScrollView(_styleProvider)
				,itemProvider(_itemProvider)
			{
				callback=new ItemCallback(this);
				itemProvider->AttachCallback(callback.Obj());
			}

			GuiListControl::~GuiListControl()
			{
				if(itemArranger)
				{
					itemProvider->DetachCallback(itemArranger.Obj());
					itemArranger->SetCallback(0);
				}
				callback->ClearCache();
			}

			GuiListControl::IItemProvider* GuiListControl::GetItemProvider()
			{
				return itemProvider.Obj();
			}

			GuiListControl::IItemStyleProvider* GuiListControl::GetStyleProvider()
			{
				return itemStyleProvider.Obj();
			}

			Ptr<GuiListControl::IItemStyleProvider> GuiListControl::SetStyleProvider(Ptr<IItemStyleProvider> value)
			{
				Ptr<IItemStyleProvider> old=itemStyleProvider;
				if(itemStyleProvider)
				{
					itemStyleProvider->DetachListControl();
					if(itemArranger)
					{
						itemArranger->SetCallback(0);
					}
					callback->ClearCache();
				}
				itemStyleProvider=value;
				if(itemStyleProvider)
				{
					if(itemArranger)
					{
						itemProvider->AttachCallback(itemArranger.Obj());
					}
					itemStyleProvider->AttachListControl(this);
				}

				GetVerticalScroll()->SetPosition(0);
				GetHorizontalScroll()->SetPosition(0);
				if(itemArranger)
				{
					itemArranger->OnItemModified(0, itemProvider->Count(), itemProvider->Count());
				}
				return old;
			}

			GuiListControl::IItemArranger* GuiListControl::GetArranger()
			{
				return itemArranger.Obj();
			}

			Ptr<GuiListControl::IItemArranger> GuiListControl::SetArranger(Ptr<IItemArranger> value)
			{
				Ptr<IItemArranger> old=itemArranger;
				if(itemArranger)
				{
					itemArranger->DetachListControl();
					itemProvider->DetachCallback(itemArranger.Obj());
					itemArranger->SetCallback(0);
					callback->ClearCache();
				}
				itemArranger=value;
				if(itemArranger)
				{
					if(itemStyleProvider)
					{
						itemArranger->SetCallback(callback.Obj());
					}
					itemProvider->AttachCallback(itemArranger.Obj());
					itemArranger->AttachListControl(this);
				}

				GetVerticalScroll()->SetPosition(0);
				GetHorizontalScroll()->SetPosition(0);
				if(itemArranger)
				{
					itemArranger->OnItemModified(0, itemProvider->Count(), itemProvider->Count());
				}
				return old;
			}

/***********************************************************************
GuiSelectableListControl::StyleEvents
***********************************************************************/

			void GuiSelectableListControl::StyleEvents::OnBoundsLeftButtonDown(elements::GuiGraphicsComposition* sender, elements::GuiMouseEventArgs& arguments)
			{
			}

			GuiSelectableListControl::StyleEvents::StyleEvents(GuiSelectableListControl* _listControl, IItemStyleController* _style)
				:listControl(_listControl)
				,style(_style)
			{
			}

			GuiSelectableListControl::StyleEvents::~StyleEvents()
			{
			}

			void GuiSelectableListControl::StyleEvents::AttachEvents()
			{
				if(!leftButtonDownHandler)
				{
					leftButtonDownHandler=style->GetBoundsComposition()->GetEventReceiver()->leftButtonDown.AttachMethod(this, &StyleEvents::OnBoundsLeftButtonDown);
				}
			}

			void GuiSelectableListControl::StyleEvents::DetachEvents()
			{
				if(leftButtonDownHandler)
				{
					style->GetBoundsComposition()->GetEventReceiver()->leftButtonDown.Detach(leftButtonDownHandler);
					leftButtonDownHandler=0;
				}
			}

/***********************************************************************
GuiSelectableListControl
***********************************************************************/

			void GuiSelectableListControl::OnItemModified(int start, int count, int newCount)
			{
				if(count!=newCount)
				{
					ClearSelection();
				}
			}

			void GuiSelectableListControl::OnStyleInstalled(IItemStyleController* style)
			{
				StyleEvents* styleEvents=new StyleEvents(this, style);
				styleEvents->AttachEvents();
				visibleStyles.Add(style, styleEvents);
			}

			void GuiSelectableListControl::OnStyleUninstalled(IItemStyleController* style)
			{
				int index=visibleStyles.Keys().IndexOf(style);
				if(index!=-1)
				{
					visibleStyles.Values()[index]->DetachEvents();
					visibleStyles.Remove(style);
				}
			}

			void GuiSelectableListControl::OnItemSelectionChanged(int itemIndex, bool value)
			{
				GuiListControl::IItemStyleController* style=itemArranger->GetVisibleStyle(itemIndex);
				if(style)
				{
					selectableStyleProvider->SetStyleSelected(style, value);
				}
			}

			void GuiSelectableListControl::OnItemSelectionCleared()
			{
				for(int i=0;i<visibleStyles.Count();i++)
				{
					selectableStyleProvider->SetStyleSelected(visibleStyles.Keys()[i], false);
				}
			}

			GuiSelectableListControl::GuiSelectableListControl(IStyleProvider* _styleProvider, IItemProvider* _itemProvider)
				:GuiListControl(_styleProvider, _itemProvider)
			{
			}

			GuiSelectableListControl::~GuiSelectableListControl()
			{
			}

			Ptr<GuiListControl::IItemStyleProvider> GuiSelectableListControl::SetStyleProvider(Ptr<GuiListControl::IItemStyleProvider> value)
			{
				selectableStyleProvider=value?value.Cast<IItemStyleProvider>():0;
				return GuiListControl::SetStyleProvider(value);
			}

			const collections::IReadonlyList<int>& GuiSelectableListControl::GetSelectedItems()
			{
				return selectedItems.Wrap();
			}

			bool GuiSelectableListControl::GetSelected(int itemIndex)
			{
				return selectedItems.Contains(itemIndex);
			}

			void GuiSelectableListControl::SetSelected(int itemIndex, bool value)
			{
				if(value)
				{
					if(!selectedItems.Contains(itemIndex))
					{
						selectedItems.Add(itemIndex);
					}
				}
				else
				{
					selectedItems.Remove(itemIndex);
				}
			}

			void GuiSelectableListControl::ClearSelection()
			{
				selectedItems.Clear();
			}

			namespace list
			{

/***********************************************************************
FixedHeightItemArranger
***********************************************************************/

				void FixedHeightItemArranger::ClearStyles()
				{
					startIndex=0;
					rowHeight=1;
					if(callback)
					{
						for(int i=0;i<visibleStyles.Count();i++)
						{
							GuiListControl::IItemStyleController* style=visibleStyles[i];
							callback->GetContainerComposition()->RemoveChild(style->GetBoundsComposition());
							callback->ReleaseItem(style);
						}
					}
					visibleStyles.Clear();
					viewBounds=Rect(0, 0, 0, 0);
				}

				void FixedHeightItemArranger::RearrangeItemBounds()
				{
					int y0=-viewBounds.Top();
					for(int i=0;i<visibleStyles.Count();i++)
					{
						GuiListControl::IItemStyleController* style=visibleStyles[i];
						style->GetBoundsComposition()->SetAlignmentToParent(Margin(0, -1, 0, -1));
						style->GetBoundsComposition()->SetBounds(Rect(Point(0, y0+(startIndex+i)*rowHeight), Size(0, rowHeight)));
					}
				}

				FixedHeightItemArranger::FixedHeightItemArranger()
					:callback(0)
					,startIndex(0)
					,rowHeight(1)
					,suppressOnViewChanged(false)
				{
				}

				FixedHeightItemArranger::~FixedHeightItemArranger()
				{
				}

				void FixedHeightItemArranger::OnAttached(GuiListControl::IItemProvider* provider)
				{
					itemProvider=provider;
					if(provider)
					{
						OnItemModified(0, 0, provider->Count());
					}
				}

				void FixedHeightItemArranger::AttachListControl(GuiListControl* value)
				{
				}

				void FixedHeightItemArranger::DetachListControl()
				{
				}

				void FixedHeightItemArranger::OnItemModified(int start, int count, int newCount)
				{
					if(callback)
					{
						if(count==newCount)
						{
							int offset=viewBounds.Top()-rowHeight*startIndex;
							int newRowHeight=rowHeight;

							for(int i=0;i<visibleStyles.Count();i++)
							{
								int index=startIndex+i;
								if(start<=index && index<start+count)
								{
									GuiListControl::IItemStyleController* style=visibleStyles[i];
									callback->GetContainerComposition()->RemoveChild(style->GetBoundsComposition());
									callback->ReleaseItem(style);

									style=callback->RequestItem(index);
									callback->GetContainerComposition()->AddChild(style->GetBoundsComposition());
								}
							}

							if(rowHeight!=newRowHeight)
							{
								rowHeight=newRowHeight;
								suppressOnViewChanged=true;
								callback->OnTotalSizeChanged();
								callback->SetViewLocation(Point(0, rowHeight*startIndex+offset));
								suppressOnViewChanged=false;
							}
							RearrangeItemBounds();
						}
						else
						{
							ClearStyles();
							suppressOnViewChanged=true;
							callback->OnTotalSizeChanged();
							suppressOnViewChanged=false;
							callback->SetViewLocation(Point(0, 0));
						}
					}
				}

				GuiListControl::IItemArrangerCallback* FixedHeightItemArranger::GetCallback()
				{
					return callback;
				}

				void FixedHeightItemArranger::SetCallback(GuiListControl::IItemArrangerCallback* value)
				{
					callback=value;
					if(!callback)
					{
						ClearStyles();
					}
				}

				Size FixedHeightItemArranger::GetTotalSize()
				{
					if(callback)
					{
						return Size(0, rowHeight*itemProvider->Count());
					}
					else
					{
						return Size(0, 0);
					}
				}

				GuiListControl::IItemStyleController* FixedHeightItemArranger::GetVisibleStyle(int itemIndex)
				{
					if(startIndex<=itemIndex && itemIndex<startIndex+visibleStyles.Count())
					{
						return visibleStyles[itemIndex-startIndex];
					}
					else
					{
						return 0;
					}
				}

				int FixedHeightItemArranger::GetVisibleIndex(GuiListControl::IItemStyleController* style)
				{
					int index=visibleStyles.IndexOf(style);
					return index==-1?-1:index+startIndex;
				}

				void FixedHeightItemArranger::OnViewChanged(Rect bounds)
				{
					if(callback)
					{
						Rect oldViewBounds=viewBounds;
						viewBounds=bounds;
						if(!suppressOnViewChanged)
						{
							int oldVisibleCount=visibleStyles.Count();
							int newRowHeight=rowHeight;
							int newStartIndex=bounds.Top()/rowHeight;

							int endIndex=startIndex+visibleStyles.Count()-1;
							int newEndIndex=(bounds.Bottom()-1)/newRowHeight;
							int itemCount=itemProvider->Count();

							for(int i=newStartIndex;i<=newEndIndex && i<itemCount;i++)
							{
								if(startIndex<=i && i<=endIndex)
								{
									GuiListControl::IItemStyleController* style=visibleStyles[i-startIndex];
									visibleStyles.Add(style);
								}
								else
								{
									GuiListControl::IItemStyleController* style=callback->RequestItem(i);
									callback->GetContainerComposition()->AddChild(style->GetBoundsComposition());
									visibleStyles.Add(style);
									int styleHeight=style->GetBoundsComposition()->GetMinNecessaryBounds().Height();
									if(newRowHeight<styleHeight)
									{
										newRowHeight=styleHeight;
										newEndIndex=(bounds.Bottom()-1)/newRowHeight;
									}
								}
							}

							for(int i=0;i<oldVisibleCount;i++)
							{
								int index=startIndex+i;
								if(index<newStartIndex || newEndIndex<index)
								{
									GuiListControl::IItemStyleController* style=visibleStyles[i];
									callback->GetContainerComposition()->RemoveChild(style->GetBoundsComposition());
									callback->ReleaseItem(style);
								}
							}
							visibleStyles.RemoveRange(0, oldVisibleCount);

							if(rowHeight!=newRowHeight)
							{
								int offset=oldViewBounds.Top()-rowHeight*startIndex;
								rowHeight=newRowHeight;
								suppressOnViewChanged=true;
								callback->OnTotalSizeChanged();
								callback->SetViewLocation(Point(0, rowHeight*newStartIndex+offset));
								suppressOnViewChanged=false;
							}
							startIndex=newStartIndex;
							RearrangeItemBounds();
						}
					}
				}

/***********************************************************************
ItemStyleControllerBase
***********************************************************************/

				void ItemStyleControllerBase::Initialize(elements::GuiBoundsComposition* _boundsComposition, GuiControl* _associatedControl)
				{
					boundsComposition=_boundsComposition;
					associatedControl=_associatedControl;
				}

				void ItemStyleControllerBase::Finalize()
				{
					if(boundsComposition && !isInstalled)
					{
						if(associatedControl)
						{
							delete associatedControl;
						}
						else
						{
							delete boundsComposition;
						}
					}
					boundsComposition=0;
					associatedControl=0;
				}

				ItemStyleControllerBase::ItemStyleControllerBase(GuiListControl::IItemStyleProvider* _provider, int _styleId)
					:provider(_provider)
					,styleId(_styleId)
					,boundsComposition(0)
					,associatedControl(0)
					,isInstalled(false)
				{
				}

				ItemStyleControllerBase::~ItemStyleControllerBase()
				{
					Finalize();
				}
					
				GuiListControl::IItemStyleProvider* ItemStyleControllerBase::GetStyleProvider()
				{
					return provider;
				}

				int ItemStyleControllerBase::GetItemStyleId()
				{
					return styleId;
				}

				elements::GuiBoundsComposition* ItemStyleControllerBase::GetBoundsComposition()
				{
					return boundsComposition;
				}

				bool ItemStyleControllerBase::IsCacheable()
				{
					return true;
				}

				bool ItemStyleControllerBase::IsInstalled()
				{
					return isInstalled;
				}

				void ItemStyleControllerBase::OnInstalled()
				{
					isInstalled=true;
				}

				void ItemStyleControllerBase::OnUninstalled()
				{
					isInstalled=false;
				}

/***********************************************************************
ItemProviderBase
***********************************************************************/

				void ItemProviderBase::InvokeOnItemModified(int start, int count, int newCount)
				{
					for(int i=0;i<callbacks.Count();i++)
					{
						callbacks[i]->OnItemModified(start, count, newCount);
					}
				}

				ItemProviderBase::ItemProviderBase()
				{
				}

				ItemProviderBase::~ItemProviderBase()
				{
					for(int i=0;i<callbacks.Count();i++)
					{
						callbacks[i]->OnAttached(0);
					}
				}

				bool ItemProviderBase::AttachCallback(GuiListControl::IItemProviderCallback* value)
				{
					if(callbacks.Contains(value))
					{
						return false;
					}
					else
					{
						callbacks.Add(value);
						value->OnAttached(this);
						return true;
					}
				}

				bool ItemProviderBase::DetachCallback(GuiListControl::IItemProviderCallback* value)
				{
					int index=callbacks.IndexOf(value);
					if(index==-1)
					{
						return false;
					}
					else
					{
						value->OnAttached(0);
						callbacks.Remove(value);
						return true;
					}
				}

/***********************************************************************
TextItem
***********************************************************************/

				TextItem::TextItem()
					:checked(false)
				{
				}

				TextItem::TextItem(const TextItem& item)
					:text(item.text)
					,checked(item.checked)
				{
				}

				TextItem::TextItem(const WString& _text, bool _checked)
					:text(_text)
					,checked(_checked)
				{
				}

				TextItem::~TextItem()
				{
				}

				bool TextItem::operator==(const TextItem& value)const
				{
					return text==value.text;
				}

				bool TextItem::operator!=(const TextItem& value)const
				{
					return text!=value.text;
				}

				const WString& TextItem::GetText()const
				{
					return text;
				}

				bool TextItem::GetChecked()const
				{
					return checked;
				}

/***********************************************************************
TextItemProvider
***********************************************************************/

				TextItemProvider::TextItemProvider()
				{
				}

				TextItemProvider::~TextItemProvider()
				{
				}
					
				void TextItemProvider::SetText(int itemIndex, const WString& value)
				{
					list[itemIndex].text=value;
					InvokeOnItemModified(itemIndex, 1, 1);
				}

				void TextItemProvider::SetChecked(int itemIndex, bool value)
				{
					list[itemIndex].checked=value;
					InvokeOnItemModified(itemIndex, 1, 1);
				}

/***********************************************************************
TextItemStyleProvider::TextItemStyleController
***********************************************************************/

				void TextItemStyleProvider::TextItemStyleController::OnBulletSelectedChanged(elements::GuiGraphicsComposition* sender, elements::GuiEventArgs& arguments)
				{
					textItemStyleProvider->OnStyleCheckedChanged(this);
				}

				TextItemStyleProvider::TextItemStyleController::TextItemStyleController(TextItemStyleProvider* provider)
					:ItemStyleControllerBase(provider, 0)
					,backgroundButton(0)
					,bulletButton(0)
					,textElement(0)
					,textItemStyleProvider(provider)
				{
					backgroundButton=new GuiSelectableButton(textItemStyleProvider->textItemStyleProvider->CreateBackgroundStyleController());
					backgroundButton->SetAutoSelection(false);
					
					textElement=GuiSolidLabelElement::Create();
					textElement->SetAlignments(Alignment::Left, Alignment::Center);

					GuiBoundsComposition* textComposition=new GuiBoundsComposition;
					textComposition->SetOwnedElement(textElement);
					textComposition->SetAlignmentToParent(Margin(0, 0, 0, 0));

					GuiSelectableButton::IStyleController* bulletStyleController=textItemStyleProvider->textItemStyleProvider->CreateBulletStyleController();
					if(bulletStyleController)
					{
						bulletButton=new GuiSelectableButton(bulletStyleController);
						bulletButton->GetBoundsComposition()->SetAlignmentToParent(Margin(0, 0, 0, 0));

						GuiTableComposition* table=new GuiTableComposition;
						table->SetRowsAndColumns(1, 2);
						table->SetRowOption(0, GuiCellOption::PercentageOption(1.0));
						table->SetColumnOption(0, GuiCellOption::MinSizeOption());
						table->SetColumnOption(1, GuiCellOption::PercentageOption(1.0));
						{
							GuiCellComposition* cell=new GuiCellComposition;
							table->AddChild(cell);
							cell->SetSite(0, 0, 1, 1);
							cell->AddChild(bulletButton->GetBoundsComposition());
						}
						{
							GuiCellComposition* cell=new GuiCellComposition;
							table->AddChild(cell);
							cell->SetSite(0, 1, 1, 1);
							cell->AddChild(textComposition);
						}
					}
					else
					{
						backgroundButton->GetContainerComposition()->AddChild(textComposition);
					}
					Initialize(backgroundButton->GetBoundsComposition(), backgroundButton);
				}

				TextItemStyleProvider::TextItemStyleController::~TextItemStyleController()
				{
				}

				bool TextItemStyleProvider::TextItemStyleController::GetSelected()
				{
					return backgroundButton->GetSelected();
				}

				void TextItemStyleProvider::TextItemStyleController::SetSelected(bool value)
				{
					backgroundButton->SetSelected(value);
				}

				bool TextItemStyleProvider::TextItemStyleController::GetChecked()
				{
					return bulletButton?bulletButton->GetSelected():false;
				}

				void TextItemStyleProvider::TextItemStyleController::SetChecked(bool value)
				{
					if(bulletButton) bulletButton->SetSelected(value);
				}
				
				const WString& TextItemStyleProvider::TextItemStyleController::GetText()
				{
					return textElement->GetText();
				}

				void TextItemStyleProvider::TextItemStyleController::SetText(const WString& value)
				{
					textElement->SetText(value);
				}

/***********************************************************************
TextItemStyleProvider
***********************************************************************/

				void TextItemStyleProvider::OnStyleCheckedChanged(TextItemStyleController* style)
				{
				}

				TextItemStyleProvider::TextItemStyleProvider(ITextItemStyleProvider* _textItemStyleProvider)
					:textItemStyleProvider(_textItemStyleProvider)
					,textItemProvider(0)
					,listControl(0)
				{
				}

				TextItemStyleProvider::~TextItemStyleProvider()
				{
				}

				void TextItemStyleProvider::AttachListControl(GuiListControl* value)
				{
					listControl=value;
					textItemProvider=dynamic_cast<TextItemProvider*>(value->GetItemProvider());
				}

				void TextItemStyleProvider::DetachListControl()
				{
					listControl=0;
				}

				int TextItemStyleProvider::GetItemStyleId(int itemIndex)
				{
					return 0;
				}

				GuiListControl::IItemStyleController* TextItemStyleProvider::CreateItemStyle(int styleId)
				{
					return new TextItemStyleController(this);
				}

				void TextItemStyleProvider::DestroyItemStyle(GuiListControl::IItemStyleController* style)
				{
					delete dynamic_cast<TextItemStyleController*>(style);
				}

				void TextItemStyleProvider::Install(GuiListControl::IItemStyleController* style, int itemIndex)
				{
					TextItemStyleController* textStyle=dynamic_cast<TextItemStyleController*>(style);
					const TextItem& item=textItemProvider->Get(itemIndex);
					textStyle->SetText(item.GetText());
					textStyle->SetChecked(item.GetChecked());
				}

				void TextItemStyleProvider::SetStyleSelected(GuiListControl::IItemStyleController* style, bool value)
				{
					TextItemStyleController* textStyle=dynamic_cast<TextItemStyleController*>(style);
					textStyle->SetSelected(value);
				}
			}

/***********************************************************************
GuiTextList
***********************************************************************/

			GuiTextList::GuiTextList(IStyleProvider* _styleProvider, list::TextItemStyleProvider::ITextItemStyleProvider* _itemStyleProvider)
				:GuiSelectableListControl(_styleProvider, new list::TextItemProvider)
				,items(0)
			{
				items=dynamic_cast<list::TextItemProvider*>(itemProvider.Obj());
				SetStyleProvider(new list::TextItemStyleProvider(_itemStyleProvider));
				SetArranger(new list::FixedHeightItemArranger);
			}

			GuiTextList::~GuiTextList()
			{
			}

			list::TextItemProvider& GuiTextList::GetItems()
			{
				return *items;
			}
		}
	}
}