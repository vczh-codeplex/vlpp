#include "BasicLanguageCodeGeneration.h"

namespace vl
{
	namespace scripting
	{
		using namespace basicil;

		namespace basiclanguage
		{
			using namespace basicil;
			using namespace stream;

/***********************************************************************
BasicCodegenInfo
***********************************************************************/

			BasicCodegenInfo::BasicCodegenInfo(BasicAnalyzer* _analyzer)
				:analyzer(_analyzer)
				,maxVariableSpace(0)
				,usedVariableSpace(0)
			{
			}

			BasicTypeInfo* BasicCodegenInfo::GetTypeInfo(BasicTypeRecord* type)
			{
				int index=typeInfos.Keys().IndexOf(type);
				if(index==-1)
				{
					BasicTypeInfo* info=new BasicTypeInfo;
					switch(type->GetType())
					{
					case BasicTypeRecord::Array:
						{
							BasicTypeInfo* element=GetTypeInfo(type->ElementType());
							info->alignment=element->alignment;
							info->size=element->size*type->ElementCount();
						}
						break;
					case BasicTypeRecord::Function:
					case BasicTypeRecord::Pointer:
						{
							info->alignment=sizeof(void*);
							info->size=sizeof(void*);
						}
						break;
					case BasicTypeRecord::Primitive:
						{
							switch(type->PrimitiveType())
							{
							case s8:case u8:case bool_type:case char_type:
								info->alignment=1;
								info->size=1;
								break;
							case s16:case u16:case wchar_type:
								info->alignment=2;
								info->size=2;
								break;
							case s32:case u32:case f32:
								info->alignment=4;
								info->size=4;
								break;
							case s64:case u64:case f64:
								info->alignment=8;
								info->size=8;
								break;
							default:
								info->alignment=1;
								info->size=1;
							}
						}
						break;
					case BasicTypeRecord::Structure:
						{
							int offset=0;
							info->alignment=0;
							info->size=0;
							for(int i=0;i<type->MemberCount();i++)
							{
								BasicTypeInfo* member=GetTypeInfo(type->MemberType(i));
								if(offset%member->alignment!=0)
								{
									offset=(offset/member->alignment+1)*member->alignment;
								}
								info->offsets.Add(offset);
								if(info->alignment<member->alignment)
								{
									info->alignment=member->alignment;
								}
								offset+=member->size;
							}
							if(info->alignment==0)
							{
								info->alignment=1;
								info->size=1;
							}
							else
							{
								if(offset%info->alignment==0)
								{
									info->size=offset;
								}
								else
								{
									info->size=(offset/info->alignment+1)*info->alignment;
								}
							}
						}
						break;
					}
					typeInfos.Add(type, info);
					return info;
				}
				else
				{
					return typeInfos.Values()[index].Obj();
				}
			}
			
			BasicEnv* BasicCodegenInfo::GetEnv()
			{
				return analyzer->GetEnv();
			}

			BasicTypeManager* BasicCodegenInfo::GetTypeManager()
			{
				return analyzer->GetTypeManager();
			}

			BasicAlgorithmConfiguration& BasicCodegenInfo::GetConfiguration()
			{
				return analyzer->GetConfiguration();
			}

			collections::IList<BasicFunctionDeclaration*>& BasicCodegenInfo::GetFunctions()
			{
				return functions.Wrap();
			}

			collections::IDictionary<BasicVariableDeclaration*, int>& BasicCodegenInfo::GetGlobalVariableOffsets()
			{
				return globalVariableOffsets.Wrap();
			}

			collections::IDictionary<BasicVariableStatement*, int>& BasicCodegenInfo::GetLocalVariableOffsets()
			{
				return localVariableOffsets.Wrap();
			}

			void BasicCodegenInfo::BeginFunction()
			{
				maxVariableSpace=0;
				usedVariableSpace=0;
				variableSpaceStack.Clear();
				variableSpaceStack.Add(0);
				breakInsStack.Clear();
				breakInstructions.Clear();
				continueInsStack.Clear();
				continueInstructions.Clear();
			}

			void BasicCodegenInfo::EnterScope()
			{
				variableSpaceStack.Add(usedVariableSpace);
			}

			void BasicCodegenInfo::LeaveScope()
			{
				usedVariableSpace=variableSpaceStack[variableSpaceStack.Count()-1];
				variableSpaceStack.RemoveAt(variableSpaceStack.Count());
			}

			int BasicCodegenInfo::UseVariable(int size)
			{
				usedVariableSpace+=size;
				if(usedVariableSpace>maxVariableSpace)
				{
					maxVariableSpace=usedVariableSpace;
				}
				return -usedVariableSpace;
			}

			void BasicCodegenInfo::EnterLoop()
			{
				breakInsStack.Add(breakInstructions.Count());
				continueInsStack.Add(continueInstructions.Count());
			}

			void BasicCodegenInfo::LeaveLoop(int breakIns, int continueIns)
			{
				int breakCount=breakInsStack[breakInsStack.Count()-1];
				breakInsStack.RemoveAt(breakCount-1);
				int continueCount=continueInsStack[continueInsStack.Count()-1];
				continueInsStack.RemoveAt(continueCount-1);

				for(int i=breakCount;i<breakInstructions.Count();i++)
				{
					breakInstructions[i]->argument.int_value=breakIns;
				}
				breakInstructions.RemoveRange(breakCount, breakInstructions.Count()-breakCount);

				for(int i=continueCount;i<continueInstructions.Count();i++)
				{
					continueInstructions[i]->argument.int_value=continueIns;
				}
				continueInstructions.RemoveRange(continueCount, continueInstructions.Count()-continueCount);
			}

