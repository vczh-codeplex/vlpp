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
ManagedAnalyzerParameter
***********************************************************************/

			ManagedAnalyzerParameter::ManagedAnalyzerParameter(ManagedSymbolManager* _symbolManager, ErrorList& _errors)
				:symbolManager(_symbolManager)
				,currentSymbol(_symbolManager->Global())
				,errors(_errors)
			{
			}

			ManagedAnalyzerParameter::ManagedAnalyzerParameter(const ManagedAnalyzerParameter& parameter, ManagedSymbolItem* _currentSymbol)
				:symbolManager(parameter.symbolManager)
				,currentSymbol(_currentSymbol)
				,errors(parameter.errors)
			{
			}

/***********************************************************************
ManagedLanguage_AnalyzeProgram
***********************************************************************/

			void ManagedLanguage_AnalyzeProgram(Ptr<ManagedProgram> program, const MAP& argument)
			{
				// build symbol item place holder
				FOREACH(Ptr<ManagedDeclaration>, declaration, program->declarations.Wrap())
				{
					ManagedLanguage_BuildGlobalScope1_Declaration(declaration, argument);
				}
				if(argument.errors.Count()>0) return;
				
				// fill symbol item
				FOREACH(Ptr<ManagedDeclaration>, declaration, program->declarations.Wrap())
				{
					ManagedLanguage_BuildGlobalScope2_Declaration(declaration, argument);
				}
				if(argument.errors.Count()>0) return;
				
				// check symbol item
				FOREACH(Ptr<ManagedDeclaration>, declaration, program->declarations.Wrap())
				{
					ManagedLanguage_BuildGlobalScope3_Declaration(declaration, argument);
				}
				if(argument.errors.Count()>0) return;
			}

/***********************************************************************
GetSystemType
***********************************************************************/

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

			ManagedTypeSymbol* GetSystemType(ManagedLanguageElement* element, const WString& name, const MAP& argument, vint genericParameterCount)
			{
				List<ManagedSymbolItem*> symbolItems;
				symbolItems.Add(argument.symbolManager->Global());
				vint start=0;
				vint end=symbolItems.Count();
				SearchType(symbolItems, start, end, L"System", 0);
				SearchType(symbolItems, start, end, name, genericParameterCount);
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

/***********************************************************************
EnsureUsingNamespaceSymbolCompleted
***********************************************************************/

			void EnsureUsingNamespaceSymbolCompleted(ManagedSymbolUsingNamespace* symbol, const MAP& argument)
			{
				ManagedSymbolItem* currentNamespace=argument.symbolManager->Global();
				if(!symbol->associatedNamespace && symbol->languageElement)
				{
					FOREACH(WString, name, symbol->languageElement->namespaceFragments.Wrap())
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
						symbol->associatedNamespace=dynamic_cast<ManagedSymbolNamespace*>(currentNamespace);
					}
					else
					{
						argument.errors.Add(ManagedLanguageCodeException::GetNamespaceNotExists(symbol->languageElement));
					}
				}
			}

/***********************************************************************
EnsureSymbolBaseTypesCompleted
***********************************************************************/

			void EnsureSymbolBaseTypesCompleted(ManagedSymbolDeclaration* symbol, const MAP& argument)
			{
				if(symbol->baseTypes.Count()==0)
				{
					if(symbol->typeLanguageElement && symbol->typeLanguageElement->baseTypes.Count()>0)
					{
						FOREACH(Ptr<ManagedType>, type, symbol->typeLanguageElement->baseTypes.Wrap())
						{
							ManagedTypeSymbol* typeSymbol=GetTypeSymbol(type, argument, symbol);
							if(typeSymbol)
							{
								symbol->baseTypes.Add(typeSymbol);
							}
						}
					}
					else if(symbol->enumerationLanguageElement)
					{
						if(ManagedTypeSymbol* baseType=GetSystemType(symbol->enumerationLanguageElement, L"EnumItemBase", argument))
						{
							symbol->baseTypes.Add(baseType);
						}
					}
				}
			}

