#include "..\..\Library\UnitTest\UnitTest.h"
#include "..\..\Library\Scripting\BasicIL\BasicILDefinition.h"
#include "..\..\Library\Scripting\BasicIL\BasicILInterpretor.h"
#include "..\..\Library\Scripting\BasicIL\BasicILSymbolResource.h"

using namespace vl;
using namespace vl::scripting;
using namespace vl::scripting::basicil;

TEST_CASE(TestBasicILInstructionAppending)
{
	BasicIL il;
	TEST_ASSERT(il.instructions.Count()==0);

	il.Ins(BasicIns::resptr);
	TEST_ASSERT(il.instructions.Count()==1);
	TEST_ASSERT(il.instructions[il.instructions.Count()-1].opcode==BasicIns::resptr);
	TEST_ASSERT(il.instructions[il.instructions.Count()-1].type1==0);
	TEST_ASSERT(il.instructions[il.instructions.Count()-1].type2==0);
	TEST_ASSERT(il.instructions[il.instructions.Count()-1].argument.s64==0);

	il.Ins(BasicIns::resptr, BasicIns::Makeu64(10));
	TEST_ASSERT(il.instructions.Count()==2);
	TEST_ASSERT(il.instructions[il.instructions.Count()-1].opcode==BasicIns::resptr);
	TEST_ASSERT(il.instructions[il.instructions.Count()-1].type1==0);
	TEST_ASSERT(il.instructions[il.instructions.Count()-1].type2==0);
	TEST_ASSERT(il.instructions[il.instructions.Count()-1].argument.u64==10);

	il.Ins(BasicIns::resptr, BasicIns::Makes32(20));
	TEST_ASSERT(il.instructions.Count()==3);
	TEST_ASSERT(il.instructions[il.instructions.Count()-1].opcode==BasicIns::resptr);
	TEST_ASSERT(il.instructions[il.instructions.Count()-1].type1==0);
	TEST_ASSERT(il.instructions[il.instructions.Count()-1].type2==0);
	TEST_ASSERT(il.instructions[il.instructions.Count()-1].argument.s32==20);

	il.Ins(BasicIns::resptr, BasicIns::Makeu8(true));
	TEST_ASSERT(il.instructions.Count()==4);
	TEST_ASSERT(il.instructions[il.instructions.Count()-1].opcode==BasicIns::resptr);
	TEST_ASSERT(il.instructions[il.instructions.Count()-1].type1==0);
	TEST_ASSERT(il.instructions[il.instructions.Count()-1].type2==0);
	TEST_ASSERT(il.instructions[il.instructions.Count()-1].argument.u8==1);

	il.Ins(BasicIns::resptr, BasicIns::Makef32(0.5f));
	TEST_ASSERT(il.instructions.Count()==5);
	TEST_ASSERT(il.instructions[il.instructions.Count()-1].opcode==BasicIns::resptr);
	TEST_ASSERT(il.instructions[il.instructions.Count()-1].type1==0);
	TEST_ASSERT(il.instructions[il.instructions.Count()-1].type2==0);
	TEST_ASSERT(il.instructions[il.instructions.Count()-1].argument.f32==0.5f);

	il.Ins(BasicIns::resptr, BasicIns::Makef64(0.5));
	TEST_ASSERT(il.instructions.Count()==6);
	TEST_ASSERT(il.instructions[il.instructions.Count()-1].opcode==BasicIns::resptr);
	TEST_ASSERT(il.instructions[il.instructions.Count()-1].type1==0);
	TEST_ASSERT(il.instructions[il.instructions.Count()-1].type2==0);
	TEST_ASSERT(il.instructions[il.instructions.Count()-1].argument.f64==0.5);

	il.Ins(BasicIns::resptr, BasicIns::s32);
	TEST_ASSERT(il.instructions.Count()==7);
	TEST_ASSERT(il.instructions[il.instructions.Count()-1].opcode==BasicIns::resptr);
	TEST_ASSERT(il.instructions[il.instructions.Count()-1].type1==BasicIns::s32);
	TEST_ASSERT(il.instructions[il.instructions.Count()-1].type2==0);
	TEST_ASSERT(il.instructions[il.instructions.Count()-1].argument.s64==0);

	il.Ins(BasicIns::resptr, BasicIns::s32, BasicIns::Makef64(0.5));
	TEST_ASSERT(il.instructions.Count()==8);
	TEST_ASSERT(il.instructions[il.instructions.Count()-1].opcode==BasicIns::resptr);
	TEST_ASSERT(il.instructions[il.instructions.Count()-1].type1==BasicIns::s32);
	TEST_ASSERT(il.instructions[il.instructions.Count()-1].type2==0);
	TEST_ASSERT(il.instructions[il.instructions.Count()-1].argument.f64==0.5);

	il.Ins(BasicIns::resptr, BasicIns::s32, BasicIns::s64);
	TEST_ASSERT(il.instructions.Count()==9);
	TEST_ASSERT(il.instructions[il.instructions.Count()-1].opcode==BasicIns::resptr);
	TEST_ASSERT(il.instructions[il.instructions.Count()-1].type1==BasicIns::s32);
	TEST_ASSERT(il.instructions[il.instructions.Count()-1].type2==BasicIns::s64);
	TEST_ASSERT(il.instructions[il.instructions.Count()-1].argument.s64==0);
}

