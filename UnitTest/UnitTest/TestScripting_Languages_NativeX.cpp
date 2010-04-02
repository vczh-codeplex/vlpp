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

/***********************************************************************
Assert Functions
***********************************************************************/

void AssertIsInt32(ResourceHandle<BasicTypeRes> type, Ptr<ResourceStream> stream)
{
	TEST_ASSERT(type);
	ResourceRecord<BasicTypeRes> record=stream->ReadRecord(type);
	TEST_ASSERT(record->type==BasicTypeRes::Primitive);
	TEST_ASSERT(record->primitiveType==BasicTypeRes::s32);
	TEST_ASSERT(!record->elementType);
	TEST_ASSERT(record->elementCount==-1);
	TEST_ASSERT(!record->subTypes);
}

void AssertIsVoid(ResourceHandle<BasicTypeRes> type, Ptr<ResourceStream> stream)
{
	TEST_ASSERT(type);
	ResourceRecord<BasicTypeRes> record=stream->ReadRecord(type);
	TEST_ASSERT(record->type==BasicTypeRes::Primitive);
	TEST_ASSERT(record->primitiveType==BasicTypeRes::void_type);
	TEST_ASSERT(!record->elementType);
	TEST_ASSERT(record->elementCount==-1);
	TEST_ASSERT(!record->subTypes);
}

ResourceRecord<BasicTypeRes> AssertIsPointer(ResourceHandle<BasicTypeRes> type, Ptr<ResourceStream> stream)
{
	TEST_ASSERT(type);
	ResourceRecord<BasicTypeRes> record=stream->ReadRecord(type);
	TEST_ASSERT(record->type==BasicTypeRes::Pointer);
	TEST_ASSERT(record->elementType);
	TEST_ASSERT(record->elementCount==-1);
	TEST_ASSERT(!record->subTypes);
	return stream->ReadRecord(record->elementType);
}

void AssertIsType(ResourceHandle<BasicTypeRes> type, ResourceHandle<BasicTypeRes> baseline)
{
	TEST_ASSERT(type);
	TEST_ASSERT(baseline);
	TEST_ASSERT(type.Pointer()==baseline.Pointer());
}

ResourceRecord<BasicDeclarationRes> AssertAvailableAndNext(ResourceHandle<BasicDeclarationLinkRes>& declaration, Ptr<ResourceStream> stream)
{
	TEST_ASSERT(declaration);
	ResourceRecord<BasicDeclarationLinkRes> record=stream->ReadRecord(declaration);
	TEST_ASSERT(record->declaration);
	declaration=record->next;
	return stream->ReadRecord(record->declaration);
}

ResourceRecord<BasicTypeRes> AssertAvailableAndNext(ResourceHandle<BasicTypeLinkRes>& type, const WString& name, Ptr<ResourceStream> stream)
{
	TEST_ASSERT(type);
	ResourceRecord<BasicTypeLinkRes> record=stream->ReadRecord(type);
	TEST_ASSERT(stream->ReadString(record->name)==name);
	TEST_ASSERT(record->type);
	type=record->next;
	return stream->ReadRecord(record->type);
}

void AssertAvailableAndNext(ResourceHandle<BasicParameterRes>& parameter, const WString& name, Ptr<ResourceStream> stream)
{
	ResourceRecord<BasicParameterRes> record=stream->ReadRecord(parameter);
	TEST_ASSERT(stream->ReadString(record->name)==name);
	parameter=record->next;
}

ResourceRecord<BasicTypeRes> AssertIsStructureDefinition(ResourceHandle<BasicDeclarationRes> declaration, const WString& name, Ptr<ResourceStream> stream)
{
	TEST_ASSERT(declaration);
	ResourceRecord<BasicDeclarationRes> record=stream->ReadRecord(declaration);
	TEST_ASSERT(record->type==BasicDeclarationRes::Structure);
	TEST_ASSERT(stream->ReadString(record->name)==name);
	TEST_ASSERT(record->declarationType);
	TEST_ASSERT(!record->parameterNames);
	TEST_ASSERT(record->address==-1);
	ResourceRecord<BasicTypeRes> result=stream->ReadRecord(record->declarationType);
	TEST_ASSERT(result->type==BasicTypeRes::Structure);
	return result;
}

