#include <windows.h>
#include "..\..\Library\Scripting\Languages\NativeX\NativeX.h"

using namespace vl;
using namespace vl::collections;
using namespace vl::scripting;

#define LINE_(X) L#X L"\r\n"

Ptr<LanguageAssembly> Compile(WString code)
{
	List<Ptr<LanguageAssembly>> references;
	List<WString> codes;
	List<Ptr<LanguageException>> errors;
	codes.Add(code);

	Ptr<ILanguageProvider> provider=CreateNativeXLanguageProvider();
	Ptr<LanguageAssembly> assembly=provider->Compile(references.Wrap(), codes.Wrap(), errors.Wrap());
	return assembly;
}

void LaunchNativeXLanguageProvider()
{
	Ptr<LanguageAssembly> assembly=Compile(
		LINE_(	unit sum_array;												)
		LINE_(	function int Sum1(int* nums, int count)						)
		LINE_(	{															)
		LINE_(		result = 0;												)
		LINE_(		variable int i=0;										)
		LINE_(		while(i<count)											)
		LINE_(		{														)
		LINE_(			result += nums[i++];								)
		LINE_(		}														)
		LINE_(	}															)
		LINE_(	function int Sum2(int* nums, int count)						)
		LINE_(	{															)
		LINE_(		result = 0;												)
		LINE_(		variable int i=0;										)
		LINE_(		if(count>0)												)
		LINE_(			while(true)											)
		LINE_(			{													)
		LINE_(				result += nums[i];								)
		LINE_(			}when(++i<count);									)
		LINE_(	}															)
		LINE_(	function int Sum3(int* nums, int count)						)
		LINE_(	{															)
		LINE_(		result = 0;												)
		LINE_(		variable int i=0;										)
		LINE_(		do														)
		LINE_(		{														)
		LINE_(			if(i>=count)break;									)
		LINE_(			result += nums[i];									)
		LINE_(		}while(i++<count);										)
		LINE_(	}															)
		LINE_(	function int Sum4(int* nums, int count)						)
		LINE_(	{															)
		LINE_(		result = 0;												)
		LINE_(		variable int i=0;										)
		LINE_(		loop													)
		LINE_(		{														)
		LINE_(			if(i>=count)break;									)
		LINE_(			result += nums[i];									)
		LINE_(			if(i++<count)continue;else break;					)
		LINE_(		}														)
		LINE_(	}															)
		LINE_(	variable int Sum5Result=0;									)
		LINE_(	function int Sum5(int* nums, int count)						)
		LINE_(	{															)
		LINE_(		Sum5Result = 0;											)
		LINE_(		variable int i=0;										)
		LINE_(		loop													)
		LINE_(		{														)
		LINE_(			if(i>=count)break;									)
		LINE_(			Sum5Result += nums[i];								)
		LINE_(			if(i++<count);else break;							)
		LINE_(		}														)
		LINE_(		result = Sum5Result;									)
		LINE_(	}															)
		LINE_(	function void Main()										)
		LINE_(	{															)
		LINE_(		variable int[10] nums;									)
		LINE_(		for variable int i=0; when(i<10) with i++; do			)
		LINE_(			nums[i] = (3*i)%10;									)
		LINE_(		variable int r=0;										)
		LINE_(		r+=Sum1(cast<int*>(&nums), 10);							)
		LINE_(		r+=Sum2(cast<int*>(&nums), 10);							)
		LINE_(		r+=Sum3(cast<int*>(&nums), 10);							)
		LINE_(		r+=Sum4(cast<int*>(&nums), 10);							)
		LINE_(	}															)
		);
}

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow)
{
	LaunchNativeXLanguageProvider();
	return 0;
}