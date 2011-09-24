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

			struct ExpressionChoice
			{
				ManagedTypeSymbol*					type;	// symbol type if exists. for method and generic parameter, it this the owner type's type.
				ManagedSymbolItem*					symbol;

				ExpressionChoice(ManagedTypeSymbol* _type=0, ManagedSymbolItem* _symbol=0)
					:type(_type)
					,symbol(_symbol)
				{
				}

				bool operator==(const ExpressionChoice& value){return false;}
				bool operator!=(const ExpressionChoice& value){return true;}
			};

			struct ExpressionResolvingResult
			{

				ManagedTypeSymbol*						expressionResult;
				ManagedSymbolNamespace*					namespaceResult;
				ManagedTypeSymbol*						typeResult;
				List<ExpressionChoice>					methodResults;

				ExpressionResolvingResult()
					:expressionResult(0)
					,namespaceResult(0)
					,typeResult(0)
				{
				}

				bool IsEmpty()
				{
					return expressionResult==0 && namespaceResult==0 && typeResult==0 && methodResults.Count()==0;
				}

				void Clear()
				{
					expressionResult=0;
					namespaceResult=0;
					typeResult=0;
					methodResults.Clear();
				}
			};

			class ManagedAnalyzerGetExpressionTypeParameter : public Object
			{
			public:
			public:
				const MAP&								context;
				ExpressionResolvingResult&				result;

				ManagedAnalyzerGetExpressionTypeParameter(const MAP& _context, ExpressionResolvingResult& _result)
					:context(_context)
					,result(_result)
				{
				}
			};
			typedef ManagedAnalyzerGetExpressionTypeParameter MAGETP;

			EXTERN_ALGORITHM_PROCEDURE(ManagedLanguage_GetTypeInternal_Expression, ManagedExpression, MAGETP)
			EXTERN_ALGORITHM_PROCEDURE(ManagedLanguage_GetTypeInternal_ExtendedExpression, ManagedExtendedExpression, MAGETP)

/***********************************************************************
GetType
***********************************************************************/

			bool CanImplicitlyConvertTo(ManagedTypeSymbol* from, ManagedTypeSymbol* to, const MAP& argument)
			{
				return from==to || IsInheritedFrom(from, to, argument);
			}

			bool IsValue(ManagedSymbolItem* symbol)
			{
				switch(symbol->GetSymbolType())
				{
				case ManagedSymbolItem::Variable:
				case ManagedSymbolItem::Lambda:
				case ManagedSymbolItem::LambdaParameter:
				case ManagedSymbolItem::Field:
				case ManagedSymbolItem::Property:
				case ManagedSymbolItem::PropertySetterValue:
				case ManagedSymbolItem::MethodParameter:
					return true;
				default:
					return false;
				}
			}

			ManagedTypeSymbol* GetType(ManagedExpression* node, ManagedTypeSymbol* expectedType, const MAP& argument)
			{
				ExpressionResolvingResult expressionResolvingResult;
				MAP a1(argument, argument.currentSymbol, expectedType);
				MAGETP a2(a1, expressionResolvingResult);
				ManagedLanguage_GetTypeInternal_Expression(node, a2);

				if(expressionResolvingResult.expressionResult)
				{
					if(expectedType)
					{
						CheckTypeInMethod(node, expressionResolvingResult.expressionResult, argument);
						if(!CanImplicitlyConvertTo(expressionResolvingResult.expressionResult, expectedType, argument))
						{
							argument.errors.Add(ManagedLanguageCodeException::GetExpressionCannotConvertToType(node, expectedType));
						}
					}
				}
				else if(!expressionResolvingResult.IsEmpty())
				{
					argument.errors.Add(ManagedLanguageCodeException::GetExpressionIsNotValue(node));
				}

				return expressionResolvingResult.expressionResult;
			}

