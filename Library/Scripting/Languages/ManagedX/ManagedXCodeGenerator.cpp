#include "ManagedXCodeGenerator.h"

namespace vl
{
	namespace scripting
	{
		namespace language_managedx
		{
			using namespace collections;

			void ManagedX_GenerateCode_Program(Ptr<ManagedProgram> program, TextWriter& writer)
			{
				MXCGP argument(writer, 0);
				for(vint i=0;i<program->declarations.Count();i++)
				{
					ManagedX_GenerateCode_Declaration(program->declarations[i], argument);
				}
			}

/***********************************************************************
��������
***********************************************************************/

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
				static wchar_t* keywords[]=
				{
					L"void",
					L"sbyte",
					L"byte",
					L"short",
					L"word",
					L"int",
					L"uint",
					L"long",
					L"ulong",
					L"bool",
					L"char",
					L"string",
					L"float",
					L"double",
					L"object",
					L"intptr",
					L"uintptr",
					L"operator",
					L"prefix",
					L"postfix",
					L"ref",
					L"out",
					L"public",
					L"protected",
					L"private",
					L"internal",
					L"sealed",
					L"abstract",
					L"virtual",
					L"override",
					L"static",
					L"const",
					L"readonly",
					L"generic",
					L"inout",
					L"in",
					L"out",
					L"new",
					L"params",
					L"global",
					L"function",
					L"event",
					L"var",
					L"dynamic",
					L"null",
					L"true",
					L"false",
					L"result",
					L"as",
					L"this",
					L"base",
					L"typeof",
					L"is",
					L"const",
					L"if",
					L"else",
					L"while",
					L"when",
					L"do",
					L"loop",
					L"for",
					L"foreach",
					L"break",
					L"continue",
					L"exit",
					L"return",
					L"try",
					L"catch",
					L"finally",
					L"throw",
					L"using",
					L"lock",
					L"switch",
					L"case",
					L"default",
					L"implicit",
					L"explicit",
					L"get",
					L"set",
					L"constructor",
					L"destructor",
					L"external",
					L"class",
					L"struct",
					L"interface",
					L"namespace",
					L"enum",
				};

				const wchar_t* buffer=identifier.Buffer();
				bool isKeyword=false;
				for(vint i=0;i<sizeof(keywords)/sizeof(*keywords);i++)
				{
					if(wcscmp(keywords[i], buffer)==0)
					{
						isKeyword=true;
						break;
					}
				}
				if(isKeyword)
				{
					writer.WriteString(L"@");
				}
				writer.WriteString(identifier);
			}

			void ArgumentToString(Ptr<ManagedArgument> node, const MXCGP& argument)
			{
				if(node->defaultParameterName!=L"")
				{
					IdentifierToString(node->defaultParameterName, argument.writer);
					argument.writer.WriteString(L":");
				}
				switch(node->argumentType)
				{
				case ManagedArgument::Ref:
					argument.writer.WriteString(L"ref ");
					break;
				case ManagedArgument::Out:
					argument.writer.WriteString(L"out ");
				}
				ManagedX_GenerateCode_Expression(node->value, argument);
			}

			void AttributeToString(declatt::Accessor att, TextWriter& writer)
			{
				switch(att)
				{
				case declatt::Public:
					writer.WriteString(L"public ");
					break;
				case declatt::Protected:
					writer.WriteString(L"protected ");
					break;
				case declatt::Private:
					writer.WriteString(L"private ");
					break;
				case declatt::ProtectedInternal:
					writer.WriteString(L"protected internal ");
					break;
				case declatt::Internal:
					writer.WriteString(L"internal ");
					break;
				}
			}

			void AttributeToString(declatt::Inheritation att, TextWriter& writer)
			{
				switch(att)
				{
				case declatt::Normal:
					break;
				case declatt::Sealed:
					writer.WriteString(L"sealed ");
					break;
				case declatt::Abstract:
					writer.WriteString(L"abstract ");
					break;
				case declatt::Virtual:
					writer.WriteString(L"virtual ");
					break;
				case declatt::Override:
					writer.WriteString(L"override ");
					break;
				}
			}

			void AttributeToString(declatt::MemberType att, TextWriter& writer)
			{
				switch(att)
				{
				case declatt::Instance:
					break;
				case declatt::Static:
					writer.WriteString(L"static ");
					break;
				}
			}

			void AttributeToString(declatt::DataType att, TextWriter& writer)
			{
				switch(att)
				{
				case declatt::Variable:
					break;
				case declatt::Constant:
					writer.WriteString(L"const ");
					break;
				case declatt::Readonly:
					writer.WriteString(L"readonly ");
					break;
				}
			}

			void InfoToString(ManagedAttributeInfo& info, const MXCGP& argument, bool newLine=true)
			{
				for(vint i=0;i<info.attributes.Count();i++)
				{
					Ptr<ManagedNewObjectExpression> att=info.attributes[i];

					if(newLine)
					{
						PrintIndentation(argument);
					}
					argument.writer.WriteString(L"[");

					ManagedX_GenerateCode_Type(att->objectType, argument);
					argument.writer.WriteString(L"(");
					for(vint j=0;j<att->arguments.Count();j++)
					{
						if(j) argument.writer.WriteString(L", ");
						ArgumentToString(att->arguments[j], argument);
					}

					for(vint j=0;j>att->properties.Count();j++)
					{
						if(j || att->arguments.Count()) argument.writer.WriteString(L", ");
						IdentifierToString(att->properties[j]->propertyName, argument.writer);
						argument.writer.WriteString(L" = ");
						ManagedX_GenerateCode_Expression(att->properties[j]->value, argument);
					}
					argument.writer.WriteString(L")");

					if(newLine)
					{
						argument.writer.WriteString(L"]\r\n");
					}
					else
					{
						argument.writer.WriteString(L"]");
					}
				}
			}

