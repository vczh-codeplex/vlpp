#include "..\..\Library\UnitTest\UnitTest.h"
#include "..\..\Library\Function.h"
#include "..\..\Library\Scripting\BasicLanguage\BasicLanguageTypeManager.h"
#include "..\..\Library\Scripting\BasicLanguage\BasicLanguageWriter.h"
#include "..\..\Library\Scripting\BasicLanguage\BasicLanguageAnalyzer.h"
#include "..\..\Library\Scripting\BasicLanguage\BasicLanguageCodeGeneration.h"
#include "..\..\Library\Scripting\BasicLanguage\BasicLanguageCommentProvider.h"
#include "..\..\Library\Scripting\BasicIL\BasicILInterpretor.h"
#include "..\..\Library\Stream\FileStream.h"
#include "..\..\Library\Stream\CharFormat.h"

using namespace vl;
using namespace vl::collections;
using namespace vl::scripting::basiclanguage;
using namespace vl::scripting::basicil;
using namespace vl::stream;

extern WString GetPath();

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

	BasicTypeRecord* typeInt8=BasicLanguage_GetTypeRecord(t_int8().GetInternalValue(), argument, true);
	BasicTypeRecord* typePInt8=BasicLanguage_GetTypeRecord((*t_int8()).GetInternalValue(), argument, true);
	BasicTypeRecord* typeFunc=BasicLanguage_GetTypeRecord(t_int8()(t_types()<<t_bool()).GetInternalValue(), argument, true);
	BasicTypeRecord* typeInt8s=BasicLanguage_GetTypeRecord(t_int8()[10].GetInternalValue(), argument, true);
	BasicTypeRecord* typeLink=BasicLanguage_GetTypeRecord(t_type(L"Link").GetInternalValue(), argument, true);
	BasicTypeRecord* typeType=BasicLanguage_GetTypeRecord(t_type(L"Type").GetInternalValue(), argument, true);

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
//from TestScripting_Languages_NativeX.cpp
extern void PrintNativeXProgram(Ptr<BasicProgram> program, TextWriter& writer);
extern void ConvertToNativeXProgram(Ptr<BasicProgram>& program);

template<typename T>
void RunBasicProgramInternal(Ptr<BasicProgram> program, T result, const WString& name)
{
	BasicAlgorithmConfiguration configuration;
	SetConfiguration(configuration);
	BasicAnalyzer analyzer(program, 0, configuration);
	analyzer.Analyze();
	TEST_ASSERT(analyzer.GetErrors().Count()==0);
	BasicCodeGenerator codegen(&analyzer, 0, L"assembly_generated");
	codegen.GenerateCode();
	BasicILInterpretor interpretor(65536);
	vint key=interpretor.LoadIL(codegen.GetIL().Obj());
	if(name!=L"")
	{
		WString fileName=GetPath()+L"Codegen_"+name+L".txt";
		FileStream fileStream(fileName, FileStream::WriteOnly);
		BomEncoder encoder(BomEncoder::Utf16);
		EncoderStream encoderStream(fileStream, encoder);
		StreamWriter writer(encoderStream);

		writer.WriteLine(L"/*NativeX Code*/");
		PrintNativeXProgram(program, writer);
		writer.WriteLine(L"");

		BasicLanguageCommentProvider commentProvider;
		TextWriter* commentProviderWriter=commentProvider.OpenWriter();
		PrintNativeXProgram(program, *commentProviderWriter);
		commentProvider.CloseWriter();

		writer.WriteLine(L"/*Assembly*/");
		codegen.GetIL()->SaveAsString(writer, &commentProvider);
	}
	BasicILStack stack(&interpretor);
	stack.Reset(0, key, 0);
	TEST_ASSERT(stack.Run()==BasicILStack::Finished);
	TEST_ASSERT(stack.GetEnv()->StackTop()==65536);
	vint ins=codegen.GetIL()->labels[0].instructionIndex;
	stack.Reset(ins, key, sizeof(T));
	TEST_ASSERT(stack.Run()==BasicILStack::Finished);
	TEST_ASSERT(stack.GetEnv()->StackTop()==65536-sizeof(T));
	TEST_ASSERT(stack.GetEnv()->Pop<T>()==result);
}

template<typename T>
void RunBasicProgram(Ptr<BasicProgram> program, T result, const WString& name)
{
	RunBasicProgramInternal(program, result, L"");
	ConvertToNativeXProgram(program);
	RunBasicProgramInternal(program, result, name);
}

/***********************************************************************
Test Operators
***********************************************************************/

TEST_CASE(TestScripting_BasicLanguage_1Plus1)
{
	BasicProgramNode program;
	program.DefineFunction(L"main").ReturnType(t_int()).Statement(
		s_expr(e_result().Assign(e_prim(1)+e_prim(1)))
		);
	RunBasicProgram<vint>(program.GetInternalValue(), 2, L"TestScripting_BasicLanguage_1Plus1");
}

TEST_CASE(TestScripting_BasicLanguage_2Minus3)
{
	BasicProgramNode program;
	program.DefineFunction(L"main").ReturnType(t_int()).Statement(
		s_expr(e_result().Assign(e_prim(2)-e_prim(3)))
		);
	RunBasicProgram<vint>(program.GetInternalValue(), -1, L"TestScripting_BasicLanguage_2Minus3");
}

TEST_CASE(TestScripting_BasicLanguage_7Multiply8)
{
	BasicProgramNode program;
	program.DefineFunction(L"main").ReturnType(t_int()).Statement(
		s_expr(e_result().Assign(e_prim(7)*e_prim(8)))
		);
	RunBasicProgram<vint>(program.GetInternalValue(), 56, L"TestScripting_BasicLanguage_7Multiply8");
}

TEST_CASE(TestScripting_BasicLanguage_6Div3)
{
	BasicProgramNode program;
	program.DefineFunction(L"main").ReturnType(t_int()).Statement(
		s_expr(e_result().Assign(e_prim(6)/e_prim(3)))
		);
	RunBasicProgram<vint>(program.GetInternalValue(), 2, L"TestScripting_BasicLanguage_6Div3");
}

TEST_CASE(TestScripting_BasicLanguage_10Mod3)
{
	BasicProgramNode program;
	program.DefineFunction(L"main").ReturnType(t_int()).Statement(
		s_expr(e_result().Assign(e_prim(10)%e_prim(3)))
		);
	RunBasicProgram<vint>(program.GetInternalValue(), 1, L"TestScripting_BasicLanguage_10Mod3");
}

TEST_CASE(TestScripting_BasicLanguage_10Shl3)
{
	BasicProgramNode program;
	program.DefineFunction(L"main").ReturnType(t_int()).Statement(
		s_expr(e_result().Assign(e_prim(10)<<e_prim(3)))
		);
	RunBasicProgram<vint>(program.GetInternalValue(), 80, L"TestScripting_BasicLanguage_10Shl3");
}

TEST_CASE(TestScripting_BasicLanguage_10Shr3)
{
	BasicProgramNode program;
	program.DefineFunction(L"main").ReturnType(t_int()).Statement(
		s_expr(e_result().Assign(e_prim(10)>>e_prim(3)))
		);
	RunBasicProgram<vint>(program.GetInternalValue(), 1, L"TestScripting_BasicLanguage_10Shr3");
}

TEST_CASE(TestScripting_BasicLanguage_6And10)
{
	BasicProgramNode program;
	program.DefineFunction(L"main").ReturnType(t_int()).Statement(
		s_expr(e_result().Assign(e_prim(6)&e_prim(10)))
		);
	RunBasicProgram<vint>(program.GetInternalValue(), 2, L"TestScripting_BasicLanguage_6And10");
}

TEST_CASE(TestScripting_BasicLanguage_6Or10)
{
	BasicProgramNode program;
	program.DefineFunction(L"main").ReturnType(t_int()).Statement(
		s_expr(e_result().Assign(e_prim(6)|e_prim(10)))
		);
	RunBasicProgram<vint>(program.GetInternalValue(), 14, L"TestScripting_BasicLanguage_6Or10");
}

TEST_CASE(TestScripting_BasicLanguage_6Xor10)
{
	BasicProgramNode program;
	program.DefineFunction(L"main").ReturnType(t_int()).Statement(
		s_expr(e_result().Assign(e_prim(6)^e_prim(10)))
		);
	RunBasicProgram<vint>(program.GetInternalValue(), 12, L"TestScripting_BasicLanguage_6Xor10");
}

TEST_CASE(TestScripting_BasicLanguage_Neg1Lt2)
{
	BasicProgramNode program;
	program.DefineFunction(L"main").ReturnType(t_bool()).Statement(
		s_expr(e_result().Assign(e_prim(-1)<e_prim(2)))
		);
	RunBasicProgram<bool>(program.GetInternalValue(), true, L"TestScripting_BasicLanguage_Neg1Lt2");
}

TEST_CASE(TestScripting_BasicLanguage_Neg1Le2)
{
	BasicProgramNode program;
	program.DefineFunction(L"main").ReturnType(t_bool()).Statement(
		s_expr(e_result().Assign(e_prim(-1)<=e_prim(2)))
		);
	RunBasicProgram<bool>(program.GetInternalValue(), true, L"TestScripting_BasicLanguage_Neg1Le2");
}

TEST_CASE(TestScripting_BasicLanguage_Neg1Gt2)
{
	BasicProgramNode program;
	program.DefineFunction(L"main").ReturnType(t_bool()).Statement(
		s_expr(e_result().Assign(e_prim(-1)>e_prim(2)))
		);
	RunBasicProgram<bool>(program.GetInternalValue(), false, L"TestScripting_BasicLanguage_Neg1Gt2");
}

TEST_CASE(TestScripting_BasicLanguage_Neg1Ge2)
{
	BasicProgramNode program;
	program.DefineFunction(L"main").ReturnType(t_bool()).Statement(
		s_expr(e_result().Assign(e_prim(-1)>=e_prim(2)))
		);
	RunBasicProgram<bool>(program.GetInternalValue(), false, L"TestScripting_BasicLanguage_Neg1Ge2");
}

