#include "..\..\Library\UnitTest\UnitTest.h"
#include "..\..\Library\Scripting\BasicIL\BasicILDefinition.h"

using namespace vl;
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
	TEST_ASSERT(il.instructions[il.instructions.Count()-1].argint==0);
	TEST_ASSERT(il.instructions[il.instructions.Count()-1].argf32==0);
	TEST_ASSERT(il.instructions[il.instructions.Count()-1].argf64==0);

	il.Ins(BasicIns::resptr, (unsigned __int64)10);
	TEST_ASSERT(il.instructions.Count()==2);
	TEST_ASSERT(il.instructions[il.instructions.Count()-1].opcode==BasicIns::resptr);
	TEST_ASSERT(il.instructions[il.instructions.Count()-1].type1==0);
	TEST_ASSERT(il.instructions[il.instructions.Count()-1].type2==0);
	TEST_ASSERT(il.instructions[il.instructions.Count()-1].argint==10);
	TEST_ASSERT(il.instructions[il.instructions.Count()-1].argf32==0);
	TEST_ASSERT(il.instructions[il.instructions.Count()-1].argf64==0);

	il.Ins(BasicIns::resptr, 20);
	TEST_ASSERT(il.instructions.Count()==3);
	TEST_ASSERT(il.instructions[il.instructions.Count()-1].opcode==BasicIns::resptr);
	TEST_ASSERT(il.instructions[il.instructions.Count()-1].type1==0);
	TEST_ASSERT(il.instructions[il.instructions.Count()-1].type2==0);
	TEST_ASSERT(il.instructions[il.instructions.Count()-1].argint==20);
	TEST_ASSERT(il.instructions[il.instructions.Count()-1].argf32==0);
	TEST_ASSERT(il.instructions[il.instructions.Count()-1].argf64==0);

	il.Ins(BasicIns::resptr, true);
	TEST_ASSERT(il.instructions.Count()==4);
	TEST_ASSERT(il.instructions[il.instructions.Count()-1].opcode==BasicIns::resptr);
	TEST_ASSERT(il.instructions[il.instructions.Count()-1].type1==0);
	TEST_ASSERT(il.instructions[il.instructions.Count()-1].type2==0);
	TEST_ASSERT(il.instructions[il.instructions.Count()-1].argint==1);
	TEST_ASSERT(il.instructions[il.instructions.Count()-1].argf32==0);
	TEST_ASSERT(il.instructions[il.instructions.Count()-1].argf64==0);

	il.Ins(BasicIns::resptr, 0.5f);
	TEST_ASSERT(il.instructions.Count()==5);
	TEST_ASSERT(il.instructions[il.instructions.Count()-1].opcode==BasicIns::resptr);
	TEST_ASSERT(il.instructions[il.instructions.Count()-1].type1==0);
	TEST_ASSERT(il.instructions[il.instructions.Count()-1].type2==0);
	TEST_ASSERT(il.instructions[il.instructions.Count()-1].argint==0);
	TEST_ASSERT(il.instructions[il.instructions.Count()-1].argf32==0.5f);
	TEST_ASSERT(il.instructions[il.instructions.Count()-1].argf64==0);

	il.Ins(BasicIns::resptr, 0.5);
	TEST_ASSERT(il.instructions.Count()==6);
	TEST_ASSERT(il.instructions[il.instructions.Count()-1].opcode==BasicIns::resptr);
	TEST_ASSERT(il.instructions[il.instructions.Count()-1].type1==0);
	TEST_ASSERT(il.instructions[il.instructions.Count()-1].type2==0);
	TEST_ASSERT(il.instructions[il.instructions.Count()-1].argint==0);
	TEST_ASSERT(il.instructions[il.instructions.Count()-1].argf32==0);
	TEST_ASSERT(il.instructions[il.instructions.Count()-1].argf64==0.5);

	il.Ins(BasicIns::resptr, BasicIns::s32);
	TEST_ASSERT(il.instructions.Count()==7);
	TEST_ASSERT(il.instructions[il.instructions.Count()-1].opcode==BasicIns::resptr);
	TEST_ASSERT(il.instructions[il.instructions.Count()-1].type1==BasicIns::s32);
	TEST_ASSERT(il.instructions[il.instructions.Count()-1].type2==0);
	TEST_ASSERT(il.instructions[il.instructions.Count()-1].argint==0);
	TEST_ASSERT(il.instructions[il.instructions.Count()-1].argf32==0);
	TEST_ASSERT(il.instructions[il.instructions.Count()-1].argf64==0);

	il.Ins(BasicIns::resptr, BasicIns::s32, (unsigned __int64)10);
	TEST_ASSERT(il.instructions.Count()==8);
	TEST_ASSERT(il.instructions[il.instructions.Count()-1].opcode==BasicIns::resptr);
	TEST_ASSERT(il.instructions[il.instructions.Count()-1].type1==BasicIns::s32);
	TEST_ASSERT(il.instructions[il.instructions.Count()-1].type2==0);
	TEST_ASSERT(il.instructions[il.instructions.Count()-1].argint==10);
	TEST_ASSERT(il.instructions[il.instructions.Count()-1].argf32==0);
	TEST_ASSERT(il.instructions[il.instructions.Count()-1].argf64==0);

	il.Ins(BasicIns::resptr, BasicIns::s32, 20);
	TEST_ASSERT(il.instructions.Count()==9);
	TEST_ASSERT(il.instructions[il.instructions.Count()-1].opcode==BasicIns::resptr);
	TEST_ASSERT(il.instructions[il.instructions.Count()-1].type1==BasicIns::s32);
	TEST_ASSERT(il.instructions[il.instructions.Count()-1].type2==0);
	TEST_ASSERT(il.instructions[il.instructions.Count()-1].argint==20);
	TEST_ASSERT(il.instructions[il.instructions.Count()-1].argf32==0);
	TEST_ASSERT(il.instructions[il.instructions.Count()-1].argf64==0);

	il.Ins(BasicIns::resptr, BasicIns::s32, true);
	TEST_ASSERT(il.instructions.Count()==10);
	TEST_ASSERT(il.instructions[il.instructions.Count()-1].opcode==BasicIns::resptr);
	TEST_ASSERT(il.instructions[il.instructions.Count()-1].type1==BasicIns::s32);
	TEST_ASSERT(il.instructions[il.instructions.Count()-1].type2==0);
	TEST_ASSERT(il.instructions[il.instructions.Count()-1].argint==1);
	TEST_ASSERT(il.instructions[il.instructions.Count()-1].argf32==0);
	TEST_ASSERT(il.instructions[il.instructions.Count()-1].argf64==0);

	il.Ins(BasicIns::resptr, BasicIns::s32, 0.5f);
	TEST_ASSERT(il.instructions.Count()==11);
	TEST_ASSERT(il.instructions[il.instructions.Count()-1].opcode==BasicIns::resptr);
	TEST_ASSERT(il.instructions[il.instructions.Count()-1].type1==BasicIns::s32);
	TEST_ASSERT(il.instructions[il.instructions.Count()-1].type2==0);
	TEST_ASSERT(il.instructions[il.instructions.Count()-1].argint==0);
	TEST_ASSERT(il.instructions[il.instructions.Count()-1].argf32==0.5f);
	TEST_ASSERT(il.instructions[il.instructions.Count()-1].argf64==0);

	il.Ins(BasicIns::resptr, BasicIns::s32, 0.5);
	TEST_ASSERT(il.instructions.Count()==12);
	TEST_ASSERT(il.instructions[il.instructions.Count()-1].opcode==BasicIns::resptr);
	TEST_ASSERT(il.instructions[il.instructions.Count()-1].type1==BasicIns::s32);
	TEST_ASSERT(il.instructions[il.instructions.Count()-1].type2==0);
	TEST_ASSERT(il.instructions[il.instructions.Count()-1].argint==0);
	TEST_ASSERT(il.instructions[il.instructions.Count()-1].argf32==0);
	TEST_ASSERT(il.instructions[il.instructions.Count()-1].argf64==0.5);

	il.Ins(BasicIns::resptr, BasicIns::s32, BasicIns::s64);
	TEST_ASSERT(il.instructions.Count()==13);
	TEST_ASSERT(il.instructions[il.instructions.Count()-1].opcode==BasicIns::resptr);
	TEST_ASSERT(il.instructions[il.instructions.Count()-1].type1==BasicIns::s32);
	TEST_ASSERT(il.instructions[il.instructions.Count()-1].type2==BasicIns::s64);
	TEST_ASSERT(il.instructions[il.instructions.Count()-1].argint==0);
	TEST_ASSERT(il.instructions[il.instructions.Count()-1].argf32==0);
	TEST_ASSERT(il.instructions[il.instructions.Count()-1].argf64==0);
}