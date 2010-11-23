#include "BasicLanguageCodeGeneration.h"

namespace vl
{
	namespace scripting
	{
		namespace basiclanguage
		{
			using namespace basicil;
			using namespace stream;
			using namespace collections;

/***********************************************************************
辅助函数
***********************************************************************/

			BasicIns::ValueType Convert(BasicPrimitiveTypeEnum type)
			{
				switch(type)
				{
				case s8:
					return BasicIns::s8;
				case s16:
					return BasicIns::s16;
				case s32:
					return BasicIns::s32;
				case s64:
					return BasicIns::s64;
				case u8:
					return BasicIns::u8;
				case u16:
					return BasicIns::u16;
				case u32:
					return BasicIns::u32;
				case u64:
					return BasicIns::u64;
				case f32:
					return BasicIns::f32;
				case f64:
					return BasicIns::f64;
				case bool_type:
					return BasicIns::bool_type;
				case char_type:
					return BasicIns::char_type;
				case wchar_type:
					return BasicIns::wchar_type;
				default:
					return BasicIns::int_type;
				}
			}

			BasicIns::ValueType Convert(BasicTypeRecord* type)
			{
				switch(type->GetType())
				{
				case BasicTypeRecord::Primitive:
					if(type->PrimitiveType()!=void_type)
					{
						return Convert(type->PrimitiveType());
					}
					break;
				case BasicTypeRecord::Pointer:
				case BasicTypeRecord::Function:
					return BasicIns::pointer_type;
				}
				CHECK_FAIL(L"Convert(BasicTypeRecord*)#不支持此操作。");
			}

			BasicIns::Argument Convert(BasicPrimitiveValueEnum value)
			{
				BasicIns::Argument argument;
				argument.s64=value.s64;
				return argument;
			}

			bool IsExternalFunction(BasicReferenceExpression* referenceExpression, const BCP& argument)
			{
				if(referenceExpression)
				{
					BasicEnv::Reference reference=argument.info->GetEnv()->GetReference(referenceExpression);
					if(!reference.isVariable)
					{
						BasicFunctionDeclaration* function=reference.scope->functions.Items()[referenceExpression->name];
						return function->linking.HasLink() && !function->foreignFunction;
					}
				}
				return false;
			}

			bool IsForeignFunction(BasicReferenceExpression* referenceExpression, const BCP& argument)
			{
				if(referenceExpression)
				{
					BasicEnv::Reference reference=argument.info->GetEnv()->GetReference(referenceExpression);
					if(!reference.isVariable)
					{
						BasicFunctionDeclaration* function=reference.scope->functions.Items()[referenceExpression->name];
						return function->linking.HasLink() && function->foreignFunction;
					}
				}
				return false;
			}

			vint GetFunctionIndex(BasicReferenceExpression* referenceExpression, const BCP& argument)
			{
				if(referenceExpression)
				{
					BasicEnv::Reference reference=argument.info->GetEnv()->GetReference(referenceExpression);
					if(!reference.isVariable)
					{
						BasicFunctionDeclaration* function=reference.scope->functions.Items()[referenceExpression->name];
						return argument.info->GetFunctions()[function];
					}
				}
				return -1;
			}

			vint GetGenericTargetIndex(BasicInstanciatedExpression* node, const BCP& argument, BasicTypeRecord* nodeType, BasicDeclaration* declaration)
			{
				BP bp(
					argument.info->GetEnv(),
					argument.info->GetSemanticScope(),
					argument.info->GetTypeManager(),
					*(List<Ptr<BasicLanguageCodeException>>*)0,
					*(SortedList<WString>*)0
					);

				Ptr<BasicCodegenInfo::GenericTarget> target=new BasicCodegenInfo::GenericTarget;
				target->ownerFunctionDeclaration=0;
				target->targetDeclaration=declaration;
				for(vint i=0;i<node->argumentTypes.Count();i++)
				{
					target->genericParameters.Add(BasicLanguage_GetTypeRecord(node->argumentTypes[i], bp, false));
				}
				return argument.info->RegisterGenericTarget(target);
			}

