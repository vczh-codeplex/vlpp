#include "NativeXCodeGenerator.h"

namespace vl
{
	namespace scripting
	{
		namespace language_nativex
		{

/***********************************************************************
×Ö·û´®×ª»»¸¨Öúº¯Êý
***********************************************************************/

			const wchar_t* PrimitiveTypeToString(BasicPrimitiveTypeEnum type)
			{
				switch(type)
				{
				case s8:
					return L"int8";
				case s16:
					return L"int16";
				case s32:
					return L"int32";
				case s64:
					return L"int64";
				case u8:
					return L"uint8";
				case u16:
					return L"uint16";
				case u32:
					return L"uint32";
				case u64:
					return L"uint64";
				case f32:
					return L"f32";
				case f64:
					return L"f64";
				case bool_type:
					return L"bool";
				case void_type:
					return L"void";
				case char_type:
					return L"char";
				case wchar_type:
					return L"wchar";
				default:
					return L"<UNKNOWN PRIMITIVE TYPE>";
				}
			}

			const wchar_t* UnaryOperatorToString(BasicUnaryExpression::OperatorType type)
			{
				switch(type)
				{
				case BasicUnaryExpression::PrefixIncrease:
					return L"++";
				case BasicUnaryExpression::PostfixIncrease:
					return L"++";
				case BasicUnaryExpression::PrefixDecrease:
					return L"--";
				case BasicUnaryExpression::PostfixDecrease:
					return L"--";
				case BasicUnaryExpression::GetAddress:
					return L"&";
				case BasicUnaryExpression::DereferencePointer:
					return L"*";
				case BasicUnaryExpression::Negative:
					return L"-";
				case BasicUnaryExpression::Not:
					return L"!";
				case BasicUnaryExpression::BitNot:
					return L"~";
				default:
					return L"<UNKNOWN UNARY OPERATOR>";
				}
			}

			const wchar_t* BinaryOperatorToString(BasicBinaryExpression::OperatorType type)
			{
				switch(type)
				{
				case BasicBinaryExpression::Add:
					return L"+";
				case BasicBinaryExpression::Sub:
					return L"-";
				case BasicBinaryExpression::Mul:
					return L"*";
				case BasicBinaryExpression::Div:
					return L"/";
				case BasicBinaryExpression::Mod:
					return L"%";
				case BasicBinaryExpression::Shl:
					return L"<<";
				case BasicBinaryExpression::Shr:
					return L">>";
				case BasicBinaryExpression::And:
					return L"&&";
				case BasicBinaryExpression::Or:
					return L"||";
				case BasicBinaryExpression::Xor:
					return L"^";
				case BasicBinaryExpression::BitAnd:
					return L"&";
				case BasicBinaryExpression::BitOr:
					return L"|";
				case BasicBinaryExpression::Lt:
					return L"<";
				case BasicBinaryExpression::Le:
					return L"<=";
				case BasicBinaryExpression::Gt:
					return L">";
				case BasicBinaryExpression::Ge:
					return L">=";
				case BasicBinaryExpression::Eq:
					return L"==";
				case BasicBinaryExpression::Ne:
					return L"!=";
				case BasicBinaryExpression::AddAssign:
					return L"+=";
				case BasicBinaryExpression::SubAssign:
					return L"-=";
				case BasicBinaryExpression::MulAssign:
					return L"*=";
				case BasicBinaryExpression::DivAssign:
					return L"/=";
				case BasicBinaryExpression::ModAssign:
					return L"%=";
				case BasicBinaryExpression::ShlAssign:
					return L"<<=";
				case BasicBinaryExpression::ShrAssign:
					return L">>=";
				case BasicBinaryExpression::AndAssign:
					return L"&&=";
				case BasicBinaryExpression::OrAssign:
					return L"||=";
				case BasicBinaryExpression::XorAssign:
					return L"^=";
				case BasicBinaryExpression::BitAndAssign:
					return L"&=";
				case BasicBinaryExpression::BitOrAssign:
					return L"|=";
				case BasicBinaryExpression::Assign:
					return L"=";
				default:
					return L"<UNKNOWN BINARY OPERATOR>";
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
					L"true"
					, L"false"
					, L"null"
					, L"exception"
					, L"stackdata"
					, L"result"
					, L"function"
					, L"cast"
					, L"variable"
					, L"if"
					, L"else"
					, L"break"
					, L"continue"
					, L"exit"
					, L"while"
					, L"do"
					, L"loop"
					, L"when"
					, L"for"
					, L"with"
					, L"try"
					, L"catch"
					, L"throw"
					, L"type"
					, L"structure"
					, L"unit"
					, L"uses"
					, L"alias"
					, L"generic"
					, L"concept"
					, L"instance"
					, L"where"
					, L"foreign"
					, L"int"
					, L"int8"
					, L"int16"
					, L"int32"
					, L"int64"
					, L"uint"
					, L"uint8"
					, L"uint16"
					, L"uint32"
					, L"uint64"
					, L"f32"
					, L"f64"
					, L"bool"
					, L"char"
					, L"wchar"
					, L"void"
				};

				const wchar_t* buffer=identifier.Buffer();
				const wchar_t* reading=buffer;
				bool isID=true;

				while(wchar_t c=*reading++)
				{
					if((L'a'<=c && c<=L'z') || (L'A'<=c && c<=L'Z') || c==L'_')
					{
					}
					else if(L'0'<=c && c<=L'9')
					{
						if(reading==buffer+1)
						{
							isID=false;
							break;
						}
					}
					else
					{
						isID=false;
						break;
					}
				}

				if(isID)
				{
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
						writer.WriteString(L"@\"");
						writer.WriteString(identifier);
						writer.WriteString(L"\"");
					}
					else
					{
						writer.WriteString(identifier);
					}
				}
				else
				{
					writer.WriteString(L"@\"");
					UnescapeStringContent(identifier, writer);
					writer.WriteString(L"\"");
				}
			}

