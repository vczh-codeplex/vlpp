#include "..\..\Library\UnitTest\UnitTest.h"
#include "..\..\Library\Scripting\BasicLanguage\BasicLanguageTypeManager.h"
#include "..\..\Library\Scripting\BasicLanguage\BasicLanguageWriter.h"
#include "..\..\Library\Scripting\BasicLanguage\BasicLanguageCodeGenerator.h"

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
Algorithm Macros
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
Code Generator Components
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

TEST_CASE(TestBasicEnv)
{
	BasicEnv env;
	BasicScope* global=env.GlobalScope();
	TEST_ASSERT(global);
	TEST_ASSERT(global->PreviousScope()==0);
	TEST_ASSERT(global->TypeManager()==&env.TypeManager());
	TEST_ASSERT(global->OwnerDeclaration()==0);
	TEST_ASSERT(global->OwnerStatement()==0);
	BasicTypeRecord* link=env.TypeManager().CreateStructureType();
	{
		List<WString> names;
		names.Add(L"data");
		names.Add(L"next");
		List<BasicTypeRecord*> types;
		types.Add(env.TypeManager().GetPrimitiveType(int_type));
		types.Add(env.TypeManager().GetPointerType(link));
		env.TypeManager().UpdateStructureType(link, names.Wrap(), types.Wrap());
	}
	global->Types().Add(L"Link", link);
	BasicFunctionDeclaration append;
	global->Functions().Add(L"append", &append);
	TEST_ASSERT(global->GetType(L"Link")==link);
	TEST_ASSERT(global->GetFunction(L"append")==&append);

	BasicScope* appendScope=env.CreateFunctionScope(global, &append);
	TEST_ASSERT(env.CreateFunctionScope(global, &append)==appendScope);
	TEST_ASSERT(env.CreateFunctionScope(appendScope, &append)==0);
	appendScope->Variables().Add(L"link", link);
	appendScope->Variables().Add(L"data", env.TypeManager().GetPrimitiveType(int_type));
	TEST_ASSERT(appendScope->PreviousScope()==global);
	TEST_ASSERT(appendScope->TypeManager()==&env.TypeManager());
	TEST_ASSERT(appendScope->OwnerDeclaration()==&append);
	TEST_ASSERT(appendScope->OwnerStatement()==0);
	TEST_ASSERT(appendScope->GetVariableType(L"link")==link);
	TEST_ASSERT(appendScope->GetVariableType(L"data")==env.TypeManager().GetPrimitiveType(int_type));
	TEST_ASSERT(appendScope->GetType(L"Link")==link);
	TEST_ASSERT(appendScope->GetFunction(L"append")==&append);
	TEST_ASSERT(appendScope->GetType(L"unexists")==0);
	TEST_ASSERT(appendScope->GetVariableType(L"unexists")==0);
	TEST_ASSERT(appendScope->GetFunction(L"unexists")==0);

	BasicCompositeStatement statement;
	BasicScope* statementScope=env.CreateStatementScope(appendScope, &statement);
	TEST_ASSERT(env.CreateStatementScope(appendScope, &statement)==statementScope);
	TEST_ASSERT(env.CreateStatementScope(statementScope, &statement)==0);
	statementScope->Variables().Add(L"temp", link);
	TEST_ASSERT(statementScope->PreviousScope()==appendScope);
	TEST_ASSERT(statementScope->TypeManager()==&env.TypeManager());
	TEST_ASSERT(statementScope->OwnerDeclaration()==&append);
	TEST_ASSERT(statementScope->OwnerStatement()==&statement);
	TEST_ASSERT(statementScope->GetVariableType(L"link")==link);
	TEST_ASSERT(statementScope->GetVariableType(L"data")==env.TypeManager().GetPrimitiveType(int_type));
	TEST_ASSERT(statementScope->GetVariableType(L"temp")==link);
	TEST_ASSERT(statementScope->GetType(L"Link")==link);
	TEST_ASSERT(statementScope->GetFunction(L"append")==&append);
	TEST_ASSERT(statementScope->GetType(L"unexists")==0);
	TEST_ASSERT(statementScope->GetVariableType(L"unexists")==0);
	TEST_ASSERT(statementScope->GetFunction(L"unexists")==0);

	BasicScope* scope=0;
	TEST_ASSERT(statementScope->GetVariableType(L"link", scope)==link);
	TEST_ASSERT(scope==appendScope);
	TEST_ASSERT(statementScope->GetVariableType(L"data", scope)==env.TypeManager().GetPrimitiveType(int_type));
	TEST_ASSERT(scope==appendScope);
	TEST_ASSERT(statementScope->GetVariableType(L"temp", scope)==link);
	TEST_ASSERT(scope==statementScope);
	TEST_ASSERT(statementScope->GetType(L"Link", scope)==link);
	TEST_ASSERT(scope==global);
	TEST_ASSERT(statementScope->GetFunction(L"append", scope)==&append);
	TEST_ASSERT(scope==global);
	TEST_ASSERT(statementScope->GetType(L"unexists", scope)==0);
	TEST_ASSERT(scope==0);
	TEST_ASSERT(statementScope->GetVariableType(L"unexists", scope)==0);
	TEST_ASSERT(scope==0);
	TEST_ASSERT(statementScope->GetFunction(L"unexists", scope)==0);
	TEST_ASSERT(scope==0);
}