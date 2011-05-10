#include "..\..\Library\UnitTest\UnitTest.h"
#include "..\..\Library\Entity\TaskEntity.h"

using namespace vl;
using namespace vl::entities;
using namespace vl::collections;
using namespace vl::unittest;

namespace TestEntityHelper
{
	SpinLock lock;
	vint counter=0;

	void OneSecondPrintingTask(vint index)
	{
		{
			SpinLock::Scope scope(lock);
			UnitTest::PrintInfo(L"    Start executing task "+itow(index));
		}
		Thread::Sleep(1000);
		{
			SpinLock::Scope scope(lock);
			UnitTest::PrintInfo(L"    Stop executing task "+itow(index));
			counter++;
		}
	}
}
using namespace TestEntityHelper;

TEST_CASE(TestEntity_ThreadPool)
{
	{
		counter=0;
		UnitTest::PrintInfo(L"Queue 10 items and notify stop immediately.");
		ThreadPool::StartThreadPool();
		for(vint i=0;i<10;i++)
		{
			TEST_ASSERT(ThreadPool::Current()->Queue(Curry(OneSecondPrintingTask)(i))==true);
		}
		ThreadPool::StopThreadPool();
		UnitTest::PrintInfo(itow(counter)+L" tasks executed.");
	}
	{
		counter=0;
		UnitTest::PrintInfo(L"Queue 10 items and notify stop after 0.5 second immediately.");
		ThreadPool::StartThreadPool();
		for(vint i=0;i<10;i++)
		{
			TEST_ASSERT(ThreadPool::Current()->Queue(Curry(OneSecondPrintingTask)(i))==true);
		}
		Thread::Sleep(500);
		ThreadPool::StopThreadPool();
		UnitTest::PrintInfo(itow(counter)+L" tasks executed.");
		TEST_ASSERT(counter==10);
	}
}