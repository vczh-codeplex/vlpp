#include "ManagedXParser.h"
#include "ManagedXErrorMessage.h"
#include "..\..\..\Regex\Regex.h"
#include "..\..\..\Combinator\Combinator.h"
#include "..\..\..\Combinator\ParserInput.h"
#include "..\..\..\Combinator\TokenCombinator.h"
#include "..\..\..\Collections\Operation.h"

/***********************************************************************
+	op_pos
-	op_neg
!	op_not
~	op_bitnot
++	op_preinc/op_postinc
--	op_predec/op_postdec
====================================
*	op_mul
/	op_div
%	op_mod

+	op_add
-	op_sub

<<	op_shl
>>	op_shr

<	op_lt
<=	op_le
>	op_gt
>=	op_ge
as	ManagedCastingExpression
is	ManagedIsTypeExpression

==	op_eq
!=	op_ne

&	op_bitand

^	op_xor

|	op_bitor

&&	op_and

||	op_or

??	ManagedNullChoiceExpression

?:	ManagedChoiceExpression

+=	op_add_eq
-=	op_sub_eq
*=	op_mul_eq
/=	op_div_eq
%=	op_mod_eq
&&=	op_and_eq
&=	op_bitand_eq
||=	op_or_eq
|=	op_bitor_eq
^=	op_xor_eq
<<=	op_shl_eq
>>=	op_shr_eq
=	ManagedAssignmentExpression
***********************************************************************/

namespace vl
{
	namespace scripting
	{
		namespace language_managedx
		{
			using namespace regex;
			using namespace combinator;

/***********************************************************************
¸¨Öúº¯Êý
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

			template<typename T, typename U>
			Ptr<T> CreateNode(Ptr<U> copy)
			{
				Ptr<T> node=new T;
				node->position.start=copy->position.start;
				node->position.lineStart=copy->position.lineStart;
				node->position.lineIndex=copy->position.lineIndex;
				node->position.codeIndex=copy->position.codeIndex;
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
Constants
***********************************************************************/

			Ptr<ManagedExpression> ToNull(const RegexToken& input)
			{
				Ptr<ManagedNullExpression> exp=CreateNode<ManagedNullExpression>(input);
				return exp;
			}

			Ptr<ManagedExpression> ToBoolean(const RegexToken& input)
			{
				Ptr<ManagedBooleanExpression> exp=CreateNode<ManagedBooleanExpression>(input);
				WString value(input.reading, input.length);
				exp->value=value==L"true";
				return exp;
			}

			Ptr<ManagedExpression> ToInteger(const RegexToken& input)
			{
				Ptr<ManagedIntegerExpression> exp=CreateNode<ManagedIntegerExpression>(input);
				WString value(input.reading, input.length);
				exp->sign=false;
				exp->value.unsignedInteger=wtou64(value);
				return exp;
			}

			Ptr<ManagedExpression> ToFloat(const RegexToken& input)
			{
				Ptr<ManagedFloatExpression> exp=CreateNode<ManagedFloatExpression>(input);
				WString value(input.reading, input.length);
				exp->value=wtof(value);
				return exp;
			}

			Ptr<ManagedExpression> ToChar(const RegexToken& input)
			{
				Ptr<ManagedCharExpression> exp=CreateNode<ManagedCharExpression>(input);
				WString value(input.reading+1, input.length-2);
				exp->value=EscapeString(value)[0];
				return exp;
			}

			Ptr<ManagedExpression> ToString(const RegexToken& input)
			{
				Ptr<ManagedStringExpression> exp=CreateNode<ManagedStringExpression>(input);
				WString value(input.reading+1, input.length-2);
				exp->value=EscapeString(value);
				return exp;
			}

/***********************************************************************
Left Recursion Helper
***********************************************************************/

			namespace typelrec
			{
				class TypeLrecBase : public Object
				{
				public:
					virtual Ptr<ManagedType>			Set(Ptr<ManagedType> operand)=0;
				};

				template<typename T>
				class TypeLrec{};

#define TYPE_LREC(TYPE, MEMBER)\
				template<>\
				class TypeLrec<TYPE> : public TypeLrecBase\
				{\
				protected:\
					Ptr<TYPE>							type;\
				public:\
					TypeLrec(Ptr<TYPE> _type)\
						:type(_type)\
					{\
					}\
					Ptr<ManagedType> Set(Ptr<ManagedType> operand)\
					{\
						type->MEMBER=operand;\
						return type;\
					}\
				};

				TYPE_LREC(ManagedMemberType, operand)
				TYPE_LREC(ManagedInstantiatedGenericType, elementType)
				TYPE_LREC(ManagedArrayType, elementType)
#undef TYPE_LREC

				template<typename T>
				Ptr<TypeLrecBase> ToTypeLrec(Ptr<T> exp)
				{
					return new TypeLrec<T>(exp);
				}
			}

			namespace explrec
			{
				class ExpLrecBase : public Object
				{
				public:
					virtual Ptr<ManagedExpression>		Set(Ptr<ManagedExpression> operand)=0;
				};

				template<typename T>
				class ExpLrec{};

#define EXP_LREC(TYPE, MEMBER)\
				template<>\
				class ExpLrec<TYPE> : public ExpLrecBase\
				{\
				protected:\
					Ptr<TYPE>							exp;\
				public:\
					ExpLrec(Ptr<TYPE> _exp)\
						:exp(_exp)\
					{\
					}\
					Ptr<ManagedExpression> Set(Ptr<ManagedExpression> operand)\
					{\
						exp->MEMBER=operand;\
						return exp;\
					}\
				};

				EXP_LREC(ManagedMemberExpression, operand)
				EXP_LREC(ManagedInstantiatedExpression, operand)
				EXP_LREC(ManagedInvokeExpression, function)
				EXP_LREC(ManagedUnaryExpression, operand)
				EXP_LREC(ManagedIndexExpression, operand)
#undef EXP_LREC

				template<typename T>
				Ptr<ExpLrecBase> ToExpLrec(Ptr<T> exp)
				{
					return new ExpLrec<T>(exp);
				}
			}

/***********************************************************************
Basic Types
***********************************************************************/

			Ptr<ManagedType> ToKeywordType(const RegexToken& input)
			{
				WString name;
				WString keyword(input.reading, input.length);
				if(keyword==L"sbyte")			name = L"SInt8";
				else if(keyword==L"byte")		name = L"UInt8";
				else if(keyword==L"short")		name = L"SInt16";
				else if(keyword==L"word")		name = L"UInt16";
				else if(keyword==L"int")		name = L"SInt32";
				else if(keyword==L"uint")		name = L"UInt32";
				else if(keyword==L"long")		name = L"SInt64";
				else if(keyword==L"ulong")		name = L"UInt64";
				else if(keyword==L"char")		name = L"Char";
				else if(keyword==L"string")		name = L"String";
				else if(keyword==L"float")		name = L"Single";
				else if(keyword==L"double")		name = L"Double";
				else if(keyword==L"bool")		name = L"Bool";
				else if(keyword==L"object")		name = L"Object";
				else if(keyword==L"void")		name = L"Void";
#ifdef VCZH_64
				else if(keyword==L"intptr")		name = L"SInt64";
				else if(keyword==L"uintptr")	name = L"UInt64";
#else
				else if(keyword==L"intptr")		name = L"SInt32";
				else if(keyword==L"uintptr")	name = L"UInt32";
#endif

				Ptr<ManagedMemberType> system=CreateNode<ManagedMemberType>(input);
				system->member=L"System";

				Ptr<ManagedMemberType> type=CreateNode<ManagedMemberType>(input);
				type->operand=system;
				type->member=name;
				return type;
			}

