#include "ManagedXCodeGenerator.h"

namespace vl
{
	namespace scripting
	{
		namespace language_managedx
		{
/***********************************************************************
Basic Types
***********************************************************************/

			BEGIN_ALGORITHM_PROCEDURE(ManagedX_GenerateCode_Type, ManagedType, MXCGP)

				ALGORITHM_PROCEDURE_MATCH(ManagedGenericArgumentType)
				{
				}

				ALGORITHM_PROCEDURE_MATCH(ManagedReferencedType)
				{
				}

				ALGORITHM_PROCEDURE_MATCH(ManagedMemberType)
				{
				}

				ALGORITHM_PROCEDURE_MATCH(ManagedInstantiatedGenericType)
				{
				}

				ALGORITHM_PROCEDURE_MATCH(ManagedExtendedType)
				{
					ManagedX_GenerateCode_ExtendedType(node, argument);
				}

			END_ALGORITHM_PROCEDURE(ManagedX_GenerateCode_Type)

/***********************************************************************
Extended Types
***********************************************************************/

			BEGIN_ALGORITHM_PROCEDURE(ManagedX_GenerateCode_ExtendedType, ManagedExtendedType, MXCGP)

				ALGORITHM_PROCEDURE_MATCH(ManagedArrayType)
				{
				}

				ALGORITHM_PROCEDURE_MATCH(ManagedFunctionType)
				{
				}

				ALGORITHM_PROCEDURE_MATCH(ManagedEventType)
				{
				}

				ALGORITHM_PROCEDURE_MATCH(ManagedAutoReferType)
				{
				}

			END_ALGORITHM_PROCEDURE(ManagedX_GenerateCode_ExtendedType)

/***********************************************************************
Basic Expressions
***********************************************************************/

			BEGIN_ALGORITHM_PROCEDURE(ManagedX_GenerateCode_Expression, ManagedExpression, MXCGP)

				ALGORITHM_PROCEDURE_MATCH(ManagedNullExpression)
				{
				}

				ALGORITHM_PROCEDURE_MATCH(ManagedIntegerExpression)
				{
				}

				ALGORITHM_PROCEDURE_MATCH(ManagedFloatExpression)
				{
				}

				ALGORITHM_PROCEDURE_MATCH(ManagedBooleanExpression)
				{
				}

				ALGORITHM_PROCEDURE_MATCH(ManagedCharExpression)
				{
				}

				ALGORITHM_PROCEDURE_MATCH(ManagedStringExpression)
				{
				}

				ALGORITHM_PROCEDURE_MATCH(ManagedReferenceExpression)
				{
				}

				ALGORITHM_PROCEDURE_MATCH(ManagedMemberExpression)
				{
				}

				ALGORITHM_PROCEDURE_MATCH(ManagedInstanciatedExpression)
				{
				}

				ALGORITHM_PROCEDURE_MATCH(ManagedInvokeExpression)
				{
				}

				ALGORITHM_PROCEDURE_MATCH(ManagedNewExpression)
				{
				}

				ALGORITHM_PROCEDURE_MATCH(ManagedFunctionResultExpression)
				{
				}

				ALGORITHM_PROCEDURE_MATCH(ManagedCastingExpression)
				{
				}

				ALGORITHM_PROCEDURE_MATCH(ManagedThisExpression)
				{
				}

				ALGORITHM_PROCEDURE_MATCH(ManagedBaseExpression)
				{
				}

				ALGORITHM_PROCEDURE_MATCH(ManagedAssignmentExpression)
				{
				}

				ALGORITHM_PROCEDURE_MATCH(ManagedExtendedExpression)
				{
					ManagedX_GenerateCode_ExtendedExpression(node, argument);
				}

			END_ALGORITHM_PROCEDURE(ManagedX_GenerateCode_Expression)

/***********************************************************************
Extended Expressions
***********************************************************************/

			BEGIN_ALGORITHM_PROCEDURE(ManagedX_GenerateCode_ExtendedExpression, ManagedExtendedExpression, MXCGP)

				ALGORITHM_PROCEDURE_MATCH(ManagedLambdaExpression)
				{
				}

				ALGORITHM_PROCEDURE_MATCH(ManagedChoiceExpression)
				{
				}