TEST_CASE(TestScripting_BasicLanguage_Neg1Eq2)
{
	BasicProgramNode program;
	program.DefineFunction(L"main").ReturnType(t_bool()).Statement(
		s_expr(e_result().Assign(e_prim(-1)==e_prim(2)))
		);
	RunBasicProgram<bool>(program.GetInternalValue(), false, L"TestScripting_BasicLanguage_Neg1Eq2");
}

TEST_CASE(TestScripting_BasicLanguage_Neg1Ne2)
{
	BasicProgramNode program;
	program.DefineFunction(L"main").ReturnType(t_bool()).Statement(
		s_expr(e_result().Assign(e_prim(-1)!=e_prim(2)))
		);
	RunBasicProgram<bool>(program.GetInternalValue(), true, L"TestScripting_BasicLanguage_Neg1Ne2");
}

TEST_CASE(TestScripting_BasicLanguage_3PlusPlus)
{
	BasicProgramNode program;
	program.DefineFunction(L"main").ReturnType(t_int()).Statement(
		s_var(t_int(), L"a", e_prim(3))
		<<s_var(t_int(), L"b", e_name(L"a")++)
		<<s_expr(e_result().Assign(e_name(L"b")*e_prim(100)+e_name(L"a")))
		);
	RunBasicProgram<vint>(program.GetInternalValue(), 304, L"TestScripting_BasicLanguage_3PlusPlus");
}

TEST_CASE(TestScripting_BasicLanguage_3MinusMinus)
{
	BasicProgramNode program;
	program.DefineFunction(L"main").ReturnType(t_int()).Statement(
		s_var(t_int(), L"a", e_prim(3))
		<<s_var(t_int(), L"b", e_name(L"a")--)
		<<s_expr(e_result().Assign(e_name(L"b")*e_prim(100)+e_name(L"a")))
		);
	RunBasicProgram<vint>(program.GetInternalValue(), 302, L"TestScripting_BasicLanguage_3MinusMinus");
}

TEST_CASE(TestScripting_BasicLanguage_PlusPlus3)
{
	BasicProgramNode program;
	program.DefineFunction(L"main").ReturnType(t_int()).Statement(
		s_var(t_int(), L"a", e_prim(3))
		<<s_var(t_int(), L"b", ++e_name(L"a"))
		<<s_expr(e_result().Assign(e_name(L"b")*e_prim(100)+e_name(L"a")))
		);
	RunBasicProgram<vint>(program.GetInternalValue(), 404, L"TestScripting_BasicLanguage_PlusPlus3");
}

TEST_CASE(TestScripting_BasicLanguage_Neg)
{
	BasicProgramNode program;
	program.DefineFunction(L"main").ReturnType(t_int()).Statement(
		s_var(t_int(), L"a", e_prim(3))
		<<s_var(t_int(), L"b", -e_name(L"a"))
		<<s_expr(e_result().Assign(e_name(L"b")*e_prim(100)+e_name(L"a")))
		);
	RunBasicProgram<vint>(program.GetInternalValue(), -297, L"TestScripting_BasicLanguage_Neg");
}

TEST_CASE(TestScripting_BasicLanguage_Not)
{
	BasicProgramNode program;
	program.DefineFunction(L"main").ReturnType(t_int()).Statement(
		s_var(t_int(), L"a", e_prim(3))
		<<s_var(t_int(), L"b", !e_name(L"a"))
		<<s_expr(e_result().Assign(e_name(L"b")*e_prim(100)+e_name(L"a")))
		);
	RunBasicProgram<vint>(program.GetInternalValue(), 3, L"TestScripting_BasicLanguage_Not");
}

TEST_CASE(TestScripting_BasicLanguage_BitNot)
{
	BasicProgramNode program;
	program.DefineFunction(L"main").ReturnType(t_int()).Statement(
		s_var(t_int(), L"a", e_prim(3))
		<<s_var(t_int(), L"b", ~e_name(L"a"))
		<<s_expr(e_result().Assign(e_name(L"b")*e_prim(100)+e_name(L"a")))
		);
	RunBasicProgram<vint>(program.GetInternalValue(), (~3)*100+3, L"TestScripting_BasicLanguage_BitNot");
}

TEST_CASE(TestScripting_BasicLanguage_MinusMinus3)
{
	BasicProgramNode program;
	program.DefineFunction(L"main").ReturnType(t_int()).Statement(
		s_var(t_int(), L"a", e_prim(3))
		<<s_var(t_int(), L"b", --e_name(L"a"))
		<<s_expr(e_result().Assign(e_name(L"b")*e_prim(100)+e_name(L"a")))
		);
	RunBasicProgram<vint>(program.GetInternalValue(), 202, L"TestScripting_BasicLanguage_MinusMinus3");
}

TEST_CASE(TestScripting_BasicLanguage_AddAssign)
{
	BasicProgramNode program;
	program.DefineFunction(L"main").ReturnType(t_int()).Statement(
		s_var(t_int(), L"a", e_prim(10))
		<<s_expr(e_result().Assign(e_name(L"a")+=e_prim(3)))
		);
	RunBasicProgram<vint>(program.GetInternalValue(), 13, L"TestScripting_BasicLanguage_AddAssign");
}

TEST_CASE(TestScripting_BasicLanguage_SubAssign)
{
	BasicProgramNode program;
	program.DefineFunction(L"main").ReturnType(t_int()).Statement(
		s_var(t_int(), L"a", e_prim(10))
		<<s_expr(e_result().Assign(e_name(L"a")-=e_prim(3)))
		);
	RunBasicProgram<vint>(program.GetInternalValue(), 7, L"TestScripting_BasicLanguage_SubAssign");
}

TEST_CASE(TestScripting_BasicLanguage_MulAssign)
{
	BasicProgramNode program;
	program.DefineFunction(L"main").ReturnType(t_int()).Statement(
		s_var(t_int(), L"a", e_prim(10))
		<<s_expr(e_result().Assign(e_name(L"a")*=e_prim(3)))
		);
	RunBasicProgram<vint>(program.GetInternalValue(), 30, L"TestScripting_BasicLanguage_MulAssign");
}

TEST_CASE(TestScripting_BasicLanguage_DivAssign)
{
	BasicProgramNode program;
	program.DefineFunction(L"main").ReturnType(t_int()).Statement(
		s_var(t_int(), L"a", e_prim(10))
		<<s_expr(e_result().Assign(e_name(L"a")/=e_prim(3)))
		);
	RunBasicProgram<vint>(program.GetInternalValue(), 3, L"TestScripting_BasicLanguage_DivAssign");
}

TEST_CASE(TestScripting_BasicLanguage_ModAssign)
{
	BasicProgramNode program;
	program.DefineFunction(L"main").ReturnType(t_int()).Statement(
		s_var(t_int(), L"a", e_prim(10))
		<<s_expr(e_result().Assign(e_name(L"a")%=e_prim(3)))
		);
	RunBasicProgram<vint>(program.GetInternalValue(), 1, L"TestScripting_BasicLanguage_ModAssign");
}

TEST_CASE(TestScripting_BasicLanguage_ShlAssign)
{
	BasicProgramNode program;
	program.DefineFunction(L"main").ReturnType(t_int()).Statement(
		s_var(t_int(), L"a", e_prim(10))
		<<s_expr(e_result().Assign(e_name(L"a")<<=e_prim(3)))
		);
	RunBasicProgram<vint>(program.GetInternalValue(), 80, L"TestScripting_BasicLanguage_ShlAssign");
}

TEST_CASE(TestScripting_BasicLanguage_ShrAssign)
{
	BasicProgramNode program;
	program.DefineFunction(L"main").ReturnType(t_int()).Statement(
		s_var(t_int(), L"a", e_prim(10))
		<<s_expr(e_result().Assign(e_name(L"a")>>=e_prim(3)))
		);
	RunBasicProgram<vint>(program.GetInternalValue(), 1, L"TestScripting_BasicLanguage_ShrAssign");
}

TEST_CASE(TestScripting_BasicLanguage_BitAndAssign)
{
	BasicProgramNode program;
	program.DefineFunction(L"main").ReturnType(t_int()).Statement(
		s_var(t_int(), L"a", e_prim(10))
		<<s_expr(e_result().Assign(e_name(L"a")&=e_prim(3)))
		);
	RunBasicProgram<vint>(program.GetInternalValue(), 2, L"TestScripting_BasicLanguage_BitAndAssign");
}

TEST_CASE(TestScripting_BasicLanguage_BitOrAssign)
{
	BasicProgramNode program;
	program.DefineFunction(L"main").ReturnType(t_int()).Statement(
		s_var(t_int(), L"a", e_prim(10))
		<<s_expr(e_result().Assign(e_name(L"a")|=e_prim(3)))
		);
	RunBasicProgram<vint>(program.GetInternalValue(), 11, L"TestScripting_BasicLanguage_BitOrAssign");
}

TEST_CASE(TestScripting_BasicLanguage_XorAssign)
{
	BasicProgramNode program;
	program.DefineFunction(L"main").ReturnType(t_int()).Statement(
		s_var(t_int(), L"a", e_prim(10))
		<<s_expr(e_result().Assign(e_name(L"a")^=e_prim(3)))
		);
	RunBasicProgram<vint>(program.GetInternalValue(), 9, L"TestScripting_BasicLanguage_XorAssign");
}

TEST_CASE(TestScripting_BasicLanguage_AddAssignRef)
{
	BasicProgramNode program;
	program.DefineFunction(L"main").ReturnType(t_int()).Statement(
		s_var(t_int(), L"a", e_prim(10))
		<<s_expr(e_result().Assign((e_name(L"a")+=e_prim(3))+=e_prim(1)))
		);
	RunBasicProgram<vint>(program.GetInternalValue(), 14, L"TestScripting_BasicLanguage_AddAssignRef");
}

TEST_CASE(TestScripting_BasicLanguage_SubAssignRef)
{
	BasicProgramNode program;
	program.DefineFunction(L"main").ReturnType(t_int()).Statement(
		s_var(t_int(), L"a", e_prim(10))
		<<s_expr(e_result().Assign((e_name(L"a")-=e_prim(3))+=e_prim(1)))
		);
	RunBasicProgram<vint>(program.GetInternalValue(), 8, L"TestScripting_BasicLanguage_SubAssignRef");
}

