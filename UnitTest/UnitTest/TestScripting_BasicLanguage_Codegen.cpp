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

template<typename T>
void RunBasicProgram(Ptr<BasicProgram> program, T result)
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
	interpretor.Reset(ins, key, sizeof(T));
	TEST_ASSERT(interpretor.Run()==BasicILInterpretor::Finished);
	TEST_ASSERT(interpretor.GetEnv()->StackTop()==65536-sizeof(T));
	TEST_ASSERT(interpretor.GetEnv()->Pop<T>()==result);
}

/***********************************************************************
Test Operators
***********************************************************************/

TEST_CASE(Test_BasicLanguage_1Plus1)
{
	BasicProgramNode program;
	program.DefineFunction(L"main").ReturnType(t_int()).Statement(
		s_expr(e_result().Assign(e_prim(1)+e_prim(1)))
		);
	RunBasicProgram<int>(program.GetInternalValue(), 2);
}

TEST_CASE(Test_BasicLanguage_2Minus3)
{
	BasicProgramNode program;
	program.DefineFunction(L"main").ReturnType(t_int()).Statement(
		s_expr(e_result().Assign(e_prim(2)-e_prim(3)))
		);
	RunBasicProgram<int>(program.GetInternalValue(), -1);
}

TEST_CASE(Test_BasicLanguage_7Multiply8)
{
	BasicProgramNode program;
	program.DefineFunction(L"main").ReturnType(t_int()).Statement(
		s_expr(e_result().Assign(e_prim(7)*e_prim(8)))
		);
	RunBasicProgram<int>(program.GetInternalValue(), 56);
}

TEST_CASE(Test_BasicLanguage_6Div3)
{
	BasicProgramNode program;
	program.DefineFunction(L"main").ReturnType(t_int()).Statement(
		s_expr(e_result().Assign(e_prim(6)/e_prim(3)))
		);
	RunBasicProgram<int>(program.GetInternalValue(), 2);
}

TEST_CASE(Test_BasicLanguage_10Mod3)
{
	BasicProgramNode program;
	program.DefineFunction(L"main").ReturnType(t_int()).Statement(
		s_expr(e_result().Assign(e_prim(10)%e_prim(3)))
		);
	RunBasicProgram<int>(program.GetInternalValue(), 1);
}

TEST_CASE(Test_BasicLanguage_10Shl3)
{
	BasicProgramNode program;
	program.DefineFunction(L"main").ReturnType(t_int()).Statement(
		s_expr(e_result().Assign(e_prim(10)<<e_prim(3)))
		);
	RunBasicProgram<int>(program.GetInternalValue(), 80);
}

TEST_CASE(Test_BasicLanguage_10Shr3)
{
	BasicProgramNode program;
	program.DefineFunction(L"main").ReturnType(t_int()).Statement(
		s_expr(e_result().Assign(e_prim(10)>>e_prim(3)))
		);
	RunBasicProgram<int>(program.GetInternalValue(), 1);
}

TEST_CASE(Test_BasicLanguage_6And10)
{
	BasicProgramNode program;
	program.DefineFunction(L"main").ReturnType(t_int()).Statement(
		s_expr(e_result().Assign(e_prim(6)&e_prim(10)))
		);
	RunBasicProgram<int>(program.GetInternalValue(), 2);
}

TEST_CASE(Test_BasicLanguage_6Or10)
{
	BasicProgramNode program;
	program.DefineFunction(L"main").ReturnType(t_int()).Statement(
		s_expr(e_result().Assign(e_prim(6)|e_prim(10)))
		);
	RunBasicProgram<int>(program.GetInternalValue(), 14);
}

TEST_CASE(Test_BasicLanguage_6Xor10)
{
	BasicProgramNode program;
	program.DefineFunction(L"main").ReturnType(t_int()).Statement(
		s_expr(e_result().Assign(e_prim(6)^e_prim(10)))
		);
	RunBasicProgram<int>(program.GetInternalValue(), 12);
}

TEST_CASE(Test_BasicLanguage_Neg1Lt2)
{
	BasicProgramNode program;
	program.DefineFunction(L"main").ReturnType(t_bool()).Statement(
		s_expr(e_result().Assign(e_prim(-1)<e_prim(2)))
		);
	RunBasicProgram<bool>(program.GetInternalValue(), true);
}