				ALGORITHM_PROCEDURE_MATCH(ManagedNullChoiceExpression)
				{
				}

				ALGORITHM_PROCEDURE_MATCH(ManagedTypeofExpression)
				{
				}

			END_ALGORITHM_PROCEDURE(ManagedX_GenerateCode_ExtendedExpression)

/***********************************************************************
Basic Statements
***********************************************************************/

			BEGIN_ALGORITHM_PROCEDURE(ManagedX_GenerateCode_Statement, ManagedStatement, MXCGP)

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

				ALGORITHM_PROCEDURE_MATCH(ManagedForStatement)
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
					ManagedX_GenerateCode_ExtendedStatement(node, argument);
				}

			END_ALGORITHM_PROCEDURE(ManagedX_GenerateCode_Statement)

/***********************************************************************
Extended Statements
***********************************************************************/

			BEGIN_ALGORITHM_PROCEDURE(ManagedX_GenerateCode_ExtendedStatement, ManagedExtendedStatement, MXCGP)

				ALGORITHM_PROCEDURE_MATCH(ManagedUsingStatement)
				{
				}

				ALGORITHM_PROCEDURE_MATCH(ManagedLockStatement)
				{
				}

				ALGORITHM_PROCEDURE_MATCH(ManagedSelectStatement)
				{
				}

			END_ALGORITHM_PROCEDURE(ManagedX_GenerateCode_ExtendedStatement)

/***********************************************************************
Basic Members
***********************************************************************/

			BEGIN_ALGORITHM_PROCEDURE(ManagedX_GenerateCode_Member, ManagedMember, MXCGP)

				ALGORITHM_PROCEDURE_MATCH(ManagedField)
				{
				}

				ALGORITHM_PROCEDURE_MATCH(ManagedMethod)
				{
				}

				ALGORITHM_PROCEDURE_MATCH(ManagedConstructor)
				{
				}

				ALGORITHM_PROCEDURE_MATCH(ManagedExtendedMember)
				{
					ManagedX_GenerateCode_ExtendedMember(node, argument);
				}

			END_ALGORITHM_PROCEDURE(ManagedX_GenerateCode_Member)

/***********************************************************************
Extended Members
***********************************************************************/

			BEGIN_ALGORITHM_PROCEDURE(ManagedX_GenerateCode_ExtendedMember, ManagedExtendedMember, MXCGP)

				ALGORITHM_PROCEDURE_MATCH(ManagedProperty)
				{
				}

				ALGORITHM_PROCEDURE_MATCH(ManagedConverterOperator)
				{
				}

				ALGORITHM_PROCEDURE_MATCH(ManagedConverterConstructor)
				{
				}

			END_ALGORITHM_PROCEDURE(ManagedX_GenerateCode_ExtendedMember)

/***********************************************************************
Basic Declaration
***********************************************************************/

			BEGIN_ALGORITHM_PROCEDURE(ManagedX_GenerateCode_Declaration, ManagedDeclaration, MXCGP)

				ALGORITHM_PROCEDURE_MATCH(ManagedTypeDeclaration)
				{
				}

				ALGORITHM_PROCEDURE_MATCH(ManagedNamespaceDeclaration)
				{
				}

				ALGORITHM_PROCEDURE_MATCH(ManagedExtendedDeclaration)
				{
					ManagedX_GenerateCode_ExtendedDeclaration(node, argument);
				}

			END_ALGORITHM_PROCEDURE(ManagedX_GenerateCode_Declaration)

/***********************************************************************
Extended Declaration
***********************************************************************/

			BEGIN_ALGORITHM_PROCEDURE(ManagedX_GenerateCode_ExtendedDeclaration, ManagedExtendedDeclaration, MXCGP)

				ALGORITHM_PROCEDURE_MATCH(ManagedEnumerationDeclaration)
				{
				}

				ALGORITHM_PROCEDURE_MATCH(ManagedTypeRenameDeclaration)
				{
				}

				ALGORITHM_PROCEDURE_MATCH(ManagedUsingNamespaceDeclaration)
				{
				}

			END_ALGORITHM_PROCEDURE(ManagedX_GenerateCode_ExtendedDeclaration)

		}
	}
}