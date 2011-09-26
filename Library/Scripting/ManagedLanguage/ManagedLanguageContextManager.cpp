#include "ManagedLanguageContextManager.h"
#include "ManagedLanguageAnalyzer.h"

namespace vl
{
	namespace scripting
	{
		namespace managedlanguage
		{

/***********************************************************************
ManagedContextManager
***********************************************************************/

			ManagedContextManager::StatementContext* ManagedContextManager::PushStatement()
			{
				StatementContext* context=new StatementContext;
				memset(context, 0, sizeof(*context));
				context->previous=currentStatementContext;
				currentStatementContext=context;
				return context;
			}

			ManagedContextManager::ManagedContextManager()
				:currentStatementContext(0)
			{
				memset(&predefinedTypes, 0, sizeof(predefinedTypes));
			}

			ManagedContextManager::~ManagedContextManager()
			{
				while(PopStatement());
			}

			ManagedTypeSymbol* ManagedContextManager::GetExpressionType(ManagedExpression* expression)
			{
				return expressionTypes[expression];
			}

			ManagedSymbolItem* ManagedContextManager::GetExpressionScope(ManagedExpression* expression)
			{
				return expressionScopes[expression];
			}

			void ManagedContextManager::SetExpression(ManagedExpression* expression, ManagedTypeSymbol* type, ManagedSymbolItem* scope)
			{
				expressionTypes.Add(expression, type);
				expressionScopes.Add(expression, scope);
			}

			ManagedSymbolItem* ManagedContextManager::GetStatementScope(ManagedStatement* statement)
			{
				return statementScopes[statement];
			}

			void ManagedContextManager::SetStatement(ManagedStatement* statement, ManagedSymbolItem* scope)
			{
				statementScopes.Add(statement, scope);
			}

			void ManagedContextManager::PushFunction(ManagedTypeSymbol* returnType, ManagedMember* member)
			{
				StatementContext* context=PushStatement();
				context->contextType=StatementContext::Method;
				context->languageElement.member=member;
			}

			void ManagedContextManager::PushFunction(ManagedTypeSymbol* returnType, ManagedLambdaExpression* lambdaExpression)
			{
				StatementContext* context=PushStatement();
				context->contextType=StatementContext::Lambda;
				context->languageElement.lambdaExpression=lambdaExpression;
			}

			void ManagedContextManager::PushLoop(ManagedStatement* statement)
			{
				StatementContext* context=PushStatement();
				context->contextType=StatementContext::Loop;
				context->languageElement.statement=statement;
			}

			void ManagedContextManager::PushSwitch(ManagedStatement* statement)
			{
				StatementContext* context=PushStatement();
				context->contextType=StatementContext::Switch;
				context->languageElement.statement=statement;
			}

			void ManagedContextManager::PushCatch(ManagedCatchClause* catchClause)
			{
				StatementContext* context=PushStatement();
				context->contextType=StatementContext::Catch;
				context->languageElement.catchClause=catchClause;
			}

			bool ManagedContextManager::PopStatement()
			{
				if(currentStatementContext)
				{
					StatementContext* previous=currentStatementContext->previous;
					delete currentStatementContext;
					currentStatementContext=previous;
					return true;
				}
				else
				{
					return false;
				}
			}

			ManagedStatement* ManagedContextManager::GetBreakTarget()
			{
				StatementContext* context=currentStatementContext;
				while(context && context->contextType!=StatementContext::Method && context->contextType!=StatementContext::Lambda)
				{
					if(context->contextType==StatementContext::Loop || context->contextType==StatementContext::Switch)
					{
						return context->languageElement.statement;
					}
					context=context->previous;
				}
				return 0;
			}

			ManagedStatement* ManagedContextManager::GetContinueTarget()
			{
				StatementContext* context=currentStatementContext;
				while(context && context->contextType!=StatementContext::Method && context->contextType!=StatementContext::Lambda)
				{
					if(context->contextType==StatementContext::Loop)
					{
						return context->languageElement.statement;
					}
					context=context->previous;
				}
				return 0;
			}

			ManagedCatchClause* ManagedContextManager::GetThrowTarget()
			{
				StatementContext* context=currentStatementContext;
				while(context && context->contextType!=StatementContext::Method && context->contextType!=StatementContext::Lambda)
				{
					if(context->contextType==StatementContext::Catch)
					{
						return context->languageElement.catchClause;
					}
					context=context->previous;
				}
				return 0;
			}

			bool ManagedContextManager::GetResultTarget(ManagedMember*& member, ManagedLambdaExpression*& lambdaExpression)
			{
				StatementContext* context=currentStatementContext;
				while(context && context->contextType!=StatementContext::Method && context->contextType!=StatementContext::Lambda)
				{
					context=context->previous;
				}
				if(context)
				{
					if(context->contextType==StatementContext::Method)
					{
						member=context->languageElement.member;
						lambdaExpression=0;
						return true;
					}
					else if(context->contextType==StatementContext::Lambda)
					{
						member=0;
						lambdaExpression=context->languageElement.lambdaExpression;
						return true;
					}
				}
				return false;
			}

			ManagedMember* ManagedContextManager::GetThisTargetMember()
			{
				StatementContext* context=currentStatementContext;
				while(context)
				{
					if(context->contextType==StatementContext::Method)
					{
						return context->languageElement.member;
					}
					context=context->previous;
				}
				return 0;
			}

/***********************************************************************
InitializeContextManager
***********************************************************************/

			void InitializeContextManager(ManagedProgram* program, const MAP& argument)
			{
				ManagedContextManager::PredefinedTypes& pt=argument.contextManager->predefinedTypes;
				pt.objectType			=GetSystemType(program, L"Object", argument);
				pt.sint8				=GetSystemType(program, L"SInt8", argument);
				pt.sint16				=GetSystemType(program, L"SInt16", argument);
				pt.sint32				=GetSystemType(program, L"SInt32", argument);
				pt.sint64				=GetSystemType(program, L"SInt64", argument);
				pt.uint8				=GetSystemType(program, L"UInt8", argument);
				pt.uint16				=GetSystemType(program, L"UInt16", argument);
				pt.uint32				=GetSystemType(program, L"UInt32", argument);
				pt.uint64				=GetSystemType(program, L"UInt64", argument);
				pt.singleType			=GetSystemType(program, L"Single", argument);
				pt.doubleType			=GetSystemType(program, L"Double", argument);
				pt.charType				=GetSystemType(program, L"Char", argument);
				pt.boolType				=GetSystemType(program, L"Bool", argument);
				pt.stringType			=GetSystemType(program, L"String", argument);
				pt.typeType				=GetSystemType(program, L"Reflection", L"Type", argument);
				pt.attributeType		=GetSystemType(program, L"Attribute", argument);
				pt.exceptionType		=GetSystemType(program, L"Exception", argument);
				pt.voidType				=GetSystemType(program, L"Void", argument);
				pt.idisposableType		=GetSystemType(program, L"IDisposable", argument);
				pt.idynamicType			=GetSystemType(program, L"IDynamic", argument);
				pt.dynamicProxyType		=GetSystemType(program, L"DynamicProxy", argument);
				pt.ienumeratorType		=GetSystemType(program, L"IEnumerator", argument);
				pt.ienumerableType		=GetSystemType(program, L"IEnumerable", argument);
				pt.ienumeratorOfTType	=GetSystemType(program, L"IEnumerator", argument, 1);
				pt.ienumerableOfTType	=GetSystemType(program, L"IEnumerable", argument, 1);
				pt.array1OfTType		=GetSystemType(program, L"Array1", argument, 1);
			}
		}
	}
}