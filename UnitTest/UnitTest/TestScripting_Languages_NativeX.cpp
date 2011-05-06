#include "..\..\Library\UnitTest\UnitTest.h"
#include "..\..\Library\Scripting\Languages\NativeX\NativeX.h"
#include "..\..\Library\Scripting\Languages\BasicFunctionExecutor.h"
#include "..\..\Library\Scripting\BasicLanguage\BasicLanguageResource.h"
#include "..\..\Library\Scripting\Languages\LanguageProviderExtension.h"
#include "..\..\Library\GlobalStorage.h"
#include "..\..\Library\Stream\FileStream.h"
#include "..\..\Library\Stream\MemoryStream.h"

using namespace vl;
using namespace vl::scripting;
using namespace vl::collections;
using namespace vl::scripting::basicil;
using namespace vl::scripting::basiclanguage;
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

void PrintNativeXProgram(Ptr<BasicProgram> program, TextWriter& writer)
{
	Ptr<ILanguageProvider> provider=GetNativeXProvider().provider;
	Ptr<IBasicLanguageProvider> basicLanguageProvider=provider.Cast<IBasicLanguageProvider>();
	basicLanguageProvider->GenerateCode(program, 0, writer);
}

void ConvertToNativeXProgram(Ptr<BasicProgram>& program)
{
	MemoryStream stream;
	StreamWriter writer(stream);
	PrintNativeXProgram(program, writer);

	stream.SeekFromBegin(0);
	StreamReader reader(stream);
	WString code=reader.ReadToEnd();

	Ptr<ILanguageProvider> provider=GetNativeXProvider().provider;
	Ptr<IBasicLanguageProvider> basicLanguageProvider=provider.Cast<IBasicLanguageProvider>();
	List<Ptr<LanguageException>> errors;
	Ptr<Object> extra;
	program=basicLanguageProvider->ParseProgram(code, extra, errors.Wrap());
	TEST_ASSERT(errors.Count()==0);
	TEST_ASSERT(program);
}

Ptr<LanguageAssembly> TestNativeXNoError(const WString& code, const WString& name=L"nativex_assembly")
{
	List<Ptr<LanguageAssembly>> references;
	List<WString> codes;
	List<Ptr<LanguageException>> errors;
	codes.Add(code);

	Ptr<ILanguageProvider> provider=GetNativeXProvider().provider;
	Ptr<LanguageAssembly> assembly=provider->Compile(name, references.Wrap(), codes.Wrap(), errors.Wrap());
	TEST_ASSERT(errors.Count()==0);
	TEST_ASSERT(assembly);
	
	MemoryStream stream;
	assembly->SaveToStream(stream);
	stream.SeekFromBegin(0);
	return new LanguageAssembly(stream);
}

#define LINE_(X) L#X L"\r\n"

/***********************************************************************
Test Cases
***********************************************************************/

