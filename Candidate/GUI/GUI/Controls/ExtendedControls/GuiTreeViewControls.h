/***********************************************************************
Vczh Library++ 3.0
Developer: ³Âè÷å«(vczh)
GacUI::Control System

Interfaces:
***********************************************************************/

#ifndef VCZH_PRESENTATION_CONTROLS_GUITREEVIEWCONTROLS
#define VCZH_PRESENTATION_CONTROLS_GUITREEVIEWCONTROLS

#include "..\GuiListControls.h"

namespace vl
{
	namespace presentation
	{
		namespace controls
		{

/***********************************************************************
TreeView NodeProvider
***********************************************************************/

			namespace tree
			{
				class INodeProvider;
				class INodeRootProvider;

				//-----------------------------------------------------------
				// Callback Interfaces
				//-----------------------------------------------------------

				class INodeProviderCallback : public virtual Interface
				{
				public:
					virtual void					OnAttached(INodeRootProvider* provider)=0;
					virtual void					OnBeforeItemModified(INodeProvider* parentNode, int start, int count, int newCount)=0;
					virtual void					OnAfterItemModified(INodeProvider* parentNode, int start, int count, int newCount)=0;
					virtual void					OnItemExpanded(INodeProvider* node)=0;
					virtual void					OnItemCollapsed(INodeProvider* node)=0;
				};

				//-----------------------------------------------------------
				// Provider Interfaces
				//-----------------------------------------------------------

				class INodeProvider : public virtual Interface
				{
				public:
					virtual bool					GetExpanding()=0;
					virtual void					SetExpanding(bool value)=0;
					virtual int						CalculateTotalVisibleNodes()=0;

					virtual int						GetChildCount()=0;
					virtual INodeProvider*			GetParent()=0;
					virtual INodeProvider*			RequestChild(int index)=0;
					virtual void					ReleaseChild(INodeProvider* node)=0;
				};

				class INodeRootProvider : public virtual INodeProvider
				{
				public:
					virtual bool					AttachCallback(INodeProviderCallback* value)=0;
					virtual bool					DetachCallback(INodeProviderCallback* value)=0;
				};
			}

/***********************************************************************
TreeView Predefined NodeProvider
***********************************************************************/

			namespace tree
			{
				//-----------------------------------------------------------
				// Tree to ListControl
				//-----------------------------------------------------------

				class INodeItemView : public virtual Interface
				{
				public:
					static const wchar_t*			Identifier;

					virtual INodeProvider*			RequestNode(int index)=0;
					virtual void					ReleaseNode(INodeProvider* node)=0;
					virtual int						CalculateNodeVisibilityIndex(INodeProvider* node)=0;
				};

				class NodeItemProvider
					: public list::ItemProviderBase
					, protected virtual INodeProviderCallback
					, protected virtual INodeItemView
				{
				protected:
					Ptr<INodeRootProvider>			root;
					int								offsetBeforeChildModified;

					INodeProvider*					GetNodeByOffset(INodeProvider* provider, int offset);
					void							OnAttached(INodeRootProvider* provider)override;
					void							OnBeforeItemModified(INodeProvider* parentNode, int start, int count, int newCount)override;
					void							OnAfterItemModified(INodeProvider* parentNode, int start, int count, int newCount)override;
					void							OnItemExpanded(INodeProvider* node)override;
					void							OnItemCollapsed(INodeProvider* node)override;
					int								CalculateNodeVisibilityIndexInternal(INodeProvider* node);
					int								CalculateNodeVisibilityIndex(INodeProvider* node)override;

					INodeProvider*					RequestNode(int index)override;
					void							ReleaseNode(INodeProvider* node)override;
				public:
					NodeItemProvider(INodeRootProvider* _root);
					~NodeItemProvider();

					Ptr<INodeRootProvider>			GetRoot();
					int								Count()override;
					Interface*						RequestView(const WString& identifier)override;
					void							ReleaseView(Interface* view)override;
				};
			}

			namespace tree
			{
				class MemoryNodeProviderBase : public Object, public virtual INodeProvider
				{
				protected:
					MemoryNodeProviderBase*			parent;
					bool							expanding;
					int								childCount;
					int								totalVisibleNodeCount;
					int								offsetBeforeChildModified;

					virtual MemoryNodeProviderBase* GetChildInternal(int index)=0;
					virtual INodeProviderCallback*	GetCallbackProxyInternal();
					void							OnChildTotalVisibleNodesChanged(int offset);
					void							OnBeforeChildModified(int start, int count, int newCount);
					void							OnAfterChildModified(int start, int count, int newCount);
					bool							OnRequestRemove(MemoryNodeProviderBase* child);
					bool							OnRequestInsert(MemoryNodeProviderBase* child);
				public:
					MemoryNodeProviderBase();
					~MemoryNodeProviderBase();

					bool							GetExpanding()override;
					void							SetExpanding(bool value)override;
					int								CalculateTotalVisibleNodes()override;

