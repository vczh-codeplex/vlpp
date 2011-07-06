/***********************************************************************
Vczh Library++ 3.0
Developer: ³Âè÷å«(vczh)
Scripting::Language Provider

Classes:
***********************************************************************/

#ifndef VCZH_SCRIPTING_LANGUAGES_MANAGEDERRORMESSAGETRANSLATOR
#define VCZH_SCRIPTING_LANGUAGES_MANAGEDERRORMESSAGETRANSLATOR

#include "LanguageProvider.h"
#include "..\ManagedLanguage\ManagedLanguageException.h"

namespace vl
{
	namespace scripting
	{
		namespace managedlanguage
		{
			class ManagedErrorMessageTranslator : public Object
			{
			protected:
				virtual WString					SymbolsToString(const collections::IReadonlyList<WString>& symbols)=0;
				virtual WString					TypeToString(ManagedType* type)=0;
			public:
				Ptr<LanguageException>			Translate(Ptr<ManagedLanguageCodeException> error, vint codeIndex);
			};
		}
	}
}

#endif