TEST_CASE(Test_NativeX_EmptyProgram)
{
	Ptr<LanguageAssembly> assembly=TestNativeXNoError(
		LINE_(	unit empty;	)
		);
	{
		BasicLanguageMetadata* metadata=assembly->GetBasicLanguageMetadata();
		TEST_ASSERT(metadata->GetDeclarationCount()==0);
	}
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
	{
		BasicLanguageMetadata* metadata=assembly->GetBasicLanguageMetadata();
		TEST_ASSERT(metadata->GetDeclarationCount()==1);

		BasicDeclarationInfo point=metadata->GetDeclaration(0);
		TEST_ASSERT(point.IsFunction()==false);
		TEST_ASSERT(point.IsVariable()==false);
		TEST_ASSERT(point.IsStructure()==true);
		TEST_ASSERT(point.GetName()==L"Point");

		BasicTypeInfo pointType=point.GetType();
		TEST_ASSERT(pointType.IsPrimitive()==false);
		TEST_ASSERT(pointType.IsPointer()==false);
		TEST_ASSERT(pointType.IsArray()==false);
		TEST_ASSERT(pointType.IsFunction()==false);
		TEST_ASSERT(pointType.IsStructure()==true);
		TEST_ASSERT(pointType.GetSize()==sizeof(vint)*2);
		TEST_ASSERT(pointType.GetComponentCount()==2);
		TEST_ASSERT(pointType.GetComponentName(0)==L"x");
		TEST_ASSERT(pointType.GetComponentOffset(0)==0);
		TEST_ASSERT(pointType.GetComponentType(0).IsPrimitive() && pointType.GetComponentType(0).GetPrimitive()==BasicTypeRes::int_type);
		TEST_ASSERT(pointType.GetComponentName(1)==L"y");
		TEST_ASSERT(pointType.GetComponentOffset(1)==sizeof(vint));
		TEST_ASSERT(pointType.GetComponentType(1).IsPrimitive() && pointType.GetComponentType(1).GetPrimitive()==BasicTypeRes::int_type);
	}
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
	{
		BasicLanguageMetadata* metadata=assembly->GetBasicLanguageMetadata();
		TEST_ASSERT(metadata->GetDeclarationCount()==2);

		BasicDeclarationInfo b=metadata->GetDeclaration(0);
		TEST_ASSERT(b.IsFunction()==false);
		TEST_ASSERT(b.IsVariable()==false);
		TEST_ASSERT(b.IsStructure()==true);
		TEST_ASSERT(b.GetName()==L"B");

		BasicDeclarationInfo a=metadata->GetDeclaration(1);
		TEST_ASSERT(a.IsFunction()==false);
		TEST_ASSERT(a.IsVariable()==false);
		TEST_ASSERT(a.IsStructure()==true);
		TEST_ASSERT(a.GetName()==L"A");

		BasicTypeInfo bType=b.GetType();
		TEST_ASSERT(bType.IsPrimitive()==false);
		TEST_ASSERT(bType.IsPointer()==false);
		TEST_ASSERT(bType.IsArray()==false);
		TEST_ASSERT(bType.IsFunction()==false);
		TEST_ASSERT(bType.IsStructure()==true);
		TEST_ASSERT(bType.GetSize()==sizeof(vint)*3);
		TEST_ASSERT(bType.GetComponentCount()==3);
		TEST_ASSERT(bType.GetComponentName(0)==L"x");
		TEST_ASSERT(bType.GetComponentOffset(0)==0);
		TEST_ASSERT(bType.GetComponentType(0).IsPrimitive() && bType.GetComponentType(0).GetPrimitive()==BasicTypeRes::int_type);
		TEST_ASSERT(bType.GetComponentName(1)==L"y");
		TEST_ASSERT(bType.GetComponentOffset(1)==sizeof(vint));
		TEST_ASSERT(bType.GetComponentType(1).IsPrimitive() && bType.GetComponentType(1).GetPrimitive()==BasicTypeRes::int_type);
		TEST_ASSERT(bType.GetComponentName(2)==L"a");
		TEST_ASSERT(bType.GetComponentOffset(2)==sizeof(vint)*2);
		TEST_ASSERT(bType.GetComponentType(2).IsPointer() && bType.GetComponentType(2).GetElementType().IsSameRecord(a.GetType()));

		BasicTypeInfo aType=a.GetType();
		TEST_ASSERT(aType.IsPrimitive()==false);
		TEST_ASSERT(aType.IsPointer()==false);
		TEST_ASSERT(aType.IsArray()==false);
		TEST_ASSERT(aType.IsFunction()==false);
		TEST_ASSERT(aType.IsStructure()==true);
		TEST_ASSERT(aType.GetSize()==sizeof(vint));
		TEST_ASSERT(aType.GetComponentCount()==1);
		TEST_ASSERT(aType.GetComponentName(0)==L"a");
		TEST_ASSERT(aType.GetComponentOffset(0)==0);
		TEST_ASSERT(aType.GetComponentType(0).IsPrimitive() && aType.GetComponentType(0).GetPrimitive()==BasicTypeRes::int_type);
	}
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
	{
		BasicLanguageMetadata* metadata=assembly->GetBasicLanguageMetadata();
		TEST_ASSERT(metadata->GetDeclarationCount()==2);

		BasicDeclarationInfo link=metadata->GetDeclaration(1);
		TEST_ASSERT(link.IsFunction()==false);
		TEST_ASSERT(link.IsVariable()==false);
		TEST_ASSERT(link.IsStructure()==true);
		TEST_ASSERT(link.GetName()==L"Link");

		BasicTypeInfo linkType=link.GetType();
		TEST_ASSERT(linkType.IsPrimitive()==false);
		TEST_ASSERT(linkType.IsPointer()==false);
		TEST_ASSERT(linkType.IsArray()==false);
		TEST_ASSERT(linkType.IsFunction()==false);
		TEST_ASSERT(linkType.IsStructure()==true);
		TEST_ASSERT(linkType.GetSize()==sizeof(vint)*2);
		TEST_ASSERT(linkType.GetComponentCount()==2);
		TEST_ASSERT(linkType.GetComponentName(0)==L"data");
		TEST_ASSERT(linkType.GetComponentOffset(0)==0);
		TEST_ASSERT(linkType.GetComponentType(0).IsPrimitive() && linkType.GetComponentType(0).GetPrimitive()==BasicTypeRes::int_type);
		TEST_ASSERT(linkType.GetComponentName(1)==L"next");
		TEST_ASSERT(linkType.GetComponentOffset(1)==sizeof(vint));
		TEST_ASSERT(linkType.GetComponentType(1).IsPointer() && linkType.GetComponentType(1).GetElementType().IsSameRecord(linkType));

		BasicDeclarationInfo plink=metadata->GetDeclaration(0);
		TEST_ASSERT(plink.IsFunction()==false);
		TEST_ASSERT(plink.IsVariable()==false);
		TEST_ASSERT(plink.IsStructure()==false);
		TEST_ASSERT(plink.IsTypeRename()==true);
		TEST_ASSERT(plink.GetName()==L"PLink");

		BasicTypeInfo plinkType=plink.GetType();
		TEST_ASSERT(plinkType.IsPointer()==true);
		TEST_ASSERT(plinkType.GetElementType().IsSameRecord(linkType));
	}
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
	{
		BasicLanguageMetadata* metadata=assembly->GetBasicLanguageMetadata();
		TEST_ASSERT(metadata->GetDeclarationCount()==3);

		BasicDeclarationInfo command=metadata->GetDeclaration(2);
		TEST_ASSERT(command.IsFunction()==false);
		TEST_ASSERT(command.IsVariable()==false);
		TEST_ASSERT(command.IsStructure()==true);
		TEST_ASSERT(command.GetName()==L"Command");

		BasicTypeInfo commandType=command.GetType();
		TEST_ASSERT(commandType.IsPrimitive()==false);
		TEST_ASSERT(commandType.IsPointer()==false);
		TEST_ASSERT(commandType.IsArray()==false);
		TEST_ASSERT(commandType.IsFunction()==false);
		TEST_ASSERT(commandType.IsStructure()==true);
		TEST_ASSERT(commandType.GetSize()==sizeof(vint)*3);
		TEST_ASSERT(commandType.GetComponentCount()==3);

		TEST_ASSERT(commandType.GetComponentName(0)==L"executor");
		TEST_ASSERT(commandType.GetComponentOffset(0)==0);
		BasicTypeInfo executorType=commandType.GetComponentType(0);
		TEST_ASSERT(executorType.IsFunction());
		TEST_ASSERT(executorType.GetComponentCount()==1);
		TEST_ASSERT(executorType.GetComponentType(0).IsPointer() && executorType.GetComponentType(0).GetElementType().IsSameRecord(commandType));
		TEST_ASSERT(executorType.GetElementType().IsPrimitive() && executorType.GetElementType().GetPrimitive()==BasicTypeRes::void_type);

		TEST_ASSERT(commandType.GetComponentName(1)==L"previous");
		TEST_ASSERT(commandType.GetComponentOffset(1)==sizeof(vint));
		TEST_ASSERT(commandType.GetComponentType(1).IsPointer() && commandType.GetComponentType(1).GetElementType().IsSameRecord(commandType));
		TEST_ASSERT(commandType.GetComponentName(2)==L"next");
		TEST_ASSERT(commandType.GetComponentOffset(2)==sizeof(vint)*2);
		TEST_ASSERT(commandType.GetComponentType(2).IsPointer() && commandType.GetComponentType(2).GetElementType().IsSameRecord(commandType));

		BasicDeclarationInfo pcommandType=metadata->GetDeclaration(0);
		TEST_ASSERT(pcommandType.IsFunction()==false);
		TEST_ASSERT(pcommandType.IsVariable()==false);
		TEST_ASSERT(pcommandType.IsStructure()==false);
		TEST_ASSERT(pcommandType.IsTypeRename()==true);
		TEST_ASSERT(pcommandType.GetName()==L"PCommand");

		TEST_ASSERT(pcommandType.GetType().IsPointer()==true);
		TEST_ASSERT(pcommandType.GetType().GetElementType().IsSameRecord(commandType));

		BasicDeclarationInfo pexecutorType=metadata->GetDeclaration(1);
		TEST_ASSERT(pexecutorType.IsFunction()==false);
		TEST_ASSERT(pexecutorType.IsVariable()==false);
		TEST_ASSERT(pexecutorType.IsStructure()==false);
		TEST_ASSERT(pexecutorType.IsTypeRename()==true);
		TEST_ASSERT(pexecutorType.GetName()==L"PExecutor");
		TEST_ASSERT(pexecutorType.GetType().IsFunction()==true);
	}
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
	{
		BasicLanguageMetadata* metadata=assembly->GetBasicLanguageMetadata();
		TEST_ASSERT(metadata->GetDeclarationCount()==2);

		BasicDeclarationInfo add=metadata->GetDeclaration(0);
		TEST_ASSERT(add.IsFunction());
		TEST_ASSERT(!add.IsVariable());
		TEST_ASSERT(!add.IsStructure());
		TEST_ASSERT(add.GetName()==L"Add");
		TEST_ASSERT(add.GetParameterCount()==2);
		TEST_ASSERT(add.GetParameterName(0)==L"a");
		TEST_ASSERT(add.GetParameterName(1)==L"b");
		BasicTypeInfo addType=add.GetType();
		TEST_ASSERT(!addType.IsArray());
		TEST_ASSERT(addType.IsFunction());
		TEST_ASSERT(!addType.IsPointer());
		TEST_ASSERT(!addType.IsPrimitive());
		TEST_ASSERT(!addType.IsStructure());
		TEST_ASSERT(addType.GetElementType().IsPrimitive() && addType.GetElementType().GetPrimitive()==BasicTypeRes::int_type);
		TEST_ASSERT(addType.GetComponentType(0).IsPrimitive() && addType.GetComponentType(0).GetPrimitive()==BasicTypeRes::int_type);
		TEST_ASSERT(addType.GetComponentType(1).IsPrimitive() && addType.GetComponentType(1).GetPrimitive()==BasicTypeRes::int_type);

		BasicDeclarationInfo sub=metadata->GetDeclaration(1);
		TEST_ASSERT(sub.IsFunction());
		TEST_ASSERT(!sub.IsVariable());
		TEST_ASSERT(!sub.IsStructure());
		TEST_ASSERT(sub.GetName()==L"Sub");
		TEST_ASSERT(sub.GetParameterCount()==2);
		TEST_ASSERT(sub.GetParameterName(0)==L"a");
		TEST_ASSERT(sub.GetParameterName(1)==L"b");
		BasicTypeInfo subType=sub.GetType();
		TEST_ASSERT(!subType.IsArray());
		TEST_ASSERT(subType.IsFunction());
		TEST_ASSERT(!subType.IsPointer());
		TEST_ASSERT(!subType.IsPrimitive());
		TEST_ASSERT(!subType.IsStructure());
		TEST_ASSERT(subType.GetElementType().IsPrimitive() && subType.GetElementType().GetPrimitive()==BasicTypeRes::int_type);
		TEST_ASSERT(subType.GetComponentType(0).IsPrimitive() && subType.GetComponentType(0).GetPrimitive()==BasicTypeRes::int_type);
		TEST_ASSERT(subType.GetComponentType(1).IsPrimitive() && subType.GetComponentType(1).GetPrimitive()==BasicTypeRes::int_type);

		LanguageHost host(65536);
		host.LoadAssembly(assembly);
		Ptr<LanguageState> state=host.CreateState();
		TEST_ASSERT(state->RunInitialization(assembly)==basicil::ILException::Finished);
		BasicFunctionExecutor<vint(vint,vint)> addFunc(add, state);
		BasicFunctionExecutor<vint(vint,vint)> subFunc(sub, state);

		TEST_ASSERT(addFunc==addFunc);
		TEST_ASSERT(addFunc!=subFunc);
		TEST_ASSERT(addFunc);
		TEST_ASSERT(subFunc);
		TEST_ASSERT(addFunc(1, 2)==3);
		TEST_ASSERT(subFunc(1, 2)==-1);
	}
}