TEST_CASE(Test_BasicLanguage_Neg1Le2)
{
	BasicProgramNode program;
	program.DefineFunction(L"main").ReturnType(t_bool()).Statement(
		s_expr(e_result().Assign(e_prim(-1)<=e_prim(2)))
		);
	RunBasicProgram<bool>(program.GetInternalValue(), true);
}

TEST_CASE(Test_BasicLanguage_Neg1Gt2)
{
	BasicProgramNode program;
	program.DefineFunction(L"main").ReturnType(t_bool()).Statement(
		s_expr(e_result().Assign(e_prim(-1)>e_prim(2)))
		);
	RunBasicProgram<bool>(program.GetInternalValue(), false);
}

TEST_CASE(Test_BasicLanguage_Neg1Ge2)
{
	BasicProgramNode program;
	program.DefineFunction(L"main").ReturnType(t_bool()).Statement(
		s_expr(e_result().Assign(e_prim(-1)>=e_prim(2)))
		);
	RunBasicProgram<bool>(program.GetInternalValue(), false);
}

TEST_CASE(Test_BasicLanguage_Neg1Eq2)
{
	BasicProgramNode program;
	program.DefineFunction(L"main").ReturnType(t_bool()).Statement(
		s_expr(e_result().Assign(e_prim(-1)==e_prim(2)))
		);
	RunBasicProgram<bool>(program.GetInternalValue(), false);
}

TEST_CASE(Test_BasicLanguage_Neg1Ne2)
{
	BasicProgramNode program;
	program.DefineFunction(L"main").ReturnType(t_bool()).Statement(
		s_expr(e_result().Assign(e_prim(-1)!=e_prim(2)))
		);
	RunBasicProgram<bool>(program.GetInternalValue(), true);
}

TEST_CASE(Test_BasicLanguage_3PlusPlus)
{
	BasicProgramNode program;
	program.DefineFunction(L"main").ReturnType(t_int()).Statement(
		s_var(t_int(), L"a", e_prim(3))
		<<s_var(t_int(), L"b", e_name(L"a")++)
		<<s_expr(e_result().Assign(e_name(L"b")*e_prim(100)+e_name(L"a")))
		);
	RunBasicProgram<int>(program.GetInternalValue(), 304);
}

TEST_CASE(Test_BasicLanguage_3MinusMinus)
{
	BasicProgramNode program;
	program.DefineFunction(L"main").ReturnType(t_int()).Statement(
		s_var(t_int(), L"a", e_prim(3))
		<<s_var(t_int(), L"b", e_name(L"a")--)
		<<s_expr(e_result().Assign(e_name(L"b")*e_prim(100)+e_name(L"a")))
		);
	RunBasicProgram<int>(program.GetInternalValue(), 302);
}

TEST_CASE(Test_BasicLanguage_PlusPlus3)
{
	BasicProgramNode program;
	program.DefineFunction(L"main").ReturnType(t_int()).Statement(
		s_var(t_int(), L"a", e_prim(3))
		<<s_var(t_int(), L"b", ++e_name(L"a"))
		<<s_expr(e_result().Assign(e_name(L"b")*e_prim(100)+e_name(L"a")))
		);
	RunBasicProgram<int>(program.GetInternalValue(), 404);
}

TEST_CASE(Test_BasicLanguage_MinusMinus3)
{
	BasicProgramNode program;
	program.DefineFunction(L"main").ReturnType(t_int()).Statement(
		s_var(t_int(), L"a", e_prim(3))
		<<s_var(t_int(), L"b", --e_name(L"a"))
		<<s_expr(e_result().Assign(e_name(L"b")*e_prim(100)+e_name(L"a")))
		);
	RunBasicProgram<int>(program.GetInternalValue(), 202);
}

TEST_CASE(Test_BasicLanguage_AddAssign)
{
	BasicProgramNode program;
	program.DefineFunction(L"main").ReturnType(t_int()).Statement(
		s_var(t_int(), L"a", e_prim(10))
		<<s_expr(e_result().Assign(e_name(L"a")+=e_prim(3)))
		);
	RunBasicProgram<int>(program.GetInternalValue(), 13);
}

