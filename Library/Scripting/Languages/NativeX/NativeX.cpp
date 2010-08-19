#include "NativeX.h"
#include "NativeXErrorMessage.h"
#include "..\LanguageProviderExtension.h"
#include "..\BasicErrorMessageTranslator.h"
#include "..\..\BasicLanguage\BasicLanguageAnalyzer.h"
#include "..\..\BasicLanguage\BasicLanguageCodeGeneration.h"
#include "..\..\..\Regex\Regex.h"
#include "..\..\..\Combinator\Combinator.h"
#include "..\..\..\Combinator\ParserInput.h"
#include "..\..\..\Combinator\TokenCombinator.h"
#include "..\..\..\Collections\Operation.h"

namespace vl
{
	namespace scripting
	{
		namespace language_nativex
		{
			using namespace basicil;
			using namespace basiclanguage;
			using namespace regex;
			using namespace combinator;
			using namespace collections;
			using namespace stream;

			class NativeXUnit : public Object
			{
			public:
				WString				name;
				List<WString>		imports;
				Ptr<BasicProgram>	program;
				vint					codeIndex;
			};

			typedef Node<TokenInput<RegexToken>, RegexToken>				TokenType;
			typedef Node<TokenInput<RegexToken>, BasicLinking>				LinkingNode;
			typedef Node<TokenInput<RegexToken>, Ptr<BasicDeclaration>>		DeclarationNode;
			typedef Rule<TokenInput<RegexToken>, Ptr<BasicExpression>>		ExpressionRule;
			typedef Rule<TokenInput<RegexToken>, Ptr<BasicType>>			TypeRule;
			typedef Rule<TokenInput<RegexToken>, Ptr<BasicStatement>>		StatementRule;
			typedef Rule<TokenInput<RegexToken>, Ptr<BasicDeclaration>>		DeclarationRule;
			typedef Rule<TokenInput<RegexToken>, Ptr<NativeXUnit>>			UnitRule;

			extern void NativeX_BasicProgram_GenerateCode(Ptr<BasicProgram> program, TextWriter& writer);

/***********************************************************************
辅助函数
***********************************************************************/

			template<typename T>
			Ptr<T> CreateNode(const RegexToken& position)
			{
				Ptr<T> node=new T;
				node->position.start=position.start;
				node->position.lineStart=position.lineStart;
				node->position.lineIndex=position.lineIndex;
				node->position.codeIndex=position.codeIndex;
				return node;
			}

			Node<TokenInput<RegexToken>, RegexToken> CreateToken(List<WString>& tokens, const WString& token)
			{
				Node<TokenInput<RegexToken>, RegexToken> node=tk(tokens.Count());
				tokens.Add(token);
				return node;
			}

			wchar_t Escape(wchar_t input)
			{
				switch(input)
				{
				case L'r':
					return L'\r';
				case L'n':
					return L'\n';
				case L't':
					return L'\t';
				default:
					return input;
				}
			}

			wchar_t EscapeWalk(const wchar_t*& input)
			{
				if(*input==L'\\')
				{
					input++;
					return Escape(*input++);
				}
				else
				{
					return *input++;
				}
			}

			WString EscapeString(const WString& input)
			{
				wchar_t* buffer=new wchar_t[input.Length()+1];
				const wchar_t* reading=input.Buffer();
				wchar_t* writing=buffer;
				while(*reading)
				{
					*writing++=EscapeWalk(reading);
				}
				*writing=0;
				WString result=buffer;
				delete[] buffer;
				return result;
			}

			WString ConvertID(const WString& input)
			{
				if(input[0]==L'@')
				{
					if(input[1]==L'\"')
					{
						return EscapeString(input.Sub(2, input.Length()-3));
					}
					else
					{
						return input.Right(input.Length()-1);
					}
				}
				else
				{
					return input;
				}
			}

/***********************************************************************
语义函数：常量
***********************************************************************/

			Ptr<BasicExpression> ToTrue(const RegexToken& input)
			{
				Ptr<BasicNumericExpression> expression=CreateNode<BasicNumericExpression>(input);
				expression->type=bool_type;
				expression->argument.s64=0;
				expression->argument.bool_value=true;
				return expression;
			}

			Ptr<BasicExpression> ToFalse(const RegexToken& input)
			{
				Ptr<BasicNumericExpression> expression=CreateNode<BasicNumericExpression>(input);
				expression->type=bool_type;
				expression->argument.s64=0;
				expression->argument.bool_value=false;
				return expression;
			}

			Ptr<BasicExpression> ToAChar(const RegexToken& input)
			{
				const wchar_t* reading=input.reading+1;
				Ptr<BasicNumericExpression> expression=CreateNode<BasicNumericExpression>(input);
				expression->type=char_type;
				expression->argument.s64=0;
				expression->argument.char_value=wtoa(WString(EscapeWalk(reading)))[0];
				return expression;
			}

			Ptr<BasicExpression> ToWChar(const RegexToken& input)
			{
				const wchar_t* reading=input.reading+2;
				Ptr<BasicNumericExpression> expression=CreateNode<BasicNumericExpression>(input);
				expression->type=char_type;
				expression->argument.s64=0;
				expression->argument.wchar_value=EscapeWalk(reading);
				return expression;
			}

			Ptr<BasicExpression> ToAString(const RegexToken& input)
			{
				Ptr<BasicMbcsStringExpression> expression=CreateNode<BasicMbcsStringExpression>(input);
				expression->value=wtoa(EscapeString(WString(input.reading+1, input.length-2)));
				return expression;
			}

			Ptr<BasicExpression> ToWString(const RegexToken& input)
			{
				Ptr<BasicUnicodeStringExpression> expression=CreateNode<BasicUnicodeStringExpression>(input);
				expression->value=EscapeString(WString(input.reading+2, input.length-3));
				return expression;
			}

			Ptr<BasicExpression> ToInteger(const RegexToken& input)
			{
				bool sign=true;
				vint bits=8*sizeof(vint);
				if(input.length>2)
				{
					switch(input.reading[input.length-2])
					{
					case L's':
						sign=true;
						bits=8;
						goto FINISHED_TYPE_RECOGNIZING;
					case L'u':
						sign=false;
						bits=8;
						goto FINISHED_TYPE_RECOGNIZING;
					}
				}
				if(input.length>3)
				{
					switch(input.reading[input.length-3])
					{
					case L's':
						sign=true;
						break;
					case L'u':
						sign=false;
						break;
					default:
						goto FINISHED_TYPE_RECOGNIZING;
					}
					if(wcsncmp(input.reading+input.length-2, L"16", 2)==0)
					{
						bits=16;
					}
					else if(wcsncmp(input.reading+input.length-2, L"32", 2)==0)
					{
						bits=32;
					}
					else if(wcsncmp(input.reading+input.length-2, L"64", 2)==0)
					{
						bits=64;
					}
					else
					{
						CHECK_ERROR(false, L"词法分析器有错。");
					}
				}
			FINISHED_TYPE_RECOGNIZING:
				Ptr<BasicNumericExpression> expression=CreateNode<BasicNumericExpression>(input);
				if(sign)
				{
					expression->argument.s64=wtoi64(WString(input.reading, input.length));
					switch(bits)
					{
					case 8:
						expression->type=s8;
						break;
					case 16:
						expression->type=s16;
						break;
					case 32:
						expression->type=s32;
						break;
					case 64:
						expression->type=s64;
						break;
					default:
						CHECK_ERROR(false, L"词法分析器有错。");
					}
				}
				else
				{
					expression->argument.u64=wtou64(WString(input.reading, input.length));
					switch(bits)
					{
					case 8:
						expression->type=u8;
						break;
					case 16:
						expression->type=u16;
						break;
					case 32:
						expression->type=u32;
						break;
					case 64:
						expression->type=u64;
						break;
					default:
						CHECK_ERROR(false, L"词法分析器有错。");
					}
				}
				return expression;
			}

			Ptr<BasicExpression> ToFloat(const RegexToken& input)
			{
				Ptr<BasicNumericExpression> expression=CreateNode<BasicNumericExpression>(input);
				expression->type=bool_type;
				expression->argument.s64=0;
				expression->argument.f32=(float)wtof(WString(input.reading, input.length));
				return expression;
			}

			Ptr<BasicExpression> ToDouble(const RegexToken& input)
			{
				Ptr<BasicNumericExpression> expression=CreateNode<BasicNumericExpression>(input);
				expression->type=bool_type;
				expression->argument.s64=0;
				expression->argument.f64=wtof(WString(input.reading, input.length));
				return expression;
			}

			Ptr<BasicExpression> ToNull(const RegexToken& input)
			{
				return new BasicNullExpression;
			}

			Ptr<BasicExpression> ToReference(const RegexToken& input)
			{
				Ptr<BasicReferenceExpression> expression=CreateNode<BasicReferenceExpression>(input);
				expression->name=ConvertID(WString(input.reading, input.length));
				return expression;
			}