			void BasicCodegenInfo::AssociateBreak(basicil::BasicIns* instruction)
			{
				breakInstructions.Add(instruction);
			}

			void BasicCodegenInfo::AssociateContinue(basicil::BasicIns* instruction)
			{
				continueInstructions.Add(instruction);
			}

/***********************************************************************
BasicCodegenExtension
***********************************************************************/

			BasicTypeRecord* BasicCodegenExtension::PushValue(BasicExtendedExpression* expression, const BCP& argument)
			{
				CHECK_ERROR(false, L"BasicCodegenExtension::PushValue(BasicExtendedExpression*, const BCP&)#不支持此操作。");
			}

			void BasicCodegenExtension::PushRef(BasicExtendedExpression* expression, const BCP& argument)
			{
				CHECK_ERROR(false, L"BasicCodegenExtension::PushRef(BasicExtendedExpression*, const BCP&)#不支持此操作。");
			}

			void BasicCodegenExtension::GenerateCode(BasicExtendedStatement* statement, const BCP& argument)
			{
				CHECK_ERROR(false, L"BasicCodegenExtension::GenerateCode(BasicExtendedStatement*, const BCP&)#不支持此操作。");
			}

			void BasicCodegenExtension::GenerateCodePass1(BasicExtendedDeclaration* statement, const BCP& argument)
			{
				CHECK_ERROR(false, L"BasicCodegenExtension::GenerateCodePass1(BasicExtendedDeclaration*, const BCP&)#不支持此操作。");
			}

			void BasicCodegenExtension::GenerateCodePass2(BasicExtendedDeclaration* statement, const BCP& argument)
			{
				CHECK_ERROR(false, L"BasicCodegenExtension::GenerateCodePass2(BasicExtendedDeclaration*, const BCP&)#不支持此操作。");
			}

/***********************************************************************
BasicCodegenParameter
***********************************************************************/
			
			BasicCodegenParameter::BasicCodegenParameter(BasicCodegenInfo* _info, basicil::BasicIL* _il, stream::MemoryStream* _globalData)
				:info(_info)
				,il(_il)
				,globalData(_globalData)
				,codegenExtension(&defaultCodegenExtension)
			{
			}

			BasicCodegenParameter::BasicCodegenParameter(const BasicCodegenParameter& parameter)
				:info(parameter.info)
				,il(parameter.il)
				,globalData(parameter.globalData)
				,codegenExtension(parameter.codegenExtension)
			{
			}

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
				// TODO: consider non-primitive type
				argument.il->Ins(BasicIns::read, Convert(type));
			}

			void Code_Write(BasicTypeRecord* type, const BCP& argument)
			{
				// TODO: consider non-primitive type
				argument.il->Ins(BasicIns::write, Convert(type));
			}

			void Code_CopyStack(BasicTypeRecord* type, const BCP& argument, int offset=0)
			{
				argument.il->Ins(BasicIns::stack_top, BasicIns::MakeInt(offset));
				Code_Read(type, argument);
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
							argument.info->GetConfiguration().EncodeInteger(primitiveType, sign, bytes);
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
				BasicLanguage_PushValue(node->rightOperand, argument, type);
				BasicLanguage_PushValue(node->leftOperand, argument, type);
				argument.il->Ins(opCode, Convert(type));
				Code_CopyStack(type, argument);
				BasicLanguage_PushRef(node->leftOperand, argument);
				Code_Write(type, argument);
				return type;
			}

			void Code_BinaryAssignRef(BasicBinaryExpression* node, const BCP& argument, BasicIns::OpCode opCode)
			{
				BasicTypeRecord* type=argument.info->GetEnv()->GetExpressionType(node);
				BasicTypeRecord* pointerType=argument.info->GetTypeManager()->GetPointerType(type);
				int size=argument.info->GetTypeInfo(type)->size;
				BasicLanguage_PushRef(node->leftOperand, argument);
				BasicLanguage_PushValue(node->rightOperand, argument, type);
				BasicLanguage_PushValue(node->leftOperand, argument, type);
				argument.il->Ins(opCode, Convert(type));
				Code_CopyStack(pointerType, argument, size);
				Code_Write(type, argument);
			}

/***********************************************************************
BasicLanguage_PushValueInternal
***********************************************************************/

			BasicTypeRecord* BasicLanguage_PushValue(Ptr<BasicExpression> expression, const BCP& argument)
			{
				return BasicLanguage_PushValueInternal(expression, argument);
			}

			BasicTypeRecord* BasicLanguage_PushValue(BasicExpression* expression, const BCP& argument)
			{
				return BasicLanguage_PushValueInternal(expression, argument);
			}

			BasicTypeRecord* BasicLanguage_PushValue(Ptr<BasicExpression> expression, const BCP& argument, BasicTypeRecord* expectedType)
			{
				return BasicLanguage_PushValue(expression.Obj(), argument, expectedType);
			}

			BasicTypeRecord* BasicLanguage_PushValue(BasicExpression* expression, const BCP& argument, BasicTypeRecord* expectedType)
			{
				BasicTypeRecord* type=BasicLanguage_PushValueInternal(expression, argument);
				Code_Convert(type, expectedType, argument);
				return expectedType;
			}

			BEGIN_ALGORITHM_FUNCTION(BasicLanguage_PushValueInternal, BasicExpression, BCP, BasicTypeRecord*)

				ALGORITHM_FUNCTION_MATCH(BasicNullExpression)
				{
					argument.il->Ins(BasicIns::push, BasicIns::pointer_type, BasicIns::MakePointer(0));
					return argument.info->GetEnv()->GetExpressionType(node);
				}

