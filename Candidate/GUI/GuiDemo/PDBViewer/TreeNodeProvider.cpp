#include "..\..\GUI\GacUI.h"

using namespace vl;
using namespace vl::stream;
using namespace vl::entities;

class TreeNodeProvider : public Object, public virtual tree::INodeProvider
{
protected:
	INodeProvider*								parent;
	bool										expanding;
	int											totalVisibleNodes;
	Ptr<TreeElement>							treeNode;
	int											treeNodeIndex;
	tree::INodeProviderCallback*				callbackProxy;

	collections::Array<Ptr<TreeNodeProvider>>	children;
	bool										childrenFilled;
	collections::SortedList<int>				expandedChildrenIndices;

	void EnsureChildren()
	{
		if(!childrenFilled)
		{
			childrenFilled=true;
			children.Resize(treeNode->children.Count());
		}
	}

	void OnChildTotalVisibleNodesChanged(int offset)
	{
		totalVisibleNodes+=offset;
		if(TreeNodeProvider* provider=dynamic_cast<TreeNodeProvider*>(parent))
		{
			provider->OnChildTotalVisibleNodesChanged(offset);
		}
	}

	TreeNodeProvider* RequestChildInternal(int index)
	{
		EnsureChildren();
		if(0<=index && index<children.Count())
		{
			if(!children[index])
			{
				children[index]=new TreeNodeProvider(this, treeNode->children[index].Cast<TreeElement>(), index, callbackProxy);
			}
			return children[index].Obj();
		}
		else
		{
			return 0;
		}
	}
public:
	TreeNodeProvider(INodeProvider* _parent, Ptr<TreeElement> _treeNode, int _treeNodeIndex, tree::INodeProviderCallback* _callbackProxy)
		:parent(_parent)
		,expanding(false)
		,totalVisibleNodes(1)
		,treeNode(_treeNode)
		,treeNodeIndex(_treeNodeIndex)
		,callbackProxy(_callbackProxy)
		,childrenFilled(false)
	{
	}

	~TreeNodeProvider()
	{
	}

	bool GetExpanding()override
	{
		return expanding;
	}

	void SetExpanding(bool value)override
	{
		if(expanding!=value)
		{
			expanding=value;
			int offset=0;
			EnsureChildren();
			for(int i=0;i<children.Count();i++)
			{
				offset+=children[i]?children[i]->totalVisibleNodes:1;
			}

			OnChildTotalVisibleNodesChanged(expanding?offset:-offset);

			TreeNodeProvider* provider=dynamic_cast<TreeNodeProvider*>(parent);
			if(provider)
			{
				if(expanding)
				{
					provider->expandedChildrenIndices.Add(treeNodeIndex);
				}
				else
				{
					provider->expandedChildrenIndices.Remove(treeNodeIndex);
				}
			}

			if(expanding)
			{
				callbackProxy->OnItemExpanded(this);
			}
			else
			{
				callbackProxy->OnItemCollapsed(this);
			}
		}
	}

	int CalculateTotalVisibleNodes()override
	{
		return totalVisibleNodes;
	}

	int GetChildCount()override
	{
		EnsureChildren();
		return children.Count();
	}

	INodeProvider* GetParent()override
	{
		return parent;
	}

	INodeProvider* RequestChild(int index)override
	{
		return RequestChildInternal(index);
	}

	void ReleaseChild(INodeProvider* node)override
	{
	}

	WString GetName()
	{
		WString name=treeNode->name;
		for(int i=0;i<treeNode->attributes.Count();i++)
		{
			WString key=treeNode->attributes.Keys()[i];
			Ptr<TreeText> value=treeNode->attributes.Values()[i].Cast<TreeText>();
			name+=L" ["+key+L": "+value->value+L"]";
		}
		return name;
	}

	TreeNodeProvider* GetNodeByVisibleIndex(int index)
	{
		if(index<=0)
		{
			return this;
		}
		else
		{
			EnsureChildren();
			index-=1;
			int totalExpandedCount=0;
			for(int i=0;i<expandedChildrenIndices.Count();i++)
			{
				int expandedIndex=expandedChildrenIndices[i];
				int expandedCount=children[expandedIndex]->totalVisibleNodes;
				int startIndex=expandedIndex+totalExpandedCount;
				if(index<startIndex)
				{
					return RequestChildInternal(index-totalExpandedCount);
				}
				else if(index<startIndex+expandedCount)
				{
					return children[expandedIndex]->GetNodeByVisibleIndex(index-startIndex);
				}
				totalExpandedCount+=expandedCount-1;
			}
			return RequestChildInternal(index-totalExpandedCount);
		}
	}
};

class TreeNodeRootProvider
	: public Object
	, public tree::NodeRootProviderBase
	, public tree::INodeProvider
	, protected virtual tree::ITreeViewItemView
{
protected:
	Ptr<TreeNodeProvider>			rootProvider;

	Ptr<GuiImageData> GetNodeImage(INodeProvider* node)
	{
		return 0;
	}

	WString GetNodeText(INodeProvider* node)
	{
		TreeNodeProvider* provider=dynamic_cast<TreeNodeProvider*>(node);
		return provider?provider->GetName():L"";
	}
public:
	TreeNodeRootProvider(Ptr<TreeElement> rootNode)
	{
		rootProvider=new TreeNodeProvider(this, rootNode, -1, this);
	}

	~TreeNodeRootProvider()
	{
	}

	tree::INodeProvider* GetRootNode()
	{
		return this;
	}

	bool CanGetNodeByVisibleIndex()
	{
		return true;
	}

	tree::INodeProvider* GetNodeByVisibleIndex(int index)
	{
		return rootProvider->GetNodeByVisibleIndex(index-1);
	};

	bool GetExpanding()override
	{
		return true;
	}

	void SetExpanding(bool value)override
	{
	}

	int CalculateTotalVisibleNodes()override
	{
		return 1+rootProvider->CalculateTotalVisibleNodes();
	}

	int GetChildCount()override
	{
		return 1;
	}

	INodeProvider* GetParent()override
	{
		return 0;
	}

	INodeProvider* RequestChild(int index)override
	{
		return index==0?rootProvider.Obj():0;
	}

	void ReleaseChild(INodeProvider* node)override
	{
	}

	Interface* RequestView(const WString& identifier)override
	{
		if(identifier==tree::ITreeViewItemView::Identifier)
		{
			return (tree::ITreeViewItemView*)this;
		}
		else
		{
			return NodeRootProviderBase::RequestView(identifier);
		}
	}

	void ReleaseView(Interface* view)override
	{
		tree::NodeRootProviderBase::ReleaseView(view);
	}
};

tree::INodeRootProvider* CreateProviderFromXml(Ptr<TreeElement> node)
{
	return new TreeNodeRootProvider(node);
}