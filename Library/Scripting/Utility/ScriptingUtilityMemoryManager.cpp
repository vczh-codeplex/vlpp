#include "ScriptingUtilityForeignFunctions.h"
#include "..\..\Entity\SmallObjectPoolEntity.h"

namespace vl
{
	namespace scripting
	{
		namespace utility
		{
			using namespace entities;

			Ptr<LanguagePlugin> CreateMemoryManagerPlugin()
			{
				return 0;
			}
		}
	}
}