			Ptr<ManagedType> ToReferenceType(const RegexToken& input)
			{
				Ptr<ManagedReferencedType> type=CreateNode<ManagedReferencedType>(input);
				type->name=ConvertID(WString(input.reading, input.length));
				return type;
			}

			Ptr<ManagedType> ToGlobalType(const RegexToken& input)
			{
				Ptr<ManagedMemberType> type=CreateNode<ManagedMemberType>(input);
				type->member=WString(input.reading, input.length);
				return type;
			}

			Ptr<ManagedType> ToLrecType(const Ptr<ManagedType>& elementType, const Ptr<typelrec::TypeLrecBase>& decoratorType)
			{
				return decoratorType->Set(elementType);
			}

			Ptr<typelrec::TypeLrecBase> ToMemberTypeLrec(const RegexToken& input)
			{
				Ptr<ManagedMemberType> type=CreateNode<ManagedMemberType>(input);
				type->member=ConvertID(WString(input.reading, input.length));
				return typelrec::ToTypeLrec(type);
			}

			Ptr<typelrec::TypeLrecBase> ToInstantiatedGenericTypeLrec(const ParsingPair<RegexToken, ParsingList<Ptr<ManagedType>>>& input)
			{
				Ptr<ManagedInstantiatedGenericType> type=CreateNode<ManagedInstantiatedGenericType>(input.First());
				Ptr<ParsingList<Ptr<ManagedType>>::Node> current=input.Second().Head();
				while(current)
				{
					type->argumentTypes.Add(current->Value());
					current=current->Next();
				}
				return typelrec::ToTypeLrec(type);
			}

/***********************************************************************
Extended Types
***********************************************************************/

			Ptr<typelrec::TypeLrecBase> ToArrayType(const ParsingPair<RegexToken, ParsingList<RegexToken>>& input)
			{
				Ptr<ManagedArrayType> type=CreateNode<ManagedArrayType>(input.First());
				Ptr<ParsingList<RegexToken>::Node> current=input.Second().Head();
				int count=1;
				while(current)
				{
					count++;
					current=current->Next();
				}
				type->dimensionCount=count;
				return typelrec::ToTypeLrec(type);
			}

			Ptr<ManagedType> ToFunctionType(const ParsingPair<ParsingPair<RegexToken, Ptr<ManagedType>>, ParsingList<Ptr<ManagedType>>>& input)
			{
				Ptr<ManagedFunctionType> type=CreateNode<ManagedFunctionType>(input.First().First());
				type->returnType=input.First().Second();
				Ptr<ParsingList<Ptr<ManagedType>>::Node> current=input.Second().Head();
				while(current)
				{
					type->parameterTypes.Add(current->Value());
					current=current->Next();
				}
				return type;
			}

			Ptr<ManagedType> ToEventType(const ParsingPair<RegexToken, ParsingList<Ptr<ManagedType>>>& input)
			{
				Ptr<ManagedEventType> type=CreateNode<ManagedEventType>(input.First());
				Ptr<ParsingList<Ptr<ManagedType>>::Node> current=input.Second().Head();
				while(current)
				{
					type->parameterTypes.Add(current->Value());
					current=current->Next();
				}
				return type;
			}

			Ptr<ManagedType> ToAutoReferType(const RegexToken& input)
			{
				return CreateNode<ManagedAutoReferType>(input);
			}

			Ptr<ManagedType> ToDynamicType(const RegexToken& input)
			{
				return CreateNode<ManagedDynamicType>(input);
			}

/***********************************************************************
Basic Expressions
***********************************************************************/

			Ptr<ManagedExpression> ToKeywordExpression(const RegexToken& input)
			{
				WString name;
				WString keyword(input.reading, input.length);
				if(keyword==L"sbyte")			name = L"SInt8";
				else if(keyword==L"byte")		name = L"UInt8";
				else if(keyword==L"short")		name = L"SInt16";
				else if(keyword==L"word")		name = L"UInt16";
				else if(keyword==L"int")		name = L"SInt32";
				else if(keyword==L"uint")		name = L"UInt32";
				else if(keyword==L"long")		name = L"SInt64";
				else if(keyword==L"ulong")		name = L"UInt64";
				else if(keyword==L"char")		name = L"Char";
				else if(keyword==L"string")		name = L"String";
				else if(keyword==L"float")		name = L"Single";
				else if(keyword==L"double")		name = L"Double";
				else if(keyword==L"bool")		name = L"Bool";
				else if(keyword==L"object")		name = L"Object";
				else if(keyword==L"void")		name = L"Void";
#ifdef VCZH_64
				else if(keyword==L"intptr")		name = L"SInt64";
				else if(keyword==L"uintptr")	name = L"UInt64";
#else
				else if(keyword==L"intptr")		name = L"SInt32";
				else if(keyword==L"uintptr")	name = L"UInt32";
#endif

				Ptr<ManagedMemberExpression> system=CreateNode<ManagedMemberExpression>(input);
				system->member=L"System";

				Ptr<ManagedMemberExpression> exp=CreateNode<ManagedMemberExpression>(input);
				exp->operand=system;
				exp->member=name;
				return exp;
			}

			Ptr<ManagedExpression> ToReferenceExpression(const RegexToken& input)
			{
				Ptr<ManagedReferenceExpression> exp=CreateNode<ManagedReferenceExpression>(input);
				exp->name=ConvertID(WString(input.reading, input.length));
				return exp;
			}

			Ptr<ManagedExpression> ToGlobalExpression(const RegexToken& input)
			{
				Ptr<ManagedMemberExpression> exp=CreateNode<ManagedMemberExpression>(input);
				exp->member=WString(input.reading, input.length);
				return exp;
			}

			Ptr<ManagedExpression> ToLrecExpression(const Ptr<ManagedExpression>& operand, const Ptr<explrec::ExpLrecBase>& decoratorExpression)
			{
				return decoratorExpression->Set(operand);
			}

			Ptr<explrec::ExpLrecBase> ToMemberExpressionLrec(const RegexToken& input)
			{
				Ptr<ManagedMemberExpression> exp=CreateNode<ManagedMemberExpression>(input);
				exp->member=ConvertID(WString(input.reading, input.length));
				return explrec::ToExpLrec(exp);
			}

			Ptr<explrec::ExpLrecBase> ToInstantiatedExpressionLrec(const ParsingPair<RegexToken, ParsingList<Ptr<ManagedType>>>& input)
			{
				Ptr<ManagedInstantiatedExpression> exp=CreateNode<ManagedInstantiatedExpression>(input.First());
				Ptr<ParsingList<Ptr<ManagedType>>::Node> current=input.Second().Head();
				while(current)
				{
					exp->argumentTypes.Add(current->Value());
					current=current->Next();
				}
				return explrec::ToExpLrec(exp);
			}

			Ptr<ManagedArgument> ToArgument(const ParsingPair<ParsingPair<
				ManagedArgument::ArgumentType,
				ParsingList<RegexToken>>,
				Ptr<ManagedExpression>>& input)
			{
				Ptr<ManagedArgument> arg=CreateNode<ManagedArgument>(input.Second());
				arg->argumentType=input.First().First();
				if(input.First().Second().Head())
				{
					RegexToken name=input.First().Second().Head()->Value();
					arg->defaultParameterName=ConvertID(WString(name.reading, name.length));
				}
				arg->value=input.Second();
				return arg;
			}

