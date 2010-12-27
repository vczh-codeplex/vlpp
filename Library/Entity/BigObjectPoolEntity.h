/***********************************************************************
Vczh Library++ 3.0
Developer: ³Âè÷å«(vczh)
Framework::SmallObjectPoolEntity

Classes:
	BigObjectPool									£º´ó¶ÔÏó³Ø
***********************************************************************/

#ifndef VCZH_ENTITY_BIGOBJECTPOOLENTITY
#define VCZH_ENTITY_BIGOBJECTPOOLENTITY

#include "..\Basic.h"

namespace vl
{
	namespace entities
	{
		class BigObjectPool : public Object
		{
		protected:
			vint					totalSize;
			vint					minObjectSize;
			vint					usedSize;
			char*					buffer;
		public:
			BigObjectPool(vint _totalSize, vint _minObjectSize);
			~BigObjectPool();

			char*					Alloc();
			bool					Free(char* handle);
			bool					IsValid(char* handle);
			char*					GetHandle(char* pointer);

			inline vint GetMinObjectSize()
			{
				return minObjectSize;
			}

			inline vint GetTotalSize()
			{
				return totalSize;
			}

			inline vint GetUsedSize()
			{
				return usedSize;
			}

			inline char* GetStartAddress()
			{
				return buffer;
			}

			inline char* GetEndAddress()
			{
				return buffer+totalSize;
			}
		};
	}
}

#endif