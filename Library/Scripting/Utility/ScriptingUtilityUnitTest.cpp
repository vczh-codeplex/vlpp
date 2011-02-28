#include "ScriptingUtilityForeignFunctions.h"
#include "..\Languages\LanguageRuntime.h"

namespace vl
{
	namespace scripting
	{
		namespace utility
		{
			using namespace basicil;
			using namespace collections;

			class SystemCoreUnitTestPlugin : public LanguagePlugin
			{
			protected:
				void(__stdcall*printer)(bool, wchar_t*);

				static bool Assert(bool condition, wchar_t* description, void* userData)
				{
					LANGUAGE_PLUGIN(SystemCoreUnitTestPlugin);
					plugin->printer(condition, description);
					return true;
				}
				
				static bool Print(wchar_t* description, void* userData)
				{
					LANGUAGE_PLUGIN(SystemCoreUnitTestPlugin);
					plugin->printer(true, description);
					return true;
				}
			protected:
				bool RegisterForeignFunctions(BasicILRuntimeSymbol* symbol)
				{
					return
						symbol->RegisterLightFunction(L"UnitTest", L"Assert", BasicILLightFunction(&LightFunctionMaker<bool(bool, wchar_t*)>::Function2<&Assert>, this)) &&
						symbol->RegisterLightFunction(L"UnitTest", L"Print", BasicILLightFunction(&LightFunctionMaker<bool(wchar_t*)>::Function2<&Print>, this));
				}
			public:
				SystemCoreUnitTestPlugin(void(__stdcall*_printer)(bool, wchar_t*))
					:printer(_printer)
				{
				}
			};

			Ptr<LanguagePlugin> CreateUnitTestPlugin(void(__stdcall*printer)(bool, wchar_t*))
			{
				return new SystemCoreUnitTestPlugin(printer);
			}
		}
	}
}