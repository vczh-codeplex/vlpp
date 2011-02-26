#ifndef VCZH_VLSCRIPT_DLL_HEADER
#define VCZH_VLSCRIPT_DLL_HEADER

#ifdef VLSCRIPT_EXPORTS
#define VLSCRIPT_API __declspec(dllexport)
#else
#define VLSCRIPT_API __declspec(dllimport)
#endif

struct VlsHost;
struct VlsState;
struct VlsAssembly;
typedef int (__stdcall VlsForeignFunction)(void* result, void* arguments, void* userData);

extern "C"
{
	const int VLS_OK = 1;
	const int VLS_ERR = 0;

	const int VLS_MIN_STACK_SIZE = 1024;
	const int VLS_MAX_STACK_SIZE = 1048576;
	const int VLS_MIN_DATA_SIZE = 1;
	const int VLS_MAX_DATA_SIZE = 256;
	const int VLS_DEFAULT_STACK_SIZE = 65536;

	VLSCRIPT_API int __stdcall VlsCreateHost(VlsHost** host, int stackSize);
	VLSCRIPT_API int __stdcall VlsDisposeHost(VlsHost* host);
	VLSCRIPT_API int __stdcall VlsCreateState(VlsHost* host, VlsState** state);
	VLSCRIPT_API int __stdcall VlsDisposeState(VlsState* state);
	VLSCRIPT_API int __stdcall VlsCreateAssemblyFromFile(VlsHost* host, const wchar_t* fileName, VlsAssembly** assembly);
	VLSCRIPT_API int __stdcall VlsDisposeAssembly(VlsAssembly* assembly);
	VLSCRIPT_API int __stdcall VlsGetLastError(VlsHost* host, const wchar_t** error, int* length);

	//--------------------------------------------------------------------------------

	VLSCRIPT_API int __stdcall VlsGetBasicFunctionCount(VlsAssembly* assembly, int* count);
	VLSCRIPT_API int __stdcall VlsGetBasicFunctionName(VlsAssembly* assembly, int index, const wchar_t** name);
	VLSCRIPT_API int __stdcall VlsGetBasicFunctionEntry(VlsAssembly* assembly, int index, int* entry);

	//--------------------------------------------------------------------------------

	VLSCRIPT_API int __stdcall VlsRegisterForeignFunction(VlsHost* host, const wchar_t* category, const wchar_t* name, VlsForeignFunction* function, void* userData);

	//--------------------------------------------------------------------------------

	VLSCRIPT_API int __stdcall VlsLoadAssembly(VlsHost* host, VlsAssembly* assembly);
	VLSCRIPT_API int __stdcall VlsInitAssembly(VlsState* state, VlsAssembly* assembly);
	VLSCRIPT_API int __stdcall VlsPush(VlsState* state, void* data, int size);
	VLSCRIPT_API int __stdcall VlsSetFunction(VlsState* state, VlsAssembly* assembly, int entry, void* resultAddress);
	VLSCRIPT_API int __stdcall VlsExecute(VlsState* state);
}

#endif