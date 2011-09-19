#include "ManagedLanguageSymbolManager.h"
#include "ManagedLanguageAnalyzer.h"
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
ManagedTypeSymbol
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
ManagedAbstractItem
***********************************************************************/

			ManagedAbstractItem::ManagedAbstractItem()
				:type(0)
				,symbol(0)
			{
			}

			ManagedAbstractItem::~ManagedAbstractItem()
			{
			}

			ManagedAbstractItem:: operator bool()const
			{
				return type!=0 && symbol!=0;
			}

			bool ManagedAbstractItem::operator==(const ManagedAbstractItem& value)
			{
				return type==value.type && symbol==value.symbol;
			}

			bool ManagedAbstractItem::operator!=(const ManagedAbstractItem& value)
			{
				return type!=value.type || symbol!=value.symbol;
			}

/***********************************************************************
ManagedSymbolManager
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
				vint index=item->associatedTypes.Keys().IndexOf(parentType);
				if(index==-1)
				{
					ManagedTypeSymbol* result=new ManagedTypeSymbol(this, item, parentType);
					item->associatedTypes.Add(parentType, result);
					return result;
				}
				else
				{
					return item->associatedTypes.Values()[index];
				}
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

			ManagedTypeSymbol* ManagedSymbolManager::ReplaceGenericArguments(ManagedTypeSymbol* type, const collections::IReadonlyDictionary<ManagedTypeSymbol*, ManagedTypeSymbol*>& replacement)
			{
				vint index=replacement.Keys().IndexOf(type);
				if(index!=-1)
				{
					return replacement.Values()[index];
				}
				else if(type->GetGenericDeclaration())
				{
					List<ManagedTypeSymbol*> genericArguments;
					FOREACH(ManagedTypeSymbol*, genericArgument, type->GetGenericArguments())
					{
						genericArguments.Add(ReplaceGenericArguments(genericArgument, replacement));
					}
					ManagedTypeSymbol* genericDeclaration=ReplaceGenericArguments(type->GetGenericDeclaration(), replacement);
					return GetInstiantiatedType(genericDeclaration, genericArguments.Wrap());
				}
				else if(type->GetParentType())
				{
					ManagedTypeSymbol* parentType=ReplaceGenericArguments(type->GetParentType(), replacement);
					return GetType(type->GetSymbol(), parentType);
				}
				else
				{
					return type;
				}
			}

			ManagedTypeSymbol* ManagedSymbolManager::ReplaceGenericArguments(ManagedTypeSymbol* type, ManagedTypeSymbol* contextType)
			{
				ManagedTypeSymbol* currentType=contextType;
				Dictionary<ManagedTypeSymbol*, ManagedTypeSymbol*> map;
				while(currentType)
				{
					if(currentType->GetGenericDeclaration())
					{
						ManagedTypeSymbol* currentDeclarationType=currentType->GetGenericDeclaration();
						ManagedSymbolDeclaration* currentDeclaration=dynamic_cast<ManagedSymbolDeclaration*>(GetRealSymbol(currentDeclarationType->GetSymbol()));
						for(vint i=0;i<currentDeclaration->orderedGenericParameterNames.Count();i++)
						{
							WString genericParameterName=currentDeclaration->orderedGenericParameterNames[i];
							ManagedTypeSymbol* key=GetType(currentDeclaration->ItemGroup(genericParameterName)->Items()[0]);
							ManagedTypeSymbol* value=currentType->GetGenericArguments()[i];
							map.Set(key, value);
						}
						currentType=currentDeclarationType;
					}
					else
					{
						currentType=currentType->GetParentType();
					}
				}
				return ReplaceGenericArguments(type, map.Wrap());
			}

			void ManagedSymbolManager::SetSymbol(ManagedLanguageElement* element, ManagedSymbolItem* symbolItem)
			{
				elementSymbolMap.Add(element, symbolItem);
			}

			ManagedSymbolItem* ManagedSymbolManager::GetSymbol(ManagedLanguageElement* element)
			{
				return elementSymbolMap[element];
			}

			ManagedTypeSymbol* ManagedSymbolManager::GetThisType(ManagedSymbolDeclaration* declaration)
			{
				ManagedTypeSymbol* declarationType=GetType(declaration);
				if(declaration->orderedGenericParameterNames.Count()>0)
				{
					List<ManagedTypeSymbol*> genericArguments;
					FOREACH(WString, name, declaration->orderedGenericParameterNames.Wrap())
					{
						genericArguments.Add(GetType(declaration->ItemGroup(name)->Items()[0]));
					}
					return GetInstiantiatedType(declarationType, genericArguments.Wrap());
				}
				else
				{
					return declarationType;
				}
			}

/***********************************************************************
Symbol Names
***********************************************************************/

			const wchar_t* const ManagedSymbolConstructor::SymbolName = L"$SpecialName$Constructor";
			const wchar_t* const ManagedSymbolUsingNamespace::SymbolName = L"$SpecialName$UsingNamespace";
			const wchar_t* const ManagedSymbolConverterOperator::SymbolName = L"$SpecialName$ConverterOperator";
			const wchar_t* const ManagedSymbolGlobal::SymbolName = L"$SpecialName$Global";
			const wchar_t* const ManagedSymbolBlock::SymbolName = L"$SpecialName$Block";
			const wchar_t* const ManagedSymbolLambda::SymbolName = L"$SpecialName$Lambda";

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
				,memberType(declatt::Instance)
				,_baseType(0)
			{
				CHECK_ERROR(_symbolType==ManagedSymbolItem::Class || _symbolType==ManagedSymbolItem::Structure || _symbolType==ManagedSymbolItem::Interface,
					L"ManagedSymbolDeclaration::ManagedSymbolDeclaration(ManagedSymbolManager*, ManagedSymbolType)#符号类型不在规定范围内。"
					);
			}

			ManagedSymbolGlobal::ManagedSymbolGlobal(ManagedSymbolManager* _manager)
				:ManagedSymbolItem(_manager, ManagedSymbolItem::Global)
			{
				SetName(SymbolName);
			}

			ManagedSymbolBlock::ManagedSymbolBlock(ManagedSymbolManager* _manager)
				:ManagedSymbolItem(_manager, ManagedSymbolItem::Block)
				,languageElement(0)
			{
				SetName(SymbolName);
			}

			ManagedSymbolVariable::ManagedSymbolVariable(ManagedSymbolManager* _manager)
				:ManagedSymbolItem(_manager, ManagedSymbolItem::Variable)
				,variableLanguageElement(0)
				,catchLanguageElement(0)
				,usingLanguageElement(0)
				,forEachLanguageElement(0)
				,type(0)
				,constant(false)
			{
			}

			ManagedSymbolLambda::ManagedSymbolLambda(ManagedSymbolManager* _manager)
				:ManagedSymbolItem(_manager, ManagedSymbolItem::Lambda)
				,languageElement(0)
				,returnType(0)
			{
				SetName(SymbolName);
			}

			ManagedSymbolLambdaParameter::ManagedSymbolLambdaParameter(ManagedSymbolManager* _manager)
				:ManagedSymbolItem(_manager, ManagedSymbolItem::LambdaParameter)
				,languageElement(0)
				,type(0)
			{
			}
		}
	}
}