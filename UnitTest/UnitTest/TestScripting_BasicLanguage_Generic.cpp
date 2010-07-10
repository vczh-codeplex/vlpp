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
		;
	RunBasicProgramInt(programMain.GetInternalValue(), 25, L"TestScripting_BasicLanguage_GenericFunction");
}