			vint GetGenericVariableTargetIndex(BasicInstanciatedExpression* node, const BCP& argument, BasicTypeRecord*& resultType)
			{
				BasicTypeRecord* nodeType=argument.info->GetEnv()->GetExpressionType(node);
				BasicEnv::Reference reference=argument.info->GetEnv()->GetReference(node->reference.Obj());
				if(reference.isVariable && reference.globalVariable)
				{
					resultType=nodeType;
					return GetGenericTargetIndex(node, argument, nodeType, reference.globalVariable);
				}
				else
				{
					resultType=0;
					return -1;
				}
			}

			vint GetGenericFunctionTargetIndex(BasicInstanciatedExpression* node, const BCP& argument, BasicTypeRecord*& resultType)
			{
				BasicTypeRecord* nodeType=argument.info->GetEnv()->GetExpressionType(node);
				BasicEnv::Reference reference=argument.info->GetEnv()->GetReference(node->reference.Obj());
				if(reference.isVariable)
				{
					resultType=0;
					return -1;
				}
				else
				{
					resultType=nodeType;
					return GetGenericTargetIndex(node, argument, nodeType, reference.function);
				}
			}

			vint GetGenericInstanceTargetIndex(BasicInstanceFunctionExpression* node, const BCP& argument)
			{
				BP bp(
					argument.info->GetEnv(),
					argument.info->GetSemanticScope(),
					argument.info->GetTypeManager(),
					*(List<Ptr<BasicLanguageCodeException>>*)0,
					*(SortedList<WString>*)0
					);
				BasicTypeRecord* type=BasicLanguage_GetTypeRecord(node->type, bp, false);
				Ptr<BasicScope::Concept> conceptObject=argument.info->GetSemanticScope()->concepts.Find(node->conceptName);

				Ptr<BasicCodegenInfo::GenericInstanceTarget> target=new BasicCodegenInfo::GenericInstanceTarget;
				target->ownerFunctionDeclaration=0;
				target->type=type;
				target->targetDeclaration=conceptObject->conceptDeclaration;
				target->functionName=node->functionName;
				return argument.info->RegisterGenericInstanceTarget(target);
			}

			void Code_ScaleAdder(BasicTypeRecord* addedValueType, const BCP& argument, bool scaleOne)
			{
				BasicOffset size=1;
				if(addedValueType->GetType()==BasicTypeRecord::Pointer)
				{
					size=argument.info->GetTypeInfo(addedValueType->ElementType())->size;
				}
				if(scaleOne)
				{
					argument.Ins(BasicIns::push, BasicIns::int_type, size);
				}
				else
				{
					argument.Ins(BasicIns::push, BasicIns::int_type, size);
					argument.Ins(BasicIns::mul, BasicIns::int_type);
				}
			}

			void Code_Read(BasicTypeRecord* type, const BCP& argument)
			{
				switch(type->GetType())
				{
				case BasicTypeRecord::Array:
				case BasicTypeRecord::Structure:
				case BasicTypeRecord::GenericArgument:
					argument.Ins(BasicIns::readmem, argument.info->GetTypeInfo(type)->size);
					break;
				default:
					argument.Ins(BasicIns::read, Convert(type));
				}
			}

			void Code_Write(BasicTypeRecord* type, const BCP& argument)
			{
				switch(type->GetType())
				{
				case BasicTypeRecord::Array:
				case BasicTypeRecord::Structure:
				case BasicTypeRecord::GenericArgument:
					argument.Ins(BasicIns::writemem, argument.info->GetTypeInfo(type)->size);
					break;
				default:
					argument.Ins(BasicIns::write, Convert(type));
				}
			}

			void Code_Copy(BasicTypeRecord* type, const BCP& argument)
			{
				argument.Ins(BasicIns::copymem, argument.info->GetTypeInfo(type)->size);
			}

			void Code_CopyStack(BasicTypeRecord* type, const BCP& argument, const BasicOffset& offset)
			{
				argument.Ins(BasicIns::stack_top, offset);
				Code_Read(type, argument);
			}

			void Code_CopyAddressInStack(BasicExpression* addressExpression, const BCP& argument, const BasicOffset& offset)
			{
				if(BasicLanguage_CanPushRefWithoutSideEffect(addressExpression, argument))
				{
					BasicLanguage_PushRefWithoutSideEffect(addressExpression, argument);
				}
				else
				{
					BasicTypeRecord* type=argument.info->GetEnv()->GetExpressionType(addressExpression);
					BasicTypeRecord* pointerType=argument.info->GetTypeManager()->GetPointerType(type);
					Code_CopyStack(pointerType, argument, offset);
				}
			}

