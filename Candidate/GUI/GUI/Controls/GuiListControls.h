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

				class IItemProviderCallback : public virtual Interface
				{
				public:
					virtual void								OnAttached(IItemProvider* provider)=0;
					virtual void								OnItemModified(int start, int count, int newCount)=0;
				};

				class IItemArrangerCallback : public virtual Interface
				{
				public:
					virtual IItemStyleController*				RequestItem(int itemIndex)=0;
					virtual void								ReleaseItem(IItemStyleController* style)=0;
					virtual void								SetViewLocation(Point value)=0;
					virtual Size								GetStylePreferredSize(IItemStyleController* style)=0;
					virtual void								SetStyleAlignmentToParent(IItemStyleController* style, Margin margin)=0;
					virtual void								SetStyleBounds(IItemStyleController* style, Rect bounds)=0;
					virtual void								OnTotalSizeChanged()=0;
				};

				//-----------------------------------------------------------
				// Provider Interfaces
				//-----------------------------------------------------------

				class IItemProvider : public virtual Interface
				{
				public:
					virtual bool								AttachCallback(IItemProviderCallback* value)=0;
					virtual bool								DetachCallback(IItemProviderCallback* value)=0;
					virtual int									Count()=0;
					virtual Interface*							RequestView(const WString& identifier)=0;
					virtual void								ReleaseView(Interface* view)=0;
				};

				class IItemStyleController : public virtual Interface
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

				class IItemStyleProvider : public virtual Interface
				{
				public:
					virtual void								AttachListControl(GuiListControl* value)=0;
					virtual void								DetachListControl()=0;
					virtual int									GetItemStyleId(int itemIndex)=0;
					virtual IItemStyleController*				CreateItemStyle(int styleId)=0;
					virtual void								DestroyItemStyle(IItemStyleController* style)=0;
					virtual void								Install(IItemStyleController* style, int itemIndex)=0;
				};

				class IItemArranger : public virtual IItemProviderCallback
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

				class IItemCoordinateTransformer : public virtual Interface
				{
				public:
					virtual Size								RealSizeToVirtualSize(Size size)=0;
					virtual Size								VirtualSizeToRealSize(Size size)=0;
					virtual Point								RealPointToVirtualPoint(Size realFullSize, Point point)=0;
					virtual Point								VirtualPointToRealPoint(Size realFullSize, Point point)=0;
					virtual Rect								RealRectToVirtualRect(Size realFullSize, Rect rect)=0;
					virtual Rect								VirtualRectToRealRect(Size realFullSize, Rect rect)=0;
					virtual Margin								RealMarginToVirtualMargin(Margin margin)=0;
					virtual Margin								VirtualMarginToRealMargin(Margin margin)=0;
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
					Size										GetStylePreferredSize(IItemStyleController* style);
					void										SetStyleAlignmentToParent(IItemStyleController* style, Margin margin);
					void										SetStyleBounds(IItemStyleController* style, Rect bounds);
					void										OnTotalSizeChanged()override;
				};

				Ptr<ItemCallback>								callback;
				Ptr<IItemProvider>								itemProvider;
				Ptr<IItemStyleProvider>							itemStyleProvider;
				Ptr<IItemArranger>								itemArranger;
				Ptr<IItemCoordinateTransformer>					itemCoordinateTransformer;
				Size											fullSize;

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

				elements::GuiNotifyEvent						StyleProviderChanged;
				elements::GuiNotifyEvent						ArrangerChanged;
				elements::GuiNotifyEvent						CoordinateTransformerChanged;

				virtual IItemProvider*							GetItemProvider();
				virtual IItemStyleProvider*						GetStyleProvider();
				virtual Ptr<IItemStyleProvider>					SetStyleProvider(Ptr<IItemStyleProvider> value);
				virtual IItemArranger*							GetArranger();
				virtual Ptr<IItemArranger>						SetArranger(Ptr<IItemArranger> value);
				virtual IItemCoordinateTransformer*				GetCoordinateTransformer();
				virtual Ptr<IItemCoordinateTransformer>			SetCoordinateTransformer(Ptr<IItemCoordinateTransformer> value);
			};

			class GuiSelectableListControl : public GuiListControl
			{
			public:
				class IItemStyleProvider : public virtual GuiListControl::IItemStyleProvider
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
Predefined ItemCoordinateTransformer
***********************************************************************/

			namespace list
			{
				class DefaultItemCoordinateTransformer : public Object, virtual public GuiListControl::IItemCoordinateTransformer
				{
				public:
					DefaultItemCoordinateTransformer();
					~DefaultItemCoordinateTransformer();

					Size										RealSizeToVirtualSize(Size size)override;
					Size										VirtualSizeToRealSize(Size size)override;
					Point										RealPointToVirtualPoint(Size realFullSize, Point point)override;
					Point										VirtualPointToRealPoint(Size realFullSize, Point point)override;
					Rect										RealRectToVirtualRect(Size realFullSize, Rect rect)override;
					Rect										VirtualRectToRealRect(Size realFullSize, Rect rect)override;
					Margin										RealMarginToVirtualMargin(Margin margin)override;
					Margin										VirtualMarginToRealMargin(Margin margin)override;
				};

				class AxisAlignedItemCoordinateTransformer : public Object, virtual public GuiListControl::IItemCoordinateTransformer
				{
				public:
					enum Alignment
					{
						LeftDown,
						RightDown,
						LeftUp,
						RightUp,
						DownLeft,
						DownRight,
						UpLeft,
						UpRight,
					};
				protected:
					Alignment									alignment;

				public:
					AxisAlignedItemCoordinateTransformer(Alignment _alignment);
					~AxisAlignedItemCoordinateTransformer();

					Alignment									GetAlignment();
					Size										RealSizeToVirtualSize(Size size)override;
					Size										VirtualSizeToRealSize(Size size)override;
					Point										RealPointToVirtualPoint(Size realFullSize, Point point)override;
					Point										VirtualPointToRealPoint(Size realFullSize, Point point)override;
					Rect										RealRectToVirtualRect(Size realFullSize, Rect rect)override;
					Rect										VirtualRectToRealRect(Size realFullSize, Rect rect)override;
					Margin										RealMarginToVirtualMargin(Margin margin)override;
					Margin										VirtualMarginToRealMargin(Margin margin)override;
				};
			};

/***********************************************************************
Predefined ItemArranger
***********************************************************************/

			namespace list
			{
				class RangedItemArrangerBase : public Object, virtual public GuiListControl::IItemArranger
				{
					typedef collections::List<GuiListControl::IItemStyleController*>		StyleList;
				protected:
					GuiListControl::IItemArrangerCallback*		callback;
					GuiListControl::IItemProvider*				itemProvider;
					Rect										viewBounds;
					int											startIndex;
					StyleList									visibleStyles;

					virtual void								ClearStyles();
					virtual void								OnStylesCleared()=0;
					virtual Size								OnCalculateTotalSize()=0;
					virtual void								OnViewChangedInternal(Rect oldBounds, Rect newBounds)=0;
				public:
					RangedItemArrangerBase();
					~RangedItemArrangerBase();

					void										OnAttached(GuiListControl::IItemProvider* provider)override;
					void										OnItemModified(int start, int count, int newCount)override;
					void										AttachListControl(GuiListControl* value)override;
					void										DetachListControl()override;
					GuiListControl::IItemArrangerCallback*		GetCallback()override;
					void										SetCallback(GuiListControl::IItemArrangerCallback* value)override;
					Size										GetTotalSize()override;
					GuiListControl::IItemStyleController*		GetVisibleStyle(int itemIndex)override;
					int											GetVisibleIndex(GuiListControl::IItemStyleController* style)override;
					void										OnViewChanged(Rect bounds)override;
				};

				class FixedHeightItemArranger : public RangedItemArrangerBase
				{
				protected:
					int											rowHeight;
					bool										suppressOnViewChanged;

					void										RearrangeItemBounds();
					void										OnStylesCleared()override;
					Size										OnCalculateTotalSize()override;
					void										OnViewChangedInternal(Rect oldBounds, Rect newBounds)override;
				public:
					FixedHeightItemArranger();
					~FixedHeightItemArranger();
				};

				class FixedSizeMultiColumnItemArranger : public RangedItemArrangerBase
				{
				protected:
					Size										itemSize;
					bool										suppressOnViewChanged;

					void										RearrangeItemBounds();
					void										CalculateRange(Size itemSize, Rect bounds, int count, int& start, int& end);
					void										OnStylesCleared()override;
					Size										OnCalculateTotalSize()override;
					void										OnViewChangedInternal(Rect oldBounds, Rect newBounds)override;
				public:
					FixedSizeMultiColumnItemArranger();
					~FixedSizeMultiColumnItemArranger();
				};

				class FixedHeightMultiColumnItemArranger : public RangedItemArrangerBase
				{
				protected:
					int											itemHeight;
					bool										suppressOnViewChanged;

					void										RearrangeItemBounds();
					void										CalculateRange(int itemHeight, Rect bounds, int& rows, int& startColumn);
					void										OnStylesCleared()override;
					Size										OnCalculateTotalSize()override;
					void										OnViewChangedInternal(Rect oldBounds, Rect newBounds)override;
				public:
					FixedHeightMultiColumnItemArranger();
					~FixedHeightMultiColumnItemArranger();
				};
			}

/***********************************************************************
Predefined ItemStyleController
***********************************************************************/

			namespace list
			{
				class ItemStyleControllerBase : public Object, public virtual GuiListControl::IItemStyleController
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
				class ItemProviderBase : public Object, public virtual GuiListControl::IItemProvider
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

					bool NotifyUpdate(int start, int count=1)
					{
						if(start<0 || start>=proxy->Count() || count<=0 || start+count>proxy->Count())
						{
							return false;
						}
						else
						{
							InvokeOnItemModified(start, count, count);
							return true;
						}
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