/***********************************************************************
EnsureTypeVisibility
***********************************************************************/

			ManagedTypeSymbol* GetThisType(ManagedSymbolDeclaration* declaration, const MAP& argument)
			{
				ManagedTypeSymbol* declarationType=argument.symbolManager->GetType(declaration);
				if(declaration->orderedGenericParameterNames.Count()>0)
				{
					List<ManagedTypeSymbol*> genericArguments;
					FOREACH(WString, name, declaration->orderedGenericParameterNames.Wrap())
					{
						genericArguments.Add(argument.symbolManager->GetType(declaration->ItemGroup(name)->Items()[0]));
					}
					return argument.symbolManager->GetInstiantiatedType(declarationType, genericArguments.Wrap());
				}
				else
				{
					return declarationType;
				}
			}

			void EnsureTypeVisibility(
				ManagedLanguageElement* languageElement,
				ManagedTypeSymbol* type,
				const MAP& argument,
				ManagedTypeSymbol* thisType,
				List<ManagedTypeSymbol*>& baseTypes
				)
			{
				List<ManagedTypeSymbol*> typeLevels;
				{
					ManagedTypeSymbol* currentType=type;
					while(currentType)
					{
						typeLevels.Add(currentType);
						currentType=currentType->GetGenericDeclaration()
							?currentType->GetGenericDeclaration()->GetParentType()
							:currentType->GetParentType()
							;
					}
				}

				ManagedTypeSymbol* parentType=0;
				for(vint i=typeLevels.Count()-1;i>=0;i--)
				{
					ManagedTypeSymbol* currentType=typeLevels[i];
					ManagedTypeSymbol* currentDeclaration=currentType->GetGenericDeclaration()
						?currentType->GetGenericDeclaration()
						:currentType
						;
					if(currentType->GetGenericDeclaration())
					{
						FOREACH(ManagedTypeSymbol*, genericArgument, currentType->GetGenericArguments())
						{
							EnsureTypeVisibility(languageElement, genericArgument, argument, thisType, baseTypes);
						}
					}

					ManagedSymbolItem* currentSymbol=currentDeclaration->GetSymbol();
					declatt::Accessor currentAccessor=declatt::Public;
					switch(currentSymbol->GetSymbolType())
					{
					case ManagedSymbolItem::Class:
					case ManagedSymbolItem::Structure:
					case ManagedSymbolItem::Interface:
						{
							ManagedSymbolDeclaration* symbol=dynamic_cast<ManagedSymbolDeclaration*>(currentSymbol);
							currentAccessor=symbol->accessor;
						}
						break;
					case ManagedSymbolItem::TypeRename:
						{
							ManagedSymbolTypeRename* symbol=dynamic_cast<ManagedSymbolTypeRename*>(currentSymbol);
							currentAccessor=symbol->accessor;
						}
						break;
					case ManagedSymbolItem::GenericParameter:
						break;
					default:
						argument.errors.Add(ManagedLanguageCodeException::GetTypeInvisible(languageElement, currentType));
						return;
					}

					if(!parentType)
					{
						ManagedSymbolItem* parentSymbol=currentSymbol->GetParentItem();
						switch(parentSymbol->GetSymbolType())
						{
						case ManagedSymbolItem::Class:
						case ManagedSymbolItem::Structure:
						case ManagedSymbolItem::Interface:
							{
								ManagedSymbolDeclaration* parentDeclaration=dynamic_cast<ManagedSymbolDeclaration*>(parentSymbol);
								parentType=GetThisType(parentDeclaration, argument);
							}
							break;
						}
					}
					if(parentType && parentType!=thisType)
					{
						if(baseTypes.Contains(parentType))
						{
							switch(currentAccessor)
							{
							case declatt::Public:
							case declatt::Protected:
							case declatt::Internal:
							case declatt::ProtectedInternal:
								break;
							default:
								argument.errors.Add(ManagedLanguageCodeException::GetTypeInvisible(languageElement, currentType));
								return;
							}
						}
						else
						{
							switch(currentAccessor)
							{
							case declatt::Public:
							case declatt::Internal:
							case declatt::ProtectedInternal:
								break;
							default:
								argument.errors.Add(ManagedLanguageCodeException::GetTypeInvisible(languageElement, currentType));
								return;
							}
						}
					}
				}
			}

			void EnsureTypeVisibility(ManagedLanguageElement* languageElement, ManagedTypeSymbol* type, ManagedSymbolItem* scopeItem, const MAP& argument)
			{
				CHECK_ERROR(
					!scopeItem
					|| scopeItem->GetSymbolType()==ManagedSymbolItem::Class
					|| scopeItem->GetSymbolType()==ManagedSymbolItem::Structure
					|| scopeItem->GetSymbolType()==ManagedSymbolItem::Interface,
					L"EnsureTypeVisibility(ManagedLanguageElement*, ManagedTypeSymbol*, ManagedSymbolItem*, const MAP&)#scopeItemÄÚÈÝ·Ç·¨¡£"
					);
			}
		}
	}
}