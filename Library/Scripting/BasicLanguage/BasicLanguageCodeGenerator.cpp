#include "BasicLanguageCodeGenerator.h"
#include "..\..\Collections\Operation.h"
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
				:element(_element)
				,exceptionCode(_exceptionCode)
			{
				CopyFrom(parameters.Wrap(), _parameters);
			}

			BasicLanguageCodeException::BasicLanguageCodeException(const BasicLanguageCodeException& exception)
				:element(exception.element)
				,exceptionCode(exception.exceptionCode)
			{
				CopyFrom(parameters.Wrap(), exception.parameters.Wrap());
			}

			BasicLanguageCodeException::~BasicLanguageCodeException()
			{
			}

			BasicLanguageElement* BasicLanguageCodeException::GetBasicLanguageElement()
			{
				return element;
			}

			BasicLanguageCodeException::ExceptionCode BasicLanguageCodeException::GetExceptionCode()
			{
				return exceptionCode;
			}

			const collections::IReadonlyList<WString>& BasicLanguageCodeException::GetParameters()
			{
				return parameters.Wrap();
			}

			BasicLanguageCodeException BasicLanguageCodeException::GetTypeNameNotExists(BasicReferenceType* type)
			{
				Array<WString> parameters(1);
				parameters[0]=type->name;
				return BasicLanguageCodeException(type, TypeNameNotExists, parameters.Wrap());
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
BuildGlobalScope
***********************************************************************/

			BEGIN_ALGORITHM_PROCEDURE(BasicLanguage_BuildGlobalScope, BasicDeclaration, BasicEnv*)

				ALGORITHM_PROCEDURE_MATCH(BasicFunctionDeclaration)
				{
				}
			
				ALGORITHM_PROCEDURE_MATCH(BasicStructureDeclaration)
				{
				}
				
				ALGORITHM_PROCEDURE_MATCH(BasicVariableDeclaration)
				{
				}
				
				ALGORITHM_PROCEDURE_MATCH(BasicTypeRenameDeclaration)
				{
				}

			END_ALGORITHM_PROCEDURE(BasicLanguage_BuildGlobalScope)

		}
	}
}