			void LinkingToString(const BasicLinking& linking, TextWriter& writer)
			{
				IdentifierToString(linking.assemblyName, writer);
				writer.WriteString(L".");
				IdentifierToString(linking.symbolName, writer);
			}

			void GenericToString(const BasicGeneric& genericDeclaration, TextWriter& writer)
			{
				writer.WriteString(L"generic<");
				for(vint i=0;i<genericDeclaration.arguments.Count();i++)
				{
					if(i)writer.WriteString(L", ");
					writer.WriteString(genericDeclaration.arguments.Get(i));
				}
				writer.WriteString(L">");
			}

/***********************************************************************
´úÂëÉú³É
***********************************************************************/

			void PrintIndentation(const NXCGP& argument, vint offset=0)
			{
				for(vint i=0;i<argument.indentation+offset;i++)
				{
					argument.writer.WriteString(L"    ");
				}
			}

			BEGIN_ALGORITHM_PROCEDURE(NativeX_BasicType_GenerateCode, BasicType, NXCGP)

				ALGORITHM_PROCEDURE_MATCH(BasicPrimitiveType)
				{
					argument.writer.WriteString(PrimitiveTypeToString(node->type));
				}

				ALGORITHM_PROCEDURE_MATCH(BasicPointerType)
				{
					NativeX_BasicType_GenerateCode(node->elementType, argument);
					argument.writer.WriteString(L"*");
				}

				ALGORITHM_PROCEDURE_MATCH(BasicArrayType)
				{
					NativeX_BasicType_GenerateCode(node->elementType, argument);
					argument.writer.WriteString(L"[");
					NativeX_BasicExpression_GenerateCode(node->count, argument);
					argument.writer.WriteString(L"]");
				}

				ALGORITHM_PROCEDURE_MATCH(BasicReferenceType)
				{
					IdentifierToString(node->name, argument.writer);
				}

