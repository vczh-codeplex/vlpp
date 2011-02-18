#include "ScriptingUtilityForeignFunctions.h"
#include "..\Languages\LanguageRuntime.h"
#include "..\..\Entity\GcEntityCommon.h"

namespace vl
{
	namespace scripting
	{
		namespace utility
		{
			using namespace basicil;
			using namespace entities;
			using namespace collections;
			
			template<typename _GcImpl, typename _Gc>
			class SystemCoreGcPluginBase : public LanguagePlugin
			{
			protected:
				Ptr<_Gc>				gc;
			public:
				inline static bool GcIsMultipleThreadingSupported()
				{
					return false;
				}

				inline static GcHandle* GcCreateHandle(GcMeta* meta, vint repeat, void* userData)
				{
					LANGUAGE_PLUGIN(SystemCoreGcPluginBase);
					_Gc* gc=plugin->gc.Obj();
					return gc->CreateHandle(meta, repeat);
				}

				inline static GcMeta* GcGetHandleMeta(GcHandle* handle, void* userData)
				{
					LANGUAGE_PLUGIN(SystemCoreGcPluginBase);
					_Gc* gc=plugin->gc.Obj();
					return gc->GetHandleMeta(handle);
				}

				inline static bool GcIsValidHandle(GcHandle* handle, void* userData)
				{
					LANGUAGE_PLUGIN(SystemCoreGcPluginBase);
					_Gc* gc=plugin->gc.Obj();
					return gc->IsValidHandle(handle);
				}

				inline static bool GcIncreaseHandleRef(GcHandle* handle, void* userData)
				{
					LANGUAGE_PLUGIN(SystemCoreGcPluginBase);
					_Gc* gc=plugin->gc.Obj();
					return gc->IncreaseHandleRef(handle);
				}

				inline static bool GcDecreaseHandleRef(GcHandle* handle, void* userData)
				{
					LANGUAGE_PLUGIN(SystemCoreGcPluginBase);
					_Gc* gc=plugin->gc.Obj();
					return gc->DecreaseHandleRef(handle);
				}

				inline static char* GcIncreaseHandlePin(GcHandle* handle, void* userData)
				{
					LANGUAGE_PLUGIN(SystemCoreGcPluginBase);
					_Gc* gc=plugin->gc.Obj();
					return gc->IncreaseHandlePin(handle);
				}

				inline static bool GcDecreaseHandlePin(GcHandle* handle, void* userData)
				{
					LANGUAGE_PLUGIN(SystemCoreGcPluginBase);
					_Gc* gc=plugin->gc.Obj();
					return gc->DecreaseHandlePin(handle);
				}

				inline static bool GcDisposeHandle(GcHandle* handle, void* userData)
				{
					LANGUAGE_PLUGIN(SystemCoreGcPluginBase);
					_Gc* gc=plugin->gc.Obj();
					return gc->DisposeHandle(handle);
				}

				inline static bool GcIsHandleDisposed(GcHandle* handle, void* userData)
				{
					LANGUAGE_PLUGIN(SystemCoreGcPluginBase);
					_Gc* gc=plugin->gc.Obj();
					return gc->IsHandleDisposed(handle);
				}

				inline static vint GcGetHandleSize(GcHandle* handle, void* userData)
				{
					LANGUAGE_PLUGIN(SystemCoreGcPluginBase);
					_Gc* gc=plugin->gc.Obj();
					return gc->GetHandleSize(handle);
				}

				inline static vint GcGetHandleRepeat(GcHandle* handle, void* userData)
				{
					LANGUAGE_PLUGIN(SystemCoreGcPluginBase);
					_Gc* gc=plugin->gc.Obj();
					return gc->GetHandleRepeat(handle);
				}

				inline static bool GcReadHandle(GcHandle* handle, vint repeat, vint index, GcHandle** value, bool increaseRef, void* userData)
				{
					LANGUAGE_PLUGIN(SystemCoreGcPluginBase);
					_Gc* gc=plugin->gc.Obj();
					return gc->ReadHandle(handle, repeat, index, value, increaseRef);
				}

				inline static bool GcWriteHandle(GcHandle* handle, vint repeat, vint index, GcHandle* value, bool decreaseRef, void* userData)
				{
					LANGUAGE_PLUGIN(SystemCoreGcPluginBase);
					_Gc* gc=plugin->gc.Obj();
					return gc->WriteHandle(handle, repeat, index, value, decreaseRef);
				}

