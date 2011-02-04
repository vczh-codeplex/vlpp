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

				static PoolPackage* MemCreate(void* userData)
				{
					LANGUAGE_PLUGIN(SystemCoreMemoryManagerPlugin);
					Ptr<PoolPackage> pool=new PoolPackage(1048576, 256);
					CriticalSection::Scope scope(plugin->pluginCs);
					plugin->pools.Add(pool);
					return pool.Obj();
				}

				static char* MemAlloc(PoolPackage* pool, vint size)
				{
					CriticalSection::Scope scope(pool->cs);
					return pool->pool.Alloc(size);
				}

				static bool MemFree(PoolPackage* pool, char* pointer)
				{
					CriticalSection::Scope scope(pool->cs);
					return pool->pool.Free(pointer);
				}

				static bool MemIsValidHandle(PoolPackage* pool, char* pointer)
				{
					CriticalSection::Scope scope(pool->cs);
					return pool->pool.IsValid(pointer);
				}

				static vint MemGetHandleSize(PoolPackage* pool, char* pointer)
				{
					CriticalSection::Scope scope(pool->cs);
					return pool->pool.GetSize(pointer);
				}

				static char* MemGetOwnerHandle(PoolPackage* pool, char* pointer)
				{
					CriticalSection::Scope scope(pool->cs);
					return pool->pool.GetHandle(pointer);
				}
			protected:
				bool RegisterForeignFunctions(BasicILRuntimeSymbol* symbol)
				{
					return
						REGISTER_LIGHT_FUNCTION2(MemCreate, PoolPackage*(), MemCreate) &&
						REGISTER_LIGHT_FUNCTION(MemAlloc, char*(PoolPackage*, vint), MemAlloc) &&
						REGISTER_LIGHT_FUNCTION(MemFree, bool(PoolPackage*, char*), MemFree) &&
						REGISTER_LIGHT_FUNCTION(MemIsValidHandle, bool(PoolPackage*, char*), MemIsValidHandle) &&
						REGISTER_LIGHT_FUNCTION(MemGetHandleSize, vint(PoolPackage*, char*), MemGetHandleSize) &&
						REGISTER_LIGHT_FUNCTION(MemGetOwnerHandle, char*(PoolPackage*, char*), MemGetOwnerHandle);
				}
			};

			Ptr<LanguagePlugin> CreateMemoryManagerPlugin()
			{
				return new SystemCoreMemoryManagerPlugin();
			}
		}
	}
}