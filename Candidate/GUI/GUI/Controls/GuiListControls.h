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

/***********************************************************************
List Control
***********************************************************************/

			class GuiListControl : public GuiScrollView
			{
			public:
				class IItemProvider;
				class IItemStyleController;
				class IItemStyleProvider;

				//-----------------------------------------------------------
				// Callback Interfaces
				//-----------------------------------------------------------

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
					virtual elements::GuiGraphicsComposition*	GetContainerComposition()=0;
					virtual void								OnTotalSizeChanged()=0;
				};

				//-----------------------------------------------------------
				// Provider Interfaces
				//-----------------------------------------------------------

				class IItemProvider : public Interface
				{
				public:
					virtual bool								AttachCallback(IItemProviderCallback* value)=0;
					virtual bool								DetachCallback(IItemProviderCallback* value)=0;
					virtual int									Count()=0;
					virtual Interface*							RequestView(const WString& identifier)=0;
					virtual void								ReleaseView(Interface* view)=0;
				};

				class IItemStyleController : public Interface
				{
				public:
					virtual IItemStyleProvider*					GetStyleProvider()=0;
					virtual int									GetItemStyleId()=0;
					virtual elements::GuiBoundsComposition*		GetBoundsComposition()=0;
					virtual bool								IsCacheable()=0;
					virtual bool								IsInstalled()=0;
					virtual void								OnInstalled()=0;
					virtual void								OnUninstalled()=0;
				};

				class IItemStyleProvider : public Interface
				{
				public:
					virtual void								AttachListControl(GuiListControl* value)=0;
					virtual void								DetachListControl()=0;
					virtual int									GetItemStyleId(int itemIndex)=0;
					virtual IItemStyleController*				CreateItemStyle(int styleId)=0;
					virtual void								DestroyItemStyle(IItemStyleController* style)=0;
					virtual void								Install(IItemStyleController* style, int itemIndex)=0;
				};

				class IItemArranger : public IItemProviderCallback
				{
				public:
					virtual void								AttachListControl(GuiListControl* value)=0;
					virtual void								DetachListControl()=0;
					virtual IItemArrangerCallback*				GetCallback()=0;
					virtual void								SetCallback(IItemArrangerCallback* value)=0;
					virtual Size								GetTotalSize()=0;
					virtual IItemStyleController*				GetVisibleStyle(int itemIndex)=0;
					virtual int									GetVisibleIndex(IItemStyleController* style)=0;
					virtual void								OnViewChanged(Rect bounds)=0;
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

					void										OnAttached(IItemProvider* provider)override;
					void										OnItemModified(int start, int count, int newCount)override;
					IItemStyleController*						RequestItem(int itemIndex)override;
					void										ReleaseItem(IItemStyleController* style)override;
					void										SetViewLocation(Point value)override;
					elements::GuiGraphicsComposition*			GetContainerComposition()override;
					void										OnTotalSizeChanged()override;
				};

				Ptr<ItemCallback>								callback;
				Ptr<IItemProvider>								itemProvider;
				Ptr<IItemStyleProvider>							itemStyleProvider;
				Ptr<IItemArranger>								itemArranger;

				virtual void									OnItemModified(int start, int count, int newCount);
				virtual void									OnStyleInstalled(int itemIndex, IItemStyleController* style);
				virtual void									OnStyleUninstalled(IItemStyleController* style);
				
				void											OnRenderTargetChanged(elements::IGuiGraphicsRenderTarget* renderTarget)override;
				Size											QueryFullSize()override;
				void											UpdateView(Rect viewBounds)override;
				
				void											OnBoundsMouseButtonDown(elements::GuiGraphicsComposition* sender, elements::GuiMouseEventArgs& arguments);
				void											SetStyleProviderAndArranger(Ptr<IItemStyleProvider> styleProvider, Ptr<IItemArranger> arranger);
			public:
				GuiListControl(IStyleProvider* _styleProvider, IItemProvider* _itemProvider, bool acceptFocus=false);
				~GuiListControl();

				virtual IItemProvider*							GetItemProvider();
				virtual IItemStyleProvider*						GetStyleProvider();
				virtual Ptr<IItemStyleProvider>					SetStyleProvider(Ptr<IItemStyleProvider> value);
				virtual IItemArranger*							GetArranger();
				virtual Ptr<IItemArranger>						SetArranger(Ptr<IItemArranger> value);
			};

			class GuiSelectableListControl : public GuiListControl
			{
			public:
				class IItemStyleProvider : public GuiListControl::IItemStyleProvider
				{
				public:
					virtual void								SetStyleSelected(IItemStyleController* style, bool value)=0;
				};

			protected:
				class StyleEvents
				{
				protected:
					GuiSelectableListControl*					listControl;
					IItemStyleController*						style;
					Ptr<elements::GuiMouseEvent::IHandler>		leftButtonDownHandler;

					void										OnBoundsLeftButtonDown(elements::GuiGraphicsComposition* sender, elements::GuiMouseEventArgs& arguments);
				public:
					StyleEvents(GuiSelectableListControl* _listControl, IItemStyleController* _style);
					~StyleEvents();

					void										AttachEvents();
					void										DetachEvents();
				};

				friend class collections::ReadonlyListEnumerator<Ptr<StyleEvents>>;
				typedef collections::Dictionary<IItemStyleController*, Ptr<StyleEvents>>	VisibleStyleMap;

			protected:

				Ptr<IItemStyleProvider>							selectableStyleProvider;
				collections::SortedList<int>					selectedItems;
				VisibleStyleMap									visibleStyles;
				bool											multiSelect;

				void											OnItemModified(int start, int count, int newCount)override;
				void											OnStyleInstalled(int itemIndex, IItemStyleController* style)override;
				void											OnStyleUninstalled(IItemStyleController* style)override;
				virtual void									OnItemSelectionChanged(int itemIndex, bool value);
				virtual void									OnItemSelectionCleared();
			public:
				GuiSelectableListControl(IStyleProvider* _styleProvider, IItemProvider* _itemProvider);
				~GuiSelectableListControl();

				elements::GuiNotifyEvent						SelectionChanged;

				Ptr<GuiListControl::IItemStyleProvider>			SetStyleProvider(Ptr<GuiListControl::IItemStyleProvider> value)override;

				bool											GetMultiSelect();
				void											SetMultiSelect(bool value);
				
				const collections::IReadonlyList<int>&			GetSelectedItems();
				bool											GetSelected(int itemIndex);
				void											SetSelected(int itemIndex, bool value);
				void											ClearSelection();
			};

