#include "LanguageMetadata.h"

namespace vl
{
	namespace scripting
	{

/***********************************************************************
LanguageException
***********************************************************************/

		LanguageMetadata::LanguageMetadata(IMetadataProvider* _metadataProvider, const WString& name)
			:metadataProvider(_metadataProvider)
		{
			resources=metadataProvider->GetResource(name);
		}

		IMetadataProvider* LanguageMetadata::GetMetadataProvider()const
		{
			return metadataProvider;
		}

		Ptr<ResourceStream> LanguageMetadata::GetResourceStream()const
		{
			return resources;
		}
	}
}