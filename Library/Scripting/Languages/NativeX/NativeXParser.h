/***********************************************************************
Vczh Library++ 3.0
Developer: ³Âè÷å«(vczh)
Scripting::Native X

Classes:
***********************************************************************/

#ifndef VCZH_SCRIPTING_LANGUAGES_NATIVEX_NATIVEXPARSER
#define VCZH_SCRIPTING_LANGUAGES_NATIVEX_NATIVEXPARSER

#include "NativeX.h"
#include "..\..\BasicLanguage\BasicLanguageExpression.h"

namespace vl
{
	namespace scripting
	{
		namespace language_nativex
		{
			using namespace collections;
			using namespace basiclanguage;

			class NativeXUnit : public Object
			{
			public:
				WString							name;
				List<WString>					imports;
				Ptr<BasicProgram>				program;
				vint							codeIndex;
			};

			class NativeXParser : public Object
			{
			public:
				static Ptr<NativeXParser>		Create();

				virtual Ptr<NativeXUnit>		Parse(const WString& code, vint codeIndex, IList<Ptr<LanguageException>>& errors)=0;
			};
		}
	}
}

#endif