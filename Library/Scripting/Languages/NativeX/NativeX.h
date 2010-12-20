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
		class NativeXLanguageExtra : public Object
		{
		public:
			WString							name;
			collections::List<WString>		imports;
		};

		class NativeXHeaderExtra : public Object
		{
		public:
			bool							publicOnly;
			bool							currentAssemblyOnly;
			WString							declarationPrefix;

			bool							importReferencedAssemblies;
			WString							assemblyPrefix;

			NativeXHeaderExtra();
		};

		extern Ptr<ILanguageProvider>		CreateNativeXLanguageProvider();
	}
}

#endif