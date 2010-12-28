/***********************************************************************
Vczh Library++ 3.0
Developer: 陈梓瀚(vczh)
Framework::SmallObjectPoolEntity

Classes:
	BinTree<T>								：平衡树
	BinTreeDefaultAllocator<T>				：平衡树缺省节点分配器
	BinValueTree<T>							：值平衡树
***********************************************************************/

#ifndef VCZH_ENTITY_BINARYBALANCETREEENTITY
#define VCZH_ENTITY_BINARYBALANCETREEENTITY

#include "..\Basic.h"

namespace vl
{
	namespace entities
	{
		template<typename T, typename _Allocator>
		class BinTree : public Object, public NotCopyable
		{
		public:
			struct Node
			{
				T				value;
				Node*			parent;
				Node*			left;
				Node*			right;
			};
		protected:

			_Allocator*			allocator;

			inline vint Compare(Node* a, Node* b)
			{
				if(a->value < b->value)
				{
					return -1;
				}
				else if(a->value > b->value)
				{
					return 1;
				}
				else
				{
					return 0;
				}
			}

			inline Node* CreateNode()
			{
				Node* node=allocator->CreateNode();
				node->parent=0;
				node->left=0;
				node->right=0;
				return node;
			}

			inline void DisposeNode(Node* node)
			{
				allocator->DisposeNode(node);
			}

			void DisposeTree(Node* node)
			{
				if(node)
				{
					DisposeTree(node->left);
					DisposeTree(node->right);
					DisposeNode(node);
				}
			}

			void InsertNode(Node* node)
			{
			}

			void RemoveAndDisposeNode(Node* node)
			{
			}
		public:
			Node*				root;

			BinTree(_Allocator* _allocator)
				:allocator(_allocator)
				,root(0)
			{
			}

			~BinTree()
			{
				Clear();
			}

			void Clear()
			{
				DisposeTree(root);
				root=0;
			}
		};

		template<typename T>
		class BinTreeDefaultAllocator : public Object, public NotCopyable
		{
			typedef typename BinTree<T, BinTreeDefaultAllocator<T>>::Node		NodeType;
		public:
			NodeType* CreateNode()
			{
				return new NodeType;
			}

			void DisposeNode(NodeType* node)
			{
				delete node;
			}
		};

		template<typename T>
		class BinValueTree : public BinTree<T, BinTreeDefaultAllocator<T>>
		{
		protected:
			BinTreeDefaultAllocator<T>		allocator;

		public:
			BinValueTree()
				:BinTree(&allocator)
			{
			}

			Node* Insert(const T& value)
			{
				Node* node=CreateNode();
				if(node)
				{
					node->value=value;
					InsertNode(node);
				}
				return node;
			}

			Node* Find(const T& value)
			{
				Node* current=root;
				while(current)
				{
					if(value < current->value)
					{
						current=current->left;
					}
					else if(value > current->value)
					{
						current=current->right;
					}
					else
					{
						return current;
					}
				}
				return 0;
			}

			bool Remove(const T& value)
			{
				Node* node=Find(value);
				return RemoveNode(node);
			}

			bool RemoveNode(Node* node)
			{
				if(node)
				{
					RemoveAndDisposeNode(node);
					return true;
				}
				else
				{
					return false;
				}
			}
		};
	}
}

#endif