			Ptr<BasicExpression> ToInstanciatedExpression(const ParsingPair<RegexToken, ParsingPair<RegexToken, Ptr<List<Ptr<BasicType>>>>>& input)
			{
				Ptr<BasicReferenceExpression> reference=CreateNode<BasicReferenceExpression>(input.First());
				reference->name=ConvertID(WString(input.First().reading, input.First().length));

				Ptr<BasicInstanciatedExpression> expression=CreateNode<BasicInstanciatedExpression>(input.Second().First());
				expression->reference=reference;
				CopyFrom(expression->argumentTypes.Wrap(), input.Second().Second()->Wrap());
				return expression;
			}

			Ptr<BasicExpression> ToBasicInstanceFunctionExpression(const ParsingPair<ParsingPair<RegexToken, Ptr<BasicType>>, RegexToken>& input)
			{
				Ptr<BasicInstanceFunctionExpression> expression=CreateNode<BasicInstanceFunctionExpression>(input.First().First());
				expression->conceptName=ConvertID(WString(input.First().First().reading, input.First().First().length));
				expression->type=input.First().Second();
				expression->functionName=ConvertID(WString(input.Second().reading, input.Second().length));
				return expression;
			}

			Ptr<BasicExpression> ToResult(const RegexToken& input)
			{
				return CreateNode<BasicFunctionResultExpression>(input);
			}

/***********************************************************************
语义函数：表达式
***********************************************************************/

			class NativeXArguments : public BasicExtendedExpression
			{
			public:
				Ptr<List<Ptr<BasicExpression>>>		arguments;
			};

			ParsingPair<RegexToken, Ptr<BasicExpression>> UpgradePostfix(const RegexToken& input)
			{
				return ParsingPair<RegexToken, Ptr<BasicExpression>>(input, 0);
			}

			Ptr<BasicExpression> UpgradeArguments(const Ptr<List<Ptr<BasicExpression>>>& arguments)
			{
				Ptr<NativeXArguments> expression=new NativeXArguments;
				expression->arguments=arguments;
				return expression;
			}

			Ptr<BasicExpression> ToPostUnary(
				const Ptr<BasicExpression>& operand, 
				const ParsingPair<RegexToken, Ptr<BasicExpression>>& unary
				)
			{
				WString op(unary.First().reading, unary.First().length);
				if(op==L"[")
				{
					Ptr<BasicSubscribeExpression> expression=CreateNode<BasicSubscribeExpression>(unary.First());
					expression->operand=operand;
					expression->subscribe=unary.Second();
					return expression;
				}
				else if(op==L"(")
				{
					Ptr<NativeXArguments> arguments=unary.Second().Cast<NativeXArguments>();
					Ptr<BasicInvokeExpression> expression=CreateNode<BasicInvokeExpression>(unary.First());
					expression->function=operand;
					CopyFrom(expression->arguments.Wrap(), arguments->arguments->Wrap());
					return expression;
				}
				else if(op==L".")
				{
					Ptr<BasicReferenceExpression> member=unary.Second().Cast<BasicReferenceExpression>();
					Ptr<BasicMemberExpression> expression=CreateNode<BasicMemberExpression>(unary.First());
					expression->operand=operand;
					expression->member=member->name;
					expression->pointerMember=false;
					return expression;
				}
				else if(op==L"->")
				{
					Ptr<BasicReferenceExpression> member=unary.Second().Cast<BasicReferenceExpression>();
					Ptr<BasicMemberExpression> expression=CreateNode<BasicMemberExpression>(unary.First());
					expression->operand=operand;
					expression->member=member->name;
					expression->pointerMember=true;
					return expression;
				}
				else if(op==L"++")
				{
					Ptr<BasicUnaryExpression> expression=CreateNode<BasicUnaryExpression>(unary.First());
					expression->operand=operand;
					expression->type=BasicUnaryExpression::PostfixIncrease;
					return expression;
				}
				else if(op==L"--")
				{
					Ptr<BasicUnaryExpression> expression=CreateNode<BasicUnaryExpression>(unary.First());
					expression->operand=operand;
					expression->type=BasicUnaryExpression::PostfixDecrease;
					return expression;
				}
				else
				{
					CHECK_ERROR(false, L"language_nativex::ToPostUnary()#错误的操作符。");
				}
			}

			Ptr<BasicExpression> ToPreUnary(const ParsingPair<RegexToken, Ptr<BasicExpression>>& input)
			{
				WString op(input.First().reading, input.First().length);
				if(op==L"++")
				{
					Ptr<BasicUnaryExpression> expression=CreateNode<BasicUnaryExpression>(input.First());
					expression->operand=input.Second();
					expression->type=BasicUnaryExpression::PrefixIncrease;
					return expression;
				}
				else if(op==L"--")
				{
					Ptr<BasicUnaryExpression> expression=CreateNode<BasicUnaryExpression>(input.First());
					expression->operand=input.Second();
					expression->type=BasicUnaryExpression::PrefixDecrease;
					return expression;
				}
				else if(op==L"&")
				{
					Ptr<BasicUnaryExpression> expression=CreateNode<BasicUnaryExpression>(input.First());
					expression->operand=input.Second();
					expression->type=BasicUnaryExpression::GetAddress;
					return expression;
				}
				else if(op==L"*")
				{
					Ptr<BasicUnaryExpression> expression=CreateNode<BasicUnaryExpression>(input.First());
					expression->operand=input.Second();
					expression->type=BasicUnaryExpression::DereferencePointer;
					return expression;
				}
				else if(op==L"-")
				{
					Ptr<BasicUnaryExpression> expression=CreateNode<BasicUnaryExpression>(input.First());
					expression->operand=input.Second();
					expression->type=BasicUnaryExpression::Negative;
					return expression;
				}
				else if(op==L"~")
				{
					Ptr<BasicUnaryExpression> expression=CreateNode<BasicUnaryExpression>(input.First());
					expression->operand=input.Second();
					expression->type=BasicUnaryExpression::BitNot;
					return expression;
				}
				else if(op==L"!")
				{
					Ptr<BasicUnaryExpression> expression=CreateNode<BasicUnaryExpression>(input.First());
					expression->operand=input.Second();
					expression->type=BasicUnaryExpression::Not;
					return expression;
				}
				else
				{
					CHECK_ERROR(false, L"language_nativex::ToPreUnary()#错误的操作符。");
				}
			}

			Ptr<BasicExpression> ToBinary(const Ptr<BasicExpression>& left, const ParsingPair<RegexToken, Ptr<BasicExpression>>& binary)
			{
				WString op(binary.First().reading, binary.First().length);
				Ptr<BasicBinaryExpression> expression=CreateNode<BasicBinaryExpression>(binary.First());
				expression->leftOperand=left;
				expression->rightOperand=binary.Second();
				if(op==L"+")
				{
					expression->type=BasicBinaryExpression::Add;
				}
				else if(op==L"-")
				{
					expression->type=BasicBinaryExpression::Sub;
				}
				else if(op==L"*")
				{
					expression->type=BasicBinaryExpression::Mul;
				}
				else if(op==L"/")
				{
					expression->type=BasicBinaryExpression::Div;
				}
				else if(op==L"%")
				{
					expression->type=BasicBinaryExpression::Mod;
				}
				else if(op==L"<")
				{
					expression->type=BasicBinaryExpression::Lt;
				}
				else if(op==L">")
				{
					expression->type=BasicBinaryExpression::Gt;
				}
				else if(op==L"<=")
				{
					expression->type=BasicBinaryExpression::Le;
				}
				else if(op==L">=")
				{
					expression->type=BasicBinaryExpression::Ge;
				}
				else if(op==L"==")
				{
					expression->type=BasicBinaryExpression::Eq;
				}
				else if(op==L"!=")
				{
					expression->type=BasicBinaryExpression::Ne;
				}
				else if(op==L"&")
				{
					expression->type=BasicBinaryExpression::BitAnd;
				}
				else if(op==L"|")
				{
					expression->type=BasicBinaryExpression::BitOr;
				}
				else if(op==L"^")
				{
					expression->type=BasicBinaryExpression::Xor;
				}
				else if(op==L"&&")
				{
					expression->type=BasicBinaryExpression::And;
				}
				else if(op==L"||")
				{
					expression->type=BasicBinaryExpression::Or;
				}
				else if(op==L"+=")
				{
					expression->type=BasicBinaryExpression::AddAssign;
				}
				else if(op==L"-=")
				{
					expression->type=BasicBinaryExpression::SubAssign;
				}
				else if(op==L"*=")
				{
					expression->type=BasicBinaryExpression::MulAssign;
				}
				else if(op==L"/=")
				{
					expression->type=BasicBinaryExpression::DivAssign;
				}
				else if(op==L"%=")
				{
					expression->type=BasicBinaryExpression::ModAssign;
				}
				else if(op==L"<<=")
				{
					expression->type=BasicBinaryExpression::ShlAssign;
				}
				else if(op==L">>=")
				{
					expression->type=BasicBinaryExpression::ShrAssign;
				}
				else if(op==L"&=")
				{
					expression->type=BasicBinaryExpression::BitAndAssign;
				}
				else if(op==L"|=")
				{
					expression->type=BasicBinaryExpression::BitOrAssign;
				}
				else if(op==L"^=")
				{
					expression->type=BasicBinaryExpression::XorAssign;
				}
				else if(op==L"&&=")
				{
					expression->type=BasicBinaryExpression::AndAssign;
				}
				else if(op==L"||=")
				{
					expression->type=BasicBinaryExpression::OrAssign;
				}
				else if(op==L"=")
				{
					expression->type=BasicBinaryExpression::Assign;
				}
				else
				{
					CHECK_ERROR(false, L"language_nativex::ToBinary()#错误的操作符。");
				}
				return expression;
			}

