#include "BasicLanguageCodeGeneration.h"

namespace vl
{
	namespace scripting
	{
		namespace basiclanguage
		{
			using namespace basicil;
			using namespace stream;

/***********************************************************************
BasicLanguage_StoreToAddressInternal
***********************************************************************/

			struct StoreToAddressParameter
			{
				const BCP& argument;
				BasicExpression* expressionAddress;

				StoreToAddressParameter(const BCP& _argument, BasicExpression* _expressionAddress)
					:argument(_argument)
					,expressionAddress(_expressionAddress)
				{
				}

				void WriteAddress()const
				{
					BasicLanguage_PushRef(expressionAddress, argument);
				}
			};

			BEGIN_ALGORITHM_FUNCTION(BasicLanguage_StoreToAddressInternal, BasicExpression, StoreToAddressParameter, bool)

				ALGORITHM_FUNCTION_MATCH(BasicNullExpression)
				{
					return false;
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
					return false;
				}

				ALGORITHM_FUNCTION_MATCH(BasicBinaryExpression)
				{
					return false;
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
					int index=0;
					int returnSize=0;
					int parameterSize=0;
					Code_InvokeFunctionPushParameters(node, argument.argument, index, returnSize, parameterSize, false);
					argument.WriteAddress();
					Code_InvokeFunctionCallFunction(node, argument.argument, index, returnSize, parameterSize, false);
					return true;
				}

				ALGORITHM_FUNCTION_MATCH(BasicFunctionResultExpression)
				{
					return false;
				}

				ALGORITHM_FUNCTION_MATCH(BasicCastingExpression)
				{
					return false;
				}

				ALGORITHM_FUNCTION_MATCH(BasicReferenceExpression)
				{
					return false;
				}

				ALGORITHM_FUNCTION_MATCH(BasicExtendedExpression)
				{
					return false;
				}

			END_ALGORITHM_FUNCTION(BasicLanguage_StoreToAddressInternal)

			void BasicLanguage_StoreToAddress(BasicExpression* expression, BasicExpression* address, const BCP& argument)
			{
				BasicTypeRecord* expressionType=argument.info->GetEnv()->GetExpressionType(expression);
				BasicTypeRecord* addressType=argument.info->GetEnv()->GetExpressionType(address);

				StoreToAddressParameter stap(argument, address);
				if(expressionType==addressType)
				{
					if(BasicLanguage_StoreToAddressInternal(expression, stap))
					{
						return;
					}
				}

				BasicLanguage_PushValue(expression, argument, addressType);
				BasicLanguage_PushRef(address, argument);
				Code_Write(addressType, argument);
			}
		}
	}
}