TEST_CASE(Test_NativeX_BubbleSort)
{
	Ptr<LanguageAssembly> assembly=TestNativeXNoError(
		LINE_(	unit bubble_sort;													)
		LINE_(	function void Sort(int* nums, int count)							)
		LINE_(	{																	)
		LINE_(		for (variable int i = 0; when(i<count - 1) with i++;) do		)
		LINE_(			for (variable int j = count - 2; when(j >= i) with j--;) do	)
		LINE_(				if(nums[j]>nums[j + 1])									)
		LINE_(				{														)
		LINE_(					variable int t=nums[j];								)
		LINE_(					nums[j]=nums[j + 1];								)
		LINE_(					nums[j + 1]=t;										)
		LINE_(				}														)
		LINE_(	}																	)
		LINE_(	function void Main()												)
		LINE_(	{																	)
		LINE_(		variable int[10] nums;											)
		LINE_(		for (variable int i = 0; when(i < 10) with i++;) do				)
		LINE_(			nums[i] = (3 * i) % 10;										)
		LINE_(		Sort(cast<int*>(&nums), 10);									)
		LINE_(	}																	)
		);
	{
		BasicLanguageMetadata* metadata=assembly->GetBasicLanguageMetadata();
		TEST_ASSERT(metadata->GetDeclarationCount()==2);

		BasicDeclarationInfo sort=metadata->GetDeclaration(0);
		TEST_ASSERT(sort.IsFunction());
		TEST_ASSERT(!sort.IsVariable());
		TEST_ASSERT(!sort.IsStructure());
		TEST_ASSERT(sort.GetName()==L"Sort");
		TEST_ASSERT(sort.GetParameterCount()==2);
		TEST_ASSERT(sort.GetParameterName(0)==L"nums");
		TEST_ASSERT(sort.GetParameterName(1)==L"count");
		BasicTypeInfo sortType=sort.GetType();
		TEST_ASSERT(!sortType.IsArray());
		TEST_ASSERT(sortType.IsFunction());
		TEST_ASSERT(!sortType.IsPointer());
		TEST_ASSERT(!sortType.IsPrimitive());
		TEST_ASSERT(!sortType.IsStructure());
		TEST_ASSERT(sortType.GetElementType().IsPrimitive() && sortType.GetElementType().GetPrimitive()==BasicTypeRes::void_type);
		TEST_ASSERT(sortType.GetComponentType(0).IsPointer() && sortType.GetComponentType(0).GetElementType().GetPrimitive()==BasicTypeRes::int_type);
		TEST_ASSERT(sortType.GetComponentType(1).IsPrimitive() && sortType.GetComponentType(1).GetPrimitive()==BasicTypeRes::int_type);

		BasicDeclarationInfo main=metadata->GetDeclaration(1);
		TEST_ASSERT(main.IsFunction());
		TEST_ASSERT(!main.IsVariable());
		TEST_ASSERT(!main.IsStructure());
		TEST_ASSERT(main.GetName()==L"Main");
		TEST_ASSERT(main.GetParameterCount()==0);
		BasicTypeInfo mainType=main.GetType();
		TEST_ASSERT(!mainType.IsArray());
		TEST_ASSERT(mainType.IsFunction());
		TEST_ASSERT(!mainType.IsPointer());
		TEST_ASSERT(!mainType.IsPrimitive());
		TEST_ASSERT(!mainType.IsStructure());
		TEST_ASSERT(mainType.GetElementType().IsPrimitive() && mainType.GetElementType().GetPrimitive()==BasicTypeRes::void_type);

		LanguageHost host(65536);
		host.LoadAssembly(assembly);
		Ptr<LanguageState> state=host.CreateState();
		TEST_ASSERT(state->RunInitialization(assembly)==basicil::ILException::Finished);
		BasicFunctionExecutor<void(vint*,vint)> sortFunc(sort, state);
		{
			sortFunc(0, 0);
		}
		{
			vint nums[]={0};
			sortFunc(nums, sizeof(nums)/sizeof(*nums));
			TEST_ASSERT(nums[0]==0);
		}
		{
			vint nums[]={3,5,1,4,2};
			sortFunc(nums, sizeof(nums)/sizeof(*nums));
			for(vint i=0;i<sizeof(nums)/sizeof(*nums);i++)
			{
				TEST_ASSERT(nums[i]==i+1);
			}
		}
	}
}

