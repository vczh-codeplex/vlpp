#include <string.h>
#include "..\..\Library\UnitTest\UnitTest.h"
#include "..\..\Library\Entity\Linear.h"

using namespace vl;

/***********************************************************************
œﬂ–‘÷µ
***********************************************************************/

TEST_CASE(TestEntity_Linear)
{
	typedef Linear<char, vint> F;

	{
		TEST_ASSERT(F(0).Constant()==0);
		TEST_ASSERT(F(0)('x', 1).Factor('x')==1);
		TEST_ASSERT(F(0)('x', 1).Factor('x', 2)==1);
		TEST_ASSERT(F(0)('x', 1).Factor('y', 2)==2);

		TEST_ASSERT(F(0)==F(0));
		TEST_ASSERT(F(0)!=F(1));
		TEST_ASSERT(F(1)!=F(0));
		TEST_ASSERT(F(0)==0);
		TEST_ASSERT(F(0)!=1);
		TEST_ASSERT(F(1)==1);
		TEST_ASSERT(F(1)!=0);
		TEST_ASSERT(0==F(0));
		TEST_ASSERT(1!=F(0));
		TEST_ASSERT(1==F(1));
		TEST_ASSERT(0!=F(1));

		TEST_ASSERT(F(0)('x', 1)==F(0)('x', 1));
		TEST_ASSERT(F(0)('x', 1)!=F(0)('y', 1));
		TEST_ASSERT(F(0)('x', 1)!=F(1)('x', 1));
		TEST_ASSERT(F(0)('x', 1)!=F(1)('y', 1));
		TEST_ASSERT(F(0)('x', 1)('y', 2)==F(0)('x', 1)('y', 2));
		TEST_ASSERT(F(0)('x', 1)('y', 2)==F(0)('y', 2)('x', 1));
		TEST_ASSERT(F(0)('x', 1)('y', 2)!=F(0)('x', 2)('y', 1));
		TEST_ASSERT(F(0)('x', 1)('y', 2)!=F(0)('y', 1)('x', 2));
		TEST_ASSERT(F(0)('x', 1)('y', 2)!=F(1)('x', 1)('y', 2));
		TEST_ASSERT(F(0)('x', 1)('y', 2)!=F(1)('y', 2)('x', 1));
		TEST_ASSERT(F(0)('x', 1)('y', 2)!=F(1)('x', 2)('y', 1));
		TEST_ASSERT(F(0)('x', 1)('y', 2)!=F(1)('y', 1)('x', 2));
		TEST_ASSERT(F(0)('x', 1)('y', 2)!=0);
		TEST_ASSERT(F(0)('x', 1)('y', 2)!=1);
		TEST_ASSERT(0!=F(0)('x', 1)('y', 2));
		TEST_ASSERT(1!=F(0)('y', 2)('x', 1));
		TEST_ASSERT(F(0)('x', 1)('y', 2)!=F(0));
		TEST_ASSERT(F(0)('x', 1)('y', 2)!=F(1));
		TEST_ASSERT(F(0)!=F(0)('x', 1)('y', 2));
		TEST_ASSERT(F(1)!=F(0)('y', 2)('x', 1));
	}
}