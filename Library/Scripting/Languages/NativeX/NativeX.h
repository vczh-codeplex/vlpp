/***********************************************************************
Vczh Library++ 3.0
Developer: ³Âè÷å«(vczh)
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
		class NativeXLanguageExtra : public Object
		{
		public:
			WString							name;
			collections::List<WString>		imports;
		};

		extern Ptr<ILanguageProvider>		CreateNativeXLanguageProvider();
	}
}

#endif