			void Code_Convert(BasicTypeRecord* from, BasicTypeRecord* to, const BCP& argument)
			{
				if(from!=to)
				{
					BasicIns::ValueType fromType=Convert(from);
					BasicIns::ValueType toType=Convert(to);
					if(to==argument.info->GetTypeManager()->GetPrimitiveType(bool_type))
					{
						argument.Ins(BasicIns::push, fromType, BasicIns::MakeInt(0));
						argument.Ins(BasicIns::ne, fromType);
					}
					else if(fromType!=toType)
					{
						argument.Ins(BasicIns::convert, toType, fromType);
					}
				}
			}

			BasicTypeRecord* Code_Binary(BasicBinaryExpression* node, const BCP& argument, BasicIns::OpCode opCode)
			{
				BasicTypeRecord* type=argument.info->GetEnv()->GetExpressionType(node);
				if(type->GetType()==BasicTypeRecord::Primitive)
				{
					bool sign=false;
					vint bytes=0;
					if(argument.info->GetConfiguration().DecodeInteger(type->PrimitiveType(), sign, bytes))
					{
						if(bytes<4)
						{
							BasicPrimitiveTypeEnum primitiveType=void_type;
							argument.info->GetConfiguration().EncodeInteger(primitiveType, sign, 4);
							type=argument.info->GetTypeManager()->GetPrimitiveType(primitiveType);
						}
					}
				}
				BasicLanguage_PushValue(node->rightOperand, argument, type);
				BasicLanguage_PushValue(node->leftOperand, argument, type);
				argument.Ins(opCode, Convert(type));
				return type;
			}

			BasicTypeRecord* Code_Compare(BasicBinaryExpression* node, BasicTypeRecord* leftType, BasicTypeRecord* rightType, const BCP& argument, BasicIns::OpCode opCode)
			{
				BasicTypeRecord* type=leftType;
				if(leftType->GetType()==BasicTypeRecord::Primitive && rightType->GetType()==BasicTypeRecord::Primitive)
				{
					BasicPrimitiveTypeEnum result=void_type;
					argument.info->GetConfiguration().NumberBinaryOperatorTypeConversion(leftType->PrimitiveType(), rightType->PrimitiveType(), result);
					type=argument.info->GetTypeManager()->GetPrimitiveType(result);
				}
				BasicLanguage_PushValue(node->rightOperand, argument, type);
				BasicLanguage_PushValue(node->leftOperand, argument, type);
				argument.Ins(opCode, Convert(type));
				return argument.info->GetTypeManager()->GetPrimitiveType(bool_type);
			}

			BasicTypeRecord* Code_BinaryAssign(BasicBinaryExpression* node, const BCP& argument, BasicIns::OpCode opCode)
			{
				BasicTypeRecord* type=argument.info->GetEnv()->GetExpressionType(node);
				BasicOffset size=argument.info->GetTypeInfo(type)->size;
				if(BasicLanguage_CanPushRefWithoutSideEffect(node->leftOperand, argument))
				{
					BasicLanguage_PushValue(node->rightOperand, argument, type);
					BasicLanguage_PushValue(node->leftOperand, argument);
					argument.Ins(opCode, Convert(type));
					Code_CopyStack(type, argument);
					BasicLanguage_PushRefWithoutSideEffect(node->leftOperand, argument);
					Code_Write(type, argument);
				}
				else
				{
					BasicLanguage_PushRef(node->leftOperand, argument);
					BasicLanguage_PushValue(node->rightOperand, argument, type);
					Code_CopyAddressInStack(node->leftOperand.Obj(), argument, size);
					Code_Read(type, argument);
					argument.Ins(opCode, Convert(type));
					Code_CopyAddressInStack(node->leftOperand.Obj(), argument, size);
					Code_Write(type, argument);
					Code_Read(type, argument);
				}
				return type;
			}

