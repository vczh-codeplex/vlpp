#include "BasicLanguageCodeGeneration.h"
#include "BasicLanguageAnalyzer.h"

namespace vl
{
	namespace scripting
	{
		namespace basiclanguage
		{
			using namespace basicil;
			using namespace stream;
			using namespace collections;

#define BASIC_LANGUAGE_STORETOADDRESS_ALGORITHM_INITIALIZER\
			BasicLanguageElement* lastCurrentLanguageElement;\
			ALGORITHM_BEFORE_CALL\
			{\
				lastCurrentLanguageElement=argument.argument.currentLanguageElement;\
				const_cast<BCP&>(argument.argument).currentLanguageElement=node;\
			}\
			ALGORITHM_AFTER_CALL\
			{\
				const_cast<BCP&>(argument.argument).currentLanguageElement=lastCurrentLanguageElement;\
			}

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
						BasicOffset offset=argument.info->GetLocalVariableOffsets()[variableStatement];
						argument.Ins(BasicIns::stack_offset, offset);
					}
					else if(variableDeclaration)
					{
						vint offset=argument.info->GetGlobalVariableOffsets()[variableDeclaration];
						if(variableDeclaration->linking.HasLink())
						{
							argument.Ins(BasicIns::link_pushforeigndata, BasicIns::MakeInt(offset));
						}
						else
						{
							argument.Ins(BasicIns::link_pushdata, BasicIns::MakeInt(offset));
						}
					}
				}
			};

			BEGIN_ALGORITHM_FUNCTION(BasicLanguage_StoreToAddressInternal, BasicExpression, StoreToAddressParameter, bool)
				BASIC_LANGUAGE_STORETOADDRESS_ALGORITHM_INITIALIZER

				ALGORITHM_FUNCTION_MATCH(BasicNullExpression)
				{
					return false;
				}

				ALGORITHM_FUNCTION_MATCH(BasicExceptionAddressExpression)
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
					BasicOffset returnSize=0;
					BasicOffset parameterSize=0;
					Code_InvokeFunctionPushParameters(node, argument.argument, returnSize, parameterSize, false);
					argument.WriteAddress();
					Code_InvokeFunctionCallFunction(node, argument.argument, returnSize, parameterSize, false);
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

				BP bp(
					argument.info->GetEnv(),
					0,
					argument.info->GetTypeManager(),
					*(List<Ptr<BasicLanguageCodeException>>*)0,
					*(SortedList<WString>*)0
					);
				if(expressionType==addressType && BasicLanguage_IsLeftValue(expression, bp))
				{
					BasicLanguage_PushRef(expression, argument);
					stap.WriteAddress();
					Code_Copy(addressType, argument);
				}
				else
				{
					BasicLanguage_PushValue(expression, argument, addressType);
					stap.WriteAddress();
					Code_Write(addressType, argument);
				}
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