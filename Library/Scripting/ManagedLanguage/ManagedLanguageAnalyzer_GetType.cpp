#include "ManagedLanguageAnalyzer.h"
#include "..\..\Collections\OperationForEach.h"

namespace vl
{
	namespace scripting
	{
		namespace managedlanguage
		{
			using namespace collections;

/***********************************************************************
GetType
***********************************************************************/

			ManagedTypeSymbol* GetType(ManagedExpression* node, ManagedTypeSymbol* expectedType, const MAP& argument)
			{
				MAP newArgument(argument, argument.currentSymbol, expectedType);
				ManagedTypeSymbol* result=ManagedLanguage_GetTypeInternal_Expression(node, argument);
				argument.contextManager->SetExpression(node, result, argument.currentSymbol);
				return result;
			}

/***********************************************************************
ManagedLanguage_GetTypeInternal_Expression
***********************************************************************/

			BEGIN_ALGORITHM_FUNCTION(ManagedLanguage_GetTypeInternal_Expression, ManagedExpression, MAP, ManagedTypeSymbol*)

				ALGORITHM_FUNCTION_MATCH(ManagedNullExpression)
				{
					throw 0;
				}

				ALGORITHM_FUNCTION_MATCH(ManagedIntegerExpression)
				{
					throw 0;
				}

				ALGORITHM_FUNCTION_MATCH(ManagedFloatExpression)
				{
					throw 0;
				}

				ALGORITHM_FUNCTION_MATCH(ManagedBooleanExpression)
				{
					throw 0;
				}

				ALGORITHM_FUNCTION_MATCH(ManagedCharExpression)
				{
					throw 0;
				}

				ALGORITHM_FUNCTION_MATCH(ManagedStringExpression)
				{
					throw 0;
				}

				ALGORITHM_FUNCTION_MATCH(ManagedReferenceExpression)
				{
					throw 0;
				}

				ALGORITHM_FUNCTION_MATCH(ManagedMemberExpression)
				{
					throw 0;
				}

				ALGORITHM_FUNCTION_MATCH(ManagedInstantiatedExpression)
				{
					throw 0;
				}

				ALGORITHM_FUNCTION_MATCH(ManagedInvokeExpression)
				{
					throw 0;
				}

				ALGORITHM_FUNCTION_MATCH(ManagedNewObjectExpression)
				{
					throw 0;
				}

				ALGORITHM_FUNCTION_MATCH(ManagedFunctionResultExpression)
				{
					throw 0;
				}

				ALGORITHM_FUNCTION_MATCH(ManagedThisExpression)
				{
					throw 0;
				}

				ALGORITHM_FUNCTION_MATCH(ManagedBaseExpression)
				{
					throw 0;
				}

				ALGORITHM_FUNCTION_MATCH(ManagedAssignmentExpression)
				{
					throw 0;
				}

				ALGORITHM_FUNCTION_MATCH(ManagedExtendedExpression)
				{
					throw ManagedLanguage_GetTypeInternal_ExtendedExpression(node, argument);
				}

			END_ALGORITHM_FUNCTION(ManagedLanguage_GetTypeInternal_Expression)

/***********************************************************************
ManagedLanguage_GetTypeInternal_ExtendedExpression
***********************************************************************/

			BEGIN_ALGORITHM_FUNCTION(ManagedLanguage_GetTypeInternal_ExtendedExpression, ManagedExtendedExpression, MAP, ManagedTypeSymbol*)

				ALGORITHM_FUNCTION_MATCH(ManagedLambdaExpression)
				{
					throw 0;
				}

				ALGORITHM_FUNCTION_MATCH(ManagedChoiceExpression)
				{
					throw 0;
				}

				ALGORITHM_FUNCTION_MATCH(ManagedNullChoiceExpression)
				{
					throw 0;
				}

				ALGORITHM_FUNCTION_MATCH(ManagedTypeofExpression)
				{
					throw 0;
				}

				ALGORITHM_FUNCTION_MATCH(ManagedDefaultExpression)
				{
					throw 0;
				}

				ALGORITHM_FUNCTION_MATCH(ManagedUnaryExpression)
				{
					throw 0;
				}

				ALGORITHM_FUNCTION_MATCH(ManagedBinaryExpression)
				{
					throw 0;
				}

				ALGORITHM_FUNCTION_MATCH(ManagedNewArrayExpression)
				{
					throw 0;
				}

				ALGORITHM_FUNCTION_MATCH(ManagedIsTypeExpression)
				{
					throw 0;
				}

				ALGORITHM_FUNCTION_MATCH(ManagedCastingExpression)
				{
					throw 0;
				}

				ALGORITHM_FUNCTION_MATCH(ManagedIndexExpression)
				{
					throw 0;
				}

			END_ALGORITHM_FUNCTION(ManagedLanguage_GetTypeInternal_ExtendedExpression)
		}
	}
}