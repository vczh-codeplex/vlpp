#include "VlScript.h"

extern "C"
{
	VLSCRIPT_API VlsHost* __stdcall VlsCreateHost()
	{
		return 0;
	}

	VLSCRIPT_API void __stdcall VlsDisposeHost(VlsHost* host)
	{
	}
}