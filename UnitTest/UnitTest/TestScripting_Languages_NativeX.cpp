#include "..\..\Library\UnitTest\UnitTest.h"
#include "..\..\Library\Scripting\Languages\NativeX\NativeX.h"
#include "..\..\Library\GlobalStorage.h"

using namespace vl;
using namespace vl::scripting;
using namespace vl::collections;

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

void TestNativeXNoError(WString code)
{
	List<Ptr<LanguageAssembly>> references;
	List<WString> codes;
	List<Ptr<LanguageException>> errors;
	codes.Add(code);

	Ptr<ILanguageProvider> provider=GetNativeXProvider().provider;
	Ptr<LanguageAssembly> assembly=provider->Compile(references.Wrap(), codes.Wrap(), errors.Wrap());
	TEST_ASSERT(errors.Count()==0);
	TEST_ASSERT(assembly);
}

#define LINE_(X) L#X L"\r\n"

TEST_CASE(Test_NativeX_EmptyProgram)
{
	TestNativeXNoError(
		LINE_(	unit empty;	)
		);
}

TEST_CASE(Test_NativeX_DefineStructure)
{
	TestNativeXNoError(
		LINE_(	unit define_structure;	)
		LINE_(	structure Point{		)
		LINE_(		int x;				)
		LINE_(		int y;				)
		LINE_(	}						)
		);
	TestNativeXNoError(
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
}

TEST_CASE(Test_NativeX_TypeRename)
{
	TestNativeXNoError(
		LINE_(	unit type_rename;		)
		LINE_(	structure Link;			)
		LINE_(	type PLink = Link*;		)
		LINE_(	structure Link{			)
		LINE_(		int data;			)
		LINE_(		PLink next;			)
		LINE_(	}						)
		);
	TestNativeXNoError(
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
}

TEST_CASE(Test_NativeX_SimpleFunction)
{
	TestNativeXNoError(
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
}

TEST_CASE(Test_NativeX_BubbleSort)
{
	TestNativeXNoError(
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
		);
}