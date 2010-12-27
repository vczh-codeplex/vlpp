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
#include "SmallObjectPoolEntity.h"

namespace vl
{
	namespace entities
	{
		class BigObjectPool : public Object
		{
		protected:
			struct BlockHandle
			{
				char*				start;
				vint				size;
				bool				used;
				BlockHandle*		prev;
				BlockHandle*		next;
			};

			struct BlockReference
			{
				BlockHandle*		handle;
				vint				checksum;
			};

			vint					totalSize;
			vint					minObjectSize;
			vint					usedSize;
			char*					buffer;

			SmallObjectPool			blockPool;
			BlockHandle*			firstBlock;
			BlockHandle*			currentBlock;

			BlockHandle*			CreateBlockHandle();
			void					DisposeBlockHandle(BlockHandle* handle);
			BlockHandle*			MergePrev(BlockHandle* handle);
			BlockHandle*			SplitBlock(BlockHandle* handle, vint size);
		public:
			static const vint		BlockHeaderSize=sizeof(BlockReference);

			BigObjectPool(vint _totalSize, vint _minObjectSize);
			~BigObjectPool();

			char*					Alloc(vint size);
			bool					Free(char* handle);
			bool					IsValid(char* handle);
			char*					GetHandle(char* pointer);
			vint					GetSize(char* handle);

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