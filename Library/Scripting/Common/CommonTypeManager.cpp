#include "CommonTypeManager.h"

namespace vl
{
	namespace scripting
	{

/***********************************************************************
CommonTypeManager
***********************************************************************/

		void CommonTypeManager::RegisterTypeRecord(CommonTypeRecord* typeRecord)
		{
			allocatedTypeRecords.Add(typeRecord);
		}
	}
}