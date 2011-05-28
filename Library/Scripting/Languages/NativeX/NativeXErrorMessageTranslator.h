/***********************************************************************
Vczh Library++ 3.0
Developer: ³Âè÷å«(vczh)
Scripting::Native X

Classes:
***********************************************************************/

#ifndef VCZH_SCRIPTING_LANGUAGES_NATIVEX_NATIVEXERRORMESSAGETRANSLATOR
#define VCZH_SCRIPTING_LANGUAGES_NATIVEX_NATIVEXERRORMESSAGETRANSLATOR

#include "..\BasicErrorMessageTranslator.h"
#include "..\..\BasicLanguage\BasicLanguageAnalyzer.h"

namespace vl
{
	namespace scripting
	{
		namespace language_nativex
		{
			using namespace collections;
			using namespace basiclanguage;

			extern Ptr<BasicErrorMessageTranslator>		CreateNativeXErrorMessageTranslator(BasicAnalyzer* analyzer);
		}
	}
}

#endif