#include "ManagedLanguageAnalyzer.h"
#include "..\..\Collections\OperationForEach.h"

namespace vl
{
	namespace scripting
	{
		namespace managedlanguage
		{
			using namespace collections;

/***********************************************************************
ManagedLanguage_BuildGlobalScope1_GenericParameter
***********************************************************************/

			void ManagedLanguage_BuildGlobalScope1_GenericParameter(ManagedGenericInfo* genericInfo, const MAP& argument)
			{
				FOREACH(Ptr<ManagedGenericInfo::Argument>, genericParameter, genericInfo->arguments.Wrap())
				{
					if(ManagedSymbolItemGroup* group=argument.currentSymbol->ItemGroup(genericParameter->name))
					{
						argument.errors.Add(ManagedLanguageCodeException::GetSymbolAlreadyDefined(0, genericParameter->name));
						return;
					}

					ManagedSymbolGenericParameter* symbol=new ManagedSymbolGenericParameter(argument.symbolManager);

					symbol->SetName(genericParameter->name);
					argument.currentSymbol->Add(symbol);
				}
			}

/***********************************************************************
ManagedLanguage_BuildGlobalScope1_Member
***********************************************************************/

			BEGIN_ALGORITHM_PROCEDURE(ManagedLanguage_BuildGlobalScope1_Member, ManagedMember, MAP)

				ALGORITHM_PROCEDURE_MATCH(ManagedField)
				{
					if(ManagedSymbolItemGroup* group=argument.currentSymbol->ItemGroup(node->name))
					{
						argument.errors.Add(ManagedLanguageCodeException::GetSymbolAlreadyDefined(node, node->name));
						return;
					}

					ManagedSymbolField* symbol=new ManagedSymbolField(argument.symbolManager);
					argument.symbolManager->SetSymbol(node, symbol);

					symbol->typeLanguageElement=node;
					symbol->enumerationLanguageElement=0;
					symbol->SetName(node->name);
					argument.currentSymbol->Add(symbol);
				}

				ALGORITHM_PROCEDURE_MATCH(ManagedMethod)
				{
					if(ManagedSymbolItemGroup* group=argument.currentSymbol->ItemGroup(node->name))
					{
						FOREACH(ManagedSymbolItem*, item, group->Items())
						{
							if(item->GetSymbolType()!=ManagedSymbolItem::Method)
							{
								argument.errors.Add(ManagedLanguageCodeException::GetSymbolAlreadyDefined(node, node->name));
								return;
							}
						}
					}

					ManagedSymbolMethod* symbol=new ManagedSymbolMethod(argument.symbolManager);
					argument.symbolManager->SetSymbol(node, symbol);

					symbol->languageElement=node;
					symbol->SetName(node->name);
					argument.currentSymbol->Add(symbol);
				}

				ALGORITHM_PROCEDURE_MATCH(ManagedConstructor)
				{
					if(ManagedSymbolItemGroup* group=argument.currentSymbol->ItemGroup(ManagedSymbolConstructor::SymbolName))
					{
						FOREACH(ManagedSymbolItem*, item, group->Items())
						{
							if(item->GetSymbolType()!=ManagedSymbolItem::Constructor)
							{
								argument.errors.Add(ManagedLanguageCodeException::GetSymbolAlreadyDefined(node, ManagedSymbolConstructor::SymbolName));
								return;
							}
						}
					}

					ManagedSymbolConstructor* symbol=new ManagedSymbolConstructor(argument.symbolManager);
					argument.symbolManager->SetSymbol(node, symbol);

					symbol->languageElement=node;
					argument.currentSymbol->Add(symbol);
				}

				ALGORITHM_PROCEDURE_MATCH(ManagedTypeMember)
				{
					ManagedLanguage_BuildGlobalScope1_Declaration(node->declaration, argument);
				}

