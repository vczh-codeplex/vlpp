/***********************************************************************
Vczh Library++ 3.0
Developer: ³Âè÷å«(vczh)
Scripting::Language Provider

Classes:
***********************************************************************/

#ifndef VCZH_SCRIPTING_LANGUAGES_LANGUAGEMETADATA
#define VCZH_SCRIPTING_LANGUAGES_LANGUAGEMETADATA

#include "..\BasicIL\BasicILResourceStream.h"
#include "..\..\Pointer.h"

namespace vl
{
	namespace scripting
	{
		class IMetadataProvider : private Interface
		{
		public:
			virtual vint					GetInstructionKey()=0;
			virtual Ptr<ResourceStream>		GetResource(const WString& name)=0;
		};

		class LanguageMetadata : public Object
		{
		protected:
			IMetadataProvider*				metadataProvider;
			Ptr<ResourceStream>				resources;

			LanguageMetadata(IMetadataProvider* _metadataProvider, const WString& name);
		public:
			
			IMetadataProvider*				GetMetadataProvider()const;
			Ptr<ResourceStream>				GetResourceStream()const;
		};
	}
}

#endif