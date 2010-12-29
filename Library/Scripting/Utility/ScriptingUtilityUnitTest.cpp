#include "ScriptingUtilityForeignFunctions.h"
#include "..\..\Entity\GeneralObjectPoolEntity.h"

namespace vl
{
	namespace scripting
	{
		namespace utility
		{
			using namespace basicil;
			using namespace entities;
			using namespace collections;

			class SystemCoreUnitTestPlugin : public LanguagePlugin
			{
			protected:
				void(*printer)(bool, wchar_t*);

				class Assert : public Object, public IBasicILForeignFunction
				{
				protected:
					SystemCoreUnitTestPlugin*		plugin;
				public:
					Assert(SystemCoreUnitTestPlugin* _plugin)
						:plugin(_plugin)
					{
					}

					void Invoke(BasicILInterpretor* interpretor, BasicILStack* stack, void* result, void* arguments)
					{
						LanguageArgumentReader reader(result, arguments, stack);
						bool condition=reader.NextArgument<bool>();
						wchar_t* description=reader.NextArgument<wchar_t*>();
					
						plugin->printer(condition, description);
						if(condition)
						{
						}
					}
				};

				class Print : public Object, public IBasicILForeignFunction
				{
				protected:
					SystemCoreUnitTestPlugin*		plugin;
				public:
					Print(SystemCoreUnitTestPlugin* _plugin)
						:plugin(_plugin)
					{
					}

					void Invoke(BasicILInterpretor* interpretor, BasicILStack* stack, void* result, void* arguments)
					{
						LanguageArgumentReader reader(result, arguments, stack);
						wchar_t* description=reader.NextArgument<wchar_t*>();
					
						plugin->printer(true, description);
					}
				};
			protected:
				bool RegisterForeignFunctions(BasicILRuntimeSymbol* symbol)
				{
					return
						symbol->RegisterForeignFunction(L"UnitTest", L"Assert", new Assert(this)) &&
						symbol->RegisterForeignFunction(L"UnitTest", L"Print", new Print(this));
				}
			public:
				SystemCoreUnitTestPlugin(void(*_printer)(bool, wchar_t*))
					:printer(_printer)
				{
				}
			};

			Ptr<LanguagePlugin> CreateUnitTestPlugin(void(*printer)(bool, wchar_t*))
			{
				return new SystemCoreUnitTestPlugin(printer);
			}
		}
	}
}