/***********************************************************************
Helper Functions
***********************************************************************/

			bool IsInStaticMethod(const MAP& argument)
			{
				ManagedMember* target=argument.contextManager->GetThisTargetMember();
				if(target)
				{
					ManagedSymbolItem* targetSymbol=argument.symbolManager->GetSymbol(target);
					switch(targetSymbol->GetSymbolType())
					{
					case ManagedSymbolItem::Method:
						{
							ManagedSymbolMethod* method=dynamic_cast<ManagedSymbolMethod*>(targetSymbol);
							if(method->memberType==declatt::Instance)
							{
								return false;
							}
						}
						break;
					case ManagedSymbolItem::Constructor:
						return false;
					case ManagedSymbolItem::ConverterOperator:
						{
							ManagedSymbolConverterOperator* converterOperator=dynamic_cast<ManagedSymbolConverterOperator*>(targetSymbol);
							if(converterOperator->memberType==declatt::Instance)
							{
								return false;
							}
						}
						break;
					}
				}
				return true;
			}

			ManagedTypeSymbol* GetTypeFromInsideScope(ManagedSymbolItem* symbol, const MAP& argument)
			{
				List<ManagedSymbolDeclaration*> types;
				{
					while(symbol)
					{
						switch(symbol->GetSymbolType())
						{
						case ManagedSymbolItem::Class:
						case ManagedSymbolItem::Structure:
						case ManagedSymbolItem::Interface:
							types.Add(dynamic_cast<ManagedSymbolDeclaration*>(symbol));
							break;
						}
						symbol=symbol->GetParentItem();
					}
				}

				ManagedTypeSymbol* type=0;
				for(vint i=types.Count()-1;i>=0;i--)
				{
					ManagedSymbolDeclaration* declaration=types[i];
					ManagedTypeSymbol* declarationType=argument.symbolManager->GetType(declaration, type);
					if(declaration->orderedGenericParameterNames.Count()>0)
					{
						List<ManagedTypeSymbol*> genericArguments;
						FOREACH(WString, genericParameterName, declaration->orderedGenericParameterNames.Wrap())
						{
							ManagedSymbolItem* genericParameter=declaration->ItemGroup(genericParameterName)->Items()[0];
							ManagedTypeSymbol* genericParameterType=argument.symbolManager->GetType(genericParameter);
							genericArguments.Add(genericParameterType);
						}
						declarationType=argument.symbolManager->GetInstiantiatedType(declarationType, genericArguments.Wrap());
					}
					type=declarationType;
				}
				return type;
			}

			ManagedTypeSymbol* GetThisType(const MAP& argument)
			{
				ManagedSymbolItem* currentSymbol=argument.currentSymbol;
				ManagedSymbolItem* thisSymbol=0;
				while(currentSymbol && !thisSymbol)
				{
					switch(currentSymbol->GetSymbolType())
					{
					case ManagedSymbolItem::Class:
					case ManagedSymbolItem::Structure:
					case ManagedSymbolItem::Interface:
						thisSymbol=currentSymbol;
						break;
					}
					currentSymbol=currentSymbol->GetParentItem();
				}
				return thisSymbol==0?0:GetTypeFromInsideScope(thisSymbol, argument);
			}

			ManagedTypeSymbol* GetBaseType(ManagedTypeSymbol* type, const MAP& argument)
			{
				ManagedSymbolDeclaration* declaration=dynamic_cast<ManagedSymbolDeclaration*>(GetRealSymbol(type->GetSymbol()));
				ManagedTypeSymbol* firstBaseType=0;
				FOREACH(ManagedTypeSymbol*, baseType, declaration->baseTypes.Wrap())
				{
					if(firstBaseType) break;
					switch(GetRealSymbol(baseType->GetSymbol())->GetSymbolType())
					{
					case ManagedSymbolItem::Class:
					case ManagedSymbolItem::Structure:
						firstBaseType=baseType;
						break;
					}
				}

				if(firstBaseType)
				{
					return argument.symbolManager->GetBaseType(firstBaseType, type);
				}
				else
				{
					return 0;
				}
			}