/***********************************************************************
Predefined ItemArranger
***********************************************************************/

			namespace list
			{
				class RangedItemArrangerBase : public Object, public GuiListControl::IItemArranger
				{
					typedef collections::List<GuiListControl::IItemStyleController*>		StyleList;
				protected:
					GuiListControl::IItemArrangerCallback*		callback;
					GuiListControl::IItemProvider*				itemProvider;
					Rect										viewBounds;
					int											startIndex;
					StyleList									visibleStyles;
					bool										suppressOnViewChanged;

					virtual void								ClearStyles();
					virtual void								RearrangeItemBounds()=0;
				public:
					RangedItemArrangerBase();
					~RangedItemArrangerBase();

					void										OnAttached(GuiListControl::IItemProvider* provider)override;
					void										AttachListControl(GuiListControl* value)override;
					void										DetachListControl()override;
					GuiListControl::IItemArrangerCallback*		GetCallback()override;
					void										SetCallback(GuiListControl::IItemArrangerCallback* value)override;
					GuiListControl::IItemStyleController*		GetVisibleStyle(int itemIndex)override;
					int											GetVisibleIndex(GuiListControl::IItemStyleController* style)override;
				};

				class FixedHeightItemArranger : public RangedItemArrangerBase
				{
					typedef collections::List<GuiListControl::IItemStyleController*>		StyleList;
				protected:
					int											rowHeight;

					void										ClearStyles()override;
					void										RearrangeItemBounds()override;
				public:
					FixedHeightItemArranger();
					~FixedHeightItemArranger();

					void										OnItemModified(int start, int count, int newCount)override;
					Size										GetTotalSize()override;
					void										OnViewChanged(Rect bounds)override;
				};
			}

