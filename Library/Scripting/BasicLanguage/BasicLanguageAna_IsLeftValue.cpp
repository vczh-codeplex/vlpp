#include "BasicLanguageAnalyzer.h"
#include "..\..\Collections\OperationCopyFrom.h"

namespace vl
{
	namespace scripting
	{
		namespace basiclanguage
		{
			using namespace collections;

/***********************************************************************
BasicLanguage_IsLeftValue
***********************************************************************/

			BEGIN_ALGORITHM_FUNCTION(BasicLanguage_IsLeftValue, BasicExpression, BP, bool)

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
					return true;
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
					case BasicUnaryExpression::DereferencePointer:
						return true;
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
						return true;
					default:
						return false;
					}
				}

				ALGORITHM_FUNCTION_MATCH(BasicSubscribeExpression)
				{
					BasicTypeRecord* operandType=argument.env->GetExpressionType(node->operand.Obj());
					return (operandType && operandType->GetType()==BasicTypeRecord::Pointer) || BasicLanguage_IsLeftValue(node->operand, argument);
				}

				ALGORITHM_FUNCTION_MATCH(BasicMemberExpression)
				{
					return node->pointerMember || BasicLanguage_IsLeftValue(node->operand, argument);
				}

				ALGORITHM_FUNCTION_MATCH(BasicInvokeExpression)
				{
					return false;
				}

				ALGORITHM_FUNCTION_MATCH(BasicFunctionResultExpression)
				{
					return true;
				}

				ALGORITHM_FUNCTION_MATCH(BasicSizeofExpression)
				{
					return false;
				}

				ALGORITHM_FUNCTION_MATCH(BasicOffsetofExpression)
				{
					return false;
				}

				ALGORITHM_FUNCTION_MATCH(BasicCastingExpression)
				{
					return false;
				}

				ALGORITHM_FUNCTION_MATCH(BasicReferenceExpression)
				{
					return argument.env->GetReference(node).isVariable;
				}

				ALGORITHM_FUNCTION_MATCH(BasicInstanciatedExpression)
				{
					return argument.env->GetReference(node->reference.Obj()).isVariable;
				}

				ALGORITHM_FUNCTION_MATCH(BasicInstanceFunctionExpression)
				{
					return false;
				}

				ALGORITHM_FUNCTION_MATCH(BasicExtendedExpression)
				{
					return argument.semanticExtension->IsLeftValue(node, argument);
				}

			END_ALGORITHM_FUNCTION(BasicLanguage_IsLeftValue)
		}
	}
}