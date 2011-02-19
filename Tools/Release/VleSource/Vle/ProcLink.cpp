#include "Common.h"

bool ProcLink(const Array<WString>& args)
{
	Console::Write(L"Welcome to Vczh Script Linker ");
	Console::WriteLine(ProcVersion());
	return true;
}