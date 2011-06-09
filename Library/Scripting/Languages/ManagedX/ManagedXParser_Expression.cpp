#include "ManagedXParser_Common.h"

namespace vl
{
	namespace scripting
	{
		namespace language_managedx
		{
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
					CHECK_ERROR(false, L"ToUnaryLrec(const RegexToken&)#未知操作符。");
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

			Ptr<ManagedExpression> ToDefault(const ParsingPair<RegexToken, Ptr<ManagedType>>& input)
			{
				Ptr<ManagedDefaultExpression> exp=CreateNode<ManagedDefaultExpression>(input.First());
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
					CHECK_ERROR(false, L"ToUnary(RegexToken, Ptr<ManagedExpression>)#未知操作符。");
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
					CHECK_ERROR(false, L"ToBinary(Ptr<ManagedExpression>, RegexToken, Ptr<ManagedExpression>)#未知操作符。");
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
					CHECK_ERROR(false, L"ToBinaryShift(Ptr<ManagedExpression>, RegexToken, Ptr<ManagedExpression>)#未知操作符。");
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
					CHECK_ERROR(false, L"ToBinaryEq(Ptr<ManagedExpression>, RegexToken, Ptr<ManagedExpression>)#未知操作符。");
				}
				exp->leftOperand=left;
				exp->rightOperand=right;
				return exp;
			}

/***********************************************************************
ManagedXParserImpl
***********************************************************************/

			void ManagedXParserImpl::LoadExpressions()
			{
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
										| (DEFAULT + (OPEN_EXP_BRACE(NeedOpenExpBrace) >> type << CLOSE_EXP_BRACE(NeedCloseExpBrace)))[ToDefault]
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
			}
		}
	}
}