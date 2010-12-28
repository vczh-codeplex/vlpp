#include <memory.h>
#include "GeneralObjectPoolEntity.h"

namespace vl
{
	namespace entities
	{
		using namespace collections;
/***********************************************************************
GeneralObjectPool::PoolTree
***********************************************************************/

		GeneralObjectPool::PoolTree::PoolTree(PoolNodeAllocator* allocator)
			:BinTree(allocator)
		{
		}

		GeneralObjectPool::PoolTree::~PoolTree()
		{
		}

		GeneralObjectPool::PoolTree::Node* GeneralObjectPool::PoolTree::Insert(const PoolNodeContent& content)
		{
			Node* node=CreateNode();
			node->value=content;
			InsertNode(node);
			return node;
		}

		GeneralObjectPool::PoolNodeContent GeneralObjectPool::PoolTree::Remove(Node* node)
		{
			PoolNodeContent content=node->value;
			RemoveAndDisposeNode(node);
			return content;
		}

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

		bool GeneralObjectPool::PoolContainer::IsAvailable()
		{
			if(size!=-1 && pools.smallObjectPool)
			{
				return pools.smallObjectPool!=0;
			}
			if(size==-1 && pools.bigObjectPool)
			{
				return pools.bigObjectPool!=0;
			}
			return false;
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
			poolContainer.Finalize(false);
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

		char* GeneralObjectPool::PoolNodeContent::GetEndAddress()const
		{
			if(poolContainer.size!=-1)
			{
				return poolContainer.pools.smallObjectPool->GetEndAddress();
			}
			else
			{
				return poolContainer.pools.bigObjectPool->GetEndAddress();
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

		void GeneralObjectPool::PoolNodeEntry::Collect(List<SmallObjectPool*>& smallPools, List<BigObjectPool*>& bigPools)
		{
			if(free.size!=-1)
			{
				PoolNode* node=first;
				while(node)
				{
					smallPools.Add(node->value.poolContainer.pools.smallObjectPool);
					node=node->value.next;
				}
			}
			else
			{
				PoolNode* node=first;
				while(node)
				{
					bigPools.Add(node->value.poolContainer.pools.bigObjectPool);
					node=node->value.next;
				}
			}
		}

/***********************************************************************
GeneralObjectPool
***********************************************************************/

		GeneralObjectPool::PoolNode* GeneralObjectPool::CreatePoolNode(PoolNodeEntry* entry, vint poolUnitSize)
		{
			PoolNodeContent content;
			content.prev=0;
			content.next=0;
			content.poolContainer=entry->free;
			bool getFromFree=entry->free.IsAvailable();
			if(entry->free.IsAvailable())
			{
				entry->free.Finalize(false);
			}
			else if(entry->free.size!=-1)
			{
				content.poolContainer.pools.smallObjectPool=new SmallObjectPool(entry->free.size, poolUnitSize/entry->free.size);
			}
			else
			{
				content.poolContainer.pools.bigObjectPool=new BigObjectPool(poolUnitSize, 32);
			}

			PoolNode* node=poolTree.Insert(content);
			if(node)
			{
				if(entry->first)
				{
					node->value.next=entry->first;
					entry->first->value.prev=node;
					entry->first=node;
				}
				else
				{
					entry->first=node;
					entry->last=node;
				}
				return node;
			}
			else
			{
				if(getFromFree)
				{
					entry->free=content.poolContainer;
				}
				else
				{
					content.poolContainer.Finalize(true);
				}
				return 0;
			}
		}

		void GeneralObjectPool::DisposePoolNode(PoolNodeEntry* entry, PoolNode* node)
		{
			if(node->value.next)
			{
				node->value.next->value.prev=node->value.prev;
			}
			if(node->value.prev)
			{
				node->value.prev->value.next=node->value.next;
			}
			if(entry->first==node)
			{
				entry->first=node->value.next;
			}
			if(entry->last==node)
			{
				entry->last=node->value.prev;
			}

			if(entry->free.IsAvailable())
			{
				entry->free.Finalize(true);
			}
			entry->free=node->value.poolContainer;
			poolTree.Remove(node);
		}

		GeneralObjectPool::PoolNodeEntry* GeneralObjectPool::FindEntry(vint size)
		{
			if(size<=8)return &pool8;
			if(size<=16)return &pool16;
			if(size<=32)return &pool32;
			if(size<=64)return &pool64;
			if(size<=96)return &pool96;
			return &poolLarge;
		}

		GeneralObjectPool::PoolNode* GeneralObjectPool::FindNode(char* pointer)
		{
			PoolNode* node=poolTree.root;
			while(node)
			{
				if(pointer<node->value.GetStartAddress())
				{
					node=node->left;
				}
				else if(pointer>=node->value.GetEndAddress())
				{
					node=node->right;
				}
				else
				{
					return node;
				}
			}
			return 0;
		}

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
			List<SmallObjectPool*> smalls;
			List<BigObjectPool*> bigs;
			pool8.Collect(smalls, bigs);
			pool16.Collect(smalls, bigs);
			pool32.Collect(smalls, bigs);
			pool64.Collect(smalls, bigs);
			pool96.Collect(smalls, bigs);
			poolLarge.Collect(smalls, bigs);
			poolTree.Clear();

			for(vint i=0;i<smalls.Count();i++)
			{
				delete smalls[i];
			}
			for(vint i=0;i<bigs.Count();i++)
			{
				delete bigs[i];
			}
		}

		char* GeneralObjectPool::Alloc(vint size)
		{
			PoolNodeEntry* entry=FindEntry(size);
			if(entry->first)
			{
				PoolNode* first=entry->first;
				do
				{
					char* object=0;
					if(entry->first->value.poolContainer.size!=-1)
					{
						object=entry->first->value.poolContainer.pools.smallObjectPool->Alloc();
					}
					else
					{
						object=entry->first->value.poolContainer.pools.bigObjectPool->Alloc(size);
					}
					if(object)
					{
						return object;
					}
					if(entry->first==entry->last)
					{
						break;
					}
					else
					{
						entry->last->value.next=entry->first;
						entry->first->value.prev=entry->last;

						entry->last=entry->first;
						entry->first=first->value.next;

						entry->last->value.next=0;
						entry->first->value.prev=0;
					}
				}while(entry->first=first);
			}

			PoolNode* node=CreatePoolNode(entry, poolUnitSize);
			if(entry->first->value.poolContainer.size!=-1)
			{
				return entry->first->value.poolContainer.pools.smallObjectPool->Alloc();
			}
			else
			{
				return entry->first->value.poolContainer.pools.bigObjectPool->Alloc(size);
			}
		}

		bool GeneralObjectPool::Free(char* handle)
		{
			PoolNode* node=FindNode(handle);
			if(node)
			{
				if(node->value.poolContainer.size!=-1)
				{
					if(!node->value.poolContainer.pools.smallObjectPool->Free(handle))
					{
						return false;
					}
					else if(node->value.poolContainer.pools.smallObjectPool->GetUsedCount()!=0)
					{
						return true;
					}
				}
				else
				{
					if(!node->value.poolContainer.pools.bigObjectPool->Free(handle))
					{
						return false;
					}
					else if(node->value.poolContainer.pools.bigObjectPool->GetUsedSize()!=0)
					{
						return true;
					}
				}
				PoolNodeEntry* entry=FindEntry(node->value.poolContainer.size);
				DisposePoolNode(entry, node);
				return true;
			}
			else
			{
				return false;
			}
		}

		bool GeneralObjectPool::IsValid(char* handle)
		{
			PoolNode* node=FindNode(handle);
			if(node)
			{
				if(node->value.poolContainer.size!=-1)
				{
					return node->value.poolContainer.pools.smallObjectPool->IsValid(handle);
				}
				else
				{
					return node->value.poolContainer.pools.bigObjectPool->IsValid(handle);
				}
			}
			else
			{
				return false;
			}
		}

		char* GeneralObjectPool::GetHandle(char* pointer)
		{
			PoolNode* node=FindNode(pointer);
			if(node)
			{
				if(node->value.poolContainer.size!=-1)
				{
					return node->value.poolContainer.pools.smallObjectPool->GetHandle(pointer);
				}
				else
				{
					return node->value.poolContainer.pools.bigObjectPool->GetHandle(pointer);
				}
			}
			else
			{
				return 0;
			}
		}

		vint GeneralObjectPool::GetSize(char* handle)
		{
			PoolNode* node=FindNode(handle);
			if(node)
			{
				if(node->value.poolContainer.size!=-1)
				{
					return node->value.poolContainer.pools.smallObjectPool->IsValid(handle)
						?node->value.poolContainer.size
						:-1;
				}
				else
				{
					return node->value.poolContainer.pools.bigObjectPool->GetSize(handle);
				}
			}
			else
			{
				return -1;
			}
		}
	}
}