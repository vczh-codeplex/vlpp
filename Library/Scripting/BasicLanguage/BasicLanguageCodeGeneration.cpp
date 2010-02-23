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

/***********************************************************************
BasicCodegenExtension
***********************************************************************/

			void BasicCodegenExtension::PushValue(BasicExtendedExpression* expression, const BCP& argument)
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

/***********************************************************************
BasicCodegenParameter
***********************************************************************/
			
			BasicCodegenParameter::BasicCodegenParameter(BasicCodegenInfo* _info, basicil::BasicIL* _il, stream::MemoryStream* _globalData)
				:info(_info)
				,il(_il)
				,globalData(_globalData)
				,codegenExtension(&defaultCodegenExtension)
				,expectedType(0)
			{
			}

			BasicCodegenParameter::BasicCodegenParameter(const BasicCodegenParameter& parameter, BasicTypeRecord* _expectedType)
				:info(parameter.info)
				,il(parameter.il)
				,globalData(parameter.globalData)
				,codegenExtension(parameter.codegenExtension)
				,expectedType(_expectedType)
			{
			}

/***********************************************************************
BasicLanguage_PushValueInternal
***********************************************************************/

			void BasicLanguage_PushValue(BasicExpression* expression, const BCP& argument)
			{
				// TODO: call BasicLanguage_PushValueInternal
				// TODO: do implicit casting
			}

			BEGIN_ALGORITHM_PROCEDURE(BasicLanguage_PushValueInternal, BasicExpression, BCP)

				ALGORITHM_PROCEDURE_MATCH(BasicNullExpression)
				{
				}

				ALGORITHM_PROCEDURE_MATCH(BasicNumericExpression)
				{
				}

				ALGORITHM_PROCEDURE_MATCH(BasicMbcsStringExpression)
				{
				}

				ALGORITHM_PROCEDURE_MATCH(BasicUnicodeStringExpression)
				{
				}

				ALGORITHM_PROCEDURE_MATCH(BasicUnaryExpression)
				{
				}

				ALGORITHM_PROCEDURE_MATCH(BasicBinaryExpression)
				{
				}

				ALGORITHM_PROCEDURE_MATCH(BasicSubscribeExpression)
				{
				}

				ALGORITHM_PROCEDURE_MATCH(BasicMemberExpression)
				{
				}

				ALGORITHM_PROCEDURE_MATCH(BasicInvokeExpression)
				{
				}

				ALGORITHM_PROCEDURE_MATCH(BasicFunctionResultExpression)
				{
				}

				ALGORITHM_PROCEDURE_MATCH(BasicCastingExpression)
				{
				}

				ALGORITHM_PROCEDURE_MATCH(BasicReferenceExpression)
				{
				}

				ALGORITHM_PROCEDURE_MATCH(BasicExtendedExpression)
				{
					argument.codegenExtension->PushValue(node, argument);
				}

			END_ALGORITHM_PROCEDURE(BasicLanguage_PushValueInternal)

/***********************************************************************
BasicLanguage_PushRef
***********************************************************************/

			BEGIN_ALGORITHM_PROCEDURE(BasicLanguage_PushRef, BasicExpression, BCP)

				ALGORITHM_PROCEDURE_MATCH(BasicNullExpression)
				{
				}

				ALGORITHM_PROCEDURE_MATCH(BasicNumericExpression)
				{
				}

				ALGORITHM_PROCEDURE_MATCH(BasicMbcsStringExpression)
				{
				}

				ALGORITHM_PROCEDURE_MATCH(BasicUnicodeStringExpression)
				{
				}

				ALGORITHM_PROCEDURE_MATCH(BasicUnaryExpression)
				{
					switch(node->type)
					{
					case BasicUnaryExpression::PrefixIncrease:
					case BasicUnaryExpression::PrefixDecrease:
					case BasicUnaryExpression::DereferencePointer:
						;
					}
				}

				ALGORITHM_PROCEDURE_MATCH(BasicBinaryExpression)
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
						;
					}
				}

				ALGORITHM_PROCEDURE_MATCH(BasicSubscribeExpression)
				{
					// TODO: Add implementation
				}

				ALGORITHM_PROCEDURE_MATCH(BasicMemberExpression)
				{
					// TODO: Add implementation
				}

				ALGORITHM_PROCEDURE_MATCH(BasicInvokeExpression)
				{
				}

				ALGORITHM_PROCEDURE_MATCH(BasicFunctionResultExpression)
				{
					// TODO: Add implementation
				}

				ALGORITHM_PROCEDURE_MATCH(BasicCastingExpression)
				{
				}

				ALGORITHM_PROCEDURE_MATCH(BasicReferenceExpression)
				{
					// TODO: Add implementation
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
				}

				ALGORITHM_PROCEDURE_MATCH(BasicExpressionStatement)
				{
				}

				ALGORITHM_PROCEDURE_MATCH(BasicVariableStatement)
				{
				}

				ALGORITHM_PROCEDURE_MATCH(BasicIfStatement)
				{
				}

				ALGORITHM_PROCEDURE_MATCH(BasicWhileStatement)
				{
				}

				ALGORITHM_PROCEDURE_MATCH(BasicForStatement)
				{
				}

				ALGORITHM_PROCEDURE_MATCH(BasicBreakStatement)
				{
				}

				ALGORITHM_PROCEDURE_MATCH(BasicContinueStatement)
				{
				}

				ALGORITHM_PROCEDURE_MATCH(BasicReturnStatement)
				{
				}

				ALGORITHM_PROCEDURE_MATCH(BasicExtendedStatement)
				{
					argument.codegenExtension->GenerateCode(node, argument);
				}

			END_ALGORITHM_PROCEDURE(BasicLanguage_GenerateCode)
		}
	}
}