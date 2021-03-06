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

#define BASIC_LANGUAGE_INVOKE_ALGORITHM_INITIALIZER\
			BasicLanguageElement* lastCurrentLanguageElement;\
			ALGORITHM_BEFORE_CALL\
			{\
				lastCurrentLanguageElement=argument.currentLanguageElement;\
				const_cast<BCP&>(argument).currentLanguageElement=node;\
			}\
			ALGORITHM_AFTER_CALL\
			{\
				const_cast<BCP&>(argument).currentLanguageElement=lastCurrentLanguageElement;\
			}

/***********************************************************************
BasicLanguage_InvokeInternal
***********************************************************************/

			BEGIN_ALGORITHM_FUNCTION(BasicLanguage_InvokeInternal, BasicExpression, BCP, bool)
				BASIC_LANGUAGE_INVOKE_ALGORITHM_INITIALIZER

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
					return false;
				}

				ALGORITHM_FUNCTION_MATCH(BasicFunctionResultExpression)
				{
					return false;
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
					vint index=GetFunctionIndex(node, argument);
					if(index==-1)
					{
						return false;
					}
					else
					{
						if(IsExternalFunction(node, argument))
						{
							argument.Ins(BasicIns::link_callfarfunc, BasicIns::MakeInt(index));
						}
						else if(IsForeignFunction(node, argument))
						{
							argument.Ins(BasicIns::link_callforeignfunc, BasicIns::MakeInt(index));
						}
						else
						{
							argument.Ins(BasicIns::codegen_callfunc, BasicIns::MakeInt(index));
						}
						return true;
					}
				}

				ALGORITHM_FUNCTION_MATCH(BasicInstantiatedExpression)
				{
					BasicTypeRecord* resultType=0;
					vint index=GetGenericFunctionTargetIndex(node, argument, resultType);
					if(index==-1)
					{
						return false;
					}
					else
					{
						argument.Ins(BasicIns::generic_callfunc, BasicIns::MakeInt(index));
						return true;
					}
				}

				ALGORITHM_FUNCTION_MATCH(BasicInstanceFunctionExpression)
				{
					vint index=GetGenericInstanceTargetIndex(node, argument);
					argument.Ins(BasicIns::generic_instance_callfunc, BasicIns::MakeInt(index));
					return true;
				}

			END_ALGORITHM_FUNCTION(BasicLanguage_InvokeInternal)

			void BasicLanguage_Invoke(BasicExpression* expression, const BCP& argument)
			{
				if(!BasicLanguage_InvokeInternal(expression, argument))
				{
					BasicLanguage_PushValue(expression, argument);
					argument.Ins(BasicIns::label);
					argument.Ins(BasicIns::call_indirect);
				}
			}
		}
	}
}