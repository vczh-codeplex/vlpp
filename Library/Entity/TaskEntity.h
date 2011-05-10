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
		public:
			bool									Cancel();
			bool									IsCancellationRequested();
		};

		class Task : public Object, protected ITask
		{
		public:
			enum TaskState
			{
				Ready,
				Running,
				Done,
			};

			enum TaskResult
			{
				Finished=1,
				Cancelled=2,
				Failed=4,
			};
		protected:
			SpinLock								taskDataLock;
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
			bool									ContinueWith(Ptr<Task> child, TaskResult conditions);

			void									Start();
			void									Wait();

			static void								WaitAll(Task* tasks, vint count);
			static vint								WaitAny(Task* tasks, vint count);
		};

		template<typename T>
		class CalTask : public Task
		{
		protected:
			T										calculationResult;
		public:
			CalTask(const Func<T()>& task, CancellationToken* token=0);
			CalTask(const Func<T(CalTask*)>& task, CancellationToken* token=0);

			const T&								GetCalculationResult();
		};
	}
}

#endif