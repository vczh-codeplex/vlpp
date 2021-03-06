#include "..\..\Library\UnitTest\UnitTest.h"
#include "..\..\Library\Threading.h"
#include "..\..\Library\Collections\List.h"
#include "..\..\Library\Collections\OperationForEach.h"

using namespace vl;
using namespace vl::collections;

/***********************************************************************
Thread
***********************************************************************/

namespace mynamespace
{
	struct ThreadData
	{
		bool modified;

		ThreadData()
			:modified(false)
		{
		}
	};

	void SimpleThreadProc(Thread* thread, void* argument)
	{
		Thread::Sleep(1000);
		((ThreadData*)argument)->modified=true;
	}
}
using namespace mynamespace;

TEST_CASE(TestSimpleThread)
{
	ThreadData data;
	Thread* thread=Thread::CreateAndStart(SimpleThreadProc, &data, false);
	TEST_ASSERT(thread->GetState()==Thread::Running);
	TEST_ASSERT(thread->Wait()==true);
	TEST_ASSERT(thread->GetState()==Thread::Stopped);
	delete thread;
	TEST_ASSERT(data.modified==true);
}

TEST_CASE(TestPauseAndStopThread)
{
	{
		ThreadData data;
		Thread* thread=Thread::CreateAndStart(SimpleThreadProc, &data, false);
		TEST_ASSERT(thread->GetState()==Thread::Running);
		TEST_ASSERT(thread->Pause()==true);
		TEST_ASSERT(thread->GetState()==Thread::Paused);
		delete thread;
		TEST_ASSERT(data.modified==false);
	}
	{
		ThreadData data;
		Thread* thread=Thread::CreateAndStart(SimpleThreadProc, &data, false);
		TEST_ASSERT(thread->GetState()==Thread::Running);
		TEST_ASSERT(thread->Stop()==true);
		TEST_ASSERT(thread->GetState()==Thread::Stopped);
		delete thread;
		TEST_ASSERT(data.modified==false);
	}
}

TEST_CASE(TestPauseAndResumeThread)
{
	ThreadData data;
	Thread* thread=Thread::CreateAndStart(SimpleThreadProc, &data, false);
	TEST_ASSERT(thread->GetState()==Thread::Running);
	TEST_ASSERT(thread->Pause()==true);
	TEST_ASSERT(thread->GetState()==Thread::Paused);
	TEST_ASSERT(thread->Resume()==true);
	TEST_ASSERT(thread->GetState()==Thread::Running);
	TEST_ASSERT(thread->Wait()==true);
	TEST_ASSERT(thread->GetState()==Thread::Stopped);
	delete thread;
	TEST_ASSERT(data.modified==true);
}

/***********************************************************************
Mutex
***********************************************************************/

namespace mynamespace
{
	struct Mutex_ThreadData
	{
		Mutex				mutex;
		volatile vint		counter;

		Mutex_ThreadData()
			:counter(0)
		{
			TEST_ASSERT(mutex.Create(true));
		}
	};

	void Mutex_ThreadProc(Thread* thread, void* argument)
	{
		Mutex_ThreadData* data=(Mutex_ThreadData*)argument;
		{
			TEST_ASSERT(data->mutex.Wait());
			data->counter++;
			TEST_ASSERT(data->mutex.Release());
		}
	}
}
using namespace mynamespace;

TEST_CASE(TestMutex)
{
	Mutex_ThreadData data;
	List<Thread*> threads;
	{
		for(vint i=0;i<10;i++)
		{
			threads.Add(Thread::CreateAndStart(Mutex_ThreadProc, &data, false));
		}
		Thread::Sleep(1000);
		TEST_ASSERT(data.counter==0);
		TEST_ASSERT(data.mutex.Release());
	}
	FOREACH(Thread*, thread, threads.Wrap())
	{
		thread->Wait();
		TEST_ASSERT(thread->GetState()==Thread::Stopped);
		delete thread;
	}
	TEST_ASSERT(data.counter==10);
}

/***********************************************************************
Semaphore
***********************************************************************/

namespace mynamespace
{
	struct Semaphore_ThreadData
	{
		CriticalSection		cs;
		Semaphore			semaphore;
		volatile vint		counter;

		Semaphore_ThreadData(vint max)
			:counter(0)
		{
			TEST_ASSERT(semaphore.Create(0, max));
		}
	};

	void Semaphore_ThreadProc(Thread* thread, void* argument)
	{
		Semaphore_ThreadData* data=(Semaphore_ThreadData*)argument;
		TEST_ASSERT(data->semaphore.Wait());
		{
			CriticalSection::Scope lock(data->cs);
			data->counter++;
		}
	}
}
using namespace mynamespace;

TEST_CASE(TestSemaphore)
{
	Semaphore_ThreadData data(10);
	List<Thread*> threads;
	{
		for(vint i=0;i<10;i++)
		{
			threads.Add(Thread::CreateAndStart(Semaphore_ThreadProc, &data, false));
		}
		Thread::Sleep(1000);
		TEST_ASSERT(data.counter==0);
	}
	{
		data.semaphore.Release(3);
		Thread::Sleep(100);
		CriticalSection::Scope lock(data.cs);
		TEST_ASSERT(data.counter==3);
	}
	{
		data.semaphore.Release(4);
		Thread::Sleep(100);
		CriticalSection::Scope lock(data.cs);
		TEST_ASSERT(data.counter==7);
	}
	{
		data.semaphore.Release(3);
		Thread::Sleep(100);
		CriticalSection::Scope lock(data.cs);
		TEST_ASSERT(data.counter==10);
	}
	FOREACH(Thread*, thread, threads.Wrap())
	{
		thread->Wait();
		TEST_ASSERT(thread->GetState()==Thread::Stopped);
		delete thread;
	}
}

