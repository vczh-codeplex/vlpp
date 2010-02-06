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
			class BasicSemanticExtension;

			typedef class BasicAlgorithmParameter
			{
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
			} BP;

			class BasicSemanticExtension : public Object, private NotCopyable
			{
			public:
				virtual Ptr<BasicExpression>							ExpressionReplacer(Ptr<BasicExpression> originalExpression, BP& argument);
				virtual Ptr<BasicStatement>								StatementReplacer(Ptr<BasicExpression> originalStatement, BP& argument);
				virtual BasicTypeRecord*								GetTypeRecord(BasicExtendedType* type, const BP& argument);
				virtual void											BuildGlobalScopePass1(BasicExtendedDeclaration* declaration, const BP& argument);
				virtual void											BuildGlobalScopePass2(BasicExtendedDeclaration* declaration, const BP& argument);
				virtual BasicTypeRecord*								GetExpressionType(BasicExtendedExpression* expression, const BP& argument);
			};

/***********************************************************************
Algorithms
***********************************************************************/

			EXTERN_ALGORITHM_FUNCTION(BasicLanguage_GetTypeRecord, BasicType, BP, BasicTypeRecord*)
			EXTERN_ALGORITHM_PROCEDURE(BasicLanguage_BuildGlobalScopePass1, BasicDeclaration, BP)
			EXTERN_ALGORITHM_PROCEDURE(BasicLanguage_BuildGlobalScopePass2, BasicDeclaration, BP)
			extern void BasicLanguage_BuildGlobalScope(Ptr<BasicProgram> program, BP& argument);
			extern BasicTypeRecord* BasicLanguage_GetExpressionType(Ptr<BasicExpression>& expression, BP& argument);
			EXTERN_ALGORITHM_FUNCTION(BasicLanguage_GetExpressionTypeInternal, BasicExpression, BP, BasicTypeRecord*)
		}
	}
}

#endif