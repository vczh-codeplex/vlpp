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
				ManagedSymbolItem*				extraGenericParameterContainer;

				ManagedAnalyzerGetTypeSymbolParameter(const MAP& _context, List<ManagedSymbolItem*>& _namespaceResults, List<ManagedTypeSymbol*>& _typeResults, ManagedSymbolItem* _extraGenericParameterContainer)
					:context(_context)
					,namespaceResults(_namespaceResults)
					,typeResults(_typeResults)
					,extraGenericParameterContainer(_extraGenericParameterContainer)
				{
				}
			};
			typedef ManagedAnalyzerGetTypeSymbolParameter MAGTSP;

			EXTERN_ALGORITHM_PROCEDURE(ManagedLanguage_GetTypeSymbol_Type, ManagedType, MAGTSP)
			EXTERN_ALGORITHM_PROCEDURE(ManagedLanguage_GetTypeSymbol_ExtendedType, ManagedExtendedType, MAGTSP)

/***********************************************************************
Helper Functions
***********************************************************************/

			void SearchMemberOfNamespaces(const MAGTSP& argument, const WString& member, List<ManagedSymbolItem*>& newNamespaceResults, List<ManagedTypeSymbol*>& newTypeResults)
			{
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
			}

			void SearchMemberOfType(ManagedTypeSymbol* containerType, const MAP& argument, const WString& member, List<ManagedTypeSymbol*>& newTypeResults)
			{
				while(containerType && containerType->GetSymbol()->GetSymbolType()==ManagedSymbolItem::TypeRename)
				{
					containerType=GetRealType(containerType, argument);
				}
				if(containerType)
				{
					switch(containerType->GetSymbol()->GetSymbolType())
					{
					case ManagedSymbolItem::Class:
					case ManagedSymbolItem::Structure:
					case ManagedSymbolItem::Interface:
						{
							bool added=false;
							if(ManagedSymbolItemGroup* group=containerType->GetSymbol()->ItemGroup(member))
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
											ManagedTypeSymbol* type=argument.symbolManager->GetType(item, containerType);
											if(!newTypeResults.Contains(type))
											{
												newTypeResults.Add(type);
												added=true;
											}
										}
										break;
									}
								}
							}
							if(!added)
							{
								FOREACH(ManagedTypeSymbol*, baseType, dynamic_cast<ManagedSymbolDeclaration*>(containerType->GetSymbol())->baseTypes.Wrap())
								{
									SearchMemberOfType(argument.symbolManager->GetBaseType(baseType, containerType), argument, member, newTypeResults);
								}
							}
						}
						break;
					case ManagedSymbolItem::GenericParameter:
						{
							// TODO: 
						}
						break;
					}
				}
			}

			void SearchMemberOfTypes(const MAGTSP& argument, const WString& member, List<ManagedTypeSymbol*>& newTypeResults)
			{
				FOREACH(ManagedTypeSymbol*, typeResult, argument.typeResults.Wrap())
				{
					SearchMemberOfType(typeResult, argument.context, member, newTypeResults);
				}
			}

			void SearchMember(const MAGTSP& argument, const WString& member, List<ManagedSymbolItem*>& newNamespaceResults, List<ManagedTypeSymbol*>& newTypeResults)
			{
				SearchMemberOfNamespaces(argument, member, newNamespaceResults, newTypeResults);
				SearchMemberOfTypes(argument, member, newTypeResults);
			}

