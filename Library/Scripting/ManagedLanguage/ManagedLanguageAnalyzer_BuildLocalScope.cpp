#include "ManagedLanguageAnalyzer.h"
#include "..\..\Collections\OperationForEach.h"

namespace vl
{
	namespace scripting
	{
		namespace managedlanguage
		{
			using namespace collections;

			EXTERN_ALGORITHM_PROCEDURE(ManagedLanguage_BuildLocalScopeInternal_Statement, ManagedStatement, MAP)
			EXTERN_ALGORITHM_PROCEDURE(ManagedLanguage_BuildLocalScopeInternal_ExtendedStatement, ManagedExtendedStatement, MAP)

/***********************************************************************
BuildLocalScope
***********************************************************************/

			void BuildLocalScope(ManagedStatement* node, const MAP& argument)
			{
				ManagedLanguage_BuildLocalScopeInternal_Statement(node, argument);
				argument.contextManager->SetStatement(node, argument.currentSymbol);
			}

/***********************************************************************
ManagedLanguage_BuildLocalScopeInternal_Statement
***********************************************************************/

			BEGIN_ALGORITHM_PROCEDURE(ManagedLanguage_BuildLocalScopeInternal_Statement, ManagedStatement, MAP)

				ALGORITHM_PROCEDURE_MATCH(ManagedEmptyStatement)
				{
				}

				ALGORITHM_PROCEDURE_MATCH(ManagedCompositeStatement)
				{
					ManagedSymbolBlock* symbol=new ManagedSymbolBlock(argument.symbolManager);
					symbol->languageElement=node;
					argument.currentSymbol->Add(symbol);

					MAP newArgument(argument, symbol);
					FOREACH(Ptr<ManagedStatement>, statement, node->statements.Wrap())
					{
						BuildLocalScope(statement.Obj(), newArgument);
					}
				}

				ALGORITHM_PROCEDURE_MATCH(ManagedExpressionStatement)
				{
					GetType(node->expression.Obj(), 0, argument);
				}

				ALGORITHM_PROCEDURE_MATCH(ManagedVariableStatement)
				{
					bool autoref=dynamic_cast<ManagedAutoReferType*>(node->type.Obj())!=0;
					ManagedTypeSymbol* variableType=autoref?0:GetTypeSymbolInMethod(node->type, argument);
					if(!node->initializer)
					{
						if(autoref)
						{
							argument.errors.Add(ManagedLanguageCodeException::GetIllegalAutoRefWithoutInitializer(node));
						}
					}
					else
					{
						ManagedTypeSymbol* resultType=GetType(node->initializer.Obj(), variableType, argument);
						if(variableType==0)
						{
							variableType=resultType;
						}
						if(node->constant)
						{
							// TODO: check initializer (constant)
						}
					}

					if(argument.currentSymbol->ItemGroup(node->name))
					{
						argument.errors.Add(ManagedLanguageCodeException::GetVariableAlreadyExists(node));
					}
					else if(variableType)
					{
						ManagedSymbolVariable* symbol=new ManagedSymbolVariable(argument.symbolManager);
						symbol->SetName(node->name);
						symbol->type=variableType;
						symbol->constant=node->constant;
						symbol->variableLanguageElement=node;
						argument.currentSymbol->Add(symbol);
					}
				}

				ALGORITHM_PROCEDURE_MATCH(ManagedIfStatement)
				{
					GetType(node->condition.Obj(), argument.contextManager->predefinedTypes.boolType, argument);
					BuildLocalScope(node->trueStatement.Obj(), argument);
					if(node->falseStatement)
					{
						BuildLocalScope(node->falseStatement.Obj(), argument);
					}
				}

				ALGORITHM_PROCEDURE_MATCH(ManagedWhileStatement)
				{
					if(node->beginCondition)
					{
						GetType(node->beginCondition.Obj(), argument.contextManager->predefinedTypes.boolType, argument);
					}
					if(node->endCondition)
					{
						GetType(node->endCondition.Obj(), argument.contextManager->predefinedTypes.boolType, argument);
					}
					argument.contextManager->PushLoop(node);
					BuildLocalScope(node->statement.Obj(), argument);
					argument.contextManager->PopStatement();
				}

				ALGORITHM_PROCEDURE_MATCH(ManagedBreakStatement)
				{
					if(!argument.contextManager->GetBreakTarget())
					{
						argument.errors.Add(ManagedLanguageCodeException::GetIllegalBreak(node));
					}
				}

				ALGORITHM_PROCEDURE_MATCH(ManagedContinueStatement)
				{
					if(!argument.contextManager->GetContinueTarget())
					{
						argument.errors.Add(ManagedLanguageCodeException::GetIllegalContinue(node));
					}
				}

