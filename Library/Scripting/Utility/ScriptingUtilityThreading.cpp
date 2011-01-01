#include "ScriptingUtilityForeignFunctions.h"
#include "..\Languages\LanguageRuntime.h"
#include "..\..\Threading.h"

namespace vl
{
	namespace scripting
	{
		namespace utility
		{
			using namespace basicil;
			using namespace collections;

			class SystemCoreThreadingPlugin : public LanguagePlugin
			{
			protected:
				LanguageHandleList<WaitableObject>		waitables;
				LanguageHandleList<CriticalSection>		criticalSections;

				/*----------------------------------------------------------------------*/

				BEGIN_FOREIGN_FUNCTION(SynCreateCriticalSection, SystemCoreThreadingPlugin)
				{
					CriticalSection* cs=new CriticalSection;
					vint handle=plugin->criticalSections.Alloc(cs);
					reader.Result<vint>()=handle;
				}
				END_FOREIGN_FUNCTION

				BEGIN_FOREIGN_FUNCTION(SynDisposeCriticalSection, SystemCoreThreadingPlugin)
				{
					vint handle=reader.NextArgument<vint>();
					reader.Result<bool>()=plugin->criticalSections.Free(handle);
				}
				END_FOREIGN_FUNCTION

				BEGIN_FOREIGN_FUNCTION(SynTryEnterCriticalSection, SystemCoreThreadingPlugin)
				{
					vint handle=reader.NextArgument<vint>();
					CriticalSection* cs=plugin->criticalSections.GetHandle(handle);
					if(cs)
					{
						reader.Result<bool>()=cs->TryEnter();
					}
					else
					{
						reader.Result<bool>()=false;
					}
				}
				END_FOREIGN_FUNCTION

				BEGIN_FOREIGN_FUNCTION(SynEnterCriticalSection, SystemCoreThreadingPlugin)
				{
					vint handle=reader.NextArgument<vint>();
					CriticalSection* cs=plugin->criticalSections.GetHandle(handle);
					if(cs)
					{
						cs->Enter();
						reader.Result<bool>()=true;
					}
					else
					{
						reader.Result<bool>()=false;
					}
				}
				END_FOREIGN_FUNCTION

				BEGIN_FOREIGN_FUNCTION(SynLeaveCriticalSection, SystemCoreThreadingPlugin)
				{
					vint handle=reader.NextArgument<vint>();
					CriticalSection* cs=plugin->criticalSections.GetHandle(handle);
					if(cs)
					{
						cs->Leave();
						reader.Result<bool>()=true;
					}
					else
					{
						reader.Result<bool>()=false;
					}
				}
				END_FOREIGN_FUNCTION

				/*----------------------------------------------------------------------*/

				BEGIN_FOREIGN_FUNCTION(SynCreateMutex, SystemCoreThreadingPlugin)
				{
					bool owned=reader.NextArgument<bool>();
					Mutex* mutex=new Mutex;
					if(mutex->Create(owned))
					{
						vint handle=plugin->waitables.Alloc(mutex);
						reader.Result<vint>()=handle;
					}
					else
					{
						delete mutex;
						reader.Result<vint>()=-1;
					}
				}
				END_FOREIGN_FUNCTION

				BEGIN_FOREIGN_FUNCTION(SynReleaseMutex, SystemCoreThreadingPlugin)
				{
					vint handle=reader.NextArgument<vint>();
					Mutex* mutex=dynamic_cast<Mutex*>(plugin->waitables.GetHandle(handle));
					if(mutex)
					{
						reader.Result<bool>()=mutex->Release();
					}
					else
					{
						reader.Result<bool>()=false;
					}
				}
				END_FOREIGN_FUNCTION

				BEGIN_FOREIGN_FUNCTION(SynCreateSemaphore, SystemCoreThreadingPlugin)
				{
					vint init=reader.NextArgument<vint>();
					vint max=reader.NextArgument<vint>();
					Semaphore* semaphore=new Semaphore;
					if(semaphore->Create(init, max))
					{
						vint handle=plugin->waitables.Alloc(semaphore);
						reader.Result<vint>()=handle;
					}
					else
					{
						delete semaphore;
						reader.Result<vint>()=-1;
					}
				}
				END_FOREIGN_FUNCTION

				BEGIN_FOREIGN_FUNCTION(SynReleaseSemaphore, SystemCoreThreadingPlugin)
				{
					vint handle=reader.NextArgument<vint>();
					vint count=reader.NextArgument<vint>();
					Semaphore* semaphore=dynamic_cast<Semaphore*>(plugin->waitables.GetHandle(handle));
					if(semaphore)
					{
						reader.Result<bool>()=semaphore->Release(count)!=-1;
					}
					else
					{
						reader.Result<bool>()=false;
					}
				}
				END_FOREIGN_FUNCTION

