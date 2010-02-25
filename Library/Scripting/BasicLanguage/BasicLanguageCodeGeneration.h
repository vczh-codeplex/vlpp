/***********************************************************************
Vczh Library++ 3.0
Developer: ³Âè÷å«(vczh)
Scripting::BasicLanguage

Classes:
***********************************************************************/

#ifndef VCZH_SCRIPTING_BASICLANGUAGE_BASICLANGUAGECODEGENERATION
#define VCZH_SCRIPTING_BASICLANGUAGE_BASICLANGUAGECODEGENERATION

#include "BasicLanguageAnalyzer.h"
#include "..\BasicIL\BasicILDefinition.h"
#include "..\..\Stream\MemoryStream.h"

namespace vl
{
	namespace scripting
	{
		namespace basiclanguage
		{
			class BasicTypeInfo : public Object
			{
			public:
				int												size;
				int												alignment;
				collections::List<int>							offsets;
			};

			class BasicCodegenInfo : public Object
			{
				typedef collections::Dictionary<BasicTypeRecord*, Ptr<BasicTypeInfo>> _TypeInfoTable;
			protected:
				BasicAnalyzer*									analyzer;
				_TypeInfoTable									typeInfos;

			public:
				BasicCodegenInfo(BasicAnalyzer* _analyzer);

				BasicTypeInfo*									GetTypeInfo(BasicTypeRecord* type);
				BasicEnv*										GetEnv();
				BasicTypeManager*								GetTypeManager();
				BasicAlgorithmConfiguration&					GetConfiguration();
			};

/***********************************************************************
Algorithms
***********************************************************************/

			struct BasicCodegenParameter;
			typedef BasicCodegenParameter BCP;

			class BasicCodegenExtension : public Object, private NotCopyable
			{
			public:
				virtual BasicTypeRecord*						PushValue(BasicExtendedExpression* expression, const BCP& argument);
				virtual void									PushRef(BasicExtendedExpression* expression, const BCP& argument);
				virtual void									GenerateCode(BasicExtendedStatement* statement, const BCP& argument);
			};

			struct BasicCodegenParameter
			{
			private:
				BasicCodegenExtension							defaultCodegenExtension;
			public:
				BasicCodegenInfo*								info;
				basicil::BasicIL*								il;
				stream::MemoryStream*							globalData;
				BasicCodegenExtension*							codegenExtension;
				collections::List<BasicFunctionDeclaration*>&	usedFunctions;

				BasicCodegenParameter(BasicCodegenInfo* _info, basicil::BasicIL* _il, stream::MemoryStream* _globalData, collections::List<BasicFunctionDeclaration*>& _usedFunctions);
				BasicCodegenParameter(const BasicCodegenParameter& parameter);
			};

			extern BasicTypeRecord* BasicLanguage_PushValue(Ptr<BasicExpression> expression, const BCP& argument);
			extern BasicTypeRecord* BasicLanguage_PushValue(BasicExpression* expression, const BCP& argument);
			extern BasicTypeRecord* BasicLanguage_PushValue(Ptr<BasicExpression> expression, const BCP& argument, BasicTypeRecord* expectedType);
			extern BasicTypeRecord* BasicLanguage_PushValue(BasicExpression* expression, const BCP& argument, BasicTypeRecord* expectedType);
			EXTERN_ALGORITHM_FUNCTION(BasicLanguage_PushValueInternal, BasicExpression, BCP, BasicTypeRecord*)
			EXTERN_ALGORITHM_PROCEDURE(BasicLanguage_PushRef, BasicExpression, BCP)
			EXTERN_ALGORITHM_PROCEDURE(BasicLanguage_GenerateCode, BasicStatement, BCP)
		}
	}
}

#endif