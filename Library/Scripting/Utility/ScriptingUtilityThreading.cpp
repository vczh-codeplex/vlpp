#include "ScriptingUtilityForeignFunctions.h"
#include "..\Languages\LanguageRuntime.h"
#include "..\..\Threading.h"
#include <intrin.h>

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

				static bool SynInitializeSpinLock(vint* lock)
				{
					*lock=0;
					return true;
				}

				static bool SynTryEnterSpinLock(vint* lock)
				{
					return _InterlockedExchange((long*)lock, 1)==0;
				}

				static bool SynEnterSpinLock(vint* lock)
				{
					while(_InterlockedExchange((long*)lock, 1)==1);
					return true;
				}

				static bool SynLeaveSpinLock(vint* lock)
				{
					_InterlockedExchange((long*)lock, 0);
					return true;
				}

				/*----------------------------------------------------------------------*/

				static vint SynCreateCriticalSection(void* userData)
				{
					LANGUAGE_PLUGIN(SystemCoreThreadingPlugin);
					CriticalSection* cs=new CriticalSection;
					vint handle=plugin->criticalSections.Alloc(cs);
					return handle;
				}

				static bool SynDisposeCriticalSection(vint handle, void* userData)
				{
					LANGUAGE_PLUGIN(SystemCoreThreadingPlugin);
					return plugin->criticalSections.Free(handle);
				}

				static bool SynTryEnterCriticalSection(vint handle, void* userData)
				{
					LANGUAGE_PLUGIN(SystemCoreThreadingPlugin);
					CriticalSection* cs=plugin->criticalSections.GetHandle(handle);
					return cs?cs->TryEnter():false;
				}

				static bool SynEnterCriticalSection(vint handle, void* userData)
				{
					LANGUAGE_PLUGIN(SystemCoreThreadingPlugin);
					CriticalSection* cs=plugin->criticalSections.GetHandle(handle);
					if(cs)
					{
						cs->Enter();
						return true;
					}
					else
					{
						return false;
					}
				}

				static bool SynLeaveCriticalSection(vint handle, void* userData)
				{
					LANGUAGE_PLUGIN(SystemCoreThreadingPlugin);
					CriticalSection* cs=plugin->criticalSections.GetHandle(handle);
					if(cs)
					{
						cs->Leave();
						return true;
					}
					else
					{
						return false;
					}
				}

				/*----------------------------------------------------------------------*/

				static vint SynCreateMutex(bool owned, void* userData)
				{
					LANGUAGE_PLUGIN(SystemCoreThreadingPlugin);
					Mutex* mutex=new Mutex;
					if(mutex->Create(owned))
					{
						vint handle=plugin->waitables.Alloc(mutex);
						return handle;
					}
					else
					{
						delete mutex;
						return 0;
					}
				}

				static bool SynReleaseMutex(vint handle, void* userData)
				{
					LANGUAGE_PLUGIN(SystemCoreThreadingPlugin);
					Mutex* mutex=dynamic_cast<Mutex*>(plugin->waitables.GetHandle(handle));
					if(mutex)
					{
						return mutex->Release();
					}
					else
					{
						return false;
					}
				}

				static vint SynCreateSemaphore(vint init, vint max, void* userData)
				{
					LANGUAGE_PLUGIN(SystemCoreThreadingPlugin);
					Semaphore* semaphore=new Semaphore;
					if(semaphore->Create(init, max))
					{
						vint handle=plugin->waitables.Alloc(semaphore);
						return handle;
					}
					else
					{
						delete semaphore;
						return 0;
					}
				}

				static bool SynReleaseSemaphore(vint handle, vint count, void* userData)
				{
					LANGUAGE_PLUGIN(SystemCoreThreadingPlugin);
					Semaphore* semaphore=dynamic_cast<Semaphore*>(plugin->waitables.GetHandle(handle));
					if(semaphore)
					{
						return semaphore->Release(count)!=-1;
					}
					else
					{
						return false;
					}
				}

				static vint SynCreateAutoEvent(bool signaled, void* userData)
				{
					LANGUAGE_PLUGIN(SystemCoreThreadingPlugin);
					EventObject* eventObject=new EventObject;
					if(eventObject->CreateAutoUnsignal(signaled))
					{
						vint handle=plugin->waitables.Alloc(eventObject);
						return handle;
					}
					else
					{
						delete eventObject;
						return 0;
					}
				}

				static vint SynCreateManualEvent(bool signaled, void* userData)
				{
					LANGUAGE_PLUGIN(SystemCoreThreadingPlugin);
					EventObject* eventObject=new EventObject;
					if(eventObject->CreateManualUnsignal(signaled))
					{
						vint handle=plugin->waitables.Alloc(eventObject);
						return handle;
					}
					else
					{
						delete eventObject;
						return 0;
					}
				}

				static bool SynSignalEvent(vint handle, void* userData)
				{
					LANGUAGE_PLUGIN(SystemCoreThreadingPlugin);
					EventObject* eventObject=dynamic_cast<EventObject*>(plugin->waitables.GetHandle(handle));
					return eventObject?eventObject->Signal():false;
				}

				static bool SynUnsignalEvent(vint handle, void* userData)
				{
					LANGUAGE_PLUGIN(SystemCoreThreadingPlugin);
					EventObject* eventObject=dynamic_cast<EventObject*>(plugin->waitables.GetHandle(handle));
					return eventObject?eventObject->Unsignal():false;
				}

				static bool SynDisposeWaitable(vint handle, void* userData)
				{
					LANGUAGE_PLUGIN(SystemCoreThreadingPlugin);
					WaitableObject* waitable=plugin->waitables.GetHandle(handle);
					return waitable?plugin->waitables.Free(handle):false;
				}

				static bool SynWait(vint handle, void* userData)
				{
					LANGUAGE_PLUGIN(SystemCoreThreadingPlugin);
					WaitableObject* waitable=plugin->waitables.GetHandle(handle);
					return waitable?waitable->Wait():false;
				}

				static bool SynWaitForTime(vint handle, vint ms, void* userData)
				{
					LANGUAGE_PLUGIN(SystemCoreThreadingPlugin);
					WaitableObject* waitable=plugin->waitables.GetHandle(handle);
					return waitable?waitable->WaitForTime(ms):false;
				}

				/*----------------------------------------------------------------------*/

				static vint SynCreateThread(vint label, void* arguments, BasicILInterpretor* interpretor, BasicILStack* stack, void* userData)
				{
					LANGUAGE_PLUGIN(SystemCoreThreadingPlugin);
					BasicILLock::Scope scope(interpretor->GetLock());
					if(interpretor->Symbols()->IsValidILIndex(interpretor->Symbols()->GetLabel(label).key))
					{
						SynThread* thread=new SynThread(interpretor, label, arguments);
						vint handle=plugin->waitables.Alloc(thread);
						thread->SetPlugin(handle, plugin);
						return handle;
					}
					else
					{
						return 0;
					}
				}

				static bool SynStartThread(vint handle, void* userData)
				{
					LANGUAGE_PLUGIN(SystemCoreThreadingPlugin);
					SynThread* thread=dynamic_cast<SynThread*>(plugin->waitables.GetHandle(handle));
					return thread?thread->Start():false;
				}

				static bool SynPauseAndWaitThread(vint handle, void* userData)
				{
					LANGUAGE_PLUGIN(SystemCoreThreadingPlugin);
					SynThread* thread=dynamic_cast<SynThread*>(plugin->waitables.GetHandle(handle));
					if(thread && thread->IsRunning())
					{
						thread->SetPauseSignal();
						thread->GetPausingEvent()->Wait();
						return true;
					}
					else
					{
						return false;
					}
				}

				static bool SynResumeThread(vint handle, void* userData)
				{
					LANGUAGE_PLUGIN(SystemCoreThreadingPlugin);
					SynThread* thread=dynamic_cast<SynThread*>(plugin->waitables.GetHandle(handle));
					if(thread && !thread->IsRunning())
					{
						thread->SetResumeSignal();
						return true;
					}
					else
					{
						return false;
					}
				}

				static bool SynStopAndWaitThread(vint handle, void* userData)
				{
					LANGUAGE_PLUGIN(SystemCoreThreadingPlugin);
					SynThread* thread=dynamic_cast<SynThread*>(plugin->waitables.GetHandle(handle));
					if(thread && thread->IsRunning())
					{
						thread->SetStopSignal();
						thread->GetPausingEvent()->Wait();
						return true;
					}
					else
					{
						return false;
					}
				}

				static bool SynSleep(vint ms)
				{
					Thread::Sleep(ms);
					return true;
				}

				class SynThread : public Thread
				{
				protected:
					EventObject					eventPausing;
					BasicILInterpretor*			interpretor;
					Ptr<BasicILStack>			stack;
					vint						procedureLabel;
					void*						procedureArguments;

					vint						handle;
					SystemCoreThreadingPlugin*	plugin;

					volatile bool				finished;
					volatile bool				running;
					volatile bool				needToStop;
					ILException::RunningResult	runningResult;

					void Run()
					{
						BasicILLabel label;
						{
							BasicILLock::Scope scope(interpretor->GetLock());
							label=interpretor->Symbols()->GetLabel(procedureLabel);
						}
						if(interpretor->Symbols()->IsValidILIndex(label.key))
						{
							stack->GetEnv()->Push<void*>(procedureArguments);
							stack->ResetBuffer(label.instruction, label.key, 0);
							while(true)
							{
								eventPausing.Unsignal();
								running=true;
								bool needToPause=false;

								ILException::RunningResult result=stack->Run();
								switch(result)
								{
								case ILException::Paused:
									if(!needToStop)
									{
										needToPause=true;
										break;
									}
								default:
									{
										finished=true;
										running=false;
										runningResult=result;
									}
								}

								running=false;
								eventPausing.Signal();
								if(needToPause)
								{
									Pause();
								}
								else
								{
									break;
								}
							}
						}
						else
						{
							running=false;
							runningResult=ILException::UnknownInstruction;
						}
					}
				public:
					SynThread(BasicILInterpretor* _interpretor, vint _label, void* _arguments)
						:interpretor(_interpretor)
						,procedureLabel(_label)
						,procedureArguments(_arguments)
						,finished(false)
						,running(false)
						,runningResult(ILException::Finished)
						,handle(0)
						,plugin(0)
					{
						eventPausing.CreateManualUnsignal(true);
						stack=new BasicILStack(interpretor);
					}

					void SetPlugin(vint _handle, SystemCoreThreadingPlugin* _plugin)
					{
						handle=_handle;
						plugin=_plugin;
					}

					EventObject* GetPausingEvent()
					{
						return &eventPausing;
					}

					BasicILInterpretor* GetInterpretor()
					{
						return interpretor;
					}

					BasicILStack* GetStack()
					{
						return stack.Obj();
					}

					bool IsFinished()
					{
						return finished;
					}

					bool IsRunning()
					{
						return running;
					}

					ILException::RunningResult GetRunningResult()
					{
						return runningResult;
					}

					void SetPauseSignal()
					{
						stack->Pause();
					}

					void SetStopSignal()
					{
						needToStop=true;
						stack->Pause();
					}

					void SetResumeSignal()
					{
						Resume();
					}
				};

				/*----------------------------------------------------------------------*/

				bool RegisterForeignFunctions(BasicILRuntimeSymbol* symbol)
				{
					return
						REGISTER_LIGHT_FUNCTION(SynInitializeSpinLock, bool(vint*), SynInitializeSpinLock) &&
						REGISTER_LIGHT_FUNCTION(SynTryEnterSpinLock, bool(vint*), SynTryEnterSpinLock) &&
						REGISTER_LIGHT_FUNCTION(SynEnterSpinLock, bool(vint*), SynEnterSpinLock) &&
						REGISTER_LIGHT_FUNCTION(SynLeaveSpinLock, bool(vint*), SynLeaveSpinLock) &&
						REGISTER_LIGHT_FUNCTION2(SynCreateCriticalSection, vint(), SynCreateCriticalSection) &&
						REGISTER_LIGHT_FUNCTION2(SynDisposeCriticalSection, bool(vint), SynDisposeCriticalSection) &&
						REGISTER_LIGHT_FUNCTION2(SynTryEnterCriticalSection, bool(vint), SynTryEnterCriticalSection) &&
						REGISTER_LIGHT_FUNCTION2(SynEnterCriticalSection, bool(vint), SynEnterCriticalSection) &&
						REGISTER_LIGHT_FUNCTION2(SynLeaveCriticalSection, bool(vint), SynLeaveCriticalSection) &&
						REGISTER_LIGHT_FUNCTION2(SynCreateMutex, vint(bool), SynCreateMutex) &&
						REGISTER_LIGHT_FUNCTION2(SynReleaseMutex, bool(vint), SynReleaseMutex) &&
						REGISTER_LIGHT_FUNCTION2(SynCreateSemaphore, vint(vint, vint), SynCreateSemaphore) &&
						REGISTER_LIGHT_FUNCTION2(SynReleaseSemaphore, bool(vint, vint), SynReleaseSemaphore) &&
						REGISTER_LIGHT_FUNCTION2(SynCreateAutoEvent, vint(bool), SynCreateAutoEvent) &&
						REGISTER_LIGHT_FUNCTION2(SynCreateManualEvent, vint(bool), SynCreateManualEvent) &&
						REGISTER_LIGHT_FUNCTION2(SynSignalEvent, bool(vint), SynSignalEvent) &&
						REGISTER_LIGHT_FUNCTION2(SynUnsignalEvent, bool(vint), SynUnsignalEvent) &&
						REGISTER_LIGHT_FUNCTION2(SynDisposeWaitable, bool(vint), SynDisposeWaitable) &&
						REGISTER_LIGHT_FUNCTION2(SynWait, bool(vint), SynWait) &&
						REGISTER_LIGHT_FUNCTION2(SynWaitForTime, bool(vint, vint), SynWaitForTime) &&
						REGISTER_LIGHT_FUNCTION3(SynCreateThread, vint(vint, void*), SynCreateThread) &&
						REGISTER_LIGHT_FUNCTION2(SynStartThread, bool(vint), SynStartThread) &&
						REGISTER_LIGHT_FUNCTION2(SynPauseAndWaitThread, bool(vint), SynPauseAndWaitThread) &&
						REGISTER_LIGHT_FUNCTION2(SynResumeThread, bool(vint), SynResumeThread) &&
						REGISTER_LIGHT_FUNCTION2(SynStopAndWaitThread, bool(vint), SynStopAndWaitThread) &&
						REGISTER_LIGHT_FUNCTION(SynSleep, bool(vint), SynSleep);
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