TEST_CASE(TestScripting_BasicLanguage_MulAssignRef)
{
	BasicProgramNode program;
	program.DefineFunction(L"main").ReturnType(t_int()).Statement(
		s_var(t_int(), L"a", e_prim(10))
		<<s_expr(e_result().Assign((e_name(L"a")*=e_prim(3))+=e_prim(1)))
		);
	RunBasicProgram<vint>(program.GetInternalValue(), 31, L"TestScripting_BasicLanguage_MulAssignRef");
}

TEST_CASE(TestScripting_BasicLanguage_DivAssignRef)
{
	BasicProgramNode program;
	program.DefineFunction(L"main").ReturnType(t_int()).Statement(
		s_var(t_int(), L"a", e_prim(10))
		<<s_expr(e_result().Assign((e_name(L"a")/=e_prim(3))+=e_prim(1)))
		);
	RunBasicProgram<vint>(program.GetInternalValue(), 4, L"TestScripting_BasicLanguage_DivAssignRef");
}

TEST_CASE(TestScripting_BasicLanguage_ModAssignRef)
{
	BasicProgramNode program;
	program.DefineFunction(L"main").ReturnType(t_int()).Statement(
		s_var(t_int(), L"a", e_prim(10))
		<<s_expr(e_result().Assign((e_name(L"a")%=e_prim(3))+=e_prim(1)))
		);
	RunBasicProgram<vint>(program.GetInternalValue(), 2, L"TestScripting_BasicLanguage_ModAssignRef");
}

TEST_CASE(TestScripting_BasicLanguage_ShlAssignRef)
{
	BasicProgramNode program;
	program.DefineFunction(L"main").ReturnType(t_int()).Statement(
		s_var(t_int(), L"a", e_prim(10))
		<<s_expr(e_result().Assign((e_name(L"a")<<=e_prim(3))+=e_prim(1)))
		);
	RunBasicProgram<vint>(program.GetInternalValue(), 81, L"TestScripting_BasicLanguage_ShlAssignRef");
}

TEST_CASE(TestScripting_BasicLanguage_ShrAssignRef)
{
	BasicProgramNode program;
	program.DefineFunction(L"main").ReturnType(t_int()).Statement(
		s_var(t_int(), L"a", e_prim(10))
		<<s_expr(e_result().Assign((e_name(L"a")>>=e_prim(3))+=e_prim(1)))
		);
	RunBasicProgram<vint>(program.GetInternalValue(), 2, L"TestScripting_BasicLanguage_ShrAssignRef");
}

TEST_CASE(TestScripting_BasicLanguage_BitAndAssignRef)
{
	BasicProgramNode program;
	program.DefineFunction(L"main").ReturnType(t_int()).Statement(
		s_var(t_int(), L"a", e_prim(10))
		<<s_expr(e_result().Assign((e_name(L"a")&=e_prim(3))+=e_prim(1)))
		);
	RunBasicProgram<vint>(program.GetInternalValue(), 3, L"TestScripting_BasicLanguage_BitAndAssignRef");
}

TEST_CASE(TestScripting_BasicLanguage_BitOrAssignRef)
{
	BasicProgramNode program;
	program.DefineFunction(L"main").ReturnType(t_int()).Statement(
		s_var(t_int(), L"a", e_prim(10))
		<<s_expr(e_result().Assign((e_name(L"a")|=e_prim(3))+=e_prim(1)))
		);
	RunBasicProgram<vint>(program.GetInternalValue(), 12, L"TestScripting_BasicLanguage_BitOrAssignRef");
}

TEST_CASE(TestScripting_BasicLanguage_XorAssignRef)
{
	BasicProgramNode program;
	program.DefineFunction(L"main").ReturnType(t_int()).Statement(
		s_var(t_int(), L"a", e_prim(10))
		<<s_expr(e_result().Assign((e_name(L"a")^=e_prim(3))+=e_prim(1)))
		);
	RunBasicProgram<vint>(program.GetInternalValue(), 10, L"TestScripting_BasicLanguage_XorAssignRef");
}

TEST_CASE(TestScripting_BasicLanguage_AssignRef)
{
	BasicProgramNode program;
	program.DefineFunction(L"main").ReturnType(t_int()).Statement(
		s_var(t_int(), L"a", e_prim(10))
		<<s_expr(e_result().Assign((e_name(L"a").Assign(e_prim(3)))+=e_prim(1)))
		);
	RunBasicProgram<vint>(program.GetInternalValue(), 4, L"TestScripting_BasicLanguage_AssignRef");
}

/***********************************************************************
Test Pointers
***********************************************************************/

TEST_CASE(TestScripting_BasicLanguage_Sum1)
{
	BasicProgramNode program;
	program.DefineFunction(L"main").ReturnType(t_int()).Statement(
		s_var(t_int()[5], L"a")
		<<s_expr(e_name(L"a")[e_prim(0)].Assign(e_prim(1)))
		<<s_expr(e_name(L"a")[e_prim(1)].Assign(e_prim(2)))
		<<s_expr(e_name(L"a")[e_prim(2)].Assign(e_prim(3)))
		<<s_expr(e_name(L"a")[e_prim(3)].Assign(e_prim(4)))
		<<s_expr(e_name(L"a")[e_prim(4)].Assign(e_prim(5)))
		<<s_var(*t_int(), L"b", e_name(L"a").Ref()[*t_int()])
		<<s_expr(e_result().Assign(e_prim(0)))
		<<s_expr(e_result()+=e_name(L"b")[e_prim(0)])
		<<s_expr(e_result()+=e_name(L"b")[e_prim(1)])
		<<s_expr(e_result()+=e_name(L"b")[e_prim(2)])
		<<s_expr(e_result()+=e_name(L"b")[e_prim(3)])
		<<s_expr(e_result()+=e_name(L"b")[e_prim(4)])
		);
	RunBasicProgram<vint>(program.GetInternalValue(), 15, L"TestScripting_BasicLanguage_Sum1");
}

TEST_CASE(TestScripting_BasicLanguage_Sum2)
{
	BasicProgramNode program;
	program.DefineFunction(L"main").ReturnType(t_int()).Statement(
		s_var(t_int()[5], L"a")
		<<s_expr(e_name(L"a")[e_prim(0)].Assign(e_prim(1)))
		<<s_expr(e_name(L"a")[e_prim(1)].Assign(e_prim(2)))
		<<s_expr(e_name(L"a")[e_prim(2)].Assign(e_prim(3)))
		<<s_expr(e_name(L"a")[e_prim(3)].Assign(e_prim(4)))
		<<s_expr(e_name(L"a")[e_prim(4)].Assign(e_prim(5)))
		<<s_var(*t_int(), L"b", e_name(L"a")[e_prim(0)].Ref())
		<<s_expr(e_result().Assign(e_prim(0)))
		<<s_expr(e_result()+=*e_name(L"b")++)
		<<s_expr(e_result()+=*e_name(L"b")++)
		<<s_expr(e_result()+=*e_name(L"b")++)
		<<s_expr(e_result()+=*e_name(L"b")++)
		<<s_expr(e_result()+=*e_name(L"b")++)
		);
	RunBasicProgram<vint>(program.GetInternalValue(), 15, L"TestScripting_BasicLanguage_Sum2");
}

TEST_CASE(TestScripting_BasicLanguage_Sum3)
{
	BasicProgramNode program;
	program.DefineFunction(L"main").ReturnType(t_int()).Statement(
		s_var(t_int()[5], L"a")
		<<s_expr(e_name(L"a")[e_prim(0)].Assign(e_prim(1)))
		<<s_expr(e_name(L"a")[e_prim(1)].Assign(e_prim(2)))
		<<s_expr(e_name(L"a")[e_prim(2)].Assign(e_prim(3)))
		<<s_expr(e_name(L"a")[e_prim(3)].Assign(e_prim(4)))
		<<s_expr(e_name(L"a")[e_prim(4)].Assign(e_prim(5)))
		<<s_var(*t_int(), L"b", e_name(L"a")[e_prim(4)].Ref())
		<<s_expr(e_result().Assign(e_prim(0)))
		<<s_expr(e_result()+=*e_name(L"b")--)
		<<s_expr(e_result()+=*e_name(L"b")--)
		<<s_expr(e_result()+=*e_name(L"b")--)
		<<s_expr(e_result()+=*e_name(L"b")--)
		<<s_expr(e_result()+=*e_name(L"b")--)
		);
	RunBasicProgram<vint>(program.GetInternalValue(), 15, L"TestScripting_BasicLanguage_Sum3");
}

TEST_CASE(TestScripting_BasicLanguage_Sum4)
{
	BasicProgramNode program;
	program.DefineFunction(L"main").ReturnType(t_int()).Statement(
		s_var(t_int()[5], L"a")
		<<s_expr(e_name(L"a")[e_prim(0)].Assign(e_prim(1)))
		<<s_expr(e_name(L"a")[e_prim(1)].Assign(e_prim(2)))
		<<s_expr(e_name(L"a")[e_prim(2)].Assign(e_prim(3)))
		<<s_expr(e_name(L"a")[e_prim(3)].Assign(e_prim(4)))
		<<s_expr(e_name(L"a")[e_prim(4)].Assign(e_prim(5)))
		<<s_var(*t_int(), L"b", e_name(L"a")[e_prim(-1)].Ref())
		<<s_expr(e_result().Assign(e_prim(0)))
		<<s_expr(e_result()+=*++e_name(L"b"))
		<<s_expr(e_result()+=*++e_name(L"b"))
		<<s_expr(e_result()+=*++e_name(L"b"))
		<<s_expr(e_result()+=*++e_name(L"b"))
		<<s_expr(e_result()+=*++e_name(L"b"))
		);
	RunBasicProgram<vint>(program.GetInternalValue(), 15, L"TestScripting_BasicLanguage_Sum4");
}