				inline static bool GcRead(GcHandle* handle, vint offset, vint length, char* buffer, void* userData)
				{
					LANGUAGE_PLUGIN(SystemCoreGcPluginBase);
					_Gc* gc=plugin->gc.Obj();
					return gc->Read(handle, offset, length, buffer);
				}

				inline static bool GcWrite(GcHandle* handle, vint offset, vint length, char* buffer, void* userData)
				{
					LANGUAGE_PLUGIN(SystemCoreGcPluginBase);
					_Gc* gc=plugin->gc.Obj();
					return gc->Write(handle, offset, length, buffer);
				}

				inline static bool GcCopy(GcHandle* hDst, vint oDst, GcHandle* hSrc, vint oSrc, vint length, void* userData)
				{
					LANGUAGE_PLUGIN(SystemCoreGcPluginBase);
					_Gc* gc=plugin->gc.Obj();
					return gc->Copy(hDst, oDst, hSrc, oSrc, length);
				}

				inline static bool GcCollect(void* userData)
				{
					LANGUAGE_PLUGIN(SystemCoreGcPluginBase);
					_Gc* gc=plugin->gc.Obj();
					return gc->Collect();
				}
			protected:
				bool RegisterForeignFunctions(vl::scripting::basicil::BasicILRuntimeSymbol* symbol)
				{
					typedef vl::entities::GcMetaSegment GcMetaSegment;
					typedef vl::entities::GcMeta GcMeta;
					typedef vl::entities::GcHandle GcHandle;

					return 
						REGISTER_LIGHT_FUNCTION3(GcInit, bool(vint), _GcImpl::GcInit) &&
						REGISTER_LIGHT_FUNCTION(GcIsMultipleThreadingSupported, bool(void), _GcImpl::GcIsMultipleThreadingSupported) &&
						REGISTER_LIGHT_FUNCTION2(GcCreateHandle, GcHandle*(GcMeta*, vint), _GcImpl::GcCreateHandle) &&
						REGISTER_LIGHT_FUNCTION2(GcGetHandleMeta, GcMeta*(GcHandle*), _GcImpl::GcGetHandleMeta) &&
						REGISTER_LIGHT_FUNCTION2(GcIsValidHandle, bool(GcHandle*), _GcImpl::GcIsValidHandle) &&
						REGISTER_LIGHT_FUNCTION2(GcIncreaseHandleRef, bool(GcHandle*), _GcImpl::GcIncreaseHandleRef) &&
						REGISTER_LIGHT_FUNCTION2(GcDecreaseHandleRef, bool(GcHandle*), _GcImpl::GcDecreaseHandleRef) &&
						REGISTER_LIGHT_FUNCTION2(GcIncreaseHandlePin, char*(GcHandle*), _GcImpl::GcIncreaseHandlePin) &&
						REGISTER_LIGHT_FUNCTION2(GcDecreaseHandlePin, bool(GcHandle*), _GcImpl::GcDecreaseHandlePin) &&
						REGISTER_LIGHT_FUNCTION2(GcDisposeHandle, bool(GcHandle*), _GcImpl::GcDisposeHandle) &&
						REGISTER_LIGHT_FUNCTION2(GcIsHandleDisposed, bool(GcHandle*), _GcImpl::GcIsHandleDisposed) &&
						REGISTER_LIGHT_FUNCTION2(GcGetHandleSize, vint(GcHandle*), _GcImpl::GcGetHandleSize) &&
						REGISTER_LIGHT_FUNCTION2(GcGetHandleRepeat, vint(GcHandle*), _GcImpl::GcGetHandleRepeat) &&
						REGISTER_LIGHT_FUNCTION2(GcReadHandle, bool(GcHandle*, vint, vint, GcHandle**, bool), _GcImpl::GcReadHandle)&&
						REGISTER_LIGHT_FUNCTION2(GcWriteHandle, bool(GcHandle*, vint, vint, GcHandle*, bool), _GcImpl::GcWriteHandle)&&
						REGISTER_LIGHT_FUNCTION2(GcRead, bool(GcHandle*, vint, vint, char*), _GcImpl::GcRead) &&
						REGISTER_LIGHT_FUNCTION2(GcWrite, bool(GcHandle*, vint, vint, char*),_GcImpl::GcWrite) &&
						REGISTER_LIGHT_FUNCTION2(GcCopy, bool(GcHandle*, vint, GcHandle*, vint, vint),_GcImpl::GcCopy) &&
						REGISTER_LIGHT_FUNCTION2(GcCollect, bool(void), _GcImpl::GcCollect);
				}
			};
		}
	}
}