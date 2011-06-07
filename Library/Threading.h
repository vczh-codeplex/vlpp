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
		struct MutexData;
		struct SemaphoreData;
		struct EventData;

		struct CriticalSectionData;
		struct ReaderWriterLockData;
		struct ConditionVariableData;
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
		
		static bool									WaitAll(WaitableObject** objects, vint count);
		static bool									WaitAllForTime(WaitableObject** objects, vint count, vint ms);
		static vint									WaitAny(WaitableObject** objects, vint count, bool* abandoned);
		static vint									WaitAnyForTime(WaitableObject** objects, vint count, vint ms, bool* abandoned);
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
进程内对象
***********************************************************************/

	class CriticalSection : public Object, public NotCopyable
	{
	private:
#ifdef VCZH_NO_OLD_OS
		friend class ConditionVariable;
#endif
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

#ifdef VCZH_NO_OLD_OS

	class ReaderWriterLock : public Object, public NotCopyable
	{
	private:
		friend class ConditionVariable;
		threading_internal::ReaderWriterLockData*	internalData;
	public:
		ReaderWriterLock();
		~ReaderWriterLock();

		bool										TryEnterReader();
		void										EnterReader();
		void										LeaveReader();
		bool										TryEnterWriter();
		void										EnterWriter();
		void										LeaveWriter();
	public:
		class ReaderScope : public Object, public NotCopyable
		{
		private:
			ReaderWriterLock*						lock;
		public:
			ReaderScope(ReaderWriterLock& _lock);
			~ReaderScope();
		};
		
		class WriterScope : public Object, public NotCopyable
		{
		private:
			ReaderWriterLock*						lock;
		public:
			WriterScope(ReaderWriterLock& _lock);
			~WriterScope();
		};
	};

	class ConditionVariable : public Object, public NotCopyable
	{
	private:
		threading_internal::ConditionVariableData*	internalData;
	public:
		ConditionVariable();
		~ConditionVariable();

		bool										SleepWith(CriticalSection& cs);
		bool										SleepWithForTime(CriticalSection& cs, vint ms);
		bool										SleepWithReader(ReaderWriterLock& lock);
		bool										SleepWithReaderForTime(ReaderWriterLock& lock, vint ms);
		bool										SleepWithWriter(ReaderWriterLock& lock);
		bool										SleepWithWriterForTime(ReaderWriterLock& lock, vint ms);
		void										WakeOnePending();
		void										WakeAllPendings();
	};
#endif

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