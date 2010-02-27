#include "BasicLanguageScopeManager.h"

namespace vl
{
	namespace scripting
	{
		namespace basiclanguage
		{

/***********************************************************************
BasicScope::Variable
***********************************************************************/

			BasicScope::Variable::Variable()
				:globalVariable(0)
				,localVariable(0)
				,parameterIndex(0)
				,type(0)
			{
			}

			BasicScope::Variable::Variable(BasicVariableDeclaration* variable, BasicTypeRecord* _type)
				:globalVariable(variable)
				,localVariable(0)
				,parameterIndex(0)
				,type(_type)
			{
			}

			BasicScope::Variable::Variable(BasicVariableStatement* variable, BasicTypeRecord* _type)
				:globalVariable(0)
				,localVariable(variable)
				,parameterIndex(0)
				,type(_type)
			{
			}

			BasicScope::Variable::Variable(int variable, BasicTypeRecord* _type)
				:globalVariable(0)
				,localVariable(0)
				,parameterIndex(variable)
				,type(_type)
			{
			}

			BasicScope::Variable::operator bool()
			{
				return type!=0;
			}

			bool BasicScope::Variable::operator==(const Variable& variable)
			{
				return globalVariable==variable.globalVariable
					&& localVariable==variable.localVariable
					&& parameterIndex==variable.parameterIndex
					&& type==variable.type;
			}

/***********************************************************************
BasicScope
***********************************************************************/

			void BasicScope::Initialize()
			{
				types.Initialize(this, &BasicScope::types, 0);
				variables.Initialize(this, &BasicScope::variables, Variable());
				functions.Initialize(this, &BasicScope::functions, 0);
			}

			BasicScope::BasicScope(BasicScope* _previousScope, BasicFunctionDeclaration* _functionDeclaration)
				:CommonScope<BasicScope>(_previousScope)
				,functionDeclaration(_functionDeclaration)
				,statement(0)
			{
				Initialize();
			}

			BasicScope::BasicScope(BasicScope* _previousScope, BasicStatement* _statement)
				:CommonScope<BasicScope>(_previousScope)
				,functionDeclaration(_previousScope->functionDeclaration)
				,statement(_statement)
			{
				Initialize();
			}

			BasicScope::BasicScope(BasicScope* _previousScope)
				:CommonScope<BasicScope>(_previousScope)
				,functionDeclaration(_previousScope->functionDeclaration)
				,statement(_previousScope->statement)
			{
				Initialize();
			}

			BasicScope::BasicScope()
				:CommonScope<BasicScope>(0)
				,functionDeclaration(0)
				,statement(0)
			{
				Initialize();
			}

			BasicScope::~BasicScope()
			{
			}
			
			BasicFunctionDeclaration* BasicScope::OwnerDeclaration()
			{
				return functionDeclaration;
			}

			BasicStatement* BasicScope::OwnerStatement()
			{
				return statement;
			}

/***********************************************************************
BasicEnv::Reference
***********************************************************************/

			BasicEnv::Reference::Reference()
				:scope(0)
				,globalVariable(0)
				,localVariable(0)
				,parameterIndex(-1)
				,function(0)
				,isVariable(true)
			{
			}
			
			BasicEnv::Reference::Reference(BasicScope* _scope, BasicVariableDeclaration* variable)
				:scope(_scope)
				,globalVariable(variable)
				,localVariable(0)
				,parameterIndex(-1)
				,function(0)
				,isVariable(true)
			{
			}

			BasicEnv::Reference::Reference(BasicScope* _scope, BasicVariableStatement* variable)
				:scope(_scope)
				,globalVariable(0)
				,localVariable(variable)
				,parameterIndex(-1)
				,function(0)
				,isVariable(true)
			{
			}

			BasicEnv::Reference::Reference(BasicScope* _scope, int parameter)
				:scope(_scope)
				,globalVariable(0)
				,localVariable(0)
				,parameterIndex(parameter)
				,function(0)
				,isVariable(true)
			{
			}

