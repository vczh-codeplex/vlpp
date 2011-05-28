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

			void UnescapeStringContent(const WString& string, TextWriter& writer)
			{
				const wchar_t* buffer=string.Buffer();
				while(*buffer)
				{
					switch(*buffer)
					{
					case L'\r':
						writer.WriteString(L"\\r");
						break;
					case L'\n':
						writer.WriteString(L"\\n");
						break;
					case L'\t':
						writer.WriteString(L"\\t");
						break;
					case L'\"':
						writer.WriteString(L"\\\"");
						break;
					case L'\\':
						writer.WriteString(L"\\\\");
						break;
					default:
						writer.WriteChar(*buffer);
					}
					buffer++;
				}
			}

			void IdentifierToString(const WString& identifier, TextWriter& writer)
			{
				writer.WriteString(identifier);
			}

/***********************************************************************
Basic Types
***********************************************************************/

			BEGIN_ALGORITHM_PROCEDURE(ManagedX_GenerateCode_Type, ManagedType, MXCGP)

				ALGORITHM_PROCEDURE_MATCH(ManagedReferencedType)
				{
					IdentifierToString(node->name, argument.writer);
				}

				ALGORITHM_PROCEDURE_MATCH(ManagedMemberType)
				{
					ManagedX_GenerateCode_Type(node->operand, argument);
					argument.writer.WriteString(L".");
					IdentifierToString(node->member, argument.writer);
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
					argument.writer.WriteString(L"null");
				}

				ALGORITHM_PROCEDURE_MATCH(ManagedIntegerExpression)
				{
					if(node->sign)
					{
						argument.writer.WriteString(i64tow(node->value.signedInteger));
					}
					else
					{
						argument.writer.WriteString(i64tow(node->value.unsignedInteger));
					}
				}

				ALGORITHM_PROCEDURE_MATCH(ManagedFloatExpression)
				{
					argument.writer.WriteString(ftow(node->value));
				}

				ALGORITHM_PROCEDURE_MATCH(ManagedBooleanExpression)
				{
					argument.writer.WriteString(node->value?L"true":L"false");
				}

				ALGORITHM_PROCEDURE_MATCH(ManagedCharExpression)
				{
					argument.writer.WriteString(L"\'");
					UnescapeStringContent(node->value, argument.writer);
					argument.writer.WriteString(L"\'");
				}

				ALGORITHM_PROCEDURE_MATCH(ManagedStringExpression)
				{
					argument.writer.WriteString(L"\"");
					UnescapeStringContent(node->value, argument.writer);
					argument.writer.WriteString(L"\"");
				}

				ALGORITHM_PROCEDURE_MATCH(ManagedReferenceExpression)
				{
					IdentifierToString(node->name, argument.writer);
				}

				ALGORITHM_PROCEDURE_MATCH(ManagedMemberExpression)
				{
					ManagedX_GenerateCode_Expression(node->operand, argument);
					argument.writer.WriteString(L".");
					IdentifierToString(node->member, argument.writer);
				}

				ALGORITHM_PROCEDURE_MATCH(ManagedInstanciatedExpression)
				{
					ManagedX_GenerateCode_Expression(node->operand, argument);
					argument.writer.WriteString(L"<");
					for(vint i=0;i<node->argumentTypes.Count();i++)
					{
						if(i) argument.writer.WriteString(L", ");
						ManagedX_GenerateCode_Type(node->argumentTypes[i], argument);
					}
					argument.writer.WriteString(L">");
				}

				ALGORITHM_PROCEDURE_MATCH(ManagedInvokeExpression)
				{
					ManagedX_GenerateCode_Expression(node->function, argument);
					argument.writer.WriteString(L"(");
					for(vint i=0;i<node->arguments.Count();i++)
					{
						if(i) argument.writer.WriteString(L", ");
						ManagedX_GenerateCode_Expression(node->arguments[i], argument);
					}

					for(vint i=0;i<node->defaultParameterNames.Count();i++)
					{
						if(i || node->arguments.Count()) argument.writer.WriteString(L", ");
						IdentifierToString(node->defaultParameterNames[i], argument.writer);
						argument.writer.WriteString(L":");
						ManagedX_GenerateCode_Expression(node->defaultParameterValues[i], argument);
					}
					argument.writer.WriteString(L")");
				}

				ALGORITHM_PROCEDURE_MATCH(ManagedNewObjectExpression)
				{
					argument.writer.WriteString(L"new ");
					ManagedX_GenerateCode_Type(node->objectType, argument);
					argument.writer.WriteString(L"(");
					for(vint i=0;i<node->arguments.Count();i++)
					{
						if(i) argument.writer.WriteString(L", ");
						ManagedX_GenerateCode_Expression(node->arguments[i], argument);
					}

					for(vint i=0;i<node->defaultParameterNames.Count();i++)
					{
						if(i || node->arguments.Count()) argument.writer.WriteString(L", ");
						IdentifierToString(node->defaultParameterNames[i], argument.writer);
						argument.writer.WriteString(L":");
						ManagedX_GenerateCode_Expression(node->defaultParameterValues[i], argument);
					}
					argument.writer.WriteString(L")");

					if(node->propertyNames.Count()>0)
					{
						argument.writer.WriteString(L"\r\n");
						PrintIndentation(argument);
						argument.writer.WriteString(L"{\r\n");
						for(int i=0;i<node->propertyNames.Count();i++)
						{
							if(i) argument.writer.WriteString(L",\r\n");
							PrintIndentation(argument, 1);
							IdentifierToString(node->propertyNames[i], argument.writer);
							argument.writer.WriteString(L" = ");
							ManagedX_GenerateCode_Expression(node->propertyValues[i], argument);
						}
						argument.writer.WriteString(L"\r\n");
						PrintIndentation(argument);
						argument.writer.WriteString(L"}");
					}
				}

				ALGORITHM_PROCEDURE_MATCH(ManagedFunctionResultExpression)
				{
					argument.writer.WriteString(L"result");
				}

				ALGORITHM_PROCEDURE_MATCH(ManagedCastingExpression)
				{
					argument.writer.WriteString(L"(");
					ManagedX_GenerateCode_Expression(node->operand, argument);
					argument.writer.WriteString(L" as ");
					ManagedX_GenerateCode_Type(node->type, argument);
					argument.writer.WriteString(L")");
				}

				ALGORITHM_PROCEDURE_MATCH(ManagedThisExpression)
				{
					argument.writer.WriteString(L"this");
				}

				ALGORITHM_PROCEDURE_MATCH(ManagedBaseExpression)
				{
					argument.writer.WriteString(L"type");
				}

				ALGORITHM_PROCEDURE_MATCH(ManagedAssignmentExpression)
				{
					argument.writer.WriteString(L"(");
					ManagedX_GenerateCode_Expression(node->leftOperand, argument);
					argument.writer.WriteString(L" = ");
					ManagedX_GenerateCode_Expression(node->rightOperand, argument);
					argument.writer.WriteString(L")");
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
					argument.writer.WriteString(L"delegate ");
					if(!node->returnType.Cast<ManagedAutoReferType>())
					{
						ManagedX_GenerateCode_Type(node->returnType, argument);
					}
					argument.writer.WriteString(L"(");
					for(vint i=0;i<node->parameterNames.Count();i++)
					{
						if(i) argument.writer.WriteString(L", ");
						if(!node->parameterTypes[i].Cast<ManagedAutoReferType>())
						{
							ManagedX_GenerateCode_Type(node->parameterTypes[i], argument);
						}
						IdentifierToString(node->parameterNames[i], argument.writer);
					}
					argument.writer.WriteString(L")");
					ManagedX_GenerateCode_Statement(node->body, argument);
				}

				ALGORITHM_PROCEDURE_MATCH(ManagedChoiceExpression)
				{
					argument.writer.WriteString(L"(");
					ManagedX_GenerateCode_Expression(node->condition, argument);
					argument.writer.WriteString(L" ? ");
					ManagedX_GenerateCode_Expression(node->trueExpression, argument);
					argument.writer.WriteString(L" : ");
					ManagedX_GenerateCode_Expression(node->falseExpression, argument);
					argument.writer.WriteString(L")");
				}

				ALGORITHM_PROCEDURE_MATCH(ManagedNullChoiceExpression)
				{
					argument.writer.WriteString(L"(");
					ManagedX_GenerateCode_Expression(node->valueExpression, argument);
					argument.writer.WriteString(L" ?? ");
					ManagedX_GenerateCode_Expression(node->candidateExpression, argument);
					argument.writer.WriteString(L" : ");
					argument.writer.WriteString(L")");
				}

				ALGORITHM_PROCEDURE_MATCH(ManagedTypeofExpression)
				{
					argument.writer.WriteString(L"typeof(");
					ManagedX_GenerateCode_Type(node->type, argument);
					argument.writer.WriteString(L")");
				}

				ALGORITHM_PROCEDURE_MATCH(ManagedUnaryExpression)
				{
				}

				ALGORITHM_PROCEDURE_MATCH(ManagedBinaryExpression)
				{
				}

				ALGORITHM_PROCEDURE_MATCH(ManagedNewArrayExpression)
				{
					ManagedX_GenerateCode_Type(node->objectType, argument);
					argument.writer.WriteString(L"[");
					for(vint i=0;i<node->sizes.Count();i++)
					{
						if(i) argument.writer.WriteString(L", ");
						ManagedX_GenerateCode_Expression(node->sizes[i], argument);
					}
					argument.writer.WriteString(L"]");
				}

				ALGORITHM_PROCEDURE_MATCH(ManagedIsTypeExpression)
				{
					argument.writer.WriteString(L"(");
					ManagedX_GenerateCode_Expression(node->operand, argument);
					argument.writer.WriteString(L" is ");
					ManagedX_GenerateCode_Type(node->type, argument);
					argument.writer.WriteString(L")");
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