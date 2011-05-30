#include "NativeXParser.h"
#include "NativeXErrorMessage.h"
#include "..\..\..\Regex\Regex.h"
#include "..\..\..\Combinator\Combinator.h"
#include "..\..\..\Combinator\ParserInput.h"
#include "..\..\..\Combinator\TokenCombinator.h"

namespace vl
{
	namespace scripting
	{
		namespace language_nativex
		{
			using namespace regex;
			using namespace combinator;

			typedef Node<TokenInput<RegexToken>, RegexToken>				TokenType;
			typedef Node<TokenInput<RegexToken>, BasicLinking>				LinkingNode;
			typedef Node<TokenInput<RegexToken>, Ptr<BasicDeclaration>>		DeclarationNode;
			typedef Rule<TokenInput<RegexToken>, Ptr<BasicExpression>>		ExpressionRule;
			typedef Rule<TokenInput<RegexToken>, Ptr<BasicType>>			TypeRule;
			typedef Rule<TokenInput<RegexToken>, Ptr<BasicStatement>>		StatementRule;
			typedef Rule<TokenInput<RegexToken>, Ptr<BasicDeclaration>>		DeclarationRule;
			typedef Rule<TokenInput<RegexToken>, Ptr<NativeXUnit>>			UnitRule;

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
				expression->implicitIntegerType=false;
				expression->type=bool_type;
				expression->argument.s64=0;
				expression->argument.bool_value=true;
				return expression;
			}

			Ptr<BasicExpression> ToFalse(const RegexToken& input)
			{
				Ptr<BasicNumericExpression> expression=CreateNode<BasicNumericExpression>(input);
				expression->implicitIntegerType=false;
				expression->type=bool_type;
				expression->argument.s64=0;
				expression->argument.bool_value=false;
				return expression;
			}

			Ptr<BasicExpression> ToAChar(const RegexToken& input)
			{
				const wchar_t* reading=input.reading+1;
				Ptr<BasicNumericExpression> expression=CreateNode<BasicNumericExpression>(input);
				expression->implicitIntegerType=false;
				expression->type=char_type;
				expression->argument.s64=0;
				expression->argument.char_value=wtoa(WString(EscapeWalk(reading)))[0];
				return expression;
			}

			Ptr<BasicExpression> ToWChar(const RegexToken& input)
			{
				const wchar_t* reading=input.reading+2;
				Ptr<BasicNumericExpression> expression=CreateNode<BasicNumericExpression>(input);
				expression->implicitIntegerType=false;
				expression->type=wchar_type;
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
				bool sign=false;
				bool implicit=true;
				vint bits=64;
				if(input.length>2)
				{
					switch(input.reading[input.length-2])
					{
					case L's':
						sign=true;
						bits=8;
						implicit=false;
						goto FINISHED_TYPE_RECOGNIZING;
					case L'u':
						sign=false;
						bits=8;
						implicit=false;
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
					implicit=false;
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
						CHECK_FAIL(L"词法分析器有错。");
					}
				}
			FINISHED_TYPE_RECOGNIZING:
				Ptr<BasicNumericExpression> expression=CreateNode<BasicNumericExpression>(input);
				expression->implicitIntegerType=implicit;
				unsigned __int64 temporaryParsing=wtou64(WString(input.reading, input.length));
				if(implicit)
				{
					unsigned __int64 temporaryParsing=wtou64(WString(input.reading, input.length));
					if(temporaryParsing < ((unsigned __int64)1<<31))
					{
						sign=true;
#ifndef VCZH_64
						bits=32;
#endif
					}
					else if(temporaryParsing < ((unsigned __int64)1<<32))
					{
#ifndef VCZH_64
						bits=32;
#endif
					}
					else if(temporaryParsing < ((unsigned __int64)1<<63))
					{
						sign=true;
					}
				}
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
						CHECK_FAIL(L"词法分析器有错。");
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
						CHECK_FAIL(L"词法分析器有错。");
					}
				}
				return expression;
			}

			Ptr<BasicExpression> ToFloat(const RegexToken& input)
			{
				Ptr<BasicNumericExpression> expression=CreateNode<BasicNumericExpression>(input);
				expression->implicitIntegerType=false;
				expression->type=f32;
				expression->argument.s64=0;
				expression->argument.f32=(float)wtof(WString(input.reading, input.length));
				return expression;
			}

			Ptr<BasicExpression> ToDouble(const RegexToken& input)
			{
				Ptr<BasicNumericExpression> expression=CreateNode<BasicNumericExpression>(input);
				expression->implicitIntegerType=false;
				expression->type=f64;
				expression->argument.s64=0;
				expression->argument.f64=wtof(WString(input.reading, input.length));
				return expression;
			}

			Ptr<BasicExpression> ToNull(const RegexToken& input)
			{
				return CreateNode<BasicNullExpression>(input);
			}

