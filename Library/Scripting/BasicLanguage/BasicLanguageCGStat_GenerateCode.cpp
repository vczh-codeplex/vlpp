#include "BasicLanguageCodeGeneration.h"

namespace vl
{
	namespace scripting
	{
		namespace basiclanguage
		{
			using namespace basicil;
			using namespace stream;

/***********************************************************************
BasicLanguage_GenerateCode
***********************************************************************/

			BEGIN_ALGORITHM_PROCEDURE(BasicLanguage_GenerateCodeInternal, BasicStatement, BCP)
				BASIC_LANGUAGE_ALGORITHM_INITIALIZER

				ALGORITHM_PROCEDURE_MATCH(BasicEmptyStatement)
				{
				}

				ALGORITHM_PROCEDURE_MATCH(BasicCompositeStatement)
				{
					argument.info->EnterScope();
					for(vint i=0;i<node->statements.Count();i++)
					{
						BasicLanguage_GenerateCode(node->statements[i], argument);
					}
					argument.info->LeaveScope();
				}

				ALGORITHM_PROCEDURE_MATCH(BasicExpressionStatement)
				{
					BasicLanguage_RunSideEffect(node->expression, argument);
				}

				ALGORITHM_PROCEDURE_MATCH(BasicVariableStatement)
				{
					BasicScope* scope=argument.info->GetEnv()->GetStatementScope(node);
					BasicTypeRecord* type=scope->variables.Items()[node->name].type;
					BasicOffset size=argument.info->GetTypeInfo(type)->size;
					BasicOffset offset=argument.info->UseVariable(size);
					argument.info->GetLocalVariableOffsets().Add(node, offset);
					if(node->initializer)
					{
						BasicLanguage_StoreToAddress(node->initializer.Obj(), node, argument);
					}
				}

				ALGORITHM_PROCEDURE_MATCH(BasicIfStatement)
				{
					BasicLanguage_PushValue(node->condition, argument, argument.info->GetTypeManager()->GetPrimitiveType(bool_type));
					argument.Ins(BasicIns::jumpfalse, BasicIns::MakeInt(0));
					vint jumpToFalseIndex=argument.il->instructions.Count()-1;
					BasicLanguage_GenerateCode(node->trueStatement, argument);
					if(node->falseStatement)
					{
						argument.Ins(BasicIns::jump, BasicIns::MakeInt(0));
						vint jumpToEndIndex=argument.il->instructions.Count()-1;
						argument.il->instructions[jumpToFalseIndex].argument.int_value=argument.il->instructions.Count();
						BasicLanguage_GenerateCode(node->falseStatement, argument);
						argument.il->instructions[jumpToEndIndex].argument.int_value=argument.il->instructions.Count();
					}
					else
					{
						argument.il->instructions[jumpToFalseIndex].argument.int_value=argument.il->instructions.Count();
					}
				}

				ALGORITHM_PROCEDURE_MATCH(BasicWhileStatement)
				{
					vint continueBegin=argument.il->instructions.Count();
					argument.info->EnterLoop();
					if(node->beginCondition)
					{
						BasicLanguage_PushValue(node->beginCondition, argument, argument.info->GetTypeManager()->GetPrimitiveType(bool_type));
						argument.Ins(BasicIns::jumpfalse, BasicIns::MakeInt(0));
						argument.info->AssociateBreak(argument.il->instructions.Count()-1);
					}
					BasicLanguage_GenerateCode(node->statement, argument);
					if(node->endCondition)
					{
						BasicLanguage_PushValue(node->endCondition, argument, argument.info->GetTypeManager()->GetPrimitiveType(bool_type));
						argument.Ins(BasicIns::jumpfalse, BasicIns::MakeInt(0));
						argument.info->AssociateBreak(argument.il->instructions.Count()-1);
					}
					argument.Ins(BasicIns::jump, BasicIns::MakeInt(0));
					argument.info->AssociateContinue(argument.il->instructions.Count()-1);
					vint breakBegin=argument.il->instructions.Count();
					argument.info->LeaveLoop(breakBegin, continueBegin, argument.il);
				}