				ALGORITHM_FUNCTION_MATCH(BasicNumericExpression)
				{
					argument.il->Ins(BasicIns::push, Convert(node->type), Convert(node->argument));
					return argument.info->GetEnv()->GetExpressionType(node);
				}

				ALGORITHM_FUNCTION_MATCH(BasicMbcsStringExpression)
				{
					void* data=(void*)node->value.Buffer();
					int length=node->value.Length()*sizeof(char);
					int offset=(int)argument.globalData->Size();
					argument.globalData->Write(data, length);
					argument.il->Ins(BasicIns::link_pushdata, BasicIns::MakeInt(offset));
					return argument.info->GetEnv()->GetExpressionType(node);
				}

				ALGORITHM_FUNCTION_MATCH(BasicUnicodeStringExpression)
				{
					void* data=(void*)node->value.Buffer();
					int length=node->value.Length()*sizeof(wchar_t);
					int offset=(int)argument.globalData->Size();
					argument.globalData->Write(data, length);
					argument.il->Ins(BasicIns::link_pushdata, BasicIns::MakeInt(offset));
					return argument.info->GetEnv()->GetExpressionType(node);
				}

				ALGORITHM_FUNCTION_MATCH(BasicUnaryExpression)
				{
					BasicTypeRecord* nodeType=argument.info->GetEnv()->GetExpressionType(node);
					BasicTypeRecord* operandType=argument.info->GetEnv()->GetExpressionType(node->operand.Obj());
					switch(node->type)
					{
					case BasicUnaryExpression::PrefixIncrease:
						{
							Code_ScaleAdder(operandType, argument, true);
							BasicLanguage_PushValue(node->operand, argument);
							argument.il->Ins(BasicIns::add, Convert(operandType));
							Code_CopyStack(operandType, argument);
							BasicLanguage_PushRef(node->operand, argument);
							argument.il->Ins(BasicIns::write, Convert(operandType));
						}
						break;
					case BasicUnaryExpression::PostfixIncrease:
						{
							BasicLanguage_PushValue(node->operand, argument);
							Code_ScaleAdder(operandType, argument, true);
							Code_CopyStack(operandType, argument, argument.info->GetTypeInfo(operandType)->size);
							argument.il->Ins(BasicIns::add, Convert(operandType));
							BasicLanguage_PushRef(node->operand, argument);
							argument.il->Ins(BasicIns::write, Convert(operandType));
						}
						break;
					case BasicUnaryExpression::PrefixDecrease:
						{
							Code_ScaleAdder(operandType, argument, true);
							BasicLanguage_PushValue(node->operand, argument);
							argument.il->Ins(BasicIns::sub, Convert(operandType));
							Code_CopyStack(operandType, argument);
							BasicLanguage_PushRef(node->operand, argument);
							argument.il->Ins(BasicIns::write, Convert(operandType));
						}
						break;
					case BasicUnaryExpression::PostfixDecrease:
						{
							BasicLanguage_PushValue(node->operand, argument);
							Code_ScaleAdder(operandType, argument, true);
							Code_CopyStack(operandType, argument, argument.info->GetTypeInfo(operandType)->size);
							argument.il->Ins(BasicIns::sub, Convert(operandType));
							BasicLanguage_PushRef(node->operand, argument);
							argument.il->Ins(BasicIns::write, Convert(operandType));
						}
						break;
					case BasicUnaryExpression::GetAddress:
						{
							BasicLanguage_PushRef(node->operand, argument);
						}
						break;
					case BasicUnaryExpression::DereferencePointer:
						{
							BasicLanguage_PushValue(node->operand, argument);
							Code_Read(operandType->ElementType(), argument);
							Code_Convert(operandType->ElementType(), nodeType, argument);
						}
						break;
					case BasicUnaryExpression::Negative:
						{
							BasicLanguage_PushValue(node->operand, argument, nodeType);
							argument.il->Ins(BasicIns::neg, Convert(nodeType));
						}
						break;
					case BasicUnaryExpression::Not:
					case BasicUnaryExpression::BitNot:
						{
							BasicTypeRecord* type=argument.info->GetEnv()->GetExpressionType(node);
							BasicLanguage_PushValue(node->operand, argument, nodeType);
							argument.il->Ins(BasicIns::not, Convert(nodeType));
						}
						break;
					}
					return argument.info->GetEnv()->GetExpressionType(node);
				}

