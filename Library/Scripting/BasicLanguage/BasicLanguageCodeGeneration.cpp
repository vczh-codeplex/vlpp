#include "BasicLanguageCodeGeneration.h"

namespace vl
{
	namespace scripting
	{
		using namespace basicil;

		namespace basiclanguage
		{

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

/***********************************************************************
BasicLanguage_PushValue
***********************************************************************/

			BEGIN_ALGORITHM_PROCEDURE(BasicLanguage_PushValue, BasicExpression, BasicCodegenInfo)

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
				}

			END_ALGORITHM_PROCEDURE(BasicLanguage_PushValue)

/***********************************************************************
BasicLanguage_PushRef
***********************************************************************/

			BEGIN_ALGORITHM_PROCEDURE(BasicLanguage_PushRef, BasicExpression, BasicCodegenInfo)

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
				}

			END_ALGORITHM_PROCEDURE(BasicLanguage_PushRef)
		}
	}
}