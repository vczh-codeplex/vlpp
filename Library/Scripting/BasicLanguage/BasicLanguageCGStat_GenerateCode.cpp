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

			BEGIN_ALGORITHM_PROCEDURE(BasicLanguage_GenerateCode, BasicStatement, BCP)
				BASIC_LANGUAGE_ALGORITHM_INITIALIZER

				ALGORITHM_PROCEDURE_MATCH(BasicEmptyStatement)
				{
				}

				ALGORITHM_PROCEDURE_MATCH(BasicCompositeStatement)
				{
					argument.info->EnterScope();
					for(int i=0;i<node->statements.Count();i++)
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
					int size=argument.info->GetTypeInfo(type)->size;
					int offset=argument.info->UseVariable(size);
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
					int jumpToFalseIndex=argument.il->instructions.Count()-1;
					BasicLanguage_GenerateCode(node->trueStatement, argument);
					if(node->falseStatement)
					{
						argument.Ins(BasicIns::jump, BasicIns::MakeInt(0));
						int jumpToEndIndex=argument.il->instructions.Count()-1;
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
					int continueBegin=argument.il->instructions.Count();
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
					int breakBegin=argument.il->instructions.Count();
					argument.info->LeaveLoop(breakBegin, continueBegin, argument.il);
				}

				ALGORITHM_PROCEDURE_MATCH(BasicForStatement)
				{
					argument.info->EnterScope();
					if(node->initializer)
					{
						BasicLanguage_GenerateCode(node->initializer, argument);
					}
					int loopBegin=argument.il->instructions.Count();
					argument.info->EnterLoop();
					BasicLanguage_PushValue(node->condition, argument, argument.info->GetTypeManager()->GetPrimitiveType(bool_type));
					argument.Ins(BasicIns::jumpfalse, BasicIns::MakeInt(0));
					argument.info->AssociateBreak(argument.il->instructions.Count()-1);
					BasicLanguage_GenerateCode(node->statement, argument);
					int continueBegin=argument.il->instructions.Count();
					if(node->sideEffect)
					{
						BasicLanguage_GenerateCode(node->sideEffect, argument);
					}
					argument.Ins(BasicIns::jump, BasicIns::MakeInt(loopBegin));
					int breakBegin=argument.il->instructions.Count();
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

				ALGORITHM_PROCEDURE_MATCH(BasicExtendedStatement)
				{
					argument.codegenExtension->GenerateCode(node, argument);
				}

			END_ALGORITHM_PROCEDURE(BasicLanguage_GenerateCode)
		}
	}
}