			Ptr<explrec::ExpLrecBase> ToInvokeLrec(const ParsingPair<
				RegexToken,
				ParsingList<Ptr<ManagedArgument>>>& input)
			{
				Ptr<ManagedInvokeExpression> exp=CreateNode<ManagedInvokeExpression>(input.First());
				Ptr<ParsingList<Ptr<ManagedArgument>>::Node> current=input.Second().Head();
				while(current)
				{
					exp->arguments.Add(current->Value());
					current=current->Next();
				}
				return explrec::ToExpLrec(exp);
			}

			Ptr<ManagedPropertySetter> ToPropertySetter(const ParsingPair<RegexToken, Ptr<ManagedExpression>>& input)
			{
				Ptr<ManagedPropertySetter> setter=CreateNode<ManagedPropertySetter>(input.First());
				setter->propertyName=ConvertID(WString(input.First().reading, input.First().length));
				setter->value=input.Second();
				return setter;
			}

			Ptr<ManagedExpression> ToNewObject(const ParsingPair<ParsingPair<ParsingPair<
				RegexToken,
				Ptr<ManagedType>>,
				ParsingList<Ptr<ManagedArgument>>>,
				ParsingList<ParsingList<Ptr<ManagedPropertySetter>>>>& input)
			{
				Ptr<ManagedNewObjectExpression> exp=CreateNode<ManagedNewObjectExpression>(input.First().First().First());
				exp->objectType=input.First().First().Second();
				{
					Ptr<ParsingList<Ptr<ManagedArgument>>::Node> current=input.First().Second().Head();
					while(current)
					{
						exp->arguments.Add(current->Value());
						current=current->Next();
					}
				}
				if(input.Second().Head())
				{
					Ptr<ParsingList<Ptr<ManagedPropertySetter>>::Node> current=input.Second().Head()->Value().Head();
					while(current)
					{
						exp->properties.Add(current->Value());
						current=current->Next();
					}
				}
				return exp;
			}

			Ptr<ManagedExpression> ToThis(const RegexToken& input)
			{
				return CreateNode<ManagedThisExpression>(input);
			}

			Ptr<ManagedExpression> ToBase(const RegexToken& input)
			{
				return CreateNode<ManagedBaseExpression>(input);
			}

			Ptr<ManagedExpression> ToResult(const RegexToken& input)
			{
				return CreateNode<ManagedFunctionResultExpression>(input);
			}

/***********************************************************************
Extended Expressions
***********************************************************************/

			Ptr<explrec::ExpLrecBase> ToUnaryLrec(const RegexToken& input)
			{
				Ptr<ManagedUnaryExpression> exp=CreateNode<ManagedUnaryExpression>(input);
				if(input==L"++")
				{
					exp->operatorName=L"op_postinc";
				}
				else if(input==L"--")
				{
					exp->operatorName=L"op_postdec";
				}
				else
				{
					CHECK_ERROR(false, L"ToUnaryLrec(const RegexToken&)#Î´Öª²Ù×÷·û¡£");
				}
				return explrec::ToExpLrec(exp);

			}

			Ptr<ManagedExpression> ToSetterValue(const RegexToken& input)
			{
				return CreateNode<ManagedSetterValueExpression>(input);
			}

			Ptr<ManagedExpression> ToTypeof(const ParsingPair<RegexToken, Ptr<ManagedType>>& input)
			{
				Ptr<ManagedTypeofExpression> exp=CreateNode<ManagedTypeofExpression>(input.First());
				exp->type=input.Second();
				return exp;
			}

			Ptr<ManagedExpression> ToNewArray(const ParsingPair<ParsingPair<
				RegexToken,
				Ptr<ManagedType>>,
				ParsingList<Ptr<ManagedExpression>>>& input)
			{
				Ptr<ManagedNewArrayExpression> exp=CreateNode<ManagedNewArrayExpression>(input.First().First());
				exp->objectType=input.First().Second();
				Ptr<ParsingList<Ptr<ManagedExpression>>::Node> current=input.Second().Head();
				while(current)
				{
					exp->sizes.Add(current->Value());
					current=current->Next();
				}
				return exp;
			}

			Ptr<explrec::ExpLrecBase> ToIndexLrec(const ParsingPair<RegexToken, ParsingList<Ptr<ManagedExpression>>>& input)
			{
				Ptr<ManagedIndexExpression> exp=CreateNode<ManagedIndexExpression>(input.First());
				Ptr<ParsingList<Ptr<ManagedExpression>>::Node> current=input.Second().Head();
				while(current)
				{
					exp->indices.Add(current->Value());
					current=current->Next();
				}
				return explrec::ToExpLrec(exp);
			}

/***********************************************************************
Extended Expressions (Lambda Expression)
***********************************************************************/

			Ptr<ManagedLambdaParameter> ToLambdaTypedParameter(const ParsingPair<ParsingList<Ptr<ManagedType>>, RegexToken>& input)
			{
				RegexToken name=input.Second();
				Ptr<ManagedLambdaParameter> parameter=new ManagedLambdaParameter;
				if(input.First().Head())
				{
					parameter->type=input.First().Head()->Value();
				}
				else
				{
					parameter->type=CreateNode<ManagedAutoReferType>(name);
				}
				parameter->name=ConvertID(WString(name.reading, name.length));
				return parameter;
			}

			Ptr<ManagedLambdaParameter> ToLambdaUntypedParameter(const RegexToken& input)
			{
				Ptr<ManagedLambdaParameter> parameter=new ManagedLambdaParameter;
				parameter->type=CreateNode<ManagedAutoReferType>(input);
				parameter->name=ConvertID(WString(input.reading, input.length));
				return parameter;
			}

			Ptr<ManagedStatement> ToLambdaStatement(const Ptr<ManagedExpression>& input)
			{
				Ptr<ManagedAssignmentExpression> assign=CreateNode<ManagedAssignmentExpression>(input);
				assign->leftOperand=CreateNode<ManagedFunctionResultExpression>(input);
				assign->rightOperand=input;
				
				Ptr<ManagedExpressionStatement> exp=CreateNode<ManagedExpressionStatement>(input);
				exp->expression=assign;

				Ptr<ManagedCompositeStatement> stat=CreateNode<ManagedCompositeStatement>(input);
				stat->statements.Add(exp);
				stat->statements.Add(CreateNode<ManagedReturnStatement>(input));
				return stat;
			}

			Ptr<ManagedExpression> ToLambda1(const ParsingPair<ParsingPair<
				Ptr<ManagedLambdaParameter>, 
				RegexToken>,
				Ptr<ManagedStatement>>& input)
			{
				Ptr<ManagedLambdaExpression> exp=CreateNode<ManagedLambdaExpression>(input.First().Second());
				exp->returnType=CreateNode<ManagedAutoReferType>(input.Second());
				exp->parameters.Add(input.First().First());
				exp->body=input.Second();
				return exp;
			}

			Ptr<ManagedExpression> ToLambda2(const ParsingPair<ParsingPair<
				RegexToken,
				ParsingList<Ptr<ManagedLambdaParameter>>>,
				Ptr<ManagedStatement>>& input)
			{
				Ptr<ManagedLambdaExpression> exp=CreateNode<ManagedLambdaExpression>(input.First().First());
				exp->returnType=CreateNode<ManagedAutoReferType>(input.First().First());
				exp->body=input.Second();

				Ptr<ParsingList<Ptr<ManagedLambdaParameter>>::Node> current=input.First().Second().Head();
				while(current)
				{
					exp->parameters.Add(current->Value());
					current=current->Next();
				}
				return exp;
			}

