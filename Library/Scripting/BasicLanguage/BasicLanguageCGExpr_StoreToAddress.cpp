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
				const BCP&					argument;
				BasicExpression*			expressionAddress;
				BasicVariableStatement*		variableStatement;
				BasicVariableDeclaration*	variableDeclaration;

				StoreToAddressParameter(const BCP& _argument, BasicExpression* _expressionAddress)
					:argument(_argument)
					,expressionAddress(_expressionAddress)
					,variableStatement(0)
					,variableDeclaration(0)
				{
				}

				StoreToAddressParameter(const BCP& _argument, BasicVariableStatement* _variableStatement)
					:argument(_argument)
					,expressionAddress(0)
					,variableStatement(_variableStatement)
					,variableDeclaration(0)
				{
				}

				StoreToAddressParameter(const BCP& _argument, BasicVariableDeclaration* _variableDeclaration)
					:argument(_argument)
					,expressionAddress(0)
					,variableStatement(0)
					,variableDeclaration(_variableDeclaration)
				{
				}

				void WriteAddress()const
				{
					if(expressionAddress)
					{
						BasicLanguage_PushRef(expressionAddress, argument);
					}
					else if(variableStatement)
					{
						int offset=argument.info->GetLocalVariableOffsets()[variableStatement];
						argument.il->Ins(BasicIns::stack_offset, BasicIns::MakeInt(offset));
					}
					else if(variableDeclaration)
					{
						int offset=argument.info->GetGlobalVariableOffsets()[variableDeclaration];
						argument.il->Ins(BasicIns::link_pushdata, BasicIns::MakeInt(offset));
					}
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

			void BasicLanguage_StoreToAddress(BasicExpression* expression, const StoreToAddressParameter& stap, BasicTypeRecord* addressType, const BCP& argument)
			{
				BasicTypeRecord* expressionType=argument.info->GetEnv()->GetExpressionType(expression);
				if(expressionType==addressType)
				{
					if(BasicLanguage_StoreToAddressInternal(expression, stap))
					{
						return;
					}
				}

				BasicLanguage_PushValue(expression, argument, addressType);
				stap.WriteAddress();
				Code_Write(addressType, argument);
			}

			void BasicLanguage_StoreToAddress(BasicExpression* expression, BasicExpression* address, const BCP& argument)
			{
				BasicTypeRecord* addressType=argument.info->GetEnv()->GetExpressionType(address);
				StoreToAddressParameter stap(argument, address);
				BasicLanguage_StoreToAddress(expression, stap, addressType, argument);
			}

			void BasicLanguage_StoreToAddress(BasicExpression* expression, BasicVariableStatement* address, const BCP& argument)
			{
				BasicScope* scope=argument.info->GetEnv()->GetStatementScope(address);
				BasicTypeRecord* addressType=scope->variables.Items()[address->name].type;
				StoreToAddressParameter stap(argument, address);
				BasicLanguage_StoreToAddress(expression, stap, addressType, argument);
			}

			void BasicLanguage_StoreToAddress(BasicExpression* expression, BasicVariableDeclaration* address, const BCP& argument)
			{
				BasicScope* scope=argument.info->GetEnv()->GlobalScope();
				BasicTypeRecord* addressType=scope->variables.Items()[address->name].type;
				StoreToAddressParameter stap(argument, address);
				BasicLanguage_StoreToAddress(expression, stap, addressType, argument);
			}
		}
	}
}