				BEGIN_FOREIGN_FUNCTION(SynCreateAutoEvent, SystemCoreThreadingPlugin)
				{
					bool signaled=reader.NextArgument<bool>();
					EventObject* eventObject=new EventObject;
					if(eventObject->CreateAutoUnsignal(signaled))
					{
						vint handle=plugin->waitables.Alloc(eventObject);
						reader.Result<vint>()=handle;
					}
					else
					{
						delete eventObject;
						reader.Result<vint>()=-1;
					}
				}
				END_FOREIGN_FUNCTION

				BEGIN_FOREIGN_FUNCTION(SynCreateManualEvent, SystemCoreThreadingPlugin)
				{
					bool signaled=reader.NextArgument<bool>();
					EventObject* eventObject=new EventObject;
					if(eventObject->CreateManualUnsignal(signaled))
					{
						vint handle=plugin->waitables.Alloc(eventObject);
						reader.Result<vint>()=handle;
					}
					else
					{
						delete eventObject;
						reader.Result<vint>()=-1;
					}
				}
				END_FOREIGN_FUNCTION

				BEGIN_FOREIGN_FUNCTION(SynSignalEvent, SystemCoreThreadingPlugin)
				{
					vint handle=reader.NextArgument<vint>();
					EventObject* eventObject=dynamic_cast<EventObject*>(plugin->waitables.GetHandle(handle));
					if(eventObject)
					{
						reader.Result<bool>()=eventObject->Signal();
					}
					else
					{
						reader.Result<bool>()=false;
					}
				}
				END_FOREIGN_FUNCTION

				BEGIN_FOREIGN_FUNCTION(SynUnsignalEvent, SystemCoreThreadingPlugin)
				{
					vint handle=reader.NextArgument<vint>();
					EventObject* eventObject=dynamic_cast<EventObject*>(plugin->waitables.GetHandle(handle));
					if(eventObject)
					{
						reader.Result<bool>()=eventObject->Unsignal();
					}
					else
					{
						reader.Result<bool>()=false;
					}
				}
				END_FOREIGN_FUNCTION

				BEGIN_FOREIGN_FUNCTION(SynDisposeWaitable, SystemCoreThreadingPlugin)
				{
					vint handle=reader.NextArgument<vint>();
					WaitableObject* waitable=plugin->waitables.GetHandle(handle);
					if(waitable)
					{
						plugin->waitables.Free(handle);
						reader.Result<bool>()=true;
					}
					else
					{
						reader.Result<bool>()=false;
					}
				}
				END_FOREIGN_FUNCTION

				BEGIN_FOREIGN_FUNCTION(SynWait, SystemCoreThreadingPlugin)
				{
					vint handle=reader.NextArgument<vint>();
					WaitableObject* waitable=plugin->waitables.GetHandle(handle);
					if(waitable)
					{
						reader.Result<bool>()=waitable->Wait();
					}
					else
					{
						reader.Result<bool>()=false;
					}
				}
				END_FOREIGN_FUNCTION

				BEGIN_FOREIGN_FUNCTION(SynWaitForTime, SystemCoreThreadingPlugin)
				{
					vint handle=reader.NextArgument<vint>();
					vint ms=reader.NextArgument<vint>();
					WaitableObject* waitable=plugin->waitables.GetHandle(handle);
					if(waitable)
					{
						reader.Result<bool>()=waitable->WaitForTime(ms);
					}
					else
					{
						reader.Result<bool>()=false;
					}
				}
				END_FOREIGN_FUNCTION

				/*----------------------------------------------------------------------*/

				bool RegisterForeignFunctions(BasicILRuntimeSymbol* symbol)
				{
					return
						REGISTER_FOREIGN_FUNCTION(SynCreateCriticalSection) &&
						REGISTER_FOREIGN_FUNCTION(SynDisposeCriticalSection) &&
						REGISTER_FOREIGN_FUNCTION(SynTryEnterCriticalSection) &&
						REGISTER_FOREIGN_FUNCTION(SynEnterCriticalSection) &&
						REGISTER_FOREIGN_FUNCTION(SynLeaveCriticalSection) &&
						REGISTER_FOREIGN_FUNCTION(SynCreateMutex) &&
						REGISTER_FOREIGN_FUNCTION(SynReleaseMutex) &&
						REGISTER_FOREIGN_FUNCTION(SynCreateSemaphore) &&
						REGISTER_FOREIGN_FUNCTION(SynReleaseSemaphore) &&
						REGISTER_FOREIGN_FUNCTION(SynCreateAutoEvent) &&
						REGISTER_FOREIGN_FUNCTION(SynCreateManualEvent) &&
						REGISTER_FOREIGN_FUNCTION(SynSignalEvent) &&
						REGISTER_FOREIGN_FUNCTION(SynUnsignalEvent) &&
						REGISTER_FOREIGN_FUNCTION(SynDisposeWaitable) &&
						REGISTER_FOREIGN_FUNCTION(SynWait) &&
						REGISTER_FOREIGN_FUNCTION(SynWaitForTime);
				}
			public:
				SystemCoreThreadingPlugin()
				{
				}
			};

			Ptr<LanguagePlugin> CreateThreadingPlugin()
			{
				return new SystemCoreThreadingPlugin();
			}
		}
	}
}