TEST_CASE(TestBasicILInstruction_AddSubMulDiv)
{
	BasicIL il;
	il
		.Ins(BasicIns::push, BasicIns::int_type, BasicIns::MakeInt(40))
		.Ins(BasicIns::push, BasicIns::int_type, BasicIns::MakeInt(30))
		.Ins(BasicIns::add, BasicIns::int_type)
		.Ins(BasicIns::push, BasicIns::int_type, BasicIns::MakeInt(20))
		.Ins(BasicIns::push, BasicIns::int_type, BasicIns::MakeInt(10))
		.Ins(BasicIns::add, BasicIns::int_type)
		.Ins(BasicIns::mul, BasicIns::int_type)
		.Ins(BasicIns::resptr)
		.Ins(BasicIns::write, BasicIns::int_type)
		.Ins(BasicIns::ret, BasicIns::MakeInt(0));

	BasicILInterpretor interpretor(1024);
	vint key=interpretor.LoadIL(&il);

	BasicILStack stack(&interpretor);
	stack.Reset(0, key, sizeof(vint));
	TEST_ASSERT(stack.Run()==BasicILStack::Finished);
	vint result=stack.GetEnv()->Pop<vint>();
	TEST_ASSERT(result==(10+20)*(30+40));
	TEST_ASSERT(stack.GetEnv()->StackTop()==stack.GetEnv()->StackSize());
}

TEST_CASE(TestBasicILInstruction_AddSubMulDiv_Double)
{
	BasicIL il;
	il
		.Ins(BasicIns::push, BasicIns::f64, BasicIns::Makef64(40))
		.Ins(BasicIns::push, BasicIns::f64, BasicIns::Makef64(30))
		.Ins(BasicIns::add, BasicIns::f64)
		.Ins(BasicIns::push, BasicIns::f64, BasicIns::Makef64(20))
		.Ins(BasicIns::push, BasicIns::f64, BasicIns::Makef64(10))
		.Ins(BasicIns::add, BasicIns::f64)
		.Ins(BasicIns::mul, BasicIns::f64)
		.Ins(BasicIns::resptr)
		.Ins(BasicIns::write, BasicIns::f64)
		.Ins(BasicIns::ret, BasicIns::MakeInt(0));

	BasicILInterpretor interpretor(1024);
	vint key=interpretor.LoadIL(&il);
	
	BasicILStack stack(&interpretor);
	stack.Reset(0, key, sizeof(double));
	TEST_ASSERT(stack.Run()==BasicILStack::Finished);
	double result=stack.GetEnv()->Pop<double>();
	TEST_ASSERT(result==(10+20)*(30+40));
	TEST_ASSERT(stack.GetEnv()->StackTop()==stack.GetEnv()->StackSize());
}

