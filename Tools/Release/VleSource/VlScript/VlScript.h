#ifndef VCZH_VLSCRIPT_DLL_HEADER
#define VCZH_VLSCRIPT_DLL_HEADER

#ifdef VLSCRIPT_EXPORTS
#define VLSCRIPT_API __declspec(dllexport)
#else
#define VLSCRIPT_API __declspec(dllimport)
#endif

struct VlsHost;

extern "C"
{
	VLSCRIPT_API VlsHost* __stdcall VlsCreateHost();
	VLSCRIPT_API void __stdcall VlsDisposeHost(VlsHost* host);
}

#endif