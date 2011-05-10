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
	}
}

#endif