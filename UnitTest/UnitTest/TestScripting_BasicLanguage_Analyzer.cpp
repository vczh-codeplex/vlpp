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

BEGIN_ALGORITHM_FUNCTION(GetTypeID, BasicType, vint, vint)

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

	ALGORITHM_FUNCTION_MATCH(BasicInstantiatedGenericType)
	{
		return 5;
	}

	ALGORITHM_FUNCTION_MATCH(BasicTypeofExpressionType)
	{
		return 6;
	}

	ALGORITHM_FUNCTION_MATCH(BasicTypeofMemberType)
	{
		return 7;
	}

END_ALGORITHM_FUNCTION(GetTypeID)

/***********************************************************************
TestTypeID algorithm procedure
***********************************************************************/

BEGIN_ALGORITHM_PROCEDURE(TestTypeID, BasicType, vint*)

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

	ALGORITHM_PROCEDURE_MATCH(BasicInstantiatedGenericType)
	{
		*argument=5;
	}

	ALGORITHM_PROCEDURE_MATCH(BasicTypeofExpressionType)
	{
		*argument=6;
	}

	ALGORITHM_PROCEDURE_MATCH(BasicTypeofMemberType)
	{
		*argument=7;
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

	vint result=-1;
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

	BasicTypeRecord* structure=manager.CreateStructureType(0);
	List<WString> names;
	names.Add(L"data");
	names.Add(L"next");
	List<BasicTypeRecord*> types;
	types.Add(manager.GetPrimitiveType(int_type));
	types.Add(manager.GetPointerType(structure));
	manager.UpdateStructureType(0, structure, names.Wrap(), types.Wrap());
	TEST_ASSERT(structure->GetType()==BasicTypeRecord::Structure);
	TEST_ASSERT(structure->MemberCount()==2);
	TEST_ASSERT(structure->MemberName(0)==L"data");
	TEST_ASSERT(structure->MemberName(1)==L"next");
	TEST_ASSERT(structure->MemberType(0)==primitiveInt);
	TEST_ASSERT(structure->MemberType(L"data")==primitiveInt);
	TEST_ASSERT(structure->MemberType(1)==manager.GetPointerType(structure));
	TEST_ASSERT(structure->MemberType(L"next")==manager.GetPointerType(structure));
}

TEST_CASE(TestTypeManagerWithGeneric)
{
	BasicTypeManager manager;
	Dictionary<BasicTypeRecord*, BasicTypeRecord*> instanciatingArguments;

	BasicTypeRecord* genericArgument=manager.GetGenericArgumentType(L"T");
	TEST_ASSERT(genericArgument->GetType()==BasicTypeRecord::GenericArgument);
	TEST_ASSERT(genericArgument->ArgumentName()==L"T");
	TEST_ASSERT(genericArgument==manager.GetGenericArgumentType(L"T"));
	TEST_ASSERT(genericArgument!=manager.GetGenericArgumentType(L"U"));

	BasicTypeRecord* genericType=manager.CreateGenericType(0);
	BasicTypeRecord* genericLink=manager.CreateStructureType(0);
	List<BasicTypeRecord*> genericArguments;
	genericArguments.Add(manager.GetGenericArgumentType(L"T"));
	manager.UpdateGenericType(genericType, genericLink, genericArguments.Wrap());

	instanciatingArguments.Add(manager.GetGenericArgumentType(L"T"), manager.GetGenericArgumentType(L"T"));
	List<WString> genericLinkNames;
	genericLinkNames.Add(L"data");
	genericLinkNames.Add(L"next");
	List<BasicTypeRecord*> genericLinkTypes;
	genericLinkTypes.Add(manager.GetGenericArgumentType(L"T"));
	genericLinkTypes.Add(manager.GetPointerType(manager.Instanciate(genericType, instanciatingArguments.Wrap())));
	manager.UpdateStructureType(0, genericLink, genericLinkNames.Wrap(), genericLinkTypes.Wrap());

	TEST_ASSERT(genericType->GetType()==BasicTypeRecord::Generic);
	TEST_ASSERT(genericType->ParameterCount()==1);
	TEST_ASSERT(genericType->ParameterType(0)==manager.GetGenericArgumentType(L"T"));
	TEST_ASSERT(genericType->ElementType()==genericLink);

	instanciatingArguments.Clear();
	instanciatingArguments.Add(manager.GetGenericArgumentType(L"T"), manager.GetPrimitiveType(int_type));
	BasicTypeRecord* intLink=manager.Instanciate(genericLink, instanciatingArguments.Wrap());
	TEST_ASSERT(intLink->GetType()==BasicTypeRecord::Structure);
	TEST_ASSERT(intLink->MemberCount()==2);
	TEST_ASSERT(intLink->MemberName(0)==L"data");
	TEST_ASSERT(intLink->MemberName(1)==L"next");
	TEST_ASSERT(intLink->MemberType(0)==manager.GetPrimitiveType(int_type));
	TEST_ASSERT(intLink->MemberType(L"data")==manager.GetPrimitiveType(int_type));
	TEST_ASSERT(intLink->MemberType(1)==manager.GetPointerType(intLink));
	TEST_ASSERT(intLink->MemberType(L"next")==manager.GetPointerType(intLink));
	
	instanciatingArguments.Clear();
	instanciatingArguments.Add(manager.GetGenericArgumentType(L"T"), manager.GetGenericArgumentType(L"U"));
	BasicTypeRecord* uLink=manager.Instanciate(genericLink, instanciatingArguments.Wrap());
	TEST_ASSERT(uLink->GetType()==BasicTypeRecord::Structure);
	TEST_ASSERT(uLink->MemberCount()==2);
	TEST_ASSERT(uLink->MemberName(0)==L"data");
	TEST_ASSERT(uLink->MemberName(1)==L"next");
	TEST_ASSERT(uLink->MemberType(0)==manager.GetGenericArgumentType(L"U"));
	TEST_ASSERT(uLink->MemberType(L"data")==manager.GetGenericArgumentType(L"U"));
	TEST_ASSERT(uLink->MemberType(1)==manager.GetPointerType(uLink));
	TEST_ASSERT(uLink->MemberType(L"next")==manager.GetPointerType(uLink));

	instanciatingArguments.Clear();
	instanciatingArguments.Add(manager.GetGenericArgumentType(L"T"), manager.GetPrimitiveType(int_type));
	TEST_ASSERT(intLink==manager.Instanciate(genericLink, instanciatingArguments.Wrap()));

	instanciatingArguments.Clear();
	instanciatingArguments.Add(manager.GetGenericArgumentType(L"U"), manager.GetPrimitiveType(int_type));
	TEST_ASSERT(intLink==manager.Instanciate(uLink, instanciatingArguments.Wrap()));
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
	BasicTypeRecord* link=tm.CreateStructureType(0);
	{
		List<WString> names;
		names.Add(L"data");
		names.Add(L"next");
		List<BasicTypeRecord*> types;
		types.Add(tm.GetPrimitiveType(int_type));
		types.Add(tm.GetPointerType(link));
		tm.UpdateStructureType(0, link, names.Wrap(), types.Wrap());
	}
	global->types.Add(L"Link", link);
	BasicFunctionDeclaration append;
	global->functions.Add(L"append", &append);
	TEST_ASSERT(global->types.Find(L"Link")==link);
	TEST_ASSERT(global->functions.Find(L"append")==&append);

	BasicScope* appendScope=env.CreateFunctionScope(global, &append);
	TEST_ASSERT(env.CreateFunctionScope(global, &append)==appendScope);
	TEST_ASSERT(env.CreateFunctionScope(appendScope, &append)==0);
	appendScope->variables.Add(L"link", BasicScope::Variable((vint)0, link));
	appendScope->variables.Add(L"data", BasicScope::Variable((vint)0, tm.GetPrimitiveType(int_type)));
	TEST_ASSERT(appendScope->PreviousScope()==global);
	TEST_ASSERT(appendScope->OwnerDeclaration()==&append);
	TEST_ASSERT(appendScope->OwnerStatement()==0);
	TEST_ASSERT(appendScope->variables.Find(L"link").type==link);
	TEST_ASSERT(appendScope->variables.Find(L"data").type==tm.GetPrimitiveType(int_type));
	TEST_ASSERT(appendScope->types.Find(L"Link")==link);
	TEST_ASSERT(appendScope->functions.Find(L"append")==&append);
	TEST_ASSERT(appendScope->types.Find(L"unexists")==0);
	TEST_ASSERT(appendScope->variables.Find(L"unexists")==false);
	TEST_ASSERT(appendScope->functions.Find(L"unexists")==0);

	BasicCompositeStatement statement;
	BasicScope* statementScope=env.CreateStatementScope(appendScope, &statement);
	TEST_ASSERT(env.CreateStatementScope(appendScope, &statement)==statementScope);
	TEST_ASSERT(env.CreateStatementScope(statementScope, &statement)==0);
	statementScope->variables.Add(L"temp", BasicScope::Variable((vint)0, link));
	TEST_ASSERT(statementScope->PreviousScope()==appendScope);
	TEST_ASSERT(statementScope->OwnerDeclaration()==&append);
	TEST_ASSERT(statementScope->OwnerStatement()==&statement);
	TEST_ASSERT(statementScope->variables.Find(L"link").type==link);
	TEST_ASSERT(statementScope->variables.Find(L"data").type==tm.GetPrimitiveType(int_type));
	TEST_ASSERT(statementScope->variables.Find(L"temp").type==link);
	TEST_ASSERT(statementScope->types.Find(L"Link")==link);
	TEST_ASSERT(statementScope->functions.Find(L"append")==&append);
	TEST_ASSERT(statementScope->types.Find(L"unexists")==0);
	TEST_ASSERT(statementScope->variables.Find(L"unexists")==false);
	TEST_ASSERT(statementScope->functions.Find(L"unexists")==0);

	BasicScope* scope=0;
	TEST_ASSERT(statementScope->variables.Find(L"link", scope).type==link);
	TEST_ASSERT(scope==appendScope);
	TEST_ASSERT(statementScope->variables.Find(L"data", scope).type==tm.GetPrimitiveType(int_type));
	TEST_ASSERT(scope==appendScope);
	TEST_ASSERT(statementScope->variables.Find(L"temp", scope).type==link);
	TEST_ASSERT(scope==statementScope);
	TEST_ASSERT(statementScope->types.Find(L"Link", scope)==link);
	TEST_ASSERT(scope==global);
	TEST_ASSERT(statementScope->functions.Find(L"append", scope)==&append);
	TEST_ASSERT(scope==global);
	TEST_ASSERT(statementScope->types.Find(L"unexists", scope)==0);
	TEST_ASSERT(scope==0);
	TEST_ASSERT(statementScope->variables.Find(L"unexists", scope)==false);
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
	BasicTypeInfoManager tim;
	BasicScope* global=env.GlobalScope();
	List<Ptr<BasicLanguageCodeException>> errors;
	SortedList<WString> forwardStructures;
	BP argument(&env, global, &tm, &tim, errors, forwardStructures);
	
	BasicTypeRecord* link=tm.CreateStructureType(0);
	global->types.Add(L"Link", link);

	TEST_ASSERT(BasicLanguage_GetTypeRecord(t_void().GetInternalValue(), argument, true)==tm.GetPrimitiveType(void_type));
	TEST_ASSERT(BasicLanguage_GetTypeRecord(t_int().GetInternalValue(), argument, true)==tm.GetPrimitiveType(int_type));
	TEST_ASSERT(BasicLanguage_GetTypeRecord(t_type(L"Link").GetInternalValue(), argument, true)==link);
	TEST_ASSERT(BasicLanguage_GetTypeRecord((*t_type(L"Link")).GetInternalValue(), argument, true)==tm.GetPointerType(link));
	TEST_ASSERT(BasicLanguage_GetTypeRecord(t_int()[10].GetInternalValue(), argument, true)==tm.GetArrayType(tm.GetPrimitiveType(int_type), 10));

	List<BasicTypeRecord*> functionParameterTypes;
	functionParameterTypes.Add(tm.GetPointerType(link));
	functionParameterTypes.Add(tm.GetPointerType(link));
	TEST_ASSERT(BasicLanguage_GetTypeRecord(
		t_void()(t_types()<<*t_type(L"Link")<<*t_type(L"Link")).GetInternalValue(), argument, true)==
		tm.GetFunctionType(tm.GetPrimitiveType(void_type), functionParameterTypes.Wrap())
		);

	Ptr<BasicType> wrongType=(*t_type(L"Link"))(t_types()<<(*t_type(L"Link"))[10]<<(*t_type(L"Wrong"))[10]).GetInternalValue();
	/*TEST_EXCEPTION(
		BasicLanguage_GetTypeRecord(wrongType, argument, true),
		Ptr<BasicLanguageCodeException>,
		Curry(TestTypeNameNotExists)(L"Wrong")
		);*/
}

