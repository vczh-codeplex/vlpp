#include "ManagedXParser_Common.h"

namespace vl
{
	namespace scripting
	{
		namespace language_managedx
		{
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

			Ptr<ManagedStatement> ToBreakStat(const RegexToken& input)
			{
				return CreateNode<ManagedBreakStatement>(input);
			}

			Ptr<ManagedStatement> ToContinueStat(const RegexToken& input)
			{
				return CreateNode<ManagedContinueStatement>(input);
			}

			Ptr<ManagedStatement> ToReturnStat(const RegexToken& input)
			{
				return CreateNode<ManagedReturnStatement>(input);
			}

			Ptr<ManagedStatement> ToVarStat(const x::tp<
				x::opt<RegexToken>,
				Ptr<ManagedType>,
				RegexToken,
				x::opt<Ptr<ManagedExpression>>
				>::ResultType& input)
			{
				Ptr<ManagedVariableStatement> stat=CreateNode<ManagedVariableStatement>(input.First().Second());
				//stat->constant=input.First().First().First().Head();
				//stat->type=input.First().First().Second();
				//stat->name=ConvertID(WString(input.First().Second().reading, input.First().Second().length));
				//if(input.Second().Head())
				//{
				//	stat->initializer=input.Second().Head()->Value();
				//}
				Fill(
					x::ref(stat->constant)
					.ref(stat->type)
					.ref(stat->name)
					.ref(stat->initializer)
					, input);
				stat->name=ConvertID(stat->name);
				return stat;
			}

			Ptr<ManagedStatement> ToIfStat(const ParsingPair<ParsingPair<ParsingPair<
				RegexToken,
				Ptr<ManagedExpression>>,
				Ptr<ManagedStatement>>,
				ParsingList<Ptr<ManagedStatement>>>& input)
			{
				Ptr<ManagedIfStatement> stat=CreateNode<ManagedIfStatement>(input.First().First().First());
				stat->condition=input.First().First().Second();
				stat->trueStatement=input.First().Second();
				if(input.Second().Head())
				{
					stat->falseStatement=input.Second().Head()->Value();
				}
				return stat;
			}

			Ptr<ManagedStatement> ToWhileStat(const ParsingPair<ParsingPair<ParsingPair<
				RegexToken,
				Ptr<ManagedExpression>>,
				Ptr<ManagedStatement>>,
				ParsingList<Ptr<ManagedExpression>>>& input)
			{
				Ptr<ManagedWhileStatement> stat=CreateNode<ManagedWhileStatement>(input.First().First().First());
				stat->beginCondition=input.First().First().Second();
				stat->statement=input.First().Second();
				if(input.Second().Head())
				{
					stat->endCondition=input.Second().Head()->Value();
				}
				return stat;
			}

			Ptr<ManagedStatement> ToDoWhileStat(const ParsingPair<ParsingPair<
				RegexToken,
				Ptr<ManagedStatement>>,
				Ptr<ManagedExpression>>& input)
			{
				Ptr<ManagedWhileStatement> stat=CreateNode<ManagedWhileStatement>(input.First().First());
				stat->statement=input.First().Second();
				stat->endCondition=input.Second();
				return stat;
			}

			Ptr<ManagedVariableStatement> ToForInitializer(const ParsingPair<ParsingPair<
				Ptr<ManagedType>,
				RegexToken>,
				Ptr<ManagedExpression>>& input)
			{
				Ptr<ManagedVariableStatement> stat=CreateNode<ManagedVariableStatement>(input.First().Second());
				stat->constant=false;
				stat->type=input.First().First();
				stat->name=ConvertID(WString(input.First().Second().reading, input.First().Second().length));
				stat->initializer=input.Second();
				return stat;
			}

			Ptr<ManagedStatement> ToForStat(const ParsingPair<ParsingPair<ParsingPair<ParsingPair<
				RegexToken,
				ParsingList<Ptr<ManagedVariableStatement>>>,
				ParsingList<Ptr<ManagedExpression>>>,
				ParsingList<Ptr<ManagedExpression>>>,
				Ptr<ManagedStatement>>& input)
			{
				Ptr<ManagedForStatement> stat=CreateNode<ManagedForStatement>(input.First().First().First().First());
				{
					Ptr<ParsingList<Ptr<ManagedVariableStatement>>::Node> current=input.First().First().First().Second().Head();
					while(current)
					{
						stat->initializers.Add(current->Value());
						current=current->Next();
					}
				}
				if(input.First().First().Second().Head())
				{
					stat->condition=input.First().First().Second().Head()->Value();
				}
				{
					Ptr<ParsingList<Ptr<ManagedExpression>>::Node> current=input.First().Second().Head();
					while(current)
					{
						stat->sideEffects.Add(current->Value());
						current=current->Next();
					}
				}
				stat->statement=input.Second();
				return stat;
			}