			Ptr<BasicExpression> ToBinary2(const Ptr<BasicExpression>& left, const ParsingPair<ParsingPair<RegexToken, RegexToken>, Ptr<BasicExpression>>& binary)
			{
				WString op(binary.First().First().reading, binary.First().First().length);
				Ptr<BasicBinaryExpression> expression=CreateNode<BasicBinaryExpression>(binary.First().First());
				expression->leftOperand=left;
				expression->rightOperand=binary.Second();
				if(op==L"<")
				{
					expression->type=BasicBinaryExpression::Shl;
				}
				else if(op==L">")
				{
					expression->type=BasicBinaryExpression::Shr;
				}
				else
				{
					CHECK_ERROR(false, L"language_nativex::ToBinary()#错误的操作符。");
				}
				if(binary.First().First().start+1!=binary.First().Second().start)
				{
					WString message=NativeXErrorMessage::OperatorShouldNotBe(op+op, op+L" "+op);
					throw CombinatorResultError<Ptr<BasicExpression>>(message, ParsingResult<Ptr<BasicExpression>>(expression));
				}
				else
				{
					return expression;
				}
			}

			Ptr<BasicExpression> ToCastExpression(const ParsingPair<ParsingPair<RegexToken, Ptr<BasicType>>, Ptr<BasicExpression>>& input)
			{
				Ptr<BasicCastingExpression> expression=CreateNode<BasicCastingExpression>(input.First().First());
				expression->operand=input.Second();
				expression->type=input.First().Second();
				return expression;
			}

/***********************************************************************
语义函数：类型
***********************************************************************/

			Ptr<BasicType> ToNamedType(const RegexToken& input)
			{
				WString name(input.reading, input.length);
				if(name==L"int")
				{
					Ptr<BasicPrimitiveType> type=CreateNode<BasicPrimitiveType>(input);
					type->type=int_type;
					return type;
				}
				else if(name==L"int8")
				{
					Ptr<BasicPrimitiveType> type=CreateNode<BasicPrimitiveType>(input);
					type->type=s8;
					return type;
				}
				else if(name==L"int16")
				{
					Ptr<BasicPrimitiveType> type=CreateNode<BasicPrimitiveType>(input);
					type->type=s16;
					return type;
				}
				else if(name==L"int32")
				{
					Ptr<BasicPrimitiveType> type=CreateNode<BasicPrimitiveType>(input);
					type->type=s32;
					return type;
				}
				else if(name==L"int64")
				{
					Ptr<BasicPrimitiveType> type=CreateNode<BasicPrimitiveType>(input);
					type->type=s64;
					return type;
				}
				else if(name==L"uint")
				{
					Ptr<BasicPrimitiveType> type=CreateNode<BasicPrimitiveType>(input);
					type->type=uint_type;
					return type;
				}
				else if(name==L"uint8")
				{
					Ptr<BasicPrimitiveType> type=CreateNode<BasicPrimitiveType>(input);
					type->type=u8;
					return type;
				}
				else if(name==L"uint16")
				{
					Ptr<BasicPrimitiveType> type=CreateNode<BasicPrimitiveType>(input);
					type->type=u16;
					return type;
				}
				else if(name==L"uint32")
				{
					Ptr<BasicPrimitiveType> type=CreateNode<BasicPrimitiveType>(input);
					type->type=u32;
					return type;
				}
				else if(name==L"uint64")
				{
					Ptr<BasicPrimitiveType> type=CreateNode<BasicPrimitiveType>(input);
					type->type=u64;
					return type;
				}
				else if(name==L"f32")
				{
					Ptr<BasicPrimitiveType> type=CreateNode<BasicPrimitiveType>(input);
					type->type=f32;
					return type;
				}
				else if(name==L"f64")
				{
					Ptr<BasicPrimitiveType> type=CreateNode<BasicPrimitiveType>(input);
					type->type=f64;
					return type;
				}
				else if(name==L"char")
				{
					Ptr<BasicPrimitiveType> type=CreateNode<BasicPrimitiveType>(input);
					type->type=char_type;
					return type;
				}
				else if(name==L"wchar")
				{
					Ptr<BasicPrimitiveType> type=CreateNode<BasicPrimitiveType>(input);
					type->type=wchar_type;
					return type;
				}
				else if(name==L"bool")
				{
					Ptr<BasicPrimitiveType> type=CreateNode<BasicPrimitiveType>(input);
					type->type=bool_type;
					return type;
				}
				else if(name==L"void")
				{
					Ptr<BasicPrimitiveType> type=CreateNode<BasicPrimitiveType>(input);
					type->type=void_type;
					return type;
				}
				else
				{
					Ptr<BasicReferenceType> type=CreateNode<BasicReferenceType>(input);
					type->name=ConvertID(name);
					return type;
				}
			}

			Ptr<BasicType> ToDecoratedType(const Ptr<BasicType>& operand, const RegexToken& decoration)
			{
				if(*decoration.reading==L'*')
				{
					Ptr<BasicPointerType> type=CreateNode<BasicPointerType>(decoration);
					type->elementType=operand;
					return type;
				}
				else
				{
					Ptr<BasicArrayType> type=CreateNode<BasicArrayType>(decoration);
					type->size=wtoi(WString(decoration.reading, decoration.length));
					type->elementType=operand;
					return type;
				}
			}

			Ptr<BasicType> ToFunctionType(const ParsingPair<ParsingPair<RegexToken, Ptr<BasicType>>, Ptr<List<Ptr<BasicType>>>>& input)
			{
				Ptr<BasicFunctionType> type=CreateNode<BasicFunctionType>(input.First().First());
				type->returnType=input.First().Second();
				CopyFrom(type->parameterTypes.Wrap(), input.Second()->Wrap());
				return type;
			}

			Ptr<BasicType> ToInstanciatedGenericType(const ParsingPair<Ptr<BasicType>, ParsingPair<RegexToken, Ptr<List<Ptr<BasicType>>>>>& input)
			{
				Ptr<BasicInstanciatedGenericType> type=CreateNode<BasicInstanciatedGenericType>(input.Second().First());
				type->elementType=input.First();
				CopyFrom(type->argumentTypes.Wrap(), input.Second().Second()->Wrap());
				return type;
			}

/***********************************************************************
语义函数：语句
***********************************************************************/

			Ptr<BasicStatement> ToEmptyStat(const RegexToken& input)
			{
				return CreateNode<BasicEmptyStatement>(input);
			}

			Ptr<BasicStatement> ToExprStat(const ParsingPair<Ptr<BasicExpression>, RegexToken>& input)
			{
				Ptr<BasicExpressionStatement> statement=CreateNode<BasicExpressionStatement>(input.Second());
				statement->expression=input.First();
				return statement;
			}

			Ptr<BasicStatement> ToVarStat(const ParsingPair<ParsingPair<ParsingPair<RegexToken, Ptr<BasicType>>, RegexToken>, ParsingList<Ptr<BasicExpression>>>& input)
			{
				Ptr<BasicVariableStatement> statement=CreateNode<BasicVariableStatement>(input.First().First().First());
				statement->name=ConvertID(WString(input.First().Second().reading, input.First().Second().length));
				statement->type=input.First().First().Second();
				if(input.Second().Head())
				{
					statement->initializer=input.Second().Head()->Value();
				}
				return statement;
			}

			Ptr<BasicStatement> ToIfStat(const ParsingPair<ParsingPair<ParsingPair<RegexToken, Ptr<BasicExpression>>, Ptr<BasicStatement>>, ParsingList<Ptr<BasicStatement>>>& input)
			{
				Ptr<BasicIfStatement> statement=CreateNode<BasicIfStatement>(input.First().First().First());
				statement->condition=input.First().First().Second();
				statement->trueStatement=input.First().Second();
				if(input.Second().Head())
				{
					statement->falseStatement=input.Second().Head()->Value();
				}
				return statement;
			}

			Ptr<BasicStatement> ToBreakStat(const RegexToken& input)
			{
				return CreateNode<BasicBreakStatement>(input);
			}