				ALGORITHM_PROCEDURE_MATCH(ManagedExtendedMember)
				{
					ManagedLanguage_BuildGlobalScope1_ExtendedMember(node, argument);
				}

			END_ALGORITHM_PROCEDURE(ManagedLanguage_BuildGlobalScope1_Member)

/***********************************************************************
ManagedLanguage_BuildGlobalScope1_ExtendedMember
***********************************************************************/

			BEGIN_ALGORITHM_PROCEDURE(ManagedLanguage_BuildGlobalScope1_ExtendedMember, ManagedExtendedMember, MAP)

				ALGORITHM_PROCEDURE_MATCH(ManagedProperty)
				{
					if(ManagedSymbolItemGroup* group=argument.currentSymbol->ItemGroup(node->name))
					{
						FOREACH(ManagedSymbolItem*, item, group->Items())
						{
							if(item->GetSymbolType()==ManagedSymbolItem::Property)
							{
								ManagedSymbolProperty* symbol=dynamic_cast<ManagedSymbolProperty*>(item);
								if(!node->implementedInterfaceType && !symbol->languageElement->implementedInterfaceType)
								{
									argument.errors.Add(ManagedLanguageCodeException::GetSymbolAlreadyDefined(node, node->name));
									return;
								}
							}
							else
							{
								argument.errors.Add(ManagedLanguageCodeException::GetSymbolAlreadyDefined(node, node->name));
								return;
							}
						}
					}

					ManagedSymbolProperty* symbol=new ManagedSymbolProperty(argument.symbolManager);
					argument.symbolManager->SetSymbol(node, symbol);

					symbol->languageElement=node;
					symbol->SetName(node->name);
					argument.currentSymbol->Add(symbol);
				}

				ALGORITHM_PROCEDURE_MATCH(ManagedConverterOperator)
				{
					if(ManagedSymbolItemGroup* group=argument.currentSymbol->ItemGroup(ManagedSymbolConverterOperator::SymbolName))
					{
						FOREACH(ManagedSymbolItem*, item, group->Items())
						{
							if(item->GetSymbolType()!=ManagedSymbolItem::ConverterOperator)
							{
								argument.errors.Add(ManagedLanguageCodeException::GetSymbolAlreadyDefined(node, ManagedSymbolConverterOperator::SymbolName));
								return;
							}
						}
					}

					ManagedSymbolConverterOperator* symbol=new ManagedSymbolConverterOperator(argument.symbolManager);
					argument.symbolManager->SetSymbol(node, symbol);

					symbol->languageElement=node;
					argument.currentSymbol->Add(symbol);
				}

			END_ALGORITHM_PROCEDURE(ManagedLanguage_BuildGlobalScope1_ExtendedMember)

/***********************************************************************
ManagedLanguage_BuildGlobalScope1_Declaration
***********************************************************************/

			BEGIN_ALGORITHM_PROCEDURE(ManagedLanguage_BuildGlobalScope1_Declaration, ManagedDeclaration, MAP)

