#include "..\..\Library\UnitTest\UnitTest.h"
#include "..\..\Library\Scripting\BasicIL\BasicILDefinition.h"
#include "..\..\Library\Scripting\BasicIL\BasicILInterpretor.h"
#include "..\..\Library\Scripting\BasicIL\BasicILSymbolResource.h"

using namespace vl;
using namespace vl::scripting;
using namespace vl::scripting::basicil;

TEST_CASE(TestBasicILInstruction_Generic_Function)
{
	BasicIL il;
	{
		Ptr<ResourceStream> symbolResource=new ResourceStream;
		il.resources.Add(BasicILResourceNames::ExportedSymbols, symbolResource);

		ResourceRecord<BasicILEntryRes> entryRes=symbolResource->CreateRecord<BasicILEntryRes>();
		entryRes->assemblyName=symbolResource->CreateString(L"Map");
		entryRes->exports=ResourceArrayHandle<BasicILExportRes>::Null();
		entryRes->linkings=ResourceArrayHandle<BasicILLinkingRes>::Null();
		entryRes->foreigns=ResourceArrayHandle<BasicILLinkingRes>::Null();
		ResourceRecord<BasicILGenericRes> genericRes=symbolResource->CreateRecord<BasicILGenericRes>();
		entryRes->genericSymbols=genericRes;

		ResourceRecord<BasicILGenericFunctionEntryRes> entry0=symbolResource->CreateRecord<BasicILGenericFunctionEntryRes>();
		{
			entry0->genericArgumentCount=1;
			entry0->startInstruction=0;
			entry0->instructionCount=39;
			entry0->name=symbolResource->CreateString(L"FindFirst");
			entry0->uniqueEntryID=symbolResource->CreateString(L"FindFirst");
		}
		ResourceRecord<BasicILGenericTargetRes> target0=symbolResource->CreateRecord<BasicILGenericTargetRes>();
		{
			target0->assemblyName=symbolResource->CreateString(L"Map");
			target0->symbolName=symbolResource->CreateString(L"FindFirst");

			ResourceRecord<BasicILGenericArgumentRes> argument0=symbolResource->CreateRecord<BasicILGenericArgumentRes>();
			{
				ResourceRecord<BasicILGenericLinearRes> sizeArgument=symbolResource->CreateRecord<BasicILGenericLinearRes>();
				sizeArgument->constant=sizeof(double);
				sizeArgument->factors=ResourceArrayHandle<BasicILGenericFactorItemRes>::Null();
				argument0->sizeArgument=sizeArgument;

				ResourceRecord<BasicILGenericNameRes> name=symbolResource->CreateRecord<BasicILGenericNameRes>();
				name->isConstant=true;
				name->constantString=symbolResource->CreateString(L"PRIMITIVE:DOUBLE");
				name->stringArgumentIndex=-1;
				argument0->nameArgument=name;

				argument0->subArgument=ResourceArrayHandle<BasicILGenericArgumentRes>::Null();
			}

			ResourceArrayRecord<BasicILGenericArgumentRes> arguments=symbolResource->CreateArrayRecord<BasicILGenericArgumentRes>(1);
			arguments.Set(0, argument0);
			target0->arguments=arguments;
		}
		ResourceRecord<BasicILGenericTargetRes> target1=symbolResource->CreateRecord<BasicILGenericTargetRes>();
		{
			target1->assemblyName=symbolResource->CreateString(L"Map");
			target1->symbolName=symbolResource->CreateString(L"FindFirst");

			ResourceRecord<BasicILGenericArgumentRes> argument0=symbolResource->CreateRecord<BasicILGenericArgumentRes>();
			{
				ResourceRecord<BasicILGenericLinearRes> sizeArgument=symbolResource->CreateRecord<BasicILGenericLinearRes>();
				sizeArgument->constant=sizeof(char);
				sizeArgument->factors=ResourceArrayHandle<BasicILGenericFactorItemRes>::Null();
				argument0->sizeArgument=sizeArgument;

				ResourceRecord<BasicILGenericNameRes> name=symbolResource->CreateRecord<BasicILGenericNameRes>();
				name->isConstant=true;
				name->constantString=symbolResource->CreateString(L"PRIMITIVE:CHAR");
				name->stringArgumentIndex=-1;
				argument0->nameArgument=name;

				argument0->subArgument=ResourceArrayHandle<BasicILGenericArgumentRes>::Null();
			}

			ResourceArrayRecord<BasicILGenericArgumentRes> arguments=symbolResource->CreateArrayRecord<BasicILGenericArgumentRes>(1);
			arguments.Set(0, argument0);
			target1->arguments=arguments;
		}
		ResourceRecord<BasicILGenericLinearRes> linear0=symbolResource->CreateRecord<BasicILGenericLinearRes>();
		{
			linear0->constant=sizeof(vint);
			ResourceRecord<BasicILGenericFactorItemRes> factor0=symbolResource->CreateRecord<BasicILGenericFactorItemRes>();
			factor0->factor=1;
			ResourceArrayRecord<BasicILGenericFactorItemRes> factors=symbolResource->CreateArrayRecord<BasicILGenericFactorItemRes>(1);
			factors.Set(0, factor0);
			linear0->factors=factors;
		}
		ResourceRecord<BasicILGenericLinearRes> linear1=symbolResource->CreateRecord<BasicILGenericLinearRes>();
		{
			linear1->constant=0;
			ResourceRecord<BasicILGenericFactorItemRes> factor0=symbolResource->CreateRecord<BasicILGenericFactorItemRes>();
			factor0->factor=1;
			ResourceArrayRecord<BasicILGenericFactorItemRes> factors=symbolResource->CreateArrayRecord<BasicILGenericFactorItemRes>(1);
			factors.Set(0, factor0);
			linear1->factors=factors;
		}

		ResourceArrayRecord<BasicILGenericFunctionEntryRes> entries=symbolResource->CreateArrayRecord<BasicILGenericFunctionEntryRes>(1);
		entries.Set(0, entry0);
		ResourceArrayRecord<BasicILGenericTargetRes> targets=symbolResource->CreateArrayRecord<BasicILGenericTargetRes>(2);
		targets.Set(0, target0);
		targets.Set(1, target1);
		ResourceArrayRecord<BasicILGenericLinearRes> linears=symbolResource->CreateArrayRecord<BasicILGenericLinearRes>(2);
		linears.Set(0, linear0);
		linears.Set(1, linear1);
		genericRes->functionEntries=entries;
		genericRes->variableEntries=ResourceArrayHandle<BasicILGenericVariableEntryRes>::Null();
		genericRes->targets=targets;
		genericRes->linears=linears;
		genericRes->concepts=ResourceArrayHandle<BasicILGenericConceptRes>::Null();
		genericRes->instances=ResourceArrayHandle<BasicILGenericInstanceRes>::Null();
		genericRes->instanceTargets=ResourceArrayHandle<BasicILGenericInstanceTargetRes>::Null();
	}

	const vint address_of_i=-(vint)sizeof(vint);
	const vint address_of_items=4*sizeof(vint);
	const vint address_of_count=5*sizeof(vint);
	const vint address_of_key=6*sizeof(vint);

	//T FindFirst<T>((int+T)* items, int count, int key);
	il
	/*	0	*/.Ins(BasicIns::stack_reserve,		BasicIns::MakeInt(sizeof(vint)))
	/*	1	*/.Ins(BasicIns::push,				BasicIns::int_type, BasicIns::MakeInt(0))
	/*	2	*/.Ins(BasicIns::stack_offset,		BasicIns::MakeInt(address_of_i))
	/*	3	*/.Ins(BasicIns::write,				BasicIns::int_type)
	/*	4	*/.Ins(BasicIns::stack_offset,		BasicIns::MakeInt(address_of_i))
	/*	5	*/.Ins(BasicIns::read,				BasicIns::int_type)
	/*	6	*/.Ins(BasicIns::stack_offset,		BasicIns::MakeInt(address_of_count))
	/*	7	*/.Ins(BasicIns::read,				BasicIns::int_type)
	/*	8	*/.Ins(BasicIns::eq,				BasicIns::int_type)
	/*	9	*/.Ins(BasicIns::jumptrue,			BasicIns::MakeInt(37))
	/*	10	*/.Ins(BasicIns::stack_offset,		BasicIns::MakeInt(address_of_items))
	/*	11	*/.Ins(BasicIns::read,				BasicIns::pointer_type)
	/*	12	*/.Ins(BasicIns::read,				BasicIns::int_type)
	/*	13	*/.Ins(BasicIns::stack_offset,		BasicIns::MakeInt(address_of_key))
	/*	14	*/.Ins(BasicIns::read,				BasicIns::int_type)
	/*	15	*/.Ins(BasicIns::eq,				BasicIns::int_type)
	/*	16	*/.Ins(BasicIns::jumpfalse,			BasicIns::MakeInt(24))
	/*	17	*/.Ins(BasicIns::stack_offset,		BasicIns::MakeInt(address_of_items))
	/*	18	*/.Ins(BasicIns::read,				BasicIns::pointer_type)
	/*	19	*/.Ins(BasicIns::push,				BasicIns::int_type, BasicIns::MakeInt(sizeof(vint)))
	/*	20	*/.Ins(BasicIns::add,				BasicIns::int_type)
	/*	21	*/.Ins(BasicIns::resptr)
	/*	22	*/.InsG(BasicIns::copymem,			BasicIns::MakeInt(1))
	/*	23	*/.Ins(BasicIns::jump,				BasicIns::MakeInt(37))
	/*	24	*/.Ins(BasicIns::stack_offset,		BasicIns::MakeInt(address_of_items))
	/*	25	*/.Ins(BasicIns::read,				BasicIns::pointer_type)
	/*	26	*/.InsG(BasicIns::push,				BasicIns::int_type, BasicIns::MakeInt(0))
	/*	27	*/.Ins(BasicIns::add,				BasicIns::int_type)
	/*	28	*/.Ins(BasicIns::stack_offset,		BasicIns::MakeInt(address_of_items))
	/*	29	*/.Ins(BasicIns::write,				BasicIns::pointer_type)
	/*	30	*/.Ins(BasicIns::stack_offset,		BasicIns::MakeInt(address_of_i))
	/*	31	*/.Ins(BasicIns::read,				BasicIns::int_type)
	/*	32	*/.Ins(BasicIns::push,				BasicIns::int_type, BasicIns::MakeInt(1))
	/*	33	*/.Ins(BasicIns::add,				BasicIns::int_type)
	/*	34	*/.Ins(BasicIns::stack_offset,		BasicIns::MakeInt(address_of_i))
	/*	35	*/.Ins(BasicIns::write,				BasicIns::int_type)
	/*	36	*/.Ins(BasicIns::jump,				BasicIns::MakeInt(4))
	/*	37	*/.Ins(BasicIns::stack_reserve,		BasicIns::MakeInt(-(vint)sizeof(vint)))
	/*	38	*/.Ins(BasicIns::ret,				BasicIns::MakeInt(3*sizeof(vint)))
	;

	il
	/*	39	*/.Ins(BasicIns::push,				BasicIns::f64, BasicIns::Makef64(90))
	/*	40	*/.Ins(BasicIns::push,				BasicIns::int_type, BasicIns::MakeInt(5))
	/*	41	*/.Ins(BasicIns::push,				BasicIns::f64, BasicIns::Makef64(60))
	/*	42	*/.Ins(BasicIns::push,				BasicIns::int_type, BasicIns::MakeInt(3))
	/*	43	*/.Ins(BasicIns::push,				BasicIns::f64, BasicIns::Makef64(30))
	/*	44	*/.Ins(BasicIns::push,				BasicIns::int_type, BasicIns::MakeInt(1))
	/*	45	*/.Ins(BasicIns::push,				BasicIns::int_type, BasicIns::MakeInt(5))
	/*	46	*/.Ins(BasicIns::push,				BasicIns::int_type, BasicIns::MakeInt(3))
	/*	47	*/.Ins(BasicIns::stack_offset,		BasicIns::MakeInt(-3*(vint)(sizeof(vint)+sizeof(double))))
	/*	48	*/.Ins(BasicIns::resptr)
	/*	49	*/.Ins(BasicIns::generic_callfunc,	BasicIns::MakeInt(0))
	/*	50	*/.Ins(BasicIns::stack_reserve,		BasicIns::MakeInt(-3*(vint)(sizeof(vint)+sizeof(double))))
	/*	51	*/.Ins(BasicIns::ret,				BasicIns::MakeInt(0))
	;

	il
	/*	52	*/.Ins(BasicIns::push,				BasicIns::char_type, BasicIns::Makes8('C'))
	/*	53	*/.Ins(BasicIns::push,				BasicIns::int_type, BasicIns::MakeInt(5))
	/*	54	*/.Ins(BasicIns::push,				BasicIns::char_type, BasicIns::Makes8('B'))
	/*	55	*/.Ins(BasicIns::push,				BasicIns::int_type, BasicIns::MakeInt(4))
	/*	56	*/.Ins(BasicIns::push,				BasicIns::char_type, BasicIns::Makes8('A'))
	/*	57	*/.Ins(BasicIns::push,				BasicIns::int_type, BasicIns::MakeInt(3))
	/*	58	*/.Ins(BasicIns::push,				BasicIns::int_type, BasicIns::MakeInt(4))
	/*	59	*/.Ins(BasicIns::push,				BasicIns::int_type, BasicIns::MakeInt(3))
	/*	60	*/.Ins(BasicIns::stack_offset,		BasicIns::MakeInt(-3*(vint)(sizeof(vint)+sizeof(char))))
	/*	61	*/.Ins(BasicIns::resptr)
	/*	62	*/.Ins(BasicIns::generic_pushfunc,	BasicIns::MakeInt(1))
	/*	63	*/.Ins(BasicIns::label)
	/*	64	*/.Ins(BasicIns::call_indirect,		BasicIns::MakeInt(0))
	/*	65	*/.Ins(BasicIns::stack_reserve,		BasicIns::MakeInt(-3*(vint)(sizeof(vint)+sizeof(char))))
	/*	66	*/.Ins(BasicIns::ret,				BasicIns::MakeInt(0))
	;

	il
	/*	67	*/.Ins(BasicIns::generic_pushfunc,	BasicIns::MakeInt(1))
	/*	68	*/.Ins(BasicIns::generic_pushfunc,	BasicIns::MakeInt(1))
	/*	69	*/.Ins(BasicIns::eq,				BasicIns::int_type)
	/*	70	*/.Ins(BasicIns::resptr)
	/*	71	*/.Ins(BasicIns::write,				BasicIns::bool_type)
	/*	72	*/.Ins(BasicIns::ret,				BasicIns::MakeInt(0))
	;

	{
		BasicILLocalLabel label;
		label.instructionIndex=39;
		il.labels.Add(label);
	}
	{
		BasicILLocalLabel label;
		label.instructionIndex=52;
		il.labels.Add(label);
	}
	{
		BasicILLocalLabel label;
		label.instructionIndex=67;
		il.labels.Add(label);
	}

	BasicILInterpretor interpretor(1024);
	vint key=interpretor.LoadIL(&il);
	BasicILStack stack(&interpretor);
	{
		stack.Reset(il.labels[0].instructionIndex, key, sizeof(double));
		TEST_ASSERT(stack.Run()==ILException::Finished);
		double result=stack.GetEnv()->Pop<double>();
		TEST_ASSERT(result==90);
		TEST_ASSERT(stack.GetEnv()->StackTop()==stack.GetEnv()->StackSize());
	}
	{
		stack.Reset(il.labels[0].instructionIndex, key, sizeof(double));
		TEST_ASSERT(stack.Run()==ILException::Finished);
		double result=stack.GetEnv()->Pop<double>();
		TEST_ASSERT(result==90);
		TEST_ASSERT(stack.GetEnv()->StackTop()==stack.GetEnv()->StackSize());
	}
	{
		stack.Reset(il.labels[1].instructionIndex, key, sizeof(char));
		TEST_ASSERT(stack.Run()==ILException::Finished);
		char result=stack.GetEnv()->Pop<char>();
		TEST_ASSERT(result=='B');
		TEST_ASSERT(stack.GetEnv()->StackTop()==stack.GetEnv()->StackSize());
	}
	{
		stack.Reset(il.labels[1].instructionIndex, key, sizeof(char));
		TEST_ASSERT(stack.Run()==ILException::Finished);
		char result=stack.GetEnv()->Pop<char>();
		TEST_ASSERT(result=='B');
		TEST_ASSERT(stack.GetEnv()->StackTop()==stack.GetEnv()->StackSize());
	}
	{
		stack.Reset(il.labels[2].instructionIndex, key, sizeof(bool));
		TEST_ASSERT(stack.Run()==ILException::Finished);
		bool result=stack.GetEnv()->Pop<bool>();
		TEST_ASSERT(result==true);
		TEST_ASSERT(stack.GetEnv()->StackTop()==stack.GetEnv()->StackSize());
	}
	{
		stack.Reset(il.labels[2].instructionIndex, key, sizeof(bool));
		TEST_ASSERT(stack.Run()==ILException::Finished);
		bool result=stack.GetEnv()->Pop<bool>();
		TEST_ASSERT(result==true);
		TEST_ASSERT(stack.GetEnv()->StackTop()==stack.GetEnv()->StackSize());
	}
}

