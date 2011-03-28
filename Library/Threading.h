/***********************************************************************
Vczh Library++ 3.0
Developer: 陈梓瀚(vczh)
Framework::Threading

Classes:
	Thread										：线程
	CriticalSection
	Mutex
	Semaphore
	EventObject
***********************************************************************/

#ifndef VCZH_THREADING
#define VCZH_THREADING

#include "Basic.h"
#include "String.h"

namespace vl
{

/***********************************************************************
内核模式对象
***********************************************************************/

	namespace threading_internal
	{
		struct WaitableData;
		struct ThreadData;
		struct CriticalSectionData;
		struct MutexData;
		struct SemaphoreData;
		struct EventData;
	}

	class WaitableObject : public Object, public NotCopyable
	{
	private:
		threading_internal::WaitableData*			waitableData;
	protected:

		WaitableObject();
		void										SetData(threading_internal::WaitableData* data);
	public:

		bool										IsCreated();
		bool										Wait();
		bool										WaitForTime(vint ms);
	};

	class Thread : public WaitableObject
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
		static vint									GetCPUCount();

		bool										Start();
		bool										Pause();
		bool										Resume();
		bool										Stop();
		ThreadState									GetState();
		void										SetCPU(vint index);
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

	public:
		class Scope : public Object, public NotCopyable
		{
		private:
			CriticalSection*						criticalSection;
		public:
			Scope(CriticalSection& _criticalSection);
			~Scope();
		};
	};

	class Mutex : public WaitableObject
	{
	private:
		threading_internal::MutexData*				internalData;
	public:
		Mutex();
		~Mutex();

		bool										Create(bool owned=false, const WString& name=L"");
		bool										Open(bool inheritable, const WString& name);

		bool										Release();
	};

	class Semaphore : public WaitableObject
	{
	private:
		threading_internal::SemaphoreData*			internalData;
	public:
		Semaphore();
		~Semaphore();

		bool										Create(vint initialCount, vint maxCount, const WString& name=L"");
		bool										Open(bool inheritable, const WString& name);

		bool										Release();
		vint										Release(vint count);
	};

	class EventObject : public WaitableObject
	{
	private:
		threading_internal::EventData*				internalData;
	public:
		EventObject();
		~EventObject();

		bool										CreateAutoUnsignal(bool signaled, const WString& name=L"");
		bool										CreateManualUnsignal(bool signaled, const WString& name=L"");
		bool										Open(bool inheritable, const WString& name);

		bool										Signal();
		bool										Unsignal();
	};

/***********************************************************************
用户模式对象
***********************************************************************/

	typedef long LockedInt;

	class SpinLock : public Object, public NotCopyable
	{
	protected:
		LockedInt									token;
	public:
		SpinLock();
		~SpinLock();

		bool										TryEnter();
		void										Enter();
		void										Leave();

	public:
		class Scope : public Object, public NotCopyable
		{
		private:
			SpinLock*								spinLock;
		public:
			Scope(SpinLock& _spinLock);
			~Scope();
		};
	};
}

#endif