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
				CHECK_FAIL(L"BasicSemanticExtension::GetTypeRecord(BasicExtendedType*, const BP&)#不支持此操作。");
			}

			void BasicSemanticExtension::BuildGlobalScopePass1(BasicExtendedDeclaration* declaration, const BP& argument)
			{
				CHECK_FAIL(L"BasicSemanticExtension::BuildGlobalScopePass1(BasicExtendedDeclaration*, const BP&)#不支持此操作。");
			}

			void BasicSemanticExtension::BuildGlobalScopePass2(BasicExtendedDeclaration* declaration, const BP& argument)
			{
				CHECK_FAIL(L"BasicSemanticExtension::BuildGlobalScopePass2(BasicExtendedDeclaration*, const BP&)#不支持此操作。");
			}

			bool BasicSemanticExtension::IsLeftValue(BasicExtendedExpression* expression, const BP& argument)
			{
				CHECK_FAIL(L"BasicSemanticExtension::IsLeftValue(BasicExtendedExpression*, const BP&)#不支持此操作。");
			}

			BasicTypeRecord* BasicSemanticExtension::GetExpressionType(BasicExtendedExpression* expression, const BP& argument)
			{
				CHECK_FAIL(L"BasicSemanticExtension::GetExpressionType(BasicExtendedExpression*, const BP&)#不支持此操作。");
			}

			void BasicSemanticExtension::CheckStatement(BasicExtendedStatement* statement, const BP& argument)
			{
				CHECK_FAIL(L"BasicSemanticExtension::CheckStatement(BasicExtendedStatement*, const BP&)#不支持此操作。");
			}

			void BasicSemanticExtension::BuildDeclarationBody(BasicExtendedDeclaration* declaration, const BP& argument)
			{
				CHECK_FAIL(L"BasicSemanticExtension::BuildDeclarationBody(BasicExtendedDeclaration*, const BP&)#不支持此操作。");
			}

/***********************************************************************
BasicAlgorithmParameter
***********************************************************************/

			BasicAlgorithmParameter::BasicAlgorithmParameter(
				BasicEnv* _env,
				BasicScope* _scope,
				BasicTypeManager* _typeManager,
				BasicTypeInfoManager* _typeInfoManager,
				List<Ptr<BasicLanguageCodeException>>& _errors,
				collections::SortedList<WString>& _forwardStructures
				)
				:env(_env)
				,scope(_scope)
				,typeManager(_typeManager)
				,typeInfoManager(_typeInfoManager)
				,errors(_errors)
				,forwardStructures(_forwardStructures)
				,semanticExtension(&defaultSemanticExtension)
			{
			}

			BasicAlgorithmParameter::BasicAlgorithmParameter(const BasicAlgorithmParameter& bp, BasicScope* _scope)
				:env(bp.env)
				,scope(_scope)
				,typeManager(bp.typeManager)
				,typeInfoManager(bp.typeInfoManager)
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
				,typeInfoManager(bp.typeInfoManager)
				,errors(bp.errors)
				,forwardStructures(bp.forwardStructures)
				,semanticExtension(bp.semanticExtension)
				,configuration(bp.configuration)
			{
			}

/***********************************************************************
BasicTypeInfoManager
***********************************************************************/

			BasicTypeInfo* BasicTypeInfoManager::GetTypeInfo(BasicTypeRecord* type)
			{
				vint index=typeInfos.Keys().IndexOf(type);
				if(index==-1)
				{
					BasicTypeInfo* info=new BasicTypeInfo;
					switch(type->GetType())
					{
					case BasicTypeRecord::Array:
						{
							BasicTypeInfo* element=GetTypeInfo(type->ElementType());
							info->size=element->size*type->ElementCount();
						}
						break;
					case BasicTypeRecord::Function:
					case BasicTypeRecord::Pointer:
						{
							info->size=sizeof(void*);
						}
						break;
					case BasicTypeRecord::Primitive:
						{
							switch(type->PrimitiveType())
							{
							case s8:case u8:case bool_type:case char_type:
								info->size=1;
								break;
							case s16:case u16:case wchar_type:
								info->size=2;
								break;
							case s32:case u32:case f32:
								info->size=4;
								break;
							case s64:case u64:case f64:
								info->size=8;
								break;
							default:
								info->size=1;
							}
						}
						break;
					case BasicTypeRecord::Structure:
						{
							BasicOffset offset=0;
							info->size=0;
							for(vint i=0;i<type->MemberCount();i++)
							{
								BasicTypeInfo* member=GetTypeInfo(type->MemberType(i));
								info->offsets.Add(offset);
								offset+=member->size;
							}
							if(offset==(vint)0)
							{
								info->size=1;
							}
							else
							{
								info->size=offset;
							}
						}
						break;
					case BasicTypeRecord::GenericArgument:
						{
							info->size(type, 1);
						}
						break;
					case BasicTypeRecord::Generic:
						{
							info->size=0;
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

			BasicTypeInfoManager* BasicAnalyzer::GetTypeInfoManager()
			{
				return &tim;
			}

			void BasicAnalyzer::Analyze()
			{
				if(!analyzed)
				{
					analyzed=true;
					BP argument(&env, env.GlobalScope(), &tm, &tim, errors, forwardStructures);
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