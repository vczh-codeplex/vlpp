#include "BasicLanguageCodeGeneration.h"

namespace vl
{
	namespace scripting
	{
		using namespace basicil;

		namespace basiclanguage
		{
			using namespace basicil;
			using namespace stream;

/***********************************************************************
BasicCodegenInfo
***********************************************************************/

			BasicCodegenInfo::BasicCodegenInfo(BasicAnalyzer* _analyzer)
				:analyzer(_analyzer)
			{
			}

			BasicTypeInfo* BasicCodegenInfo::GetTypeInfo(BasicTypeRecord* type)
			{
				int index=typeInfos.Keys().IndexOf(type);
				if(index==-1)
				{
					BasicTypeInfo* info=new BasicTypeInfo;
					switch(type->GetType())
					{
					case BasicTypeRecord::Array:
						{
							BasicTypeInfo* element=GetTypeInfo(type->ElementType());
							info->alignment=element->alignment;
							info->size=element->size*type->ElementCount();
						}
						break;
					case BasicTypeRecord::Function:
					case BasicTypeRecord::Pointer:
						{
							info->alignment=sizeof(void*);
							info->size=sizeof(void*);
						}
						break;
					case BasicTypeRecord::Primitive:
						{
							switch(type->PrimitiveType())
							{
							case s8:case u8:case bool_type:case char_type:
								info->alignment=1;
								info->size=1;
								break;
							case s16:case u16:case wchar_type:
								info->alignment=2;
								info->size=2;
								break;
							case s32:case u32:case f32:
								info->alignment=4;
								info->size=4;
								break;
							case s64:case u64:case f64:
								info->alignment=8;
								info->size=8;
								break;
							default:
								info->alignment=1;
								info->size=1;
							}
						}
						break;
					case BasicTypeRecord::Structure:
						{
							int offset=0;
							info->alignment=0;
							info->size=0;
							for(int i=0;i<type->MemberCount();i++)
							{
								BasicTypeInfo* member=GetTypeInfo(type->MemberType(i));
								if(offset%member->alignment!=0)
								{
									offset=(offset/member->alignment+1)*member->alignment;
								}
								info->offsets.Add(offset);
								if(info->alignment<member->alignment)
								{
									info->alignment=member->alignment;
								}
								offset+=member->size;
							}
							if(info->alignment==0)
							{
								info->alignment=1;
								info->size=1;
							}
							else
							{
								if(offset%info->alignment==0)
								{
									info->size=offset;
								}
								else
								{
									info->size=(offset/info->alignment+1)*info->alignment;
								}
							}
						}
						break;
					}
					typeInfos.Add(type, info);
					return info;
				}
				else
				{
					return typeInfos.Values()[index].Obj();
				}
			}
			
			BasicEnv* BasicCodegenInfo::GetEnv()
			{
				return analyzer->GetEnv();
			}

			BasicTypeManager* BasicCodegenInfo::GetTypeManager()
			{
				return analyzer->GetTypeManager();
			}

			BasicAlgorithmConfiguration& BasicCodegenInfo::GetConfiguration()
			{
				return analyzer->GetConfiguration();
			}

/***********************************************************************
BasicCodegenExtension
***********************************************************************/

			BasicTypeRecord* BasicCodegenExtension::PushValue(BasicExtendedExpression* expression, const BCP& argument)
			{
				CHECK_ERROR(false, L"BasicCodegenExtension::PushValue(BasicExtendedExpression*, const BCP&)#不支持此操作。");
			}

			void BasicCodegenExtension::PushRef(BasicExtendedExpression* expression, const BCP& argument)
			{
				CHECK_ERROR(false, L"BasicCodegenExtension::PushRef(BasicExtendedExpression*, const BCP&)#不支持此操作。");
			}

			void BasicCodegenExtension::GenerateCode(BasicExtendedStatement* statement, const BCP& argument)
			{
				CHECK_ERROR(false, L"BasicCodegenExtension::GenerateCode(BasicExtendedStatement*, const BCP&)#不支持此操作。");
			}

/***********************************************************************
BasicCodegenParameter
***********************************************************************/
			
