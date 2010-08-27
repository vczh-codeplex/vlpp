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

/*

	programMain
		.DefineFunction(L"main")
		.ReturnType(t_int())
		.Statement(
			s_expr(e_result().Assign(e_prim(0)))
			);
*/

/***********************************************************************
Empty
***********************************************************************/

TEST_CASE(TestScripting_BasicLanguage_ExceptionEmpty)
{
	BasicProgramNode programMain;
	programMain
		.DefineFunction(L"main")
		.ReturnType(t_int())
		.Statement(
			s_try_catch(s_empty(), s_empty())
			<<s_expr(e_result().Assign(e_prim(0)))
			);
	RunBasicProgramInt(programMain.GetInternalValue(), 0, L"TestScripting_BasicLanguage_ExceptionEmpty");
}

/***********************************************************************
Simple Throw
***********************************************************************/

TEST_CASE(TestScripting_BasicLanguage_ExceptionSimpleThrow)
{
	BasicProgramNode programMain;
	programMain
		.DefineFunction(L"main")
		.ReturnType(t_int())
		.Statement(
			s_expr(e_result().Assign(e_prim(10)))
			<<s_try_catch(
				s_throw(),
				s_expr(e_result().Assign(e_prim(20)))
				)
			);
	RunBasicProgramInt(programMain.GetInternalValue(), 20, L"TestScripting_BasicLanguage_ExceptionSimpleThrow");
}