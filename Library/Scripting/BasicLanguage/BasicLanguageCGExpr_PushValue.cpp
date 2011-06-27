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
BasicLanguage_PushValueInternal
***********************************************************************/

			BEGIN_ALGORITHM_FUNCTION(BasicLanguage_PushValueInternal, BasicExpression, BCP, BasicTypeRecord*)
				BASIC_LANGUAGE_ALGORITHM_INITIALIZER

				ALGORITHM_FUNCTION_MATCH(BasicNullExpression)
				{
					argument.Ins(BasicIns::push, BasicIns::pointer_type, BasicIns::MakePointer(0));
					return argument.info->GetEnv()->GetExpressionType(node);
				}

				ALGORITHM_FUNCTION_MATCH(BasicExceptionAddressExpression)
				{
					argument.Ins(BasicIns::exception_object_address);
					return argument.info->GetEnv()->GetExpressionType(node);
				}

				ALGORITHM_FUNCTION_MATCH(BasicStackDataAddressExpression)
				{
					argument.Ins(BasicIns::stack_data);
					argument.Ins(BasicIns::read, BasicIns::pointer_type);
					return argument.info->GetEnv()->GetExpressionType(node);
				}

				ALGORITHM_FUNCTION_MATCH(BasicNumericExpression)
				{
					argument.Ins(BasicIns::push, Convert(node->type), Convert(node->argument));
					return argument.info->GetEnv()->GetExpressionType(node);
				}

				ALGORITHM_FUNCTION_MATCH(BasicMbcsStringExpression)
				{
					void* data=(void*)node->value.Buffer();
					vint length=(node->value.Length()+1)*sizeof(char);
					vint offset=(vint)argument.globalData->Size();
					argument.globalData->Write(data, length);
					argument.Ins(BasicIns::link_pushdata, BasicIns::MakeInt(offset));
					return argument.info->GetEnv()->GetExpressionType(node);
				}

				ALGORITHM_FUNCTION_MATCH(BasicUnicodeStringExpression)
				{
					void* data=(void*)node->value.Buffer();
					vint length=(node->value.Length()+1)*sizeof(wchar_t);
					vint offset=(vint)argument.globalData->Size();
					argument.globalData->Write(data, length);
					argument.Ins(BasicIns::link_pushdata, BasicIns::MakeInt(offset));
					return argument.info->GetEnv()->GetExpressionType(node);
				}

				ALGORITHM_FUNCTION_MATCH(BasicUnaryExpression)
				{
					BasicTypeRecord* nodeType=argument.info->GetEnv()->GetExpressionType(node);
					BasicTypeRecord* operandType=argument.info->GetEnv()->GetExpressionType(node->operand.Obj());
					BasicOffset operandSize=argument.info->GetTypeInfo(operandType)->size;
					BasicOffset operandAddressSize=argument.info->GetTypeInfo(argument.info->GetTypeManager()->GetPointerType(operandType))->size;
					switch(node->type)
					{
					case BasicUnaryExpression::PrefixIncrease:
						{
							if(BasicLanguage_CanPushRefWithoutSideEffect(node->operand, argument))
							{
								Code_ScaleAdder(operandType, argument, true);
								BasicLanguage_PushValue(node->operand, argument);
								argument.Ins(BasicIns::add, Convert(operandType));
								Code_CopyStack(operandType, argument);
								BasicLanguage_PushRefWithoutSideEffect(node->operand, argument);
								Code_Write(operandType, argument);
							}
							else
							{
								BasicLanguage_PushRef(node->operand, argument);
								Code_ScaleAdder(operandType, argument, true);
								Code_CopyAddressInStack(node->operand.Obj(), argument, operandSize);
								Code_Read(operandType, argument);
								argument.Ins(BasicIns::add, Convert(operandType));
								Code_CopyAddressInStack(node->operand.Obj(), argument, operandSize);
								Code_Write(operandType, argument);
								Code_Read(operandType, argument);
							}
						}
						break;
					case BasicUnaryExpression::PostfixIncrease:
						{
							if(BasicLanguage_CanPushRefWithoutSideEffect(node->operand, argument))
							{
								BasicLanguage_PushValue(node->operand, argument);
								Code_ScaleAdder(operandType, argument, true);
								BasicLanguage_PushRefWithoutSideEffect(node->operand, argument);
								Code_Read(operandType, argument);
								argument.Ins(BasicIns::add, Convert(operandType));
								BasicLanguage_PushRefWithoutSideEffect(node->operand, argument);
								Code_Write(operandType, argument);
							}
							else
							{
								argument.Ins(BasicIns::stack_reserve, operandSize);
								BasicLanguage_PushRef(node->operand, argument);
								Code_CopyAddressInStack(node->operand.Obj(), argument);
								argument.Ins(BasicIns::stack_top, operandAddressSize*(vint)2);
								Code_Copy(operandType, argument);
								Code_ScaleAdder(operandType, argument, true);
								Code_CopyAddressInStack(node->operand.Obj(), argument, operandSize);
								Code_Read(operandType, argument);
								argument.Ins(BasicIns::add, Convert(operandType));
								Code_CopyAddressInStack(node->operand.Obj(), argument, operandSize);
								argument.Ins(BasicIns::write, Convert(operandType));
								argument.Ins(BasicIns::stack_reserve, -operandAddressSize);
							}
						}
						break;
					case BasicUnaryExpression::PrefixDecrease:
						{
							if(BasicLanguage_CanPushRefWithoutSideEffect(node->operand, argument))
							{
								Code_ScaleAdder(operandType, argument, true);
								BasicLanguage_PushValue(node->operand, argument);
								argument.Ins(BasicIns::sub, Convert(operandType));
								Code_CopyStack(operandType, argument);
								BasicLanguage_PushRefWithoutSideEffect(node->operand, argument);
								Code_Write(operandType, argument);
							}
							else
							{
								BasicLanguage_PushRef(node->operand, argument);
								Code_ScaleAdder(operandType, argument, true);
								Code_CopyAddressInStack(node->operand.Obj(), argument, operandSize);
								Code_Read(operandType, argument);
								argument.Ins(BasicIns::sub, Convert(operandType));
								Code_CopyAddressInStack(node->operand.Obj(), argument, operandSize);
								Code_Write(operandType, argument);
								Code_Read(operandType, argument);
							}
						}
						break;
					case BasicUnaryExpression::PostfixDecrease:
						{
							if(BasicLanguage_CanPushRefWithoutSideEffect(node->operand, argument))
							{
								BasicLanguage_PushValue(node->operand, argument);
								Code_ScaleAdder(operandType, argument, true);
								BasicLanguage_PushRefWithoutSideEffect(node->operand, argument);
								Code_Read(operandType, argument);
								argument.Ins(BasicIns::sub, Convert(operandType));
								BasicLanguage_PushRefWithoutSideEffect(node->operand, argument);
								Code_Write(operandType, argument);
							}
							else
							{
								argument.Ins(BasicIns::stack_reserve, operandSize);
								BasicLanguage_PushRef(node->operand, argument);
								Code_CopyAddressInStack(node->operand.Obj(), argument);
								argument.Ins(BasicIns::stack_top,operandAddressSize*(vint)2);
								Code_Copy(operandType, argument);
								Code_ScaleAdder(operandType, argument, true);
								Code_CopyAddressInStack(node->operand.Obj(), argument, operandSize);
								Code_Read(operandType, argument);
								argument.Ins(BasicIns::sub, Convert(operandType));
								Code_CopyAddressInStack(node->operand.Obj(), argument, operandSize);
								argument.Ins(BasicIns::write, Convert(operandType));
								argument.Ins(BasicIns::stack_reserve, -operandAddressSize);
							}
						}
						break;
					case BasicUnaryExpression::GetAddress:
						{
							BasicLanguage_PushRef(node->operand, argument);
						}
						break;
					case BasicUnaryExpression::DereferencePointer:
						{
							BasicLanguage_PushValue(node->operand, argument);
							Code_Read(operandType->ElementType(), argument);
							Code_Convert(operandType->ElementType(), nodeType, argument);
						}
						break;
					case BasicUnaryExpression::Negative:
						{
							BasicLanguage_PushValue(node->operand, argument, nodeType);
							argument.Ins(BasicIns::neg, Convert(nodeType));
						}
						break;
					case BasicUnaryExpression::Not:
						{
							BasicTypeRecord* type=argument.info->GetEnv()->GetExpressionType(node);
							BasicLanguage_PushValue(node->operand, argument, nodeType);
							argument.Ins(BasicIns::push, BasicIns::s8, BasicIns::Makes8(1));
							argument.Ins(BasicIns::xor, Convert(nodeType));
						}
						break;
					case BasicUnaryExpression::BitNot:
						{
							BasicTypeRecord* type=argument.info->GetEnv()->GetExpressionType(node);
							BasicLanguage_PushValue(node->operand, argument, nodeType);
							argument.Ins(BasicIns::not, Convert(nodeType));
						}
						break;
					}
					return argument.info->GetEnv()->GetExpressionType(node);
				}

				ALGORITHM_FUNCTION_MATCH(BasicBinaryExpression)
				{
					BasicTypeRecord* leftType=argument.info->GetEnv()->GetExpressionType(node->leftOperand.Obj());
					BasicTypeRecord* rightType=argument.info->GetEnv()->GetExpressionType(node->rightOperand.Obj());
					BasicOffset leftSize=argument.info->GetTypeInfo(leftType)->size;

					switch(node->type)
					{
					case BasicBinaryExpression::Add:
						if(leftType->GetType()==BasicTypeRecord::Pointer)
						{
							BasicLanguage_PushValue(node->rightOperand, argument, argument.info->GetTypeManager()->GetPrimitiveType(int_type));
							Code_ScaleAdder(leftType, argument, false);
							BasicLanguage_PushValue(node->leftOperand, argument);
							argument.Ins(BasicIns::add, BasicIns::pointer_type);
							return leftType;
						}
						else if(rightType->GetType()==BasicTypeRecord::Pointer)
						{
							BasicLanguage_PushValue(node->rightOperand, argument);
							BasicLanguage_PushValue(node->leftOperand, argument, argument.info->GetTypeManager()->GetPrimitiveType(int_type));
							Code_ScaleAdder(rightType, argument, false);
							argument.Ins(BasicIns::add, BasicIns::pointer_type);
							return rightType;
						}
						else
						{
							return Code_Binary(node, argument, BasicIns::add);
						}
					case BasicBinaryExpression::Sub:
						if(leftType->GetType()==BasicTypeRecord::Pointer)
						{
							BasicOffset size=argument.info->GetTypeInfo(leftType->ElementType())->size;
							if(rightType->GetType()==BasicTypeRecord::Pointer)
							{
								argument.Ins(BasicIns::push, BasicIns::int_type, size);
								BasicLanguage_PushValue(node->rightOperand, argument);
								BasicLanguage_PushValue(node->leftOperand, argument);
								argument.Ins(BasicIns::sub, BasicIns::pointer_type);
								argument.Ins(BasicIns::div, BasicIns::int_type);
								return argument.info->GetTypeManager()->GetPrimitiveType(int_type);
							}
							else
							{
								BasicLanguage_PushValue(node->rightOperand, argument, argument.info->GetTypeManager()->GetPrimitiveType(int_type));
								Code_ScaleAdder(leftType, argument, false);
								BasicLanguage_PushValue(node->leftOperand, argument);
								argument.Ins(BasicIns::sub, BasicIns::pointer_type);
								return leftType;
							}
						}
						else
						{
							return Code_Binary(node, argument, BasicIns::sub);
						}
					case BasicBinaryExpression::Mul:
						return Code_Binary(node, argument, BasicIns::mul);
					case BasicBinaryExpression::Div:
						return Code_Binary(node, argument, BasicIns::div);
					case BasicBinaryExpression::Mod:
						return Code_Binary(node, argument, BasicIns::mod);
					case BasicBinaryExpression::Shl:
						return Code_Binary(node, argument, BasicIns::shl);
					case BasicBinaryExpression::Shr:
						return Code_Binary(node, argument, BasicIns::shr);
					case BasicBinaryExpression::And:
					case BasicBinaryExpression::BitAnd:
						return Code_Binary(node, argument, BasicIns::and);
					case BasicBinaryExpression::Or:
					case BasicBinaryExpression::BitOr:
						return Code_Binary(node, argument, BasicIns::or);
					case BasicBinaryExpression::Xor:
						return Code_Binary(node, argument, BasicIns::xor);
					case BasicBinaryExpression::Lt:
						return Code_Compare(node, leftType, rightType, argument, BasicIns::lt);
					case BasicBinaryExpression::Le:
						return Code_Compare(node, leftType, rightType, argument, BasicIns::le);
					case BasicBinaryExpression::Gt:
						return Code_Compare(node, leftType, rightType, argument, BasicIns::gt);
					case BasicBinaryExpression::Ge:
						return Code_Compare(node, leftType, rightType, argument, BasicIns::ge);
					case BasicBinaryExpression::Eq:
						return Code_Compare(node, leftType, rightType, argument, BasicIns::eq);
					case BasicBinaryExpression::Ne:
						return Code_Compare(node, leftType, rightType, argument, BasicIns::ne);
					case BasicBinaryExpression::AddAssign:
						if(leftType->GetType()==BasicTypeRecord::Pointer)
						{
							if(BasicLanguage_CanPushRefWithoutSideEffect(node->leftOperand, argument))
							{
								BasicLanguage_PushValue(node->rightOperand, argument, leftType);
								Code_ScaleAdder(leftType, argument, false);
								BasicLanguage_PushValue(node->leftOperand, argument, leftType);
								argument.Ins(BasicIns::add, Convert(leftType));
								Code_CopyStack(leftType, argument);
								BasicLanguage_PushRefWithoutSideEffect(node->leftOperand, argument);
								Code_Write(leftType, argument);
							}
							else
							{
								BasicLanguage_PushRef(node->leftOperand.Obj(), argument);
								BasicLanguage_PushValue(node->rightOperand, argument, leftType);
								Code_ScaleAdder(leftType, argument, false);
								Code_CopyAddressInStack(node->leftOperand.Obj(), argument, leftSize);
								Code_Read(leftType, argument);
								argument.Ins(BasicIns::add, Convert(leftType));
								Code_CopyAddressInStack(node->leftOperand.Obj(), argument, leftSize);
								Code_Write(leftType, argument);
								Code_Read(leftType, argument);
							}
							return leftType;
						}
						else
						{
							return Code_BinaryAssign(node, argument, BasicIns::add);
						}
					case BasicBinaryExpression::SubAssign:
						if(leftType->GetType()==BasicTypeRecord::Pointer)
						{
							if(BasicLanguage_CanPushRefWithoutSideEffect(node->leftOperand, argument))
							{
								BasicLanguage_PushValue(node->rightOperand, argument, leftType);
								Code_ScaleAdder(leftType, argument, false);
								BasicLanguage_PushValue(node->leftOperand, argument, leftType);
								argument.Ins(BasicIns::sub, Convert(leftType));
								Code_CopyStack(leftType, argument);
								BasicLanguage_PushRefWithoutSideEffect(node->leftOperand, argument);
								Code_Write(leftType, argument);
							}
							else
							{
								BasicLanguage_PushRef(node->leftOperand.Obj(), argument);
								BasicLanguage_PushValue(node->rightOperand, argument, leftType);
								Code_ScaleAdder(leftType, argument, false);
								Code_CopyAddressInStack(node->leftOperand.Obj(), argument, leftSize);
								Code_Read(leftType, argument);
								argument.Ins(BasicIns::sub, Convert(leftType));
								Code_CopyAddressInStack(node->leftOperand.Obj(), argument, leftSize);
								Code_Write(leftType, argument);
								Code_Read(leftType, argument);
							}
							return leftType;
						}
						else
						{
							return Code_BinaryAssign(node, argument, BasicIns::sub);
						}
					case BasicBinaryExpression::MulAssign:
						return Code_BinaryAssign(node, argument, BasicIns::mul);
					case BasicBinaryExpression::DivAssign:
						return Code_BinaryAssign(node, argument, BasicIns::div);
					case BasicBinaryExpression::ModAssign:
						return Code_BinaryAssign(node, argument, BasicIns::mod);
					case BasicBinaryExpression::ShlAssign:
						return Code_BinaryAssign(node, argument, BasicIns::shl);
					case BasicBinaryExpression::ShrAssign:
						return Code_BinaryAssign(node, argument, BasicIns::shr);
					case BasicBinaryExpression::AndAssign:
					case BasicBinaryExpression::BitAndAssign:
						return Code_BinaryAssign(node, argument, BasicIns::and);
					case BasicBinaryExpression::OrAssign:
					case BasicBinaryExpression::BitOrAssign:
						return Code_BinaryAssign(node, argument, BasicIns::or);
					case BasicBinaryExpression::XorAssign:
						return Code_BinaryAssign(node, argument, BasicIns::xor);
					case BasicBinaryExpression::Assign:
						{
							BasicLanguage_PushValue(node->rightOperand, argument, leftType);
							if(leftType->GetType()==BasicTypeRecord::Array || leftType->GetType()==BasicTypeRecord::Structure)
							{
								argument.Ins(BasicIns::stack_top, BasicIns::MakeInt(0));
								BasicLanguage_PushRef(node->leftOperand, argument);
								Code_Copy(leftType, argument);
							}
							else
							{
								Code_CopyStack(leftType, argument);
								BasicLanguage_PushRef(node->leftOperand, argument);
								Code_Write(leftType, argument);
							}
							return leftType;
						}
					}
					return 0;
				}

				ALGORITHM_FUNCTION_MATCH(BasicSubscribeExpression)
				{
					BasicTypeRecord* nodeType=argument.info->GetEnv()->GetExpressionType(node);
					BasicLanguage_PushRef(node, argument);
					Code_Read(nodeType, argument);
					return nodeType;
				}

				ALGORITHM_FUNCTION_MATCH(BasicMemberExpression)
				{
					BasicTypeRecord* nodeType=argument.info->GetEnv()->GetExpressionType(node);
					BasicLanguage_PushRef(node, argument);
					Code_Read(nodeType, argument);
					return nodeType;
				}

				ALGORITHM_FUNCTION_MATCH(BasicInvokeExpression)
				{
					return Code_InvokeFunction(node, argument, false);
				}

				ALGORITHM_FUNCTION_MATCH(BasicFunctionResultExpression)
				{
					BasicTypeRecord* type=argument.info->GetEnv()->GetExpressionType(node);
					BasicLanguage_PushRef(node, argument);
					Code_Read(type, argument);
					return type;
				}

				ALGORITHM_FUNCTION_MATCH(BasicSizeofExpression)
				{
					BP bp(
						argument.info->GetEnv(),
						argument.info->GetSemanticScope(),
						argument.info->GetTypeManager(),
						argument.info->GetTypeInfoManager(),
						*(List<Ptr<BasicLanguageCodeException>>*)0,
						*(SortedList<WString>*)0
						);
					BasicTypeRecord* nodeType=BasicLanguage_GetTypeRecord(node->type, bp, false);
					BasicOffset size=argument.info->GetTypeInfo(nodeType)->size;
					argument.Ins(BasicIns::push, BasicIns::int_type, size);
					return argument.info->GetEnv()->GetExpressionType(node);
				}

				ALGORITHM_FUNCTION_MATCH(BasicOffsetofExpression)
				{
					BP bp(
						argument.info->GetEnv(),
						argument.info->GetSemanticScope(),
						argument.info->GetTypeManager(),
						argument.info->GetTypeInfoManager(),
						*(List<Ptr<BasicLanguageCodeException>>*)0,
						*(SortedList<WString>*)0
						);
					BasicTypeRecord* structureType=BasicLanguage_GetTypeRecord(node->type, bp, false);
					BasicTypeInfo* structureInfo=argument.info->GetTypeInfo(structureType);
					BasicOffset offset=structureInfo->offsets[structureType->MemberNameIndex(node->member)];
					argument.Ins(BasicIns::push, BasicIns::int_type, offset);
					return argument.info->GetEnv()->GetExpressionType(node);
				}

				ALGORITHM_FUNCTION_MATCH(BasicCastingExpression)
				{
					BasicTypeRecord* nodeType=argument.info->GetEnv()->GetExpressionType(node);
					BasicTypeRecord* operandType=BasicLanguage_PushValue(node->operand, argument);
					Code_Convert(operandType, nodeType, argument);
					return nodeType;
				}

				ALGORITHM_FUNCTION_MATCH(BasicReferenceExpression)
				{
					BasicTypeRecord* nodeType=argument.info->GetEnv()->GetExpressionType(node);
					BasicEnv::Reference reference=argument.info->GetEnv()->GetReference(node);
					if(reference.isVariable)
					{
						if(reference.globalVariable && reference.globalVariable->constant)
						{
							BasicCompileTimeConstant constantValue=argument.info->GetEnv()->GlobalScope()->variables.Find(node->name).constantValue;
							if(nodeType->GetType()==BasicTypeRecord::Pointer)
							{
								argument.Ins(BasicIns::push, BasicIns::pointer_type, BasicIns::MakePointer((void*)constantValue.u));
							}
							else if(nodeType->GetType()==BasicTypeRecord::Primitive)
							{
								argument.Ins(BasicIns::push, Convert(nodeType->PrimitiveType()), Convert(constantValue));
							}
							else
							{
								CHECK_ERROR(false, L"BasicLanguage_PushValueInternal(BasicReferenceExpression*, const BCP&)#不支持此操作。");
							}
						}
						else
						{
							BasicLanguage_PushRef(node, argument);
							Code_Read(nodeType, argument);
						}
					}
					else
					{
						vint index=GetFunctionIndex(node, argument);
						if(IsExternalFunction(node, argument))
						{
							argument.Ins(BasicIns::link_pushfarfunc, BasicIns::MakeInt(index));
						}
						else if(IsForeignFunction(node, argument))
						{
							argument.Ins(BasicIns::link_pushforeignfunc, BasicIns::MakeInt(index));
						}
						else
						{
							argument.Ins(BasicIns::link_pushfunc, BasicIns::MakeInt(index));
						}
					}
					return nodeType;
				}

				ALGORITHM_FUNCTION_MATCH(BasicInstantiatedExpression)
				{
					BasicTypeRecord* nodeType=argument.info->GetEnv()->GetExpressionType(node);
					BasicEnv::Reference reference=argument.info->GetEnv()->GetReference(node->reference.Obj());
					if(reference.isVariable)
					{
						BasicLanguage_PushRef(node, argument);
						Code_Read(nodeType, argument);
						return nodeType;
					}
					else
					{
						BasicTypeRecord* resultType=0;
						vint index=GetGenericFunctionTargetIndex(node, argument, resultType);
						argument.Ins(BasicIns::generic_pushfunc, BasicIns::MakeInt(index));
						return resultType;
					}
				}

				ALGORITHM_FUNCTION_MATCH(BasicInstanceFunctionExpression)
				{
					vint index=GetGenericInstanceTargetIndex(node, argument);
					argument.Ins(BasicIns::generic_instance_pushfunc, BasicIns::MakeInt(index));
					return argument.info->GetEnv()->GetExpressionType(node);
				}

				ALGORITHM_FUNCTION_MATCH(BasicExtendedExpression)
				{
					return argument.codegenExtension->PushValue(node, argument);
				}

			END_ALGORITHM_FUNCTION(BasicLanguage_PushValueInternal)

			BasicTypeRecord* BasicLanguage_PushValue(Ptr<BasicExpression> expression, const BCP& argument)
			{
				return BasicLanguage_PushValueInternal(expression, argument);
			}

			BasicTypeRecord* BasicLanguage_PushValue(BasicExpression* expression, const BCP& argument)
			{
				return BasicLanguage_PushValueInternal(expression, argument);
			}

			BasicTypeRecord* BasicLanguage_PushValue(Ptr<BasicExpression> expression, const BCP& argument, BasicTypeRecord* expectedType)
			{
				return BasicLanguage_PushValue(expression.Obj(), argument, expectedType);
			}

			BasicTypeRecord* BasicLanguage_PushValue(BasicExpression* expression, const BCP& argument, BasicTypeRecord* expectedType)
			{
				BasicTypeRecord* type=BasicLanguage_PushValueInternal(expression, argument);
				Code_Convert(type, expectedType, argument);
				return expectedType;
			}
		}
	}
}