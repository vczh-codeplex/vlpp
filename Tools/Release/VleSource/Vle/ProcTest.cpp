#include "Common.h"

bool ProcTest(const Array<WString>& args)
{
	Console::Write(L"Welcome to Vczh Script Unit Test Launcher ");
	Console::WriteLine(ProcVersion());
	return true;
}