				ALGORITHM_FUNCTION_MATCH(BasicBinaryExpression)
				{
					BasicTypeRecord* leftType=argument.info->GetEnv()->GetExpressionType(node->leftOperand.Obj());
					BasicTypeRecord* rightType=argument.info->GetEnv()->GetExpressionType(node->rightOperand.Obj());

					switch(node->type)
					{
					case BasicBinaryExpression::Add:
						if(leftType->GetType()==BasicTypeRecord::Pointer)
						{
							BasicLanguage_PushValue(node->rightOperand, argument, argument.info->GetTypeManager()->GetPrimitiveType(int_type));
							Code_ScaleAdder(leftType, argument, false);
							BasicLanguage_PushValue(node->leftOperand, argument);
							argument.il->Ins(BasicIns::add, BasicIns::pointer_type);
							return leftType;
						}
						else if(rightType->GetType()==BasicTypeRecord::Pointer)
						{
							BasicLanguage_PushValue(node->rightOperand, argument);
							BasicLanguage_PushValue(node->leftOperand, argument, argument.info->GetTypeManager()->GetPrimitiveType(int_type));
							Code_ScaleAdder(leftType, argument, false);
							argument.il->Ins(BasicIns::add, BasicIns::pointer_type);
							return rightType;
						}
						else
						{
							return Code_Binary(node, argument, BasicIns::add);
						}
					case BasicBinaryExpression::Sub:
						if(leftType->GetType()==BasicTypeRecord::Pointer)
						{
							int size=argument.info->GetTypeInfo(leftType->ElementType())->size;
							if(rightType->GetType()==BasicTypeRecord::Pointer)
							{
								if(size>1)
								{
									argument.il->Ins(BasicIns::push, BasicIns::int_type, BasicIns::MakeInt(size));
								}
								BasicLanguage_PushValue(node->rightOperand, argument);
								BasicLanguage_PushValue(node->leftOperand, argument);
								argument.il->Ins(BasicIns::sub, BasicIns::pointer_type);
								if(size>1)
								{
									argument.il->Ins(BasicIns::div, BasicIns::int_type);
								}
								return argument.info->GetTypeManager()->GetPrimitiveType(int_type);
							}
							else
							{
								BasicLanguage_PushValue(node->rightOperand, argument, argument.info->GetTypeManager()->GetPrimitiveType(int_type));
								Code_ScaleAdder(leftType, argument, false);
								BasicLanguage_PushValue(node->leftOperand, argument);
								argument.il->Ins(BasicIns::sub, BasicIns::pointer_type);
								return leftType;
							}
						}
						else
						{
							return Code_Binary(node, argument, BasicIns::sub);
						}
					case BasicBinaryExpression::Mul:
						return Code_Binary(node, argument, BasicIns::mul);
					case BasicBinaryExpression::Div:
						return Code_Binary(node, argument, BasicIns::div);
					case BasicBinaryExpression::Mod:
						return Code_Binary(node, argument, BasicIns::mod);
					case BasicBinaryExpression::Shl:
						return Code_Binary(node, argument, BasicIns::shl);
					case BasicBinaryExpression::Shr:
						return Code_Binary(node, argument, BasicIns::shr);
					case BasicBinaryExpression::And:
					case BasicBinaryExpression::BitAnd:
						return Code_Binary(node, argument, BasicIns::and);
					case BasicBinaryExpression::Or:
					case BasicBinaryExpression::BitOr:
						return Code_Binary(node, argument, BasicIns::or);
					case BasicBinaryExpression::Xor:
						return Code_Binary(node, argument, BasicIns::xor);
					case BasicBinaryExpression::Lt:
						return Code_Compare(node, leftType, rightType, argument, BasicIns::lt);
					case BasicBinaryExpression::Le:
						return Code_Compare(node, leftType, rightType, argument, BasicIns::le);
					case BasicBinaryExpression::Gt:
						return Code_Compare(node, leftType, rightType, argument, BasicIns::gt);
					case BasicBinaryExpression::Ge:
						return Code_Compare(node, leftType, rightType, argument, BasicIns::ge);
					case BasicBinaryExpression::Eq:
						return Code_Compare(node, leftType, rightType, argument, BasicIns::eq);
					case BasicBinaryExpression::Ne:
						return Code_Compare(node, leftType, rightType, argument, BasicIns::ne);
					case BasicBinaryExpression::AddAssign:
						if(leftType->GetType()==BasicTypeRecord::Pointer)
						{
							BasicLanguage_PushValue(node->rightOperand, argument, leftType);
							Code_ScaleAdder(leftType, argument, false);
							BasicLanguage_PushValue(node->leftOperand, argument, leftType);
							argument.il->Ins(BasicIns::add, Convert(leftType));
							Code_CopyStack(leftType, argument);
							BasicLanguage_PushRef(node->leftOperand, argument);
							Code_Write(leftType, argument);
							return leftType;
						}
						else
						{
							return Code_BinaryAssign(node, argument, BasicIns::add);
						}
					case BasicBinaryExpression::SubAssign:
						if(leftType->GetType()==BasicTypeRecord::Pointer)
						{
							BasicLanguage_PushValue(node->rightOperand, argument, leftType);
							Code_ScaleAdder(leftType, argument, false);
							BasicLanguage_PushValue(node->leftOperand, argument, leftType);
							argument.il->Ins(BasicIns::sub, Convert(leftType));
							Code_CopyStack(leftType, argument);
							BasicLanguage_PushRef(node->leftOperand, argument);
							Code_Write(leftType, argument);
							return leftType;
						}
						else
						{
							return Code_BinaryAssign(node, argument, BasicIns::sub);
						}
					case BasicBinaryExpression::MulAssign:
						return Code_BinaryAssign(node, argument, BasicIns::mul);
					case BasicBinaryExpression::DivAssign:
						return Code_BinaryAssign(node, argument, BasicIns::div);
					case BasicBinaryExpression::ModAssign:
						return Code_BinaryAssign(node, argument, BasicIns::mod);
					case BasicBinaryExpression::ShlAssign:
						return Code_BinaryAssign(node, argument, BasicIns::shl);
					case BasicBinaryExpression::ShrAssign:
						return Code_BinaryAssign(node, argument, BasicIns::shr);
					case BasicBinaryExpression::AndAssign:
					case BasicBinaryExpression::BitAndAssign:
						return Code_BinaryAssign(node, argument, BasicIns::and);
					case BasicBinaryExpression::OrAssign:
					case BasicBinaryExpression::BitOrAssign:
						return Code_BinaryAssign(node, argument, BasicIns::or);
					case BasicBinaryExpression::XorAssign:
						return Code_BinaryAssign(node, argument, BasicIns::xor);
					case BasicBinaryExpression::Assign:
						{
							// TODO: Optimize for big type
							BasicLanguage_PushValue(node->rightOperand, argument, leftType);
							Code_CopyStack(leftType, argument);
							BasicLanguage_PushRef(node->leftOperand, argument);
							Code_Write(leftType, argument);
							return leftType;
						}
					}
					return 0;
				}

