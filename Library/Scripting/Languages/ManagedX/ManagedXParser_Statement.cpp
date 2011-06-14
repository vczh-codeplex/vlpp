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
				x::Fill(
					x::ref(stat->constant)
					.ref(stat->type)
					.ref(stat->name)
					.ref(stat->initializer)
					, input);
				stat->name=ConvertID(stat->name);
				return stat;
			}

			Ptr<ManagedStatement> ToIfStat(const x::tp<
					RegexToken,
					Ptr<ManagedExpression>,
					Ptr<ManagedStatement>,
					x::opt<Ptr<ManagedStatement>>
				>::ResultType& input)
			{
				Ptr<ManagedIfStatement> stat=CreateNode<ManagedIfStatement>(input.First().First().First());
				x::Fill(
					x::skip<RegexToken>()
					.ref(stat->condition)
					.ref(stat->trueStatement)
					.ref(stat->falseStatement)
					, input);
				return stat;
			}

			Ptr<ManagedStatement> ToWhileStat(const x::tp<
					RegexToken,
					Ptr<ManagedExpression>,
					Ptr<ManagedStatement>,
					x::opt<Ptr<ManagedExpression>>
				>::ResultType& input)
			{
				Ptr<ManagedWhileStatement> stat=CreateNode<ManagedWhileStatement>(input.First().First().First());
				x::Fill(
					x::skip<RegexToken>()
					.ref(stat->beginCondition)
					.ref(stat->statement)
					.ref(stat->endCondition)
					, input);
				return stat;
			}

			Ptr<ManagedStatement> ToDoWhileStat(const x::tp<
					RegexToken,
					Ptr<ManagedStatement>,
					Ptr<ManagedExpression>
				>::ResultType& input)
			{
				Ptr<ManagedWhileStatement> stat=CreateNode<ManagedWhileStatement>(input.First().First());
				x::Fill(
					x::skip<RegexToken>()
					.ref(stat->statement)
					.ref(stat->endCondition)
					, input);
				return stat;
			}

			Ptr<ManagedVariableStatement> ToForInitializer(const x::tp<
					Ptr<ManagedType>,
					RegexToken,
					Ptr<ManagedExpression>
				>::ResultType& input)
			{
				Ptr<ManagedVariableStatement> stat=CreateNode<ManagedVariableStatement>(input.First().Second());
				stat->constant=false;
				x::Fill(
					x::ref(stat->type)
					.ref(stat->name)
					.ref(stat->initializer)
					, input);
				stat->name=ConvertID(stat->name);
				return stat;
			}

			Ptr<ManagedStatement> ToForStat(const x::tp<
					RegexToken,
					x::rep<Ptr<ManagedVariableStatement>>,
					x::opt<Ptr<ManagedExpression>>,
					x::rep<Ptr<ManagedExpression>>,
					Ptr<ManagedStatement>
				>::ResultType& input)
			{
				Ptr<ManagedForStatement> stat=CreateNode<ManagedForStatement>(input.First().First().First().First());
				x::Fill(
					x::skip<RegexToken>()
					.ref(stat->initializers)
					.ref(stat->condition)
					.ref(stat->sideEffects)
					.ref(stat->statement)
					, input);
				return stat;
			}

			Ptr<ManagedCatchClause> ToCatchClause(const x::tp<
					RegexToken,
					Ptr<ManagedType>,
					x::opt<RegexToken>,
					Ptr<ManagedStatement>
				>::ResultType& input)
			{
				Ptr<ManagedCatchClause> clause=CreateNode<ManagedCatchClause>(input.First().First().First());
				x::Fill(
					x::skip<RegexToken>()
					.ref(clause->exceptionType)
					.ref(clause->exceptionName)
					.ref(clause->exceptionHandler)
					, input);
				clause->exceptionName=ConvertID(clause->exceptionName);
				return clause;
			}

			Ptr<ManagedStatement> ToTryStat(const x::tp<
					RegexToken,
					Ptr<ManagedStatement>,
					x::rep<Ptr<ManagedCatchClause>>,
					x::opt<Ptr<ManagedStatement>>
				>::ResultType& input)
			{
				Ptr<ManagedTryCatchStatement> stat=CreateNode<ManagedTryCatchStatement>(input.First().First().First());
				x::Fill(
					x::skip<RegexToken>()
					.ref(stat->tryStatement)
					.ref(stat->catches)
					.ref(stat->finallyStatement)
					, input);
				return stat;
			}

			Ptr<ManagedStatement> ToThrowStat(const x::tp<
					RegexToken,
					ParsingList<Ptr<ManagedExpression>>
				>::ResultType& input)
			{
				Ptr<ManagedThrowStatement> stat=CreateNode<ManagedThrowStatement>(input.First());
				x::Fill(
					x::skip<RegexToken>()
					.ref(stat->expression)
					, input);
				return stat;
			}

/***********************************************************************
Extended Statements
***********************************************************************/

			Ptr<ManagedStatement> ToUsingStat(const x::tp<
					RegexToken,
					Ptr<ManagedType>,
					RegexToken,
					Ptr<ManagedExpression>,
					Ptr<ManagedStatement>
				>::ResultType& input)
			{
				Ptr<ManagedUsingStatement> stat=CreateNode<ManagedUsingStatement>(input.First().First().First().First());
				x::Fill(
					x::skip<RegexToken>()
					.ref(stat->type)
					.ref(stat->name)
					.ref(stat->initialize)
					.ref(stat->statement)
					, input);
				stat->name=ConvertID(stat->name);
				return stat;
			}

			Ptr<ManagedStatement> ToLockStat(const x::tp<
					RegexToken,
					Ptr<ManagedExpression>,
					Ptr<ManagedStatement>
				>::ResultType& input)
			{
				Ptr<ManagedLockStatement> stat=CreateNode<ManagedLockStatement>(input.First().First());
				x::Fill(
					x::skip<RegexToken>()
					.ref(stat->lock)
					.ref(stat->statement)
					, input);
				return stat;
			}

			Ptr<ManagedCaseClause> ToCaseClause(const x::tp<
					x::rep<Ptr<ManagedExpression>>,
					Ptr<ManagedStatement>
				>::ResultType& input)
			{
				Ptr<ManagedCaseClause> clause=CreateNode<ManagedCaseClause>(input.Second());
				x::Fill(
					x::ref(clause->conditions)
					.ref(clause->statement)
					, input);
				return clause;
			}

			Ptr<ManagedStatement> ToSelectStat(const x::tp<
					RegexToken,
					Ptr<ManagedExpression>,
					ParsingList<Ptr<ManagedCaseClause>>,
					ParsingList<Ptr<ManagedStatement>>
				>::ResultType& input)
			{
				Ptr<ManagedSelectStatement> stat=CreateNode<ManagedSelectStatement>(input.First().First().First());
				x::Fill(
					x::skip<RegexToken>()
					.ref(stat->expression)
					.ref(stat->cases)
					.ref(stat->defaultStatement)
					, input);
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