/***********************************************************************
Vczh Library++ 3.0
Developer: 陈梓瀚(vczh)
Scripting::BasicLanguage

Classes:
	BasicScope									：对象定义域
	BasicEnv									：对象全局环境
***********************************************************************/

#ifndef VCZH_SCRIPTING_BASICLANGUAGE_BASICSLANGUAGESCOPEMANAGER
#define VCZH_SCRIPTING_BASICLANGUAGE_BASICSLANGUAGESCOPEMANAGER

#include "BasicLanguageExpression.h"
#include "BasicLanguageTypeManager.h"
#include "..\Common\CommonScopeManager.h"

namespace vl
{
	namespace scripting
	{
		namespace basiclanguage
		{
			class BasicScope : public CommonScope<BasicScope>
			{
				friend class BasicEnv;
				friend class Ptr<BasicScope>;
			protected:
				BasicFunctionDeclaration*											functionDeclaration;
				BasicStatement*														statement;

				void																Initialize();

				BasicScope(BasicScope* _previousScope, BasicFunctionDeclaration* _functionDeclaration);
				BasicScope(BasicScope* _previousScope, BasicStatement* _statement);
				BasicScope(BasicScope* _previousScope);
				BasicScope();
				~BasicScope();
			public:
				struct Variable
				{
					BasicVariableDeclaration*			globalVariable;
					BasicVariableStatement*				localVariable;
					int									parameterIndex;
					BasicTypeRecord*					type;

					Variable();
					Variable(BasicVariableDeclaration* variable, BasicTypeRecord* _type);
					Variable(BasicVariableStatement* variable, BasicTypeRecord* _type);
					Variable(int variable, BasicTypeRecord* _type);

					operator bool();
					bool								operator==(const Variable& variable);
				};

				BasicFunctionDeclaration*											OwnerDeclaration();
				BasicStatement*														OwnerStatement();

				CommonScopeItems<BasicScope, WString, BasicTypeRecord*>				types;
				CommonScopeItems<BasicScope, WString, Variable>						variables;
				CommonScopeItems<BasicScope, WString, BasicFunctionDeclaration*>	functions;
			};

			class BasicEnv : public Object, private NotCopyable
			{
			public:
				struct Reference
				{
					BasicScope*							scope;
					BasicVariableDeclaration*			globalVariable;
					BasicVariableStatement*				localVariable;
					int									parameterIndex;
					BasicFunctionDeclaration*			function;
					bool								isVariable;

					Reference();
					Reference(BasicScope* _scope, BasicVariableDeclaration* variable);
					Reference(BasicScope* _scope, BasicVariableStatement* variable);
					Reference(BasicScope* _scope, int parameter);
					Reference(BasicScope* _scope, BasicFunctionDeclaration* function);

					bool			operator==(const Reference& r);
				};
				typedef collections::Dictionary<BasicFunctionDeclaration*, BasicScope*>			_FunctionScopeTable;
				typedef collections::Dictionary<BasicFunctionDeclaration*, BasicTypeRecord*>	_FunctionTypeTable;
				typedef collections::Dictionary<BasicStatement*, BasicScope*>					_StatementScopeTable;
				typedef collections::Dictionary<BasicExpression*, BasicTypeRecord*>				_ExpressionTypeTable;
				typedef collections::Dictionary<BasicReferenceExpression*, Reference>			_ReferenceTable;
			protected:
				collections::List<Ptr<BasicScope>>								allocatedScopes;
				BasicScope*														globalScope;
				_FunctionScopeTable												functionScopes;
				_StatementScopeTable											statementScopes;
				_FunctionTypeTable												functionTypes;
				_ExpressionTypeTable											expressionTypes;
				_ReferenceTable													referenceTypes;
			public:
				BasicEnv();
				~BasicEnv();

				BasicScope*														GlobalScope();
				BasicScope*														CreateScope(BasicScope* previousScope);
				BasicScope*														CreateFunctionScope(BasicScope* previousScope, BasicFunctionDeclaration* functionDeclaration);
				BasicScope*														CreateStatementScope(BasicScope* previousScope, BasicStatement* statement);
				void															RegisterFunctionType(BasicFunctionDeclaration* function, BasicTypeRecord* type);
				void															RegisterExpressionType(BasicExpression* expression, BasicTypeRecord* type);
				void															RegisterReference(BasicReferenceExpression* expression, Reference reference);

				BasicScope*														GetFunctionScope(BasicFunctionDeclaration* function);
				BasicScope*														GetStatementScope(BasicStatement* statement);
				BasicTypeRecord*												GetFunctionType(BasicFunctionDeclaration* function);
				BasicTypeRecord*												GetExpressionType(BasicExpression* expression);
				Reference														GetReference(BasicReferenceExpression* expression);
			};
		}
	}
}

#endif