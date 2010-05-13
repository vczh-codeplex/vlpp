#include "BasicLanguageCodeGeneration.h"
#include "BasicLanguageResource.h"

namespace vl
{
	namespace scripting
	{
		namespace basiclanguage
		{
			using namespace basicil;
			using namespace stream;

/***********************************************************************
BasicLanguage_CanPushRefWithoutSideEffect
***********************************************************************/

			BEGIN_ALGORITHM_FUNCTION(BasicLanguage_CanPushRefWithoutSideEffect, BasicExpression, BCP, bool)

				ALGORITHM_FUNCTION_MATCH(BasicNullExpression)
				{
					return false;
				}

				ALGORITHM_FUNCTION_MATCH(BasicNumericExpression)
				{
					return false;
				}

				ALGORITHM_FUNCTION_MATCH(BasicMbcsStringExpression)
				{
					return false;
				}

				ALGORITHM_FUNCTION_MATCH(BasicUnicodeStringExpression)
				{
					return false;
				}

				ALGORITHM_FUNCTION_MATCH(BasicUnaryExpression)
				{
					switch(node->type)
					{
					case BasicUnaryExpression::PrefixIncrease:
					case BasicUnaryExpression::PrefixDecrease:
						return BasicLanguage_CanPushRefWithoutSideEffect(node->operand, argument);
					default:
						return false;
					}
				}

				ALGORITHM_FUNCTION_MATCH(BasicBinaryExpression)
				{
					switch(node->type)
					{
					case BasicBinaryExpression::AddAssign:
					case BasicBinaryExpression::SubAssign:
					case BasicBinaryExpression::MulAssign:
					case BasicBinaryExpression::DivAssign:
					case BasicBinaryExpression::ModAssign:
					case BasicBinaryExpression::ShlAssign:
					case BasicBinaryExpression::ShrAssign:
					case BasicBinaryExpression::AndAssign:
					case BasicBinaryExpression::BitAndAssign:
					case BasicBinaryExpression::OrAssign:
					case BasicBinaryExpression::BitOrAssign:
					case BasicBinaryExpression::XorAssign:
					case BasicBinaryExpression::Assign:
						return BasicLanguage_CanPushRefWithoutSideEffect(node->leftOperand, argument);
					default:
						return false;
					}
				}

				ALGORITHM_FUNCTION_MATCH(BasicSubscribeExpression)
				{
					BasicTypeRecord* type=argument.info->GetEnv()->GetExpressionType(node->operand.Obj());
					if(type->GetType()==BasicTypeRecord::Array)
					{
						return BasicLanguage_CanPushRefWithoutSideEffect(node->operand, argument) && dynamic_cast<BasicNumericExpression*>(node->subscribe.Obj())!=0;
					}
					else
					{
						return false;
					}
				}

				ALGORITHM_FUNCTION_MATCH(BasicMemberExpression)
				{
					return !node->pointerMember && BasicLanguage_CanPushRefWithoutSideEffect(node->operand, argument);
				}

				ALGORITHM_FUNCTION_MATCH(BasicInvokeExpression)
				{
					return false;
				}

				ALGORITHM_FUNCTION_MATCH(BasicFunctionResultExpression)
				{
					return true;
				}

				ALGORITHM_FUNCTION_MATCH(BasicCastingExpression)
				{
					return false;
				}

				ALGORITHM_FUNCTION_MATCH(BasicReferenceExpression)
				{
					BasicTypeRecord* nodeType=argument.info->GetEnv()->GetExpressionType(node);
					BasicEnv::Reference reference=argument.info->GetEnv()->GetReference(node);
					if(reference.isVariable)
					{
						return true;
					}
					else
					{
						return false;
					}
				}

				ALGORITHM_FUNCTION_MATCH(BasicExtendedExpression)
				{
					return argument.codegenExtension->CanPushRefWithoutSideEffect(node, argument);
				}

			END_ALGORITHM_FUNCTION(BasicLanguage_CanPushRefWithoutSideEffect)

/***********************************************************************
BasicLanguage_PushRefWithoutSideEffect
***********************************************************************/

			BEGIN_ALGORITHM_PROCEDURE(BasicLanguage_PushRefWithoutSideEffect, BasicExpression, BCP)

