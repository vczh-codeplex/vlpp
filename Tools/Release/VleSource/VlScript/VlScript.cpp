#include "VlScript.h"
#include "..\..\..\..\Library\Scripting\Languages\LanguageRuntime.h"

using namespace vl;
using namespace vl::collections;
using namespace vl::scripting;

struct VlsHost
{
	Ptr<LanguageHost>			host;
};

struct VlsState
{
	Ptr<LanguageState>			state;
};

struct VlsAssembly
{
	Ptr<LanguageAssembly>		assembly;
	List<WString>				basicFunctionNames;
	List<int>					basicFunctionEntries;
};

extern "C"
{
	VLSCRIPT_API int __stdcall VlsCreateHost(VlsHost** host)
	{
		return VLS_ERR;
	}

	VLSCRIPT_API int __stdcall VlsDisposeHost(VlsHost* host)
	{
		return VLS_ERR;
	}

	VLSCRIPT_API int __stdcall VlsCreateState(VlsHost* host, VlsState** state)
	{
		return VLS_ERR;
	}

	VLSCRIPT_API int __stdcall VlsDisposeState(VlsState* state)
	{
		return VLS_ERR;
	}

	VLSCRIPT_API int __stdcall VlsCreateAssemblyFromFile(wchar_t* fileName, VlsAssembly** assembly)
	{
		return VLS_ERR;
	}

	VLSCRIPT_API int __stdcall VlsDisposeAssembly(VlsAssembly* assembly)
	{
		return VLS_ERR;
	}

	VLSCRIPT_API int __stdcall VlsGetLastHostError(VlsHost* host, wchar_t** error)
	{
		return VLS_ERR;
	}

	VLSCRIPT_API int __stdcall VlsGetBasicFunctionCount(VlsAssembly* assembly, int** count)
	{
		return VLS_ERR;
	}

	VLSCRIPT_API int __stdcall VlsGetBasicFunctionName(VlsAssembly* assembly, int index, wchar_t** name)
	{
		return VLS_ERR;
	}

	VLSCRIPT_API int __stdcall VlsGetBasicFunctionEntry(VlsAssembly* assembly, int index, int* entry)
	{
		return VLS_ERR;
	}

	VLSCRIPT_API int __stdcall VlsRegisterForeignFunction(VlsHost* host, wchar_t* name, VlsForeignFunction* function, void* userData)
	{
		return VLS_ERR;
	}

	VLSCRIPT_API int __stdcall VlsLoadAssembly(VlsHost* host, VlsAssembly* assembly)
	{
		return VLS_ERR;
	}

	VLSCRIPT_API int __stdcall VlsInitAssembly(VlsState* state, VlsAssembly* assembly)
	{
		return VLS_ERR;
	}

	VLSCRIPT_API int __stdcall VlsPush(VlsState* state, void* data, int size)
	{
		return VLS_ERR;
	}

	VLSCRIPT_API int __stdcall VlsSetFunction(VlsState* state, VlsAssembly* assembly, int entry, void* resultAddress)
	{
		return VLS_ERR;
	}

	VLSCRIPT_API int __stdcall VlsExecute(VlsState* state)
	{
		return VLS_ERR;
	}
}