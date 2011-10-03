/***********************************************************************
Vczh Library++ 3.0
Developer: ³Âè÷å«(vczh)
Scripting::ManagedLanguage

Classes:
***********************************************************************/

#ifndef VCZH_SCRIPTING_MANAGEDLANGUAGE_MANAGEDLANGUAGECODEGENERATION
#define VCZH_SCRIPTING_MANAGEDLANGUAGE_MANAGEDLANGUAGECODEGENERATION

#include "ManagedLanguageAnalyzer.h"
#include "ManagedLanguageResource.h"

namespace vl
{
	namespace scripting
	{
		namespace managedlanguage
		{
			extern ResourceHandle<ManagedEntryRes>		ManagedLanguage_GenerateResource(Ptr<ResourceStream> resourceStream, ManagedSymbolManager* symbolManager);
			extern bool									ManagedLanguage_ImportSymbols(Ptr<ResourceStream> resourceStream, ManagedSymbolManager* symbolManager, const WString& assemblyName, collections::List<ResourceHandle<ManagedSymbolItemRes>>& unresolvedTypes);
		}
	}
}

#endif