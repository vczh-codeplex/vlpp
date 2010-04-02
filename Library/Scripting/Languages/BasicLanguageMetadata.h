/***********************************************************************
Vczh Library++ 3.0
Developer: ³Âè÷å«(vczh)
Scripting::Language Provider

Classes:
***********************************************************************/

#ifndef VCZH_SCRIPTING_LANGUAGES_BASICLANGUAGEMETADATA
#define VCZH_SCRIPTING_LANGUAGES_BASICLANGUAGEMETADATA

#include "LanguageMetadata.h"
#include "..\BasicLanguage\BasicLanguageResource.h"

namespace vl
{
	namespace scripting
	{
		class BasicLanguageMetadata : public LanguageMetadata
		{
		public:
			BasicLanguageMetadata(IMetadataProvider* metadataProvider);
		};
	}
}

#endif