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
BuildLocalScope
***********************************************************************/

			void BuildLocalScope(ManagedStatement* node, const MAP& argument)
			{
				ManagedLanguage_BuildLocalScopeInternal_Statement(node, argument);
				argument.contextManager->SetStatement(node, argument.currentSymbol);
			}

/***********************************************************************
ManagedLanguage_BuildLocalScopeInternal_Statement
***********************************************************************/

			BEGIN_ALGORITHM_PROCEDURE(ManagedLanguage_BuildLocalScopeInternal_Statement, ManagedStatement, MAP)

				ALGORITHM_PROCEDURE_MATCH(ManagedEmptyStatement)
				{
				}

				ALGORITHM_PROCEDURE_MATCH(ManagedCompositeStatement)
				{
				}

				ALGORITHM_PROCEDURE_MATCH(ManagedExpressionStatement)
				{
				}

				ALGORITHM_PROCEDURE_MATCH(ManagedVariableStatement)
				{
				}

				ALGORITHM_PROCEDURE_MATCH(ManagedIfStatement)
				{
				}

				ALGORITHM_PROCEDURE_MATCH(ManagedWhileStatement)
				{
				}

				ALGORITHM_PROCEDURE_MATCH(ManagedBreakStatement)
				{
				}

				ALGORITHM_PROCEDURE_MATCH(ManagedContinueStatement)
				{
				}

				ALGORITHM_PROCEDURE_MATCH(ManagedReturnStatement)
				{
				}

				ALGORITHM_PROCEDURE_MATCH(ManagedTryCatchStatement)
				{
				}

				ALGORITHM_PROCEDURE_MATCH(ManagedThrowStatement)
				{
				}

				ALGORITHM_PROCEDURE_MATCH(ManagedExtendedStatement)
				{
					ManagedLanguage_BuildLocalScopeInternal_ExtendedStatement(node, argument);
				}

			END_ALGORITHM_PROCEDURE(ManagedLanguage_BuildLocalScopeInternal_Statement)

/***********************************************************************
ManagedLanguage_BuildLocalScopeInternal_ExtendedStatement
***********************************************************************/

			BEGIN_ALGORITHM_PROCEDURE(ManagedLanguage_BuildLocalScopeInternal_ExtendedStatement, ManagedExtendedStatement, MAP)

				ALGORITHM_PROCEDURE_MATCH(ManagedUsingStatement)
				{
				}

				ALGORITHM_PROCEDURE_MATCH(ManagedLockStatement)
				{
				}

				ALGORITHM_PROCEDURE_MATCH(ManagedSelectStatement)
				{
				}

				ALGORITHM_PROCEDURE_MATCH(ManagedForStatement)
				{
				}

				ALGORITHM_PROCEDURE_MATCH(ManagedForEachStatement)
				{
				}

			END_ALGORITHM_PROCEDURE(ManagedLanguage_BuildLocalScopeInternal_ExtendedStatement)
		}
	}
}