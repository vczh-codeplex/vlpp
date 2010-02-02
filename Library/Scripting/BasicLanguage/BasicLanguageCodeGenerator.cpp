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

			void BasicScope::Initialize()
			{
				types.Initialize(this, &BasicScope::types, 0);
				variables.Initialize(this, &BasicScope::variables, 0);
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

			void BasicEnv::RegisterFunctionType(BasicFunctionDeclaration* function, BasicTypeRecord* type)
			{
				functionTypes.Add(function, type);
			}

			void BasicEnv::RegisterExpressionType(BasicExpression* expression, BasicTypeRecord* type)
			{
				expressionTypes.Add(expression, type);
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

/***********************************************************************
GetTypeRecord
***********************************************************************/

			BasicLanguageCodeException::BasicLanguageCodeException(BasicLanguageElement* _element, ExceptionCode _exceptionCode, const collections::IReadonlyList<WString>& _parameters)
				:Exception(L"����BasicLanguage�����������")
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
BasicAlgorithmParameter
***********************************************************************/

			BasicAlgorithmConfiguration::BasicAlgorithmConfiguration()
			{
				integerConversion=FreeConversion;
				treatCharacterAsInteger=true;
				treatCharAsSignedInteger=true;
				treatWCharAsSignedInteger=false;
				treatBooleanAsInteger=true;
				enablePointerArithmetic=true;
				enableImplicitIntegerToBooleanConversion=true;
				enableImplicitPointerToBooleanConversion=true;
				enableSubscribeOnPointer=true;
			}

			bool BasicAlgorithmConfiguration::DecodeInteger(BasicPrimitiveTypeEnum type, bool& sign, int& bytes)
			{
				switch(type)
				{
				case s8:
					sign=true;
					bytes=1;
					return true;
				case s16:
					sign=true;
					bytes=2;
					return true;
				case s32:
					sign=true;
					bytes=4;
					return true;
				case s64:
					sign=true;
					bytes=8;
					return true;
				case u8:
					sign=false;
					bytes=1;
					return true;
				case u16:
					sign=false;
					bytes=2;
					return true;
				case u32:
					sign=false;
					bytes=4;
					return true;
				case u64:
					sign=false;
					bytes=8;
					return true;
				case bool_type:
					if(treatBooleanAsInteger)
					{
						sign=false;
						bytes=1;
						return true;
					}
					else
					{
						return false;
					}
				case char_type:
					if(treatCharacterAsInteger)
					{
						sign=treatCharAsSignedInteger;
						bytes=1;
						return true;
					}
					else
					{
						return false;
					}
				case wchar_type:
					if(treatCharacterAsInteger)
					{
						sign=treatWCharAsSignedInteger;
						bytes=2;
						return true;
					}
					else
					{
						return false;
					}
				default:
					return false;
				}
			}

			bool BasicAlgorithmConfiguration::EncodeInteger(BasicPrimitiveTypeEnum& type, bool sign, int bytes)
			{
				switch(bytes)
				{
				case 1:
					type=sign?s8:u8;
					return true;
				case 2:
					type=sign?s16:u16;
					return true;
				case 4:
					type=sign?s32:u32;
					return true;
				case 8:
					type=sign?s64:u64;
					return true;
				default:
					return false;
				}
			}

			bool BasicAlgorithmConfiguration::CanConvertToBoolean(BasicPrimitiveTypeEnum type)
			{
				switch(type)
				{
				case s8:
				case s16:
				case s32:
				case s64:
				case u8:
				case u16:
				case u32:
				case u64:
					return enableImplicitIntegerToBooleanConversion;
				case bool_type:
					return true;
				case char_type:
				case wchar_type:
					return enableImplicitIntegerToBooleanConversion && treatCharacterAsInteger;
				default:
					return false;
				}
			}

/***********************************************************************
BasicSemanticExtension
***********************************************************************/

			Ptr<BasicExpression> BasicSemanticExtension::ExpressionReplacer(Ptr<BasicExpression> originalExpression, BP& argument)
			{
				return originalExpression;
			}

			Ptr<BasicStatement> BasicSemanticExtension::StatementReplacer(Ptr<BasicExpression> originalStatement, BP& argument)
			{
				return originalStatement;
			}

			BasicTypeRecord* BasicSemanticExtension::GetTypeRecord(BasicExtendedType* type, const BP& argument)
			{
				CHECK_ERROR(false, L"BasicSemanticExtension::::GetTypeRecord(BasicExtendedType*, const BP&)#��֧�ִ˲�����");
			}

			void BasicSemanticExtension::BuildGlobalScopePass1(BasicExtendedDeclaration* declaration, const BP& argument)
			{
				CHECK_ERROR(false, L"BasicSemanticExtension::::BuildGlobalScopePass1(BasicExtendedDeclaration*, const BP&)#��֧�ִ˲�����");
			}

			void BasicSemanticExtension::BuildGlobalScopePass2(BasicExtendedDeclaration* declaration, const BP& argument)
			{
				CHECK_ERROR(false, L"BasicSemanticExtension::::BuildGlobalScopePass2(BasicExtendedDeclaration*, const BP&)#��֧�ִ˲�����");
			}

			BasicTypeRecord* BasicSemanticExtension::GetExpressionType(BasicExtendedExpression* expression, const BP& argument)
			{
				CHECK_ERROR(false, L"BasicSemanticExtension::::GetExpressionType(BasicExtendedExpression*, const BP&)#��֧�ִ˲�����");
			}

			BasicSemanticExtension defaultSemanticExtension;

			BasicAlgorithmParameter::BasicAlgorithmParameter(
				BasicEnv* _env,
				BasicScope* _scope,
				BasicTypeManager* _typeManager,
				List<Ptr<BasicLanguageCodeException>>& _errors,
				collections::SortedList<WString>& _forwardStructures
				)
				:env(_env)
				,scope(_scope)
				,typeManager(_typeManager)
				,errors(_errors)
				,forwardStructures(_forwardStructures)
				,semanticExtension(&defaultSemanticExtension)
			{
			}

			BasicAlgorithmParameter::BasicAlgorithmParameter(const BasicAlgorithmParameter& bp, BasicScope* _scope)
				:env(bp.env)
				,scope(_scope)
				,typeManager(bp.typeManager)
				,errors(bp.errors)
				,forwardStructures(bp.forwardStructures)
				,semanticExtension(bp.semanticExtension)
			{
			}

/***********************************************************************
BasicLanguage_GetTypeRecord
***********************************************************************/

			BEGIN_ALGORITHM_FUNCTION(BasicLanguage_GetTypeRecord, BasicType, BP, BasicTypeRecord*)

				ALGORITHM_FUNCTION_MATCH(BasicPrimitiveType)
				{
					return argument.typeManager->GetPrimitiveType(node->type);
				}

				ALGORITHM_FUNCTION_MATCH(BasicPointerType)
				{
					return argument.typeManager->GetPointerType(BasicLanguage_GetTypeRecord(node->elementType, argument));
				}

				ALGORITHM_FUNCTION_MATCH(BasicArrayType)
				{
					return argument.typeManager->GetArrayType(BasicLanguage_GetTypeRecord(node->elementType, argument), node->size);
				}

				ALGORITHM_FUNCTION_MATCH(BasicReferenceType)
				{
					BasicTypeRecord* type=argument.scope->types.Find(node->name);
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
					return argument.typeManager->GetFunctionType(
						BasicLanguage_GetTypeRecord(node->returnType, argument),
						parameterTypes.Wrap()
						);
				}

				ALGORITHM_FUNCTION_MATCH(BasicExtendedType)
				{
					return argument.semanticExtension->GetTypeRecord(node, argument);
				}

			END_ALGORITHM_FUNCTION(BasicLanguage_GetTypeRecord)

/***********************************************************************
BasicLanguage_BuildGlobalScopePass1
***********************************************************************/

			BEGIN_ALGORITHM_PROCEDURE(BasicLanguage_BuildGlobalScopePass1, BasicDeclaration, BP)

				ALGORITHM_PROCEDURE_MATCH(BasicFunctionDeclaration)
				{
					if(argument.scope->functions.Items().Keys().Contains(node->name))
					{
						argument.errors.Add(BasicLanguageCodeException::GetFunctionAlreadyExists(node));
					}
					else
					{
						argument.scope->functions.Add(node->name, node);
						BasicTypeRecord* type=0;
						try
						{
							type=BasicLanguage_GetTypeRecord(node->signatureType, argument);
						}
						catch(Ptr<BasicLanguageCodeException> e)
						{
							argument.errors.Add(e);
						}
						if(type)
						{
							argument.env->RegisterFunctionType(node, type);
						}
					}
				}
			
				ALGORITHM_PROCEDURE_MATCH(BasicStructureDeclaration)
				{
					if(node->defined)
					{
						if(argument.scope->types.Items().Keys().Contains(node->name))
						{
							int forward=argument.forwardStructures.IndexOf(node->name);
							if(forward==-1)
							{
								argument.errors.Add(BasicLanguageCodeException::GetTypeAlreadyExists(node));
							}
							else
							{
								argument.forwardStructures.RemoveAt(forward);
							}
						}
						else
						{
							BasicTypeRecord* structure=argument.typeManager->CreateStructureType();
							argument.scope->types.Add(node->name, structure);
						}
					}
					else
					{
						if(!argument.forwardStructures.Contains(node->name))
						{
							argument.forwardStructures.Add(node->name);
							BasicTypeRecord* structure=argument.typeManager->CreateStructureType();
							argument.scope->types.Add(node->name, structure);
						}
					}
				}
				
				ALGORITHM_PROCEDURE_MATCH(BasicVariableDeclaration)
				{
					if(argument.scope->variables.Items().Keys().Contains(node->name))
					{
						argument.errors.Add(BasicLanguageCodeException::GetVariableAlreadyExists(node));
					}
					else
					{
						try
						{
							argument.scope->variables.Add(node->name, BasicLanguage_GetTypeRecord(node->type, argument));
						}
						catch(Ptr<BasicLanguageCodeException> e)
						{
							argument.errors.Add(e);
						}
					}
				}
				
				ALGORITHM_PROCEDURE_MATCH(BasicTypeRenameDeclaration)
				{
					if(argument.scope->types.Items().Keys().Contains(node->name))
					{
						argument.errors.Add(BasicLanguageCodeException::GetTypeAlreadyExists(node));
					}
					else
					{
						try
						{
							argument.scope->types.Add(node->name, BasicLanguage_GetTypeRecord(node->type, argument));
						}
						catch(Ptr<BasicLanguageCodeException> e)
						{
							argument.errors.Add(e);
						}
					}
				}

				ALGORITHM_PROCEDURE_MATCH(BasicExtendedDeclaration)
				{
					argument.semanticExtension->BuildGlobalScopePass1(node, argument);
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
					BasicStructureTypeRecord* structure=dynamic_cast<BasicStructureTypeRecord*>(argument.scope->types.Items()[node->name]);
					List<WString> names;
					List<BasicTypeRecord*> types;
					for(int i=0;i<node->memberNames.Count();i++)
					{
						if(node->memberNames.IndexOf(node->memberNames[i])==i)
						{
							try
							{
								types.Add(BasicLanguage_GetTypeRecord(node->memberTypes[i], argument));
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
					argument.typeManager->UpdateStructureType(structure, names.Wrap(), types.Wrap());
				}
				
				ALGORITHM_PROCEDURE_MATCH(BasicVariableDeclaration)
				{
				}
				
				ALGORITHM_PROCEDURE_MATCH(BasicTypeRenameDeclaration)
				{
				}

				ALGORITHM_PROCEDURE_MATCH(BasicExtendedDeclaration)
				{
					argument.semanticExtension->BuildGlobalScopePass2(node, argument);
				}

			END_ALGORITHM_PROCEDURE(BasicLanguage_BuildGlobalScopePass2)

/***********************************************************************
BasicLanguage_BuildGlobalScope
***********************************************************************/

			void BasicLanguage_BuildGlobalScope(Ptr<BasicProgram> program, BP& argument)
			{
				for(int i=0;i<program->declarations.Count();i++)
				{
					BasicLanguage_BuildGlobalScopePass1(program->declarations[i], argument);
				}
				for(int i=0;i<program->declarations.Count();i++)
				{
					BasicLanguage_BuildGlobalScopePass2(program->declarations[i], argument);
				}
			}

/***********************************************************************
BasicLanguage_GetExpressionType
***********************************************************************/

			BasicTypeRecord* BasicLanguage_GetExpressionType(Ptr<BasicExpression>& expression, BP& argument)
			{
				BasicTypeRecord* type=argument.env->GetExpressionType(expression.Obj());
				if(!type)
				{
					expression=argument.semanticExtension->ExpressionReplacer(expression, argument);
					try
					{
						type=BasicLanguage_GetExpressionTypeInternal(expression, argument);
						argument.env->RegisterExpressionType(expression.Obj(), type);
					}
					catch(Ptr<BasicLanguageCodeException> e)
					{
						argument.errors.Add(e);
					}
				}
				return type;
			}

			BEGIN_ALGORITHM_FUNCTION(BasicLanguage_GetExpressionTypeInternal, BasicExpression, BP, BasicTypeRecord*)

				ALGORITHM_FUNCTION_MATCH(BasicNumericExpression)
				{
					return 0;
				}

				ALGORITHM_FUNCTION_MATCH(BasicMbcsStringExpression)
				{
					return 0;
				}

				ALGORITHM_FUNCTION_MATCH(BasicUnicodeStringExpression)
				{
					return 0;
				}

				ALGORITHM_FUNCTION_MATCH(BasicUnaryExpression)
				{
					return 0;
				}

				ALGORITHM_FUNCTION_MATCH(BasicBinaryExpression)
				{
					return 0;
				}

				ALGORITHM_FUNCTION_MATCH(BasicSubscribeExpression)
				{
					return 0;
				}

				ALGORITHM_FUNCTION_MATCH(BasicMemberExpression)
				{
					return 0;
				}

				ALGORITHM_FUNCTION_MATCH(BasicInvokeExpression)
				{
					return 0;
				}

				ALGORITHM_FUNCTION_MATCH(BasicFunctionResultExpression)
				{
					return 0;
				}

				ALGORITHM_FUNCTION_MATCH(BasicCastingExpression)
				{
					return 0;
				}

				ALGORITHM_FUNCTION_MATCH(BasicReferenceExpression)
				{
					return 0;
				}

				ALGORITHM_FUNCTION_MATCH(BasicExtendedExpression)
				{
					return argument.semanticExtension->GetExpressionType(node, argument);
				}

			END_ALGORITHM_FUNCTION(BasicLanguage_GetExpressionTypeInternal)

/***********************************************************************
BasicLanguage_BuildGlobalScope
***********************************************************************/

		}
	}
}