ResourceRecord<BasicTypeRes> AssertIsFunctionDefinition(ResourceHandle<BasicDeclarationRes> declaration, const WString& name, Ptr<ResourceStream> stream, const wchar_t** parameterNames, int parameterCount)
{
	TEST_ASSERT(declaration);
	ResourceRecord<BasicDeclarationRes> record=stream->ReadRecord(declaration);
	TEST_ASSERT(record->type==BasicDeclarationRes::Function);
	TEST_ASSERT(stream->ReadString(record->name)==name);
	TEST_ASSERT(record->declarationType);
	TEST_ASSERT(record->address!=-1);
	ResourceRecord<BasicTypeRes> result=stream->ReadRecord(record->declarationType);
	TEST_ASSERT(result->type==BasicTypeRes::Function);
	TEST_ASSERT(result->elementCount==-1);

	ResourceHandle<BasicParameterRes> parameter=record->parameterNames;
	while(parameterCount--)
	{
		AssertAvailableAndNext(parameter, *parameterNames++, stream);
	}
	TEST_ASSERT(!parameter);
	return result;
}

ResourceRecord<BasicTypeRes> AssertIsVariableDefinition(ResourceHandle<BasicDeclarationRes> declaration, const WString& name, Ptr<ResourceStream> stream)
{
	TEST_ASSERT(declaration);
	ResourceRecord<BasicDeclarationRes> record=stream->ReadRecord(declaration);
	TEST_ASSERT(record->type==BasicDeclarationRes::Variable);
	TEST_ASSERT(stream->ReadString(record->name)==name);
	TEST_ASSERT(record->declarationType);
	TEST_ASSERT(!record->parameterNames);
	TEST_ASSERT(record->address!=-1);
	return stream->ReadRecord(record->declarationType);
}

/***********************************************************************
Test Cases
***********************************************************************/

