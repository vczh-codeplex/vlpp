#include "GacUICpp.h"
#include "..\..\..\..\..\Candidate\GUI\GUI\GacUI.h"
#include "..\..\..\..\..\Candidate\GUI\GUI\Reflection\GuiTypeDescriptorImpProvider_codegen.h"
#include <Windows.h>

BOOL APIENTRY DllMain( HMODULE hModule, DWORD ul_reason_for_call, LPVOID lpReserved)
{
	switch (ul_reason_for_call)
	{
	case DLL_PROCESS_ATTACH:
	case DLL_THREAD_ATTACH:
	case DLL_THREAD_DETACH:
	case DLL_PROCESS_DETACH:
		break;
	}
	return TRUE;
}

void (__stdcall*guimain_callback)()=0;

void GuiMain()
{
	if(guimain_callback)
	{
		ITypeProvider* typeProvider=reflection_implementation::CreateDefaultTypeProvider();
		SetTypeProvider(typeProvider);
		guimain_callback();
	}
}

extern "C"
{
	GACUI_API void SetGuiMain(void(__stdcall*guimain)())
	{
		guimain_callback=guimain;
	}

	GACUI_API void StartGDI()
	{
		SetupWindowsGDIRenderer();
	}

	GACUI_API void StartDirect2D()
	{
		SetupWindowsDirect2DRenderer();
	}
}