TEST_CASE(TestBasicILInstruction_Comparision_Shift_Convert)
{
	BasicIL il;
	il
		.Ins(BasicIns::push, BasicIns::int_type, BasicIns::MakeInt(17))
		.Ins(BasicIns::push, BasicIns::int_type, BasicIns::MakeInt(2))
		.Ins(BasicIns::push, BasicIns::int_type, BasicIns::MakeInt(1))
		.Ins(BasicIns::lt, BasicIns::int_type)
		.Ins(BasicIns::convert, BasicIns::int_type, BasicIns::bool_type)
		.Ins(BasicIns::shl, BasicIns::int_type)

		.Ins(BasicIns::push, BasicIns::int_type, BasicIns::MakeInt(16))
		.Ins(BasicIns::push, BasicIns::int_type, BasicIns::MakeInt(2))
		.Ins(BasicIns::push, BasicIns::int_type, BasicIns::MakeInt(1))
		.Ins(BasicIns::le, BasicIns::int_type)
		.Ins(BasicIns::convert, BasicIns::int_type, BasicIns::bool_type)
		.Ins(BasicIns::shl, BasicIns::int_type)
		.Ins(BasicIns::add, BasicIns::int_type)

		.Ins(BasicIns::push, BasicIns::int_type, BasicIns::MakeInt(15))
		.Ins(BasicIns::push, BasicIns::int_type, BasicIns::MakeInt(2))
		.Ins(BasicIns::push, BasicIns::int_type, BasicIns::MakeInt(1))
		.Ins(BasicIns::gt, BasicIns::int_type)
		.Ins(BasicIns::convert, BasicIns::int_type, BasicIns::bool_type)
		.Ins(BasicIns::shl, BasicIns::int_type)
		.Ins(BasicIns::add, BasicIns::int_type)

		.Ins(BasicIns::push, BasicIns::int_type, BasicIns::MakeInt(14))
		.Ins(BasicIns::push, BasicIns::int_type, BasicIns::MakeInt(2))
		.Ins(BasicIns::push, BasicIns::int_type, BasicIns::MakeInt(1))
		.Ins(BasicIns::ge, BasicIns::int_type)
		.Ins(BasicIns::convert, BasicIns::int_type, BasicIns::bool_type)
		.Ins(BasicIns::shl, BasicIns::int_type)
		.Ins(BasicIns::add, BasicIns::int_type)

		.Ins(BasicIns::push, BasicIns::int_type, BasicIns::MakeInt(13))
		.Ins(BasicIns::push, BasicIns::int_type, BasicIns::MakeInt(2))
		.Ins(BasicIns::push, BasicIns::int_type, BasicIns::MakeInt(1))
		.Ins(BasicIns::eq, BasicIns::int_type)
		.Ins(BasicIns::convert, BasicIns::int_type, BasicIns::bool_type)
		.Ins(BasicIns::shl, BasicIns::int_type)
		.Ins(BasicIns::add, BasicIns::int_type)

		.Ins(BasicIns::push, BasicIns::int_type, BasicIns::MakeInt(12))
		.Ins(BasicIns::push, BasicIns::int_type, BasicIns::MakeInt(2))
		.Ins(BasicIns::push, BasicIns::int_type, BasicIns::MakeInt(1))
		.Ins(BasicIns::ne, BasicIns::int_type)
		.Ins(BasicIns::convert, BasicIns::int_type, BasicIns::bool_type)
		.Ins(BasicIns::shl, BasicIns::int_type)
		.Ins(BasicIns::add, BasicIns::int_type)

		//////////////////////////////////////////////////////////////

		.Ins(BasicIns::push, BasicIns::int_type, BasicIns::MakeInt(11))
		.Ins(BasicIns::push, BasicIns::f64, BasicIns::Makef64(1))
		.Ins(BasicIns::push, BasicIns::f64, BasicIns::Makef64(2))
		.Ins(BasicIns::lt, BasicIns::f64)
		.Ins(BasicIns::convert, BasicIns::int_type, BasicIns::bool_type)
		.Ins(BasicIns::shl, BasicIns::int_type)
		.Ins(BasicIns::add, BasicIns::int_type)

		.Ins(BasicIns::push, BasicIns::int_type, BasicIns::MakeInt(10))
		.Ins(BasicIns::push, BasicIns::f64, BasicIns::Makef64(1))
		.Ins(BasicIns::push, BasicIns::f64, BasicIns::Makef64(2))
		.Ins(BasicIns::le, BasicIns::f64)
		.Ins(BasicIns::convert, BasicIns::int_type, BasicIns::bool_type)
		.Ins(BasicIns::shl, BasicIns::int_type)
		.Ins(BasicIns::add, BasicIns::int_type)

		.Ins(BasicIns::push, BasicIns::int_type, BasicIns::MakeInt(9))
		.Ins(BasicIns::push, BasicIns::f64, BasicIns::Makef64(1))
		.Ins(BasicIns::push, BasicIns::f64, BasicIns::Makef64(2))
		.Ins(BasicIns::gt, BasicIns::f64)
		.Ins(BasicIns::convert, BasicIns::int_type, BasicIns::bool_type)
		.Ins(BasicIns::shl, BasicIns::int_type)
		.Ins(BasicIns::add, BasicIns::int_type)

		.Ins(BasicIns::push, BasicIns::int_type, BasicIns::MakeInt(8))
		.Ins(BasicIns::push, BasicIns::f64, BasicIns::Makef64(1))
		.Ins(BasicIns::push, BasicIns::f64, BasicIns::Makef64(2))
		.Ins(BasicIns::ge, BasicIns::f64)
		.Ins(BasicIns::convert, BasicIns::int_type, BasicIns::bool_type)
		.Ins(BasicIns::shl, BasicIns::int_type)
		.Ins(BasicIns::add, BasicIns::int_type)

		.Ins(BasicIns::push, BasicIns::int_type, BasicIns::MakeInt(7))
		.Ins(BasicIns::push, BasicIns::f64, BasicIns::Makef64(1))
		.Ins(BasicIns::push, BasicIns::f64, BasicIns::Makef64(2))
		.Ins(BasicIns::eq, BasicIns::f64)
		.Ins(BasicIns::convert, BasicIns::int_type, BasicIns::bool_type)
		.Ins(BasicIns::shl, BasicIns::int_type)
		.Ins(BasicIns::add, BasicIns::int_type)

		.Ins(BasicIns::push, BasicIns::int_type, BasicIns::MakeInt(6))
		.Ins(BasicIns::push, BasicIns::f64, BasicIns::Makef64(1))
		.Ins(BasicIns::push, BasicIns::f64, BasicIns::Makef64(2))
		.Ins(BasicIns::ne, BasicIns::f64)
		.Ins(BasicIns::convert, BasicIns::int_type, BasicIns::bool_type)
		.Ins(BasicIns::shl, BasicIns::int_type)
		.Ins(BasicIns::add, BasicIns::int_type)

		//////////////////////////////////////////////////////////////

		.Ins(BasicIns::push, BasicIns::int_type, BasicIns::MakeInt(5))
		.Ins(BasicIns::push, BasicIns::f64, BasicIns::Makef64(1))
		.Ins(BasicIns::push, BasicIns::f64, BasicIns::Makef64(2))
		.Ins(BasicIns::lt, BasicIns::f64)
		.Ins(BasicIns::convert, BasicIns::int_type, BasicIns::bool_type)
		.Ins(BasicIns::shl, BasicIns::int_type)
		.Ins(BasicIns::add, BasicIns::int_type)

		.Ins(BasicIns::push, BasicIns::int_type, BasicIns::MakeInt(4))
		.Ins(BasicIns::push, BasicIns::u8, BasicIns::Makeu8(1))
		.Ins(BasicIns::push, BasicIns::u8, BasicIns::Makeu8(1))
		.Ins(BasicIns::le, BasicIns::u8)
		.Ins(BasicIns::convert, BasicIns::int_type, BasicIns::bool_type)
		.Ins(BasicIns::shl, BasicIns::int_type)
		.Ins(BasicIns::add, BasicIns::int_type)

		.Ins(BasicIns::push, BasicIns::int_type, BasicIns::MakeInt(3))
		.Ins(BasicIns::push, BasicIns::u8, BasicIns::Makeu8(1))
		.Ins(BasicIns::push, BasicIns::u8, BasicIns::Makeu8(1))
		.Ins(BasicIns::gt, BasicIns::u8)
		.Ins(BasicIns::convert, BasicIns::int_type, BasicIns::bool_type)
		.Ins(BasicIns::shl, BasicIns::int_type)
		.Ins(BasicIns::add, BasicIns::int_type)

		.Ins(BasicIns::push, BasicIns::int_type, BasicIns::MakeInt(2))
		.Ins(BasicIns::push, BasicIns::u8, BasicIns::Makeu8(1))
		.Ins(BasicIns::push, BasicIns::u8, BasicIns::Makeu8(1))
		.Ins(BasicIns::ge, BasicIns::u8)
		.Ins(BasicIns::convert, BasicIns::int_type, BasicIns::bool_type)
		.Ins(BasicIns::shl, BasicIns::int_type)
		.Ins(BasicIns::add, BasicIns::int_type)

		.Ins(BasicIns::push, BasicIns::int_type, BasicIns::MakeInt(1))
		.Ins(BasicIns::push, BasicIns::u8, BasicIns::Makeu8(1))
		.Ins(BasicIns::push, BasicIns::u8, BasicIns::Makeu8(1))
		.Ins(BasicIns::eq, BasicIns::u8)
		.Ins(BasicIns::convert, BasicIns::int_type, BasicIns::bool_type)
		.Ins(BasicIns::shl, BasicIns::int_type)
		.Ins(BasicIns::add, BasicIns::int_type)

		.Ins(BasicIns::push, BasicIns::int_type, BasicIns::MakeInt(0))
		.Ins(BasicIns::push, BasicIns::u8, BasicIns::Makeu8(1))
		.Ins(BasicIns::push, BasicIns::u8, BasicIns::Makeu8(1))
		.Ins(BasicIns::ne, BasicIns::u8)
		.Ins(BasicIns::convert, BasicIns::int_type, BasicIns::bool_type)
		.Ins(BasicIns::shl, BasicIns::int_type)
		.Ins(BasicIns::add, BasicIns::int_type)

		//////////////////////////////////////////////////////////////

		.Ins(BasicIns::resptr)
		.Ins(BasicIns::write, BasicIns::int_type)
		.Ins(BasicIns::ret, BasicIns::MakeInt(0));

	BasicILInterpretor interpretor(1024);
	vint key=interpretor.LoadIL(&il);

	BasicILStack stack(&interpretor);
	stack.Reset(0, key, sizeof(vint));
	TEST_ASSERT(stack.Run()==BasicILStack::Finished);
	vint result=stack.GetEnv()->Pop<vint>();
	vint baseline=0;
	baseline+=1<<17;
	baseline+=1<<16;
	baseline+=1<<12;
	baseline+=1<<9;
	baseline+=1<<8;
	baseline+=1<<6;
	baseline+=1<<4;
	baseline+=1<<2;
	baseline+=1<<1;
	TEST_ASSERT(result==baseline);
	TEST_ASSERT(stack.GetEnv()->StackTop()==stack.GetEnv()->StackSize());
}

