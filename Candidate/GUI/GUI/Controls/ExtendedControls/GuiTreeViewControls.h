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
GuiVirtualTreeListControl NodeProvider
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

				class INodeRootProvider : public virtual Interface
				{
				public:
					virtual INodeProvider*			GetRootNode()=0;
					virtual bool					AttachCallback(INodeProviderCallback* value)=0;
					virtual bool					DetachCallback(INodeProviderCallback* value)=0;
					virtual Interface*				RequestView(const WString& identifier)=0;
					virtual void					ReleaseView(Interface* view)=0;
				};
			}

			namespace tree
			{
				//-----------------------------------------------------------
				// Tree to ListControl (IItemProvider)
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

				//-----------------------------------------------------------
				// Tree to ListControl (IItemStyleProvider)
				//-----------------------------------------------------------

				class INodeItemStyleProvider;

				class INodeItemStyleController : public virtual GuiListControl::IItemStyleController
				{
				public:
					virtual INodeItemStyleProvider*					GetNodeStyleProvider()=0;
				};

				class INodeItemStyleProvider : public virtual Interface
				{
				public:
					virtual void									BindItemStyleProvider(GuiListControl::IItemStyleProvider* styleProvider)=0;
					virtual GuiListControl::IItemStyleProvider*		GetBindedItemStyleProvider()=0;
					virtual void									AttachListControl(GuiListControl* value)=0;
					virtual void									DetachListControl()=0;
					virtual int										GetItemStyleId(INodeProvider* node)=0;
					virtual INodeItemStyleController*				CreateItemStyle(int styleId)=0;
					virtual void									DestroyItemStyle(INodeItemStyleController* style)=0;
					virtual void									Install(INodeItemStyleController* style, INodeProvider* node)=0;
				};
				
#pragma warning(push)
#pragma warning(disable:4250)
				class NodeItemStyleControllerBase : public list::ItemStyleControllerBase, public virtual INodeItemStyleController
				{
				protected:
					INodeItemStyleProvider*							provider;

					NodeItemStyleControllerBase(INodeItemStyleProvider* _provider, int _styleId);
				public:
				};
#pragma warning(pop)

				class NodeItemStyleProvider : public Object, public virtual GuiListControl::IItemStyleProvider
				{
				protected:
					Ptr<INodeItemStyleProvider>						nodeItemStyleProvider;
					GuiListControl*									listControl;
					INodeItemView*									nodeItemView;
				public:
					NodeItemStyleProvider(Ptr<INodeItemStyleProvider> provider);
					~NodeItemStyleProvider();

					void											AttachListControl(GuiListControl* value)override;
					void											DetachListControl()override;
					int												GetItemStyleId(int itemIndex)override;
					GuiListControl::IItemStyleController*			CreateItemStyle(int styleId)override;
					void											DestroyItemStyle(GuiListControl::IItemStyleController* style)override;
					void											Install(GuiListControl::IItemStyleController* style, int itemIndex)override;
				};
			}

/***********************************************************************
GuiVirtualTreeListControl Predefined NodeProvider
***********************************************************************/

			namespace tree
			{
				class MemoryNodeProvider
					: public Object
					, public virtual INodeProvider
					, private collections::IList<Ptr<MemoryNodeProvider>>
				{
					typedef collections::List<Ptr<MemoryNodeProvider>> ChildList;
					typedef collections::IList<Ptr<MemoryNodeProvider>> IChildList;
					typedef collections::IEnumerator<Ptr<MemoryNodeProvider>> ChildListEnumerator;
				protected:
					MemoryNodeProvider*			parent;
					bool							expanding;
					int								childCount;
					int								totalVisibleNodeCount;
					int								offsetBeforeChildModified;
					Ptr<Object>						data;
					ChildList						children;

					virtual INodeProviderCallback*	GetCallbackProxyInternal();
					void							OnChildTotalVisibleNodesChanged(int offset);
					void							OnBeforeChildModified(int start, int count, int newCount);
					void							OnAfterChildModified(int start, int count, int newCount);
					bool							OnRequestRemove(MemoryNodeProvider* child);
					bool							OnRequestInsert(MemoryNodeProvider* child);
				private:
					
					ChildListEnumerator*			CreateEnumerator()const;
					bool							Contains(const KeyType<Ptr<MemoryNodeProvider>>::Type& item)const;
					vint							Count()const;
					vint							Count();
					const							Ptr<MemoryNodeProvider>& Get(vint index)const;
					const							Ptr<MemoryNodeProvider>& operator[](vint index)const;
					vint							IndexOf(const KeyType<Ptr<MemoryNodeProvider>>::Type& item)const;
					vint							Add(const Ptr<MemoryNodeProvider>& item);
					bool							Remove(const KeyType<Ptr<MemoryNodeProvider>>::Type& item);
					bool							RemoveAt(vint index);
					bool							RemoveRange(vint index, vint count);
					bool							Clear();
					vint							Insert(vint index, const Ptr<MemoryNodeProvider>& item);
					bool							Set(vint index, const Ptr<MemoryNodeProvider>& item);
				public:
					MemoryNodeProvider();
					MemoryNodeProvider(const Ptr<Object>& _data);
					~MemoryNodeProvider();

					Ptr<Object>						GetData();
					void							SetData(const Ptr<Object>& value);
					void							NotifyDataModified();
					IChildList&						Children();

					bool							GetExpanding()override;
					void							SetExpanding(bool value)override;
					int								CalculateTotalVisibleNodes()override;

					int								GetChildCount()override;
					INodeProvider*					GetParent()override;
					INodeProvider*					RequestChild(int index)override;
					void							ReleaseChild(INodeProvider* node)override;
				};

				class MemoryNodeRootProvider
					: public MemoryNodeProvider
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
					MemoryNodeRootProvider();
					~MemoryNodeRootProvider();

					INodeProvider*					GetRootNode()override;
					bool							AttachCallback(INodeProviderCallback* value)override;
					bool							DetachCallback(INodeProviderCallback* value)override;
					Interface*						RequestView(const WString& identifier)override;
					void							ReleaseView(Interface* view)override;
				};
			}

