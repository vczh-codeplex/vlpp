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
			public:
				inline static bool GcIsMultipleThreadingSupported(_Gc* gc)
				{
					return false;
				}

				inline static GcHandle* GcCreateHandle(_Gc* gc, GcMeta* meta, vint repeat)
				{
					return gc->CreateHandle(meta, repeat);
				}

				inline static GcMeta* GcGetHandleMeta(_Gc* gc, GcHandle* handle)
				{
					return gc->GetHandleMeta(handle);
				}

				inline static bool GcIsValidHandle(_Gc* gc, GcHandle* handle)
				{
					return gc->IsValidHandle(handle);
				}

				inline static bool GcIncreaseHandleRef(_Gc* gc, GcHandle* handle)
				{
					return gc->IncreaseHandleRef(handle);
				}

				inline static bool GcDecreaseHandleRef(_Gc* gc, GcHandle* handle)
				{
					return gc->DecreaseHandleRef(handle);
				}

				inline static char* GcIncreaseHandlePin(_Gc* gc, GcHandle* handle)
				{
					return gc->IncreaseHandlePin(handle);
				}

				inline static bool GcDecreaseHandlePin(_Gc* gc, GcHandle* handle)
				{
					return gc->DecreaseHandlePin(handle);
				}

				inline static bool GcDisposeHandle(_Gc* gc, GcHandle* handle)
				{
					return gc->DisposeHandle(handle);
				}

				inline static bool GcIsHandleDisposed(_Gc* gc, GcHandle* handle)
				{
					return gc->IsHandleDisposed(handle);
				}

				inline static vint GcGetHandleSize(_Gc* gc, GcHandle* handle)
				{
					return gc->GetHandleSize(handle);
				}

				inline static vint GcGetHandleRepeat(_Gc* gc, GcHandle* handle)
				{
					return gc->GetHandleRepeat(handle);
				}

				inline static bool GcReadHandle(_Gc* gc, GcHandle* handle, vint repeat, vint index, GcHandle** value, bool increaseRef)
				{
					return gc->ReadHandle(handle, repeat, index, value, increaseRef);
				}

				inline static bool GcWriteHandle(_Gc* gc, GcHandle* handle, vint repeat, vint index, GcHandle* value, bool decreaseRef)
				{
					return gc->WriteHandle(handle, repeat, index, value, decreaseRef);
				}

				inline static bool GcRead(_Gc* gc, GcHandle* handle, vint offset, vint length, char* buffer)
				{
					return gc->Read(handle, offset, length, buffer);
				}

				inline static bool GcWrite(_Gc* gc, GcHandle* handle, vint offset, vint length, char* buffer)
				{
					return gc->Write(handle, offset, length, buffer);
				}

				inline static bool GcCopy(_Gc* gc, GcHandle* hDst, vint oDst, GcHandle* hSrc, vint oSrc, vint length)
				{
					return gc->Copy(hDst, oDst, hSrc, oSrc, length);
				}

				inline static bool GcCollect(_Gc* gc)
				{
					return gc->Collect();
				}
			protected:
				bool RegisterForeignFunctions(vl::scripting::basicil::BasicILRuntimeSymbol* symbol)
				{
					typedef vl::entities::GcMetaSegment GcMetaSegment;
					typedef vl::entities::GcMeta GcMeta;
					typedef vl::entities::GcHandle GcHandle;

					return 
						REGISTER_LIGHT_FUNCTION3(GcCreate, _Gc*(vint), _GcImpl::GcCreate) &&
						REGISTER_LIGHT_FUNCTION(GcIsMultipleThreadingSupported, bool(_Gc*), _GcImpl::GcIsMultipleThreadingSupported) &&
						REGISTER_LIGHT_FUNCTION(GcCreateHandle, GcHandle*(_Gc*, GcMeta*, vint), _GcImpl::GcCreateHandle) &&
						REGISTER_LIGHT_FUNCTION(GcGetHandleMeta, GcMeta*(_Gc*, GcHandle*), _GcImpl::GcGetHandleMeta) &&
						REGISTER_LIGHT_FUNCTION(GcIsValidHandle, bool(_Gc*, GcHandle*), _GcImpl::GcIsValidHandle) &&
						REGISTER_LIGHT_FUNCTION(GcIncreaseHandleRef, bool(_Gc*, GcHandle*), _GcImpl::GcIncreaseHandleRef) &&
						REGISTER_LIGHT_FUNCTION(GcDecreaseHandleRef, bool(_Gc*, GcHandle*), _GcImpl::GcDecreaseHandleRef) &&
						REGISTER_LIGHT_FUNCTION(GcIncreaseHandlePin, char*(_Gc*, GcHandle*), _GcImpl::GcIncreaseHandlePin) &&
						REGISTER_LIGHT_FUNCTION(GcDecreaseHandlePin, bool(_Gc*, GcHandle*), _GcImpl::GcDecreaseHandlePin) &&
						REGISTER_LIGHT_FUNCTION(GcDisposeHandle, bool(_Gc*, GcHandle*), _GcImpl::GcDisposeHandle) &&
						REGISTER_LIGHT_FUNCTION(GcIsHandleDisposed, bool(_Gc*, GcHandle*), _GcImpl::GcIsHandleDisposed) &&
						REGISTER_LIGHT_FUNCTION(GcGetHandleSize, vint(_Gc*, GcHandle*), _GcImpl::GcGetHandleSize) &&
						REGISTER_LIGHT_FUNCTION(GcGetHandleRepeat, vint(_Gc*, GcHandle*), _GcImpl::GcGetHandleRepeat) &&
						REGISTER_LIGHT_FUNCTION(GcReadHandle, bool(_Gc*, GcHandle*, vint, vint, GcHandle**, bool), _GcImpl::GcReadHandle)&&
						REGISTER_LIGHT_FUNCTION(GcWriteHandle, bool(_Gc*, GcHandle*, vint, vint, GcHandle*, bool), _GcImpl::GcWriteHandle)&&
						REGISTER_LIGHT_FUNCTION(GcRead, bool(_Gc*, GcHandle*, vint, vint, char*), _GcImpl::GcRead) &&
						REGISTER_LIGHT_FUNCTION(GcWrite, bool(_Gc*, GcHandle*, vint, vint, char*),_GcImpl::GcWrite) &&
						REGISTER_LIGHT_FUNCTION(GcCopy, bool(_Gc*, GcHandle*, vint, GcHandle*, vint, vint),_GcImpl::GcCopy) &&
						REGISTER_LIGHT_FUNCTION(GcCollect, bool(_Gc*), _GcImpl::GcCollect);
				}
			};
		}
	}
}