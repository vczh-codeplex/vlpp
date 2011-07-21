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

			void EnsureTypeVisibilityInternal(
				ManagedLanguageElement* languageElement,
				ManagedTypeSymbol* type,
				const MAP& argument,
				List<ManagedTypeSymbol*>& thisTypes,
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
							EnsureTypeVisibilityInternal(languageElement, genericArgument, argument, thisTypes, baseTypes);
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
								parentType=argument.symbolManager->GetThisType(parentDeclaration);
							}
							break;
						}
					}
					if(parentType && !thisTypes.Contains(parentType))
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

					if(currentSymbol->GetSymbolType()==ManagedSymbolItem::TypeRename)
					{
						ManagedSymbolTypeRename* symbol=dynamic_cast<ManagedSymbolTypeRename*>(currentSymbol);
						if(currentType->GetGenericDeclaration())
						{
							Dictionary<ManagedTypeSymbol*, ManagedTypeSymbol*> replacement;
							for(vint i=0;i<symbol->orderedGenericParameterNames.Count();i++)
							{
								ManagedTypeSymbol* key=argument.symbolManager->GetType(symbol->ItemGroup(symbol->orderedGenericParameterNames[i])->Items()[0]);
								ManagedTypeSymbol* value=currentType->GetGenericArguments()[i];
								replacement.Add(key, value);
							}
							parentType=argument.symbolManager->ReplaceGenericArguments(currentType, replacement.Wrap());
						}
						else
						{
							parentType=symbol->type;
						}
					}
					else
					{
						parentType=currentType;
					}
				}
			}

			void CollectBaseTypes(ManagedTypeSymbol* thisType, List<ManagedTypeSymbol*>& baseTypes, const MAP& argument)
			{
				vint oldCount=baseTypes.Count();
				if(thisType->GetGenericDeclaration())
				{
					ManagedSymbolDeclaration* symbol=dynamic_cast<ManagedSymbolDeclaration*>(thisType->GetGenericDeclaration()->GetSymbol());

					Dictionary<ManagedTypeSymbol*, ManagedTypeSymbol*> replacement;
					for(vint i=0;i<symbol->orderedGenericParameterNames.Count();i++)
					{
						ManagedTypeSymbol* key=argument.symbolManager->GetType(symbol->ItemGroup(symbol->orderedGenericParameterNames[i])->Items()[0]);
						ManagedTypeSymbol* value=thisType->GetGenericArguments()[i];
						replacement.Add(key, value);
					}

					FOREACH(ManagedTypeSymbol*, baseType, symbol->baseTypes.Wrap())
					{
						ManagedTypeSymbol* translatedBaseType=argument.symbolManager->ReplaceGenericArguments(baseType, replacement.Wrap());
						if(!baseTypes.Contains(translatedBaseType))
						{
							baseTypes.Add(translatedBaseType);
						}
					}
				}
				else
				{
					ManagedSymbolDeclaration* symbol=dynamic_cast<ManagedSymbolDeclaration*>(thisType->GetSymbol());
					FOREACH(ManagedTypeSymbol*, baseType, symbol->baseTypes.Wrap())
					{
						if(!baseTypes.Contains(baseType))
						{
							baseTypes.Add(baseType);
						}
					}
				}
				for(vint i=oldCount;i<baseTypes.Count();i++)
				{
					CollectBaseTypes(baseTypes[i], baseTypes, argument);
				}
			}

			void EnsureTypeVisibilityInternal(ManagedLanguageElement* languageElement, ManagedTypeSymbol* type, ManagedSymbolItem* scopeItem, const MAP& argument)
			{
				CHECK_ERROR(
					!scopeItem
					|| scopeItem->GetSymbolType()==ManagedSymbolItem::Class
					|| scopeItem->GetSymbolType()==ManagedSymbolItem::Structure
					|| scopeItem->GetSymbolType()==ManagedSymbolItem::Interface,
					L"EnsureTypeVisibility(ManagedLanguageElement*, ManagedTypeSymbol*, ManagedSymbolItem*, const MAP&)#scopeItemÄÚÈÝ·Ç·¨¡£"
					);

				List<ManagedTypeSymbol*> thisTypes, baseTypes;
				{
					ManagedSymbolDeclaration* currentDeclaration=dynamic_cast<ManagedSymbolDeclaration*>(scopeItem);
					while(currentDeclaration)
					{
						thisTypes.Add(argument.symbolManager->GetThisType(currentDeclaration));
						currentDeclaration=dynamic_cast<ManagedSymbolDeclaration*>(currentDeclaration->GetParentItem());
					}
				}
				FOREACH(ManagedTypeSymbol*, thisType, thisTypes.Wrap())
				{
					CollectBaseTypes(thisType, baseTypes, argument);
				}
				EnsureTypeVisibilityInternal(languageElement, type, argument, thisTypes, baseTypes);
			}

