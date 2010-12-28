#include <string.h>
#include "..\..\Library\UnitTest\UnitTest.h"
#include "..\..\Library\Entity\Linear.h"
#include "..\..\Library\Entity\GeneralObjectPoolEntity.h"
#include "..\..\Library\Collections\List.h"

using namespace vl;
using namespace vl::entities;
using namespace vl::collections;

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
小对象池
***********************************************************************/

namespace TestEntityHelper
{
	void SmallObjectPoolAssertion(SmallObjectPool& pool)
	{
		char* start=pool.GetStartAddress();
		TEST_ASSERT(pool.GetObjectSize()==8);
		TEST_ASSERT(pool.GetMaxCount()==512);
		TEST_ASSERT(pool.GetUsedCount()==0);
		TEST_ASSERT(pool.GetEndAddress()-start==pool.GetObjectSize()*pool.GetMaxCount());

		for(vint i=0;i<pool.GetMaxCount();i++)
		{
			char* object=pool.Alloc();
			TEST_ASSERT(object);
			TEST_ASSERT(object-start==i*8);
			TEST_ASSERT(pool.GetUsedCount()==i+1);
		}
		TEST_ASSERT(pool.Alloc()==0);

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
}
using namespace TestEntityHelper;

TEST_CASE(TestEntity_SmallObjectPool)
{
	{
		SmallObjectPool pool(8, 512);
		SmallObjectPoolAssertion(pool);
	}
	{
		SmallObjectPool pool(8, 384);
		SmallObjectPoolAssertion(pool);
	}
}

/***********************************************************************
大对象池
***********************************************************************/

namespace TestEntityHelper
{
	void BigObjectPoolAssertHandleIndex(BigObjectPool& pool, char* object, vint index)
	{
		char* start=pool.GetStartAddress();
		TEST_ASSERT(object==start+index*pool.GetMinObjectSize()+BigObjectPool::BlockHeaderSize);
		TEST_ASSERT(pool.IsValid(object)==true);
		TEST_ASSERT(pool.GetHandle(object)==object);
		TEST_ASSERT(pool.GetSize(object)==pool.GetMinObjectSize()-BigObjectPool::BlockHeaderSize);
	}

	void BigObjectPoolAssertPointerIndex(BigObjectPool& pool, char* pointer, vint index)
	{
		char* start=pool.GetStartAddress();
		char* object=pointer-(pointer-start)%pool.GetMinObjectSize()+BigObjectPool::BlockHeaderSize;
		TEST_ASSERT(pool.IsValid(pointer)==false);
		TEST_ASSERT(pool.GetHandle(pointer)==object);
		TEST_ASSERT(pool.GetSize(pointer)==-1);
		BigObjectPoolAssertHandleIndex(pool, object, index);
	}