/***********************************************************************
Predefined ItemStyleController
***********************************************************************/

			namespace list
			{
				class ItemStyleControllerBase : public Object, public GuiListControl::IItemStyleController
				{
				protected:
					GuiListControl::IItemStyleProvider*			provider;
					int											styleId;
					elements::GuiBoundsComposition*				boundsComposition;
					GuiControl*									associatedControl;
					bool										isInstalled;

					void										Initialize(elements::GuiBoundsComposition* _boundsComposition, GuiControl* _associatedControl);
					void										Finalize();

					ItemStyleControllerBase(GuiListControl::IItemStyleProvider* _provider, int _styleId);
				public:
					~ItemStyleControllerBase();
					
					GuiListControl::IItemStyleProvider*			GetStyleProvider()override;
					int											GetItemStyleId()override;
					elements::GuiBoundsComposition*				GetBoundsComposition()override;
					bool										IsCacheable()override;
					bool										IsInstalled()override;
					void										OnInstalled()override;
					void										OnUninstalled()override;
				};
			}

/***********************************************************************
Predefined ItemProvider
***********************************************************************/

			namespace list
			{
				class ItemProviderBase : public Object, public GuiListControl::IItemProvider
				{
				protected:
					collections::List<GuiListControl::IItemProviderCallback*>	callbacks;

					virtual void								InvokeOnItemModified(int start, int count, int newCount);
				public:
					ItemProviderBase();
					~ItemProviderBase();

					bool										AttachCallback(GuiListControl::IItemProviderCallback* value);
					bool										DetachCallback(GuiListControl::IItemProviderCallback* value);
				};

				template<typename T, typename K=typename KeyType<T>::Type>
				class ListWrapperProvider : public ItemProviderBase, public collections::IList<T, K>
				{
				protected:
					collections::IList<T, K>*			proxy;

					ListWrapperProvider()
						:proxy(0)
					{
					}
				public:
					ListWrapperProvider(collections::IList<T, K>* _proxy)
						:proxy(_proxy)
					{
					}

					~ListWrapperProvider()
					{
					}

					collections::IEnumerator<T>* CreateEnumerator()const
					{
						return proxy->CreateEnumerator();
					}

					bool Contains(const K& item)const
					{
						return proxy->Contains(item);
					}

					vint Count()const
					{
						return proxy->Count();
					}

					vint Count()
					{
						return proxy->Count();
					}

					const T& Get(vint index)const
					{
						return proxy->Get(index);
					}

					const T& operator[](vint index)const
					{
						return (*proxy)[index];
					}

					vint IndexOf(const K& item)const
					{
						return proxy->IndexOf(item);
					}

					vint Add(const T& item)
					{
						return Insert(proxy->Count(), item);
					}

					bool Remove(const K& item)
					{
						vint index=proxy->IndexOf(item);
						if(index==-1) return false;
						return RemoveAt(index);
					}

					bool RemoveAt(vint index)
					{
						if(proxy->RemoveAt(index))
						{
							InvokeOnItemModified(index, 1, 0);
							return true;
						}
						else
						{
							return false;
						}
					}

					bool RemoveRange(vint index, vint count)
					{
						if(proxy->RemoveRange(index, count))
						{
							InvokeOnItemModified(index, count, 0);
							return true;
						}
						else
						{
							return false;
						}
					}

					bool Clear()
					{
						vint count=proxy->Count();
						if(proxy->Clear())
						{
							InvokeOnItemModified(0, count, 0);
							return true;
						}
						else
						{
							return false;
						}
					}

					vint Insert(vint index, const T& item)
					{
						vint result=proxy->Insert(index, item);
						InvokeOnItemModified(index, 0, 1);
						return result;
					}

					bool Set(vint index, const T& item)
					{
						if(proxy->Set(index, item))
						{
							InvokeOnItemModified(index, 1, 1);
							return true;
						}
						else
						{
							return false;
						}
					}
				};

				template<typename T>
				class ListProvider : public ListWrapperProvider<T>
				{
				protected:
					collections::List<T>		list;

				public:
					ListProvider()
					{
						proxy=&list.Wrap();
					}

					~ListProvider()
					{
					}
				};
			}
		}
	}
}

#endif