			Ptr<BasicStatement> ToContinueStat(const RegexToken& input)
			{
				return CreateNode<BasicContinueStatement>(input);
			}

			Ptr<BasicStatement> ToReturnStat(const RegexToken& input)
			{
				return CreateNode<BasicReturnStatement>(input);
			}

			Ptr<BasicStatement> ToCompositeStat(const ParsingPair<RegexToken, Ptr<List<Ptr<BasicStatement>>>>& input)
			{
				Ptr<BasicCompositeStatement> statement=CreateNode<BasicCompositeStatement>(input.First());
				CopyFrom(statement->statements.Wrap(), input.Second()->Wrap());
				return statement;
			}

			Ptr<BasicStatement> ToDoWhileStat(const ParsingPair<ParsingPair<RegexToken, Ptr<BasicStatement>>, Ptr<BasicExpression>>& input)
			{
				Ptr<BasicWhileStatement> statement=CreateNode<BasicWhileStatement>(input.First().First());
				statement->statement=input.First().Second();
				statement->endCondition=input.Second();
				return statement;
			}

			Ptr<BasicStatement> ToLoopStat(const ParsingPair<RegexToken, Ptr<BasicStatement>>& input)
			{
				Ptr<BasicWhileStatement> statement=CreateNode<BasicWhileStatement>(input.First());
				statement->statement=input.Second();
				return statement;
			}

			Ptr<BasicStatement> ToWhileStat(const ParsingPair<ParsingPair<ParsingPair<RegexToken, Ptr<BasicExpression>>, Ptr<BasicStatement>>, ParsingList<Ptr<BasicExpression>>>& input)
			{
				Ptr<BasicWhileStatement> statement=CreateNode<BasicWhileStatement>(input.First().First().First());
				statement->beginCondition=input.First().First().Second();
				statement->statement=input.First().Second();
				if(input.Second().Head())
				{
					statement->endCondition=input.Second().Head()->Value();
				}
				return statement;
			}

			Ptr<BasicStatement> ConvertStats(Ptr<List<Ptr<BasicStatement>>> statements, const RegexToken& token)
			{
				if(statements->Count()==1)
				{
					return statements->Get(0);
				}
				else
				{
					Ptr<BasicCompositeStatement> composite=CreateNode<BasicCompositeStatement>(token);
					CopyFrom(composite->statements.Wrap(), statements->Wrap());
					return composite;
				}
			}

			Ptr<BasicStatement> ToForStat(const ParsingPair<ParsingPair<ParsingPair<ParsingPair<
				RegexToken,
				Ptr<List<Ptr<BasicStatement>>>>,
				Ptr<BasicExpression>>,
				Ptr<List<Ptr<BasicStatement>>>>,
				Ptr<BasicStatement>>& input)
			{
				const RegexToken& token=input.First().First().First().First();
				Ptr<BasicForStatement> statement=CreateNode<BasicForStatement>(token);
				statement->initializer=ConvertStats(input.First().First().First().Second(), token);
				statement->condition=input.First().First().Second();
				statement->sideEffect=ConvertStats(input.First().Second(), token);
				statement->statement=input.Second();
				return statement;
			}

/***********************************************************************
语义函数：声明
***********************************************************************/

			void AssignBasicLinking(BasicLinking& linking, ParsingList<BasicLinking> input)
			{
				if(input.Head())
				{
					linking=input.Head()->Value();
				}
			}

			BasicLinking ToLinking(const ParsingPair<RegexToken, RegexToken>& input)
			{
				BasicLinking linking;
				linking.assemblyName=ConvertID(WString(input.First().reading, input.First().length));
				linking.symbolName=ConvertID(WString(input.Second().reading, input.Second().length));
				return linking;
			}

			Ptr<BasicDeclaration> ToGeneric(const ParsingPair<ParsingPair<ParsingList<RegexToken>, ParsingList<ParsingList<ParsingPair<RegexToken, RegexToken>>>>, Ptr<BasicDeclaration>>& input)
			{
				BasicGeneric& genericDeclaration=input.Second()->genericDeclaration;
				{
					ParsingList<RegexToken>::Node::Ref current=input.First().First().Head();
					while(current)
					{
						const RegexToken& token=current->Value();
						genericDeclaration.arguments.Add(WString(token.reading, token.length));
						current=current->Next();
					}
				}
				if(input.First().Second().Head())
				{
					ParsingList<ParsingPair<RegexToken, RegexToken>>::Node::Ref current=input.First().Second().Head()->Value().Head();
					while(current)
					{
						Ptr<BasicGeneric::Constraint> constraint=new BasicGeneric::Constraint;
						constraint->argumentName=WString(current->Value().First().reading, current->Value().First().length);
						constraint->conceptName=WString(current->Value().Second().reading, current->Value().Second().length);
						genericDeclaration.constraints.Add(constraint);
						current=current->Next();
					}
				}
				return input.Second();
			}

			Ptr<BasicDeclaration> ToVarDecl(const ParsingPair<ParsingPair<ParsingPair<ParsingPair<RegexToken, Ptr<BasicType>>, RegexToken>, ParsingList<BasicLinking>>, ParsingList<Ptr<BasicExpression>>>& input)
			{
				Ptr<BasicVariableDeclaration> declaration=CreateNode<BasicVariableDeclaration>(input.First().First().First().First());
				declaration->name=ConvertID(WString(input.First().First().Second().reading, input.First().First().Second().length));
				declaration->type=input.First().First().First().Second();
				AssignBasicLinking(declaration->linking, input.First().Second());
				if(input.Second().Head())
				{
					declaration->initializer=input.Second().Head()->Value();
				}
				return declaration;
			}

			Ptr<BasicDeclaration> ToTypedefDecl(const ParsingPair<ParsingPair<RegexToken, RegexToken>, Ptr<BasicType>>& input)
			{
				Ptr<BasicTypeRenameDeclaration> declaration=CreateNode<BasicTypeRenameDeclaration>(input.First().First());
				declaration->name=ConvertID(WString(input.First().Second().reading, input.First().Second().length));
				declaration->type=input.Second();
				return declaration;
			}

			Ptr<BasicDeclaration> ToFuncDecl(const ParsingPair<ParsingPair<ParsingPair<ParsingPair<ParsingPair<
				RegexToken, 
				Ptr<BasicType>>, 
				RegexToken>, 
				ParsingList<ParsingPair<Ptr<BasicType>, RegexToken>>>,
				ParsingList<BasicLinking>>,
				ParsingList<Ptr<BasicStatement>>>& input)
			{
				Ptr<BasicFunctionDeclaration> declaration=CreateNode<BasicFunctionDeclaration>(input.First().First().First().First().First());
				declaration->name=ConvertID(WString(input.First().First().First().Second().reading, input.First().First().First().Second().length));
				if(input.Second().Head())
				{
					declaration->statement=input.Second().Head()->Value();
				}
				declaration->signatureType=CreateNode<BasicFunctionType>(input.First().First().First().First().First());
				declaration->signatureType->returnType=input.First().First().First().First().Second();
				AssignBasicLinking(declaration->linking, input.First().Second());
				
				ParsingList<ParsingPair<Ptr<BasicType>, RegexToken>>::Node::Ref current=input.First().First().Second().Head();
				while(current)
				{
					declaration->signatureType->parameterTypes.Add(current->Value().First());
					declaration->parameterNames.Add(ConvertID(WString(current->Value().Second().reading, current->Value().Second().length)));
					current=current->Next();
				}
				return declaration;
			}

			Ptr<BasicDeclaration> ToStructDecl(const ParsingPair<ParsingPair<ParsingPair<
				RegexToken,
				RegexToken>,
				ParsingList<BasicLinking>>,
				ParsingList<ParsingPair<Ptr<BasicType>, RegexToken>>>& input)
			{
				Ptr<BasicStructureDeclaration> declaration=CreateNode<BasicStructureDeclaration>(input.First().First().First());
				declaration->defined=true;
				declaration->name=ConvertID(WString(input.First().First().Second().reading, input.First().First().Second().length));
				ParsingList<ParsingPair<Ptr<BasicType>, RegexToken>>::Node::Ref current=input.Second().Head();
				AssignBasicLinking(declaration->linking, input.First().Second());
				while(current)
				{
					declaration->memberTypes.Add(current->Value().First());
					declaration->memberNames.Add(ConvertID(WString(current->Value().Second().reading, current->Value().Second().length)));
					current=current->Next();
				}
				return declaration;
			}

			Ptr<BasicDeclaration> ToStructPreDecl(const ParsingPair<RegexToken, RegexToken>& input)
			{
				Ptr<BasicStructureDeclaration> declaration=CreateNode<BasicStructureDeclaration>(input.First());
				declaration->defined=false;
				declaration->name=ConvertID(WString(input.Second().reading, input.Second().length));
				return declaration;
			}

