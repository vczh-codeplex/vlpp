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
				virtual WString					NamespaceToString(const collections::IReadonlyList<WString>& fragments)=0;
				virtual WString					SystemTypeToString(const WString& name)=0;
			public:
				Ptr<LanguageException>			Translate(Ptr<ManagedLanguageCodeException> error, vint codeIndex);
			};
		}
	}
}

#endif