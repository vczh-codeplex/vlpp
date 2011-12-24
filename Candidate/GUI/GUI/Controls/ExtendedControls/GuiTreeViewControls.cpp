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
					return GetNodeByOffset(root->GetRootNode(), index+1);
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
					return root->GetRootNode()->CalculateTotalVisibleNodes()-1;
				}

				Interface* NodeItemProvider::RequestView(const WString& identifier)
				{
					if(identifier==INodeItemView::Identifier)
					{
						return (INodeItemView*)this;
					}
					else
					{
						return root->RequestView(identifier);
					}
				}

				void NodeItemProvider::ReleaseView(Interface* view)
				{
					if(dynamic_cast<INodeItemView*>(view)==0)
					{
						root->ReleaseView(view);
					}
				}

/***********************************************************************
NodeItemStyleControllerBase
***********************************************************************/

				NodeItemStyleControllerBase::NodeItemStyleControllerBase(INodeItemStyleProvider* _provider, int _styleId)
					:list::ItemStyleControllerBase(_provider->GetBindedItemStyleProvider(), styleId)
					,provider(_provider)
				{
				}

/***********************************************************************
NodeItemProvider
***********************************************************************/

				NodeItemStyleProvider::NodeItemStyleProvider(Ptr<INodeItemStyleProvider> provider)
					:nodeItemStyleProvider(provider)
					,listControl(0)
					,nodeItemView(0)
				{
					nodeItemStyleProvider->BindItemStyleProvider(this);
				}

				NodeItemStyleProvider::~NodeItemStyleProvider()
				{
				}

				void NodeItemStyleProvider::AttachListControl(GuiListControl* value)
				{
					listControl=value;
					nodeItemView=dynamic_cast<INodeItemView*>(listControl->GetItemProvider()->RequestView(INodeItemView::Identifier));
					nodeItemStyleProvider->AttachListControl(value);
				}

				void NodeItemStyleProvider::DetachListControl()
				{
					nodeItemStyleProvider->DetachListControl();
					if(nodeItemView)
					{
						listControl->GetItemProvider()->ReleaseView(nodeItemView);
						nodeItemView=0;
					}
					listControl=0;
				}

				int NodeItemStyleProvider::GetItemStyleId(int itemIndex)
				{
					int result=-1;
					if(nodeItemView)
					{
						INodeProvider* node=nodeItemView->RequestNode(itemIndex);
						if(node)
						{
							result=nodeItemStyleProvider->GetItemStyleId(node);
							nodeItemView->ReleaseNode(node);
						}
					}
					return result;
				}

				GuiListControl::IItemStyleController* NodeItemStyleProvider::CreateItemStyle(int styleId)
				{
					return nodeItemStyleProvider->CreateItemStyle(styleId);
				}

				void NodeItemStyleProvider::DestroyItemStyle(GuiListControl::IItemStyleController* style)
				{
					IItemStyleController* nodeStyle=dynamic_cast<IItemStyleController*>(style);
					if(nodeStyle)
					{
						nodeItemStyleProvider->DestroyItemStyle(nodeStyle);
					}
				}

				void NodeItemStyleProvider::Install(GuiListControl::IItemStyleController* style, int itemIndex)
				{
					if(nodeItemView)
					{
						IItemStyleController* nodeStyle=dynamic_cast<IItemStyleController*>(style);
						if(nodeStyle)
						{
							INodeProvider* node=nodeItemView->RequestNode(itemIndex);
							if(node)
							{
								nodeItemStyleProvider->Install(nodeStyle, node);
								nodeItemView->ReleaseNode(node);
							}
						}
					}
				}

