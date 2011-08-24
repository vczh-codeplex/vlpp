/***********************************************************************
Vczh Library++ 3.0
Developer: ³Âè÷å«(vczh)
Scripting::ManagedLanguage

Classes:
***********************************************************************/

#ifndef VCZH_SCRIPTING_MANAGEDLANGUAGE_MANAGEDLANGUAGECONTEXTMANAGER
#define VCZH_SCRIPTING_MANAGEDLANGUAGE_MANAGEDLANGUAGECONTEXTMANAGER

#include "ManagedLanguageSymbolManager.h"

namespace vl
{
	namespace scripting
	{
		namespace managedlanguage
		{
			class ManagedContextManager : public Object
			{
				typedef collections::Dictionary<ManagedExpression*, ManagedTypeSymbol*>		ExpressionTypeMap;
				typedef collections::Dictionary<ManagedExpression*, ManagedSymbolItem*>		ExpressionScopeMap;
				typedef collections::Dictionary<ManagedStatement*, ManagedSymbolItem*>		StatementScopeMap;
			public:
				struct PredefinedTypes
				{
					ManagedTypeSymbol*						sint8;
					ManagedTypeSymbol*						sint16;
					ManagedTypeSymbol*						sint32;
					ManagedTypeSymbol*						sint64;
					ManagedTypeSymbol*						uint8;
					ManagedTypeSymbol*						uint16;
					ManagedTypeSymbol*						uint32;
					ManagedTypeSymbol*						uint64;
					ManagedTypeSymbol*						singleType;
					ManagedTypeSymbol*						doubleType;
					ManagedTypeSymbol*						charType;
					ManagedTypeSymbol*						boolType;
					ManagedTypeSymbol*						stringType;
					ManagedTypeSymbol*						typeType;
					ManagedTypeSymbol*						attributeType;
					ManagedTypeSymbol*						exceptionType;
					ManagedTypeSymbol*						voidType;
					ManagedTypeSymbol*						idisposableType;
					ManagedTypeSymbol*						idynamicType;
					ManagedTypeSymbol*						dynamicProxyType;
					ManagedTypeSymbol*						ienumeratorType;
					ManagedTypeSymbol*						ienumerableType;
					ManagedTypeSymbol*						ienumeratorOfTType;
					ManagedTypeSymbol*						ienumerableOfTType;
				};

				PredefinedTypes								predefinedTypes;
			protected:
				struct StatementContext
				{
					enum ContextType
					{
						Method,
						Lambda,
						Loop,
						Switch,
						Catch,
					};

					ContextType								contextType;
					StatementContext*						previous;
					union
					{
						ManagedMember*						member;
						ManagedLambdaExpression*			lambdaExpression;
						ManagedStatement*					statement;
						ManagedCatchClause*					catchClause;
					}										languageElement;
				};
			protected:
				ExpressionTypeMap							expressionTypes;
				ExpressionScopeMap							expressionScopes;
				StatementScopeMap							statementScopes;
				StatementContext*							currentStatementContext;

				StatementContext*							PushStatement();
			public:

				ManagedContextManager();
				~ManagedContextManager();

				ManagedTypeSymbol*							GetExpressionType(ManagedExpression* expression);
				ManagedSymbolItem*							GetExpressionScope(ManagedExpression* expression);
				void										SetExpression(ManagedExpression* expression, ManagedTypeSymbol* type, ManagedSymbolItem* scope);

				ManagedSymbolItem*							GetStatementScope(ManagedStatement* statement);
				void										SetStatement(ManagedStatement* statement, ManagedSymbolItem* scope);

				void										PushFunction(ManagedTypeSymbol* returnType, ManagedMember* member);
				void										PushFunction(ManagedTypeSymbol* returnType, ManagedLambdaExpression* lambdaExpression);
				void										PushLoop(ManagedStatement* statement);
				void										PushSwitch(ManagedStatement* statement);
				void										PushCatch(ManagedCatchClause* catchClause);
				bool										PopStatement();
				ManagedStatement*							GetBreakTarget();
				ManagedStatement*							GetContinueTarget();
				ManagedCatchClause*							GetThrowTarget();
				bool										GetResultTarget(ManagedMember*& member, ManagedLambdaExpression*& lambdaExpression);
				ManagedMember*								GetThisTargetMember();
			};
		}
	}
}

#endif