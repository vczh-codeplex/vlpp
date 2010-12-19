#include <Windows.h>
#include "Threading.h"

namespace vl
{
	using namespace threading_internal;

/***********************************************************************
Thread
***********************************************************************/

	namespace threading_internal
	{
		struct ThreadData
		{
			HANDLE		threadHandle;
			DWORD		threadId;

			ThreadData()
			{
				threadHandle=NULL;
				threadId=-1;
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
		internalData=new ThreadData();
		internalData->threadHandle=CreateThread(NULL, 0, InternalThreadProcWrapper, this, CREATE_SUSPENDED, &internalData->threadId);
		threadState=Thread::NotStarted;
	}

	Thread::~Thread()
	{
		Stop();
		CloseHandle(internalData->threadHandle);
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
		if(threadState==Thread::NotStarted && internalData->threadHandle!=NULL)
		{
			if(ResumeThread(internalData->threadHandle)!=-1)
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
			if(SuspendThread(internalData->threadHandle)!=-1)
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
			if(ResumeThread(internalData->threadHandle)!=-1)
			{
				threadState=Thread::Running;
				return true;
			}
		}
		return false;
	}

	bool Thread::Stop()
	{
		if(internalData->threadHandle!=NULL)
		{
			Pause();
			threadState=Thread::Stopped;
			return true;
		}
		return false;
	}

	bool Thread::Wait()
	{
		return WaitForTime(INFINITE);
	}

	bool Thread::WaitForTime(vint ms)
	{
		if(threadState==Thread::Running)
		{
			if(WaitForSingleObject(internalData->threadHandle, (DWORD)ms)==WAIT_OBJECT_0)
			{
				return true;
			}
		}
		return false;
	}

	Thread::ThreadState Thread::GetState()
	{
		return threadState;
	}
}