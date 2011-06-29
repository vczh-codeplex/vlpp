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
				BASIC_LANGUAGE_ALGORITHM_INITIALIZER

				ALGORITHM_PROCEDURE_MATCH(BasicNullExpression)
				{
				}

				ALGORITHM_PROCEDURE_MATCH(BasicExceptionAddressExpression)
				{
				}

				ALGORITHM_PROCEDURE_MATCH(BasicStackDataAddressExpression)
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
					BasicTypeRecord* operandType=argument.info->GetEnv()->GetExpressionType(node->operand.Obj());
					BasicTypeRecord* pointerType=argument.info->GetTypeManager()->GetPointerType(operandType);
					BasicOffset size=argument.info->GetTypeInfo(operandType)->size;
					BasicOffset pointerSize=argument.info->GetTypeInfo(pointerType)->size;
					switch(node->type)
					{
					case BasicUnaryExpression::PrefixIncrease:
					case BasicUnaryExpression::PostfixIncrease:
						{
							if(BasicLanguage_CanPushRefWithoutSideEffect(node->operand, argument))
							{
								Code_ScaleAdder(operandType, argument, true);
								BasicLanguage_PushValue(node->operand, argument);
								argument.Ins(BasicIns::add, Convert(operandType));
								BasicLanguage_PushRefWithoutSideEffect(node->operand, argument);
								argument.Ins(BasicIns::write, Convert(operandType));
							}
							else
							{
								BasicLanguage_PushRef(node->operand, argument);
								Code_ScaleAdder(operandType, argument, true);
								Code_CopyAddressInStack(node->operand.Obj(), argument, size);
								Code_Read(operandType, argument);
								argument.Ins(BasicIns::add, Convert(operandType));
								Code_CopyAddressInStack(node->operand.Obj(), argument, size);
								argument.Ins(BasicIns::write, Convert(operandType));
								argument.Ins(BasicIns::stack_reserve, -pointerSize);
							}
						}
						break;
					case BasicUnaryExpression::PrefixDecrease:
					case BasicUnaryExpression::PostfixDecrease:
						{
							if(BasicLanguage_CanPushRefWithoutSideEffect(node->operand, argument))
							{
								Code_ScaleAdder(operandType, argument, true);
								BasicLanguage_PushValue(node->operand, argument);
								argument.Ins(BasicIns::sub, Convert(operandType));
								BasicLanguage_PushRefWithoutSideEffect(node->operand, argument);
								argument.Ins(BasicIns::write, Convert(operandType));
							}
							else
							{
								BasicLanguage_PushRef(node->operand, argument);
								Code_ScaleAdder(operandType, argument, true);
								Code_CopyAddressInStack(node->operand.Obj(), argument, size);
								Code_Read(operandType, argument);
								argument.Ins(BasicIns::sub, Convert(operandType));
								Code_CopyAddressInStack(node->operand.Obj(), argument, size);
								argument.Ins(BasicIns::write, Convert(operandType));
								argument.Ins(BasicIns::stack_reserve, -pointerSize);
							}
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
					BasicTypeRecord* pointerType=argument.info->GetTypeManager()->GetPointerType(leftType);
					BasicOffset size=argument.info->GetTypeInfo(leftType)->size;
					BasicOffset pointerSize=argument.info->GetTypeInfo(pointerType)->size;

					switch(node->type)
					{
					case BasicBinaryExpression::AddAssign:
						if(leftType->GetType()==BasicTypeRecord::Pointer)
						{
							if(BasicLanguage_CanPushRefWithoutSideEffect(node->leftOperand, argument))
							{
								BasicLanguage_PushValue(node->rightOperand, argument, leftType);
								Code_ScaleAdder(leftType, argument, false);
								BasicLanguage_PushValue(node->leftOperand, argument, leftType);
								argument.Ins(BasicIns::add, Convert(leftType));
								BasicLanguage_PushRef(node->leftOperand, argument);
								Code_Write(leftType, argument);
							}
							else
							{
								BasicLanguage_PushRef(node->leftOperand, argument);
								BasicLanguage_PushValue(node->rightOperand, argument, leftType);
								Code_ScaleAdder(leftType, argument, false);
								Code_CopyAddressInStack(node->leftOperand.Obj(), argument, size);
								Code_Read(leftType, argument);
								argument.Ins(BasicIns::add, Convert(leftType));
								Code_CopyAddressInStack(node->leftOperand.Obj(), argument, size);
								Code_Write(leftType, argument);
								argument.Ins(BasicIns::stack_reserve, -pointerSize);
							}
						}
						else
						{
							Code_BinaryAssignSideEffect(node, argument, BasicIns::add);
						}
						break;
					case BasicBinaryExpression::SubAssign:
						if(leftType->GetType()==BasicTypeRecord::Pointer)
						{
							if(BasicLanguage_CanPushRefWithoutSideEffect(node->leftOperand, argument))
							{
								BasicLanguage_PushValue(node->rightOperand, argument, leftType);
								Code_ScaleAdder(leftType, argument, false);
								BasicLanguage_PushValue(node->leftOperand, argument, leftType);
								argument.Ins(BasicIns::sub, Convert(leftType));
								BasicLanguage_PushRef(node->leftOperand, argument);
								Code_Write(leftType, argument);
							}
							else
							{
								BasicLanguage_PushRef(node->leftOperand, argument);
								BasicLanguage_PushValue(node->rightOperand, argument, leftType);
								Code_ScaleAdder(leftType, argument, false);
								Code_CopyAddressInStack(node->leftOperand.Obj(), argument, size);
								Code_Read(leftType, argument);
								argument.Ins(BasicIns::sub, Convert(leftType));
								Code_CopyAddressInStack(node->leftOperand.Obj(), argument, size);
								Code_Write(leftType, argument);
								argument.Ins(BasicIns::stack_reserve, -pointerSize);
							}
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

				ALGORITHM_PROCEDURE_MATCH(BasicSizeofExpression)
				{
				}

				ALGORITHM_PROCEDURE_MATCH(BasicOffsetofExpression)
				{
				}

				ALGORITHM_PROCEDURE_MATCH(BasicCastingExpression)
				{
					BasicLanguage_RunSideEffect(node->operand, argument);
				}

				ALGORITHM_PROCEDURE_MATCH(BasicReferenceExpression)
				{
				}

				ALGORITHM_PROCEDURE_MATCH(BasicInstantiatedExpression)
				{
				}

				ALGORITHM_PROCEDURE_MATCH(BasicInstanceFunctionExpression)
				{
				}

			END_ALGORITHM_PROCEDURE(BasicLanguage_RunSideEffect)
		}
	}
}