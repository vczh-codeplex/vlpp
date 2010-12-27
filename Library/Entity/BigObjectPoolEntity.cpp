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

		BigObjectPool::BlockHandle* BigObjectPool::CreateFreeHandle()
		{
			BlockHandle* free=(BlockHandle*)blockPool.Alloc();
			free->start=0;
			free->size=0;
			free->prev=0;
			free->next=0;
			free->used=false;
			return free;
		}

		void BigObjectPool::DisposeFreeHandle(BlockHandle* handle)
		{
			blockPool.Free((char*)handle);
		}
		
		BigObjectPool::BigObjectPool(vint _totalSize, vint _minObjectSize)
			:totalSize(_totalSize)
			,minObjectSize(MIN_OBJECT_SIZE(_minObjectSize))
			,usedSize(0)
			,blockPool(sizeof(BlockHandle), 1+_totalSize/MIN_OBJECT_SIZE(_minObjectSize))
		{
			buffer=new char[totalSize];
			firstBlock=CreateFreeHandle();
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
			throw 0;
		}

		bool BigObjectPool::Free(char* handle)
		{
			throw 0;
		}

		bool BigObjectPool::IsValid(char* handle)
		{
			BlockReference* reference=(BlockReference*)(handle-sizeof(BlockReference));
			return
				(reference->checksum==((vint)reference->handle^CHECKSUM_TOKEN)) &&
				blockPool.IsValid((char*)reference->handle) &&
				reference->handle->used;
		}

		char* BigObjectPool::GetHandle(char* pointer)
		{
			BlockHandle* block=firstBlock;
			while(block)
			{
				if(block->used && block->start<=pointer && pointer<block->start+block->size)
				{
					return block->start;
				}
				block=block->next;
			}
			return 0;
		}

		vint BigObjectPool::GetSize(char* handle)
		{
			if(IsValid(handle))
			{
				BlockReference* reference=(BlockReference*)(handle-sizeof(BlockReference));
				return reference->handle->size;
			}
			else
			{
				return -1;
			}
		}
	}
}