#include "ManagedLanguageAnalyzer.h"
#include "..\..\Collections\OperationForEach.h"
#include "..\..\Collections\OperationCopyFrom.h"

namespace vl
{
	namespace scripting
	{
		namespace managedlanguage
		{
			using namespace collections;

			class ManagedAnalyzerGetTypeSymbolParameter : public Object
			{
			public:
				const MAP&						context;
				List<ManagedSymbolItem*>&		namespaceResults;
				List<ManagedTypeSymbol*>&		typeResults;

				ManagedAnalyzerGetTypeSymbolParameter(const MAP& _context, List<ManagedSymbolItem*>& _namespaceResults, List<ManagedTypeSymbol*>& _typeResults)
					:context(_context)
					,namespaceResults(_namespaceResults)
					,typeResults(_typeResults)
				{
				}
			};
			typedef ManagedAnalyzerGetTypeSymbolParameter MAGTSP;

			EXTERN_ALGORITHM_PROCEDURE(ManagedLanguage_GetTypeSymbol_Type, ManagedType, MAGTSP)
			EXTERN_ALGORITHM_PROCEDURE(ManagedLanguage_GetTypeSymbol_ExtendedType, ManagedExtendedType, MAGTSP)

/***********************************************************************
Helper Functions
***********************************************************************/

			void SearchMember(const MAGTSP& argument, const WString& member, List<ManagedSymbolItem*>& newNamespaceResults, List<ManagedTypeSymbol*>& newTypeResults)
			{
				// consider private
				FOREACH(ManagedSymbolItem*, namespaceResult, argument.namespaceResults.Wrap())
				{
					if(ManagedSymbolItemGroup* group=namespaceResult->ItemGroup(member))
					{
						FOREACH(ManagedSymbolItem*, item, group->Items())
						{
							switch(item->GetSymbolType())
							{
							case ManagedSymbolItem::Namespace:
								{
									if(!newNamespaceResults.Contains(item))
									{
										newNamespaceResults.Add(item);
									}
								}
								break;
							case ManagedSymbolItem::Class:
							case ManagedSymbolItem::Structure:
							case ManagedSymbolItem::Interface:
							case ManagedSymbolItem::TypeRename:
								{
									ManagedTypeSymbol* type=argument.context.symbolManager->GetType(item);
									if(!newTypeResults.Contains(type))
									{
										newTypeResults.Add(type);
									}
								}
								break;
							}
						}
					}
				}

				FOREACH(ManagedTypeSymbol*, typeResult, argument.typeResults.Wrap())
				{
					bool failed=false;
					ManagedSymbolItem* typeResultSymbol=0;
					switch(typeResult->GetSymbol()->GetSymbolType())
					{
					case ManagedSymbolItem::Class:
					case ManagedSymbolItem::Structure:
					case ManagedSymbolItem::Interface:
						{
							typeResultSymbol=typeResult->GetSymbol();
						}
						break;
					case ManagedSymbolItem::TypeRename:
						{
							ManagedSymbolTypeRename* symbol=dynamic_cast<ManagedSymbolTypeRename*>(typeResult->GetSymbol());
							ManagedTypeSymbol* targetType=
								symbol->type
								?symbol->type
								:symbol->languageElement
									?GetTypeSymbol(symbol->languageElement->type, MAP(argument.context, symbol))
									:0
								;
							if(!targetType || targetType->GetSymbol()->GetSymbolType()==ManagedSymbolItem::TypeRename)
							{
								failed=true;
							}
							else
							{
								typeResultSymbol=targetType->GetSymbol();
							}
						}
						break;
					default:
						failed=true;
						break;
					}

					if(!failed)
					{
						if(ManagedSymbolItemGroup* group=typeResultSymbol->ItemGroup(member))
						{
							FOREACH(ManagedSymbolItem*, item, group->Items())
							{
								switch(item->GetSymbolType())
								{
								case ManagedSymbolItem::Class:
								case ManagedSymbolItem::Structure:
								case ManagedSymbolItem::Interface:
								case ManagedSymbolItem::TypeRename:
									{
										ManagedTypeSymbol* type=argument.context.symbolManager->GetType(item, typeResult);
										if(!newTypeResults.Contains(type))
										{
											newTypeResults.Add(type);
										}
									}
									break;
								}
							}
						}
					}
				}
			}

/***********************************************************************
ManagedLanguage_GetTypeSymbol_Type
***********************************************************************/

			BEGIN_ALGORITHM_PROCEDURE(ManagedLanguage_GetTypeSymbol_Type, ManagedType, MAGTSP)