			void InfoToString(ManagedGenericInfo& info, const MXCGP& argument)
			{
				if(info.HasGeneric())
				{
					PrintIndentation(argument);
					argument.writer.WriteString(L"generic<\r\n");
					for(vint i=0;i<info.arguments.Count();i++)
					{
						Ptr<ManagedGenericInfo::Argument> arg=info.arguments[i];
						if(i) argument.writer.WriteString(L",\r\n");
						PrintIndentation(argument, 1);

						switch(arg->conversion)
						{
						case ManagedGenericInfo::InOut:
							argument.writer.WriteString(L"inout ");
							break;
						case ManagedGenericInfo::In:
							argument.writer.WriteString(L"in ");
							break;
						case ManagedGenericInfo::Out:
							argument.writer.WriteString(L"out ");
							break;
						}
						IdentifierToString(arg->name, argument.writer);

						if(arg->newConstraint || arg->typeConstraints.Count())
						{
							argument.writer.WriteString(L" : ");
							for(vint j=0;j<arg->typeConstraints.Count();j++)
							{
								if(j) argument.writer.WriteString(L", ");
								ManagedX_GenerateCode_Type(arg->typeConstraints[j], argument);
							}

							if(arg->newConstraint)
							{
								if(arg->typeConstraints.Count())
								{
									argument.writer.WriteString(L", ");
								}
								argument.writer.WriteString(L"new()");
							}
						}
					}
					argument.writer.WriteString(L"\r\n");
					PrintIndentation(argument, 1);
					argument.writer.WriteString(L">\r\n");
				}
			}

			void ParametersToString(List<Ptr<ManagedParameter>>& parameters, const MXCGP& argument)
			{
				argument.writer.WriteString(L"(");
				for(vint i=0;i<parameters.Count();i++)
				{
					Ptr<ManagedParameter> p=parameters[i];
					if(i) argument.writer.WriteString(L", ");

					InfoToString(parameters[i]->attributeInfo, argument, false);
					switch(p->parameterType)
					{
					case ManagedParameter::Params:
						argument.writer.WriteString(L"params ");
						break;
					case ManagedParameter::Ref:
						argument.writer.WriteString(L"ref ");
						break;
					case ManagedParameter::Out:
						argument.writer.WriteString(L"out ");
						break;
					case ManagedParameter::This:
						argument.writer.WriteString(L"this ");
						break;
					}
					ManagedX_GenerateCode_Type(p->type, argument);
					argument.writer.WriteString(L" ");
					IdentifierToString(p->name, argument.writer);
					if(p->defaultValue)
					{
						argument.writer.WriteString(L" = ");
						ManagedX_GenerateCode_Expression(p->defaultValue, argument);
					}
				}
				argument.writer.WriteString(L")");
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
					if(node->operand)
					{
						ManagedX_GenerateCode_Type(node->operand, argument);
						argument.writer.WriteString(L".");
					}
					else
					{
						argument.writer.WriteString(L"global::");
					}
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
					argument.writer.WriteString(L"event (");
					for(vint i=0;i<node->parameterTypes.Count();i++)
					{
						if(i) argument.writer.WriteString(L", ");
						ManagedX_GenerateCode_Type(node->parameterTypes[i], argument);
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
					if(node->operand)
					{
						ManagedX_GenerateCode_Expression(node->operand, argument);
						argument.writer.WriteString(L".");
					}
					else
					{
						argument.writer.WriteString(L"global::");
					}
					IdentifierToString(node->member, argument.writer);
				}

				ALGORITHM_PROCEDURE_MATCH(ManagedInstantiatedExpression)
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
						ArgumentToString(node->arguments[i], argument);
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
						ArgumentToString(node->arguments[i], argument);
					}
					argument.writer.WriteString(L")");

