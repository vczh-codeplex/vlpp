#include "BasicLanguageAnalyzer.h"

namespace vl
{
	namespace scripting
	{
		namespace basiclanguage
		{
			using namespace collections;

/***********************************************************************
BasicCompileTimeConstant
***********************************************************************/
			
			signed __int64 BasicCompileTimeConstant::S(BasicTypeRecord* sourceType)
			{
				switch(GetConstantType(sourceType))
				{
				case Signed:
					return (signed __int64)s;
				case Unsigned:
					return (signed __int64)u;
				case Float:
					return (signed __int64)d;
				default:
					return 0;
				}
			}

			unsigned __int64 BasicCompileTimeConstant::U(BasicTypeRecord* sourceType)
			{
				switch(GetConstantType(sourceType))
				{
				case Signed:
					return (unsigned __int64)s;
				case Unsigned:
					return (unsigned __int64)u;
				case Float:
					return (unsigned __int64)d;
				default:
					return 0;
				}
			}

			double BasicCompileTimeConstant::F(BasicTypeRecord* sourceType)
			{
				switch(GetConstantType(sourceType))
				{
				case Signed:
					return (double)s;
				case Unsigned:
					return (double)u;
				case Float:
					return (double)d;
				default:
					return 0;
				}
			}

			BasicCompileTimeConstant::ConstantType GetConstantType(BasicTypeRecord* type)
			{
				if(type->GetType()==BasicTypeRecord::Primitive)
				{
					switch(type->PrimitiveType())
					{
					case s8:
					case s16:
					case s32:
					case s64:
						return BasicCompileTimeConstant::Signed;
					case u8:
					case u16:
					case u32:
					case u64:
						return BasicCompileTimeConstant::Unsigned;
					case f32:
					case f64:
						return BasicCompileTimeConstant::Float;
					case char_type:
					case wchar_type:
					case bool_type:
						return BasicCompileTimeConstant::Signed;
					}
				}
				else if(type->GetType()==BasicTypeRecord::Pointer)
				{
					return BasicCompileTimeConstant::Unsigned;
				}
				CHECK_FAIL(L"GetConstantType(BasicTypeRecord* type))#非法常熟类型。");
			}

/***********************************************************************
BasicLanguage_IsConstantExpression
***********************************************************************/

			BEGIN_ALGORITHM_FUNCTION(BasicLanguage_IsConstantExpression, BasicExpression, BP, bool)

				ALGORITHM_FUNCTION_MATCH(BasicNullExpression)
				{
					return true;
				}

				ALGORITHM_FUNCTION_MATCH(BasicExceptionAddressExpression)
				{
					return false;
				}

				ALGORITHM_FUNCTION_MATCH(BasicStackDataAddressExpression)
				{
					return false;
				}

				ALGORITHM_FUNCTION_MATCH(BasicNumericExpression)
				{
					return true;
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
					case BasicUnaryExpression::Negative:
					case BasicUnaryExpression::Not:
					case BasicUnaryExpression::BitNot:
						return BasicLanguage_IsConstantExpression(node->operand, argument);
					default:
						return false;
					}
				}

				ALGORITHM_FUNCTION_MATCH(BasicBinaryExpression)
				{
					switch(node->type)
					{
					case BasicBinaryExpression::Add:
					case BasicBinaryExpression::Sub:
					case BasicBinaryExpression::Mul:
					case BasicBinaryExpression::Div:
					case BasicBinaryExpression::Mod:
					case BasicBinaryExpression::Shl:
					case BasicBinaryExpression::Shr:
					case BasicBinaryExpression::And:
					case BasicBinaryExpression::Or:
					case BasicBinaryExpression::Xor:
					case BasicBinaryExpression::BitAnd:
					case BasicBinaryExpression::BitOr:
					case BasicBinaryExpression::Lt:
					case BasicBinaryExpression::Le:
					case BasicBinaryExpression::Gt:
					case BasicBinaryExpression::Ge:
					case BasicBinaryExpression::Eq:
					case BasicBinaryExpression::Ne:
						return
							BasicLanguage_IsConstantExpression(node->leftOperand, argument) &&
							BasicLanguage_IsConstantExpression(node->rightOperand, argument);
					default:
						return false;
					}
				}

