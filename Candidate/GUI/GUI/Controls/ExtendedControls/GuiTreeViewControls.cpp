#include "GuiTreeViewControls.h"

namespace vl
{
	namespace presentation
	{
		namespace controls
		{
			using namespace elements;

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
						InvokeOnItemModified(base+1, 0, visibility-1);
					}
				}

				void NodeItemProvider::OnItemCollapsed(INodeProvider* node)
				{
					int base=CalculateNodeVisibilityIndexInternal(node);
					if(base!=-2)
					{
						int visibility=0;
						int count=node->GetChildCount();
						for(int i=0;i<count;i++)
						{
							INodeProvider* child=node->RequestChild(i);
							visibility+=child->CalculateTotalVisibleNodes();
							node->ReleaseChild(child);
						}
						InvokeOnItemModified(base+1, visibility, 0);
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
					if(root->CanGetNodeByVisibleIndex())
					{
						return root->GetNodeByVisibleIndex(index+1);
					}
					else
					{
						return GetNodeByOffset(root->GetRootNode(), index+1);
					}
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
					INodeItemStyleController* nodeStyle=dynamic_cast<INodeItemStyleController*>(style);
					if(nodeStyle)
					{
						nodeItemStyleProvider->DestroyItemStyle(nodeStyle);
					}
				}

				void NodeItemStyleProvider::Install(GuiListControl::IItemStyleController* style, int itemIndex)
				{
					if(nodeItemView)
					{
						INodeItemStyleController* nodeStyle=dynamic_cast<INodeItemStyleController*>(style);
						if(nodeStyle)
						{
							INodeProvider* node=nodeItemView->RequestNode(itemIndex);
							if(node)
							{
								nodeItemStyleProvider->Install(nodeStyle, node);
							}
						}
					}
				}

				void NodeItemStyleProvider::SetStyleSelected(GuiListControl::IItemStyleController* style, bool value)
				{
					if(nodeItemView)
					{
						INodeItemStyleController* nodeStyle=dynamic_cast<INodeItemStyleController*>(style);
						if(nodeStyle)
						{
							nodeItemStyleProvider->SetStyleSelected(nodeStyle, value);
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

						OnChildTotalVisibleNodesChanged(expanding?offset:-offset);
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
NodeRootProviderBase
***********************************************************************/

				void NodeRootProviderBase::OnAttached(INodeRootProvider* provider)
				{
				}

				void NodeRootProviderBase::OnBeforeItemModified(INodeProvider* parentNode, int start, int count, int newCount)
				{
					for(int i=0;i<callbacks.Count();i++)
					{
						callbacks[i]->OnBeforeItemModified(parentNode, start, count, newCount);
					}
				}

				void NodeRootProviderBase::OnAfterItemModified(INodeProvider* parentNode, int start, int count, int newCount)
				{
					for(int i=0;i<callbacks.Count();i++)
					{
						callbacks[i]->OnAfterItemModified(parentNode, start, count, newCount);
					}
				}

				void NodeRootProviderBase::OnItemExpanded(INodeProvider* node)
				{
					for(int i=0;i<callbacks.Count();i++)
					{
						callbacks[i]->OnItemExpanded(node);
					}
				}

				void NodeRootProviderBase::OnItemCollapsed(INodeProvider* node)
				{
					for(int i=0;i<callbacks.Count();i++)
					{
						callbacks[i]->OnItemCollapsed(node);
					}
				}

				NodeRootProviderBase::NodeRootProviderBase()
				{
				}

				NodeRootProviderBase::~NodeRootProviderBase()
				{
				}

				bool NodeRootProviderBase::CanGetNodeByVisibleIndex()
				{
					return false;
				}

				INodeProvider* NodeRootProviderBase::GetNodeByVisibleIndex(int index)
				{
					return 0;
				}

				bool NodeRootProviderBase::AttachCallback(INodeProviderCallback* value)
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

				bool NodeRootProviderBase::DetachCallback(INodeProviderCallback* value)
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

				Interface* NodeRootProviderBase::RequestView(const WString& identifier)
				{
					return 0;
				}

				void NodeRootProviderBase::ReleaseView(Interface* view)
				{
				}

/***********************************************************************
MemoryNodeRootProvider
***********************************************************************/

				INodeProviderCallback* MemoryNodeRootProvider::GetCallbackProxyInternal()
				{
					return this;
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
			}

/***********************************************************************
GuiVirtualTreeListControl
***********************************************************************/

			GuiVirtualTreeListControl::GuiVirtualTreeListControl(IStyleProvider* _styleProvider, tree::INodeRootProvider* _nodeRootProvider)
				:GuiSelectableListControl(_styleProvider, new tree::NodeItemProvider(_nodeRootProvider))
			{
				nodeItemProvider=dynamic_cast<tree::NodeItemProvider*>(GetItemProvider());
				nodeItemView=dynamic_cast<tree::INodeItemView*>(GetItemProvider()->RequestView(tree::INodeItemView::Identifier));
			}

			GuiVirtualTreeListControl::~GuiVirtualTreeListControl()
			{
				GetItemProvider()->ReleaseView(nodeItemView);
			}

			tree::INodeItemView* GuiVirtualTreeListControl::GetNodeItemView()
			{
				return nodeItemView;
			}

			tree::INodeRootProvider* GuiVirtualTreeListControl::GetNodeRootProvider()
			{
				return nodeItemProvider->GetRoot().Obj();
			}

			tree::INodeItemStyleProvider* GuiVirtualTreeListControl::GetNodeStyleProvider()
			{
				return nodeStyleProvider.Obj();
			}

			Ptr<tree::INodeItemStyleProvider> GuiVirtualTreeListControl::SetNodeStyleProvider(Ptr<tree::INodeItemStyleProvider> styleProvider)
			{
				Ptr<tree::INodeItemStyleProvider> old=nodeStyleProvider;
				nodeStyleProvider=styleProvider;
				GuiSelectableListControl::SetStyleProvider(new tree::NodeItemStyleProvider(nodeStyleProvider));
				return old;
			}

			namespace tree
			{

/***********************************************************************
TreeViewItem
***********************************************************************/

				const wchar_t* ITreeViewItemView::Identifier = L"vl::presentation::controls::tree::ITreeViewItemView";

				TreeViewItem::TreeViewItem()
				{
				}

				TreeViewItem::TreeViewItem(const Ptr<GuiImageData>& _image, const WString& _text)
					:image(_image)
					,text(_text)
				{
				}

/***********************************************************************
TreeViewItemRootProvider
***********************************************************************/

				Ptr<GuiImageData> TreeViewItemRootProvider::GetNodeImage(INodeProvider* node)
				{
					MemoryNodeProvider* memoryNode=dynamic_cast<MemoryNodeProvider*>(node);
					if(memoryNode)
					{
						Ptr<TreeViewItem> data=memoryNode->GetData().Cast<TreeViewItem>();
						if(data)
						{
							return data->image;
						}
					}
					return 0;
				}

				WString	TreeViewItemRootProvider::GetNodeText(INodeProvider* node)
				{
					MemoryNodeProvider* memoryNode=dynamic_cast<MemoryNodeProvider*>(node);
					if(memoryNode)
					{
						Ptr<TreeViewItem> data=memoryNode->GetData().Cast<TreeViewItem>();
						if(data)
						{
							return data->text;
						}
					}
					return L"";
				}

				TreeViewItemRootProvider::TreeViewItemRootProvider()
				{
				}

				TreeViewItemRootProvider::~TreeViewItemRootProvider()
				{
				}

				Interface* TreeViewItemRootProvider::RequestView(const WString& identifier)
				{
					if(identifier==ITreeViewItemView::Identifier)
					{
						return (ITreeViewItemView*)this;
					}
					else
					{
						return MemoryNodeRootProvider::RequestView(identifier);
					}
				}

				void TreeViewItemRootProvider::ReleaseView(Interface* view)
				{
					return MemoryNodeRootProvider::ReleaseView(view);
				}

/***********************************************************************
TreeViewNodeItemStyleProvider::ItemController
***********************************************************************/

				void TreeViewNodeItemStyleProvider::ItemController::SwitchNodeExpanding()
				{
					if(backgroundButton->GetBoundsComposition()->GetParent())
					{
						GuiListControl::IItemArranger* arranger=styleProvider->treeControl->GetArranger();
						int index=arranger->GetVisibleIndex(this);
						if(index!=-1)
						{
							INodeItemView* view=styleProvider->treeControl->GetNodeItemView();
							INodeProvider* node=view->RequestNode(index);
							if(node)
							{
								bool expanding=node->GetExpanding();
								node->SetExpanding(!expanding);
								//UpdateExpandingButton(node);
								view->ReleaseNode(node);
							}
						}
					}
				}

				void TreeViewNodeItemStyleProvider::ItemController::OnBackgroundButtonDoubleClick(elements::GuiGraphicsComposition* sender, elements::GuiMouseEventArgs& arguments)
				{
					SwitchNodeExpanding();
				}

				void TreeViewNodeItemStyleProvider::ItemController::OnExpandingButtonDoubleClick(elements::GuiGraphicsComposition* sender, elements::GuiMouseEventArgs& arguments)
				{
					arguments.handled=true;
				}

				void TreeViewNodeItemStyleProvider::ItemController::OnExpandingButtonClicked(elements::GuiGraphicsComposition* sender, elements::GuiEventArgs& arguments)
				{
					SwitchNodeExpanding();
				}

				TreeViewNodeItemStyleProvider::ItemController::ItemController(TreeViewNodeItemStyleProvider* _styleProvider)
					:list::ItemStyleControllerBase(_styleProvider->GetBindedItemStyleProvider(), 0)
					,styleProvider(_styleProvider)
				{
					backgroundButton=new GuiSelectableButton(styleProvider->treeControl->GetTreeViewStyleProvider()->CreateItemBackground());
					backgroundButton->SetAutoSelection(false);
					backgroundButton->GetBoundsComposition()->SetMinSizeLimitation(GuiGraphicsComposition::LimitToElementAndChildren);
					backgroundButton->GetEventReceiver()->leftButtonDoubleClick.AttachMethod(this, &ItemController::OnBackgroundButtonDoubleClick);

					table=new GuiTableComposition;
					backgroundButton->GetBoundsComposition()->AddChild(table);
					table->SetRowsAndColumns(3, 4);
					table->SetRowOption(0, GuiCellOption::PercentageOption(0.5));
					table->SetRowOption(1, GuiCellOption::MinSizeOption());
					table->SetRowOption(2, GuiCellOption::PercentageOption(0.5));
					table->SetColumnOption(0, GuiCellOption::AbsoluteOption(0));
					table->SetColumnOption(1, GuiCellOption::MinSizeOption());
					table->SetColumnOption(2, GuiCellOption::MinSizeOption());
					table->SetColumnOption(3, GuiCellOption::MinSizeOption());
					table->SetAlignmentToParent(Margin(0, 0, 0, 0));
					table->SetCellPadding(2);
					{
						GuiCellComposition* cell=new GuiCellComposition;
						table->AddChild(cell);
						cell->SetSite(0, 1, 3, 1);
						cell->SetPreferredMinSize(Size(16, 16));

						expandingButton=new GuiSelectableButton(styleProvider->treeControl->GetTreeViewStyleProvider()->CreateItemExpandingDecorator());
						expandingButton->SetAutoSelection(false);
						expandingButton->GetBoundsComposition()->SetAlignmentToParent(Margin(0, 0, 0, 0));
						expandingButton->GetEventReceiver()->leftButtonDoubleClick.AttachMethod(this, &ItemController::OnExpandingButtonDoubleClick);
						expandingButton->Clicked.AttachMethod(this, &ItemController::OnExpandingButtonClicked);
						cell->AddChild(expandingButton->GetBoundsComposition());
					}
					{
						GuiCellComposition* cell=new GuiCellComposition;
						table->AddChild(cell);
						cell->SetSite(1, 2, 1, 1);
						cell->SetPreferredMinSize(Size(16, 16));

						image=GuiImageFrameElement::Create();
						image->SetStretch(true);
						cell->SetOwnedElement(image);
					}
					{
						GuiCellComposition* cell=new GuiCellComposition;
						table->AddChild(cell);
						cell->SetSite(0, 3, 3, 1);
						cell->SetPreferredMinSize(Size(192, 0));

						text=GuiSolidLabelElement::Create();
						text->SetAlignments(Alignment::Left, Alignment::Center);
						text->SetFont(styleProvider->treeControl->GetFont());
						text->SetEllipse(true);
						cell->SetOwnedElement(text);
					}
					Initialize(backgroundButton->GetBoundsComposition(), backgroundButton);
				}

				INodeItemStyleProvider* TreeViewNodeItemStyleProvider::ItemController::GetNodeStyleProvider()
				{
					return styleProvider;
				}

				void TreeViewNodeItemStyleProvider::ItemController::Install(INodeProvider* node)
				{
					ITreeViewItemView* view=styleProvider->treeViewItemView;
					Ptr<GuiImageData> imageData=view->GetNodeImage(node);
					if(imageData)
					{
						image->SetImage(imageData->GetImage(), imageData->GetFrameIndex());
					}
					else
					{
						image->SetImage(0);
					}
					text->SetText(view->GetNodeText(node));
					text->SetColor(styleProvider->treeControl->GetTreeViewStyleProvider()->GetTextColor());
					UpdateExpandingButton(node);

					int level=-2;
					while(node)
					{
						node=node->GetParent();
						level++;
					}
					table->SetColumnOption(0, GuiCellOption::AbsoluteOption(level*16));
				}

				bool TreeViewNodeItemStyleProvider::ItemController::GetSelected()
				{
					return backgroundButton->GetSelected();
				}

				void TreeViewNodeItemStyleProvider::ItemController::SetSelected(bool value)
				{
					backgroundButton->SetSelected(value);
				}

				void TreeViewNodeItemStyleProvider::ItemController::UpdateExpandingButton(INodeProvider* associatedNode)
				{
					expandingButton->SetSelected(associatedNode->GetExpanding());
					expandingButton->SetVisible(associatedNode->GetChildCount()>0);
				}

/***********************************************************************
TreeViewNodeItemStyleProvider
***********************************************************************/

				TreeViewNodeItemStyleProvider::TreeViewNodeItemStyleProvider()
					:treeControl(0)
					,bindedItemStyleProvider(0)
					,treeViewItemView(0)
				{
				}

				TreeViewNodeItemStyleProvider::~TreeViewNodeItemStyleProvider()
				{
				}

				TreeViewNodeItemStyleProvider::ItemController* TreeViewNodeItemStyleProvider::GetRelatedController(INodeProvider* node)
				{
					int index=treeControl->GetNodeItemView()->CalculateNodeVisibilityIndex(node);
					if(index!=-1)
					{
						GuiListControl::IItemStyleController* style=treeControl->GetArranger()->GetVisibleStyle(index);
						if(style)
						{
							ItemController* itemController=dynamic_cast<ItemController*>(style);
							return itemController;
						}
					}
					return 0;
				}

				void TreeViewNodeItemStyleProvider::UpdateExpandingButton(INodeProvider* node)
				{
					ItemController* itemController=GetRelatedController(node);
					if(itemController)
					{
						itemController->UpdateExpandingButton(node);
					}
				}

				void TreeViewNodeItemStyleProvider::OnAttached(INodeRootProvider* provider)
				{
				}

				void TreeViewNodeItemStyleProvider::OnBeforeItemModified(INodeProvider* parentNode, int start, int count, int newCount)
				{
				}

				void TreeViewNodeItemStyleProvider::OnAfterItemModified(INodeProvider* parentNode, int start, int count, int newCount)
				{
					UpdateExpandingButton(parentNode);
				}

				void TreeViewNodeItemStyleProvider::OnItemExpanded(INodeProvider* node)
				{
					UpdateExpandingButton(node);
				}

				void TreeViewNodeItemStyleProvider::OnItemCollapsed(INodeProvider* node)
				{
					UpdateExpandingButton(node);
				}

				void TreeViewNodeItemStyleProvider::BindItemStyleProvider(GuiListControl::IItemStyleProvider* styleProvider)
				{
					bindedItemStyleProvider=styleProvider;
				}

				GuiListControl::IItemStyleProvider* TreeViewNodeItemStyleProvider::GetBindedItemStyleProvider()
				{
					return bindedItemStyleProvider;
				}

				void TreeViewNodeItemStyleProvider::AttachListControl(GuiListControl* value)
				{
					treeControl=dynamic_cast<GuiTreeView*>(value);
					if(treeControl)
					{
						treeViewItemView=dynamic_cast<ITreeViewItemView*>(treeControl->GetItemProvider()->RequestView(ITreeViewItemView::Identifier));
						treeControl->GetNodeRootProvider()->AttachCallback(this);
					}
				}

				void TreeViewNodeItemStyleProvider::DetachListControl()
				{
					if(treeViewItemView)
					{
						treeControl->GetItemProvider()->ReleaseView(treeViewItemView);
						treeViewItemView=0;
					}
					if(treeControl)
					{
						treeControl->GetNodeRootProvider()->DetachCallback(this);
						treeControl=0;
					}
				}

				int TreeViewNodeItemStyleProvider::GetItemStyleId(INodeProvider* node)
				{
					return 0;
				}

				INodeItemStyleController* TreeViewNodeItemStyleProvider::CreateItemStyle(int styleId)
				{
					return new ItemController(this);
				}

				void TreeViewNodeItemStyleProvider::DestroyItemStyle(INodeItemStyleController* style)
				{
					ItemController* itemController=dynamic_cast<ItemController*>(style);
					if(itemController)
					{
						delete itemController;
					}
				}

				void TreeViewNodeItemStyleProvider::Install(INodeItemStyleController* style, INodeProvider* node)
				{
					ItemController* itemController=dynamic_cast<ItemController*>(style);
					if(itemController)
					{
						itemController->Install(node);
					}
				}

				void TreeViewNodeItemStyleProvider::SetStyleSelected(INodeItemStyleController* style, bool value)
				{
					ItemController* itemController=dynamic_cast<ItemController*>(style);
					if(itemController)
					{
						itemController->SetSelected(value);
					}
				}
			}

/***********************************************************************
GuiTreeView
***********************************************************************/

			GuiTreeView::GuiTreeView(IStyleProvider* _styleProvider, tree::INodeRootProvider* _nodeRootProvider)
				:GuiVirtualTreeListControl(_styleProvider, (_nodeRootProvider?_nodeRootProvider:new tree::TreeViewItemRootProvider))
			{
				styleProvider=dynamic_cast<IStyleProvider*>(styleController->GetStyleProvider());
				nodes=nodeItemProvider->GetRoot().Cast<tree::TreeViewItemRootProvider>();
				SetNodeStyleProvider(new tree::TreeViewNodeItemStyleProvider);
				SetArranger(new list::FixedHeightItemArranger);
			}

			GuiTreeView::~GuiTreeView()
			{
			}

			Ptr<tree::TreeViewItemRootProvider> GuiTreeView::Nodes()
			{
				return nodes;
			}

			GuiTreeView::IStyleProvider* GuiTreeView::GetTreeViewStyleProvider()
			{
				return styleProvider;
			}
		}
	}
}