TEST_CASE(TestScripting_BasicLanguage_Sum5)
{
	BasicProgramNode program;
	program.DefineFunction(L"main").ReturnType(t_int()).Statement(
		s_var(t_int()[5], L"a")
		<<s_expr(e_name(L"a")[e_prim(0)].Assign(e_prim(1)))
		<<s_expr(e_name(L"a")[e_prim(1)].Assign(e_prim(2)))
		<<s_expr(e_name(L"a")[e_prim(2)].Assign(e_prim(3)))
		<<s_expr(e_name(L"a")[e_prim(3)].Assign(e_prim(4)))
		<<s_expr(e_name(L"a")[e_prim(4)].Assign(e_prim(5)))
		<<s_var(*t_int(), L"b", e_name(L"a")[e_prim(5)].Ref())
		<<s_expr(e_result().Assign(e_prim(0)))
		<<s_expr(e_result()+=*--e_name(L"b"))
		<<s_expr(e_result()+=*--e_name(L"b"))
		<<s_expr(e_result()+=*--e_name(L"b"))
		<<s_expr(e_result()+=*--e_name(L"b"))
		<<s_expr(e_result()+=*--e_name(L"b"))
		);
	RunBasicProgram<vint>(program.GetInternalValue(), 15, L"TestScripting_BasicLanguage_Sum5");
}

TEST_CASE(TestScripting_BasicLanguage_PointerArithmetic)
{
	{
		BasicProgramNode program;
		program.DefineFunction(L"main").ReturnType(t_int()).Statement(
			s_var(t_int()[5], L"a")
			<<s_expr(e_name(L"a")[e_prim(0)].Assign(e_prim(1)))
			<<s_expr(e_name(L"a")[e_prim(1)].Assign(e_prim(2)))
			<<s_expr(e_name(L"a")[e_prim(2)].Assign(e_prim(3)))
			<<s_expr(e_name(L"a")[e_prim(3)].Assign(e_prim(4)))
			<<s_expr(e_name(L"a")[e_prim(4)].Assign(e_prim(5)))
			<<s_var(*t_int(), L"b", e_name(L"a")[e_prim(0)].Ref())
			<<s_expr(e_result().Assign(*(e_name(L"b")+e_prim(3))))
			);
		RunBasicProgram<vint>(program.GetInternalValue(), 4, L"TestScripting_BasicLanguage_PointerArithmetic[1]");
	}
	{
		BasicProgramNode program;
		program.DefineFunction(L"main").ReturnType(t_int()).Statement(
			s_var(t_int()[5], L"a")
			<<s_expr(e_name(L"a")[e_prim(0)].Assign(e_prim(1)))
			<<s_expr(e_name(L"a")[e_prim(1)].Assign(e_prim(2)))
			<<s_expr(e_name(L"a")[e_prim(2)].Assign(e_prim(3)))
			<<s_expr(e_name(L"a")[e_prim(3)].Assign(e_prim(4)))
			<<s_expr(e_name(L"a")[e_prim(4)].Assign(e_prim(5)))
			<<s_var(*t_int(), L"b", e_name(L"a")[e_prim(0)].Ref())
			<<s_expr(e_result().Assign(*(e_name(L"b")+=e_prim(3))))
			);
		RunBasicProgram<vint>(program.GetInternalValue(), 4, L"TestScripting_BasicLanguage_PointerArithmetic[2]");
	}
	{
		BasicProgramNode program;
		program.DefineFunction(L"main").ReturnType(t_int()).Statement(
			s_var(t_int()[5], L"a")
			<<s_expr(e_name(L"a")[e_prim(0)].Assign(e_prim(1)))
			<<s_expr(e_name(L"a")[e_prim(1)].Assign(e_prim(2)))
			<<s_expr(e_name(L"a")[e_prim(2)].Assign(e_prim(3)))
			<<s_expr(e_name(L"a")[e_prim(3)].Assign(e_prim(4)))
			<<s_expr(e_name(L"a")[e_prim(4)].Assign(e_prim(5)))
			<<s_var(*t_int(), L"b", e_name(L"a")[e_prim(0)].Ref())
			<<s_expr(e_result().Assign(*((e_name(L"b")+=e_prim(2))+=e_prim(1))))
			);
		RunBasicProgram<vint>(program.GetInternalValue(), 4, L"TestScripting_BasicLanguage_PointerArithmetic[3]");
	}
	{
		BasicProgramNode program;
		program.DefineFunction(L"main").ReturnType(t_int()).Statement(
			s_var(t_int()[5], L"a")
			<<s_expr(e_name(L"a")[e_prim(0)].Assign(e_prim(1)))
			<<s_expr(e_name(L"a")[e_prim(1)].Assign(e_prim(2)))
			<<s_expr(e_name(L"a")[e_prim(2)].Assign(e_prim(3)))
			<<s_expr(e_name(L"a")[e_prim(3)].Assign(e_prim(4)))
			<<s_expr(e_name(L"a")[e_prim(4)].Assign(e_prim(5)))
			<<s_var(*t_int(), L"b", e_name(L"a")[e_prim(0)].Ref())
			<<s_expr(e_result().Assign(*((++e_name(L"b"))+=e_prim(2))))
			);
		RunBasicProgram<vint>(program.GetInternalValue(), 4, L"TestScripting_BasicLanguage_PointerArithmetic[4]");
	}
	{
		BasicProgramNode program;
		program.DefineFunction(L"main").ReturnType(t_int()).Statement(
			s_var(t_int()[5], L"a")
			<<s_expr(e_name(L"a")[e_prim(0)].Assign(e_prim(1)))
			<<s_expr(e_name(L"a")[e_prim(1)].Assign(e_prim(2)))
			<<s_expr(e_name(L"a")[e_prim(2)].Assign(e_prim(3)))
			<<s_expr(e_name(L"a")[e_prim(3)].Assign(e_prim(4)))
			<<s_expr(e_name(L"a")[e_prim(4)].Assign(e_prim(5)))
			<<s_var(*t_int(), L"b", e_name(L"a")[e_prim(0)].Ref())
			<<s_expr(e_result().Assign(*(e_prim(2)+e_name(L"b"))))
			);
		RunBasicProgram<vint>(program.GetInternalValue(), 3, L"TestScripting_BasicLanguage_PointerArithmetic[5]");
	}
	{
		BasicProgramNode program;
		program.DefineFunction(L"main").ReturnType(t_int()).Statement(
			s_var(t_int()[5], L"a")
			<<s_expr(e_name(L"a")[e_prim(0)].Assign(e_prim(1)))
			<<s_expr(e_name(L"a")[e_prim(1)].Assign(e_prim(2)))
			<<s_expr(e_name(L"a")[e_prim(2)].Assign(e_prim(3)))
			<<s_expr(e_name(L"a")[e_prim(3)].Assign(e_prim(4)))
			<<s_expr(e_name(L"a")[e_prim(4)].Assign(e_prim(5)))
			<<s_var(*t_int(), L"b", e_name(L"a")[e_prim(4)].Ref())
			<<s_expr(e_result().Assign(*(e_name(L"b")-e_prim(3))))
			);
		RunBasicProgram<vint>(program.GetInternalValue(), 2, L"TestScripting_BasicLanguage_PointerArithmetic[6]");
	}
	{
		BasicProgramNode program;
		program.DefineFunction(L"main").ReturnType(t_int()).Statement(
			s_var(t_int()[5], L"a")
			<<s_expr(e_name(L"a")[e_prim(0)].Assign(e_prim(1)))
			<<s_expr(e_name(L"a")[e_prim(1)].Assign(e_prim(2)))
			<<s_expr(e_name(L"a")[e_prim(2)].Assign(e_prim(3)))
			<<s_expr(e_name(L"a")[e_prim(3)].Assign(e_prim(4)))
			<<s_expr(e_name(L"a")[e_prim(4)].Assign(e_prim(5)))
			<<s_var(*t_int(), L"b", e_name(L"a")[e_prim(4)].Ref())
			<<s_expr(e_result().Assign(*(e_name(L"b")-=e_prim(3))))
			);
		RunBasicProgram<vint>(program.GetInternalValue(), 2, L"TestScripting_BasicLanguage_PointerArithmetic[7]");
	}
	{
		BasicProgramNode program;
		program.DefineFunction(L"main").ReturnType(t_int()).Statement(
			s_var(t_int()[5], L"a")
			<<s_expr(e_name(L"a")[e_prim(0)].Assign(e_prim(1)))
			<<s_expr(e_name(L"a")[e_prim(1)].Assign(e_prim(2)))
			<<s_expr(e_name(L"a")[e_prim(2)].Assign(e_prim(3)))
			<<s_expr(e_name(L"a")[e_prim(3)].Assign(e_prim(4)))
			<<s_expr(e_name(L"a")[e_prim(4)].Assign(e_prim(5)))
			<<s_var(*t_int(), L"b", e_name(L"a")[e_prim(4)].Ref())
			<<s_expr(e_result().Assign(*((e_name(L"b")-=e_prim(2))-=e_prim(1))))
			);
		RunBasicProgram<vint>(program.GetInternalValue(), 2, L"TestScripting_BasicLanguage_PointerArithmetic[8]");
	}
	{
		BasicProgramNode program;
		program.DefineFunction(L"main").ReturnType(t_int()).Statement(
			s_var(t_int()[5], L"a")
			<<s_expr(e_name(L"a")[e_prim(0)].Assign(e_prim(1)))
			<<s_expr(e_name(L"a")[e_prim(1)].Assign(e_prim(2)))
			<<s_expr(e_name(L"a")[e_prim(2)].Assign(e_prim(3)))
			<<s_expr(e_name(L"a")[e_prim(3)].Assign(e_prim(4)))
			<<s_expr(e_name(L"a")[e_prim(4)].Assign(e_prim(5)))
			<<s_var(*t_int(), L"b", e_name(L"a")[e_prim(4)].Ref())
			<<s_expr(e_result().Assign(*((--e_name(L"b"))-=e_prim(2))))
			);
		RunBasicProgram<vint>(program.GetInternalValue(), 2, L"TestScripting_BasicLanguage_PointerArithmetic[9]");
	}
	{
		BasicProgramNode program;
		program.DefineFunction(L"main").ReturnType(t_int()).Statement(
			s_var(t_int()[5], L"a")
			<<s_expr(e_name(L"a")[e_prim(0)].Assign(e_prim(1)))
			<<s_expr(e_name(L"a")[e_prim(1)].Assign(e_prim(2)))
			<<s_expr(e_name(L"a")[e_prim(2)].Assign(e_prim(3)))
			<<s_expr(e_name(L"a")[e_prim(3)].Assign(e_prim(4)))
			<<s_expr(e_name(L"a")[e_prim(4)].Assign(e_prim(5)))
			<<s_var(*t_int(), L"b", e_name(L"a")[e_prim(4)].Ref())
			<<s_var(*t_int(), L"c", e_name(L"a")[e_prim(2)].Ref())
			<<s_expr(e_result().Assign(e_name(L"b")-e_name(L"c")))
			);
		RunBasicProgram<vint>(program.GetInternalValue(), 2, L"TestScripting_BasicLanguage_PointerArithmetic[10]");
	}
	{
		BasicProgramNode program;
		program.DefineFunction(L"main").ReturnType(t_int()).Statement(
			s_var(t_int()[5], L"a")
			<<s_expr(e_name(L"a")[e_prim(0)].Assign(e_prim(1)))
			<<s_expr(e_name(L"a")[e_prim(1)].Assign(e_prim(2)))
			<<s_expr(e_name(L"a")[e_prim(2)].Assign(e_prim(3)))
			<<s_expr(e_name(L"a")[e_prim(3)].Assign(e_prim(4)))
			<<s_expr(e_name(L"a")[e_prim(4)].Assign(e_prim(5)))
			<<s_var(*t_int(), L"b", e_name(L"a")[e_prim(0)].Ref())
			<<s_var(*t_int(), L"c", e_name(L"a")[e_prim(4)].Ref())
			<<s_expr(e_name(L"b")+=e_prim(4))
			<<s_expr(e_name(L"c")-=e_prim(2))
			<<s_expr(e_result().Assign(e_name(L"b")-e_name(L"c")))
			);
		RunBasicProgram<vint>(program.GetInternalValue(), 2, L"TestScripting_BasicLanguage_PointerArithmetic[11]");
	}
}

