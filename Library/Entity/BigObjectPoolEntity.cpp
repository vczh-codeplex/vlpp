#include "BigObjectPoolEntity.h"

namespace vl
{
	namespace entities
	{
/***********************************************************************
BigObjectPool
***********************************************************************/
#define CHECKSUM_TOKEN		0x1ABCDEF1
#define MAX(A, B)			((A)>(B)?(A):(B))
#define MIN_OBJECT_SIZE(S)	MAX(32, (S))

		BigObjectPool::BlockHandle* BigObjectPool::CreateBlockHandle()
		{
			BlockHandle* free=(BlockHandle*)blockPool.Alloc();
			free->start=0;
			free->size=0;
			free->prev=0;
			free->next=0;
			free->used=false;
			return free;
		}

		void BigObjectPool::DisposeBlockHandle(BlockHandle* handle)
		{
			blockPool.Free((char*)handle);
		}

		BigObjectPool::BlockHandle* BigObjectPool::MergePrev(BlockHandle* handle)
		{
			BlockHandle* prev=handle->prev;
			prev->next=handle->next;
			prev->size+=handle->size;
			if(handle->next)
			{
				handle->next->prev=prev;
			}
			if(currentBlock==handle)
			{
				currentBlock=handle->next;
				if(!currentBlock)
				{
					currentBlock=firstBlock;
				}
			}
			DisposeBlockHandle(handle);
			return prev;
		}

		BigObjectPool::BlockHandle* BigObjectPool::SplitBlock(BlockHandle* handle, vint size)
		{
			if(size<handle->size)
			{
				BlockHandle* next=CreateBlockHandle();
				next->prev=handle;
				next->next=handle->next;
				next->start=handle->start+size;
				next->size=handle->size-size;
				next->used=handle->used;
				if(handle->next)
				{
					handle->next->prev=next;
				}
				handle->next=next;
				handle->size=size;
			}
			return handle;
		}
		
		BigObjectPool::BigObjectPool(vint _totalSize, vint _minObjectSize)
			:totalSize(_totalSize)
			,minObjectSize(MIN_OBJECT_SIZE(_minObjectSize))
			,usedSize(0)
			,blockPool(sizeof(BlockHandle), 1+_totalSize/MIN_OBJECT_SIZE(_minObjectSize))
		{
			buffer=new char[totalSize];
			firstBlock=CreateBlockHandle();
			firstBlock->start=buffer;
			firstBlock->size=totalSize;
			currentBlock=firstBlock;
		}

		BigObjectPool::~BigObjectPool()
		{
			delete[] buffer;
		}

		char* BigObjectPool::Alloc(vint size)
		{
			if(usedSize==totalSize)return 0;
			size=MAX(size+(vint)BlockHeaderSize, minObjectSize);

			BlockHandle* block=currentBlock;
			while(true)
			{
				if(!block->used && block->size>=size)
				{
					block=SplitBlock(block, size);
					block->used=true;
					usedSize+=size;

					BlockReference* reference=(BlockReference*)block->start;
					reference->handle=block;
					reference->checksum=((vint)block)^CHECKSUM_TOKEN;
					currentBlock=block->next;
					if(!currentBlock)
					{
						currentBlock=firstBlock;
					}
					return block->start+BlockHeaderSize;
				}
				block=block->next;
				if(!block)block=firstBlock;
				if(block==currentBlock)break;
			}
			return 0;
		}

		bool BigObjectPool::Free(char* handle)
		{
			if(IsValid(handle))
			{
				BlockReference* reference=(BlockReference*)(handle-BlockHeaderSize);
				BlockHandle* block=reference->handle;
				block->used=false;
				vint size=block->size;
				if(block->prev && !block->prev->used)
				{
					block=MergePrev(block);
				}
				if(block->next && !block->next->used)
				{
					MergePrev(block->next);
				}
				usedSize-=size;
				return true;
			}
			else
			{
				return false;
			}
		}

		bool BigObjectPool::IsValid(char* handle)
		{
			BlockReference* reference=(BlockReference*)(handle-BlockHeaderSize);
			return
				(reference->checksum==((vint)reference->handle^CHECKSUM_TOKEN)) &&
				blockPool.IsValid((char*)reference->handle) &&
				reference->handle->start==(char*)reference &&
				reference->handle->used;
		}

		char* BigObjectPool::GetHandle(char* pointer)
		{
			BlockHandle* block=firstBlock;
			while(block)
			{
				if(block->used && block->start<=pointer && pointer<block->start+block->size)
				{
					return block->start+BlockHeaderSize;
				}
				block=block->next;
			}
			return 0;
		}

		vint BigObjectPool::GetSize(char* handle)
		{
			if(IsValid(handle))
			{
				BlockReference* reference=(BlockReference*)(handle-BlockHeaderSize);
				return reference->handle->size-BlockHeaderSize;
			}
			else
			{
				return -1;
			}
		}
	}
}