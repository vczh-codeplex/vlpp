#include "TaskEntity.h"
#include <Windows.h>

namespace vl
{
	namespace entities
	{
 
/***********************************************************************
ThreadPool
***********************************************************************/

		namespace threadpoolhelpers
		{
			class FuncTask : public Object, public ITask
			{
			protected:
				Func<void()>			task;
			public:
				FuncTask(const Func<void()>& _task)
					:task(_task)
				{
				}

				void Execute()
				{
					task();
				}
			};
		}
		using namespace threadpoolhelpers;

		unsigned long __stdcall ThreadPool::ThreadPoolProc(void* parameter)
		{
			ITask* task=(ITask*)parameter;
			bool needToExecute=true;
			ThreadPool* pool=ThreadPool::Current();
			{
				SpinLock::Scope scope(pool->threadPoolLock);
				vint index=pool->queuedTasks.IndexOf(task);
				pool->executingTasks.Add(pool->queuedTasks[index]);
				pool->queuedTasks.RemoveAt(index);
				needToExecute=!pool->stopped;
			}
			if(needToExecute)
			{
				task->Execute();
			}
			{
				SpinLock::Scope scope(pool->threadPoolLock);
				pool->executingTasks.Remove(task);
				if(pool->stopped && pool->queuedTasks.Count()==0 && pool->executingTasks.Count()==0)
				{
					pool->taskCounterEvent.Signal();
				}
			}
			return 0;
		}

		ThreadPool::ThreadPool()
			:stopped(false)
		{
			taskCounterEvent.CreateManualUnsignal(false);
		}

		ThreadPool::~ThreadPool()
		{
		}

		void ThreadPool::StopAcceptingTask()
		{
			{
				SpinLock::Scope scope(threadPoolLock);
				stopped=true;
				if(queuedTasks.Count()==0 && executingTasks.Count()==0)
				{
					taskCounterEvent.Signal();
				}
			}
			taskCounterEvent.Wait();
		}

		bool ThreadPool::Queue(Ptr<ITask> task)
		{
			SpinLock::Scope scope(threadPoolLock);
			if(!stopped)
			{
				vint index=queuedTasks.Add(task);
				if(QueueUserWorkItem(&ThreadPool::ThreadPoolProc, task.Obj(), WT_EXECUTEDEFAULT))
				{
					return true;
				}
				else
				{
					queuedTasks.RemoveAt(index);
					return false;
				}
			}
			else
			{
				return false;
			}
		}

		bool ThreadPool::Queue(const Func<void()>& task)
		{
			return Queue(Ptr<ITask>(new FuncTask(task)));
		}

		vint ThreadPool::GetQueuedTaskCount()
		{
			return queuedTasks.Count();
		}

		vint ThreadPool::GetExecutingTaskCount()
		{
			return executingTasks.Count();
		}

		bool ThreadPool::IsTurnedOff()
		{
			return stopped;
		}
 
/***********************************************************************
ThreadPool::Static
***********************************************************************/

		ThreadPool* currentThreadPool=0;

		void ThreadPool::StartThreadPool()
		{
			if(!currentThreadPool)
			{
				currentThreadPool=new ThreadPool();
			}
		}

		void ThreadPool::StopThreadPool()
		{
			if(currentThreadPool)
			{
				currentThreadPool->StopAcceptingTask();
				delete currentThreadPool;
				currentThreadPool=0;
			}
		}

		ThreadPool* ThreadPool::Current()
		{
			return currentThreadPool;
		}
	}
}