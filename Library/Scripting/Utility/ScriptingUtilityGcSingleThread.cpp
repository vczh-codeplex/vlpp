#include "ScriptingUtilityForeignFunctions.h"
#include "..\Languages\LanguageRuntime.h"
#include "..\..\Entity\GeneralObjectPoolEntity.h"
#include "..\..\Threading.h"
#include "..\..\Entity\GcSingleThreadEntity.h"

namespace vl
{
	namespace scripting
	{
		namespace utility
		{
			using namespace basicil;
			using namespace entities;
			using namespace collections;

			class SystemCoreGcSingleThreadPlugin : public SystemCoreGcPluginBase<SystemCoreGcSingleThreadPlugin, GcSingleThread>
			{
			private:
				struct GcSingleThreadCallbackStruct
				{
					BasicILInterpretor*				interpretor;
					BasicILStack*					stack;
					vint							label;
				};

				GcSingleThreadCallbackStruct		callback;
				List<Ptr<GcSingleThread>>			gcs;
			public:
				inline static GcSingleThread* GcCreate(vint label, vl::scripting::basicil::BasicILInterpretor* interpretor, vl::scripting::basicil::BasicILStack* stack, void* userData)
				{
					LANGUAGE_PLUGIN(SystemCoreGcSingleThreadPlugin);
					plugin->callback.interpretor=interpretor;
					plugin->callback.stack=stack;
					plugin->callback.label=label;
					GcSingleThread* gc=new GcSingleThread(&GcCallback, &plugin->callback, 1048576, 256);
					plugin->gcs.Add(gc);
					return gc;
				}

				inline static bool GcIsMultipleThreadingSupported(GcSingleThread* gc)
				{
					return false;
				}

				inline static GcHandle* GcCreateHandle(GcSingleThread* gc, GcMeta* meta, vint repeat)
				{
					return gc->CreateHandle(meta, repeat);
				}

				inline static GcMeta* GcGetHandleMeta(GcSingleThread* gc, GcHandle* handle)
				{
					return gc->GetHandleMeta(handle);
				}

				inline static bool GcIsValidHandle(GcSingleThread* gc, GcHandle* handle)
				{
					return gc->IsValidHandle(handle);
				}

				inline static bool GcIncreaseHandleRef(GcSingleThread* gc, GcHandle* handle)
				{
					return gc->IncreaseHandleRef(handle);
				}

				inline static bool GcDecreaseHandleRef(GcSingleThread* gc, GcHandle* handle)
				{
					return gc->DecreaseHandleRef(handle);
				}

				inline static char* GcIncreaseHandlePin(GcSingleThread* gc, GcHandle* handle)
				{
					return gc->IncreaseHandlePin(handle);
				}

				inline static bool GcDecreaseHandlePin(GcSingleThread* gc, GcHandle* handle)
				{
					return gc->DecreaseHandlePin(handle);
				}

				inline static bool GcDisposeHandle(GcSingleThread* gc, GcHandle* handle)
				{
					return gc->DisposeHandle(handle);
				}

				inline static bool GcIsHandleDisposed(GcSingleThread* gc, GcHandle* handle)
				{
					return gc->IsHandleDisposed(handle);
				}

				inline static vint GcGetHandleSize(GcSingleThread* gc, GcHandle* handle)
				{
					return gc->GetHandleSize(handle);
				}

				inline static vint GcGetHandleRepeat(GcSingleThread* gc, GcHandle* handle)
				{
					return gc->GetHandleRepeat(handle);
				}

				inline static bool GcReadHandle(GcSingleThread* gc, GcHandle* handle, vint offset, vint length, char* buffer)
				{
					return gc->ReadHandle(handle, offset, length, buffer);
				}

				inline static bool GcWriteHandle(GcSingleThread* gc, GcHandle* handle, vint offset, vint length, char* buffer)
				{
					return gc->WriteHandle(handle, offset, length, buffer);
				}

				inline static bool GcCollect(GcSingleThread* gc)
				{
					return gc->Collect();
				}

				inline static void GcCallback(GcSingleThread* gc, GcHandle* handle, void* userData)
				{
					GcSingleThreadCallbackStruct* callback=(GcSingleThreadCallbackStruct*)userData;
					
					BasicILLabel label;
					{
						CriticalSection::Scope scope(callback->interpretor->GetCriticalSection());
						label=callback->interpretor->Symbols()->GetLabel(callback->label);
						if(callback->interpretor->Symbols()->IsValidILIndex(label.key))
						{
							callback->stack->GetEnv()->Push<GcSingleThread*>(gc);
							callback->stack->GetEnv()->Push<GcHandle*>(handle);
							ILException::RunningResult result=callback->stack->Run();
							if(result!=ILException::Finished)
							{
								CHECK_FAIL(L"TODO");
							}
						}
					}
				}
			};

			Ptr<LanguagePlugin> CreateGcSingleThreadPlugin()
			{
				return new SystemCoreGcSingleThreadPlugin();
			}
		}
	}
}