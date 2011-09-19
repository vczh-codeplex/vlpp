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

			class ManagedAnalyzerGetExpressionTypeParameter : public Object
			{
			public:
				struct Choice
				{
					ManagedTypeSymbol*					type;	// symbol type if exists. for method and generic parameter, it this the owner type's type.
					ManagedSymbolItem*					symbol;

					Choice(ManagedTypeSymbol* _type=0, ManagedSymbolItem* _symbol=0)
						:type(_type)
						,symbol(_symbol)
					{
					}

					bool operator==(const Choice& value){return false;}
					bool operator!=(const Choice& value){return true;}
				};
			public:
				const MAP&								context;
				List<Choice>&							choices;

				ManagedAnalyzerGetExpressionTypeParameter(const MAP& _context, List<Choice>& _choices)
					:context(_context)
					,choices(_choices)
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
				List<MAGETP::Choice> choices;
				MAP a1(argument, argument.currentSymbol, expectedType);
				MAGETP a2(a1, choices);
				ManagedLanguage_GetTypeInternal_Expression(node, a2);

				if(expectedType)
				{
					for(vint i=choices.Count()-1;i>=0;i--)
					{
						if(!CanImplicitlyConvertTo(choices[i].type, expectedType, argument))
						{
							choices.RemoveAt(i);
						}
					}
				}

				if(choices.Count()>1)
				{
					argument.errors.Add(ManagedLanguageCodeException::GetExpressionResolvedToDuplicatedTargets(node, expectedType));
				}
				else if(choices.Count()==1)
				{
					ManagedSymbolItem* symbol=a2.choices[0].symbol;
					ManagedTypeSymbol* type=a2.choices[0].type;
					
					if(symbol && !IsValue(symbol))
					{
						argument.errors.Add(ManagedLanguageCodeException::GetExpressionIsNotValue(node));
					}

					if(type)
					{
						CheckTypeInMethod(node, type, argument);
						if(expectedType && !CanImplicitlyConvertTo(type, expectedType, argument))
						{
							argument.errors.Add(ManagedLanguageCodeException::GetExpressionCannotConvertToType(node, expectedType));
						}
					}
					return type;
				}
				return 0;
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

			void SearchMemberOfNamespace(ManagedSymbolItem* namespaceItem, const WString& name, const MAP& argument, List<MAGETP::Choice>& choices)
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
								choices.Add(MAGETP::Choice(argument.symbolManager->GetType(item), item));
							}
							break;
						case ManagedSymbolItem::Namespace:
							{
								choices.Add(MAGETP::Choice(0, item));
							}
							break;
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

			void SearchMemberOfType(ManagedTypeSymbol* thisType, ManagedTypeSymbol* containerType, bool staticOnly, const WString& name, const MAP& argument, List<MAGETP::Choice>& choices)
			{
				vint oldChoicesCount=choices.Count();
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
								{
									ManagedSymbolField* symbol=dynamic_cast<ManagedSymbolField*>(item);
									if(IsVisible(staticOnly, symbol->memberType) && IsVisible(thisType, containerType, symbol->accessor, argument))
									{
										ManagedTypeSymbol* symbolType=argument.symbolManager->ReplaceGenericArguments(symbol->type, containerType);
										choices.Add(MAGETP::Choice(symbolType, item));
									}
								}
								break;
							case ManagedSymbolItem::Property:
								{
									ManagedSymbolProperty* symbol=dynamic_cast<ManagedSymbolProperty*>(item);
									if(IsVisible(staticOnly, symbol->memberType) && IsVisible(thisType, containerType, symbol->accessor, argument) && !symbol->implementedInterfaceType)
									{
										ManagedTypeSymbol* symbolType=argument.symbolManager->ReplaceGenericArguments(symbol->type, containerType);
										choices.Add(MAGETP::Choice(symbolType, item));
									}
								}
								break;
							case ManagedSymbolItem::Method:
								{
									ManagedSymbolMethod* symbol=dynamic_cast<ManagedSymbolMethod*>(item);
									if(IsVisible(staticOnly, symbol->memberType) && IsVisible(thisType, containerType, symbol->accessor, argument) && !symbol->implementedInterfaceType)
									{
										choices.Add(MAGETP::Choice(containerType, item));
									}
								}
								break;
							case ManagedSymbolItem::GenericParameter:
								{
									choices.Add(MAGETP::Choice(argument.symbolManager->GetType(item), item));
								}
								break;
							case ManagedSymbolItem::Class:
							case ManagedSymbolItem::Structure:
							case ManagedSymbolItem::Interface:
								{
									if(IsVisible(thisType, containerType, dynamic_cast<ManagedSymbolDeclaration*>(item)->accessor, argument))
									{
										ManagedTypeSymbol* symbolType=argument.symbolManager->GetType(item, containerType);
										choices.Add(MAGETP::Choice(symbolType, item));
									}
								}
								break;
							case ManagedSymbolItem::TypeRename:
								{
									ManagedSymbolTypeRename* symbol=dynamic_cast<ManagedSymbolTypeRename*>(item);
									if(IsVisible(thisType, containerType, symbol->accessor, argument))
									{
										// TODO: this is wrong
										ManagedTypeSymbol* symbolType=GetRealType(symbol->type, argument);
										choices.Add(MAGETP::Choice(symbolType, item));
									}
								}
								break;
							case ManagedSymbolItem::Namespace:
								{
									choices.Add(MAGETP::Choice(0, item));
								}
								break;
							}
						}
					}

					if(thisType==containerType)
					{
					}

					if(choices.Count()==oldChoicesCount)
					{
						FOREACH(ManagedTypeSymbol*, baseType, dynamic_cast<ManagedSymbolDeclaration*>(containerScope)->baseTypes.Wrap())
						{
							if(containerScope->GetSymbolType()==ManagedSymbolItem::Interface || GetRealSymbol(baseType->GetSymbol())->GetSymbolType()!=ManagedSymbolItem::Interface)
							{
								SearchMemberOfType(thisType, argument.symbolManager->GetBaseType(baseType, containerType), staticOnly, name, argument, choices);
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
							argument.choices.Add(argument.context.expectedType);
						}
					}
					if(argument.choices.Count()==0)
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
						argument.choices.Add(argument.context.expectedType);
					}
					else if(!node->sign && (
						argument.context.expectedType==argument.context.contextManager->predefinedTypes.uint8 ||
						argument.context.expectedType==argument.context.contextManager->predefinedTypes.uint16 ||
						argument.context.expectedType==argument.context.contextManager->predefinedTypes.uint32 ||
						argument.context.expectedType==argument.context.contextManager->predefinedTypes.uint64))
					{
						argument.choices.Add(argument.context.expectedType);
					}
					else if(
						argument.context.expectedType==argument.context.contextManager->predefinedTypes.singleType ||
						argument.context.expectedType==argument.context.contextManager->predefinedTypes.doubleType)
					{
						argument.choices.Add(argument.context.expectedType);
					}
					else
					{
						argument.choices.Add(argument.context.contextManager->predefinedTypes.sint32);
					}
				}

				ALGORITHM_PROCEDURE_MATCH(ManagedFloatExpression)
				{
					if(argument.context.expectedType==argument.context.contextManager->predefinedTypes.singleType)
					{
						argument.choices.Add(argument.context.contextManager->predefinedTypes.singleType);
					}
					else
					{
						argument.choices.Add(argument.context.contextManager->predefinedTypes.doubleType);
					}
				}

				ALGORITHM_PROCEDURE_MATCH(ManagedBooleanExpression)
				{
					argument.choices.Add(argument.context.contextManager->predefinedTypes.boolType);
				}

				ALGORITHM_PROCEDURE_MATCH(ManagedCharExpression)
				{
					argument.choices.Add(argument.context.contextManager->predefinedTypes.charType);
				}

				ALGORITHM_PROCEDURE_MATCH(ManagedStringExpression)
				{
					argument.choices.Add(argument.context.contextManager->predefinedTypes.stringType);
				}

				ALGORITHM_PROCEDURE_MATCH(ManagedReferenceExpression)
				{
					bool inStaticMethod=IsInStaticMethod(argument.context);
					ManagedSymbolItem* currentScope=argument.context.currentSymbol;
					ManagedTypeSymbol* thisType=GetThisType(argument.context);
					while(argument.choices.Count()==0 && currentScope!=0)
					{
						int typeCounter=0;
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
												argument.choices.Add(MAGETP::Choice(symbol->type, item));
											}
											break;
										case ManagedSymbolItem::LambdaParameter:
											{
												ManagedSymbolLambdaParameter* symbol=dynamic_cast<ManagedSymbolLambdaParameter*>(item);
												argument.choices.Add(MAGETP::Choice(symbol->type, item));
											}
											break;
										case ManagedSymbolItem::GenericParameter:
											{
												argument.choices.Add(MAGETP::Choice(thisType, item));
											}
											break;
										case ManagedSymbolItem::MethodParameter:
											{
												ManagedSymbolMethodParameter* symbol=dynamic_cast<ManagedSymbolMethodParameter*>(item);
												argument.choices.Add(MAGETP::Choice(symbol->type, item));
											}
											break;
										case ManagedSymbolItem::PropertySetterValue:
											{
												ManagedSymbolPropertySetterValue* symbol=dynamic_cast<ManagedSymbolPropertySetterValue*>(item);
												ManagedSymbolProperty* propertySymbol=dynamic_cast<ManagedSymbolProperty*>(item->GetParentItem());
												argument.choices.Add(MAGETP::Choice(propertySymbol->type, item));
											}
											break;
										}
									}
								}
							}
							break;
						case ManagedSymbolItem::Class:
						case ManagedSymbolItem::Structure:
						case ManagedSymbolItem::Interface:
							{
								typeCounter++;
								ManagedTypeSymbol* containerType=GetTypeFromInsideScope(currentScope, argument.context);
								SearchMemberOfType(thisType, containerType, (typeCounter>1 || inStaticMethod), node->name, argument.context, argument.choices);

								ManagedSymbolDeclaration* currentDeclaration=dynamic_cast<ManagedSymbolDeclaration*>(currentScope);
								FOREACH(ManagedSymbolNamespace*, namespaceSymbol, currentDeclaration->_availableUsingNamespaces.Wrap())
								{
									SearchMemberOfNamespace(namespaceSymbol, node->name, argument.context, argument.choices);
								}
							}
							break;
						case ManagedSymbolItem::Global:
						case ManagedSymbolItem::Namespace:
							{
								SearchMemberOfNamespace(currentScope, node->name, argument.context, argument.choices);
							}
							break;
						}
						currentScope=currentScope->GetParentItem();
					}

					if(argument.choices.Count()==0)
					{
						argument.context.errors.Add(ManagedLanguageCodeException::GetExpressionResolvingFailed(node));
					}
				}

				ALGORITHM_PROCEDURE_MATCH(ManagedMemberExpression)
				{
					ManagedLanguage_GetTypeInternal_Expression(node->operand, argument);
					if(argument.choices.Count()>0)
					{
						List<MAGETP::Choice> choices;
					
						bool inStaticMethod=IsInStaticMethod(argument.context);
						ManagedTypeSymbol* thisType=GetThisType(argument.context);

						FOREACH(MAGETP::Choice, choice, argument.choices.Wrap())
						{
							if(choice.type==0)
							{
								SearchMemberOfNamespace(choice.symbol, node->member, argument.context, choices);
							}
							else if(choice.symbol->GetSymbolType()!=ManagedSymbolItem::Method)
							{
								CheckTypeInMethod(node->operand.Obj(), choice.type, argument.context);
								SearchMemberOfType(thisType, choice.type, inStaticMethod, node->member, argument.context, choices);
							}
						}

						if(choices.Count()==0)
						{
							argument.context.errors.Add(ManagedLanguageCodeException::GetExpressionResolvingFailed(node));
						}
						CopyFrom(argument.choices.Wrap(), choices.Wrap());
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
						argument.choices.Add(MAGETP::Choice(thisType, thisType->GetSymbol()));
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
						ManagedTypeSymbol* thisType=GetBaseType(GetThisType(argument.context), argument.context);
						argument.choices.Add(MAGETP::Choice(thisType, thisType->GetSymbol()));
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

			ManagedSymbolMethod* GetOperator(MAGETP::Choice choice, vint operandCount, ManagedTypeSymbol* thisType, const MAP& argument)
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

				bool FitOperatorParameter(ManagedSymbolMethod* method, vint parameterIndex, ManagedTypeSymbol* operandType, ManagedTypeSymbol* containerType, const MAP& argument)
				{
					ManagedSymbolMethodParameter* operand=dynamic_cast<ManagedSymbolMethodParameter*>(method->ItemGroup(method->orderedMethodParameterNames[parameterIndex])->Items()[0]);
					if(CanImplicitlyConvertTo(operandType, argument.symbolManager->ReplaceGenericArguments(operand->type, containerType), argument))
					{
						return true;
					}
					return false;
				}

				ALGORITHM_PROCEDURE_MATCH(ManagedUnaryExpression)
				{
					ManagedTypeSymbol* operandType=GetType(node->operand.Obj(), 0, argument.context);
					if(operandType)
					{
						ManagedTypeSymbol* thisType=GetThisType(argument.context);
						List<MAGETP::Choice> operatorChoices;
						SearchMemberOfType(thisType, operandType, true, node->operatorName, argument.context, operatorChoices);

						FOREACH(MAGETP::Choice, operatorChoice, operatorChoices.Wrap())
						{
							ManagedSymbolMethod* method=GetOperator(operatorChoice, 1, thisType, argument.context);
							if(method && FitOperatorParameter(method, 0, operandType, operatorChoice.type, argument.context))
							{
								ManagedTypeSymbol* returnType=argument.context.symbolManager->ReplaceGenericArguments(method->returnType, operatorChoice.type);
								argument.choices.Add(MAGETP::Choice(returnType));
							}
						}

						if(argument.choices.Count()==0)
						{
							argument.context.errors.Add(ManagedLanguageCodeException::GetMatchedMethodNotExists(node));
						}
						else if(argument.choices.Count()>1)
						{
							argument.context.errors.Add(ManagedLanguageCodeException::GetMatchedMethodTooMuch(node));
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
						List<MAGETP::Choice> leftOperatorChoices, rightOperatorChoices;
						SearchMemberOfType(thisType, leftType, true, node->operatorName, argument.context, leftOperatorChoices);
						SearchMemberOfType(thisType, rightType, true, node->operatorName, argument.context, rightOperatorChoices);

						FOREACH(MAGETP::Choice, operatorChoice, leftOperatorChoices.Wrap())
						{
							ManagedSymbolMethod* method=GetOperator(operatorChoice, 2, thisType, argument.context);
							if(method
								&& FitOperatorParameter(method, 0, leftType, operatorChoice.type, argument.context)
								&& FitOperatorParameter(method, 1, rightType, operatorChoice.type, argument.context))
							{
								ManagedTypeSymbol* returnType=argument.context.symbolManager->ReplaceGenericArguments(method->returnType, operatorChoice.type);
								argument.choices.Add(MAGETP::Choice(returnType));
							}
						}

						if(argument.choices.Count()==0)
						{
							FOREACH(MAGETP::Choice, operatorChoice, rightOperatorChoices.Wrap())
							{
								ManagedSymbolMethod* method=GetOperator(operatorChoice, 2, thisType, argument.context);
								if(method
									&& FitOperatorParameter(method, 0, leftType, operatorChoice.type, argument.context)
									&& FitOperatorParameter(method, 1, rightType, operatorChoice.type, argument.context))
								{
									ManagedTypeSymbol* returnType=argument.context.symbolManager->ReplaceGenericArguments(method->returnType, operatorChoice.type);
									argument.choices.Add(MAGETP::Choice(returnType));
								}
							}
						}

						if(argument.choices.Count()==0)
						{
							argument.context.errors.Add(ManagedLanguageCodeException::GetMatchedMethodNotExists(node));
						}
						else if(argument.choices.Count()>1)
						{
							argument.context.errors.Add(ManagedLanguageCodeException::GetMatchedMethodTooMuch(node));
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