	void BigObjectPoolAssertFreeIndex(BigObjectPool& pool, vint index)
	{
		char* start=pool.GetStartAddress();
		char* object=start+index*pool.GetMinObjectSize()+BigObjectPool::BlockHeaderSize;
		TEST_ASSERT(pool.Free(object)==true);
		TEST_ASSERT(pool.IsValid(object)==false);
		TEST_ASSERT(pool.GetHandle(object)==0);
	}
}
using namespace TestEntityHelper;

TEST_CASE(TestEntity_BigObjectPool)
{
	BigObjectPool pool(512, 16);
	char* start=pool.GetStartAddress();
	TEST_ASSERT(pool.GetMinObjectSize()==32);
	TEST_ASSERT(pool.GetTotalSize()==512);
	TEST_ASSERT(pool.GetUsedSize()==0);
	TEST_ASSERT(pool.GetEndAddress()==start+pool.GetTotalSize());
	vint count=8;
	vint tailCount=pool.GetTotalSize()/pool.GetMinObjectSize()-count;

	for(vint i=0;i<count;i++)
	{
		BigObjectPoolAssertHandleIndex(pool, pool.Alloc(16), i);
	}
	TEST_ASSERT(pool.GetUsedSize()==pool.GetTotalSize()-count*pool.GetMinObjectSize());

	for(vint i=0;i<count*pool.GetMinObjectSize();i++)
	{
		char* object=start+i;
		vint index=i/pool.GetMinObjectSize();
		if(i%pool.GetMinObjectSize()==BigObjectPool::BlockHeaderSize)
		{
			BigObjectPoolAssertHandleIndex(pool, object, index);
		}
		else
		{
			BigObjectPoolAssertPointerIndex(pool, object, index);
		}
	}

	BigObjectPoolAssertFreeIndex(pool, 1);
	BigObjectPoolAssertFreeIndex(pool, 3);
	BigObjectPoolAssertFreeIndex(pool, 5);
	BigObjectPoolAssertFreeIndex(pool, 4);

	for(vint i=0;i<tailCount;i++)
	{
		BigObjectPoolAssertHandleIndex(pool, pool.Alloc(16), i+count);
	}
	BigObjectPoolAssertHandleIndex(pool, pool.Alloc(16), 1);
	BigObjectPoolAssertHandleIndex(pool, pool.Alloc(16), 3);
	BigObjectPoolAssertHandleIndex(pool, pool.Alloc(16), 4);
	BigObjectPoolAssertHandleIndex(pool, pool.Alloc(16), 5);

	BigObjectPoolAssertFreeIndex(pool, count+tailCount-2);
	BigObjectPoolAssertHandleIndex(pool, pool.Alloc(16), count+tailCount-2);
	BigObjectPoolAssertFreeIndex(pool, count+tailCount-3);
	BigObjectPoolAssertFreeIndex(pool, count+tailCount-1);
	BigObjectPoolAssertHandleIndex(pool, pool.Alloc(16), count+tailCount-1);
	BigObjectPoolAssertHandleIndex(pool, pool.Alloc(16), count+tailCount-3);

	for(vint i=count+tailCount-1;i>=0;i--)
	{
		if(i%2==0)
		{
			BigObjectPoolAssertFreeIndex(pool, i);
		}
	}
	for(vint i=count+tailCount-1;i>=0;i--)
	{
		if(i%2==1)
		{
			BigObjectPoolAssertFreeIndex(pool, i);
		}
	}
}

/***********************************************************************
平衡树
***********************************************************************/

namespace TestEntityHelper
{
	template<typename T>
	void CompareTree(typename BinValueTree<T>::Node* node, const IReadonlyList<T>& values, vint& index)
	{
		vint leftDepth=0;
		if(node->left)
		{
			leftDepth=node->left->depth;
			TEST_ASSERT(node->left->parent==node);
			CompareTree(node->left, values, index);
		}
		TEST_ASSERT(index<values.Count());
		TEST_ASSERT(node->value==values[index]);
		index++;
		vint rightDepth=0;
		if(node->right)
		{
			rightDepth=node->right->depth;
			TEST_ASSERT(node->right->parent==node);
			CompareTree(node->right, values, index);
		}
		TEST_ASSERT(node->depth==1+(leftDepth>rightDepth?leftDepth:rightDepth));
		vint dDepth=leftDepth-rightDepth;
		TEST_ASSERT(-1<=dDepth && dDepth<=1);
	}

	template<typename T>
	void CompareTree(typename BinValueTree<T>::Node* node, const IReadonlyList<T>& values)
	{
		vint index=0;
		if(node)
		{
			CompareTree(node, values, index);
		}
		TEST_ASSERT(index==values.Count());
	}

	void AssertTree(BinValueTree<vint>& tree, vint* numbers, vint count)
	{
		SortedList<vint> values;
		for(vint i=0;i<count;i++)
		{
			values.Add(numbers[i]);
			tree.Insert(numbers[i]);
		}
		CompareTree(tree.root, values.Wrap());
		for(vint i=0;i<count;i++)
		{
			BinValueTree<vint>::Node* node=tree.Find(numbers[i]);
			TEST_ASSERT(node);
			TEST_ASSERT(node->value==numbers[i]);
		}
		while(tree.root)
		{
			vint value=tree.root->value;
			tree.Remove(tree.root->value);
			values.Remove(value);
			CompareTree(tree.root, values.Wrap());
		}
	}