			void Code_BinaryAssignSideEffect(BasicBinaryExpression* node, const BCP& argument, BasicIns::OpCode opCode)
			{
				BasicTypeRecord* type=argument.info->GetEnv()->GetExpressionType(node);
				BasicTypeRecord* pointerType=argument.info->GetTypeManager()->GetPointerType(type);
				BasicOffset size=argument.info->GetTypeInfo(type)->size;
				BasicOffset pointerSize=argument.info->GetTypeInfo(pointerType)->size;

				if(BasicLanguage_CanPushRefWithoutSideEffect(node->leftOperand, argument))
				{
					BasicLanguage_PushValue(node->rightOperand, argument, type);
					BasicLanguage_PushValue(node->leftOperand, argument, type);
					argument.Ins(opCode, Convert(type));
					BasicLanguage_PushRef(node->leftOperand, argument);
					Code_Write(type, argument);
				}
				else
				{
					BasicLanguage_PushRef(node->leftOperand, argument);
					BasicLanguage_PushValue(node->rightOperand, argument, type);
					Code_CopyAddressInStack(node->leftOperand.Obj(), argument, size);
					Code_Read(type, argument);
					argument.Ins(opCode, Convert(type));
					Code_CopyAddressInStack(node->leftOperand.Obj(), argument, size);
					Code_Write(type, argument);
					argument.Ins(BasicIns::stack_reserve, -pointerSize);
				}
			}

			void Code_BinaryAssignRef(BasicBinaryExpression* node, const BCP& argument, BasicIns::OpCode opCode)
			{
				BasicTypeRecord* type=argument.info->GetEnv()->GetExpressionType(node);
				BasicOffset size=argument.info->GetTypeInfo(type)->size;

				if(BasicLanguage_CanPushRefWithoutSideEffect(node->leftOperand, argument))
				{
					BasicLanguage_PushValue(node->rightOperand, argument, type);
					BasicLanguage_PushValue(node->leftOperand, argument);
					argument.Ins(opCode, Convert(type));
					BasicLanguage_PushRefWithoutSideEffect(node->leftOperand, argument);
					Code_Write(type, argument);
					BasicLanguage_PushRefWithoutSideEffect(node->leftOperand, argument);
				}
				else
				{
					BasicLanguage_PushRef(node->leftOperand, argument);
					BasicLanguage_PushValue(node->rightOperand, argument, type);
					Code_CopyAddressInStack(node->leftOperand.Obj(), argument, size);
					Code_Read(type, argument);
					argument.Ins(opCode, Convert(type));
					Code_CopyAddressInStack(node->leftOperand.Obj(), argument, size);
					Code_Write(type, argument);
				}
			}

			BasicTypeRecord* Code_InvokeFunctionPushParameters(BasicInvokeExpression* node, const BCP& argument, BasicOffset& returnSize, BasicOffset& parameterSize, bool returnInStack)
			{
				BasicTypeRecord* functionType=argument.info->GetEnv()->GetExpressionType(node->function.Obj());
				returnSize=argument.info->GetTypeInfo(functionType->ReturnType())->size;
				if(returnInStack)
				{
					argument.Ins(BasicIns::stack_reserve,returnSize);
				}

				parameterSize=0;
				for(vint i=node->arguments.Count()-1;i>=0;i--)
				{
					BasicLanguage_PushValue(node->arguments[i], argument, functionType->ParameterType(i));
					parameterSize+=argument.info->GetTypeInfo(functionType->ParameterType(i))->size;
				}
				return functionType;
			}

			void Code_InvokeFunctionCallFunction(BasicInvokeExpression* node, const BCP& argument, const BasicOffset& returnSize, const BasicOffset& parameterSize, bool clearReturnInStack)
			{
				BasicLanguage_Invoke(node->function.Obj(), argument);
				if(clearReturnInStack)
				{
					argument.Ins(BasicIns::stack_reserve, -returnSize);
				}
			}

			BasicTypeRecord* Code_InvokeFunction(BasicInvokeExpression* node, const BCP& argument, bool sideEffectOnly)
			{
				BasicOffset returnSize=0;
				BasicOffset parameterSize=0;
				BasicTypeRecord* functionType=Code_InvokeFunctionPushParameters(node, argument, returnSize, parameterSize, true);
				argument.Ins(BasicIns::stack_top, parameterSize);
				Code_InvokeFunctionCallFunction(node, argument, returnSize, parameterSize, sideEffectOnly);
				return functionType->ReturnType();
			}
		}
	}
}