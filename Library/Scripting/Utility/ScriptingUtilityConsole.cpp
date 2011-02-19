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

			class SystemCoreConsolePlugin : public LanguagePlugin
			{
			protected:
				void(*reader)(wchar_t*);
				void(*writer)(wchar_t*);
				
				static bool Read(wchar_t* description, void* userData)
				{
					LANGUAGE_PLUGIN(SystemCoreConsolePlugin);
					plugin->reader(description);
					return true;
				}
				
				static bool Write(wchar_t* description, void* userData)
				{
					LANGUAGE_PLUGIN(SystemCoreConsolePlugin);
					plugin->writer(description);
					return true;
				}
			protected:
				bool RegisterForeignFunctions(BasicILRuntimeSymbol* symbol)
				{
					return
						symbol->RegisterLightFunction(L"Console", L"Read", BasicILLightFunction(&LightFunctionMaker<bool(wchar_t*)>::Function2<&Read>, this)) &&
						symbol->RegisterLightFunction(L"Console", L"Write", BasicILLightFunction(&LightFunctionMaker<bool(wchar_t*)>::Function2<&Write>, this));
				}
			public:
				SystemCoreConsolePlugin(void(*_reader)(wchar_t*), void(*_writer)(wchar_t*))
					:reader(_reader)
					,writer(_writer)
				{
				}
			};

			Ptr<LanguagePlugin> CreateConsolePlugin(void(*reader)(wchar_t*), void(*writer)(wchar_t*))
			{
				return new SystemCoreConsolePlugin(reader, writer);
			}
		}
	}
}