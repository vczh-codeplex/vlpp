#include "GuiListControls.h"
#include "..\..\..\..\Library\Collections\OperationCopyFrom.h"

namespace vl
{
	namespace presentation
	{
		namespace controls
		{
			using namespace collections;

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
					listControl->itemStyleProvider->DestroyItemStyle(cachedStyles[i]);
				}
			}

			void GuiListControl::ItemCallback::OnAttached(IItemProvider* provider)
			{
			}

			void GuiListControl::ItemCallback::OnItemModified(int start, int count, int newCount)
			{
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
						break;
					}
				}
				if(!style)
				{
					style=listControl->itemStyleProvider->CreateItemStyle(id);
				}
				listControl->itemStyleProvider->Install(style, itemIndex);
				style->OnInstalled();
				return style;
			}

			void GuiListControl::ItemCallback::ReleaseItem(IItemStyleController* style)
			{
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
				SetStyleProvider(0);
				SetArranger(0);
				itemProvider=0;
				callback=0;
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
				}

				GetVerticalScroll()->SetPosition(0);
				GetHorizontalScroll()->SetPosition(0);
				if(itemArranger)
				{
					itemArranger->OnItemModified(0, itemProvider->Count(), itemProvider->Count());
				}
				return old;
			}

			namespace list
			{

/***********************************************************************
FixedHeightItemArranger
***********************************************************************/

				void FixedHeightItemArranger::RearrangeItemBounds()
				{
				}

				FixedHeightItemArranger::FixedHeightItemArranger()
					:callback(0)
					,startIndex(0)
					,endIndex(-1)
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

				void FixedHeightItemArranger::OnItemModified(int start, int count, int newCount)
				{
					if(callback)
					{
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
						startIndex=0;
						endIndex=-1;
						rowHeight=1;
						visibleStyles.Clear();
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
							int newEndIndex=(bounds.Bottom()-1)/newRowHeight;

							for(int i=newStartIndex;i<=newEndIndex;i++)
							{
								if(startIndex<=i && i<=endIndex)
								{
									visibleStyles.Add(visibleStyles[startIndex+i-newStartIndex]);
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
							endIndex=newEndIndex;
							RearrangeItemBounds();
						}
					}
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