#include "BasicLanguageAnalyzer.h"
#include "..\..\Collections\OperationCopyFrom.h"

namespace vl
{
	namespace scripting
	{
		namespace basiclanguage
		{
			using namespace collections;

/***********************************************************************
BasicLanguage_CheckStatement
***********************************************************************/

			void BasicLanguage_CheckStatement(Ptr<BasicStatement>& statement, const BP& argument)
			{
				statement=argument.semanticExtension->ReplaceStatement(statement, argument);
				try
				{
					BasicLanguage_CheckStatementInternal(statement, argument);
					argument.env->RegisterStatementScope(statement.Obj(), argument.scope);
				}
				catch(Ptr<BasicLanguageCodeException> e)
				{
					argument.errors.Add(e);
				}
			}

			BEGIN_ALGORITHM_PROCEDURE(BasicLanguage_CheckStatementInternal, BasicStatement, BP)

				ALGORITHM_PROCEDURE_MATCH(BasicEmptyStatement)
				{
				}

				ALGORITHM_PROCEDURE_MATCH(BasicCompositeStatement)
				{
					BP newArgument(argument, argument.env->CreateStatementScope(argument.scope, node));
					for(vint i=0;i<node->statements.Count();i++)
					{
						BasicLanguage_CheckStatement(node->statements[i], newArgument);
					}
				}

				ALGORITHM_PROCEDURE_MATCH(BasicExpressionStatement)
				{
					BasicLanguage_GetExpressionType(node->expression, argument);
				}

				ALGORITHM_PROCEDURE_MATCH(BasicVariableStatement)
				{
					bool wrong=false;
					BasicTypeRecord* type=0;

					if(argument.scope->variables.Items().Keys().Contains(node->name))
					{
						argument.errors.Add(BasicLanguageCodeException::GetVariableAlreadyExists(node));
						wrong=true;
					}

					try
					{
						type=BasicLanguage_GetTypeRecord(node->type, argument, false);
					}
					catch(Ptr<BasicLanguageCodeException> e)
					{
						argument.errors.Add(e);
						wrong=true;
					}

					if(node->initializer)
					{
						BasicTypeRecord* initializerType=BasicLanguage_GetExpressionType(node->initializer, argument);
						if(type && initializerType)
						{
							if(!CanImplicitConvertTo(initializerType, type, node->initializer.Obj(), argument))
							{
								argument.errors.Add(BasicLanguageCodeException::GetInitializerTypeNotMatch(node));
							}
						}
					}

					if(!wrong)
					{
						argument.scope->variables.Add(node->name, BasicScope::Variable(node, type));
					}
				}

				ALGORITHM_PROCEDURE_MATCH(BasicIfStatement)
				{
					BasicTypeRecord* conditionType=BasicLanguage_GetExpressionType(node->condition, argument);
					if(conditionType && !CanImplicitConvertTo(conditionType, argument.typeManager->GetPrimitiveType(bool_type), node->condition.Obj(), argument))
					{
						argument.errors.Add(BasicLanguageCodeException::GetConditionCannotConvertToBool(node->condition.Obj()));
					}
					BasicLanguage_CheckStatement(node->trueStatement, argument);
					if(node->falseStatement)
					{
						BasicLanguage_CheckStatement(node->falseStatement, argument);
					}
				}

				ALGORITHM_PROCEDURE_MATCH(BasicWhileStatement)
				{
					if(node->beginCondition)
					{
						BasicTypeRecord* beginConditionType=BasicLanguage_GetExpressionType(node->beginCondition, argument);
						if(beginConditionType && !CanImplicitConvertTo(beginConditionType, argument.typeManager->GetPrimitiveType(bool_type), node->beginCondition.Obj(), argument))
						{
							argument.errors.Add(BasicLanguageCodeException::GetConditionCannotConvertToBool(node->beginCondition.Obj()));
						}
					}
					if(node->endCondition)
					{
						BasicTypeRecord* endConditionType=BasicLanguage_GetExpressionType(node->endCondition, argument);
						if(endConditionType && !CanImplicitConvertTo(endConditionType, argument.typeManager->GetPrimitiveType(bool_type), node->endCondition.Obj(), argument))
						{
							argument.errors.Add(BasicLanguageCodeException::GetConditionCannotConvertToBool(node->endCondition.Obj()));
						}
					}
					BP newArgument(argument, argument.env->CreateStatementScope(argument.scope, node));
					BasicLanguage_CheckStatement(node->statement, newArgument);
				}

