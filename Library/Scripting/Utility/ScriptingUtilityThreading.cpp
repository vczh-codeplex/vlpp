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
						reader.Result<vint>()=0;
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
						reader.Result<vint>()=0;
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
						reader.Result<vint>()=0;
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
						reader.Result<vint>()=0;
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

				BEGIN_FOREIGN_FUNCTION(SynCreateThread, SystemCoreThreadingPlugin)
				{
					vint label=reader.NextArgument<vint>();
					void* arguments=reader.NextArgument<void*>();
					CriticalSection::Scope scope(stack->GetInterpretor()->GetCriticalSection());
					
					if(interpretor->Symbols()->IsValidILIndex(stack->GetInterpretor()->Symbols()->GetLabel(label).key))
					{
						SynThread* thread=new SynThread(stack->GetInterpretor(), label, arguments);
						vint handle=plugin->waitables.Alloc(thread);
						thread->SetPlugin(handle, plugin);
						reader.Result<vint>()=handle;
					}
					else
					{
						reader.Result<vint>()=0;
					}
				}
				END_FOREIGN_FUNCTION

				BEGIN_FOREIGN_FUNCTION(SynStartThread, SystemCoreThreadingPlugin)
				{
					vint handle=reader.NextArgument<vint>();
					SynThread* thread=dynamic_cast<SynThread*>(plugin->waitables.GetHandle(handle));
					if(thread)
					{
						reader.Result<bool>()=thread->Start();
					}
					else
					{
						reader.Result<bool>()=false;
					}
				}
				END_FOREIGN_FUNCTION

				BEGIN_FOREIGN_FUNCTION(SynPauseAndWaitThread, SystemCoreThreadingPlugin)
				{
					vint handle=reader.NextArgument<vint>();
					SynThread* thread=dynamic_cast<SynThread*>(plugin->waitables.GetHandle(handle));
					if(thread && thread->IsRunning())
					{
						thread->SetPauseSignal();
						thread->GetPausingEvent()->Wait();
						reader.Result<bool>()=true;
					}
					else
					{
						reader.Result<bool>()=false;
					}
				}
				END_FOREIGN_FUNCTION

				BEGIN_FOREIGN_FUNCTION(SynResumeThread, SystemCoreThreadingPlugin)
				{
					vint handle=reader.NextArgument<vint>();
					SynThread* thread=dynamic_cast<SynThread*>(plugin->waitables.GetHandle(handle));
					if(thread && !thread->IsRunning())
					{
						thread->SetResumeSignal();
						reader.Result<bool>()=true;
					}
					else
					{
						reader.Result<bool>()=false;
					}
				}
				END_FOREIGN_FUNCTION

				BEGIN_FOREIGN_FUNCTION(SynStopAndWaitThread, SystemCoreThreadingPlugin)
				{
					vint handle=reader.NextArgument<vint>();
					SynThread* thread=dynamic_cast<SynThread*>(plugin->waitables.GetHandle(handle));
					if(thread && thread->IsRunning())
					{
						thread->SetStopSignal();
						thread->GetPausingEvent()->Wait();
						reader.Result<bool>()=true;
					}
					else
					{
						reader.Result<bool>()=false;
					}
				}
				END_FOREIGN_FUNCTION

				BEGIN_FOREIGN_FUNCTION(SynSleep, SystemCoreThreadingPlugin)
				{
					vint ms=reader.NextArgument<vint>();
					Thread::Sleep(ms);
				}
				END_FOREIGN_FUNCTION

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
							CriticalSection::Scope scope(interpretor->GetCriticalSection());
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
						REGISTER_FOREIGN_FUNCTION(SynWaitForTime) &&
						REGISTER_FOREIGN_FUNCTION(SynCreateThread) &&
						REGISTER_FOREIGN_FUNCTION(SynStartThread) &&
						REGISTER_FOREIGN_FUNCTION(SynPauseAndWaitThread) &&
						REGISTER_FOREIGN_FUNCTION(SynResumeThread) &&
						REGISTER_FOREIGN_FUNCTION(SynStopAndWaitThread) &&
						REGISTER_FOREIGN_FUNCTION(SynSleep);
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