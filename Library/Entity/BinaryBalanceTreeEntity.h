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
				vint			depth;
				Node*			parent;
				Node*			left;
				Node*			right;

				inline vint Depth()
				{
					return this?depth:0;
				}
			};
		protected:

			_Allocator*			allocator;

			inline Node* CreateNode()
			{
				Node* node=allocator->CreateNode();
				node->parent=0;
				node->left=0;
				node->right=0;
				node->depth=1;
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

			void UpdateDepthWithoutParent(Node* node)
			{
				vint leftDepth=node->left->Depth();
				vint rightDepth=node->right->Depth();
				node->depth=1+(leftDepth>rightDepth?leftDepth:rightDepth);
			}

			void UpdateDepth(Node* node)
			{
				while(node)
				{
					UpdateDepthWithoutParent(node);
					node=node->parent;
				}
			}

			void Balance(Node* node)
			{
				while(node)
				{
					UpdateDepthWithoutParent(node);
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

					vint leftDepth=node->left->Depth();
					vint rightDepth=node->right->Depth();
					vint dDepth=leftDepth-rightDepth;
					if(dDepth>1)
					{
						Node* a=node;
						Node* b=node->left;
						leftDepth=b->left->Depth();
						rightDepth=b->right->Depth();
						vint dDepth=leftDepth-rightDepth;
						if(dDepth>=0)
						{
							Node* x=b->right;
							b->right=a;
							a->left=x;
							a->parent=b;
							if(x)x->parent=a;

							*parentSlot=b;
							b->parent=parent;
							node=b;
							UpdateDepthWithoutParent(a);
							UpdateDepthWithoutParent(b);
						}
						else
						{
							Node* c=b->right;
							Node* x=c->left;
							Node* y=c->right;

							c->left=b;
							c->right=a;
							b->parent=c;
							a->parent=c;
							b->right=x;
							a->left=y;
							if(x)x->parent=b;
							if(y)y->parent=a;

							*parentSlot=c;
							c->parent=parent;
							node=c;
							UpdateDepthWithoutParent(a);
							UpdateDepthWithoutParent(b);
							UpdateDepthWithoutParent(c);
						}
					}
					else if(dDepth<-1)
					{
						Node* a=node;
						Node* b=node->right;
						leftDepth=b->left->Depth();
						rightDepth=b->right->Depth();
						vint dDepth=leftDepth-rightDepth;
						if(dDepth<=0)
						{
							Node* x=b->left;
							b->left=a;
							a->right=x;
							a->parent=b;
							if(x)x->parent=a;

							*parentSlot=b;
							b->parent=parent;
							node=b;
							UpdateDepthWithoutParent(a);
							UpdateDepthWithoutParent(b);
						}
						else
						{
							Node* c=b->left;
							Node* x=c->right;
							Node* y=c->left;

							c->right=b;
							c->left=a;
							b->parent=c;
							a->parent=c;
							b->left=x;
							a->right=y;
							if(x)x->parent=b;
							if(y)y->parent=a;

							*parentSlot=c;
							c->parent=parent;
							node=c;
							UpdateDepthWithoutParent(a);
							UpdateDepthWithoutParent(b);
							UpdateDepthWithoutParent(c);
						}
					}
					node=parent;
				}
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
					UpdateDepth(node);
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
					UpdateDepth(parent);
					Balance(parent);
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
					UpdateDepth(node->right);
					Balance(node->right);
				}
				else
				{
					Node* current=node->right->left;
					while(current->left)
					{
						current=current->left;
					}
					Node* balanceNode=current->parent;

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

					UpdateDepth(balanceNode);
					Balance(balanceNode);
				}
				DisposeNode(node);
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