void TestSumFunction(BasicDeclarationInfo info, const WString& name)
{
	TEST_ASSERT(info.IsFunction());
	TEST_ASSERT(!info.IsVariable());
	TEST_ASSERT(!info.IsStructure());
	TEST_ASSERT(info.GetName()==name);
	TEST_ASSERT(info.GetParameterCount()==2);
	TEST_ASSERT(info.GetParameterName(0)==L"nums");
	TEST_ASSERT(info.GetParameterName(1)==L"count");
	BasicTypeInfo infoType=info.GetType();
	TEST_ASSERT(!infoType.IsArray());
	TEST_ASSERT(infoType.IsFunction());
	TEST_ASSERT(!infoType.IsPointer());
	TEST_ASSERT(!infoType.IsPrimitive());
	TEST_ASSERT(!infoType.IsStructure());
	TEST_ASSERT(infoType.GetElementType().IsPrimitive() && infoType.GetElementType().GetPrimitive()==BasicTypeRes::int_type);
	TEST_ASSERT(infoType.GetComponentType(0).IsPointer() && infoType.GetComponentType(0).GetElementType().GetPrimitive()==BasicTypeRes::int_type);
	TEST_ASSERT(infoType.GetComponentType(1).IsPrimitive() && infoType.GetComponentType(1).GetPrimitive()==BasicTypeRes::int_type);
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
		LINE_(		for (variable int i=0; when(i<10) with i++;) do			)
		LINE_(			nums[i] = (3*i)%10;									)
		LINE_(		variable int r=0;										)
		LINE_(		r+=Sum1(cast<int*>(&nums), 10);							)
		LINE_(		r+=Sum2(cast<int*>(&nums), 10);							)
		LINE_(		r+=Sum3(cast<int*>(&nums), 10);							)
		LINE_(		r+=Sum4(cast<int*>(&nums), 10);							)
		LINE_(	}															)
		);
	{
		BasicILMetadata* metadata=assembly->GetBasicILMetadata();
		TEST_ASSERT(metadata->GetExportCount()==7);

		TEST_ASSERT(metadata->GetExport(0).IsFunction()==true);
		TEST_ASSERT(metadata->GetExport(0).IsVariable()==false);
		TEST_ASSERT(metadata->GetExport(0).GetName()==L"Sum1");

		TEST_ASSERT(metadata->GetExport(1).IsFunction()==true);
		TEST_ASSERT(metadata->GetExport(1).IsVariable()==false);
		TEST_ASSERT(metadata->GetExport(1).GetName()==L"Sum2");

		TEST_ASSERT(metadata->GetExport(2).IsFunction()==true);
		TEST_ASSERT(metadata->GetExport(2).IsVariable()==false);
		TEST_ASSERT(metadata->GetExport(2).GetName()==L"Sum3");

		TEST_ASSERT(metadata->GetExport(3).IsFunction()==true);
		TEST_ASSERT(metadata->GetExport(3).IsVariable()==false);
		TEST_ASSERT(metadata->GetExport(3).GetName()==L"Sum4");

		TEST_ASSERT(metadata->GetExport(4).IsFunction()==false);
		TEST_ASSERT(metadata->GetExport(4).IsVariable()==true);
		TEST_ASSERT(metadata->GetExport(4).GetName()==L"Sum5Result");

		TEST_ASSERT(metadata->GetExport(5).IsFunction()==true);
		TEST_ASSERT(metadata->GetExport(5).IsVariable()==false);
		TEST_ASSERT(metadata->GetExport(5).GetName()==L"Sum5");

		TEST_ASSERT(metadata->GetExport(6).IsFunction()==true);
		TEST_ASSERT(metadata->GetExport(6).IsVariable()==false);
		TEST_ASSERT(metadata->GetExport(6).GetName()==L"Main");
	}
	{
		BasicLanguageMetadata* metadata=assembly->GetBasicLanguageMetadata();
		TEST_ASSERT(metadata->GetDeclarationCount()==7);

		TestSumFunction(metadata->GetDeclaration(0), L"Sum1");
		TestSumFunction(metadata->GetDeclaration(1), L"Sum2");
		TestSumFunction(metadata->GetDeclaration(2), L"Sum3");
		TestSumFunction(metadata->GetDeclaration(3), L"Sum4");
		TestSumFunction(metadata->GetDeclaration(5), L"Sum5");

		BasicDeclarationInfo sum5=metadata->GetDeclaration(4);
		TEST_ASSERT(!sum5.IsFunction());
		TEST_ASSERT(sum5.IsVariable());
		TEST_ASSERT(!sum5.IsStructure());
		TEST_ASSERT(sum5.GetName()==L"Sum5Result");
		TEST_ASSERT(sum5.GetType().IsPrimitive() && sum5.GetType().GetPrimitive()==BasicTypeRes::int_type);

		BasicDeclarationInfo main=metadata->GetDeclaration(6);
		TEST_ASSERT(main.IsFunction());
		TEST_ASSERT(!main.IsVariable());
		TEST_ASSERT(!main.IsStructure());
		TEST_ASSERT(main.GetName()==L"Main");
		TEST_ASSERT(main.GetParameterCount()==0);
		BasicTypeInfo mainType=main.GetType();
		TEST_ASSERT(!mainType.IsArray());
		TEST_ASSERT(mainType.IsFunction());
		TEST_ASSERT(!mainType.IsPointer());
		TEST_ASSERT(!mainType.IsPrimitive());
		TEST_ASSERT(!mainType.IsStructure());
		TEST_ASSERT(mainType.GetElementType().IsPrimitive() && mainType.GetElementType().GetPrimitive()==BasicTypeRes::void_type);

		LanguageHost host(65536);
		host.LoadAssembly(assembly);
		Ptr<LanguageState> state=host.CreateState();
		TEST_ASSERT(state->RunInitialization(assembly)==basicil::ILException::Finished);

		List<BasicDeclarationInfo> sums;
		sums.Add(metadata->GetDeclaration(0));
		sums.Add(metadata->GetDeclaration(1));
		sums.Add(metadata->GetDeclaration(2));
		sums.Add(metadata->GetDeclaration(3));
		sums.Add(metadata->GetDeclaration(5));

		for(vint i=0;i<sums.Count();i++)
		{
			BasicFunctionExecutor<vint(vint*,vint)> sum(sums[i], state);
			{
				TEST_ASSERT(sum(0, 0)==0);
			}
			{
				vint nums[]={3,5,1,4,2};
				TEST_ASSERT(sum(nums, sizeof(nums)/sizeof(*nums))==15);
			}
		}
	}
}

