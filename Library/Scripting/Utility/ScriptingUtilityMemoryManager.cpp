#include "ScriptingUtilityForeignFunctions.h"
#include "..\Languages\LanguageRuntime.h"
#include "..\..\Entity\GeneralObjectPoolEntity.h"
#include "..\..\Threading.h"

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
			public:
				struct PoolPackage
				{
					GeneralObjectPool					pool;
					CriticalSection						cs;

					PoolPackage(vint poolUnitSize, vint poolUnitCount)
						:pool(poolUnitSize, poolUnitCount)
					{
					}
				};

				List<Ptr<PoolPackage>>					pools;
				CriticalSection							pluginCs;

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
						Ptr<PoolPackage> pool=new PoolPackage(1048576, 256);

						CriticalSection::Scope scope(plugin->pluginCs);
						plugin->pools.Add(pool);
						reader.Result<PoolPackage*>()=pool.Obj();
					}
				};

				static vint MemAlloc(void* result, void* arguments)
				{
					LanguageArgumentReader reader(result, arguments);
					PoolPackage* pool=reader.NextArgument<PoolPackage*>();
					vint size=reader.NextArgument<vint>();

					CriticalSection::Scope scope(pool->cs);
					reader.Result<char*>()=pool->pool.Alloc(size);
					return reader.BytesToPop();
				}

				static vint MemFree(void* result, void* arguments)
				{
					LanguageArgumentReader reader(result, arguments);
					PoolPackage* pool=reader.NextArgument<PoolPackage*>();
					char* pointer=reader.NextArgument<char*>();
					
					CriticalSection::Scope scope(pool->cs);
					reader.Result<bool>()=pool->pool.Free(pointer);
					return reader.BytesToPop();
				}

				static vint MemIsValidHandle(void* result, void* arguments)
				{
					LanguageArgumentReader reader(result, arguments);
					PoolPackage* pool=reader.NextArgument<PoolPackage*>();
					char* pointer=reader.NextArgument<char*>();
					
					CriticalSection::Scope scope(pool->cs);
					reader.Result<bool>()=pool->pool.IsValid(pointer);
					return reader.BytesToPop();
				}

				static vint MemGetHandleSize(void* result, void* arguments)
				{
					LanguageArgumentReader reader(result, arguments);
					PoolPackage* pool=reader.NextArgument<PoolPackage*>();
					char* pointer=reader.NextArgument<char*>();
					
					CriticalSection::Scope scope(pool->cs);
					reader.Result<vint>()=pool->pool.GetSize(pointer);
					return reader.BytesToPop();
				}

				static vint MemGetOwnerHandle(void* result, void* arguments)
				{
					LanguageArgumentReader reader(result, arguments);
					PoolPackage* pool=reader.NextArgument<PoolPackage*>();
					char* pointer=reader.NextArgument<char*>();
					
					CriticalSection::Scope scope(pool->cs);
					reader.Result<char*>()=pool->pool.GetHandle(pointer);
					return reader.BytesToPop();
				}
			protected:
				bool RegisterForeignFunctions(BasicILRuntimeSymbol* symbol)
				{
					return
						symbol->RegisterForeignFunction(L"SystemCoreForeignFunctions", L"MemCreate", new MemCreate(this)) &&
						symbol->RegisterLightFunction(L"SystemCoreForeignFunctions", L"MemAlloc", MemAlloc) &&
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