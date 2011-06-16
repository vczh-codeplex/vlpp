/***********************************************************************
Vczh Library++ 3.0
Developer: ³Âè÷å«(vczh)
Scripting::Native X

Classes:
***********************************************************************/

#ifndef VCZH_SCRIPTING_LANGUAGES_NATIVEX_NATIVEX
#define VCZH_SCRIPTING_LANGUAGES_NATIVEX_NATIVEX

#include "..\LanguageProvider.h"

namespace vl
{
	namespace scripting
	{
		// ParseProgram
		// GenerateCode
		class NativeXLanguageExtra : public Object
		{
		public:
			WString							name;
			collections::List<WString>		imports;
		};

		// GenerateHeader
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