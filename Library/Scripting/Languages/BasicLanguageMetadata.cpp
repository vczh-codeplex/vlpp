#include "BasicLanguageMetadata.h"
#include "..\BasicIL\BasicILDefinition.h"

namespace vl
{
	namespace scripting
	{

/***********************************************************************
LanguageAssembly
***********************************************************************/

		BasicLanguageMetadata::BasicLanguageMetadata(IMetadataProvider* metadataProvider)
			:LanguageMetadata(metadataProvider, basicil::BasicILResourceNames::BasicLanguageInterfaces)
		{
		}
	}
}