TEST_CASE(Test_BasicLanguage_SubAssign)
{
	BasicProgramNode program;
	program.DefineFunction(L"main").ReturnType(t_int()).Statement(
		s_var(t_int(), L"a", e_prim(10))
		<<s_expr(e_result().Assign(e_name(L"a")-=e_prim(3)))
		);
	RunBasicProgram<int>(program.GetInternalValue(), 7);
}

TEST_CASE(Test_BasicLanguage_MulAssign)
{
	BasicProgramNode program;
	program.DefineFunction(L"main").ReturnType(t_int()).Statement(
		s_var(t_int(), L"a", e_prim(10))
		<<s_expr(e_result().Assign(e_name(L"a")*=e_prim(3)))
		);
	RunBasicProgram<int>(program.GetInternalValue(), 30);
}

TEST_CASE(Test_BasicLanguage_DivAssign)
{
	BasicProgramNode program;
	program.DefineFunction(L"main").ReturnType(t_int()).Statement(
		s_var(t_int(), L"a", e_prim(10))
		<<s_expr(e_result().Assign(e_name(L"a")/=e_prim(3)))
		);
	RunBasicProgram<int>(program.GetInternalValue(), 3);
}

TEST_CASE(Test_BasicLanguage_ModAssign)
{
	BasicProgramNode program;
	program.DefineFunction(L"main").ReturnType(t_int()).Statement(
		s_var(t_int(), L"a", e_prim(10))
		<<s_expr(e_result().Assign(e_name(L"a")%=e_prim(3)))
		);
	RunBasicProgram<int>(program.GetInternalValue(), 1);
}

TEST_CASE(Test_BasicLanguage_ShlAssign)
{
	BasicProgramNode program;
	program.DefineFunction(L"main").ReturnType(t_int()).Statement(
		s_var(t_int(), L"a", e_prim(10))
		<<s_expr(e_result().Assign(e_name(L"a")<<=e_prim(3)))
		);
	RunBasicProgram<int>(program.GetInternalValue(), 80);
}

TEST_CASE(Test_BasicLanguage_ShrAssign)
{
	BasicProgramNode program;
	program.DefineFunction(L"main").ReturnType(t_int()).Statement(
		s_var(t_int(), L"a", e_prim(10))
		<<s_expr(e_result().Assign(e_name(L"a")>>=e_prim(3)))
		);
	RunBasicProgram<int>(program.GetInternalValue(), 1);
}

TEST_CASE(Test_BasicLanguage_BitAndAssign)
{
	BasicProgramNode program;
	program.DefineFunction(L"main").ReturnType(t_int()).Statement(
		s_var(t_int(), L"a", e_prim(10))
		<<s_expr(e_result().Assign(e_name(L"a")&=e_prim(3)))
		);
	RunBasicProgram<int>(program.GetInternalValue(), 2);
}

TEST_CASE(Test_BasicLanguage_BitOrAssign)
{
	BasicProgramNode program;
	program.DefineFunction(L"main").ReturnType(t_int()).Statement(
		s_var(t_int(), L"a", e_prim(10))
		<<s_expr(e_result().Assign(e_name(L"a")|=e_prim(3)))
		);
	RunBasicProgram<int>(program.GetInternalValue(), 11);
}

TEST_CASE(Test_BasicLanguage_XorAssign)
{
	BasicProgramNode program;
	program.DefineFunction(L"main").ReturnType(t_int()).Statement(
		s_var(t_int(), L"a", e_prim(10))
		<<s_expr(e_result().Assign(e_name(L"a")^=e_prim(3)))
		);
	RunBasicProgram<int>(program.GetInternalValue(), 9);
}

TEST_CASE(Test_BasicLanguage_AddAssignRef)
{
	BasicProgramNode program;
	program.DefineFunction(L"main").ReturnType(t_int()).Statement(
		s_var(t_int(), L"a", e_prim(10))
		<<s_expr(e_result().Assign((e_name(L"a")+=e_prim(3))+=e_prim(1)))
		);
	RunBasicProgram<int>(program.GetInternalValue(), 14);
}