				ALGORITHM_PROCEDURE_MATCH(ManagedReturnStatement)
				{
				}

				ALGORITHM_PROCEDURE_MATCH(ManagedTryCatchStatement)
				{
					BuildLocalScope(node->tryStatement.Obj(), argument);
					FOREACH(Ptr<ManagedCatchClause>, catchClause, node->catches.Wrap())
					{
						ManagedTypeSymbol* exceptionType=GetTypeSymbolInMethod(catchClause->exceptionType, argument);
						if(exceptionType && !IsInheritedFrom(exceptionType, argument.contextManager->predefinedTypes.exceptionType, argument))
						{
							argument.errors.Add(ManagedLanguageCodeException::GetExceptionTypeShouldDerivedFromException(catchClause->exceptionType.Obj(), argument.contextManager->predefinedTypes.exceptionType));
						}

						ManagedSymbolBlock* block=new ManagedSymbolBlock(argument.symbolManager);
						block->languageElement=catchClause->exceptionHandler.Obj();
						argument.currentSymbol->Add(block);

						if(catchClause->exceptionName!=L"" && exceptionType)
						{
							ManagedSymbolVariable* variable=new ManagedSymbolVariable(argument.symbolManager);
							variable->SetName(catchClause->exceptionName);
							variable->type=exceptionType;
							variable->constant=true;
							variable->catchLanguageElement=catchClause.Obj();
							block->Add(variable);
						}

						argument.contextManager->PushCatch(catchClause.Obj());
						MAP newArgument(argument, block);
						BuildLocalScope(catchClause->exceptionHandler.Obj(), newArgument);
						argument.contextManager->PopStatement();
					}
					if(node->finallyStatement)
					{
						BuildLocalScope(node->finallyStatement.Obj(), argument);
					}
				}

				ALGORITHM_PROCEDURE_MATCH(ManagedThrowStatement)
				{
					if(node->expression)
					{
						GetType(node->expression.Obj(), argument.contextManager->predefinedTypes.exceptionType, argument);
					}
					else
					{
						if(!argument.contextManager->GetThrowTarget())
						{
							argument.errors.Add(ManagedLanguageCodeException::GetIllegalThrow(node));
						}
					}
				}

				ALGORITHM_PROCEDURE_MATCH(ManagedExtendedStatement)
				{
					ManagedLanguage_BuildLocalScopeInternal_ExtendedStatement(node, argument);
				}

			END_ALGORITHM_PROCEDURE(ManagedLanguage_BuildLocalScopeInternal_Statement)

/***********************************************************************
ManagedLanguage_BuildLocalScopeInternal_ExtendedStatement
***********************************************************************/

			BEGIN_ALGORITHM_PROCEDURE(ManagedLanguage_BuildLocalScopeInternal_ExtendedStatement, ManagedExtendedStatement, MAP)

				ALGORITHM_PROCEDURE_MATCH(ManagedUsingStatement)
				{
					bool autoref=dynamic_cast<ManagedAutoReferType*>(node->type.Obj())!=0;
					ManagedTypeSymbol* type=autoref?0:GetTypeSymbolInMethod(node->type, argument);
					{
						ManagedTypeSymbol* resultType=GetType(node->initialize.Obj(), type, argument);
						if(type==0)
						{
							type=resultType;
						}
					}
					if(type)
					{
						if(!CanImplicitlyConvertTo(type, argument.contextManager->predefinedTypes.idisposableType, argument))
						{
							argument.errors.Add(ManagedLanguageCodeException::GetCannotDisposeNonIDisposableType(node, type));
						}
					}

					ManagedSymbolBlock* block=new ManagedSymbolBlock(argument.symbolManager);
					block->languageElement=node;
					argument.currentSymbol->Add(block);

					if(type)
					{
						ManagedSymbolVariable* variable=new ManagedSymbolVariable(argument.symbolManager);
						variable->SetName(node->name);
						variable->type=type;
						variable->constant=true;
						variable->usingLanguageElement=node;
						block->Add(variable);
					}

					MAP newArgument(argument, block);
					BuildLocalScope(node->statement.Obj(), newArgument);
				}

				ALGORITHM_PROCEDURE_MATCH(ManagedLockStatement)
				{
					GetType(node->lock.Obj(), 0, argument);
					BuildLocalScope(node->statement.Obj(), argument);
				}