				ALGORITHM_PROCEDURE_MATCH(ManagedTypeDeclaration)
				{
					if(ManagedSymbolItemGroup* group=argument.currentSymbol->ItemGroup(node->name))
					{
						FOREACH(ManagedSymbolItem*, item, group->Items())
						{
							if(item->GetSymbolType()!=ManagedSymbolItem::Namespace)
							{
								switch(item->GetSymbolType())
								{
								case ManagedSymbolItem::Class:
								case ManagedSymbolItem::Structure:
								case ManagedSymbolItem::Interface:
									{
										ManagedSymbolDeclaration* symbol=dynamic_cast<ManagedSymbolDeclaration*>(item);
										if(symbol->orderedGenericParameterNames.Count()==node->genericInfo.arguments.Count())
										{
											argument.errors.Add(ManagedLanguageCodeException::GetSymbolAlreadyDefined(node, node->name));
											return;
										}
									}
									break;
								default:
									argument.errors.Add(ManagedLanguageCodeException::GetSymbolAlreadyDefined(node, node->name));
									return;
								}
							}
						}
					}

					ManagedSymbolItem::ManagedSymbolType symbolType=ManagedSymbolItem::Class;
					switch(node->declarationType)
					{
					case ManagedTypeDeclaration::Class:
						symbolType=ManagedSymbolItem::Class;
						break;
					case ManagedTypeDeclaration::Structure:
						symbolType=ManagedSymbolItem::Structure;
						break;
					case ManagedTypeDeclaration::Interface:
						symbolType=ManagedSymbolItem::Interface;
						break;
					}
					ManagedSymbolDeclaration* symbol=new ManagedSymbolDeclaration(argument.symbolManager, symbolType);
					argument.symbolManager->SetSymbol(node, symbol);

					symbol->typeLanguageElement=node;
					symbol->enumerationLanguageElement=0;
					symbol->SetName(node->name);
					argument.currentSymbol->Add(symbol);

					MAP newArgument(argument, symbol);
					ManagedLanguage_BuildGlobalScope1_GenericParameter(&node->genericInfo, newArgument);

					FOREACH(Ptr<ManagedMember>, member, node->members.Wrap())
					{
						ManagedLanguage_BuildGlobalScope1_Member(member, newArgument);
					}
				}

				ALGORITHM_PROCEDURE_MATCH(ManagedNamespaceDeclaration)
				{
					ManagedSymbolItem* currentSymbol=argument.currentSymbol;
					FOREACH(WString, name, node->namespaceFragments.Wrap())
					{
						bool pass=false;
						if(ManagedSymbolItemGroup* group=currentSymbol->ItemGroup(name))
						{
							FOREACH(ManagedSymbolItem*, item, group->Items())
							{
								switch(item->GetSymbolType())
								{
								case ManagedSymbolItem::Class:
								case ManagedSymbolItem::Structure:
								case ManagedSymbolItem::Interface:
								case ManagedSymbolItem::TypeRename:
									break;
								case ManagedSymbolItem::Namespace:
									currentSymbol=item;
									pass=true;
									break;
								default:
									argument.errors.Add(ManagedLanguageCodeException::GetSymbolAlreadyDefined(node, name));
									return;
								}
							}
						}

						if(!pass)
						{
							ManagedSymbolNamespace* symbol=new ManagedSymbolNamespace(argument.symbolManager);
							symbol->SetName(name);
							currentSymbol->Add(symbol);
							currentSymbol=symbol;
						}
					}

					ManagedSymbolNamespace* symbol=dynamic_cast<ManagedSymbolNamespace*>(currentSymbol);
					argument.symbolManager->SetSymbol(node, symbol);
					symbol->languageElements.Add(node);

					MAP newArgument(argument, currentSymbol);
					FOREACH(Ptr<ManagedDeclaration>, declaration, node->declarations.Wrap())
					{
						ManagedLanguage_BuildGlobalScope1_Declaration(declaration, newArgument);
					}
				}

				ALGORITHM_PROCEDURE_MATCH(ManagedExtendedDeclaration)
				{
					ManagedLanguage_BuildGlobalScope1_ExtendedDeclaration(node, argument);
				}

			END_ALGORITHM_PROCEDURE(ManagedLanguage_BuildGlobalScope1_Declaration)

/***********************************************************************
ManagedLanguage_BuildGlobalScope1_ExtendedDeclaration
***********************************************************************/

			BEGIN_ALGORITHM_PROCEDURE(ManagedLanguage_BuildGlobalScope1_ExtendedDeclaration, ManagedExtendedDeclaration, MAP)