TEST_CASE(Test_BasicLanguage_SubAssignRef)
{
	BasicProgramNode program;
	program.DefineFunction(L"main").ReturnType(t_int()).Statement(
		s_var(t_int(), L"a", e_prim(10))
		<<s_expr(e_result().Assign((e_name(L"a")-=e_prim(3))+=e_prim(1)))
		);
	RunBasicProgram<int>(program.GetInternalValue(), 8);
}

TEST_CASE(Test_BasicLanguage_MulAssignRef)
{
	BasicProgramNode program;
	program.DefineFunction(L"main").ReturnType(t_int()).Statement(
		s_var(t_int(), L"a", e_prim(10))
		<<s_expr(e_result().Assign((e_name(L"a")*=e_prim(3))+=e_prim(1)))
		);
	RunBasicProgram<int>(program.GetInternalValue(), 31);
}

TEST_CASE(Test_BasicLanguage_DivAssignRef)
{
	BasicProgramNode program;
	program.DefineFunction(L"main").ReturnType(t_int()).Statement(
		s_var(t_int(), L"a", e_prim(10))
		<<s_expr(e_result().Assign((e_name(L"a")/=e_prim(3))+=e_prim(1)))
		);
	RunBasicProgram<int>(program.GetInternalValue(), 4);
}

TEST_CASE(Test_BasicLanguage_ModAssignRef)
{
	BasicProgramNode program;
	program.DefineFunction(L"main").ReturnType(t_int()).Statement(
		s_var(t_int(), L"a", e_prim(10))
		<<s_expr(e_result().Assign((e_name(L"a")%=e_prim(3))+=e_prim(1)))
		);
	RunBasicProgram<int>(program.GetInternalValue(), 2);
}

TEST_CASE(Test_BasicLanguage_ShlAssignRef)
{
	BasicProgramNode program;
	program.DefineFunction(L"main").ReturnType(t_int()).Statement(
		s_var(t_int(), L"a", e_prim(10))
		<<s_expr(e_result().Assign((e_name(L"a")<<=e_prim(3))+=e_prim(1)))
		);
	RunBasicProgram<int>(program.GetInternalValue(), 81);
}

TEST_CASE(Test_BasicLanguage_ShrAssignRef)
{
	BasicProgramNode program;
	program.DefineFunction(L"main").ReturnType(t_int()).Statement(
		s_var(t_int(), L"a", e_prim(10))
		<<s_expr(e_result().Assign((e_name(L"a")>>=e_prim(3))+=e_prim(1)))
		);
	RunBasicProgram<int>(program.GetInternalValue(), 2);
}

TEST_CASE(Test_BasicLanguage_BitAndAssignRef)
{
	BasicProgramNode program;
	program.DefineFunction(L"main").ReturnType(t_int()).Statement(
		s_var(t_int(), L"a", e_prim(10))
		<<s_expr(e_result().Assign((e_name(L"a")&=e_prim(3))+=e_prim(1)))
		);
	RunBasicProgram<int>(program.GetInternalValue(), 3);
}

TEST_CASE(Test_BasicLanguage_BitOrAssignRef)
{
	BasicProgramNode program;
	program.DefineFunction(L"main").ReturnType(t_int()).Statement(
		s_var(t_int(), L"a", e_prim(10))
		<<s_expr(e_result().Assign((e_name(L"a")|=e_prim(3))+=e_prim(1)))
		);
	RunBasicProgram<int>(program.GetInternalValue(), 12);
}

TEST_CASE(Test_BasicLanguage_XorAssignRef)
{
	BasicProgramNode program;
	program.DefineFunction(L"main").ReturnType(t_int()).Statement(
		s_var(t_int(), L"a", e_prim(10))
		<<s_expr(e_result().Assign((e_name(L"a")^=e_prim(3))+=e_prim(1)))
		);
	RunBasicProgram<int>(program.GetInternalValue(), 10);
}

/***********************************************************************
Test Conversions
***********************************************************************/

TEST_CASE(Test_BasicLanguage_IntegralConversion)
{
	{
		BasicProgramNode program;
	}
}