TEST_CASE(TestBasicILInstruction_Generic_GlobalVariable)
{
	BasicIL il;
	{
		Ptr<ResourceStream> symbolResource=new ResourceStream;
		il.resources.Add(BasicILResourceNames::ExportedSymbols, symbolResource);

		ResourceRecord<BasicILEntryRes> entryRes=symbolResource->CreateRecord<BasicILEntryRes>();
		entryRes->assemblyName=symbolResource->CreateString(L"Generic");
		entryRes->exports=ResourceArrayHandle<BasicILExportRes>::Null();
		entryRes->linkings=ResourceArrayHandle<BasicILLinkingRes>::Null();
		entryRes->foreigns=ResourceArrayHandle<BasicILLinkingRes>::Null();
		ResourceRecord<BasicILGenericRes> genericRes=symbolResource->CreateRecord<BasicILGenericRes>();
		entryRes->genericSymbols=genericRes;

		ResourceRecord<BasicILGenericVariableEntryRes> entry0=symbolResource->CreateRecord<BasicILGenericVariableEntryRes>();
		{
			entry0->genericArgumentCount=1;
			entry0->name=symbolResource->CreateString(L"Storage");
			entry0->uniqueEntryID=symbolResource->CreateString(L"Storage");

			ResourceRecord<BasicILGenericLinearRes> linear=symbolResource->CreateRecord<BasicILGenericLinearRes>();
			{
				linear->constant=0;
				ResourceRecord<BasicILGenericFactorItemRes> factor0=symbolResource->CreateRecord<BasicILGenericFactorItemRes>();
				factor0->factor=1;
				ResourceArrayRecord<BasicILGenericFactorItemRes> factors=symbolResource->CreateArrayRecord<BasicILGenericFactorItemRes>(1);
				factors.Set(0, factor0);
				linear->factors=factors;
			}
			entry0->size=linear;
		}
		ResourceRecord<BasicILGenericTargetRes> target0=symbolResource->CreateRecord<BasicILGenericTargetRes>();
		{
			target0->assemblyName=symbolResource->CreateString(L"Generic");
			target0->symbolName=symbolResource->CreateString(L"Storage");

			ResourceRecord<BasicILGenericArgumentRes> argument0=symbolResource->CreateRecord<BasicILGenericArgumentRes>();
			{
				ResourceRecord<BasicILGenericLinearRes> sizeArgument=symbolResource->CreateRecord<BasicILGenericLinearRes>();
				sizeArgument->constant=sizeof(vint);
				sizeArgument->factors=ResourceArrayHandle<BasicILGenericFactorItemRes>::Null();
				argument0->sizeArgument=sizeArgument;

				ResourceRecord<BasicILGenericNameRes> name=symbolResource->CreateRecord<BasicILGenericNameRes>();
				name->isConstant=true;
				name->constantString=symbolResource->CreateString(L"PRIMITIVE:INTEGER");
				name->stringArgumentIndex=-1;
				argument0->nameArgument=name;

				argument0->subArgument=ResourceArrayHandle<BasicILGenericArgumentRes>::Null();
			}

			ResourceArrayRecord<BasicILGenericArgumentRes> arguments=symbolResource->CreateArrayRecord<BasicILGenericArgumentRes>(1);
			arguments.Set(0, argument0);
			target0->arguments=arguments;
		}
		ResourceRecord<BasicILGenericTargetRes> target1=symbolResource->CreateRecord<BasicILGenericTargetRes>();
		{
			target1->assemblyName=symbolResource->CreateString(L"Generic");
			target1->symbolName=symbolResource->CreateString(L"Storage");

			ResourceRecord<BasicILGenericArgumentRes> argument0=symbolResource->CreateRecord<BasicILGenericArgumentRes>();
			{
				ResourceRecord<BasicILGenericLinearRes> sizeArgument=symbolResource->CreateRecord<BasicILGenericLinearRes>();
				sizeArgument->constant=sizeof(char);
				sizeArgument->factors=ResourceArrayHandle<BasicILGenericFactorItemRes>::Null();
				argument0->sizeArgument=sizeArgument;

				ResourceRecord<BasicILGenericNameRes> name=symbolResource->CreateRecord<BasicILGenericNameRes>();
				name->isConstant=true;
				name->constantString=symbolResource->CreateString(L"PRIMITIVE:CHAR");
				name->stringArgumentIndex=-1;
				argument0->nameArgument=name;

				argument0->subArgument=ResourceArrayHandle<BasicILGenericArgumentRes>::Null();
			}

			ResourceArrayRecord<BasicILGenericArgumentRes> arguments=symbolResource->CreateArrayRecord<BasicILGenericArgumentRes>(1);
			arguments.Set(0, argument0);
			target1->arguments=arguments;
		}

		ResourceArrayRecord<BasicILGenericVariableEntryRes> entries=symbolResource->CreateArrayRecord<BasicILGenericVariableEntryRes>(1);
		entries.Set(0, entry0);
		ResourceArrayRecord<BasicILGenericTargetRes> targets=symbolResource->CreateArrayRecord<BasicILGenericTargetRes>(2);
		targets.Set(0, target0);
		targets.Set(1, target1);
		genericRes->functionEntries=ResourceArrayHandle<BasicILGenericFunctionEntryRes>::Null();
		genericRes->variableEntries=entries;
		genericRes->targets=targets;
		genericRes->linears=ResourceArrayHandle<BasicILGenericLinearRes>::Null();
		genericRes->concepts=ResourceArrayHandle<BasicILGenericConceptRes>::Null();
		genericRes->instances=ResourceArrayHandle<BasicILGenericInstanceRes>::Null();
		genericRes->instanceTargets=ResourceArrayHandle<BasicILGenericInstanceTargetRes>::Null();
	}

	il
		.Ins(BasicIns::push, BasicIns::int_type, BasicIns::MakeInt(10))
		.Ins(BasicIns::generic_pushdata, BasicIns::MakeInt(0))
		.Ins(BasicIns::write, BasicIns::int_type)
		.Ins(BasicIns::push, BasicIns::s8, BasicIns::Makes8(20))
		.Ins(BasicIns::generic_pushdata, BasicIns::MakeInt(1))
		.Ins(BasicIns::write, BasicIns::s8)
		
		.Ins(BasicIns::generic_pushdata, BasicIns::MakeInt(0))
		.Ins(BasicIns::read, BasicIns::int_type)
		.Ins(BasicIns::generic_pushdata, BasicIns::MakeInt(1))
		.Ins(BasicIns::read, BasicIns::s8)
		.Ins(BasicIns::convert, BasicIns::int_type, BasicIns::s8)
		.Ins(BasicIns::add, BasicIns::int_type)
		.Ins(BasicIns::resptr)
		.Ins(BasicIns::write, BasicIns::int_type)

		.Ins(BasicIns::ret);
		;

	BasicILInterpretor interpretor(1024);
	vint key=interpretor.LoadIL(&il);
	BasicILStack stack(&interpretor);
	
	stack.Reset(0, key, sizeof(vint));
	TEST_ASSERT(stack.Run()==ILException::Finished);
	vint result=stack.GetEnv()->Pop<vint>();
	TEST_ASSERT(result==30);
	TEST_ASSERT(stack.GetEnv()->StackTop()==stack.GetEnv()->StackSize());
}