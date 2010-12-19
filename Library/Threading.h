/***********************************************************************
Vczh Library++ 3.0
Developer: ³Âè÷å«(vczh)
Framework::Threading

Classes:
	Thread										£ºÏß³Ì
***********************************************************************/

#ifndef VCZH_THREADING
#define VCZH_THREADING

#include "Basic.h"
#include "String.h"

namespace vl
{
	namespace threading_internal
	{
		struct ThreadData;
		struct CriticalSectionData;
		struct MutexData;
		struct SemaphoreData;
		struct EventData;
	}

	class Thread : public Object, public NotCopyable
	{
		friend void InternalThreadProc(Thread* thread);
	public:
		enum ThreadState
		{
			NotStarted,
			Running,
			Paused,
			Stopped
		};

		typedef void(*ThreadProcedure)(Thread*, void*);
	private:
		threading_internal::ThreadData*				internalData;
		volatile ThreadState						threadState;

	protected:

		virtual void								Run()=0;
	public:
		Thread();
		~Thread();

		static Thread*								CreateAndStart(ThreadProcedure procedure, void* argument=0, bool deleteAfterStopped=true);
		static void									Sleep(vint ms);

		bool										Start();
		bool										Pause();
		bool										Resume();
		bool										Stop();
		bool										Wait();
		bool										WaitForTime(vint ms);
		ThreadState									GetState();
	};

	class CriticalSection : public Object, public NotCopyable
	{
	private:
		threading_internal::CriticalSectionData*	internalData;
	public:
		CriticalSection();
		~CriticalSection();

		bool										TryEnter();
		void										Enter();
		void										Leave();
	};

	class Mutex : public Object, public NotCopyable
	{
	private:
		threading_internal::MutexData*				internalData;
	public:
		Mutex();
		~Mutex();

		bool										Create(bool owned=false, const WString& name=L"");
		bool										Open(bool inheritable, const WString& name);

		bool										IsCreated();
		bool										Wait();
		bool										WaitForTime(vint ms);
		bool										Release();
	};

	class Semaphore : public Object, public NotCopyable
	{
	private:
		threading_internal::SemaphoreData*			internalData;
	public:
		Semaphore();
		~Semaphore();

		bool										Create(vint initialCount, vint maxCount, const WString& name=L"");
		bool										Open(bool inheritable, const WString& name);

		bool										IsCreated();
		bool										Wait();
		bool										WaitForTime(vint ms);
		bool										Release();
		vint										Release(vint count);
	};

	class EventObject : public Object, public NotCopyable
	{
	private:
		threading_internal::EventData*				internalData;
	public:
		EventObject();
		~EventObject();
	};
}

#endif