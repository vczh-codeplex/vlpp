#include <memory.h>
#include "SmallObjectPoolEntity.h"

namespace vl
{
	namespace entities
	{
/***********************************************************************
SmallObjectPool
***********************************************************************/

#define GROUP_MAX						256
#define MARK_INDEX(OBJECT_INDEX)		((OBJECT_INDEX)/8)
#define MARK(OBJECT_INDEX)				(((unsigned char)1)<<((OBJECT_INDEX)%8))
#define GROUP_INDEX(OBJECT_INDEX)		((OBJECT_INDEX)/GROUP_MAX)

		SmallObjectPool::SmallObjectPool(vint _objectSize, vint _maxCount)
			:objectSize(_objectSize)
			,usedCount(0)
			,lastObjectIndex(0)
		{
			maxCount=(_maxCount+(GROUP_MAX-1))/GROUP_MAX*GROUP_MAX;
			poolSize=objectSize*maxCount;
			markCount=maxCount/8;
			groupCount=maxCount/GROUP_MAX;

			items=new char[poolSize];
			marks=new unsigned __int8[markCount];
			groups=new unsigned __int32[groupCount];
			memset(items, 0, poolSize*sizeof(char));
			memset(marks, 0, markCount*sizeof(unsigned __int8));
			memset(groups, 0, groupCount*sizeof(unsigned __int32));
		}

		SmallObjectPool::~SmallObjectPool()
		{
			delete[] items;
			delete[] marks;
			delete[] groups;
		}

		char* SmallObjectPool::Alloc()
		{
			if(usedCount==maxCount)return 0;

			vint groupIndex=GROUP_INDEX(lastObjectIndex);
			while(groups[groupIndex]==GROUP_MAX)
				groupIndex=(groupIndex+1)%groupCount;

			vint markIndex=groupIndex*(GROUP_MAX/8);
			while(marks[markIndex]==255)markIndex++;

			vint markBitIndex=0;
			unsigned __int8 markNumber=marks[markIndex];
			while(markNumber&(1<<markBitIndex))markBitIndex++;

			marks[markIndex]|=(1<<markBitIndex);
			groups[groupIndex]++;
			usedCount++;
			lastObjectIndex=groupIndex*GROUP_MAX + (markIndex%(GROUP_MAX/8))*8 + markBitIndex;
			return &items[lastObjectIndex*objectSize];
		}

		bool SmallObjectPool::Free(char* handle)
		{
			if(IsValid(handle))
			{
				vint objectIndex=(handle-items)/objectSize;
				vint markIndex=MARK_INDEX(objectIndex);
				unsigned char mark=MARK(objectIndex);
				if(marks[markIndex]&mark)
				{
					vint groupIndex=GROUP_INDEX(objectIndex);
					marks[markIndex]&=~mark;
					groups[groupIndex]--;
					usedCount--;
					return true;
				}
			}
			return false;
		}

		bool SmallObjectPool::IsValid(char* handle)
		{
			vint index=handle-items;
			return index>=0 && index<poolSize && index%objectSize==0;
		}

		char* SmallObjectPool::GetHandle(char* pointer)
		{
			vint index=pointer-items;
			if(index>=0 && index<poolSize)
			{
				return &items[index-index%objectSize];
			}
			else
			{
				return 0;
			}
		}
	}
}