			Ptr<BasicConceptBaseDeclaration::FunctionConcept> ToFunctionConcept(const ParsingPair<RegexToken, Ptr<BasicType>>& input)
			{
				Ptr<BasicConceptBaseDeclaration::FunctionConcept> functionConcept=new BasicConceptBaseDeclaration::FunctionConcept;
				functionConcept->name=ConvertID(WString(input.First().reading, input.First().length));
				functionConcept->signatureType=input.Second().Cast<BasicFunctionType>();
				return functionConcept;
			}

			Ptr<BasicDeclaration> ToConceptDecl(const ParsingPair<ParsingPair<RegexToken, RegexToken>, ParsingList<Ptr<BasicConceptBaseDeclaration::FunctionConcept>>>& input)
			{
				Ptr<BasicConceptBaseDeclaration> declaration=CreateNode<BasicConceptBaseDeclaration>(input.First().First());
				declaration->conceptType=ConvertID(WString(input.First().First().reading, input.First().First().length));
				declaration->name=ConvertID(WString(input.First().Second().reading, input.First().Second().length));
				CopyFrom(declaration->functions.Wrap(), input.Second());
				return declaration;
			}

			Ptr<BasicConceptInstanceDeclaration::FunctionInstance> ToFunctionInstance(const ParsingPair<RegexToken, Ptr<BasicExpression>>& input)
			{
				Ptr<BasicConceptInstanceDeclaration::FunctionInstance> functionInstance=new BasicConceptInstanceDeclaration::FunctionInstance;
				functionInstance->name=ConvertID(WString(input.First().reading, input.First().length));
				functionInstance->normalFunction=input.Second().Cast<BasicReferenceExpression>();
				functionInstance->genericFunction=input.Second().Cast<BasicInstanciatedExpression>();
				return functionInstance;
			}

			Ptr<BasicDeclaration> ToInstancePreDecl(const ParsingPair<Ptr<BasicType>, RegexToken>& input)
			{
				Ptr<BasicConceptInstanceDeclaration> declaration=CreateNode<BasicConceptInstanceDeclaration>(input.Second());
				declaration->defined=false;
				declaration->instanceType=input.First();
				declaration->name=ConvertID(WString(input.Second().reading, input.Second().length));
				return declaration;
			}

			Ptr<BasicDeclaration> ToInstanceDecl(const ParsingPair<ParsingPair<Ptr<BasicType>, RegexToken>, ParsingList<Ptr<BasicConceptInstanceDeclaration::FunctionInstance>>>& input)
			{
				Ptr<BasicConceptInstanceDeclaration> declaration=CreateNode<BasicConceptInstanceDeclaration>(input.First().Second());
				declaration->defined=true;
				declaration->instanceType=input.First().First();
				declaration->name=ConvertID(WString(input.First().Second().reading, input.First().Second().length));
				CopyFrom(declaration->functions.Wrap(), input.Second());
				return declaration;
			}

/***********************************************************************
语义函数：主程序
***********************************************************************/

			Ptr<NativeXUnit> ToUnit(const ParsingPair<ParsingPair<
				RegexToken,
				Ptr<List<RegexToken>>>,
				Ptr<List<Ptr<BasicDeclaration>>>>& input)
			{
				Ptr<NativeXUnit> unit=new NativeXUnit;
				unit->name=ConvertID(WString(input.First().First().reading, input.First().First().length));
				unit->program=CreateNode<BasicProgram>(input.First().First());
				for(vint i=0;i<input.First().Second()->Count();i++)
				{
					const RegexToken& name=input.First().Second()->Get(i);
					unit->imports.Add(ConvertID(WString(name.reading, name.length)));
				}
				CopyFrom(unit->program->declarations.Wrap(), input.Second()->Wrap());
				return unit;
			}

/***********************************************************************
错误恢复
***********************************************************************/

#define ERROR_HANDLER(NAME, TYPE)																						\
			ParsingResult<TYPE> NAME(TokenInput<RegexToken>& input, Types<TokenInput<RegexToken>>::GlobalInfo& info)	\
			{																											\
				info.errors.Clear();																					\
				info.errors.Add(new CombinatorError<TokenInput<RegexToken>>(NativeXErrorMessage::NAME(), input));		\
				return ParsingResult<TYPE>();																			\
			}

			ERROR_HANDLER(NeedExpression,		Ptr<BasicExpression>)
			ERROR_HANDLER(NeedType,				RegexToken)
			ERROR_HANDLER(NeedStatement,		RegexToken)
			ERROR_HANDLER(NeedDeclaration,		Ptr<BasicDeclaration>)
			ERROR_HANDLER(NeedID,				RegexToken)
			ERROR_HANDLER(NeedTypeExpression,	Ptr<BasicType>)

			ERROR_HANDLER(NeedLt,				RegexToken)
			ERROR_HANDLER(NeedGt,				RegexToken)
			ERROR_HANDLER(NeedOpenBrace,		RegexToken)
			ERROR_HANDLER(NeedCloseBrace,		RegexToken)
			ERROR_HANDLER(NeedCloseArray,		RegexToken)
			ERROR_HANDLER(NeedComma,			RegexToken)
			ERROR_HANDLER(NeedSemicolon,		RegexToken)
			ERROR_HANDLER(NeedColon,			RegexToken)
			ERROR_HANDLER(NeedCloseStat,		RegexToken)
			ERROR_HANDLER(NeedAssign,			RegexToken)
			ERROR_HANDLER(NeedOpenStruct,		RegexToken)
			ERROR_HANDLER(NeedCloseStruct,		RegexToken)
			ERROR_HANDLER(NeedOpenConcept,		RegexToken)
			ERROR_HANDLER(NeedCloseConcept,		RegexToken)

			ERROR_HANDLER(NeedWhile,			RegexToken)
			ERROR_HANDLER(NeedWhen,				RegexToken)
			ERROR_HANDLER(NeedWith,				RegexToken)
			ERROR_HANDLER(NeedDo,				RegexToken)
			ERROR_HANDLER(NeedUnit,				RegexToken)

#undef ERROR_HANDLER

/***********************************************************************
语法分析器
***********************************************************************/

			class NativeXParser : public Object
			{
			protected:
				Ptr<RegexLexer>						lexer;
				vint								blankID;

				TokenType							ACHAR;
				TokenType							WCHAR;
				TokenType							ASTRING;
				TokenType							WSTRING;
				TokenType							INTEGER;
				TokenType							FLOAT;
				TokenType							DOUBLE;
				TokenType							ID;
				TokenType							PRIM_TYPE;

				TokenType							TRUE, FALSE, NULL_VALUE, RESULT, FUNCTION, CAST, VARIABLE;
				TokenType							IF, ELSE, BREAK, CONTINUE, EXIT, WHILE, DO, LOOP, WHEN, FOR, WITH;
				TokenType							TYPE, STRUCTURE, UNIT, USES, ALIAS, GENERIC, CONCEPT, INSTANCE, WHERE;

				TokenType							OPEN_ARRAY;
				TokenType							CLOSE_ARRAY;
				TokenType							OPEN_BRACE;
				TokenType							CLOSE_BRACE;
				TokenType							OPEN_STAT;
				TokenType							CLOSE_STAT;
				TokenType							DOT;
				TokenType							POINTER;
				TokenType							COMMA;
				TokenType							SEMICOLON;
				TokenType							COLON;

				TokenType							INCREASE, DECREASE, BIT_NOT, NOT;
				TokenType							ADD, SUB, MUL, DIV, MOD;
				TokenType							LT, GT, LE, GE, EQ, NE;
				TokenType							BIT_AND, BIT_OR, AND, OR, XOR;
				TokenType							OP_ASSIGN, ASSIGN;