/***********************************************************************
SearchMember
***********************************************************************/

			void SearchMemberOfNamespace(ManagedExpression* node, ManagedSymbolItem* namespaceItem, const WString& name, const MAGETP& argument)
			{
				if(ManagedSymbolItemGroup* group=namespaceItem->ItemGroup(name))
				{
					FOREACH(ManagedSymbolItem*, item, group->Items())
					{
						switch(item->GetSymbolType())
						{
						case ManagedSymbolItem::Class:
						case ManagedSymbolItem::Structure:
						case ManagedSymbolItem::Interface:
							{
								if(argument.result.typeResult)
								{
									argument.context.errors.Add(ManagedLanguageCodeException::GetExpressionResolvedToDuplicatedTargets(node, 0));
								}
								else
								{
									argument.result.typeResult=argument.context.symbolManager->GetType(item);
								}
							}
							break;
						case ManagedSymbolItem::Namespace:
							{
								if(argument.result.typeResult)
								{
									argument.result.Clear();
									argument.result.namespaceResult=dynamic_cast<ManagedSymbolNamespace*>(item);
								}
								else
								{
									argument.context.errors.Add(ManagedLanguageCodeException::GetExpressionResolvedToDuplicatedTargets(node, 0));
								}
							}
							return;
						}
					}
				}
			}

			bool IsVisible(bool staticOnly, declatt::MemberType memberType)
			{
				return memberType==declatt::Static || !staticOnly;
			}

			bool IsVisible(ManagedTypeSymbol* thisType, ManagedTypeSymbol* containerType, declatt::Accessor accessor, const MAP& argument)
			{
				if(thisType==containerType)
				{
					return true;
				}
				else if(IsInheritedFrom(thisType, containerType, argument))
				{
					return accessor!=declatt::Private;
				}
				else
				{
					return accessor!=declatt::Private && accessor!=declatt::Protected;
				}
			}

			void SearchMemberOfType(ManagedExpression* node, ManagedTypeSymbol* thisType, ManagedTypeSymbol* containerType, bool staticOnly, const WString& name, const MAGETP& argument, bool searchForMethodOnly=false, bool searchForOperator=false)
			{
				// TODO: Search methods in base types
				ManagedSymbolItem* containerScope=GetRealSymbol(containerType->GetSymbol());
				switch(containerScope->GetSymbolType())
				{
				case ManagedSymbolItem::GenericParameter:
					// TODO:
					throw 0;
					break;
				case ManagedSymbolItem::Class:
				case ManagedSymbolItem::Structure:
				case ManagedSymbolItem::Interface:
					if(ManagedSymbolItemGroup* group=containerScope->ItemGroup(name))
					{
						FOREACH(ManagedSymbolItem*, item, group->Items())
						{
							switch(item->GetSymbolType())
							{
							case ManagedSymbolItem::Field:
								if(!searchForMethodOnly)
								{
									ManagedSymbolField* symbol=dynamic_cast<ManagedSymbolField*>(item);
									if(IsVisible(staticOnly, symbol->memberType) && IsVisible(thisType, containerType, symbol->accessor, argument.context))
									{
										ManagedTypeSymbol* symbolType=argument.context.symbolManager->ReplaceGenericArguments(symbol->type, containerType);
										if(argument.result.IsEmpty())
										{
											argument.result.expressionResult=symbolType;
										}
										else
										{
											argument.context.errors.Add(ManagedLanguageCodeException::GetExpressionResolvedToDuplicatedTargets(node, 0));
										}
									}
								}
								break;
							case ManagedSymbolItem::Property:
								if(!searchForMethodOnly)
								{
									ManagedSymbolProperty* symbol=dynamic_cast<ManagedSymbolProperty*>(item);
									if(IsVisible(staticOnly, symbol->memberType) && IsVisible(thisType, containerType, symbol->accessor, argument.context) && !symbol->implementedInterfaceType)
									{
										ManagedTypeSymbol* symbolType=argument.context.symbolManager->ReplaceGenericArguments(symbol->type, containerType);
										if(argument.result.IsEmpty())
										{
											argument.result.expressionResult=symbolType;
										}
										else
										{
											argument.context.errors.Add(ManagedLanguageCodeException::GetExpressionResolvedToDuplicatedTargets(node, 0));
										}
									}
								}
								break;
							case ManagedSymbolItem::Method:
								{
									ManagedSymbolMethod* symbol=dynamic_cast<ManagedSymbolMethod*>(item);
									if(symbol->inheritation!=declatt::Override)
									{
										if(IsVisible(staticOnly, symbol->memberType) && IsVisible(thisType, containerType, symbol->accessor, argument.context) && !symbol->implementedInterfaceType)
										{
											if(!argument.result.IsEmpty() && argument.result.methodResults.Count()==0)
											{
												argument.context.errors.Add(ManagedLanguageCodeException::GetExpressionResolvedToDuplicatedTargets(node, 0));
											}
											else
											{
												argument.result.methodResults.Add(ExpressionChoice(containerType, item));
											}
										}
									}
								}
								break;
							case ManagedSymbolItem::Class:
							case ManagedSymbolItem::Structure:
							case ManagedSymbolItem::Interface:
								if(!searchForMethodOnly)
								{
									if(IsVisible(thisType, containerType, dynamic_cast<ManagedSymbolDeclaration*>(item)->accessor, argument.context))
									{
										ManagedTypeSymbol* symbolType=argument.context.symbolManager->GetType(item, containerType);
										if(argument.result.IsEmpty())
										{
											argument.result.typeResult=symbolType;
										}
										else
										{
											argument.context.errors.Add(ManagedLanguageCodeException::GetExpressionResolvedToDuplicatedTargets(node, 0));
										}
									}
								}
								break;
							case ManagedSymbolItem::TypeRename:
								if(!searchForMethodOnly)
								{
									ManagedSymbolTypeRename* symbol=dynamic_cast<ManagedSymbolTypeRename*>(item);
									if(IsVisible(thisType, containerType, symbol->accessor, argument.context))
									{
										// TODO: this is wrong
										ManagedTypeSymbol* symbolType=GetRealType(symbol->type, argument.context);
										if(argument.result.IsEmpty())
										{
											argument.result.typeResult=symbolType;
										}
										else
										{
											argument.context.errors.Add(ManagedLanguageCodeException::GetExpressionResolvedToDuplicatedTargets(node, 0));
										}
									}
								}
								break;
							}
						}
					}

					if(argument.result.IsEmpty() || (argument.result.methodResults.Count()>0 && !searchForOperator))
					{
						FOREACH(ManagedTypeSymbol*, baseType, dynamic_cast<ManagedSymbolDeclaration*>(containerScope)->baseTypes.Wrap())
						{
							if(containerScope->GetSymbolType()==ManagedSymbolItem::Interface || GetRealSymbol(baseType->GetSymbol())->GetSymbolType()!=ManagedSymbolItem::Interface)
							{
								SearchMemberOfType(node, thisType, argument.context.symbolManager->GetBaseType(baseType, containerType), staticOnly, name, argument, (argument.result.methodResults.Count()>0), searchForOperator);
							}
						}
					}
					break;
				}
			}