					if(node->properties.Count()>0)
					{
						argument.writer.WriteString(L"\r\n");
						PrintIndentation(argument);
						argument.writer.WriteString(L"{\r\n");
						for(vint i=0;i<node->properties.Count();i++)
						{
							if(i) argument.writer.WriteString(L",\r\n");
							PrintIndentation(argument, 1);
							IdentifierToString(node->properties[i]->propertyName, argument.writer);
							argument.writer.WriteString(L" = ");
							ManagedX_GenerateCode_Expression(node->properties[i]->value, argument);
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

				ALGORITHM_PROCEDURE_MATCH(ManagedThisExpression)
				{
					argument.writer.WriteString(L"this");
				}

				ALGORITHM_PROCEDURE_MATCH(ManagedBaseExpression)
				{
					argument.writer.WriteString(L"base");
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
					argument.writer.WriteString(L"function ");
					if(!node->returnType.Cast<ManagedAutoReferType>())
					{
						ManagedX_GenerateCode_Type(node->returnType, argument);
					}
					argument.writer.WriteString(L"(");
					for(vint i=0;i<node->parameters.Count();i++)
					{
						if(i) argument.writer.WriteString(L", ");
						if(!node->parameters[i]->type.Cast<ManagedAutoReferType>())
						{
							ManagedX_GenerateCode_Type(node->parameters[i]->type, argument);
							argument.writer.WriteString(L" ");
						}
						IdentifierToString(node->parameters[i]->name, argument.writer);
					}
					argument.writer.WriteString(L")\r\n");
					MXCGP newArgument(argument.writer, argument.indentation+1);
					ManagedX_GenerateCode_Statement(node->body, newArgument);
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
					argument.writer.WriteString(L")");
				}

				ALGORITHM_PROCEDURE_MATCH(ManagedTypeofExpression)
				{
					argument.writer.WriteString(L"typeof(");
					ManagedX_GenerateCode_Type(node->type, argument);
					argument.writer.WriteString(L")");
				}

				ALGORITHM_PROCEDURE_MATCH(ManagedDefaultExpression)
				{
					argument.writer.WriteString(L"default(");
					ManagedX_GenerateCode_Type(node->type, argument);
					argument.writer.WriteString(L")");
				}

				ALGORITHM_PROCEDURE_MATCH(ManagedUnaryExpression)
				{
					argument.writer.WriteString(L"(");
					if(node->operatorName==L"op_pos")
					{
						argument.writer.WriteString(L"+ ");
					}
					else if(node->operatorName==L"op_neg")
					{
						argument.writer.WriteString(L"- ");
					}
					else if(node->operatorName==L"op_not")
					{
						argument.writer.WriteString(L"! ");
					}
					else if(node->operatorName==L"op_bitnot")
					{
						argument.writer.WriteString(L"~ ");
					}
					else if(node->operatorName==L"op_preinc")
					{
						argument.writer.WriteString(L"++ ");
					}
					else if(node->operatorName==L"op_predec")
					{
						argument.writer.WriteString(L"-- ");
					}
					else if(node->operatorName!=L"op_postinc" && node->operatorName!=L"op_postdec")
					{
						argument.writer.WriteString(L"<UNKNOWN-OPERATOR-");
						IdentifierToString(node->operatorName, argument.writer);
						argument.writer.WriteString(L">");
					}
					ManagedX_GenerateCode_Expression(node->operand, argument);
					if(node->operatorName==L"op_postinc")
					{
						argument.writer.WriteString(L" ++");
					}
					else if(node->operatorName==L"op_postdec")
					{
						argument.writer.WriteString(L" --");
					}
					argument.writer.WriteString(L")");
				}

				ALGORITHM_PROCEDURE_MATCH(ManagedBinaryExpression)
				{
					argument.writer.WriteString(L"(");
					ManagedX_GenerateCode_Expression(node->leftOperand, argument);
					if(node->operatorName==L"op_add")
					{
						argument.writer.WriteString(L" + ");
					}
					else if(node->operatorName==L"op_sub")
					{
						argument.writer.WriteString(L" - ");
					}
					else if(node->operatorName==L"op_mul")
					{
						argument.writer.WriteString(L" * ");
					}
					else if(node->operatorName==L"op_div")
					{
						argument.writer.WriteString(L" / ");
					}
					else if(node->operatorName==L"op_mod")
					{
						argument.writer.WriteString(L" % ");
					}
					else if(node->operatorName==L"op_lt")
					{
						argument.writer.WriteString(L" < ");
					}
					else if(node->operatorName==L"op_le")
					{
						argument.writer.WriteString(L" <= ");
					}
					else if(node->operatorName==L"op_gt")
					{
						argument.writer.WriteString(L" > ");
					}
					else if(node->operatorName==L"op_ge")
					{
						argument.writer.WriteString(L" >= ");
					}
					else if(node->operatorName==L"op_eq")
					{
						argument.writer.WriteString(L" == ");
					}
					else if(node->operatorName==L"op_ne")
					{
						argument.writer.WriteString(L" != ");
					}
					else if(node->operatorName==L"op_and")
					{
						argument.writer.WriteString(L" && ");
					}
					else if(node->operatorName==L"op_bitand")
					{
						argument.writer.WriteString(L" & ");
					}
					else if(node->operatorName==L"op_or")
					{
						argument.writer.WriteString(L" || ");
					}
					else if(node->operatorName==L"op_bitor")
					{
						argument.writer.WriteString(L" | ");
					}
					else if(node->operatorName==L"op_xor")
					{
						argument.writer.WriteString(L" ^ ");
					}
					else if(node->operatorName==L"op_shl")
					{
						argument.writer.WriteString(L" << ");
					}
					else if(node->operatorName==L"op_shr")
					{
						argument.writer.WriteString(L" >> ");
					}
					else if(node->operatorName==L"op_add_eq")
					{
						argument.writer.WriteString(L" += ");
					}
					else if(node->operatorName==L"op_sub_eq")
					{
						argument.writer.WriteString(L" -= ");
					}
					else if(node->operatorName==L"op_mul_eq")
					{
						argument.writer.WriteString(L" *= ");
					}
					else if(node->operatorName==L"op_div_eq")
					{
						argument.writer.WriteString(L" /= ");
					}
					else if(node->operatorName==L"op_mod_eq")
					{
						argument.writer.WriteString(L" %= ");
					}
					else if(node->operatorName==L"op_and_eq")
					{
						argument.writer.WriteString(L" &&= ");
					}
					else if(node->operatorName==L"op_bitand_eq")
					{
						argument.writer.WriteString(L" &= ");
					}
					else if(node->operatorName==L"op_or_eq")
					{
						argument.writer.WriteString(L" ||= ");
					}
					else if(node->operatorName==L"op_bitor_eq")
					{
						argument.writer.WriteString(L" |= ");
					}
					else if(node->operatorName==L"op_xor_eq")
					{
						argument.writer.WriteString(L" ^= ");
					}
					else if(node->operatorName==L"op_shl_eq")
					{
						argument.writer.WriteString(L" <<= ");
					}
					else if(node->operatorName==L"op_shr_eq")
					{
						argument.writer.WriteString(L" >>= ");
					}
					else
					{
						argument.writer.WriteString(L"<UNKNOWN-OPERATOR-");
						IdentifierToString(node->operatorName, argument.writer);
						argument.writer.WriteString(L">");
					}
					ManagedX_GenerateCode_Expression(node->rightOperand, argument);
					argument.writer.WriteString(L")");
				}

				ALGORITHM_PROCEDURE_MATCH(ManagedNewArrayExpression)
				{
					argument.writer.WriteString(L"new ");
					ManagedX_GenerateCode_Type(node->objectType, argument);
					argument.writer.WriteString(L"[");
					for(vint i=0;i<node->sizes.Count();i++)
					{
						if(i) argument.writer.WriteString(L", ");
						ManagedX_GenerateCode_Expression(node->sizes[i], argument);
					}
					argument.writer.WriteString(L"]");
				}

				ALGORITHM_PROCEDURE_MATCH(ManagedCastingExpression)
				{
					argument.writer.WriteString(L"(");
					ManagedX_GenerateCode_Expression(node->operand, argument);
					argument.writer.WriteString(L" as ");
					ManagedX_GenerateCode_Type(node->type, argument);
					argument.writer.WriteString(L")");
				}

				ALGORITHM_PROCEDURE_MATCH(ManagedIsTypeExpression)
				{
					argument.writer.WriteString(L"(");
					ManagedX_GenerateCode_Expression(node->operand, argument);
					argument.writer.WriteString(L" is ");
					ManagedX_GenerateCode_Type(node->type, argument);
					argument.writer.WriteString(L")");
				}

				ALGORITHM_PROCEDURE_MATCH(ManagedIndexExpression)
				{
					ManagedX_GenerateCode_Expression(node->operand, argument);
					argument.writer.WriteString(L"[");
					for(vint i=0;i<node->indices.Count();i++)
					{
						if(i) argument.writer.WriteString(L", ");
						ManagedX_GenerateCode_Expression(node->indices[i], argument);
					}
					argument.writer.WriteString(L"]");
				}

			END_ALGORITHM_PROCEDURE(ManagedX_GenerateCode_ExtendedExpression)

/***********************************************************************
Basic Statements
***********************************************************************/

			BEGIN_ALGORITHM_PROCEDURE(ManagedX_GenerateCode_Statement, ManagedStatement, MXCGP)

				ALGORITHM_PROCEDURE_MATCH(ManagedEmptyStatement)
				{
					PrintIndentation(argument);
					argument.writer.WriteString(L";");
				}

				ALGORITHM_PROCEDURE_MATCH(ManagedCompositeStatement)
				{
					PrintIndentation(argument, -1);
					argument.writer.WriteString(L"{\r\n");
					for(vint i=0;i<node->statements.Count();i++)
					{
						if(node->statements[i].Cast<ManagedCompositeStatement>())
						{
							MXCGP newArgument(argument.writer, argument.indentation+1);
							ManagedX_GenerateCode_Statement(node->statements[i], newArgument);
						}
						else
						{
							ManagedX_GenerateCode_Statement(node->statements[i], argument);
						}
						argument.writer.WriteString(L"\r\n");
					}
					PrintIndentation(argument, -1);
					argument.writer.WriteString(L"}");
				}

				ALGORITHM_PROCEDURE_MATCH(ManagedExpressionStatement)
				{
					PrintIndentation(argument);
					ManagedX_GenerateCode_Expression(node->expression, argument);
					argument.writer.WriteString(L";");
				}

				ALGORITHM_PROCEDURE_MATCH(ManagedVariableStatement)
				{
					PrintIndentation(argument);
					if(node->constant)
					{
						argument.writer.WriteString(L"const ");
					}
					ManagedX_GenerateCode_Type(node->type, argument);
					argument.writer.WriteString(L" ");
					IdentifierToString(node->name, argument.writer);
					if(node->initializer)
					{
						argument.writer.WriteString(L" = ");
						ManagedX_GenerateCode_Expression(node->initializer, argument);
					}
					argument.writer.WriteString(L";");
				}

				ALGORITHM_PROCEDURE_MATCH(ManagedIfStatement)
				{
					MXCGP newArgument(argument.writer, argument.indentation+1);
					PrintIndentation(argument);
					argument.writer.WriteString(L"if(");
					ManagedX_GenerateCode_Expression(node->condition, argument);
					argument.writer.WriteLine(L")");
					ManagedX_GenerateCode_Statement(node->trueStatement, newArgument);
					if(node->falseStatement)
					{
						argument.writer.WriteString(L"\r\n");
						PrintIndentation(argument);
						argument.writer.WriteLine(L"else");
						ManagedX_GenerateCode_Statement(node->falseStatement, newArgument);
					}
				}

				ALGORITHM_PROCEDURE_MATCH(ManagedWhileStatement)
				{
					MXCGP newArgument(argument.writer, argument.indentation+1);
					PrintIndentation(argument);
					if(node->beginCondition)
					{
						argument.writer.WriteString(L"while(");
						ManagedX_GenerateCode_Expression(node->beginCondition, argument);
						argument.writer.WriteLine(L")");
					}
					else if(node->endCondition)
					{
						argument.writer.WriteLine(L"do");
					}
					else
					{
						argument.writer.WriteLine(L"while(true)");
					}
					ManagedX_GenerateCode_Statement(node->statement, newArgument);
					if(node->endCondition)
					{
						argument.writer.WriteString(L"\r\n");
						PrintIndentation(argument);
						if(node->beginCondition)
						{
							argument.writer.WriteString(L"when(");
						}
						else
						{
							argument.writer.WriteString(L"while(");
						}
						ManagedX_GenerateCode_Expression(node->endCondition, argument);
						argument.writer.WriteString(L");");
					}
				}

				ALGORITHM_PROCEDURE_MATCH(ManagedBreakStatement)
				{
					PrintIndentation(argument);
					argument.writer.WriteString(L"break;");
				}

				ALGORITHM_PROCEDURE_MATCH(ManagedContinueStatement)
				{
					PrintIndentation(argument);
					argument.writer.WriteString(L"continue;");
				}

				ALGORITHM_PROCEDURE_MATCH(ManagedReturnStatement)
				{
					PrintIndentation(argument);
					argument.writer.WriteString(L"exit;");
				}

				ALGORITHM_PROCEDURE_MATCH(ManagedTryCatchStatement)
				{
					MXCGP newArgument(argument.writer, argument.indentation+1);
					PrintIndentation(argument);
					argument.writer.WriteString(L"try\r\n");
					ManagedX_GenerateCode_Statement(node->tryStatement, newArgument);
					for(vint i=0;i<node->catches.Count();i++)
					{
						argument.writer.WriteString(L"\r\n");
						PrintIndentation(argument);
						argument.writer.WriteString(L"catch(");
						ManagedX_GenerateCode_Type(node->catches[i]->exceptionType, argument);
						if(node->catches[i]->exceptionName!=L"")
						{
							argument.writer.WriteString(L" ");
							IdentifierToString(node->catches[i]->exceptionName, argument.writer);
						}
						argument.writer.WriteString(L")\r\n");
						ManagedX_GenerateCode_Statement(node->catches[i]->exceptionHandler, newArgument);
					}
					if(node->finallyStatement)
					{
						argument.writer.WriteString(L"\r\n");
						PrintIndentation(argument);
						argument.writer.WriteString(L"finally\r\n");
						ManagedX_GenerateCode_Statement(node->finallyStatement, newArgument);
					}
				}

				ALGORITHM_PROCEDURE_MATCH(ManagedThrowStatement)
				{
					PrintIndentation(argument);
					if(node->expression)
					{
						argument.writer.WriteString(L"throw ");
						ManagedX_GenerateCode_Expression(node->expression, argument);
						argument.writer.WriteString(L";");
					}
					else
					{
						argument.writer.WriteString(L"throw;");
					}
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
					PrintIndentation(argument);
					argument.writer.WriteString(L"using(");
					ManagedX_GenerateCode_Type(node->type, argument);
					argument.writer.WriteString(L" ");
					IdentifierToString(node->name, argument.writer);
					argument.writer.WriteString(L" = ");
					ManagedX_GenerateCode_Expression(node->initialize, argument);
					argument.writer.WriteString(L")\r\n");
					MXCGP newArgument(argument.writer, argument.indentation+1);
					ManagedX_GenerateCode_Statement(node->statement, newArgument);
				}

				ALGORITHM_PROCEDURE_MATCH(ManagedLockStatement)
				{
					PrintIndentation(argument);
					argument.writer.WriteString(L"lock(");
					ManagedX_GenerateCode_Expression(node->lock, argument);
					argument.writer.WriteString(L")\r\n");
					MXCGP newArgument(argument.writer, argument.indentation+1);
					ManagedX_GenerateCode_Statement(node->statement, newArgument);
				}

				ALGORITHM_PROCEDURE_MATCH(ManagedSelectStatement)
				{
					PrintIndentation(argument);
					argument.writer.WriteString(L"switch(");
					ManagedX_GenerateCode_Expression(node->expression, argument);
					argument.writer.WriteLine(L")");
					PrintIndentation(argument);
					argument.writer.WriteLine(L"{");
					
					MXCGP newArgument(argument.writer, argument.indentation+2);
					for(vint i=0;i<node->cases.Count();i++)
					{
						Ptr<ManagedCaseClause> caseClause=node->cases[i];
						for(vint j=0;j<caseClause->conditions.Count();j++)
						{
							PrintIndentation(argument, 1);
							argument.writer.WriteString(L"case ");
							ManagedX_GenerateCode_Expression(caseClause->conditions[j], argument);
							argument.writer.WriteString(L":\r\n");
						}
						ManagedX_GenerateCode_Statement(caseClause->statement, newArgument);
						argument.writer.WriteLine(L"");
					}
					if(node->defaultStatement)
					{
						PrintIndentation(argument, 1);
						argument.writer.WriteString(L"default:\r\n");
						ManagedX_GenerateCode_Statement(node->defaultStatement, newArgument);
						argument.writer.WriteLine(L"");
					}

					PrintIndentation(argument);
					argument.writer.WriteString(L"}");
				}

				ALGORITHM_PROCEDURE_MATCH(ManagedForStatement)
				{
					PrintIndentation(argument);
					argument.writer.WriteString(L"for(");
					for(vint i=0;i<node->initializers.Count();i++)
					{
						if(i) argument.writer.WriteString(L", ");
						Ptr<ManagedVariableStatement> variable=node->initializers[i];

						ManagedX_GenerateCode_Type(variable->type, argument);
						argument.writer.WriteString(L" ");
						IdentifierToString(variable->name, argument.writer);
						if(variable->initializer)
						{
							argument.writer.WriteString(L" = ");
							ManagedX_GenerateCode_Expression(variable->initializer, argument);
						}
					}
					argument.writer.WriteString(L"; ");
					if(node->condition)
					{
						ManagedX_GenerateCode_Expression(node->condition, argument);
					}
					argument.writer.WriteString(L"; ");
					for(vint i=0;i<node->sideEffects.Count();i++)
					{
						if(i) argument.writer.WriteString(L", ");
						ManagedX_GenerateCode_Expression(node->sideEffects[i], argument);
					}
					argument.writer.WriteString(L")\r\n");
					MXCGP newArgument(argument.writer, argument.indentation+1);
					ManagedX_GenerateCode_Statement(node->statement, newArgument);
				}

				ALGORITHM_PROCEDURE_MATCH(ManagedForEachStatement)
				{
					PrintIndentation(argument);
					argument.writer.WriteString(L"foreach(");
					ManagedX_GenerateCode_Type(node->type, argument);
					argument.writer.WriteString(L" ");
					IdentifierToString(node->name, argument.writer);
					argument.writer.WriteString(L" in ");
					ManagedX_GenerateCode_Expression(node->container, argument);
					argument.writer.WriteString(L")\r\n");
					MXCGP newArgument(argument.writer, argument.indentation+1);
					ManagedX_GenerateCode_Statement(node->statement, newArgument);
				}

			END_ALGORITHM_PROCEDURE(ManagedX_GenerateCode_ExtendedStatement)

/***********************************************************************
Basic Members
***********************************************************************/

			BEGIN_ALGORITHM_PROCEDURE(ManagedX_GenerateCode_Member, ManagedMember, MXCGP)

				ALGORITHM_PROCEDURE_MATCH(ManagedField)
				{
					InfoToString(node->attributeInfo, argument);
					PrintIndentation(argument);
					AttributeToString(node->accessor, argument.writer);
					AttributeToString(node->memberType, argument.writer);
					AttributeToString(node->dataType, argument.writer);

					ManagedX_GenerateCode_Type(node->type, argument);
					argument.writer.WriteString(L" ");
					IdentifierToString(node->name, argument.writer);
					if(node->initializer)
					{
						argument.writer.WriteString(L" = ");
						ManagedX_GenerateCode_Expression(node->initializer, argument);
					}
					argument.writer.WriteString(L";\r\n\r\n");
				}

				ALGORITHM_PROCEDURE_MATCH(ManagedMethod)
				{
					InfoToString(node->attributeInfo, argument);
					InfoToString(node->genericInfo, argument);
					PrintIndentation(argument);
					AttributeToString(node->accessor, argument.writer);
					AttributeToString(node->memberType, argument.writer);
					AttributeToString(node->inheritation, argument.writer);

					ManagedX_GenerateCode_Type(node->returnType, argument);
					argument.writer.WriteString(L" ");
					if(node->implementedInterfaceType)
					{
						ManagedX_GenerateCode_Type(node->implementedInterfaceType, argument);
						argument.writer.WriteString(L"::");
					}

					if(node->name==L"op_pos")
					{
						argument.writer.WriteString(L"operator prefix +");
					}
					else if(node->name==L"op_neg")
					{
						argument.writer.WriteString(L"operator prefix -");
					}
					else if(node->name==L"op_not")
					{
						argument.writer.WriteString(L"operator prefix !");
					}
					else if(node->name==L"op_bitnot")
					{
						argument.writer.WriteString(L"operator prefix ~");
					}
					else if(node->name==L"op_preinc")
					{
						argument.writer.WriteString(L"operator prefix ++");
					}
					else if(node->name==L"op_predec")
					{
						argument.writer.WriteString(L"operator prefix --");
					}
					else if(node->name==L"op_postinc")
					{
						argument.writer.WriteString(L"operator postfix ++");
					}
					else if(node->name==L"op_postdec")
					{
						argument.writer.WriteString(L"operator postfix --");
					}
					else if(node->name==L"op_add_eq")
					{
						argument.writer.WriteString(L"operator +=");
					}
					else if(node->name==L"op_sub_eq")
					{
						argument.writer.WriteString(L"operator -=");
					}
					else if(node->name==L"op_mul_eq")
					{
						argument.writer.WriteString(L"operator *=");
					}
					else if(node->name==L"op_div_eq")
					{
						argument.writer.WriteString(L"operator /=");
					}
					else if(node->name==L"op_mod_eq")
					{
						argument.writer.WriteString(L"operator %=");
					}
					else if(node->name==L"op_and_eq")
					{
						argument.writer.WriteString(L"operator &&=");
					}
					else if(node->name==L"op_bitand_eq")
					{
						argument.writer.WriteString(L"operator &=");
					}
					else if(node->name==L"op_or_eq")
					{
						argument.writer.WriteString(L"operator ||=");
					}
					else if(node->name==L"op_bitor_eq")
					{
						argument.writer.WriteString(L"operator |=");
					}
					else if(node->name==L"op_xor_eq")
					{
						argument.writer.WriteString(L"operator ^=");
					}
					else if(node->name==L"op_shl_eq")
					{
						argument.writer.WriteString(L"operator <<=");
					}
					else if(node->name==L"op_shr_eq")
					{
						argument.writer.WriteString(L"operator >>=");
					}
					else if(node->name==L"op_mul")
					{
						argument.writer.WriteString(L"operator *");
					}
					else if(node->name==L"op_div")
					{
						argument.writer.WriteString(L"operator /");
					}
					else if(node->name==L"op_mod")
					{
						argument.writer.WriteString(L"operator %");
					}
					else if(node->name==L"op_add")
					{
						argument.writer.WriteString(L"operator +");
					}
					else if(node->name==L"op_sub")
					{
						argument.writer.WriteString(L"operator -");
					}
					else if(node->name==L"op_shl")
					{
						argument.writer.WriteString(L"operator <<");
					}
					else if(node->name==L"op_shr")
					{
						argument.writer.WriteString(L"operator >>");
					}
					else if(node->name==L"op_lt")
					{
						argument.writer.WriteString(L"operator <");
					}
					else if(node->name==L"op_le")
					{
						argument.writer.WriteString(L"operator <=");
					}
					else if(node->name==L"op_gt")
					{
						argument.writer.WriteString(L"operator >");
					}
					else if(node->name==L"op_ge")
					{
						argument.writer.WriteString(L"operator >=");
					}
					else if(node->name==L"op_eq")
					{
						argument.writer.WriteString(L"operator ==");
					}
					else if(node->name==L"op_ne")
					{
						argument.writer.WriteString(L"operator !=");
					}
					else if(node->name==L"op_bitand")
					{
						argument.writer.WriteString(L"operator &");
					}
					else if(node->name==L"op_xor")
					{
						argument.writer.WriteString(L"operator ^");
					}
					else if(node->name==L"op_bitor")
					{
						argument.writer.WriteString(L"operator |");
					}
					else if(node->name==L"op_and")
					{
						argument.writer.WriteString(L"operator &&");
					}
					else if(node->name==L"op_or")
					{
						argument.writer.WriteString(L"operator ||");
					}
					else if(node->name==L"op_invoke")
					{
						argument.writer.WriteString(L"operator ()");
					}
					else if(node->name==L"op_index_get")
					{
						argument.writer.WriteString(L"operator get[]");
					}
					else if(node->name==L"op_index_set")
					{
						argument.writer.WriteString(L"operator set[]");
					}
					else
					{
						IdentifierToString(node->name, argument.writer);
					}
					ParametersToString(node->parameters, argument);
					
					argument.writer.WriteString(L"\r\n");
					MXCGP newArgument(argument.writer, argument.indentation+1);
					ManagedX_GenerateCode_Statement(node->body, newArgument);
					argument.writer.WriteString(L"\r\n\r\n");
				}

				ALGORITHM_PROCEDURE_MATCH(ManagedConstructor)
				{
					InfoToString(node->attributeInfo, argument);
					PrintIndentation(argument);
					AttributeToString(node->accessor, argument.writer);

					if(node->implicit)
					{
						argument.writer.WriteString(L"implicit constructor");
					}
					else
					{
						argument.writer.WriteString(L"explicit constructor");
					}
					ParametersToString(node->parameters, argument);

					if(node->baseArguments.Count())
					{
						argument.writer.WriteString(L"\r\n");
						PrintIndentation(argument, 1);
						argument.writer.WriteString(L":base(");
						for(vint i=0;i<node->baseArguments.Count();i++)
						{
							if(i) argument.writer.WriteString(L", ");
							ArgumentToString(node->baseArguments[i], argument);
						}
						argument.writer.WriteString(L")");
					}
					
					argument.writer.WriteString(L"\r\n");
					MXCGP newArgument(argument.writer, argument.indentation+1);
					ManagedX_GenerateCode_Statement(node->body, newArgument);
					argument.writer.WriteString(L"\r\n\r\n");
				}

				ALGORITHM_PROCEDURE_MATCH(ManagedTypeMember)
				{
					ManagedX_GenerateCode_Declaration(node->declaration, argument);
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
					InfoToString(node->attributeInfo, argument);
					PrintIndentation(argument);
					AttributeToString(node->accessor, argument.writer);
					AttributeToString(node->memberType, argument.writer);
					AttributeToString(node->inheritation, argument.writer);
					
					ManagedX_GenerateCode_Type(node->type, argument);
					argument.writer.WriteString(L" ");
					if(node->implementedInterfaceType)
					{
						ManagedX_GenerateCode_Type(node->implementedInterfaceType, argument);
						argument.writer.WriteString(L"::");
					}
					IdentifierToString(node->name, argument.writer);
					argument.writer.WriteString(L"\r\n");
					PrintIndentation(argument);
					argument.writer.WriteString(L"{\r\n");
					
					MXCGP newArgument(argument.writer, argument.indentation+2);

					if(node->getter)
					{
						PrintIndentation(argument, 1);
						argument.writer.WriteString(L"get\r\n");
						ManagedX_GenerateCode_Statement(node->getter, newArgument);
						argument.writer.WriteString(L"\r\n");
					}
					if(node->setter)
					{
						PrintIndentation(argument, 1);
						AttributeToString(node->setterAccessor, argument.writer);
						if(node->setterParameter==L"")
						{
							argument.writer.WriteString(L"set\r\n");
						}
						else
						{
							argument.writer.WriteString(L"set("+node->setterParameter+L")\r\n");
						}
						ManagedX_GenerateCode_Statement(node->setter, newArgument);
						argument.writer.WriteString(L"\r\n");
					}

					PrintIndentation(argument);
					argument.writer.WriteString(L"}\r\n\r\n");
				}

				ALGORITHM_PROCEDURE_MATCH(ManagedConverterOperator)
				{
					InfoToString(node->attributeInfo, argument);
					InfoToString(node->genericInfo, argument);
					PrintIndentation(argument);
					AttributeToString(node->accessor, argument.writer);
					AttributeToString(node->memberType, argument.writer);
					AttributeToString(node->inheritation, argument.writer);

					if(node->implicit)
					{
						argument.writer.WriteString(L"implicit as ");
					}
					else
					{
						argument.writer.WriteString(L"explicit as ");
					}
					ManagedX_GenerateCode_Type(node->targetType, argument);
					argument.writer.WriteString(L"()\r\n");

					MXCGP newArgument(argument.writer, argument.indentation+1);
					ManagedX_GenerateCode_Statement(node->body, newArgument);
					argument.writer.WriteString(L"\r\n\r\n");
				}

			END_ALGORITHM_PROCEDURE(ManagedX_GenerateCode_ExtendedMember)

/***********************************************************************
Basic Declaration
***********************************************************************/

			BEGIN_ALGORITHM_PROCEDURE(ManagedX_GenerateCode_Declaration, ManagedDeclaration, MXCGP)

				ALGORITHM_PROCEDURE_MATCH(ManagedTypeDeclaration)
				{
					InfoToString(node->attributeInfo, argument);
					InfoToString(node->genericInfo, argument);
					PrintIndentation(argument);
					AttributeToString(node->accessor, argument.writer);
					AttributeToString(node->inheritation, argument.writer);

					switch(node->declarationType)
					{
					case ManagedTypeDeclaration::Class:
						argument.writer.WriteString(L"class ");
						break;
					case ManagedTypeDeclaration::Structure:
						argument.writer.WriteString(L"struct ");
						break;
					case ManagedTypeDeclaration::Interface:
						argument.writer.WriteString(L"interface ");
						break;
					}
					IdentifierToString(node->name, argument.writer);
					if(node->baseTypes.Count())
					{
						argument.writer.WriteString(L" : ");
						for(vint i=0;i<node->baseTypes.Count();i++)
						{
							if(i) argument.writer.WriteString(L", ");
							ManagedX_GenerateCode_Type(node->baseTypes[i], argument);
						}
					}
					argument.writer.WriteString(L"\r\n");
					PrintIndentation(argument);
					argument.writer.WriteString(L"{\r\n");

					MXCGP newArgument(argument.writer, argument.indentation+1);
					for(vint i=0;i<node->members.Count();i++)
					{
						ManagedX_GenerateCode_Member(node->members[i], newArgument);
					}

					PrintIndentation(argument);
					argument.writer.WriteString(L"}\r\n\r\n");
				}

				ALGORITHM_PROCEDURE_MATCH(ManagedNamespaceDeclaration)
				{
					PrintIndentation(argument);
					argument.writer.WriteString(L"namespace ");
					for(vint i=0;i<node->namespaceFragments.Count();i++)
					{
						if(i) argument.writer.WriteString(L".");
						IdentifierToString(node->namespaceFragments[i], argument.writer);
					}
					argument.writer.WriteString(L"\r\n");
					PrintIndentation(argument);
					argument.writer.WriteString(L"{\r\n");

					MXCGP newArgument(argument.writer, argument.indentation+1);
					for(vint i=0;i<node->declarations.Count();i++)
					{
						ManagedX_GenerateCode_Declaration(node->declarations[i], newArgument);
					}
					
					PrintIndentation(argument);
					argument.writer.WriteString(L"}\r\n\r\n");
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
					InfoToString(node->attributeInfo, argument);
					PrintIndentation(argument);
					AttributeToString(node->accessor, argument.writer);

					if(node->composable)
					{
						argument.writer.WriteString(L"enum switch ");
					}
					else
					{
						argument.writer.WriteString(L"enum ");
					}
					IdentifierToString(node->name, argument.writer);
					argument.writer.WriteString(L"\r\n");
					PrintIndentation(argument);
					argument.writer.WriteString(L"{\r\n");
					MXCGP newArgument(argument.writer, argument.indentation+1);
					for(vint i=0;i<node->items.Count();i++)
					{
						if(i) argument.writer.WriteString(L",\r\n");
						InfoToString(node->items[i]->attributeInfo, newArgument);
						PrintIndentation(argument, 1);
						IdentifierToString(node->items[i]->name, argument.writer);
						if(node->items[i]->value)
						{
							argument.writer.WriteString(L" = ");
							ManagedX_GenerateCode_Expression(node->items[i]->value, newArgument);
						}
					}
					argument.writer.WriteString(L"\r\n");
					PrintIndentation(argument);
					argument.writer.WriteString(L"}\r\n\r\n");
				}

				ALGORITHM_PROCEDURE_MATCH(ManagedTypeRenameDeclaration)
				{
					InfoToString(node->genericInfo, argument);
					PrintIndentation(argument);
					AttributeToString(node->accessor, argument.writer);

					argument.writer.WriteString(L"using ");
					IdentifierToString(node->name, argument.writer);
					argument.writer.WriteString(L" = ");
					ManagedX_GenerateCode_Type(node->type, argument);
					argument.writer.WriteString(L";\r\n\r\n");
				}

				ALGORITHM_PROCEDURE_MATCH(ManagedUsingNamespaceDeclaration)
				{
					PrintIndentation(argument);
					argument.writer.WriteString(L"using ");
					for(vint i=0;i<node->namespaceFragments.Count();i++)
					{
						if(i) argument.writer.WriteString(L".");
						IdentifierToString(node->namespaceFragments[i], argument.writer);
					}
					argument.writer.WriteString(L";\r\n\r\n");
				}

			END_ALGORITHM_PROCEDURE(ManagedX_GenerateCode_ExtendedDeclaration)

		}
	}
}