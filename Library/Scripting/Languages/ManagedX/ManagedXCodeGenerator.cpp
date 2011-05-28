#include "ManagedXCodeGenerator.h"

namespace vl
{
	namespace scripting
	{
		namespace language_managedx
		{
			void PrintIndentation(const MXCGP& argument, vint offset=0)
			{
				for(vint i=0;i<argument.indentation+offset;i++)
				{
					argument.writer.WriteString(L"    ");
				}
			}

/***********************************************************************
Basic Types
***********************************************************************/

			BEGIN_ALGORITHM_PROCEDURE(ManagedX_GenerateCode_Type, ManagedType, MXCGP)

				ALGORITHM_PROCEDURE_MATCH(ManagedReferencedType)
				{
					argument.writer.WriteString(node->name);
				}

				ALGORITHM_PROCEDURE_MATCH(ManagedMemberType)
				{
					ManagedX_GenerateCode_Type(node->operand, argument);
					argument.writer.WriteString(L".");
					argument.writer.WriteString(node->member);
				}

				ALGORITHM_PROCEDURE_MATCH(ManagedInstantiatedGenericType)
				{
					ManagedX_GenerateCode_Type(node->elementType, argument);
					argument.writer.WriteString(L"<");
					for(vint i=0;i<node->argumentTypes.Count();i++)
					{
						if(i) argument.writer.WriteString(L", ");
						ManagedX_GenerateCode_Type(node->argumentTypes[i], argument);
					}
					argument.writer.WriteString(L">");
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
					ManagedX_GenerateCode_Type(node->elementType, argument);
					argument.writer.WriteString(L"[");
					for(vint i=1;i<node->dimensionCount;i++)
					{
						argument.writer.WriteString(L",");
					}
					argument.writer.WriteString(L"]");
				}

				ALGORITHM_PROCEDURE_MATCH(ManagedFunctionType)
				{
					argument.writer.WriteString(L"function ");
					ManagedX_GenerateCode_Type(node->returnType, argument);
					argument.writer.WriteString(L"(");
					for(vint i=0;i<node->parameterTypes.Count();i++)
					{
						if(i) argument.writer.WriteString(L", ");
						ManagedX_GenerateCode_Type(node->parameterTypes[i], argument);
					}
					argument.writer.WriteString(L")");
				}

				ALGORITHM_PROCEDURE_MATCH(ManagedEventType)
				{
					argument.writer.WriteString(L"event ");
					ManagedX_GenerateCode_Type(node->functionType->returnType, argument);
					argument.writer.WriteString(L"(");
					for(vint i=0;i<node->functionType->parameterTypes.Count();i++)
					{
						if(i) argument.writer.WriteString(L", ");
						ManagedX_GenerateCode_Type(node->functionType->parameterTypes[i], argument);
					}
					argument.writer.WriteString(L")");
				}

				ALGORITHM_PROCEDURE_MATCH(ManagedAutoReferType)
				{
					argument.writer.WriteString(L"var");
				}

				ALGORITHM_PROCEDURE_MATCH(ManagedDynamicType)
				{
					argument.writer.WriteString(L"dynamic");
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

				ALGORITHM_PROCEDURE_MATCH(ManagedNewObjectExpression)
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

				ALGORITHM_PROCEDURE_MATCH(ManagedUnaryExpression)
				{
				}

				ALGORITHM_PROCEDURE_MATCH(ManagedBinaryExpression)
				{
				}

				ALGORITHM_PROCEDURE_MATCH(ManagedNewArrayExpression)
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