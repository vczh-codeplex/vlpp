#include "NativeX.h"
#include "..\..\BasicLanguage\BasicLanguageExpression.h"
#include "..\..\..\Regex\Regex.h"
#include "..\..\..\Combinator\Combinator.h"
#include "..\..\..\Combinator\ParserInput.h"
#include "..\..\..\Combinator\TokenCombinator.h"
#include "..\..\..\Collections\OperationCopyFrom.h"

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

			class NativeXUnit : public Object
			{
			public:
				WString				name;
				List<WString>		imports;
				Ptr<BasicProgram>	program;
			};

			typedef Node<TokenInput<RegexToken>, RegexToken>				TokenType;
			typedef Rule<TokenInput<RegexToken>, Ptr<BasicExpression>>		ExpressionRule;

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
				expression->argument.bool_value=true;
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
				__int64 number=wtoi64(WString(input.reading, input.length));
				unsigned __int64 unsignedNumber=wtou64(WString(input.reading, input.length));
				Ptr<BasicNumericExpression> expression=CreateNode<BasicNumericExpression>(input);
				expression->argument.s64=0;
				if(-128<=number && number<=127)
				{
					expression->type=s8;
					expression->argument.s8=(__int8)number;
				}
				else if(128<=number && number<=255)
				{
					expression->type=u8;
					expression->argument.u8=(unsigned __int8)number;
				}
				else if(-32768<=number && number<32767)
				{
					expression->type=s16;
					expression->argument.s16=(__int16)number;
				}
				else if(32768<=number && number<=65535)
				{
					expression->type=u16;
					expression->argument.u16=(unsigned __int16)number;
				}
				else if(-(__int64)2147483648<=number && number<=(__int64)2147483647)
				{
					expression->type=s32;
					expression->argument.s32=(__int32)number;
				}
				else if((__int64)2147483648<=number && number<=(__int64)4294967295)
				{
					expression->type=u32;
					expression->argument.u32=(unsigned __int32)number;
				}
				else if(number<0 || number==unsignedNumber)
				{
					expression->type=s64;
					expression->argument.s64=number;
				}
				else
				{
					expression->type=u64;
					expression->argument.u64=unsignedNumber;
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
					Ptr<NativeXArguments> arguments=unary.Second();
					Ptr<BasicInvokeExpression> expression=CreateNode<BasicInvokeExpression>(unary.First());
					expression->function=operand;
					CopyFrom(expression->arguments.Wrap(), arguments->arguments->Wrap());
					return expression;
				}
				else if(op==L".")
				{
					Ptr<BasicReferenceExpression> member=unary.Second();
					Ptr<BasicMemberExpression> expression=CreateNode<BasicMemberExpression>(unary.First());
					expression->operand=operand;
					expression->member=member->name;
					expression->pointerMember=false;
					return expression;
				}
				else if(op==L"->")
				{
					Ptr<BasicReferenceExpression> member=unary.Second();
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
				else if(op==L"<<")
				{
					expression->type=BasicBinaryExpression::Shl;
				}
				else if(op==L">>")
				{
					expression->type=BasicBinaryExpression::Shr;
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

/***********************************************************************
错误恢复
***********************************************************************/

/***********************************************************************
语法分析器
***********************************************************************/

			class NativeXParser : public Object
			{
			protected:
				Ptr<RegexLexer>						lexer;

				TokenType							TRUE;
				TokenType							FALSE;
				TokenType							ACHAR;
				TokenType							WCHAR;
				TokenType							ASTRING;
				TokenType							WSTRING;
				TokenType							INTEGER;
				TokenType							FLOAT;
				TokenType							DOUBLE;
				TokenType							NULL_VALUE;
				TokenType							ID;
				TokenType							RESULT;

				TokenType							OPEN_ARRAY;
				TokenType							CLOSE_ARRAY;
				TokenType							OPEN_BRACE;
				TokenType							CLOSE_BRACE;
				TokenType							DOT;
				TokenType							POINTER;
				TokenType							COMMA;

				TokenType							INCREASE, DECREASE, BIT_NOT, NOT;
				TokenType							ADD, SUB, MUL, DIV, MOD, SHL, SHR;
				TokenType							LT, GT, LE, GE, EQ, NE;
				TokenType							BIT_AND, BIT_OR, AND, OR, XOR;
				TokenType							OP_ASSIGN, ASSIGN;

				ExpressionRule						primitive;
				ExpressionRule						reference;
				ExpressionRule						exp0, exp1, exp2, exp3, exp4, exp5, exp6, exp7, exp8, exp9, exp10, exp11, exp12;
				ExpressionRule						exp;
			public:
				NativeXParser()
				{
					List<WString> tokens;

					TRUE			= CreateToken(tokens, L"true");
					FALSE			= CreateToken(tokens, L"false");
					ACHAR			= CreateToken(tokens, L"\'([^\']|\\\\\\.)\'");
					WCHAR			= CreateToken(tokens, L"L\'([^\']|\\\\\\.)\'");
					ASTRING			= CreateToken(tokens, L"\"([^\"]|\\\\\\.)*\"");
					WSTRING			= CreateToken(tokens, L"L\"([^\"]|\\\\\\.)*\"");
					INTEGER			= CreateToken(tokens, L"[+/-]?/d+");
					FLOAT			= CreateToken(tokens, L"[+/-]?/d+(./d+)[fF]");
					DOUBLE			= CreateToken(tokens, L"[+/-]?/d+./d+");
					NULL_VALUE		= CreateToken(tokens, L"null");
					ID				= CreateToken(tokens, L"(@?[a-zA-Z_]/w*)|(@\"([^\"]|\\\\\\.)*\")");
					RESULT			= CreateToken(tokens, L"result");

					OPEN_ARRAY		= CreateToken(tokens, L"/[");
					CLOSE_ARRAY		= CreateToken(tokens, L"/]");
					OPEN_BRACE		= CreateToken(tokens, L"/(");
					CLOSE_BRACE		= CreateToken(tokens, L"/)");
					DOT				= CreateToken(tokens, L".");
					POINTER			= CreateToken(tokens, L"->");
					COMMA			= CreateToken(tokens, L",");

					INCREASE		= CreateToken(tokens, L"/+/+");
					DECREASE		= CreateToken(tokens, L"--");
					BIT_NOT			= CreateToken(tokens, L"~");
					NOT				= CreateToken(tokens, L"!");
					ADD				= CreateToken(tokens, L"/+");
					SUB				= CreateToken(tokens, L"-");
					MUL				= CreateToken(tokens, L"/*");
					DIV				= CreateToken(tokens, L"//");
					MOD				= CreateToken(tokens, L"%");
					SHL				= CreateToken(tokens, L"<<");
					SHR				= CreateToken(tokens, L">>");
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

					primitive		= TRUE[ToTrue] | FALSE[ToFalse]
									| ACHAR[ToAChar] | WCHAR[ToWChar]
									| ASTRING[ToAString] | WSTRING[ToWString]
									| FLOAT[ToFloat] | DOUBLE[ToDouble]
									| NULL_VALUE[ToNull]
									| INTEGER[ToInteger]
									;
					reference		= ID[ToReference];

					exp0			= primitive | reference | RESULT[ToResult];
					exp1			= lrec(exp0 +  *(
													(OPEN_ARRAY + exp0 << CLOSE_ARRAY)
													| (OPEN_BRACE + list(exp + *(COMMA >> exp))[UpgradeArguments] << CLOSE_BRACE)
													| ((DOT | POINTER) + reference)
													| (INCREASE | DECREASE)[UpgradePostfix]
													), ToPostUnary);
					exp2			= exp1 | ((INCREASE | DECREASE | BIT_AND | MUL | SUB | BIT_NOT | NOT) + exp1)[ToPreUnary];
					exp3			= lrec(exp2 + *((MUL | DIV | MOD) + exp2), ToBinary);
					exp4			= lrec(exp3 + *((ADD | SUB) + exp3), ToBinary);
					exp5			= lrec(exp4 + *((SHL | SHR) + exp4), ToBinary);
					exp6			= lrec(exp5 + *((LT | GT | LE | GE) + exp5), ToBinary);
					exp7			= lrec(exp6 + *((EQ | NE) + exp6), ToBinary);
					exp8			= lrec(exp7 + *(BIT_AND + exp7), ToBinary);
					exp9			= lrec(exp8 + *(XOR + exp8), ToBinary);
					exp10			= lrec(exp9 + *(BIT_OR + exp9), ToBinary);
					exp11			= lrec(exp10 + *(AND + exp10), ToBinary);
					exp12			= lrec(exp11 + *(OR + exp11), ToBinary);
					exp				= lrec(exp12 + *((OP_ASSIGN | ASSIGN) + exp12), ToBinary);
				}
			};

/***********************************************************************
接口
***********************************************************************/

			class NativeXProvider : public Object, public ILanguageProvider
			{
			protected:
				NativeXParser			parser;
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
					collections::IReadonlyList<Ptr<LanguageAssembly>>& references,
					collections::IReadonlyList<WString>& codes, 
					collections::IList<Ptr<LanguageException>>& errors
					)
				{
					return 0;
				}
			};
		}

		Ptr<ILanguageProvider> CreateNativeXLanguageProvider()
		{
			return new language_nativex::NativeXProvider;
		}
	}
}