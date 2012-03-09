#include <Windows.h>
#include <iostream>
#include <string>
#include "dia2.h"
#include "diacreate.h"

#pragma comment(lib, "diaguids.lib")

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
    //HRESULT hr = CoCreateInstance(
    //    CLSID_DiaSource,
    //    NULL,
    //    CLSCTX_INPROC_SERVER,
    //    IID_IDiaDataSource,
    //    (void**) &pSource
    //    );
	HRESULT hr = NoRegCoCreate(
		L"msdia100.dll",
		CLSID_DiaSource,
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
		std::wcout<<L"importing "<<argv[1]<<std::endl;
		IDiaSymbol* exeSymbol=CreateDiaSymbol(argv[1]);
		if(exeSymbol)
		{
			std::wcout<<L"exporting "<<argv[2]<<std::endl;
			dumppdb::DumpPdbToXml(exeSymbol, argv[2]);
			std::wcout<<L"exported "<<argv[2]<<std::endl;
		}
		else
		{
			std::wcout<<L"Failed to read pdb("<<argv[1]<<L")"<<std::endl;
		}
	}
	else
	{
		std::wcout<<L"Pdb2Xml.exe <pdb-path> <xml-path>"<<std::endl;
	}
	return 0;
}