				ALGORITHM_FUNCTION_MATCH(BasicSubscribeExpression)
				{
					BasicTypeRecord* nodeType=argument.info->GetEnv()->GetExpressionType(node);
					BasicLanguage_PushRef(node, argument);
					Code_Read(nodeType, argument);
					return nodeType;
				}

				ALGORITHM_FUNCTION_MATCH(BasicMemberExpression)
				{
					BasicTypeRecord* nodeType=argument.info->GetEnv()->GetExpressionType(node);
					BasicLanguage_PushRef(node, argument);
					Code_Read(nodeType, argument);
					return nodeType;
				}

				ALGORITHM_FUNCTION_MATCH(BasicInvokeExpression)
				{
					BasicReferenceExpression* referenceExpression=dynamic_cast<BasicReferenceExpression*>(node->function.Obj());
					int index=GetFunctionIndex(referenceExpression, argument);

					BasicTypeRecord* functionType=argument.info->GetEnv()->GetExpressionType(node->function.Obj());
					argument.il->Ins(BasicIns::stack_reserve, BasicIns::MakeInt(argument.info->GetTypeInfo(functionType->ReturnType())->size));
					int parameterSize=0;
					for(int i=node->arguments.Count()-1;i>=0;i--)
					{
						BasicLanguage_PushValue(node->arguments[i], argument, functionType->ParameterType(i));
						parameterSize+=argument.info->GetTypeInfo(functionType->ParameterType(i))->size;
					}
					argument.il->Ins(BasicIns::stack_top, BasicIns::MakeInt(parameterSize));
					BasicLanguage_PushValue(node->function, argument);
					if(index==-1)
					{
						argument.il->Ins(BasicIns::label);
						argument.il->Ins(BasicIns::call_indirect);
					}
					else
					{
						argument.il->Ins(BasicIns::codegen_callfunc, BasicIns::MakeInt(index));
					}
					return functionType->ReturnType();
				}

				ALGORITHM_FUNCTION_MATCH(BasicFunctionResultExpression)
				{
					BasicTypeRecord* type=argument.info->GetEnv()->GetExpressionType(node);
					BasicLanguage_PushRef(node, argument);
					Code_Read(type, argument);
					return type;
				}

				ALGORITHM_FUNCTION_MATCH(BasicCastingExpression)
				{
					BasicTypeRecord* nodeType=argument.info->GetEnv()->GetExpressionType(node);
					BasicTypeRecord* operandType=argument.info->GetEnv()->GetExpressionType(node->operand.Obj());
					BasicLanguage_PushValue(node->operand, argument);
					Code_Convert(operandType, nodeType, argument);
					return nodeType;
				}

				ALGORITHM_FUNCTION_MATCH(BasicReferenceExpression)
				{
					BasicTypeRecord* nodeType=argument.info->GetEnv()->GetExpressionType(node);
					BasicEnv::Reference reference=argument.info->GetEnv()->GetReference(node);
					if(reference.isVariable)
					{
						BasicLanguage_PushRef(node, argument);
						Code_Read(nodeType, argument);
					}
					else
					{
						int index=GetFunctionIndex(node, argument);
						argument.il->Ins(BasicIns::codegen_pushfunc, BasicIns::MakeInt(index));
					}
					return nodeType;
				}

				ALGORITHM_FUNCTION_MATCH(BasicExtendedExpression)
				{
					return argument.codegenExtension->PushValue(node, argument);
				}

			END_ALGORITHM_FUNCTION(BasicLanguage_PushValueInternal)

/***********************************************************************
BasicLanguage_PushRef
***********************************************************************/

			BEGIN_ALGORITHM_PROCEDURE(BasicLanguage_PushRef, BasicExpression, BCP)

				ALGORITHM_PROCEDURE_MATCH(BasicNullExpression)
				{
					CHECK_ERROR(false, L"BasicLanguage_PushRef(BasicNullExpression*, const BCP&)#不支持此操作。");
				}

				ALGORITHM_PROCEDURE_MATCH(BasicNumericExpression)
				{
					CHECK_ERROR(false, L"BasicLanguage_PushRef(BasicNumericExpression*, const BCP&)#不支持此操作。");
				}

				ALGORITHM_PROCEDURE_MATCH(BasicMbcsStringExpression)
				{
					CHECK_ERROR(false, L"BasicLanguage_PushRef(BasicMbcsStringExpression*, const BCP&)#不支持此操作。");
				}

				ALGORITHM_PROCEDURE_MATCH(BasicUnicodeStringExpression)
				{
					CHECK_ERROR(false, L"BasicLanguage_PushRef(BasicUnicodeStringExpression*, const BCP&)#不支持此操作。");
				}

				ALGORITHM_PROCEDURE_MATCH(BasicUnaryExpression)
				{
					BasicTypeRecord* operandType=argument.info->GetEnv()->GetExpressionType(node->operand.Obj());
					BasicTypeRecord* pointerType=argument.info->GetTypeManager()->GetPointerType(operandType);
					int operandSize=argument.info->GetTypeInfo(operandType)->size;
					switch(node->type)
					{
					case BasicUnaryExpression::PrefixIncrease:
						{
							BasicLanguage_PushRef(node->operand, argument);
							Code_ScaleAdder(operandType, argument, true);
							BasicLanguage_PushValue(node->operand, argument);
							Code_CopyStack(operandType, argument, operandSize);
							argument.il->Ins(BasicIns::add, Convert(operandType));
							argument.il->Ins(BasicIns::write, Convert(operandType));
						}
						break;
					case BasicUnaryExpression::PrefixDecrease:
						{
							BasicLanguage_PushRef(node->operand, argument);
							Code_ScaleAdder(operandType, argument, true);
							BasicLanguage_PushValue(node->operand, argument);
							Code_CopyStack(operandType, argument, operandSize);
							argument.il->Ins(BasicIns::sub, Convert(operandType));
							argument.il->Ins(BasicIns::write, Convert(operandType));
						}
						break;
					case BasicUnaryExpression::DereferencePointer:
						BasicLanguage_PushValue(node->operand, argument);
						break;
					default:
						CHECK_ERROR(false, L"BasicLanguage_PushRef(BasicUnaryExpression*, const BCP&)#不支持此操作。");
					}
				}

