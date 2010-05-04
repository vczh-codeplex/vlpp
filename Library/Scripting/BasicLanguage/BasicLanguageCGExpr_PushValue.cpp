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
BasicLanguage_PushValueInternal
***********************************************************************/

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

			BEGIN_ALGORITHM_FUNCTION(BasicLanguage_PushValueInternal, BasicExpression, BCP, BasicTypeRecord*)

				ALGORITHM_FUNCTION_MATCH(BasicNullExpression)
				{
					argument.il->Ins(BasicIns::push, BasicIns::pointer_type, BasicIns::MakePointer(0));
					return argument.info->GetEnv()->GetExpressionType(node);
				}

				ALGORITHM_FUNCTION_MATCH(BasicNumericExpression)
				{
					argument.il->Ins(BasicIns::push, Convert(node->type), Convert(node->argument));
					return argument.info->GetEnv()->GetExpressionType(node);
				}

				ALGORITHM_FUNCTION_MATCH(BasicMbcsStringExpression)
				{
					void* data=(void*)node->value.Buffer();
					int length=(node->value.Length()+1)*sizeof(char);
					int offset=(int)argument.globalData->Size();
					argument.globalData->Write(data, length);
					argument.il->Ins(BasicIns::link_pushdata, BasicIns::MakeInt(offset));
					return argument.info->GetEnv()->GetExpressionType(node);
				}

				ALGORITHM_FUNCTION_MATCH(BasicUnicodeStringExpression)
				{
					void* data=(void*)node->value.Buffer();
					int length=(node->value.Length()+1)*sizeof(wchar_t);
					int offset=(int)argument.globalData->Size();
					argument.globalData->Write(data, length);
					argument.il->Ins(BasicIns::link_pushdata, BasicIns::MakeInt(offset));
					return argument.info->GetEnv()->GetExpressionType(node);
				}

				ALGORITHM_FUNCTION_MATCH(BasicUnaryExpression)
				{
					BasicTypeRecord* nodeType=argument.info->GetEnv()->GetExpressionType(node);
					BasicTypeRecord* operandType=argument.info->GetEnv()->GetExpressionType(node->operand.Obj());
					switch(node->type)
					{
					case BasicUnaryExpression::PrefixIncrease:
						{
							Code_ScaleAdder(operandType, argument, true);
							BasicLanguage_PushValue(node->operand, argument);
							argument.il->Ins(BasicIns::add, Convert(operandType));
							Code_CopyStack(operandType, argument);
							BasicLanguage_PushRef(node->operand, argument);
							argument.il->Ins(BasicIns::write, Convert(operandType));
						}
						break;
					case BasicUnaryExpression::PostfixIncrease:
						{
							BasicLanguage_PushValue(node->operand, argument);
							Code_ScaleAdder(operandType, argument, true);
							Code_CopyStack(operandType, argument, argument.info->GetTypeInfo(operandType)->size);
							argument.il->Ins(BasicIns::add, Convert(operandType));
							BasicLanguage_PushRef(node->operand, argument);
							argument.il->Ins(BasicIns::write, Convert(operandType));
						}
						break;
					case BasicUnaryExpression::PrefixDecrease:
						{
							Code_ScaleAdder(operandType, argument, true);
							BasicLanguage_PushValue(node->operand, argument);
							argument.il->Ins(BasicIns::sub, Convert(operandType));
							Code_CopyStack(operandType, argument);
							BasicLanguage_PushRef(node->operand, argument);
							argument.il->Ins(BasicIns::write, Convert(operandType));
						}
						break;
					case BasicUnaryExpression::PostfixDecrease:
						{
							BasicLanguage_PushValue(node->operand, argument);
							Code_ScaleAdder(operandType, argument, true);
							Code_CopyStack(operandType, argument, argument.info->GetTypeInfo(operandType)->size);
							argument.il->Ins(BasicIns::sub, Convert(operandType));
							BasicLanguage_PushRef(node->operand, argument);
							argument.il->Ins(BasicIns::write, Convert(operandType));
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
							argument.il->Ins(BasicIns::neg, Convert(nodeType));
						}
						break;
					case BasicUnaryExpression::Not:
						{
							BasicTypeRecord* type=argument.info->GetEnv()->GetExpressionType(node);
							BasicLanguage_PushValue(node->operand, argument, nodeType);
							argument.il->Ins(BasicIns::push, BasicIns::s8, BasicIns::Makes8(1));
							argument.il->Ins(BasicIns::xor, Convert(nodeType));
						}
						break;
					case BasicUnaryExpression::BitNot:
						{
							BasicTypeRecord* type=argument.info->GetEnv()->GetExpressionType(node);
							BasicLanguage_PushValue(node->operand, argument, nodeType);
							argument.il->Ins(BasicIns::not, Convert(nodeType));
						}
						break;
					}
					return argument.info->GetEnv()->GetExpressionType(node);
				}

				ALGORITHM_FUNCTION_MATCH(BasicBinaryExpression)
				{
					BasicTypeRecord* leftType=argument.info->GetEnv()->GetExpressionType(node->leftOperand.Obj());
					BasicTypeRecord* rightType=argument.info->GetEnv()->GetExpressionType(node->rightOperand.Obj());

					switch(node->type)
					{
					case BasicBinaryExpression::Add:
						if(leftType->GetType()==BasicTypeRecord::Pointer)
						{
							BasicLanguage_PushValue(node->rightOperand, argument, argument.info->GetTypeManager()->GetPrimitiveType(int_type));
							Code_ScaleAdder(leftType, argument, false);
							BasicLanguage_PushValue(node->leftOperand, argument);
							argument.il->Ins(BasicIns::add, BasicIns::pointer_type);
							return leftType;
						}
						else if(rightType->GetType()==BasicTypeRecord::Pointer)
						{
							BasicLanguage_PushValue(node->rightOperand, argument);
							BasicLanguage_PushValue(node->leftOperand, argument, argument.info->GetTypeManager()->GetPrimitiveType(int_type));
							Code_ScaleAdder(rightType, argument, false);
							argument.il->Ins(BasicIns::add, BasicIns::pointer_type);
							return rightType;
						}
						else
						{
							return Code_Binary(node, argument, BasicIns::add);
						}
					case BasicBinaryExpression::Sub:
						if(leftType->GetType()==BasicTypeRecord::Pointer)
						{
							int size=argument.info->GetTypeInfo(leftType->ElementType())->size;
							if(rightType->GetType()==BasicTypeRecord::Pointer)
							{
								if(size>1)
								{
									argument.il->Ins(BasicIns::push, BasicIns::int_type, BasicIns::MakeInt(size));
								}
								BasicLanguage_PushValue(node->rightOperand, argument);
								BasicLanguage_PushValue(node->leftOperand, argument);
								argument.il->Ins(BasicIns::sub, BasicIns::pointer_type);
								if(size>1)
								{
									argument.il->Ins(BasicIns::div, BasicIns::int_type);
								}
								return argument.info->GetTypeManager()->GetPrimitiveType(int_type);
							}
							else
							{
								BasicLanguage_PushValue(node->rightOperand, argument, argument.info->GetTypeManager()->GetPrimitiveType(int_type));
								Code_ScaleAdder(leftType, argument, false);
								BasicLanguage_PushValue(node->leftOperand, argument);
								argument.il->Ins(BasicIns::sub, BasicIns::pointer_type);
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
							BasicLanguage_PushValue(node->rightOperand, argument, leftType);
							Code_ScaleAdder(leftType, argument, false);
							BasicLanguage_PushValue(node->leftOperand, argument, leftType);
							argument.il->Ins(BasicIns::add, Convert(leftType));
							Code_CopyStack(leftType, argument);
							BasicLanguage_PushRef(node->leftOperand, argument);
							Code_Write(leftType, argument);
							return leftType;
						}
						else
						{
							return Code_BinaryAssign(node, argument, BasicIns::add);
						}
					case BasicBinaryExpression::SubAssign:
						if(leftType->GetType()==BasicTypeRecord::Pointer)
						{
							BasicLanguage_PushValue(node->rightOperand, argument, leftType);
							Code_ScaleAdder(leftType, argument, false);
							BasicLanguage_PushValue(node->leftOperand, argument, leftType);
							argument.il->Ins(BasicIns::sub, Convert(leftType));
							Code_CopyStack(leftType, argument);
							BasicLanguage_PushRef(node->leftOperand, argument);
							Code_Write(leftType, argument);
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
							// TODO: Optimize for big type
							BasicLanguage_PushValue(node->rightOperand, argument, leftType);
							Code_CopyStack(leftType, argument);
							BasicLanguage_PushRef(node->leftOperand, argument);
							Code_Write(leftType, argument);
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

				ALGORITHM_FUNCTION_MATCH(BasicCastingExpression)
				{
					BasicTypeRecord* nodeType=argument.info->GetEnv()->GetExpressionType(node);
					BasicTypeRecord* operandType=argument.info->GetEnv()->GetExpressionType(node->operand.Obj());
					BasicLanguage_PushValue(node->operand, argument);
					Code_Convert(operandType, nodeType, argument);
					return nodeType;
				}

				ALGORITHM_FUNCTION_MATCH(BasicReferenceExpression)
				{
					BasicTypeRecord* nodeType=argument.info->GetEnv()->GetExpressionType(node);
					BasicEnv::Reference reference=argument.info->GetEnv()->GetReference(node);
					if(reference.isVariable)
					{
						BasicLanguage_PushRef(node, argument);
						Code_Read(nodeType, argument);
					}
					else
					{
						int index=GetFunctionIndex(node, argument);
						argument.il->Ins(BasicIns::codegen_pushfunc, BasicIns::MakeInt(index));
					}
					return nodeType;
				}

				ALGORITHM_FUNCTION_MATCH(BasicExtendedExpression)
				{
					return argument.codegenExtension->PushValue(node, argument);
				}

			END_ALGORITHM_FUNCTION(BasicLanguage_PushValueInternal)
		}
	}
}