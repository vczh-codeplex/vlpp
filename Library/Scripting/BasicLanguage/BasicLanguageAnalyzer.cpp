#include "BasicLanguageAnalyzer.h"

namespace vl
{
	namespace scripting
	{
		namespace basiclanguage
		{
			using namespace collections;

/***********************************************************************
BasicSemanticExtension
***********************************************************************/

			Ptr<BasicExpression> BasicSemanticExtension::ExpressionReplacer(Ptr<BasicExpression> originalExpression, const BP& argument)
			{
				return originalExpression;
			}

			Ptr<BasicStatement> BasicSemanticExtension::StatementReplacer(Ptr<BasicExpression> originalStatement, const BP& argument)
			{
				return originalStatement;
			}

			BasicTypeRecord* BasicSemanticExtension::GetTypeRecord(BasicExtendedType* type, const BP& argument)
			{
				CHECK_ERROR(false, L"BasicSemanticExtension::::GetTypeRecord(BasicExtendedType*, const BP&)#不支持此操作。");
			}

			void BasicSemanticExtension::BuildGlobalScopePass1(BasicExtendedDeclaration* declaration, const BP& argument)
			{
				CHECK_ERROR(false, L"BasicSemanticExtension::::BuildGlobalScopePass1(BasicExtendedDeclaration*, const BP&)#不支持此操作。");
			}

			void BasicSemanticExtension::BuildGlobalScopePass2(BasicExtendedDeclaration* declaration, const BP& argument)
			{
				CHECK_ERROR(false, L"BasicSemanticExtension::::BuildGlobalScopePass2(BasicExtendedDeclaration*, const BP&)#不支持此操作。");
			}

			bool BasicSemanticExtension::IsLeftValue(BasicExtendedExpression* expression, const BP& argument)
			{
				CHECK_ERROR(false, L"BasicSemanticExtension::::IsLeftValue(BasicExtendedExpression*, const BP&)#不支持此操作。");
			}

