/***********************************************************************
Vczh Library++ 3.0
Developer: 陈梓瀚(vczh)
Scripting::BasicLanguage

Classes:
	BasicScope									：对象定义域
	BasicEnv									：对象全局环境
***********************************************************************/

#ifndef VCZH_SCRIPTING_BASICLANGUAGE_BASICLANGUAGEANALYZER
#define VCZH_SCRIPTING_BASICLANGUAGE_BASICLANGUAGEANALYZER

#include "..\..\Exception.h"
#include "BasicLanguageScopeManager.h"
#include "BasicLanguageException.h"
#include "BasicLanguageConfiguration.h"

namespace vl
{
	namespace scripting
	{
		namespace basiclanguage
		{
			class BasicAlgorithmParameter;
			typedef BasicAlgorithmParameter BP;

			class BasicSemanticExtension : public Object, private NotCopyable
			{
			public:
				virtual Ptr<BasicExpression>							ReplaceExpression(Ptr<BasicExpression> originalExpression, const BP& argument);
				virtual Ptr<BasicStatement>								ReplaceStatement(Ptr<BasicStatement> originalStatement, const BP& argument);
				virtual BasicTypeRecord*								GetTypeRecord(BasicExtendedType* type, const BP& argument);
				virtual void											BuildGlobalScopePass1(BasicExtendedDeclaration* declaration, const BP& argument);
				virtual void											BuildGlobalScopePass2(BasicExtendedDeclaration* declaration, const BP& argument);
				virtual bool											IsLeftValue(BasicExtendedExpression* expression, const BP& argument);
				virtual BasicTypeRecord*								GetExpressionType(BasicExtendedExpression* expression, const BP& argument);
				virtual void											CheckStatement(BasicExtendedStatement* statement, const BP& argument);
				virtual void											BuildDeclarationBody(BasicExtendedDeclaration* declaration, const BP& argument);
			};

			class BasicAlgorithmParameter
			{
			private:
				BasicSemanticExtension									defaultSemanticExtension;
			public:
				BasicEnv*												env;
				BasicScope*												scope;
				BasicTypeManager*										typeManager;
				collections::List<Ptr<BasicLanguageCodeException>>&		errors;
				collections::SortedList<WString>&						forwardStructures;
				BasicSemanticExtension*									semanticExtension;
				BasicAlgorithmConfiguration								configuration;

				BasicAlgorithmParameter(
					BasicEnv* _env,
					BasicScope* _scope,
					BasicTypeManager* _typeManager,
					collections::List<Ptr<BasicLanguageCodeException>>& _errors,
					collections::SortedList<WString>& _forwardStructures
					);
				BasicAlgorithmParameter(const BasicAlgorithmParameter& bp, BasicScope* _scope);
				BasicAlgorithmParameter(const BasicAlgorithmParameter& bp);
			};

/***********************************************************************
Algorithms
***********************************************************************/
			
			extern void CheckRequiredInstance(BasicTypeRecord* type, BasicType* typeExpression, const WString& conceptName, const BP& argument);
			extern BasicTypeRecord* BasicLanguage_GetTypeRecord(BasicType* type, const BP& argument, bool acceptRawGenericType);
			extern BasicTypeRecord* BasicLanguage_GetTypeRecord(Ptr<BasicType> type, const BP& argument, bool acceptRawGenericType);
			extern bool IsPublicType(BasicTypeRecord* type, const BP& argument);

			EXTERN_ALGORITHM_PROCEDURE(BasicLanguage_BuildGlobalScopePass1, BasicDeclaration, BP)
			EXTERN_ALGORITHM_PROCEDURE(BasicLanguage_BuildGlobalScopePass2, BasicDeclaration, BP)
			extern void BasicLanguage_BuildGlobalScope(Ptr<BasicProgram> program, BP& argument);

			extern bool CanImplicitConvertTo(BasicTypeRecord* from, BasicTypeRecord* to, BasicExpression* fromExpression, const BP& argument);
			extern BasicTypeRecord* BasicLanguage_GetExpressionType(Ptr<BasicExpression>& expression, const BP& argument);
			EXTERN_ALGORITHM_FUNCTION(BasicLanguage_IsLeftValue, BasicExpression, BP, bool)
			EXTERN_ALGORITHM_FUNCTION(BasicLanguage_GetExpressionTypeInternal, BasicExpression, BP, BasicTypeRecord*)

			extern void BasicLanguage_CheckStatement(Ptr<BasicStatement>& statement, const BP& argument);
			EXTERN_ALGORITHM_PROCEDURE(BasicLanguage_CheckStatementInternal, BasicStatement, BP);
			EXTERN_ALGORITHM_PROCEDURE(BasicLanguage_BuildDeclarationBody, BasicDeclaration, BP);
			extern void BasicLanguage_BuildDeclarationBody(Ptr<BasicProgram> program, BP& argument);

			EXTERN_ALGORITHM_FUNCTION(BasicLanguage_IsConstantExpression, BasicExpression, BP, bool);
			EXTERN_ALGORITHM_FUNCTION(BasicLanguage_GetConstantValue, BasicExpression, BP, Ptr<BasicNumericExpression>);

/***********************************************************************
BasicAnalyzer
***********************************************************************/

			class BasicAnalyzer : public Object, private NotCopyable
			{
			protected:
				BasicEnv												env;
				BasicTypeManager										tm;
				collections::List<Ptr<BasicLanguageCodeException>>		errors;
				collections::SortedList<WString>						forwardStructures;
				BasicSemanticExtension*									semanticExtension;
				BasicAlgorithmConfiguration								configuration;
				Ptr<BasicProgram>										program;
				bool													analyzed;
			public:
				BasicAnalyzer(Ptr<BasicProgram> _program, BasicSemanticExtension* _semanticExtension, BasicAlgorithmConfiguration _configuration);
				~BasicAnalyzer();

				BasicEnv*															GetEnv();
				BasicTypeManager*													GetTypeManager();
				const collections::IReadonlyList<Ptr<BasicLanguageCodeException>>&	GetErrors();
				BasicAlgorithmConfiguration&										GetConfiguration();
				void																Analyze();
				Ptr<BasicProgram>													GetProgram();
			};
		}
	}
}

#endif