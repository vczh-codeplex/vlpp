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
extern void RunBasicProgramInt(Ptr<BasicProgram> program, int result, const WString& name);

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