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
			typedef Rule<TokenInput<RegexToken>, Ptr<BasicType>>			TypeRule;
			typedef Rule<TokenInput<RegexToken>, Ptr<BasicStatement>>		StatementRule;
			typedef Rule<TokenInput<RegexToken>, Ptr<BasicDeclaration>>		DeclarationRule;

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
				statement->name=ConvertID((input.First().Second().reading, input.First().Second().length));
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
				declaration->name=ConvertID((input.First().Second().reading, input.First().Second().length));
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
错误恢复
***********************************************************************/

/***********************************************************************
语法分析器
***********************************************************************/

			class NativeXParser : public Object
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
				TokenType							PRIM_TYPE;

				TokenType							TRUE, FALSE, NULL_VALUE, RESULT, FUNCTION, CAST, VARIABLE;
				TokenType							IF, ELSE, BREAK, CONTINUE, EXIT, WHILE, DO, LOOP, WHEN, FOR, WITH;
				TokenType							TYPE, STRUCTURE;

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
			public:
				NativeXParser()
				{
					List<WString> tokens;

					ACHAR			= CreateToken(tokens, L"\'([^\']|\\\\\\.)\'");
					WCHAR			= CreateToken(tokens, L"L\'([^\']|\\\\\\.)\'");
					ASTRING			= CreateToken(tokens, L"\"([^\"]|\\\\\\.)*\"");
					WSTRING			= CreateToken(tokens, L"L\"([^\"]|\\\\\\.)*\"");
					INTEGER			= CreateToken(tokens, L"/d+");
					FLOAT			= CreateToken(tokens, L"/d+(./d+)[fF]");
					DOUBLE			= CreateToken(tokens, L"/d+./d+");
					ID				= CreateToken(tokens, L"(@?[a-zA-Z_]/w*)|(@\"([^\"]|\\\\\\.)*\")");
					PRIM_TYPE		= CreateToken(tokens, L"int|int8|int16|int32|int64|uint|uint8|uint16|uint32|uint64|f32|f64|bool|char|wchar|void");

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

					exp0			= primitive
									| reference
									| RESULT[ToResult]
									| (CAST + (LT >> type << GT) + (OPEN_BRACE >> exp << CLOSE_BRACE))[ToCastExpression]
									;
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

					primType		= (FUNCTION + type + (OPEN_BRACE >> list(opt(type + *(COMMA >> type))) << CLOSE_BRACE))[ToFunctionType]
									| (PRIM_TYPE | ID)[ToNamedType]
									;
					type			= lrec(primType + *(MUL | (OPEN_ARRAY >> INTEGER << CLOSE_ARRAY)), ToDecoratedType);

					statement		= SEMICOLON[ToEmptyStat]
									| (exp + SEMICOLON)[ToExprStat]
									| (VARIABLE + type + ID + opt(ASSIGN >> exp) << SEMICOLON)[ToVarStat]
									| (IF + (OPEN_BRACE >> exp << CLOSE_BRACE) + statement + opt(ELSE >> statement))[ToIfStat]
									| (BREAK << SEMICOLON)[ToBreakStat]
									| (CONTINUE << SEMICOLON)[ToContinueStat]
									| (EXIT << SEMICOLON)[ToReturnStat]
									| (OPEN_STAT + list(*statement) << OPEN_STAT)[ToCompositeStat]
									| (DO + statement + (WHILE >> OPEN_BRACE >> exp << CLOSE_BRACE << SEMICOLON))[ToDoWhileStat]
									| (LOOP + statement)[ToLoopStat]
									| (WHILE + (OPEN_BRACE >> exp << CLOSE_BRACE) + statement + opt(WHEN >> OPEN_BRACE >> exp << CLOSE_BRACE << SEMICOLON))[ToWhileStat]
									| (FOR + list(*statement) + (WHEN >> OPEN_BRACE >> exp << CLOSE_BRACE) + (WITH >> list(*statement)) + (DO >> statement))[ToForStat]
									;
					declaration		= (VARIABLE + type + ID + opt(ASSIGN >> exp) << SEMICOLON)[ToVarDecl]
									| (TYPE + ID + (ASSIGN >> type) << SEMICOLON)[ToTypedefDecl]
									| (STRUCTURE + ID << SEMICOLON)[ToStructPreDecl]
									| (STRUCTURE + ID + (OPEN_STAT >> *(type + ID << SEMICOLON) << CLOSE_STAT))[ToStructDecl]
									| (FUNCTION + type + ID + (OPEN_BRACE >> plist(opt((type + ID) + *(COMMA >> (type + ID)))) << CLOSE_BRACE) + statement)[ToFuncDecl]
									;
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