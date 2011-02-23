#include "Common.h"

extern bool ProcMake(const Array<WString>& args);
extern bool ProcHeader(const Array<WString>& args);
extern bool ProcLink(const Array<WString>& args);
extern bool ProcTest(const Array<WString>& args);
extern bool ProcSimulate(const Array<WString>& args);
extern bool ProcInteract(const Array<WString>& args);

/*
make "..\Test Files\Make\Make.txt"
make "..\..\vlpp\ScriptSample\CrossAssemblyInvoking\Expression\Expression.Make.txt"
*/

int wmain(vint argc , wchar_t* args[])
{
	try
	{
		Array<WString> arguments(argc-1);
		for(vint i=1;i<argc;i++)arguments[i-1]=args[i];

		if(arguments.Count()>=1)
		{
			WString proc=wupper(arguments[0]);
			if(proc==L"MAKE")
			{
				if(ProcMake(arguments))return 0;
			}
			else if(proc==L"HEADER")
			{
				if(ProcHeader(arguments))return 0;
			}
			else if(proc==L"LINK")
			{
				if(ProcLink(arguments))return 0;
			}
			else if(proc==L"TEST")
			{
				if(ProcTest(arguments))return 0;
			}
			else if(proc==L"SIMULATE")
			{
				if(ProcSimulate(arguments))return 0;
			}
			else if(proc==L"INTERACT")
			{
				if(ProcInteract(arguments))return 0;
			}
		}
	}
	catch(const LanguageMakerException& e)
	{
		Console::SetColor(true, false, false, true);
		Console::WriteLine(L"Exception: "+e.Message());
		for(vint i=0;i<e.errors->Count();i++)
		{
			LanguageException* se=e.errors->Get(i).Obj();
			Console::WriteLine(L"  Sub Exception: "+se->Message());
			Console::WriteLine(L"    Line Number: "+itow(se->LineIndex()+1));
			Console::WriteLine(L"    Code File: "+itow(se->CodeIndex()+1));
		}
		Console::SetColor(true, true, true, false);
		Console::WriteLine(L"");
		return 0;
	}
	catch(const Exception& e)
	{
		Console::SetColor(true, false, false, true);
		Console::WriteLine(L"Exception: "+e.Message());
		Console::SetColor(true, true, true, false);
		Console::WriteLine(L"");
		return 0;
	}
	catch(const Error& e)
	{
		Console::SetColor(true, false, false, true);
		Console::WriteLine(WString(L"Internal Error: ")+e.Description());
		Console::SetColor(true, true, true, false);
		Console::WriteLine(L"");
		return 0;
	}
	Console::Write(L"Welcome to Vczh Script ");
	Console::WriteLine(ProcVersion());
	Console::WriteLine(L"The following commands are supported:");
	Console::WriteLine(L"  vle make <make-file>");
	Console::WriteLine(L"  vle header <language> <assembly> <filename>");
	Console::WriteLine(L"  vle link <assembly-list-file> <assembly-output> [<debug-assembly-output>]");
	Console::WriteLine(L"  vle test <assembly-list-file>");
	Console::WriteLine(L"  vle test linked <linked-assembly>");
	Console::WriteLine(L"  vle simulate <assembly-list-file>");
	Console::WriteLine(L"  vle simulate linked <linked-assembly>");
	//Console::WriteLine(L"  vle interact <language>");
	Console::WriteLine(L"This version supports the following languages:");
	Console::WriteLine(L"  NativeX");
	return 0;
}