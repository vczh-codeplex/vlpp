#include <Windows.h>
#include "Threading.h"

namespace vl
{
	using namespace threading_internal;

/***********************************************************************
WaitableObject
***********************************************************************/

	namespace threading_internal
	{
		struct WaitableData
		{
			HANDLE			handle;

			WaitableData(HANDLE _handle)
				:handle(_handle)
			{
			}
		};
	}

	WaitableObject::WaitableObject()
		:waitableData(0)
	{
	}

	WaitableObject::~WaitableObject()
	{
	}

	void WaitableObject::SetData(threading_internal::WaitableData* data)
	{
		waitableData=data;
	}

	bool WaitableObject::IsCreated()
	{
		return waitableData!=0;
	}

	bool WaitableObject::Wait()
	{
		return WaitForTime(INFINITE);
	}

	bool WaitableObject::WaitForTime(vint ms)
	{
		if(IsCreated())
		{
			if(WaitForSingleObject(waitableData->handle, (DWORD)ms)==WAIT_OBJECT_0)
			{
				return true;
			}
		}
		return false;
	}

/***********************************************************************
Thread
***********************************************************************/

	namespace threading_internal
	{
		struct ThreadData : public WaitableData
		{
			DWORD						id;

			ThreadData()
				:WaitableData(NULL)
			{
				id=-1;
			}
		};

		class ProceduredThread : public Thread
		{
		private:
			Thread::ThreadProcedure		procedure;
			void*						argument;
			bool						deleteAfterStopped;

		protected:
			void Run()
			{
				procedure(this, argument);
				if(deleteAfterStopped)
				{
					delete this;
				}
			}
		public:
			ProceduredThread(Thread::ThreadProcedure _procedure, void* _argument, bool _deleteAfterStopped)
				:procedure(_procedure)
				,argument(_argument)
				,deleteAfterStopped(_deleteAfterStopped)
			{
			}
		};
	}

	void InternalThreadProc(Thread* thread)
	{
		thread->Run();
		thread->threadState=Thread::Stopped;
	}

	DWORD WINAPI InternalThreadProcWrapper(LPVOID lpParameter)
	{
		InternalThreadProc((Thread*)lpParameter);
		return 0;
	}

	Thread::Thread()
	{
		internalData=new ThreadData;
		internalData->handle=CreateThread(NULL, 0, InternalThreadProcWrapper, this, CREATE_SUSPENDED, &internalData->id);
		threadState=Thread::NotStarted;
		SetData(internalData);
	}

	Thread::~Thread()
	{
		Stop();
		CloseHandle(internalData->handle);
		delete internalData;
	}

	Thread* Thread::CreateAndStart(ThreadProcedure procedure, void* argument, bool deleteAfterStopped)
	{
		if(procedure)
		{
			Thread* thread=new ProceduredThread(procedure, argument, deleteAfterStopped);
			if(thread->Start())
			{
				return thread;
			}
			else if(deleteAfterStopped)
			{
				delete thread;
			}
		}
		return 0;
	}
	
	void Thread::Sleep(vint ms)
	{
		::Sleep((DWORD)ms);
	}

	bool Thread::Start()
	{
		if(threadState==Thread::NotStarted && internalData->handle!=NULL)
		{
			if(ResumeThread(internalData->handle)!=-1)
			{
				threadState=Thread::Running;
				return true;
			}
		}
		return false;
	}

	bool Thread::Pause()
	{
		if(threadState==Thread::Running)
		{
			if(SuspendThread(internalData->handle)!=-1)
			{
				threadState=Thread::Paused;
				return true;
			}
		}
		return false;
	}

	bool Thread::Resume()
	{
		if(threadState==Thread::Paused)
		{
			if(ResumeThread(internalData->handle)!=-1)
			{
				threadState=Thread::Running;
				return true;
			}
		}
		return false;
	}

	bool Thread::Stop()
	{
		if(internalData->handle!=NULL)
		{
			Pause();
			threadState=Thread::Stopped;
			return true;
		}
		return false;
	}

	Thread::ThreadState Thread::GetState()
	{
		return threadState;
	}

/***********************************************************************
CriticalSection
***********************************************************************/

	namespace threading_internal
	{
		struct CriticalSectionData
		{
			CRITICAL_SECTION		criticalSection;
		};
	}

	CriticalSection::Scope::Scope(CriticalSection& _criticalSection)
		:criticalSection(&_criticalSection)
	{
		criticalSection->Enter();
	}

	CriticalSection::Scope::~Scope()
	{
		criticalSection->Leave();
	}
			
	CriticalSection::CriticalSection()
	{
		internalData=new CriticalSectionData;
		InitializeCriticalSection(&internalData->criticalSection);
	}

	CriticalSection::~CriticalSection()
	{
		DeleteCriticalSection(&internalData->criticalSection);
		delete internalData;
	}

	bool CriticalSection::TryEnter()
	{
		return TryEnterCriticalSection(&internalData->criticalSection)!=0;
	}

	void CriticalSection::Enter()
	{
		EnterCriticalSection(&internalData->criticalSection);
	}

	void CriticalSection::Leave()
	{
		LeaveCriticalSection(&internalData->criticalSection);
	}

/***********************************************************************
Mutex
***********************************************************************/

