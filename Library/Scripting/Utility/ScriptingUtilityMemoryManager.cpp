#include "ScriptingUtilityForeignFunctions.h"
#include "..\..\Entity\SmallObjectPoolEntity.h"

namespace vl
{
	namespace scripting
	{
		namespace utility
		{
			using namespace basicil;
			using namespace entities;

			class SystemCoreMemoryManagerPlugin : public LanguagePlugin
			{
			protected:
				bool RegisterForeignFunctions(BasicILRuntimeSymbol* symbol)
				{
					return true;
				}
			};

			Ptr<LanguagePlugin> CreateMemoryManagerPlugin()
			{
				return new SystemCoreMemoryManagerPlugin();
			}
		}
	}
}