				ALGORITHM_FUNCTION_MATCH(BasicSubscribeExpression)
				{
					return false;
				}

				ALGORITHM_FUNCTION_MATCH(BasicMemberExpression)
				{
					return false;
				}

				ALGORITHM_FUNCTION_MATCH(BasicInvokeExpression)
				{
					return false;
				}

				ALGORITHM_FUNCTION_MATCH(BasicFunctionResultExpression)
				{
					return false;
				}

				ALGORITHM_FUNCTION_MATCH(BasicSizeofExpression)
				{
					return true;
				}

				ALGORITHM_FUNCTION_MATCH(BasicOffsetofExpression)
				{
					return true;
				}

				ALGORITHM_FUNCTION_MATCH(BasicCastingExpression)
				{
					return BasicLanguage_IsConstantExpression(node->operand, argument) && node->type.Cast<BasicPrimitiveType>();
				}

				ALGORITHM_FUNCTION_MATCH(BasicReferenceExpression)
				{
					return false;
				}

				ALGORITHM_FUNCTION_MATCH(BasicInstanciatedExpression)
				{
					return false;
				}

				ALGORITHM_FUNCTION_MATCH(BasicInstanceFunctionExpression)
				{
					return false;
				}

				ALGORITHM_FUNCTION_MATCH(BasicExtendedExpression)
				{
					return false;
				}

			END_ALGORITHM_FUNCTION(BasicLanguage_IsConstantExpression)

/***********************************************************************
BasicLanguage_GetConstantValue
***********************************************************************/

			BEGIN_ALGORITHM_FUNCTION(BasicLanguage_GetConstantValue, BasicExpression, BP, BasicCompileTimeConstant)

				ALGORITHM_FUNCTION_MATCH(BasicNullExpression)
				{
					return BasicCompileTimeConstant((unsigned __int64)0);
				}

				ALGORITHM_FUNCTION_MATCH(BasicExceptionAddressExpression)
				{
					return BasicCompileTimeConstant();
				}

				ALGORITHM_FUNCTION_MATCH(BasicStackDataAddressExpression)
				{
					return BasicCompileTimeConstant();
				}

				ALGORITHM_FUNCTION_MATCH(BasicNumericExpression)
				{
					// TODO:
					switch(node->type)
					{
					case s8:
						return BasicCompileTimeConstant((signed __int64)node->argument.s8);
					case s16:
						return BasicCompileTimeConstant((signed __int64)node->argument.s16);
					case s32:
						return BasicCompileTimeConstant((signed __int64)node->argument.s32);
					case s64:
						return BasicCompileTimeConstant((signed __int64)node->argument.s64);
					case u8:
						return BasicCompileTimeConstant((unsigned __int64)node->argument.s8);
					case u16:
						return BasicCompileTimeConstant((unsigned __int64)node->argument.s16);
					case u32:
						return BasicCompileTimeConstant((unsigned __int64)node->argument.s32);
					case u64:
						return BasicCompileTimeConstant((unsigned __int64)node->argument.s64);
					case f32:
						return BasicCompileTimeConstant((double)node->argument.f32);
					case f64:
						return BasicCompileTimeConstant((double)node->argument.f64);
					case char_type:
						if(argument.configuration.treatCharAsSignedInteger)
						{
							return BasicCompileTimeConstant((signed __int64)(signed __int8)node->argument.char_value);
						}
						else
						{
							return BasicCompileTimeConstant((signed __int64)(unsigned __int8)node->argument.char_value);
						}
					case wchar_type:
						if(argument.configuration.treatWCharAsSignedInteger)
						{
							return BasicCompileTimeConstant((signed __int64)(signed __int16)node->argument.wchar_value);
						}
						else
						{
							return BasicCompileTimeConstant((signed __int64)(unsigned __int16)node->argument.wchar_value);
						}
					case bool_type:
						return BasicCompileTimeConstant(node->argument.bool_value);
					default:
						return BasicCompileTimeConstant();
					}
				}

