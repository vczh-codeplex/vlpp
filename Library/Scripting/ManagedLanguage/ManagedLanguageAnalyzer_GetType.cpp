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
ManagedLanguage_GetType_Expression
***********************************************************************/

			BEGIN_ALGORITHM_FUNCTION(ManagedLanguage_GetType_Expression, ManagedExpression, MAP, ManagedTypeSymbol*)

				ALGORITHM_FUNCTION_MATCH(ManagedNullExpression)
				{
					return 0;
				}

				ALGORITHM_FUNCTION_MATCH(ManagedIntegerExpression)
				{
					return 0;
				}

				ALGORITHM_FUNCTION_MATCH(ManagedFloatExpression)
				{
					return 0;
				}

				ALGORITHM_FUNCTION_MATCH(ManagedBooleanExpression)
				{
					return 0;
				}

				ALGORITHM_FUNCTION_MATCH(ManagedCharExpression)
				{
					return 0;
				}

				ALGORITHM_FUNCTION_MATCH(ManagedStringExpression)
				{
					return 0;
				}

				ALGORITHM_FUNCTION_MATCH(ManagedReferenceExpression)
				{
					return 0;
				}

				ALGORITHM_FUNCTION_MATCH(ManagedMemberExpression)
				{
					return 0;
				}

				ALGORITHM_FUNCTION_MATCH(ManagedInstantiatedExpression)
				{
					return 0;
				}

				ALGORITHM_FUNCTION_MATCH(ManagedInvokeExpression)
				{
					return 0;
				}

				ALGORITHM_FUNCTION_MATCH(ManagedNewObjectExpression)
				{
					return 0;
				}

				ALGORITHM_FUNCTION_MATCH(ManagedFunctionResultExpression)
				{
					return 0;
				}

				ALGORITHM_FUNCTION_MATCH(ManagedThisExpression)
				{
					return 0;
				}

				ALGORITHM_FUNCTION_MATCH(ManagedBaseExpression)
				{
					return 0;
				}

				ALGORITHM_FUNCTION_MATCH(ManagedAssignmentExpression)
				{
					return 0;
				}

				ALGORITHM_FUNCTION_MATCH(ManagedExtendedExpression)
				{
					return ManagedLanguage_GetType_ExtendedExpression(node, argument);
				}

			END_ALGORITHM_FUNCTION(ManagedLanguage_GetType_Expression)

/***********************************************************************
ManagedLanguage_GetType_ExtendedExpression
***********************************************************************/

			BEGIN_ALGORITHM_FUNCTION(ManagedLanguage_GetType_ExtendedExpression, ManagedExtendedExpression, MAP, ManagedTypeSymbol*)

				ALGORITHM_FUNCTION_MATCH(ManagedLambdaExpression)
				{
					return 0;
				}

				ALGORITHM_FUNCTION_MATCH(ManagedChoiceExpression)
				{
					return 0;
				}

				ALGORITHM_FUNCTION_MATCH(ManagedNullChoiceExpression)
				{
					return 0;
				}

				ALGORITHM_FUNCTION_MATCH(ManagedTypeofExpression)
				{
					return 0;
				}

				ALGORITHM_FUNCTION_MATCH(ManagedDefaultExpression)
				{
					return 0;
				}

				ALGORITHM_FUNCTION_MATCH(ManagedUnaryExpression)
				{
					return 0;
				}

				ALGORITHM_FUNCTION_MATCH(ManagedBinaryExpression)
				{
					return 0;
				}

				ALGORITHM_FUNCTION_MATCH(ManagedNewArrayExpression)
				{
					return 0;
				}

				ALGORITHM_FUNCTION_MATCH(ManagedIsTypeExpression)
				{
					return 0;
				}

				ALGORITHM_FUNCTION_MATCH(ManagedCastingExpression)
				{
					return 0;
				}

				ALGORITHM_FUNCTION_MATCH(ManagedIndexExpression)
				{
					return 0;
				}

			END_ALGORITHM_FUNCTION(ManagedLanguage_GetType_ExtendedExpression)
		}
	}
}