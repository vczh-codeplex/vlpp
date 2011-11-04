#include "GuiListControls.h"

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
				listControl->OnStyleInstalled(itemIndex, style);
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

			void GuiListControl::OnStyleInstalled(int itemIndex, IItemStyleController* style)
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

			void GuiListControl::OnBoundsMouseButtonDown(elements::GuiGraphicsComposition* sender, elements::GuiMouseEventArgs& arguments)
			{
				if(GetVisuallyEnabled())
				{
					boundsComposition->GetRelatedGraphicsHost()->SetFocus(boundsComposition);
				}
			}

			void GuiListControl::SetStyleProviderAndArranger(Ptr<IItemStyleProvider> styleProvider, Ptr<IItemArranger> arranger)
			{
				if(itemStyleProvider)
				{
					itemStyleProvider->DetachListControl();
				}
				if(itemArranger)
				{
					itemProvider->DetachCallback(itemArranger.Obj());
					itemArranger->SetCallback(0);
				}
				callback->ClearCache();

				itemStyleProvider=styleProvider;
				itemArranger=arranger;
				GetVerticalScroll()->SetPosition(0);
				GetHorizontalScroll()->SetPosition(0);

				if(itemStyleProvider)
				{
					itemStyleProvider->AttachListControl(this);
				}
				if(itemArranger)
				{
					itemArranger->SetCallback(callback.Obj());
					itemProvider->AttachCallback(itemArranger.Obj());
				}
				CalculateView();
			}

			GuiListControl::GuiListControl(IStyleProvider* _styleProvider, IItemProvider* _itemProvider, bool acceptFocus)
				:GuiScrollView(_styleProvider)
				,itemProvider(_itemProvider)
			{
				callback=new ItemCallback(this);
				itemProvider->AttachCallback(callback.Obj());

				if(acceptFocus)
				{
					boundsComposition->GetEventReceiver()->leftButtonDown.AttachMethod(this, &GuiListControl::OnBoundsMouseButtonDown);
					boundsComposition->GetEventReceiver()->middleButtonDown.AttachMethod(this, &GuiListControl::OnBoundsMouseButtonDown);
					boundsComposition->GetEventReceiver()->rightButtonDown.AttachMethod(this, &GuiListControl::OnBoundsMouseButtonDown);
					SetFocusableComposition(boundsComposition);
				}
			}

			GuiListControl::~GuiListControl()
			{
				if(itemArranger)
				{
					itemProvider->DetachCallback(itemArranger.Obj());
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
				SetStyleProviderAndArranger(value, itemArranger);
				return old;
			}

			GuiListControl::IItemArranger* GuiListControl::GetArranger()
			{
				return itemArranger.Obj();
			}

			Ptr<GuiListControl::IItemArranger> GuiListControl::SetArranger(Ptr<IItemArranger> value)
			{
				Ptr<IItemArranger> old=itemArranger;
				SetStyleProviderAndArranger(itemStyleProvider, value);
				return old;
			}

/***********************************************************************
GuiSelectableListControl::StyleEvents
***********************************************************************/

			void GuiSelectableListControl::StyleEvents::OnBoundsLeftButtonDown(elements::GuiGraphicsComposition* sender, elements::GuiMouseEventArgs& arguments)
			{
				if(listControl->GetVisuallyEnabled())
				{
					if(listControl->multiSelect)
					{
					}
					else
					{
						int index=listControl->GetArranger()->GetVisibleIndex(style);
						if(index!=-1)
						{
							listControl->SetSelected(index, true);
						}
					}
				}
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

			void GuiSelectableListControl::OnStyleInstalled(int itemIndex, IItemStyleController* style)
			{
				StyleEvents* styleEvents=new StyleEvents(this, style);
				styleEvents->AttachEvents();
				visibleStyles.Add(style, styleEvents);
				selectableStyleProvider->SetStyleSelected(style, selectedItems.Contains(itemIndex));
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
				:GuiListControl(_styleProvider, _itemProvider, true)
				,multiSelect(false)
			{
				SelectionChanged.SetAssociatedComposition(boundsComposition);
			}

			GuiSelectableListControl::~GuiSelectableListControl()
			{
			}

			Ptr<GuiListControl::IItemStyleProvider> GuiSelectableListControl::SetStyleProvider(Ptr<GuiListControl::IItemStyleProvider> value)
			{
				selectableStyleProvider=value?value.Cast<IItemStyleProvider>():0;
				return GuiListControl::SetStyleProvider(value);
			}

			bool GuiSelectableListControl::GetMultiSelect()
			{
				return multiSelect;
			}

			void GuiSelectableListControl::SetMultiSelect(bool value)
			{
				if(multiSelect!=value)
				{
					multiSelect=value;
					ClearSelection();
				}
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
						if(!multiSelect)
						{
							selectedItems.Clear();
							OnItemSelectionCleared();
						}
						selectedItems.Add(itemIndex);
						OnItemSelectionChanged(itemIndex, value);
						SelectionChanged.Execute(GetNotifyEventArguments());
					}
				}
				else
				{
					if(selectedItems.Remove(itemIndex))
					{
						OnItemSelectionChanged(itemIndex, value);
						SelectionChanged.Execute(GetNotifyEventArguments());
					}
				}
			}

			void GuiSelectableListControl::ClearSelection()
			{
				if(selectedItems.Count()>0)
				{
					selectedItems.Clear();
					OnItemSelectionCleared();
					SelectionChanged.Execute(GetNotifyEventArguments());
				}
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
							callback->OnTotalSizeChanged();
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
					if(!value)
					{
						ClearStyles();
					}
					callback=value;
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
			}
		}
	}
}