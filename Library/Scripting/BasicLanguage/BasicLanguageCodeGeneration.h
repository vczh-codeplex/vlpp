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

namespace vl
{
	namespace scripting
	{
		namespace basiclanguage
		{
			class BasicTypeInfo : public Object
			{
			public:
				int							size;
				int							alignment;
				collections::List<int>		offsets;
			};

			class BasicCodegenInfo : public Object
			{
				typedef collections::Dictionary<BasicTypeRecord*, Ptr<BasicTypeInfo>> _TypeInfoTable;
			protected:
				BasicAnalyzer*				analyzer;
				_TypeInfoTable				typeInfos;

			public:
				BasicCodegenInfo(BasicAnalyzer* _analyzer);

				BasicTypeInfo*				GetTypeInfo(BasicTypeRecord* type);
			};

/***********************************************************************
Algorithms
***********************************************************************/

			EXTERN_ALGORITHM_PROCEDURE(BasicLanguage_PushValue, BasicExpression, BasicCodegenInfo)
			EXTERN_ALGORITHM_PROCEDURE(BasicLanguage_PushRef, BasicExpression, BasicCodegenInfo)
		}
	}
}

#endif