TEST_CASE(Test_BasicLanguage_GetTypeRecord_GenericTypeRename)
{
	BasicProgramNode program;
	program
		.Generic().GenericArgument(L"T")
		.DefineRename(L"Unit", t_type(L"T"));

	BasicEnv env;
	BasicTypeManager tm;
	BasicTypeInfoManager tim;
	BasicScope* global=env.GlobalScope();
	List<Ptr<BasicLanguageCodeException>> errors;
	SortedList<WString> forwardStructures;
	BP argument(&env, global, &tm, &tim, errors, forwardStructures);
	BasicLanguage_BuildGlobalScope(program.GetInternalValue(), argument);

	BasicTypeRecord* unitType=BasicLanguage_GetTypeRecord(t_type(L"Unit").GetInternalValue(), argument, true);
	TEST_ASSERT(unitType->GetType()==BasicTypeRecord::Generic);
	TEST_ASSERT(unitType->ParameterCount()==1);
	TEST_ASSERT(unitType->ParameterType(0)==tm.GetGenericArgumentType(L"T"));
	TEST_ASSERT(unitType->ElementType()==tm.GetGenericArgumentType(L"T"));

	BasicTypeRecord* intType=BasicLanguage_GetTypeRecord(t_type(L"Unit")[t_types()<<t_int()].GetInternalValue(), argument, false);
	TEST_ASSERT(intType==tm.GetPrimitiveType(int_type));
}

TEST_CASE(Test_BasicLanguage_GetTypeRecord_GenericStructure)
{
	BasicProgramNode program;
	program
		.Generic().GenericArgument(L"T")
		.DefineStructure(L"Link")
		.Member(L"data", t_type(L"T"))
		.Member(L"next", *t_type(L"Link")[t_types()<<t_type(L"T")]);

	BasicEnv env;
	BasicTypeManager tm;
	BasicTypeInfoManager tim;
	BasicScope* global=env.GlobalScope();
	List<Ptr<BasicLanguageCodeException>> errors;
	SortedList<WString> forwardStructures;
	BP argument(&env, global, &tm, &tim, errors, forwardStructures);
	BasicLanguage_BuildGlobalScope(program.GetInternalValue(), argument);

	BasicTypeRecord* linkType=BasicLanguage_GetTypeRecord(t_type(L"Link").GetInternalValue(), argument, true);
	TEST_ASSERT(linkType->GetType()==BasicTypeRecord::Generic);
	TEST_ASSERT(linkType->ParameterCount()==1);
	TEST_ASSERT(linkType->ParameterType(0)==tm.GetGenericArgumentType(L"T"));

	Dictionary<BasicTypeRecord*, BasicTypeRecord*> argumentTypes;
	argumentTypes.Add(tm.GetGenericArgumentType(L"T"), tm.GetGenericArgumentType(L"T"));
	BasicTypeRecord* tLinkType=linkType->ElementType();
	TEST_ASSERT(tLinkType->GetType()==BasicTypeRecord::Structure);
	TEST_ASSERT(tLinkType->MemberCount()==2);
	TEST_ASSERT(tLinkType->MemberType(L"data")==tm.GetGenericArgumentType(L"T"));
	TEST_ASSERT(tLinkType->MemberType(L"next")==tm.GetPointerType(tm.Instanciate(linkType, argumentTypes.Wrap())));

	BasicTypeRecord* intLinkType=BasicLanguage_GetTypeRecord(t_type(L"Link")[t_types()<<t_int()].GetInternalValue(), argument, false);
	TEST_ASSERT(intLinkType->GetType()==BasicTypeRecord::Structure);
	TEST_ASSERT(intLinkType->MemberCount()==2);
	TEST_ASSERT(intLinkType->MemberType(L"data")==tm.GetPrimitiveType(int_type));
	TEST_ASSERT(intLinkType->MemberType(L"next")==tm.GetPointerType(intLinkType));
}

/***********************************************************************
BasicLanguage_BuildGlobalScope
***********************************************************************/

