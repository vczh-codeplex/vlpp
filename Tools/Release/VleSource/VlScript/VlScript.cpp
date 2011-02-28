#include "VlScript.h"
#include "..\..\..\..\Library\Stream\FileStream.h"
#include "..\..\..\..\Library\Scripting\Languages\LanguageRuntime.h"
#include "..\..\..\..\Library\Scripting\Utility\ScriptingUtilityForeignFunctions.h"

using namespace vl;
using namespace vl::collections;
using namespace vl::stream;
using namespace vl::scripting;
using namespace vl::scripting::basicil;
using namespace vl::scripting::utility;

struct VlsHost
{
	Ptr<LanguageHost>			host;
	WString						lastError;
};

struct VlsState
{
	VlsHost*					host;
	Ptr<LanguageState>			state;
};

struct VlsAssembly
{
	VlsHost*					host;
	Ptr<LanguageAssembly>		assembly;
	List<WString>				basicFunctionNames;
	List<int>					basicFunctionEntries;
};

#define VLS_ASSERT(CONDITION) do{if(!(CONDITION))return VLS_ERR;}while(0)

void ClearHostError(VlsHost* host)
{
	if(host)host->lastError=L"";
}

void SetHostError(VlsHost* host, const WString& error)
{
	host->lastError=error;
}

void SetHostError(VlsHost* host, ILException::RunningResult result)
{
	ILException e(result);
	SetHostError(host, e.Message());
}

