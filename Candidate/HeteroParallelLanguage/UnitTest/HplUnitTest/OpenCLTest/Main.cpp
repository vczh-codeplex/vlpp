#define _CRTDBG_MAP_ALLOC
#include <stdlib.h>
#include <crtdbg.h>
#include <windows.h>
#include "..\..\..\..\..\Library\Basic.h"

using namespace vl;

int wmain(vint argc , wchar_t* args[])
{
	_CrtDumpMemoryLeaks();
	return 0;
}

//------------you can organize your cases in different cpp files------------------//

#include "..\..\..\..\..\Library\UnitTest\UnitTest.h"

using namespace vl::unittest;

TEST_CASE(MyTestCase)
{
	TEST_ASSERT(true);
}