TEST_CASE(TestScripting_BasicLanguage_ComplexSideEffect)
{
	{
		BasicProgramNode program;
		program.DefineFunction(L"main").ReturnType(t_int()).Statement(
			s_var(t_int()[5], L"a")
			<<s_expr(e_name(L"a")[e_prim(0)].Assign(e_prim(1)))
			<<s_expr(e_name(L"a")[e_prim(1)].Assign(e_prim(2)))
			<<s_expr(e_name(L"a")[e_prim(2)].Assign(e_prim(3)))
			<<s_expr(e_name(L"a")[e_prim(3)].Assign(e_prim(4)))
			<<s_expr(e_name(L"a")[e_prim(4)].Assign(e_prim(5)))
			<<s_var(t_int(), L"i", e_prim(0))
			<<s_var(t_int(), L"j", e_prim(0))
			<<s_expr(e_name(L"a")[e_name(L"i")++]++)
			<<s_expr(e_name(L"a")[e_name(L"i")++]--)
			<<s_expr(e_name(L"a")[e_name(L"i")++]++)
			<<s_expr(e_name(L"a")[e_name(L"i")++]--)
			<<s_expr(e_name(L"a")[e_name(L"i")++]++)
			<<s_expr(e_result().Assign(e_prim(0)))
			<<s_expr((e_result()*=e_prim(10))+=e_name(L"a")[e_name(L"j")++])
			<<s_expr((e_result()*=e_prim(10))+=e_name(L"a")[e_name(L"j")++])
			<<s_expr((e_result()*=e_prim(10))+=e_name(L"a")[e_name(L"j")++])
			<<s_expr((e_result()*=e_prim(10))+=e_name(L"a")[e_name(L"j")++])
			<<s_expr((e_result()*=e_prim(10))+=e_name(L"a")[e_name(L"j")++])
			);
		RunBasicProgram<vint>(program.GetInternalValue(), 21436, L"TestScripting_BasicLanguage_ComplexSideEffect[1]");
	}
	{
		BasicProgramNode program;
		program.DefineStructure(L"PPair")
			.Member(L"a", *t_int())
			.Member(L"b", *t_int());
		program.DefineFunction(L"main").ReturnType(t_int()).Statement(
			s_var(t_type(L"PPair"), L"c")
			<<s_var(t_int()[10], L"a")
			<<s_expr(e_name(L"c").Member(L"a").Assign(e_name(L"a")[e_prim(5)].Ref()))
			<<s_expr(e_name(L"c").Member(L"b").Assign(e_name(L"a")[e_prim(6)].Ref()))
			<<s_var(*t_type(L"PPair"), L"pc", e_name(L"c").Ref())
			<<s_expr(e_name(L"pc").PMember(L"a")+=e_prim(3))
			<<s_expr(e_name(L"pc").PMember(L"b")-=e_prim(4))
			<<s_expr(e_result().Assign(e_name(L"c").Member(L"a")-e_name(L"c").Member(L"b")))
			);
		RunBasicProgram<vint>(program.GetInternalValue(), 6, L"TestScripting_BasicLanguage_ComplexSideEffect[2]");
	}
	{
		BasicProgramNode program;
		program.DefineStructure(L"PPair")
			.Member(L"a", *t_int())
			.Member(L"b", *t_int());
		program.DefineFunction(L"main").ReturnType(t_int()).Statement(
			s_var(t_type(L"PPair")[10], L"a")
			<<s_var(t_int(), L"i", e_prim(3))
			<<s_var(t_int(), L"j", e_prim(4))
			<<s_var(t_int(), L"k", e_prim(5))
			<<s_var(t_int(), L"l", e_prim(6))

			<<s_expr(e_name(L"i")++ + e_name(L"j")++)
			<<s_expr(++e_name(L"i") + ++e_name(L"j"))
			<<s_expr(e_name(L"a")[e_name(L"k")--].Member(L"a"))
			<<s_expr(e_name(L"a")[--e_name(L"l")].Member(L"b"))
			<<s_expr((e_name(L"i")++)[t_uint()])
			<<s_expr(-(++e_name(L"i")))

			<<s_expr(e_result().Assign(
				e_name(L"i")*e_prim(1000)
				+e_name(L"j")*e_prim(100)
				+e_name(L"k")*e_prim(10)
				+e_name(L"l")
				))
			);
		RunBasicProgram<vint>(program.GetInternalValue(), 7645, L"TestScripting_BasicLanguage_ComplexSideEffect[3]");
	}
	{
		BasicProgramNode program;
		program.DefineStructure(L"PPair")
			.Member(L"a", *t_int())
			.Member(L"b", *t_int());
		program.DefineFunction(L"main").ReturnType(t_int()).Statement(
			s_var(t_type(L"PPair")[3], L"c")
			<<s_var(t_int()[10], L"a")
			<<s_expr(e_name(L"c")[e_prim(0)].Member(L"a").Assign(e_name(L"a")[e_prim(1)].Ref()))
			<<s_expr(e_name(L"c")[e_prim(0)].Member(L"b").Assign(e_name(L"a")[e_prim(2)].Ref()))
			<<s_expr(e_name(L"c")[e_prim(1)].Member(L"a").Assign(e_name(L"a")[e_prim(3)].Ref()))
			<<s_expr(e_name(L"c")[e_prim(1)].Member(L"b").Assign(e_name(L"a")[e_prim(4)].Ref()))
			<<s_expr(e_name(L"c")[e_prim(2)].Member(L"a").Assign(e_name(L"a")[e_prim(5)].Ref()))
			<<s_expr(e_name(L"c")[e_prim(2)].Member(L"b").Assign(e_name(L"a")[e_prim(6)].Ref()))
			<<s_var(*t_type(L"PPair"), L"pc1", e_name(L"c")[e_prim(0)].Ref())
			<<s_var(*t_type(L"PPair"), L"pc2", e_name(L"c")[e_prim(1)].Ref())
			<<s_var(*t_type(L"PPair"), L"pc3", e_name(L"c")[e_prim(2)].Ref())
			
			<<s_var(t_int(), L"n1", e_name(L"copy")(e_exps()
				<<(e_name(L"pc1").PMember(L"a")++)
				<<e_name(L"a")[e_prim(0)].Ref()
				))
			<<s_var(t_int(), L"n2", e_name(L"copy")(e_exps()
				<<(e_name(L"pc1").PMember(L"b")--)
				<<e_name(L"a")[e_prim(0)].Ref()
				))
			<<s_var(t_int(), L"n3", e_name(L"copy")(e_exps()
				<<(e_name(L"pc1").PMember(L"a")++)
				<<e_name(L"a")[e_prim(0)].Ref()
				))
			<<s_var(t_int(), L"n4", e_name(L"copy")(e_exps()
				<<(e_name(L"pc1").PMember(L"b")--)
				<<e_name(L"a")[e_prim(0)].Ref()
				))
			<<s_var(t_int(), L"n5", e_name(L"copy")(e_exps()
				<<(--e_name(L"pc2").PMember(L"a"))
				<<e_name(L"a")[e_prim(0)].Ref()
				))
			<<s_var(t_int(), L"n6", e_name(L"copy")(e_exps()
				<<(++e_name(L"pc2").PMember(L"b"))
				<<e_name(L"a")[e_prim(0)].Ref()
				))
			<<s_var(t_int(), L"n7", e_name(L"copy")(e_exps()
				<<(e_name(L"pc3").PMember(L"a")+=e_prim(3))
				<<e_name(L"a")[e_prim(0)].Ref()
				))
			<<s_var(t_int(), L"n8", e_name(L"copy")(e_exps()
				<<(e_name(L"pc3").PMember(L"b")-=e_prim(4))
				<<e_name(L"a")[e_prim(0)].Ref()
				))

			<<s_expr(e_result().Assign(
				e_name(L"n1")*e_prim(10000000)
				+e_name(L"n2")*e_prim(1000000)
				+e_name(L"n3")*e_prim(100000)
				+e_name(L"n4")*e_prim(10000)
				+e_name(L"n5")*e_prim(1000)
				+e_name(L"n6")*e_prim(100)
				+e_name(L"n7")*e_prim(10)
				+e_name(L"n8")
				))
			);
		program.DefineFunction(L"copy").ReturnType(t_int())
			.Parameter(L"i", *t_int())
			.Parameter(L"j", *t_int())
			.Statement(
				s_expr(e_result().Assign(e_name(L"i")-e_name(L"j")))
			);
		RunBasicProgram<vint>(program.GetInternalValue(), 12212582, L"TestScripting_BasicLanguage_ComplexSideEffect[4]");
	}
	{
		BasicProgramNode program;
		program.DefineStructure(L"PPair")
			.Member(L"a", *t_int())
			.Member(L"b", *t_int())
			.Member(L"c", *t_int());
		program.DefineFunction(L"main").ReturnType(t_int()).Statement(
			s_var(t_type(L"PPair"), L"c")
			<<s_var(t_int()[10], L"a")
			<<s_expr(e_name(L"c").Member(L"a").Assign(e_name(L"a")[e_prim(1)].Ref()))
			<<s_expr(e_name(L"c").Member(L"b").Assign(e_name(L"a")[e_prim(2)].Ref()))
			<<s_expr(e_name(L"c").Member(L"c").Assign(e_name(L"a")[e_prim(3)].Ref()))
			<<s_var(*t_type(L"PPair"), L"pc", e_name(L"c").Ref())
			
			<<s_var(t_int(), L"n1", e_name(L"copy")(e_exps()
				<<(e_name(L"pc").PMember(L"a").Assign(e_name(L"a")[e_prim(4)].Ref())).Ref()
				<<e_name(L"a")[e_prim(0)].Ref()
				))
			<<s_var(t_int(), L"n2", e_name(L"copy")(e_exps()
				<<(e_name(L"pc").PMember(L"b")+=e_prim(2)).Ref()
				<<e_name(L"a")[e_prim(0)].Ref()
				))
			<<s_var(t_int(), L"n3", e_name(L"copy")(e_exps()
				<<(e_name(L"pc").PMember(L"c")-=e_prim(1)).Ref()
				<<e_name(L"a")[e_prim(0)].Ref()
				))

			<<s_expr(e_result().Assign(
				e_name(L"n1")*e_prim(100)
				+e_name(L"n2")*e_prim(10)
				+e_name(L"n3")
				))
			);
		program.DefineFunction(L"copy").ReturnType(t_int())
			.Parameter(L"i", **t_int())
			.Parameter(L"j", *t_int())
			.Statement(
				s_expr(e_result().Assign(*e_name(L"i")-e_name(L"j")))
			);
		RunBasicProgram<vint>(program.GetInternalValue(), 442, L"TestScripting_BasicLanguage_ComplexSideEffect[5]");
	}
	{
		BasicProgramNode program;
		program.DefineStructure(L"PPair")
			.Member(L"a", t_int())
			.Member(L"b", t_int())
			.Member(L"c", t_int());
		program.DefineFunction(L"main").ReturnType(t_int()).Statement(
			s_var(t_type(L"PPair"), L"c")
			<<s_expr(e_name(L"c").Member(L"a").Assign(e_prim(1)))
			<<s_expr(e_name(L"c").Member(L"b").Assign(e_prim(2)))
			<<s_expr(e_name(L"c").Member(L"c").Assign(e_prim(3)))
			<<s_var(*t_type(L"PPair"), L"pc", e_name(L"c").Ref())
			
			<<s_var(t_int(), L"n1", e_name(L"copy")(e_exps()
				<<(e_name(L"pc").PMember(L"a").Assign(e_prim(4)))
				))
			<<s_var(t_int(), L"n2", e_name(L"copy")(e_exps()
				<<(e_name(L"pc").PMember(L"b")+=e_prim(2))
				))
			<<s_var(t_int(), L"n3", e_name(L"copy")(e_exps()
				<<(e_name(L"pc").PMember(L"c")-=e_prim(1))
				))

			<<s_expr(e_result().Assign(
				e_name(L"n1")*e_prim(100)
				+e_name(L"n2")*e_prim(10)
				+e_name(L"n3")
				))
			);
		program.DefineFunction(L"copy").ReturnType(t_int())
			.Parameter(L"i", t_int())
			.Statement(
				s_expr(e_result().Assign(e_name(L"i")))
			);
		RunBasicProgram<vint>(program.GetInternalValue(), 442, L"TestScripting_BasicLanguage_ComplexSideEffect[6]");
	}
	{
		BasicProgramNode program;
		program.DefineStructure(L"PPair")
			.Member(L"a", t_int())
			.Member(L"b", t_int())
			.Member(L"c", t_int());
		program.DefineFunction(L"main").ReturnType(t_int()).Statement(
			s_var(t_type(L"PPair"), L"c")
			<<s_var(t_int()[3], L"a")
			<<s_expr(e_name(L"a")[e_prim(0)].Assign(e_prim(0)))
			<<s_expr(e_name(L"a")[e_prim(1)].Assign(e_prim(1)))
			<<s_expr(e_name(L"a")[e_prim(2)].Assign(e_prim(2)))
			<<s_expr(e_name(L"c").Member(L"a").Assign(e_prim(3)))
			<<s_expr(e_name(L"c").Member(L"b").Assign(e_prim(4)))
			<<s_expr(e_name(L"c").Member(L"c").Assign(e_prim(5)))

			<<s_var(t_int(), L"n1", ++e_name(L"a")[e_prim(0)])
			<<s_var(t_int(), L"n2", ++e_name(L"a")[e_prim(1)])
			<<s_var(t_int(), L"n3", ++e_name(L"a")[e_prim(2)])
			<<s_var(t_int(), L"n4", ++e_name(L"c").Member(L"a"))
			<<s_var(t_int(), L"n5", ++e_name(L"c").Member(L"b"))
			<<s_var(t_int(), L"n6", ++e_name(L"c").Member(L"c"))

			<<s_expr(e_result().Assign(
				e_name(L"n1")*e_prim(100000)
				+e_name(L"n2")*e_prim(10000)
				+e_name(L"n3")*e_prim(1000)
				+e_name(L"n4")*e_prim(100)
				+e_name(L"n5")*e_prim(10)
				+e_name(L"n6")
				))
			);
		program.DefineFunction(L"copy").ReturnType(t_int())
			.Parameter(L"i", t_int())
			.Statement(
				s_expr(e_result().Assign(e_name(L"i")))
			);
		RunBasicProgram<vint>(program.GetInternalValue(), 123456, L"TestScripting_BasicLanguage_ComplexSideEffect[7]");
	}
	{
		BasicProgramNode program;
		program.DefineStructure(L"PPair")
			.Member(L"a", t_int())
			.Member(L"b", t_int())
			.Member(L"c", t_int());
		program.DefineFunction(L"main").ReturnType(t_int()).Statement(
			s_var(t_type(L"PPair"), L"c1")
			<<s_var(t_type(L"PPair"), L"c2")
			<<s_var(*t_type(L"PPair"), L"pc")
			<<s_var(t_int()[3], L"a")
			<<s_expr(e_name(L"c1").Member(L"a").Assign(e_prim(3)))
			<<s_expr(e_name(L"c1").Member(L"b").Assign(e_prim(4)))
			<<s_expr(e_name(L"c1").Member(L"c").Assign(e_prim(5)))

			<<s_expr(e_name(L"pc").Assign(e_name(L"c2").Assign(e_name(L"copy")(e_exps()<<e_name(L"c1"))).Ref()))
			<<s_var(t_int(), L"n1", ++e_name(L"pc").PMember(L"a"))
			<<s_var(t_int(), L"n2", ++e_name(L"pc").PMember(L"b"))
			<<s_var(t_int(), L"n3", ++e_name(L"pc").PMember(L"c"))

			<<s_expr(e_result().Assign(
				e_name(L"n1")*e_prim(100)
				+e_name(L"n2")*e_prim(10)
				+e_name(L"n3")
				))
			);
		program.DefineFunction(L"copy").ReturnType(t_type(L"PPair"))
			.Parameter(L"i", t_type(L"PPair"))
			.Statement(
				s_var(t_type(L"PPair"), L"j")
				<<s_expr(e_result().Assign(e_name(L"i")))
				<<s_expr(e_name(L"j").Assign(e_name(L"copy2")(e_exps()<<e_result())))
				<<s_expr(e_name(L"j").Member(L"a")++)
				<<s_expr(e_name(L"j").Member(L"b")++)
				<<s_expr(e_name(L"j").Member(L"c")++)
				<<s_expr(e_result().Assign(e_name(L"j")))
			);
		program.DefineFunction(L"copy2").ReturnType(t_type(L"PPair"))
			.Parameter(L"i", t_type(L"PPair"))
			.Statement(
				s_expr(e_result().Assign(e_name(L"i")))
			);
		RunBasicProgram<vint>(program.GetInternalValue(), 567, L"TestScripting_BasicLanguage_ComplexSideEffect[8]");
	}
	{
		BasicProgramNode program;
		program.DefineStructure(L"PPair")
			.Member(L"a", t_int())
			.Member(L"b", t_int())
			.Member(L"c", t_int());
		program.DefineFunction(L"main").ReturnType(t_int()).Statement(
			s_var(t_type(L"PPair"), L"c1")
			<<s_var(t_type(L"PPair"), L"c2")
			<<s_var(t_type(L"PPair"), L"c3")
			<<s_var(t_int()[3], L"a")
			<<s_expr(e_name(L"c1").Member(L"a").Assign(e_prim(3)))
			<<s_expr(e_name(L"c1").Member(L"b").Assign(e_prim(4)))
			<<s_expr(e_name(L"c1").Member(L"c").Assign(e_prim(5)))

			<<s_expr(e_name(L"c3").Assign(e_name(L"copy")(e_exps()<<(e_name(L"c2").Assign(e_name(L"c1"))))))
			<<s_var(t_int(), L"n1", ++e_name(L"c3").Member(L"a"))
			<<s_var(t_int(), L"n2", ++e_name(L"c3").Member(L"b"))
			<<s_var(t_int(), L"n3", ++e_name(L"c3").Member(L"c"))

			<<s_expr(e_result().Assign(
				e_name(L"n1")*e_prim(100)
				+e_name(L"n2")*e_prim(10)
				+e_name(L"n3")
				))
			);
		program.DefineFunction(L"copy").ReturnType(t_type(L"PPair"))
			.Parameter(L"i", t_type(L"PPair"))
			.Statement(
				s_expr(e_result().Assign(e_name(L"i")))
			);
		RunBasicProgram<vint>(program.GetInternalValue(), 456, L"TestScripting_BasicLanguage_ComplexSideEffect[9]");
	}
	{
		BasicProgramNode program;
		program.DefineStructure(L"PPair")
			.Member(L"a", t_int())
			.Member(L"b", t_int())
			.Member(L"c", t_int());
		program.DefineFunction(L"main").ReturnType(t_int()).Statement(
			s_var(t_type(L"PPair"), L"c1")
			<<s_var(t_type(L"PPair"), L"c2")
			<<s_var(t_type(L"PPair"), L"c3")
			<<s_var(t_int()[3], L"a")
			<<s_expr(e_name(L"c1").Member(L"a").Assign(e_prim(3)))
			<<s_expr(e_name(L"c1").Member(L"b").Assign(e_prim(4)))
			<<s_expr(e_name(L"c1").Member(L"c").Assign(e_prim(5)))

			<<s_expr(e_name(L"c3").Assign(e_name(L"copy")(e_exps()<<(e_name(L"c2").Assign(e_name(L"c1")).Ref()))))
			<<s_var(t_int(), L"n1", ++e_name(L"c3").Member(L"a"))
			<<s_var(t_int(), L"n2", ++e_name(L"c3").Member(L"b"))
			<<s_var(t_int(), L"n3", ++e_name(L"c3").Member(L"c"))

			<<s_expr(e_result().Assign(
				e_name(L"n1")*e_prim(100)
				+e_name(L"n2")*e_prim(10)
				+e_name(L"n3")
				))
			);
		program.DefineFunction(L"copy").ReturnType(t_type(L"PPair"))
			.Parameter(L"i", *t_type(L"PPair"))
			.Statement(
				s_expr(e_result().Assign(*e_name(L"i")))
			);
		RunBasicProgram<vint>(program.GetInternalValue(), 456, L"TestScripting_BasicLanguage_ComplexSideEffect[10]");
	}
}