				ALGORITHM_PROCEDURE_MATCH(BasicNullExpression)
				{
					CHECK_ERROR(false, L"BasicLanguage_PushRefWithoutSideEffect(BasicUnaryExpression*, const BCP&)#不支持此操作。");
				}

				ALGORITHM_PROCEDURE_MATCH(BasicNumericExpression)
				{
					CHECK_ERROR(false, L"BasicLanguage_PushRefWithoutSideEffect(BasicUnaryExpression*, const BCP&)#不支持此操作。");
				}

				ALGORITHM_PROCEDURE_MATCH(BasicMbcsStringExpression)
				{
					CHECK_ERROR(false, L"BasicLanguage_PushRefWithoutSideEffect(BasicUnaryExpression*, const BCP&)#不支持此操作。");
				}

				ALGORITHM_PROCEDURE_MATCH(BasicUnicodeStringExpression)
				{
					CHECK_ERROR(false, L"BasicLanguage_PushRefWithoutSideEffect(BasicUnaryExpression*, const BCP&)#不支持此操作。");
				}

				ALGORITHM_PROCEDURE_MATCH(BasicUnaryExpression)
				{
					BasicLanguage_PushRefWithoutSideEffect(node->operand, argument);
				}

				ALGORITHM_PROCEDURE_MATCH(BasicBinaryExpression)
				{
					BasicLanguage_PushRefWithoutSideEffect(node->leftOperand, argument);
				}

				ALGORITHM_PROCEDURE_MATCH(BasicSubscribeExpression)
				{
					BasicTypeRecord* nodeType=argument.info->GetEnv()->GetExpressionType(node);
					BasicTypeRecord* operandType=argument.info->GetEnv()->GetExpressionType(node->operand.Obj());
					if(operandType->GetType()==BasicTypeRecord::Pointer)
					{
						CHECK_ERROR(false, L"BasicLanguage_PushRefWithoutSideEffect(BasicUnaryExpression*, const BCP&)#不支持此操作。");
					}
					else
					{
						BasicLanguage_PushRefWithoutSideEffect(node->operand, argument);
						operandType=argument.info->GetTypeManager()->GetPointerType(nodeType);
					}
					BasicLanguage_PushValue(node->subscribe, argument, argument.info->GetTypeManager()->GetPrimitiveType(int_type));
					Code_ScaleAdder(operandType, argument, false);
					argument.il->Ins(BasicIns::add, BasicIns::int_type);
				}

				ALGORITHM_PROCEDURE_MATCH(BasicMemberExpression)
				{
					if(node->pointerMember)
					{
						CHECK_ERROR(false, L"BasicLanguage_PushRefWithoutSideEffect(BasicUnaryExpression*, const BCP&)#不支持此操作。");
					}
					BasicTypeRecord* structureType=argument.info->GetEnv()->GetExpressionType(node->operand.Obj());
					BasicTypeInfo* structureInfo=argument.info->GetTypeInfo(structureType);
					int offset=structureInfo->offsets[structureType->MemberNameIndex(node->member)];
					BasicLanguage_PushRefWithoutSideEffect(node->operand, argument);
					argument.il->Ins(BasicIns::push, BasicIns::int_type, BasicIns::MakeInt(offset));
					argument.il->Ins(BasicIns::add, BasicIns::int_type);
				}

				ALGORITHM_PROCEDURE_MATCH(BasicInvokeExpression)
				{
					CHECK_ERROR(false, L"BasicLanguage_PushRefWithoutSideEffect(BasicUnaryExpression*, const BCP&)#不支持此操作。");
				}

				ALGORITHM_PROCEDURE_MATCH(BasicFunctionResultExpression)
				{
					BasicLanguage_PushRef(node, argument);
				}

				ALGORITHM_PROCEDURE_MATCH(BasicCastingExpression)
				{
					CHECK_ERROR(false, L"BasicLanguage_PushRefWithoutSideEffect(BasicUnaryExpression*, const BCP&)#不支持此操作。");
				}

				ALGORITHM_PROCEDURE_MATCH(BasicReferenceExpression)
				{
					BasicLanguage_PushRef(node, argument);
				}

				ALGORITHM_PROCEDURE_MATCH(BasicExtendedExpression)
				{
					argument.codegenExtension->PushRefWithoutSideEffect(node, argument);
				}

			END_ALGORITHM_PROCEDURE(BasicLanguage_PushRefWithoutSideEffect)
		}
	}
}