				LinkingNode							linking;
				ExpressionRule						primitive;
				ExpressionRule						reference;
				ExpressionRule						exp0, exp1, exp2, exp3, exp4, exp5, exp6, exp7, exp8, exp9, exp10, exp11, exp12;
				ExpressionRule						exp;
				TypeRule							primType, functionType, type, instanceType;
				StatementRule						statement;
				DeclarationNode						nonGenericDeclaration;
				DeclarationRule						declaration;
				UnitRule							unit;
			public:
				NativeXParser()
				{
					List<WString> tokens;
					tokens.Add(L"/s+");
										
					TRUE			= CreateToken(tokens, L"true");
					FALSE			= CreateToken(tokens, L"false");
					NULL_VALUE		= CreateToken(tokens, L"null");
					RESULT			= CreateToken(tokens, L"result");
					FUNCTION		= CreateToken(tokens, L"function");
					CAST			= CreateToken(tokens, L"cast");
					VARIABLE		= CreateToken(tokens, L"variable");
					IF				= CreateToken(tokens, L"if");
					ELSE			= CreateToken(tokens, L"else");
					BREAK			= CreateToken(tokens, L"break");
					CONTINUE		= CreateToken(tokens, L"continue");
					EXIT			= CreateToken(tokens, L"exit");
					WHILE			= CreateToken(tokens, L"while");
					DO				= CreateToken(tokens, L"do");
					LOOP			= CreateToken(tokens, L"loop");
					WHEN			= CreateToken(tokens, L"when");
					FOR				= CreateToken(tokens, L"for");
					WITH			= CreateToken(tokens, L"with");
					TYPE			= CreateToken(tokens, L"type");
					STRUCTURE		= CreateToken(tokens, L"structure");
					UNIT			= CreateToken(tokens, L"unit");
					USES			= CreateToken(tokens, L"uses");
					ALIAS			= CreateToken(tokens, L"alias");
					GENERIC			= CreateToken(tokens, L"generic");
					CONCEPT			= CreateToken(tokens, L"concept");
					INSTANCE		= CreateToken(tokens, L"instance");
					WHERE			= CreateToken(tokens, L"where");

					PRIM_TYPE		= CreateToken(tokens, L"int|int8|int16|int32|int64|uint|uint8|uint16|uint32|uint64|f32|f64|bool|char|wchar|void");
					ACHAR			= CreateToken(tokens, L"\'([^\']|\\\\\\.)\'");
					WCHAR			= CreateToken(tokens, L"L\'([^\']|\\\\\\.)\'");
					ASTRING			= CreateToken(tokens, L"\"([^\"]|\\\\\\.)*\"");
					WSTRING			= CreateToken(tokens, L"L\"([^\"]|\\\\\\.)*\"");
					INTEGER			= CreateToken(tokens, L"[+/-]?/d+([su](8|16|32|64))?");
					FLOAT			= CreateToken(tokens, L"[+/-]?/d+(./d+)[fF]");
					DOUBLE			= CreateToken(tokens, L"[+/-]?/d+./d+");
					ID				= CreateToken(tokens, L"(@?[a-zA-Z_]/w*)|(@\"([^\"]|\\\\\\.)*\")");

					OPEN_ARRAY		= CreateToken(tokens, L"/[");
					CLOSE_ARRAY		= CreateToken(tokens, L"/]");
					OPEN_BRACE		= CreateToken(tokens, L"/(");
					CLOSE_BRACE		= CreateToken(tokens, L"/)");
					OPEN_STAT		= CreateToken(tokens, L"/{");
					CLOSE_STAT		= CreateToken(tokens, L"/}");
					DOT				= CreateToken(tokens, L".");
					POINTER			= CreateToken(tokens, L"->");
					COMMA			= CreateToken(tokens, L",");
					SEMICOLON		= CreateToken(tokens, L";");
					COLON			= CreateToken(tokens, L":");

					INCREASE		= CreateToken(tokens, L"/+/+");
					DECREASE		= CreateToken(tokens, L"--");
					BIT_NOT			= CreateToken(tokens, L"~");
					NOT				= CreateToken(tokens, L"!");
					ADD				= CreateToken(tokens, L"/+");
					SUB				= CreateToken(tokens, L"-");
					MUL				= CreateToken(tokens, L"/*");
					DIV				= CreateToken(tokens, L"//");
					MOD				= CreateToken(tokens, L"%");
					LT				= CreateToken(tokens, L"<");
					GT				= CreateToken(tokens, L">");
					LE				= CreateToken(tokens, L"<=");
					GE				= CreateToken(tokens, L">=");
					EQ				= CreateToken(tokens, L"==");
					NE				= CreateToken(tokens, L"!=");
					BIT_AND			= CreateToken(tokens, L"&");
					BIT_OR			= CreateToken(tokens, L"/|");
					AND				= CreateToken(tokens, L"&&");
					OR				= CreateToken(tokens, L"/|/|");
					XOR				= CreateToken(tokens, L"/^");
					OP_ASSIGN		= CreateToken(tokens, L"(/+|-|/*|//|%|<<|>>|&|/||/^|&&|/|/|)=");
					ASSIGN			= CreateToken(tokens, L"=");

					lexer=new RegexLexer(tokens.Wrap());

					linking			= (ALIAS >> (ID + (DOT >> ID))[ToLinking]);

					primitive		= TRUE[ToTrue] | FALSE[ToFalse]
									| ACHAR[ToAChar] | WCHAR[ToWChar]
									| ASTRING[ToAString] | WSTRING[ToWString]
									| FLOAT[ToFloat] | DOUBLE[ToDouble]
									| NULL_VALUE[ToNull]
									| INTEGER[ToInteger]
									;

					reference		= (ID + (LT(NeedLt) >> type(NeedTypeExpression) << GT(NeedGt)) + (COLON(NeedColon) + COLON(NeedColon) >> ID(NeedID)))[ToBasicInstanceFunctionExpression]
									| (ID + (LT(NeedLt) + list(opt(type + *(COMMA >> type))) << GT(NeedGt)))[ToInstanciatedExpression]
									| ID[ToReference]
									;

					exp0			= primitive(NeedExpression)
									| reference
									| RESULT[ToResult]
									| (CAST + (LT(NeedLt) >> type << GT(NeedGt)) + (OPEN_BRACE(NeedOpenBrace) >> exp << CLOSE_BRACE(NeedCloseBrace)))[ToCastExpression]
									| (OPEN_BRACE >> exp << CLOSE_BRACE(NeedCloseBrace))
									;

					exp1			= lrec(exp0 +  *(
													(OPEN_ARRAY + exp << CLOSE_ARRAY(NeedCloseArray))
													| (OPEN_BRACE + list(opt(exp + *(COMMA >> exp)))[UpgradeArguments] << CLOSE_BRACE(NeedCloseBrace))
													| ((DOT | POINTER) + reference)
													| ((INCREASE | DECREASE)[UpgradePostfix])
													), ToPostUnary);
					exp2			= exp1 | ((INCREASE | DECREASE | BIT_AND | MUL | SUB | BIT_NOT | NOT) + exp1)[ToPreUnary];
					exp3			= lrec(exp2 + *((MUL | DIV | MOD) + exp2), ToBinary);
					exp4			= lrec(exp3 + *((ADD | SUB) + exp3), ToBinary);
					exp5			= lrec(exp4 + *((LT+LT | GT+GT) + exp4), ToBinary2);
					exp6			= lrec(exp5 + *((LT | GT | LE | GE) + exp5), ToBinary);
					exp7			= lrec(exp6 + *((EQ | NE) + exp6), ToBinary);
					exp8			= lrec(exp7 + *(BIT_AND + exp7), ToBinary);
					exp9			= lrec(exp8 + *(XOR + exp8), ToBinary);
					exp10			= lrec(exp9 + *(BIT_OR + exp9), ToBinary);
					exp11			= lrec(exp10 + *(AND + exp10), ToBinary);
					exp12			= lrec(exp11 + *(OR + exp11), ToBinary);
					exp				= lrec(exp12 + *((OP_ASSIGN | ASSIGN) + exp12), ToBinary);

					functionType	= (FUNCTION(NeedType) + type + (OPEN_BRACE(NeedOpenBrace) >> list(opt(type + *(COMMA >> type))) << CLOSE_BRACE(NeedCloseBrace)))[ToFunctionType];
					primType		= functionType
									| ((PRIM_TYPE | ID)[ToNamedType] + (LT(NeedLt) + list(opt(type + *(COMMA >> type))) << GT(NeedGt)))[ToInstanciatedGenericType]
									| (PRIM_TYPE | ID)[ToNamedType]
									;

					type			= lrec(primType + *(MUL | (OPEN_ARRAY >> INTEGER << CLOSE_ARRAY)), ToDecoratedType);

					statement		= SEMICOLON(NeedStatement)[ToEmptyStat]
									| (exp + SEMICOLON(NeedSemicolon))[ToExprStat]
									| (VARIABLE + type + ID(NeedID) + opt(ASSIGN >> exp) << SEMICOLON(NeedSemicolon))[ToVarStat]
									| (IF + (OPEN_BRACE(NeedOpenBrace) >> exp << CLOSE_BRACE(NeedCloseBrace)) + statement + opt(ELSE >> statement))[ToIfStat]
									| (BREAK << SEMICOLON(NeedSemicolon))[ToBreakStat]
									| (CONTINUE << SEMICOLON(NeedSemicolon))[ToContinueStat]
									| (EXIT << SEMICOLON(NeedSemicolon))[ToReturnStat]
									| (OPEN_STAT + list(*statement) << CLOSE_STAT(NeedCloseStat))[ToCompositeStat]
									| (DO + statement + (WHILE(NeedWhile) >> OPEN_BRACE(NeedOpenBrace) >> exp << CLOSE_BRACE(NeedCloseBrace) << SEMICOLON(NeedSemicolon)))[ToDoWhileStat]
									| (LOOP + statement)[ToLoopStat]
									| (WHILE + (OPEN_BRACE(NeedOpenBrace) >> exp << CLOSE_BRACE(NeedCloseBrace)) + statement + opt(WHEN >> OPEN_BRACE(NeedOpenBrace) >> exp << CLOSE_BRACE(NeedCloseBrace) << SEMICOLON(NeedSemicolon)))[ToWhileStat]
									| (FOR + list(*statement) + (WHEN(NeedWhen) >> OPEN_BRACE(NeedOpenBrace) >> exp << CLOSE_BRACE(NeedCloseBrace)) + (WITH(NeedWith) >> list(*statement)) + (DO(NeedDo) >> statement))[ToForStat]
									;

					instanceType	= (PRIM_TYPE | ID)[ToNamedType]
									;

					nonGenericDeclaration		
									= (VARIABLE + type + ID(NeedID) + opt(linking) + opt(ASSIGN >> exp) << SEMICOLON(NeedSemicolon))[ToVarDecl]
									| (TYPE + ID + (ASSIGN(NeedAssign) >> type) << SEMICOLON(NeedSemicolon))[ToTypedefDecl]
									| (STRUCTURE + ID(NeedID) << SEMICOLON(NeedSemicolon))[ToStructPreDecl]
									| (STRUCTURE + ID(NeedID) + opt(linking) + (OPEN_STAT(NeedOpenStruct) >> *(type + ID(NeedID) << SEMICOLON(NeedSemicolon)) << CLOSE_STAT(NeedCloseStruct)))[ToStructDecl]
									| (FUNCTION + type + ID(NeedID) + (OPEN_BRACE(NeedOpenBrace) >> plist(opt((type + ID(NeedID)) + *(COMMA >> (type + ID(NeedID))))) << CLOSE_BRACE(NeedCloseBrace)) + opt(linking << SEMICOLON(NeedSemicolon)) + opt(statement))[ToFuncDecl]
									| (INSTANCE >> (instanceType + (COLON(NeedColon) >> ID << SEMICOLON(NeedSemicolon))))[ToInstancePreDecl]
									| ((INSTANCE >> (instanceType + (COLON(NeedColon) >> ID)))+(OPEN_STAT(NeedOpenConcept)>>*((ID(NeedID)+(ASSIGN(NeedAssign)>>reference)<<SEMICOLON(NeedSemicolon)))[ToFunctionInstance]<<CLOSE_STAT(NeedCloseConcept)))[ToInstanceDecl]
									;

					Node<TokenInput<RegexToken>, ParsingList<RegexToken>> genericHead
									= (GENERIC>>LT(NeedLt)>>plist(ID(NeedID)+*(COMMA>>ID(NeedID)))<<GT(NeedGt))
									;
					Node<TokenInput<RegexToken>, ParsingPair<RegexToken, RegexToken>> genericConstraintClause
									= ID(NeedID) + (COLON(NeedColon) >> ID(NeedID))
									;
					Node<TokenInput<RegexToken>, ParsingList<ParsingPair<RegexToken, RegexToken>>> genericConstraint
									= WHERE >> (plist(genericConstraintClause + *(COMMA >> genericConstraintClause)))
									;
					declaration		= nonGenericDeclaration
									| (CONCEPT>>ID(NeedID)+(COLON(NeedColon)>>ID(NeedID))+(OPEN_STAT(NeedOpenConcept)>>*((ID(NeedID)+(ASSIGN(NeedAssign)>>functionType)<<SEMICOLON(NeedSemicolon)))[ToFunctionConcept]<<CLOSE_STAT(NeedCloseConcept)))[ToConceptDecl]
									| (genericHead+opt(genericConstraint)+nonGenericDeclaration(NeedDeclaration))[ToGeneric]
									;

					unit			= ((UNIT(NeedUnit) >> ID(NeedID) << SEMICOLON(NeedSemicolon)) + list(opt(USES >> (ID(NeedID) + *(COMMA >> ID(NeedID))) << SEMICOLON(NeedSemicolon))) + list(*declaration))[ToUnit];
				}

