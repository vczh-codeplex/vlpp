#include "BasicILMetadata.h"
#include "..\BasicIL\BasicILDefinition.h"

namespace vl
{
	namespace scripting
	{
		using namespace basicil;

/***********************************************************************
BasicILExportInfo
***********************************************************************/

		BasicILExportInfo::BasicILExportInfo(ResourceHandle<BasicILExportRes> _exportInfo, const BasicILMetadata* _metadata)
			:metadata(_metadata)
			,exportInfo(metadata->GetResourceStream()->ReadRecord(_exportInfo))
		{
		}

		BasicILExportInfo::BasicILExportInfo()
			:metadata(0)
		{
		}

		BasicILExportInfo::operator bool()const
		{
			return exportInfo;
		}

		bool BasicILExportInfo::IsVariable()const
		{
			return exportInfo->type==BasicILExportRes::Variable;
		}

		bool BasicILExportInfo::IsFunction()const
		{
			return exportInfo->type==BasicILExportRes::Function;
		}

		WString BasicILExportInfo::GetName()const
		{
			return metadata->GetResourceStream()->ReadString(exportInfo->name);
		}

		vint BasicILExportInfo::GetInstructionIndex()const
		{
			return exportInfo->address;
		}

/***********************************************************************
BasicILMetadata
***********************************************************************/

		BasicILMetadata::BasicILMetadata(IMetadataProvider* metadataProvider)
			:LanguageMetadata(metadataProvider, basicil::BasicILResourceNames::ExportedSymbols)
		{
			if(resources)
			{
				entry=resources->ReadRootRecord<BasicILEntryRes>();
			}
		}

		vint BasicILMetadata::GetExportCount()const
		{
			if(entry->exports)
			{
				ResourceArrayRecord<BasicILExportRes> exports=GetResourceStream()->ReadArrayRecord(entry->exports);
				return exports.Count();
			}
			else
			{
				return 0;
			}
		}

		BasicILExportInfo BasicILMetadata::GetExport(vint index)const
		{
			ResourceArrayRecord<BasicILExportRes> exports=GetResourceStream()->ReadArrayRecord(entry->exports);
			return BasicILExportInfo(exports.Get(index), this);
		}
	}
}