/***********************************************************************
ManagedLanguage_GetTypeSymbol_Type
***********************************************************************/

			BEGIN_ALGORITHM_PROCEDURE(ManagedLanguage_GetTypeSymbol_Type, ManagedType, MAGTSP)

				ALGORITHM_PROCEDURE_MATCH(ManagedReferencedType)
				{
					argument.namespaceResults.Clear();
					argument.typeResults.Clear();
					if(argument.extraGenericParameterContainer)
					{
						if(ManagedSymbolItemGroup* itemGroup=argument.extraGenericParameterContainer->ItemGroup(node->name))
						{
							FOREACH(ManagedSymbolItem*, item, itemGroup->Items())
							{
								if(item->GetSymbolType()==ManagedSymbolItem::GenericParameter)
								{
									argument.typeResults.Add(argument.context.symbolManager->GetType(item));
								}
							}
						}
					}

					if(argument.namespaceResults.Count()+argument.typeResults.Count()==0)
					{
						List<ManagedSymbolItem*> newNamespaceResults;
						List<ManagedTypeSymbol*> newTypeResults;

						ManagedSymbolItem* currentSymbol=argument.context.currentSymbol;
						while(currentSymbol)
						{
							argument.namespaceResults.Clear();
							argument.typeResults.Clear();

							switch(currentSymbol->GetSymbolType())
							{
							case ManagedSymbolItem::Namespace:
							case ManagedSymbolItem::Global:
								{
									if(!argument.namespaceResults.Contains(currentSymbol))
									{
										argument.namespaceResults.Add(currentSymbol);
									}
								}
								break;
							case ManagedSymbolItem::Class:
							case ManagedSymbolItem::Structure:
							case ManagedSymbolItem::Interface:
								{
									if(ManagedSymbolItemGroup* group=currentSymbol->ItemGroup(node->name))
									{
										FOREACH(ManagedSymbolItem*, item, group->Items())
										{
											if(item->GetSymbolType()==ManagedSymbolItem::GenericParameter)
											{
												argument.typeResults.Add(argument.context.symbolManager->GetType(item));
												return;
											}
										}
									}
									FOREACH(ManagedSymbolNamespace*, item, dynamic_cast<ManagedSymbolDeclaration*>(currentSymbol)->_availableUsingNamespaces.Wrap())
									{
										if(!argument.namespaceResults.Contains(item))
										{
											argument.namespaceResults.Add(item);
										}
									}
									argument.typeResults.Add(GetTypeFromInsideScope(currentSymbol, argument.context));
								}
								break;
							}

							SearchMember(argument, node->name, newNamespaceResults, newTypeResults);
							if(newNamespaceResults.Count()+newTypeResults.Count()>0)
							{
								CopyFrom(argument.namespaceResults.Wrap(), newNamespaceResults.Wrap());
								CopyFrom(argument.typeResults.Wrap(), newTypeResults.Wrap());
								return;
							}
							currentSymbol=currentSymbol->GetParentItem();
						}
						argument.namespaceResults.Clear();
						argument.typeResults.Clear();
					}
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
						ManagedTypeSymbol* genericArgument=GetTypeSymbol(argumentType, argument.context, argument.extraGenericParameterContainer);
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
					ManagedTypeSymbol* element=GetTypeSymbol(node->elementType, argument.context, argument.extraGenericParameterContainer);
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
						ManagedTypeSymbol* parameterType=GetTypeSymbol(parameter, argument.context, argument.extraGenericParameterContainer);
						if(parameterType)
						{
							genericArguments.Add(parameterType);
						}
					}
					ManagedTypeSymbol* returnType=GetTypeSymbol(node->returnType, argument.context, argument.extraGenericParameterContainer);
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
						ManagedTypeSymbol* parameterType=GetTypeSymbol(parameter, argument.context, argument.extraGenericParameterContainer);
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
					ManagedTypeSymbol* dynamicDecl=GetSystemType(node, L"DynamicProxy", argument.context);
					if(dynamicDecl)
					{
						argument.typeResults.Add(dynamicDecl);
					}
				}

			END_ALGORITHM_PROCEDURE(ManagedLanguage_GetTypeSymbol_ExtendedType)

/***********************************************************************
GetTypeSymbol
***********************************************************************/

			ManagedTypeSymbol* GetTypeSymbol(Ptr<ManagedType> type, const MAP& argument, ManagedSymbolItem* extraGenericParameterContainer)
			{
				List<ManagedSymbolItem*> namespaceResults;
				List<ManagedTypeSymbol*> typeResults;
				ManagedLanguage_GetTypeSymbol_Type(type, MAGTSP(argument, namespaceResults, typeResults, extraGenericParameterContainer));

				List<ManagedTypeSymbol*> acceptableTypeResults;
				FOREACH(ManagedTypeSymbol*, typeResult, typeResults.Wrap())
				{
					if(typeResult->GetGenericDeclaration()==0)
					{
						switch(typeResult->GetSymbol()->GetSymbolType())
						{
							case ManagedSymbolItem::Class:
							case ManagedSymbolItem::Structure:
							case ManagedSymbolItem::Interface:
								{
									if(dynamic_cast<ManagedSymbolDeclaration*>(typeResult->GetSymbol())->orderedGenericParameterNames.Count()==0)
									{
										acceptableTypeResults.Add(typeResult);
									}
								}
								break;
							case ManagedSymbolItem::TypeRename:
								{
									if(dynamic_cast<ManagedSymbolTypeRename*>(typeResult->GetSymbol())->orderedGenericParameterNames.Count()==0)
									{
										acceptableTypeResults.Add(typeResult);
									}
								}
								break;
							case ManagedSymbolItem::GenericParameter:
								{
									acceptableTypeResults.Add(typeResult);
								}
								break;
						}
					}
					else
					{
						acceptableTypeResults.Add(typeResult);
					}
				}
				if(acceptableTypeResults.Count()==0)
				{
					argument.errors.Add(ManagedLanguageCodeException::GetTypeNotExists(type.Obj()));
					return 0;
				}
				else if(acceptableTypeResults.Count()>1)
				{
					argument.errors.Add(ManagedLanguageCodeException::GetTypeDuplicated(type.Obj()));
					return 0;
				}
				else
				{
					return acceptableTypeResults[0];
				}
			}
		}
	}
}