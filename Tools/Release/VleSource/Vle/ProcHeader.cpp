#include "..\..\..\..\Library\Basic.h"
#include "..\..\..\..\Library\Console.h"
#include "..\..\..\..\Library\Collections\List.h"

using namespace vl;
using namespace vl::console;
using namespace vl::collections;

extern wchar_t* ProcVersion();

bool ProcHeader(const Array<WString>& args)
{
	Console::Write(L"Welcome to Vczh Script Header Extracter ");
	Console::WriteLine(ProcVersion());
	return true;
}