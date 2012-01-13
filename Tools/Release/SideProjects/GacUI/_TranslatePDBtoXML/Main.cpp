#include <Windows.h>
#include "dia2.h"
#include "..\..\..\..\..\Library\Console.h"

#pragma comment(lib, "diaguids.lib")

using namespace vl;
using namespace vl::console;

namespace dumppdb
{
	extern void DumpPdbToXml(IDiaSymbol* exeSymbol, const wchar_t* xml);
}

IDiaSymbol* CreateDiaSymbol(const wchar_t* pdbPath)
{
    IDiaDataSource* pSource=0;
    IDiaSession* pSession=0;
    IDiaSymbol* pSymbol=0;
    CoInitialize(NULL);
    HRESULT hr = CoCreateInstance(
        CLSID_DiaSource,
        NULL,
        CLSCTX_INPROC_SERVER,
        IID_IDiaDataSource,
        (void**) &pSource
        );
    if(SUCCEEDED(hr))
	if(SUCCEEDED(pSource->loadDataFromPdb(pdbPath)))
    if(SUCCEEDED(pSource->openSession(&pSession)))
	if(SUCCEEDED(pSession->get_globalScope(&pSymbol)))
	{
		return pSymbol;
	}
	return 0;
}

int wmain(int argc, wchar_t* argv[])
{
	if(argc==3)
	{
		IDiaSymbol* exeSymbol=CreateDiaSymbol(argv[1]);
		if(exeSymbol)
		{
			dumppdb::DumpPdbToXml(exeSymbol, argv[2]);
		}
		else
		{
			Console::WriteLine(L"Failed to read pdb("+WString(argv[1])+L")");
		}
	}
	else
	{
		Console::WriteLine(L"_TranslatePDBtoXML.exe <pdb-path> <xml-path>");
	}
	return 0;
}