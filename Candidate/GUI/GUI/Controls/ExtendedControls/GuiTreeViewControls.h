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
TreeListControl NodeProvider
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

				class IItemStyleController : public virtual GuiListControl::IItemStyleController
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
					virtual IItemStyleController*					CreateItemStyle(int styleId)=0;
					virtual void									DestroyItemStyle(IItemStyleController* style)=0;
					virtual void									Install(IItemStyleController* style, INodeProvider* node)=0;
				};
				
#pragma warning(push)
#pragma warning(disable:4250)
				class NodeItemStyleControllerBase : public list::ItemStyleControllerBase, public virtual IItemStyleController
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
TreeListControl Predefined NodeProvider
***********************************************************************/

			namespace tree
			{
				class MemoryNodeProviderBase
					: public Object
					, public virtual INodeProvider
					, private collections::IList<Ptr<MemoryNodeProviderBase>>
				{
					typedef collections::List<Ptr<MemoryNodeProviderBase>> ChildList;
					typedef collections::IList<Ptr<MemoryNodeProviderBase>> IChildList;
					typedef collections::IEnumerator<Ptr<MemoryNodeProviderBase>> ChildListEnumerator;
				protected:
					MemoryNodeProviderBase*			parent;
					bool							expanding;
					int								childCount;
					int								totalVisibleNodeCount;
					int								offsetBeforeChildModified;
					ChildList						children;

					virtual INodeProviderCallback*	GetCallbackProxyInternal();
					void							OnChildTotalVisibleNodesChanged(int offset);
					void							OnBeforeChildModified(int start, int count, int newCount);
					void							OnAfterChildModified(int start, int count, int newCount);
					bool							OnRequestRemove(MemoryNodeProviderBase* child);
					bool							OnRequestInsert(MemoryNodeProviderBase* child);
					void							OnSelfDataModified();
				private:
					
					ChildListEnumerator*			CreateEnumerator()const;
					bool							Contains(const KeyType<Ptr<MemoryNodeProviderBase>>::Type& item)const;
					vint							Count()const;
					vint							Count();
					const							Ptr<MemoryNodeProviderBase>& Get(vint index)const;
					const							Ptr<MemoryNodeProviderBase>& operator[](vint index)const;
					vint							IndexOf(const KeyType<Ptr<MemoryNodeProviderBase>>::Type& item)const;
					vint							Add(const Ptr<MemoryNodeProviderBase>& item);
					bool							Remove(const KeyType<Ptr<MemoryNodeProviderBase>>::Type& item);
					bool							RemoveAt(vint index);
					bool							RemoveRange(vint index, vint count);
					bool							Clear();
					vint							Insert(vint index, const Ptr<MemoryNodeProviderBase>& item);
					bool							Set(vint index, const Ptr<MemoryNodeProviderBase>& item);
				public:
					MemoryNodeProviderBase();
					~MemoryNodeProviderBase();

					IChildList&						Children();

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
					Interface*						RequestView(const WString& identifier)override;
					void							ReleaseView(Interface* view)override;
				};

				template<typename T>
				class MemoryNodeProvider : public MemoryNodeProviderBase
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

					void SetData(const T& value)
					{
						data=value;
						OnSelfDataModified();
					}
				};
				
				template<typename T>
				class MemoryNodeRootProvider : public MemoryNodeRootProviderBase
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
TreeListControl
***********************************************************************/

			class TreeListControl : public GuiSelectableListControl
			{
			protected:
				tree::NodeItemProvider*				nodeItemProvider;
				tree::INodeItemView*				nodeItemView;
				Ptr<tree::INodeItemStyleProvider>	nodeStyleProvider;
			public:
				TreeListControl(IStyleProvider* _styleProvider, tree::INodeRootProvider* _nodeRootProvider);
				~TreeListControl();

				tree::INodeItemView*				GetNodeItemView();
				tree::INodeRootProvider*			GetNodeRootProvider();
				tree::INodeItemStyleProvider*		GetNodeStyleProvider();
				Ptr<tree::INodeItemStyleProvider>	SetNodeStyleProvider(Ptr<tree::INodeItemStyleProvider> styleProvider);
			};
		}
	}
}

#endif