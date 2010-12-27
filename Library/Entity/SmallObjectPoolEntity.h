/***********************************************************************
Vczh Library++ 3.0
Developer: ³Âè÷å«(vczh)
Framework::SmallObjectPoolEntity

Classes:
	SmallObjectPoolEntity							£ºÐ¡¶ÔÏó³Ø
***********************************************************************/

#ifndef VCZH_ENTITY_SMALLOBJECTPOOLENTITY
#define VCZH_ENTITY_SMALLOBJECTPOOLENTITY

#include "..\Basic.h"

namespace vl
{
	namespace entities
	{
		class SmallObjectPool : public Object
		{
		protected:
			vint					objectSize;
			vint					maxCount;
			vint					poolSize;
			vint					markCount;
			vint					groupCount;
			char*					items;
			unsigned __int8*		marks;
			unsigned __int32*		groups;
			vint					usedCount;
			vint					lastObjectIndex;
		public:
			SmallObjectPool(vint _objectSize, vint _maxCount);
			~SmallObjectPool();

			char*					Alloc();
			bool					Free(char* handle);
			bool					IsValid(char* handle);
			char*					GetHandle(char* pointer);

			inline vint GetObjectSize()
			{
				return objectSize;
			}

			inline vint GetMaxCount()
			{
				return maxCount;
			}

			inline vint GetUsedCount()
			{
				return usedCount;
			}

			inline char* GetStartAddress()
			{
				return items;
			}

			inline char* GetEndAddress()
			{
				return items+objectSize*maxCount;
			}
		};
	}
}

#endif