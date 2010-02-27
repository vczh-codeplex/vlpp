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

			Ptr<BasicExpression> BasicSemanticExtension::ReplaceExpression(Ptr<BasicExpression> originalExpression, const BP& argument)
			{
				return originalExpression;
			}

			Ptr<BasicStatement> BasicSemanticExtension::ReplaceStatement(Ptr<BasicStatement> originalStatement, const BP& argument)
			{
				return originalStatement;
			}

			BasicTypeRecord* BasicSemanticExtension::GetTypeRecord(BasicExtendedType* type, const BP& argument)
			{
				CHECK_ERROR(false, L"BasicSemanticExtension::GetTypeRecord(BasicExtendedType*, const BP&)#不支持此操作。");
			}

			void BasicSemanticExtension::BuildGlobalScopePass1(BasicExtendedDeclaration* declaration, const BP& argument)
			{
				CHECK_ERROR(false, L"BasicSemanticExtension::BuildGlobalScopePass1(BasicExtendedDeclaration*, const BP&)#不支持此操作。");
			}

			void BasicSemanticExtension::BuildGlobalScopePass2(BasicExtendedDeclaration* declaration, const BP& argument)
			{
				CHECK_ERROR(false, L"BasicSemanticExtension::BuildGlobalScopePass2(BasicExtendedDeclaration*, const BP&)#不支持此操作。");
			}

			bool BasicSemanticExtension::IsLeftValue(BasicExtendedExpression* expression, const BP& argument)
			{
				CHECK_ERROR(false, L"BasicSemanticExtension::IsLeftValue(BasicExtendedExpression*, const BP&)#不支持此操作。");
			}

			BasicTypeRecord* BasicSemanticExtension::GetExpressionType(BasicExtendedExpression* expression, const BP& argument)
			{
				CHECK_ERROR(false, L"BasicSemanticExtension::GetExpressionType(BasicExtendedExpression*, const BP&)#不支持此操作。");
			}

			void BasicSemanticExtension::CheckStatement(BasicExtendedStatement* statement, const BP& argument)
			{
				CHECK_ERROR(false, L"BasicSemanticExtension::CheckStatement(BasicExtendedStatement*, const BP&)#不支持此操作。");
			}

			void BasicSemanticExtension::BuildDeclarationBody(BasicExtendedDeclaration* declaration, const BP& argument)
			{
				CHECK_ERROR(false, L"BasicSemanticExtension::BuildDeclarationBody(BasicExtendedDeclaration*, const BP&)#不支持此操作。");
			}

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
				,configuration(bp.configuration)
			{
			}

			BasicAlgorithmParameter::BasicAlgorithmParameter(const BasicAlgorithmParameter& bp)
				:env(bp.env)
				,scope(bp.scope)
				,typeManager(bp.typeManager)
				,errors(bp.errors)
				,forwardStructures(bp.forwardStructures)
				,semanticExtension(bp.semanticExtension)
				,configuration(bp.configuration)
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
						BasicTypeRecord* structure=0;
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
								structure=dynamic_cast<BasicStructureTypeRecord*>(argument.scope->types.Items()[node->name]);
							}
						}
						else
						{
							structure=argument.typeManager->CreateStructureType();
							argument.scope->types.Add(node->name, structure);
						}

						if(structure)
						{
							List<WString> names;
							List<BasicTypeRecord*> types;
							for(int i=0;i<node->memberNames.Count();i++)
							{
								if(node->memberNames.IndexOf(node->memberNames[i])==i)
								{
									try
									{
										BasicTypeRecord* type=BasicLanguage_GetTypeRecord(node->memberTypes[i], argument);
										if(type->GetType()==BasicTypeRecord::Structure)
										{
											if(!type->Defined())
											{
												argument.errors.Add(BasicLanguageCodeException::GetStructureMemberCannotBeUndefinedType(node, i));
											}
										}
										types.Add(type);
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
							argument.scope->variables.Add(node->name, BasicScope::Variable(node, BasicLanguage_GetTypeRecord(node->type, argument)));
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
					if(!node->defined)
					{
						BasicStructureTypeRecord* structure=dynamic_cast<BasicStructureTypeRecord*>(argument.scope->types.Items()[node->name]);
						if(!structure->Defined())
						{
							argument.errors.Add(BasicLanguageCodeException::GetPredeclaredStructureShouldBeDefined(node));
						}
					}
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
					case BasicBinaryExpression::BitAndAssign:
					case BasicBinaryExpression::OrAssign:
					case BasicBinaryExpression::BitOrAssign:
					case BasicBinaryExpression::XorAssign:
					case BasicBinaryExpression::Assign:
						return true;
					default:
						return false;
					}
				}

				ALGORITHM_FUNCTION_MATCH(BasicSubscribeExpression)
				{
					BasicTypeRecord* operandType=argument.env->GetExpressionType(node->operand.Obj());
					return (operandType && operandType->GetType()==BasicTypeRecord::Pointer) || BasicLanguage_IsLeftValue(node->operand, argument);
				}

				ALGORITHM_FUNCTION_MATCH(BasicMemberExpression)
				{
					return node->pointerMember || BasicLanguage_IsLeftValue(node->operand, argument);
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
					return argument.env->GetReference(node).isVariable;
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
					expression=argument.semanticExtension->ReplaceExpression(expression, argument);
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
					return to->ElementType()==argument.typeManager->GetPrimitiveType(void_type);
				}
				else if(from->GetType()==BasicTypeRecord::Function && to->GetType()==BasicTypeRecord::Pointer)
				{
					return to->ElementType()==argument.typeManager->GetPrimitiveType(void_type);
				}
				else
				{
					return false;
				}
			}

			BasicTypeRecord* AdjustBinaryResult(bool isAssignment, BasicPrimitiveTypeEnum result, BasicPrimitiveTypeEnum leftPrimitive, BasicBinaryExpression* node, const BP& argument)
			{
				if(isAssignment)
				{
					if(!argument.configuration.CanImplicitConvertTo(result, leftPrimitive))
					{
						argument.errors.Add(BasicLanguageCodeException::GetBinaryTypeNotMatch(node));
					}
					return argument.typeManager->GetPrimitiveType(leftPrimitive);
				}
				else
				{
					return argument.typeManager->GetPrimitiveType(result);
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
					{
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
							if(operandType->GetType()==BasicTypeRecord::Pointer)
							{
								return operandType;
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
					}
					return 0;
				}

				ALGORITHM_FUNCTION_MATCH(BasicBinaryExpression)
				{
					BasicTypeRecord* leftType=BasicLanguage_GetExpressionType(node->leftOperand, argument);
					BasicTypeRecord* rightType=BasicLanguage_GetExpressionType(node->rightOperand, argument);
					if(leftType && rightType)
					{
						bool isAssignment=false;
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
						case BasicBinaryExpression::BitAndAssign:
						case BasicBinaryExpression::BitOrAssign:
						case BasicBinaryExpression::Assign:
							isAssignment=true;
							if(!BasicLanguage_IsLeftValue(node->leftOperand, argument))
							{
								argument.errors.Add(BasicLanguageCodeException::GetBinaryLeftOperandShouldBeLeftValue(node));
							}
						}
						switch(node->type)
						{
						case BasicBinaryExpression::AddAssign:
						case BasicBinaryExpression::Add:
							if(leftType->GetType()==BasicTypeRecord::Primitive && rightType->GetType()==BasicTypeRecord::Primitive)
							{
								BasicPrimitiveTypeEnum leftPrimitive=leftType->PrimitiveType();
								BasicPrimitiveTypeEnum rightPrimitive=rightType->PrimitiveType();
								BasicPrimitiveTypeEnum result=void_type;
								if(argument.configuration.NumberBinaryOperatorTypeConversion(leftPrimitive, rightPrimitive, result))
								{
									return AdjustBinaryResult(isAssignment, result, leftPrimitive, node, argument);
								}
							}
							else if(leftType->GetType()==BasicTypeRecord::Pointer && rightType->GetType()==BasicTypeRecord::Primitive)
							{
								if(leftType->ElementType()!=argument.typeManager->GetPrimitiveType(void_type))
								{
									if(argument.configuration.CanImplicitConvertTo(rightType->PrimitiveType(), int_type))
									{
										return leftType;
									}
								}
							}
							else if(!isAssignment && leftType->GetType()==BasicTypeRecord::Primitive && rightType->GetType()==BasicTypeRecord::Pointer)
							{
								if(rightType->ElementType()!=argument.typeManager->GetPrimitiveType(void_type))
								{
									if(argument.configuration.CanImplicitConvertTo(leftType->PrimitiveType(), int_type))
									{
										return rightType;
									}
								}
							}
							break;
						case BasicBinaryExpression::SubAssign:
						case BasicBinaryExpression::Sub:
							if(leftType->GetType()==BasicTypeRecord::Primitive && rightType->GetType()==BasicTypeRecord::Primitive)
							{
								BasicPrimitiveTypeEnum leftPrimitive=leftType->PrimitiveType();
								BasicPrimitiveTypeEnum rightPrimitive=rightType->PrimitiveType();
								BasicPrimitiveTypeEnum result=void_type;
								if(argument.configuration.NumberBinaryOperatorTypeConversion(leftPrimitive, rightPrimitive, result))
								{
									return AdjustBinaryResult(isAssignment, result, leftPrimitive, node, argument);
								}
							}
							else if(leftType->GetType()==BasicTypeRecord::Pointer && rightType->GetType()==BasicTypeRecord::Primitive)
							{
								if(leftType->ElementType()!=argument.typeManager->GetPrimitiveType(void_type))
								{
									if(argument.configuration.CanImplicitConvertTo(rightType->PrimitiveType(), int_type))
									{
										return leftType;
									}
								}
							}
							else if(!isAssignment && leftType->GetType()==BasicTypeRecord::Pointer && rightType->GetType()==BasicTypeRecord::Pointer)
							{
								if(leftType->ElementType()==rightType->ElementType() && leftType->ElementType()!=argument.typeManager->GetPrimitiveType(void_type))
								{
									return argument.typeManager->GetPrimitiveType(int_type);
								}
							}
							break;
						case BasicBinaryExpression::MulAssign:
						case BasicBinaryExpression::DivAssign:
						case BasicBinaryExpression::Mul:
						case BasicBinaryExpression::Div:
							if(leftType->GetType()==BasicTypeRecord::Primitive && rightType->GetType()==BasicTypeRecord::Primitive)
							{
								BasicPrimitiveTypeEnum leftPrimitive=leftType->PrimitiveType();
								BasicPrimitiveTypeEnum rightPrimitive=rightType->PrimitiveType();
								BasicPrimitiveTypeEnum result=void_type;
								if(argument.configuration.NumberBinaryOperatorTypeConversion(leftPrimitive, rightPrimitive, result))
								{
									return AdjustBinaryResult(isAssignment, result, leftPrimitive, node, argument);
								}
							}
							break;
						case BasicBinaryExpression::ModAssign:
						case BasicBinaryExpression::ShlAssign:
						case BasicBinaryExpression::ShrAssign:
						case BasicBinaryExpression::BitAndAssign:
						case BasicBinaryExpression::BitOrAssign:
						case BasicBinaryExpression::Mod:
						case BasicBinaryExpression::Shl:
						case BasicBinaryExpression::Shr:
						case BasicBinaryExpression::BitAnd:
						case BasicBinaryExpression::BitOr:
							if(leftType->GetType()==BasicTypeRecord::Primitive && rightType->GetType()==BasicTypeRecord::Primitive)
							{
								BasicPrimitiveTypeEnum leftPrimitive=leftType->PrimitiveType();
								BasicPrimitiveTypeEnum rightPrimitive=rightType->PrimitiveType();
								BasicPrimitiveTypeEnum result=void_type;
								if(argument.configuration.IntegerBinaryOperatorTypeConversion(leftPrimitive, rightPrimitive, result))
								{
									return AdjustBinaryResult(isAssignment, result, leftPrimitive, node, argument);
								}
							}
							break;
						case BasicBinaryExpression::AndAssign:
						case BasicBinaryExpression::OrAssign:
						case BasicBinaryExpression::And:
						case BasicBinaryExpression::Or:
							if(leftType->GetType()==BasicTypeRecord::Primitive && rightType->GetType()==BasicTypeRecord::Primitive)
							{
								BasicPrimitiveTypeEnum leftPrimitive=leftType->PrimitiveType();
								BasicPrimitiveTypeEnum rightPrimitive=rightType->PrimitiveType();
								BasicPrimitiveTypeEnum result=void_type;
								if(argument.configuration.BooleanBinaryOperatorTypeConversion(leftPrimitive, rightPrimitive, result))
								{
									return AdjustBinaryResult(isAssignment, result, leftPrimitive, node, argument);
								}
							}
							break;
						case BasicBinaryExpression::XorAssign:
						case BasicBinaryExpression::Xor:
							if(leftType->GetType()==BasicTypeRecord::Primitive && rightType->GetType()==BasicTypeRecord::Primitive)
							{
								BasicPrimitiveTypeEnum leftPrimitive=leftType->PrimitiveType();
								BasicPrimitiveTypeEnum rightPrimitive=rightType->PrimitiveType();
								BasicPrimitiveTypeEnum result=void_type;
								bool failed=false;
								if(leftPrimitive==bool_type && rightPrimitive==bool_type)
								{
									result=bool_type;
								}
								else if(!argument.configuration.IntegerBinaryOperatorTypeConversion(leftPrimitive, rightPrimitive, result))
								{
									failed=true;
								}
								if(!failed)
								{
									return AdjustBinaryResult(isAssignment, result, leftPrimitive, node, argument);
								}
							}
							break;
						case BasicBinaryExpression::Lt:
						case BasicBinaryExpression::Le:
						case BasicBinaryExpression::Gt:
						case BasicBinaryExpression::Ge:
						case BasicBinaryExpression::Eq:
						case BasicBinaryExpression::Ne:
							if(leftType->GetType()==rightType->GetType())
							{
								switch(leftType->GetType())
								{
								case BasicTypeRecord::Primitive:
									{
										BasicPrimitiveTypeEnum result=void_type;
										if(!argument.configuration.NumberBinaryOperatorTypeConversion(leftType->PrimitiveType(), rightType->PrimitiveType(), result))
										{
											argument.errors.Add(BasicLanguageCodeException::GetBinaryTypeNotMatch(node));
										}
									}
									break;
								case BasicTypeRecord::Pointer:
									if(leftType->ElementType()!=rightType->ElementType() &&
										leftType->ElementType()!=argument.typeManager->GetPrimitiveType(void_type) &&
										rightType->ElementType()!=argument.typeManager->GetPrimitiveType(void_type))
									{
										argument.errors.Add(BasicLanguageCodeException::GetBinaryTypeNotMatch(node));
									}
									break;
								}
								return argument.typeManager->GetPrimitiveType(bool_type);
							}
							break;
						case BasicBinaryExpression::Assign:
							if(CanImplicitConvertTo(rightType, leftType, argument))
							{
								return leftType;
							}
							break;
						}
						argument.errors.Add(BasicLanguageCodeException::GetBinaryTypeNotMatch(node));
					}
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
								argument.errors.Add(BasicLanguageCodeException::GetLeftOperandShouldBePointerToStructure(node));
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
							else
							{
								return member;
							}
						}
					}
					if(node->pointerMember)
					{
						argument.errors.Add(BasicLanguageCodeException::GetLeftOperandShouldBePointerToStructure(node));
					}
					else
					{
						argument.errors.Add(BasicLanguageCodeException::GetLeftOperandShouldBeStructure(node));
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
					if(argument.scope->OwnerDeclaration()==0)
					{
						argument.errors.Add(BasicLanguageCodeException::GetGlobalNotHaveResult(node));
						return 0;
					}
					else
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
				}

				ALGORITHM_FUNCTION_MATCH(BasicCastingExpression)
				{
					try
					{
						BasicTypeRecord* dstType=BasicLanguage_GetTypeRecord(node->type, argument);
						BasicTypeRecord* srcType=BasicLanguage_GetExpressionType(node->operand, argument);
						if(dstType && srcType)
						{
							if(dstType==srcType)
							{
								return dstType;
							}
							else
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
								else if(dstType->GetType()==BasicTypeRecord::Pointer || dstType->GetType()==BasicTypeRecord::Function)
								{
									if(srcType->GetType()==BasicTypeRecord::Primitive)
									{
										if(srcPrimitive==int_type || srcPrimitive==uint_type)
										{
											return dstType;
										}
									}
									else if(srcType->GetType()==BasicTypeRecord::Pointer || srcType->GetType()==BasicTypeRecord::Function)
									{
										return dstType;
									}
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
					BasicScope* variableScope=0;
					BasicScope* functionScope=0;
					BasicScope::Variable variableType=argument.scope->variables.Find(node->name, variableScope);
					BasicFunctionDeclaration* functionDeclaration=argument.scope->functions.Find(node->name, functionScope);
					BasicTypeRecord* functionType=argument.env->GetFunctionType(functionDeclaration);
					if(variableType && functionType)
					{
						if(variableScope->Level()>functionScope->Level())
						{
							functionType=0;
						}
						else
						{
							variableType=BasicScope::Variable();
						}
					}
					if(variableType)
					{
						if(variableType.globalVariable)
						{
							argument.env->RegisterReference(node, BasicEnv::Reference(variableScope, variableType.globalVariable));
						}
						else if(variableType.localVariable)
						{
							argument.env->RegisterReference(node, BasicEnv::Reference(variableScope, variableType.localVariable));
						}
						else
						{
							argument.env->RegisterReference(node, BasicEnv::Reference(variableScope, variableType.parameterIndex));
						}
						return variableType.type;
					}
					else if(functionType)
					{
						argument.env->RegisterReference(node, BasicEnv::Reference(functionScope, functionDeclaration));
						return functionType;
					}
					else
					{
						argument.errors.Add(BasicLanguageCodeException::GetVariableNotExists(node));
						return 0;
					}
				}

				ALGORITHM_FUNCTION_MATCH(BasicExtendedExpression)
				{
					return argument.semanticExtension->GetExpressionType(node, argument);
				}

			END_ALGORITHM_FUNCTION(BasicLanguage_GetExpressionTypeInternal)

/***********************************************************************
BasicLanguage_GetExpressionType
***********************************************************************/

			void BasicLanguage_CheckStatement(Ptr<BasicStatement>& statement, const BP& argument)
			{
				statement=argument.semanticExtension->ReplaceStatement(statement, argument);
				try
				{
					BasicLanguage_CheckStatementInternal(statement, argument);
					argument.env->RegisterStatementScope(statement.Obj(), argument.scope);
				}
				catch(Ptr<BasicLanguageCodeException> e)
				{
					argument.errors.Add(e);
				}
			}

			BEGIN_ALGORITHM_PROCEDURE(BasicLanguage_CheckStatementInternal, BasicStatement, BP)

				ALGORITHM_PROCEDURE_MATCH(BasicEmptyStatement)
				{
				}

				ALGORITHM_PROCEDURE_MATCH(BasicCompositeStatement)
				{
					BP newArgument(argument, argument.env->CreateStatementScope(argument.scope, node));
					for(int i=0;i<node->statements.Count();i++)
					{
						BasicLanguage_CheckStatement(node->statements[i], newArgument);
					}
				}

				ALGORITHM_PROCEDURE_MATCH(BasicExpressionStatement)
				{
					BasicLanguage_GetExpressionType(node->expression, argument);
				}

				ALGORITHM_PROCEDURE_MATCH(BasicVariableStatement)
				{
					bool wrong=false;
					BasicTypeRecord* type=0;

					if(argument.scope->variables.Items().Keys().Contains(node->name))
					{
						argument.errors.Add(BasicLanguageCodeException::GetVariableAlreadyExists(node));
						wrong=true;
					}

					try
					{
						type=BasicLanguage_GetTypeRecord(node->type, argument);
					}
					catch(Ptr<BasicLanguageCodeException> e)
					{
						argument.errors.Add(e);
						wrong=true;
					}

					if(node->initializer)
					{
						BasicTypeRecord* initializerType=BasicLanguage_GetExpressionType(node->initializer, argument);
						if(type && initializerType)
						{
							if(!CanImplicitConvertTo(initializerType, type, argument))
							{
								argument.errors.Add(BasicLanguageCodeException::GetInitializerTypeNotMatch(node));
							}
						}
					}

					if(!wrong)
					{
						argument.scope->variables.Add(node->name, BasicScope::Variable(node, type));
					}
				}

				ALGORITHM_PROCEDURE_MATCH(BasicIfStatement)
				{
					BasicTypeRecord* conditionType=BasicLanguage_GetExpressionType(node->condition, argument);
					if(conditionType && !CanImplicitConvertTo(conditionType, argument.typeManager->GetPrimitiveType(bool_type), argument))
					{
						argument.errors.Add(BasicLanguageCodeException::GetConditionCannotConvertToBool(node->condition.Obj()));
					}
					BasicLanguage_CheckStatement(node->trueStatement, argument);
					if(node->falseStatement)
					{
						BasicLanguage_CheckStatement(node->falseStatement, argument);
					}
				}

				ALGORITHM_PROCEDURE_MATCH(BasicWhileStatement)
				{
					if(node->beginCondition)
					{
						BasicTypeRecord* beginConditionType=BasicLanguage_GetExpressionType(node->beginCondition, argument);
						if(beginConditionType && !CanImplicitConvertTo(beginConditionType, argument.typeManager->GetPrimitiveType(bool_type), argument))
						{
							argument.errors.Add(BasicLanguageCodeException::GetConditionCannotConvertToBool(node->beginCondition.Obj()));
						}
					}
					if(node->endCondition)
					{
						BasicTypeRecord* endConditionType=BasicLanguage_GetExpressionType(node->endCondition, argument);
						if(endConditionType && !CanImplicitConvertTo(endConditionType, argument.typeManager->GetPrimitiveType(bool_type), argument))
						{
							argument.errors.Add(BasicLanguageCodeException::GetConditionCannotConvertToBool(node->endCondition.Obj()));
						}
					}
					BP newArgument(argument, argument.env->CreateStatementScope(argument.scope, node));
					BasicLanguage_CheckStatement(node->statement, newArgument);
				}

				ALGORITHM_PROCEDURE_MATCH(BasicForStatement)
				{
					BP newArgument(argument, argument.env->CreateStatementScope(argument.scope, node));
					if(node->initializer)
					{
						BasicLanguage_CheckStatement(node->initializer, newArgument);
					}
					BasicTypeRecord* conditionType=BasicLanguage_GetExpressionType(node->condition, newArgument);
					if(conditionType && !CanImplicitConvertTo(conditionType, argument.typeManager->GetPrimitiveType(bool_type), argument))
					{
						argument.errors.Add(BasicLanguageCodeException::GetConditionCannotConvertToBool(node->condition.Obj()));
					}
					BasicLanguage_CheckStatement(node->statement, newArgument);
					if(node->sideEffect)
					{
						BasicLanguage_CheckStatement(node->sideEffect, newArgument);
					}
				}

				ALGORITHM_PROCEDURE_MATCH(BasicBreakStatement)
				{
					BasicScope* scope=argument.scope;
					while(scope->OwnerStatement())
					{
						if(dynamic_cast<BasicWhileStatement*>(scope->OwnerStatement()) || dynamic_cast<BasicForStatement*>(scope->OwnerStatement()))
						{
							return;
						}
						scope=scope->PreviousScope();
					}
					argument.errors.Add(BasicLanguageCodeException::GetBreakShouldBeInLooping(node));
				}

				ALGORITHM_PROCEDURE_MATCH(BasicContinueStatement)
				{
					BasicScope* scope=argument.scope;
					while(scope->OwnerStatement())
					{
						if(dynamic_cast<BasicWhileStatement*>(scope->OwnerStatement()) || dynamic_cast<BasicForStatement*>(scope->OwnerStatement()))
						{
							return;
						}
						scope=scope->PreviousScope();
					}
					argument.errors.Add(BasicLanguageCodeException::GetContinueShouldBeInLooping(node));
				}

				ALGORITHM_PROCEDURE_MATCH(BasicReturnStatement)
				{
				}

				ALGORITHM_PROCEDURE_MATCH(BasicExtendedStatement)
				{
					argument.semanticExtension->CheckStatement(node, argument);
				}

			END_ALGORITHM_PROCEDURE(BasicLanguage_CheckStatementInternal)

/***********************************************************************
BasicLanguage_BuildDeclarationBody
***********************************************************************/

			BEGIN_ALGORITHM_PROCEDURE(BasicLanguage_BuildDeclarationBody, BasicDeclaration, BP)

				ALGORITHM_PROCEDURE_MATCH(BasicFunctionDeclaration)
				{
					BasicTypeRecord* functionType=argument.env->GetFunctionType(node);
					if(functionType->ParameterCount()==node->parameterNames.Count())
					{
						if(node->statement)
						{
							BasicScope* functionScope=argument.env->CreateFunctionScope(argument.scope, node);
							for(int i=0;i<node->parameterNames.Count();i++)
							{
								if(functionScope->variables.Items().Keys().Contains(node->parameterNames[i]))
								{
									argument.errors.Add(BasicLanguageCodeException::GetParameterAlreadyExists(node, i));
								}
								else
								{
									functionScope->variables.Add(node->parameterNames[i], BasicScope::Variable(i, functionType->ParameterType(i)));
								}
							}
							BP newArgument(argument, functionScope);
							BasicLanguage_CheckStatement(node->statement, newArgument);
						}
					}
					else
					{
						argument.errors.Add(BasicLanguageCodeException::GetParameterCountNotMatch(node));
					}
				}

				ALGORITHM_PROCEDURE_MATCH(BasicVariableDeclaration)
				{
					if(node->initializer)
					{
						BasicTypeRecord* initializerType=BasicLanguage_GetExpressionType(node->initializer, argument);
						try
						{
							BasicTypeRecord* variableType=BasicLanguage_GetTypeRecord(node->type, argument);
							if(initializerType && variableType)
							{
								if(!CanImplicitConvertTo(initializerType, variableType, argument))
								{
									argument.errors.Add(BasicLanguageCodeException::GetInitializerTypeNotMatch(node));
								}
							}
						}
						catch(Ptr<BasicLanguageCodeException> e)
						{
							argument.errors.Add(e);
						}
					}
				}

				ALGORITHM_PROCEDURE_MATCH(BasicTypeRenameDeclaration)
				{
				}

				ALGORITHM_PROCEDURE_MATCH(BasicStructureDeclaration)
				{
				}

				ALGORITHM_PROCEDURE_MATCH(BasicExtendedDeclaration)
				{
					argument.semanticExtension->BuildDeclarationBody(node, argument);
				}

			END_ALGORITHM_PROCEDURE(BasicLanguage_BuildDeclarationBody)

			void BasicLanguage_BuildDeclarationBody(Ptr<BasicProgram> program, BP& argument)
			{
				for(int i=0;i<program->declarations.Count();i++)
				{
					BasicLanguage_BuildDeclarationBody(program->declarations[i], argument);
				}
			}

/***********************************************************************
BasicAnalyzer
***********************************************************************/

			BasicAnalyzer::BasicAnalyzer(Ptr<BasicProgram> _program, BasicSemanticExtension* _semanticExtension, BasicAlgorithmConfiguration _configuration)
				:program(_program)
				,semanticExtension(_semanticExtension)
				,configuration(_configuration)
				,analyzed(false)
			{
			}

			BasicAnalyzer::~BasicAnalyzer()
			{
			}

			BasicEnv* BasicAnalyzer::GetEnv()
			{
				return &env;
			}

			BasicTypeManager* BasicAnalyzer::GetTypeManager()
			{
				return &tm;
			}

			const collections::IReadonlyList<Ptr<BasicLanguageCodeException>>& BasicAnalyzer::GetErrors()
			{
				return errors.Wrap();
			}

			BasicAlgorithmConfiguration& BasicAnalyzer::GetConfiguration()
			{
				return configuration;
			}

			void BasicAnalyzer::Analyze()
			{
				if(!analyzed)
				{
					analyzed=true;
					BP argument(&env, env.GlobalScope(), &tm, errors, forwardStructures);
					argument.configuration=configuration;
					if(semanticExtension)
					{
						argument.semanticExtension=semanticExtension;
					}

					BasicLanguage_BuildGlobalScope(program, argument);
					if(errors.Count()==0)
					{
						BasicLanguage_BuildDeclarationBody(program, argument);
					}
				}
			}

			Ptr<BasicProgram> BasicAnalyzer::GetProgram()
			{
				return program;
			}
		}
	}
}