			Ptr<ManagedCatchClause> ToCatchClause(const ParsingPair<ParsingPair<ParsingPair<
				RegexToken,
				Ptr<ManagedType>>,
				ParsingList<RegexToken>>,
				Ptr<ManagedStatement>>& input)
			{
				Ptr<ManagedCatchClause> clause=CreateNode<ManagedCatchClause>(input.First().First().First());
				clause->exceptionType=input.First().First().Second();
				if(input.First().Second().Head())
				{
					RegexToken name=input.First().Second().Head()->Value();
					clause->exceptionName=ConvertID(WString(name.reading, name.length));
				}
				clause->exceptionHandler=input.Second();
				return clause;
			}

			Ptr<ManagedStatement> ToTryStat(const ParsingPair<ParsingPair<ParsingPair<
				RegexToken,
				Ptr<ManagedStatement>>,
				ParsingList<Ptr<ManagedCatchClause>>>,
				ParsingList<Ptr<ManagedStatement>>>& input)
			{
				Ptr<ManagedTryCatchStatement> stat=CreateNode<ManagedTryCatchStatement>(input.First().First().First());
				stat->tryStatement=input.First().First().Second();

				Ptr<ParsingList<Ptr<ManagedCatchClause>>::Node> current=input.First().Second().Head();
				while(current)
				{
					stat->catches.Add(current->Value());
					current=current->Next();
				}

				if(input.Second().Head())
				{
					stat->finallyStatement=input.Second().Head()->Value();
				}
				return stat;
			}

			Ptr<ManagedStatement> ToThrowStat(const ParsingPair<
				RegexToken,
				ParsingList<Ptr<ManagedExpression>>>& input)
			{
				Ptr<ManagedThrowStatement> stat=CreateNode<ManagedThrowStatement>(input.First());
				if(input.Second().Head())
				{
					stat->expression=input.Second().Head()->Value();
				}
				return stat;
			}

/***********************************************************************
Extended Statements
***********************************************************************/

			Ptr<ManagedStatement> ToUsingStat(const ParsingPair<ParsingPair<ParsingPair<ParsingPair<
				RegexToken,
				Ptr<ManagedType>>,
				RegexToken>,
				Ptr<ManagedExpression>>,
				Ptr<ManagedStatement>>& input)
			{
				Ptr<ManagedUsingStatement> stat=CreateNode<ManagedUsingStatement>(input.First().First().First().First());
				stat->type=input.First().First().First().Second();
				RegexToken name=input.First().First().Second();
				stat->name=ConvertID(WString(name.reading, name.length));
				stat->initialize=input.First().Second();
				stat->statement=input.Second();
				return stat;
			}

			Ptr<ManagedStatement> ToLockStat(const ParsingPair<ParsingPair<
				RegexToken,
				Ptr<ManagedExpression>>,
				Ptr<ManagedStatement>>& input)
			{
				Ptr<ManagedLockStatement> stat=CreateNode<ManagedLockStatement>(input.First().First());
				stat->lock=input.First().Second();
				stat->statement=input.Second();
				return stat;
			}

			Ptr<ManagedCaseClause> ToCaseClause(const ParsingPair<
				ParsingList<Ptr<ManagedExpression>>,
				Ptr<ManagedStatement>>& input)
			{
				Ptr<ManagedCaseClause> clause=CreateNode<ManagedCaseClause>(input.Second());
				Ptr<ParsingList<Ptr<ManagedExpression>>::Node> current=input.First().Head();
				while(current)
				{
					clause->conditions.Add(current->Value());
					current=current->Next();
				}
				clause->statement=input.Second();
				return clause;
			}

			Ptr<ManagedStatement> ToSelectStat(const ParsingPair<ParsingPair<ParsingPair<
				RegexToken,
				Ptr<ManagedExpression>>,
				ParsingList<Ptr<ManagedCaseClause>>>,
				ParsingList<Ptr<ManagedStatement>>>& input)
			{
				Ptr<ManagedSelectStatement> stat=CreateNode<ManagedSelectStatement>(input.First().First().First());
				stat->expression=input.First().First().Second();
				Ptr<ParsingList<Ptr<ManagedCaseClause>>::Node> current=input.First().Second().Head();
				while(current)
				{
					stat->cases.Add(current->Value());
					current=current->Next();
				}
				if(input.Second().Head())
				{
					stat->defaultStatement=input.Second().Head()->Value();
				}
				return stat;
			}