				ALGORITHM_PROCEDURE_MATCH(ManagedSelectStatement)
				{
					argument.contextManager->PushSwitch(node);
					ManagedTypeSymbol* expressionType=GetType(node->expression.Obj(), 0, argument);
					FOREACH(Ptr<ManagedCaseClause>, caseClause, node->cases.Wrap())
					{
						FOREACH(Ptr<ManagedExpression>, condition, caseClause->conditions.Wrap())
						{
							GetType(condition.Obj(), expressionType, argument);
						}
						BuildLocalScope(caseClause->statement.Obj(), argument);
					}
					if(node->defaultStatement)
					{
						BuildLocalScope(node->defaultStatement.Obj(), argument);
					}
					argument.contextManager->PopStatement();
				}

				ALGORITHM_PROCEDURE_MATCH(ManagedForStatement)
				{
					ManagedSymbolBlock* symbol=new ManagedSymbolBlock(argument.symbolManager);
					symbol->languageElement=node;
					argument.currentSymbol->Add(symbol);

					MAP newArgument(argument, symbol);
					FOREACH(Ptr<ManagedVariableStatement>, variable, node->initializers.Wrap())
					{
						BuildLocalScope(variable.Obj(), newArgument);
					}
					if(node->condition)
					{
						GetType(node->condition.Obj(), argument.contextManager->predefinedTypes.boolType, argument);
					}
					FOREACH(Ptr<ManagedExpression>, sideEffect, node->sideEffects.Wrap())
					{
						GetType(sideEffect.Obj(), 0, argument);
					}
					argument.contextManager->PushLoop(node);
					BuildLocalScope(node->statement.Obj(), newArgument);
					argument.contextManager->PopStatement();
				}

				ALGORITHM_PROCEDURE_MATCH(ManagedForEachStatement)
				{
					bool autoref=dynamic_cast<ManagedAutoReferType*>(node->type.Obj())!=0;
					ManagedTypeSymbol* elementType=autoref?0:GetTypeSymbolInMethod(node->type, argument);
					{
						ManagedTypeSymbol* containerType=GetType(node->container.Obj(), 0, argument);
						if(containerType)
						{
							if(autoref)
							{
								ManagedTypeSymbol* currentType=containerType;
								while(currentType)
								{
									ManagedSymbolDeclaration* currentSymbol=dynamic_cast<ManagedSymbolDeclaration*>(GetRealSymbol(currentType->GetSymbol()));
									FOREACH(ManagedTypeSymbol*, interfaceType, currentSymbol->_baseInterfaces.Wrap())
									{
										if(interfaceType->GetGenericDeclaration()==argument.contextManager->predefinedTypes.ienumerableOfTType)
										{
											ManagedTypeSymbol* currentElementType=argument.symbolManager->ReplaceGenericArguments(interfaceType, currentType->GetGenericArguments()[0]);
											if(elementType==0)
											{
												elementType=currentElementType;
											}
											else if(elementType!=currentElementType)
											{
												argument.errors.Add(ManagedLanguageCodeException::GetCannotDetermineContainerElementType(node, containerType));
												goto FINISH_ELEMENT_TYPE_CHECKING;
											}
										}
									}
									if(currentSymbol->_baseType)
									{
										currentType=argument.symbolManager->GetBaseType(currentSymbol->_baseType, currentType);
									}
								}
							}
							else if(elementType)
							{
								ManagedTypeSymbol* enumerableInterface=argument.contextManager->predefinedTypes.ienumerableOfTType;
								List<ManagedTypeSymbol*> genericArguments;
								genericArguments.Add(elementType);
								ManagedTypeSymbol* elementEnumerableType=argument.symbolManager->GetInstiantiatedType(enumerableInterface, genericArguments.Wrap());
								if(!CanImplicitlyConvertTo(containerType, elementEnumerableType, argument))
								{
									argument.errors.Add(ManagedLanguageCodeException::GetExpressionCannotConvertToType(node->container.Obj(), elementEnumerableType));
								}
							}
						}
					}
				FINISH_ELEMENT_TYPE_CHECKING:

					ManagedSymbolBlock* block=new ManagedSymbolBlock(argument.symbolManager);
					block->languageElement=node;
					argument.currentSymbol->Add(block);

					if(elementType)
					{
						ManagedSymbolVariable* variable=new ManagedSymbolVariable(argument.symbolManager);
						variable->SetName(node->name);
						variable->type=elementType;
						variable->constant=true;
						variable->forEachLanguageElement=node;
						block->Add(variable);
					}

					argument.contextManager->PushLoop(node);
					MAP newArgument(argument, block);
					BuildLocalScope(node->statement.Obj(), newArgument);
					argument.contextManager->PopStatement();
				}

			END_ALGORITHM_PROCEDURE(ManagedLanguage_BuildLocalScopeInternal_ExtendedStatement)
		}
	}
}