/***********************************************************************
Test Structures
***********************************************************************/

TEST_CASE(TestScripting_BasicLanguage_ComplexNumber1)
{
	BasicProgramNode program;
	program.DefineStructure(L"Complex")
		.Member(L"r", t_int())
		.Member(L"i", t_int());
	program.DefineFunction(L"main").ReturnType(t_int()).Statement(
		s_var(t_type(L"Complex"), L"a")
		<<s_var(t_type(L"Complex"), L"b")
		<<s_var(t_type(L"Complex"), L"c")
		<<s_expr(e_name(L"a").Member(L"r").Assign(e_prim(1)))
		<<s_expr(e_name(L"a").Member(L"i").Assign(e_prim(2)))
		<<s_expr(e_name(L"b").Member(L"r").Assign(e_prim(3)))
		<<s_expr(e_name(L"b").Member(L"i").Assign(e_prim(4)))
		<<s_expr(e_name(L"c").Member(L"r").Assign(
			e_name(L"a").Member(L"r") + e_name(L"b").Member(L"r")
			))
		<<s_expr(e_name(L"c").Member(L"i").Assign(
			e_name(L"a").Member(L"i") + e_name(L"b").Member(L"i")
			))
		<<s_expr(e_result().Assign(
			e_name(L"c").Member(L"r")*e_prim(100) + e_name(L"c").Member(L"i")
			))
		);
	RunBasicProgram<vint>(program.GetInternalValue(), 406, L"TestScripting_BasicLanguage_ComplexNumber1");
}