TEST_CASE(TestBasicILInstruction_Jump_Variable)
{
	BasicIL il;
	il
		// vint i=0
		.Ins(BasicIns::stack_reserve, BasicIns::MakeInt(sizeof(vint)))
		.Ins(BasicIns::push, BasicIns::int_type, BasicIns::MakeInt(0))
		.Ins(BasicIns::stack_offset, BasicIns::MakeInt(-(vint)sizeof(vint)))
		.Ins(BasicIns::write, BasicIns::int_type)
		// result=0
		.Ins(BasicIns::push, BasicIns::int_type, BasicIns::MakeInt(0))
		.Ins(BasicIns::resptr)
		.Ins(BasicIns::write, BasicIns::int_type)
		// BEGIN_LOOP:
		// if(i==100)jump END_LOOP
		.Ins(BasicIns::push, BasicIns::int_type, BasicIns::MakeInt(10))
		.Ins(BasicIns::stack_offset, BasicIns::MakeInt(-(vint)sizeof(vint)))
		.Ins(BasicIns::read, BasicIns::int_type)
		.Ins(BasicIns::eq, BasicIns::int_type)
		.Ins(BasicIns::jumptrue, BasicIns::MakeInt(26))
		// i+=1
		.Ins(BasicIns::push, BasicIns::int_type, BasicIns::MakeInt(1))
		.Ins(BasicIns::stack_offset, BasicIns::MakeInt(-(vint)sizeof(vint)))
		.Ins(BasicIns::read, BasicIns::int_type)
		.Ins(BasicIns::add, BasicIns::int_type)
		.Ins(BasicIns::stack_offset, BasicIns::MakeInt(-(vint)sizeof(vint)))
		.Ins(BasicIns::write, BasicIns::int_type)
		// result+=i
		.Ins(BasicIns::resptr)
		.Ins(BasicIns::read, BasicIns::int_type)
		.Ins(BasicIns::stack_offset, BasicIns::MakeInt(-(vint)sizeof(vint)))
		.Ins(BasicIns::read, BasicIns::int_type)
		.Ins(BasicIns::add, BasicIns::int_type)
		.Ins(BasicIns::resptr)
		.Ins(BasicIns::write, BasicIns::int_type)
		// jump BEGIN_LOOP
		.Ins(BasicIns::jump, BasicIns::MakeInt(7))
		// END_LOOP:
		// exit
		.Ins(BasicIns::stack_reserve, BasicIns::MakeInt(-(vint)sizeof(vint)))
		.Ins(BasicIns::ret, BasicIns::MakeInt(0));

	BasicILInterpretor interpretor(1024);
	vint key=interpretor.LoadIL(&il);

	BasicILStack stack(&interpretor);
	stack.Reset(0, key, sizeof(vint));
	TEST_ASSERT(stack.Run()==BasicILStack::Finished);
	vint result=stack.GetEnv()->Pop<vint>();
	TEST_ASSERT(result==55);
	TEST_ASSERT(stack.GetEnv()->StackTop()==stack.GetEnv()->StackSize());
}

