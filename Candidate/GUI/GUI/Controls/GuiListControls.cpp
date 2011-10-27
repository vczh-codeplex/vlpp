#include "GuiListControls.h"

namespace vl
{
	namespace presentation
	{
		namespace controls
		{

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

/***********************************************************************
GuiListControl
***********************************************************************/

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
					callback->ClearCache();
				}
				itemStyleProvider=value;
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
					callback->ClearCache();
				}
				itemArranger=value;
				if(itemArranger)
				{
					itemProvider->AttachCallback(itemArranger.Obj());
				}
				return old;
			}

			namespace list
			{

/***********************************************************************
GuiListControl
***********************************************************************/

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