				ALGORITHM_PROCEDURE_MATCH(BasicForStatement)
				{
					BP newArgument(argument, argument.env->CreateStatementScope(argument.scope, node));
					if(node->initializer)
					{
						Ptr<BasicCompositeStatement> composite=node->initializer.Cast<BasicCompositeStatement>();
						if(composite)
						{
							for(vint i=0;i<composite->statements.Count();i++)
							{
								BasicLanguage_CheckStatement(composite->statements[i], newArgument);
							}
						}
						else
						{
							BasicLanguage_CheckStatement(node->initializer, newArgument);
						}
					}
					BasicTypeRecord* conditionType=BasicLanguage_GetExpressionType(node->condition, newArgument);
					if(conditionType && !CanImplicitConvertTo(conditionType, argument.typeManager->GetPrimitiveType(bool_type), node->condition.Obj(), argument))
					{
						argument.errors.Add(BasicLanguageCodeException::GetConditionCannotConvertToBool(node->condition.Obj()));
					}
					BasicLanguage_CheckStatement(node->statement, newArgument);
					if(node->sideEffect)
					{
						BasicLanguage_CheckStatement(node->sideEffect, newArgument);
					}
				}

				ALGORITHM_PROCEDURE_MATCH(BasicBreakStatement)
				{
					BasicScope* scope=argument.scope;
					while(scope->OwnerStatement())
					{
						if(dynamic_cast<BasicWhileStatement*>(scope->OwnerStatement()) || dynamic_cast<BasicForStatement*>(scope->OwnerStatement()))
						{
							return;
						}
						scope=scope->PreviousScope();
					}
					argument.errors.Add(BasicLanguageCodeException::GetBreakShouldBeInLooping(node));
				}

				ALGORITHM_PROCEDURE_MATCH(BasicContinueStatement)
				{
					BasicScope* scope=argument.scope;
					while(scope->OwnerStatement())
					{
						if(dynamic_cast<BasicWhileStatement*>(scope->OwnerStatement()) || dynamic_cast<BasicForStatement*>(scope->OwnerStatement()))
						{
							return;
						}
						scope=scope->PreviousScope();
					}
					argument.errors.Add(BasicLanguageCodeException::GetContinueShouldBeInLooping(node));
				}

				ALGORITHM_PROCEDURE_MATCH(BasicReturnStatement)
				{
				}

				ALGORITHM_PROCEDURE_MATCH(BasicTryCatchStatement)
				{
					BasicLanguage_CheckStatement(node->tryStatement, argument);
					BasicLanguage_CheckStatement(node->catchStatement, argument);
				}

				ALGORITHM_PROCEDURE_MATCH(BasicThrowStatement)
				{
					if(node->expression)
					{
						BasicTypeRecord* expressionType=BasicLanguage_GetExpressionType(node->expression, argument);
						if(expressionType==argument.typeManager->GetPrimitiveType(void_type))
						{
							argument.errors.Add(BasicLanguageCodeException::GetCannotThrowVoidValue(node));
						}
					}
				}

				ALGORITHM_PROCEDURE_MATCH(BasicExtendedStatement)
				{
					argument.semanticExtension->CheckStatement(node, argument);
				}

			END_ALGORITHM_PROCEDURE(BasicLanguage_CheckStatementInternal)
		}
	}
}