TEST_CASE(Test_BasicLanguage_BuildGlobalScope)
{
	BasicEnv env;
	BasicTypeManager tm;
	BasicTypeInfoManager tim;
	List<Ptr<BasicLanguageCodeException>> errors;
	SortedList<WString> forwardStructures;
	BP argument(&env, env.GlobalScope(), &tm, &tim, errors, forwardStructures);

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
	TEST_ASSERT(env.GlobalScope()->variables.Items()[L"head"].type==typePLink);
	TEST_ASSERT(env.GlobalScope()->variables.Items()[L"tail"].type==typePLink);

	TEST_ASSERT(env.GlobalScope()->functions.Items().Count()==3);
	BasicFunctionDeclaration* functionCreate=env.GlobalScope()->functions.Items()[L"Create"];
	BasicFunctionDeclaration* functionDestroy=env.GlobalScope()->functions.Items()[L"Destroy"];
	BasicFunctionDeclaration* functionAppend=env.GlobalScope()->functions.Items()[L"Append"];
	BasicTypeRecord* typeCreate=env.GetFunctionType(functionCreate, false);
	BasicTypeRecord* typeDestroy=env.GetFunctionType(functionDestroy, false);
	BasicTypeRecord* typeAppend=env.GetFunctionType(functionAppend, false);

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

void SetConfiguration(BasicAlgorithmConfiguration& config)
{
	config.treatCharacterAsInteger=true;
	config.enableImplicitBooleanToIntegerConversion=true;
	config.enableImplicitIntegerToBooleanConversion=true;
	config.enableImplicitIntegerToFloatConversion=true;
	config.enableImplicitUnsignedToSignedConversion=true;
	config.enableImplicitPointerToBooleanConversion=true;
	config.enablePointerArithmetic=true;
	config.enableSubscribeOnPointer=true;
}

TEST_CASE(Test_BasicLanguage_GetExpressionType_BasicPrimitiveExpression)
{
	BasicEnv env;
	BasicTypeManager tm;
	BasicTypeInfoManager tim;
	List<Ptr<BasicLanguageCodeException>> errors;
	SortedList<WString> forwardStructures;
	BasicScope* globalScope=env.GlobalScope();
	BP argument(&env, globalScope, &tm, &tim, errors, forwardStructures);

	Ptr<BasicExpression> intExpr=e_prim((vsint)0).GetInternalValue();
	Ptr<BasicExpression> uintExpr=e_prim((vuint)0).GetInternalValue();
	Ptr<BasicExpression> s8Expr=e_prim((signed __int8)0).GetInternalValue();
	Ptr<BasicExpression> u8Expr=e_prim((unsigned __int8)0).GetInternalValue();
	Ptr<BasicExpression> s16Expr=e_prim((signed __int16)0).GetInternalValue();
	Ptr<BasicExpression> u16Expr=e_prim((unsigned __int16)0).GetInternalValue();
	Ptr<BasicExpression> s32Expr=e_prim((signed __int32)0).GetInternalValue();
	Ptr<BasicExpression> u32Expr=e_prim((unsigned __int32)0).GetInternalValue();
	Ptr<BasicExpression> s64Expr=e_prim((signed __int64)0).GetInternalValue();
	Ptr<BasicExpression> u64Expr=e_prim((unsigned __int64)0).GetInternalValue();
	Ptr<BasicExpression> f32Expr=e_prim((float)0).GetInternalValue();
	Ptr<BasicExpression> f64Expr=e_prim((double)0).GetInternalValue();
	Ptr<BasicExpression> charExpr=e_prim('a').GetInternalValue();
	Ptr<BasicExpression> wcharExpr=e_prim(L'a').GetInternalValue();
	Ptr<BasicExpression> boolExpr1=e_prim(true).GetInternalValue();
	Ptr<BasicExpression> boolExpr2=e_prim(false).GetInternalValue();
	Ptr<BasicExpression> astrExpr1=e_prim("vczh").GetInternalValue();
	Ptr<BasicExpression> wstrExpr1=e_prim(L"vczh").GetInternalValue();
	Ptr<BasicExpression> astrExpr2=e_prim(AString("vczh")).GetInternalValue();
	Ptr<BasicExpression> wstrExpr2=e_prim(WString(L"vczh")).GetInternalValue();

	TEST_ASSERT(BasicLanguage_GetExpressionType(intExpr, argument)==tm.GetPrimitiveType(int_type));
	TEST_ASSERT(BasicLanguage_GetExpressionType(uintExpr, argument)==tm.GetPrimitiveType(uint_type));
	TEST_ASSERT(BasicLanguage_GetExpressionType(s8Expr, argument)==tm.GetPrimitiveType(s8));
	TEST_ASSERT(BasicLanguage_GetExpressionType(u8Expr, argument)==tm.GetPrimitiveType(u8));
	TEST_ASSERT(BasicLanguage_GetExpressionType(s16Expr, argument)==tm.GetPrimitiveType(s16));
	TEST_ASSERT(BasicLanguage_GetExpressionType(u16Expr, argument)==tm.GetPrimitiveType(u16));
	TEST_ASSERT(BasicLanguage_GetExpressionType(s32Expr, argument)==tm.GetPrimitiveType(s32));
	TEST_ASSERT(BasicLanguage_GetExpressionType(u32Expr, argument)==tm.GetPrimitiveType(u32));
	TEST_ASSERT(BasicLanguage_GetExpressionType(s64Expr, argument)==tm.GetPrimitiveType(s64));
	TEST_ASSERT(BasicLanguage_GetExpressionType(u64Expr, argument)==tm.GetPrimitiveType(u64));
	TEST_ASSERT(BasicLanguage_GetExpressionType(f32Expr, argument)==tm.GetPrimitiveType(f32));
	TEST_ASSERT(BasicLanguage_GetExpressionType(f64Expr, argument)==tm.GetPrimitiveType(f64));
	TEST_ASSERT(BasicLanguage_GetExpressionType(charExpr, argument)==tm.GetPrimitiveType(char_type));
	TEST_ASSERT(BasicLanguage_GetExpressionType(wcharExpr, argument)==tm.GetPrimitiveType(wchar_type));
	TEST_ASSERT(BasicLanguage_GetExpressionType(boolExpr1, argument)==tm.GetPrimitiveType(bool_type));
	TEST_ASSERT(BasicLanguage_GetExpressionType(boolExpr2, argument)==tm.GetPrimitiveType(bool_type));
	TEST_ASSERT(BasicLanguage_GetExpressionType(astrExpr1, argument)==tm.GetPointerType(tm.GetPrimitiveType(char_type)));
	TEST_ASSERT(BasicLanguage_GetExpressionType(wstrExpr1, argument)==tm.GetPointerType(tm.GetPrimitiveType(wchar_type)));
	TEST_ASSERT(BasicLanguage_GetExpressionType(astrExpr2, argument)==tm.GetPointerType(tm.GetPrimitiveType(char_type)));
	TEST_ASSERT(BasicLanguage_GetExpressionType(wstrExpr2, argument)==tm.GetPointerType(tm.GetPrimitiveType(wchar_type)));
}

void Test_BasicLanguage_RunImplicitInteger()
{
	BasicEnv env;
	BasicTypeManager tm;
	BasicTypeInfoManager tim;
	List<Ptr<BasicLanguageCodeException>> errors;
	SortedList<WString> forwardStructures;
	BasicScope* globalScope=env.GlobalScope();
	BP argument(&env, globalScope, &tm, &tim, errors, forwardStructures);
	SetConfiguration(argument.configuration);

	Ptr<BasicStatement> s1=s_if(e_prim(1.1), s_empty()).GetInternalValue();
	Ptr<BasicStatement> s2=s_if(e_prim(true), s_expr(e_prim(0).Assign(e_prim(0)))).GetInternalValue();
	Ptr<BasicStatement> s3=s_if(e_prim(true), s_empty(), s_expr(e_prim(0).Assign(e_prim(0)))).GetInternalValue();

	BasicLanguage_CheckStatement(s1, argument);
	TEST_ASSERT(errors.Count()==1);
	BasicLanguage_CheckStatement(s2, argument);
	TEST_ASSERT(errors.Count()==2);
	BasicLanguage_CheckStatement(s3, argument);
	TEST_ASSERT(errors.Count()==3);
}

TEST_CASE(Test_BasicLanguage_ImplicitInteger)
{
	BasicPrimitiveTypeEnum types[]={s8, s16, s32, s64, u8, u16, u32, u64, f32, f64};
	for(vint i=0;i<10;i++)
	{
		for(vint j=0;j<8;j++)
		{
			BasicEnv env;
			BasicTypeManager tm;
			BasicTypeInfoManager tim;
			List<Ptr<BasicLanguageCodeException>> errors;
			SortedList<WString> forwardStructures;
			BasicScope* globalScope=env.GlobalScope();
			BP argument(&env, globalScope, &tm, &tim, errors, forwardStructures);
			SetConfiguration(argument.configuration);

			Ptr<BasicVariableStatement> statement=new BasicVariableStatement;
			Ptr<BasicPrimitiveType> type=new BasicPrimitiveType;
			type->type=types[i];
			statement->type=type;
			statement->name=L"a";

			Ptr<BasicNumericExpression> expression=new BasicNumericExpression;
			expression->implicitIntegerType=1;
			expression->type=types[j];
			expression->argument.s64=1;
			statement->initializer=expression;

			Ptr<BasicStatement> result=statement;
			BasicLanguage_CheckStatement(result, argument);
			TEST_ASSERT(errors.Count()==0);
		}
	}
}

TEST_CASE(Test_BasicLanguage_GetExpressionType_BasicUnaryExpression)
{
	BasicEnv env;
	BasicTypeManager tm;
	BasicTypeInfoManager tim;
	List<Ptr<BasicLanguageCodeException>> errors;
	SortedList<WString> forwardStructures;
	BasicScope* globalScope=env.GlobalScope();
	BP argument(&env, globalScope, &tm, &tim, errors, forwardStructures);
	SetConfiguration(argument.configuration);

	BasicProgramNode program;
	program.DefineVariable(L"pointer", *t_int());
	program.DefineVariable(L"integer", t_int());
	program.DefineVariable(L"float", t_float());
	BasicLanguage_BuildGlobalScope(program.GetInternalValue(), argument);

	Ptr<BasicExpression> e1=e_name(L"pointer")++.GetInternalValue();
	Ptr<BasicExpression> e2=e_name(L"integer")++.GetInternalValue();
	Ptr<BasicExpression> e3=e_name(L"float")++.GetInternalValue();

	Ptr<BasicExpression> e4=(*e_name(L"pointer")).GetInternalValue();
	Ptr<BasicExpression> e5=(*e_name(L"integer")).GetInternalValue();
	Ptr<BasicExpression> e6=(*e_name(L"float")).GetInternalValue();

	Ptr<BasicExpression> e7=e_name(L"pointer").Ref().GetInternalValue();
	Ptr<BasicExpression> e8=e_name(L"integer").Ref().GetInternalValue();
	Ptr<BasicExpression> e9=e_name(L"float").Ref().GetInternalValue();

	TEST_ASSERT(BasicLanguage_GetExpressionType(e1, argument)==tm.GetPointerType(tm.GetPrimitiveType(int_type)));
	TEST_ASSERT(BasicLanguage_GetExpressionType(e2, argument)==tm.GetPrimitiveType(int_type));
	TEST_ASSERT(BasicLanguage_GetExpressionType(e3, argument)==0);

	TEST_ASSERT(BasicLanguage_GetExpressionType(e4, argument)==tm.GetPrimitiveType(int_type));
	TEST_ASSERT(BasicLanguage_GetExpressionType(e5, argument)==0);
	TEST_ASSERT(BasicLanguage_GetExpressionType(e6, argument)==0);

	TEST_ASSERT(BasicLanguage_GetExpressionType(e7, argument)==tm.GetPointerType(tm.GetPointerType(tm.GetPrimitiveType(int_type))));
	TEST_ASSERT(BasicLanguage_GetExpressionType(e8, argument)==tm.GetPointerType(tm.GetPrimitiveType(int_type)));
	TEST_ASSERT(BasicLanguage_GetExpressionType(e9, argument)==tm.GetPointerType(tm.GetPrimitiveType(f32)));
}

TEST_CASE(Test_BasicLanguage_GetExpressionType_BasicBinaryExpression)
{
	BasicEnv env;
	BasicTypeManager tm;
	BasicTypeInfoManager tim;
	List<Ptr<BasicLanguageCodeException>> errors;
	SortedList<WString> forwardStructures;
	BasicScope* globalScope=env.GlobalScope();
	BP argument(&env, globalScope, &tm, &tim, errors, forwardStructures);
	SetConfiguration(argument.configuration);

	BasicProgramNode program;
	program.DefineVariable(L"pointer", *t_int());
	program.DefineVariable(L"integer", t_int());
	program.DefineVariable(L"float", t_float());
	BasicLanguage_BuildGlobalScope(program.GetInternalValue(), argument);

	Ptr<BasicExpression> a1=(e_name(L"pointer")+e_name(L"pointer")).GetInternalValue();
	Ptr<BasicExpression> a2=(e_name(L"pointer")+e_name(L"integer")).GetInternalValue();
	Ptr<BasicExpression> a3=(e_name(L"pointer")+e_name(L"float")).GetInternalValue();
	Ptr<BasicExpression> a4=(e_name(L"integer")+e_name(L"pointer")).GetInternalValue();
	Ptr<BasicExpression> a5=(e_name(L"integer")+e_name(L"integer")).GetInternalValue();
	Ptr<BasicExpression> a6=(e_name(L"integer")+e_name(L"float")).GetInternalValue();
	Ptr<BasicExpression> a7=(e_name(L"float")+e_name(L"pointer")).GetInternalValue();
	Ptr<BasicExpression> a8=(e_name(L"float")+e_name(L"integer")).GetInternalValue();
	Ptr<BasicExpression> a9=(e_name(L"float")+e_name(L"float")).GetInternalValue();

	Ptr<BasicExpression> b1=(e_name(L"pointer")-e_name(L"pointer")).GetInternalValue();
	Ptr<BasicExpression> b2=(e_name(L"pointer")-e_name(L"integer")).GetInternalValue();
	Ptr<BasicExpression> b3=(e_name(L"pointer")-e_name(L"float")).GetInternalValue();
	Ptr<BasicExpression> b4=(e_name(L"integer")-e_name(L"pointer")).GetInternalValue();
	Ptr<BasicExpression> b5=(e_name(L"integer")-e_name(L"integer")).GetInternalValue();
	Ptr<BasicExpression> b6=(e_name(L"integer")-e_name(L"float")).GetInternalValue();
	Ptr<BasicExpression> b7=(e_name(L"float")-e_name(L"pointer")).GetInternalValue();
	Ptr<BasicExpression> b8=(e_name(L"float")-e_name(L"integer")).GetInternalValue();
	Ptr<BasicExpression> b9=(e_name(L"float")-e_name(L"float")).GetInternalValue();

	Ptr<BasicExpression> c1=(e_name(L"pointer")+=e_name(L"pointer")).GetInternalValue();
	Ptr<BasicExpression> c2=(e_name(L"pointer")+=e_name(L"integer")).GetInternalValue();
	Ptr<BasicExpression> c3=(e_name(L"pointer")+=e_name(L"float")).GetInternalValue();
	Ptr<BasicExpression> c4=(e_name(L"integer")+=e_name(L"pointer")).GetInternalValue();
	Ptr<BasicExpression> c5=(e_name(L"integer")+=e_name(L"integer")).GetInternalValue();
	Ptr<BasicExpression> c6=(e_name(L"integer")+=e_name(L"float")).GetInternalValue();
	Ptr<BasicExpression> c7=(e_name(L"float")+=e_name(L"pointer")).GetInternalValue();
	Ptr<BasicExpression> c8=(e_name(L"float")+=e_name(L"integer")).GetInternalValue();
	Ptr<BasicExpression> c9=(e_name(L"float")+=e_name(L"float")).GetInternalValue();

	BasicTypeRecord* pointerType=tm.GetPointerType(tm.GetPrimitiveType(int_type));
	BasicTypeRecord* integerType=tm.GetPrimitiveType(int_type);
	BasicTypeRecord* floatType=tm.GetPrimitiveType(f32);
#ifdef VCZH_64
	BasicTypeRecord* numericType=tm.GetPrimitiveType(f64);
#else
	BasicTypeRecord* numericType=tm.GetPrimitiveType(f32);
#endif

	TEST_ASSERT(BasicLanguage_GetExpressionType(a1, argument)==0);
	TEST_ASSERT(errors.Count()==1);
	TEST_ASSERT(BasicLanguage_GetExpressionType(a2, argument)==pointerType);
	TEST_ASSERT(errors.Count()==1);
	TEST_ASSERT(BasicLanguage_GetExpressionType(a3, argument)==0);
	TEST_ASSERT(errors.Count()==2);
	TEST_ASSERT(BasicLanguage_GetExpressionType(a4, argument)==pointerType);
	TEST_ASSERT(errors.Count()==2);
	TEST_ASSERT(BasicLanguage_GetExpressionType(a5, argument)==integerType);
	TEST_ASSERT(errors.Count()==2);
	TEST_ASSERT(BasicLanguage_GetExpressionType(a6, argument)==numericType);
	TEST_ASSERT(errors.Count()==2);
	TEST_ASSERT(BasicLanguage_GetExpressionType(a7, argument)==0);
	TEST_ASSERT(errors.Count()==3);
	TEST_ASSERT(BasicLanguage_GetExpressionType(a8, argument)==numericType);
	TEST_ASSERT(errors.Count()==3);
	TEST_ASSERT(BasicLanguage_GetExpressionType(a9, argument)==floatType);
	TEST_ASSERT(errors.Count()==3);

	TEST_ASSERT(BasicLanguage_GetExpressionType(b1, argument)==integerType);
	TEST_ASSERT(errors.Count()==3);
	TEST_ASSERT(BasicLanguage_GetExpressionType(b2, argument)==pointerType);
	TEST_ASSERT(errors.Count()==3);
	TEST_ASSERT(BasicLanguage_GetExpressionType(b3, argument)==0);
	TEST_ASSERT(errors.Count()==4);
	TEST_ASSERT(BasicLanguage_GetExpressionType(b4, argument)==0);
	TEST_ASSERT(errors.Count()==5);
	TEST_ASSERT(BasicLanguage_GetExpressionType(b5, argument)==integerType);
	TEST_ASSERT(errors.Count()==5);
	TEST_ASSERT(BasicLanguage_GetExpressionType(b6, argument)==numericType);
	TEST_ASSERT(errors.Count()==5);
	TEST_ASSERT(BasicLanguage_GetExpressionType(b7, argument)==0);
	TEST_ASSERT(errors.Count()==6);
	TEST_ASSERT(BasicLanguage_GetExpressionType(b8, argument)==numericType);
	TEST_ASSERT(errors.Count()==6);
	TEST_ASSERT(BasicLanguage_GetExpressionType(b9, argument)==floatType);
	TEST_ASSERT(errors.Count()==6);

	TEST_ASSERT(BasicLanguage_GetExpressionType(c1, argument)==0);
	TEST_ASSERT(errors.Count()==7);
	TEST_ASSERT(BasicLanguage_GetExpressionType(c2, argument)==pointerType);
	TEST_ASSERT(errors.Count()==7);
	TEST_ASSERT(BasicLanguage_GetExpressionType(c3, argument)==0);
	TEST_ASSERT(errors.Count()==8);
	TEST_ASSERT(BasicLanguage_GetExpressionType(c4, argument)==0);
	TEST_ASSERT(errors.Count()==9);
	TEST_ASSERT(BasicLanguage_GetExpressionType(c5, argument)==integerType);
	TEST_ASSERT(errors.Count()==9);
	TEST_ASSERT(BasicLanguage_GetExpressionType(c6, argument)==integerType);
	TEST_ASSERT(errors.Count()==10);
	TEST_ASSERT(BasicLanguage_GetExpressionType(c7, argument)==0);
	TEST_ASSERT(errors.Count()==11);
	TEST_ASSERT(BasicLanguage_GetExpressionType(c8, argument)==floatType);
#ifdef VCZH_64
	const vint ErrorOffset=1;
#else
	const vint ErrorOffset=0;
#endif
	TEST_ASSERT(errors.Count()==11+ErrorOffset);
	TEST_ASSERT(BasicLanguage_GetExpressionType(c9, argument)==floatType);
	TEST_ASSERT(errors.Count()==11+ErrorOffset);
}

TEST_CASE(Test_BasicLanguage_GetExpressionType_BasicSubscribeExpression)
{
	BasicEnv env;
	BasicTypeManager tm;
	BasicTypeInfoManager tim;
	List<Ptr<BasicLanguageCodeException>> errors;
	SortedList<WString> forwardStructures;
	BasicScope* globalScope=env.GlobalScope();
	BP argument(&env, globalScope, &tm, &tim, errors, forwardStructures);
	SetConfiguration(argument.configuration);
	
	BasicProgramNode program;
	program.DefineVariable(L"pointer", *t_int());
	program.DefineVariable(L"array", t_double()[10]);
	program.DefineVariable(L"integer", t_int());
	BasicLanguage_BuildGlobalScope(program.GetInternalValue(), argument);

	Ptr<BasicExpression> s1=e_name(L"pointer")[e_prim(0)].GetInternalValue();
	Ptr<BasicExpression> s2=e_name(L"array")[e_prim(0)].GetInternalValue();
	Ptr<BasicExpression> s3=e_name(L"integer")[e_prim(0)].GetInternalValue();
	Ptr<BasicExpression> s4=e_name(L"pointer")[e_null()].GetInternalValue();
	Ptr<BasicExpression> s5=e_name(L"array")[e_null()].GetInternalValue();
	Ptr<BasicExpression> s6=e_name(L"integer")[e_null()].GetInternalValue();

	TEST_ASSERT(BasicLanguage_GetExpressionType(s1, argument)==tm.GetPrimitiveType(int_type));
	TEST_ASSERT(errors.Count()==0);
	TEST_ASSERT(BasicLanguage_GetExpressionType(s2, argument)==tm.GetPrimitiveType(f64));
	TEST_ASSERT(errors.Count()==0);
	TEST_ASSERT(BasicLanguage_GetExpressionType(s3, argument)==0);
	TEST_ASSERT(errors.Count()==1);
	TEST_ASSERT(BasicLanguage_GetExpressionType(s4, argument)==tm.GetPrimitiveType(int_type));
	TEST_ASSERT(errors.Count()==2);
	TEST_ASSERT(BasicLanguage_GetExpressionType(s5, argument)==tm.GetPrimitiveType(f64));
	TEST_ASSERT(errors.Count()==3);
	TEST_ASSERT(BasicLanguage_GetExpressionType(s6, argument)==0);
	TEST_ASSERT(errors.Count()==5);
}

TEST_CASE(Test_BasicLanguage_GetExpressionType_BasicMemberExpression)
{
	BasicEnv env;
	BasicTypeManager tm;
	BasicTypeInfoManager tim;
	List<Ptr<BasicLanguageCodeException>> errors;
	SortedList<WString> forwardStructures;
	BasicScope* globalScope=env.GlobalScope();
	BP argument(&env, globalScope, &tm, &tim, errors, forwardStructures);
	SetConfiguration(argument.configuration);

	BasicProgramNode program;
	program.DefineStructureForward(L"Link");
	program.DefineRename(L"PLink", *t_type(L"Link"));
	program.DefineStructure(L"Link")
		.Member(L"data", t_int())
		.Member(L"prev", t_type(L"PLink"))
		.Member(L"next", t_type(L"PLink"));
	program.DefineVariable(L"plink", t_type(L"PLink"));
	program.DefineVariable(L"link", t_type(L"Link"));
	BasicLanguage_BuildGlobalScope(program.GetInternalValue(), argument);

	Ptr<BasicExpression> m1=e_name(L"plink").Member(L"data").GetInternalValue();
	Ptr<BasicExpression> m2=e_name(L"plink").Member(L"prev").GetInternalValue();
	Ptr<BasicExpression> m3=e_name(L"plink").Member(L"wrong").GetInternalValue();
	Ptr<BasicExpression> m4=e_name(L"link").Member(L"data").GetInternalValue();
	Ptr<BasicExpression> m5=e_name(L"link").Member(L"prev").GetInternalValue();
	Ptr<BasicExpression> m6=e_name(L"link").Member(L"wrong").GetInternalValue();
	Ptr<BasicExpression> p1=e_name(L"plink").PMember(L"data").GetInternalValue();
	Ptr<BasicExpression> p2=e_name(L"plink").PMember(L"prev").GetInternalValue();
	Ptr<BasicExpression> p3=e_name(L"plink").PMember(L"wrong").GetInternalValue();
	Ptr<BasicExpression> p4=e_name(L"link").PMember(L"data").GetInternalValue();
	Ptr<BasicExpression> p5=e_name(L"link").PMember(L"prev").GetInternalValue();
	Ptr<BasicExpression> p6=e_name(L"link").PMember(L"wrong").GetInternalValue();
	Ptr<BasicExpression> w1=e_prim(0).Member(L"data").GetInternalValue();

	BasicTypeRecord* PLink=BasicLanguage_GetTypeRecord(t_type(L"PLink").GetInternalValue(), argument, true);
	TEST_ASSERT(BasicLanguage_GetExpressionType(m1, argument)==0);
	TEST_ASSERT(BasicLanguage_GetExpressionType(m2, argument)==0);
	TEST_ASSERT(BasicLanguage_GetExpressionType(m3, argument)==0);
	TEST_ASSERT(BasicLanguage_GetExpressionType(m4, argument)==tm.GetPrimitiveType(int_type));
	TEST_ASSERT(BasicLanguage_GetExpressionType(m5, argument)==PLink);
	TEST_ASSERT(BasicLanguage_GetExpressionType(m6, argument)==0);
	TEST_ASSERT(BasicLanguage_GetExpressionType(p1, argument)==tm.GetPrimitiveType(int_type));
	TEST_ASSERT(BasicLanguage_GetExpressionType(p2, argument)==PLink);
	TEST_ASSERT(BasicLanguage_GetExpressionType(p3, argument)==0);
	TEST_ASSERT(BasicLanguage_GetExpressionType(p4, argument)==0);
	TEST_ASSERT(BasicLanguage_GetExpressionType(p5, argument)==0);
	TEST_ASSERT(BasicLanguage_GetExpressionType(p6, argument)==0);
	TEST_ASSERT(BasicLanguage_GetExpressionType(w1, argument)==0);
}

TEST_CASE(Test_BasicLanguage_GetExpressionType_BasicInvokeExpression)
{
	BasicEnv env;
	BasicTypeManager tm;
	BasicTypeInfoManager tim;
	List<Ptr<BasicLanguageCodeException>> errors;
	SortedList<WString> forwardStructures;
	BasicScope* globalScope=env.GlobalScope();
	BP argument(&env, globalScope, &tm, &tim, errors, forwardStructures);
	SetConfiguration(argument.configuration);

	BasicProgramNode program;
	program.DefineFunction(L"expd")
		.Parameter(L"base", t_double())
		.Parameter(L"exponent", t_double())
		.ReturnType(t_double());
	program.DefineFunction(L"expf")
		.Parameter(L"base", t_float())
		.Parameter(L"exponent", t_float())
		.ReturnType(t_float());
	program.DefineFunction(L"expi")
		.Parameter(L"base", t_int())
		.Parameter(L"exponent", t_int())
		.ReturnType(t_int());
	program.DefineFunction(L"sin")
		.Parameter(L"x", t_double())
		.ReturnType(t_double());

	Ptr<BasicExpression> ddExpr=e_name(L"expd")(e_exps()<<e_prim((double)1)<<e_prim((double)2)).GetInternalValue();
	Ptr<BasicExpression> dfExpr=e_name(L"expd")(e_exps()<<e_prim((float)1)<<e_prim((float)2)).GetInternalValue();
	Ptr<BasicExpression> diExpr=e_name(L"expd")(e_exps()<<e_prim((vint)1)<<e_prim((vint)2)).GetInternalValue();
	Ptr<BasicExpression> fdExpr=e_name(L"expf")(e_exps()<<e_prim((double)1)<<e_prim((double)2)).GetInternalValue();
	Ptr<BasicExpression> ffExpr=e_name(L"expf")(e_exps()<<e_prim((float)1)<<e_prim((float)2)).GetInternalValue();
	Ptr<BasicExpression> fiExpr=e_name(L"expf")(e_exps()<<e_prim((vint)1)<<e_prim((vint)2)).GetInternalValue();
	Ptr<BasicExpression> idExpr=e_name(L"expi")(e_exps()<<e_prim((double)1)<<e_prim((double)2)).GetInternalValue();
	Ptr<BasicExpression> ifExpr=e_name(L"expi")(e_exps()<<e_prim((float)1)<<e_prim((float)2)).GetInternalValue();
	Ptr<BasicExpression> iiExpr=e_name(L"expi")(e_exps()<<e_prim((vint)1)<<e_prim((vint)2)).GetInternalValue();
	Ptr<BasicExpression> wrongExpr1=e_name(L"expvczh")(e_exps()<<e_prim((vint)1)<<e_prim((vint)2)).GetInternalValue();
	Ptr<BasicExpression> wrongExpr2=e_prim(0)(e_exps()<<e_prim((vint)1)<<e_prim((vint)2)).GetInternalValue();
	Ptr<BasicExpression> wrongExpr3=e_name(L"sin")(e_exps()<<e_prim((vint)1)<<e_prim((vint)2)).GetInternalValue();

#ifdef VCZH_64
	const vint error_offset=2;
#else
	const vint error_offset=0;
#endif

	BasicLanguage_BuildGlobalScope(program.GetInternalValue(), argument);
	TEST_ASSERT(BasicLanguage_GetExpressionType(ddExpr, argument)==tm.GetPrimitiveType(f64));
	TEST_ASSERT(errors.Count()==0);
	TEST_ASSERT(BasicLanguage_GetExpressionType(dfExpr, argument)==tm.GetPrimitiveType(f64));
	TEST_ASSERT(errors.Count()==0);
	TEST_ASSERT(BasicLanguage_GetExpressionType(diExpr, argument)==tm.GetPrimitiveType(f64));
	TEST_ASSERT(errors.Count()==0+error_offset);
	TEST_ASSERT(BasicLanguage_GetExpressionType(fdExpr, argument)==tm.GetPrimitiveType(f32));
	TEST_ASSERT(errors.Count()==2+error_offset);
	TEST_ASSERT(BasicLanguage_GetExpressionType(ffExpr, argument)==tm.GetPrimitiveType(f32));
	TEST_ASSERT(errors.Count()==2+error_offset);
	TEST_ASSERT(BasicLanguage_GetExpressionType(fiExpr, argument)==tm.GetPrimitiveType(f32));
	TEST_ASSERT(errors.Count()==4+error_offset);
	TEST_ASSERT(BasicLanguage_GetExpressionType(idExpr, argument)==tm.GetPrimitiveType(int_type));
	TEST_ASSERT(errors.Count()==6+error_offset);
	TEST_ASSERT(BasicLanguage_GetExpressionType(ifExpr, argument)==tm.GetPrimitiveType(int_type));
	TEST_ASSERT(errors.Count()==8+error_offset);
	TEST_ASSERT(BasicLanguage_GetExpressionType(iiExpr, argument)==tm.GetPrimitiveType(int_type));
	TEST_ASSERT(errors.Count()==8+error_offset);
	TEST_ASSERT(BasicLanguage_GetExpressionType(wrongExpr1, argument)==0);
	TEST_ASSERT(errors.Count()==9+error_offset);
	TEST_ASSERT(BasicLanguage_GetExpressionType(wrongExpr2, argument)==0);
	TEST_ASSERT(errors.Count()==10+error_offset);
	TEST_ASSERT(BasicLanguage_GetExpressionType(wrongExpr3, argument)==tm.GetPrimitiveType(f64));
	TEST_ASSERT(errors.Count()==11+error_offset);
}

TEST_CASE(Test_BasicLanguage_GetExpressionType_BasicResultExpression)
{
	BasicEnv env;
	BasicTypeManager tm;
	BasicTypeInfoManager tim;
	List<Ptr<BasicLanguageCodeException>> errors;
	SortedList<WString> forwardStructures;
	BasicScope* globalScope=env.GlobalScope();
	BP argument(&env, globalScope, &tm, &tim, errors, forwardStructures);
	SetConfiguration(argument.configuration);

	BasicProgramNode program;
	program.DefineFunction(L"a");
	program.DefineFunction(L"b").Parameter(L"x", t_int()).ReturnType(t_int());
	BasicLanguage_BuildGlobalScope(program.GetInternalValue(), argument);
	BasicScope* aScope=env.CreateFunctionScope(globalScope, dynamic_cast<BasicFunctionDeclaration*>(program.GetInternalValue()->declarations[0].Obj()));
	BasicScope* bScope=env.CreateFunctionScope(globalScope, dynamic_cast<BasicFunctionDeclaration*>(program.GetInternalValue()->declarations[1].Obj()));

	Ptr<BasicExpression> globalResult=e_result().GetInternalValue();
	Ptr<BasicExpression> aResult=e_result().GetInternalValue();
	Ptr<BasicExpression> bResult=e_result().GetInternalValue();

	BP aArgument(argument, aScope);
	BP bArgument(argument, bScope);

	TEST_ASSERT(BasicLanguage_GetExpressionType(globalResult, argument)==0);
	TEST_ASSERT(BasicLanguage_GetExpressionType(aResult, aArgument)==0);
	TEST_ASSERT(BasicLanguage_GetExpressionType(bResult, bArgument)==tm.GetPrimitiveType(int_type));
}

TEST_CASE(Test_BasicLanguage_GetExpressionType_BasicCastingExpression)
{
	BasicEnv env;
	BasicTypeManager tm;
	BasicTypeInfoManager tim;
	List<Ptr<BasicLanguageCodeException>> errors;
	SortedList<WString> forwardStructures;
	BasicScope* globalScope=env.GlobalScope();
	BP argument(&env, globalScope, &tm, &tim, errors, forwardStructures);
	SetConfiguration(argument.configuration);

	Ptr<BasicExpression> cast_int_to_uint=e_prim((vsint)0)[t_uint()].GetInternalValue();
	Ptr<BasicExpression> cast_int_to_pvoid=e_prim((vsint)0)[*t_void()].GetInternalValue();
	Ptr<BasicExpression> cast_pwchar_to_pint=e_prim(L"vczh")[*t_int()].GetInternalValue();
	Ptr<BasicExpression> cast_pwchar_to_pvoid=e_prim(L"vczh")[*t_void()].GetInternalValue();
	Ptr<BasicExpression> cast_pvoid_to_pwchar=e_null()[*t_wchar()].GetInternalValue();

	TEST_ASSERT(BasicLanguage_GetExpressionType(cast_int_to_uint, argument)==tm.GetPrimitiveType(uint_type));
	TEST_ASSERT(BasicLanguage_GetExpressionType(cast_int_to_pvoid, argument)==tm.GetPointerType(tm.GetPrimitiveType(void_type)));
	TEST_ASSERT(BasicLanguage_GetExpressionType(cast_pwchar_to_pint, argument)==tm.GetPointerType(tm.GetPrimitiveType(int_type)));
	TEST_ASSERT(BasicLanguage_GetExpressionType(cast_pwchar_to_pvoid, argument)==tm.GetPointerType(tm.GetPrimitiveType(void_type)));
	TEST_ASSERT(BasicLanguage_GetExpressionType(cast_pvoid_to_pwchar, argument)==tm.GetPointerType(tm.GetPrimitiveType(wchar_type)));
}

TEST_CASE(Test_BasicLanguage_GetExpressionType_BasicReferenceExpression)
{
	BasicEnv env;
	BasicTypeManager tm;
	BasicTypeInfoManager tim;
	List<Ptr<BasicLanguageCodeException>> errors;
	SortedList<WString> forwardStructures;
	BasicScope* globalScope=env.GlobalScope();
	BP argument(&env, globalScope, &tm, &tim, errors, forwardStructures);

	BasicProgramNode program;
	program.DefineFunction(L"a");
	program.DefineFunction(L"b");
	program.DefineVariable(L"c", t_int());
	BasicLanguage_BuildGlobalScope(program.GetInternalValue(), argument);
	BasicScope* functionScope=env.CreateFunctionScope(globalScope, dynamic_cast<BasicFunctionDeclaration*>(program.GetInternalValue()->declarations[0].Obj()));
	functionScope->variables.Add(L"b", BasicScope::Variable((vint)0, tm.GetPrimitiveType(uint_type)));
	functionScope->variables.Add(L"c", BasicScope::Variable((vint)0, tm.GetPrimitiveType(bool_type)));

	Ptr<BasicExpression> aExpr=e_name(L"a").GetInternalValue();
	Ptr<BasicExpression> bExpr=e_name(L"b").GetInternalValue();
	Ptr<BasicExpression> cExpr=e_name(L"c").GetInternalValue();

	BP argumentExpression(argument, functionScope);
	TEST_ASSERT(BasicLanguage_GetExpressionType(aExpr, argumentExpression)==BasicLanguage_GetTypeRecord(t_void()(t_types()).GetInternalValue(), argumentExpression, true));
	TEST_ASSERT(BasicLanguage_GetExpressionType(bExpr, argumentExpression)==BasicLanguage_GetTypeRecord(t_uint().GetInternalValue(), argumentExpression, true));
	TEST_ASSERT(BasicLanguage_GetExpressionType(cExpr, argumentExpression)==BasicLanguage_GetTypeRecord(t_bool().GetInternalValue(), argumentExpression, true));
}

TEST_CASE(Test_BasicLanguage_GetExpressionType_BasicInstantiatedExpression)
{
	BasicEnv env;
	BasicTypeManager tm;
	BasicTypeInfoManager tim;
	List<Ptr<BasicLanguageCodeException>> errors;
	SortedList<WString> forwardStructures;
	BasicScope* globalScope=env.GlobalScope();
	BP argument(&env, globalScope, &tm, &tim, errors, forwardStructures);

	BasicProgramNode program;
	program.Generic().GenericArgument(L"T").GenericArgument(L"U")
		.DefineFunction(L"Map")
		.ReturnType(t_void())
		.Parameter(L"ts", *t_type(L"T"))
		.Parameter(L"us", *t_type(L"U"))
		.Parameter(L"count", t_int())
		.Parameter(L"mapper", t_type(L"T")(t_types()<<t_type(L"U")))
		;
	program.Generic().GenericArgument(L"T")
		.DefineStructure(L"Vector")
		.Member(L"x", t_type(L"T"))
		.Member(L"y", t_type(L"T"))
		;
	program.Generic().GenericArgument(L"T")
		.DefineVariable(L"var", t_type(L"Vector")[t_types()<<t_type(L"T")])
		;
	BasicLanguage_BuildGlobalScope(program.GetInternalValue(), argument);


	Ptr<BasicExpression> exprFunction=e_name(L"Map", t_types()<<t_int()<<t_char()).GetInternalValue();
	Ptr<BasicType> functionType=t_void()(t_types()<<*t_int()<<*t_char()<<t_int()<<(t_int()(t_types()<<t_char()))).GetInternalValue();
	TEST_ASSERT(BasicLanguage_GetExpressionType(exprFunction, argument)==BasicLanguage_GetTypeRecord(functionType, argument, false));

	Ptr<BasicExpression> exprVariable=e_name(L"var", t_types()<<t_int()).GetInternalValue();
	BasicTypeRecord* variableType=BasicLanguage_GetExpressionType(exprVariable, argument);
	TEST_ASSERT(variableType->GetType()==BasicTypeRecord::Structure);
	TEST_ASSERT(variableType->MemberType(L"x")==tm.GetPrimitiveType(int_type));
	TEST_ASSERT(variableType->MemberType(L"y")==tm.GetPrimitiveType(int_type));
}

/***********************************************************************
BasicLanguage_CheckStatement
***********************************************************************/

TEST_CASE(Test_BasicLanguage_CheckStatement_BasicEmptyStatement)
{
	BasicEnv env;
	BasicTypeManager tm;
	BasicTypeInfoManager tim;
	List<Ptr<BasicLanguageCodeException>> errors;
	SortedList<WString> forwardStructures;
	BasicScope* globalScope=env.GlobalScope();
	BP argument(&env, globalScope, &tm, &tim, errors, forwardStructures);
	SetConfiguration(argument.configuration);

	Ptr<BasicStatement> statement=s_empty().GetInternalValue();
	BasicLanguage_CheckStatement(statement, argument);
	TEST_ASSERT(errors.Count()==0);
}

TEST_CASE(Test_BasicLanguage_CheckStatement_BasicExpressionStatement)
{
	BasicEnv env;
	BasicTypeManager tm;
	BasicTypeInfoManager tim;
	List<Ptr<BasicLanguageCodeException>> errors;
	SortedList<WString> forwardStructures;
	BasicScope* globalScope=env.GlobalScope();
	BP argument(&env, globalScope, &tm, &tim, errors, forwardStructures);
	SetConfiguration(argument.configuration);

	Ptr<BasicStatement> s1=s_expr(e_prim(0)).GetInternalValue();
	Ptr<BasicStatement> s2=s_expr(e_prim(0).Assign(e_prim(0))).GetInternalValue();
	BasicLanguage_CheckStatement(s1, argument);
	TEST_ASSERT(errors.Count()==0);
	BasicLanguage_CheckStatement(s2, argument);
	TEST_ASSERT(errors.Count()==1);
}

TEST_CASE(Test_BasicLanguage_CheckStatement_BasicVariableStatement)
{
	BasicEnv env;
	BasicTypeManager tm;
	BasicTypeInfoManager tim;
	List<Ptr<BasicLanguageCodeException>> errors;
	SortedList<WString> forwardStructures;
	BasicScope* globalScope=env.GlobalScope();
	BP argument(&env, globalScope, &tm, &tim, errors, forwardStructures);
	SetConfiguration(argument.configuration);

	BasicProgramNode program;
	program.DefineFunction(L"main");
	BasicScope* functionScope=env.CreateFunctionScope(globalScope, dynamic_cast<BasicFunctionDeclaration*>(program.GetInternalValue()->declarations[0].Obj()));

	Ptr<BasicStatement> statement=
		(s_var(t_int(), L"a")<<
		s_var(t_int(), L"b", e_prim(1))<<
		s_var(t_int(), L"c", e_prim(1.2))<<
		s_var(t_int(), L"a", e_prim(1))<<
		s_expr(e_name(L"a").Assign(e_name(L"b").Assign(e_name(L"c").Assign(e_prim(1.2))))))
		.GetInternalValue();
	BasicLanguage_CheckStatement(statement, argument);
	TEST_ASSERT(errors.Count()==3);
	TEST_ASSERT(errors[0]->GetExceptionCode()==BasicLanguageCodeException::InitializerTypeNotMatch);
	TEST_ASSERT(errors[1]->GetExceptionCode()==BasicLanguageCodeException::VariableAlreadyExists);
	TEST_ASSERT(errors[2]->GetExceptionCode()==BasicLanguageCodeException::BinaryTypeNotMatch);
}

TEST_CASE(Test_BasicLanguage_CheckStatement_BasicIfStatement)
{
	BasicEnv env;
	BasicTypeManager tm;
	BasicTypeInfoManager tim;
	List<Ptr<BasicLanguageCodeException>> errors;
	SortedList<WString> forwardStructures;
	BasicScope* globalScope=env.GlobalScope();
	BP argument(&env, globalScope, &tm, &tim, errors, forwardStructures);
	SetConfiguration(argument.configuration);

	Ptr<BasicStatement> s1=s_if(e_prim(1.1), s_empty()).GetInternalValue();
	Ptr<BasicStatement> s2=s_if(e_prim(true), s_expr(e_prim(0).Assign(e_prim(0)))).GetInternalValue();
	Ptr<BasicStatement> s3=s_if(e_prim(true), s_empty(), s_expr(e_prim(0).Assign(e_prim(0)))).GetInternalValue();

	BasicLanguage_CheckStatement(s1, argument);
	TEST_ASSERT(errors.Count()==1);
	BasicLanguage_CheckStatement(s2, argument);
	TEST_ASSERT(errors.Count()==2);
	BasicLanguage_CheckStatement(s3, argument);
	TEST_ASSERT(errors.Count()==3);
}

TEST_CASE(Test_BasicLanguage_CheckStatement_BasicWhileStatement)
{
	BasicEnv env;
	BasicTypeManager tm;
	BasicTypeInfoManager tim;
	List<Ptr<BasicLanguageCodeException>> errors;
	SortedList<WString> forwardStructures;
	BasicScope* globalScope=env.GlobalScope();
	BP argument(&env, globalScope, &tm, &tim, errors, forwardStructures);
	SetConfiguration(argument.configuration);

	Ptr<BasicStatement> a1=s_while(e_prim(1.1), s_expr(e_prim(0).Assign(e_prim(0)))).GetInternalValue();
	Ptr<BasicStatement> a2=s_while(e_prim(true), s_expr(e_prim(0).Assign(e_prim(0)))).GetInternalValue();
	Ptr<BasicStatement> a3=s_while(e_prim(1.1), s_empty()).GetInternalValue();
	Ptr<BasicStatement> a4=s_while(e_prim(true), s_empty()).GetInternalValue();

	Ptr<BasicStatement> b1=s_do_while(e_prim(1.1), s_expr(e_prim(0).Assign(e_prim(0)))).GetInternalValue();
	Ptr<BasicStatement> b2=s_do_while(e_prim(true), s_expr(e_prim(0).Assign(e_prim(0)))).GetInternalValue();
	Ptr<BasicStatement> b3=s_do_while(e_prim(1.1), s_empty()).GetInternalValue();
	Ptr<BasicStatement> b4=s_do_while(e_prim(true), s_empty()).GetInternalValue();

	Ptr<BasicStatement> c1=s_loop(s_expr(e_prim(0).Assign(e_prim(0)))).GetInternalValue();
	Ptr<BasicStatement> c2=s_loop(s_empty()).GetInternalValue();

	Ptr<BasicStatement> d1=s_conditional_loop(e_prim(1.1), e_prim(1.1), s_expr(e_prim(0).Assign(e_prim(0)))).GetInternalValue();
	Ptr<BasicStatement> d2=s_conditional_loop(e_prim(true), e_prim(1.1), s_expr(e_prim(0).Assign(e_prim(0)))).GetInternalValue();
	Ptr<BasicStatement> d3=s_conditional_loop(e_prim(1.1), e_prim(true), s_expr(e_prim(0).Assign(e_prim(0)))).GetInternalValue();
	Ptr<BasicStatement> d4=s_conditional_loop(e_prim(1.1), e_prim(1.1), s_empty()).GetInternalValue();
	Ptr<BasicStatement> d5=s_conditional_loop(e_prim(true), e_prim(true), s_empty()).GetInternalValue();

	BasicLanguage_CheckStatement(a1, argument);
	TEST_ASSERT(errors.Count()==2);
	BasicLanguage_CheckStatement(a2, argument);
	TEST_ASSERT(errors.Count()==3);
	BasicLanguage_CheckStatement(a3, argument);
	TEST_ASSERT(errors.Count()==4);
	BasicLanguage_CheckStatement(a4, argument);
	TEST_ASSERT(errors.Count()==4);
	BasicLanguage_CheckStatement(b1, argument);
	TEST_ASSERT(errors.Count()==6);
	BasicLanguage_CheckStatement(b2, argument);
	TEST_ASSERT(errors.Count()==7);
	BasicLanguage_CheckStatement(b3, argument);
	TEST_ASSERT(errors.Count()==8);
	BasicLanguage_CheckStatement(b4, argument);
	TEST_ASSERT(errors.Count()==8);
	BasicLanguage_CheckStatement(c1, argument);
	TEST_ASSERT(errors.Count()==9);
	BasicLanguage_CheckStatement(c2, argument);
	TEST_ASSERT(errors.Count()==9);
	BasicLanguage_CheckStatement(d1, argument);
	TEST_ASSERT(errors.Count()==12);
	BasicLanguage_CheckStatement(d2, argument);
	TEST_ASSERT(errors.Count()==14);
	BasicLanguage_CheckStatement(d3, argument);
	TEST_ASSERT(errors.Count()==16);
	BasicLanguage_CheckStatement(d4, argument);
	TEST_ASSERT(errors.Count()==18);
	BasicLanguage_CheckStatement(d5, argument);
	TEST_ASSERT(errors.Count()==18);
}

TEST_CASE(Test_BasicLanguage_CheckStatement_BasicForStatement)
{
	BasicEnv env;
	BasicTypeManager tm;
	BasicTypeInfoManager tim;
	List<Ptr<BasicLanguageCodeException>> errors;
	SortedList<WString> forwardStructures;
	BasicScope* globalScope=env.GlobalScope();
	BP argument(&env, globalScope, &tm, &tim, errors, forwardStructures);
	SetConfiguration(argument.configuration);

	Ptr<BasicStatement> s1=s_for(s_empty(), e_prim(true), s_empty(), s_empty()).GetInternalValue();
	Ptr<BasicStatement> s2=s_for(s_expr(e_prim(0).Assign(e_prim(0))), e_prim(true), s_empty(), s_empty()).GetInternalValue();
	Ptr<BasicStatement> s3=s_for(s_empty(), e_prim(1.1), s_empty(), s_empty()).GetInternalValue();
	Ptr<BasicStatement> s4=s_for(s_empty(), e_prim(true), s_expr(e_prim(0).Assign(e_prim(0))), s_empty()).GetInternalValue();
	Ptr<BasicStatement> s5=s_for(s_empty(), e_prim(true), s_empty(), s_expr(e_prim(0).Assign(e_prim(0)))).GetInternalValue();
	Ptr<BasicStatement> s6=s_for(
		s_var(t_int(), L"a", e_prim(0))<<s_var(t_int(), L"b", e_prim(0)),
		e_name(L"a")<e_prim(10),
		s_expr(e_name(L"a")++),
		s_expr(e_name(L"b")+=e_name(L"a"))
		).GetInternalValue();

	BasicLanguage_CheckStatement(s1, argument);
	TEST_ASSERT(errors.Count()==0);
	BasicLanguage_CheckStatement(s2, argument);
	TEST_ASSERT(errors.Count()==1);
	BasicLanguage_CheckStatement(s3, argument);
	TEST_ASSERT(errors.Count()==2);
	BasicLanguage_CheckStatement(s4, argument);
	TEST_ASSERT(errors.Count()==3);
	BasicLanguage_CheckStatement(s5, argument);
	TEST_ASSERT(errors.Count()==4);
	BasicLanguage_CheckStatement(s6, argument);
	TEST_ASSERT(errors.Count()==4);
}

TEST_CASE(Test_BasicLanguage_CheckStatement_BasicBreakContinueStatement)
{
	BasicEnv env;
	BasicTypeManager tm;
	BasicTypeInfoManager tim;
	List<Ptr<BasicLanguageCodeException>> errors;
	SortedList<WString> forwardStructures;
	BasicScope* globalScope=env.GlobalScope();
	BP argument(&env, globalScope, &tm, &tim, errors, forwardStructures);
	SetConfiguration(argument.configuration);

	Ptr<BasicStatement> s1=s_break().GetInternalValue();
	Ptr<BasicStatement> s2=s_loop(s_break()).GetInternalValue();
	Ptr<BasicStatement> s3=s_for(s_empty(), e_prim(true), s_empty(), s_break()).GetInternalValue();
	Ptr<BasicStatement> s4=s_continue().GetInternalValue();
	Ptr<BasicStatement> s5=s_loop(s_continue()).GetInternalValue();
	Ptr<BasicStatement> s6=s_for(s_empty(), e_prim(true), s_empty(), s_continue()).GetInternalValue();

	BasicLanguage_CheckStatement(s1, argument);
	TEST_ASSERT(errors.Count()==1);
	BasicLanguage_CheckStatement(s2, argument);
	TEST_ASSERT(errors.Count()==1);
	BasicLanguage_CheckStatement(s3, argument);
	TEST_ASSERT(errors.Count()==1);
	BasicLanguage_CheckStatement(s4, argument);
	TEST_ASSERT(errors.Count()==2);
	BasicLanguage_CheckStatement(s5, argument);
	TEST_ASSERT(errors.Count()==2);
	BasicLanguage_CheckStatement(s6, argument);
	TEST_ASSERT(errors.Count()==2);
}

TEST_CASE(Test_BasicLanguage_CheckStatement_BasicReturnStatement)
{
	BasicEnv env;
	BasicTypeManager tm;
	BasicTypeInfoManager tim;
	List<Ptr<BasicLanguageCodeException>> errors;
	SortedList<WString> forwardStructures;
	BasicScope* globalScope=env.GlobalScope();
	BP argument(&env, globalScope, &tm, &tim, errors, forwardStructures);
	SetConfiguration(argument.configuration);

	Ptr<BasicStatement> statement=s_return().GetInternalValue();
	BasicLanguage_CheckStatement(statement, argument);
	TEST_ASSERT(errors.Count()==0);
}

/***********************************************************************
BasicLanguage_BuildDeclarationBody
***********************************************************************/

TEST_CASE(Test_BasicLanguage_BuildDeclarationBody)
{
	BasicEnv env;
	BasicTypeManager tm;
	BasicTypeInfoManager tim;
	List<Ptr<BasicLanguageCodeException>> errors;
	SortedList<WString> forwardStructures;
	BasicScope* globalScope=env.GlobalScope();
	BP argument(&env, globalScope, &tm, &tim, errors, forwardStructures);
	SetConfiguration(argument.configuration);

	BasicProgramNode program;
	program.DefineFunction(L"a").Parameter(L"b", t_int()).Parameter(L"b", t_int()).Statement(s_empty());
	program.DefineFunction(L"b").Parameter(L"b", t_int()).Parameter(L"c", t_int()).Statement(s_empty());
	BasicLanguage_BuildGlobalScope(program.GetInternalValue(), argument);

	BasicLanguage_BuildDeclarationBody(program.GetInternalValue()->declarations[0].Obj(), argument);
	TEST_ASSERT(errors.Count()==1);
	BasicLanguage_BuildDeclarationBody(program.GetInternalValue()->declarations[1].Obj(), argument);
	TEST_ASSERT(errors.Count()==1);
}

/***********************************************************************
Concept
***********************************************************************/

TEST_CASE(Test_BasicLanguage_ConceptDeclaration)
{
	BasicEnv env;
	BasicTypeManager tm;
	BasicTypeInfoManager tim;
	List<Ptr<BasicLanguageCodeException>> errors;
	SortedList<WString> forwardStructures;
	BasicScope* globalScope=env.GlobalScope();
	BP argument(&env, globalScope, &tm, &tim, errors, forwardStructures);
	SetConfiguration(argument.configuration);

	BasicProgramNode program;
	program.DefineConcept(L"Eq", L"T")
		.Member(L"Equals", t_bool()(t_types()<<t_type(L"T")<<t_type(L"T")))
		.Member(L"NotEquals", t_bool()(t_types()<<t_type(L"T")<<t_type(L"T")))
		;
	BasicLanguage_BuildGlobalScope(program.GetInternalValue(), argument);
	BasicScope::Concept* conceptObject=globalScope->concepts.Find(L"Eq").Obj();
	TEST_ASSERT(conceptObject!=0);
	TEST_ASSERT(conceptObject->conceptType==tm.GetGenericArgumentType(L"T"));

	BasicTypeRecord* typeEquals=conceptObject->functions[L"Equals"];
	TEST_ASSERT(typeEquals->GetType()==BasicTypeRecord::Function);
	TEST_ASSERT(typeEquals->ReturnType()==tm.GetPrimitiveType(bool_type));
	TEST_ASSERT(typeEquals->ParameterCount()==2);
	TEST_ASSERT(typeEquals->ParameterType(0)==tm.GetGenericArgumentType(L"T"));
	TEST_ASSERT(typeEquals->ParameterType(1)==tm.GetGenericArgumentType(L"T"));

	BasicTypeRecord* typeNotEquals=conceptObject->functions[L"NotEquals"];
	TEST_ASSERT(typeNotEquals->GetType()==BasicTypeRecord::Function);
	TEST_ASSERT(typeNotEquals->ReturnType()==tm.GetPrimitiveType(bool_type));
	TEST_ASSERT(typeNotEquals->ParameterCount()==2);
	TEST_ASSERT(typeNotEquals->ParameterType(0)==tm.GetGenericArgumentType(L"T"));
	TEST_ASSERT(typeNotEquals->ParameterType(1)==tm.GetGenericArgumentType(L"T"));
}

/***********************************************************************
BasicAnalyzer
***********************************************************************/

TEST_CASE(Test_BasicAnalyzer)
{
	BasicAlgorithmConfiguration configuration;
	SetConfiguration(configuration);

	BasicProgramNode program;
	program.DefineFunction(L"fab")
		.Parameter(L"n", t_int())
		.ReturnType(t_int())
		.Statement(
			s_if(e_name(L"n")<e_prim(2),
				s_expr(e_result().Assign(e_prim(1)))<<s_return(),
				s_expr(e_result().Assign(
					e_name(L"fab")(e_exps()<<e_name(L"n")-e_prim(1))+
					e_name(L"fab")(e_exps()<<e_name(L"n")-e_prim(2))
					))<<s_return()
				)
		);

	BasicAnalyzer analyzer(program.GetInternalValue(), configuration);
	analyzer.Analyze();
	TEST_ASSERT(analyzer.GetErrors().Count()==0);
}