				ALGORITHM_PROCEDURE_MATCH(ManagedReferencedType)
				{
					// consider private
					{
						ManagedSymbolItem* currentSymbol=argument.context.currentSymbol;
						while(currentSymbol)
						{
							switch(currentSymbol->GetSymbolType())
							{
							case ManagedSymbolItem::Namespace:
							case ManagedSymbolItem::Global:
								{
									if(!argument.namespaceResults.Contains(currentSymbol))
									{
										argument.namespaceResults.Add(currentSymbol);
									}
									if(ManagedSymbolItemGroup* group=currentSymbol->ItemGroup(ManagedSymbolUsingNamespace::SymbolName))
									{
										FOREACH(ManagedSymbolItem*, item, group->Items())
										{
											ManagedSymbolUsingNamespace* symbol=dynamic_cast<ManagedSymbolUsingNamespace*>(item);
											EnsureUsingNamespaceSymbolCompleted(symbol, argument.context);
											ManagedSymbolItem* namespaceReference=symbol->associatedNamespace;
											if(namespaceReference && !argument.namespaceResults.Contains(namespaceReference))
											{
												argument.namespaceResults.Add(namespaceReference);
											}
										}
									}
								}
								break;
							default:
								{
									argument.typeResults.Add(argument.context.symbolManager->GetType(currentSymbol));
								}
								break;
							}
							currentSymbol=currentSymbol->GetParentItem();
						}
					}
					{
						vint start=0;
						vint end=argument.typeResults.Count();
						while(start<end)
						{
							for(vint i=start;i<end;i++)
							{
								ManagedTypeSymbol* type=argument.typeResults[i];
								ManagedSymbolDeclaration* symbol=dynamic_cast<ManagedSymbolDeclaration*>(type->GetSymbol());
								EnsureSymbolBaseTypesCompleted(symbol, MAP(argument.context, type->GetSymbol()->GetParentItem()));
								FOREACH(ManagedTypeSymbol*, baseType, symbol->baseTypes.Wrap())
								{
									if(!argument.typeResults.Add(baseType))
									{
										argument.typeResults.Add(baseType);
									}
								}
							}
							start=end;
							end=argument.typeResults.Count();
						}
					}
					
					List<ManagedSymbolItem*> newNamespaceResults;
					List<ManagedTypeSymbol*> newTypeResults;
					SearchMember(argument, node->name, newNamespaceResults, newTypeResults);

					argument.namespaceResults.Clear();
					argument.typeResults.Clear();
					CopyFrom(argument.namespaceResults.Wrap(), newNamespaceResults.Wrap());
					CopyFrom(argument.typeResults.Wrap(), newTypeResults.Wrap());
				}

				ALGORITHM_PROCEDURE_MATCH(ManagedMemberType)
				{
					List<ManagedSymbolItem*> newNamespaceResults;
					List<ManagedTypeSymbol*> newTypeResults;
					if(node->operand)
					{
						ManagedLanguage_GetTypeSymbol_Type(node->operand, argument);
					}
					else
					{
						argument.namespaceResults.Add(argument.context.symbolManager->Global());
					}
					SearchMember(argument, node->member, newNamespaceResults, newTypeResults);

					argument.namespaceResults.Clear();
					argument.typeResults.Clear();
					CopyFrom(argument.namespaceResults.Wrap(), newNamespaceResults.Wrap());
					CopyFrom(argument.typeResults.Wrap(), newTypeResults.Wrap());
				}

				ALGORITHM_PROCEDURE_MATCH(ManagedInstantiatedGenericType)
				{
					ManagedLanguage_GetTypeSymbol_Type(node->elementType, argument);
					List<ManagedTypeSymbol*> acceptableTypes;
					FOREACH(ManagedTypeSymbol*, resultType, argument.typeResults.Wrap())
					{
						if(resultType->GetGenericDeclaration()==0)
						{
							switch(resultType->GetSymbol()->GetSymbolType())
							{
							case ManagedSymbolItem::Class:
							case ManagedSymbolItem::Structure:
							case ManagedSymbolItem::Interface:
								{
									if(dynamic_cast<ManagedSymbolDeclaration*>(resultType->GetSymbol())->orderedGenericParameterNames.Count()==node->argumentTypes.Count())
									{
										acceptableTypes.Add(resultType);
									}
								}
								break;
							case ManagedSymbolItem::TypeRename:
								{
									if(dynamic_cast<ManagedSymbolTypeRename*>(resultType->GetSymbol())->orderedGenericParameterNames.Count()==node->argumentTypes.Count())
									{
										acceptableTypes.Add(resultType);
									}
								}
								break;
							}
						}
					}

					List<ManagedTypeSymbol*> genericArguments;
					FOREACH(Ptr<ManagedType>, argumentType, node->argumentTypes.Wrap())
					{
						ManagedTypeSymbol* genericArgument=GetTypeSymbol(argumentType, argument.context);
						if(genericArgument)
						{
							genericArguments.Add(genericArgument);
						}
					}

					argument.namespaceResults.Clear();
					argument.typeResults.Clear();
					if(genericArguments.Count()==node->argumentTypes.Count())
					{
						FOREACH(ManagedTypeSymbol*, acceptableType, acceptableTypes.Wrap())
						{
							argument.typeResults.Add(argument.context.symbolManager->GetInstiantiatedType(acceptableType, genericArguments.Wrap()));
						}
					}
				}

				ALGORITHM_PROCEDURE_MATCH(ManagedExtendedType)
				{
					ManagedLanguage_GetTypeSymbol_ExtendedType(node, argument);
				}

