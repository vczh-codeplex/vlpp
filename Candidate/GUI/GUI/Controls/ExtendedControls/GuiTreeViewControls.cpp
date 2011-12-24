#include "GuiTreeViewControls.h"

namespace vl
{
	namespace presentation
	{
		namespace controls
		{
			namespace tree
			{
				const wchar_t* INodeItemView::Identifier = L"vl::presentation::controls::tree::INodeItemView";

/***********************************************************************
NodeItemProvider
***********************************************************************/

				INodeProvider* NodeItemProvider::GetNodeByOffset(INodeProvider* provider, int offset)
				{
					if(offset==0) return provider;
					INodeProvider* result=0;
					if(provider->GetExpanding() && offset>0)
					{
						offset-=1;
						int count=provider->GetChildCount();
						for(int i=0;(!result && i<count);i++)
						{
							INodeProvider* child=provider->RequestChild(i);
							int visibleCount=child->CalculateTotalVisibleNodes();
							if(offset<visibleCount)
							{
								result=GetNodeByOffset(child, offset);
							}
							else
							{
								offset-=visibleCount;
							}
						}
					}
					ReleaseNode(provider);
					return result;
				}

				void NodeItemProvider::OnAttached(INodeRootProvider* provider)
				{
				}

				void NodeItemProvider::OnBeforeItemModified(INodeProvider* parentNode, int start, int count, int newCount)
				{
					offsetBeforeChildModified=0;
					int base=CalculateNodeVisibilityIndexInternal(parentNode);
					if(base!=-2 && parentNode->GetExpanding())
					{
						for(int i=0;i<count;i++)
						{
							INodeProvider* child=parentNode->RequestChild(start+i);
							offsetBeforeChildModified+=child->CalculateTotalVisibleNodes();
							parentNode->ReleaseChild(child);
						}
					}
				}

				void NodeItemProvider::OnAfterItemModified(INodeProvider* parentNode, int start, int count, int newCount)
				{
					int base=CalculateNodeVisibilityIndexInternal(parentNode);
					if(base!=-2 && parentNode->GetExpanding())
					{
						int offset=0;
						int firstChildStart=-1;
						for(int i=0;i<newCount;i++)
						{
							INodeProvider* child=parentNode->RequestChild(start+i);
							if(i==0)
							{
								firstChildStart=CalculateNodeVisibilityIndexInternal(child);
							}
							offset+=child->CalculateTotalVisibleNodes();
							parentNode->ReleaseChild(child);
						}

						if(firstChildStart==-1)
						{
							int childCount=parentNode->GetChildCount();
							if(childCount==0)
							{
								firstChildStart=base+1;
							}
							else if(start<childCount)
							{
								INodeProvider* child=parentNode->RequestChild(start);
								firstChildStart=CalculateNodeVisibilityIndexInternal(child);
								parentNode->ReleaseChild(child);
							}
							else
							{
								INodeProvider* child=parentNode->RequestChild(start-1);
								firstChildStart=CalculateNodeVisibilityIndexInternal(child);
								firstChildStart+=child->CalculateTotalVisibleNodes();
								parentNode->ReleaseChild(child);
							}
						}
						InvokeOnItemModified(firstChildStart, offsetBeforeChildModified, offset);
					}
				}

				void NodeItemProvider::OnItemExpanded(INodeProvider* node)
				{
					int base=CalculateNodeVisibilityIndexInternal(node);
					if(base!=-2)
					{
						int visibility=node->CalculateTotalVisibleNodes();
						InvokeOnItemModified(base, 1, visibility);
					}
				}

				void NodeItemProvider::OnItemCollapsed(INodeProvider* node)
				{
					int base=CalculateNodeVisibilityIndexInternal(node);
					if(base!=-2)
					{
						int visibility=node->CalculateTotalVisibleNodes();
						InvokeOnItemModified(base, visibility, 1);
					}
				}

