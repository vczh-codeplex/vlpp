/***********************************************************************
Vczh Library++ 3.0
Developer: ³Âè÷å«(vczh)
GacUI::Control System

Interfaces:
***********************************************************************/

#ifndef VCZH_PRESENTATION_CONTROLS_GUILISTCONTROLS
#define VCZH_PRESENTATION_CONTROLS_GUILISTCONTROLS

#include "GuiBasicControls.h"

namespace vl
{
	namespace presentation
	{
		namespace controls
		{
			class GuiListControl : public GuiScrollView
			{
			public:

				enum ItemState
				{
					Normal,
					Active,
					Selected,
				};

				class IItemProvider;
				class IItemStyleController;
				class IItemStyleProvider;

				class IItemProviderCallback : public Interface
				{
				public:
					virtual void								OnAttached(IItemProvider* provider)=0;
					virtual void								OnItemModified(int start, int count, int newCount)=0;
				};

				class IItemArrangerCallback : public Interface
				{
				public:
					virtual IItemStyleController*				RequestItem(int itemIndex)=0;
					virtual void								ReleaseItem(IItemStyleController* style)=0;
					virtual void								SetViewLocation(Point value)=0;
				};

				//-----------------------------------------------------------

				class IItemProvider : public Interface
				{
				public:
					virtual bool								AttachCallback(IItemProviderCallback* value)=0;
					virtual bool								DetachCallback(IItemProviderCallback* value)=0;
					virtual int									Count()=0;
				};

				class IItemStyleController : public Interface
				{
				public:
					virtual IItemStyleProvider*					GetStyleProvider()=0;
					virtual int									GetItemStyleId()=0;
					virtual elements::GuiBoundsComposition*		GetBoundsComposition()=0;
					virtual void								Transfer(ItemState state)=0;
					virtual bool								IsCacheable()=0;
					virtual bool								IsInstalled()=0;
					virtual void								OnInstalled()=0;
					virtual void								OnUninstalled()=0;
				};

				class IItemStyleProvider : public Interface
				{
				public:
					virtual int									GetItemStyleId(int itemIndex)=0;
					virtual IItemStyleController*				CreateItemStyle(int styleId)=0;
					virtual bool								DestroyItemStyle(IItemStyleController* style)=0;
					virtual void								Install(IItemStyleController* style, int itemIndex)=0;
				};

				class IItemArranger : public IItemProviderCallback
				{
				public:
					virtual IItemArrangerCallback*				GetCallback()=0;
					virtual void								SetCallback(IItemArrangerCallback* value)=0;
					virtual Size								GetTotalSize()=0;
					virtual void								OnViewChanged(Rect bounds);
				};

			protected:
				class ItemCallback : public IItemProviderCallback, public IItemArrangerCallback
				{
					typedef collections::List<IItemStyleController*>			StyleList;
				protected:
					GuiListControl*								listControl;
					IItemProvider*								itemProvider;
					StyleList									cachedStyles;
					StyleList									installedStyles;

				public:
					ItemCallback(GuiListControl* _listControl);
					~ItemCallback();

					void										ClearCache();

					void										OnAttached(IItemProvider* provider);
					void										OnItemModified(int start, int count, int newCount);
					IItemStyleController*						RequestItem(int itemIndex);
					void										ReleaseItem(IItemStyleController* style);
					void										SetViewLocation(Point value);
				};

				Ptr<ItemCallback>								callback;
				Ptr<IItemProvider>								itemProvider;
				Ptr<IItemStyleProvider>							itemStyleProvider;
				Ptr<IItemArranger>								itemArranger;

				Size											QueryFullSize();
				void											UpdateView(Rect viewBounds);
			public:
				GuiListControl(IStyleProvider* _styleProvider, IItemProvider* _itemProvider);
				~GuiListControl();

				IItemStyleProvider*								GetStyleProvider();
				Ptr<IItemStyleProvider>							SetStyleProvider(Ptr<IItemStyleProvider> value);
				IItemArranger*									GetArranger();
				Ptr<IItemArranger>								SetArranger(Ptr<IItemArranger> value);
			};

			namespace list
			{
				class ItemProviderBase : public Object, public GuiListControl::IItemProvider
				{
				protected:
					collections::List<GuiListControl::IItemProviderCallback*>	callbacks;
				public:
					ItemProviderBase();
					~ItemProviderBase();

					bool										AttachCallback(GuiListControl::IItemProviderCallback* value);
					bool										DetachCallback(GuiListControl::IItemProviderCallback* value);
				};

				template<typename T>
				class ListProvider : public ItemProviderBase
				{
				};
			};
		}
	}
}

#endif