/***********************************************************************
GetOrderedGenericParameterNames
***********************************************************************/

			List<WString>* GetOrderedGenericParameterNames(ManagedSymbolItem* declSymbol)
			{
				switch(declSymbol->GetSymbolType())
				{
				case ManagedSymbolItem::Class:
				case ManagedSymbolItem::Structure:
				case ManagedSymbolItem::Interface:
					{
						ManagedSymbolDeclaration* symbol=dynamic_cast<ManagedSymbolDeclaration*>(declSymbol);
						return &symbol->orderedGenericParameterNames;
					}
					break;
				case ManagedSymbolItem::TypeRename:
					{
						ManagedSymbolTypeRename* symbol=dynamic_cast<ManagedSymbolTypeRename*>(declSymbol);
						return &symbol->orderedGenericParameterNames;
					}
					break;
				default:
					return 0;
				}
			}

/***********************************************************************
GetSymbolDeclaration
***********************************************************************/

			ManagedSymbolItem* GetRealSymbol(ManagedSymbolItem* declSymbol)
			{
				while(true)
				{
					switch(declSymbol->GetSymbolType())
					{
					case ManagedSymbolItem::Class:
					case ManagedSymbolItem::Structure:
					case ManagedSymbolItem::Interface:
					case ManagedSymbolItem::GenericParameter:
						return declSymbol;
					case ManagedSymbolItem::TypeRename:
						declSymbol=dynamic_cast<ManagedSymbolTypeRename*>(declSymbol)->type->GetSymbol();
						break;
					default:
						return 0;
					}
				}
			}

/***********************************************************************
IsInheritedFrom
***********************************************************************/

			bool IsInheritedFrom(ManagedSymbolItem* realSymbol, ManagedSymbolItem* parentSymbol)
			{
				switch(realSymbol->GetSymbolType())
				{
				case ManagedSymbolItem::Class:
				case ManagedSymbolItem::Structure:
				case ManagedSymbolItem::Interface:
					{
						ManagedSymbolDeclaration* symbol=dynamic_cast<ManagedSymbolDeclaration*>(realSymbol);
						FOREACH(ManagedTypeSymbol*, type, symbol->baseTypes.Wrap())
						{
							ManagedSymbolItem* typeSymbol=type->GetSymbol();
							if(typeSymbol==parentSymbol || IsInheritedFrom(typeSymbol, parentSymbol))
							{
								return true;
							}
						}
						return false;
					}
					break;
				case ManagedSymbolItem::GenericParameter:
					{
						ManagedSymbolGenericParameter* symbol=dynamic_cast<ManagedSymbolGenericParameter*>(realSymbol);
						FOREACH(ManagedTypeSymbol*, type, symbol->typeConstraints.Wrap())
						{
							ManagedSymbolItem* typeSymbol=type->GetSymbol();
							if(typeSymbol==parentSymbol || IsInheritedFrom(typeSymbol, parentSymbol))
							{
								return true;
							}
						}
						return false;
					}
					break;
				default:
					return false;
				}
			}

			bool IsInheritedFrom(ManagedTypeSymbol* type, ManagedTypeSymbol* parentType, const MAP& argument)
			{
				if(type==parentType)
				{
					return true;
				}
				else
				{
					ManagedSymbolItem* realSymbol=GetRealSymbol(type->GetSymbol());
					switch(realSymbol->GetSymbolType())
					{
					case ManagedSymbolItem::Class:
					case ManagedSymbolItem::Structure:
					case ManagedSymbolItem::Interface:
						{
							ManagedSymbolDeclaration* symbol=dynamic_cast<ManagedSymbolDeclaration*>(realSymbol);
							if(type->GetGenericDeclaration())
							{
								if(symbol->orderedGenericParameterNames.Count()==type->GetGenericArguments().Count())
								{
									Dictionary<ManagedTypeSymbol*, ManagedTypeSymbol*> replacement;
									for(vint i=0;i<symbol->orderedGenericParameterNames.Count();i++)
									{
										ManagedTypeSymbol* key=argument.symbolManager->GetType(symbol->ItemGroup(symbol->orderedGenericParameterNames[i])->Items()[0]);
										ManagedTypeSymbol* value=type->GetGenericArguments()[i];
										replacement.Add(key, value);
									}
									FOREACH(ManagedTypeSymbol*, baseType, symbol->baseTypes.Wrap())
									{
										if(IsInheritedFrom(argument.symbolManager->ReplaceGenericArguments(baseType, replacement.Wrap()), parentType, argument))
										{
											return true;
										}
									}
									return false;
								}
								else
								{
									return false;
								}
							}
							else
							{
								if(symbol->orderedGenericParameterNames.Count()==0)
								{
									FOREACH(ManagedTypeSymbol*, baseType, symbol->baseTypes.Wrap())
									{
										if(IsInheritedFrom(baseType, parentType, argument))
										{
											return true;
										}
									}
									return false;
								}
								else
								{
									return false;
								}
							}
						}
						break;
					case ManagedSymbolItem::GenericParameter:
						{
							ManagedSymbolGenericParameter* symbol=dynamic_cast<ManagedSymbolGenericParameter*>(realSymbol);
							FOREACH(ManagedTypeSymbol*, typeConstraint, symbol->typeConstraints.Wrap())
							{
								if(IsInheritedFrom(typeConstraint, parentType, argument))
								{
									return true;
								}
							}
							return false;
						}
						break;
					default:
						return false;
					}
				}
			}

