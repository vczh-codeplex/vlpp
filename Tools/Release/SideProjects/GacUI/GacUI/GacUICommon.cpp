#include "GacUICommon.h"
#include "..\..\..\..\..\Candidate\GUI\GUI\GacUI.h"
#include "..\..\..\..\..\Candidate\GUI\GUI\Reflection\GuiTypeDescriptorImpProvider_codegen.h"

namespace gacui_cpp
{
	using namespace vl;

/***********************************************************************
GacString
***********************************************************************/

	struct GacStringInternalData
	{
		WString		string;
	};

	GacString::GacString(GacStringInternalData* _internalData)
		:internalData(_internalData)
	{
	}

	GacString::GacString(const wchar_t* data)
	{
		internalData=new GacStringInternalData;
		internalData->string=(data!=0?data:L"");
	}

	GacString::GacString(const GacString& data)
	{
		internalData=new GacStringInternalData;
		internalData->string=data.internalData->string;
	}

	GacString::~GacString()
	{
		delete internalData;
	}

	int GacString::Length()
	{
		return internalData->string.Length();
	}

	const wchar_t* GacString::Buffer()
	{
		return internalData->string.Buffer();
	}

	int GacString::CalculateLengthForAsciiString()
	{
		AString a=wtoa(internalData->string);
		return a.Length();
	}

	void GacString::FillAsciiStringBuffer(char* buffer)
	{
		AString a=wtoa(internalData->string);
		strcpy_s(buffer, a.Length(), a.Buffer());
	}
}

/***********************************************************************
Helper Functions
***********************************************************************/

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