			BasicEnv::Reference::Reference(BasicScope* _scope, BasicFunctionDeclaration* function)
				:scope(_scope)
				,globalVariable(0)
				,localVariable(0)
				,parameterIndex(-1)
				,function(function)
				,isVariable(false)
			{
			}

			bool BasicEnv::Reference::operator==(const Reference& r)
			{
				return scope==r.scope
					&& isVariable==r.isVariable
					&& globalVariable==r.globalVariable
					&& localVariable==r.localVariable
					&& parameterIndex==r.parameterIndex
					&& function==r.function;
			}

/***********************************************************************
BasicEnv
***********************************************************************/

			BasicEnv::BasicEnv()
				:globalScope(CreateScope(0))
			{
			}

			BasicEnv::~BasicEnv()
			{
			}

			BasicScope* BasicEnv::GlobalScope()
			{
				return globalScope;
			}

			BasicScope* BasicEnv::CreateScope(BasicScope* previousScope)
			{
				BasicScope* scope=previousScope?new BasicScope(previousScope):new BasicScope();
				allocatedScopes.Add(scope);
				return scope;
			}

			BasicScope* BasicEnv::CreateFunctionScope(BasicScope* previousScope, BasicFunctionDeclaration* functionDeclaration)
			{
				int index=functionScopes.Keys().IndexOf(functionDeclaration);
				if(index!=-1)
				{
					BasicScope* scope=functionScopes.Values()[index];
					return scope->PreviousScope()==previousScope?scope:0;
				}
				else
				{
					BasicScope* scope=new BasicScope(previousScope, functionDeclaration);
					allocatedScopes.Add(scope);
					functionScopes.Add(functionDeclaration, scope);
					return scope;
				}
			}

			BasicScope* BasicEnv::CreateStatementScope(BasicScope* previousScope, BasicStatement* statement)
			{
				int index=statementScopes.Keys().IndexOf(statement);
				if(index!=-1)
				{
					BasicScope* scope=statementScopes.Values()[index];
					return scope->PreviousScope()==previousScope?scope:0;
				}
				else
				{
					BasicScope* scope=new BasicScope(previousScope, statement);
					allocatedScopes.Add(scope);
					statementScopes.Add(statement, scope);
					return scope;
				}
			}

			void BasicEnv::RegisterStatementScope(BasicStatement* statement, BasicScope* scope)
			{
				int index=statementScopes.Keys().IndexOf(statement);
				if(index==-1)
				{
					statementScopes.Add(statement, scope);
				}
			}

			void BasicEnv::RegisterFunctionType(BasicFunctionDeclaration* function, BasicTypeRecord* type)
			{
				functionTypes.Add(function, type);
			}

			void BasicEnv::RegisterExpressionType(BasicExpression* expression, BasicTypeRecord* type)
			{
				expressionTypes.Add(expression, type);
			}

			void BasicEnv::RegisterReference(BasicReferenceExpression* expression, Reference reference)
			{
				referenceTypes.Add(expression, reference);
			}

			BasicScope* BasicEnv::GetFunctionScope(BasicFunctionDeclaration* function)
			{
				int index=functionScopes.Keys().IndexOf(function);
				return index==-1?0:functionScopes.Values()[index];
			}

			BasicScope* BasicEnv::GetStatementScope(BasicStatement* statement)
			{
				int index=statementScopes.Keys().IndexOf(statement);
				return index==-1?0:statementScopes.Values()[index];
			}

			BasicTypeRecord* BasicEnv::GetFunctionType(BasicFunctionDeclaration* function)
			{
				int index=functionTypes.Keys().IndexOf(function);
				return index==-1?0:functionTypes.Values()[index];
			}

			BasicTypeRecord* BasicEnv::GetExpressionType(BasicExpression* expression)
			{
				int index=expressionTypes.Keys().IndexOf(expression);
				return index==-1?0:expressionTypes.Values()[index];
			}

			BasicEnv::Reference BasicEnv::GetReference(BasicReferenceExpression* expression)
			{
				return referenceTypes[expression];
			}
		}
	}
}