/***********************************************************************
EnsureTypeSatisfiesConstraintsInternal
***********************************************************************/

			void EnsureTypeSatisfiesConstraintsInternal(ManagedLanguageElement* languageElement, ManagedTypeSymbol* type, const MAP& argument)
			{
				ManagedTypeSymbol* decl=type->GetGenericDeclaration();
				if(decl)
				{
					if(decl->GetParentType())
					{
						EnsureTypeSatisfiesConstraintsInternal(languageElement, decl->GetParentType(), argument);
					}
					ManagedSymbolItem* declSymbol=decl->GetSymbol();
					List<WString>& orderedGenericParameterNames=*GetOrderedGenericParameterNames(declSymbol);

					if(orderedGenericParameterNames.Count()!=type->GetGenericArguments().Count())
					{
						argument.errors.Add(ManagedLanguageCodeException::GetGenericTypeArgumentCountNotMatches(languageElement, type));
					}
					for(int i=0;i<type->GetGenericArguments().Count();i++)
					{
						ManagedTypeSymbol* genericArgument=type->GetGenericArguments()[i];
						EnsureTypeSatisfiesConstraintsInternal(languageElement, genericArgument, argument);

						if(i<orderedGenericParameterNames.Count())
						{
							ManagedSymbolGenericParameter* genericParameter=dynamic_cast<ManagedSymbolGenericParameter*>(declSymbol->ItemGroup(orderedGenericParameterNames[i])->Items()[0]);
							FOREACH(ManagedTypeSymbol*, typeConstraint, genericParameter->typeConstraints.Wrap())
							{
								if(!IsInheritedFrom(genericArgument, typeConstraint, argument))
								{
									argument.errors.Add(ManagedLanguageCodeException::GetGenericTypeNewConstraintNotSatisfied(languageElement, type, i));
									break;
								}
							}
							if(genericParameter->newConstraint)
							{
								ManagedSymbolItem* genericArgumentSymbol=GetRealSymbol(genericArgument->GetSymbol());
								switch(genericArgumentSymbol->GetSymbolType())
								{
								case ManagedSymbolItem::Class:
								case ManagedSymbolItem::Structure:
								case ManagedSymbolItem::Interface:
									{
										ManagedSymbolDeclaration* symbol=dynamic_cast<ManagedSymbolDeclaration*>(genericArgumentSymbol);
										ManagedSymbolItemGroup* constructors=symbol->ItemGroup(ManagedSymbolConstructor::SymbolName);
										bool find=false;
										FOREACH(ManagedSymbolItem*, constructor, constructors->Items())
										{
											if(dynamic_cast<ManagedSymbolConstructor*>(constructor)->accessor==declatt::Public)
											{
												find=true;
												break;
											}
										}
										if(!find)
										{
											argument.errors.Add(ManagedLanguageCodeException::GetGenericTypeNewConstraintNotSatisfied(languageElement, type, i));
										}
									}
									break;
								case ManagedSymbolItem::GenericParameter:
									{
										ManagedSymbolGenericParameter* symbol=dynamic_cast<ManagedSymbolGenericParameter*>(genericArgumentSymbol);
										if(!symbol->newConstraint)
										{
											argument.errors.Add(ManagedLanguageCodeException::GetGenericTypeNewConstraintNotSatisfied(languageElement, type, i));
										}
									}
									break;
								}
							}
						}
					}
				}
				else
				{
					List<WString>* orderedGenericParameterNames=GetOrderedGenericParameterNames(type->GetSymbol());
					if(orderedGenericParameterNames && orderedGenericParameterNames->Count()>0)
					{
						argument.errors.Add(ManagedLanguageCodeException::GetCannotUseUninstantiatedGenericType(languageElement, type));
					}
				}
				if(type->GetParentType())
				{
					EnsureTypeSatisfiesConstraintsInternal(languageElement, type->GetParentType(), argument);
				}
			}

/***********************************************************************
CheckType
***********************************************************************/

			void CheckType(ManagedLanguageElement* languageElement, ManagedTypeSymbol* type, ManagedSymbolItem* scopeItem, const MAP& argument)
			{
				EnsureTypeVisibilityInternal(languageElement, type, scopeItem, argument);
				EnsureTypeSatisfiesConstraintsInternal(languageElement, type, argument);
			}
		}
	}
}