				ALGORITHM_PROCEDURE_MATCH(BasicBinaryExpression)
				{
					BasicTypeRecord* leftType=argument.info->GetEnv()->GetExpressionType(node->leftOperand.Obj());
					BasicTypeRecord* pointerType=argument.info->GetTypeManager()->GetPointerType(leftType);
					int leftSize=argument.info->GetTypeInfo(leftType)->size;
					switch(node->type)
					{
					case BasicBinaryExpression::AddAssign:
						if(leftType->GetType()==BasicTypeRecord::Pointer)
						{
							BasicLanguage_PushRef(node->leftOperand, argument);
							BasicLanguage_PushValue(node->rightOperand, argument, leftType);
							Code_ScaleAdder(leftType, argument, false);
							BasicLanguage_PushValue(node->leftOperand, argument, leftType);
							argument.il->Ins(BasicIns::add, Convert(leftType));
							Code_CopyStack(pointerType, argument, leftSize);
							Code_Write(leftType, argument);
						}
						else
						{
							Code_BinaryAssignRef(node, argument, BasicIns::add);
						}
						break;
					case BasicBinaryExpression::SubAssign:
						if(leftType->GetType()==BasicTypeRecord::Pointer)
						{
							BasicLanguage_PushRef(node->leftOperand, argument);
							BasicLanguage_PushValue(node->rightOperand, argument, leftType);
							Code_ScaleAdder(leftType, argument, false);
							BasicLanguage_PushValue(node->leftOperand, argument, leftType);
							argument.il->Ins(BasicIns::sub, Convert(leftType));
							Code_CopyStack(pointerType, argument, leftSize);
							Code_Write(leftType, argument);
						}
						else
						{
							Code_BinaryAssignRef(node, argument, BasicIns::sub);
						}
						break;
					case BasicBinaryExpression::MulAssign:
						Code_BinaryAssignRef(node, argument, BasicIns::mul);
						break;
					case BasicBinaryExpression::DivAssign:
						Code_BinaryAssignRef(node, argument, BasicIns::div);
						break;
					case BasicBinaryExpression::ModAssign:
						Code_BinaryAssignRef(node, argument, BasicIns::mod);
						break;
					case BasicBinaryExpression::ShlAssign:
						Code_BinaryAssignRef(node, argument, BasicIns::shl);
						break;
					case BasicBinaryExpression::ShrAssign:
						Code_BinaryAssignRef(node, argument, BasicIns::shr);
						break;
					case BasicBinaryExpression::AndAssign:
					case BasicBinaryExpression::BitAndAssign:
						Code_BinaryAssignRef(node, argument, BasicIns::and);
						break;
					case BasicBinaryExpression::OrAssign:
					case BasicBinaryExpression::BitOrAssign:
						Code_BinaryAssignRef(node, argument, BasicIns::or);
						break;
					case BasicBinaryExpression::XorAssign:
						Code_BinaryAssignRef(node, argument, BasicIns::xor);
						break;
					case BasicBinaryExpression::Assign:
						{
							// TODO: Optimize for big type
							BasicLanguage_PushRef(node->leftOperand, argument);
							BasicLanguage_PushValue(node->rightOperand, argument, leftType);
							Code_CopyStack(pointerType, argument, leftSize);
							Code_Write(leftType, argument);
						}
						break;
					default:
						CHECK_ERROR(false, L"BasicLanguage_PushRef(BasicUnaryExpression*, const BCP&)#不支持此操作。");
					}
				}

				ALGORITHM_PROCEDURE_MATCH(BasicSubscribeExpression)
				{
					BasicTypeRecord* nodeType=argument.info->GetEnv()->GetExpressionType(node);
					BasicTypeRecord* operandType=argument.info->GetEnv()->GetExpressionType(node->operand.Obj());
					if(operandType->GetType()==BasicTypeRecord::Pointer)
					{
						BasicLanguage_PushValue(node->operand, argument);
					}
					else
					{
						BasicLanguage_PushRef(node->operand, argument);
						operandType=argument.info->GetTypeManager()->GetPointerType(nodeType);
					}
					BasicLanguage_PushValue(node->subscribe, argument, argument.info->GetTypeManager()->GetPrimitiveType(int_type));
					Code_ScaleAdder(operandType, argument, false);
					argument.il->Ins(BasicIns::add, BasicIns::int_type);
				}

				ALGORITHM_PROCEDURE_MATCH(BasicMemberExpression)
				{
					BasicTypeRecord* structureType=argument.info->GetEnv()->GetExpressionType(node->operand.Obj());
					if(node->pointerMember)
					{
						structureType=structureType->ElementType();
					}
					BasicTypeInfo* structureInfo=argument.info->GetTypeInfo(structureType);
					int offset=structureInfo->offsets[structureType->MemberNameIndex(node->member)];
					if(node->pointerMember)
					{
						BasicLanguage_PushValue(node->operand, argument);
					}
					else
					{
						BasicLanguage_PushRef(node->operand, argument);
					}
					argument.il->Ins(BasicIns::push, BasicIns::int_type, BasicIns::MakeInt(offset));
					argument.il->Ins(BasicIns::add, BasicIns::int_type);
				}

