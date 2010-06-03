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
				int					codeIndex;
			};

			typedef Node<TokenInput<RegexToken>, RegexToken>				TokenType;
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
					type->name=name;
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

			Ptr<BasicDeclaration> ToVarDecl(const ParsingPair<ParsingPair<ParsingPair<RegexToken, Ptr<BasicType>>, RegexToken>, ParsingList<Ptr<BasicExpression>>>& input)
			{
				Ptr<BasicVariableDeclaration> declaration=CreateNode<BasicVariableDeclaration>(input.First().First().First());
				declaration->name=ConvertID(WString(input.First().Second().reading, input.First().Second().length));
				declaration->type=input.First().First().Second();
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

			Ptr<BasicDeclaration> ToFuncDecl(const ParsingPair<ParsingPair<ParsingPair<ParsingPair<
				RegexToken, 
				Ptr<BasicType>>, 
				RegexToken>, 
				ParsingList<ParsingPair<Ptr<BasicType>, RegexToken>>>,
				Ptr<BasicStatement>>& input)
			{
				Ptr<BasicFunctionDeclaration> declaration=CreateNode<BasicFunctionDeclaration>(input.First().First().First().First());
				declaration->name=ConvertID(WString(input.First().First().Second().reading, input.First().First().Second().length));
				declaration->statement=input.Second();
				declaration->signatureType=CreateNode<BasicFunctionType>(input.First().First().First().First());
				declaration->signatureType->returnType=input.First().First().First().Second();
				
				ParsingList<ParsingPair<Ptr<BasicType>, RegexToken>>::Node::Ref current=input.First().Second().Head();
				while(current)
				{
					declaration->signatureType->parameterTypes.Add(current->Value().First());
					declaration->parameterNames.Add(ConvertID(WString(current->Value().Second().reading, current->Value().Second().length)));
					current=current->Next();
				}
				return declaration;
			}

			Ptr<BasicDeclaration> ToStructDecl(const ParsingPair<ParsingPair<
				RegexToken,
				RegexToken>,
				ParsingList<ParsingPair<Ptr<BasicType>, RegexToken>>>& input)
			{
				Ptr<BasicStructureDeclaration> declaration=CreateNode<BasicStructureDeclaration>(input.First().First());
				declaration->defined=true;
				declaration->name=ConvertID(WString(input.First().Second().reading, input.First().Second().length));
				ParsingList<ParsingPair<Ptr<BasicType>, RegexToken>>::Node::Ref current=input.Second().Head();
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
				for(int i=0;i<input.First().Second()->Count();i++)
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

			ERROR_HANDLER(NeedExpression,	Ptr<BasicExpression>)
			ERROR_HANDLER(NeedType,			RegexToken)
			ERROR_HANDLER(NeedStatement,	RegexToken)
			ERROR_HANDLER(NeedID,			RegexToken)

			ERROR_HANDLER(NeedLt,			RegexToken)
			ERROR_HANDLER(NeedGt,			RegexToken)
			ERROR_HANDLER(NeedOpenBrace,	RegexToken)
			ERROR_HANDLER(NeedCloseBrace,	RegexToken)
			ERROR_HANDLER(NeedCloseArray,	RegexToken)
			ERROR_HANDLER(NeedComma,		RegexToken)
			ERROR_HANDLER(NeedSemicolon,	RegexToken)
			ERROR_HANDLER(NeedCloseStat,	RegexToken)
			ERROR_HANDLER(NeedAssign,		RegexToken)
			ERROR_HANDLER(NeedOpenStruct,	RegexToken)
			ERROR_HANDLER(NeedCloseStruct,	RegexToken)

			ERROR_HANDLER(NeedWhile,		RegexToken)
			ERROR_HANDLER(NeedWhen,			RegexToken)
			ERROR_HANDLER(NeedWith,			RegexToken)
			ERROR_HANDLER(NeedDo,			RegexToken)
			ERROR_HANDLER(NeedUnit,			RegexToken)

#undef ERROR_HANDLER

/***********************************************************************
语法分析器
***********************************************************************/

			class NativeXParser : public Object
			{
			protected:
				Ptr<RegexLexer>						lexer;
				int									blankID;

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
				TokenType							TYPE, STRUCTURE, UNIT, USES;

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

				TokenType							INCREASE, DECREASE, BIT_NOT, NOT;
				TokenType							ADD, SUB, MUL, DIV, MOD, SHL, SHR;
				TokenType							LT, GT, LE, GE, EQ, NE;
				TokenType							BIT_AND, BIT_OR, AND, OR, XOR;
				TokenType							OP_ASSIGN, ASSIGN;

				ExpressionRule						primitive;
				ExpressionRule						reference;
				ExpressionRule						exp0, exp1, exp2, exp3, exp4, exp5, exp6, exp7, exp8, exp9, exp10, exp11, exp12;
				ExpressionRule						exp;
				TypeRule							primType,type;
				StatementRule						statement;
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

					PRIM_TYPE		= CreateToken(tokens, L"int|int8|int16|int32|int64|uint|uint8|uint16|uint32|uint64|f32|f64|bool|char|wchar|void");
					ACHAR			= CreateToken(tokens, L"\'([^\']|\\\\\\.)\'");
					WCHAR			= CreateToken(tokens, L"L\'([^\']|\\\\\\.)\'");
					ASTRING			= CreateToken(tokens, L"\"([^\"]|\\\\\\.)*\"");
					WSTRING			= CreateToken(tokens, L"L\"([^\"]|\\\\\\.)*\"");
					INTEGER			= CreateToken(tokens, L"/d+");
					FLOAT			= CreateToken(tokens, L"/d+(./d+)[fF]");
					DOUBLE			= CreateToken(tokens, L"/d+./d+");
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
					exp5			= lrec(exp4 + *((SHL | SHR) + exp4), ToBinary);
					exp6			= lrec(exp5 + *((LT | GT | LE | GE) + exp5), ToBinary);
					exp7			= lrec(exp6 + *((EQ | NE) + exp6), ToBinary);
					exp8			= lrec(exp7 + *(BIT_AND + exp7), ToBinary);
					exp9			= lrec(exp8 + *(XOR + exp8), ToBinary);
					exp10			= lrec(exp9 + *(BIT_OR + exp9), ToBinary);
					exp11			= lrec(exp10 + *(AND + exp10), ToBinary);
					exp12			= lrec(exp11 + *(OR + exp11), ToBinary);
					exp				= lrec(exp12 + *((OP_ASSIGN | ASSIGN) + exp12), ToBinary);

					primType		= (FUNCTION(NeedType) + type + (OPEN_BRACE(NeedOpenBrace) >> list(opt(type + *(COMMA >> type))) << CLOSE_BRACE(NeedCloseBrace)))[ToFunctionType]
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

					declaration		= (VARIABLE + type + ID(NeedID) + opt(ASSIGN >> exp) << SEMICOLON(NeedSemicolon))[ToVarDecl]
									| (TYPE + ID + (ASSIGN(NeedAssign) >> type) << SEMICOLON(NeedSemicolon))[ToTypedefDecl]
									| (STRUCTURE + ID(NeedID) << SEMICOLON(NeedSemicolon))[ToStructPreDecl]
									| (STRUCTURE + ID(NeedID) + (OPEN_STAT(NeedOpenStruct) >> *(type + ID(NeedID) << SEMICOLON(NeedSemicolon)) << CLOSE_STAT(NeedCloseStruct)))[ToStructDecl]
									| (FUNCTION + type + ID(NeedID) + (OPEN_BRACE(NeedOpenBrace) >> plist(opt((type + ID(NeedID)) + *(COMMA >> (type + ID(NeedID))))) << CLOSE_BRACE(NeedCloseBrace)) + statement)[ToFuncDecl]
									;

					unit			= ((UNIT(NeedUnit) >> ID(NeedID) << SEMICOLON(NeedSemicolon)) + list(opt(USES >> (ID(NeedID) + *(COMMA >> ID(NeedID))) << SEMICOLON(NeedSemicolon))) + list(*declaration))[ToUnit];
				}

				static bool Blank(int token)
				{
					return token==0;
				}

				Ptr<NativeXUnit> Parse(const WString& code, int codeIndex, IList<Ptr<LanguageException>>& errors)
				{
					List<RegexToken> tokens;
					lexer->Parse(code, codeIndex).ReadToEnd(tokens, Blank);
					for(int i=0;i<tokens.Count();i++)
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
						for(int i=0;i<exception.GetGlobalInfo().errors.Count();i++)
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
				writer.WriteString(L"@\"");
				UnescapeStringContent(identifier, writer);
				writer.WriteString(L"\"");
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
					return ToString(BasicLanguage_GetTypeRecord(type, argument));
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
							for(int i=0;i<type->ParameterCount();i++)
							{
								if(i)result+=L", ";
								result+=ToString(type->ParameterType(i));
							}
							result+=L")";
							return result;
						}
					default:
						return L"structure";
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
					for(int i=0;i<codes.Count();i++)
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
					for(int i=0;i<units.Count();i++)
					{
						Ptr<NativeXUnit> unit=units.Values()[i];
						for(int j=0;j<unit->imports.Count();i++)
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
					for(int i=0;i<current->imports.Count();i++)
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
					for(int i=0;i<units.Count();i++)
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
						for(int i=0;i<units.Count();i++)
						{
							WString name=units.Keys()[i];
							int count=0;
							for(int j=0;j<units.Count();j++)
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
					for(int i=0;i<sortedUnits.Count();i++)
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
						for(int i=0;i<analyzer.GetErrors().Count();i++)
						{
							errors.Add(translator.Translate(analyzer.GetErrors()[i], analyzer.GetErrors()[i]->GetBasicLanguageElement()->position.codeIndex));
						}
						return 0;
					}

					BasicCodeGenerator codegen(&analyzer, 0);
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
				int					indentation;

				NativeXCodeGeneratorParameter(TextWriter& _writer, int _indentation)
					:writer(_writer)
					,indentation(_indentation)
				{
				}
			} NXCGP;

			BEGIN_ALGORITHM_PROCEDURE(NativeX_BasicType_GenerateCode, BasicType, NXCGP)

				ALGORITHM_PROCEDURE_MATCH(BasicPrimitiveType)
				{
					argument.writer.WriteString(node->type);
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
					for(int i=0;i<node->parameterTypes.Count();i++)
					{
						if(i)
						{
							argument.writer.WriteString(L", ");
						}
						NativeX_BasicType_GenerateCode(node->parameterTypes[i], argument);
					}
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

				ALGORITHM_PROCEDURE_MATCH(BasicNumericExpression)
				{
					switch(node->type)
					{
					case s8:			argument.writer.WriteString(itow(node->argument.s8));						break;
					case s16:			argument.writer.WriteString(itow(node->argument.s16));						break;
					case s32:			argument.writer.WriteString(itow(node->argument.s32));						break;
					case s64:			argument.writer.WriteString(i64tow(node->argument.s64));					break;
					case u8:			argument.writer.WriteString(utow(node->argument.u8));						break;
					case u16:			argument.writer.WriteString(utow(node->argument.u16));						break;
					case u32:			argument.writer.WriteString(utow(node->argument.u32));						break;
					case u64:			argument.writer.WriteString(u64tow(node->argument.u64));					break;
					case f32:			argument.writer.WriteString(ftow(node->argument.f32));						break;
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
					argument.writer.WriteString(UnaryOperatorToString(node->type));
					NativeX_BasicExpression_GenerateCode(node->operand, argument);
				}

				ALGORITHM_PROCEDURE_MATCH(BasicBinaryExpression)
				{
					argument.writer.WriteString(L"(");
					NativeX_BasicExpression_GenerateCode(node->leftOperand, argument);
					argument.writer.WriteString(BinaryOperatorToString(node->type));
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
					for(int i=0;i<node->arguments.Count();i++)
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

				ALGORITHM_PROCEDURE_MATCH(BasicExtendedExpression)
				{
				}

			END_ALGORITHM_PROCEDURE(NativeX_BasicExpression_GenerateCode)

			BEGIN_ALGORITHM_PROCEDURE(NativeX_BasicStatement_GenerateCode, BasicStatement, NXCGP)

				ALGORITHM_PROCEDURE_MATCH(BasicEmptyStatement)
				{
				}

				ALGORITHM_PROCEDURE_MATCH(BasicCompositeStatement)
				{
				}

				ALGORITHM_PROCEDURE_MATCH(BasicExpressionStatement)
				{
				}

				ALGORITHM_PROCEDURE_MATCH(BasicVariableStatement)
				{
				}

				ALGORITHM_PROCEDURE_MATCH(BasicIfStatement)
				{
				}

				ALGORITHM_PROCEDURE_MATCH(BasicWhileStatement)
				{
				}

				ALGORITHM_PROCEDURE_MATCH(BasicForStatement)
				{
				}

				ALGORITHM_PROCEDURE_MATCH(BasicBreakStatement)
				{
				}

				ALGORITHM_PROCEDURE_MATCH(BasicContinueStatement)
				{
				}

				ALGORITHM_PROCEDURE_MATCH(BasicReturnStatement)
				{
				}

				ALGORITHM_PROCEDURE_MATCH(BasicExtendedStatement)
				{
				}

			END_ALGORITHM_PROCEDURE(NativeX_BasicStatement_GenerateCode)

			BEGIN_ALGORITHM_PROCEDURE(NativeX_BasicDeclaration_GenerateCode, BasicDeclaration, NXCGP)

				ALGORITHM_PROCEDURE_MATCH(BasicFunctionDeclaration)
				{
				}

				ALGORITHM_PROCEDURE_MATCH(BasicVariableDeclaration)
				{
				}

				ALGORITHM_PROCEDURE_MATCH(BasicTypeRenameDeclaration)
				{
				}

				ALGORITHM_PROCEDURE_MATCH(BasicStructureDeclaration)
				{
				}

				ALGORITHM_PROCEDURE_MATCH(BasicExtendedDeclaration)
				{
				}

			END_ALGORITHM_PROCEDURE(NativeX_BasicDeclaration_GenerateCode)

			void NativeX_BasicProgram_GenerateCode(Ptr<BasicProgram> program, TextWriter& writer)
			{
				NXCGP argument(writer, 0);
				for(int i=0;i<program->declarations.Count();i++)
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