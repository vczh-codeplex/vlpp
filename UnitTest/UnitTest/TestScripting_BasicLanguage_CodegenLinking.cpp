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
//from TestScripting_BasicLanguage_Codegen.cpp
extern void LogInterpretor(BasicILInterpretor& interpretor, const WString& fileName);

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

TEST_CASE(TestScripting_BasicLanguage_LinkingFunction)
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
	vint addKey=interpretor.LoadIL(ilAdd.Obj());
	vint mainKey=interpretor.LoadIL(ilMain.Obj());
	
	BasicILStack stack(&interpretor);
	stack.Reset(0, addKey, 0);
	TEST_ASSERT(stack.Run()==BasicILStack::Finished);
	TEST_ASSERT(stack.GetEnv()->StackTop()==stack.GetEnv()->StackSize());
	stack.Reset(0, mainKey, 0);
	TEST_ASSERT(stack.Run()==BasicILStack::Finished);
	TEST_ASSERT(stack.GetEnv()->StackTop()==stack.GetEnv()->StackSize());
	
	vint ins=ilMain->labels[0].instructionIndex;
	stack.Reset(ins, mainKey, sizeof(vint));
	TEST_ASSERT(stack.Run()==BasicILStack::Finished);
	TEST_ASSERT(stack.GetEnv()->Pop<vint>()==3);
	TEST_ASSERT(stack.GetEnv()->StackTop()==stack.GetEnv()->StackSize());

	LogInterpretor(interpretor, L"TestScripting_BasicLanguage_Linking_[Interpretor]");
}

/***********************************************************************
Function Pointer
***********************************************************************/

TEST_CASE(TestScripting_BasicLanguage_LinkingPointer)
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
	Ptr<BasicIL> ilAdd=Compile(programAdd.GetInternalValue(), L"programAdd", L"TestScripting_BasicLanguage_LinkingPointer_programAdd");

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
	Ptr<BasicIL> ilMain=Compile(programMain.GetInternalValue(), L"programMain", L"TestScripting_BasicLanguage_LinkingPointer_programMain");

	BasicILInterpretor interpretor(65536);
	vint addKey=interpretor.LoadIL(ilAdd.Obj());
	vint mainKey=interpretor.LoadIL(ilMain.Obj());
	
	BasicILStack stack(&interpretor);
	stack.Reset(0, addKey, 0);
	TEST_ASSERT(stack.Run()==BasicILStack::Finished);
	TEST_ASSERT(stack.GetEnv()->StackTop()==stack.GetEnv()->StackSize());
	stack.Reset(0, mainKey, 0);
	TEST_ASSERT(stack.Run()==BasicILStack::Finished);
	TEST_ASSERT(stack.GetEnv()->StackTop()==stack.GetEnv()->StackSize());
	
	vint ins=ilMain->labels[0].instructionIndex;
	stack.Reset(ins, mainKey, sizeof(vint));
	TEST_ASSERT(stack.Run()==BasicILStack::Finished);
	TEST_ASSERT(stack.GetEnv()->Pop<vint>()==37);
	TEST_ASSERT(stack.GetEnv()->StackTop()==stack.GetEnv()->StackSize());

	LogInterpretor(interpretor, L"TestScripting_BasicLanguage_LinkingPointer_[Interpretor]");
}

/***********************************************************************
Generic Function
***********************************************************************/

