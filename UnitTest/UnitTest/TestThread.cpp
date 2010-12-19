#include "..\..\Library\UnitTest\UnitTest.h"
#include "..\..\Library\Threading.h"

using namespace vl;

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