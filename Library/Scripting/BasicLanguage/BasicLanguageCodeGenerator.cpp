#include "BasicLanguageCodeGenerator.h"
#include "..\..\Collections\OperationCopyFrom.h"
#include "..\Common\AlgorithmDeclaration.h"

namespace vl
{
	namespace scripting
	{
		namespace basiclanguage
		{
			using namespace collections;

/***********************************************************************
BasicScope
***********************************************************************/

			template<typename T>
			T* GetTypeInternal(const WString& name, collections::Dictionary<WString, T*> BasicScope::* container, BasicScope*& scope)
			{
				while(scope)
				{
					int index=(scope->*container).Keys().IndexOf(name);
					if(index!=-1)
					{
						return (scope->*container).Values()[index];
					}
					scope=scope->PreviousScope();
				}
				return 0;
			}

			BasicScope::BasicScope(BasicTypeManager* _typeManager, BasicFunctionDeclaration* _functionDeclaration, BasicStatement* _statement)
				:previousScope(0)
				,typeManager(_typeManager)
				,functionDeclaration(_functionDeclaration)
				,statement(_statement)
			{
			}

			BasicScope::BasicScope(BasicScope* _previousScope, BasicFunctionDeclaration* _functionDeclaration)
				:previousScope(_previousScope)
				,typeManager(_previousScope->typeManager)
				,functionDeclaration(_functionDeclaration)
				,statement(0)
			{
			}

			BasicScope::BasicScope(BasicScope* _previousScope, BasicStatement* _statement)
				:previousScope(_previousScope)
				,typeManager(_previousScope->typeManager)
				,functionDeclaration(_previousScope->functionDeclaration)
				,statement(_statement)
			{
			}

			BasicScope::BasicScope(BasicScope* _previousScope)
				:previousScope(_previousScope)
				,typeManager(_previousScope->typeManager)
				,functionDeclaration(_previousScope->functionDeclaration)
				,statement(_previousScope->statement)
			{
			}

			BasicScope::~BasicScope()
			{
			}

			BasicScope* BasicScope::PreviousScope()
			{
				return previousScope;
			}

			BasicTypeManager* BasicScope::TypeManager()
			{
				return typeManager;
			}
			
			BasicFunctionDeclaration* BasicScope::OwnerDeclaration()
			{
				return functionDeclaration;
			}

			BasicStatement* BasicScope::OwnerStatement()
			{
				return statement;
			}

			collections::IDictionary<WString, BasicTypeRecord*>& BasicScope::Types()
			{
				return types.Wrap();
			}

			collections::IDictionary<WString, BasicTypeRecord*>& BasicScope::Variables()
			{
				return variables.Wrap();
			}

			collections::IDictionary<WString, BasicFunctionDeclaration*>& BasicScope::Functions()
			{
				return functions.Wrap();
			}

			BasicTypeRecord* BasicScope::GetType(const WString& name)
			{
				BasicScope* scope=this;
				return GetTypeInternal(name, &BasicScope::types, scope);
			}

			BasicTypeRecord* BasicScope::GetType(const WString& name, BasicScope*& scope)
			{
				scope=this;
				return GetTypeInternal(name, &BasicScope::types, scope);
			}

			BasicTypeRecord* BasicScope::GetVariableType(const WString& name)
			{
				BasicScope* scope=this;
				return GetTypeInternal(name, &BasicScope::variables, scope);
			}

			BasicTypeRecord* BasicScope::GetVariableType(const WString& name, BasicScope*& scope)
			{
				scope=this;
				return GetTypeInternal(name, &BasicScope::variables, scope);
			}
			
			BasicFunctionDeclaration* BasicScope::GetFunction(const WString& name)
			{
				BasicScope* scope=this;
				return GetTypeInternal(name, &BasicScope::functions, scope);
			}