/***********************************************************************
ManagedLanguage_GetTypeInternal_Expression
***********************************************************************/

			BEGIN_ALGORITHM_PROCEDURE(ManagedLanguage_GetTypeInternal_Expression, ManagedExpression, MAGETP)

				ALGORITHM_PROCEDURE_MATCH(ManagedNullExpression)
				{
					if(argument.context.expectedType)
					{
						ManagedSymbolItem* symbol=GetRealSymbol(argument.context.expectedType->GetSymbol());
						switch(symbol->GetSymbolType())
						{
						case ManagedSymbolItem::Class:
						case ManagedSymbolItem::Interface:
							argument.result.expressionResult=argument.context.expectedType;
						}
					}
					if(!argument.result.expressionResult)
					{
						//argument.context.errors.Add(ManagedLanguageCodeException::GetIllegalNull(node));
					}
				}

				ALGORITHM_PROCEDURE_MATCH(ManagedIntegerExpression)
				{
					if(
						argument.context.expectedType==argument.context.contextManager->predefinedTypes.sint8 ||
						argument.context.expectedType==argument.context.contextManager->predefinedTypes.sint16 ||
						argument.context.expectedType==argument.context.contextManager->predefinedTypes.sint32 ||
						argument.context.expectedType==argument.context.contextManager->predefinedTypes.sint64)
					{
						argument.result.expressionResult=argument.context.expectedType;
					}
					else if(!node->sign && (
						argument.context.expectedType==argument.context.contextManager->predefinedTypes.uint8 ||
						argument.context.expectedType==argument.context.contextManager->predefinedTypes.uint16 ||
						argument.context.expectedType==argument.context.contextManager->predefinedTypes.uint32 ||
						argument.context.expectedType==argument.context.contextManager->predefinedTypes.uint64))
					{
						argument.result.expressionResult=argument.context.expectedType;
					}
					else if(
						argument.context.expectedType==argument.context.contextManager->predefinedTypes.singleType ||
						argument.context.expectedType==argument.context.contextManager->predefinedTypes.doubleType)
					{
						argument.result.expressionResult=argument.context.expectedType;
					}
					else
					{
						argument.result.expressionResult=argument.context.contextManager->predefinedTypes.sint32;
					}
				}

				ALGORITHM_PROCEDURE_MATCH(ManagedFloatExpression)
				{
					if(argument.context.expectedType==argument.context.contextManager->predefinedTypes.singleType)
					{
						argument.result.expressionResult=argument.context.contextManager->predefinedTypes.singleType;
					}
					else
					{
						argument.result.expressionResult=argument.context.contextManager->predefinedTypes.doubleType;
					}
				}

				ALGORITHM_PROCEDURE_MATCH(ManagedBooleanExpression)
				{
					argument.result.expressionResult=argument.context.contextManager->predefinedTypes.boolType;
				}

				ALGORITHM_PROCEDURE_MATCH(ManagedCharExpression)
				{
					argument.result.expressionResult=argument.context.contextManager->predefinedTypes.charType;
				}

				ALGORITHM_PROCEDURE_MATCH(ManagedStringExpression)
				{
					argument.result.expressionResult=argument.context.contextManager->predefinedTypes.stringType;
				}

				ALGORITHM_PROCEDURE_MATCH(ManagedReferenceExpression)
				{
					bool inStaticMethod=IsInStaticMethod(argument.context);
					ManagedSymbolItem* currentScope=argument.context.currentSymbol;
					ManagedTypeSymbol* thisType=GetThisType(argument.context);
					bool firstType=true;

					while(argument.result.IsEmpty() && currentScope!=0)
					{
						switch(currentScope->GetSymbolType())
						{
						case ManagedSymbolItem::Block:
						case ManagedSymbolItem::Lambda:
						case ManagedSymbolItem::Property:
						case ManagedSymbolItem::Method:
						case ManagedSymbolItem::Constructor:
						case ManagedSymbolItem::ConverterOperator:
							{
								if(ManagedSymbolItemGroup* group=currentScope->ItemGroup(node->name))
								{
									FOREACH(ManagedSymbolItem*, item, group->Items())
									{
										switch(item->GetSymbolType())
										{
										case ManagedSymbolItem::Variable:
											{
												ManagedSymbolVariable* symbol=dynamic_cast<ManagedSymbolVariable*>(item);
												argument.result.expressionResult=symbol->type;
											}
											return;
										case ManagedSymbolItem::LambdaParameter:
											{
												ManagedSymbolLambdaParameter* symbol=dynamic_cast<ManagedSymbolLambdaParameter*>(item);
												argument.result.expressionResult=symbol->type;
											}
											return;
										case ManagedSymbolItem::GenericParameter:
											{
												argument.result.typeResult=argument.context.symbolManager->GetType(item);
											}
											return;
										case ManagedSymbolItem::MethodParameter:
											{
												ManagedSymbolMethodParameter* symbol=dynamic_cast<ManagedSymbolMethodParameter*>(item);
												argument.result.expressionResult=symbol->type;
											}
											return;
										case ManagedSymbolItem::PropertySetterValue:
											{
												ManagedSymbolPropertySetterValue* symbol=dynamic_cast<ManagedSymbolPropertySetterValue*>(item);
												ManagedSymbolProperty* propertySymbol=dynamic_cast<ManagedSymbolProperty*>(item->GetParentItem());
												argument.result.expressionResult=propertySymbol->type;
											}
											return;
										}
									}
								}
							}
							break;
						case ManagedSymbolItem::Class:
						case ManagedSymbolItem::Structure:
						case ManagedSymbolItem::Interface:
							{
								if(ManagedSymbolItemGroup* group=currentScope->ItemGroup(node->name))
								{
									FOREACH(ManagedSymbolItem*, item, group->Items())
									{
										switch(item->GetSymbolType())
										{
										case ManagedSymbolItem::GenericParameter:
											{
												argument.result.typeResult=argument.context.symbolManager->GetType(item);
											}
											return;
										}
									}
								}
								ManagedTypeSymbol* containerType=GetTypeFromInsideScope(currentScope, argument.context);
								SearchMemberOfType(node, thisType, containerType, (!firstType || inStaticMethod), node->name, argument);

								if(argument.result.IsEmpty())
								{
									ManagedSymbolDeclaration* currentDeclaration=dynamic_cast<ManagedSymbolDeclaration*>(currentScope);
									FOREACH(ManagedSymbolNamespace*, namespaceSymbol, currentDeclaration->_availableUsingNamespaces.Wrap())
									{
										SearchMemberOfNamespace(node, namespaceSymbol, node->name, argument);
									}
								}
								firstType=false;
							}
							break;
						case ManagedSymbolItem::Global:
						case ManagedSymbolItem::Namespace:
							{
								SearchMemberOfNamespace(node, currentScope, node->name, argument);
							}
							break;
						}
						currentScope=currentScope->GetParentItem();
					}

					if(argument.result.IsEmpty())
					{
						argument.context.errors.Add(ManagedLanguageCodeException::GetExpressionResolvingFailed(node));
					}
				}

				ALGORITHM_PROCEDURE_MATCH(ManagedMemberExpression)
				{
					ManagedLanguage_GetTypeInternal_Expression(node->operand, argument);
					if(!argument.result.IsEmpty())
					{
						if(argument.result.methodResults.Count()>0)
						{
							argument.context.errors.Add(ManagedLanguageCodeException::GetExpressionIsNotValue(node));
						}
						else
						{
							bool inStaticMethod=IsInStaticMethod(argument.context);
							ManagedTypeSymbol* thisType=GetThisType(argument.context);
							
							if(argument.result.expressionResult)
							{
								ManagedTypeSymbol* type=argument.result.expressionResult;
								argument.result.Clear();
								SearchMemberOfType(node, thisType, type, inStaticMethod, node->member, argument);
							}
							else if(argument.result.typeResult)
							{
								ManagedTypeSymbol* type=argument.result.typeResult;
								argument.result.Clear();
								SearchMemberOfType(node, thisType, type, inStaticMethod, node->member, argument);
							}
							else if(argument.result.namespaceResult)
							{
								ManagedSymbolNamespace* namespaceResult=argument.result.namespaceResult;
								argument.result.Clear();
								SearchMemberOfNamespace(node, namespaceResult, node->member, argument);
							}

							if(argument.result.IsEmpty())
							{
								argument.context.errors.Add(ManagedLanguageCodeException::GetExpressionResolvingFailed(node));
							}
						}
					}
				}

				ALGORITHM_PROCEDURE_MATCH(ManagedInstantiatedExpression)
				{
					throw 0;
				}

				ALGORITHM_PROCEDURE_MATCH(ManagedInvokeExpression)
				{
					throw 0;
				}

				ALGORITHM_PROCEDURE_MATCH(ManagedNewObjectExpression)
				{
					throw 0;
				}

				ALGORITHM_PROCEDURE_MATCH(ManagedFunctionResultExpression)
				{
					throw 0;
				}

				ALGORITHM_PROCEDURE_MATCH(ManagedThisExpression)
				{
					if(IsInStaticMethod(argument.context))
					{
						argument.context.errors.Add(ManagedLanguageCodeException::GetIllegalThis(node));
					}
					else
					{
						ManagedTypeSymbol* thisType=GetThisType(argument.context);
						argument.result.expressionResult=thisType;
					}
				}

				ALGORITHM_PROCEDURE_MATCH(ManagedBaseExpression)
				{
					bool fail=true;
					if(!IsInStaticMethod(argument.context))
					{
						ManagedMember* target=argument.context.contextManager->GetThisTargetMember();
						if(target)
						{
							ManagedSymbolItem* targetSymbol=argument.context.symbolManager->GetSymbol(target);
							ManagedSymbolItem* definitionType=targetSymbol->GetParentItem();

							switch(definitionType->GetSymbolType())
							{
							case ManagedSymbolItem::Class:
							case ManagedSymbolItem::Structure:
							case ManagedSymbolItem::Interface:
								{
									ManagedSymbolDeclaration* declaration=dynamic_cast<ManagedSymbolDeclaration*>(definitionType);
									FOREACH(ManagedTypeSymbol*, baseType, declaration->baseTypes.Wrap())
									{
										switch(GetRealSymbol(baseType->GetSymbol())->GetSymbolType())
										{
										case ManagedSymbolItem::Class:
										case ManagedSymbolItem::Structure:
											fail=false;
											break;
										}
									}
								}
								break;
							}
						}
					}

					if(fail)
					{
						argument.context.errors.Add(ManagedLanguageCodeException::GetIllegalBase(node));
					}
					else
					{
						ManagedTypeSymbol* baseType=GetBaseType(GetThisType(argument.context), argument.context);
						argument.result.expressionResult=baseType;
					}
				}

				ALGORITHM_PROCEDURE_MATCH(ManagedAssignmentExpression)
				{
					throw 0;
				}

				ALGORITHM_PROCEDURE_MATCH(ManagedExtendedExpression)
				{
					ManagedLanguage_GetTypeInternal_ExtendedExpression(node, argument);
				}

			END_ALGORITHM_PROCEDURE(ManagedLanguage_GetTypeInternal_Expression)