				ALGORITHM_FUNCTION_MATCH(BasicMbcsStringExpression)
				{
					return BasicCompileTimeConstant();
				}

				ALGORITHM_FUNCTION_MATCH(BasicUnicodeStringExpression)
				{
					return BasicCompileTimeConstant();
				}

				ALGORITHM_FUNCTION_MATCH(BasicUnaryExpression)
				{
					BasicCompileTimeConstant::ConstantType constantType=GetConstantType(argument.env->GetExpressionType(node));
					BasicCompileTimeConstant value=BasicLanguage_GetConstantValue(node->operand, argument);
					BasicTypeRecord* valueType=argument.env->GetExpressionType(node->operand.Obj());
					switch(node->type)
					{
					case BasicUnaryExpression::Negative:
						switch(constantType)
						{
						case BasicCompileTimeConstant::Signed:
							return BasicCompileTimeConstant((signed __int64)-value.S(valueType));
						case BasicCompileTimeConstant::Unsigned:
							return BasicCompileTimeConstant((unsigned __int64)-value.S(valueType));
						case BasicCompileTimeConstant::Float:
							return BasicCompileTimeConstant((double)-value.F(valueType));
						}
						break;
					case BasicUnaryExpression::Not:
						return BasicCompileTimeConstant(!value.S(valueType));
					case BasicUnaryExpression::BitNot:
						switch(constantType)
						{
						case BasicCompileTimeConstant::Signed:
							return BasicCompileTimeConstant(~value.S(valueType));
						case BasicCompileTimeConstant::Unsigned:
							return BasicCompileTimeConstant(~value.U(valueType));
						}
						break;
					}
					return BasicCompileTimeConstant();
				}

#define NUMERIC_BINARY(OPERATOR)\
						switch(constantType)\
						{\
						case BasicCompileTimeConstant::Signed:\
							return BasicCompileTimeConstant(lv.S(lvt) OPERATOR rv.S(rvt));\
						case BasicCompileTimeConstant::Unsigned:\
							return BasicCompileTimeConstant(lv.U(lvt) OPERATOR rv.U(rvt));\
						case BasicCompileTimeConstant::Float:\
							return BasicCompileTimeConstant(lv.F(lvt) OPERATOR rv.F(rvt));\
						}\
						break

#define INTEGER_BINARY(OPERATOR)\
						switch(constantType)\
						{\
						case BasicCompileTimeConstant::Signed:\
							return BasicCompileTimeConstant(lv.S(lvt) OPERATOR rv.S(rvt));\
						case BasicCompileTimeConstant::Unsigned:\
							return BasicCompileTimeConstant(lv.U(lvt) OPERATOR rv.U(rvt));\
						}\
						break

#define BOOLEAN_BINARY(OPERATOR)\
						return BasicCompileTimeConstant((bool)(lv.S(lvt) OPERATOR rv.S(rvt)))

