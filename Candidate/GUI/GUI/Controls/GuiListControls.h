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
					virtual elements::GuiGraphicsComposition*	GetContainerComposition()=0;
					virtual void								OnTotalSizeChanged()=0;
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

					void										OnAttached(IItemProvider* provider);
					void										OnItemModified(int start, int count, int newCount);
					IItemStyleController*						RequestItem(int itemIndex);
					void										ReleaseItem(IItemStyleController* style);
					void										SetViewLocation(Point value);
					elements::GuiGraphicsComposition*			GetContainerComposition();
					void										OnTotalSizeChanged();
				};

				Ptr<ItemCallback>								callback;
				Ptr<IItemProvider>								itemProvider;
				Ptr<IItemStyleProvider>							itemStyleProvider;
				Ptr<IItemArranger>								itemArranger;
				
				void											OnRenderTargetChanged(elements::IGuiGraphicsRenderTarget* renderTarget);
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

/***********************************************************************
Predefined ItemArranger
***********************************************************************/

			namespace list
			{
				class FixedHeightItemArranger : public Object, public GuiListControl::IItemArranger
				{
					typedef collections::List<GuiListControl::IItemStyleController*>		StyleList;
				protected:
					GuiListControl::IItemArrangerCallback*		callback;
					GuiListControl::IItemProvider*				itemProvider;
					Rect										viewBounds;
					int											startIndex;
					int											rowHeight;
					StyleList									visibleStyles;
					bool										suppressOnViewChanged;

					void										ClearStyles();
					void										RearrangeItemBounds();
				public:
					FixedHeightItemArranger();
					~FixedHeightItemArranger();

					void										OnAttached(GuiListControl::IItemProvider* provider);
					void										OnItemModified(int start, int count, int newCount);
					void										AttachListControl(GuiListControl* value);
					void										DetachListControl();
					GuiListControl::IItemArrangerCallback*		GetCallback();
					void										SetCallback(GuiListControl::IItemArrangerCallback* value);
					Size										GetTotalSize();
					GuiListControl::IItemStyleController*		GetVisibleStyle(int itemIndex);
					int											GetVisibleIndex(GuiListControl::IItemStyleController* style);
					void										OnViewChanged(Rect bounds);
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
					
					GuiListControl::IItemStyleProvider*			GetStyleProvider();
					int											GetItemStyleId();
					elements::GuiBoundsComposition*				GetBoundsComposition();
					bool										IsCacheable();
					bool										IsInstalled();
					void										OnInstalled();
					void										OnUninstalled();
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

					void										InvokeOnItemModified(int start, int count, int newCount);
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

/***********************************************************************
TextList Components
***********************************************************************/

			namespace list
			{
				class TextItem
				{
				protected:
					WString						text;
					bool						checked;
					bool						selected;

				public:
					TextItem();
					TextItem(const TextItem& item);
					TextItem(const WString& _text, bool _checked=false, bool _selected=false);
					~TextItem();

					bool						operator==(const TextItem& value)const;
					bool						operator!=(const TextItem& value)const;

					const WString&				GetText()const;
					bool						GetChecked()const;
					bool						GetSelected()const;
				};

				class TextItemProvider : public ListProvider<TextItem>
				{
				public:
					TextItemProvider();
					~TextItemProvider();
					
					void						SetText(int itemIndex, const WString& value);
					void						SetChecked(int itemIndex, bool value);
					void						SetSelected(int itemIndex, bool value);
				};

				class TextItemStyleProvider : public Object, public GuiListControl::IItemStyleProvider
				{
				public:
					class IBulletStyleProvider : public Interface
					{
					public:
						virtual GuiSelectableButton::IStyleController*		CreateBulletStyleController()=0;
					};

				protected:
					class TextItemStyleController : public ItemStyleControllerBase
					{
					protected:
						GuiSelectableButton*					backgroundButton;
						GuiSelectableButton*					bulletButton;
						TextItemStyleProvider*					textItemProvider;

						void									OnBackgroundSelectedChanged(elements::GuiGraphicsComposition* sender, elements::GuiEventArgs& arguments);
						void									OnBulletSelectedChanged(elements::GuiGraphicsComposition* sender, elements::GuiEventArgs& arguments);
					public:
						TextItemStyleController(TextItemStyleProvider* provider, GuiSelectableButton* _bulletButton);
						~TextItemStyleController();

						bool									GetSelected();
						void									SetSelected(bool value);
						bool									GetChecked();
						void									SetChecked(bool value);
					};

					IBulletStyleProvider*						bulletStyleProvider;
					TextItemProvider*							textItemProvider;

					void										OnSelectedChanged(TextItemStyleController* style);
					void										OnCheckedChanged(TextItemStyleController* style);
				public:
					TextItemStyleProvider(TextItemProvider* _textItemProvider, IBulletStyleProvider* _bulletStyleProvider);
					~TextItemStyleProvider();

					void										AttachListControl(GuiListControl* value);
					void										DetachListControl();
					int											GetItemStyleId(int itemIndex);
					GuiListControl::IItemStyleController*		CreateItemStyle(int styleId);
					void										DestroyItemStyle(GuiListControl::IItemStyleController* style);
					void										Install(GuiListControl::IItemStyleController* style, int itemIndex);
				};
			}
		}
	}
}

#endif