			Ptr<ManagedExpression> ToLambda3(const ParsingPair<ParsingPair<ParsingPair<
				RegexToken,
				ParsingList<Ptr<ManagedType>>>,
				ParsingList<Ptr<ManagedLambdaParameter>>>,
				Ptr<ManagedStatement>>& input)
			{
				Ptr<ManagedLambdaExpression> exp=CreateNode<ManagedLambdaExpression>(input.First().First().First());
				if(input.First().First().Second().Head())
				{
					exp->returnType=input.First().First().Second().Head()->Value();
				}
				else
				{
					exp->returnType=CreateNode<ManagedAutoReferType>(input.First().First().First());
				}
				exp->body=input.Second();

				Ptr<ParsingList<Ptr<ManagedLambdaParameter>>::Node> current=input.First().Second().Head();
				while(current)
				{
					exp->parameters.Add(current->Value());
					current=current->Next();
				}
				return exp;
			}


/***********************************************************************
Operator Expressions
***********************************************************************/

			Ptr<ManagedExpression> ToCasting(Ptr<ManagedExpression> expression, ParsingPair<RegexToken, Ptr<ManagedType>> input)
			{
				Ptr<ManagedCastingExpression> exp=CreateNode<ManagedCastingExpression>(input.First());
				exp->type=input.Second();
				exp->operand=expression;
				return exp;
			}

			Ptr<ManagedExpression> ToAssignment(Ptr<ManagedExpression> left, RegexToken op, Ptr<ManagedExpression> right)
			{
				Ptr<ManagedAssignmentExpression> exp=CreateNode<ManagedAssignmentExpression>(op);
				exp->leftOperand=left;
				exp->rightOperand=right;
				return exp;
			}

			Ptr<ManagedExpression> ToIsType(Ptr<ManagedExpression> expression, ParsingPair<RegexToken, Ptr<ManagedType>> input)
			{
				Ptr<ManagedIsTypeExpression> exp=CreateNode<ManagedIsTypeExpression>(input.First());
				exp->type=input.Second();
				exp->operand=expression;
				return exp;
			}

			Ptr<ManagedExpression> ToChoice(Ptr<ManagedExpression> left, ParsingPair<RegexToken, Ptr<ManagedExpression>> op, Ptr<ManagedExpression> right)
			{
				Ptr<ManagedChoiceExpression> exp=CreateNode<ManagedChoiceExpression>(op.First());
				exp->condition=left;
				exp->trueExpression=op.Second();
				exp->falseExpression=right;
				return exp;
			}

			Ptr<ManagedExpression> ToNullChoice(Ptr<ManagedExpression> left, RegexToken op, Ptr<ManagedExpression> right)
			{
				Ptr<ManagedNullChoiceExpression> exp=CreateNode<ManagedNullChoiceExpression>(op);
				exp->valueExpression=left;
				exp->candidateExpression=right;
				return exp;
			}

			Ptr<ManagedExpression> ToPreUnary(RegexToken op, Ptr<ManagedExpression> expression)
			{
				Ptr<ManagedUnaryExpression> exp=CreateNode<ManagedUnaryExpression>(op);
				if(op==L"+")
				{
					exp->operatorName=L"op_pos";
				}
				else if(op==L"-")
				{
					exp->operatorName=L"op_neg";
				}
				else if(op==L"!")
				{
					exp->operatorName=L"op_not";
				}
				else if(op==L"~")
				{
					exp->operatorName=L"op_bitnot";
				}
				else if(op==L"++")
				{
					exp->operatorName=L"op_preinc";
				}
				else if(op==L"--")
				{
					exp->operatorName=L"op_predec";
				}
				else
				{
					CHECK_ERROR(false, L"ToUnary(RegexToken, Ptr<ManagedExpression>)#Î´Öª²Ù×÷·û¡£");
				}
				exp->operand=expression;
				return exp;
			}

			Ptr<ManagedExpression> ToBinary(Ptr<ManagedExpression> left, RegexToken op, Ptr<ManagedExpression> right)
			{
				Ptr<ManagedBinaryExpression> exp=CreateNode<ManagedBinaryExpression>(op);
				if(op==L"+")
				{
					exp->operatorName=L"op_add";
				}
				else if(op==L"-")
				{
					exp->operatorName=L"op_sub";
				}
				else if(op==L"*")
				{
					exp->operatorName=L"op_mul";
				}
				else if(op==L"/")
				{
					exp->operatorName=L"op_div";
				}
				else if(op==L"%")
				{
					exp->operatorName=L"op_mod";
				}
				else if(op==L"<")
				{
					exp->operatorName=L"op_lt";
				}
				else if(op==L"<=")
				{
					exp->operatorName=L"op_le";
				}
				else if(op==L">")
				{
					exp->operatorName=L"op_gt";
				}
				else if(op==L">=")
				{
					exp->operatorName=L"op_ge";
				}
				else if(op==L"==")
				{
					exp->operatorName=L"op_eq";
				}
				else if(op==L"!=")
				{
					exp->operatorName=L"op_ne";
				}
				else if(op==L"&&")
				{
					exp->operatorName=L"op_and";
				}
				else if(op==L"&")
				{
					exp->operatorName=L"op_bitand";
				}
				else if(op==L"||")
				{
					exp->operatorName=L"op_or";
				}
				else if(op==L"|")
				{
					exp->operatorName=L"op_bitor";
				}
				else if(op==L"^")
				{
					exp->operatorName=L"op_xor";
				}
				else
				{
					CHECK_ERROR(false, L"ToBinary(Ptr<ManagedExpression>, RegexToken, Ptr<ManagedExpression>)#Î´Öª²Ù×÷·û¡£");
				}
				exp->leftOperand=left;
				exp->rightOperand=right;
				return exp;
			}

			Ptr<ManagedExpression> ToBinaryShift(Ptr<ManagedExpression> left, RegexToken op, Ptr<ManagedExpression> right)
			{
				Ptr<ManagedBinaryExpression> exp=CreateNode<ManagedBinaryExpression>(op);
				if(op==L"<")
				{
					exp->operatorName=L"op_shl";
				}
				else if(op==L">")
				{
					exp->operatorName=L"op_shr";
				}
				else
				{
					CHECK_ERROR(false, L"ToBinaryShift(Ptr<ManagedExpression>, RegexToken, Ptr<ManagedExpression>)#Î´Öª²Ù×÷·û¡£");
				}
				exp->leftOperand=left;
				exp->rightOperand=right;
				return exp;
			}

			Ptr<ManagedExpression> ToBinaryEq(Ptr<ManagedExpression> left, RegexToken op, Ptr<ManagedExpression> right)
			{
				Ptr<ManagedBinaryExpression> exp=CreateNode<ManagedBinaryExpression>(op);
				if(op==L"+=")
				{
					exp->operatorName=L"op_add_eq";
				}
				else if(op==L"-=")
				{
					exp->operatorName=L"op_sub_eq";
				}
				else if(op==L"*=")
				{
					exp->operatorName=L"op_mul_eq";
				}
				else if(op==L"/=")
				{
					exp->operatorName=L"op_div_eq";
				}
				else if(op==L"%=")
				{
					exp->operatorName=L"op_mod_eq";
				}
				else if(op==L"&&=")
				{
					exp->operatorName=L"op_and_eq";
				}
				else if(op==L"&=")
				{
					exp->operatorName=L"op_bitand_eq";
				}
				else if(op==L"||=")
				{
					exp->operatorName=L"op_or_eq";
				}
				else if(op==L"|=")
				{
					exp->operatorName=L"op_bitor_eq";
				}
				else if(op==L"^=")
				{
					exp->operatorName=L"op_xor_eq";
				}
				else if(op==L"<<=")
				{
					exp->operatorName=L"op_shl_eq";
				}
				else if(op==L">>=")
				{
					exp->operatorName=L"op_shr_eq";
				}
				else
				{
					CHECK_ERROR(false, L"ToBinaryEq(Ptr<ManagedExpression>, RegexToken, Ptr<ManagedExpression>)#Î´Öª²Ù×÷·û¡£");
				}
				exp->leftOperand=left;
				exp->rightOperand=right;
				return exp;
			}

/***********************************************************************
Basic Statements
***********************************************************************/