TEST_CASE(TestScripting_BasicLanguage_LinkingGenericFunction)
{
	BasicProgramNode programMain;
	BasicProgramNode programGeneric;

	programGeneric
		.Generic().GenericArgument(L"U")
		.DefineFunction(L"Apply2")
		.Parameter(L"f", t_type(L"U")(t_types()<<t_type(L"U")<<t_type(L"U")))
		.Parameter(L"a", t_type(L"U"))
		.Parameter(L"b", t_type(L"U"))
		.ReturnType(t_type(L"U"))
		.Statement(
			s_expr(e_result().Assign(e_name(L"f")(e_exps()<<e_name(L"a")<<e_name(L"b"))))
			);
	programGeneric
		.Generic().GenericArgument(L"T")
		.DefineFunction(L"Sum")
		.Parameter(L"items", *t_type(L"T"))
		.Parameter(L"count", t_int())
		.Parameter(L"init", t_type(L"T"))
		.Parameter(L"f",  t_type(L"T")(t_types()<<t_type(L"T")<<t_type(L"T")))
		.ReturnType(t_type(L"T"))
		.Statement(
			s_expr(e_result().Assign(e_name(L"init")))
			<<s_while(e_name(L"count")>e_prim(0),
					s_expr(e_result().Assign(e_name(L"Apply2", t_types()<<t_type(L"T"))(e_exps()<<e_name(L"f")<<e_result()<<*e_name(L"items"))))
					<<s_expr(e_name(L"count")--)
					<<s_expr(e_name(L"items")++)
				)
			);
		;
	Ptr<BasicIL> ilGeneric=Compile(programGeneric.GetInternalValue(), L"programGeneric", L"TestScripting_BasicLanguage_LinkingGeneric_ProgramGeneric");

	programMain
		.DefineFunction(L"main")
		.ReturnType(t_int())
		.Statement(
			s_var(t_int()[5], L"numbers")
			<<s_expr(e_name(L"numbers")[e_prim(0)].Assign(e_prim(1)))
			<<s_expr(e_name(L"numbers")[e_prim(1)].Assign(e_prim(3)))
			<<s_expr(e_name(L"numbers")[e_prim(2)].Assign(e_prim(5)))
			<<s_expr(e_name(L"numbers")[e_prim(3)].Assign(e_prim(7)))
			<<s_expr(e_name(L"numbers")[e_prim(4)].Assign(e_prim(9)))
			<<s_expr(e_result().Assign(
				e_name(L"Sum", t_types()<<t_int())(e_exps()
					<<e_name(L"numbers").Ref()[*t_int()]
					<<e_prim(5)
					<<e_prim(0)
					<<e_name(L"Add")
					)
				))
			);
	programMain
		.DefineFunction(L"Add")
		.Parameter(L"a", t_int())
		.Parameter(L"b", t_int())
		.ReturnType(t_int())
		.Statement(
			s_expr(e_result().Assign(e_name(L"a")+e_name(L"b")))
			);
	programMain
		.Generic().GenericArgument(L"T")
		.DefineFunction(L"Sum")
		.Parameter(L"items", *t_type(L"T"))
		.Parameter(L"count", t_int())
		.Parameter(L"init", t_type(L"T"))
		.Parameter(L"f",  t_type(L"T")(t_types()<<t_type(L"T")<<t_type(L"T")))
		.ReturnType(t_type(L"T"))
		.Linking(L"programGeneric", L"Sum");
	Ptr<BasicIL> ilMain=Compile(programMain.GetInternalValue(), L"programMain", L"TestScripting_BasicLanguage_LinkingGeneric_ProgramMain");

	BasicILInterpretor interpretor(65536);
	vint sumKey=interpretor.LoadIL(ilGeneric.Obj());
	vint mainKey=interpretor.LoadIL(ilMain.Obj());
	
	BasicILStack stack(&interpretor);
	stack.Reset(0, sumKey, 0);
	TEST_ASSERT(stack.Run()==BasicILStack::Finished);
	TEST_ASSERT(stack.GetEnv()->StackTop()==stack.GetEnv()->StackSize());
	stack.Reset(0, mainKey, 0);
	TEST_ASSERT(stack.Run()==BasicILStack::Finished);
	TEST_ASSERT(stack.GetEnv()->StackTop()==stack.GetEnv()->StackSize());
	
	vint ins=ilMain->labels[0].instructionIndex;
	stack.Reset(ins, mainKey, sizeof(vint));
	TEST_ASSERT(stack.Run()==BasicILStack::Finished);
	TEST_ASSERT(stack.GetEnv()->Pop<vint>()==25);
	TEST_ASSERT(stack.GetEnv()->StackTop()==stack.GetEnv()->StackSize());

	LogInterpretor(interpretor, L"TestScripting_BasicLanguage_LinkingGeneric_[Interpretor]");
}

/***********************************************************************
Generic Global Variable
***********************************************************************/

