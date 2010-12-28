/***********************************************************************
Vczh Library++ 3.0
Developer: 陈梓瀚(vczh)
Framework::GeneralObjectPoolEntity

Classes:
	GeneralObjectPool							：通用对象池
***********************************************************************/

#ifndef VCZH_ENTITY_GENERALOBJECTPOOLENTITY
#define VCZH_ENTITY_GENERALOBJECTPOOLENTITY

#include "SmallObjectPoolEntity.h"
#include "BigObjectPoolEntity.h"
#include "BinaryBalanceTreeEntity.h"
#include "..\Collections\List.h"

namespace vl
{
	namespace entities
	{
		class GeneralObjectPool : public Object, public NotCopyable
		{
		protected:
			struct PoolNodeContent;
			class PoolNodeAllocator;

			class PoolTree : public BinTree<PoolNodeContent, PoolNodeAllocator>
			{
			public:
				PoolTree(PoolNodeAllocator* allocator);
				~PoolTree();

				Node*								Insert(const PoolNodeContent& content);
				PoolNodeContent						Remove(Node* node);
			};

			typedef PoolTree::Node					PoolNode;

			class PoolNodeAllocator
			{
			protected:
				SmallObjectPool						nodePool;
			public:
				PoolNodeAllocator(vint maxCount);
				~PoolNodeAllocator();

				PoolNode*							CreateNode();
				void								DisposeNode(PoolNode* node);
			};

			struct PoolContainer
			{
				union
				{
					SmallObjectPool*				smallObjectPool;
					BigObjectPool*					bigObjectPool;
				}									pools;
				vint								size;

				void								Initialize();
				void								Finalize(bool deletePool);
				bool								IsAvailable();
			};

			struct PoolNodeContent
			{
				PoolContainer						poolContainer;
				PoolNode*							prev;
				PoolNode*							next;

				PoolNodeContent();
				~PoolNodeContent();

				void								Initialize();
				void								Finalize();

				char*								GetStartAddress()const;
				char*								GetEndAddress()const;
				bool								operator<(const PoolNodeContent& node)const;
				bool								operator<=(const PoolNodeContent& node)const;
				bool								operator>(const PoolNodeContent& node)const;
				bool								operator>=(const PoolNodeContent& node)const;
				bool								operator==(const PoolNodeContent& node)const;
				bool								operator!=(const PoolNodeContent& node)const;
			};

			struct PoolNodeEntry
			{
				PoolNode*							first;
				PoolNode*							last;
				PoolContainer						free;

				PoolNodeEntry(vint _size);
				~PoolNodeEntry();

				void								Collect(collections::List<SmallObjectPool*>& smallPools, collections::List<BigObjectPool*>& bigPools);
			};
		protected:
			vint									poolUnitSize;
			vint									poolUnitCount;
			PoolNodeAllocator						poolNodeAllocator;
			PoolTree								poolTree;

			PoolNodeEntry							pool8;
			PoolNodeEntry							pool16;
			PoolNodeEntry							pool32;
			PoolNodeEntry							pool64;
			PoolNodeEntry							pool96;
			PoolNodeEntry							poolLarge;

			PoolNode*								CreatePoolNode(PoolNodeEntry* entry, vint poolUnitSize);
			void									DisposePoolNode(PoolNodeEntry* entry, PoolNode* node);
			PoolNodeEntry*							FindEntry(vint size);
			PoolNode*								FindNode(char* pointer);
		public:
			GeneralObjectPool(vint _poolUnitSize, vint _poolUnitCount);
			~GeneralObjectPool();

			char*									Alloc(vint size);
			bool									Free(char* handle);
			bool									IsValid(char* handle);
			char*									GetHandle(char* pointer);
			vint									GetSize(char* handle);
		};
	}
}

#endif