TEST_CASE(TestScripting_BasicLanguage_ComplexNumber2)
{
	BasicProgramNode program;
	program.DefineStructure(L"Complex")
		.Member(L"r", t_int())
		.Member(L"i", t_int());
	program.DefineFunction(L"main").ReturnType(t_int()).Statement(
		s_var(t_type(L"Complex"), L"a")
		<<s_var(t_type(L"Complex"), L"b")
		<<s_var(t_type(L"Complex"), L"c")
		<<s_var(*t_type(L"Complex"), L"pa", e_name(L"a").Ref())
		<<s_var(*t_type(L"Complex"), L"pb", e_name(L"b").Ref())
		<<s_var(*t_type(L"Complex"), L"pc", e_name(L"c").Ref())
		<<s_expr(e_name(L"pa").PMember(L"r").Assign(e_prim(1)))
		<<s_expr(e_name(L"pa").PMember(L"i").Assign(e_prim(2)))
		<<s_expr(e_name(L"pb").PMember(L"r").Assign(e_prim(3)))
		<<s_expr(e_name(L"pb").PMember(L"i").Assign(e_prim(4)))
		<<s_expr(e_name(L"pc").PMember(L"r").Assign(
			e_name(L"pa").PMember(L"r") + e_name(L"pb").PMember(L"r")
			))
		<<s_expr(e_name(L"pc").PMember(L"i").Assign(
			e_name(L"pa").PMember(L"i") + e_name(L"pb").PMember(L"i")
			))
		<<s_expr(e_result().Assign(
			e_name(L"pc").PMember(L"r")*e_prim(100) + e_name(L"pc").PMember(L"i")
			))
		);
	RunBasicProgram<vint>(program.GetInternalValue(), 406, L"TestScripting_BasicLanguage_ComplexNumber2");
}

TEST_CASE(TestScripting_BasicLanguage_ComplexNumber3)
{
	BasicProgramNode program;
	program.DefineStructure(L"Complex")
		.Member(L"r", t_int())
		.Member(L"i", t_int());
	program.DefineFunction(L"main").ReturnType(t_int()).Statement(
		s_var(t_type(L"Complex"), L"a")
		<<s_var(t_type(L"Complex"), L"b")
		<<s_var(t_type(L"Complex"), L"c")
		<<s_expr(e_name(L"a").Member(L"r").Assign(e_prim(1)))
		<<s_expr(e_name(L"a").Member(L"i").Assign(e_prim(2)))
		<<s_expr(e_name(L"b").Member(L"r").Assign(e_prim(3)))
		<<s_expr(e_name(L"b").Member(L"i").Assign(e_prim(4)))
		<<s_var(t_type(L"Complex"), L"x", e_name(L"a"))
		<<s_var(t_type(L"Complex"), L"y")
		<<s_expr(e_name(L"y").Assign(e_name(L"b")))
		<<s_expr(e_name(L"c").Member(L"r").Assign(
			e_name(L"x").Member(L"r") + e_name(L"y").Member(L"r")
			))
		<<s_expr(e_name(L"c").Member(L"i").Assign(
			e_name(L"x").Member(L"i") + e_name(L"y").Member(L"i")
			))
		<<s_expr(e_result().Assign(
			e_name(L"c").Member(L"r")*e_prim(100) + e_name(L"c").Member(L"i")
			))
		);
	RunBasicProgram<vint>(program.GetInternalValue(), 406, L"TestScripting_BasicLanguage_ComplexNumber3");
}

/***********************************************************************
Test Statements
***********************************************************************/

TEST_CASE(TestScripting_BasicLanguage_IfStatement)
{
	{
		BasicProgramNode program;
		program.DefineFunction(L"main").ReturnType(t_int()).Statement(
			s_var(t_int(), L"a", e_prim(0))
			<<s_expr(e_result().Assign(e_prim(0)))
			<<s_if(e_name(L"a")
				,s_expr(e_result().Assign(e_prim(100)))
				)
			);
		RunBasicProgram<vint>(program.GetInternalValue(), 0, L"TestScripting_BasicLanguage_IfStatement[1]");
	}
	{
		BasicProgramNode program;
		program.DefineFunction(L"main").ReturnType(t_int()).Statement(
			s_var(t_int(), L"a", e_prim(1))
			<<s_expr(e_result().Assign(e_prim(0)))
			<<s_if(e_name(L"a")
				,s_expr(e_result().Assign(e_prim(100)))
				)
			);
		RunBasicProgram<vint>(program.GetInternalValue(), 100, L"TestScripting_BasicLanguage_IfStatement[2]");
	}
	{
		BasicProgramNode program;
		program.DefineFunction(L"main").ReturnType(t_int()).Statement(
			s_var(t_int(), L"a", e_prim(0))
			<<s_expr(e_result().Assign(e_prim(0)))
			<<s_if(e_name(L"a")
				,s_expr(e_result().Assign(e_prim(100)))
				,s_expr(e_result().Assign(e_prim(200)))
				)
			);
		RunBasicProgram<vint>(program.GetInternalValue(), 200, L"TestScripting_BasicLanguage_IfStatement[3]");
	}
	{
		BasicProgramNode program;
		program.DefineFunction(L"main").ReturnType(t_int()).Statement(
			s_var(t_int(), L"a", e_prim(1))
			<<s_expr(e_result().Assign(e_prim(0)))
			<<s_if(e_name(L"a")
				,s_expr(e_result().Assign(e_prim(100)))
				,s_expr(e_result().Assign(e_prim(200)))
				)
			);
		RunBasicProgram<vint>(program.GetInternalValue(), 100, L"TestScripting_BasicLanguage_IfStatement[4]");
	}
}