			Ptr<ManagedStatement> ToEmptyStat(const RegexToken& input)
			{
				return CreateNode<ManagedEmptyStatement>(input);
			}

			Ptr<ManagedStatement> ToCompositeStat(const ParsingPair<RegexToken, ParsingList<Ptr<ManagedStatement>>>& input)
			{
				Ptr<ManagedCompositeStatement> stat=CreateNode<ManagedCompositeStatement>(input.First());
				Ptr<ParsingList<Ptr<ManagedStatement>>::Node> current=input.Second().Head();
				while(current)
				{
					stat->statements.Add(current->Value());
					current=current->Next();
				}
				return stat;
			}

			Ptr<ManagedStatement> ToExpressionStat(const Ptr<ManagedExpression>& input)
			{
				Ptr<ManagedExpressionStatement> stat=CreateNode<ManagedExpressionStatement>(input);
				stat->expression=input;
				return stat;
			}

			Ptr<ManagedStatement> ToReturnStat(const RegexToken& input)
			{
				return CreateNode<ManagedReturnStatement>(input);
			}

/***********************************************************************
Extended Statements
***********************************************************************/

/***********************************************************************
Basic Declaration Fragments
***********************************************************************/

			Ptr<ManagedGenericInfo::Argument> ToGenericArgument(const ParsingPair<
						ParsingPair<ManagedGenericInfo::ArgumentConversion, RegexToken>,
						ParsingList<ParsingList<Ptr<ManagedType>>>
					>& input)
			{
				Ptr<ManagedGenericInfo::Argument> argument=new ManagedGenericInfo::Argument;
				argument->newConstraint=false;
				argument->conversion=input.First().First();
				argument->name=WString(input.First().Second().reading, input.First().Second().length);
				if(input.Second().Head())
				{
					Ptr<ParsingList<Ptr<ManagedType>>::Node> type=input.Second().Head()->Value().Head();
					while(type)
					{
						if(type->Value())
						{
							argument->typeConstraints.Add(type->Value());
						}
						else
						{
							argument->newConstraint=true;
						}
						type=type->Next();
					}
				}
				return argument;
			}

			Ptr<ManagedGenericInfo> ToGenericInfo(const ParsingList<Ptr<ManagedGenericInfo::Argument>>& input)
			{
				Ptr<ManagedGenericInfo> info=new ManagedGenericInfo;
				Ptr<ParsingList<Ptr<ManagedGenericInfo::Argument>>::Node> current=input.Head();
				while(current)
				{
					info->arguments.Add(current->Value());
					current=current->Next();
				}
				return info;
			}

/***********************************************************************
Basic Declaration Members
***********************************************************************/

/***********************************************************************
Extended Declaration Members
***********************************************************************/

/***********************************************************************
Basic Declarations
***********************************************************************/

			Ptr<ManagedDeclaration> ToNamespaceDecl(const ParsingPair<ParsingPair<RegexToken, ParsingList<RegexToken>>, ParsingList<Ptr<ManagedDeclaration>>>& input)
			{
				Ptr<ManagedNamespaceDeclaration> decl=CreateNode<ManagedNamespaceDeclaration>(input.First().First());
				{
					Ptr<ParsingList<RegexToken>::Node> current=input.First().Second().Head();
					while(current)
					{
						decl->namespaceFragments.Add(WString(current->Value().reading, current->Value().length));
						current=current->Next();
					}
				}
				{
					Ptr<ParsingList<Ptr<ManagedDeclaration>>::Node> current=input.Second().Head();
					while(current)
					{
						decl->declarations.Add(current->Value());
						current=current->Next();
					}
				}
				return decl;
			}

/***********************************************************************
Extended Declarations
***********************************************************************/

			Ptr<ManagedEnumItem> ToEnumItem(const ParsingPair<RegexToken, ParsingList<Ptr<ManagedExpression>>>& input)
			{
				Ptr<ManagedEnumItem> item=CreateNode<ManagedEnumItem>(input.First());
				item->name=ConvertID(WString(input.First().reading, input.First().length));
				if(input.Second().Head())
				{
					item->value=input.Second().Head()->Value();
				}
				return item;
			}

			Ptr<ManagedDeclaration> ToEnum(const ParsingPair<ParsingPair<ParsingPair<ParsingPair<
				declatt::Accessor,
				RegexToken>,
				ParsingList<RegexToken>>,
				RegexToken>,
				ParsingList<Ptr<ManagedEnumItem>>>& input)
			{
				Ptr<ManagedEnumerationDeclaration> decl=CreateNode<ManagedEnumerationDeclaration>(input.First().First().First().Second());
				decl->accessor=input.First().First().First().First();
				decl->composable=input.First().First().Second().Head();
				decl->name=ConvertID(WString(input.First().Second().reading, input.First().Second().length));

				Ptr<ParsingList<Ptr<ManagedEnumItem>>::Node> current=input.Second().Head();
				while(current)
				{
					decl->items.Add(current->Value());
					current=current->Next();
				}
				return decl;
			}

			Ptr<ManagedDeclaration> ToTypeRenameDecl(const ParsingPair<ParsingPair<ParsingPair<
				ParsingList<Ptr<ManagedGenericInfo>>, 
				declatt::Accessor>, 
				RegexToken>, 
				Ptr<ManagedType>>& input)
			{
				Ptr<ManagedTypeRenameDeclaration> decl=CreateNode<ManagedTypeRenameDeclaration>(input.First().Second());
				if(input.First().First().First().Head())
				{
					Ptr<ManagedGenericInfo> genericInfo=input.First().First().First().Head()->Value();
					CopyFrom(decl->genericInfo.arguments.Wrap(), genericInfo->arguments.Wrap());
				}
				decl->accessor=input.First().First().Second();
				decl->name=WString(input.First().Second().reading, input.First().Second().length);
				decl->type=input.Second();
				return decl;
			}

			Ptr<ManagedDeclaration> ToUsingNamespaceDecl(const ParsingPair<RegexToken, ParsingList<RegexToken>>& input)
			{
				Ptr<ManagedUsingNamespaceDeclaration> decl=CreateNode<ManagedUsingNamespaceDeclaration>(input.First());
				Ptr<ParsingList<RegexToken>::Node> current=input.Second().Head();
				while(current)
				{
					decl->namespaceFragments.Add(WString(current->Value().reading, current->Value().length));
					current=current->Next();
				}
				return decl;
			}

/***********************************************************************
Unit
***********************************************************************/

			Ptr<ManagedXUnit> ToUnit(const ParsingList<Ptr<ManagedDeclaration>>& input)
			{
				Ptr<ManagedProgram> program=new ManagedProgram;
				Ptr<ParsingList<Ptr<ManagedDeclaration>>::Node> current=input.Head();
				while(current)
				{
					program->declarations.Add(current->Value());
					current=current->Next();
				}
				
				Ptr<ManagedXUnit> unit=new ManagedXUnit;
				unit->program=program;
				unit->codeIndex=-1;
				return unit;
			}

/***********************************************************************
Error Handlers
***********************************************************************/

#define ERROR_HANDLER(NAME, TYPE)																						\
			ParsingResult<TYPE> NAME(TokenInput<RegexToken>& input, Types<TokenInput<RegexToken>>::GlobalInfo& info)	\
			{																											\
				info.errors.Clear();																					\
				info.errors.Add(new CombinatorError<TokenInput<RegexToken>>(ManagedXErrorMessage::NAME(), input));		\
				return ParsingResult<TYPE>();																			\
			}
			