TEST_CASE(TestScripting_BasicLanguage_LinkingGenericVariable)
{
	BasicProgramNode programStorage;
	programStorage
		.Generic().GenericArgument(L"T")
		.DefineStructure(L"Storage")
		.Member(L"data", t_type(L"T"))
		;
	programStorage
		.Generic().GenericArgument(L"U")
		.DefineVariable(L"storage", t_type(L"Storage")[t_types()<<t_type(L"U")])
		;
	programStorage
		.Generic().GenericArgument(L"V")
		.DefineFunction(L"Get")
		.ReturnType(t_type(L"V"))
		.Statement(
			s_expr(e_result().Assign(e_name(L"storage", t_types()<<t_type(L"V")).Member(L"data")))
			);
	Ptr<BasicIL> ilStorage=Compile(programStorage.GetInternalValue(), L"programStorage", L"TestScripting_BasicLanguage_LinkingStorage_ProgramStorage");
	
	BasicProgramNode programMain;
	programMain
		.Generic().GenericArgument(L"T")
		.DefineStructure(L"Storage")
		.Member(L"data", t_type(L"T"))
		.Linking(L"programStorage", L"Storage")
		;
	programMain
		.Generic().GenericArgument(L"U")
		.DefineVariable(L"storage", t_type(L"Storage")[t_types()<<t_type(L"U")], L"programStorage", L"storage")
		;
	programMain
		.Generic().GenericArgument(L"V")
		.DefineFunction(L"Get")
		.ReturnType(t_type(L"V"))
		.Linking(L"programStorage", L"Get")
		;
	programMain
		.DefineFunction(L"main")
		.ReturnType(t_int())
		.Statement(
			s_expr(e_name(L"storage", t_types()<<t_int()).Member(L"data").Assign(e_prim(10)))
			<<s_expr(e_name(L"Set", t_types()<<t_char())(e_exps()<<e_prim((char)20)))
			<<s_var(t_int(), L"a", e_name(L"Get", t_types()<<t_int())(e_exps()))
			<<s_var(t_int(), L"b", e_name(L"storage", t_types()<<t_char()).Member(L"data"))
			<<s_expr(e_result().Assign(e_name(L"a")+e_name(L"b")))
			);
	programMain
		.Generic().GenericArgument(L"W")
		.DefineFunction(L"Set")
		.Parameter(L"value", t_type(L"W"))
		.Statement(
			s_expr(e_name(L"storage", t_types()<<t_type(L"W")).Member(L"data").Assign(e_name(L"value")))
			);
	Ptr<BasicIL> ilMain=Compile(programMain.GetInternalValue(), L"programMain", L"TestScripting_BasicLanguage_LinkingStorage_ProgramMain");

	BasicILInterpretor interpretor(65536);
	vint sumKey=interpretor.LoadIL(ilStorage.Obj());
	vint mainKey=interpretor.LoadIL(ilMain.Obj());
	
	BasicILStack stack(&interpretor);
	stack.Reset(0, sumKey, 0);
	TEST_ASSERT(stack.Run()==BasicILStack::Finished);
	TEST_ASSERT(stack.GetEnv()->StackTop()==stack.GetEnv()->StackSize());
	stack.Reset(0, mainKey, 0);
	TEST_ASSERT(stack.Run()==BasicILStack::Finished);
	TEST_ASSERT(stack.GetEnv()->StackTop()==stack.GetEnv()->StackSize());
	
	vint ins=ilMain->labels[0].instructionIndex;
	stack.Reset(ins, mainKey, sizeof(vint));
	TEST_ASSERT(stack.Run()==BasicILStack::Finished);
	TEST_ASSERT(stack.GetEnv()->Pop<vint>()==30);
	TEST_ASSERT(stack.GetEnv()->StackTop()==stack.GetEnv()->StackSize());

	LogInterpretor(interpretor, L"TestScripting_BasicLanguage_LinkingStorage_[Interpretor]");
}

/***********************************************************************
Generic Concept
***********************************************************************/

