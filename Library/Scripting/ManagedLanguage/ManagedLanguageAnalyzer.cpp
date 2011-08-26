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

			ManagedAnalyzerParameter::ManagedAnalyzerParameter(ManagedSymbolManager* _symbolManager, ManagedContextManager* _contextManager, ErrorList& _errors)
				:symbolManager(_symbolManager)
				,contextManager(_contextManager)
				,currentSymbol(_symbolManager->Global())
				,errors(_errors)
				,expectedType(0)
			{
			}

			ManagedAnalyzerParameter::ManagedAnalyzerParameter(const ManagedAnalyzerParameter& parameter, ManagedSymbolItem* _currentSymbol, ManagedTypeSymbol* _expectedType)
				:symbolManager(parameter.symbolManager)
				,contextManager(parameter.contextManager)
				,currentSymbol(_currentSymbol)
				,errors(parameter.errors)
				,expectedType(_expectedType)
			{
			}

/***********************************************************************
ManagedLanguage_AnalyzeProgram
***********************************************************************/

			void ManagedLanguage_AnalyzeProgram(Ptr<ManagedProgram> program, const MAP& argument)
			{
				typedef void (*ScopeBuilder)(Ptr<ManagedDeclaration>, MAP);
				ScopeBuilder builders[]=
				{
					&ManagedLanguage_BuildGlobalScope1_Declaration,
					&ManagedLanguage_BuildGlobalScope2_Declaration,
					&ManagedLanguage_BuildGlobalScope3_Declaration,
					0,
					&ManagedLanguage_BuildGlobalScope4_Declaration,
					&ManagedLanguage_BuildGlobalScope5_Declaration,
				};

				for(int i=0;i<sizeof(builders)/sizeof(*builders);i++)
				{
					ScopeBuilder builder=builders[i];
					if(builder)
					{
						FOREACH(Ptr<ManagedDeclaration>, declaration, program->declarations.Wrap())
						{
							builders[i](declaration, argument);
						}
					}
					else
					{
						InitializeContextManager(program.Obj(), argument);
					}
					if(argument.errors.Count()>0) return;
				}
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

			ManagedTypeSymbol* GetSystemType(ManagedLanguageElement* element, const WString& extraNamespace, const WString& name, const MAP& argument, vint genericParameterCount)
			{
				List<ManagedSymbolItem*> symbolItems;
				symbolItems.Add(argument.symbolManager->Global());
				vint start=0;
				vint end=symbolItems.Count();
				SearchType(symbolItems, start, end, L"System", 0);
				if(extraNamespace!=L"")
				{
					SearchType(symbolItems, start, end, extraNamespace, 0);
				}
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

			ManagedTypeSymbol* GetSystemType(ManagedLanguageElement* element, const WString& name, const MAP& argument, vint genericParameterCount)
			{
				return GetSystemType(element, L"", name, argument, genericParameterCount);
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
					if(symbol->typeLanguageElement)
					{
						bool containsNonInterface=false;
						if(symbol->typeLanguageElement->baseTypes.Count()>0)
						{
							FOREACH(Ptr<ManagedType>, type, symbol->typeLanguageElement->baseTypes.Wrap())
							{
								ManagedTypeSymbol* typeSymbol=GetTypeSymbol(type, argument, symbol);
								if(typeSymbol)
								{
									switch(GetRealSymbol(typeSymbol->GetSymbol())->GetSymbolType())
									{
									case ManagedSymbolItem::Class:
									case ManagedSymbolItem::Structure:
										containsNonInterface=true;
										break;
									}
									symbol->baseTypes.Add(typeSymbol);
								}
							}
						}
						if(symbol->GetSymbolType()!=ManagedSymbolItem::Interface && !containsNonInterface)
						{
							ManagedTypeSymbol* objectType=GetSystemType(symbol->typeLanguageElement, L"Object", argument);
							if(objectType->GetSymbol()!=symbol)
							{
								symbol->baseTypes.Add(objectType);
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

			ManagedTypeSymbol* GetRealType(ManagedTypeSymbol* typeRenameType, const MAP& argument)
			{
				if(typeRenameType->GetSymbol()->GetSymbolType()==ManagedSymbolItem::TypeRename)
				{
					ManagedSymbolTypeRename* symbol=dynamic_cast<ManagedSymbolTypeRename*>(typeRenameType->GetSymbol());
					if(!symbol->type)
					{
						MAP newArgument(argument, symbol->GetParentItem());
						ManagedLanguage_BuildGlobalScope2_ExtendedDeclaration(symbol->languageElement, newArgument);
					}

					if(symbol->type && typeRenameType->GetGenericDeclaration())
					{
						vint count=typeRenameType->GetGenericArguments().Count();
						if(symbol->orderedGenericParameterNames.Count()>0)
						{
							if(count>symbol->orderedGenericParameterNames.Count())
							{
								count=symbol->orderedGenericParameterNames.Count();
							}

							Dictionary<ManagedTypeSymbol*, ManagedTypeSymbol*> map;
							for(vint i=0;i<count;i++)
							{
								ManagedTypeSymbol* key=argument.symbolManager->GetType(symbol->ItemGroup(symbol->orderedGenericParameterNames[i])->Items()[0]);
								ManagedTypeSymbol* value=typeRenameType->GetGenericArguments()[i];
							}
							return argument.symbolManager->ReplaceGenericArguments(symbol->type, map.Wrap());
						}
						return 0;
					}
					else
					{
						return symbol->type;
					}
				}
				else
				{
					return typeRenameType;
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
EnsureTypeVisibilityOutSideOfAssemblyInternal
***********************************************************************/
			
			// -1==unspecified
			// 0==public
			// 1==protected
			// 2==private
			void GetAccessor(declatt::Accessor accessor, vint& publicLevel, vint& internalLevel)
			{
				switch(accessor)
				{
				case declatt::Public:
					if(publicLevel<0) publicLevel=0;
					break;
				case declatt::Protected:
					if(publicLevel<1) publicLevel=1;
					break;
				case declatt::Private:
					if(publicLevel<2) publicLevel=2;
					break;
				case declatt::Internal:
					if(internalLevel<0) internalLevel=0;
					break;
				case declatt::ProtectedInternal:
					if(internalLevel<1) internalLevel=1;
					break;
				}
			}

			void GetSymbolAccessor(ManagedSymbolItem* item, vint& publicLevel, vint& internalLevel)
			{
				switch(item->GetSymbolType())
				{
				case ManagedSymbolItem::TypeRename:
					GetAccessor(dynamic_cast<ManagedSymbolTypeRename*>(item)->accessor, publicLevel, internalLevel);
					break;
				case ManagedSymbolItem::Class:
				case ManagedSymbolItem::Structure:
				case ManagedSymbolItem::Interface:
					GetAccessor(dynamic_cast<ManagedSymbolDeclaration*>(item)->accessor, publicLevel, internalLevel);
					break;
				case ManagedSymbolItem::Field:
					GetAccessor(dynamic_cast<ManagedSymbolField*>(item)->accessor, publicLevel, internalLevel);
					break;
				case ManagedSymbolItem::Property:
					break;
				case ManagedSymbolItem::Method:
					GetAccessor(dynamic_cast<ManagedSymbolMethod*>(item)->accessor, publicLevel, internalLevel);
					break;
				case ManagedSymbolItem::Constructor:
					GetAccessor(dynamic_cast<ManagedSymbolConstructor*>(item)->accessor, publicLevel, internalLevel);
					break;
				case ManagedSymbolItem::ConverterOperator:
					GetAccessor(dynamic_cast<ManagedSymbolConverterOperator*>(item)->accessor, publicLevel, internalLevel);
					break;
				default:
					GetAccessor(declatt::Public, publicLevel, internalLevel);
					break;
				}
			}

			void GetSymbolAccessorRecursively(ManagedSymbolItem* item, vint& publicLevel, vint& internalLevel)
			{
				ManagedSymbolItem* currentItem=item;
				while(currentItem)
				{
					GetSymbolAccessor(currentItem, publicLevel, internalLevel);
					currentItem=currentItem->GetParentItem();
				}
				if(internalLevel==-1)
				{
					internalLevel=publicLevel;
				}
			}

			void EnsureTypeVisibilityOutSideOfAssemblyInternal(ManagedLanguageElement* languageElement, ManagedTypeSymbol* type, vint publicLevel, vint internalLevel, const MAP& argument)
			{
				if(type->GetGenericDeclaration())
				{
					FOREACH(ManagedTypeSymbol*, genericArgument, type->GetGenericArguments())
					{
						EnsureTypeVisibilityOutSideOfAssemblyInternal(languageElement, genericArgument, publicLevel, internalLevel, argument);
					}
					EnsureTypeVisibilityOutSideOfAssemblyInternal(languageElement, type->GetGenericDeclaration(), publicLevel, internalLevel, argument);
				}
				else
				{
					vint typePublicLevel=-1;
					vint typeInternalLevel=-1;
					GetSymbolAccessorRecursively(type->GetSymbol(), typePublicLevel, typeInternalLevel);
					if(publicLevel<typePublicLevel || internalLevel<typeInternalLevel)
					{
						argument.errors.Add(ManagedLanguageCodeException::GetTypeInvisibleOutSideOfAssembly(languageElement, type));
					}
				}
				if(type->GetParentType())
				{
					EnsureTypeVisibilityOutSideOfAssemblyInternal(languageElement, type->GetParentType(), publicLevel, internalLevel, argument);
				}
			}

			void EnsureTypeVisibilityOutSideOfAssemblyInternal(ManagedLanguageElement* languageElement, ManagedTypeSymbol* type, ManagedSymbolItem* memberItem, const MAP& argument)
			{
				vint publicLevel=-1;
				vint internalLevel=-1;
				GetSymbolAccessorRecursively(memberItem, publicLevel, internalLevel);
				EnsureTypeVisibilityOutSideOfAssemblyInternal(languageElement, type, publicLevel, internalLevel, argument);
			}

/***********************************************************************
CheckType
***********************************************************************/

			void CheckType(ManagedLanguageElement* languageElement, ManagedTypeSymbol* type, ManagedSymbolItem* scopeItem, ManagedSymbolItem* memberItem, const MAP& argument)
			{
				EnsureTypeVisibilityInternal(languageElement, type, scopeItem, argument);
				EnsureTypeSatisfiesConstraintsInternal(languageElement, type, argument);
				EnsureTypeVisibilityOutSideOfAssemblyInternal(languageElement, type, memberItem, argument);
			}

/***********************************************************************
GetTypeSymbolInMethod
***********************************************************************/

			ManagedSymbolItem* FindScopeItemInMethod(const MAP& argument)
			{
				ManagedSymbolItem* symbol=argument.currentSymbol;
				while(true)
				{
					switch(symbol->GetSymbolType())
					{
					case ManagedSymbolItem::Class:
					case ManagedSymbolItem::Structure:
					case ManagedSymbolItem::Interface:
						return symbol;
					default:
						symbol=symbol->GetParentItem();
					}
				}
			}

			void CheckTypeInMethod(ManagedLanguageElement* languageElement, ManagedTypeSymbol* typeSymbol, const MAP& argument)
			{
				EnsureTypeVisibilityInternal(languageElement, typeSymbol, FindScopeItemInMethod(argument) ,argument);
				EnsureTypeSatisfiesConstraintsInternal(languageElement, typeSymbol, argument);
			}

			ManagedTypeSymbol* GetTypeSymbolInMethod(Ptr<ManagedType> type, const MAP& argument)
			{
				ManagedTypeSymbol* typeSymbol=GetTypeSymbol(type, argument);
				CheckTypeInMethod(type.Obj(), typeSymbol, argument);
				return typeSymbol;
			}
		}
	}
}