extern "C"
{
	VLSCRIPT_API int __stdcall VlsCreateHost(VlsHost** host, int stackSize)
	{
		VLS_ASSERT(host && VLS_MIN_STACK_SIZE<=stackSize && stackSize<=VLS_MAX_STACK_SIZE);
		VlsHost* vHost=new VlsHost;
		vHost->host=new LanguageHost(stackSize);
		*host=vHost;
		return VLS_OK;
	}

	VLSCRIPT_API int __stdcall VlsDisposeHost(VlsHost* host)
	{
		VLS_ASSERT(host);
		ClearHostError(host);
		delete host;
		return VLS_OK;
	}

	VLSCRIPT_API int __stdcall VlsCreateState(VlsHost* host, VlsState** state)
	{
		VLS_ASSERT(host && state);
		ClearHostError(host);
		VlsState* vState=new VlsState;
		vState->host=host;
		vState->state=host->host->CreateState();
		*state=vState;
		return VLS_OK;
	}

	VLSCRIPT_API int __stdcall VlsDisposeState(VlsState* state)
	{
		VLS_ASSERT(state);
		ClearHostError(state->host);
		delete state;
		return VLS_OK;
	}

	VLSCRIPT_API int __stdcall VlsCreateAssemblyFromFile(VlsHost* host, const wchar_t* fileName, VlsAssembly** assembly)
	{
		VLS_ASSERT(host && fileName && assembly);
		ClearHostError(host);
		FileStream fileStream(fileName, FileStream::ReadOnly);
		if(!fileStream.IsAvailable())
		{
			SetHostError(host, L"Cannot open \""+WString(fileName)+L"\" to read.");
		}
		VlsAssembly* vAssembly=new VlsAssembly;
		vAssembly->host=host;
		vAssembly->assembly=new LanguageAssembly(fileStream);
		if(vAssembly->assembly->GetBasicILMetadata()->IsAvailable())
		{
			for(int i=0;i<vAssembly->assembly->GetBasicILMetadata()->GetExportCount();i++)
			{
				BasicILExportInfo info=vAssembly->assembly->GetBasicILMetadata()->GetExport(i);
				if(info.IsFunction())
				{
					vAssembly->basicFunctionNames.Add(info.GetName());
					vAssembly->basicFunctionEntries.Add(info.GetInstructionIndex());
				}
			}
		}
		return VLS_OK;
	}

	VLSCRIPT_API int __stdcall VlsDisposeAssembly(VlsAssembly* assembly)
	{
		VLS_ASSERT(assembly);
		ClearHostError(assembly->host);
		delete assembly;
		return VLS_OK;
	}

	VLSCRIPT_API int __stdcall VlsGetLastHostError(VlsHost* host, const wchar_t** error)
	{
		VLS_ASSERT(host && error);
		*error=host->lastError.Buffer();
		return VLS_OK;
	}

	VLSCRIPT_API int __stdcall VlsGetBasicFunctionCount(VlsAssembly* assembly, int* count)
	{
		VLS_ASSERT(assembly && count);
		ClearHostError(assembly->host);
		*count=assembly->basicFunctionNames.Count();
		return VLS_OK;
	}

	VLSCRIPT_API int __stdcall VlsGetBasicFunctionName(VlsAssembly* assembly, int index, const wchar_t** name)
	{
		VLS_ASSERT(assembly && name);
		VLS_ASSERT(0<=index && index<assembly->basicFunctionNames.Count());
		*name=assembly->basicFunctionNames[index].Buffer();
		return VLS_OK;
	}

	VLSCRIPT_API int __stdcall VlsGetBasicFunctionEntry(VlsAssembly* assembly, int index, int* entry)
	{
		VLS_ASSERT(assembly && entry);
		VLS_ASSERT(0<=index && index<assembly->basicFunctionNames.Count());
		*entry=assembly->basicFunctionEntries[index];
		return VLS_OK;
	}

	VLSCRIPT_API int __stdcall VlsRegisterForeignFunction(VlsHost* host, const wchar_t* category, const wchar_t* name, VlsForeignFunction* function, void* userData)
	{
		VLS_ASSERT(host && category && name && function);
		if(host->host->RegisterForeignFunction(category, name, function, userData))
		{
			return VLS_OK;
		}
		else
		{
			SetHostError(host, L"Symbol "+WString(category)+L"::"+WString(name)+L" exists.");
			return VLS_ERR;
		}
	}

	VLSCRIPT_API int __stdcall VlsLoadPlugin_CoreNative(VlsHost* host)
	{
		VLS_ASSERT(host);
		if(	host->host->RegisterPlugin(CreateMemoryManagerPlugin()) ||
			host->host->RegisterPlugin(CreateThreadingPlugin()) ||
			host->host->RegisterPlugin(CreateStdlibPlugin()) ||
			host->host->RegisterPlugin(CreateGcSingleThreadPlugin()))
		{
			return VLS_OK;
		}
		else
		{
			return VLS_ERR;
		}
	}

	VLSCRIPT_API int __stdcall VlsLoadPlugin_ConsoleNative(VlsHost* host, void(__stdcall*reader)(wchar_t*), void(__stdcall*writer)(wchar_t*))
	{
		VLS_ASSERT(host);
		return host->host->RegisterPlugin(CreateConsolePlugin(reader, writer))?VLS_OK:VLS_ERR;
	}

	VLSCRIPT_API int __stdcall VlsLoadPlugin_UnitTestNative(VlsHost* host, void(__stdcall*printer)(bool, wchar_t*))
	{
		VLS_ASSERT(host);
		return host->host->RegisterPlugin(CreateUnitTestPlugin(printer))?VLS_OK:VLS_ERR;
	}

	VLSCRIPT_API int __stdcall VlsLoadAssembly(VlsHost* host, VlsAssembly* assembly)
	{
		VLS_ASSERT(host && assembly);
		try
		{
			host->host->LoadAssembly(assembly->assembly);
			return VLS_OK;
		}
		catch(const Exception& e)
		{
			SetHostError(host, e.Message());
			return VLS_ERR;
		}
	}

	VLSCRIPT_API int __stdcall VlsInitAssembly(VlsState* state, VlsAssembly* assembly)
	{
		VLS_ASSERT(state && assembly);
		ILException::RunningResult result=state->state->RunInitialization(assembly->assembly);
		if(result!=ILException::Finished)
		{
			SetHostError(state->host, result);
			return VLS_ERR;
		}
		return VLS_OK;
	}

	VLSCRIPT_API int __stdcall VlsPush(VlsState* state, void* data, int size)
	{
		VLS_ASSERT(state && data && VLS_MIN_DATA_SIZE<=size && size<=VLS_MAX_DATA_SIZE);
		try
		{
			memmove(state->state->GetStack()->Reserve(size), data, size);
			return VLS_OK;
		}
		catch(const Exception& e)
		{
			SetHostError(state->host, e.Message());
			return VLS_ERR;
		}
	}

	VLSCRIPT_API int __stdcall VlsSetFunction(VlsState* state, VlsAssembly* assembly, int entry, void* resultAddress)
	{
		VLS_ASSERT(state && assembly);
		try
		{
			state->state->PrepareToRun(assembly->assembly, entry, resultAddress);
			return VLS_OK;
		}
		catch(const Exception& e)
		{
			SetHostError(state->host, e.Message());
			return VLS_ERR;
		}
	}

	VLSCRIPT_API int __stdcall VlsExecute(VlsState* state)
	{
		VLS_ASSERT(state);
		ILException::RunningResult result=state->state->Run();
		if(result!=ILException::Finished)
		{
			SetHostError(state->host, result);
			return VLS_ERR;
		}
		return VLS_OK;
	}
}