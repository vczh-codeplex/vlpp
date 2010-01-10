/***********************************************************************
Vczh Library++ 3.0
Developer: 陈梓瀚(vczh)
Scripting::BasicLanguage

Classes:
	BasicScope									：对象定义域
	BasicEnv									：对象全局环境
***********************************************************************/

#ifndef VCZH_SCRIPTING_BASICLANGUAGE_BASICLANGUAGECODEGENERATOR
#define VCZH_SCRIPTING_BASICLANGUAGE_BASICLANGUAGECODEGENERATOR

#include "BasicLanguageExpression.h"
#include "BasicLanguageTypeManager.h"

namespace vl
{
	namespace scripting
	{
		namespace basiclanguage
		{
			class BasicScope : public Object, private NotCopyable
			{
				friend class BasicEnv;
				friend class Ptr<BasicScope>;
			protected:
				BasicScope*														previousScope;
				BasicTypeManager*												typeManager;
				BasicFunctionDeclaration*										functionDeclaration;
				BasicStatement*													statement;
				collections::Dictionary<WString, BasicTypeRecord*>				types;
				collections::Dictionary<WString, BasicTypeRecord*>				variables;
				collections::Dictionary<WString, BasicFunctionDeclaration*>		functions;

				BasicScope(BasicTypeManager* _typeManager, BasicFunctionDeclaration* _functionDeclaration, BasicStatement* _statement);
				BasicScope(BasicScope* _previousScope, BasicFunctionDeclaration* _functionDeclaration);
				BasicScope(BasicScope* _previousScope, BasicStatement* _statement);
				BasicScope(BasicScope* _previousScope);
				~BasicScope();
			public:

				BasicScope*														PreviousScope();
				BasicTypeManager*												TypeManager();
				BasicFunctionDeclaration*										OwnerDeclaration();
				BasicStatement*													OwnerStatement();
				collections::IDictionary<WString, BasicTypeRecord*>&			Types();
				collections::IDictionary<WString, BasicTypeRecord*>&			Variables();
				collections::IDictionary<WString, BasicFunctionDeclaration*>&	Functions();
				BasicTypeRecord*												GetType(const WString& name);
				BasicTypeRecord*												GetType(const WString& name, BasicScope*& scope);
				BasicTypeRecord*												GetVariableType(const WString& name);
				BasicTypeRecord*												GetVariableType(const WString& name, BasicScope*& scope);
				BasicFunctionDeclaration*										GetFunction(const WString& name);
				BasicFunctionDeclaration*										GetFunction(const WString& name, BasicScope*& scope);
			};

			class BasicEnv : public Object, private NotCopyable
			{
				typedef collections::Dictionary<BasicFunctionDeclaration*, BasicScope*>			_FunctionScopeTable;
				typedef collections::Dictionary<BasicStatement*, BasicScope*>					_StatementScopeTable;
			protected:
				BasicTypeManager												typeManager;
				collections::List<Ptr<BasicScope>>								allocatedScopes;
				BasicScope*														globalScope;
				_FunctionScopeTable												functionScopes;
				_StatementScopeTable											statementScopes;
			public:
				BasicEnv();
				~BasicEnv();

				BasicTypeManager&												TypeManager();
				BasicScope*														GlobalScope();
				BasicScope*														CreateScope(BasicScope* previousScope);
				BasicScope*														CreateFunctionScope(BasicScope* previousScope, BasicFunctionDeclaration* functionDeclaration);
				BasicScope*														CreateStatementScope(BasicScope* previousScope, BasicStatement* statement);
			};
		}
	}
}

#endif