			Ptr<ManagedStatement> ToSetResultAndExitStat(const ParsingPair<RegexToken, Ptr<ManagedExpression>>& input)
			{
				Ptr<ManagedCompositeStatement> stat=CreateNode<ManagedCompositeStatement>(input.First());
				{
					Ptr<ManagedAssignmentExpression> exp=CreateNode<ManagedAssignmentExpression>(input.First());
					exp->leftOperand=CreateNode<ManagedFunctionResultExpression>(input.First());
					exp->rightOperand=input.Second();

					Ptr<ManagedExpressionStatement> expstat=CreateNode<ManagedExpressionStatement>(input.First());
					expstat->expression=exp;
					stat->statements.Add(expstat);
				}
				stat->statements.Add(CreateNode<ManagedReturnStatement>(input.First()));
				return stat;
			}

/***********************************************************************
Lambda Statement
***********************************************************************/

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

/***********************************************************************
ManagedXParserImpl
***********************************************************************/

			void ManagedXParserImpl::LoadStatements()
			{
				/*--------STATENENTS--------*/

				compositeStatement		= ((OPEN_DECL_BRACE(NeedStatement) + *statement) << CLOSE_DECL_BRACE(NeedCloseDeclBrace))[ToCompositeStat];

				lambdaBody				= expression[ToLambdaStatement]
										| compositeStatement
										;

				forInitializer			= (type + ID(NeedId) + (EQ(NeedEq) >> expression))[ToForInitializer];

				catchClause				= ((CATCH << OPEN_EXP_BRACE(NeedOpenExpBrace)) + type + opt(ID) + (CLOSE_EXP_BRACE(NeedCloseExpBrace) >> statement))[ToCatchClause];

				caseClause				= (*(CASE >> expression << COLON(NeedColon)) + statement)[ToCaseClause];

				statement				= compositeStatement
										| SEMICOLON(NeedStatement)[ToEmptyStat]
										| (BREAK << SEMICOLON(NeedSemicolon))[ToBreakStat]
										| (CONTINUE << SEMICOLON(NeedSemicolon))[ToContinueStat]
										| (EXIT << SEMICOLON(NeedSemicolon))[ToReturnStat]
										| ((RETURN + expression) << SEMICOLON(NeedSemicolon))[ToSetResultAndExitStat]
										| (IF + (OPEN_EXP_BRACE(NeedOpenExpBrace) >> expression << CLOSE_EXP_BRACE(NeedCloseExpBrace)) + statement + opt(ELSE >> statement))[ToIfStat]
										| (WHILE + (OPEN_EXP_BRACE(NeedOpenExpBrace) >> expression << CLOSE_EXP_BRACE(NeedCloseExpBrace)) + statement + 
											opt(WHEN >> OPEN_EXP_BRACE(NeedOpenExpBrace) >> expression << CLOSE_EXP_BRACE(NeedCloseExpBrace) << SEMICOLON(NeedSemicolon))
											)[ToWhileStat]
										| (DO + statement + (WHILE(NeedWhile) >> (OPEN_EXP_BRACE(NeedOpenExpBrace) >> expression << CLOSE_EXP_BRACE(NeedCloseExpBrace) << SEMICOLON(NeedSemicolon))))[ToDoWhileStat]
										| (
											(FOR << OPEN_EXP_BRACE(NeedOpenExpBrace)) +
											(plist(opt(forInitializer + *(COMMA >> forInitializer))) << SEMICOLON(NeedSemicolon))
											+ opt(expression) +
											(SEMICOLON(NeedSemicolon) >> plist(opt(expression + *(COMMA >> expression))))
											+ (CLOSE_EXP_BRACE(NeedCloseExpBrace) >> statement)
											)[ToForStat]
										| (TRY + statement + *catchClause + opt(FINALLY >> statement))[ToTryStat]
										| (THROW + (opt(expression) << SEMICOLON(NeedSemicolon)))[ToThrowStat]
										| ((USING << OPEN_EXP_BRACE(NeedOpenExpBrace)) + type + ID(NeedId) + (EQ >> expression) + (CLOSE_EXP_BRACE(NeedCloseExpBrace) >> statement))[ToUsingStat]
										| (LOCK + (OPEN_EXP_BRACE(NeedOpenExpBrace) >> expression << CLOSE_EXP_BRACE(NeedCloseExpBrace)) + statement)[ToLockStat]
										| (SWITCH + 
											(OPEN_EXP_BRACE(NeedOpenExpBrace) >> expression << CLOSE_EXP_BRACE(NeedCloseExpBrace)) +
											(OPEN_DECL_BRACE(NeedOpenDeclBrace) >> *caseClause) +
											(opt(DEFAULT >> COLON(NeedColon) >> statement) << CLOSE_DECL_BRACE(NeedCloseDeclBrace))
										  )[ToSelectStat]
										| ((opt(CONST) + type + ID(NeedId) + opt(EQ >> expression))<<SEMICOLON(NeedSemicolon))[ToVarStat]
										| (expression << SEMICOLON(NeedSemicolon))[ToExpressionStat]
										;
			}
		}
	}
}