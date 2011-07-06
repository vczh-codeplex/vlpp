#include "ManagedLanguageAnalyzer.h"
#include "..\..\Collections\OperationForEach.h"

namespace vl
{
	namespace scripting
	{
		namespace managedlanguage
		{
			using namespace collections;
			
			void ManagedLanguage_BuildGlobalScope2_GenericParameter(ManagedSymbolItem* symbol, List<WString>& orderedGenericParameterNames, ManagedGenericInfo* genericInfo, const MAP& argument)
			{
			}

/***********************************************************************
ManagedLanguage_BuildGlobalScope2_Member
***********************************************************************/

			BEGIN_ALGORITHM_PROCEDURE(ManagedLanguage_BuildGlobalScope2_Member, ManagedMember, MAP)

				ALGORITHM_PROCEDURE_MATCH(ManagedField)
				{
					ManagedSymbolField* symbol=argument.symbolManager->GetTypedSymbol<ManagedSymbolField>(node);
					symbol->type=GetTypeSymbol(node->type, argument);
				}

				ALGORITHM_PROCEDURE_MATCH(ManagedMethod)
				{
					ManagedSymbolMethod* symbol=argument.symbolManager->GetTypedSymbol<ManagedSymbolMethod>(node);
					symbol->returnType=GetTypeSymbol(node->returnType, argument);
					if(node->implementedInterfaceType)
					{
						symbol->implementedInterfaceType=GetTypeSymbol(node->implementedInterfaceType, argument);
					}
					ManagedLanguage_BuildGlobalScope2_GenericParameter(symbol, symbol->orderedGenericParameterNames, &node->genericInfo, argument);

					FOREACH(Ptr<ManagedParameter>, parameter, node->parameters.Wrap())
					{
						symbol->orderedMethodParameterNames.Add(parameter->name);
						ManagedSymbolMethodParameter* parameterSymbol=argument.symbolManager->GetTypedSymbol<ManagedSymbolMethodParameter>(parameter.Obj());

						parameterSymbol->parameterType=parameter->parameterType;
						parameterSymbol->containsDefaultValue=parameter->defaultValue;
						parameterSymbol->type=GetTypeSymbol(parameter->type, argument);
					}
				}

				ALGORITHM_PROCEDURE_MATCH(ManagedConstructor)
				{
					ManagedSymbolConstructor* symbol=argument.symbolManager->GetTypedSymbol<ManagedSymbolConstructor>(node);
					symbol->implicit=node->implicit;

					FOREACH(Ptr<ManagedParameter>, parameter, node->parameters.Wrap())
					{
						symbol->orderedMethodParameterNames.Add(parameter->name);
						ManagedSymbolMethodParameter* parameterSymbol=argument.symbolManager->GetTypedSymbol<ManagedSymbolMethodParameter>(parameter.Obj());

						parameterSymbol->parameterType=parameter->parameterType;
						parameterSymbol->containsDefaultValue=parameter->defaultValue;
						parameterSymbol->type=GetTypeSymbol(parameter->type, argument);
					}
				}

				ALGORITHM_PROCEDURE_MATCH(ManagedTypeMember)
				{
					ManagedLanguage_BuildGlobalScope2_Declaration(node->declaration, argument);
				}

				ALGORITHM_PROCEDURE_MATCH(ManagedExtendedMember)
				{
					ManagedLanguage_BuildGlobalScope2_ExtendedMember(node, argument);
				}

			END_ALGORITHM_PROCEDURE(ManagedLanguage_BuildGlobalScope2_Member)

/***********************************************************************
ManagedLanguage_BuildGlobalScope2_ExtendedMember
***********************************************************************/

			BEGIN_ALGORITHM_PROCEDURE(ManagedLanguage_BuildGlobalScope2_ExtendedMember, ManagedExtendedMember, MAP)

				ALGORITHM_PROCEDURE_MATCH(ManagedProperty)
				{
					ManagedSymbolProperty* symbol=argument.symbolManager->GetTypedSymbol<ManagedSymbolProperty>(node);
					symbol->type=GetTypeSymbol(node->type, argument);
					if(node->implementedInterfaceType)
					{
						symbol->implementedInterfaceType=GetTypeSymbol(node->implementedInterfaceType, argument);
					}
					symbol->containsGetter=node->getter;
					symbol->containsSetter=node->setter;
					if(symbol->containsSetter)
					{
						ManagedSymbolPropertySetterValue* setterValue=new ManagedSymbolPropertySetterValue(argument.symbolManager);
						setterValue->SetName(node->setterParameter);
						setterValue->associatedProperty=symbol;
						symbol->Add(setterValue);
					}
				}

				ALGORITHM_PROCEDURE_MATCH(ManagedConverterOperator)
				{
					ManagedSymbolConverterOperator* symbol=argument.symbolManager->GetTypedSymbol<ManagedSymbolConverterOperator>(node);
					symbol->implicit=node->implicit;
					symbol->targetType=GetTypeSymbol(node->targetType, argument);
					ManagedLanguage_BuildGlobalScope2_GenericParameter(symbol, symbol->orderedGenericParameterNames, &node->genericInfo, argument);
				}

