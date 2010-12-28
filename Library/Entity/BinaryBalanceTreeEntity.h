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

			void Balance(Node* node)
			{
			}

			void InsertNode(Node* node)
			{
				if(root==0)
				{
					root=node;
				}
				else
				{
					Node* current=root;
					while(true)
					{
						if(node->value < current->value)
						{
							if(current->left)
							{
								current=current->left;
							}
							else
							{
								current->left=node;
								node->parent=current;
								break;
							}
						}
						else
						{
							if(current->right)
							{
								current=current->right;
							}
							else
							{
								current->right=node;
								node->parent=current;
								break;
							}
						}
					}
					Balance(node);
				}
			}

			void RemoveAndDisposeNode(Node* node)
			{
				Node* parent=node->parent;
				Node** parentSlot=0;
				if(!parent)
				{
					parentSlot=&root;
				}
				else if(node==parent->left)
				{
					parentSlot=&parent->left;
				}
				else
				{
					parentSlot=&parent->right;
				}

				if(!node->right)
				{
					*parentSlot=node->left;
					if(node->left)
					{
						node->left->parent=parent;
					}
				}
				else if(!node->right->left)
				{
					node->right->left=node->left;
					if(node->left)
					{
						node->left->parent=node->right;
					}

					*parentSlot=node->right;
					node->right->parent=parent;
				}
				else
				{
					Node* current=node->right->left;
					while(current->left)
					{
						current=current->left;
					}

					current->parent->left=current->right;
					if(current->right)
					{
						current->right->parent=current->parent;
					}

					*parentSlot=current;
					current->parent=node->parent;

					current->left=node->left;
					if(node->left)
					{
						node->left->parent=current;
					}

					current->right=node->right;
					node->right->parent=current;
				}
				DisposeNode(node);

				if(*parentSlot)
				{
					Balance(*parentSlot);
				}
				else if(parent)
				{
					Balance(parent);
				}
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