	namespace threading_internal
	{
		struct MutexData : public WaitableData
		{
			MutexData(HANDLE _handle)
				:WaitableData(_handle)
			{
			}
		};
	}

	Mutex::Mutex()
		:internalData(0)
	{
	}

	Mutex::~Mutex()
	{
		if(internalData)
		{
			CloseHandle(internalData->handle);
			delete internalData;
		}
	}

	bool Mutex::Create(bool owned, const WString& name)
	{
		if(IsCreated())return false;
		BOOL aOwned=owned?TRUE:FALSE;
		LPCTSTR aName=name==L""?NULL:name.Buffer();
		HANDLE handle=CreateMutex(NULL, aOwned, aName);
		if(handle)
		{
			internalData=new MutexData(handle);
			SetData(internalData);
		}
		return IsCreated();
	}

	bool Mutex::Open(bool inheritable, const WString& name)
	{
		if(IsCreated())return false;
		BOOL aInteritable=inheritable?TRUE:FALSE;
		HANDLE handle=OpenMutex(SYNCHRONIZE, aInteritable, name.Buffer());
		if(handle)
		{
			internalData=new MutexData(handle);
			SetData(internalData);
		}
		return IsCreated();
	}

	bool Mutex::Release()
	{
		if(IsCreated())
		{
			return ReleaseMutex(internalData->handle)!=0;
		}
		return false;
	}

/***********************************************************************
Semaphore
***********************************************************************/

	namespace threading_internal
	{
		struct SemaphoreData : public WaitableData
		{
			SemaphoreData(HANDLE _handle)
				:WaitableData(_handle)
			{
			}
		};
	}

	Semaphore::Semaphore()
		:internalData(0)
	{
	}

	Semaphore::~Semaphore()
	{
		if(internalData)
		{
			CloseHandle(internalData->handle);
			delete internalData;
		}
	}

	bool Semaphore::Create(vint initialCount, vint maxCount, const WString& name)
	{
		if(IsCreated())return false;
		LONG aInitial=(LONG)initialCount;
		LONG aMax=(LONG)maxCount;
		LPCTSTR aName=name==L""?NULL:name.Buffer();
		HANDLE handle=CreateSemaphore(NULL, aInitial, aMax, aName);
		if(handle)
		{
			internalData=new SemaphoreData(handle);
			SetData(internalData);
		}
		return IsCreated();
	}

	bool Semaphore::Open(bool inheritable, const WString& name)
	{
		if(IsCreated())return false;
		BOOL aInteritable=inheritable?TRUE:FALSE;
		HANDLE handle=OpenSemaphore(SYNCHRONIZE, aInteritable, name.Buffer());
		if(handle)
		{
			internalData=new SemaphoreData(handle);
			SetData(internalData);
		}
		return IsCreated();
	}

	bool Semaphore::Release()
	{
		if(IsCreated())
		{
			return Release(1)!=-1;
		}
		return false;
	}

	vint Semaphore::Release(vint count)
	{
		if(IsCreated())
		{
			LONG previous=-1;
			if(ReleaseSemaphore(internalData->handle, (LONG)count, &previous)!=0)
			{
				return (vint)previous;
			}
		}
		return false;
	}

/***********************************************************************
EventObject
***********************************************************************/

	namespace threading_internal
	{
		struct EventData : public WaitableData
		{
			EventData(HANDLE _handle)
				:WaitableData(_handle)
			{
			}
		};
	}

	EventObject::EventObject()
		:internalData(0)
	{
	}

	EventObject::~EventObject()
	{
		if(internalData)
		{
			CloseHandle(internalData->handle);
			delete internalData;
		}
	}

	bool EventObject::CreateAutoUnsignal(bool signaled, const WString& name)
	{
		if(IsCreated())return false;
		BOOL aSignaled=signaled?TRUE:FALSE;
		LPCTSTR aName=name==L""?NULL:name.Buffer();
		HANDLE handle=CreateEvent(NULL, FALSE, aSignaled, aName);
		if(handle)
		{
			internalData=new EventData(handle);
			SetData(internalData);
		}
		return IsCreated();
	}

	bool EventObject::CreateManualUnsignal(bool signaled, const WString& name)
	{
		if(IsCreated())return false;
		BOOL aSignaled=signaled?TRUE:FALSE;
		LPCTSTR aName=name==L""?NULL:name.Buffer();
		HANDLE handle=CreateEvent(NULL, TRUE, aSignaled, aName);
		if(handle)
		{
			internalData=new EventData(handle);
			SetData(internalData);
		}
		return IsCreated();
	}

	bool EventObject::Open(bool inheritable, const WString& name)
	{
		if(IsCreated())return false;
		BOOL aInteritable=inheritable?TRUE:FALSE;
		HANDLE handle=OpenEvent(SYNCHRONIZE, aInteritable, name.Buffer());
		if(handle)
		{
			internalData=new EventData(handle);
			SetData(internalData);
		}
		return IsCreated();
	}

	bool EventObject::Signal()
	{
		if(IsCreated())
		{
			return SetEvent(internalData->handle)!=0;
		}
		return false;
	}

	bool EventObject::Unsignal()
	{
		if(IsCreated())
		{
			return ResetEvent(internalData->handle)!=0;
		}
		return false;
	}
}