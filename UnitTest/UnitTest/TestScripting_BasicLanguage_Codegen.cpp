#include "..\..\Library\UnitTest\UnitTest.h"
#include "..\..\Library\Function.h"
#include "..\..\Library\Scripting\BasicLanguage\BasicLanguageTypeManager.h"
#include "..\..\Library\Scripting\BasicLanguage\BasicLanguageWriter.h"
#include "..\..\Library\Scripting\BasicLanguage\BasicLanguageAnalyzer.h"
#include "..\..\Library\Scripting\BasicLanguage\BasicLanguageCodeGeneration.h"
#include "..\..\Library\Scripting\BasicIL\BasicILInterpretor.h"

using namespace vl;
using namespace vl::collections;
using namespace vl::scripting::basiclanguage;
using namespace vl::scripting::basicil;

/***********************************************************************
Size
***********************************************************************/

TEST_CASE(TestBasicTypeInfo)
{
	BasicProgramNode program;
	program.DefineStructureForward(L"Link");
	program.DefineRename(L"PLink", *t_type(L"Link"));
	program.DefineStructure(L"Link")
		.Member(L"data", t_int())
		.Member(L"prev", t_type(L"PLink"))
		.Member(L"next", t_type(L"PLink"));
	program.DefineStructure(L"Type")
		.Member(L"a", t_char())
		.Member(L"b", t_int32())
		.Member(L"c", t_int8())
		.Member(L"d", t_int16())
		.Member(L"e", t_double())
		.Member(L"f", t_int8());

	BasicAnalyzer analyzer(program.GetInternalValue(), 0, BasicAlgorithmConfiguration());
	analyzer.Analyze();
	BasicCodegenInfo info(&analyzer);

	List<Ptr<BasicLanguageCodeException>> errors;
	SortedList<WString> forwardStructures;
	BP argument(analyzer.GetEnv(), analyzer.GetEnv()->GlobalScope(), analyzer.GetTypeManager(), errors, forwardStructures);

	BasicTypeRecord* typeInt8=BasicLanguage_GetTypeRecord(t_int8().GetInternalValue(), argument);
	BasicTypeRecord* typePInt8=BasicLanguage_GetTypeRecord((*t_int8()).GetInternalValue(), argument);
	BasicTypeRecord* typeFunc=BasicLanguage_GetTypeRecord(t_int8()(t_types()<<t_bool()).GetInternalValue(), argument);
	BasicTypeRecord* typeInt8s=BasicLanguage_GetTypeRecord(t_int8()[10].GetInternalValue(), argument);
	BasicTypeRecord* typeLink=BasicLanguage_GetTypeRecord(t_type(L"Link").GetInternalValue(), argument);
	BasicTypeRecord* typeType=BasicLanguage_GetTypeRecord(t_type(L"Type").GetInternalValue(), argument);

	BasicTypeInfo* infoInt8=info.GetTypeInfo(typeInt8);
	BasicTypeInfo* infoPInt8=info.GetTypeInfo(typePInt8);
	BasicTypeInfo* infoFunc=info.GetTypeInfo(typeFunc);
	BasicTypeInfo* infoInt8s=info.GetTypeInfo(typeInt8s);
	BasicTypeInfo* infoLink=info.GetTypeInfo(typeLink);
	BasicTypeInfo* infoType=info.GetTypeInfo(typeType);

	TEST_ASSERT(infoInt8->alignment==1);
	TEST_ASSERT(infoInt8->size==1);
	TEST_ASSERT(infoPInt8->alignment==sizeof(void*));
	TEST_ASSERT(infoPInt8->size==sizeof(void*));
	TEST_ASSERT(infoFunc->alignment==sizeof(void*));
	TEST_ASSERT(infoFunc->size==sizeof(void*));
	TEST_ASSERT(infoInt8s->alignment==1);
	TEST_ASSERT(infoInt8s->size==10);
	TEST_ASSERT(infoLink->alignment==sizeof(void*));
	TEST_ASSERT(infoLink->size==sizeof(void*)*3);
	TEST_ASSERT(infoLink->offsets.Count()==3);
	TEST_ASSERT(infoLink->offsets[0]==0);
	TEST_ASSERT(infoLink->offsets[1]==sizeof(void*));
	TEST_ASSERT(infoLink->offsets[2]==sizeof(void*)*2);
	TEST_ASSERT(infoType->alignment==8);
	TEST_ASSERT(infoType->size==32);
	TEST_ASSERT(infoType->offsets.Count()==6);
	TEST_ASSERT(infoType->offsets[0]==0);
	TEST_ASSERT(infoType->offsets[1]==4);
	TEST_ASSERT(infoType->offsets[2]==8);
	TEST_ASSERT(infoType->offsets[3]==10);
	TEST_ASSERT(infoType->offsets[4]==16);
	TEST_ASSERT(infoType->offsets[5]==24);
}

/***********************************************************************
Runner
***********************************************************************/

//from TestScripting_BasicLanguage_Analyzer.cpp
extern void SetConfiguration(BasicAlgorithmConfiguration& config);

void RunBasicProgram(Ptr<BasicProgram> program, int result)
{
	BasicAlgorithmConfiguration configuration;
	SetConfiguration(configuration);
	BasicAnalyzer analyzer(program, 0, configuration);
	analyzer.Analyze();
	TEST_ASSERT(analyzer.GetErrors().Count()==0);
	BasicCodeGenerator codegen(&analyzer, 0);
	codegen.GenerateCode();
	BasicILInterpretor interpretor(65536);
	int key=interpretor.LoadIL(codegen.GetIL().Obj());
	interpretor.Reset(0, key, 0);
	TEST_ASSERT(interpretor.Run()==BasicILInterpretor::Finished);
	TEST_ASSERT(interpretor.GetEnv()->StackTop()==65536);
	int ins=codegen.GetIL()->labels[0].instructionIndex;
	interpretor.Reset(ins, key, sizeof(int));
	TEST_ASSERT(interpretor.Run()==BasicILInterpretor::Finished);
	TEST_ASSERT(interpretor.GetEnv()->StackTop()==65536-sizeof(int));
	TEST_ASSERT(interpretor.GetEnv()->Pop<int>()==result);
}

/***********************************************************************
TestPrograms
***********************************************************************/

TEST_CASE(Test_BasicLanguage_1Plus1)
{
	BasicProgramNode program;
	program.DefineFunction(L"main").ReturnType(t_int()).Statement(
		s_expr(e_result().Assign(e_prim(1)+e_prim(1)))
		);
	RunBasicProgram(program.GetInternalValue(), 2);
}