			END_ALGORITHM_PROCEDURE(ManagedLanguage_GetTypeSymbol_Type)

/***********************************************************************
ManagedLanguage_GetTypeSymbol_ExtendedType
***********************************************************************/

			BEGIN_ALGORITHM_PROCEDURE(ManagedLanguage_GetTypeSymbol_ExtendedType, ManagedExtendedType, MAGTSP)

				ALGORITHM_PROCEDURE_MATCH(ManagedArrayType)
				{
					ManagedTypeSymbol* arrayDecl=GetSystemType(node, L"Array"+itow(node->dimensionCount), argument.context, 1);
					ManagedTypeSymbol* element=GetTypeSymbol(node->elementType, argument.context);
					if(arrayDecl && element)
					{
						List<ManagedTypeSymbol*> genericArguments;
						genericArguments.Add(element);
						argument.typeResults.Add(argument.context.symbolManager->GetInstiantiatedType(arrayDecl, genericArguments.Wrap()));
					}
				}

				ALGORITHM_PROCEDURE_MATCH(ManagedFunctionType)
				{
					List<ManagedTypeSymbol*> genericArguments;
					FOREACH(Ptr<ManagedType>, parameter, node->parameterTypes.Wrap())
					{
						ManagedTypeSymbol* parameterType=GetTypeSymbol(parameter, argument.context);
						if(parameterType)
						{
							genericArguments.Add(parameterType);
						}
					}
					ManagedTypeSymbol* returnType=GetTypeSymbol(node->returnType, argument.context);
					ManagedTypeSymbol* voidType=GetSystemType(node, L"Void", argument.context);

					if(returnType && voidType && genericArguments.Count()==node->parameterTypes.Count())
					{
						if(returnType==voidType)
						{
							ManagedTypeSymbol* procedureDecl=GetSystemType(node, L"Procedure", argument.context, genericArguments.Count());
							if(procedureDecl)
							{
								if(genericArguments.Count()==0)
								{
									argument.typeResults.Add(procedureDecl);
								}
								else
								{
									argument.typeResults.Add(argument.context.symbolManager->GetInstiantiatedType(procedureDecl, genericArguments.Wrap()));
								}
							}
						}
						else
						{
							genericArguments.Insert(0, returnType);
							ManagedTypeSymbol* functionDecl=GetSystemType(node, L"Function", argument.context, genericArguments.Count());
							if(functionDecl)
							{
								argument.typeResults.Add(argument.context.symbolManager->GetInstiantiatedType(functionDecl, genericArguments.Wrap()));
							}
						}
					}
				}

				ALGORITHM_PROCEDURE_MATCH(ManagedEventType)
				{
					List<ManagedTypeSymbol*> genericArguments;
					FOREACH(Ptr<ManagedType>, parameter, node->parameterTypes.Wrap())
					{
						ManagedTypeSymbol* parameterType=GetTypeSymbol(parameter, argument.context);
						if(parameterType)
						{
							genericArguments.Add(parameterType);
						}
					}

					if(genericArguments.Count()==node->parameterTypes.Count())
					{
						ManagedTypeSymbol* eventDecl=GetSystemType(node, L"Event", argument.context, genericArguments.Count());
						if(eventDecl)
						{
							if(genericArguments.Count()==0)
							{
								argument.typeResults.Add(eventDecl);
							}
							else
							{
								argument.typeResults.Add(argument.context.symbolManager->GetInstiantiatedType(eventDecl, genericArguments.Wrap()));
							}
						}
					}
				}

				ALGORITHM_PROCEDURE_MATCH(ManagedAutoReferType)
				{
					argument.context.errors.Add(ManagedLanguageCodeException::GetIllegalAutoRefer(node));
				}

				ALGORITHM_PROCEDURE_MATCH(ManagedDynamicType)
				{
					ManagedTypeSymbol* dynamicDecl=GetSystemType(node, L"IDynamic", argument.context);
					if(dynamicDecl)
					{
						argument.typeResults.Add(dynamicDecl);
					}
				}

			END_ALGORITHM_PROCEDURE(ManagedLanguage_GetTypeSymbol_ExtendedType)

/***********************************************************************
GetTypeSymbol
***********************************************************************/

			ManagedTypeSymbol* GetTypeSymbol(Ptr<ManagedType> type, const MAP& argument)
			{
				List<ManagedSymbolItem*> namespaceResults;
				List<ManagedTypeSymbol*> typeResults;
				ManagedLanguage_GetTypeSymbol_Type(type, MAGTSP(argument, namespaceResults, typeResults));
				if(typeResults.Count()==0)
				{
					//argument.errors.Add(ManagedLanguageCodeException::GetTypeNotExists(type.Obj()));
					return 0;
				}
				else if(typeResults.Count()>1)
				{
					//argument.errors.Add(ManagedLanguageCodeException::GetTypeDuplicated(type.Obj()));
					return 0;
				}
				else
				{
					return typeResults[0];
				}
			}
		}
	}
}