			BasicCodegenParameter::BasicCodegenParameter(BasicCodegenInfo* _info, basicil::BasicIL* _il, stream::MemoryStream* _globalData)
				:info(_info)
				,il(_il)
				,globalData(_globalData)
				,codegenExtension(&defaultCodegenExtension)
			{
			}

			BasicCodegenParameter::BasicCodegenParameter(const BasicCodegenParameter& parameter)
				:info(parameter.info)
				,il(parameter.il)
				,globalData(parameter.globalData)
				,codegenExtension(parameter.codegenExtension)
			{
			}

/***********************************************************************
辅助函数
***********************************************************************/

			BasicIns::ValueType Convert(BasicPrimitiveTypeEnum type)
			{
				switch(type)
				{
				case s8:
					return BasicIns::s8;
				case s16:
					return BasicIns::s16;
				case s32:
					return BasicIns::s32;
				case s64:
					return BasicIns::s64;
				case u8:
					return BasicIns::u8;
				case u16:
					return BasicIns::u16;
				case u32:
					return BasicIns::u32;
				case u64:
					return BasicIns::u64;
				case f32:
					return BasicIns::f32;
				case f64:
					return BasicIns::f64;
				case bool_type:
					return BasicIns::bool_type;
				case char_type:
					return BasicIns::char_type;
				case wchar_type:
					return BasicIns::wchar_type;
				default:
					return BasicIns::int_type;
				}
			}

			BasicIns::ValueType Convert(BasicTypeRecord* type)
			{
				switch(type->GetType())
				{
				case BasicTypeRecord::Primitive:
					if(type->PrimitiveType()!=void_type)
					{
						return Convert(type->PrimitiveType());
					}
					break;
				case BasicTypeRecord::Pointer:
					return BasicIns::pointer_type;
				}
				CHECK_ERROR(false, L"Convert(BasicTypeRecord*)#不支持此操作。");
			}

			BasicIns::Argument Convert(BasicPrimitiveValueEnum value)
			{
				BasicIns::Argument argument;
				argument.s64=value.s64;
				return argument;
			}

			void Code_ScaleAdder(BasicTypeRecord* addedValueType, const BCP& argument, bool scaleOne)
			{
				int size=1;
				if(addedValueType->GetType()==BasicTypeRecord::Pointer)
				{
					size=argument.info->GetTypeInfo(addedValueType->ElementType())->size;
				}
				if(scaleOne)
				{
					if(size>1)
					{
						argument.il->Ins(BasicIns::push, BasicIns::int_type, BasicIns::MakeInt(size));
					}
					else
					{
						argument.il->Ins(BasicIns::push, BasicIns::int_type, BasicIns::MakeInt(1));
					}
				}
				else if(size>1)
				{
					argument.il->Ins(BasicIns::push, BasicIns::int_type, BasicIns::MakeInt(size));
					argument.il->Ins(BasicIns::mul, BasicIns::int_type);
				}
			}

			void Code_DereferencePointer(BasicTypeRecord* type, const BCP& argument)
			{
				// TODO: consider non-primitive type
				argument.il->Ins(BasicIns::read, Convert(type));
			}

			void Code_CopyStack(BasicTypeRecord* type, const BCP& argument, int offset=0)
			{
				argument.il->Ins(BasicIns::stack_top, BasicIns::MakeInt(offset));
				Code_DereferencePointer(type, argument);
			}

			void Code_Convert(BasicTypeRecord* from, BasicTypeRecord* to, const BCP& argument)
			{
				if(from!=to)
				{
					BasicIns::ValueType fromType=Convert(from);
					BasicIns::ValueType toType=Convert(to);
					if(to==argument.info->GetTypeManager()->GetPrimitiveType(bool_type))
					{
						argument.il->Ins(BasicIns::push, fromType, BasicIns::MakeInt(0));
						argument.il->Ins(BasicIns::ne, fromType);
					}
					else if(fromType!=toType)
					{
						argument.il->Ins(BasicIns::convert, toType, fromType);
					}
				}
			}

