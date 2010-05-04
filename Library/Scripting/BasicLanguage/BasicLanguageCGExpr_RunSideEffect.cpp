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
BasicLanguage_RunSideEffect
***********************************************************************/

			BEGIN_ALGORITHM_PROCEDURE(BasicLanguage_RunSideEffect, BasicExpression, BCP)

				ALGORITHM_PROCEDURE_MATCH(BasicNullExpression)
				{
				}

				ALGORITHM_PROCEDURE_MATCH(BasicNumericExpression)
				{
				}

				ALGORITHM_PROCEDURE_MATCH(BasicMbcsStringExpression)
				{
				}

				ALGORITHM_PROCEDURE_MATCH(BasicUnicodeStringExpression)
				{
				}

				ALGORITHM_PROCEDURE_MATCH(BasicUnaryExpression)
				{
					BasicTypeRecord* nodeType=argument.info->GetEnv()->GetExpressionType(node);
					BasicTypeRecord* operandType=argument.info->GetEnv()->GetExpressionType(node->operand.Obj());
					switch(node->type)
					{
					case BasicUnaryExpression::PrefixIncrease:
					case BasicUnaryExpression::PostfixIncrease:
						{
							Code_ScaleAdder(operandType, argument, true);
							BasicLanguage_PushValue(node->operand, argument);
							argument.il->Ins(BasicIns::add, Convert(operandType));
							BasicLanguage_PushRef(node->operand, argument);
							argument.il->Ins(BasicIns::write, Convert(operandType));
						}
						break;
					case BasicUnaryExpression::PrefixDecrease:
					case BasicUnaryExpression::PostfixDecrease:
						{
							Code_ScaleAdder(operandType, argument, true);
							BasicLanguage_PushValue(node->operand, argument);
							argument.il->Ins(BasicIns::sub, Convert(operandType));
							BasicLanguage_PushRef(node->operand, argument);
							argument.il->Ins(BasicIns::write, Convert(operandType));
						}
						break;
					default:
						BasicLanguage_RunSideEffect(node->operand, argument);
					}
				}

				ALGORITHM_PROCEDURE_MATCH(BasicBinaryExpression)
				{
					BasicTypeRecord* leftType=argument.info->GetEnv()->GetExpressionType(node->leftOperand.Obj());
					BasicTypeRecord* rightType=argument.info->GetEnv()->GetExpressionType(node->rightOperand.Obj());

					switch(node->type)
					{
					case BasicBinaryExpression::AddAssign:
						if(leftType->GetType()==BasicTypeRecord::Pointer)
						{
							BasicLanguage_PushValue(node->rightOperand, argument, leftType);
							Code_ScaleAdder(leftType, argument, false);
							BasicLanguage_PushValue(node->leftOperand, argument, leftType);
							argument.il->Ins(BasicIns::add, Convert(leftType));
							BasicLanguage_PushRef(node->leftOperand, argument);
							Code_Write(leftType, argument);
						}
						else
						{
							Code_BinaryAssignSideEffect(node, argument, BasicIns::add);
						}
						break;
					case BasicBinaryExpression::SubAssign:
						if(leftType->GetType()==BasicTypeRecord::Pointer)
						{
							BasicLanguage_PushValue(node->rightOperand, argument, leftType);
							Code_ScaleAdder(leftType, argument, false);
							BasicLanguage_PushValue(node->leftOperand, argument, leftType);
							argument.il->Ins(BasicIns::sub, Convert(leftType));
							BasicLanguage_PushRef(node->leftOperand, argument);
							Code_Write(leftType, argument);
						}
						else
						{
							Code_BinaryAssignSideEffect(node, argument, BasicIns::sub);
						}
						break;
					case BasicBinaryExpression::MulAssign:
						Code_BinaryAssignSideEffect(node, argument, BasicIns::mul);
						break;
					case BasicBinaryExpression::DivAssign:
						Code_BinaryAssignSideEffect(node, argument, BasicIns::div);
						break;
					case BasicBinaryExpression::ModAssign:
						Code_BinaryAssignSideEffect(node, argument, BasicIns::mod);
						break;
					case BasicBinaryExpression::ShlAssign:
						Code_BinaryAssignSideEffect(node, argument, BasicIns::shl);
						break;
					case BasicBinaryExpression::ShrAssign:
						Code_BinaryAssignSideEffect(node, argument, BasicIns::shr);
						break;
					case BasicBinaryExpression::AndAssign:
					case BasicBinaryExpression::BitAndAssign:
						Code_BinaryAssignSideEffect(node, argument, BasicIns::and);
						break;
					case BasicBinaryExpression::OrAssign:
					case BasicBinaryExpression::BitOrAssign:
						Code_BinaryAssignSideEffect(node, argument, BasicIns::or);
						break;
					case BasicBinaryExpression::XorAssign:
						Code_BinaryAssignSideEffect(node, argument, BasicIns::xor);
						break;
					case BasicBinaryExpression::Assign:
						{
							BasicLanguage_StoreToAddress(node->rightOperand.Obj(), node->leftOperand.Obj(), argument);
						}
						break;
					default:
						BasicLanguage_RunSideEffect(node->leftOperand, argument);
						BasicLanguage_RunSideEffect(node->rightOperand, argument);
					}
				}

				ALGORITHM_PROCEDURE_MATCH(BasicSubscribeExpression)
				{
					BasicLanguage_RunSideEffect(node->operand, argument);
					BasicLanguage_RunSideEffect(node->subscribe, argument);
				}

				ALGORITHM_PROCEDURE_MATCH(BasicMemberExpression)
				{
					BasicLanguage_RunSideEffect(node->operand, argument);
				}

				ALGORITHM_PROCEDURE_MATCH(BasicInvokeExpression)
				{
					Code_InvokeFunction(node, argument, true);
				}

				ALGORITHM_PROCEDURE_MATCH(BasicFunctionResultExpression)
				{
				}

				ALGORITHM_PROCEDURE_MATCH(BasicCastingExpression)
				{
					BasicLanguage_RunSideEffect(node->operand, argument);
				}

				ALGORITHM_PROCEDURE_MATCH(BasicReferenceExpression)
				{
				}

				ALGORITHM_PROCEDURE_MATCH(BasicExtendedExpression)
				{
					argument.codegenExtension->RunSideEffect(node, argument);
				}

			END_ALGORITHM_PROCEDURE(BasicLanguage_RunSideEffect)
		}
	}
}