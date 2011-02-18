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
			public:
				SystemCoreGcSingleThreadPlugin()
				{
					callback.interpretor=0;
					callback.stack=0;
					callback.label=0;
					gc=new GcSingleThread(&GcCallback, &callback, 1048576, 256);
				}

				inline static bool GcInit(vint label, vl::scripting::basicil::BasicILInterpretor* interpretor, vl::scripting::basicil::BasicILStack* stack, void* userData)
				{
					LANGUAGE_PLUGIN(SystemCoreGcSingleThreadPlugin);
					plugin->callback.interpretor=interpretor;
					plugin->callback.stack=stack;
					plugin->callback.label=label;
					return true;
				}

				inline static void GcCallback(GcSingleThread* gc, GcHandle* handle, void* userData)
				{
					GcSingleThreadCallbackStruct* callback=(GcSingleThreadCallbackStruct*)userData;
					if(!callback->interpretor)
					{
						return;
					}
					
					BasicILLabel label;
					{
						CriticalSection::Scope scope(callback->interpretor->GetCriticalSection());
						label=callback->interpretor->Symbols()->GetLabel(callback->label);
					}
					if(callback->interpretor->Symbols()->IsValidILIndex(label.key))
					{
						callback->stack->GetEnv()->Push<GcHandle*>(handle);
						callback->stack->ResetBuffer(label.instruction, label.key, 0);
						ILException::RunningResult result=callback->stack->Run();
						if(result!=ILException::Finished)
						{
							CHECK_FAIL(L"TODO");
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