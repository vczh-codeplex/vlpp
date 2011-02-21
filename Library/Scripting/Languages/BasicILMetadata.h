/***********************************************************************
Vczh Library++ 3.0
Developer: ³Âè÷å«(vczh)
Scripting::Language Provider

Classes:
***********************************************************************/

#ifndef VCZH_SCRIPTING_LANGUAGES_BASICILMETADATA
#define VCZH_SCRIPTING_LANGUAGES_BASICILMETADATA

#include "LanguageMetadata.h"
#include "..\BasicIL\BasicILSymbolResource.h"

namespace vl
{
	namespace scripting
	{
		class BasicILMetadata;

/***********************************************************************
Metadata
***********************************************************************/

		class BasicILExportInfo : public Object
		{
			friend class BasicILMetadata;
		protected:
			const BasicILMetadata*					metadata;
			ResourceRecord<BasicILExportRes>		exportInfo;
			
			BasicILExportInfo(ResourceHandle<BasicILExportRes> _exportInfo, const BasicILMetadata* _metadata);
		public:
			BasicILExportInfo();
			operator bool()const;

			bool									IsVariable()const;
			bool									IsFunction()const;
			WString									GetName()const;
			vint									GetInstructionIndex()const;
		};

/***********************************************************************
BasicILMetadata
***********************************************************************/

		class BasicILMetadata : public LanguageMetadata
		{
		protected:
			ResourceRecord<BasicILEntryRes>			entry;
		public:
			BasicILMetadata(IMetadataProvider* metadataProvider);

			vint									GetExportCount()const;
			BasicILExportInfo						GetExport(vint index)const;
		};
	}
}

#endif