				ALGORITHM_PROCEDURE_MATCH(BasicInvokeExpression)
				{
					CHECK_ERROR(false, L"BasicLanguage_PushRef(BasicInvokeExpression*, const BCP&)#不支持此操作。");
				}

				ALGORITHM_PROCEDURE_MATCH(BasicFunctionResultExpression)
				{
					argument.il->Ins(BasicIns::resptr);
				}

				ALGORITHM_PROCEDURE_MATCH(BasicCastingExpression)
				{
					CHECK_ERROR(false, L"BasicLanguage_PushRef(BasicCastingExpression*, const BCP&)#不支持此操作。");
				}

				ALGORITHM_PROCEDURE_MATCH(BasicReferenceExpression)
				{
					BasicEnv::Reference reference=argument.info->GetEnv()->GetReference(node);
					if(reference.isVariable)
					{
						if(reference.globalVariable)
						{
							int offset=argument.info->GetGlobalVariableOffsets()[reference.globalVariable];
							argument.il->Ins(BasicIns::link_pushdata, BasicIns::MakeInt(offset));
						}
						else if(reference.localVariable)
						{
							int offset=argument.info->GetLocalVariableOffsets()[reference.localVariable];
							argument.il->Ins(BasicIns::stack_offset, BasicIns::MakeInt(offset));
						}
						else
						{
							int offset=0;
							BasicFunctionDeclaration* function=reference.scope->OwnerDeclaration();
							BasicTypeRecord* functionType=argument.info->GetEnv()->GetFunctionType(function);
							for(int i=0;i<reference.parameterIndex;i++)
							{
								offset+=argument.info->GetTypeInfo(functionType->ParameterType(i))->size;
							}
							argument.il->Ins(BasicIns::stack_offset, BasicIns::MakeInt(offset+sizeof(int)*4));
						}
					}
					else
					{
						CHECK_ERROR(false, L"BasicLanguage_PushRef(BasicCastingExpression*, const BCP&)#不支持此操作。");
					}
				}

				ALGORITHM_PROCEDURE_MATCH(BasicExtendedExpression)
				{
					argument.codegenExtension->PushRef(node, argument);
				}

			END_ALGORITHM_PROCEDURE(BasicLanguage_PushRef)

/***********************************************************************
BasicLanguage_GenerateCode
***********************************************************************/

			BEGIN_ALGORITHM_PROCEDURE(BasicLanguage_GenerateCode, BasicStatement, BCP)

				ALGORITHM_PROCEDURE_MATCH(BasicEmptyStatement)
				{
				}

				ALGORITHM_PROCEDURE_MATCH(BasicCompositeStatement)
				{
					argument.info->EnterScope();
					for(int i=0;i<node->statements.Count();i++)
					{
						BasicLanguage_GenerateCode(node->statements[i], argument);
					}
					argument.info->LeaveScope();
				}

				ALGORITHM_PROCEDURE_MATCH(BasicExpressionStatement)
				{
					// TODO: optimize for laziness
					BasicTypeRecord* type=BasicLanguage_PushValue(node->expression, argument);
					int size=argument.info->GetTypeInfo(type)->size;
					argument.il->Ins(BasicIns::stack_reserve, BasicIns::MakeInt(-size));
				}

				ALGORITHM_PROCEDURE_MATCH(BasicVariableStatement)
				{
					BasicScope* scope=argument.info->GetEnv()->GetStatementScope(node);
					BasicTypeRecord* type=scope->variables.Items()[node->name].type;
					int size=argument.info->GetTypeInfo(type)->size;
					int offset=argument.info->UseVariable(size);
					argument.info->GetLocalVariableOffsets().Add(node, offset);
					if(node->initializer)
					{
						BasicLanguage_PushValue(node->initializer, argument, type);
						argument.il->Ins(BasicIns::stack_offset, BasicIns::MakeInt(offset));
						Code_Write(type, argument);
					}
				}

				ALGORITHM_PROCEDURE_MATCH(BasicIfStatement)
				{
					BasicLanguage_PushValue(node->condition, argument, argument.info->GetTypeManager()->GetPrimitiveType(bool_type));
					argument.il->Ins(BasicIns::jumpfalse, BasicIns::MakeInt(0));
					BasicIns& jumpToFalse=argument.il->Last();
					BasicLanguage_GenerateCode(node->trueStatement, argument);
					if(node->falseStatement)
					{
						argument.il->Ins(BasicIns::jump, BasicIns::MakeInt(0));
						BasicIns& jumpToEnd=argument.il->Last();
						jumpToFalse.argument.int_value=argument.il->instructions.Count();
						BasicLanguage_GenerateCode(node->falseStatement, argument);
						jumpToEnd.argument.int_value=argument.il->instructions.Count();
					}
					else
					{
						jumpToFalse.argument.int_value=argument.il->instructions.Count();
					}
				}

