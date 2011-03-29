#include "..\..\..\Library\Console.h"
#include "..\..\..\Library\Pointer.h"
#include "..\..\..\Library\Stream\FileStream.h"
#include "..\..\..\Library\Stream\Accessor.h"
#include "..\..\..\Library\Stream\CharFormat.h"

using namespace vl;
using namespace vl::stream;
using namespace vl::console;

int wmain(int argc, wchar_t* argv[])
{
	Console::WriteLine(L"Welcome to DocWrite.");
	return 0;
}