			BasicFunctionDeclaration* BasicScope::GetFunction(const WString& name, BasicScope*& scope)
			{
				scope=this;
				return GetTypeInternal(name, &BasicScope::functions, scope);
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

			BasicTypeManager& BasicEnv::TypeManager()
			{
				return typeManager;
			}

			BasicScope* BasicEnv::GlobalScope()
			{
				return globalScope;
			}

			BasicScope* BasicEnv::CreateScope(BasicScope* previousScope)
			{
				BasicScope* scope=previousScope?new BasicScope(previousScope):new BasicScope(&typeManager, 0, 0);
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

/***********************************************************************
GetTypeRecord
***********************************************************************/

			BasicLanguageCodeException::BasicLanguageCodeException(BasicLanguageElement* _element, ExceptionCode _exceptionCode, const collections::IReadonlyList<WString>& _parameters)
				:Exception(L"发生BasicLanguage语义分析错误。")
				,element(_element)
				,exceptionCode(_exceptionCode)
			{
				CopyFrom(parameters.Wrap(), _parameters);
			}

			BasicLanguageCodeException::BasicLanguageCodeException(const BasicLanguageCodeException& exception)
				:Exception(exception.Message())
				,element(exception.element)
				,exceptionCode(exception.exceptionCode)
			{
				CopyFrom(parameters.Wrap(), exception.parameters.Wrap());
			}

			BasicLanguageCodeException::~BasicLanguageCodeException()
			{
			}

			BasicLanguageElement* BasicLanguageCodeException::GetBasicLanguageElement()const
			{
				return element;
			}

			BasicLanguageCodeException::ExceptionCode BasicLanguageCodeException::GetExceptionCode()const
			{
				return exceptionCode;
			}

			const collections::IReadonlyList<WString>& BasicLanguageCodeException::GetParameters()const
			{
				return parameters.Wrap();
			}

			Ptr<BasicLanguageCodeException> BasicLanguageCodeException::GetTypeNameNotExists(BasicReferenceType* type)
			{
				Array<WString> parameters(1);
				parameters[0]=type->name;
				return new BasicLanguageCodeException(type, TypeNameNotExists, parameters.Wrap());
			}

			Ptr<BasicLanguageCodeException> BasicLanguageCodeException::GetFunctionAlreadyExists(BasicFunctionDeclaration* function)
			{
				Array<WString> parameters(1);
				parameters[0]=function->name;
				return new BasicLanguageCodeException(function, FunctionAlreadyExists, parameters.Wrap());
			}

			Ptr<BasicLanguageCodeException> BasicLanguageCodeException::GetVariableAlreadyExists(BasicVariableDeclaration* variable)
			{
				Array<WString> parameters(1);
				parameters[0]=variable->name;
				return new BasicLanguageCodeException(variable, VariableAlreadyExists, parameters.Wrap());
			}

			Ptr<BasicLanguageCodeException> BasicLanguageCodeException::GetTypeAlreadyExists(BasicStructureDeclaration* type)
			{
				Array<WString> parameters(1);
				parameters[0]=type->name;
				return new BasicLanguageCodeException(type, TypeAlreadyExists, parameters.Wrap());
			}

			Ptr<BasicLanguageCodeException> BasicLanguageCodeException::GetTypeAlreadyExists(BasicTypeRenameDeclaration* type)
			{
				Array<WString> parameters(1);
				parameters[0]=type->name;
				return new BasicLanguageCodeException(type, TypeAlreadyExists, parameters.Wrap());
			}

			Ptr<BasicLanguageCodeException> BasicLanguageCodeException::GetStructureMemberAlreadyExists(BasicStructureDeclaration* type, int memberIndex)
			{
				Array<WString> parameters(1);
				parameters[0]=type->memberNames[memberIndex];
				return new BasicLanguageCodeException(type, TypeAlreadyExists, parameters.Wrap());
			}

/***********************************************************************
GetTypeRecord
***********************************************************************/

			BasicAlgorithmParameter::BasicAlgorithmParameter(BasicEnv* _env, BasicScope* _scope, List<Ptr<BasicLanguageCodeException>>& _errors)
				:env(_env)
				,scope(_scope)
				,errors(_errors)
			{
			}

/***********************************************************************
GetTypeRecord
***********************************************************************/

			BEGIN_ALGORITHM_FUNCTION(BasicLanguage_GetTypeRecord, BasicType, BasicScope*, BasicTypeRecord*)

				ALGORITHM_FUNCTION_MATCH(BasicPrimitiveType)
				{
					return argument->TypeManager()->GetPrimitiveType(node->type);
				}

				ALGORITHM_FUNCTION_MATCH(BasicPointerType)
				{
					return argument->TypeManager()->GetPointerType(BasicLanguage_GetTypeRecord(node->elementType, argument));
				}

				ALGORITHM_FUNCTION_MATCH(BasicArrayType)
				{
					return argument->TypeManager()->GetArrayType(BasicLanguage_GetTypeRecord(node->elementType, argument), node->size);
				}

				ALGORITHM_FUNCTION_MATCH(BasicReferenceType)
				{
					BasicTypeRecord* type=argument->GetType(node->name);
					if(type)
					{
						return type;
					}
					else
					{
						throw BasicLanguageCodeException::GetTypeNameNotExists(node);
					}
				}

				ALGORITHM_FUNCTION_MATCH(BasicFunctionType)
				{
					List<BasicTypeRecord*> parameterTypes;
					for(int i=0;i<node->parameterTypes.Count();i++)
					{
						parameterTypes.Add(BasicLanguage_GetTypeRecord(node->parameterTypes[i], argument));
					}
					return argument->TypeManager()->GetFunctionType(
						BasicLanguage_GetTypeRecord(node->returnType, argument),
						parameterTypes.Wrap()
						);
				}

			END_ALGORITHM_FUNCTION(BasicLanguage_GetTypeRecord)

/***********************************************************************
BasicLanguage_BuildGlobalScopePass1
***********************************************************************/

			BEGIN_ALGORITHM_PROCEDURE(BasicLanguage_BuildGlobalScopePass1, BasicDeclaration, BP)

				ALGORITHM_PROCEDURE_MATCH(BasicFunctionDeclaration)
				{
					if(argument.scope->Functions().Keys().Contains(node->name))
					{
						argument.errors.Add(BasicLanguageCodeException::GetFunctionAlreadyExists(node));
					}
					else
					{
						argument.scope->Functions().Add(node->name, node);
					}
				}
			
				ALGORITHM_PROCEDURE_MATCH(BasicStructureDeclaration)
				{
					if(argument.scope->Types().Keys().Contains(node->name))
					{
						argument.errors.Add(BasicLanguageCodeException::GetTypeAlreadyExists(node));
					}
					else
					{
						BasicTypeRecord* structure=argument.env->TypeManager().CreateStructureType();
						argument.scope->Types().Add(node->name, structure);
					}
				}
				
				ALGORITHM_PROCEDURE_MATCH(BasicVariableDeclaration)
				{
					if(argument.scope->Variables().Keys().Contains(node->name))
					{
						argument.errors.Add(BasicLanguageCodeException::GetVariableAlreadyExists(node));
					}
					else
					{
						try
						{
							argument.scope->Variables().Add(node->name, BasicLanguage_GetTypeRecord(node->type, argument.scope));
						}
						catch(Ptr<BasicLanguageCodeException> e)
						{
							argument.errors.Add(e);
						}
					}
				}
				
				ALGORITHM_PROCEDURE_MATCH(BasicTypeRenameDeclaration)
				{
					if(argument.scope->Types().Keys().Contains(node->name))
					{
						argument.errors.Add(BasicLanguageCodeException::GetTypeAlreadyExists(node));
					}
					else
					{
						try
						{
							argument.scope->Types().Add(node->name, BasicLanguage_GetTypeRecord(node->type, argument.scope));
						}
						catch(Ptr<BasicLanguageCodeException> e)
						{
							argument.errors.Add(e);
						}
					}
				}

			END_ALGORITHM_PROCEDURE(BasicLanguage_BuildGlobalScopePass1)

/***********************************************************************
BasicLanguage_BuildGlobalScopePass2
***********************************************************************/

			BEGIN_ALGORITHM_PROCEDURE(BasicLanguage_BuildGlobalScopePass2, BasicDeclaration, BP)

				ALGORITHM_PROCEDURE_MATCH(BasicFunctionDeclaration)
				{
				}
			
				ALGORITHM_PROCEDURE_MATCH(BasicStructureDeclaration)
				{
					BasicStructureTypeRecord* structure=dynamic_cast<BasicStructureTypeRecord*>(argument.scope->Types()[node->name]);
					List<WString> names;
					List<BasicTypeRecord*> types;
					for(int i=0;i<node->memberNames.Count();i++)
					{
						if(node->memberNames.IndexOf(node->memberNames[i])==i)
						{
							try
							{
								types.Add(BasicLanguage_GetTypeRecord(node->memberTypes[i], argument.scope));
								names.Add(node->memberNames[i]);
							}
							catch(Ptr<BasicLanguageCodeException> e)
							{
								argument.errors.Add(e);
							}
						}
						else
						{
							argument.errors.Add(BasicLanguageCodeException::GetStructureMemberAlreadyExists(node, i));
						}
					}
					argument.env->TypeManager().UpdateStructureType(structure, names.Wrap(), types.Wrap());
				}
				
				ALGORITHM_PROCEDURE_MATCH(BasicVariableDeclaration)
				{
				}
				
				ALGORITHM_PROCEDURE_MATCH(BasicTypeRenameDeclaration)
				{
				}

			END_ALGORITHM_PROCEDURE(BasicLanguage_BuildGlobalScopePass2)

		}
	}
}