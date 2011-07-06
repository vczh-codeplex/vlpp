#include "ManagedLanguageAnalyzer.h"
#include "..\..\Collections\OperationForEach.h"

namespace vl
{
	namespace scripting
	{
		namespace managedlanguage
		{
			using namespace collections;

			void SearchType(List<ManagedSymbolItem*>& symbolItems, vint& start, vint& end, const WString& name, vint genericParameterCount)
			{
				// TODO: consider accessor
				for(vint i=start;i<end;i++)
				{
					ManagedSymbolItemGroup* group=symbolItems[i]->ItemGroup(name);
					if(group)
					{
						FOREACH(ManagedSymbolItem*, item, group->Items())
						{
							switch(item->GetSymbolType())
							{
							case ManagedSymbolItem::Namespace:
								{
									if(genericParameterCount==0)
									{
										symbolItems.Add(item);
									}
								}
								break;
							case ManagedSymbolItem::Class:
							case ManagedSymbolItem::Structure:
							case ManagedSymbolItem::Interface:
								{
									if(genericParameterCount==dynamic_cast<ManagedSymbolDeclaration*>(item)->orderedGenericParameterNames.Count())
									{
										symbolItems.Add(item);
									}
								}
								break;
							case ManagedSymbolItem::TypeRename:
								{
									if(genericParameterCount==dynamic_cast<ManagedSymbolTypeRename*>(item)->orderedGenericParameterNames.Count())
									{
										symbolItems.Add(item);
									}
								}
								break;
							}
						}
					}
				}
				start=end;
				end=symbolItems.Count();
			}

			void FilterType(List<ManagedSymbolItem*>& symbolItems, vint& start, vint& end)
			{
				for(vint i=start;i<end;i++)
				{
					ManagedSymbolItem* item=symbolItems[i];
					switch(item->GetSymbolType())
					{
					case ManagedSymbolItem::Class:
					case ManagedSymbolItem::Structure:
					case ManagedSymbolItem::Interface:
					case ManagedSymbolItem::TypeRename:
						{
							symbolItems.Add(item);
						}
						break;
					}
				}
				start=end;
				end=symbolItems.Count();
			}

			ManagedTypeSymbol* GetSystemType(ManagedLanguageElement* element, const WString& name, const MAP& argument)
			{
				List<ManagedSymbolItem*> symbolItems;
				symbolItems.Add(argument.symbolManager->Global());
				vint start=0;
				vint end=symbolItems.Count();
				SearchType(symbolItems, start, end, L"System", 0);
				SearchType(symbolItems, start, end, name, 0);
				FilterType(symbolItems, start, end);
				if(start==end)
				{
					argument.errors.Add(ManagedLanguageCodeException::GetSystemTypeNotExists(element, name));
					return 0;
				}
				else if(start+1<end)
				{
					argument.errors.Add(ManagedLanguageCodeException::GetSystemTypeDuplicated(element, name));
					return 0;
				}
				else
				{
					ManagedSymbolItem* baseType=symbolItems[start];
					return argument.symbolManager->GetType(baseType);
				}
			}
			
			void ManagedLanguage_BuildGlobalScope2_GenericParameter(ManagedSymbolItem* symbol, List<WString>& orderedGenericParameterNames, ManagedGenericInfo* genericInfo, const MAP& argument)
			{
			}

/***********************************************************************
ManagedLanguage_GetTypeSymbol_Type
***********************************************************************/

			BEGIN_ALGORITHM_FUNCTION(ManagedLanguage_GetTypeSymbol_Type, ManagedType, MAP, ManagedTypeSymbol*)

				ALGORITHM_FUNCTION_MATCH(ManagedReferencedType)
				{
					return 0;
				}

				ALGORITHM_FUNCTION_MATCH(ManagedMemberType)
				{
					return 0;
				}

				ALGORITHM_FUNCTION_MATCH(ManagedInstantiatedGenericType)
				{
					return 0;
				}

				ALGORITHM_FUNCTION_MATCH(ManagedExtendedType)
				{
					return ManagedLanguage_GetTypeSymbol_ExtendedType(node, argument);
				}

			END_ALGORITHM_FUNCTION(ManagedLanguage_GetTypeSymbol_Type)

/***********************************************************************
ManagedLanguage_GetTypeSymbol_ExtendedType
***********************************************************************/

			BEGIN_ALGORITHM_FUNCTION(ManagedLanguage_GetTypeSymbol_ExtendedType, ManagedExtendedType, MAP, ManagedTypeSymbol*)

				ALGORITHM_FUNCTION_MATCH(ManagedArrayType)
				{
					return 0;
				}

				ALGORITHM_FUNCTION_MATCH(ManagedFunctionType)
				{
					return 0;
				}

				ALGORITHM_FUNCTION_MATCH(ManagedEventType)
				{
					return 0;
				}

				ALGORITHM_FUNCTION_MATCH(ManagedAutoReferType)
				{
					return 0;
				}

				ALGORITHM_FUNCTION_MATCH(ManagedDynamicType)
				{
					return 0;
				}

			END_ALGORITHM_FUNCTION(ManagedLanguage_GetTypeSymbol_ExtendedType)

/***********************************************************************
ManagedLanguage_BuildGlobalScope2_Member
***********************************************************************/

			BEGIN_ALGORITHM_PROCEDURE(ManagedLanguage_BuildGlobalScope2_Member, ManagedMember, MAP)

				ALGORITHM_PROCEDURE_MATCH(ManagedField)
				{
				}

				ALGORITHM_PROCEDURE_MATCH(ManagedMethod)
				{
				}

				ALGORITHM_PROCEDURE_MATCH(ManagedConstructor)
				{
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
				}

				ALGORITHM_PROCEDURE_MATCH(ManagedConverterOperator)
				{
				}

			END_ALGORITHM_PROCEDURE(ManagedLanguage_BuildGlobalScope2_ExtendedMember)

/***********************************************************************
ManagedLanguage_BuildGlobalScope2_Declaration
***********************************************************************/

			BEGIN_ALGORITHM_PROCEDURE(ManagedLanguage_BuildGlobalScope2_Declaration, ManagedDeclaration, MAP)

				ALGORITHM_PROCEDURE_MATCH(ManagedTypeDeclaration)
				{
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
					symbol->accessor=node->accessor;
					symbol->inheritation=declatt::Sealed;

					if(ManagedTypeSymbol* baseType=GetSystemType(node, L"EnumItemBase", argument))
					{
						symbol->baseTypes.Add(baseType);
					}

					FOREACH(Ptr<ManagedEnumItem>, enumItem, node->items.Wrap())
					{
						symbol->orderedDataMemberNames.Add(enumItem->name);
						ManagedSymbolField* field=dynamic_cast<ManagedSymbolField*>(symbol->ItemGroup(enumItem->name)->Items()[0]);

						field->accessor=declatt::Public;
						field->memberType=declatt::Static;
						field->dataType=declatt::Readonly;
						field->type=argument.symbolManager->GetType(symbol);
					}
				}

				ALGORITHM_PROCEDURE_MATCH(ManagedTypeRenameDeclaration)
				{
					ManagedSymbolTypeRename* symbol=argument.symbolManager->GetTypedSymbol<ManagedSymbolTypeRename>(node);
					symbol->accessor=node->accessor;
					symbol->type=ManagedLanguage_GetTypeSymbol_Type(node->type, argument);
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