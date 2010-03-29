#include "..\..\Library\UnitTest\UnitTest.h"
#include "..\..\Library\Scripting\Languages\NativeX\NativeX.h"
#include "..\..\Library\Scripting\BasicLanguage\BasicLanguageResource.h"
#include "..\..\Library\GlobalStorage.h"
#include "..\..\Library\Stream\FileStream.h"

using namespace vl;
using namespace vl::scripting;
using namespace vl::collections;
using namespace vl::scripting::basicil;
using namespace vl::stream;

extern WString GetPath();

BEGIN_GLOBAL_STORAGE_CLASS(NativeXProvider)
	Ptr<ILanguageProvider>	provider;
INITIALIZE_GLOBAL_STORAGE_CLASS
	provider=CreateNativeXLanguageProvider();
FINALIZE_GLOBAL_STORAGE_CLASS
	provider=0;
END_GLOBAL_STORAGE_CLASS(NativeXProvider)

TEST_CASE(TestCreateNativeXProvider)
{
	Ptr<ILanguageProvider> provider=GetNativeXProvider().provider;
}

Ptr<LanguageAssembly> TestNativeXNoError(WString code)
{
	List<Ptr<LanguageAssembly>> references;
	List<WString> codes;
	List<Ptr<LanguageException>> errors;
	codes.Add(code);

	Ptr<ILanguageProvider> provider=GetNativeXProvider().provider;
	Ptr<LanguageAssembly> assembly=provider->Compile(references.Wrap(), codes.Wrap(), errors.Wrap());
	TEST_ASSERT(errors.Count()==0);
	TEST_ASSERT(assembly);
	return assembly;
}

#define LINE_(X) L#X L"\r\n"

TEST_CASE(Test_NativeX_EmptyProgram)
{
	Ptr<LanguageAssembly> assembly=TestNativeXNoError(
		LINE_(	unit empty;	)
		);
	Ptr<ResourceStream> stream=assembly->GetResources()[BasicILResourceNames::BasicLanguageInterfaces];
	ResourceRecord<BasicEntryRes> entry=stream->ReadRootRecord<BasicEntryRes>();
	TEST_ASSERT(!entry->declarations);
}

TEST_CASE(Test_NativeX_DefineStructure1)
{
	Ptr<LanguageAssembly> assembly=TestNativeXNoError(
		LINE_(	unit define_structure;	)
		LINE_(	structure Point{		)
		LINE_(		int x;				)
		LINE_(		int y;				)
		LINE_(	}						)
		);
	Ptr<ResourceStream> stream=assembly->GetResources()[BasicILResourceNames::BasicLanguageInterfaces];
	ResourceRecord<BasicEntryRes> entry=stream->ReadRootRecord<BasicEntryRes>();
}

TEST_CASE(Test_NativeX_DefineStructure2)
{
	Ptr<LanguageAssembly> assembly=TestNativeXNoError(
		LINE_(	unit define_structure;	)
		LINE_(	structure A;			)
		LINE_(	structure B{			)
		LINE_(		int x;				)
		LINE_(		int y;				)
		LINE_(		A* a;				)
		LINE_(	}						)
		LINE_(	structure A{			)
		LINE_(		int a;				)
		LINE_(	}						)
		);
	Ptr<ResourceStream> stream=assembly->GetResources()[BasicILResourceNames::BasicLanguageInterfaces];
	ResourceRecord<BasicEntryRes> entry=stream->ReadRootRecord<BasicEntryRes>();
}

TEST_CASE(Test_NativeX_TypeRename1)
{
	Ptr<LanguageAssembly> assembly=TestNativeXNoError(
		LINE_(	unit type_rename;		)
		LINE_(	structure Link;			)
		LINE_(	type PLink = Link*;		)
		LINE_(	structure Link{			)
		LINE_(		int data;			)
		LINE_(		PLink next;			)
		LINE_(	}						)
		);
	Ptr<ResourceStream> stream=assembly->GetResources()[BasicILResourceNames::BasicLanguageInterfaces];
	ResourceRecord<BasicEntryRes> entry=stream->ReadRootRecord<BasicEntryRes>();
}

TEST_CASE(Test_NativeX_TypeName2)
{
	Ptr<LanguageAssembly> assembly=TestNativeXNoError(
		LINE_(	unit type_rename;							)
		LINE_(	structure Command;							)
		LINE_(	type PCommand = Command*;					)
		LINE_(	type PExecutor = function void(PCommand);	)
		LINE_(	structure Command{							)
		LINE_(		PExecutor executor;						)
		LINE_(		PCommand previous;						)
		LINE_(		PCommand next;							)
		LINE_(	}											)
		);
	Ptr<ResourceStream> stream=assembly->GetResources()[BasicILResourceNames::BasicLanguageInterfaces];
	ResourceRecord<BasicEntryRes> entry=stream->ReadRootRecord<BasicEntryRes>();
}

TEST_CASE(Test_NativeX_SimpleFunction)
{
	Ptr<LanguageAssembly> assembly=TestNativeXNoError(
		LINE_(	unit simple_function;				)
		LINE_(	function int Add(int a, int b){		)
		LINE_(		result=a+b;						)
		LINE_(		exit;							)
		LINE_(	}									)
		LINE_(	function int Sub(int a, int b){		)
		LINE_(		result=a-b;						)
		LINE_(		exit;							)
		LINE_(	}									)
		);
	Ptr<ResourceStream> stream=assembly->GetResources()[BasicILResourceNames::BasicLanguageInterfaces];
	ResourceRecord<BasicEntryRes> entry=stream->ReadRootRecord<BasicEntryRes>();
}

TEST_CASE(Test_NativeX_BubbleSort)
{
	Ptr<LanguageAssembly> assembly=TestNativeXNoError(
		LINE_(	unit bubble_sort;											)
		LINE_(	function void Sort(int* nums, int count)					)
		LINE_(	{															)
		LINE_(		for variable int i=0; when(i<count-1) with i++; do		)
		LINE_(			for variable int j=count-1; when(j>=i) with j--; do	)
		LINE_(				if(nums[j]>nums[j+1])							)
		LINE_(				{												)
		LINE_(					variable int t=nums[j];						)
		LINE_(					nums[j]=nums[j+1];							)
		LINE_(					nums[j+1]=t;								)
		LINE_(				}												)
		LINE_(	}															)
		LINE_(	function void Main()										)
		LINE_(	{															)
		LINE_(		variable int[10] nums;									)
		LINE_(		for variable int i=0; when(i<10) with i++; do			)
		LINE_(			nums[i] = (3*i)%10;									)
		LINE_(		Sort(cast<int*>(&nums), 10);							)
		LINE_(	}															)
		);
	Ptr<ResourceStream> stream=assembly->GetResources()[BasicILResourceNames::BasicLanguageInterfaces];
	ResourceRecord<BasicEntryRes> entry=stream->ReadRootRecord<BasicEntryRes>();
}

TEST_CASE(Test_NativeX_Sum)
{
	Ptr<LanguageAssembly> assembly=TestNativeXNoError(
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
		LINE_(		while(true)												)
		LINE_(		{														)
		LINE_(			result += nums[i];									)
		LINE_(		}when(i++<count);										)
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
	Ptr<ResourceStream> stream=assembly->GetResources()[BasicILResourceNames::BasicLanguageInterfaces];
	ResourceRecord<BasicEntryRes> entry=stream->ReadRootRecord<BasicEntryRes>();
}