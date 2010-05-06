#include "BasicLanguageCodeGeneration.h"

namespace vl
{
	namespace scripting
	{
		namespace basiclanguage
		{
			using namespace basicil;
			using namespace stream;

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
				CHECK_ERROR(false, L"Convert(BasicTypeRecord*)#不支持此操作。");
			}

			BasicIns::Argument Convert(BasicPrimitiveValueEnum value)
			{
				BasicIns::Argument argument;
				argument.s64=value.s64;
				return argument;
			}

			int GetFunctionIndex(BasicReferenceExpression* referenceExpression, const BCP& argument)
			{
				if(referenceExpression)
				{
					BasicEnv::Reference reference=argument.info->GetEnv()->GetReference(referenceExpression);
					if(!reference.isVariable)
					{
						BasicFunctionDeclaration* function=reference.scope->functions.Items()[referenceExpression->name];
						return argument.info->GetFunctions().IndexOf(function);
					}
				}
				return -1;
			}

			void Code_ScaleAdder(BasicTypeRecord* addedValueType, const BCP& argument, bool scaleOne)
			{
				int size=1;
				if(addedValueType->GetType()==BasicTypeRecord::Pointer)
				{
					size=argument.info->GetTypeInfo(addedValueType->ElementType())->size;
				}
				if(scaleOne)
				{
					if(size>1)
					{
						argument.il->Ins(BasicIns::push, BasicIns::int_type, BasicIns::MakeInt(size));
					}
					else
					{
						argument.il->Ins(BasicIns::push, BasicIns::int_type, BasicIns::MakeInt(1));
					}
				}
				else if(size>1)
				{
					argument.il->Ins(BasicIns::push, BasicIns::int_type, BasicIns::MakeInt(size));
					argument.il->Ins(BasicIns::mul, BasicIns::int_type);
				}
			}

			void Code_Read(BasicTypeRecord* type, const BCP& argument)
			{
				switch(type->GetType())
				{
				case BasicTypeRecord::Array:
				case BasicTypeRecord::Structure:
					argument.il->Ins(BasicIns::readmem, BasicIns::MakeInt(argument.info->GetTypeInfo(type)->size));
					break;
				default:
					argument.il->Ins(BasicIns::read, Convert(type));
				}
			}

			void Code_Write(BasicTypeRecord* type, const BCP& argument)
			{
				switch(type->GetType())
				{
				case BasicTypeRecord::Array:
				case BasicTypeRecord::Structure:
					argument.il->Ins(BasicIns::writemem, BasicIns::MakeInt(argument.info->GetTypeInfo(type)->size));
					break;
				default:
					argument.il->Ins(BasicIns::write, Convert(type));
				}
			}

			void Code_Copy(BasicTypeRecord* type, const BCP& argument)
			{
				argument.il->Ins(BasicIns::copymem, BasicIns::MakeInt(argument.info->GetTypeInfo(type)->size));
			}

			void Code_CopyStack(BasicTypeRecord* type, const BCP& argument, int offset)
			{
				argument.il->Ins(BasicIns::stack_top, BasicIns::MakeInt(offset));
				Code_Read(type, argument);
			}

			void Code_CopyAddressInStack(BasicExpression* addressExpression, const BCP& argument, int offset)
			{
				BasicTypeRecord* type=argument.info->GetEnv()->GetExpressionType(addressExpression);
				BasicTypeRecord* pointerType=argument.info->GetTypeManager()->GetPointerType(type);
				Code_CopyStack(pointerType, argument, offset);
			}

			void Code_Convert(BasicTypeRecord* from, BasicTypeRecord* to, const BCP& argument)
			{
				if(from!=to)
				{
					BasicIns::ValueType fromType=Convert(from);
					BasicIns::ValueType toType=Convert(to);
					if(to==argument.info->GetTypeManager()->GetPrimitiveType(bool_type))
					{
						argument.il->Ins(BasicIns::push, fromType, BasicIns::MakeInt(0));
						argument.il->Ins(BasicIns::ne, fromType);
					}
					else if(fromType!=toType)
					{
						argument.il->Ins(BasicIns::convert, toType, fromType);
					}
				}
			}