			BasicTypeRecord* BasicSemanticExtension::GetExpressionType(BasicExtendedExpression* expression, const BP& argument)
			{
				CHECK_ERROR(false, L"BasicSemanticExtension::::GetExpressionType(BasicExtendedExpression*, const BP&)#不支持此操作。");
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
BasicLanguage_IsLeftValue
***********************************************************************/

			BEGIN_ALGORITHM_FUNCTION(BasicLanguage_IsLeftValue, BasicExpression, BP, bool)

				ALGORITHM_FUNCTION_MATCH(BasicNullExpression)
				{
					return false;
				}

				ALGORITHM_FUNCTION_MATCH(BasicNumericExpression)
				{
					return false;
				}

				ALGORITHM_FUNCTION_MATCH(BasicMbcsStringExpression)
				{
					return false;
				}

				ALGORITHM_FUNCTION_MATCH(BasicUnicodeStringExpression)
				{
					return false;
				}

				ALGORITHM_FUNCTION_MATCH(BasicUnaryExpression)
				{
					switch(node->type)
					{
					case BasicUnaryExpression::PrefixIncrease:
					case BasicUnaryExpression::PrefixDecrease:
					case BasicUnaryExpression::DereferencePointer:
						return true;
					default:
						return false;
					}
				}

				ALGORITHM_FUNCTION_MATCH(BasicBinaryExpression)
				{
					switch(node->type)
					{
					case BasicBinaryExpression::AddAssign:
					case BasicBinaryExpression::SubAssign:
					case BasicBinaryExpression::MulAssign:
					case BasicBinaryExpression::DivAssign:
					case BasicBinaryExpression::ModAssign:
					case BasicBinaryExpression::ShlAssign:
					case BasicBinaryExpression::ShrAssign:
					case BasicBinaryExpression::AndAssign:
					case BasicBinaryExpression::OrAssign:
					case BasicBinaryExpression::XorAssign:
					case BasicBinaryExpression::Assign:
						return true;
					default:
						return false;
					}
				}

				ALGORITHM_FUNCTION_MATCH(BasicSubscribeExpression)
				{
					return true;
				}

				ALGORITHM_FUNCTION_MATCH(BasicMemberExpression)
				{
					return true;
				}

				ALGORITHM_FUNCTION_MATCH(BasicInvokeExpression)
				{
					return false;
				}

				ALGORITHM_FUNCTION_MATCH(BasicFunctionResultExpression)
				{
					return true;
				}

				ALGORITHM_FUNCTION_MATCH(BasicCastingExpression)
				{
					return false;
				}

				ALGORITHM_FUNCTION_MATCH(BasicReferenceExpression)
				{
					return true;
				}

				ALGORITHM_FUNCTION_MATCH(BasicExtendedExpression)
				{
					return argument.semanticExtension->IsLeftValue(node, argument);
				}

			END_ALGORITHM_FUNCTION(BasicLanguage_IsLeftValue)

/***********************************************************************
BasicLanguage_GetExpressionType
***********************************************************************/

			BasicTypeRecord* BasicLanguage_GetExpressionType(Ptr<BasicExpression>& expression, const BP& argument)
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

			bool CanImplicitConvertTo(BasicTypeRecord* from, BasicTypeRecord* to, const BP& argument)
			{
				if(from==to)
				{
					return true;
				}
				else if(from->GetType()==BasicTypeRecord::Primitive && to->GetType()==BasicTypeRecord::Primitive)
				{
					return argument.configuration.CanImplicitConvertTo(from->PrimitiveType(), to->PrimitiveType());
				}
				else if(from->GetType()==BasicTypeRecord::Pointer && to->GetType()==BasicTypeRecord::Pointer)
				{
					return from->ElementType()==argument.typeManager->GetPrimitiveType(void_type);
				}
				else
				{
					return false;
				}
			}

			BEGIN_ALGORITHM_FUNCTION(BasicLanguage_GetExpressionTypeInternal, BasicExpression, BP, BasicTypeRecord*)

				ALGORITHM_FUNCTION_MATCH(BasicNullExpression)
				{
					return argument.typeManager->GetPointerType(argument.typeManager->GetPrimitiveType(void_type));
				}

				ALGORITHM_FUNCTION_MATCH(BasicNumericExpression)
				{
					return argument.typeManager->GetPrimitiveType(node->type);
				}

				ALGORITHM_FUNCTION_MATCH(BasicMbcsStringExpression)
				{
					return argument.typeManager->GetPointerType(argument.typeManager->GetPrimitiveType(char_type));
				}

				ALGORITHM_FUNCTION_MATCH(BasicUnicodeStringExpression)
				{
					return argument.typeManager->GetPointerType(argument.typeManager->GetPrimitiveType(wchar_type));
				}

				ALGORITHM_FUNCTION_MATCH(BasicUnaryExpression)
				{
					BasicTypeRecord* operandType=BasicLanguage_GetExpressionType(node->operand, argument);
					if(operandType) 
					switch(node->type)
					{
					case BasicUnaryExpression::PrefixIncrease:
					case BasicUnaryExpression::PrefixDecrease:
					case BasicUnaryExpression::PostfixIncrease:
					case BasicUnaryExpression::PostfixDecrease:
						if(!BasicLanguage_IsLeftValue(node->operand, argument))
						{
							argument.errors.Add(BasicLanguageCodeException::GetUnaryOperandShouldBeLeftValue(node));
						}
					case BasicUnaryExpression::BitNot:
						if(operandType->GetType()==BasicTypeRecord::Primitive)
						{
							BasicPrimitiveTypeEnum result=void_type;
							if(argument.configuration.IntegerUnaryOperatorTypeConversion(operandType->PrimitiveType(), result))
							{
								return argument.typeManager->GetPrimitiveType(result);
							}
						}
						break;
					case BasicUnaryExpression::GetAddress:
						if(!BasicLanguage_IsLeftValue(node->operand, argument))
						{
							argument.errors.Add(BasicLanguageCodeException::GetUnaryOperandShouldBeLeftValue(node));
						}
						return argument.typeManager->GetPointerType(operandType);
					case BasicUnaryExpression::DereferencePointer:
						if(operandType->GetType()==BasicTypeRecord::Pointer)
						{
							return operandType->ElementType();
						}
						break;
					case BasicUnaryExpression::Negative:
						if(operandType->GetType()==BasicTypeRecord::Primitive)
						{
							BasicPrimitiveTypeEnum result=void_type;
							if(argument.configuration.NumberUnaryOperatorTypeConversion(operandType->PrimitiveType(), result))
							{
								return argument.typeManager->GetPrimitiveType(result);
							}
						}
						break;
					case BasicUnaryExpression::Not:
						if(operandType->GetType()==BasicTypeRecord::Primitive)
						{
							BasicPrimitiveTypeEnum result=void_type;
							if(argument.configuration.BooleanUnaryOperatorTypeConversion(operandType->PrimitiveType(), result))
							{
								return argument.typeManager->GetPrimitiveType(result);
							}
						}
						break;
					}
					argument.errors.Add(BasicLanguageCodeException::GetUnaryTypeNotMatch(node));
					return 0;
				}

				ALGORITHM_FUNCTION_MATCH(BasicBinaryExpression)
				{
					return 0;
				}

				ALGORITHM_FUNCTION_MATCH(BasicSubscribeExpression)
				{
					BasicTypeRecord* operandType=BasicLanguage_GetExpressionType(node->operand, argument);
					BasicTypeRecord* indexType=BasicLanguage_GetExpressionType(node->subscribe, argument);
					if(operandType && indexType)
					{
						if(indexType->GetType()!=BasicTypeRecord::Primitive || !argument.configuration.CanImplicitConvertTo(indexType->PrimitiveType(), int_type))
						{
							argument.errors.Add(BasicLanguageCodeException::GetCannotConvertIndexToInt(node));
						}
						if(operandType->GetType()==BasicTypeRecord::Array)
						{
							return operandType->ElementType();
						}
						else if(operandType->GetType()==BasicTypeRecord::Pointer && argument.configuration.enableSubscribeOnPointer)
						{
							return operandType->ElementType();
						}
						argument.errors.Add(BasicLanguageCodeException::GetCannotSubscribe(node));
					}
					return 0;
				}

				ALGORITHM_FUNCTION_MATCH(BasicMemberExpression)
				{
					BasicTypeRecord* structure=BasicLanguage_GetExpressionType(node->operand, argument);
					if(structure)
					{
						if(node->pointerMember)
						{
							if(structure->GetType()!=BasicTypeRecord::Pointer)
							{
								return 0;
							}
							structure=structure->ElementType();
						}
						if(structure->GetType()==BasicTypeRecord::Structure)
						{
							BasicTypeRecord* member=structure->MemberType(node->member);
							if(!member)
							{
								argument.errors.Add(BasicLanguageCodeException::GetStructureMemberNotExists(node));
							}
							return member;
						}
					}
					return 0;
				}

				ALGORITHM_FUNCTION_MATCH(BasicInvokeExpression)
				{
					BasicTypeRecord* function=BasicLanguage_GetExpressionType(node->function, argument);
					if(function==0)
					{
						return 0;
					}
					if(function->GetType()!=BasicTypeRecord::Function)
					{
						argument.errors.Add(BasicLanguageCodeException::GetCannotInvokeNonFunctionValue(node));
						return 0;
					}
					if(function->ParameterCount()!=node->arguments.Count())
					{
						argument.errors.Add(BasicLanguageCodeException::GetArgumentNumberNotMatch(node));
						return function->ReturnType();
					}
					for(int i=0;i<node->arguments.Count();i++)
					{
						BasicTypeRecord* argumentType=BasicLanguage_GetExpressionType(node->arguments[i], argument);
						if(argumentType!=0)
						{
							if(!CanImplicitConvertTo(argumentType, function->ParameterType(i), argument))
							{
								argument.errors.Add(BasicLanguageCodeException::GetArgumentTypeNotMatch(node, i));
							}
						}
					}
					return function->ReturnType();
				}

				ALGORITHM_FUNCTION_MATCH(BasicFunctionResultExpression)
				{
					BasicTypeRecord* returnType=argument.env->GetFunctionType(argument.scope->OwnerDeclaration())->ReturnType();
					if(returnType->GetType()==BasicTypeRecord::Primitive && returnType->PrimitiveType()==void_type)
					{
						argument.errors.Add(BasicLanguageCodeException::GetVoidFunctionNotHaveResult(node));
						return 0;
					}
					else
					{
						return returnType;
					}
				}

				ALGORITHM_FUNCTION_MATCH(BasicCastingExpression)
				{
					try
					{
						BasicTypeRecord* dstType=BasicLanguage_GetTypeRecord(node->type, argument);
						BasicTypeRecord* srcType=BasicLanguage_GetExpressionType(node->operand, argument);
						if(dstType && srcType)
						{
							BasicPrimitiveTypeEnum dstPrimitive=dstType->GetType()==BasicTypeRecord::Primitive?dstType->PrimitiveType():void_type;
							BasicPrimitiveTypeEnum srcPrimitive=srcType->GetType()==BasicTypeRecord::Primitive?srcType->PrimitiveType():void_type;
							if(dstType->GetType()==BasicTypeRecord::Primitive)
							{
								if(srcType->GetType()==BasicTypeRecord::Primitive)
								{
									if(argument.configuration.CanExplicitConvertTo(srcPrimitive, dstPrimitive))
									{
										return dstType;
									}
								}
								else if(srcType->GetType()==BasicTypeRecord::Pointer)
								{
									if(dstPrimitive==int_type || dstPrimitive==uint_type)
									{
										return dstType;
									}
								}
							}
							else if(dstType->GetType()==BasicTypeRecord::Pointer)
							{
								if(srcType->GetType()==BasicTypeRecord::Primitive)
								{
									if(srcPrimitive==int_type || srcPrimitive==uint_type)
									{
										return dstType;
									}
								}
								else if(srcType->GetType()==BasicTypeRecord::Pointer)
								{
									return dstType;
								}
							}
						}
						argument.errors.Add(BasicLanguageCodeException::GetFailToCast(node));
						return 0;
					}
					catch(Ptr<BasicLanguageCodeException> e)
					{
						argument.errors.Add(e);
						return 0;
					}
				}

				ALGORITHM_FUNCTION_MATCH(BasicReferenceExpression)
				{
					BasicTypeRecord* type=argument.scope->variables.Find(node->name);
					if(!type)
					{
						argument.errors.Add(BasicLanguageCodeException::GetVariableNotExists(node));
					}
					return type;
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