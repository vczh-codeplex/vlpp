/***********************************************************************
Vczh Library++ 3.0
Developer: �����(vczh)
Scripting::Native X

Classes:
***********************************************************************/

#ifndef VCZH_SCRIPTING_LANGUAGES_NATIVEX_NATIVEXPARSER
#define VCZH_SCRIPTING_LANGUAGES_NATIVEX_NATIVEXPARSER

#include "..\LanguageProvider.h"

namespace vl
{
	namespace scripting
	{
		namespace language_nativex
		{
			class NativeXMessageTranslator : public Object
			{
			};

			class NativeXChineseMessageTranslator : public NativeXMessageTranslator
			{
			};

			class NativeXEnglishMessageTranslator : public NativeXMessageTranslator
			{
			};

			extern Ptr<ILanguageProvider>		CreateNativeXLanguageProvider(NativeXMessageTranslator* messageTranslator);
		}
	}
}

#endif