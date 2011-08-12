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
				};

				PredefinedTypes								predefinedTypes;
			protected:
				ExpressionTypeMap							expressionTypes;
				ExpressionScopeMap							expressionScopes;
				StatementScopeMap							statementScopes;
			public:

				ManagedContextManager();
				~ManagedContextManager();

				ManagedTypeSymbol*							GetExpressionType(ManagedExpression* expression);
				ManagedSymbolItem*							GetExpressionScope(ManagedExpression* expression);
				void										SetExpression(ManagedExpression* expression, ManagedTypeSymbol* type, ManagedSymbolItem* scope);

				ManagedSymbolItem*							GetStatementScope(ManagedStatement* statement);
				void										SetStatement(ManagedStatement* statement, ManagedSymbolItem* scope);
			};
		}
	}
}

#endif