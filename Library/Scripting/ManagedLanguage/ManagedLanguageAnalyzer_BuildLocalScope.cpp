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
					// TODO: check expression
				}

				ALGORITHM_PROCEDURE_MATCH(ManagedVariableStatement)
				{
					bool autoref=dynamic_cast<ManagedAutoReferType*>(node->type.Obj())!=0;
					ManagedTypeSymbol* variableType=0;
					if(!node->initializer)
					{
						if(autoref)
						{
							argument.errors.Add(ManagedLanguageCodeException::GetIllegalAutoRefWithoutInitializer(node));
						}
					}
					else
					{
						// TODO: check initializer (constant)
					}

					if(argument.currentSymbol->ItemGroup(node->name))
					{
						argument.errors.Add(ManagedLanguageCodeException::GetVariableAlreadyExists(node));
					}
					else
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
					// TODO: check condition;
					BuildLocalScope(node->trueStatement.Obj(), argument);
					if(node->falseStatement)
					{
						BuildLocalScope(node->falseStatement.Obj(), argument);
					}
				}

				ALGORITHM_PROCEDURE_MATCH(ManagedWhileStatement)
				{
					// TODO: check condition;
					BuildLocalScope(node->statement.Obj(), argument);
				}

				ALGORITHM_PROCEDURE_MATCH(ManagedBreakStatement)
				{
					// TODO: check loop or switch
				}

				ALGORITHM_PROCEDURE_MATCH(ManagedContinueStatement)
				{
					// TODO: check loop
				}

				ALGORITHM_PROCEDURE_MATCH(ManagedReturnStatement)
				{
					// TODO: check function return type
				}

				ALGORITHM_PROCEDURE_MATCH(ManagedTryCatchStatement)
				{
					BuildLocalScope(node->tryStatement.Obj(), argument);
					FOREACH(Ptr<ManagedCatchClause>, catchClause, node->catches.Wrap())
					{
						ManagedTypeSymbol* exceptionType=GetTypeSymbol(catchClause->exceptionType, argument);
						// TODO: check derived from System.Exception

						ManagedSymbolBlock* block=new ManagedSymbolBlock(argument.symbolManager);
						block->languageElement=catchClause->exceptionHandler.Obj();
						argument.currentSymbol->Add(block);

						ManagedSymbolVariable* variable=new ManagedSymbolVariable(argument.symbolManager);
						variable->SetName(catchClause->exceptionName);
						variable->type=exceptionType;
						variable->constant=true;
						variable->catchLanguageElement=catchClause.Obj();
						block->Add(variable);

						MAP newArgument(argument, block);
						BuildLocalScope(catchClause->exceptionHandler.Obj(), newArgument);
					}
					if(node->finallyStatement)
					{
						BuildLocalScope(node->finallyStatement.Obj(), argument);
					}
				}

				ALGORITHM_PROCEDURE_MATCH(ManagedThrowStatement)
				{
					// TODO: check try catch
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
					ManagedTypeSymbol* type=0;
					// TODO: check initializer

					ManagedSymbolBlock* block=new ManagedSymbolBlock(argument.symbolManager);
					block->languageElement=node;
					argument.currentSymbol->Add(block);

					ManagedSymbolVariable* variable=new ManagedSymbolVariable(argument.symbolManager);
					variable->SetName(node->name);
					variable->type=type;
					variable->constant=true;
					variable->usingLanguageElement=node;
					block->Add(variable);

					MAP newArgument(argument, block);
					BuildLocalScope(node->statement.Obj(), newArgument);
				}

				ALGORITHM_PROCEDURE_MATCH(ManagedLockStatement)
				{
					// TODO: check lock;
					BuildLocalScope(node->statement.Obj(), argument);
				}

				ALGORITHM_PROCEDURE_MATCH(ManagedSelectStatement)
				{
					// TODO: check expression;
					FOREACH(Ptr<ManagedCaseClause>, caseClause, node->cases.Wrap())
					{
						// TODO: check conditions
						BuildLocalScope(caseClause->statement.Obj(), argument);
					}
					if(node->defaultStatement)
					{
						BuildLocalScope(node->defaultStatement.Obj(), argument);
					}
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
					// TODO: check condition
					// TODO: check side effects
					BuildLocalScope(node->statement.Obj(), newArgument);
				}

				ALGORITHM_PROCEDURE_MATCH(ManagedForEachStatement)
				{
					ManagedTypeSymbol* type=0;
					// TODO: check type
					// TODO: check container

					ManagedSymbolBlock* block=new ManagedSymbolBlock(argument.symbolManager);
					block->languageElement=node;
					argument.currentSymbol->Add(block);

					ManagedSymbolVariable* variable=new ManagedSymbolVariable(argument.symbolManager);
					variable->SetName(node->name);
					variable->type=type;
					variable->constant=true;
					variable->forEachLanguageElement=node;
					block->Add(variable);

					MAP newArgument(argument, block);
					BuildLocalScope(node->statement.Obj(), newArgument);
				}

			END_ALGORITHM_PROCEDURE(ManagedLanguage_BuildLocalScopeInternal_ExtendedStatement)
		}
	}
}