			BasicTypeRecord* Code_Binary(BasicBinaryExpression* node, const BCP& argument, BasicIns::OpCode opCode)
			{
				BasicTypeRecord* type=argument.info->GetEnv()->GetExpressionType(node);
				if(type->GetType()==BasicTypeRecord::Primitive)
				{
					bool sign=false;
					int bytes=0;
					if(argument.info->GetConfiguration().DecodeInteger(type->PrimitiveType(), sign, bytes))
					{
						if(bytes<4)
						{
							BasicPrimitiveTypeEnum primitiveType=void_type;
							argument.info->GetConfiguration().EncodeInteger(primitiveType, sign, bytes);
							type=argument.info->GetTypeManager()->GetPrimitiveType(primitiveType);
						}
					}
				}
				BasicLanguage_PushValue(node->rightOperand, argument, type);
				BasicLanguage_PushValue(node->leftOperand, argument, type);
				argument.il->Ins(opCode, Convert(type));
				return type;
			}

			BasicTypeRecord* Code_Compare(BasicBinaryExpression* node, BasicTypeRecord* leftType, BasicTypeRecord* rightType, const BCP& argument, BasicIns::OpCode opCode)
			{
				BasicTypeRecord* type=leftType;
				if(leftType->GetType()==BasicTypeRecord::Primitive && rightType->GetType()==BasicTypeRecord::Primitive)
				{
					BasicPrimitiveTypeEnum result=void_type;
					argument.info->GetConfiguration().NumberBinaryOperatorTypeConversion(leftType->PrimitiveType(), rightType->PrimitiveType(), result);
					type=argument.info->GetTypeManager()->GetPrimitiveType(result);
				}
				BasicLanguage_PushValue(node->rightOperand, argument, type);
				BasicLanguage_PushValue(node->leftOperand, argument, type);
				argument.il->Ins(opCode, Convert(type));
				return argument.info->GetTypeManager()->GetPrimitiveType(bool_type);
			}

			BasicTypeRecord* Code_BinaryAssign(BasicBinaryExpression* node, const BCP& argument, BasicIns::OpCode opCode)
			{
				BasicTypeRecord* type=argument.info->GetEnv()->GetExpressionType(node);
				BasicLanguage_PushValue(node->rightOperand, argument, type);
				BasicLanguage_PushValue(node->leftOperand, argument, type);
				argument.il->Ins(opCode, Convert(type));
				Code_CopyStack(type, argument);
				BasicLanguage_PushRef(node->leftOperand, argument);
				argument.il->Ins(BasicIns::write, Convert(type));
				return type;
			}

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
					int length=node->value.Length()*sizeof(char);
					int offset=(int)argument.globalData->Size();
					argument.globalData->Write(data, length);
					argument.il->Ins(BasicIns::link_push_data, BasicIns::MakeInt(offset));
					return argument.info->GetEnv()->GetExpressionType(node);
				}