TEST_CASE(Test_NativeX_EmptyProgram)
{
	Ptr<LanguageAssembly> assembly=TestNativeXNoError(
		LINE_(	unit empty;	)
		);
	{
		Ptr<ResourceStream> stream=assembly->GetResources()[BasicILResourceNames::BasicLanguageInterfaces];
		ResourceRecord<BasicEntryRes> entry=stream->ReadRootRecord<BasicEntryRes>();
		TEST_ASSERT(!entry->declarations);
	}
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
		Ptr<ResourceStream> stream=assembly->GetResources()[BasicILResourceNames::BasicLanguageInterfaces];
		ResourceRecord<BasicEntryRes> entry=stream->ReadRootRecord<BasicEntryRes>();

		ResourceHandle<BasicDeclarationLinkRes> currentDeclarationLink=entry->declarations;
		ResourceRecord<BasicDeclarationRes> pointDeclaration=AssertAvailableAndNext(currentDeclarationLink, stream);
		TEST_ASSERT(!currentDeclarationLink);
		{
			ResourceRecord<BasicTypeRes> pointType=AssertIsStructureDefinition(pointDeclaration, L"Point", stream);

			ResourceHandle<BasicTypeLinkRes> currentMember=pointType->subTypes;
			AssertIsInt32(AssertAvailableAndNext(currentMember, L"x", stream), stream);
			AssertIsInt32(AssertAvailableAndNext(currentMember, L"y", stream), stream);
			TEST_ASSERT(!currentMember);
		}
	}
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
		TEST_ASSERT(pointType.GetSize()==sizeof(int)*2);
		TEST_ASSERT(pointType.GetComponentCount()==2);
		TEST_ASSERT(pointType.GetComponentName(0)==L"x");
		TEST_ASSERT(pointType.GetComponentOffset(0)==0);
		TEST_ASSERT(pointType.GetComponentType(0).IsPrimitive() && pointType.GetComponentType(0).GetPrimitive()==BasicTypeRes::int_type);
		TEST_ASSERT(pointType.GetComponentName(1)==L"y");
		TEST_ASSERT(pointType.GetComponentOffset(1)==sizeof(int));
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
		Ptr<ResourceStream> stream=assembly->GetResources()[BasicILResourceNames::BasicLanguageInterfaces];
		ResourceRecord<BasicEntryRes> entry=stream->ReadRootRecord<BasicEntryRes>();

		ResourceHandle<BasicDeclarationLinkRes> currentDeclarationLink=entry->declarations;
		ResourceRecord<BasicDeclarationRes> bDeclaration=AssertAvailableAndNext(currentDeclarationLink, stream);
		ResourceRecord<BasicDeclarationRes> aDeclaration=AssertAvailableAndNext(currentDeclarationLink, stream);
		TEST_ASSERT(!currentDeclarationLink);
		{
			ResourceRecord<BasicTypeRes> bType=AssertIsStructureDefinition(bDeclaration, L"B", stream);

			ResourceHandle<BasicTypeLinkRes> currentMember=bType->subTypes;
			AssertIsInt32(AssertAvailableAndNext(currentMember, L"x", stream), stream);
			AssertIsInt32(AssertAvailableAndNext(currentMember, L"y", stream), stream);
			AssertIsType(AssertIsPointer(AssertAvailableAndNext(currentMember, L"a", stream), stream), aDeclaration->declarationType);
			TEST_ASSERT(!currentMember);
		}
		{
			ResourceRecord<BasicTypeRes> aType=AssertIsStructureDefinition(aDeclaration, L"A", stream);

			ResourceHandle<BasicTypeLinkRes> currentMember=aType->subTypes;
			AssertIsInt32(AssertAvailableAndNext(currentMember, L"a", stream), stream);
			TEST_ASSERT(!currentMember);
		}
	}
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
		TEST_ASSERT(bType.GetSize()==sizeof(int)*3);
		TEST_ASSERT(bType.GetComponentCount()==3);
		TEST_ASSERT(bType.GetComponentName(0)==L"x");
		TEST_ASSERT(bType.GetComponentOffset(0)==0);
		TEST_ASSERT(bType.GetComponentType(0).IsPrimitive() && bType.GetComponentType(0).GetPrimitive()==BasicTypeRes::int_type);
		TEST_ASSERT(bType.GetComponentName(1)==L"y");
		TEST_ASSERT(bType.GetComponentOffset(1)==sizeof(int));
		TEST_ASSERT(bType.GetComponentType(1).IsPrimitive() && bType.GetComponentType(1).GetPrimitive()==BasicTypeRes::int_type);
		TEST_ASSERT(bType.GetComponentName(2)==L"a");
		TEST_ASSERT(bType.GetComponentOffset(2)==sizeof(int)*2);
		TEST_ASSERT(bType.GetComponentType(2).IsPointer() && bType.GetComponentType(2).GetElementType().IsSameRecord(a.GetType()));

		BasicTypeInfo aType=a.GetType();
		TEST_ASSERT(aType.IsPrimitive()==false);
		TEST_ASSERT(aType.IsPointer()==false);
		TEST_ASSERT(aType.IsArray()==false);
		TEST_ASSERT(aType.IsFunction()==false);
		TEST_ASSERT(aType.IsStructure()==true);
		TEST_ASSERT(aType.GetSize()==sizeof(int));
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
		Ptr<ResourceStream> stream=assembly->GetResources()[BasicILResourceNames::BasicLanguageInterfaces];
		ResourceRecord<BasicEntryRes> entry=stream->ReadRootRecord<BasicEntryRes>();

		ResourceHandle<BasicDeclarationLinkRes> currentDeclarationLink=entry->declarations;
		ResourceRecord<BasicDeclarationRes> linkDeclaration=AssertAvailableAndNext(currentDeclarationLink, stream);
		TEST_ASSERT(!currentDeclarationLink);
		{
			ResourceRecord<BasicTypeRes> linkType=AssertIsStructureDefinition(linkDeclaration, L"Link", stream);

			ResourceHandle<BasicTypeLinkRes> currentMember=linkType->subTypes;
			AssertIsInt32(AssertAvailableAndNext(currentMember, L"data", stream), stream);
			AssertIsType(AssertIsPointer(AssertAvailableAndNext(currentMember, L"next", stream), stream), linkType);
			TEST_ASSERT(!currentMember);
		}
	}
	{
		BasicLanguageMetadata* metadata=assembly->GetBasicLanguageMetadata();
		TEST_ASSERT(metadata->GetDeclarationCount()==1);

		BasicDeclarationInfo link=metadata->GetDeclaration(0);
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
		TEST_ASSERT(linkType.GetSize()==sizeof(int)*2);
		TEST_ASSERT(linkType.GetComponentCount()==2);
		TEST_ASSERT(linkType.GetComponentName(0)==L"data");
		TEST_ASSERT(linkType.GetComponentOffset(0)==0);
		TEST_ASSERT(linkType.GetComponentType(0).IsPrimitive() && linkType.GetComponentType(0).GetPrimitive()==BasicTypeRes::int_type);
		TEST_ASSERT(linkType.GetComponentName(1)==L"next");
		TEST_ASSERT(linkType.GetComponentOffset(1)==sizeof(int));
		TEST_ASSERT(linkType.GetComponentType(1).IsPointer() && linkType.GetComponentType(1).GetElementType().IsSameRecord(linkType));
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
		Ptr<ResourceStream> stream=assembly->GetResources()[BasicILResourceNames::BasicLanguageInterfaces];
		ResourceRecord<BasicEntryRes> entry=stream->ReadRootRecord<BasicEntryRes>();

		ResourceHandle<BasicDeclarationLinkRes> currentDeclarationLink=entry->declarations;
		ResourceRecord<BasicDeclarationRes> commandDeclaration=AssertAvailableAndNext(currentDeclarationLink, stream);
		TEST_ASSERT(!currentDeclarationLink);
		{
			ResourceRecord<BasicTypeRes> commandType=AssertIsStructureDefinition(commandDeclaration, L"Command", stream);

			ResourceHandle<BasicTypeLinkRes> currentMember=commandType->subTypes;
			ResourceRecord<BasicTypeRes> functionType=AssertAvailableAndNext(currentMember, L"executor", stream);
			{
				TEST_ASSERT(functionType->type==BasicTypeRes::Function);
				AssertIsVoid(functionType->elementType, stream);
				TEST_ASSERT(functionType->elementCount=-1);
				ResourceHandle<BasicTypeLinkRes> currentParameter=functionType->subTypes;
				AssertIsType(AssertIsPointer(AssertAvailableAndNext(currentParameter, L"", stream), stream), commandType);
				TEST_ASSERT(!currentParameter);
			}
			AssertIsType(AssertIsPointer(AssertAvailableAndNext(currentMember, L"previous", stream), stream), commandType);
			AssertIsType(AssertIsPointer(AssertAvailableAndNext(currentMember, L"next", stream), stream), commandType);
			TEST_ASSERT(!currentMember);
		}
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
		Ptr<ResourceStream> stream=assembly->GetResources()[BasicILResourceNames::BasicLanguageInterfaces];
		ResourceRecord<BasicEntryRes> entry=stream->ReadRootRecord<BasicEntryRes>();

		ResourceHandle<BasicDeclarationLinkRes> currentDeclaration=entry->declarations;
		ResourceRecord<BasicDeclarationRes> addFunction=AssertAvailableAndNext(currentDeclaration, stream);
		ResourceRecord<BasicDeclarationRes> subFunction=AssertAvailableAndNext(currentDeclaration, stream);
		TEST_ASSERT(!currentDeclaration);
		{
			const wchar_t* parameters[]={L"a", L"b"};
			ResourceRecord<BasicTypeRes> functionType=AssertIsFunctionDefinition(addFunction, L"Add", stream, parameters, sizeof(parameters)/sizeof(*parameters));
			AssertIsInt32(functionType->elementType, stream);
			ResourceHandle<BasicTypeLinkRes> currentParameter=functionType->subTypes;
			AssertIsInt32(AssertAvailableAndNext(currentParameter, L"", stream), stream);
			AssertIsInt32(AssertAvailableAndNext(currentParameter, L"", stream), stream);
			TEST_ASSERT(!currentParameter);
		}
		{
			const wchar_t* parameters[]={L"a", L"b"};
			ResourceRecord<BasicTypeRes> functionType=AssertIsFunctionDefinition(subFunction, L"Sub", stream, parameters, sizeof(parameters)/sizeof(*parameters));
			AssertIsInt32(functionType->elementType, stream);
			ResourceHandle<BasicTypeLinkRes> currentParameter=functionType->subTypes;
			AssertIsInt32(AssertAvailableAndNext(currentParameter, L"", stream), stream);
			AssertIsInt32(AssertAvailableAndNext(currentParameter, L"", stream), stream);
			TEST_ASSERT(!currentParameter);
		}
	}
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
	{
		Ptr<ResourceStream> stream=assembly->GetResources()[BasicILResourceNames::BasicLanguageInterfaces];
		ResourceRecord<BasicEntryRes> entry=stream->ReadRootRecord<BasicEntryRes>();

		ResourceHandle<BasicDeclarationLinkRes> currentDeclaration=entry->declarations;
		ResourceRecord<BasicDeclarationRes> sortFunction=AssertAvailableAndNext(currentDeclaration, stream);
		ResourceRecord<BasicDeclarationRes> mainFunction=AssertAvailableAndNext(currentDeclaration, stream);
		TEST_ASSERT(!currentDeclaration);
		{
			const wchar_t* parameters[]={L"nums", L"count"};
			ResourceRecord<BasicTypeRes> functionType=AssertIsFunctionDefinition(sortFunction, L"Sort", stream, parameters, sizeof(parameters)/sizeof(*parameters));
			AssertIsVoid(functionType->elementType, stream);
			ResourceHandle<BasicTypeLinkRes> currentParameter=functionType->subTypes;
			AssertIsInt32(AssertIsPointer(AssertAvailableAndNext(currentParameter, L"", stream), stream), stream);
			AssertIsInt32(AssertAvailableAndNext(currentParameter, L"", stream), stream);
			TEST_ASSERT(!currentParameter);
		}
		{
			ResourceRecord<BasicTypeRes> functionType=AssertIsFunctionDefinition(mainFunction, L"Main", stream, 0, 0);
			AssertIsVoid(functionType->elementType, stream);
			ResourceHandle<BasicTypeLinkRes> currentParameter=functionType->subTypes;
			TEST_ASSERT(!currentParameter);
		}
	}
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
	{
		Ptr<ResourceStream> stream=assembly->GetResources()[BasicILResourceNames::BasicLanguageInterfaces];
		ResourceRecord<BasicEntryRes> entry=stream->ReadRootRecord<BasicEntryRes>();

		ResourceHandle<BasicDeclarationLinkRes> currentDeclaration=entry->declarations;
		ResourceRecord<BasicDeclarationRes> sum1Function=AssertAvailableAndNext(currentDeclaration, stream);
		ResourceRecord<BasicDeclarationRes> sum2Function=AssertAvailableAndNext(currentDeclaration, stream);
		ResourceRecord<BasicDeclarationRes> sum3Function=AssertAvailableAndNext(currentDeclaration, stream);
		ResourceRecord<BasicDeclarationRes> sum4Function=AssertAvailableAndNext(currentDeclaration, stream);
		ResourceRecord<BasicDeclarationRes> sum5Variable=AssertAvailableAndNext(currentDeclaration, stream);
		ResourceRecord<BasicDeclarationRes> sum5Function=AssertAvailableAndNext(currentDeclaration, stream);
		ResourceRecord<BasicDeclarationRes> mainFunction=AssertAvailableAndNext(currentDeclaration, stream);
		TEST_ASSERT(!currentDeclaration);
		{
			const wchar_t* parameters[]={L"nums", L"count"};
			ResourceRecord<BasicTypeRes> functionType=AssertIsFunctionDefinition(sum1Function, L"Sum1", stream, parameters, sizeof(parameters)/sizeof(*parameters));
			AssertIsInt32(functionType->elementType, stream);
			ResourceHandle<BasicTypeLinkRes> currentParameter=functionType->subTypes;
			AssertIsInt32(AssertIsPointer(AssertAvailableAndNext(currentParameter, L"", stream), stream), stream);
			AssertIsInt32(AssertAvailableAndNext(currentParameter, L"", stream), stream);
			TEST_ASSERT(!currentParameter);
		}
		{
			const wchar_t* parameters[]={L"nums", L"count"};
			ResourceRecord<BasicTypeRes> functionType=AssertIsFunctionDefinition(sum2Function, L"Sum2", stream, parameters, sizeof(parameters)/sizeof(*parameters));
			AssertIsInt32(functionType->elementType, stream);
			ResourceHandle<BasicTypeLinkRes> currentParameter=functionType->subTypes;
			AssertIsInt32(AssertIsPointer(AssertAvailableAndNext(currentParameter, L"", stream), stream), stream);
			AssertIsInt32(AssertAvailableAndNext(currentParameter, L"", stream), stream);
			TEST_ASSERT(!currentParameter);
		}
		{
			const wchar_t* parameters[]={L"nums", L"count"};
			ResourceRecord<BasicTypeRes> functionType=AssertIsFunctionDefinition(sum3Function, L"Sum3", stream, parameters, sizeof(parameters)/sizeof(*parameters));
			AssertIsInt32(functionType->elementType, stream);
			ResourceHandle<BasicTypeLinkRes> currentParameter=functionType->subTypes;
			AssertIsInt32(AssertIsPointer(AssertAvailableAndNext(currentParameter, L"", stream), stream), stream);
			AssertIsInt32(AssertAvailableAndNext(currentParameter, L"", stream), stream);
			TEST_ASSERT(!currentParameter);
		}
		{
			const wchar_t* parameters[]={L"nums", L"count"};
			ResourceRecord<BasicTypeRes> functionType=AssertIsFunctionDefinition(sum4Function, L"Sum4", stream, parameters, sizeof(parameters)/sizeof(*parameters));
			AssertIsInt32(functionType->elementType, stream);
			ResourceHandle<BasicTypeLinkRes> currentParameter=functionType->subTypes;
			AssertIsInt32(AssertIsPointer(AssertAvailableAndNext(currentParameter, L"", stream), stream), stream);
			AssertIsInt32(AssertAvailableAndNext(currentParameter, L"", stream), stream);
			TEST_ASSERT(!currentParameter);
		}
		{
			AssertIsInt32(AssertIsVariableDefinition(sum5Variable, L"Sum5Result", stream), stream);
		}
		{
			const wchar_t* parameters[]={L"nums", L"count"};
			ResourceRecord<BasicTypeRes> functionType=AssertIsFunctionDefinition(sum5Function, L"Sum5", stream, parameters, sizeof(parameters)/sizeof(*parameters));
			AssertIsInt32(functionType->elementType, stream);
			ResourceHandle<BasicTypeLinkRes> currentParameter=functionType->subTypes;
			AssertIsInt32(AssertIsPointer(AssertAvailableAndNext(currentParameter, L"", stream), stream), stream);
			AssertIsInt32(AssertAvailableAndNext(currentParameter, L"", stream), stream);
			TEST_ASSERT(!currentParameter);
		}
		{
			ResourceRecord<BasicTypeRes> functionType=AssertIsFunctionDefinition(mainFunction, L"Main", stream, 0, 0);
			AssertIsVoid(functionType->elementType, stream);
			ResourceHandle<BasicTypeLinkRes> currentParameter=functionType->subTypes;
			TEST_ASSERT(!currentParameter);
		}
	}
}