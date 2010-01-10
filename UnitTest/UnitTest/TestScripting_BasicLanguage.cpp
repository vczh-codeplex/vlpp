#include "..\..\Library\UnitTest\UnitTest.h"
#include "..\..\Library\Scripting\BasicLanguage\BasicLanguageTypeManager.h"
#include "..\..\Library\Scripting\BasicLanguage\BasicLanguageWriter.h"

using namespace vl;
using namespace vl::scripting::basiclanguage;

/***********************************************************************
GetTypeID algorithm function
***********************************************************************/

BEGIN_ALGORITHM_FUNCTION(GetTypeID, BasicType, int, int)

	ALGORITHM_FUNCTION_MATCH(BasicPrimitiveType)
	{
		return 0;
	}

	ALGORITHM_FUNCTION_MATCH(BasicPointerType)
	{
		return 1;
	}

	ALGORITHM_FUNCTION_MATCH(BasicArrayType)
	{
		return 2;
	}

	ALGORITHM_FUNCTION_MATCH(BasicReferenceType)
	{
		return 3;
	}

	ALGORITHM_FUNCTION_MATCH(BasicFunctionType)
	{
		return 4;
	}

END_ALGORITHM_FUNCTION

/***********************************************************************
TestTypeID algorithm procedure
***********************************************************************/

BEGIN_ALGORITHM_PROCEDURE(TestTypeID, BasicType, int*)

	ALGORITHM_PROCEDURE_MATCH(BasicPrimitiveType)
	{
		*argument=0;
	}

	ALGORITHM_PROCEDURE_MATCH(BasicPointerType)
	{
		*argument=1;
	}

	ALGORITHM_PROCEDURE_MATCH(BasicArrayType)
	{
		*argument=2;
	}

	ALGORITHM_PROCEDURE_MATCH(BasicReferenceType)
	{
		*argument=3;
	}

	ALGORITHM_PROCEDURE_MATCH(BasicFunctionType)
	{
		*argument=4;
	}

END_ALGORITHM_PROCEDURE

/***********************************************************************
Algorithms
***********************************************************************/

TEST_CASE(TestAlgorithmDeclaration)
{
	TEST_ASSERT(GetTypeID().Execute(t_int8().GetInternalValue(), 0)==0);
	TEST_ASSERT(GetTypeID().Execute((*t_int8()).GetInternalValue(), 0)==1);
	TEST_ASSERT(GetTypeID().Execute(t_int8()[10].GetInternalValue(), 0)==2);
	TEST_ASSERT(GetTypeID().Execute(t_type(L"vczh").GetInternalValue(), 0)==3);
	TEST_ASSERT(GetTypeID().Execute(t_int8()(t_types()<<t_int16()<<t_int32()).GetInternalValue(), 0)==4);

	int result=-1;
	TestTypeID().Execute(t_int8().GetInternalValue(), &result);
	TEST_ASSERT(result==0);
	TestTypeID().Execute((*t_int8()).GetInternalValue(), &result);
	TEST_ASSERT(result==1);
	TestTypeID().Execute(t_int8()[10].GetInternalValue(), &result);
	TEST_ASSERT(result==2);
	TestTypeID().Execute(t_type(L"vczh").GetInternalValue(), &result);
	TEST_ASSERT(result==3);
	TestTypeID().Execute(t_int8()(t_types()<<t_int16()<<t_int32()).GetInternalValue(), &result);
	TEST_ASSERT(result==4);
}