TEST_CASE(TestBasicILInstruction_Recursion)
{
	BasicIL il;
	il
		// main:
		// fab(4)
		.Ins(BasicIns::push, BasicIns::int_type, BasicIns::MakeInt(9))
		.Ins(BasicIns::resptr)
		.Ins(BasicIns::call, BasicIns::MakeInt(4))
		// exit;
		.Ins(BasicIns::ret, BasicIns::MakeInt(0))
		// fab:
		.Ins(BasicIns::stack_reserve, BasicIns::MakeInt(2*sizeof(vint)))
		// if(n<2)
		.Ins(BasicIns::push, BasicIns::int_type, BasicIns::MakeInt(2))
		.Ins(BasicIns::stack_offset, BasicIns::MakeInt(4*sizeof(vint)))
		.Ins(BasicIns::read, BasicIns::int_type)
		.Ins(BasicIns::lt, BasicIns::int_type)
		.Ins(BasicIns::jumpfalse, BasicIns::MakeInt(15))
		// result=1
		.Ins(BasicIns::push, BasicIns::int_type, BasicIns::MakeInt(1))
		.Ins(BasicIns::resptr)
		.Ins(BasicIns::write, BasicIns::int_type)
		.Ins(BasicIns::stack_reserve, BasicIns::MakeInt(-2*(vint)sizeof(vint)))
		.Ins(BasicIns::ret, BasicIns::MakeInt(sizeof(vint)))
		// else
		// result=fab(n-2)+fab(n-1)
		.Ins(BasicIns::push, BasicIns::int_type, BasicIns::MakeInt(2))
		.Ins(BasicIns::stack_offset, BasicIns::MakeInt(4*sizeof(vint)))
		.Ins(BasicIns::read, BasicIns::int_type)
		.Ins(BasicIns::sub, BasicIns::int_type)
		.Ins(BasicIns::stack_offset, BasicIns::MakeInt(-(vint)sizeof(vint)))
		.Ins(BasicIns::call, BasicIns::MakeInt(4))
		
		.Ins(BasicIns::push, BasicIns::int_type, BasicIns::MakeInt(1))
		.Ins(BasicIns::stack_offset, BasicIns::MakeInt(4*sizeof(vint)))
		.Ins(BasicIns::read, BasicIns::int_type)
		.Ins(BasicIns::sub, BasicIns::int_type)
		.Ins(BasicIns::stack_offset, BasicIns::MakeInt(-2*(vint)sizeof(vint)))
		.Ins(BasicIns::call, BasicIns::MakeInt(4))
		
		.Ins(BasicIns::stack_offset, BasicIns::MakeInt(-(vint)sizeof(vint)))
		.Ins(BasicIns::read, BasicIns::int_type)
		.Ins(BasicIns::stack_offset, BasicIns::MakeInt(-2*(vint)sizeof(vint)))
		.Ins(BasicIns::read, BasicIns::int_type)
		.Ins(BasicIns::add, BasicIns::int_type)
		
		.Ins(BasicIns::resptr)
		.Ins(BasicIns::write, BasicIns::int_type)
		.Ins(BasicIns::stack_reserve, BasicIns::MakeInt(-2*(vint)sizeof(vint)))
		.Ins(BasicIns::ret, BasicIns::MakeInt(sizeof(vint)));
		// exit;

	BasicILInterpretor interpretor(1024);
	vint key=interpretor.LoadIL(&il);

	BasicILStack stack(&interpretor);
	stack.Reset(0, key, sizeof(vint));
	TEST_ASSERT(stack.Run()==BasicILStack::Finished);
	vint result=stack.GetEnv()->Pop<vint>();
	TEST_ASSERT(result==55);
	TEST_ASSERT(stack.GetEnv()->StackTop()==stack.GetEnv()->StackSize());
}

