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

/***********************************************************************
BasicCompileTimeConstant
***********************************************************************/

			struct BasicCompileTimeConstant
			{
				union
				{
					signed __int64			s;
					unsigned __int64		u;
					double					d;
				};

				enum ConstantType
				{
					Unsigned,
					Signed,
					Float,
				};

				BasicCompileTimeConstant(){s=0;}
				BasicCompileTimeConstant(signed __int64 v){s=v;}
				BasicCompileTimeConstant(unsigned __int64 v){u=v;}
				BasicCompileTimeConstant(double v){d=v;}
				BasicCompileTimeConstant(bool v){s=(v?-1:0);}

				signed __int64				S(BasicTypeRecord* sourceType);
				unsigned __int64			U(BasicTypeRecord* sourceType);
				double						F(BasicTypeRecord* sourceType);
			};

			extern BasicCompileTimeConstant::ConstantType GetConstantType(BasicTypeRecord* type);

/***********************************************************************
BasicScope
***********************************************************************/

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
					BasicVariableDeclaration*					globalVariable;
					BasicVariableStatement*						localVariable;
					vint										parameterIndex;
					BasicTypeRecord*							type;
					BasicCompileTimeConstant					constantValue;

					Variable();
					Variable(BasicVariableDeclaration* variable, BasicTypeRecord* _type);
					Variable(BasicVariableDeclaration* variable, BasicTypeRecord* _type, BasicCompileTimeConstant _constantValue);
					Variable(BasicVariableStatement* variable, BasicTypeRecord* _type);
					Variable(vint variable, BasicTypeRecord* _type);

					operator bool();
					bool										operator==(const Variable& variable);
				};

				struct Concept
				{
					typedef collections::Dictionary<WString, BasicTypeRecord*>		ConceptFunctionMap;

					BasicTypeRecord*							conceptType;
					BasicScope*									conceptScope;
					BasicConceptBaseDeclaration*				conceptDeclaration;
					ConceptFunctionMap							functions;
				};

				struct Instance
				{
					struct FunctionInstance
					{
						typedef collections::Dictionary<WString, Ptr<FunctionInstance>>	MapType;

						BasicFunctionDeclaration*				functionDeclaration;
						collections::List<BasicTypeRecord*>		genericParameters;
					};

					Concept*									targetConcept;
					BasicTypeRecord*							instanceType;
					BasicScope*									instanceScope;
					BasicConceptInstanceDeclaration*			instanceDeclaration;
					FunctionInstance::MapType					functions;
				};

				BasicFunctionDeclaration*											OwnerDeclaration();
				BasicStatement*														OwnerStatement();

				CommonScopeItems<BasicScope, WString, BasicTypeRecord*>				types;
				CommonScopeItems<BasicScope, WString, Variable>						variables;
				CommonScopeItems<BasicScope, WString, BasicFunctionDeclaration*>	functions;
				CommonScopeItems<BasicScope, WString, Ptr<Concept>>					concepts;
				collections::List<Ptr<Instance>>									instances;
				collections::Group<BasicTypeRecord*, WString>						genericConstraints;

				Ptr<Instance>									FindInstance(BasicTypeRecord* type, const WString& conceptName);
				bool											RequiredInstanceExists(BasicTypeRecord* type, const WString& conceptName, Ptr<BasicScope::Instance>& instance);
			};

/***********************************************************************
BasicEnv
***********************************************************************/

			class BasicEnv : public Object, private NotCopyable
			{
			public:
				struct Reference
				{
					BasicScope*									scope;
					BasicVariableDeclaration*					globalVariable;
					BasicVariableStatement*						localVariable;
					vint										parameterIndex;
					BasicFunctionDeclaration*					function;
					bool										isVariable;

					Reference();
					Reference(BasicScope* _scope, BasicVariableDeclaration* variable);
					Reference(BasicScope* _scope, BasicVariableStatement* variable);
					Reference(BasicScope* _scope, vint parameter);
					Reference(BasicScope* _scope, BasicFunctionDeclaration* function);

					bool			operator==(const Reference& r);
				};
				typedef collections::Dictionary<BasicFunctionDeclaration*, BasicScope*>			_FunctionScopeTable;
				typedef collections::Dictionary<BasicFunctionDeclaration*, BasicTypeRecord*>	_FunctionTypeTable;
				typedef collections::Dictionary<BasicStructureDeclaration*, BasicTypeRecord*>	_StructureTypeTable;
				typedef collections::Dictionary<BasicStatement*, BasicScope*>					_StatementScopeTable;
				typedef collections::Dictionary<BasicExpression*, BasicTypeRecord*>				_ExpressionTypeTable;
				typedef collections::Dictionary<BasicReferenceExpression*, Reference>			_ReferenceTable;
			protected:
				collections::List<Ptr<BasicScope>>								allocatedScopes;
				BasicScope*														globalScope;
				_FunctionScopeTable												functionScopes;
				_StatementScopeTable											statementScopes;
				_FunctionTypeTable												functionTypes;
				_StructureTypeTable												structureTypes;
				_ExpressionTypeTable											expressionTypes;
				_ReferenceTable													referenceTypes;
			public:
				BasicEnv();
				~BasicEnv();

				BasicScope*														GlobalScope();
				BasicScope*														CreateScope(BasicScope* previousScope);
				BasicScope*														CreateFunctionScope(BasicScope* previousScope, BasicFunctionDeclaration* functionDeclaration);
				BasicScope*														CreateStatementScope(BasicScope* previousScope, BasicStatement* statement);
				void															RegisterStatementScope(BasicStatement* statement, BasicScope* scope);
				void															RegisterFunctionType(BasicFunctionDeclaration* function, BasicTypeRecord* type);
				void															RegisterStructureType(BasicStructureDeclaration* structure, BasicTypeRecord* type);
				void															RegisterExpressionType(BasicExpression* expression, BasicTypeRecord* type);
				void															RegisterReference(BasicReferenceExpression* expression, Reference reference);

				BasicScope*														GetFunctionScope(BasicFunctionDeclaration* function);
				BasicScope*														GetStatementScope(BasicStatement* statement);
				BasicTypeRecord*												GetFunctionType(BasicFunctionDeclaration* function, bool avoidUninstanciatedType);
				BasicTypeRecord*												GetStructureType(BasicStructureDeclaration* structure);
				BasicTypeRecord*												GetExpressionType(BasicExpression* expression);
				Reference														GetReference(BasicReferenceExpression* expression);
			};
		}
	}
}

#endif