				ALGORITHM_PROCEDURE_MATCH(BasicFunctionType)
				{
					argument.writer.WriteString(L"function ");
					NativeX_BasicType_GenerateCode(node->returnType, argument);
					argument.writer.WriteString(L"(");
					for(vint i=0;i<node->parameterTypes.Count();i++)
					{
						if(i)
						{
							argument.writer.WriteString(L", ");
						}
						NativeX_BasicType_GenerateCode(node->parameterTypes[i], argument);
					}
					argument.writer.WriteString(L")");
				}

				ALGORITHM_PROCEDURE_MATCH(BasicInstantiatedGenericType)
				{
					NativeX_BasicType_GenerateCode(node->elementType, argument);
					argument.writer.WriteString(L"<");
					for(vint i=0;i<node->argumentTypes.Count();i++)
					{
						if(i)
						{
							argument.writer.WriteString(L", ");
						}
						NativeX_BasicType_GenerateCode(node->argumentTypes[i], argument);
					}
					argument.writer.WriteString(L">");
				}

				ALGORITHM_PROCEDURE_MATCH(BasicTypeofExpressionType)
				{
					argument.writer.WriteString(L"typeof((");
					NativeX_BasicExpression_GenerateCode(node->expression, argument);
					argument.writer.WriteString(L"))");
				}

				ALGORITHM_PROCEDURE_MATCH(BasicTypeofMemberType)
				{
					argument.writer.WriteString(L"typeof(");
					NativeX_BasicType_GenerateCode(node->type, argument);
					argument.writer.WriteString(L"::");
					argument.writer.WriteString(node->member);
					argument.writer.WriteString(L")");
				}

				ALGORITHM_PROCEDURE_MATCH(BasicExtendedType)
				{
				}

			END_ALGORITHM_PROCEDURE(NativeX_BasicType_GenerateCode)

			BEGIN_ALGORITHM_PROCEDURE(NativeX_BasicExpression_GenerateCode, BasicExpression, NXCGP)

				ALGORITHM_PROCEDURE_MATCH(BasicNullExpression)
				{
					argument.writer.WriteString(L"null");
				}

				ALGORITHM_PROCEDURE_MATCH(BasicExceptionAddressExpression)
				{
					argument.writer.WriteString(L"exception");
				}

				ALGORITHM_PROCEDURE_MATCH(BasicStackDataAddressExpression)
				{
					argument.writer.WriteString(L"stackdata");
				}

				ALGORITHM_PROCEDURE_MATCH(BasicNumericExpression)
				{
					switch(node->type)
					{
					case s8:			argument.writer.WriteString(itow(node->argument.s8)+L"s8");					break;
					case s16:			argument.writer.WriteString(itow(node->argument.s16)+L"s16");				break;
					case s32:			argument.writer.WriteString(itow(node->argument.s32)+L"s32");				break;
					case s64:			argument.writer.WriteString(i64tow(node->argument.s64)+L"s64");				break;
					case u8:			argument.writer.WriteString(utow(node->argument.u8)+L"u8");					break;
					case u16:			argument.writer.WriteString(utow(node->argument.u16)+L"u16");				break;
					case u32:			argument.writer.WriteString(utow(node->argument.u32)+L"u32");				break;
					case u64:			argument.writer.WriteString(u64tow(node->argument.u64)+L"u64");				break;
					case f32:			argument.writer.WriteString(ftow(node->argument.f32)+L"f");					break;
					case f64:			argument.writer.WriteString(ftow(node->argument.f64));						break;
					case bool_type:		argument.writer.WriteString(node->argument.bool_value?L"true":L"false");	break;
					case char_type:
						argument.writer.WriteString(L"\'");
						UnescapeStringContent(atow(AString(node->argument.char_value)), argument.writer);
						argument.writer.WriteString(L"\'");
						break;
					case wchar_type:
						argument.writer.WriteString(L"L\'");
						UnescapeStringContent(WString(node->argument.char_value), argument.writer);
						argument.writer.WriteString(L"\'");
						break;
					}
				}