TEST_CASE(TestBasicILInstruction_FunctionPointer)
{
	BasicIL il;
	il
		.Ins(BasicIns::stack_reserve, BasicIns::MakeInt(2*sizeof(vint)))

		.Ins(BasicIns::push, BasicIns::int_type, BasicIns::MakeInt(40))
		.Ins(BasicIns::push, BasicIns::int_type, BasicIns::MakeInt(30))
		.Ins(BasicIns::stack_offset, BasicIns::MakeInt(-(vint)sizeof(vint)))
		.Ins(BasicIns::pushins, BasicIns::MakeInt(20))
		.Ins(BasicIns::call_indirect)

		.Ins(BasicIns::push, BasicIns::int_type, BasicIns::MakeInt(20))
		.Ins(BasicIns::push, BasicIns::int_type, BasicIns::MakeInt(10))
		.Ins(BasicIns::stack_offset, BasicIns::MakeInt(-2*(vint)sizeof(vint)))
		.Ins(BasicIns::pushins, BasicIns::MakeInt(20))
		.Ins(BasicIns::call_indirect)
		
		.Ins(BasicIns::stack_offset, BasicIns::MakeInt(-(vint)sizeof(vint)))
		.Ins(BasicIns::read, BasicIns::int_type)
		.Ins(BasicIns::stack_offset, BasicIns::MakeInt(-2*(vint)sizeof(vint)))
		.Ins(BasicIns::read, BasicIns::int_type)
		.Ins(BasicIns::mul, BasicIns::int_type)

		.Ins(BasicIns::resptr)
		.Ins(BasicIns::write, BasicIns::int_type)
		.Ins(BasicIns::stack_reserve, BasicIns::MakeInt(-2*(vint)sizeof(vint)))
		.Ins(BasicIns::ret, BasicIns::MakeInt(0))

		.Ins(BasicIns::stack_offset, BasicIns::MakeInt(4*sizeof(vint)))
		.Ins(BasicIns::read, BasicIns::int_type)
		.Ins(BasicIns::stack_offset, BasicIns::MakeInt(5*sizeof(vint)))
		.Ins(BasicIns::read, BasicIns::int_type)
		.Ins(BasicIns::add, BasicIns::int_type)
		.Ins(BasicIns::resptr)
		.Ins(BasicIns::write, BasicIns::int_type)
		.Ins(BasicIns::ret, BasicIns::MakeInt(2*sizeof(vint)));

	BasicILInterpretor interpretor(1024);
	vint key=interpretor.LoadIL(&il);

	BasicILStack stack(&interpretor);
	stack.Reset(0, key, sizeof(vint));
	TEST_ASSERT(stack.Run()==BasicILStack::Finished);
	vint result=stack.GetEnv()->Pop<vint>();
	TEST_ASSERT(result==(10+20)*(30+40));
	TEST_ASSERT(stack.GetEnv()->StackTop()==stack.GetEnv()->StackSize());
}

