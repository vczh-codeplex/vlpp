#include "BasicLanguageAnalyzer.h"

namespace vl
{
	namespace scripting
	{
		namespace basiclanguage
		{
			using namespace collections;

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
					BasicTypeRecord* lvt=argument.env->GetExpressionType(node->leftOperand.Obj());
					if(lvt->GetType()==BasicTypeRecord::Pointer)
					{
						BasicTypeInfo* info=argument.typeInfoManager->GetTypeInfo(lvt->ElementType());
						if(!info->size.IsConstant()) return false;
					}

					BasicTypeRecord* rvt=argument.env->GetExpressionType(node->rightOperand.Obj());
					if(rvt->GetType()==BasicTypeRecord::Pointer)
					{
						BasicTypeInfo* info=argument.typeInfoManager->GetTypeInfo(rvt->ElementType());
						if(!info->size.IsConstant()) return false;
					}

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
					BasicTypeRecord* nodeType=BasicLanguage_GetTypeRecord(node->type, argument, false);
					BasicOffset size=argument.typeInfoManager->GetTypeInfo(nodeType)->size;
					return size.IsConstant();
				}

				ALGORITHM_FUNCTION_MATCH(BasicOffsetofExpression)
				{
					BasicTypeRecord* structureType=BasicLanguage_GetTypeRecord(node->type, argument, false);
					BasicTypeInfo* structureInfo=argument.typeInfoManager->GetTypeInfo(structureType);
					BasicOffset offset=structureInfo->offsets[structureType->MemberNameIndex(node->member)];
					return offset.IsConstant();
				}

				ALGORITHM_FUNCTION_MATCH(BasicCastingExpression)
				{
					BasicTypeRecord* castType=BasicLanguage_GetTypeRecord(node->type, argument, false);
					return BasicLanguage_IsConstantExpression(node->operand, argument) &&
						(castType->GetType()==BasicTypeRecord::Primitive || castType->GetType()==BasicTypeRecord::Pointer);
				}

				ALGORITHM_FUNCTION_MATCH(BasicReferenceExpression)
				{
					BasicEnv::Reference reference=argument.env->GetReference(node);
					return reference.isVariable && reference.globalVariable && reference.globalVariable->constant;
				}

				ALGORITHM_FUNCTION_MATCH(BasicInstantiatedExpression)
				{
					return false;
				}

				ALGORITHM_FUNCTION_MATCH(BasicInstanceFunctionExpression)
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
					case BasicBinaryExpression::Add:
						if(lvt->GetType()==BasicTypeRecord::Pointer)
						{
							vint size=argument.typeInfoManager->GetTypeInfo(lvt->ElementType())->size.Constant();
							return BasicCompileTimeConstant(lv.S(lvt) + size*rv.S(lvt));
						}
						else if(rvt->GetType()==BasicTypeRecord::Pointer)
						{
							vint size=argument.typeInfoManager->GetTypeInfo(rvt->ElementType())->size.Constant();
							return BasicCompileTimeConstant(size*lv.S(lvt) + rv.S(lvt));
						}
						else
						{
							NUMERIC_BINARY(+);
						}
					case BasicBinaryExpression::Sub:
						if(lvt->GetType()==BasicTypeRecord::Pointer)
						{
							vint size=argument.typeInfoManager->GetTypeInfo(lvt->ElementType())->size.Constant();
							if(rvt->GetType()==BasicTypeRecord::Pointer)
							{
								return BasicCompileTimeConstant((lv.S(lvt) - rv.S(rvt))/size);
							}
							else
							{
								return BasicCompileTimeConstant(lv.S(lvt) - size*rv.S(lvt));
							}
						}
						else
						{
							NUMERIC_BINARY(-);
						}
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
					BasicTypeRecord* nodeType=BasicLanguage_GetTypeRecord(node->type, argument, false);
					BasicOffset size=argument.typeInfoManager->GetTypeInfo(nodeType)->size;
					return BasicCompileTimeConstant((signed __int64)size.Constant());
				}

				ALGORITHM_FUNCTION_MATCH(BasicOffsetofExpression)
				{
					BasicTypeRecord* structureType=BasicLanguage_GetTypeRecord(node->type, argument, false);
					BasicTypeInfo* structureInfo=argument.typeInfoManager->GetTypeInfo(structureType);
					BasicOffset offset=structureInfo->offsets[structureType->MemberNameIndex(node->member)];
					return BasicCompileTimeConstant((signed __int64)offset.Constant());
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
					return argument.scope->variables.Find(node->name).constantValue;
				}

				ALGORITHM_FUNCTION_MATCH(BasicInstantiatedExpression)
				{
					return BasicCompileTimeConstant();
				}

				ALGORITHM_FUNCTION_MATCH(BasicInstanceFunctionExpression)
				{
					return BasicCompileTimeConstant();
				}

			END_ALGORITHM_FUNCTION(BasicLanguage_GetConstantValue)
		}
	}
}