			END_ALGORITHM_PROCEDURE(ManagedLanguage_BuildGlobalScope2_ExtendedMember)

/***********************************************************************
ManagedLanguage_BuildGlobalScope2_Declaration
***********************************************************************/

			BEGIN_ALGORITHM_PROCEDURE(ManagedLanguage_BuildGlobalScope2_Declaration, ManagedDeclaration, MAP)

				ALGORITHM_PROCEDURE_MATCH(ManagedTypeDeclaration)
				{
					ManagedSymbolDeclaration* symbol=argument.symbolManager->GetTypedSymbol<ManagedSymbolDeclaration>(node);
					ManagedLanguage_BuildGlobalScope2_GenericParameter(symbol, symbol->orderedGenericParameterNames, &node->genericInfo, argument);

					FOREACH(Ptr<ManagedType>, type, node->baseTypes.Wrap())
					{
						ManagedTypeSymbol* typeSymbol=GetTypeSymbol(type, argument);
						if(typeSymbol)
						{
							symbol->baseTypes.Add(typeSymbol);
						}
					}

					MAP newArgument(argument, symbol);
					FOREACH(Ptr<ManagedMember>, member, node->members.Wrap())
					{
						ManagedLanguage_BuildGlobalScope2_Member(member, newArgument);
						if(Ptr<ManagedField> field=member.Cast<ManagedField>())
						{
							symbol->orderedDataMemberNames.Add(field->name);
						}
					}
				}

				ALGORITHM_PROCEDURE_MATCH(ManagedNamespaceDeclaration)
				{
					MAP newArgument(argument, argument.symbolManager->GetSymbol(node));
					FOREACH(Ptr<ManagedDeclaration>, declaration, node->declarations.Wrap())
					{
						ManagedLanguage_BuildGlobalScope2_Declaration(declaration, newArgument);
					}
				}

				ALGORITHM_PROCEDURE_MATCH(ManagedExtendedDeclaration)
				{
					ManagedLanguage_BuildGlobalScope2_ExtendedDeclaration(node, argument);
				}

			END_ALGORITHM_PROCEDURE(ManagedLanguage_BuildGlobalScope2_Declaration)

/***********************************************************************
ManagedLanguage_BuildGlobalScope2_ExtendedDeclaration
***********************************************************************/

			BEGIN_ALGORITHM_PROCEDURE(ManagedLanguage_BuildGlobalScope2_ExtendedDeclaration, ManagedExtendedDeclaration, MAP)

				ALGORITHM_PROCEDURE_MATCH(ManagedEnumerationDeclaration)
				{
					ManagedSymbolDeclaration* symbol=argument.symbolManager->GetTypedSymbol<ManagedSymbolDeclaration>(node);

					if(ManagedTypeSymbol* baseType=GetSystemType(node, L"EnumItemBase", argument))
					{
						symbol->baseTypes.Add(baseType);
					}

					FOREACH(Ptr<ManagedEnumItem>, enumItem, node->items.Wrap())
					{
						symbol->orderedDataMemberNames.Add(enumItem->name);
						ManagedSymbolField* field=dynamic_cast<ManagedSymbolField*>(symbol->ItemGroup(enumItem->name)->Items()[0]);

						field->type=argument.symbolManager->GetType(symbol);
					}
				}

				ALGORITHM_PROCEDURE_MATCH(ManagedTypeRenameDeclaration)
				{
					ManagedSymbolTypeRename* symbol=argument.symbolManager->GetTypedSymbol<ManagedSymbolTypeRename>(node);
					symbol->type=GetTypeSymbol(node->type, argument);
					ManagedLanguage_BuildGlobalScope2_GenericParameter(symbol, symbol->orderedGenericParameterNames, &node->genericInfo, argument);
				}

				ALGORITHM_PROCEDURE_MATCH(ManagedUsingNamespaceDeclaration)
				{
					ManagedSymbolItem* currentNamespace=argument.symbolManager->Global();

					FOREACH(WString, name, node->namespaceFragments.Wrap())
					{
						if(currentNamespace)
						{
							if(ManagedSymbolItemGroup* group=currentNamespace->ItemGroup(name))
							{
								ManagedSymbolItem* nextNamespace=0;
								FOREACH(ManagedSymbolItem*, item, group->Items())
								{
									if(item->GetSymbolType()==ManagedSymbolItem::Namespace)
									{
										nextNamespace=item;
										break;
									}
								}
								currentNamespace=nextNamespace;
							}
						}
					}

					if(currentNamespace && currentNamespace->GetSymbolType()==ManagedSymbolItem::Namespace)
					{
						ManagedSymbolUsingNamespace* symbol=argument.symbolManager->GetTypedSymbol<ManagedSymbolUsingNamespace>(node);
						symbol->associatedNamespace=dynamic_cast<ManagedSymbolNamespace*>(currentNamespace);
					}
					else
					{
						argument.errors.Add(ManagedLanguageCodeException::GetNamespaceNotExists(node));
					}
				}

			END_ALGORITHM_PROCEDURE(ManagedLanguage_BuildGlobalScope2_ExtendedDeclaration)
		}
	}
}