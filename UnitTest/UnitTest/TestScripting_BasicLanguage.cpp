#include "..\..\Library\UnitTest\UnitTest.h"
#include "..\..\Library\Scripting\BasicLanguage\BasicLanguageTypeManager.h"
#include "..\..\Library\Scripting\BasicLanguage\BasicLanguageWriter.h"

using namespace vl;
using namespace vl::collections;
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

END_ALGORITHM_FUNCTION(GetTypeID)

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

END_ALGORITHM_PROCEDURE(TestTypeID)

/***********************************************************************
Algorithms
***********************************************************************/

TEST_CASE(TestAlgorithmDeclaration)
{
	TEST_ASSERT(GetTypeID(t_int8().GetInternalValue(), 0)==0);
	TEST_ASSERT(GetTypeID((*t_int8()).GetInternalValue(), 0)==1);
	TEST_ASSERT(GetTypeID(t_int8()[10].GetInternalValue(), 0)==2);
	TEST_ASSERT(GetTypeID(t_type(L"vczh").GetInternalValue(), 0)==3);
	TEST_ASSERT(GetTypeID(t_int8()(t_types()<<t_int16()<<t_int32()).GetInternalValue(), 0)==4);

	int result=-1;
	TestTypeID(t_int8().GetInternalValue(), &result);
	TEST_ASSERT(result==0);
	TestTypeID((*t_int8()).GetInternalValue(), &result);
	TEST_ASSERT(result==1);
	TestTypeID(t_int8()[10].GetInternalValue(), &result);
	TEST_ASSERT(result==2);
	TestTypeID(t_type(L"vczh").GetInternalValue(), &result);
	TEST_ASSERT(result==3);
	TestTypeID(t_int8()(t_types()<<t_int16()<<t_int32()).GetInternalValue(), &result);
	TEST_ASSERT(result==4);
}

/***********************************************************************
Type Manager
***********************************************************************/

TEST_CASE(TestTypeManager)
{
	BasicTypeManager manager;

	BasicTypeRecord* primitiveInt=manager.GetPrimitiveType(int_type);
	TEST_ASSERT(primitiveInt->GetType()==BasicTypeRecord::Primitive);
	TEST_ASSERT(primitiveInt->PrimitiveType()==int_type);
	BasicTypeRecord* primitiveVoid=manager.GetPrimitiveType(void_type);
	TEST_ASSERT(primitiveVoid->GetType()==BasicTypeRecord::Primitive);
	TEST_ASSERT(primitiveVoid->PrimitiveType()==void_type);
	TEST_ASSERT(primitiveInt==manager.GetPrimitiveType(int_type));
	TEST_ASSERT(primitiveVoid==manager.GetPrimitiveType(void_type));

	BasicTypeRecord* intptr=manager.GetPointerType(primitiveInt);
	TEST_ASSERT(intptr->GetType()==BasicTypeRecord::Pointer);
	TEST_ASSERT(intptr->ElementType()==primitiveInt);
	BasicTypeRecord* voidptr=manager.GetPointerType(primitiveVoid);
	TEST_ASSERT(voidptr->GetType()==BasicTypeRecord::Pointer);
	TEST_ASSERT(voidptr->ElementType()==primitiveVoid);
	TEST_ASSERT(intptr==manager.GetPointerType(primitiveInt));
	TEST_ASSERT(voidptr==manager.GetPointerType(primitiveVoid));

	BasicTypeRecord* intarray1=manager.GetArrayType(primitiveInt, 100);
	TEST_ASSERT(intarray1->GetType()==BasicTypeRecord::Array);
	TEST_ASSERT(intarray1->ElementType()==primitiveInt);
	TEST_ASSERT(intarray1->ElementCount()==100);
	BasicTypeRecord* intarray2=manager.GetArrayType(primitiveInt, 200);
	TEST_ASSERT(intarray2->GetType()==BasicTypeRecord::Array);
	TEST_ASSERT(intarray2->ElementType()==primitiveInt);
	TEST_ASSERT(intarray2->ElementCount()==200);
	TEST_ASSERT(intarray1==manager.GetArrayType(primitiveInt, 100));
	TEST_ASSERT(intarray2==manager.GetArrayType(primitiveInt, 200));

	List<BasicTypeRecord*> parameters;
	parameters.Add(manager.GetPointerType(intptr));
	parameters.Add(manager.GetArrayType(voidptr, 10));
	BasicTypeRecord* function=manager.GetFunctionType(primitiveVoid, parameters.Wrap());
	TEST_ASSERT(function->GetType()==BasicTypeRecord::Function);
	TEST_ASSERT(function->ReturnType()==primitiveVoid);
	TEST_ASSERT(function->ParameterCount()==2);
	TEST_ASSERT(function->ParameterType(0)==manager.GetPointerType(intptr));
	TEST_ASSERT(function->ParameterType(1)==manager.GetArrayType(voidptr, 10));
	TEST_ASSERT(function==manager.GetFunctionType(primitiveVoid, parameters.Wrap()));
	TEST_ASSERT(function!=manager.GetFunctionType(primitiveInt, parameters.Wrap()));
	parameters.Add(primitiveInt);
	TEST_ASSERT(function!=manager.GetFunctionType(primitiveVoid, parameters.Wrap()));
	parameters.RemoveAt(0);
	TEST_ASSERT(function!=manager.GetFunctionType(primitiveVoid, parameters.Wrap()));

	BasicTypeRecord* structure=manager.CreateStructureType();
	List<WString> names;
	names.Add(L"data");
	names.Add(L"next");
	List<BasicTypeRecord*> types;
	types.Add(manager.GetPrimitiveType(int_type));
	types.Add(manager.GetPointerType(structure));
	manager.UpdateStructureType(structure, names.Wrap(), types.Wrap());
	TEST_ASSERT(structure->GetType()==BasicTypeRecord::Structure);
	TEST_ASSERT(structure->MemberCount()==2);
	TEST_ASSERT(structure->MemberName(0)==L"data");
	TEST_ASSERT(structure->MemberName(1)==L"next");
	TEST_ASSERT(structure->MemberType(0)==primitiveInt);
	TEST_ASSERT(structure->MemberType(L"data")==primitiveInt);
	TEST_ASSERT(structure->MemberType(1)==manager.GetPointerType(structure));
	TEST_ASSERT(structure->MemberType(L"next")==manager.GetPointerType(structure));
}