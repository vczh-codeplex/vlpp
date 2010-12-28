#include <memory.h>
#include "GeneralObjectPoolEntity.h"

namespace vl
{
	namespace entities
	{
/***********************************************************************
GeneralObjectPool::PoolNodeAllocator
***********************************************************************/

		GeneralObjectPool::PoolNodeAllocator::PoolNodeAllocator(vint maxCount)
			:nodePool(sizeof(PoolNode), maxCount)
		{
		}

		GeneralObjectPool::PoolNodeAllocator::~PoolNodeAllocator()
		{
		}

		GeneralObjectPool::PoolNode* GeneralObjectPool::PoolNodeAllocator::CreateNode()
		{
			PoolNode* node=(PoolNode*)nodePool.Alloc();
			node->value.Initialize();
			return node;
		}

		void GeneralObjectPool::PoolNodeAllocator::DisposeNode(PoolNode* node)
		{
			node->value.Finalize();
			nodePool.Free((char*)node);
		}

/***********************************************************************
GeneralObjectPool::PoolContainer
***********************************************************************/

		void GeneralObjectPool::PoolContainer::Initialize()
		{
			size=0;
			pools.smallObjectPool=0;
			pools.bigObjectPool=0;
		}

		void GeneralObjectPool::PoolContainer::Finalize(bool deletePool)
		{
			if(size!=-1 && pools.smallObjectPool)
			{
				if(deletePool)
				{
					delete pools.smallObjectPool;
				}
				pools.smallObjectPool=0;
			}
			if(size==-1 && pools.bigObjectPool)
			{
				if(deletePool)
				{
					delete pools.bigObjectPool;
				}
				pools.bigObjectPool=0;
			}
		}

/***********************************************************************
GeneralObjectPool::PoolNodeContent
***********************************************************************/

		GeneralObjectPool::PoolNodeContent::PoolNodeContent()
		{
			Initialize();
		}

		GeneralObjectPool::PoolNodeContent::~PoolNodeContent()
		{
			Finalize();
		}

		void GeneralObjectPool::PoolNodeContent::Initialize()
		{
			poolContainer.Initialize();
			prev=0;
			next=0;
		}

		void GeneralObjectPool::PoolNodeContent::Finalize()
		{
			poolContainer.Finalize(true);
		}
		
		char* GeneralObjectPool::PoolNodeContent::GetStartAddress()const
		{
			if(poolContainer.size!=-1)
			{
				return poolContainer.pools.smallObjectPool->GetStartAddress();
			}
			else
			{
				return poolContainer.pools.bigObjectPool->GetStartAddress();
			}
		}

		bool GeneralObjectPool::PoolNodeContent::operator<(const PoolNodeContent& node)const
		{
			return GetStartAddress()<node.GetStartAddress();
		}

		bool GeneralObjectPool::PoolNodeContent::operator<=(const PoolNodeContent& node)const
		{
			return GetStartAddress()<=node.GetStartAddress();
		}

		bool GeneralObjectPool::PoolNodeContent::operator>(const PoolNodeContent& node)const
		{
			return GetStartAddress()>node.GetStartAddress();
		}

		bool GeneralObjectPool::PoolNodeContent::operator>=(const PoolNodeContent& node)const
		{
			return GetStartAddress()>=node.GetStartAddress();
		}

		bool GeneralObjectPool::PoolNodeContent::operator==(const PoolNodeContent& node)const
		{
			return GetStartAddress()==node.GetStartAddress();
		}

		bool GeneralObjectPool::PoolNodeContent::operator!=(const PoolNodeContent& node)const
		{
			return GetStartAddress()!=node.GetStartAddress();
		}

/***********************************************************************
GeneralObjectPool::PoolNodeEntry
***********************************************************************/

		GeneralObjectPool::PoolNodeEntry::PoolNodeEntry(vint _size)
			:first(0)
			,last(0)
		{
			free.Initialize();
			free.size=_size;
		}

		GeneralObjectPool::PoolNodeEntry::~PoolNodeEntry()
		{
			free.Finalize(true);
		}

/***********************************************************************
GeneralObjectPool
***********************************************************************/

		GeneralObjectPool::GeneralObjectPool(vint _poolUnitSize, vint _poolUnitCount)
			:poolUnitSize(_poolUnitSize)
			,poolUnitCount(_poolUnitCount)
			,poolNodeAllocator(_poolUnitCount)
			,poolTree(&poolNodeAllocator)
			,pool8(8)
			,pool16(16)
			,pool32(32)
			,pool64(64)
			,pool96(96)
			,poolLarge(-1)
		{
		}

		GeneralObjectPool::~GeneralObjectPool()
		{
		}
	}
}