				ALGORITHM_FUNCTION_MATCH(BasicUnicodeStringExpression)
				{
					void* data=(void*)node->value.Buffer();
					int length=node->value.Length()*sizeof(wchar_t);
					int offset=(int)argument.globalData->Size();
					argument.globalData->Write(data, length);
					argument.il->Ins(BasicIns::link_push_data, BasicIns::MakeInt(offset));
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
							Code_DereferencePointer(operandType->ElementType(), argument);
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
							Code_ScaleAdder(leftType, argument, false);
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
							argument.il->Ins(BasicIns::write, Convert(leftType));
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
							argument.il->Ins(BasicIns::write, Convert(leftType));
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
							Code_CopyStack(leftType, argument);
							BasicLanguage_PushRef(node->leftOperand, argument);
							argument.il->Ins(BasicIns::write, Convert(leftType));
							return leftType;
						}
					}
					return 0;
				}

				ALGORITHM_FUNCTION_MATCH(BasicSubscribeExpression)
				{
					return 0;
				}

				ALGORITHM_FUNCTION_MATCH(BasicMemberExpression)
				{
					return 0;
				}

				ALGORITHM_FUNCTION_MATCH(BasicInvokeExpression)
				{
					return 0;
				}

				ALGORITHM_FUNCTION_MATCH(BasicFunctionResultExpression)
				{
					return 0;
				}

				ALGORITHM_FUNCTION_MATCH(BasicCastingExpression)
				{
					return 0;
				}

				ALGORITHM_FUNCTION_MATCH(BasicReferenceExpression)
				{
					return 0;
				}

				ALGORITHM_FUNCTION_MATCH(BasicExtendedExpression)
				{
					return argument.codegenExtension->PushValue(node, argument);
				}

			END_ALGORITHM_FUNCTION(BasicLanguage_PushValueInternal)

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
					switch(node->type)
					{
					case BasicUnaryExpression::PrefixIncrease:
					case BasicUnaryExpression::PrefixDecrease:
					case BasicUnaryExpression::DereferencePointer:
						break;
					default:
						CHECK_ERROR(false, L"BasicLanguage_PushRef(BasicUnaryExpression*, const BCP&)#不支持此操作。");
					}
				}

				ALGORITHM_PROCEDURE_MATCH(BasicBinaryExpression)
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
					case BasicBinaryExpression::OrAssign:
					case BasicBinaryExpression::XorAssign:
					case BasicBinaryExpression::Assign:
						break;
					default:
						CHECK_ERROR(false, L"BasicLanguage_PushRef(BasicUnaryExpression*, const BCP&)#不支持此操作。");
					}
				}

				ALGORITHM_PROCEDURE_MATCH(BasicSubscribeExpression)
				{
					// TODO: Add implementation
				}

				ALGORITHM_PROCEDURE_MATCH(BasicMemberExpression)
				{
					// TODO: Add implementation
				}

				ALGORITHM_PROCEDURE_MATCH(BasicInvokeExpression)
				{
				}

				ALGORITHM_PROCEDURE_MATCH(BasicFunctionResultExpression)
				{
					// TODO: Add implementation
				}

				ALGORITHM_PROCEDURE_MATCH(BasicCastingExpression)
				{
					CHECK_ERROR(false, L"BasicLanguage_PushRef(BasicCastingExpression*, const BCP&)#不支持此操作。");
				}

				ALGORITHM_PROCEDURE_MATCH(BasicReferenceExpression)
				{
					// TODO: Add implementation
				}

				ALGORITHM_PROCEDURE_MATCH(BasicExtendedExpression)
				{
					argument.codegenExtension->PushRef(node, argument);
				}

			END_ALGORITHM_PROCEDURE(BasicLanguage_PushRef)

/***********************************************************************
BasicLanguage_GenerateCode
***********************************************************************/

			BEGIN_ALGORITHM_PROCEDURE(BasicLanguage_GenerateCode, BasicStatement, BCP)

				ALGORITHM_PROCEDURE_MATCH(BasicEmptyStatement)
				{
				}

				ALGORITHM_PROCEDURE_MATCH(BasicCompositeStatement)
				{
				}

				ALGORITHM_PROCEDURE_MATCH(BasicExpressionStatement)
				{
				}

				ALGORITHM_PROCEDURE_MATCH(BasicVariableStatement)
				{
				}

				ALGORITHM_PROCEDURE_MATCH(BasicIfStatement)
				{
				}

				ALGORITHM_PROCEDURE_MATCH(BasicWhileStatement)
				{
				}

				ALGORITHM_PROCEDURE_MATCH(BasicForStatement)
				{
				}

				ALGORITHM_PROCEDURE_MATCH(BasicBreakStatement)
				{
				}

				ALGORITHM_PROCEDURE_MATCH(BasicContinueStatement)
				{
				}

				ALGORITHM_PROCEDURE_MATCH(BasicReturnStatement)
				{
				}

				ALGORITHM_PROCEDURE_MATCH(BasicExtendedStatement)
				{
					argument.codegenExtension->GenerateCode(node, argument);
				}

			END_ALGORITHM_PROCEDURE(BasicLanguage_GenerateCode)
		}
	}
}