				ALGORITHM_FUNCTION_MATCH(BasicBinaryExpression)
				{
					BasicCompileTimeConstant::ConstantType constantType=GetConstantType(argument.env->GetExpressionType(node));
					BasicCompileTimeConstant lv=BasicLanguage_GetConstantValue(node->leftOperand, argument);
					BasicTypeRecord* lvt=argument.env->GetExpressionType(node->leftOperand.Obj());
					BasicCompileTimeConstant rv=BasicLanguage_GetConstantValue(node->rightOperand, argument);
					BasicTypeRecord* rvt=argument.env->GetExpressionType(node->rightOperand.Obj());

					switch(node->type)
					{
					case BasicBinaryExpression::Add: // TODO: process pointer
						NUMERIC_BINARY(+);
					case BasicBinaryExpression::Sub: // TODO: process pointer
						NUMERIC_BINARY(-);
					case BasicBinaryExpression::Mul:
						NUMERIC_BINARY(*);
					case BasicBinaryExpression::Div:
						NUMERIC_BINARY(/);
					case BasicBinaryExpression::Mod:
						INTEGER_BINARY(%);
					case BasicBinaryExpression::Shl:
						INTEGER_BINARY(<<);
					case BasicBinaryExpression::Shr:
						INTEGER_BINARY(>>);
					case BasicBinaryExpression::And:
						BOOLEAN_BINARY(&&);
					case BasicBinaryExpression::Or:
						BOOLEAN_BINARY(||);
					case BasicBinaryExpression::Xor:
						INTEGER_BINARY(^);
					case BasicBinaryExpression::BitAnd:
						INTEGER_BINARY(&);
					case BasicBinaryExpression::BitOr:
						INTEGER_BINARY(|);
					case BasicBinaryExpression::Lt:
						NUMERIC_BINARY(<);
					case BasicBinaryExpression::Le:
						NUMERIC_BINARY(<=);
					case BasicBinaryExpression::Gt:
						NUMERIC_BINARY(>);
					case BasicBinaryExpression::Ge:
						NUMERIC_BINARY(>=);
					case BasicBinaryExpression::Eq:
						NUMERIC_BINARY(==);
					case BasicBinaryExpression::Ne:
						NUMERIC_BINARY(!=);
					}
					return BasicCompileTimeConstant();
				}
				
#undef BOOLEAN_BINARY
#undef INTEGER_BINARY
#undef NUMERIC_BINARY

				ALGORITHM_FUNCTION_MATCH(BasicSubscribeExpression)
				{
					return BasicCompileTimeConstant();
				}

				ALGORITHM_FUNCTION_MATCH(BasicMemberExpression)
				{
					return BasicCompileTimeConstant();
				}

				ALGORITHM_FUNCTION_MATCH(BasicInvokeExpression)
				{
					return BasicCompileTimeConstant();
				}

				ALGORITHM_FUNCTION_MATCH(BasicFunctionResultExpression)
				{
					return BasicCompileTimeConstant();
				}

				ALGORITHM_FUNCTION_MATCH(BasicSizeofExpression)
				{
					// TODO:
					return BasicCompileTimeConstant();
				}

				ALGORITHM_FUNCTION_MATCH(BasicOffsetofExpression)
				{
					// TODO:
					return BasicCompileTimeConstant();
				}

				ALGORITHM_FUNCTION_MATCH(BasicCastingExpression)
				{
					BasicCompileTimeConstant::ConstantType constantType=GetConstantType(argument.env->GetExpressionType(node));
					BasicCompileTimeConstant value=BasicLanguage_GetConstantValue(node->operand, argument);
					BasicTypeRecord* valueType=argument.env->GetExpressionType(node->operand.Obj());

					switch(constantType)
					{
					case BasicCompileTimeConstant::Signed:
						return BasicCompileTimeConstant(value.S(valueType));
					case BasicCompileTimeConstant::Unsigned:
						return BasicCompileTimeConstant(value.U(valueType));
					case BasicCompileTimeConstant::Float:
						return BasicCompileTimeConstant(value.F(valueType));
					}
					return BasicCompileTimeConstant();
				}

				ALGORITHM_FUNCTION_MATCH(BasicReferenceExpression)
				{
					return BasicCompileTimeConstant();
				}

				ALGORITHM_FUNCTION_MATCH(BasicInstanciatedExpression)
				{
					return BasicCompileTimeConstant();
				}

				ALGORITHM_FUNCTION_MATCH(BasicInstanceFunctionExpression)
				{
					return BasicCompileTimeConstant();
				}

				ALGORITHM_FUNCTION_MATCH(BasicExtendedExpression)
				{
					return BasicCompileTimeConstant();
				}

			END_ALGORITHM_FUNCTION(BasicLanguage_GetConstantValue)
		}
	}
}