				ALGORITHM_PROCEDURE_MATCH(BasicForStatement)
				{
					argument.info->EnterScope();
					if(node->initializer)
					{
						BasicLanguage_GenerateCode(node->initializer, argument);
					}
					vint loopBegin=argument.il->instructions.Count();
					argument.info->EnterLoop();
					BasicLanguage_PushValue(node->condition, argument, argument.info->GetTypeManager()->GetPrimitiveType(bool_type));
					argument.Ins(BasicIns::jumpfalse, BasicIns::MakeInt(0));
					argument.info->AssociateBreak(argument.il->instructions.Count()-1);
					BasicLanguage_GenerateCode(node->statement, argument);
					vint continueBegin=argument.il->instructions.Count();
					if(node->sideEffect)
					{
						BasicLanguage_GenerateCode(node->sideEffect, argument);
					}
					argument.Ins(BasicIns::jump, BasicIns::MakeInt(loopBegin));
					vint breakBegin=argument.il->instructions.Count();
					argument.info->LeaveLoop(breakBegin, continueBegin, argument.il);
					argument.info->LeaveScope();
				}

				ALGORITHM_PROCEDURE_MATCH(BasicBreakStatement)
				{
					argument.Ins(BasicIns::jump, BasicIns::MakeInt(0));
					argument.info->AssociateBreak(argument.il->instructions.Count()-1);
				}

				ALGORITHM_PROCEDURE_MATCH(BasicContinueStatement)
				{
					argument.Ins(BasicIns::jump, BasicIns::MakeInt(0));
					argument.info->AssociateContinue(argument.il->instructions.Count()-1);
				}

				ALGORITHM_PROCEDURE_MATCH(BasicReturnStatement)
				{
					argument.Ins(BasicIns::jump, BasicIns::MakeInt(0));
					argument.info->AssociateReturn(argument.il->instructions.Count()-1);
				}

				ALGORITHM_PROCEDURE_MATCH(BasicTryCatchStatement)
				{
					vint catchIndex=argument.il->instructions.Count();
					argument.Ins(BasicIns::exception_handler_push, BasicIns::MakeInt(0));
					BasicLanguage_GenerateCode(node->tryStatement, argument);
					argument.Ins(BasicIns::exception_handler_pop);
					vint jumpIndex=argument.il->instructions.Count();
					argument.Ins(BasicIns::jump, BasicIns::MakeInt(0));
					argument.il->instructions[catchIndex].argument.int_value=argument.il->instructions.Count();

					argument.Ins(BasicIns::exception_handler_pop);
					BasicLanguage_GenerateCode(node->catchStatement, argument);
					argument.il->instructions[jumpIndex].argument.int_value=argument.il->instructions.Count();
				}

				ALGORITHM_PROCEDURE_MATCH(BasicThrowStatement)
				{
					if(node->expression)
					{
						vint reserveIndex=argument.il->instructions.Count();
						argument.Ins(BasicIns::exception_object_reserve, BasicIns::MakeInt(0));
						BasicTypeRecord* type=BasicLanguage_PushValue(node->expression, argument);
						argument.Ins(BasicIns::exception_object_address);
						Code_Write(type, argument);
						
						{
							BasicIns& ins=argument.il->instructions[reserveIndex];
							BasicOffset size=argument.info->GetTypeInfo(type)->size;
							if(size.IsConstant())
							{
								ins.argument.int_value=size.Constant();
							}
							else
							{
								ins.argumentType=BasicIns::linearArgument;
								ins.argument.int_value=argument.info->RegisterLinear(size);
							}
						}
					}
					argument.Ins(BasicIns::exception_raise);
				}

			END_ALGORITHM_PROCEDURE(BasicLanguage_GenerateCodeInternal)

			void BasicLanguage_GenerateCode(BasicStatement* node, const BCP& argument)
			{
				argument.info->EnterSemanticScope(argument.info->GetEnv()->GetStatementScope(node));
				BasicLanguage_GenerateCodeInternal(node, argument);
				argument.info->LeaveSemanticScope();
			}

			void BasicLanguage_GenerateCode(Ptr<BasicStatement> node, const BCP& argument)
			{
				BasicLanguage_GenerateCode(node.Obj(), argument);
			}
		}
	}
}