/***********************************************************************
语义函数：基本表达式
***********************************************************************/

			Ptr<BasicExpression> ToException(const RegexToken& input)
			{
				return CreateNode<BasicExceptionAddressExpression>(input);
			}

			Ptr<BasicExpression> ToStackData(const RegexToken& input)
			{
				return CreateNode<BasicStackDataAddressExpression>(input);
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

			Ptr<BasicExpression> ToSizeof(const ParsingPair<RegexToken, Ptr<BasicType>>& input)
			{
				Ptr<BasicSizeofExpression> expression=CreateNode<BasicSizeofExpression>(input.First());
				expression->type=input.Second();
				return expression;
			}

			Ptr<BasicExpression> ToOffsetof(const ParsingPair<RegexToken, ParsingPair<Ptr<BasicType>, RegexToken>>& input)
			{
				Ptr<BasicOffsetofExpression> expression=CreateNode<BasicOffsetofExpression>(input.First());
				expression->type=input.Second().First();
				expression->member=WString(input.Second().Second().reading, input.Second().Second().length);
				return expression;
			}

/***********************************************************************
语义函数：复合表达式
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
					CHECK_FAIL(L"language_nativex::ToPostUnary()#错误的操作符。");
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
					CHECK_FAIL(L"language_nativex::ToPreUnary()#错误的操作符。");
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
					CHECK_FAIL(L"language_nativex::ToBinary()#错误的操作符。");
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
					CHECK_FAIL(L"language_nativex::ToBinary()#错误的操作符。");
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

			ParsingPair<RegexToken, Ptr<BasicExpression>> ToPointerTypeDecorator(const RegexToken& decoration)
			{
				return ParsingPair<RegexToken, Ptr<BasicExpression>>(decoration, 0);
			}

			ParsingPair<RegexToken, Ptr<BasicExpression>> ToArrayTypeDecorator(const ParsingPair<RegexToken, Ptr<BasicExpression>>& input)
			{
				return input;
			}

			Ptr<BasicType> ToDecoratedType(const Ptr<BasicType>& decoratingType, const ParsingPair<RegexToken, Ptr<BasicExpression>>& decorator)
			{
				if(decorator.Second())
				{
					Ptr<BasicArrayType> type=CreateNode<BasicArrayType>(decorator.First());
					type->count=decorator.Second();
					type->elementType=decoratingType;
					return type;
				}
				else
				{
					Ptr<BasicPointerType> type=CreateNode<BasicPointerType>(decorator.First());
					type->elementType=decoratingType;
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

			Ptr<BasicType> ToTypeofExpression(const ParsingPair<RegexToken, Ptr<BasicExpression>>& input)
			{
				Ptr<BasicTypeofExpressionType> type=CreateNode<BasicTypeofExpressionType>(input.First());
				type->expression=input.Second();
				return type;
			}

			Ptr<BasicType> ToTypeofMember(const ParsingPair<RegexToken, ParsingPair<Ptr<BasicType>, RegexToken>>& input)
			{
				Ptr<BasicTypeofMemberType> type=CreateNode<BasicTypeofMemberType>(input.First());
				type->type=input.Second().First();
				type->member=WString(input.Second().Second().reading, input.Second().Second().length);
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

			Ptr<BasicStatement> ToTryCatch(const ParsingPair<RegexToken, ParsingPair<Ptr<BasicStatement>, Ptr<BasicStatement>>>& input)
			{
				Ptr<BasicTryCatchStatement> statement=CreateNode<BasicTryCatchStatement>(input.First());
				statement->tryStatement=input.Second().First();
				statement->catchStatement=input.Second().Second();
				return statement;
			}

			Ptr<BasicStatement> ToThrow(const ParsingPair<RegexToken, ParsingList<Ptr<BasicExpression>>>& input)
			{
				Ptr<BasicThrowStatement> statement=CreateNode<BasicThrowStatement>(input.First());
				if(input.Second().Head())
				{
					statement->expression=input.Second().Head()->Value();
				}
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
				declaration->constant=false;
				return declaration;
			}

			Ptr<BasicDeclaration> ToConstDecl(const ParsingPair<ParsingPair<ParsingPair<RegexToken, Ptr<BasicType>>, RegexToken>, Ptr<BasicExpression>>& input)
			{
				Ptr<BasicVariableDeclaration> declaration=CreateNode<BasicVariableDeclaration>(input.First().First().First());
				declaration->name=ConvertID(WString(input.First().Second().reading, input.First().Second().length));
				declaration->type=input.First().First().Second();
				declaration->initializer=input.Second();
				declaration->constant=true;
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
				declaration->foreignFunction=false;
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

			Ptr<BasicDeclaration> ToForeignFunctionDecl(const Ptr<BasicDeclaration>& input)
			{
				input.Cast<BasicFunctionDeclaration>()->foreignFunction=true;
				return input;
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

			Ptr<BasicDeclaration> ToConceptDecl(const ParsingPair<ParsingPair<ParsingPair<RegexToken, RegexToken>, ParsingList<BasicLinking>>, ParsingList<Ptr<BasicConceptBaseDeclaration::FunctionConcept>>>& input)
			{
				Ptr<BasicConceptBaseDeclaration> declaration=CreateNode<BasicConceptBaseDeclaration>(input.First().First().First());
				declaration->conceptType=ConvertID(WString(input.First().First().First().reading, input.First().First().First().length));
				declaration->name=ConvertID(WString(input.First().First().Second().reading, input.First().First().Second().length));
				AssignBasicLinking(declaration->linking, input.First().Second());
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

			Ptr<BasicAttribute> ToAttribute(const RegexToken& input)
			{
				Ptr<BasicAttribute> attribute=new BasicAttribute;
				attribute->attributeName=WString(input.reading+1, input.length-1);
				return attribute;
			}

			Ptr<BasicDeclaration> ToAttributedDeclaration(const ParsingPair<ParsingList<Ptr<BasicAttribute>>, Ptr<BasicDeclaration>>& input)
			{
				Ptr<BasicDeclaration> declaration=input.Second();
				CopyFrom(declaration->attributes.Wrap(), input.First());
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
			ERROR_HANDLER(NeedCatch,			RegexToken)

#undef ERROR_HANDLER

/***********************************************************************
语法分析器
***********************************************************************/

			class NativeXParserImpl : public NativeXParser
			{
			protected:
				Ptr<RegexLexer>						lexer;

				TokenType							ACHAR;
				TokenType							WCHAR;
				TokenType							ASTRING;
				TokenType							WSTRING;
				TokenType							INTEGER;
				TokenType							FLOAT;
				TokenType							DOUBLE;
				TokenType							ID;
				TokenType							ATTRIBUTE_NAME;
				TokenType							PRIM_TYPE;
				
				TokenType							SIZEOF, OFFSETOF, TYPEOF;
				TokenType							TRUE, FALSE, NULL_VALUE, EXCEPTION_VALUE, STACK_DATA, RESULT, FUNCTION, CAST, VARIABLE, CONSTANT;
				TokenType							IF, ELSE, BREAK, CONTINUE, EXIT, WHILE, DO, LOOP, WHEN, FOR, WITH, TRY, CATCH, THROW;
				TokenType							TYPE, STRUCTURE, UNIT, USES, ALIAS, GENERIC, CONCEPT, INSTANCE, WHERE, FOREIGN;

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
				DeclarationNode						nonGenericDeclaration, functionDeclaration, rawDeclaration;
				DeclarationRule						declaration;
				UnitRule							unit;
			public:
				NativeXParserImpl()
				{
					List<WString> tokens;
					tokens.Add(L"/s+");
					tokens.Add(L"////[^\r\n]*(\r\n|\n)?");
					tokens.Add(L"///*([^*//]+|/*+[^//])*/*+//");
										
					SIZEOF			= CreateToken(tokens, L"sizeof");
					OFFSETOF		= CreateToken(tokens, L"offsetof");
					TYPEOF			= CreateToken(tokens, L"typeof");
					TRUE			= CreateToken(tokens, L"true");
					FALSE			= CreateToken(tokens, L"false");
					NULL_VALUE		= CreateToken(tokens, L"null");
					EXCEPTION_VALUE	= CreateToken(tokens, L"exception");
					STACK_DATA		= CreateToken(tokens, L"stackdata");
					RESULT			= CreateToken(tokens, L"result");
					FUNCTION		= CreateToken(tokens, L"function");
					CAST			= CreateToken(tokens, L"cast");
					VARIABLE		= CreateToken(tokens, L"variable");
					CONSTANT		= CreateToken(tokens, L"constant");
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
					TRY				= CreateToken(tokens, L"try");
					CATCH			= CreateToken(tokens, L"catch");
					THROW			= CreateToken(tokens, L"throw");
					TYPE			= CreateToken(tokens, L"type");
					STRUCTURE		= CreateToken(tokens, L"structure");
					UNIT			= CreateToken(tokens, L"unit");
					USES			= CreateToken(tokens, L"uses");
					ALIAS			= CreateToken(tokens, L"alias");
					GENERIC			= CreateToken(tokens, L"generic");
					CONCEPT			= CreateToken(tokens, L"concept");
					INSTANCE		= CreateToken(tokens, L"instance");
					WHERE			= CreateToken(tokens, L"where");
					FOREIGN			= CreateToken(tokens, L"foreign");

					PRIM_TYPE		= CreateToken(tokens, L"int|int8|int16|int32|int64|uint|uint8|uint16|uint32|uint64|f32|f64|bool|char|wchar|void");
					ACHAR			= CreateToken(tokens, L"\'([^\\\\\']|\\\\\\.)\'");
					WCHAR			= CreateToken(tokens, L"L\'([^\\\\\']|\\\\\\.)\'");
					ASTRING			= CreateToken(tokens, L"\"([^\\\\\"]|\\\\\\.)*\"");
					WSTRING			= CreateToken(tokens, L"L\"([^\\\\\"]|\\\\\\.)*\"");
					INTEGER			= CreateToken(tokens, L"/d+([su](8|16|32|64))?");
					FLOAT			= CreateToken(tokens, L"/d+(./d+)[fF]");
					DOUBLE			= CreateToken(tokens, L"/d+./d+");
					ID				= CreateToken(tokens, L"(@?[a-zA-Z_]/w*)|(@\"([^\"]|\\\\\\.)*\")");
					ATTRIBUTE_NAME	= CreateToken(tokens, L"#[a-zA-Z_]/w*");

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
									| EXCEPTION_VALUE[ToException]
									| STACK_DATA[ToStackData]
									| INTEGER[ToInteger]
									;

					reference		= (ID + (LT(NeedLt) >> type(NeedTypeExpression) << GT(NeedGt)) + (COLON(NeedColon) + COLON(NeedColon) >> ID(NeedID)))[ToBasicInstanceFunctionExpression]
									| (ID + (LT(NeedLt) + list(opt(type + *(COMMA >> type))) << GT(NeedGt)))[ToInstanciatedExpression]
									| ID[ToReference]
									;

					exp0			= primitive(NeedExpression)
									| reference
									| RESULT[ToResult]
									| (OFFSETOF + (OPEN_BRACE(NeedOpenBrace) >> type + ((COLON(NeedColon) + COLON(NeedColon) >> ID(NeedID))) <<CLOSE_BRACE(NeedCloseBrace)))[ToOffsetof]
									| (SIZEOF + (OPEN_BRACE(NeedOpenBrace) >> type << CLOSE_BRACE(NeedCloseBrace)))[ToSizeof]
									| (CAST + (LT(NeedLt) >> type << GT(NeedGt)) + (OPEN_BRACE(NeedOpenBrace) >> exp << CLOSE_BRACE(NeedCloseBrace)))[ToCastExpression]
									| (OPEN_BRACE >> exp << CLOSE_BRACE(NeedCloseBrace))
									;

					exp1			= lrec(exp0 +  *(
													(OPEN_ARRAY + exp << CLOSE_ARRAY(NeedCloseArray))
													| (OPEN_BRACE + list(opt(exp + *(COMMA >> exp)))[UpgradeArguments] << CLOSE_BRACE(NeedCloseBrace))
													| ((DOT | POINTER) + ID[ToReference])
													| ((INCREASE | DECREASE)[UpgradePostfix])
													), ToPostUnary);
					exp2			= exp1 | ((INCREASE | DECREASE | BIT_AND | MUL | SUB | BIT_NOT | NOT) + exp2)[ToPreUnary];
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
									| (TYPEOF + (OPEN_BRACE(NeedOpenBrace) >> type + ((COLON(NeedColon) + COLON(NeedColon) >> ID(NeedID))) << CLOSE_BRACE(NeedCloseBrace)))[ToTypeofMember]
									| (TYPEOF + (OPEN_BRACE(NeedOpenBrace) >> exp << CLOSE_BRACE(NeedCloseBrace)))[ToTypeofExpression]
									;

					type			= lrec(primType + *(MUL[ToPointerTypeDecorator] | ((OPEN_ARRAY + exp)[ToArrayTypeDecorator] << CLOSE_ARRAY)), ToDecoratedType);

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
									| ((FOR << OPEN_BRACE(NeedOpenBrace)) + list(*statement) + (WHEN(NeedWhen) >> OPEN_BRACE(NeedOpenBrace) >> exp << CLOSE_BRACE(NeedCloseBrace)) + (WITH(NeedWith) >> list(*statement)) + (CLOSE_BRACE(NeedCloseBrace) >> DO(NeedDo) >> statement))[ToForStat]
									| (TRY + (statement + (CATCH(NeedCatch) >> statement)))[ToTryCatch]
									| (THROW + opt(exp) << SEMICOLON(NeedSemicolon))[ToThrow]
									;

					instanceType	= (PRIM_TYPE | ID)[ToNamedType]
									;

					functionDeclaration
									= (FUNCTION + type + ID(NeedID) + (OPEN_BRACE(NeedOpenBrace) >> plist(opt((type + ID(NeedID)) + *(COMMA >> (type + ID(NeedID))))) << CLOSE_BRACE(NeedCloseBrace)) + opt(linking << SEMICOLON(NeedSemicolon)) + opt(statement))[ToFuncDecl]
									;

					nonGenericDeclaration		
									= (VARIABLE + type + ID(NeedID) + opt(linking) + opt(ASSIGN >> exp) << SEMICOLON(NeedSemicolon))[ToVarDecl]
									| ((CONSTANT + type + ID(NeedID) + (ASSIGN(NeedAssign) >> exp(NeedExpression))) << SEMICOLON(NeedSemicolon))[ToConstDecl]
									| (TYPE + ID(NeedID) + (ASSIGN(NeedAssign) >> type) << SEMICOLON(NeedSemicolon))[ToTypedefDecl]
									| (STRUCTURE + ID(NeedID) << SEMICOLON(NeedSemicolon))[ToStructPreDecl]
									| (STRUCTURE + ID(NeedID) + opt(linking) + (OPEN_STAT(NeedOpenStruct) >> *(type + ID(NeedID) << SEMICOLON(NeedSemicolon)) << CLOSE_STAT(NeedCloseStruct)))[ToStructDecl]
									| (INSTANCE >> (instanceType + (COLON(NeedColon) >> ID << SEMICOLON(NeedSemicolon))))[ToInstancePreDecl]
									| ((INSTANCE >> (instanceType + (COLON(NeedColon) >> ID)))+(OPEN_STAT(NeedOpenConcept)>>*((ID(NeedID)+(ASSIGN(NeedAssign)>>reference)<<SEMICOLON(NeedSemicolon)))[ToFunctionInstance]<<CLOSE_STAT(NeedCloseConcept)))[ToInstanceDecl]
									| functionDeclaration
									| (FOREIGN >> functionDeclaration)[ToForeignFunctionDecl]
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
					rawDeclaration	= nonGenericDeclaration
									| (CONCEPT>>ID(NeedID)+(COLON(NeedColon)>>ID(NeedID))+opt(linking)+(OPEN_STAT(NeedOpenConcept)>>*((ID(NeedID)+(ASSIGN(NeedAssign)>>functionType)<<SEMICOLON(NeedSemicolon)))[ToFunctionConcept]<<CLOSE_STAT(NeedCloseConcept)))[ToConceptDecl]
									| (genericHead+opt(genericConstraint)+nonGenericDeclaration(NeedDeclaration))[ToGeneric]
									;

					Node<TokenInput<RegexToken>, Ptr<BasicAttribute>> attribute
									= (ATTRIBUTE_NAME)[ToAttribute]
									;

					declaration		= (*attribute + rawDeclaration)[ToAttributedDeclaration]
									;

					unit			= ((UNIT(NeedUnit) >> ID(NeedID) << SEMICOLON(NeedSemicolon)) + list(opt(USES >> (ID(NeedID) + *(COMMA >> ID(NeedID))) << SEMICOLON(NeedSemicolon))) + list(*declaration))[ToUnit];
				}

				static bool Blank(vint token)
				{
					return token<3;
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

			Ptr<NativeXParser> NativeXParser::Create()
			{
				return new NativeXParserImpl;
			}
		}
	}
}