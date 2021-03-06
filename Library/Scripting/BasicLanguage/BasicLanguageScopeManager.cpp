#include "BasicLanguageScopeManager.h"

namespace vl
{
	namespace scripting
	{
		namespace basiclanguage
		{

/***********************************************************************
BasicCompileTimeConstant
***********************************************************************/
			
			signed __int64 BasicCompileTimeConstant::S(BasicTypeRecord* sourceType)
			{
				switch(GetConstantType(sourceType))
				{
				case Signed:
					return (signed __int64)s;
				case Unsigned:
					return (signed __int64)u;
				case Float:
					return (signed __int64)d;
				default:
					return 0;
				}
			}

			unsigned __int64 BasicCompileTimeConstant::U(BasicTypeRecord* sourceType)
			{
				switch(GetConstantType(sourceType))
				{
				case Signed:
					return (unsigned __int64)s;
				case Unsigned:
					return (unsigned __int64)u;
				case Float:
					return (unsigned __int64)d;
				default:
					return 0;
				}
			}

			double BasicCompileTimeConstant::F(BasicTypeRecord* sourceType)
			{
				switch(GetConstantType(sourceType))
				{
				case Signed:
					return (double)s;
				case Unsigned:
					return (double)u;
				case Float:
					return (double)d;
				default:
					return 0;
				}
			}

			BasicCompileTimeConstant::ConstantType GetConstantType(BasicTypeRecord* type)
			{
				if(type->GetType()==BasicTypeRecord::Primitive)
				{
					switch(type->PrimitiveType())
					{
					case s8:
					case s16:
					case s32:
					case s64:
						return BasicCompileTimeConstant::Signed;
					case u8:
					case u16:
					case u32:
					case u64:
						return BasicCompileTimeConstant::Unsigned;
					case f32:
					case f64:
						return BasicCompileTimeConstant::Float;
					case char_type:
					case wchar_type:
					case bool_type:
						return BasicCompileTimeConstant::Signed;
					}
				}
				else if(type->GetType()==BasicTypeRecord::Pointer)
				{
					return BasicCompileTimeConstant::Unsigned;
				}
				CHECK_FAIL(L"GetConstantType(BasicTypeRecord* type))#非法常熟类型。");
			}

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

			BasicScope::Variable::Variable(BasicVariableDeclaration* variable, BasicTypeRecord* _type, BasicCompileTimeConstant _constantValue)
				:globalVariable(variable)
				,localVariable(0)
				,parameterIndex(0)
				,constantValue(_constantValue)
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

			BasicScope::Variable::Variable(vint variable, BasicTypeRecord* _type)
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
				concepts.Initialize(this, &BasicScope::concepts, 0);
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

			Ptr<BasicScope::Instance> BasicScope::FindInstance(BasicTypeRecord* type, const WString& conceptName)
			{
				BasicScope* scope=this;
				while(scope)
				{
					for(vint i=0;i<scope->instances.Count();i++)
					{
						Ptr<BasicScope::Instance> instance=scope->instances[i];
						BasicTypeRecord* secondaryType=instance->instanceType;
						if(secondaryType->GetType()==BasicTypeRecord::Generic)
						{
							secondaryType=secondaryType->ElementType();
						}
						if((instance->instanceType==type || secondaryType==type) && instance->targetConcept->conceptDeclaration->name==conceptName)
						{
							return instance;
						}
					}
					scope=scope->previousScope;
				}
				return 0;
			}

			bool BasicScope::RequiredInstanceExists(BasicTypeRecord* type, const WString& conceptName, Ptr<BasicScope::Instance>& instance)
			{
				instance=0;
				BasicScope* scope=this;
				while(scope)
				{
					vint genericArgumentIndex=scope->genericConstraints.Keys().IndexOf(type);
					if(genericArgumentIndex!=-1)
					{
						return scope->genericConstraints.GetByIndex(genericArgumentIndex).Contains(conceptName);
					}
					scope=scope->previousScope;
				}

				BasicGenericStructureProxyTypeRecord* structureProxyType=dynamic_cast<BasicGenericStructureProxyTypeRecord*>(type);
				if(structureProxyType)
				{
					instance=FindInstance(structureProxyType->UninstantiatedStructureType(), conceptName);
				}
				else
				{
					instance=FindInstance(type, conceptName);
				}
				return instance;
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

			BasicEnv::Reference::Reference(BasicScope* _scope, vint parameter)
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
				vint index=functionScopes.Keys().IndexOf(functionDeclaration);
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
				vint index=statementScopes.Keys().IndexOf(statement);
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
				vint index=statementScopes.Keys().IndexOf(statement);
				if(index==-1)
				{
					statementScopes.Add(statement, scope);
				}
			}

			void BasicEnv::RegisterFunctionType(BasicFunctionDeclaration* function, BasicTypeRecord* type)
			{
				functionTypes.Add(function, type);
			}

			void BasicEnv::RegisterStructureType(BasicStructureDeclaration* structure, BasicTypeRecord* type)
			{
				structureTypes.Add(structure, type);
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
				vint index=functionScopes.Keys().IndexOf(function);
				return index==-1?0:functionScopes.Values()[index];
			}

			BasicScope* BasicEnv::GetStatementScope(BasicStatement* statement)
			{
				vint index=statementScopes.Keys().IndexOf(statement);
				return index==-1?0:statementScopes.Values()[index];
			}

			BasicTypeRecord* BasicEnv::GetFunctionType(BasicFunctionDeclaration* function, bool avoidUninstantiatedType)
			{
				vint index=functionTypes.Keys().IndexOf(function);
				if(index==-1)
				{
					return 0;
				}
				else
				{
					BasicTypeRecord* type=functionTypes.Values()[index];
					if(avoidUninstantiatedType && type->GetType()==BasicTypeRecord::Generic)
					{
						type=type->ElementType();
					}
					return type;
				}
			}

			BasicTypeRecord* BasicEnv::GetStructureType(BasicStructureDeclaration* structure)
			{
				vint index=structureTypes.Keys().IndexOf(structure);
				return index==-1?0:structureTypes.Values()[index];
			}

			BasicTypeRecord* BasicEnv::GetExpressionType(BasicExpression* expression)
			{
				vint index=expressionTypes.Keys().IndexOf(expression);
				return index==-1?0:expressionTypes.Values()[index];
			}

			BasicEnv::Reference BasicEnv::GetReference(BasicReferenceExpression* expression)
			{
				return referenceTypes[expression];
			}
		}
	}
}