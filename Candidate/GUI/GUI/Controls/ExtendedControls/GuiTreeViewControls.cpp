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
					if(provider->GetExpending() && offset>0)
					{
						offset-=1;
						int count=provider->Count();
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

				void NodeItemProvider::OnAttached(INodeProvider* provider)
				{
				}

				void NodeItemProvider::OnItemModified(INodeProvider* parentNode, int start, int count, int newCount)
				{
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
			}
		}
	}
}