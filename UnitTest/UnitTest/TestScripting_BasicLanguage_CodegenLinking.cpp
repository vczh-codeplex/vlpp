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

//from TestScripting_BasicLanguage_Analyzer.cpp
extern void SetConfiguration(BasicAlgorithmConfiguration& config);
//from TestScripting_Languages_NativeX.cpp
extern void PrintNativeXProgram(Ptr<BasicProgram> program, TextWriter& writer);
extern void ConvertToNativeXProgram(Ptr<BasicProgram>& program);

Ptr<BasicIL> Compile(Ptr<BasicProgram> program, const WString& name, const WString& fileName)
{
	ConvertToNativeXProgram(program);
	BasicAlgorithmConfiguration configuration;
	SetConfiguration(configuration);
	BasicAnalyzer analyzer(program, 0, configuration);
	analyzer.Analyze();
	TEST_ASSERT(analyzer.GetErrors().Count()==0);
	BasicCodeGenerator codegen(&analyzer, 0, name);
	codegen.GenerateCode();
	{
		WString path=GetPath()+L"Codegen_"+fileName+L".txt";
		FileStream fileStream(path, FileStream::WriteOnly);
		BomEncoder encoder(BomEncoder::Utf16);
		EncoderStream encoderStream(fileStream, encoder);
		StreamWriter fileWriter(encoderStream);

		fileWriter.WriteLine(L"/*NativeX Code*/");
		PrintNativeXProgram(program, fileWriter);
		fileWriter.WriteLine(L"");

		BasicLanguageCommentProvider commentProvider;
		TextWriter* commentProviderWriter=commentProvider.OpenWriter();
		PrintNativeXProgram(program, *commentProviderWriter);
		commentProvider.CloseWriter();

		fileWriter.WriteLine(L"/*Assembly*/");
		codegen.GetIL()->SaveAsString(fileWriter, &commentProvider);
	}
	return codegen.GetIL();
}

/***********************************************************************
Simple Linking
***********************************************************************/

TEST_CASE(TestScripting_BasicLanguage_Linking)
{
	BasicProgramNode programAdd;
	programAdd
		.DefineVariable(L"leftOperand", t_int(), e_prim(0));
	programAdd
		.DefineFunction(L"add")
		.ReturnType(t_int())
		.Parameter(L"rightOperand", t_int())
		.Statement(
			s_expr(e_result().Assign(e_name(L"leftOperand")+e_name(L"rightOperand")))
			);
	Ptr<BasicIL> ilAdd=Compile(programAdd.GetInternalValue(), L"programAdd", L"TestScripting_BasicLanguage_Linking_programAdd");

	BasicProgramNode programMain;
	programMain
		.DefineVariable(L"adder", t_int(), L"programAdd", L"leftOperand");
	programMain
		.DefineFunction(L"add")
		.ReturnType(t_int())
		.Parameter(L"offset", t_int())
		.Linking(L"programAdd", L"add")
		;
	programMain
		.DefineFunction(L"main")
		.ReturnType(t_int())
		.Statement(
			s_expr(e_name(L"adder").Assign(e_prim(1)))
			<<s_expr(e_result().Assign(e_name(L"add")(e_exps()<<e_prim(2))))
			);
	Ptr<BasicIL> ilMain=Compile(programMain.GetInternalValue(), L"programMain", L"TestScripting_BasicLanguage_Linking_programMain");

	BasicILInterpretor interpretor(65536);
	int addKey=interpretor.LoadIL(ilAdd.Obj());
	int mainKey=interpretor.LoadIL(ilMain.Obj());
	
	BasicILStack stack(&interpretor);
	stack.Reset(0, addKey, 0);
	TEST_ASSERT(stack.Run()==BasicILStack::Finished);
	TEST_ASSERT(stack.GetEnv()->StackTop()==stack.GetEnv()->StackSize());
	stack.Reset(0, mainKey, 0);
	TEST_ASSERT(stack.Run()==BasicILStack::Finished);
	TEST_ASSERT(stack.GetEnv()->StackTop()==stack.GetEnv()->StackSize());
	
	int ins=ilMain->labels[0].instructionIndex;
	stack.Reset(ins, mainKey, sizeof(int));
	TEST_ASSERT(stack.Run()==BasicILStack::Finished);
	TEST_ASSERT(stack.GetEnv()->Pop<int>()==3);
	TEST_ASSERT(stack.GetEnv()->StackTop()==stack.GetEnv()->StackSize());
}

/***********************************************************************
Function Pointer
***********************************************************************/

TEST_CASE(TestScripting_BasicLanguage_Pointer)
{
	BasicProgramNode programAdd;
	programAdd
		.DefineFunction(L"add")
		.ReturnType(t_int())
		.Parameter(L"a", t_int())
		.Parameter(L"b", t_int())
		.Statement(
			s_expr(e_result().Assign(e_name(L"a")+e_name(L"b")))
			);
	Ptr<BasicIL> ilAdd=Compile(programAdd.GetInternalValue(), L"programAdd", L"TestScripting_BasicLanguage_Pointer_programAdd");

	BasicProgramNode programMain;
	programMain
		.DefineFunction(L"main")
		.ReturnType(t_int())
		.Statement(
			s_var(t_int()(t_types()<<t_int()<<t_int()), L"padd1", e_name(L"add1"))
			<<s_var(t_int()(t_types()<<t_int()<<t_int()), L"padd2", e_name(L"add2"))
			<<s_var(t_int(), L"a", e_name(L"padd1")(e_exps()<<e_prim(1)<<e_prim(2)))
			<<s_var(t_int(), L"b", e_name(L"padd2")(e_exps()<<e_prim(3)<<e_prim(4)))
			<<s_expr(e_result().Assign(e_name(L"a")*e_prim(10)+e_name(L"b")))
			);
	programMain
		.DefineFunction(L"add1")
		.ReturnType(t_int())
		.Parameter(L"a", t_int())
		.Parameter(L"b", t_int())
		.Linking(L"programAdd", L"add")
		;
	programMain
		.DefineFunction(L"add2")
		.ReturnType(t_int())
		.Parameter(L"a", t_int())
		.Parameter(L"b", t_int())
		.Statement(
			s_expr(e_result().Assign(e_name(L"a")+e_name(L"b")))
			);
	Ptr<BasicIL> ilMain=Compile(programMain.GetInternalValue(), L"programMain", L"TestScripting_BasicLanguage_Pointer_programMain");

	BasicILInterpretor interpretor(65536);
	int addKey=interpretor.LoadIL(ilAdd.Obj());
	int mainKey=interpretor.LoadIL(ilMain.Obj());
	
	BasicILStack stack(&interpretor);
	stack.Reset(0, addKey, 0);
	TEST_ASSERT(stack.Run()==BasicILStack::Finished);
	TEST_ASSERT(stack.GetEnv()->StackTop()==stack.GetEnv()->StackSize());
	stack.Reset(0, mainKey, 0);
	TEST_ASSERT(stack.Run()==BasicILStack::Finished);
	TEST_ASSERT(stack.GetEnv()->StackTop()==stack.GetEnv()->StackSize());
	
	int ins=ilMain->labels[0].instructionIndex;
	stack.Reset(ins, mainKey, sizeof(int));
	TEST_ASSERT(stack.Run()==BasicILStack::Finished);
	TEST_ASSERT(stack.GetEnv()->Pop<int>()==37);
	TEST_ASSERT(stack.GetEnv()->StackTop()==stack.GetEnv()->StackSize());
}