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

			class SystemCoreMemoryManagerPlugin : public LanguagePlugin
			{
			protected:
				List<Ptr<GeneralObjectPool>>			pools;

				class MemCreate : public Object, public IBasicILForeignFunction
				{
				protected:
					SystemCoreMemoryManagerPlugin*		plugin;
				public:
					MemCreate(SystemCoreMemoryManagerPlugin* _plugin)
						:plugin(_plugin)
					{
					}

					void Invoke(BasicILInterpretor* interpretor, BasicILStack* stack, void* result, void* arguments)
					{
						LanguageArgumentReader reader(result, arguments, stack);
						Ptr<GeneralObjectPool> pool=new GeneralObjectPool(1048576, 256);
						plugin->pools.Add(pool);
						reader.Result<GeneralObjectPool*>()=pool.Obj();
					}
				};

				static vint MemFree(void* result, void* arguments)
				{
					LanguageArgumentReader reader(result, arguments);
					GeneralObjectPool* pool=reader.NextArgument<GeneralObjectPool*>();
					vint size=reader.NextArgument<vint>();

					reader.Result<char*>()=pool->Alloc(size);
					return reader.BytesToPop();
				}

				static vint MemIsValidHandle(void* result, void* arguments)
				{
					LanguageArgumentReader reader(result, arguments);
					GeneralObjectPool* pool=reader.NextArgument<GeneralObjectPool*>();
					char* pointer=reader.NextArgument<char*>();

					reader.Result<bool>()=pool->IsValid(pointer);
					return reader.BytesToPop();
				}

				static vint MemGetHandleSize(void* result, void* arguments)
				{
					LanguageArgumentReader reader(result, arguments);
					GeneralObjectPool* pool=reader.NextArgument<GeneralObjectPool*>();
					char* pointer=reader.NextArgument<char*>();

					reader.Result<vint>()=pool->GetSize(pointer);
					return reader.BytesToPop();
				}

				static vint MemGetOwnerHandle(void* result, void* arguments)
				{
					LanguageArgumentReader reader(result, arguments);
					GeneralObjectPool* pool=reader.NextArgument<GeneralObjectPool*>();
					char* pointer=reader.NextArgument<char*>();

					reader.Result<char*>()=pool->GetHandle(pointer);
					return reader.BytesToPop();
				}
			protected:
				bool RegisterForeignFunctions(BasicILRuntimeSymbol* symbol)
				{
					return
						symbol->RegisterForeignFunction(L"SystemCoreForeignFunctions", L"MemCreate", new MemCreate(this)) &&
						symbol->RegisterLightFunction(L"SystemCoreForeignFunctions", L"MemFree", MemFree) &&
						symbol->RegisterLightFunction(L"SystemCoreForeignFunctions", L"MemIsValidHandle", MemIsValidHandle) &&
						symbol->RegisterLightFunction(L"SystemCoreForeignFunctions", L"MemGetHandleSize", MemGetHandleSize) &&
						symbol->RegisterLightFunction(L"SystemCoreForeignFunctions", L"MemGetOwnerHandle", MemGetOwnerHandle);
				}
			};

			Ptr<LanguagePlugin> CreateMemoryManagerPlugin()
			{
				return new SystemCoreMemoryManagerPlugin();
			}
		}
	}
}