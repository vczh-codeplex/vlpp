/***********************************************************************
Vczh Library++ 3.0
Developer: ³Âè÷å«(vczh)
Framework::Threading

Classes:
	Thread										£ºÏß³Ì
***********************************************************************/

#ifndef VCZH_THREADING
#define VCZH_THREADING

#include "Basic.h"

namespace vl
{
	namespace threading_internal
	{
		struct ThreadData;
	}

	class Thread : public Object, public NotCopyable
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
		threading_internal::ThreadData*			internalData;
		volatile ThreadState					threadState;

	protected:

		virtual void							Run()=0;
	public:
		Thread();
		~Thread();

		static Thread*							CreateAndStart(ThreadProcedure procedure, void* argument=0, bool deleteAfterStopped=true);
		static void								Sleep(vint ms);

		bool									Start();
		bool									Pause();
		bool									Resume();
		bool									Wait();
		bool									WaitForTime(vint ms);
		ThreadState								GetState();
	};
}

#endif