				ALGORITHM_PROCEDURE_MATCH(ManagedEnumerationDeclaration)
				{
					if(ManagedSymbolItemGroup* group=argument.currentSymbol->ItemGroup(node->name))
					{
						FOREACH(ManagedSymbolItem*, item, group->Items())
						{
							if(item->GetSymbolType()!=ManagedSymbolItem::Namespace)
							{
								switch(item->GetSymbolType())
								{
								case ManagedSymbolItem::Class:
								case ManagedSymbolItem::Structure:
								case ManagedSymbolItem::Interface:
									{
										ManagedSymbolDeclaration* symbol=dynamic_cast<ManagedSymbolDeclaration*>(item);
										if(symbol->orderedGenericParameterNames.Count()==0)
										{
											argument.errors.Add(ManagedLanguageCodeException::GetSymbolAlreadyDefined(node, node->name));
											return;
										}
									}
									break;
								default:
									argument.errors.Add(ManagedLanguageCodeException::GetSymbolAlreadyDefined(node, node->name));
									return;
								}
							}
						}
					}

					ManagedSymbolDeclaration* symbol=new ManagedSymbolDeclaration(argument.symbolManager, ManagedSymbolItem::Structure);
					argument.symbolManager->SetSymbol(node, symbol);

					symbol->typeLanguageElement=0;
					symbol->enumerationLanguageElement=node;
					symbol->SetName(node->name);
					argument.currentSymbol->Add(symbol);

					FOREACH(Ptr<ManagedEnumItem>, enumItem, node->items.Wrap())
					{
						if(symbol->ItemGroup(enumItem->name))
						{
							argument.errors.Add(ManagedLanguageCodeException::GetSymbolAlreadyDefined(enumItem.Obj(), enumItem->name));
						}
						else
						{
							ManagedSymbolField* field=new ManagedSymbolField(argument.symbolManager);
							argument.symbolManager->SetSymbol(enumItem.Obj(), field);

							field->typeLanguageElement=0;
							field->enumerationLanguageElement=enumItem.Obj();
							field->SetName(enumItem->name);
							symbol->Add(field);
						}
					}
				}

				ALGORITHM_PROCEDURE_MATCH(ManagedTypeRenameDeclaration)
				{
					if(ManagedSymbolItemGroup* group=argument.currentSymbol->ItemGroup(node->name))
					{
						FOREACH(ManagedSymbolItem*, item, group->Items())
						{
							if(item->GetSymbolType()!=ManagedSymbolItem::Namespace)
							{
								argument.errors.Add(ManagedLanguageCodeException::GetSymbolAlreadyDefined(node, node->name));
								return;
							}
						}
					}

					ManagedSymbolTypeRename* symbol=new ManagedSymbolTypeRename(argument.symbolManager);
					argument.symbolManager->SetSymbol(node, symbol);
					
					symbol->languageElement=node;
					symbol->SetName(node->name);
					argument.currentSymbol->Add(symbol);

					ManagedLanguage_BuildGlobalScope1_GenericParameter(&node->genericInfo, MAP(argument, symbol));
				}

				ALGORITHM_PROCEDURE_MATCH(ManagedUsingNamespaceDeclaration)
				{
					if(ManagedSymbolItemGroup* group=argument.currentSymbol->ItemGroup(ManagedSymbolUsingNamespace::SymbolName))
					{
						FOREACH(ManagedSymbolItem*, item, group->Items())
						{
							if(item->GetSymbolType()!=ManagedSymbolItem::UsingNamespace)
							{
								argument.errors.Add(ManagedLanguageCodeException::GetSymbolAlreadyDefined(node, ManagedSymbolUsingNamespace::SymbolName));
								return;
							}
						}
					}

					ManagedSymbolUsingNamespace* symbol=new ManagedSymbolUsingNamespace(argument.symbolManager);
					argument.symbolManager->SetSymbol(node, symbol);
					
					symbol->languageElement=node;
					argument.currentSymbol->Add(symbol);
				}

			END_ALGORITHM_PROCEDURE(ManagedLanguage_BuildGlobalScope1_ExtendedDeclaration)
		}
	}
}