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

			struct GcSingleThread{};

			class SystemCoreGcSingleThreadPlugin : public SystemCoreGcPluginBase<SystemCoreGcSingleThreadPlugin, GcSingleThread>
			{
			public:
				inline static GcSingleThread* GcCreate()
				{
					CHECK_FAIL(L"NotImplemented");
				}

				inline static bool GcIsMultipleThreadingSupported(GcSingleThread* gc)
				{
					return false;
				}

				inline static GcHandle* GcCreateHandle(GcSingleThread* gc, GcMeta* meta, vint repeat)
				{
					CHECK_FAIL(L"NotImplemented");
				}

				inline static GcMeta* GcGetHandleMeta(GcSingleThread* gc, GcHandle* handle)
				{
					CHECK_FAIL(L"NotImplemented");
				}

				inline static bool GcIsValidHandle(GcSingleThread* gc, GcHandle* handle)
				{
					CHECK_FAIL(L"NotImplemented");
				}

				inline static bool GcIncreaseHandleRef(GcSingleThread* gc, GcHandle* handle)
				{
					CHECK_FAIL(L"NotImplemented");
				}

				inline static bool GcDecreaseHandleRef(GcSingleThread* gc, GcHandle* handle)
				{
					CHECK_FAIL(L"NotImplemented");
				}

				inline static char* GcIncreaseHandlePin(GcSingleThread* gc, GcHandle* handle)
				{
					CHECK_FAIL(L"NotImplemented");
				}

				inline static bool GcDecreaseHandlePin(GcSingleThread* gc, GcHandle* handle)
				{
					CHECK_FAIL(L"NotImplemented");
				}

				inline static bool GcDisposeHandle(GcSingleThread* gc, GcHandle* handle)
				{
					CHECK_FAIL(L"NotImplemented");
				}

				inline static bool GcIsHandleDisposed(GcSingleThread* gc, GcHandle* handle)
				{
					CHECK_FAIL(L"NotImplemented");
				}

				inline static vint GcGetHandleSize(GcSingleThread* gc, GcHandle* handle)
				{
					CHECK_FAIL(L"NotImplemented");
				}

				inline static vint GcGetHandleRepeat(GcSingleThread* gc, GcHandle* handle)
				{
					CHECK_FAIL(L"NotImplemented");
				}

				inline static bool GcReadHandle(GcSingleThread* gc, GcHandle* handle, vint offset, vint length, char* buffer)
				{
					CHECK_FAIL(L"NotImplemented");
				}

				inline static bool GcWriteHandle(GcSingleThread* gc, GcHandle* handle, vint offset, vint length, char* buffer)
				{
					CHECK_FAIL(L"NotImplemented");
				}

				inline static bool GcCollect(GcSingleThread* gc, vint label)
				{
					CHECK_FAIL(L"NotImplemented");
				}
			};

			Ptr<LanguagePlugin> CreateGcSingleThreadPlugin()
			{
				return new SystemCoreGcSingleThreadPlugin();
			}
		}
	}
}