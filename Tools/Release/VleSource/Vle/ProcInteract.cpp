#include "Common.h"

bool ProcInteract(const Array<WString>& args)
{
	Console::Write(L"Welcome to Vczh Script Interact ");
	Console::WriteLine(ProcVersion());
	return true;
}