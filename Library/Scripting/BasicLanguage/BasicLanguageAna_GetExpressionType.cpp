#include "BasicLanguageAnalyzer.h"
#include "..\..\Collections\OperationCopyFrom.h"

namespace vl
{
	namespace scripting
	{
		namespace basiclanguage
		{
			using namespace collections;

/***********************************************************************
BasicLanguage_GetExpressionType
***********************************************************************/

			BasicTypeRecord* BasicLanguage_GetExpressionType(Ptr<BasicExpression> expression, const BP& argument)
			{
				BasicTypeRecord* type=argument.env->GetExpressionType(expression.Obj());
				if(!type)
				{
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

			bool CanImplicitConvertTo(BasicTypeRecord* from, BasicTypeRecord* to, BasicExpression* fromExpression, const BP& argument)
			{
				bool fromNull=dynamic_cast<BasicNullExpression*>(fromExpression)!=0;
				BasicNumericExpression* implicitInteger=dynamic_cast<BasicNumericExpression*>(fromExpression);
				if(implicitInteger && !implicitInteger->implicitIntegerType)
				{
					implicitInteger=0;
				}

				if(from==to)
				{
					return true;
				}
				if(implicitInteger && to->GetType()==BasicTypeRecord::Primitive)
				{
					switch(to->PrimitiveType())
					{
					case s8:
						return implicitInteger->argument.u64<((unsigned __int64)1<<7);
					case s16:
						return implicitInteger->argument.u64<((unsigned __int64)1<<15);
					case s32:
						return implicitInteger->argument.u64<((unsigned __int64)1<<31);
					case s64:
						return implicitInteger->argument.u64<((unsigned __int64)1<<63);
					case u8:
						return implicitInteger->argument.u64<((unsigned __int64)1<<8);
					case u16:
						return implicitInteger->argument.u64<((unsigned __int64)1<<16);
					case u32:
						return implicitInteger->argument.u64<((unsigned __int64)1<<32);
					case u64:
					case f32:
					case f64:
						return true;
					}
				}

				if(from->GetType()==BasicTypeRecord::Primitive && to->GetType()==BasicTypeRecord::Primitive)
				{
					return argument.configuration.CanImplicitConvertTo(from->PrimitiveType(), to->PrimitiveType());
				}
				else if(to->GetType()==BasicTypeRecord::Pointer)
				{
					if(fromNull)
					{
						return true;
					}
					switch(from->GetType())
					{
					case BasicTypeRecord::Pointer:
					case BasicTypeRecord::Function:
						return to->ElementType()==argument.typeManager->GetPrimitiveType(void_type);
					default:
						return false;
					}
				}
				else if(to->GetType()==BasicTypeRecord::Function)
				{
					return fromNull;
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

				ALGORITHM_FUNCTION_MATCH(BasicExceptionAddressExpression)
				{
					return argument.typeManager->GetPointerType(argument.typeManager->GetPrimitiveType(void_type));
				}

				ALGORITHM_FUNCTION_MATCH(BasicStackDataAddressExpression)
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
										dynamic_cast<BasicNullExpression*>(node->leftOperand.Obj())==0 &&
										dynamic_cast<BasicNullExpression*>(node->rightOperand.Obj())==0)
									{
										argument.errors.Add(BasicLanguageCodeException::GetBinaryTypeNotMatch(node));
									}
									break;
								}
								return argument.typeManager->GetPrimitiveType(bool_type);
							}
							if(leftType->GetType()==BasicTypeRecord::Pointer || leftType->GetType()==BasicTypeRecord::Function)
							{
								if(dynamic_cast<BasicNullExpression*>(node->rightOperand.Obj())==0)
								{
									argument.errors.Add(BasicLanguageCodeException::GetBinaryTypeNotMatch(node));
								}
								return argument.typeManager->GetPrimitiveType(bool_type);
							}
							if(rightType->GetType()==BasicTypeRecord::Pointer || rightType->GetType()==BasicTypeRecord::Function)
							{
								if(dynamic_cast<BasicNullExpression*>(node->leftOperand.Obj())==0)
								{
									argument.errors.Add(BasicLanguageCodeException::GetBinaryTypeNotMatch(node));
								}
								return argument.typeManager->GetPrimitiveType(bool_type);
							}
							break;
						case BasicBinaryExpression::Assign:
							if(CanImplicitConvertTo(rightType, leftType, node->rightOperand.Obj(), argument))
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
					for(vint i=0;i<node->arguments.Count();i++)
					{
						BasicTypeRecord* argumentType=BasicLanguage_GetExpressionType(node->arguments[i], argument);
						if(argumentType!=0)
						{
							if(!CanImplicitConvertTo(argumentType, function->ParameterType(i), node->arguments[i].Obj(), argument))
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
						BasicTypeRecord* returnType=argument.env->GetFunctionType(argument.scope->OwnerDeclaration(), true)->ReturnType();
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

				ALGORITHM_FUNCTION_MATCH(BasicSizeofExpression)
				{
					BasicLanguage_GetTypeRecord(node->type, argument, false);
					return argument.typeManager->GetPrimitiveType(int_type);
				}

				ALGORITHM_FUNCTION_MATCH(BasicOffsetofExpression)
				{
					BasicTypeRecord* structure=BasicLanguage_GetTypeRecord(node->type, argument, false);
					if(structure && structure->GetType()==BasicTypeRecord::Structure)
					{
						BasicTypeRecord* member=structure->MemberType(node->member);
						if(!member)
						{
							argument.errors.Add(BasicLanguageCodeException::GetStructureMemberNotExists(node));
						}
					}
					else
					{
						argument.errors.Add(BasicLanguageCodeException::GetLeftOperandShouldBeStructure(node));
					}
					return argument.typeManager->GetPrimitiveType(int_type);
				}

				ALGORITHM_FUNCTION_MATCH(BasicCastingExpression)
				{
					try
					{
						BasicTypeRecord* dstType=BasicLanguage_GetTypeRecord(node->type, argument, false);
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
							argument.errors.Add(BasicLanguageCodeException::GetFailToCast(node));
						}
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
					BasicTypeRecord* functionType=argument.env->GetFunctionType(functionDeclaration, false);
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
						if(variableType.type->GetType()==BasicTypeRecord::Generic)
						{
							argument.errors.Add(BasicLanguageCodeException::GetCannotUseUninstantiatedGenericVariable(node));
						}
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
						if(functionType->GetType()==BasicTypeRecord::Generic)
						{
							argument.errors.Add(BasicLanguageCodeException::GetCannotUseUninstantiatedGenericFunction(node));
						}
						argument.env->RegisterReference(node, BasicEnv::Reference(functionScope, functionDeclaration));
						return functionType;
					}
					else
					{
						argument.errors.Add(BasicLanguageCodeException::GetVariableNotExists(node));
						return 0;
					}
				}

				ALGORITHM_FUNCTION_MATCH(BasicInstantiatedExpression)
				{
					BasicScope* variableScope=0;
					BasicScope* functionScope=0;
					BasicScope::Variable variableType=argument.scope->variables.Find(node->reference->name, variableScope);
					BasicFunctionDeclaration* functionDeclaration=argument.scope->functions.Find(node->reference->name, functionScope);
					BasicTypeRecord* genericType=
						variableScope
						?variableType.type
						:argument.env->GetFunctionType(functionDeclaration, false);

					if(!variableType.globalVariable && !functionDeclaration)
					{
						throw BasicLanguageCodeException::GetGenericArgumentCannotApplyToNonGenericType(node);
					}
					else if(genericType->GetType()!=BasicTypeRecord::Generic)
					{
						throw BasicLanguageCodeException::GetGenericArgumentCannotApplyToNonGenericType(node);
					}
					
					if(node->argumentTypes.Count()!=genericType->ParameterCount())
					{
						throw BasicLanguageCodeException::GetGenericArgumentNumberNotMatch(node);
					}
					Dictionary<BasicTypeRecord*, BasicTypeRecord*> argumentTypes;
					for(vint i=0;i<genericType->ParameterCount();i++)
					{
						argumentTypes.Add(genericType->ParameterType(i), BasicLanguage_GetTypeRecord(node->argumentTypes[i], argument, false));
					}

					BasicDeclaration* genericDeclaration=0;
					if(variableType.globalVariable)
					{
						genericDeclaration=variableType.globalVariable;
						argument.env->RegisterReference(node->reference.Obj(), BasicEnv::Reference(variableScope, variableType.globalVariable));
					}
					else if(functionDeclaration)
					{
						genericDeclaration=functionDeclaration;
						argument.env->RegisterReference(node->reference.Obj(), BasicEnv::Reference(functionScope, functionDeclaration));
					}
					if(genericDeclaration)
					{
						for(vint i=0;i<genericDeclaration->genericDeclaration.constraints.Count();i++)
						{
							BasicGeneric::Constraint* constraint=genericDeclaration->genericDeclaration.constraints[i].Obj();
							vint argumentIndex=genericDeclaration->genericDeclaration.arguments.IndexOf(constraint->argumentName);
							BasicTypeRecord* argumentType=argumentTypes[genericType->ParameterType(argumentIndex)];
							CheckRequiredInstance(argumentType, node->argumentTypes[argumentIndex].Obj(), constraint->conceptName, argument);
						}
					}
					return argument.typeManager->Instanciate(genericType, argumentTypes.Wrap());
				}

				ALGORITHM_FUNCTION_MATCH(BasicInstanceFunctionExpression)
				{
					BasicTypeRecord* type=BasicLanguage_GetTypeRecord(node->type, argument, false);
					if(!type)
					{
						return 0;
					}
					CheckRequiredInstance(type, node->type.Obj(), node->conceptName, argument);
					Ptr<BasicScope::Concept> conceptObject=argument.scope->concepts.Find(node->conceptName);
					vint functionIndex=conceptObject->functions.Keys().IndexOf(node->functionName);
					if(functionIndex==-1)
					{
						argument.errors.Add(BasicLanguageCodeException::GetConceptFunctionNotExists(node));
						return 0;
					}

					Dictionary<BasicTypeRecord*, BasicTypeRecord*> typeMap;
					typeMap.Add(conceptObject->conceptType, type);
					return argument.typeManager->Instanciate(conceptObject->functions.Values()[functionIndex], typeMap.Wrap());
				}

			END_ALGORITHM_FUNCTION(BasicLanguage_GetExpressionTypeInternal)
		}
	}
}