TEST_CASE(Test_NativeX_GenericStructure)
{
	Ptr<LanguageAssembly> assembly=TestNativeXNoError(
		LINE_(	unit generic_structure;										)
		LINE_(	generic<T>     												)
		LINE_(	structure Vector											)
		LINE_(	{															)
		LINE_(		T x;													)
		LINE_(		T y;													)
		LINE_(	}															)
		LINE_(	function int Main()											)
		LINE_(	{															)
		LINE_(		variable Vector<int> v;									)
		LINE_(		v.x=10;													)
		LINE_(		v.y=20;													)
		LINE_(		result=v.x+v.y;											)
		LINE_(	}															)
		);
	{
		BasicLanguageMetadata* metadata=assembly->GetBasicLanguageMetadata();
		TEST_ASSERT(metadata->GetDeclarationCount()==2);

		BasicDeclarationInfo vector=metadata->GetDeclaration(0);
		BasicDeclarationInfo main=metadata->GetDeclaration(1);

		TEST_ASSERT(vector.IsFunction()==false);
		TEST_ASSERT(vector.IsStructure()==true);
		TEST_ASSERT(vector.IsVariable()==false);
		TEST_ASSERT(vector.IsLinkingSymbol()==false);
		TEST_ASSERT(vector.GetName()==L"Vector");
		TEST_ASSERT(vector.GetGenericArgumentCount()==1);
		TEST_ASSERT(vector.GetGenericArgumentName(0)==L"T");
		BasicTypeInfo vectorType=vector.GetType();
		TEST_ASSERT(vectorType.IsArray()==false);
		TEST_ASSERT(vectorType.IsFunction()==false);
		TEST_ASSERT(vectorType.IsPointer()==false);
		TEST_ASSERT(vectorType.IsStructure()==true);
		TEST_ASSERT(vectorType.IsPrimitive()==false);
		TEST_ASSERT(vectorType.IsGenericArgument()==false);
		TEST_ASSERT(vectorType.GetComponentCount()==2);
		TEST_ASSERT(vectorType.GetComponentName(0)==L"x");
		TEST_ASSERT(vectorType.GetComponentOffset(0)==-1);
		TEST_ASSERT(vectorType.GetComponentType(0).IsGenericArgument()==true);
		TEST_ASSERT(vectorType.GetComponentType(0).GetGenericArgumentName()==L"T");
		TEST_ASSERT(vectorType.GetComponentName(1)==L"y");
		TEST_ASSERT(vectorType.GetComponentOffset(1)==-1);
		TEST_ASSERT(vectorType.GetComponentType(1).IsGenericArgument()==true);
		TEST_ASSERT(vectorType.GetComponentType(1).GetGenericArgumentName()==L"T");

		TEST_ASSERT(main.IsFunction()==true);
		TEST_ASSERT(main.IsStructure()==false);
		TEST_ASSERT(main.IsVariable()==false);
		TEST_ASSERT(main.IsLinkingSymbol()==false);
		TEST_ASSERT(main.GetName()==L"Main");
		TEST_ASSERT(main.GetGenericArgumentCount()==0);
		TEST_ASSERT(main.GetParameterCount()==0);
		BasicTypeInfo mainType=main.GetType();
		TEST_ASSERT(mainType.IsArray()==false);
		TEST_ASSERT(mainType.IsFunction()==true);
		TEST_ASSERT(mainType.IsPointer()==false);
		TEST_ASSERT(mainType.IsStructure()==false);
		TEST_ASSERT(mainType.IsPrimitive()==false);
		TEST_ASSERT(mainType.IsGenericArgument()==false);
		TEST_ASSERT(mainType.GetElementType().IsPrimitive() && mainType.GetElementType().GetPrimitive()==BasicTypeRes::int_type);

		LanguageHost host(65536);
		host.LoadAssembly(assembly);
		Ptr<LanguageState> state=host.CreateState();
		TEST_ASSERT(state->RunInitialization(assembly)==basicil::ILException::Finished);
		BasicFunctionExecutor<vint()> mainFunction(main, state);
		TEST_ASSERT(mainFunction()==30);
	}
}

