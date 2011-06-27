#include "ManagedLanguageSymbolManager.h"
#include "..\..\Collections\Operation.h"

namespace vl
{
	namespace scripting
	{
		namespace managedlanguage
		{
			using namespace collections;

/***********************************************************************
ManagedSymbolItem
***********************************************************************/

			ManagedSymbolItem::ManagedSymbolItem(ManagedSymbolManager* _manager)
				:manager(_manager)
				,parent(0)
				,associatedType(0)
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

			void ManagedSymbolItem::SetName(const WString& value)
			{
				name=value;
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
					itemGroups.Add(item->GetName(), group);
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
				CHECK_ERROR(item->parent==0, L"ManagedSymbolItemGroup::Add(ManagedSymbolItem*)#一个ManagedSymbolItem只能属于一个ManagedSymbolItemGroup。");
				item->parent=this;
				items.Add(item);
			}

/***********************************************************************
ManagedSymbolItemGroup
***********************************************************************/

			ManagedTypeSymbol::ManagedTypeSymbol(ManagedSymbolManager* _manager, ManagedSymbolItem* _typeSymbol)
				:manager(_manager)
				,typeSymbol(_typeSymbol)
			{
				manager->Register(this);
			}

			ManagedTypeSymbol::~ManagedTypeSymbol()
			{
			}

			ManagedSymbolManager* ManagedTypeSymbol::GetManager()
			{
				return manager;
			}

			ManagedSymbolItem* ManagedTypeSymbol::GetSymbol()
			{
				return typeSymbol;
			}

			ManagedTypeSymbol* ManagedTypeSymbol::GetGenericDeclaration()
			{
				return manager->GetType(typeSymbol);
			}

			const ManagedTypeSymbol::ITypeList& ManagedTypeSymbol::GetGenericArguments()
			{
				return genericArguments.Wrap();
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

			void ManagedSymbolManager::Register(ManagedTypeSymbol* typeSymbol)
			{
				allocatedTypes.Add(typeSymbol);
			}

			ManagedSymbolItem* ManagedSymbolManager::Global()
			{
				return global;
			}

			ManagedTypeSymbol* ManagedSymbolManager::GetType(ManagedSymbolItem* item)
			{
				if(item->associatedType==0)
				{
					item->associatedType=new ManagedTypeSymbol(this, item);
				}
				return item->associatedType;
			}

			ManagedTypeSymbol* ManagedSymbolManager::GetType(ManagedSymbolItem* item, const collections::IReadonlyList<ManagedTypeSymbol*>& genericArguments)
			{
				ManagedTypeSymbol* genericDeclaration=GetType(item);
				for(vint i=0;i<genericDeclaration->associatedInstantiatedTypes.Count();i++)
				{
					ManagedTypeSymbol* instantiated=genericDeclaration->associatedInstantiatedTypes[i];
					if(CompareEnumerable(instantiated->genericArguments.Wrap(), genericArguments)==0)
					{
						return instantiated;
					}
				}

				ManagedTypeSymbol* instantiated=new ManagedTypeSymbol(this, genericDeclaration->typeSymbol);
				CopyFrom(instantiated->genericArguments.Wrap(), genericArguments);
				genericDeclaration->associatedInstantiatedTypes.Add(instantiated);
				return instantiated;
			}
		}
	}
}