				ALGORITHM_PROCEDURE_MATCH(BasicMbcsStringExpression)
				{
					argument.writer.WriteString(L"\"");
					UnescapeStringContent(atow(node->value), argument.writer);
					argument.writer.WriteString(L"\"");
				}

				ALGORITHM_PROCEDURE_MATCH(BasicUnicodeStringExpression)
				{
					argument.writer.WriteString(L"L\"");
					UnescapeStringContent(node->value, argument.writer);
					argument.writer.WriteString(L"\"");
				}

				ALGORITHM_PROCEDURE_MATCH(BasicUnaryExpression)
				{
					argument.writer.WriteString(L"(");
					switch(node->type)
					{
					case BasicUnaryExpression::PostfixIncrease:
					case BasicUnaryExpression::PostfixDecrease:
						NativeX_BasicExpression_GenerateCode(node->operand, argument);
						argument.writer.WriteString(L" ");
						argument.writer.WriteString(UnaryOperatorToString(node->type));
						argument.writer.WriteString(L" ");
						break;
					default:
						argument.writer.WriteString(L" ");
						argument.writer.WriteString(UnaryOperatorToString(node->type));
						argument.writer.WriteString(L" ");
						NativeX_BasicExpression_GenerateCode(node->operand, argument);
					}
					argument.writer.WriteString(L")");
				}

				ALGORITHM_PROCEDURE_MATCH(BasicBinaryExpression)
				{
					argument.writer.WriteString(L"(");
					NativeX_BasicExpression_GenerateCode(node->leftOperand, argument);
					argument.writer.WriteString(L" ");
					argument.writer.WriteString(BinaryOperatorToString(node->type));
					argument.writer.WriteString(L" ");
					NativeX_BasicExpression_GenerateCode(node->rightOperand, argument);
					argument.writer.WriteString(L")");
				}

				ALGORITHM_PROCEDURE_MATCH(BasicSubscribeExpression)
				{
					NativeX_BasicExpression_GenerateCode(node->operand, argument);
					argument.writer.WriteString(L"[");
					NativeX_BasicExpression_GenerateCode(node->subscribe, argument);
					argument.writer.WriteString(L"]");
				}

				ALGORITHM_PROCEDURE_MATCH(BasicMemberExpression)
				{
					NativeX_BasicExpression_GenerateCode(node->operand, argument);
					argument.writer.WriteString(node->pointerMember?L"->":L".");
					IdentifierToString(node->member, argument.writer);
				}

				ALGORITHM_PROCEDURE_MATCH(BasicInvokeExpression)
				{
					NativeX_BasicExpression_GenerateCode(node->function, argument);
					argument.writer.WriteString(L"(");
					for(vint i=0;i<node->arguments.Count();i++)
					{
						if(i)argument.writer.WriteString(L", ");
						NativeX_BasicExpression_GenerateCode(node->arguments[i], argument);
					}
					argument.writer.WriteString(L")");
				}

				ALGORITHM_PROCEDURE_MATCH(BasicFunctionResultExpression)
				{
					argument.writer.WriteString(L"result");
				}

				ALGORITHM_PROCEDURE_MATCH(BasicSizeofExpression)
				{
					argument.writer.WriteString(L"sizeof(");
					NativeX_BasicType_GenerateCode(node->type, argument);
					argument.writer.WriteString(L")");
				}

				ALGORITHM_PROCEDURE_MATCH(BasicOffsetofExpression)
				{
					argument.writer.WriteString(L"offsetof(");
					NativeX_BasicType_GenerateCode(node->type, argument);
					argument.writer.WriteString(L"::");
					argument.writer.WriteString(node->member);
					argument.writer.WriteString(L")");
				}

				ALGORITHM_PROCEDURE_MATCH(BasicCastingExpression)
				{
					argument.writer.WriteString(L"cast<");
					NativeX_BasicType_GenerateCode(node->type, argument);
					argument.writer.WriteString(L">(");
					NativeX_BasicExpression_GenerateCode(node->operand, argument);
					argument.writer.WriteString(L")");
				}

