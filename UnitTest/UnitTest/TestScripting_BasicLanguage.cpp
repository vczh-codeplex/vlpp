#include "..\..\Library\UnitTest\UnitTest.h"
#include "..\..\Library\Function.h"
#include "..\..\Library\Scripting\BasicLanguage\BasicLanguageTypeManager.h"
#include "..\..\Library\Scripting\BasicLanguage\BasicLanguageWriter.h"
#include "..\..\Library\Scripting\BasicLanguage\BasicLanguageAnalyzer.h"

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

	ALGORITHM_FUNCTION_MATCH(BasicExtendedType)
	{
		return 5;
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

	ALGORITHM_PROCEDURE_MATCH(BasicExtendedType)
	{
		*argument=5;
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
	BasicTypeManager tm;

	TEST_ASSERT(global);
	TEST_ASSERT(global->PreviousScope()==0);
	TEST_ASSERT(global->OwnerDeclaration()==0);
	TEST_ASSERT(global->OwnerStatement()==0);
	BasicTypeRecord* link=tm.CreateStructureType();
	{
		List<WString> names;
		names.Add(L"data");
		names.Add(L"next");
		List<BasicTypeRecord*> types;
		types.Add(tm.GetPrimitiveType(int_type));
		types.Add(tm.GetPointerType(link));
		tm.UpdateStructureType(link, names.Wrap(), types.Wrap());
	}
	global->types.Add(L"Link", link);
	BasicFunctionDeclaration append;
	global->functions.Add(L"append", &append);
	TEST_ASSERT(global->types.Find(L"Link")==link);
	TEST_ASSERT(global->functions.Find(L"append")==&append);

	BasicScope* appendScope=env.CreateFunctionScope(global, &append);
	TEST_ASSERT(env.CreateFunctionScope(global, &append)==appendScope);
	TEST_ASSERT(env.CreateFunctionScope(appendScope, &append)==0);
	appendScope->variables.Add(L"link", link);
	appendScope->variables.Add(L"data", tm.GetPrimitiveType(int_type));
	TEST_ASSERT(appendScope->PreviousScope()==global);
	TEST_ASSERT(appendScope->OwnerDeclaration()==&append);
	TEST_ASSERT(appendScope->OwnerStatement()==0);
	TEST_ASSERT(appendScope->variables.Find(L"link")==link);
	TEST_ASSERT(appendScope->variables.Find(L"data")==tm.GetPrimitiveType(int_type));
	TEST_ASSERT(appendScope->types.Find(L"Link")==link);
	TEST_ASSERT(appendScope->functions.Find(L"append")==&append);
	TEST_ASSERT(appendScope->types.Find(L"unexists")==0);
	TEST_ASSERT(appendScope->variables.Find(L"unexists")==0);
	TEST_ASSERT(appendScope->functions.Find(L"unexists")==0);

	BasicCompositeStatement statement;
	BasicScope* statementScope=env.CreateStatementScope(appendScope, &statement);
	TEST_ASSERT(env.CreateStatementScope(appendScope, &statement)==statementScope);
	TEST_ASSERT(env.CreateStatementScope(statementScope, &statement)==0);
	statementScope->variables.Add(L"temp", link);
	TEST_ASSERT(statementScope->PreviousScope()==appendScope);
	TEST_ASSERT(statementScope->OwnerDeclaration()==&append);
	TEST_ASSERT(statementScope->OwnerStatement()==&statement);
	TEST_ASSERT(statementScope->variables.Find(L"link")==link);
	TEST_ASSERT(statementScope->variables.Find(L"data")==tm.GetPrimitiveType(int_type));
	TEST_ASSERT(statementScope->variables.Find(L"temp")==link);
	TEST_ASSERT(statementScope->types.Find(L"Link")==link);
	TEST_ASSERT(statementScope->functions.Find(L"append")==&append);
	TEST_ASSERT(statementScope->types.Find(L"unexists")==0);
	TEST_ASSERT(statementScope->variables.Find(L"unexists")==0);
	TEST_ASSERT(statementScope->functions.Find(L"unexists")==0);

	BasicScope* scope=0;
	TEST_ASSERT(statementScope->variables.Find(L"link", scope)==link);
	TEST_ASSERT(scope==appendScope);
	TEST_ASSERT(statementScope->variables.Find(L"data", scope)==tm.GetPrimitiveType(int_type));
	TEST_ASSERT(scope==appendScope);
	TEST_ASSERT(statementScope->variables.Find(L"temp", scope)==link);
	TEST_ASSERT(scope==statementScope);
	TEST_ASSERT(statementScope->types.Find(L"Link", scope)==link);
	TEST_ASSERT(scope==global);
	TEST_ASSERT(statementScope->functions.Find(L"append", scope)==&append);
	TEST_ASSERT(scope==global);
	TEST_ASSERT(statementScope->types.Find(L"unexists", scope)==0);
	TEST_ASSERT(scope==0);
	TEST_ASSERT(statementScope->variables.Find(L"unexists", scope)==0);
	TEST_ASSERT(scope==0);
	TEST_ASSERT(statementScope->functions.Find(L"unexists", scope)==0);
	TEST_ASSERT(scope==0);
}

/***********************************************************************
Exception Assertions
***********************************************************************/

void TestTypeNameNotExists(const WString& name, Ptr<BasicLanguageCodeException> e)
{
	TEST_ASSERT(e->GetExceptionCode()==BasicLanguageCodeException::TypeNameNotExists);
	BasicReferenceType* type=dynamic_cast<BasicReferenceType*>(e->GetBasicLanguageElement());
	TEST_ASSERT(type);
	TEST_ASSERT(type->name==name);
}

/***********************************************************************
BasicLanguage_GetTypeRecord
***********************************************************************/

TEST_CASE(Test_BasicLanguage_GetTypeRecord)
{
	BasicEnv env;
	BasicTypeManager tm;
	BasicScope* global=env.GlobalScope();
	List<Ptr<BasicLanguageCodeException>> errors;
	SortedList<WString> forwardStructures;
	BP argument(&env, global, &tm, errors, forwardStructures);
	
	BasicTypeRecord* link=tm.CreateStructureType();
	global->types.Add(L"Link", link);

	TEST_ASSERT(BasicLanguage_GetTypeRecord(t_void().GetInternalValue(), argument)==tm.GetPrimitiveType(void_type));
	TEST_ASSERT(BasicLanguage_GetTypeRecord(t_int().GetInternalValue(), argument)==tm.GetPrimitiveType(int_type));
	TEST_ASSERT(BasicLanguage_GetTypeRecord(t_type(L"Link").GetInternalValue(), argument)==link);
	TEST_ASSERT(BasicLanguage_GetTypeRecord((*t_type(L"Link")).GetInternalValue(), argument)==tm.GetPointerType(link));
	TEST_ASSERT(BasicLanguage_GetTypeRecord(t_int()[10].GetInternalValue(), argument)==tm.GetArrayType(tm.GetPrimitiveType(int_type), 10));

	List<BasicTypeRecord*> functionParameterTypes;
	functionParameterTypes.Add(tm.GetPointerType(link));
	functionParameterTypes.Add(tm.GetPointerType(link));
	TEST_ASSERT(BasicLanguage_GetTypeRecord(
		t_void()(t_types()<<*t_type(L"Link")<<*t_type(L"Link")).GetInternalValue(), argument)==
		tm.GetFunctionType(tm.GetPrimitiveType(void_type), functionParameterTypes.Wrap())
		);

	Ptr<BasicType> wrongType=(*t_type(L"Link"))(t_types()<<(*t_type(L"Link"))[10]<<(*t_type(L"Wrong"))[10]).GetInternalValue();
	TEST_EXCEPTION(
		BasicLanguage_GetTypeRecord(wrongType, argument),
		Ptr<BasicLanguageCodeException>,
		Curry(TestTypeNameNotExists)(L"Wrong")
		);
}

/***********************************************************************
BasicLanguage_BuildGlobalScope
***********************************************************************/

TEST_CASE(Test_BasicLanguage_BuildGlobalScope)
{
	BasicEnv env;
	BasicTypeManager tm;
	List<Ptr<BasicLanguageCodeException>> errors;
	SortedList<WString> forwardStructures;
	BP argument(&env, env.GlobalScope(), &tm, errors, forwardStructures);

	BasicProgramNode program;
	program.DefineStructureForward(L"Link");
	program.DefineRename(L"PLink", *t_type(L"Link"));
	program.DefineStructure(L"Link")
		.Member(L"data", t_int())
		.Member(L"prev", t_type(L"PLink"))
		.Member(L"next", t_type(L"PLink"));
	program.DefineVariable(L"head", t_type(L"PLink"));
	program.DefineVariable(L"tail", t_type(L"PLink"));
	program.DefineFunction(L"Create")
		.ReturnType(t_type(L"PLink"));
	program.DefineFunction(L"Destroy")
		.ReturnType(t_void())
		.Parameter(L"link", t_type(L"PLink"));
	program.DefineFunction(L"Append")
		.ReturnType(t_type(L"PLink"))
		.Parameter(L"link", t_type(L"PLink"))
		.Parameter(L"data", t_int());
	BasicLanguage_BuildGlobalScope(program.GetInternalValue(), argument);

	TEST_ASSERT(errors.Count()==0);
	TEST_ASSERT(forwardStructures.Count()==0);

	TEST_ASSERT(env.GlobalScope()->types.Items().Count()==2);
	BasicTypeRecord* typeLink=env.GlobalScope()->types.Items()[L"Link"];
	BasicTypeRecord* typePLink=env.GlobalScope()->types.Items()[L"PLink"];
	TEST_ASSERT(typeLink);
	TEST_ASSERT(typeLink->GetType()==BasicTypeRecord::Structure);
	TEST_ASSERT(typeLink->MemberCount()==3);
	TEST_ASSERT(typeLink->MemberName(0)==L"data");
	TEST_ASSERT(typeLink->MemberType(0)==tm.GetPrimitiveType(int_type));
	TEST_ASSERT(typeLink->MemberName(1)==L"prev");
	TEST_ASSERT(typeLink->MemberType(1)==typePLink);
	TEST_ASSERT(typeLink->MemberName(2)==L"next");
	TEST_ASSERT(typeLink->MemberType(2)==typePLink);
	TEST_ASSERT(typePLink);
	TEST_ASSERT(typePLink->GetType()==BasicTypeRecord::Pointer);
	TEST_ASSERT(typePLink->ElementType()==typeLink);

	TEST_ASSERT(env.GlobalScope()->variables.Items().Count()==2);
	TEST_ASSERT(env.GlobalScope()->variables.Items()[L"head"]==typePLink);
	TEST_ASSERT(env.GlobalScope()->variables.Items()[L"tail"]==typePLink);

	TEST_ASSERT(env.GlobalScope()->functions.Items().Count()==3);
	BasicFunctionDeclaration* functionCreate=env.GlobalScope()->functions.Items()[L"Create"];
	BasicFunctionDeclaration* functionDestroy=env.GlobalScope()->functions.Items()[L"Destroy"];
	BasicFunctionDeclaration* functionAppend=env.GlobalScope()->functions.Items()[L"Append"];
	BasicTypeRecord* typeCreate=env.GetFunctionType(functionCreate);
	BasicTypeRecord* typeDestroy=env.GetFunctionType(functionDestroy);
	BasicTypeRecord* typeAppend=env.GetFunctionType(functionAppend);

	TEST_ASSERT(functionCreate->parameterNames.Count()==0);
	TEST_ASSERT(typeCreate->GetType()==BasicTypeRecord::Function);
	TEST_ASSERT(typeCreate->ReturnType()==typePLink);
	TEST_ASSERT(typeCreate->ParameterCount()==0);

	TEST_ASSERT(functionDestroy->parameterNames.Count()==1);
	TEST_ASSERT(functionDestroy->parameterNames[0]==L"link");
	TEST_ASSERT(typeDestroy->GetType()==BasicTypeRecord::Function);
	TEST_ASSERT(typeDestroy->ReturnType()==tm.GetPrimitiveType(void_type));
	TEST_ASSERT(typeDestroy->ParameterCount()==1);
	TEST_ASSERT(typeDestroy->ParameterType(0)==typePLink);

	TEST_ASSERT(functionAppend->parameterNames.Count()==2);
	TEST_ASSERT(functionAppend->parameterNames[0]==L"link");
	TEST_ASSERT(functionAppend->parameterNames[1]==L"data");
	TEST_ASSERT(typeAppend->GetType()==BasicTypeRecord::Function);
	TEST_ASSERT(typeCreate->ReturnType()==typePLink);
	TEST_ASSERT(typeAppend->ParameterCount()==2);
	TEST_ASSERT(typeAppend->ParameterType(0)==typePLink);
	TEST_ASSERT(typeAppend->ParameterType(1)==tm.GetPrimitiveType(int_type));
}

/***********************************************************************
BasicLanguage_GetExpressionType
***********************************************************************/

TEST_CASE(Test_BasicLanguage_GetExpressionType_BasicReferenceExpression)
{
	BasicEnv env;
	BasicTypeManager tm;
	List<Ptr<BasicLanguageCodeException>> errors;
	SortedList<WString> forwardStructures;
	BasicScope* globalScope=env.GlobalScope();
	BP argument(&env, globalScope, &tm, errors, forwardStructures);

	BasicProgramNode program;
	program.DefineFunction(L"a");
	program.DefineFunction(L"b");
	program.DefineVariable(L"c", t_int());
	BasicLanguage_BuildGlobalScope(program.GetInternalValue(), argument);
	BasicScope* functionScope=env.CreateFunctionScope(globalScope, dynamic_cast<BasicFunctionDeclaration*>(program.GetInternalValue()->declarations[0].Obj()));
	functionScope->variables.Add(L"b", tm.GetPrimitiveType(uint_type));
	functionScope->variables.Add(L"c", tm.GetPrimitiveType(bool_type));

	Ptr<BasicExpression> aExpr=e_name(L"a").GetInternalValue();
	Ptr<BasicExpression> bExpr=e_name(L"b").GetInternalValue();
	Ptr<BasicExpression> cExpr=e_name(L"c").GetInternalValue();

	BP argumentExpression(argument, functionScope);
	TEST_ASSERT(BasicLanguage_GetExpressionType(aExpr, argumentExpression)==BasicLanguage_GetTypeRecord(t_void()(t_types()).GetInternalValue(), argumentExpression));
	TEST_ASSERT(BasicLanguage_GetExpressionType(bExpr, argumentExpression)==BasicLanguage_GetTypeRecord(t_uint().GetInternalValue(), argumentExpression));
	TEST_ASSERT(BasicLanguage_GetExpressionType(cExpr, argumentExpression)==BasicLanguage_GetTypeRecord(t_bool().GetInternalValue(), argumentExpression));
}