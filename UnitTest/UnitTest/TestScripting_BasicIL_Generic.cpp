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
		ResourceRecord<BasicILGenericRes> genericRes=symbolResource->CreateRecord<BasicILGenericRes>();
		entryRes->genericSymbols=genericRes;

		ResourceRecord<BasicILGenericFunctionEntryRes> entry0=symbolResource->CreateRecord<BasicILGenericFunctionEntryRes>();
		{
			entry0->genericArgumentCount=1;
			entry0->startInstruction=0;
			entry0->instructionCount=39;
			entry0->name=symbolResource->CreateString(L"FindFirst");
			ResourceRecord<BasicILGenericNameRes> name0=symbolResource->CreateRecord<BasicILGenericNameRes>();
			name0->isConstant=true;
			name0->constantString=symbolResource->CreateString(L"FindFirst<");
			name0->stringArgumentIndex=-1;
			ResourceRecord<BasicILGenericNameRes> name1=symbolResource->CreateRecord<BasicILGenericNameRes>();
			name1->isConstant=false;
			name1->constantString=ResourceString::Null();
			name1->stringArgumentIndex=0;
			ResourceRecord<BasicILGenericNameRes> name2=symbolResource->CreateRecord<BasicILGenericNameRes>();
			name2->isConstant=true;
			name2->constantString=symbolResource->CreateString(L">");
			name2->stringArgumentIndex=-1;
			ResourceArrayRecord<BasicILGenericNameRes> names=symbolResource->CreateArrayRecord<BasicILGenericNameRes>(3);
			names.Set(0, name0);
			names.Set(1, name1);
			names.Set(2, name2);
			entry0->uniqueNameTemplate=names;
		}
		ResourceRecord<BasicILGenericFunctionTargetRes> target0=symbolResource->CreateRecord<BasicILGenericFunctionTargetRes>();
		{
			target0->assemblyName=symbolResource->CreateString(L"Map");
			target0->symbolName=symbolResource->CreateString(L"FindFirst");

			ResourceRecord<BasicILGenericArgumentRes> argument0=symbolResource->CreateRecord<BasicILGenericArgumentRes>();
			{
				argument0->sizeArgument=sizeof(double);
				ResourceRecord<BasicILGenericNameRes> name0=symbolResource->CreateRecord<BasicILGenericNameRes>();
				name0->isConstant=true;
				name0->constantString=symbolResource->CreateString(L"PRIMITIVE:DOUBLE");
				name0->stringArgumentIndex=-1;
				ResourceArrayRecord<BasicILGenericNameRes> names=symbolResource->CreateArrayRecord<BasicILGenericNameRes>(1);
				names.Set(0, name0);
				argument0->nameArgument=names;
			}

			ResourceArrayRecord<BasicILGenericArgumentRes> arguments=symbolResource->CreateArrayRecord<BasicILGenericArgumentRes>(1);
			arguments.Set(0, argument0);
			target0->arguments=arguments;
		}
		ResourceRecord<BasicILGenericFunctionTargetRes> target1=symbolResource->CreateRecord<BasicILGenericFunctionTargetRes>();
		{
			target1->assemblyName=symbolResource->CreateString(L"Map");
			target1->symbolName=symbolResource->CreateString(L"FindFirst");

			ResourceRecord<BasicILGenericArgumentRes> argument0=symbolResource->CreateRecord<BasicILGenericArgumentRes>();
			{
				argument0->sizeArgument=sizeof(vint);
				ResourceRecord<BasicILGenericNameRes> name0=symbolResource->CreateRecord<BasicILGenericNameRes>();
				name0->isConstant=true;
				name0->constantString=symbolResource->CreateString(L"PRIMITIVE:INTEGER");
				name0->stringArgumentIndex=-1;
				ResourceArrayRecord<BasicILGenericNameRes> names=symbolResource->CreateArrayRecord<BasicILGenericNameRes>(1);
				names.Set(0, name0);
				argument0->nameArgument=names;
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
		ResourceArrayRecord<BasicILGenericFunctionTargetRes> targets=symbolResource->CreateArrayRecord<BasicILGenericFunctionTargetRes>(2);
		targets.Set(0, target0);
		targets.Set(1, target1);
		ResourceArrayRecord<BasicILGenericLinearRes> linears=symbolResource->CreateArrayRecord<BasicILGenericLinearRes>(2);
		linears.Set(0, linear0);
		linears.Set(0, linear1);
		genericRes->functionEntries=entries;
		genericRes->functionTargets=targets;
		genericRes->linears=linears;
	}

	const vint address_of_i=-(vint)sizeof(vint);
	const vint address_of_items=4*sizeof(vint);
	const vint address_of_count=5*sizeof(vint);
	const vint address_of_key=6*sizeof(vint);

	//T FindFirst<T>((int+T)* items, int count, int key);
	il
	/*	0	*/.Ins(BasicIns::stack_reserve,		BasicIns::MakeInt(sizeof(vint)))
	/*	1	*/.Ins(BasicIns::stack_offset,		BasicIns::MakeInt(address_of_i))
	/*	2	*/.Ins(BasicIns::push,				BasicIns::int_type, BasicIns::MakeInt(0))
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
	/*	22	*/.InsG(BasicIns::writemem,			BasicIns::MakeInt(1))
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
	/*	37	*/.Ins(BasicIns::stack_reserve,		BasicIns::MakeInt(sizeof(vint)))
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
	/*	47	*/.Ins(BasicIns::stack_offset,		BasicIns::MakeInt(3*(sizeof(vint)+sizeof(double))))
	/*	48	*/.Ins(BasicIns::resptr)
	/*	49	*/.Ins(BasicIns::generic_callfunc,	BasicIns::MakeInt(0))
	/*	50	*/.Ins(BasicIns::stack_reserve,		BasicIns::MakeInt(3*(sizeof(vint)+sizeof(double))))
	/*	51	*/.Ins(BasicIns::ret,				BasicIns::MakeInt(0))
	;

	{
		BasicILLocalLabel label;
		label.instructionIndex=39;
		il.labels.Add(label);
	}

	BasicILInterpretor interpretor(1024);
	vint key=interpretor.LoadIL(&il);

	BasicILStack stack(&interpretor);
	stack.Reset(il.labels[0].instructionIndex, key, sizeof(double));
	TEST_ASSERT(stack.Run()==BasicILStack::Finished);
	vint result=stack.GetEnv()->Pop<vint>();
	TEST_ASSERT(result==90);
	TEST_ASSERT(stack.GetEnv()->StackTop()==stack.GetEnv()->StackSize());
}

TEST_CASE(TestBasicILInstruction_Generic_FunctionPointer)
{
}