/***********************************************************************
EventObject
***********************************************************************/

namespace mynamespace
{
	struct Event_ThreadData
	{
		CriticalSection		cs;
		EventObject			eventObject;
		volatile vint		counter;

		Event_ThreadData()
			:counter(0)
		{
			TEST_ASSERT(eventObject.CreateManualUnsignal(false));
		}
	};

	void Event_ThreadProc(Thread* thread, void* argument)
	{
		Event_ThreadData* data=(Event_ThreadData*)argument;
		TEST_ASSERT(data->eventObject.Wait());
		{
			CriticalSection::Scope lock(data->cs);
			data->counter++;
		}
	}
}
using namespace mynamespace;

TEST_CASE(TestEventObject)
{
	Event_ThreadData data;
	List<Thread*> threads;
	{
		for(vint i=0;i<10;i++)
		{
			threads.Add(Thread::CreateAndStart(Event_ThreadProc, &data, false));
		}
		Thread::Sleep(1000);
		TEST_ASSERT(data.counter==0);
		TEST_ASSERT(data.eventObject.Signal());
	}
	FOREACH(Thread*, thread, threads.Wrap())
	{
		thread->Wait();
		TEST_ASSERT(thread->GetState()==Thread::Stopped);
		delete thread;
	}
	TEST_ASSERT(data.counter==10);
}

/***********************************************************************
CriticalSection
***********************************************************************/

namespace mynamespace
{
	struct CS_ThreadData
	{
		CriticalSection			cs;
		volatile vint			counter;

		CS_ThreadData()
			:counter(0)
		{
		}
	};

	void CS_ThreadProc(Thread* thread, void* argument)
	{
		CS_ThreadData* data=(CS_ThreadData*)argument;
		{
			CriticalSection::Scope lock(data->cs);
			data->counter++;
		}
	}
}
using namespace mynamespace;

TEST_CASE(TestCriticalSection)
{
	CS_ThreadData data;
	List<Thread*> threads;
	{
		CriticalSection::Scope lock(data.cs);
		for(vint i=0;i<10;i++)
		{
			threads.Add(Thread::CreateAndStart(CS_ThreadProc, &data, false));
		}
		Thread::Sleep(1000);
		TEST_ASSERT(data.counter==0);
	}
	FOREACH(Thread*, thread, threads.Wrap())
	{
		thread->Wait();
		TEST_ASSERT(thread->GetState()==Thread::Stopped);
		delete thread;
	}
	TEST_ASSERT(data.cs.TryEnter());
	TEST_ASSERT(data.counter==10);
}

#ifdef VCZH_NO_OLD_OS

/***********************************************************************
ReaderWriterLock
***********************************************************************/

namespace mynamespace
{
	struct SRW_ThreadData
	{
		EventObject				ev;
		SpinLock				sl;
		ReaderWriterLock		lock;
		volatile vint			counter;

		SRW_ThreadData()
			:counter(0)
		{
			ev.CreateManualUnsignal(false);
		}
	};

	void SRW_ReaderProc(Thread* thread, void* argument)
	{
		SRW_ThreadData* data=(SRW_ThreadData*)argument;
		data->ev.Wait();
		for(vint i=0;i<10;i++)
		{
			ReaderWriterLock::ReaderScope srw(data->lock);
			SpinLock::Scope sl(data->sl);
			data->counter++;
		}
	}

	void SRW_WriterProc(Thread* thread, void* argument)
	{
		SRW_ThreadData* data=(SRW_ThreadData*)argument;
		data->ev.Wait();
		for(vint i=0;i<10;i++)
		{
			ReaderWriterLock::WriterScope srw(data->lock);
			SpinLock::Scope sl(data->sl);
			data->counter++;
		}
	}
}
using namespace mynamespace;

TEST_CASE(TestReaderWriterLock)
{
	SRW_ThreadData data;
	List<Thread*> threads;
	{
		threads.Add(Thread::CreateAndStart(SRW_WriterProc, &data, false));
		for(vint i=0;i<9;i++)
		{
			threads.Add(Thread::CreateAndStart(SRW_ReaderProc, &data, false));
		}
		Thread::Sleep(1000);
		TEST_ASSERT(data.counter==0);
	}
	data.ev.Signal();
	FOREACH(Thread*, thread, threads.Wrap())
	{
		thread->Wait();
		TEST_ASSERT(thread->GetState()==Thread::Stopped);
		delete thread;
	}
	TEST_ASSERT(data.counter==100);
}

#endif

/***********************************************************************
SpinLock
***********************************************************************/

namespace mynamespace
{
	struct SL_ThreadData
	{
		SpinLock				lock;
		volatile vint			counter;

		SL_ThreadData()
			:counter(0)
		{
		}
	};

	void SL_ThreadProc(Thread* thread, void* argument)
	{
		SL_ThreadData* data=(SL_ThreadData*)argument;
		{
			SpinLock::Scope lock(data->lock);
			data->counter++;
		}
	}
}
using namespace mynamespace;

TEST_CASE(TestSpinLock)
{
	SL_ThreadData data;
	List<Thread*> threads;
	{
		SpinLock::Scope lock(data.lock);
		for(vint i=0;i<10;i++)
		{
			threads.Add(Thread::CreateAndStart(SL_ThreadProc, &data, false));
		}
		Thread::Sleep(1000);
		TEST_ASSERT(data.counter==0);
	}
	FOREACH(Thread*, thread, threads.Wrap())
	{
		thread->Wait();
		TEST_ASSERT(thread->GetState()==Thread::Stopped);
		delete thread;
	}
	TEST_ASSERT(data.lock.TryEnter());
	TEST_ASSERT(data.counter==10);
}