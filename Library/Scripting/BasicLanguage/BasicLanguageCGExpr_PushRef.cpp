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
BasicLanguage_PushRef
***********************************************************************/

			BEGIN_ALGORITHM_PROCEDURE(BasicLanguage_PushRef, BasicExpression, BCP)

				ALGORITHM_PROCEDURE_MATCH(BasicNullExpression)
				{
					CHECK_ERROR(false, L"BasicLanguage_PushRef(BasicNullExpression*, const BCP&)#不支持此操作。");
				}

				ALGORITHM_PROCEDURE_MATCH(BasicNumericExpression)
				{
					CHECK_ERROR(false, L"BasicLanguage_PushRef(BasicNumericExpression*, const BCP&)#不支持此操作。");
				}

				ALGORITHM_PROCEDURE_MATCH(BasicMbcsStringExpression)
				{
					CHECK_ERROR(false, L"BasicLanguage_PushRef(BasicMbcsStringExpression*, const BCP&)#不支持此操作。");
				}

				ALGORITHM_PROCEDURE_MATCH(BasicUnicodeStringExpression)
				{
					CHECK_ERROR(false, L"BasicLanguage_PushRef(BasicUnicodeStringExpression*, const BCP&)#不支持此操作。");
				}

				ALGORITHM_PROCEDURE_MATCH(BasicUnaryExpression)
				{
					BasicTypeRecord* operandType=argument.info->GetEnv()->GetExpressionType(node->operand.Obj());
					int operandSize=argument.info->GetTypeInfo(operandType)->size;
					switch(node->type)
					{
					case BasicUnaryExpression::PrefixIncrease:
						{
							BasicLanguage_PushRef(node->operand, argument);
							Code_ScaleAdder(operandType, argument, true);
							BasicLanguage_PushValue(node->operand, argument);
							argument.il->Ins(BasicIns::add, Convert(operandType));
							Code_CopyAddressInStack(node->operand.Obj(), argument, operandSize);
							argument.il->Ins(BasicIns::write, Convert(operandType));
						}
						break;
					case BasicUnaryExpression::PrefixDecrease:
						{
							BasicLanguage_PushRef(node->operand, argument);
							Code_ScaleAdder(operandType, argument, true);
							BasicLanguage_PushValue(node->operand, argument);
							argument.il->Ins(BasicIns::sub, Convert(operandType));
							Code_CopyAddressInStack(node->operand.Obj(), argument, operandSize);
							argument.il->Ins(BasicIns::write, Convert(operandType));
						}
						break;
					case BasicUnaryExpression::DereferencePointer:
						BasicLanguage_PushValue(node->operand, argument);
						break;
					default:
						CHECK_ERROR(false, L"BasicLanguage_PushRef(BasicUnaryExpression*, const BCP&)#不支持此操作。");
					}
				}

				ALGORITHM_PROCEDURE_MATCH(BasicBinaryExpression)
				{
					BasicTypeRecord* leftType=argument.info->GetEnv()->GetExpressionType(node->leftOperand.Obj());
					BasicTypeRecord* pointerType=argument.info->GetTypeManager()->GetPointerType(leftType);
					int leftSize=argument.info->GetTypeInfo(leftType)->size;
					switch(node->type)
					{
					case BasicBinaryExpression::AddAssign:
						if(leftType->GetType()==BasicTypeRecord::Pointer)
						{
							BasicLanguage_PushRef(node->leftOperand, argument);
							BasicLanguage_PushValue(node->rightOperand, argument, leftType);
							Code_ScaleAdder(leftType, argument, false);
							Code_CopyAddressInStack(node->leftOperand.Obj(), argument, leftSize);
							Code_Read(leftType, argument);
							argument.il->Ins(BasicIns::add, Convert(leftType));
							Code_CopyAddressInStack(node->leftOperand.Obj(), argument, leftSize);
							Code_Write(leftType, argument);
						}
						else
						{
							Code_BinaryAssignRef(node, argument, BasicIns::add);
						}
						break;
					case BasicBinaryExpression::SubAssign:
						if(leftType->GetType()==BasicTypeRecord::Pointer)
						{
							BasicLanguage_PushRef(node->leftOperand, argument);
							BasicLanguage_PushValue(node->rightOperand, argument, leftType);
							Code_ScaleAdder(leftType, argument, false);
							Code_CopyAddressInStack(node->leftOperand.Obj(), argument, leftSize);
							Code_Read(leftType, argument);
							argument.il->Ins(BasicIns::sub, Convert(leftType));
							Code_CopyAddressInStack(node->leftOperand.Obj(), argument, leftSize);
							Code_Write(leftType, argument);
						}
						else
						{
							Code_BinaryAssignRef(node, argument, BasicIns::sub);
						}
						break;
					case BasicBinaryExpression::MulAssign:
						Code_BinaryAssignRef(node, argument, BasicIns::mul);
						break;
					case BasicBinaryExpression::DivAssign:
						Code_BinaryAssignRef(node, argument, BasicIns::div);
						break;
					case BasicBinaryExpression::ModAssign:
						Code_BinaryAssignRef(node, argument, BasicIns::mod);
						break;
					case BasicBinaryExpression::ShlAssign:
						Code_BinaryAssignRef(node, argument, BasicIns::shl);
						break;
					case BasicBinaryExpression::ShrAssign:
						Code_BinaryAssignRef(node, argument, BasicIns::shr);
						break;
					case BasicBinaryExpression::AndAssign:
					case BasicBinaryExpression::BitAndAssign:
						Code_BinaryAssignRef(node, argument, BasicIns::and);
						break;
					case BasicBinaryExpression::OrAssign:
					case BasicBinaryExpression::BitOrAssign:
						Code_BinaryAssignRef(node, argument, BasicIns::or);
						break;
					case BasicBinaryExpression::XorAssign:
						Code_BinaryAssignRef(node, argument, BasicIns::xor);
						break;
					case BasicBinaryExpression::Assign:
						{
							// TODO: Optimize for big type
							BasicLanguage_PushRef(node->leftOperand, argument);
							BasicLanguage_PushValue(node->rightOperand, argument, leftType);
							Code_CopyAddressInStack(node->leftOperand.Obj(), argument, leftSize);
							Code_Write(leftType, argument);
						}
						break;
					default:
						CHECK_ERROR(false, L"BasicLanguage_PushRef(BasicUnaryExpression*, const BCP&)#不支持此操作。");
					}
				}

				ALGORITHM_PROCEDURE_MATCH(BasicSubscribeExpression)
				{
					BasicTypeRecord* nodeType=argument.info->GetEnv()->GetExpressionType(node);
					BasicTypeRecord* operandType=argument.info->GetEnv()->GetExpressionType(node->operand.Obj());
					if(operandType->GetType()==BasicTypeRecord::Pointer)
					{
						BasicLanguage_PushValue(node->operand, argument);
					}
					else
					{
						BasicLanguage_PushRef(node->operand, argument);
						operandType=argument.info->GetTypeManager()->GetPointerType(nodeType);
					}
					BasicLanguage_PushValue(node->subscribe, argument, argument.info->GetTypeManager()->GetPrimitiveType(int_type));
					Code_ScaleAdder(operandType, argument, false);
					argument.il->Ins(BasicIns::add, BasicIns::int_type);
				}

				ALGORITHM_PROCEDURE_MATCH(BasicMemberExpression)
				{
					BasicTypeRecord* structureType=argument.info->GetEnv()->GetExpressionType(node->operand.Obj());
					if(node->pointerMember)
					{
						structureType=structureType->ElementType();
					}
					BasicTypeInfo* structureInfo=argument.info->GetTypeInfo(structureType);
					int offset=structureInfo->offsets[structureType->MemberNameIndex(node->member)];
					if(node->pointerMember)
					{
						BasicLanguage_PushValue(node->operand, argument);
					}
					else
					{
						BasicLanguage_PushRef(node->operand, argument);
					}
					argument.il->Ins(BasicIns::push, BasicIns::int_type, BasicIns::MakeInt(offset));
					argument.il->Ins(BasicIns::add, BasicIns::int_type);
				}

				ALGORITHM_PROCEDURE_MATCH(BasicInvokeExpression)
				{
					CHECK_ERROR(false, L"BasicLanguage_PushRef(BasicInvokeExpression*, const BCP&)#不支持此操作。");
				}

				ALGORITHM_PROCEDURE_MATCH(BasicFunctionResultExpression)
				{
					argument.il->Ins(BasicIns::resptr);
				}

				ALGORITHM_PROCEDURE_MATCH(BasicCastingExpression)
				{
					CHECK_ERROR(false, L"BasicLanguage_PushRef(BasicCastingExpression*, const BCP&)#不支持此操作。");
				}

				ALGORITHM_PROCEDURE_MATCH(BasicReferenceExpression)
				{
					BasicEnv::Reference reference=argument.info->GetEnv()->GetReference(node);
					if(reference.isVariable)
					{
						if(reference.globalVariable)
						{
							int offset=argument.info->GetGlobalVariableOffsets()[reference.globalVariable];
							argument.il->Ins(BasicIns::link_pushdata, BasicIns::MakeInt(offset));
						}
						else if(reference.localVariable)
						{
							int offset=argument.info->GetLocalVariableOffsets()[reference.localVariable];
							argument.il->Ins(BasicIns::stack_offset, BasicIns::MakeInt(offset));
						}
						else
						{
							int offset=0;
							BasicFunctionDeclaration* function=reference.scope->OwnerDeclaration();
							BasicTypeRecord* functionType=argument.info->GetEnv()->GetFunctionType(function);
							for(int i=0;i<reference.parameterIndex;i++)
							{
								offset+=argument.info->GetTypeInfo(functionType->ParameterType(i))->size;
							}
							argument.il->Ins(BasicIns::stack_offset, BasicIns::MakeInt(offset+sizeof(int)*4));
						}
					}
					else
					{
						CHECK_ERROR(false, L"BasicLanguage_PushRef(BasicCastingExpression*, const BCP&)#不支持此操作。");
					}
				}

				ALGORITHM_PROCEDURE_MATCH(BasicExtendedExpression)
				{
					argument.codegenExtension->PushRef(node, argument);
				}

			END_ALGORITHM_PROCEDURE(BasicLanguage_PushRef)
		}
	}
}