				ALGORITHM_PROCEDURE_MATCH(BasicReferenceExpression)
				{
					IdentifierToString(node->name, argument.writer);
				}

				ALGORITHM_PROCEDURE_MATCH(BasicInstantiatedExpression)
				{
					IdentifierToString(node->reference->name, argument.writer);
					argument.writer.WriteString(L"<");
					for(vint i=0;i<node->argumentTypes.Count();i++)
					{
						if(i)argument.writer.WriteString(L", ");
						NativeX_BasicType_GenerateCode(node->argumentTypes[i], argument);
					}
					argument.writer.WriteString(L">");
				}

				ALGORITHM_PROCEDURE_MATCH(BasicInstanceFunctionExpression)
				{
					IdentifierToString(node->conceptName, argument.writer);
					argument.writer.WriteString(L"<");
					NativeX_BasicType_GenerateCode(node->type, argument);
					argument.writer.WriteString(L">::");
					IdentifierToString(node->functionName, argument.writer);
				}

				ALGORITHM_PROCEDURE_MATCH(BasicExtendedExpression)
				{
				}

			END_ALGORITHM_PROCEDURE(NativeX_BasicExpression_GenerateCode)

			BEGIN_ALGORITHM_PROCEDURE(NativeX_BasicStatement_GenerateCode, BasicStatement, NXCGP)

				ALGORITHM_PROCEDURE_MATCH(BasicEmptyStatement)
				{
					PrintIndentation(argument);
					argument.writer.WriteLine(L";");
				}

				ALGORITHM_PROCEDURE_MATCH(BasicCompositeStatement)
				{
					PrintIndentation(argument, -1);
					argument.writer.WriteLine(L"{");
					for(vint i=0;i<node->statements.Count();i++)
					{
						NativeX_BasicStatement_GenerateCode(node->statements[i], argument);
					}
					PrintIndentation(argument, -1);
					argument.writer.WriteLine(L"}");
				}

				ALGORITHM_PROCEDURE_MATCH(BasicExpressionStatement)
				{
					PrintIndentation(argument);
					NativeX_BasicExpression_GenerateCode(node->expression, argument);
					argument.writer.WriteLine(L";");
				}

				ALGORITHM_PROCEDURE_MATCH(BasicVariableStatement)
				{
					PrintIndentation(argument);
					argument.writer.WriteString(L"variable ");
					NativeX_BasicType_GenerateCode(node->type, argument);
					argument.writer.WriteString(L" ");
					IdentifierToString(node->name, argument.writer);
					if(node->initializer)
					{
						argument.writer.WriteString(L" = ");
						NativeX_BasicExpression_GenerateCode(node->initializer, argument);
					}
					argument.writer.WriteLine(L";");
				}

				ALGORITHM_PROCEDURE_MATCH(BasicIfStatement)
				{
					NXCGP newArgument(argument.writer, argument.indentation+1);
					PrintIndentation(argument);
					argument.writer.WriteString(L"if(");
					NativeX_BasicExpression_GenerateCode(node->condition, argument);
					argument.writer.WriteLine(L")");
					NativeX_BasicStatement_GenerateCode(node->trueStatement, newArgument);
					if(node->falseStatement)
					{
						PrintIndentation(argument);
						argument.writer.WriteLine(L"else");
						NativeX_BasicStatement_GenerateCode(node->falseStatement, newArgument);
					}
				}

				ALGORITHM_PROCEDURE_MATCH(BasicWhileStatement)
				{
					NXCGP newArgument(argument.writer, argument.indentation+1);
					PrintIndentation(argument);
					if(node->beginCondition)
					{
						argument.writer.WriteString(L"while(");
						NativeX_BasicExpression_GenerateCode(node->beginCondition, argument);
						argument.writer.WriteLine(L")");
					}
					else
					{
						argument.writer.WriteLine(L"while(true)");
					}
					NativeX_BasicStatement_GenerateCode(node->statement, newArgument);
					if(node->endCondition)
					{
						PrintIndentation(argument);
						argument.writer.WriteString(L"when(");
						NativeX_BasicExpression_GenerateCode(node->endCondition, argument);
						argument.writer.WriteLine(L");");
					}
				}