/***********************************************************************
MemoryNodeProvider
***********************************************************************/

				INodeProviderCallback* MemoryNodeProvider::GetCallbackProxyInternal()
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

				void MemoryNodeProvider::OnChildTotalVisibleNodesChanged(int offset)
				{
					totalVisibleNodeCount+=offset;
					if(parent)
					{
						parent->OnChildTotalVisibleNodesChanged(offset);
					}
				}

				void MemoryNodeProvider::OnBeforeChildModified(int start, int count, int newCount)
				{
					offsetBeforeChildModified=0;
					if(expanding)
					{
						for(int i=0;i<count;i++)
						{
							offsetBeforeChildModified+=children[start+i]->totalVisibleNodeCount;
						}
					}
					INodeProviderCallback* proxy=GetCallbackProxyInternal();
					if(proxy)
					{
						proxy->OnBeforeItemModified(this, start, count, newCount);
					}
				}

				void MemoryNodeProvider::OnAfterChildModified(int start, int count, int newCount)
				{
					childCount+=(newCount-count);
					if(expanding)
					{
						int offset=0;
						for(int i=0;i<newCount;i++)
						{
							offset+=children[start+i]->totalVisibleNodeCount;
						}
						OnChildTotalVisibleNodesChanged(offset-offsetBeforeChildModified);
					}
					INodeProviderCallback* proxy=GetCallbackProxyInternal();
					if(proxy)
					{
						proxy->OnAfterItemModified(this, start, count, newCount);
					}
				}

				bool MemoryNodeProvider::OnRequestRemove(MemoryNodeProvider* child)
				{
					if(child->parent==this)
					{
						child->parent=0;
						return true;
					}
					return false;
				}

				bool MemoryNodeProvider::OnRequestInsert(MemoryNodeProvider* child)
				{
					if(child->parent==0)
					{
						child->parent=this;
						return true;
					}
					return false;
				}

				MemoryNodeProvider::MemoryNodeProvider()
					:parent(0)
					,expanding(false)
					,childCount(0)
					,totalVisibleNodeCount(1)
					,offsetBeforeChildModified(0)
				{
				}

				MemoryNodeProvider::MemoryNodeProvider(const Ptr<Object>& _data)
					:parent(0)
					,expanding(false)
					,childCount(0)
					,totalVisibleNodeCount(1)
					,offsetBeforeChildModified(0)
					,data(_data)
				{
				}

				MemoryNodeProvider::~MemoryNodeProvider()
				{
				}

				Ptr<Object> MemoryNodeProvider::GetData()
				{
					return data;
				}

				void MemoryNodeProvider::SetData(const Ptr<Object>& value)
				{
					data=value;
					NotifyDataModified();
				}

				void MemoryNodeProvider::NotifyDataModified()
				{
					if(parent)
					{
						int index=parent->children.IndexOf(this);
						INodeProviderCallback* proxy=GetCallbackProxyInternal();
						if(proxy)
						{
							proxy->OnBeforeItemModified(parent, index, 1, 1);
							proxy->OnAfterItemModified(parent, index, 1, 1);
						}
					}
				}

				MemoryNodeProvider::IChildList& MemoryNodeProvider::Children()
				{
					return *this;
				}

				bool MemoryNodeProvider::GetExpanding()
				{
					return expanding;
				}

				void MemoryNodeProvider::SetExpanding(bool value)
				{
					if(expanding!=value)
					{
						expanding=value;
						int offset=0;
						for(int i=0;i<childCount;i++)
						{
							offset+=children[i]->totalVisibleNodeCount;
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

				int MemoryNodeProvider::CalculateTotalVisibleNodes()
				{
					return totalVisibleNodeCount;
				}

				int MemoryNodeProvider::GetChildCount()
				{
					return childCount;
				}

				INodeProvider* MemoryNodeProvider::GetParent()
				{
					return parent;
				}

				INodeProvider* MemoryNodeProvider::RequestChild(int index)
				{
					if(0<=index && index<childCount)
					{
						return children[index].Obj();
					}
					else
					{
						return 0;
					}
				}

				void MemoryNodeProvider::ReleaseChild(INodeProvider* node)
				{
				}

				//---------------------------------------------------

				MemoryNodeProvider::ChildListEnumerator* MemoryNodeProvider::CreateEnumerator()const
				{
					return children.Wrap().CreateEnumerator();
				}

				bool MemoryNodeProvider::Contains(const KeyType<Ptr<MemoryNodeProvider>>::Type& item)const
				{
					return children.Contains(item);
				}

				vint MemoryNodeProvider::Count()const
				{
					return children.Count();
				}

				vint MemoryNodeProvider::Count()
				{
					return children.Count();
				}

				const Ptr<MemoryNodeProvider>& MemoryNodeProvider::Get(vint index)const
				{
					return children.Get(index);
				}

				const Ptr<MemoryNodeProvider>& MemoryNodeProvider::operator[](vint index)const
				{
					return children.Get(index);
				}

				vint MemoryNodeProvider::IndexOf(const KeyType<Ptr<MemoryNodeProvider>>::Type& item)const
				{
					return children.IndexOf(item);
				}

				vint MemoryNodeProvider::Add(const Ptr<MemoryNodeProvider>& item)
				{
					return Insert(children.Count(), item);
				}

				bool MemoryNodeProvider::Remove(const KeyType<Ptr<MemoryNodeProvider>>::Type& item)
				{
					vint index=children.IndexOf(item);
					if(index==-1) return false;
					return RemoveAt(index);
				}

				bool MemoryNodeProvider::RemoveAt(vint index)
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

				bool MemoryNodeProvider::RemoveRange(vint index, vint count)
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

				bool MemoryNodeProvider::Clear()
				{
					return RemoveRange(0, children.Count());
				}

				vint MemoryNodeProvider::Insert(vint index, const Ptr<MemoryNodeProvider>& item)
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

				bool MemoryNodeProvider::Set(vint index, const Ptr<MemoryNodeProvider>& item)
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

/***********************************************************************
MemoryNodeRootProvider
***********************************************************************/

				INodeProviderCallback* MemoryNodeRootProvider::GetCallbackProxyInternal()
				{
					return this;
				}

				void MemoryNodeRootProvider::OnAttached(INodeRootProvider* provider)
				{
				}

				void MemoryNodeRootProvider::OnBeforeItemModified(INodeProvider* parentNode, int start, int count, int newCount)
				{
					for(int i=0;i<callbacks.Count();i++)
					{
						callbacks[i]->OnBeforeItemModified(parentNode, start, count, newCount);
					}
				}

				void MemoryNodeRootProvider::OnAfterItemModified(INodeProvider* parentNode, int start, int count, int newCount)
				{
					for(int i=0;i<callbacks.Count();i++)
					{
						callbacks[i]->OnAfterItemModified(parentNode, start, count, newCount);
					}
				}

				void MemoryNodeRootProvider::OnItemExpanded(INodeProvider* node)
				{
					for(int i=0;i<callbacks.Count();i++)
					{
						callbacks[i]->OnItemExpanded(node);
					}
				}

				void MemoryNodeRootProvider::OnItemCollapsed(INodeProvider* node)
				{
					for(int i=0;i<callbacks.Count();i++)
					{
						callbacks[i]->OnItemCollapsed(node);
					}
				}

				MemoryNodeRootProvider::MemoryNodeRootProvider()
				{
					SetExpanding(true);
				}

				MemoryNodeRootProvider::~MemoryNodeRootProvider()
				{
				}

				INodeProvider* MemoryNodeRootProvider::GetRootNode()
				{
					return this;
				}

				bool MemoryNodeRootProvider::AttachCallback(INodeProviderCallback* value)
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

				bool MemoryNodeRootProvider::DetachCallback(INodeProviderCallback* value)
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

				Interface* MemoryNodeRootProvider::RequestView(const WString& identifier)
				{
					return 0;
				}

				void MemoryNodeRootProvider::ReleaseView(Interface* view)
				{
				}
			}

/***********************************************************************
TreeListControl
***********************************************************************/

			TreeListControl::TreeListControl(IStyleProvider* _styleProvider, tree::INodeRootProvider* _nodeRootProvider)
				:GuiSelectableListControl(_styleProvider, new tree::NodeItemProvider(_nodeRootProvider))
			{
				nodeItemProvider=dynamic_cast<tree::NodeItemProvider*>(GetItemProvider());
				nodeItemView=dynamic_cast<tree::INodeItemView*>(GetItemProvider()->RequestView(tree::INodeItemView::Identifier));
			}

			TreeListControl::~TreeListControl()
			{
				GetItemProvider()->ReleaseView(nodeItemView);
			}

			tree::INodeItemView* TreeListControl::GetNodeItemView()
			{
				return nodeItemView;
			}

			tree::INodeRootProvider* TreeListControl::GetNodeRootProvider()
			{
				return nodeItemProvider->GetRoot().Obj();
			}

			tree::INodeItemStyleProvider* TreeListControl::GetNodeStyleProvider()
			{
				return nodeStyleProvider.Obj();
			}

			Ptr<tree::INodeItemStyleProvider> TreeListControl::SetNodeStyleProvider(Ptr<tree::INodeItemStyleProvider> styleProvider)
			{
				Ptr<tree::INodeItemStyleProvider> old=nodeStyleProvider;
				nodeStyleProvider=styleProvider;
				GuiSelectableListControl::SetStyleProvider(new tree::NodeItemStyleProvider(nodeStyleProvider));
				return old;
			}
		}
	}
}