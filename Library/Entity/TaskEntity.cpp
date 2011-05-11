#include "TaskEntity.h"
#include "..\Exception.h"
#include <Windows.h>

namespace vl
{
	namespace entities
	{
		using namespace collections;
 
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
 
/***********************************************************************
CancellationToken
***********************************************************************/

		CancellationToken::CancellationToken()
			:cancelled(false)
		{
		}

		CancellationToken::~CancellationToken()
		{
		}

		void CancellationToken::Cancel()
		{
			cancelled=true;
		}

		bool CancellationToken::IsCancellationRequested()
		{
			return cancelled;
		}
 
/***********************************************************************
Task
***********************************************************************/

		void Task::Execute()
		{
			try
			{
				taskState=Running;
				task();
				taskExecutionEvent.Signal();
				taskResult=token->IsCancellationRequested()?Cancelled:Finished;
			}
			catch(const Exception&)
			{
				taskResult=Failed;
			}
			taskState=Done;

			SpinLock::Scope scope(taskLock);
			switch(taskResult)
			{
			case Finished:
				for(vint i=0;i<finishedTasks.Count();i++)
				{
					ThreadPool::Current()->Queue(finishedTasks[i].Cast<ITask>());
				}
				break;
			case Cancelled:
				for(vint i=0;i<cancelledTasks.Count();i++)
				{
					ThreadPool::Current()->Queue(cancelledTasks[i].Cast<ITask>());
				}
				break;
			case Failed:
				for(vint i=0;i<failedTasks.Count();i++)
				{
					ThreadPool::Current()->Queue(failedTasks[i].Cast<ITask>());
				}
				break;
			}
		}

		Task::Task(CancellationToken* _token)
			:taskState(Ready)
			,taskResult(NotExecuted)
			,token(_token)
			,owningToken(_token!=0)
		{
			taskExecutionEvent.CreateManualUnsignal(false);
		}

		Task::Task(const Func<void()>& _task, CancellationToken* _token)
			:taskState(Ready)
			,taskResult(NotExecuted)
			,token(_token)
			,owningToken(_token!=0)
		{
			task=_task;
		}

		Task::Task(const Func<void(Task*)>& _task, CancellationToken* _token)
			:taskState(Ready)
			,taskResult(NotExecuted)
			,token(_token)
			,owningToken(_token!=0)
		{
			task=Curry(_task)(this);
		}

		Task::~Task()
		{
			if(owningToken)
			{
				delete token;
			}
		}

		CancellationToken* Task::GetCancellationToken()
		{
			return token;
		}

		Task::TaskState Task::GetState()
		{
			return taskState;
		}

		Task::TaskResult Task::GetResult()
		{
			return taskResult;
		}

		void Task::ContinueWith(Ptr<Task> child, TaskResult conditions)
		{
			SpinLock::Scope scope(taskLock);
			if(conditions&Finished)
			{
				finishedTasks.Add(child);
			}
			if(conditions&Cancelled)
			{
				cancelledTasks.Add(child);
			}
			if(conditions&Failed)
			{
				failedTasks.Add(child);
			}
			
			if(taskState==Done && conditions&taskResult)
			{
				ThreadPool::Current()->Queue(child.Cast<ITask>());
			}
		}

		void Task::Wait()
		{
			taskExecutionEvent.Wait();
		}

		bool Task::WaitAll(Task** tasks, vint count)
		{
			Array<WaitableObject*> objects(count);
			for(vint i=0;i<count;i++)
			{
				objects[i]=&tasks[i]->taskExecutionEvent;
			}
			return WaitableObject::WaitAll(&objects[0], count);
		}

		bool Task::WaitAllForTime(Task** tasks, vint count, vint ms)
		{
			Array<WaitableObject*> objects(count);
			for(vint i=0;i<count;i++)
			{
				objects[i]=&tasks[i]->taskExecutionEvent;
			}
			return WaitableObject::WaitAllForTime(&objects[0], count, ms);
		}

		vint Task::WaitAny(Task** tasks, vint count)
		{
			Array<WaitableObject*> objects(count);
			for(vint i=0;i<count;i++)
			{
				objects[i]=&tasks[i]->taskExecutionEvent;
			}
			bool abandoned=false;
			return WaitableObject::WaitAny(&objects[0], count, &abandoned);
		}

		vint Task::WaitAnyForTime(Task** tasks, vint count, vint ms)
		{
			Array<WaitableObject*> objects(count);
			for(vint i=0;i<count;i++)
			{
				objects[i]=&tasks[i]->taskExecutionEvent;
			}
			bool abandoned=false;
			return WaitableObject::WaitAnyForTime(&objects[0], count, ms, &abandoned);
		}
	}
}