			ERROR_HANDLER(NeedId,							RegexToken)
				
			ERROR_HANDLER(NeedColon,						RegexToken)
			ERROR_HANDLER(NeedSemicolon,					RegexToken)
			ERROR_HANDLER(NeedLambda,						RegexToken)
			ERROR_HANDLER(NeedEq,							RegexToken)
			ERROR_HANDLER(NeedLt,							RegexToken)
			ERROR_HANDLER(NeedGt,							RegexToken)
			ERROR_HANDLER(NeedOpenDeclBrace,				RegexToken)
			ERROR_HANDLER(NeedCloseDeclBrace,				RegexToken)
			ERROR_HANDLER(NeedOpenArrayBrace,				RegexToken)
			ERROR_HANDLER(NeedCloseArrayBrace,				RegexToken)
			ERROR_HANDLER(NeedOpenExpBrace,					RegexToken)
			ERROR_HANDLER(NeedCloseExpBrace,				RegexToken)
			
			ERROR_HANDLER(NeedExpression,					RegexToken)
			ERROR_HANDLER(NeedStatement,					RegexToken)

/***********************************************************************
Óï·¨·ÖÎöÆ÷
***********************************************************************/

			typedef Node<TokenInput<RegexToken>, RegexToken>									TokenType;
			typedef Node<TokenInput<RegexToken>, declatt::Accessor>								AccessorNode;
			typedef Node<TokenInput<RegexToken>, declatt::Inheritation>							InheritationNode;
			typedef Node<TokenInput<RegexToken>, declatt::MemberType>							MemberTypeNode;
			typedef Node<TokenInput<RegexToken>, declatt::DataType>								DataTypeNode;
			typedef Node<TokenInput<RegexToken>, ManagedGenericInfo::ArgumentConversion>		GenericArgconv;
			typedef Node<TokenInput<RegexToken>, ManagedParameter::ParameterType>				FunctionArgconv;
			typedef Node<TokenInput<RegexToken>, ManagedArgument::ArgumentType>					InvokeArgconv;

			typedef Rule<TokenInput<RegexToken>, Ptr<ManagedType>>								TypeNode;
			
			typedef Node<TokenInput<RegexToken>, Ptr<ManagedArgument>>							ArgumentNode;
			typedef Node<TokenInput<RegexToken>, Ptr<ManagedLambdaParameter>>					LambdaParameterNode;
			typedef Node<TokenInput<RegexToken>, Ptr<ManagedPropertySetter>>					PropertySetterNode;
			typedef Node<TokenInput<RegexToken>, Ptr<ManagedExpression>>						IncompleteExpressionNode;
			typedef Rule<TokenInput<RegexToken>, Ptr<ManagedExpression>>						ExpressionNode;
			
			typedef Node<TokenInput<RegexToken>, Ptr<ManagedStatement>>							IncompleteStatementNode;
			typedef Rule<TokenInput<RegexToken>, Ptr<ManagedStatement>>							StatementNode;

			typedef Node<TokenInput<RegexToken>, Ptr<ManagedGenericInfo::Argument>>				GenericArgumentNode;
			typedef Node<TokenInput<RegexToken>, Ptr<ManagedGenericInfo>>						GenericInfoNode;
			typedef Node<TokenInput<RegexToken>, Ptr<ManagedEnumItem>>							EnumItemNode;
			typedef Rule<TokenInput<RegexToken>, Ptr<ManagedDeclaration>>						DeclarationNode;

			typedef Rule<TokenInput<RegexToken>, Ptr<ManagedXUnit>>								UnitRule;

			class ManagedXParserImpl : public ManagedXParser
			{
			protected:
				Ptr<RegexLexer>						lexer;

				/*--------KEYWORDS--------*/

				TokenType							TYPEKEYWORD, VAR, DYNAMIC, FUNCTION, EVENT;
				TokenType							SWITCH, THIS, BASE, NEW, VALUE, AS, IS, RESULT, TYPEOF;
				TokenType							EXIT;
				TokenType							GLOBAL, NAMESPACE, USING, GENERIC, ENUM;
				
				TokenType							IN, OUT, PARAMS, REF;
				TokenType							PUBLIC, PROTECTED, PRIVATE, INTERNAL;
				TokenType							SEALED, ABSTRACT, VIRTUAL, OVERRIDE;
				TokenType							STATIC, CONST, READONLY;

				/*--------OBJECTS--------*/

				TokenType							VCHAR, VSTRING, VINTEGER, VFLOAT, VNULL, VBOOLEAN, ID;

				/*--------SYMBOLS--------*/

				TokenType							DOT, COLON, SEMICOLON, COMMA, LAMBDA;
				TokenType							OPEQ;
				TokenType							LE, GE, EE, NE, EQ, LT, GT, QQ, QT;
				TokenType							ADD_SUB, MUL_DIV_MOD, NOT_BITNOT, INC_DEC;
				TokenType							AND, BITAND, OR, BITOR, XOR;
				TokenType							OPEN_DECL_BRACE, CLOSE_DECL_BRACE;
				TokenType							OPEN_ARRAY_BRACE, CLOSE_ARRAY_BRACE;
				TokenType							OPEN_EXP_BRACE, CLOSE_EXP_BRACE;

				/*--------RULES--------*/

				AccessorNode						accessor;
				InheritationNode					inheritation;
				MemberTypeNode						memberType;
				DataTypeNode						dataType;
				GenericArgconv						genericArgconv;
				FunctionArgconv						functionArgconv;
				InvokeArgconv						invokeArgconv;

				TypeNode							type, primitiveType, genericTypeConstraint;

				ArgumentNode						argument;
				LambdaParameterNode					lambdaUntypedParameter, lambdaParameter;
				PropertySetterNode					propertySetter;
				IncompleteExpressionNode			constant, primitiveExpression, exp0;
				ExpressionNode						expression;

				IncompleteStatementNode				compositeStatement, lambdaBody;
				StatementNode						statement;

				GenericArgumentNode					genericArgument;
				GenericInfoNode						genericInfo;
				EnumItemNode						enumItem;
				DeclarationNode						declaration;

				UnitRule							unit;