TEST_CASE(TestScripting_BasicLanguage_WhileStatement)
{
	{
		BasicProgramNode program;
		program.DefineFunction(L"main").ReturnType(t_int()).Statement(
			s_var(t_int(), L"i", e_prim(1))
			<<s_expr(e_result().Assign(e_prim(0)))
			<<s_while(e_name(L"i")<=e_prim(10),
				s_expr(e_result()+=e_name(L"i"))
				<<s_expr(e_name(L"i")++)
				)
			);
		RunBasicProgram<vint>(program.GetInternalValue(), 55, L"TestScripting_BasicLanguage_WhileStatement[1]");
	}
	{
		BasicProgramNode program;
		program.DefineFunction(L"main").ReturnType(t_int()).Statement(
			s_var(t_int(), L"i", e_prim(1))
			<<s_expr(e_result().Assign(e_prim(0)))
			<<s_do_while(e_name(L"i")<=e_prim(10),
				s_expr(e_result()+=e_name(L"i"))
				<<s_expr(e_name(L"i")++)
				)
			);
		RunBasicProgram<vint>(program.GetInternalValue(), 55, L"TestScripting_BasicLanguage_WhileStatement[2]");
	}
	{
		BasicProgramNode program;
		program.DefineFunction(L"main").ReturnType(t_int()).Statement(
			s_var(t_int(), L"i", e_prim(1))
			<<s_expr(e_result().Assign(e_prim(0)))
			<<s_conditional_loop(e_name(L"i")<=e_prim(10),e_name(L"i")<=e_prim(10),
				s_expr(e_result()+=e_name(L"i"))
				<<s_expr(e_name(L"i")++)
				)
			);
		RunBasicProgram<vint>(program.GetInternalValue(), 55, L"TestScripting_BasicLanguage_WhileStatement[3]");
	}
	{
		BasicProgramNode program;
		program.DefineFunction(L"main").ReturnType(t_int()).Statement(
			s_var(t_int(), L"i", e_prim(1))
			<<s_expr(e_result().Assign(e_prim(0)))
			<<s_loop(
				s_expr(e_result()+=e_name(L"i"))
				<<s_expr(e_name(L"i")++)
				<<s_if(e_name(L"i")<=e_prim(10),s_continue(),s_break())
				)
			);
		RunBasicProgram<vint>(program.GetInternalValue(), 55, L"TestScripting_BasicLanguage_WhileStatement[4]");
	}
}

TEST_CASE(TestScripting_BasicLanguage_ForStatement)
{
	{
		BasicProgramNode program;
		program.DefineFunction(L"main").ReturnType(t_int()).Statement(
			s_expr(e_result().Assign(e_prim(0)))
			<<s_for(
				s_var(t_int(), L"i", e_prim(1)),
				e_name(L"i")<=e_prim(10),
				s_expr(e_name(L"i")++),
				s_expr(e_result()+=e_name(L"i"))
				)
			);
		RunBasicProgram<vint>(program.GetInternalValue(), 55, L"TestScripting_BasicLanguage_ForStatement[1]");
	}
	{
		BasicProgramNode program;
		program.DefineFunction(L"main").ReturnType(t_int()).Statement(
			s_expr(e_result().Assign(e_prim(0)))
			<<s_for(
				s_var(t_int(), L"i", e_prim(1))<<s_var(t_int(), L"j", e_prim(1)),
				e_name(L"i")+e_name(L"j")<=e_prim(20),
				s_expr(e_name(L"i")++)<<s_expr(e_name(L"j")++),
				s_expr(e_result()+=e_name(L"i")+e_name(L"j"))
				)
			);
		RunBasicProgram<vint>(program.GetInternalValue(), 110, L"TestScripting_BasicLanguage_ForStatement[2]");
	}
}

TEST_CASE(TestScripting_BasicLanguage_ReturnStatement)
{
	BasicProgramNode program;
	program.DefineFunction(L"main").ReturnType(t_int()).Statement(
		s_var(t_int(), L"i", e_prim(1))
		<<s_expr(e_result().Assign(e_prim(0)))
		<<s_loop(
			s_expr(e_result()+=e_name(L"i"))
			<<s_expr(e_name(L"i")++)
			<<s_if(e_name(L"i")>e_prim(10),s_return())
			)
		);
	RunBasicProgram<vint>(program.GetInternalValue(), 55, L"TestScripting_BasicLanguage_ReturnStatement");
}

/***********************************************************************
Test Functions
***********************************************************************/

TEST_CASE(TestScripting_BasicLanguage_SimpleFunction)
{
	BasicProgramNode program;
	program.DefineFunction(L"main").ReturnType(t_int()).Statement(
		s_expr(e_result().Assign(e_name(L"add")(e_exps()<<e_prim(1)<<e_prim(2))))
		);
	program.DefineFunction(L"add").ReturnType(t_int())
		.Parameter(L"a", t_int())
		.Parameter(L"b", t_int())
		.Statement(
		s_expr(e_result().Assign(e_name(L"a")+e_name(L"b")))
		);
	RunBasicProgram<vint>(program.GetInternalValue(), 3, L"TestScripting_BasicLanguage_SimpleFunction");
}

TEST_CASE(TestScripting_BasicLanguage_FunctionPointer)
{
	BasicProgramNode program;
	program.DefineFunction(L"main").ReturnType(t_int()).Statement(
		s_var(t_int()(t_types()<<t_int()<<t_int()), L"padd", e_name(L"add"))
		<<s_expr(e_result().Assign(e_name(L"padd")(e_exps()<<e_prim(1)<<e_prim(2))))
		);
	program.DefineFunction(L"add").ReturnType(t_int())
		.Parameter(L"a", t_int())
		.Parameter(L"b", t_int())
		.Statement(
		s_expr(e_result().Assign(e_name(L"a")+e_name(L"b")))
		);
	RunBasicProgram<vint>(program.GetInternalValue(), 3, L"TestScripting_BasicLanguage_FunctionPointer");
}

/***********************************************************************
Test Global Variables
***********************************************************************/

TEST_CASE(TestScripting_BasicLanguage_GlobalVariable1)
{
	BasicProgramNode program;
	program.DefineVariable(L"x", t_int(), e_prim(100));
	program.DefineFunction(L"main").ReturnType(t_int()).Statement(
		s_expr(e_result().Assign(e_name(L"x")))
		);
	RunBasicProgram<vint>(program.GetInternalValue(), 100, L"TestScripting_BasicLanguage_GlobalVariable1");
}

TEST_CASE(TestScripting_BasicLanguage_GlobalVariable2)
{
	BasicProgramNode program;
	program.DefineVariable(L"x", t_int(), e_prim(100));
	program.DefineFunction(L"main").ReturnType(t_int()).Statement(
		s_expr(e_name(L"add")(e_exps()<<e_prim(1)<<e_prim(2)))
		<<s_expr(e_result().Assign(e_name(L"x")))
		);
	program.DefineFunction(L"add").ReturnType(t_void())
		.Parameter(L"a", t_int())
		.Parameter(L"b", t_int())
		.Statement(
		s_expr(e_name(L"x").Assign(e_name(L"a")+e_name(L"b")))
		);
	RunBasicProgram<vint>(program.GetInternalValue(), 3, L"TestScripting_BasicLanguage_GlobalVariable2");
}

/***********************************************************************
Test Others
***********************************************************************/

TEST_CASE(TestScripting_BasicLanguage_TypeUpgrade)
{
	BasicProgramNode program;
	program.DefineFunction(L"main").ReturnType(t_int()).Statement(
		s_var(t_uint8(), L"a", e_prim((unsigned __int8)200))
		<<s_var(t_uint8(), L"b", e_prim((unsigned __int8)200))
		<<s_expr(e_result().Assign(e_name(L"a")+e_name(L"b")))
		);
	RunBasicProgram<vint>(program.GetInternalValue(), 400, L"TestScripting_BasicLanguage_TypeUpgrade");
}

TEST_CASE(TestScripting_BasicLanguage_DereferenceLeftValue)
{
	BasicProgramNode program;
	program.DefineFunction(L"main").ReturnType(t_int()).Statement(
		s_var(t_uint8(), L"a", e_prim((unsigned __int8)200))
		<<s_var(t_uint8(), L"b", e_prim((unsigned __int8)200))
		<<s_expr((*e_result().Ref()).Assign(e_name(L"a")+e_name(L"b")))
		);
	RunBasicProgram<vint>(program.GetInternalValue(), 400, L"TestScripting_BasicLanguage_DereferenceLeftValue");
}

TEST_CASE(TestScripting_BasicLanguage_AnsiString)
{
	BasicProgramNode program;
	program.DefineFunction(L"main").ReturnType(t_int()).Statement(
		s_var(*t_char(), L"a", e_prim("ABCDE"))
		<<s_expr(e_result().Assign(e_prim(0)))
		<<s_while(*e_name(L"a"),
			s_expr(e_result()+=*e_name(L"a"))
			<<s_expr(e_name(L"a")++)
			)
		);
	RunBasicProgram<vint>(program.GetInternalValue(), 335, L"TestScripting_BasicLanguage_AnsiString");
}

TEST_CASE(TestScripting_BasicLanguage_WideString)
{
	BasicProgramNode program;
	program.DefineFunction(L"main").ReturnType(t_int()).Statement(
		s_var(*t_wchar(), L"a", e_prim(L"ABCDE"))
		<<s_expr(e_result().Assign(e_prim(0)))
		<<s_while(*e_name(L"a"),
			s_expr(e_result()+=*e_name(L"a"))
			<<s_expr(e_name(L"a")++)
			)
		);
	RunBasicProgram<vint>(program.GetInternalValue(), 335, L"TestScripting_BasicLanguage_WideString");
}

TEST_CASE(TestScripting_BasicLanguage_Null)
{
	{
		BasicProgramNode program;
		program.DefineFunction(L"main").ReturnType(t_bool()).Statement(
			s_var(*t_int(), L"a", e_null())
			<<s_expr(e_result().Assign(e_name(L"a")==e_null()))
			);
		RunBasicProgram<bool>(program.GetInternalValue(), true, L"TestScripting_BasicLanguage_Null[1]");
	}
	{
		BasicProgramNode program;
		program.DefineFunction(L"main").ReturnType(t_bool()).Statement(
			s_expr(e_result().Assign(e_name(L"main")==e_null()))
			);
		RunBasicProgram<bool>(program.GetInternalValue(), false, L"TestScripting_BasicLanguage_Null[2]");
	}
}