				ALGORITHM_PROCEDURE_MATCH(BasicForStatement)
				{
					NXCGP newArgument(argument.writer, argument.indentation+1);

					PrintIndentation(argument);
					argument.writer.WriteLine(L"for (");
					NativeX_BasicStatement_GenerateCode(node->initializer, newArgument);
					
					PrintIndentation(argument);
					argument.writer.WriteString(L"when(");
					NativeX_BasicExpression_GenerateCode(node->condition, argument);
					argument.writer.WriteLine(L")");
					
					PrintIndentation(argument);
					argument.writer.WriteLine(L"with");
					NativeX_BasicStatement_GenerateCode(node->sideEffect, newArgument);
					
					PrintIndentation(argument);
					argument.writer.WriteLine(L") do");
					NativeX_BasicStatement_GenerateCode(node->statement, newArgument);
				}

				ALGORITHM_PROCEDURE_MATCH(BasicBreakStatement)
				{
					PrintIndentation(argument);
					argument.writer.WriteLine(L"break;");
				}

				ALGORITHM_PROCEDURE_MATCH(BasicContinueStatement)
				{
					PrintIndentation(argument);
					argument.writer.WriteLine(L"continue;");
				}

				ALGORITHM_PROCEDURE_MATCH(BasicReturnStatement)
				{
					PrintIndentation(argument);
					argument.writer.WriteLine(L"exit;");
				}
				

				ALGORITHM_PROCEDURE_MATCH(BasicTryCatchStatement)
				{
					NXCGP newArgument(argument.writer, argument.indentation+1);

					PrintIndentation(argument);
					argument.writer.WriteLine(L"try");
					NativeX_BasicStatement_GenerateCode(node->tryStatement, newArgument);
					PrintIndentation(argument);
					argument.writer.WriteLine(L"catch");
					NativeX_BasicStatement_GenerateCode(node->catchStatement, newArgument);
				}

				ALGORITHM_PROCEDURE_MATCH(BasicThrowStatement)
				{
					PrintIndentation(argument);
					if(node->expression)
					{
						argument.writer.WriteString(L"throw ");
						NativeX_BasicExpression_GenerateCode(node->expression, argument);
						argument.writer.WriteLine(L";");
					}
					else
					{
						argument.writer.WriteLine(L"throw;");
					}
				}

				ALGORITHM_PROCEDURE_MATCH(BasicExtendedStatement)
				{
				}

			END_ALGORITHM_PROCEDURE(NativeX_BasicStatement_GenerateCode)

			void PrintGeneric(BasicDeclaration* node, const NXCGP& argument)
			{
				if(node->genericDeclaration.HasGeneric())
				{
					PrintIndentation(argument);
					GenericToString(node->genericDeclaration, argument.writer);
					if(node->genericDeclaration.constraints.Count()>0)
					{
						argument.writer.WriteLine(L" where");
						for(vint i=0;i<node->genericDeclaration.constraints.Count();i++)
						{
							if(i)
							{
								argument.writer.WriteLine(L",");
							}
							Ptr<BasicGeneric::Constraint> constraint=node->genericDeclaration.constraints[i];
							PrintIndentation(argument);
							argument.writer.WriteString(L"  ");
							IdentifierToString(constraint->argumentName, argument.writer);
							argument.writer.WriteString(L" : ");
							IdentifierToString(constraint->conceptName, argument.writer);
						}
					}
					argument.writer.WriteLine(L"");
				}
			}

			void PrintAttribute(BasicDeclaration* node, const NXCGP& argument)
			{
				for(vint i=0;i<node->attributes.Count();i++)
				{
					PrintIndentation(argument);
					argument.writer.WriteString(L"#");
					argument.writer.WriteLine(node->attributes[i]->attributeName);
				}
			}