			public:
				ManagedXParserImpl()
				{
					List<WString> tokens;
					tokens.Add(L"/s+");
					tokens.Add(L"////[^\r\n]*(\r\n|\n)?");
					tokens.Add(L"///*([^*//]+|/*+[^//])*/*+//");

					/*--------KEYWORDS--------*/

					TYPEKEYWORD				= CreateToken(tokens, L"sbyte|byte|short|word|int|uint|long|ulong|char|string|float|double|bool|object|void|intptr|uintptr");
					VAR						= CreateToken(tokens, L"var");
					DYNAMIC					= CreateToken(tokens, L"dynamic");
					FUNCTION				= CreateToken(tokens, L"function");
					EVENT					= CreateToken(tokens, L"event");
					
					SWITCH					= CreateToken(tokens, L"switch");
					THIS					= CreateToken(tokens, L"this");
					BASE					= CreateToken(tokens, L"base");
					NEW						= CreateToken(tokens, L"new");
					VALUE					= CreateToken(tokens, L"value");
					AS						= CreateToken(tokens, L"as");
					IS						= CreateToken(tokens, L"is");
					RESULT					= CreateToken(tokens, L"result");
					TYPEOF					= CreateToken(tokens, L"typeof");

					EXIT					= CreateToken(tokens, L"exit");
										
					GLOBAL					= CreateToken(tokens, L"global");
					NAMESPACE				= CreateToken(tokens, L"namespace");
					USING					= CreateToken(tokens, L"using");
					GENERIC					= CreateToken(tokens, L"generic");
					ENUM					= CreateToken(tokens, L"enum");
					

					IN						= CreateToken(tokens, L"in");
					OUT						= CreateToken(tokens, L"out");
					PARAMS					= CreateToken(tokens, L"params");
					REF						= CreateToken(tokens, L"ref");
					PUBLIC					= CreateToken(tokens, L"public");
					PROTECTED				= CreateToken(tokens, L"protected");
					PRIVATE					= CreateToken(tokens, L"private");
					INTERNAL				= CreateToken(tokens, L"internal");
					SEALED					= CreateToken(tokens, L"sealed");
					ABSTRACT				= CreateToken(tokens, L"abstract");
					VIRTUAL					= CreateToken(tokens, L"virtual");
					OVERRIDE				= CreateToken(tokens, L"override");
					STATIC					= CreateToken(tokens, L"static");
					CONST					= CreateToken(tokens, L"const");
					READONLY				= CreateToken(tokens, L"readonly");

					/*--------OBJECTS--------*/
					VCHAR					= CreateToken(tokens, L"\'([^\\\\\']|\\\\\\.)\'");
					VSTRING					= CreateToken(tokens, L"\"([^\\\\\"]|\\\\\\.)*\"");
					VFLOAT					= CreateToken(tokens, L"/d+./d+");
					VINTEGER				= CreateToken(tokens, L"/d+");
					VNULL					= CreateToken(tokens, L"null");
					VBOOLEAN				= CreateToken(tokens, L"true|false");
					ID						= CreateToken(tokens, L"(@?[a-zA-Z_]/w*)|(@\"([^\"]|\\\\\\.)*\")");

					/*--------SYMBOLS--------*/

					DOT						= CreateToken(tokens, L".");
					COLON					= CreateToken(tokens, L":");
					SEMICOLON				= CreateToken(tokens, L";");
					COMMA					= CreateToken(tokens, L",");
					LAMBDA					= CreateToken(tokens, L"=>");
					
					OPEQ					= CreateToken(tokens, L"(/+|-|/*|//|%|&&|&|/|/||/||/^|<<|>>)=");

					LE						= CreateToken(tokens, L">=");
					GE						= CreateToken(tokens, L"<=");
					EE						= CreateToken(tokens, L"==");
					NE						= CreateToken(tokens, L"!=");
					EQ						= CreateToken(tokens, L"=");
					LT						= CreateToken(tokens, L"<");
					GT						= CreateToken(tokens, L">");
					QQ						= CreateToken(tokens, L"/?/?");
					QT						= CreateToken(tokens, L"/?");
					
					INC_DEC					= CreateToken(tokens, L"/+/+|--");
					NOT_BITNOT				= CreateToken(tokens, L"!|~");
					ADD_SUB					= CreateToken(tokens, L"/+|-");
					MUL_DIV_MOD				= CreateToken(tokens, L"/*|//|%");

					AND						= CreateToken(tokens, L"&&");
					BITAND					= CreateToken(tokens, L"&");
					OR						= CreateToken(tokens, L"/|/|");
					BITOR					= CreateToken(tokens, L"/|");
					XOR						= CreateToken(tokens, L"/^");

					OPEN_DECL_BRACE			= CreateToken(tokens, L"/{");
					CLOSE_DECL_BRACE		= CreateToken(tokens, L"/}");
					OPEN_ARRAY_BRACE		= CreateToken(tokens, L"/[");
					CLOSE_ARRAY_BRACE		= CreateToken(tokens, L"/]");
					OPEN_EXP_BRACE			= CreateToken(tokens, L"/(");
					CLOSE_EXP_BRACE			= CreateToken(tokens, L"/)");

					/*--------LEXICAL ANALYZER--------*/

					lexer=new RegexLexer(tokens.Wrap());

					/*--------ATTRIBUTES--------*/

					accessor				= def(	let(PUBLIC, declatt::Public)
												|	let(PROTECTED+INTERNAL, declatt::ProtectedInternal)
												|	let(PROTECTED, declatt::Protected)
												|	let(PRIVATE, declatt::Private)
												|	let(INTERNAL, declatt::Internal)
												,	declatt::Private);

					inheritation			= def(	let(SEALED, declatt::Sealed)
												|	let(ABSTRACT, declatt::Abstract)
												|	let(VIRTUAL, declatt::Virtual)
												|	let(OVERRIDE, declatt::Override)
												,	declatt::Normal);

					memberType				= def(let(STATIC, declatt::Static), declatt::Instance);

					dataType				= def(	let(CONST, declatt::Constant)
												|	let(READONLY, declatt::Readonly)
												,	declatt::Variable);

					genericArgconv			= def(	let(IN, ManagedGenericInfo::In)
												|	let(OUT, ManagedGenericInfo::Out)
												,	ManagedGenericInfo::None);

					functionArgconv			= def(	let(PARAMS, ManagedParameter::Params)
												|	let(REF, ManagedParameter::Ref)
												|	let(OUT, ManagedParameter::Out)
												,	ManagedParameter::Normal);

					invokeArgconv			= def(	let(REF, ManagedArgument::Ref)
												|	let(OUT, ManagedArgument::Out)
												,	ManagedArgument::Normal);

					/*--------TYPES--------*/

					primitiveType			= TYPEKEYWORD[ToKeywordType]
											| ID(NeedId)[ToReferenceType]
											| VAR[ToAutoReferType]
											| DYNAMIC[ToDynamicType]
											| (FUNCTION + type + (OPEN_EXP_BRACE(NeedOpenExpBrace) >> plist(opt(type + *(COMMA >> type))) << CLOSE_EXP_BRACE(NeedCloseExpBrace)))[ToFunctionType]
											| (EVENT + (OPEN_EXP_BRACE(NeedOpenExpBrace) >> plist(opt(type + *(COMMA >> type))) << CLOSE_EXP_BRACE(NeedCloseExpBrace)))[ToEventType]
											| ((GLOBAL + COLON(NeedColon) + COLON(NeedColon)) >> ID(NeedId))[ToGlobalType]
											;

					type					= lrec(primitiveType + *( (DOT >> ID)[ToMemberTypeLrec]
																	| ((LT + plist(type + *(COMMA >> type))) << GT(NeedGt))[ToInstantiatedGenericTypeLrec]
																	| ((OPEN_ARRAY_BRACE + *COMMA) << CLOSE_ARRAY_BRACE(NeedCloseArrayBrace))[ToArrayType]
																	), ToLrecType);

					/*--------STATENENTS--------*/

					compositeStatement		= ((OPEN_DECL_BRACE(NeedStatement) + *statement) << CLOSE_DECL_BRACE(NeedCloseDeclBrace))[ToCompositeStat];

					lambdaBody				= expression[ToLambdaStatement]
											| compositeStatement
											;

					statement				= compositeStatement
											| SEMICOLON(NeedStatement)[ToEmptyStat]
											| (EXIT << SEMICOLON(NeedSemicolon))[ToReturnStat]
											| (expression << SEMICOLON(NeedSemicolon))[ToExpressionStat]
											;

					/*--------EXPRESSIONS--------*/

					argument				= (invokeArgconv + opt(ID << COLON(NeedColon)) + expression)[ToArgument];

					lambdaUntypedParameter	= ID(NeedId)[ToLambdaUntypedParameter];
					lambdaParameter			= (opt(type) + ID(NeedId))[ToLambdaTypedParameter]
											| lambdaUntypedParameter;

					propertySetter			= (ID(NeedId) + (EQ(NeedEq) >> expression))[ToPropertySetter];

					constant				= VNULL[ToNull] | VBOOLEAN[ToBoolean] | VINTEGER[ToInteger] | VFLOAT[ToFloat] | VCHAR[ToChar] | VSTRING[ToString];
					
					primitiveExpression		= (lambdaUntypedParameter + LAMBDA(NeedLambda) + lambdaBody)[ToLambda1]
											| (
												((OPEN_EXP_BRACE + plist(opt(lambdaUntypedParameter + *(COMMA >> lambdaUntypedParameter)))) << CLOSE_EXP_BRACE(NeedCloseExpBrace)) + 
												(LAMBDA(NeedLambda) >> lambdaBody)
											  )[ToLambda2]
											| (FUNCTION + opt(type) + (
												OPEN_EXP_BRACE(NeedOpenExpBrace) >> 
													plist(opt(lambdaParameter + *(COMMA >> lambdaParameter)))
												<< CLOSE_EXP_BRACE(NeedCloseExpBrace)
												) +
												compositeStatement
											  )[ToLambda3]
											| (OPEN_EXP_BRACE >> expression << CLOSE_EXP_BRACE(NeedCloseExpBrace))
											| (constant
											| TYPEKEYWORD[ToKeywordExpression]
											| THIS[ToThis]
											| BASE[ToBase]
											| RESULT[ToResult]
											| VALUE[ToSetterValue]
											| ID(NeedExpression)[ToReferenceExpression]
											| ((GLOBAL + COLON(NeedColon) + COLON(NeedColon)) >> ID(NeedId))[ToGlobalExpression]
											| (NEW + type + 
												(OPEN_EXP_BRACE(NeedOpenExpBrace) >> plist(opt(argument + *(COMMA >> argument))) << CLOSE_EXP_BRACE(NeedCloseExpBrace)) +
												opt(OPEN_DECL_BRACE(NeedOpenDeclBrace) >> plist(opt(propertySetter + *(COMMA >> propertySetter))) << CLOSE_DECL_BRACE(NeedCloseDeclBrace))
											  )[ToNewObject])
											| (NEW + type + 
												(OPEN_ARRAY_BRACE(NeedOpenArrayBrace) >> plist(expression + *(COMMA >> expression)) << CLOSE_ARRAY_BRACE(NeedCloseArrayBrace))
											  )[ToNewArray]
											| (TYPEOF + (OPEN_EXP_BRACE(NeedOpenExpBrace) >> type << CLOSE_EXP_BRACE(NeedCloseExpBrace)))[ToTypeof]
											;

					exp0					= lrec(primitiveExpression +   *( (DOT >> ID)[ToMemberExpressionLrec]
																			| ((LT + plist(type + *(COMMA >> type))) << GT(NeedGt))[ToInstantiatedExpressionLrec]
																			| (OPEN_EXP_BRACE(NeedOpenExpBrace) + plist(opt(argument + *(COMMA >> argument))) << CLOSE_EXP_BRACE(NeedCloseExpBrace))[ToInvokeLrec]
																			| (OPEN_ARRAY_BRACE(NeedOpenArrayBrace) + plist(expression + *(COMMA >> expression)) << CLOSE_ARRAY_BRACE(NeedCloseArrayBrace))[ToIndexLrec]
																			| (INC_DEC)[ToUnaryLrec]
																			), ToLrecExpression);

					expression				= binop(exp0)
												.pre(ADD_SUB, ToPreUnary).pre(NOT_BITNOT, ToPreUnary).pre(INC_DEC, ToPreUnary).precedence()
												.lbin(MUL_DIV_MOD, ToBinary).precedence()
												.lbin(ADD_SUB, ToBinary).precedence()
												.lbin(LT << LT, ToBinaryShift).lbin(GT >> GT, ToBinaryShift).precedence()
												.lbin(LT, ToBinary).lbin(LE, ToBinary).lbin(GT, ToBinary).lbin(GE, ToBinary).precedence()
												.post(AS + type, ToCasting).post(IS + type, ToIsType).precedence()
												.lbin(EE, ToBinary).lbin(NE, ToBinary).precedence()
												.lbin(BITAND, ToBinary).precedence()
												.lbin(XOR, ToBinary).precedence()
												.lbin(BITOR, ToBinary).precedence()
												.lbin(AND, ToBinary).precedence()
												.lbin(OR, ToBinary).precedence()
												.lbin(QQ, ToNullChoice).precedence()
												.lbin(QT + (expression << COLON(NeedColon)), ToChoice).precedence()
												.rbin(OPEQ, ToBinaryEq).rbin(EQ, ToAssignment).precedence()
												;

					/*--------DECLARATION FRAGMENTS--------*/

					genericTypeConstraint	= type | let(NEW + OPEN_EXP_BRACE(NeedOpenExpBrace) + CLOSE_EXP_BRACE(NeedCloseExpBrace), Ptr<ManagedType>());

					genericArgument			= ((genericArgconv + ID(NeedId)) + opt(COLON(NeedColon) >> (
												OPEN_DECL_BRACE(NeedOpenDeclBrace) >> plist(genericTypeConstraint + *(COMMA >> genericTypeConstraint)) << CLOSE_DECL_BRACE(NeedCloseDeclBrace)
											)))[ToGenericArgument];

					genericInfo				= (GENERIC + LT(NeedLt) >> (
												plist(genericArgument + *(COMMA >> genericArgument))
											) << GT(NeedGt))[ToGenericInfo];

					/*--------DECLARATIONS--------*/

					enumItem				= (ID(NeedId) + opt(EQ >> expression))[ToEnumItem];

					declaration				= ((USING + plist(ID(NeedId) + *(DOT >> ID(NeedId))) << SEMICOLON(NeedSemicolon)))[ToUsingNamespaceDecl]
											| ((opt(genericInfo) + accessor + (USING >> ID(NeedId)) + (EQ(NeedEq) >> type)) << SEMICOLON(NeedSemicolon))[ToTypeRenameDecl]
											| (NAMESPACE + plist(ID(NeedId) + *(DOT >> ID(NeedId))) + (OPEN_DECL_BRACE(NeedOpenDeclBrace) >> *declaration << CLOSE_DECL_BRACE(NeedCloseDeclBrace)))[ToNamespaceDecl]
											| (accessor + ENUM + opt(SWITCH) + ID + (
												OPEN_DECL_BRACE(NeedOpenDeclBrace) >> plist(opt(enumItem + *(COMMA >> enumItem))) << CLOSE_DECL_BRACE(NeedCloseDeclBrace)
												))[ToEnum]
											;
					unit					= (*declaration)[ToUnit];
				}

				static bool Blank(vint token)
				{
					return token<3;
				}

				Ptr<ManagedXUnit> Parse(const WString& code, vint codeIndex, IList<Ptr<LanguageException>>& errors)
				{
					List<RegexToken> tokens;
					lexer->Parse(code, codeIndex).ReadToEnd(tokens, Blank);
					for(vint i=0;i<tokens.Count();i++)
					{
						if(tokens[i].token==-1)
						{
							WString message=ManagedXErrorMessage::UnrecognizedToken(WString(tokens[i].reading, tokens[i].length));
							errors.Add(new LanguageException(message, tokens[i].lineIndex, tokens[i].lineStart, tokens[i].start, codeIndex));
							return 0;
						}
					}

					TokenInput<RegexToken> input(&tokens[0], tokens.Count());
					try
					{
						Ptr<ManagedXUnit> result=unit.ParseFull(input, false);
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

			Ptr<ManagedXParser> ManagedXParser::Create()
			{
				return new ManagedXParserImpl;
			}
		}
	}
}