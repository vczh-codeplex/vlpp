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
#include "..\..\Entity\Linear.h"
#include "BasicLanguageScopeManager.h"
#include "BasicLanguageException.h"
#include "BasicLanguageConfiguration.h"

namespace vl
{
	namespace scripting
	{
		namespace basiclanguage
		{

/***********************************************************************
BasicTypeInfo
***********************************************************************/

			typedef Linear<BasicTypeRecord*, vint>									BasicOffset;

			class BasicTypeInfo : public Object
			{
			public:
				BasicOffset															size;
				collections::List<BasicOffset>										offsets;
			};

			class BasicTypeInfoManager : public Object
			{
				typedef collections::Dictionary<BasicTypeRecord*, Ptr<BasicTypeInfo>>			_TypeInfoTable;
			protected:
				_TypeInfoTable														typeInfos;
			public:
				BasicTypeInfo*														GetTypeInfo(BasicTypeRecord* type);
			};

/***********************************************************************
BasicAlgorithmParameter
***********************************************************************/

			class BasicAlgorithmParameter
			{
			public:
				BasicEnv*												env;
				BasicScope*												scope;
				BasicTypeManager*										typeManager;
				BasicTypeInfoManager*									typeInfoManager;
				collections::List<Ptr<BasicLanguageCodeException>>&		errors;
				collections::SortedList<WString>&						forwardStructures;
				BasicAlgorithmConfiguration								configuration;

				BasicAlgorithmParameter(
					BasicEnv* _env,
					BasicScope* _scope,
					BasicTypeManager* _typeManager,
					BasicTypeInfoManager* _typeInfoManager,
					collections::List<Ptr<BasicLanguageCodeException>>& _errors,
					collections::SortedList<WString>& _forwardStructures
					);
				BasicAlgorithmParameter(const BasicAlgorithmParameter& bp, BasicScope* _scope);
				BasicAlgorithmParameter(const BasicAlgorithmParameter& bp);
			};
			typedef BasicAlgorithmParameter BP;

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
			extern BasicTypeRecord* BasicLanguage_GetExpressionType(Ptr<BasicExpression> expression, const BP& argument);
			EXTERN_ALGORITHM_FUNCTION(BasicLanguage_IsLeftValue, BasicExpression, BP, bool)
			EXTERN_ALGORITHM_FUNCTION(BasicLanguage_GetExpressionTypeInternal, BasicExpression, BP, BasicTypeRecord*)

			extern void BasicLanguage_CheckStatement(Ptr<BasicStatement> statement, const BP& argument);
			EXTERN_ALGORITHM_PROCEDURE(BasicLanguage_CheckStatementInternal, BasicStatement, BP);
			EXTERN_ALGORITHM_PROCEDURE(BasicLanguage_BuildDeclarationBody, BasicDeclaration, BP);
			extern void BasicLanguage_BuildDeclarationBody(Ptr<BasicProgram> program, BP& argument);

			EXTERN_ALGORITHM_FUNCTION(BasicLanguage_IsConstantExpression, BasicExpression, BP, bool);
			EXTERN_ALGORITHM_FUNCTION(BasicLanguage_GetConstantValue, BasicExpression, BP, BasicCompileTimeConstant);

/***********************************************************************
BasicAnalyzer
***********************************************************************/

			class BasicAnalyzer : public Object, private NotCopyable
			{
			protected:
				BasicEnv															env;
				BasicTypeManager													tm;
				BasicTypeInfoManager												tim;
				collections::List<Ptr<BasicLanguageCodeException>>					errors;
				collections::SortedList<WString>									forwardStructures;
				BasicAlgorithmConfiguration											configuration;
				Ptr<BasicProgram>													program;
				bool																analyzed;
			public:
				BasicAnalyzer(Ptr<BasicProgram> _program, BasicAlgorithmConfiguration _configuration);
				~BasicAnalyzer();

				BasicEnv*															GetEnv();
				BasicTypeManager*													GetTypeManager();
				const collections::IReadonlyList<Ptr<BasicLanguageCodeException>>&	GetErrors();
				BasicAlgorithmConfiguration&										GetConfiguration();
				BasicTypeInfoManager*												GetTypeInfoManager();
				void																Analyze();
				Ptr<BasicProgram>													GetProgram();
			};
		}
	}
}

#endif