			BEGIN_ALGORITHM_PROCEDURE(NativeX_BasicDeclaration_GenerateCode, BasicDeclaration, NXCGP)

				ALGORITHM_PROCEDURE_MATCH(BasicFunctionDeclaration)
				{
					PrintAttribute(node, argument);
					PrintGeneric(node, argument);
					PrintIndentation(argument);
					if(node->foreignFunction)
					{
						argument.writer.WriteString(L"foreign function ");
					}
					else
					{
						argument.writer.WriteString(L"function ");
					}
					NativeX_BasicType_GenerateCode(node->signatureType->returnType, argument);
					argument.writer.WriteString(L" ");
					IdentifierToString(node->name, argument.writer);
					argument.writer.WriteString(L"");
					PrintIndentation(argument);
					argument.writer.WriteString(L"(");

					vint min=node->parameterNames.Count()<node->signatureType->parameterTypes.Count()?node->parameterNames.Count():node->signatureType->parameterTypes.Count();
					for(vint i=0;i<min;i++)
					{
						if(i)argument.writer.WriteString(L", ");
						NativeX_BasicType_GenerateCode(node->signatureType->parameterTypes[i], argument);
						argument.writer.WriteString(L" ");
						IdentifierToString(node->parameterNames[i], argument.writer);
					}

					if(node->linking.HasLink())
					{
						argument.writer.WriteString(L") alias ");
						LinkingToString(node->linking, argument.writer);
						argument.writer.WriteLine(L";");
					}
					else
					{
						argument.writer.WriteLine(L")");
					}

					if(node->statement)
					{
						NXCGP newArgument(argument.writer, argument.indentation+1);
						NativeX_BasicStatement_GenerateCode(node->statement, newArgument);
					}
				}

				ALGORITHM_PROCEDURE_MATCH(BasicVariableDeclaration)
				{
					PrintAttribute(node, argument);
					PrintGeneric(node, argument);
					PrintIndentation(argument);
					if(node->constant)
					{
						argument.writer.WriteString(L"constant ");
					}
					else
					{
						argument.writer.WriteString(L"variable ");
					}
					NativeX_BasicType_GenerateCode(node->type, argument);
					argument.writer.WriteString(L" ");
					IdentifierToString(node->name, argument.writer);

					if(node->linking.HasLink())
					{
						argument.writer.WriteString(L" alias ");
						LinkingToString(node->linking, argument.writer);
					}

					if(node->initializer)
					{
						argument.writer.WriteString(L" = ");
						NativeX_BasicExpression_GenerateCode(node->initializer, argument);
					}
					argument.writer.WriteLine(L";");
				}

				ALGORITHM_PROCEDURE_MATCH(BasicTypeRenameDeclaration)
				{
					PrintAttribute(node, argument);
					PrintGeneric(node, argument);
					PrintIndentation(argument);
					argument.writer.WriteString(L"type ");
					IdentifierToString(node->name, argument.writer);
					argument.writer.WriteString(L" = ");
					NativeX_BasicType_GenerateCode(node->type, argument);
					argument.writer.WriteLine(L";");
				}

				ALGORITHM_PROCEDURE_MATCH(BasicStructureDeclaration)
				{
					PrintAttribute(node, argument);
					PrintGeneric(node, argument);
					if(node->defined)
					{
						PrintIndentation(argument);
						argument.writer.WriteString(L"structure ");
						IdentifierToString(node->name, argument.writer);

						if(node->linking.HasLink())
						{
							argument.writer.WriteString(L" alias ");
							LinkingToString(node->linking, argument.writer);
						}
						argument.writer.WriteLine(L"");

						PrintIndentation(argument);
						argument.writer.WriteLine(L"{");

						vint min=node->memberNames.Count()<node->memberTypes.Count()?node->memberNames.Count():node->memberTypes.Count();
						NXCGP newArgument(argument.writer, argument.indentation+1);
						for(vint i=0;i<min;i++)
						{
							PrintIndentation(newArgument);
							NativeX_BasicType_GenerateCode(node->memberTypes[i], newArgument);
							argument.writer.WriteString(L" ");
							IdentifierToString(node->memberNames[i], argument.writer);
							argument.writer.WriteLine(L";");
						}

						PrintIndentation(argument);
						argument.writer.WriteLine(L"}");
					}
					else
					{
						PrintIndentation(argument);
						argument.writer.WriteString(L"structure ");
						IdentifierToString(node->name, argument.writer);
						argument.writer.WriteLine(L";");
					}
				}

