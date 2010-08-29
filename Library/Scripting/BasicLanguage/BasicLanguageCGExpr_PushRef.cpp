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
			using namespace collections;

/***********************************************************************
BasicLanguage_PushRef
***********************************************************************/

			BEGIN_ALGORITHM_PROCEDURE(BasicLanguage_PushRef, BasicExpression, BCP)
				BASIC_LANGUAGE_ALGORITHM_INITIALIZER

				ALGORITHM_PROCEDURE_MATCH(BasicNullExpression)
				{
					CHECK_ERROR(false, L"BasicLanguage_PushRef(BasicNullExpression*, const BCP&)#不支持此操作。");
				}

				ALGORITHM_PROCEDURE_MATCH(BasicExceptionAddressExpression)
				{
					CHECK_ERROR(false, L"BasicLanguage_PushRef(BasicExceptionAddressExpression*, const BCP&)#不支持此操作。");
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
					BasicOffset operandSize=argument.info->GetTypeInfo(operandType)->size;
					switch(node->type)
					{
					case BasicUnaryExpression::PrefixIncrease:
						{
							BasicLanguage_PushRef(node->operand, argument);
							Code_ScaleAdder(operandType, argument, true);
							Code_CopyAddressInStack(node->operand.Obj(), argument, operandSize);
							Code_Read(operandType, argument);
							argument.Ins(BasicIns::add, Convert(operandType));
							Code_CopyAddressInStack(node->operand.Obj(), argument, operandSize);
							argument.Ins(BasicIns::write, Convert(operandType));
						}
						break;
					case BasicUnaryExpression::PrefixDecrease:
						{
							BasicLanguage_PushRef(node->operand, argument);
							Code_ScaleAdder(operandType, argument, true);
							Code_CopyAddressInStack(node->operand.Obj(), argument, operandSize);
							Code_Read(operandType, argument);
							argument.Ins(BasicIns::sub, Convert(operandType));
							Code_CopyAddressInStack(node->operand.Obj(), argument, operandSize);
							argument.Ins(BasicIns::write, Convert(operandType));
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
					BasicTypeRecord* rightType=argument.info->GetEnv()->GetExpressionType(node->rightOperand.Obj());
					BasicTypeRecord* pointerType=argument.info->GetTypeManager()->GetPointerType(leftType);
					BasicOffset leftSize=argument.info->GetTypeInfo(leftType)->size;
					BasicOffset pointerSize=argument.info->GetTypeInfo(pointerType)->size;
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
							argument.Ins(BasicIns::add, Convert(leftType));
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
							argument.Ins(BasicIns::sub, Convert(leftType));
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
							BP bp(
								argument.info->GetEnv(),
								0,
								argument.info->GetTypeManager(),
								*(List<Ptr<BasicLanguageCodeException>>*)0,
								*(SortedList<WString>*)0
								);
							if(leftType==rightType && BasicLanguage_IsLeftValue(node->rightOperand, bp) && (leftType->GetType()==BasicTypeRecord::Array || leftType->GetType()==BasicTypeRecord::Structure))
							{
								if(BasicLanguage_CanPushRefWithoutSideEffect(node->leftOperand, argument))
								{
									BasicLanguage_PushRef(node->leftOperand, argument);
									BasicLanguage_PushRef(node->rightOperand, argument);
									BasicLanguage_PushRef(node->leftOperand, argument);
									Code_Copy(leftType, argument);
								}
								else
								{
									// TODO: Need test case
									BasicLanguage_PushRef(node->leftOperand, argument);
									BasicLanguage_PushRef(node->rightOperand, argument);
									Code_CopyAddressInStack(node->leftOperand.Obj(), argument, pointerSize);
									Code_Copy(leftType, argument);
								}
							}
							else
							{
								BasicLanguage_PushRef(node->leftOperand, argument);
								BasicLanguage_PushValue(node->rightOperand, argument, leftType);
								Code_CopyAddressInStack(node->leftOperand.Obj(), argument, leftSize);
								Code_Write(leftType, argument);
							}
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
					argument.Ins(BasicIns::add, BasicIns::int_type);
				}

				ALGORITHM_PROCEDURE_MATCH(BasicMemberExpression)
				{
					BasicTypeRecord* structureType=argument.info->GetEnv()->GetExpressionType(node->operand.Obj());
					if(node->pointerMember)
					{
						structureType=structureType->ElementType();
					}
					BasicTypeInfo* structureInfo=argument.info->GetTypeInfo(structureType);
					BasicOffset offset=structureInfo->offsets[structureType->MemberNameIndex(node->member)];
					if(node->pointerMember)
					{
						BasicLanguage_PushValue(node->operand, argument);
					}
					else
					{
						BasicLanguage_PushRef(node->operand, argument);
					}
					argument.Ins(BasicIns::push, BasicIns::int_type, offset);
					argument.Ins(BasicIns::add, BasicIns::int_type);
				}

				ALGORITHM_PROCEDURE_MATCH(BasicInvokeExpression)
				{
					CHECK_ERROR(false, L"BasicLanguage_PushRef(BasicInvokeExpression*, const BCP&)#不支持此操作。");
				}

				ALGORITHM_PROCEDURE_MATCH(BasicFunctionResultExpression)
				{
					argument.Ins(BasicIns::resptr);
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
							vint offset=argument.info->GetGlobalVariableOffsets()[reference.globalVariable];
							if(reference.globalVariable->linking.HasLink())
							{
								argument.Ins(BasicIns::link_pushfardata, BasicIns::MakeInt(offset));
							}
							else
							{
								argument.Ins(BasicIns::link_pushdata, BasicIns::MakeInt(offset));
							}
						}
						else if(reference.localVariable)
						{
							BasicOffset offset=argument.info->GetLocalVariableOffsets()[reference.localVariable];
							argument.Ins(BasicIns::stack_offset, offset);
						}
						else
						{
							BasicOffset offset=0;
							BasicFunctionDeclaration* function=reference.scope->OwnerDeclaration();
							BasicTypeRecord* functionType=argument.info->GetEnv()->GetFunctionType(function, true);
							for(vint i=0;i<reference.parameterIndex;i++)
							{
								offset+=argument.info->GetTypeInfo(functionType->ParameterType(i))->size;
							}
							argument.Ins(BasicIns::stack_offset, offset+(vint)sizeof(vint)*4);
						}
					}
					else
					{
						CHECK_ERROR(false, L"BasicLanguage_PushRef(BasicCastingExpression*, const BCP&)#不支持此操作。");
					}
				}

				ALGORITHM_PROCEDURE_MATCH(BasicInstanciatedExpression)
				{
					BasicEnv::Reference reference=argument.info->GetEnv()->GetReference(node->reference.Obj());
					if(reference.isVariable && reference.globalVariable)
					{
						BasicTypeRecord* resultType=0;
						vint index=GetGenericVariableTargetIndex(node, argument, resultType);
						argument.Ins(BasicIns::generic_pushdata, index);
					}
					else
					{
						CHECK_ERROR(false, L"BasicLanguage_PushRef(BasicInstanciatedExpression*, const BCP&)#不支持此操作。");
					}
				}

				ALGORITHM_PROCEDURE_MATCH(BasicInstanceFunctionExpression)
				{
					CHECK_ERROR(false, L"BasicLanguage_PushRef(BasicInstanceFunctionExpression*, const BCP&)#不支持此操作。");
				}

				ALGORITHM_PROCEDURE_MATCH(BasicExtendedExpression)
				{
					argument.codegenExtension->PushRef(node, argument);
				}

			END_ALGORITHM_PROCEDURE(BasicLanguage_PushRef)
		}
	}
}