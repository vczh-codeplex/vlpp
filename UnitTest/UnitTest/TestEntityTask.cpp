#include "..\..\Library\UnitTest\UnitTest.h"
#include "..\..\Library\Entity\TaskEntity.h"

using namespace vl;
using namespace vl::entities;
using namespace vl::collections;
using namespace vl::unittest;

/***********************************************************************
ThreadPool
***********************************************************************/

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
		ThreadPool::StopThreadPool(true);
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
		ThreadPool::StopThreadPool(true);
		UnitTest::PrintInfo(itow(counter)+L" tasks executed.");
		TEST_ASSERT(counter==10);
	}
}

TEST_CASE(TestEntity_ThreadPoolStopTrue)
{
	ThreadPool::StartThreadPool();
	ThreadPool::StopThreadPool(true);
}

TEST_CASE(TestEntity_ThreadPoolStopFalse)
{
	ThreadPool::StartThreadPool();
	ThreadPool::StopThreadPool(false);
}

/***********************************************************************
Task
***********************************************************************/

namespace TestEntityHelper
{
	void SimpleTask(const WString& name)
	{
		{
			SpinLock::Scope scope(lock);
			UnitTest::PrintInfo(L"    Start executing task "+name);
		}
		Thread::Sleep(1000);
		{
			SpinLock::Scope scope(lock);
			UnitTest::PrintInfo(L"    Stop executing task "+name);
		}
	}
}
using namespace TestEntityHelper;

TEST_CASE(TestEntity_RunSimpleTask)
{
	Ptr<Task> taskA=new Task(Curry(SimpleTask)(L"A"));
	Ptr<Task> taskB=new Task(Curry(SimpleTask)(L"B"));
	Ptr<Task> taskC=new Task(Curry(SimpleTask)(L"C"));
	Ptr<Task> taskD=new Task(Curry(SimpleTask)(L"D"));
	Ptr<Task> taskE=new Task(Curry(SimpleTask)(L"E"));
	Ptr<Task> taskF=new Task(Curry(SimpleTask)(L"F"));
	Ptr<Task> taskG=new Task(Curry(SimpleTask)(L"G"));

	taskA->ContinueWith(taskB, Task::Finished);
	taskA->ContinueWith(taskC, Task::Finished);
	taskA->ContinueWith(taskD, Task::Finished);
	taskB->ContinueWith(taskE, Task::Finished);
	taskE->ContinueWith(taskF, Task::Finished);
	taskC->ContinueWith(taskG, Task::Finished);

	ThreadPool::StartThreadPool();
	ThreadPool::Current()->Queue(taskA);
	ThreadPool::StopThreadPool(false);
}

/***********************************************************************
Task
***********************************************************************/

namespace TestEntityHelper
{
	vint SquareTask(vint input)
	{
		{
			SpinLock::Scope scope(lock);
			UnitTest::PrintInfo(L"    Start executing task square("+itow(input)+L")");
		}
		Thread::Sleep(1000);
		{
			SpinLock::Scope scope(lock);
			UnitTest::PrintInfo(L"    Stop executing task square("+itow(input)+L") = "+itow(input*input));
		}
		return input*input;
	}

	vint SquareTask2(vint input, Task* self)
	{
		if(input<5)
		{
			self->ContinueWith(new CalTask<vint>(Curry(SquareTask2)(input+1)), Task::Finished);
			self->ContinueWith(new CalTask<vint>(Curry(SquareTask2)(input+2)), Task::Finished);
			self->ContinueWith(new CalTask<vint>(Curry(SquareTask2)(input+3)), Task::Finished);
		}
		{
			SpinLock::Scope scope(lock);
			UnitTest::PrintInfo(L"    Start executing task square2("+itow(input)+L")");
		}
		Thread::Sleep(1000);
		{
			SpinLock::Scope scope(lock);
			UnitTest::PrintInfo(L"    Stop executing task square2("+itow(input)+L") = "+itow(input*input));
		}
		return input*input;
	}
}
using namespace TestEntityHelper;

TEST_CASE(TestEntity_RunSquareTask)
{
	Ptr<CalTask<vint>> taskA=new CalTask<vint>(Curry(SquareTask)(1));
	Ptr<CalTask<vint>> taskB=new CalTask<vint>(Curry(SquareTask)(2));
	Ptr<CalTask<vint>> taskC=new CalTask<vint>(Curry(SquareTask)(3));
	Ptr<CalTask<vint>> taskD=new CalTask<vint>(Curry(SquareTask)(4));
	Ptr<CalTask<vint>> taskE=new CalTask<vint>(Curry(SquareTask)(5));
	Ptr<CalTask<vint>> taskF=new CalTask<vint>(Curry(SquareTask)(6));
	Ptr<CalTask<vint>> taskG=new CalTask<vint>(Curry(SquareTask)(7));

	taskA->ContinueWith(taskB, Task::Finished);
	taskA->ContinueWith(taskC, Task::Finished);
	taskA->ContinueWith(taskD, Task::Finished);
	taskB->ContinueWith(taskE, Task::Finished);
	taskE->ContinueWith(taskF, Task::Finished);
	taskC->ContinueWith(taskG, Task::Finished);

	ThreadPool::StartThreadPool();
	ThreadPool::Current()->Queue(taskA);
	ThreadPool::StopThreadPool(false);

	TEST_ASSERT(taskA->GetCalculationResult()==1);
	TEST_ASSERT(taskB->GetCalculationResult()==4);
	TEST_ASSERT(taskC->GetCalculationResult()==9);
	TEST_ASSERT(taskD->GetCalculationResult()==16);
	TEST_ASSERT(taskE->GetCalculationResult()==25);
	TEST_ASSERT(taskF->GetCalculationResult()==36);
	TEST_ASSERT(taskG->GetCalculationResult()==49);
}

TEST_CASE(TestEntity_RunSquareTask2)
{
	ThreadPool::StartThreadPool();
	ThreadPool::Current()->Queue(Ptr<CalTask<vint>>(new CalTask<vint>(Curry(SquareTask2)(1))));
	ThreadPool::StopThreadPool(false);
}