	void AssertTreeWithPermutation(BinValueTree<vint>& tree, vint* numbers, vint count, bool* marks, vint index)
	{
		if(index==count)
		{
			AssertTree(tree, numbers, count);
		}
		else
		{
			for(vint i=0;i<count;i++)
			{
				if(!marks[i])
				{
					marks[i]=true;
					numbers[index]=i;
					AssertTreeWithPermutation(tree, numbers, count, marks, index+1);
					marks[i]=false;
				}
			}
		}
	}
}
using namespace TestEntityHelper;

TEST_CASE(TestEntity_BinaryBalanceTree)
{
	BinValueTree<vint> tree;
	{
		vint numbers[]={1,2,3,4,5,6,7,8,9,10};
		AssertTree(tree, numbers, sizeof(numbers)/sizeof(*numbers));
	}
	{
		vint numbers[]={7,1,12,2,8,3,11,4,9,5,13,6,10};
		AssertTree(tree, numbers, sizeof(numbers)/sizeof(*numbers));
	}
	{
		const vint count=8;
		vint numbers[count];
		bool marks[count];
		for(vint i=0;i<count;i++)
		{
			marks[i]=false;
		}
		AssertTreeWithPermutation(tree, numbers, count, marks, 0);
	}
}

/***********************************************************************
通用对象池
***********************************************************************/

namespace TestEntityHelper
{
	void AssertGeneralObjectAllocAndFree(GeneralObjectPool& pool, bool needFree)
	{
		char* obj8=pool.Alloc(8);
		char* obj16=pool.Alloc(16);
		char* obj32=pool.Alloc(32);
		char* obj64=pool.Alloc(64);
		char* obj96=pool.Alloc(96);
		char* objLarge=pool.Alloc(512);

		TEST_ASSERT(pool.IsValid(obj8)==true);
		TEST_ASSERT(pool.GetHandle(obj8)==obj8);
		TEST_ASSERT(pool.GetSize(obj8)==8);
		TEST_ASSERT(pool.IsValid(obj8+4)==false);
		TEST_ASSERT(pool.GetHandle(obj8+4)==obj8);
		TEST_ASSERT(pool.GetSize(obj8+4)==-1);

		TEST_ASSERT(pool.IsValid(obj16)==true);
		TEST_ASSERT(pool.GetHandle(obj16)==obj16);
		TEST_ASSERT(pool.GetSize(obj16)==16);
		TEST_ASSERT(pool.IsValid(obj16+4)==false);
		TEST_ASSERT(pool.GetHandle(obj16+4)==obj16);
		TEST_ASSERT(pool.GetSize(obj16+4)==-1);

		TEST_ASSERT(pool.IsValid(obj32)==true);
		TEST_ASSERT(pool.GetHandle(obj32)==obj32);
		TEST_ASSERT(pool.GetSize(obj32)==32);
		TEST_ASSERT(pool.IsValid(obj32+4)==false);
		TEST_ASSERT(pool.GetHandle(obj32+4)==obj32);
		TEST_ASSERT(pool.GetSize(obj32+4)==-1);

		TEST_ASSERT(pool.IsValid(obj64)==true);
		TEST_ASSERT(pool.GetHandle(obj64)==obj64);
		TEST_ASSERT(pool.GetSize(obj64)==64);
		TEST_ASSERT(pool.IsValid(obj64+4)==false);
		TEST_ASSERT(pool.GetHandle(obj64+4)==obj64);
		TEST_ASSERT(pool.GetSize(obj64+4)==-1);

		TEST_ASSERT(pool.IsValid(obj96)==true);
		TEST_ASSERT(pool.GetHandle(obj96)==obj96);
		TEST_ASSERT(pool.GetSize(obj96)==96);
		TEST_ASSERT(pool.IsValid(obj96+4)==false);
		TEST_ASSERT(pool.GetHandle(obj96+4)==obj96);
		TEST_ASSERT(pool.GetSize(obj96+4)==-1);

		TEST_ASSERT(pool.IsValid(objLarge)==true);
		TEST_ASSERT(pool.GetHandle(objLarge)==objLarge);
		TEST_ASSERT(pool.GetSize(objLarge)==512);
		TEST_ASSERT(pool.IsValid(objLarge+4)==false);
		TEST_ASSERT(pool.GetHandle(objLarge+4)==objLarge);
		TEST_ASSERT(pool.GetSize(objLarge+4)==-1);

		if(needFree)
		{
			TEST_ASSERT(pool.Free(obj8)==true);
			TEST_ASSERT(pool.IsValid(obj8)==false);
			TEST_ASSERT(pool.GetHandle(obj8)==0);
			TEST_ASSERT(pool.GetSize(obj8)==-1);
			
			TEST_ASSERT(pool.Free(obj16)==true);
			TEST_ASSERT(pool.IsValid(obj16)==false);
			TEST_ASSERT(pool.GetHandle(obj16)==0);
			TEST_ASSERT(pool.GetSize(obj16)==-1);
			
			TEST_ASSERT(pool.Free(obj32)==true);
			TEST_ASSERT(pool.IsValid(obj32)==false);
			TEST_ASSERT(pool.GetHandle(obj32)==0);
			TEST_ASSERT(pool.GetSize(obj32)==-1);
			
			TEST_ASSERT(pool.Free(obj64)==true);
			TEST_ASSERT(pool.IsValid(obj64)==false);
			TEST_ASSERT(pool.GetHandle(obj64)==0);
			TEST_ASSERT(pool.GetSize(obj64)==-1);
			
			TEST_ASSERT(pool.Free(obj96)==true);
			TEST_ASSERT(pool.IsValid(obj96)==false);
			TEST_ASSERT(pool.GetHandle(obj96)==0);
			TEST_ASSERT(pool.GetSize(obj96)==-1);
			
			TEST_ASSERT(pool.Free(objLarge)==true);
			TEST_ASSERT(pool.IsValid(objLarge)==false);
			TEST_ASSERT(pool.GetHandle(objLarge)==0);
			TEST_ASSERT(pool.GetSize(objLarge)==-1);
		}
	}
}
using namespace TestEntityHelper;

TEST_CASE(TestEntity_GeneralObjectPool)
{
	{
		GeneralObjectPool pool(1024, 16);
		AssertGeneralObjectAllocAndFree(pool, false);
	}
	{
		GeneralObjectPool pool(1024, 16);
		AssertGeneralObjectAllocAndFree(pool, true);
	}
}