TEST_CASE(TestBasicILInstruction_FunctionPointerInLabel)
{
	BasicIL il;
	il
		.Ins(BasicIns::stack_reserve, BasicIns::MakeInt(2*sizeof(vint)))

		.Ins(BasicIns::push, BasicIns::int_type, BasicIns::MakeInt(40))
		.Ins(BasicIns::push, BasicIns::int_type, BasicIns::MakeInt(30))
		.Ins(BasicIns::stack_offset, BasicIns::MakeInt(-(vint)sizeof(vint)))
		.Ins(BasicIns::pushlabel, BasicIns::MakeInt(1))
		.Ins(BasicIns::label)
		.Ins(BasicIns::call_indirect)

		.Ins(BasicIns::push, BasicIns::int_type, BasicIns::MakeInt(20))
		.Ins(BasicIns::push, BasicIns::int_type, BasicIns::MakeInt(10))
		.Ins(BasicIns::stack_offset, BasicIns::MakeInt(-2*(vint)sizeof(vint)))
		.Ins(BasicIns::pushlabel, BasicIns::MakeInt(1))
		.Ins(BasicIns::label)
		.Ins(BasicIns::call_indirect)
		
		.Ins(BasicIns::stack_offset, BasicIns::MakeInt(-(vint)sizeof(vint)))
		.Ins(BasicIns::read, BasicIns::int_type)
		.Ins(BasicIns::stack_offset, BasicIns::MakeInt(-2*(vint)sizeof(vint)))
		.Ins(BasicIns::read, BasicIns::int_type)
		.Ins(BasicIns::mul, BasicIns::int_type)

		.Ins(BasicIns::resptr)
		.Ins(BasicIns::write, BasicIns::int_type)
		.Ins(BasicIns::stack_reserve, BasicIns::MakeInt(-2*(vint)sizeof(vint)))
		.Ins(BasicIns::ret, BasicIns::MakeInt(0))

		.Ins(BasicIns::stack_offset, BasicIns::MakeInt(4*sizeof(vint)))
		.Ins(BasicIns::read, BasicIns::int_type)
		.Ins(BasicIns::stack_offset, BasicIns::MakeInt(5*sizeof(vint)))
		.Ins(BasicIns::read, BasicIns::int_type)
		.Ins(BasicIns::add, BasicIns::int_type)
		.Ins(BasicIns::resptr)
		.Ins(BasicIns::write, BasicIns::int_type)
		.Ins(BasicIns::ret, BasicIns::MakeInt(2*sizeof(vint)));

	BasicILInterpretor interpretor(1024);
	vint key=interpretor.LoadIL(&il);
	BasicILLabel label;
	label.key=key;
	label.instruction=22;
	interpretor.GetLabels().Add(label);

	BasicILStack stack(&interpretor);
	stack.Reset(0, key, sizeof(vint));
	TEST_ASSERT(stack.Run()==BasicILStack::Finished);
	vint result=stack.GetEnv()->Pop<vint>();
	TEST_ASSERT(result==(10+20)*(30+40));
	TEST_ASSERT(stack.GetEnv()->StackTop()==stack.GetEnv()->StackSize());
}

