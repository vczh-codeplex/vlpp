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
					virtual void					OnAttached(INodeProvider* provider)=0;
					virtual void					OnItemModified(INodeProvider* parentNode, int start, int count, int newCount)=0;
				};

				//-----------------------------------------------------------
				// Provider Interfaces
				//-----------------------------------------------------------

				class INodeProvider : public virtual Interface
				{
				public:
					virtual int						IndexInParent()=0;
					virtual bool					GetExpending()=0;
					virtual void					SetExpending(bool value)=0;
					virtual int						CalculateTotalVisibleNodes()=0;

					virtual int						Count()=0;
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
				};

				class NodeItemProvider
					: public list::ItemProviderBase
					, protected virtual INodeProviderCallback
					, protected virtual INodeItemView
				{
				protected:
					Ptr<INodeRootProvider>			root;

					INodeProvider*					GetNodeByOffset(INodeProvider* provider, int offset);
					void							OnAttached(INodeProvider* provider)override;
					void							OnItemModified(INodeProvider* parentNode, int start, int count, int newCount)override;
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
		}
	}
}

#endif