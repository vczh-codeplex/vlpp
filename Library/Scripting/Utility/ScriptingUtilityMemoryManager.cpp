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
					SpinLock							lock;

					PoolPackage(vint poolUnitSize, vint poolUnitCount)
						:pool(poolUnitSize, poolUnitCount)
					{
					}
				};

				PoolPackage								pool;

				SystemCoreMemoryManagerPlugin()
					:pool(1048576, 256)
				{
				}

				static char* MemAlloc(vint size, void* userData)
				{
					LANGUAGE_PLUGIN(SystemCoreMemoryManagerPlugin);
					PoolPackage* pool=&plugin->pool;
					SpinLock::Scope scope(pool->lock);
					return pool->pool.Alloc(size);
				}

				static bool MemFree(char* pointer, void* userData)
				{
					LANGUAGE_PLUGIN(SystemCoreMemoryManagerPlugin);
					PoolPackage* pool=&plugin->pool;
					SpinLock::Scope scope(pool->lock);
					return pool->pool.Free(pointer);
				}

				static bool MemIsValidHandle(char* pointer, void* userData)
				{
					LANGUAGE_PLUGIN(SystemCoreMemoryManagerPlugin);
					PoolPackage* pool=&plugin->pool;
					SpinLock::Scope scope(pool->lock);
					return pool->pool.IsValid(pointer);
				}

				static vint MemGetHandleSize(char* pointer, void* userData)
				{
					LANGUAGE_PLUGIN(SystemCoreMemoryManagerPlugin);
					PoolPackage* pool=&plugin->pool;
					SpinLock::Scope scope(pool->lock);
					return pool->pool.GetSize(pointer);
				}

				static char* MemGetOwnerHandle(char* pointer, void* userData)
				{
					LANGUAGE_PLUGIN(SystemCoreMemoryManagerPlugin);
					PoolPackage* pool=&plugin->pool;
					SpinLock::Scope scope(pool->lock);
					return pool->pool.GetHandle(pointer);
				}
			protected:
				bool RegisterForeignFunctions(BasicILRuntimeSymbol* symbol)
				{
					return
						REGISTER_LIGHT_FUNCTION2(MemAlloc, char*(vint), MemAlloc) &&
						REGISTER_LIGHT_FUNCTION2(MemFree, bool(char*), MemFree) &&
						REGISTER_LIGHT_FUNCTION2(MemIsValidHandle, bool(char*), MemIsValidHandle) &&
						REGISTER_LIGHT_FUNCTION2(MemGetHandleSize, vint(char*), MemGetHandleSize) &&
						REGISTER_LIGHT_FUNCTION2(MemGetOwnerHandle, char*(char*), MemGetOwnerHandle);
				}
			};

			Ptr<LanguagePlugin> CreateMemoryManagerPlugin()
			{
				return new SystemCoreMemoryManagerPlugin();
			}
		}
	}
}