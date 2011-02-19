#include "Common.h"

bool ProcHeader(const Array<WString>& args)
{
	Console::Write(L"Welcome to Vczh Script Header Extracter ");
	Console::WriteLine(ProcVersion());
	return true;
}