					int								GetChildCount()override;
					INodeProvider*					GetParent()override;
					INodeProvider*					RequestChild(int index)override;
					void							ReleaseChild(INodeProvider* node)override;
				};
				
#pragma warning(push)
#pragma warning(disable:4250)
				class MemoryNodeRootProviderBase
					: public MemoryNodeProviderBase
					, public virtual INodeRootProvider
					, private virtual INodeProviderCallback
				{
				protected:
					collections::List<INodeProviderCallback*>		callbacks;

					INodeProviderCallback*			GetCallbackProxyInternal()override;
				private:
					void							OnAttached(INodeRootProvider* provider)override;
					void							OnBeforeItemModified(INodeProvider* parentNode, int start, int count, int newCount)override;
					void							OnAfterItemModified(INodeProvider* parentNode, int start, int count, int newCount)override;
					void							OnItemExpanded(INodeProvider* node)override;
					void							OnItemCollapsed(INodeProvider* node)override;
				public:
					MemoryNodeRootProviderBase();
					~MemoryNodeRootProviderBase();

					bool							AttachCallback(INodeProviderCallback* value)override;
					bool							DetachCallback(INodeProviderCallback* value)override;
				};

				template<typename TBase, typename TNode>
				class MemoryNodeProviderWithChildren : public TBase, private collections::IList<Ptr<TNode>>
				{
				protected:
					collections::List<Ptr<TNode>>	children;

					MemoryNodeProviderBase* GetChildInternal(int index)override
					{
						return children[index].Obj();
					}
				public:
					collections::IList<Ptr<TNode>>& Children()
					{
						return *this;
					}

				private:
					//---------------------------------------------------

					collections::IEnumerator<Ptr<TNode>>* CreateEnumerator()const
					{
						return children.Wrap().CreateEnumerator();
					}

					bool Contains(const typename KeyType<Ptr<TNode>>::Type& item)const
					{
						return children.Contains(item);
					}

					vint Count()const
					{
						return children.Count();
					}

					vint Count()
					{
						return children.Count();
					}

					const Ptr<TNode>& Get(vint index)const
					{
						return children.Get(index);
					}

					const Ptr<TNode>& operator[](vint index)const
					{
						return children.Get(index);
					}

					vint IndexOf(const typename KeyType<Ptr<TNode>>::Type& item)const
					{
						return children.IndexOf(item);
					}

					vint Add(const Ptr<TNode>& item)
					{
						return Insert(children.Count(), item);
					}

					bool Remove(const typename KeyType<Ptr<TNode>>::Type& item)
					{
						vint index=children.IndexOf(item);
						if(index==-1) return false;
						return RemoveAt(index);
					}

					bool RemoveAt(vint index)
					{
						if(0<=index && index<children.Count())
						{
							if(OnRequestRemove(children[index].Obj()))
							{
								OnBeforeChildModified(index, 1, 0);
								children.RemoveAt(index);
								OnAfterChildModified(index, 1, 0);
								return true;
							}
						}
						return false;
					}

					bool RemoveRange(vint index, vint count)
					{
						for(int i=0;i<index;i++)
						{
							int j=index+i;
							if(0<=j && j<children.Count())
							{
								OnRequestRemove(children[j].Obj());
							}
						}
						OnBeforeChildModified(index, count, 0);
						children.RemoveRange(index, count);
						OnAfterChildModified(index, count, 0);
						return true;
					}

					bool Clear()
					{
						return RemoveRange(0, children.Count());
					}

					vint Insert(vint index, const Ptr<TNode>& item)
					{
						if(OnRequestInsert(item.Obj()))
						{
							OnBeforeChildModified(index, 0, 1);
							vint result=children.Insert(index, item);
							OnAfterChildModified(index, 0, 1);
							return result;
						}
						return -1;
					}

					bool Set(vint index, const Ptr<TNode>& item)
					{
						if(0<=index && index<children.Count())
						{
							if(OnRequestInsert(item.Obj()))
							{
								OnRequestRemove(children[index].Obj());
								OnBeforeChildModified(index, 1, 1);
								children[index]=item;
								OnAfterChildModified(index, 1, 1);
								return true;
							}
						}
						return false;
					}
				};

				template<typename T>
				class MemoryNodeProvider : public MemoryNodeProviderWithChildren<MemoryNodeProviderBase, MemoryNodeProvider<T>>
				{
				protected:
					T							data;

				public:
					MemoryNodeProvider()
					{
					}

					MemoryNodeProvider(const T& _data)
						:data(_data)
					{
					}

					~MemoryNodeProvider()
					{
					}

					const T& GetData()
					{
						return data;
					}
				};
				
				template<typename T>
				class MemoryNodeRootProvider : public MemoryNodeProviderWithChildren<MemoryNodeRootProviderBase, MemoryNodeProvider<T>>
				{
				public:
					MemoryNodeRootProvider()
					{
						SetExpanding(true);
					}
				};
#pragma warning(pop)
			}

/***********************************************************************
TreeView
***********************************************************************/
		}
	}
}

#endif