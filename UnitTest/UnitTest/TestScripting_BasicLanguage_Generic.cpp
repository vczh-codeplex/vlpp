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

//from TestScripting_BasicLanguage_Codegen.cpp
extern void RunBasicProgramInt(Ptr<BasicProgram> program, vint result, const WString& name);
//from TestScripting_BasicLanguage_Codegen.cpp
extern void LogInterpretor(BasicILInterpretor& interpretor, const WString& fileName);

/***********************************************************************
Generic Structure
***********************************************************************/

TEST_CASE(TestScripting_BasicLanguage_GenericStructure)
{
	BasicProgramNode programMain;
	programMain
		.Generic().GenericArgument(L"T")
		.DefineStructure(L"Vector")
		.Member(L"x", t_type(L"T"))
		.Member(L"y", t_type(L"T"));
	programMain
		.DefineFunction(L"main")
		.ReturnType(t_int())
		.Statement(
			s_var(t_type(L"Vector")[t_types()<<t_int()], L"v")
			<<s_expr(e_name(L"v").Member(L"x").Assign(e_prim(10)))
			<<s_expr(e_name(L"v").Member(L"y").Assign(e_prim(20)))
			<<s_expr(e_result().Assign(e_name(L"v").Member(L"x")+e_name(L"v").Member(L"y")))
			);
	RunBasicProgramInt(programMain.GetInternalValue(), 30, L"TestScripting_BasicLanguage_GenericStructure");
}

/***********************************************************************
Generic Type Rename
***********************************************************************/

TEST_CASE(TestScripting_BasicLanguage_GenericTypeRename)
{
	BasicProgramNode programMain;
	programMain
		.Generic().GenericArgument(L"T")
		.DefineRename(L"Unit", t_type(L"T"));
	programMain
		.Generic().GenericArgument(L"T")
		.DefineStructure(L"Vector")
		.Member(L"x", t_type(L"Unit")[t_types()<<t_type(L"T")])
		.Member(L"y", t_type(L"Unit")[t_types()<<t_type(L"T")]);
	programMain
		.DefineFunction(L"main")
		.ReturnType(t_type(L"Unit")[t_types()<<t_int()])
		.Statement(
			s_var(t_type(L"Unit")[t_types()<<t_type(L"Vector")[t_types()<<t_int()]], L"v")
			<<s_expr(e_name(L"v").Member(L"x").Assign(e_prim(10)))
			<<s_expr(e_name(L"v").Member(L"y").Assign(e_prim(20)))
			<<s_expr(e_result().Assign(e_name(L"v").Member(L"x")+e_name(L"v").Member(L"y")))
			);
	RunBasicProgramInt(programMain.GetInternalValue(), 30, L"TestScripting_BasicLanguage_GenericTypeRename");
}

/***********************************************************************
Generic Function
***********************************************************************/

TEST_CASE(TestScripting_BasicLanguage_GenericFunction)
{
	BasicProgramNode programMain;
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
		.DefineFunction(L"Apply2")
		.Parameter(L"f", t_type(L"T")(t_types()<<t_type(L"T")<<t_type(L"T")))
		.Parameter(L"a", t_type(L"T"))
		.Parameter(L"b", t_type(L"T"))
		.ReturnType(t_type(L"T"))
		.Statement(
			s_expr(e_result().Assign(e_name(L"f")(e_exps()<<e_name(L"a")<<e_name(L"b"))))
			);
	programMain
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
	RunBasicProgramInt(programMain.GetInternalValue(), 25, L"TestScripting_BasicLanguage_GenericFunction");
}

/***********************************************************************
Generic Variable
***********************************************************************/

TEST_CASE(TestScripting_BasicLanguage_GenericVariable)
{
	BasicProgramNode programMain;
	programMain
		.Generic().GenericArgument(L"T")
		.DefineStructure(L"Storage")
		.Member(L"data", t_type(L"T"))
		;
	programMain
		.Generic().GenericArgument(L"U")
		.DefineVariable(L"storage", t_type(L"Storage")[t_types()<<t_type(L"U")])
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
		.Generic().GenericArgument(L"V")
		.DefineFunction(L"Get")
		.ReturnType(t_type(L"V"))
		.Statement(
			s_expr(e_result().Assign(e_name(L"storage", t_types()<<t_type(L"V")).Member(L"data")))
			);
	programMain
		.Generic().GenericArgument(L"W")
		.DefineFunction(L"Set")
		.Parameter(L"value", t_type(L"W"))
		.Statement(
			s_expr(e_name(L"storage", t_types()<<t_type(L"W")).Member(L"data").Assign(e_name(L"value")))
			);
	RunBasicProgramInt(programMain.GetInternalValue(), 30, L"TestScripting_BasicLanguage_GenericVariable");
}

/***********************************************************************
Generic Concept
***********************************************************************/

TEST_CASE(TestScripting_BasicLanguage_GenericConcept)
{
	BasicProgramNode programMain;
	programMain
		.DefineFunction(L"main")
		.ReturnType(t_int())
		.Statement(
			s_expr(e_result().Assign(e_prim(0)))
			);
	programMain
		.DefineConcept(L"Eq", L"T")
		.Member(L"Equals", t_bool()(t_types()<<t_type(L"T")<<t_type(L"T")))
		.Member(L"NotEquals", t_bool()(t_types()<<t_type(L"T")<<t_type(L"T")))
		;

	programMain
		.DefineInstance(t_int(), L"Eq")
		.Member(L"Equals", L"IntEquals")
		.Member(L"NotEquals", L"IntNotEquals")
		;
	programMain
		.DefineFunction(L"IntEquals")
		.ReturnType(t_bool())
		.Parameter(L"a", t_int())
		.Parameter(L"b", t_int())
		.Statement(
			s_expr(e_result().Assign(e_name(L"a")==e_name(L"b")))
			);
	programMain
		.DefineFunction(L"IntNotEquals")
		.ReturnType(t_bool())
		.Parameter(L"a", t_int())
		.Parameter(L"b", t_int())
		.Statement(
			s_expr(e_result().Assign(e_name(L"a")!=e_name(L"b")))
			);

	programMain
		.Generic().GenericArgument(L"U")
		.DefineStructure(L"Vector")
		.Member(L"X", t_type(L"U"))
		.Member(L"Y", t_type(L"U"))
		;
	programMain
		.Generic().GenericArgument(L"V")
		.DefineInstance(t_type(L"Vector"), L"Eq")
		.Member(L"Equals", L"VectorEquals", t_types()<<t_type(L"V"))
		.Member(L"NotEquals", L"VectorNotEquals", t_types()<<t_type(L"V"))
		;
	programMain
		.Generic().GenericArgument(L"W")
		.DefineFunction(L"VectorEquals")
		.ReturnType(t_bool())
		.Parameter(L"a", t_type(L"Vector")[t_types()<<t_type(L"W")])
		.Parameter(L"b", t_type(L"Vector")[t_types()<<t_type(L"W")])
		.Statement(
			s_empty()
			);
	programMain
		.Generic().GenericArgument(L"W")
		.DefineFunction(L"VectorNotEquals")
		.ReturnType(t_bool())
		.Parameter(L"a", t_type(L"Vector")[t_types()<<t_type(L"W")])
		.Parameter(L"b", t_type(L"Vector")[t_types()<<t_type(L"W")])
		.Statement(
			s_empty()
			);

	RunBasicProgramInt(programMain.GetInternalValue(), 0, L"TestScripting_BasicLanguage_GenericConcept");
}