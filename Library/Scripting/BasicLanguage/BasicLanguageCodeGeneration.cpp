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

/***********************************************************************
BasicCodegenExtension
***********************************************************************/

			void BasicCodegenExtension::PushValue(BasicExtendedExpression* expression, const BCP& argument)
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
					size=argument.info->GetTypeInfo(addedValueType)->size;
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

			void Code_CopyStack(BasicTypeRecord* type, const BCP& argument)
			{
				argument.il->Ins(BasicIns::stack_top, BasicIns::MakeInt(0));
				argument.il->Ins(BasicIns::read, Convert(type));
			}

			void Code_DereferencePointer(BasicTypeRecord* type, const BCP& argument)
			{
				// TODO: consider non-primitive type
				argument.il->Ins(BasicIns::read, Convert(type));
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

/***********************************************************************
BasicLanguage_PushValueInternal
***********************************************************************/

			void BasicLanguage_PushValue(Ptr<BasicExpression> expression, const BCP& argument)
			{
				BasicLanguage_PushValueInternal(expression, argument);
			}

			void BasicLanguage_PushValue(BasicExpression* expression, const BCP& argument)
			{
				BasicLanguage_PushValueInternal(expression, argument);
			}

			void BasicLanguage_PushValue(Ptr<BasicExpression> expression, const BCP& argument, BasicTypeRecord* expectedType)
			{
				BasicLanguage_PushValue(expression.Obj(), argument, expectedType);
			}

			void BasicLanguage_PushValue(BasicExpression* expression, const BCP& argument, BasicTypeRecord* expectedType)
			{
				BasicLanguage_PushValueInternal(expression, argument);
				Code_Convert(argument.info->GetEnv()->GetExpressionType(expression), expectedType, argument);
			}

			BEGIN_ALGORITHM_PROCEDURE(BasicLanguage_PushValueInternal, BasicExpression, BCP)

				ALGORITHM_PROCEDURE_MATCH(BasicNullExpression)
				{
					argument.il->Ins(BasicIns::push, BasicIns::pointer_type, BasicIns::MakePointer(0));
				}

				ALGORITHM_PROCEDURE_MATCH(BasicNumericExpression)
				{
					argument.il->Ins(BasicIns::push, Convert(node->type), Convert(node->argument));
				}

				ALGORITHM_PROCEDURE_MATCH(BasicMbcsStringExpression)
				{
					void* data=(void*)node->value.Buffer();
					int length=node->value.Length()*sizeof(char);
					int offset=(int)argument.globalData->Size();
					argument.globalData->Write(data, length);
					argument.il->Ins(BasicIns::link_push_data, BasicIns::MakeInt(offset));
				}

				ALGORITHM_PROCEDURE_MATCH(BasicUnicodeStringExpression)
				{
					void* data=(void*)node->value.Buffer();
					int length=node->value.Length()*sizeof(wchar_t);
					int offset=(int)argument.globalData->Size();
					argument.globalData->Write(data, length);
					argument.il->Ins(BasicIns::link_push_data, BasicIns::MakeInt(offset));
				}

				ALGORITHM_PROCEDURE_MATCH(BasicUnaryExpression)
				{
					switch(node->type)
					{
					case BasicUnaryExpression::PrefixIncrease:
						{
							BasicTypeRecord* type=argument.info->GetEnv()->GetExpressionType(node->operand.Obj());
							BasicLanguage_PushValue(node->operand, argument);
							Code_ScaleAdder(type, argument, true);
							argument.il->Ins(BasicIns::add, Convert(type));
							Code_CopyStack(type, argument);
							BasicLanguage_PushRef(node->operand, argument);
							argument.il->Ins(BasicIns::write, Convert(type));
						}
						break;
					case BasicUnaryExpression::PostfixIncrease:
						{
							BasicTypeRecord* type=argument.info->GetEnv()->GetExpressionType(node->operand.Obj());
							BasicLanguage_PushValue(node->operand, argument);
							Code_CopyStack(type, argument);
							Code_ScaleAdder(type, argument, true);
							argument.il->Ins(BasicIns::add, Convert(type));
							BasicLanguage_PushRef(node->operand, argument);
							argument.il->Ins(BasicIns::write, Convert(type));
						}
						break;
					case BasicUnaryExpression::PrefixDecrease:
						{
							BasicTypeRecord* type=argument.info->GetEnv()->GetExpressionType(node->operand.Obj());
							BasicLanguage_PushValue(node->operand, argument);
							Code_ScaleAdder(type, argument, true);
							argument.il->Ins(BasicIns::sub, Convert(type));
							Code_CopyStack(type, argument);
							BasicLanguage_PushRef(node->operand, argument);
							argument.il->Ins(BasicIns::write, Convert(type));
						}
						break;
					case BasicUnaryExpression::PostfixDecrease:
						{
							BasicTypeRecord* type=argument.info->GetEnv()->GetExpressionType(node->operand.Obj());
							BasicLanguage_PushValue(node->operand, argument);
							Code_CopyStack(type, argument);
							Code_ScaleAdder(type, argument, true);
							argument.il->Ins(BasicIns::sub, Convert(type));
							BasicLanguage_PushRef(node->operand, argument);
							argument.il->Ins(BasicIns::write, Convert(type));
						}
						break;
					case BasicUnaryExpression::GetAddress:
						{
							BasicLanguage_PushRef(node->operand, argument);
						}
						break;
					case BasicUnaryExpression::DereferencePointer:
						{
							BasicTypeRecord* type=argument.info->GetEnv()->GetExpressionType(node->operand.Obj());
							BasicLanguage_PushValue(node->operand, argument);
							Code_DereferencePointer(type->ElementType(), argument);
						}
						break;
					case BasicUnaryExpression::Negative:
						{
							BasicTypeRecord* type=argument.info->GetEnv()->GetExpressionType(node);
							BasicLanguage_PushValue(node->operand, argument, type);
							argument.il->Ins(BasicIns::neg, Convert(type));
						}
						break;
					case BasicUnaryExpression::Not:
					case BasicUnaryExpression::BitNot:
						{
							BasicTypeRecord* type=argument.info->GetEnv()->GetExpressionType(node);
							BasicLanguage_PushValue(node->operand, argument, type);
							argument.il->Ins(BasicIns::not, Convert(type));
						}
						break;
					}
				}

				ALGORITHM_PROCEDURE_MATCH(BasicBinaryExpression)
				{
					switch(node->type)
					{
					case BasicBinaryExpression::Add:
						break;
					case BasicBinaryExpression::Sub:
						break;
					case BasicBinaryExpression::Mul:
						break;
					case BasicBinaryExpression::Div:
						break;
					case BasicBinaryExpression::Mod:
						break;
					case BasicBinaryExpression::Shl:
						break;
					case BasicBinaryExpression::Shr:
						break;
					case BasicBinaryExpression::And:
						break;
					case BasicBinaryExpression::Or:
						break;
					case BasicBinaryExpression::Xor:
						break;
					case BasicBinaryExpression::BitAnd:
						break;
					case BasicBinaryExpression::BitOr:
						break;
					case BasicBinaryExpression::Lt:
						break;
					case BasicBinaryExpression::Le:
						break;
					case BasicBinaryExpression::Gt:
						break;
					case BasicBinaryExpression::Ge:
						break;
					case BasicBinaryExpression::Eq:
						break;
					case BasicBinaryExpression::Ne:
						break;
					case BasicBinaryExpression::AddAssign:
						break;
					case BasicBinaryExpression::SubAssign:
						break;
					case BasicBinaryExpression::MulAssign:
						break;
					case BasicBinaryExpression::DivAssign:
						break;
					case BasicBinaryExpression::ModAssign:
						break;
					case BasicBinaryExpression::ShlAssign:
						break;
					case BasicBinaryExpression::ShrAssign:
						break;
					case BasicBinaryExpression::AndAssign:
						break;
					case BasicBinaryExpression::OrAssign:
						break;
					case BasicBinaryExpression::XorAssign:
						break;
					case BasicBinaryExpression::Assign:
						break;
					}
				}

				ALGORITHM_PROCEDURE_MATCH(BasicSubscribeExpression)
				{
				}

				ALGORITHM_PROCEDURE_MATCH(BasicMemberExpression)
				{
				}

				ALGORITHM_PROCEDURE_MATCH(BasicInvokeExpression)
				{
				}

				ALGORITHM_PROCEDURE_MATCH(BasicFunctionResultExpression)
				{
				}

				ALGORITHM_PROCEDURE_MATCH(BasicCastingExpression)
				{
				}

				ALGORITHM_PROCEDURE_MATCH(BasicReferenceExpression)
				{
				}

				ALGORITHM_PROCEDURE_MATCH(BasicExtendedExpression)
				{
					argument.codegenExtension->PushValue(node, argument);
				}

			END_ALGORITHM_PROCEDURE(BasicLanguage_PushValueInternal)

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