/***********************************************************************
ManagedLanguage_GetTypeInternal_ExtendedExpression
***********************************************************************/

			ManagedSymbolMethod* GetOperator(ExpressionChoice choice, vint operandCount, ManagedTypeSymbol* thisType, const MAP& argument)
			{
				if(choice.symbol->GetSymbolType()==ManagedSymbolItem::Method)
				{
					ManagedSymbolMethod* method=dynamic_cast<ManagedSymbolMethod*>(choice.symbol);
					if(method->memberType==declatt::Static && method->orderedMethodParameterNames.Count()==operandCount)
					{
						if(IsVisible(thisType, choice.type, method->accessor, argument))
						{
							FOREACH(WString, parameterName, method->orderedMethodParameterNames.Wrap())
							{
								ManagedSymbolMethodParameter* parameter=dynamic_cast<ManagedSymbolMethodParameter*>(method->ItemGroup(parameterName)->Items()[0]);
								switch(parameter->parameterType)
								{
								case ManagedParameter::Out:
								case ManagedParameter::Params:
									return 0;
								}
							}
							return method;
						}
					}
				}
				return 0;
			}

			bool FitOperatorParameter(ManagedSymbolMethod* method, vint parameterIndex, ManagedTypeSymbol* operandType, ManagedTypeSymbol* containerType, const MAP& argument)
			{
				ManagedSymbolMethodParameter* operand=dynamic_cast<ManagedSymbolMethodParameter*>(method->ItemGroup(method->orderedMethodParameterNames[parameterIndex])->Items()[0]);
				if(CanImplicitlyConvertTo(operandType, argument.symbolManager->ReplaceGenericArguments(operand->type, containerType), argument))
				{
					return true;
				}
				return false;
			}

			BEGIN_ALGORITHM_PROCEDURE(ManagedLanguage_GetTypeInternal_ExtendedExpression, ManagedExtendedExpression, MAGETP)

				ALGORITHM_PROCEDURE_MATCH(ManagedLambdaExpression)
				{
					// TODO: push lambda expression
					throw 0;
				}

				ALGORITHM_PROCEDURE_MATCH(ManagedChoiceExpression)
				{
					throw 0;
				}

				ALGORITHM_PROCEDURE_MATCH(ManagedNullChoiceExpression)
				{
					throw 0;
				}

				ALGORITHM_PROCEDURE_MATCH(ManagedTypeofExpression)
				{
					throw 0;
				}

				ALGORITHM_PROCEDURE_MATCH(ManagedDefaultExpression)
				{
					throw 0;
				}

				ALGORITHM_PROCEDURE_MATCH(ManagedUnaryExpression)
				{
					ManagedTypeSymbol* operandType=GetType(node->operand.Obj(), 0, argument.context);
					if(operandType)
					{
						ManagedTypeSymbol* thisType=GetThisType(argument.context);
						ExpressionResolvingResult operandResult;
						MAGETP operandArgument(argument.context, operandResult);
						SearchMemberOfType(node, thisType, operandType, true, node->operatorName, operandArgument, false, true);

						if(!argument.result.IsEmpty())
						{
							if(operandResult.methodResults.Count()>0)
							{
								FOREACH(ExpressionChoice, operatorChoice, operandResult.methodResults.Wrap())
								{
									ManagedSymbolMethod* method=GetOperator(operatorChoice, 1, thisType, argument.context);
									if(method && FitOperatorParameter(method, 0, operandType, operatorChoice.type, argument.context))
									{
										if(argument.result.IsEmpty())
										{
											ManagedTypeSymbol* returnType=argument.context.symbolManager->ReplaceGenericArguments(method->returnType, operatorChoice.type);
											argument.result.expressionResult=returnType;
										}
										else
										{
											argument.context.errors.Add(ManagedLanguageCodeException::GetMatchedMethodTooMuch(node));
											break;
										}
									}
								}

								if(argument.result.IsEmpty())
								{
									argument.context.errors.Add(ManagedLanguageCodeException::GetMatchedMethodNotExists(node));
								}
							}
							else
							{
								argument.context.errors.Add(ManagedLanguageCodeException::GetMatchedMethodNotExists(node));
							}
						}
					}
				}

				ALGORITHM_PROCEDURE_MATCH(ManagedBinaryExpression)
				{
					ManagedTypeSymbol* leftType=GetType(node->leftOperand.Obj(), 0, argument.context);
					ManagedTypeSymbol* rightType=GetType(node->rightOperand.Obj(), 0, argument.context);
					if(leftType && rightType)
					{
						ManagedTypeSymbol* thisType=GetThisType(argument.context);
						ExpressionResolvingResult leftResult, rightResult;
						MAGETP leftArgument(argument.context, leftResult), rightArgument(argument.context, rightResult);
						SearchMemberOfType(node, thisType, leftType, true, node->operatorName, leftArgument, false, true);
						SearchMemberOfType(node, thisType, rightType, true, node->operatorName, rightArgument, false, true);

						if(!leftResult.IsEmpty())
						{
							if(leftResult.methodResults.Count()>0)
							{
								FOREACH(ExpressionChoice, operatorChoice, leftResult.methodResults.Wrap())
								{
									ManagedSymbolMethod* method=GetOperator(operatorChoice, 2, thisType, argument.context);
									if(method
										&& FitOperatorParameter(method, 0, leftType, operatorChoice.type, argument.context)
										&& FitOperatorParameter(method, 1, rightType, operatorChoice.type, argument.context))
									{
										if(argument.result.IsEmpty())
										{
											ManagedTypeSymbol* returnType=argument.context.symbolManager->ReplaceGenericArguments(method->returnType, operatorChoice.type);
											argument.result.expressionResult=returnType;
										}
										else
										{
											argument.context.errors.Add(ManagedLanguageCodeException::GetMatchedMethodTooMuch(node));
											break;
										}
									}
								}
							}
							else
							{
								argument.context.errors.Add(ManagedLanguageCodeException::GetMatchedMethodNotExists(node));
							}
						}
						else if(!rightResult.IsEmpty())
						{
							if(rightResult.methodResults.Count()>0)
							{
								FOREACH(ExpressionChoice, operatorChoice, rightResult.methodResults.Wrap())
								{
									ManagedSymbolMethod* method=GetOperator(operatorChoice, 2, thisType, argument.context);
									if(method
										&& FitOperatorParameter(method, 0, leftType, operatorChoice.type, argument.context)
										&& FitOperatorParameter(method, 1, rightType, operatorChoice.type, argument.context))
									{
										if(argument.result.IsEmpty())
										{
											ManagedTypeSymbol* returnType=argument.context.symbolManager->ReplaceGenericArguments(method->returnType, operatorChoice.type);
											argument.result.expressionResult=returnType;
										}
										else
										{
											argument.context.errors.Add(ManagedLanguageCodeException::GetMatchedMethodTooMuch(node));
											break;
										}
									}
								}
							}
							else
							{
								argument.context.errors.Add(ManagedLanguageCodeException::GetMatchedMethodNotExists(node));
							}
						}
					}
				}

				ALGORITHM_PROCEDURE_MATCH(ManagedNewArrayExpression)
				{
					throw 0;
				}

				ALGORITHM_PROCEDURE_MATCH(ManagedIsTypeExpression)
				{
					throw 0;
				}

				ALGORITHM_PROCEDURE_MATCH(ManagedCastingExpression)
				{
					throw 0;
				}

				ALGORITHM_PROCEDURE_MATCH(ManagedIndexExpression)
				{
					throw 0;
				}

			END_ALGORITHM_PROCEDURE(ManagedLanguage_GetTypeInternal_ExtendedExpression)
		}
	}
}