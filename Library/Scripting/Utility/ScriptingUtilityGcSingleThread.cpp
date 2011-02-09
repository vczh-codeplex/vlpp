#include "ScriptingUtilityForeignFunctions.h"
#include "..\Languages\LanguageRuntime.h"
#include "..\..\Entity\GcSingleThreadEntity.h"
#include "ScriptingUtilityGcCommon.h"

namespace vl
{
	namespace scripting
	{
		namespace utility
		{
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