TEST_CASE(TestScripting_BasicLanguage_LinkingGenericConcept)
{
	BasicProgramNode programEq;
	programEq
		.DefineConcept(L"Eq", L"T")
		.Member(L"Equals", t_bool()(t_types()<<t_type(L"T")<<t_type(L"T")))
		.Member(L"NotEquals", t_bool()(t_types()<<t_type(L"T")<<t_type(L"T")))
		;
	programEq
		.DefineInstance(t_int(), L"Eq")
		.Member(L"Equals", L"IntEquals")
		.Member(L"NotEquals", L"IntNotEquals")
		;
	programEq
		.DefineFunction(L"IntEquals")
		.ReturnType(t_bool())
		.Parameter(L"a", t_int())
		.Parameter(L"b", t_int())
		.Statement(
			s_expr(e_result().Assign(e_name(L"a")==e_name(L"b")))
			);
	programEq
		.DefineFunction(L"IntNotEquals")
		.ReturnType(t_bool())
		.Parameter(L"a", t_int())
		.Parameter(L"b", t_int())
		.Statement(
			s_expr(e_result().Assign(e_name(L"a")!=e_name(L"b")))
			);
	Ptr<BasicIL> ilEq=Compile(programEq.GetInternalValue(), L"programEq", L"TestScripting_BasicLanguage_LinkingConcept_ProgramEq");
	
	BasicProgramNode programVector;
	programVector
		.DefineConcept(L"Eq", L"T")
		.Member(L"Equals", t_bool()(t_types()<<t_type(L"T")<<t_type(L"T")))
		.Member(L"NotEquals", t_bool()(t_types()<<t_type(L"T")<<t_type(L"T")))
		.Linking(L"programEq", L"Eq")
		;
	programVector
		.Generic().GenericArgument(L"U")
		.DefineStructure(L"Vector")
		.Member(L"X", t_type(L"U"))
		.Member(L"Y", t_type(L"U"))
		;
	programVector
		.Generic().GenericArgument(L"V")
		.Where(L"V", L"Eq")
		.DefineInstance(t_type(L"Vector"), L"Eq")
		.Member(L"Equals", L"VectorEquals", t_types()<<t_type(L"V"))
		.Member(L"NotEquals", L"VectorNotEquals", t_types()<<t_type(L"V"))
		;
	programVector
		.Generic().GenericArgument(L"W")
		.Where(L"W", L"Eq")
		.DefineFunction(L"VectorEquals")
		.ReturnType(t_bool())
		.Parameter(L"a", t_type(L"Vector")[t_types()<<t_type(L"W")])
		.Parameter(L"b", t_type(L"Vector")[t_types()<<t_type(L"W")])
		.Statement(
			s_var(t_bool(), L"x_equals", e_name(L"Eq", t_type(L"W"), L"Equals")(e_exps()<<e_name(L"a").Member(L"X")<<e_name(L"b").Member(L"X")))
			<<s_var(t_bool(), L"y_equals", e_name(L"Eq", t_type(L"W"), L"Equals")(e_exps()<<e_name(L"a").Member(L"Y")<<e_name(L"b").Member(L"Y")))
			<<s_expr(e_result().Assign(e_name(L"x_equals")&&e_name(L"y_equals")))
			);
	programVector
		.Generic().GenericArgument(L"W")
		.Where(L"W", L"Eq")
		.DefineFunction(L"VectorNotEquals")
		.ReturnType(t_bool())
		.Parameter(L"a", t_type(L"Vector")[t_types()<<t_type(L"W")])
		.Parameter(L"b", t_type(L"Vector")[t_types()<<t_type(L"W")])
		.Statement(
			s_expr(e_result().Assign(!e_name(L"VectorEquals", t_types()<<t_type(L"W"))(e_exps()<<e_name(L"a")<<e_name(L"b"))))
			);
	Ptr<BasicIL> ilVector=Compile(programVector.GetInternalValue(), L"programVector", L"TestScripting_BasicLanguage_LinkingConcept_ProgramVector");


	BasicProgramNode programMain;
	programMain
		.Generic().GenericArgument(L"U")
		.DefineStructure(L"Vector")
		.Member(L"X", t_type(L"U"))
		.Member(L"Y", t_type(L"U"))
		.Linking(L"programVector", L"Vector")
		;
	programMain
		.DefineConcept(L"Eq", L"T")
		.Member(L"Equals", t_bool()(t_types()<<t_type(L"T")<<t_type(L"T")))
		.Member(L"NotEquals", t_bool()(t_types()<<t_type(L"T")<<t_type(L"T")))
		.Linking(L"programEq", L"Eq")
		;
	programMain
		.DefineInstanceForward(t_int(), L"Eq")
		;
	programMain
		.Generic().GenericArgument(L"V")
		.Where(L"V", L"Eq")
		.DefineInstanceForward(t_type(L"Vector"), L"Eq")
		;
	programMain
		.DefineFunction(L"main")
		.ReturnType(t_int())
		.Statement(
			s_expr(e_result().Assign(
				e_name(L"main1")(e_exps())+e_name(L"main2")(e_exps())
				))
			);
	programMain
		.DefineFunction(L"main1")
		.ReturnType(t_int())
		.Statement(
			s_var(t_type(L"Vector")[t_types()<<t_int()], L"v1")
			<<s_var(t_type(L"Vector")[t_types()<<t_int()], L"v2")
			<<s_expr(e_name(L"v1").Member(L"X").Assign(e_prim(0)))
			<<s_expr(e_name(L"v1").Member(L"Y").Assign(e_prim(1)))
			<<s_expr(e_name(L"v2").Member(L"X").Assign(e_prim(2)))
			<<s_expr(e_name(L"v2").Member(L"Y").Assign(e_prim(3)))
			<<s_if(e_name(L"Eq", t_type(L"Vector")[t_types()<<t_int()], L"Equals")(e_exps()<<e_name(L"v1")<<e_name(L"v2")),
				s_expr(e_result().Assign(e_prim(1))),
				s_expr(e_result().Assign(e_prim(2))))
			);
	programMain
		.DefineFunction(L"main2")
		.ReturnType(t_int())
		.Statement(
			s_var(t_type(L"Vector")[t_types()<<t_int()], L"v1")
			<<s_var(t_type(L"Vector")[t_types()<<t_int()], L"v2")
			<<s_var(t_bool()(t_types()<<t_type(L"Vector")[t_types()<<t_int()]<<t_type(L"Vector")[t_types()<<t_int()]), L"f", e_name(L"Eq", t_type(L"Vector")[t_types()<<t_int()], L"NotEquals"))
			<<s_expr(e_name(L"v1").Member(L"X").Assign(e_prim(0)))
			<<s_expr(e_name(L"v1").Member(L"Y").Assign(e_prim(1)))
			<<s_expr(e_name(L"v2").Member(L"X").Assign(e_prim(0)))
			<<s_expr(e_name(L"v2").Member(L"Y").Assign(e_prim(1)))
			<<s_if(e_name(L"f")(e_exps()<<e_name(L"v1")<<e_name(L"v2")),
				s_expr(e_result().Assign(e_prim(30))),
				s_expr(e_result().Assign(e_prim(40))))
			);
	Ptr<BasicIL> ilMain=Compile(programMain.GetInternalValue(), L"programMain", L"TestScripting_BasicLanguage_LinkingConcept_ProgramMain");

	BasicILInterpretor interpretor(65536);
	vint keyEq=interpretor.LoadIL(ilEq.Obj());
	vint keyVector=interpretor.LoadIL(ilVector.Obj());
	vint keyMain=interpretor.LoadIL(ilMain.Obj());
	
	BasicILStack stack(&interpretor);
	stack.Reset(0, keyEq, 0);
	TEST_ASSERT(stack.Run()==BasicILStack::Finished);
	TEST_ASSERT(stack.GetEnv()->StackTop()==stack.GetEnv()->StackSize());
	stack.Reset(0, keyVector, 0);
	TEST_ASSERT(stack.Run()==BasicILStack::Finished);
	TEST_ASSERT(stack.GetEnv()->StackTop()==stack.GetEnv()->StackSize());
	stack.Reset(0, keyMain, 0);
	TEST_ASSERT(stack.Run()==BasicILStack::Finished);
	TEST_ASSERT(stack.GetEnv()->StackTop()==stack.GetEnv()->StackSize());
	
	vint ins=ilMain->labels[0].instructionIndex;
	stack.Reset(ins, keyMain, sizeof(vint));
	TEST_ASSERT(stack.Run()==BasicILStack::Finished);
	TEST_ASSERT(stack.GetEnv()->Pop<vint>()==42);
	TEST_ASSERT(stack.GetEnv()->StackTop()==stack.GetEnv()->StackSize());

	LogInterpretor(interpretor, L"TestScripting_BasicLanguage_LinkingConcept_[Interpretor]");
}