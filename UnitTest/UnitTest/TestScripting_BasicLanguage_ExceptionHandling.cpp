#include "UnitTestCompilingHelper.h"

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
			<<s_expr(e_result().Assign(e_prim((vint)0)))
			);
	RunBasicProgram<vint>(programMain.GetInternalValue(), 0, L"TestScripting_BasicLanguage_ExceptionEmpty");
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
			s_expr(e_result().Assign(e_prim((vint)10)))
			<<s_try_catch(
				s_throw(),
				s_expr(e_result().Assign(e_prim((vint)20)))
				)
			);
	RunBasicProgram<vint>(programMain.GetInternalValue(), 20, L"TestScripting_BasicLanguage_ExceptionSimpleThrow");
}

/***********************************************************************
Throw Integer
***********************************************************************/

TEST_CASE(TestScripting_BasicLanguage_ExceptionThrowInteger)
{
	BasicProgramNode programMain;
	programMain
		.DefineFunction(L"main")
		.ReturnType(t_int())
		.Statement(
			s_expr(e_result().Assign(e_prim((vint)10)))
			<<s_try_catch(
				s_throw(e_prim((vint)20)),
				s_expr(e_result().Assign(*e_exception()[*t_int()]))
				)
			);
	RunBasicProgram<vint>(programMain.GetInternalValue(), 20, L"TestScripting_BasicLanguage_ExceptionThrowInteger");
}

/***********************************************************************
Throw In Function
***********************************************************************/

TEST_CASE(TestScripting_BasicLanguage_ExceptionThrowInFunction)
{
	BasicProgramNode programMain;
	programMain
		.DefineFunction(L"main")
		.ReturnType(t_int())
		.Statement(
			s_expr(e_result().Assign(e_prim((vint)10)))
			<<s_try_catch(
				s_expr(e_name(L"throw")(e_exps())),
				s_expr(e_result().Assign(*e_exception()[*t_int()]))
				)
			);
	programMain
		.DefineFunction(L"throw")
		.Statement(
			s_throw(e_prim((vint)20))
			);
	RunBasicProgram<vint>(programMain.GetInternalValue(), 20, L"TestScripting_BasicLanguage_ExceptionThrowInFunction");
}

/***********************************************************************
Double Try
***********************************************************************/

TEST_CASE(TestScripting_BasicLanguage_ExceptionDoubleTry)
{
	BasicProgramNode programMain;
	programMain
		.DefineFunction(L"main")
		.ReturnType(t_int())
		.Statement(
			s_expr(e_result().Assign(e_prim((vint)10)))
			<<s_try_catch(
				s_try_catch(
					s_throw(e_prim((vint)20)),
					s_throw()
					),
				s_expr(e_result().Assign(*e_exception()[*t_int()]))
				)
			);
	RunBasicProgram<vint>(programMain.GetInternalValue(), 20, L"TestScripting_BasicLanguage_ExceptionDoubleTry");
}