/***********************************************************************
GuiVirtualTreeListControl
***********************************************************************/

			class GuiVirtualTreeListControl : public GuiSelectableListControl
			{
			protected:
				tree::NodeItemProvider*				nodeItemProvider;
				tree::INodeItemView*				nodeItemView;
				Ptr<tree::INodeItemStyleProvider>	nodeStyleProvider;
			public:
				GuiVirtualTreeListControl(IStyleProvider* _styleProvider, tree::INodeRootProvider* _nodeRootProvider);
				~GuiVirtualTreeListControl();

				tree::INodeItemView*				GetNodeItemView();
				tree::INodeRootProvider*			GetNodeRootProvider();
				tree::INodeItemStyleProvider*		GetNodeStyleProvider();
				Ptr<tree::INodeItemStyleProvider>	SetNodeStyleProvider(Ptr<tree::INodeItemStyleProvider> styleProvider);
			};

/***********************************************************************
TreeView
***********************************************************************/

			namespace tree
			{
				class ITreeViewItemView : public virtual Interface
				{
				public:
					static const wchar_t*			Identifier;

					virtual Ptr<GuiImageData>		GetNodeImage(INodeProvider* node)=0;
					virtual WString					GetNodeText(INodeProvider* node)=0;
				};

				class TreeViewItem : public Object
				{
				public:
					Ptr<GuiImageData>				image;
					WString							text;
				};

				class TreeViewItemRootProvider
					: public MemoryNodeRootProvider
					, protected virtual ITreeViewItemView
				{
				protected:

					Ptr<GuiImageData>				GetNodeImage(INodeProvider* node)override;
					WString							GetNodeText(INodeProvider* node)override;
				public:
					TreeViewItemRootProvider();
					~TreeViewItemRootProvider();

					Interface*						RequestView(const WString& identifier)override;
					void							ReleaseView(Interface* view)override;
				};

				class TreeViewNodeItemStyleProvider : public Object, public virtual INodeItemStyleProvider
				{
				protected:
					GuiListControl*							listControl;
					GuiListControl::IItemStyleProvider*		bindedItemStyleProvider;
					ITreeViewItemView*						treeViewItemView;

				public:
					TreeViewNodeItemStyleProvider();
					~TreeViewNodeItemStyleProvider();

					void									BindItemStyleProvider(GuiListControl::IItemStyleProvider* styleProvider)override;
					GuiListControl::IItemStyleProvider*		GetBindedItemStyleProvider()override;
					void									AttachListControl(GuiListControl* value)override;
					void									DetachListControl()override;
					int										GetItemStyleId(INodeProvider* node)override;
					INodeItemStyleController*				CreateItemStyle(int styleId)override;
					void									DestroyItemStyle(INodeItemStyleController* style)override;
					void									Install(INodeItemStyleController* style, INodeProvider* node)override;
				};
			}

			class GuiTreeView : public GuiVirtualTreeListControl
			{
			protected:
				Ptr<tree::TreeViewItemRootProvider>	nodes;
			public:
				GuiTreeView(IStyleProvider* _styleProvider);
				~GuiTreeView();

				Ptr<tree::TreeViewItemRootProvider>			Nodes();
			};
		}
	}
}

#endif