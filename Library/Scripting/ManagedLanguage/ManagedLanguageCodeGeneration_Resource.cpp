#include "ManagedLanguageCodeGeneration.h"

namespace vl
{
	namespace scripting
	{
		namespace managedlanguage
		{

/***********************************************************************
Symbol to Metadata
***********************************************************************/

			namespace resource_transformation
			{
			};
			using namespace resource_transformation;

			ResourceHandle<ManagedEntryRes> ManagedLanguage_GenerateResource(Ptr<ResourceStream> resourceStream, ManagedSymbolManager* symbolManager)
			{
				ResourceRecord<ManagedEntryRes> entryRes=resourceStream->CreateRecord<ManagedEntryRes>();
				return entryRes;
			}

/***********************************************************************
Metadata to Symbol
***********************************************************************/

			namespace resource_transformation
			{
			};
			using namespace resource_transformation;

			void ManagedLanguage_ImportSymbols(Ptr<ResourceStream> resourceStream, ManagedSymbolManager* symbolManager, const WString& assemblyName)
			{
			}
		}
	}
}