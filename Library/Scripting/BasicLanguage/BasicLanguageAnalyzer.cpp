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