/***********************************************************************
Vczh Library++ 3.0
Developer: ³Âè÷å«(vczh)
Framework::Entity

Classes:
***********************************************************************/

#ifndef VCZH_ENTITY_TASKENTITY
#define VCZH_ENTITY_TASKENTITY

#include "..\Basic.h"
#include "..\Pointer.h"
#include "..\Function.h"
#include "..\Threading.h"
#include "..\Collections\List.h"

namespace vl
{
	namespace entities
	{
 
/***********************************************************************
ThreadPool
***********************************************************************/

		class ITask : public Interface
		{
		public:
			virtual void							Execute()=0;
		};

		class ThreadPool
		{
		private:
			static unsigned long __stdcall			ThreadPoolProc(void* parameter);
		private:
			SpinLock								threadPoolLock;
			EventObject								taskCounterEvent;
			collections::List<Ptr<ITask>>			queuedTasks;
			collections::List<Ptr<ITask>>			executingTasks;
			volatile bool							stopped;

			ThreadPool();
			~ThreadPool();

			void									StopAcceptingTask();
		public:

			bool									Queue(Ptr<ITask> task);
			bool									Queue(const Func<void()>& task);
			vint									GetQueuedTaskCount();
			vint									GetExecutingTaskCount();
			bool									IsTurnedOff();

			static void								StartThreadPool();
			static void								StopThreadPool();
			static ThreadPool*						Current();
		};
 
/***********************************************************************
Task
***********************************************************************/

		class CancellationToken : public Object
		{
		protected:
			bool									cancelled;
		public:
			CancellationToken();
			~CancellationToken();

			void									Cancel();
			bool									IsCancellationRequested();
		};

		class Task : public Object, protected ITask
		{
			friend class Ptr<Task>;
		public:
			enum TaskState
			{
				Ready,
				Running,
				Done,
			};

			enum TaskResult
			{
				NotExecuted=0,
				Finished=1,
				Cancelled=2,
				Failed=4,
			};
		protected:
			SpinLock								taskLock;
			EventObject								taskExecutionEvent;
			TaskState								taskState;
			TaskResult								taskResult;

			CancellationToken*						token;
			bool									owningToken;
			Func<void()>							task;

			collections::List<Ptr<Task>>			finishedTasks;
			collections::List<Ptr<Task>>			cancelledTasks;
			collections::List<Ptr<Task>>			failedTasks;

			void									Execute();

			Task(CancellationToken* _token);
		public:
			Task(const Func<void()>& _task, CancellationToken* _token=0);
			Task(const Func<void(Task*)>& _task, CancellationToken* _token=0);
			~Task();

			CancellationToken*						GetCancellationToken();
			TaskState								GetState();
			TaskResult								GetResult();
			void									ContinueWith(Ptr<Task> child, TaskResult conditions);

			void									Wait();

			static bool								WaitAll(Task** tasks, vint count);
			static bool								WaitAllForTime(Task** tasks, vint count, vint ms);
			static vint								WaitAny(Task** tasks, vint count);
			static vint								WaitAnyForTime(Task** tasks, vint count, vint ms);
		};

		template<typename T>
		class CalTask : public Task
		{
		protected:
			T										calculationResult;

			void RunTask(Func<T()> task)
			{
				calculationResult=task();
			}
		public:
			CalTask(const Func<T()>& _task, CancellationToken* _token=0)
				:Task(_token)
			{
				task=Curry(Func<void()>(this, CalTask<T>::RunTask))(_task);
			}

			CalTask(const Func<T(CalTask*)>& _task, CancellationToken* _token=0)
				:Task(_token)
			{
				task=Curry(Func<void()>(this, CalTask<T>::RunTask))(Curry(_task)(this));
			}

			const T& GetCalculationResult()
			{
				return taskResult==Finished?calculationResult:*(T*)0;
			}
		};
	}
}

#endif