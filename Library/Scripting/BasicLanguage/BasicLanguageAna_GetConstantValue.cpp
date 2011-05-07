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
					return false;
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

			BEGIN_ALGORITHM_FUNCTION(BasicLanguage_GetConstantValue, BasicExpression, BP, Ptr<BasicNumericExpression>)

				ALGORITHM_FUNCTION_MATCH(BasicNullExpression)
				{
					return 0;
				}

				ALGORITHM_FUNCTION_MATCH(BasicExceptionAddressExpression)
				{
					return 0;
				}

				ALGORITHM_FUNCTION_MATCH(BasicStackDataAddressExpression)
				{
					return 0;
				}

				ALGORITHM_FUNCTION_MATCH(BasicNumericExpression)
				{
					Ptr<BasicNumericExpression> expression=new BasicNumericExpression;
					expression->implicitIntegerType=node->implicitIntegerType;
					expression->type=node->type;
					expression->argument=node->argument;
					return expression;
				}

				ALGORITHM_FUNCTION_MATCH(BasicMbcsStringExpression)
				{
					return 0;
				}

				ALGORITHM_FUNCTION_MATCH(BasicUnicodeStringExpression)
				{
					return 0;
				}

				ALGORITHM_FUNCTION_MATCH(BasicUnaryExpression)
				{
					// TODO:
					return 0;
				}

				ALGORITHM_FUNCTION_MATCH(BasicBinaryExpression)
				{
					// TODO:
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

				ALGORITHM_FUNCTION_MATCH(BasicSizeofExpression)
				{
					// TODO:
					return 0;
				}

				ALGORITHM_FUNCTION_MATCH(BasicOffsetofExpression)
				{
					// TODO:
					return 0;
				}

				ALGORITHM_FUNCTION_MATCH(BasicCastingExpression)
				{
					// TODO:
					return 0;
				}

				ALGORITHM_FUNCTION_MATCH(BasicReferenceExpression)
				{
					return 0;
				}

				ALGORITHM_FUNCTION_MATCH(BasicInstanciatedExpression)
				{
					return 0;
				}

				ALGORITHM_FUNCTION_MATCH(BasicInstanceFunctionExpression)
				{
					return 0;
				}

				ALGORITHM_FUNCTION_MATCH(BasicExtendedExpression)
				{
					return 0;
				}

			END_ALGORITHM_FUNCTION(BasicLanguage_GetConstantValue)
		}
	}
}