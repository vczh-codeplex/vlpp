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

			ManagedSymbolItem::ManagedSymbolItem(ManagedSymbolManager* _manager, ManagedSymbolType _symbolType)
				:manager(_manager)
				,parent(0)
				,associatedType(0)
				,symbolType(_symbolType)
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

			ManagedSymbolItemGroup* ManagedSymbolItem::GetParentGroup()
			{
				return parent;
			}

			ManagedSymbolItem* ManagedSymbolItem::GetParentItem()
			{
				return parent?parent->GetParentItem():0;
			}

			const WString& ManagedSymbolItem::GetName()
			{
				return name;
			}

			void ManagedSymbolItem::SetName(const WString& value)
			{
				name=value;
			}

			ManagedSymbolItem::ManagedSymbolType ManagedSymbolItem::GetSymbolType()
			{
				return symbolType;
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

			ManagedSymbolItem* ManagedSymbolItemGroup::GetParentItem()
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

			ManagedTypeSymbol::ManagedTypeSymbol(ManagedSymbolManager* _manager, ManagedSymbolItem* _typeSymbol, ManagedTypeSymbol* _parentType)
				:manager(_manager)
				,typeSymbol(_typeSymbol)
				,parentType(_parentType)
				,genericDeclaration(0)
			{
				manager->Register(this);
			}

			ManagedTypeSymbol::ManagedTypeSymbol(ManagedSymbolManager* _manager, ManagedTypeSymbol* _genericDeclaration)
				:manager(_manager)
				,typeSymbol(_genericDeclaration->typeSymbol)
				,parentType(0)
				,genericDeclaration(_genericDeclaration)
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

			ManagedTypeSymbol* ManagedTypeSymbol::GetParentType()
			{
				return parentType;
			}

			ManagedTypeSymbol* ManagedTypeSymbol::GetGenericDeclaration()
			{
				return genericDeclaration;
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
				global=new ManagedSymbolGlobal(this);
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

			ManagedTypeSymbol* ManagedSymbolManager::GetType(ManagedSymbolItem* item, ManagedTypeSymbol* parentType)
			{
				if(item->associatedType==0)
				{
					item->associatedType=new ManagedTypeSymbol(this, item, parentType);
				}
				return item->associatedType;
			}

			ManagedTypeSymbol* ManagedSymbolManager::GetInstiantiatedType(ManagedTypeSymbol* genericDeclaration, const collections::IReadonlyList<ManagedTypeSymbol*>& genericArguments)
			{
				for(vint i=0;i<genericDeclaration->associatedInstantiatedTypes.Count();i++)
				{
					ManagedTypeSymbol* instantiated=genericDeclaration->associatedInstantiatedTypes[i];
					if(CompareEnumerable(instantiated->genericArguments.Wrap(), genericArguments)==0)
					{
						return instantiated;
					}
				}

				ManagedTypeSymbol* instantiated=new ManagedTypeSymbol(this, genericDeclaration);
				CopyFrom(instantiated->genericArguments.Wrap(), genericArguments);
				genericDeclaration->associatedInstantiatedTypes.Add(instantiated);
				return instantiated;
			}

			void ManagedSymbolManager::SetSymbol(ManagedLanguageElement* element, ManagedSymbolItem* symbolItem)
			{
				elementSymbolMap.Add(element, symbolItem);
			}

			ManagedSymbolItem* ManagedSymbolManager::GetSymbol(ManagedLanguageElement* element)
			{
				return elementSymbolMap[element];
			}

/***********************************************************************
Symbol Names
***********************************************************************/

			const wchar_t* const ManagedSymbolConstructor::SymbolName = L"$SpecialName$Constructor";
			const wchar_t* const ManagedSymbolUsingNamespace::SymbolName = L"$SpecialName$UsingNamespace";
			const wchar_t* const ManagedSymbolConverterOperator::SymbolName = L"$SpecialName$ConverterOperator";
			const wchar_t* const ManagedSymbolGlobal::SymbolName = L"$SpecialName$Global";

/***********************************************************************
Symbol Constructors
***********************************************************************/

			ManagedSymbolGenericParameter::ManagedSymbolGenericParameter(ManagedSymbolManager* _manager)
				:ManagedSymbolItem(_manager, ManagedSymbolItem::GenericParameter)
				,conversion(ManagedGenericInfo::InOut)
				,newConstraint(false)
			{
			}

			ManagedSymbolMethodParameter::ManagedSymbolMethodParameter(ManagedSymbolManager* _manager)
				:ManagedSymbolItem(_manager, ManagedSymbolItem::MethodParameter)
				,languageElement(0)
				,parameterType(ManagedParameter::Normal)
				,containsDefaultValue(false)
				,type(0)
			{
			}

			ManagedSymbolField::ManagedSymbolField(ManagedSymbolManager* _manager)
				:ManagedSymbolItem(_manager, ManagedSymbolItem::Field)
				,typeLanguageElement(0)
				,enumerationLanguageElement(0)
				,accessor(declatt::Private)
				,memberType(declatt::Instance)
				,dataType(declatt::Variable)
				,type(0)
			{
			}

			ManagedSymbolProperty::ManagedSymbolProperty(ManagedSymbolManager* _manager)
				:ManagedSymbolItem(_manager, ManagedSymbolItem::Property)
				,languageElement(0)
				,accessor(declatt::Private)
				,setterAccessor(declatt::Private)
				,memberType(declatt::Instance)
				,inheritation(declatt::Sealed)
				,type(0)
				,implementedInterfaceType(0)
				,containsGetter(false)
				,containsSetter(false)
			{
			}

			ManagedSymbolPropertySetterValue::ManagedSymbolPropertySetterValue(ManagedSymbolManager* _manager)
				:ManagedSymbolItem(_manager, ManagedSymbolItem::PropertySetterValue)
				,associatedProperty(0)
			{
			}

			ManagedSymbolConverterOperator::ManagedSymbolConverterOperator(ManagedSymbolManager* _manager)
				:ManagedSymbolItem(_manager, ManagedSymbolItem::ConverterOperator)
				,languageElement(0)
				,accessor(declatt::Private)
				,memberType(declatt::Instance)
				,inheritation(declatt::Sealed)
				,implicit(false)
				,targetType(0)
			{
				SetName(SymbolName);
			}

			ManagedSymbolMethod::ManagedSymbolMethod(ManagedSymbolManager* _manager)
				:ManagedSymbolItem(_manager, ManagedSymbolItem::Method)
				,languageElement(0)
				,accessor(declatt::Private)
				,memberType(declatt::Instance)
				,inheritation(declatt::Sealed)
				,returnType(0)
				,implementedInterfaceType(0)
			{
			}

			ManagedSymbolConstructor::ManagedSymbolConstructor(ManagedSymbolManager* _manager)
				:ManagedSymbolItem(_manager, ManagedSymbolItem::Constructor)
				,languageElement(0)
				,accessor(declatt::Private)
				,implicit(false)
			{
				SetName(SymbolName);
			}

			ManagedSymbolNamespace::ManagedSymbolNamespace(ManagedSymbolManager* _manager)
				:ManagedSymbolItem(_manager, ManagedSymbolItem::Namespace)
			{
			}

			ManagedSymbolUsingNamespace::ManagedSymbolUsingNamespace(ManagedSymbolManager* _manager)
				:ManagedSymbolItem(_manager, ManagedSymbolItem::UsingNamespace)
				,languageElement(0)
				,associatedNamespace(0)
			{
				SetName(SymbolName);
			}

			ManagedSymbolTypeRename::ManagedSymbolTypeRename(ManagedSymbolManager* _manager)
				:ManagedSymbolItem(_manager, ManagedSymbolItem::TypeRename)
				,languageElement(0)
				,accessor(declatt::Private)
				,type(0)
			{
			}

			ManagedSymbolDeclaration::ManagedSymbolDeclaration(ManagedSymbolManager* _manager, ManagedSymbolType _symbolType)
				:ManagedSymbolItem(_manager, _symbolType)
				,typeLanguageElement(0)
				,enumerationLanguageElement(0)
				,accessor(declatt::Private)
				,inheritation(declatt::Sealed)
			{
				CHECK_ERROR(_symbolType==ManagedSymbolItem::Class || _symbolType==ManagedSymbolItem::Structure || _symbolType==ManagedSymbolItem::Interface,
					L"ManagedSymbolDeclaration::ManagedSymbolDeclaration(ManagedSymbolManager*, ManagedSymbolType)#符号类型不在规定范围内。"
					);
			}

			ManagedSymbolGlobal::ManagedSymbolGlobal(ManagedSymbolManager* _manager)
				:ManagedSymbolItem(_manager, Global)
			{
				SetName(SymbolName);
			}
		}
	}
}