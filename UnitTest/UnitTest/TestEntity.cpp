#include <string.h>
#include "..\..\Library\UnitTest\UnitTest.h"
#include "..\..\Library\Entity\Linear.h"
#include "..\..\Library\Entity\SmallObjectPoolEntity.h"

using namespace vl;
using namespace vl::entities;

/***********************************************************************
线性值
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

/***********************************************************************
对想池
***********************************************************************/

TEST_CASE(TestEntity_SmallObjectPool)
{
	SmallObjectPool pool(8, 512);
	char* start=pool.GetStartAddress();
	TEST_ASSERT(pool.GetObjectSize()==8);
	TEST_ASSERT(pool.GetMaxCount()==512);
	TEST_ASSERT(pool.GetEndAddress()-start==4096);

	for(vint i=0;i<pool.GetMaxCount();i++)
	{
		char* object=pool.Alloc();
		TEST_ASSERT(object);
		TEST_ASSERT(object-start==i*8);
		TEST_ASSERT(pool.GetUsedCount()==i+1);
	}

	for(vint i=0;i<pool.GetMaxCount()*pool.GetObjectSize();i++)
	{
		if(i%pool.GetObjectSize()==0)
		{
			TEST_ASSERT(pool.IsValid(start+i)==true);
			TEST_ASSERT(pool.GetHandle(start+i)==start+i);
		}
		else
		{
			TEST_ASSERT(pool.Free(start+i)==false);
			TEST_ASSERT(pool.IsValid(start+i)==false);
			TEST_ASSERT(pool.GetHandle(start+i)==start+i-i%pool.GetObjectSize());
		}
	}
	TEST_ASSERT(pool.GetUsedCount()==pool.GetMaxCount());

	for(vint i=pool.GetMaxCount()-1;i>=0;i--)
	{
		char* object=start+i*pool.GetObjectSize();
		TEST_ASSERT(pool.Free(object)==true);
		TEST_ASSERT(pool.GetUsedCount()==pool.GetMaxCount()-1);
		TEST_ASSERT(pool.Alloc()==object);
		TEST_ASSERT(pool.GetUsedCount()==pool.GetMaxCount());
	}

	for(vint i=pool.GetMaxCount()-1;i>=0;i--)
	{
		char* object=start+i*pool.GetObjectSize();
		TEST_ASSERT(pool.Free(object)==true);
		TEST_ASSERT(pool.GetUsedCount()==i);
	}
}