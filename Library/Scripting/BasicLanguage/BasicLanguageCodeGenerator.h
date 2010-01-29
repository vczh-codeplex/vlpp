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

#include "..\..\Exception.h"
#include "BasicLanguageExpression.h"
#include "BasicLanguageTypeManager.h"
#include "..\Common\CommonScopeManager.h"

namespace vl
{
	namespace scripting
	{
		namespace basiclanguage
		{

/***********************************************************************
Scope Manager
***********************************************************************/

			class BasicScope : public CommonScope<BasicScope>
			{
				friend class BasicEnv;
				friend class Ptr<BasicScope>;
			protected:
				BasicTypeManager*													typeManager;
				BasicFunctionDeclaration*											functionDeclaration;
				BasicStatement*														statement;

				void																Initialize();

				BasicScope(BasicTypeManager* _typeManager, BasicFunctionDeclaration* _functionDeclaration, BasicStatement* _statement);
				BasicScope(BasicScope* _previousScope, BasicFunctionDeclaration* _functionDeclaration);
				BasicScope(BasicScope* _previousScope, BasicStatement* _statement);
				BasicScope(BasicScope* _previousScope);
				~BasicScope();
			public:

				BasicTypeManager*													TypeManager();
				BasicFunctionDeclaration*											OwnerDeclaration();
				BasicStatement*														OwnerStatement();

				CommonScopeItems<BasicScope, WString, BasicTypeRecord*>				types;
				CommonScopeItems<BasicScope, WString, BasicTypeRecord*>				variables;
				CommonScopeItems<BasicScope, WString, BasicFunctionDeclaration*>	functions;
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

/***********************************************************************
Exception
***********************************************************************/

			class BasicLanguageCodeException : public Exception
			{
			public:
				enum ExceptionCode
				{
					TypeNameNotExists,
					FunctionAlreadyExists,
					VariableAlreadyExists,
					TypeAlreadyExists,
					StructureMemberAlreadyExists,
				};
			protected:
				BasicLanguageElement*											element;
				ExceptionCode													exceptionCode;
				collections::List<WString>										parameters;
			public:
				BasicLanguageCodeException(BasicLanguageElement* _element, ExceptionCode _exceptionCode, const collections::IReadonlyList<WString>& _parameters);
				BasicLanguageCodeException(const BasicLanguageCodeException& exception);
				~BasicLanguageCodeException();

				BasicLanguageElement*											GetBasicLanguageElement()const;
				ExceptionCode													GetExceptionCode()const;
				const collections::IReadonlyList<WString>&						GetParameters()const;

				static Ptr<BasicLanguageCodeException>							GetTypeNameNotExists(BasicReferenceType* type);
				static Ptr<BasicLanguageCodeException>							GetFunctionAlreadyExists(BasicFunctionDeclaration* function);
				static Ptr<BasicLanguageCodeException>							GetVariableAlreadyExists(BasicVariableDeclaration* variable);
				static Ptr<BasicLanguageCodeException>							GetTypeAlreadyExists(BasicStructureDeclaration* type);
				static Ptr<BasicLanguageCodeException>							GetTypeAlreadyExists(BasicTypeRenameDeclaration* type);
				static Ptr<BasicLanguageCodeException>							GetStructureMemberAlreadyExists(BasicStructureDeclaration* type, int memberIndex);
			};

			typedef class BasicAlgorithmParameter
			{
			public:
				BasicEnv*												env;
				BasicScope*												scope;
				collections::List<Ptr<BasicLanguageCodeException>>&		errors;

				BasicAlgorithmParameter(BasicEnv* _env, BasicScope* _scope, collections::List<Ptr<BasicLanguageCodeException>>& _errors);
			} BP;

/***********************************************************************
Algorithms
***********************************************************************/

			EXTERN_ALGORITHM_FUNCTION(BasicLanguage_GetTypeRecord, BasicType, BasicScope*, BasicTypeRecord*)
			EXTERN_ALGORITHM_PROCEDURE(BasicLanguage_BuildGlobalScopePass1, BasicDeclaration, BP)
			EXTERN_ALGORITHM_PROCEDURE(BasicLanguage_BuildGlobalScopePass2, BasicDeclaration, BP)
		}
	}
}

#endif