vint __stdcall Test_NativeX_ForeignFunction_Summer(void* result, void* arguments)
{
	vint* numbers=*(vint**)(arguments);
	vint count=*(vint*)((char*)arguments+sizeof(vint*));
	vint sum=0;
	for(vint i=0;i<count;i++)
	{
		sum+=numbers[i];
	}
	*((vint*)result)=sum;
	return sizeof(vint*)+sizeof(vint);
}

TEST_CASE(Test_NativeX_ForeignFunction)
{
	Ptr<LanguageAssembly> assembly=TestNativeXNoError(
		LINE_(	unit foreign_function;													)
		LINE_(	foreign function int Sum(int* numbers, int count) alias Foreign.Sum;	)
		LINE_(	function int main()														)
		LINE_(	{																		)
		LINE_(		variable int[5] numbers;											)
		LINE_(		(numbers[0] = 1);													)
		LINE_(		(numbers[1] = 2);													)
		LINE_(		(numbers[2] = 3);													)
		LINE_(		(numbers[3] = 4);													)
		LINE_(		(numbers[4] = 5);													)
		LINE_(		(result = Sum(cast<int*>(( & numbers)), 5));						)
		LINE_(	}																		)
		);
	{
		BasicLanguageMetadata* metadata=assembly->GetBasicLanguageMetadata();
		BasicDeclarationInfo main=metadata->GetDeclaration(1);

		LanguageHost host(65536);
		host.RegisterForeignFunction(L"Foreign", L"Sum", Test_NativeX_ForeignFunction_Summer);
		host.LoadAssembly(assembly);
		Ptr<LanguageState> state=host.CreateState();
		TEST_ASSERT(state->RunInitialization(assembly)==basicil::ILException::Finished);
		BasicFunctionExecutor<vint()> mainFunction(main, state);
		TEST_ASSERT(mainFunction()==15);
	}
}