TEST_CASE(TestBasicILInstruction_Linking)
{
	BasicIL iladd, ilmain;
	{
		iladd
			.Ins(BasicIns::stack_offset, BasicIns::MakeInt(5*sizeof(vint)))
			.Ins(BasicIns::read, BasicIns::int_type)
			.Ins(BasicIns::stack_offset, BasicIns::MakeInt(4*sizeof(vint)))
			.Ins(BasicIns::read, BasicIns::int_type)
			.Ins(BasicIns::add, BasicIns::int_type)
			.Ins(BasicIns::resptr)
			.Ins(BasicIns::write, BasicIns::int_type)
			.Ins(BasicIns::ret, BasicIns::MakeInt(2*sizeof(vint)))
			;

		BasicILLocalLabel label;
		label.instructionIndex=0;
		iladd.labels.Add(label);

		Ptr<ResourceStream> resource=new ResourceStream;
		iladd.resources.Add(BasicILResourceNames::ExportedSymbols, resource);
		
		ResourceRecord<BasicILEntryRes> entry=resource->CreateRecord<BasicILEntryRes>();
		entry->assemblyName=resource->CreateString(L"Add1.0");
		entry->genericSymbols=ResourceHandle<BasicILGenericRes>::Null();
		entry->linkings=ResourceArrayHandle<BasicILLinkingRes>::Null();
		entry->foreigns=ResourceArrayHandle<BasicILLinkingRes>::Null();

		ResourceRecord<BasicILExportRes> exportAdd=resource->CreateRecord<BasicILExportRes>();
		exportAdd->name=resource->CreateString(L"Add");
		exportAdd->address=0;

		ResourceArrayRecord<BasicILExportRes> exportArray=resource->CreateArrayRecord<BasicILExportRes>(1);
		exportArray[0]=exportAdd;
		entry->exports=exportArray;
	}
	{
		ilmain
			.Ins(BasicIns::push, BasicIns::int_type, BasicIns::MakeInt(1))
			.Ins(BasicIns::push, BasicIns::int_type, BasicIns::MakeInt(2))
			.Ins(BasicIns::resptr)
			.Ins(BasicIns::link_callfarfunc, BasicIns::MakeInt(0))
			.Ins(BasicIns::ret, BasicIns::MakeInt(0))
			;

		BasicILLocalLabel label;
		label.instructionIndex=0;
		ilmain.labels.Add(label);

		Ptr<ResourceStream> resource=new ResourceStream;
		ilmain.resources.Add(BasicILResourceNames::ExportedSymbols, resource);
		
		ResourceRecord<BasicILEntryRes> entry=resource->CreateRecord<BasicILEntryRes>();
		entry->assemblyName=resource->CreateString(L"Main1.0");
		entry->genericSymbols=ResourceHandle<BasicILGenericRes>::Null();
		entry->exports=ResourceArrayHandle<BasicILExportRes>::Null();
		entry->foreigns=ResourceArrayHandle<BasicILLinkingRes>::Null();
		
		ResourceRecord<BasicILLinkingRes> linkingAdd=resource->CreateRecord<BasicILLinkingRes>();
		linkingAdd->assemblyName=resource->CreateString(L"Add1.0");
		linkingAdd->symbolName=resource->CreateString(L"Add");

		ResourceArrayRecord<BasicILLinkingRes> linkingArray=resource->CreateArrayRecord<BasicILLinkingRes>(1);
		linkingArray[0]=linkingAdd;
		entry->linkings=linkingArray;
	}

	BasicILInterpretor interpretor(1024);
	interpretor.LoadIL(&iladd);
	vint key=interpretor.LoadIL(&ilmain);

	BasicILStack stack(&interpretor);
	stack.Reset(0, key, sizeof(vint));
	TEST_ASSERT(stack.Run()==BasicILStack::Finished);
	vint result=stack.GetEnv()->Pop<vint>();
	TEST_ASSERT(result==3);
	TEST_ASSERT(stack.GetEnv()->StackTop()==stack.GetEnv()->StackSize());
}

namespace mynamespace
{
	struct Node
	{
		vint							data;
		ResourceString				empty;
		ResourceString				name;
		ResourceHandle<Node>		next;
	};
}

using namespace mynamespace;

TEST_CASE(TestBasicILResourceManager_Primitive)
{
	ResourceStream resource;
	{
		ResourceRecord<Node> lastNode;
		for(vint i=0;i<10;i++)
		{
			ResourceRecord<Node> currentNode=resource.CreateRecord<Node>();
			ResourceString name=resource.CreateString(L"ID:"+itow(i));
			currentNode->data=i;
			currentNode->empty=ResourceString::Null();
			currentNode->name=name;
			currentNode->next=ResourceHandle<Node>::Null();
			if(lastNode)
			{
				lastNode->next=currentNode;
				lastNode=currentNode;
			}
			lastNode=currentNode;
		}
	}
	{
		ResourceRecord<Node> currentNode=resource.ReadRootRecord<Node>();
		vint index=0;
		while(currentNode)
		{
			TEST_ASSERT(index==currentNode->data);
			TEST_ASSERT(resource.ReadString(currentNode->empty)==L"");
			TEST_ASSERT(resource.ReadString(currentNode->name)==L"ID:"+itow(index));
			index++;
			currentNode=resource.ReadRecord(currentNode->next);
		}
		TEST_ASSERT(index==10);
	}
}

TEST_CASE(TestBasicILResourceManager_Array)
{
	ResourceStream resource;
	{
		ResourceArrayRecord<Node> nodes=resource.CreateArrayRecord<Node>(10);
		TEST_ASSERT(nodes.Count()==10);
		for(vint i=0;i<10;i++)
		{
			ResourceRecord<Node> currentNode=resource.CreateRecord<Node>();
			ResourceString name=resource.CreateString(L"ID:"+itow(i));
			currentNode->data=i;
			currentNode->empty=ResourceString::Null();
			currentNode->name=name;
			currentNode->next=ResourceHandle<Node>::Null();
			nodes[i]=currentNode;
		}
	}
	{
		ResourceArrayRecord<Node> nodes=resource.ReadRootArrayRecord<Node>();
		TEST_ASSERT(nodes.Count()==10);
		for(vint i=0;i<10;i++)
		{
			ResourceRecord<Node> currentNode=nodes[i];
			TEST_ASSERT(i==currentNode->data);
			TEST_ASSERT(resource.ReadString(currentNode->empty)==L"");
			TEST_ASSERT(resource.ReadString(currentNode->name)==L"ID:"+itow(i));
			TEST_ASSERT(!currentNode->next);
		}
	}
}