			BasicTypeRecord* Code_Binary(BasicBinaryExpression* node, const BCP& argument, BasicIns::OpCode opCode)
			{
				BasicTypeRecord* type=argument.info->GetEnv()->GetExpressionType(node);
				if(type->GetType()==BasicTypeRecord::Primitive)
				{
					bool sign=false;
					int bytes=0;
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
				argument.il->Ins(opCode, Convert(type));
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
				argument.il->Ins(opCode, Convert(type));
				return argument.info->GetTypeManager()->GetPrimitiveType(bool_type);
			}

			BasicTypeRecord* Code_BinaryAssign(BasicBinaryExpression* node, const BCP& argument, BasicIns::OpCode opCode)
			{
				BasicTypeRecord* type=argument.info->GetEnv()->GetExpressionType(node);
				int size=argument.info->GetTypeInfo(type)->size;
				BasicLanguage_PushRef(node->leftOperand, argument);
				BasicLanguage_PushValue(node->rightOperand, argument, type);
				Code_CopyAddressInStack(node->leftOperand.Obj(), argument, size);
				Code_Read(type, argument);
				argument.il->Ins(opCode, Convert(type));
				Code_CopyAddressInStack(node->leftOperand.Obj(), argument, size);
				Code_Write(type, argument);
				Code_Read(type, argument);
				return type;
			}

			void Code_BinaryAssignSideEffect(BasicBinaryExpression* node, const BCP& argument, BasicIns::OpCode opCode)
			{
				BasicTypeRecord* type=argument.info->GetEnv()->GetExpressionType(node);
				BasicLanguage_PushValue(node->rightOperand, argument, type);
				BasicLanguage_PushValue(node->leftOperand, argument, type);
				argument.il->Ins(opCode, Convert(type));
				BasicLanguage_PushRef(node->leftOperand, argument);
				Code_Write(type, argument);
			}

			void Code_BinaryAssignRef(BasicBinaryExpression* node, const BCP& argument, BasicIns::OpCode opCode)
			{
				BasicTypeRecord* type=argument.info->GetEnv()->GetExpressionType(node);
				int size=argument.info->GetTypeInfo(type)->size;
				BasicLanguage_PushRef(node->leftOperand, argument);
				BasicLanguage_PushValue(node->rightOperand, argument, type);
				Code_CopyAddressInStack(node->leftOperand.Obj(), argument, size);
				Code_Read(type, argument);
				argument.il->Ins(opCode, Convert(type));
				Code_CopyAddressInStack(node->leftOperand.Obj(), argument, size);
				Code_Write(type, argument);
			}

			BasicTypeRecord* Code_InvokeFunctionPushParameters(BasicInvokeExpression* node, const BCP& argument, int& index, int& returnSize, int& parameterSize, bool returnInStack)
			{
				BasicReferenceExpression* referenceExpression=dynamic_cast<BasicReferenceExpression*>(node->function.Obj());
				index=GetFunctionIndex(referenceExpression, argument);

				BasicTypeRecord* functionType=argument.info->GetEnv()->GetExpressionType(node->function.Obj());
				returnSize=argument.info->GetTypeInfo(functionType->ReturnType())->size;
				if(returnInStack)
				{
					argument.il->Ins(BasicIns::stack_reserve, BasicIns::MakeInt(returnSize));
				}

				parameterSize=0;
				for(int i=node->arguments.Count()-1;i>=0;i--)
				{
					BasicLanguage_PushValue(node->arguments[i], argument, functionType->ParameterType(i));
					parameterSize+=argument.info->GetTypeInfo(functionType->ParameterType(i))->size;
				}
				return functionType;
			}

			void Code_InvokeFunctionCallFunction(BasicInvokeExpression* node, const BCP& argument, int index, int returnSize, int parameterSize, bool clearReturnInStack)
			{
				if(index==-1)
				{
					BasicLanguage_PushValue(node->function, argument);
					argument.il->Ins(BasicIns::label);
					argument.il->Ins(BasicIns::call_indirect);
				}
				else
				{
					argument.il->Ins(BasicIns::codegen_callfunc, BasicIns::MakeInt(index));
				}
				if(clearReturnInStack)
				{
					argument.il->Ins(BasicIns::stack_reserve, BasicIns::MakeInt(-returnSize));
				}
			}

			BasicTypeRecord* Code_InvokeFunction(BasicInvokeExpression* node, const BCP& argument, bool sideEffectOnly)
			{
				int index=0;
				int returnSize=0;
				int parameterSize=0;
				BasicTypeRecord* functionType=Code_InvokeFunctionPushParameters(node, argument, index, returnSize, parameterSize, true);
				argument.il->Ins(BasicIns::stack_top, BasicIns::MakeInt(parameterSize));
				Code_InvokeFunctionCallFunction(node, argument, index, returnSize, parameterSize, sideEffectOnly);
				return functionType->ReturnType();
			}
		}
	}
}