				ALGORITHM_PROCEDURE_MATCH(BasicWhileStatement)
				{
					int continueBegin=argument.il->instructions.Count();
					argument.info->EnterLoop();
					if(node->beginCondition)
					{
						BasicLanguage_PushValue(node->beginCondition, argument, argument.info->GetTypeManager()->GetPrimitiveType(bool_type));
						argument.il->Ins(BasicIns::jumpfalse, BasicIns::MakeInt(0));
						argument.info->AssociateBreak(&argument.il->Last());
					}
					BasicLanguage_GenerateCode(node->statement, argument);
					if(node->endCondition)
					{
						BasicLanguage_PushValue(node->beginCondition, argument, argument.info->GetTypeManager()->GetPrimitiveType(bool_type));
						argument.il->Ins(BasicIns::jumpfalse, BasicIns::MakeInt(0));
						argument.info->AssociateBreak(&argument.il->Last());
					}
					argument.il->Ins(BasicIns::jump, BasicIns::MakeInt(0));
					argument.info->AssociateContinue(&argument.il->Last());
					int breakBegin=argument.il->instructions.Count();
					argument.info->LeaveLoop(breakBegin, continueBegin);
				}

				ALGORITHM_PROCEDURE_MATCH(BasicForStatement)
				{
					argument.info->EnterScope();
					if(node->initializer)
					{
						BasicLanguage_GenerateCode(node->initializer, argument);
					}
					int loopBegin=argument.il->instructions.Count();
					argument.info->EnterLoop();
					BasicLanguage_PushValue(node->condition, argument, argument.info->GetTypeManager()->GetPrimitiveType(bool_type));
					argument.il->Ins(BasicIns::jumpfalse, BasicIns::MakeInt(0));
					argument.info->AssociateBreak(&argument.il->Last());
					BasicLanguage_GenerateCode(node->statement, argument);
					int continueBegin=argument.il->instructions.Count();
					if(node->sideEffect)
					{
						BasicLanguage_GenerateCode(node->statement, argument);
					}
					argument.il->Ins(BasicIns::jump, BasicIns::MakeInt(loopBegin));
					int breakBegin=argument.il->instructions.Count();
					argument.info->LeaveLoop(breakBegin, continueBegin);
					argument.info->LeaveScope();
				}

				ALGORITHM_PROCEDURE_MATCH(BasicBreakStatement)
				{
					argument.il->Ins(BasicIns::jump, BasicIns::MakeInt(0));
					argument.info->AssociateBreak(&argument.il->Last());
				}

				ALGORITHM_PROCEDURE_MATCH(BasicContinueStatement)
				{
					argument.il->Ins(BasicIns::jump, BasicIns::MakeInt(0));
					argument.info->AssociateContinue(&argument.il->Last());
				}

				ALGORITHM_PROCEDURE_MATCH(BasicReturnStatement)
				{
				}

				ALGORITHM_PROCEDURE_MATCH(BasicExtendedStatement)
				{
					argument.codegenExtension->GenerateCode(node, argument);
				}

			END_ALGORITHM_PROCEDURE(BasicLanguage_GenerateCode)

/***********************************************************************
BasicLanguage_GenerateCodePass1
***********************************************************************/

			BEGIN_ALGORITHM_PROCEDURE(BasicLanguage_GenerateCodePass1, BasicDeclaration, BCP)

				ALGORITHM_PROCEDURE_MATCH(BasicFunctionDeclaration)
				{
					argument.info->GetFunctions().Add(node);
				}

				ALGORITHM_PROCEDURE_MATCH(BasicVariableDeclaration)
				{
					BasicTypeRecord* type=argument.info->GetEnv()->GlobalScope()->variables.Items()[node->name].type;
					int size=argument.info->GetTypeInfo(type)->size;
					char* data=new char[size];
					memset(data, 0, size);
					int offset=(int)argument.globalData->Size();
					argument.globalData->Write(data, size);
					argument.info->GetGlobalVariableOffsets().Add(node, offset);
					delete[] data;

					// TODO: Generate code for initializer
				}

				ALGORITHM_PROCEDURE_MATCH(BasicTypeRenameDeclaration)
				{
				}

				ALGORITHM_PROCEDURE_MATCH(BasicStructureDeclaration)
				{
				}

				ALGORITHM_PROCEDURE_MATCH(BasicExtendedDeclaration)
				{
					argument.codegenExtension->GenerateCodePass1(node, argument);
				}

			END_ALGORITHM_PROCEDURE(BasicLanguage_GenerateCodePass1)

/***********************************************************************
BasicLanguage_GenerateCodePass2
***********************************************************************/

			BEGIN_ALGORITHM_PROCEDURE(BasicLanguage_GenerateCodePass2, BasicDeclaration, BCP)

				ALGORITHM_PROCEDURE_MATCH(BasicFunctionDeclaration)
				{
					if(node->statement)
					{
						// TODO: initialization
						argument.info->BeginFunction();
						BasicLanguage_GenerateCode(node->statement, argument);
						// TODO: finalization
					}
				}

				ALGORITHM_PROCEDURE_MATCH(BasicVariableDeclaration)
				{
				}

				ALGORITHM_PROCEDURE_MATCH(BasicTypeRenameDeclaration)
				{
				}

				ALGORITHM_PROCEDURE_MATCH(BasicStructureDeclaration)
				{
				}

				ALGORITHM_PROCEDURE_MATCH(BasicExtendedDeclaration)
				{
					argument.codegenExtension->GenerateCodePass2(node, argument);
				}

			END_ALGORITHM_PROCEDURE(BasicLanguage_GenerateCodePass2)

/***********************************************************************
BasicLanguage_GenerateCode
***********************************************************************/

			void BasicLanguage_GenerateCode(Ptr<BasicProgram> program, const BCP& argument)
			{
				for(int i=0;i<program->declarations.Count();i++)
				{
					BasicLanguage_GenerateCodePass1(program->declarations[i], argument);
				}
				for(int i=0;i<program->declarations.Count();i++)
				{
					BasicLanguage_GenerateCodePass2(program->declarations[i], argument);
				}
			}
		}
	}
}