				int NodeItemProvider::CalculateNodeVisibilityIndexInternal(INodeProvider* node)
				{
					INodeProvider* parent=node->GetParent();
					if(parent==0)
					{
						return -1;
					}
					if(!parent->GetExpanding())
					{
						return -2;
					}

					int index=CalculateNodeVisibilityIndexInternal(parent);
					if(index==-2)
					{
						return -2;
					}

					int count=parent->GetChildCount();
					for(int i=0;i<count;i++)
					{
						INodeProvider* child=parent->RequestChild(i);
						bool findResult=child==node;
						if(findResult)
						{
							index++;
						}
						else
						{
							index+=child->CalculateTotalVisibleNodes();
						}
						parent->ReleaseChild(child);
						if(findResult)
						{
							return index;
						}
					}
					return -1;
				}

				int NodeItemProvider::CalculateNodeVisibilityIndex(INodeProvider* node)
				{
					int result=CalculateNodeVisibilityIndexInternal(node);
					return result<0?-1:result;
				}

				INodeProvider* NodeItemProvider::RequestNode(int index)
				{
					return GetNodeByOffset(root.Obj(), index+1);
				}

				void NodeItemProvider::ReleaseNode(INodeProvider* node)
				{
					if(node)
					{
						INodeProvider* parent=node;
						if(parent)
						{
							parent->ReleaseChild(node);
						}
					}
				}

				NodeItemProvider::NodeItemProvider(INodeRootProvider* _root)
					:root(_root)
				{
					root->AttachCallback(this);
				}

				NodeItemProvider::~NodeItemProvider()
				{
					root->DetachCallback(this);
				}

				Ptr<INodeRootProvider> NodeItemProvider::GetRoot()
				{
					return root;
				}

				int NodeItemProvider::Count()
				{
					return root->CalculateTotalVisibleNodes()-1;
				}

				Interface* NodeItemProvider::RequestView(const WString& identifier)
				{
					if(identifier==INodeItemView::Identifier)
					{
						return (INodeItemView*)this;
					}
					else
					{
						return 0;
					}
				}

				void NodeItemProvider::ReleaseView(Interface* view)
				{
				}

/***********************************************************************
MemoryNodeProviderBase
***********************************************************************/

				INodeProviderCallback* MemoryNodeProviderBase::GetCallbackProxyInternal()
				{
					if(parent)
					{
						return parent->GetCallbackProxyInternal();
					}
					else
					{
						return 0;
					}
				}

				void MemoryNodeProviderBase::OnChildTotalVisibleNodesChanged(int offset)
				{
					totalVisibleNodeCount+=offset;
					if(parent)
					{
						parent->OnChildTotalVisibleNodesChanged(offset);
					}
				}

				void MemoryNodeProviderBase::OnBeforeChildModified(int start, int count, int newCount)
				{
					offsetBeforeChildModified=0;
					if(expanding)
					{
						for(int i=0;i<count;i++)
						{
							offsetBeforeChildModified+=GetChildInternal(start+i)->totalVisibleNodeCount;
						}
					}
					INodeProviderCallback* proxy=GetCallbackProxyInternal();
					if(proxy)
					{
						proxy->OnBeforeItemModified(this, start, count, newCount);
					}
				}

				void MemoryNodeProviderBase::OnAfterChildModified(int start, int count, int newCount)
				{
					childCount+=(newCount-count);
					if(expanding)
					{
						int offset=0;
						for(int i=0;i<newCount;i++)
						{
							offset+=GetChildInternal(start+i)->totalVisibleNodeCount;
						}
						OnChildTotalVisibleNodesChanged(offset-offsetBeforeChildModified);
					}
					INodeProviderCallback* proxy=GetCallbackProxyInternal();
					if(proxy)
					{
						proxy->OnAfterItemModified(this, start, count, newCount);
					}
				}

				bool MemoryNodeProviderBase::OnRequestRemove(MemoryNodeProviderBase* child)
				{
					if(child->parent==this)
					{
						child->parent=0;
						return true;
					}
					return false;
				}

				bool MemoryNodeProviderBase::OnRequestInsert(MemoryNodeProviderBase* child)
				{
					if(child->parent==0)
					{
						child->parent=this;
						return true;
					}
					return false;
				}

