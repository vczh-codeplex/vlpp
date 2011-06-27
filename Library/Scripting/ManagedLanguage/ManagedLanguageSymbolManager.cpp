#include "ManagedLanguageSymbolManager.h"

namespace vl
{
	namespace scripting
	{
		namespace managedlanguage
		{

/***********************************************************************
ManagedSymbolItem
***********************************************************************/

			ManagedSymbolItem::ManagedSymbolItem(ManagedSymbolManager* _manager)
				:manager(_manager)
				,parent(0)
			{
				manager->Register(this);
			}

			ManagedSymbolItem::~ManagedSymbolItem()
			{
			}

			ManagedSymbolManager* ManagedSymbolItem::GetManager()
			{
				return manager;
			}

			ManagedSymbolItemGroup* ManagedSymbolItem::GetParent()
			{
				return parent;
			}

			const WString& ManagedSymbolItem::GetName()
			{
				return name;
			}

			const ManagedSymbolItem::IGroupMap& ManagedSymbolItem::ItemGroups()
			{
				return itemGroups.Wrap();
			}

			ManagedSymbolItemGroup* ManagedSymbolItem::ItemGroup(const WString& name)
			{
				vint index=itemGroups.Keys().IndexOf(name);
				return index==-1?0:itemGroups.Values()[index];
			}

			void ManagedSymbolItem::Add(ManagedSymbolItem* item)
			{
				ManagedSymbolItemGroup* group=ItemGroup(item->GetName());
				if(!group)
				{
					group=new ManagedSymbolItemGroup(this);
					manager->Register(group);
				}
				group->Add(item);
			}

/***********************************************************************
ManagedSymbolItemGroup
***********************************************************************/

			ManagedSymbolItemGroup::ManagedSymbolItemGroup(ManagedSymbolItem* _parent)
				:parent(_parent)
			{
			}

			ManagedSymbolItemGroup::~ManagedSymbolItemGroup()
			{
			}

			ManagedSymbolItem* ManagedSymbolItemGroup::GetParent()
			{
				return parent;
			}

			const ManagedSymbolItemGroup::IItemList& ManagedSymbolItemGroup::Items()
			{
				return items.Wrap();
			}

			void ManagedSymbolItemGroup::Add(ManagedSymbolItem* item)
			{
				CHECK_ERROR(item->parent==0, L"ManagedSymbolItemGroup(ManagedSymbolItem*)#一个ManagedSymbolItem只能属于一个ManagedSymbolItemGroup。");
				item->parent=this;
				items.Add(item);
			}

/***********************************************************************
ManagedSymbolItemGroup
***********************************************************************/

			ManagedSymbolManager::ManagedSymbolManager()
			{
				global=new ManagedSymbolItem(this);
				global->name=L"global::";
			}

			ManagedSymbolManager::~ManagedSymbolManager()
			{
			}

			void ManagedSymbolManager::Register(ManagedSymbolItem* item)
			{
				allocatedItems.Add(item);
			}

			void ManagedSymbolManager::Register(ManagedSymbolItemGroup* group)
			{
				allocatedGroups.Add(group);
			}

			ManagedSymbolItem* ManagedSymbolManager::Global()
			{
				return global;
			}
		}
	}
}