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
		((ThreadData*)argument)->modified=true;
	}
}
using namespace mynamespace;

TEST_CASE(TestSimpleThread)
{
	ThreadData data;
	Thread* thread=Thread::CreateAndStart(SimpleThreadProc, &data, false);
	thread->Wait();
	delete thread;
	TEST_ASSERT(data.modified==true);
}