				MemoryNodeProviderBase::MemoryNodeProviderBase()
					:parent(0)
					,expanding(false)
					,childCount(0)
					,totalVisibleNodeCount(1)
					,offsetBeforeChildModified(0)
				{
				}

				MemoryNodeProviderBase::~MemoryNodeProviderBase()
				{
				}

				bool MemoryNodeProviderBase::GetExpanding()
				{
					return expanding;
				}

				void MemoryNodeProviderBase::SetExpanding(bool value)
				{
					if(expanding!=value)
					{
						expanding=value;
						int offset=0;
						for(int i=0;i<childCount;i++)
						{
							offset+=GetChildInternal(i)->totalVisibleNodeCount;
						}

						if(expanding) OnChildTotalVisibleNodesChanged(offset);
						INodeProviderCallback* proxy=GetCallbackProxyInternal();
						if(proxy)
						{
							if(expanding)
							{
								proxy->OnItemExpanded(this);
							}
							else
							{
								proxy->OnItemCollapsed(this);
							}
						}
						if(!expanding) OnChildTotalVisibleNodesChanged(-offset);
					}
				}

				int MemoryNodeProviderBase::CalculateTotalVisibleNodes()
				{
					return totalVisibleNodeCount;
				}

				int MemoryNodeProviderBase::GetChildCount()
				{
					return childCount;
				}

				INodeProvider* MemoryNodeProviderBase::GetParent()
				{
					return parent;
				}

				INodeProvider* MemoryNodeProviderBase::RequestChild(int index)
				{
					if(0<=index && index<childCount)
					{
						return GetChildInternal(index);
					}
					else
					{
						return 0;
					}
				}

				void MemoryNodeProviderBase::ReleaseChild(INodeProvider* node)
				{
				}

/***********************************************************************
MemoryNodeRootProviderBase
***********************************************************************/

				INodeProviderCallback* MemoryNodeRootProviderBase::GetCallbackProxyInternal()
				{
					return this;
				}

				void MemoryNodeRootProviderBase::OnAttached(INodeRootProvider* provider)
				{
				}

				void MemoryNodeRootProviderBase::OnBeforeItemModified(INodeProvider* parentNode, int start, int count, int newCount)
				{
					for(int i=0;i<callbacks.Count();i++)
					{
						callbacks[i]->OnBeforeItemModified(parentNode, start, count, newCount);
					}
				}

				void MemoryNodeRootProviderBase::OnAfterItemModified(INodeProvider* parentNode, int start, int count, int newCount)
				{
					for(int i=0;i<callbacks.Count();i++)
					{
						callbacks[i]->OnAfterItemModified(parentNode, start, count, newCount);
					}
				}

				void MemoryNodeRootProviderBase::OnItemExpanded(INodeProvider* node)
				{
					for(int i=0;i<callbacks.Count();i++)
					{
						callbacks[i]->OnItemExpanded(node);
					}
				}

				void MemoryNodeRootProviderBase::OnItemCollapsed(INodeProvider* node)
				{
					for(int i=0;i<callbacks.Count();i++)
					{
						callbacks[i]->OnItemCollapsed(node);
					}
				}

				MemoryNodeRootProviderBase::MemoryNodeRootProviderBase()
				{
				}

				MemoryNodeRootProviderBase::~MemoryNodeRootProviderBase()
				{
				}

				bool MemoryNodeRootProviderBase::AttachCallback(INodeProviderCallback* value)
				{
					if(callbacks.Contains(value))
					{
						return false;
					}
					else
					{
						callbacks.Add(value);
						value->OnAttached(this);
						return true;
					}
				}

				bool MemoryNodeRootProviderBase::DetachCallback(INodeProviderCallback* value)
				{
					int index=callbacks.IndexOf(value);
					if(index==-1)
					{
						return false;
					}
					else
					{
						value->OnAttached(0);
						callbacks.Remove(value);
						return true;
					}
				}
			}
		}
	}
}