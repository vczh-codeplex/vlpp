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

namespace vl
{
	namespace entities
	{
		class GeneralObjectPool : public Object, public NotCopyable
		{
		protected:
			struct PoolNodeContent;
			class PoolNodeAllocator;
			typedef BinTree<PoolNodeContent, PoolNodeAllocator>		PoolTree;
			typedef PoolTree::Node									PoolNode;

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
		public:
			GeneralObjectPool(vint _poolUnitSize, vint _poolUnitCount);
			~GeneralObjectPool();
		};
	}
}

#endif