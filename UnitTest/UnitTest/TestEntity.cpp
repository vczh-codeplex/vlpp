#include <string.h>
#include "..\..\Library\UnitTest\UnitTest.h"
#include "..\..\Library\Entity\Linear.h"

using namespace vl;

/***********************************************************************
œﬂ–‘÷µ
***********************************************************************/

TEST_CASE(TestEntity_Linear)
{
	typedef Linear<char, int> F;

	{
		TEST_ASSERT(F(0).Constant()==0);
		TEST_ASSERT(F(0)('x', 1).Factor('x')==1);
		TEST_ASSERT(F(0)('x', 1).Factor('y')==0);

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
	{
		TEST_ASSERT(F(1)+2==F(3));
		TEST_ASSERT(2+F(1)==F(3));
		TEST_ASSERT(F(1)+F(2)==F(3));
		TEST_ASSERT(F(1)('x', 2)+F(3)('x', 4)==F(4)('x', 6));
		TEST_ASSERT(F(1)('x', 2)('y', 3)+F(4)('x', 5)('z', 6)==F(5)('x', 7)('y', 3)('z', 6));
	}
	{
		TEST_ASSERT(F(1)-2==F(-1));
		TEST_ASSERT(2-F(1)==F(1));
		TEST_ASSERT(F(1)-F(2)==F(-1));
		TEST_ASSERT(F(1)('x', 2)-F(3)('x', 4)==F(-2)('x', -2));
		TEST_ASSERT(F(1)('x', 2)('y', 3)-F(4)('x', 5)('z', 6)==F(-3)('x', -3)('y', 3)('z', -6));
	}
	{
		TEST_ASSERT(F(2)*3==F(6));
		TEST_ASSERT(3*F(2)==F(6));
		TEST_ASSERT(F(2)('x', 3)('y', 4)*2==F(4)('x', 6)('y', 8));
		TEST_ASSERT(2*F(2)('x', 3)('y', 4)==F(4)('x', 6)('y', 8));
	}
	{
		TEST_ASSERT(F(6)/3==F(2));
		TEST_ASSERT(F(4)('x', 6)('y', 8)/2==F(2)('x', 3)('y', 4));
	}
	{
		TEST_ASSERT(F(6)%4==F(2));
		TEST_ASSERT(F(4)('x', 6)('y', 8)%4==F(0)('x', 2));
	}
}