				static bool Blank(vint token)
				{
					return token==0;
				}

				Ptr<NativeXUnit> Parse(const WString& code, vint codeIndex, IList<Ptr<LanguageException>>& errors)
				{
					List<RegexToken> tokens;
					lexer->Parse(code, codeIndex).ReadToEnd(tokens, Blank);
					for(vint i=0;i<tokens.Count();i++)
					{
						if(tokens[i].token==-1)
						{
							WString message=NativeXErrorMessage::UnrecognizedToken(WString(tokens[i].reading, tokens[i].length));
							errors.Add(new LanguageException(message, tokens[i].lineIndex, tokens[i].lineStart, tokens[i].start, codeIndex));
							return 0;
						}
					}

					TokenInput<RegexToken> input(&tokens[0], tokens.Count());
					try
					{
						Ptr<NativeXUnit> result=unit.ParseFull(input, false);
						if(result)
						{
							result->codeIndex=codeIndex;
						}
						return result;
					}
					catch(const CombinatorException<TokenInput<RegexToken>>& exception)
					{
						for(vint i=0;i<exception.GetGlobalInfo().errors.Count();i++)
						{
							Ptr<CombinatorError<TokenInput<RegexToken>>> error=exception.GetGlobalInfo().errors.Get(i);
							RegexToken position=error->GetPosition().Current();
							errors.Add(new LanguageException(error->Message(), position.lineIndex, position.lineStart, position.start, codeIndex));
						}
						return 0;
					}
				}
			};

/***********************************************************************
字符串转换辅助函数
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
					, L"type"
					, L"structure"
					, L"unit"
					, L"uses"
					, L"alias"
					, L"generic"
					, L"concept"
					, L"instance"
					, L"where"
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
接口
***********************************************************************/

			class NativeXMessageTranslator : public BasicErrorMessageTranslator
			{
			protected:
				BasicAnalyzer*			analyzer;

				BasicTypeRecord* GetExpressionType(BasicExpression* expression)
				{
					return analyzer->GetEnv()->GetExpressionType(expression);
				}

				WString ToString(BasicType* type)
				{
					BP argument(
						analyzer->GetEnv(),
						analyzer->GetEnv()->GlobalScope(),
						analyzer->GetTypeManager(),
						*(List<Ptr<BasicLanguageCodeException>>*)0,
						*(SortedList<WString>*)0
						);
					return ToString(BasicLanguage_GetTypeRecord(type, argument, true));
				}

				WString ToString(BasicTypeRecord* type)
				{
					switch(type->GetType())
					{
					case BasicTypeRecord::Primitive:
						return PrimitiveTypeToString(type->PrimitiveType());
					case BasicTypeRecord::Pointer:
						return ToString(type->ElementType())+L"*";
					case BasicTypeRecord::Array:
						return ToString(type->ElementType())+L"["+itow(type->ElementCount())+L"]";
					case BasicTypeRecord::Function:
						{
							WString result=L"function "+ToString(type->ReturnType())+L"(";
							for(vint i=0;i<type->ParameterCount();i++)
							{
								if(i)result+=L", ";
								result+=ToString(type->ParameterType(i));
							}
							result+=L")";
							return result;
						}
					case BasicTypeRecord::Structure:
						{
							BasicGenericStructureProxyTypeRecord* proxy=dynamic_cast<BasicGenericStructureProxyTypeRecord*>(type);
							if(proxy)
							{
								BasicTypeRecord* genericType=proxy->UninstanciatedStructureType();
								WString result=genericType->Declaration()->name;
								for(vint i=0;i<genericType->ParameterCount();i++)
								{
									result+=(i?L", ":L"<");
									result+=ToString(genericType->ParameterType(i));
								}
								result+=L">";
								return result;
							}
							else
							{
								return type->Declaration()->name;
							}
						}
						break;
					case BasicTypeRecord::GenericArgument:
						return type->ArgumentName();
					case BasicTypeRecord::Generic:
						return type->Declaration()->name;
					default:
						return L"[UNKNOWN-NATIVEX-TYPE]";
					}
				}

				WString OpToString(BasicUnaryExpression* expression)
				{
					return UnaryOperatorToString(expression->type);
				}

				WString OpToString(BasicBinaryExpression* expression)
				{
					return BinaryOperatorToString(expression->type);
				}

				WString BreakStatementToString()
				{
					return L"break";
				}

				WString ContinueStatementToString()
				{
					return L"continue";
				}

				BasicTypeRecord* GetIntegerType()
				{
					return analyzer->GetTypeManager()->GetPrimitiveType(int_type);
				}

				BasicTypeRecord* GetBooleanType()
				{
					return analyzer->GetTypeManager()->GetPrimitiveType(bool_type);
				}

			public:
				NativeXMessageTranslator(BasicAnalyzer* _analyzer)
					:analyzer(_analyzer)
				{
				}
			};

			class NativeXProvider : public Object, public ILanguageProvider, public IBasicLanguageProvider
			{
			protected:
				NativeXParser			parser;