				ALGORITHM_PROCEDURE_MATCH(BasicConceptBaseDeclaration)
				{
					PrintAttribute(node, argument);
					PrintIndentation(argument);
					argument.writer.WriteString(L"concept ");
					IdentifierToString(node->conceptType, argument.writer);
					argument.writer.WriteString(L" : ");
					IdentifierToString(node->name, argument.writer);

					if(node->linking.HasLink())
					{
						argument.writer.WriteString(L" alias ");
						LinkingToString(node->linking, argument.writer);
					}
					argument.writer.WriteLine(L"");

					PrintIndentation(argument);
					argument.writer.WriteLine(L"{");

					NXCGP newArgument(argument.writer, argument.indentation+1);
					for(vint i=0;i<node->functions.Count();i++)
					{
						BasicConceptBaseDeclaration::FunctionConcept* functionConcept=node->functions[i].Obj();
						PrintIndentation(newArgument);
						IdentifierToString(functionConcept->name, argument.writer);
						argument.writer.WriteString(L" = ");
						NativeX_BasicType_GenerateCode(functionConcept->signatureType, newArgument);
						argument.writer.WriteLine(L";");
					}

					PrintIndentation(argument);
					argument.writer.WriteLine(L"}");
				}

				ALGORITHM_PROCEDURE_MATCH(BasicConceptInstanceDeclaration)
				{
					PrintAttribute(node, argument);
					PrintGeneric(node, argument);
					argument.writer.WriteString(L"instance ");
					NativeX_BasicType_GenerateCode(node->instanceType, argument);
					argument.writer.WriteString(L" : ");
					IdentifierToString(node->name, argument.writer);
					if(node->defined)
					{
						argument.writer.WriteLine(L"");

						PrintIndentation(argument);
						argument.writer.WriteLine(L"{");

						NXCGP newArgument(argument.writer, argument.indentation+1);
						for(vint i=0;i<node->functions.Count();i++)
						{
							BasicConceptInstanceDeclaration::FunctionInstance* functionInstance=node->functions[i].Obj();
							PrintIndentation(newArgument);
							IdentifierToString(functionInstance->name, argument.writer);
							argument.writer.WriteString(L" = ");
							if(functionInstance->normalFunction)
							{
								NativeX_BasicExpression_GenerateCode(functionInstance->normalFunction, newArgument);
							}
							else
							{
								NativeX_BasicExpression_GenerateCode(functionInstance->genericFunction, newArgument);
							}
							argument.writer.WriteLine(L";");
						}

						PrintIndentation(argument);
						argument.writer.WriteLine(L"}");
					}
					else
					{
						argument.writer.WriteLine(L";");
					}
				}

				ALGORITHM_PROCEDURE_MATCH(BasicExtendedDeclaration)
				{
				}

			END_ALGORITHM_PROCEDURE(NativeX_BasicDeclaration_GenerateCode)

			void NativeX_BasicProgram_GenerateCode(Ptr<BasicProgram> program, TextWriter& writer)
			{
				NXCGP argument(writer, 0);
				for(vint i=0;i<program->declarations.Count();i++)
				{
					NativeX_BasicDeclaration_GenerateCode(program->declarations[i], argument);
					writer.WriteLine(L"");
				}
			}
		}
	}
}