				bool Parse(IReadonlyList<WString>& codes, IList<Ptr<LanguageException>>& errors, IDictionary<WString, Ptr<NativeXUnit>>& units)
				{
					for(vint i=0;i<codes.Count();i++)
					{
						Ptr<NativeXUnit> unit=parser.Parse(codes[i], i, errors);
						if(unit)
						{
							if(units.Keys().Contains(unit->name))
							{
								errors.Add(new LanguageException(NativeXErrorMessage::UnitAlreadyExists(unit->name), 0, 0, 0, unit->codeIndex));
							}
							else
							{
								units.Add(unit->name, unit);
							}
						}
					}
					for(vint i=0;i<units.Count();i++)
					{
						Ptr<NativeXUnit> unit=units.Values()[i];
						for(vint j=0;j<unit->imports.Count();i++)
						{
							if(!units.Keys().Contains(unit->imports[j]))
							{
								errors.Add(new LanguageException(NativeXErrorMessage::UnitNotExists(unit->imports[j]), 0, 0, 0, unit->codeIndex));
							}
						}
					}
					return errors.Count()>0;
				}

				Ptr<NativeXUnit> SearchCircularImportsInternal(
					IReadonlyDictionary<WString, Ptr<NativeXUnit>>& units,
					ICollection<Ptr<NativeXUnit>>& searchedUnits,
					ICollection<Ptr<NativeXUnit>>& safeUnits,
					Ptr<NativeXUnit> current
					)
				{
					if(safeUnits.Contains(current.Obj()))return 0;
					if(searchedUnits.Contains(current.Obj()))return current;
					searchedUnits.Add(current);
					for(vint i=0;i<current->imports.Count();i++)
					{
						Ptr<NativeXUnit> result=SearchCircularImportsInternal(units, searchedUnits, safeUnits, units[current->imports[i]]);
						if(result)
						{
							return result;
						}
					}
					safeUnits.Add(current);
					return 0;
				}

				Ptr<NativeXUnit> SearchCircularImports(IReadonlyDictionary<WString, Ptr<NativeXUnit>>& units)
				{
					SortedList<Ptr<NativeXUnit>> searchedUnits;
					SortedList<Ptr<NativeXUnit>> safeUnits;
					for(vint i=0;i<units.Count();i++)
					{
						searchedUnits.Clear();
						Ptr<NativeXUnit> result=SearchCircularImportsInternal(units, searchedUnits.Wrap(), safeUnits.Wrap(), units.Values()[i]);
						if(result)
						{
							return result;
						}
					}
					return 0;
				}

				void SortUnits(IDictionary<WString, Ptr<NativeXUnit>>& units, IList<Ptr<NativeXUnit>>& sortedUnits)
				{
					while(units.Count())
					{
						for(vint i=0;i<units.Count();i++)
						{
							WString name=units.Keys()[i];
							vint count=0;
							for(vint j=0;j<units.Count();j++)
							{
								if(units.Values()[j]->imports.Contains(name))
								{
									count++;
								}
							}
							if(count==0)
							{
								sortedUnits.Add(units.Values()[i]);
								units.Remove(name);
								break;
							}
						}
					}
				}

				void SetConfiguration(BasicAlgorithmConfiguration& config)
				{
					config.treatCharacterAsInteger=true;
					config.enableImplicitBooleanToIntegerConversion=true;
					config.enableImplicitIntegerToBooleanConversion=true;
					config.enableImplicitIntegerToFloatConversion=true;
					config.enableImplicitUnsignedToSignedConversion=true;
					config.enableImplicitPointerToBooleanConversion=true;
					config.enablePointerArithmetic=true;
					config.enableSubscribeOnPointer=true;
				}
			public:
				WString LanguageName()
				{
					return L"NativeX";
				}

				LanguageTypeEnum LanguageType()
				{
					return ILanguageProvider::Native;
				}

				Ptr<LanguageAssembly> Compile(
					const WString& assemblyName,
					IReadonlyList<Ptr<LanguageAssembly>>& references,
					IReadonlyList<WString>& codes, 
					IList<Ptr<LanguageException>>& errors
					)
				{
					Dictionary<WString, Ptr<NativeXUnit>> units;
					Parse(codes, errors, units.Wrap());
					if(errors.Count()>0)
					{
						return 0;
					}
					if(Ptr<NativeXUnit> unit=SearchCircularImports(units.Wrap()))
					{
						errors.Add(new LanguageException(NativeXErrorMessage::UnitCircularReferenced(unit->name), 0, 0, 0, unit->codeIndex));
						return 0;
					}

					List<Ptr<NativeXUnit>> sortedUnits;
					SortUnits(units.Wrap(), sortedUnits.Wrap());
					Ptr<BasicProgram> program=new BasicProgram;
					for(vint i=0;i<sortedUnits.Count();i++)
					{
						CopyFrom(program->declarations.Wrap(), sortedUnits[i]->program->declarations.Wrap(), true);
					}

					BasicAlgorithmConfiguration configuration;
					SetConfiguration(configuration);
					BasicAnalyzer analyzer(program, 0, configuration);
					analyzer.Analyze();
					if(analyzer.GetErrors().Count()>0)
					{
						NativeXMessageTranslator translator(&analyzer);
						for(vint i=0;i<analyzer.GetErrors().Count();i++)
						{
							errors.Add(translator.Translate(analyzer.GetErrors()[i], analyzer.GetErrors()[i]->GetBasicLanguageElement()->position.codeIndex));
						}
						return 0;
					}

					BasicCodeGenerator codegen(&analyzer, 0, assemblyName);
					codegen.GenerateCode();
					return new LanguageAssembly(codegen.GetIL());
				}
				
				Ptr<BasicProgram> ParseProgram(const WString& code, IList<Ptr<LanguageException>>& errors)
				{
					Ptr<NativeXUnit> unit=parser.Parse(code, 0, errors);
					if(unit)
					{
						return unit->program;
					}
					else
					{
						return 0;
					}
				}
				
				void GenerateCode(Ptr<basiclanguage::BasicProgram> program, stream::TextWriter& writer)
				{
					NativeX_BasicProgram_GenerateCode(program, writer);
				}
			};

/***********************************************************************
代码生成
***********************************************************************/

			typedef struct NativeXCodeGeneratorParameter
			{
				TextWriter&			writer;
				vint					indentation;

				NativeXCodeGeneratorParameter(TextWriter& _writer, vint _indentation)
					:writer(_writer)
					,indentation(_indentation)
				{
				}
			} NXCGP;

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
					argument.writer.WriteString(itow(node->size));
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

				ALGORITHM_PROCEDURE_MATCH(BasicInstanciatedGenericType)
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

				ALGORITHM_PROCEDURE_MATCH(BasicExtendedType)
				{
				}

			END_ALGORITHM_PROCEDURE(NativeX_BasicType_GenerateCode)

			BEGIN_ALGORITHM_PROCEDURE(NativeX_BasicExpression_GenerateCode, BasicExpression, NXCGP)

				ALGORITHM_PROCEDURE_MATCH(BasicNullExpression)
				{
					argument.writer.WriteString(L"null");
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

				ALGORITHM_PROCEDURE_MATCH(BasicCastingExpression)
				{
					argument.writer.WriteString(L"cast<");
					NativeX_BasicType_GenerateCode(node->type, argument);
					argument.writer.WriteString(L">");
					NativeX_BasicExpression_GenerateCode(node->operand, argument);
				}

				ALGORITHM_PROCEDURE_MATCH(BasicReferenceExpression)
				{
					IdentifierToString(node->name, argument.writer);
				}

				ALGORITHM_PROCEDURE_MATCH(BasicInstanciatedExpression)
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
					argument.writer.WriteLine(L"for");
					NativeX_BasicStatement_GenerateCode(node->initializer, newArgument);
					
					PrintIndentation(argument);
					argument.writer.WriteString(L"when(");
					NativeX_BasicExpression_GenerateCode(node->condition, argument);
					argument.writer.WriteLine(L")");
					
					PrintIndentation(argument);
					argument.writer.WriteLine(L"with");
					NativeX_BasicStatement_GenerateCode(node->sideEffect, newArgument);
					
					PrintIndentation(argument);
					argument.writer.WriteLine(L"do");
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

			BEGIN_ALGORITHM_PROCEDURE(NativeX_BasicDeclaration_GenerateCode, BasicDeclaration, NXCGP)

				ALGORITHM_PROCEDURE_MATCH(BasicFunctionDeclaration)
				{
					PrintGeneric(node, argument);
					PrintIndentation(argument);
					argument.writer.WriteString(L"function ");
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
					PrintGeneric(node, argument);
					PrintIndentation(argument);
					argument.writer.WriteString(L"variable ");
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
				writer.WriteLine(L"unit nativex_program_generated;");
				for(vint i=0;i<program->declarations.Count();i++)
				{
					NativeX_BasicDeclaration_GenerateCode(program->declarations[i], argument);
					writer.WriteLine(L"");
				}
			}
		}

		Ptr<ILanguageProvider> CreateNativeXLanguageProvider()
		{
			return new language_nativex::NativeXProvider;
		}
	}
}