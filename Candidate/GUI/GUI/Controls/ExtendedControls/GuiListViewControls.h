/***********************************************************************
Vczh Library++ 3.0
Developer: ³Âè÷å«(vczh)
GacUI::Control System

Interfaces:
***********************************************************************/

#ifndef VCZH_PRESENTATION_CONTROLS_GUILISTVIEWCONTROLS
#define VCZH_PRESENTATION_CONTROLS_GUILISTVIEWCONTROLS

#include "..\GuiListControls.h"

namespace vl
{
	namespace presentation
	{
		namespace controls
		{
			class GuiListView;

			namespace list
			{

/***********************************************************************
ListView Style Provider
***********************************************************************/

				class ListViewItemStyleProvider : public Object, public GuiSelectableListControl::IItemStyleProvider
				{
				public:
					class IListViewItemView : public Interface
					{
					public:
						static const wchar_t*					Identifier;

						virtual Ptr<GuiImageData>				GetSmallImage(int itemIndex)=0;
						virtual Ptr<GuiImageData>				GetLargeImage(int itemIndex)=0;
						virtual WString							GetText(int itemIndex)=0;
						virtual WString							GetSubItem(int itemIndex, int index)=0;
					};

				protected:
					class ListViewItemStyleController : public ItemStyleControllerBase
					{
					protected:
						GuiSelectableButton*					backgroundButton;
						ListViewItemStyleProvider*				listViewItemStyleProvider;

					public:
						ListViewItemStyleController(ListViewItemStyleProvider* provider);
						~ListViewItemStyleController();

						bool									GetSelected();
						void									SetSelected(bool value);
					};

					IListViewItemView*							listViewItemView;
					GuiListView*								listControl;

				public:
					ListViewItemStyleProvider();
					~ListViewItemStyleProvider();

					void										AttachListControl(GuiListControl* value)override;
					void										DetachListControl()override;
					int											GetItemStyleId(int itemIndex)override;
					GuiListControl::IItemStyleController*		CreateItemStyle(int styleId)override;
					void										DestroyItemStyle(GuiListControl::IItemStyleController* style)override;
					void										Install(GuiListControl::IItemStyleController* style, int itemIndex)override;
					void										SetStyleSelected(GuiListControl::IItemStyleController* style, bool value)override;
				};

/***********************************************************************
ListView Data Source
***********************************************************************/

				class ListViewItem
				{
				public:
					Ptr<GuiImageData>							smallImage;
					Ptr<GuiImageData>							largeImage;
					WString										text;
					collections::List<WString>					subItems;
				};

				class ListViewItemProvider : public ListProvider<Ptr<ListViewItem>>, protected ListViewItemStyleProvider::IListViewItemView
				{
				protected:
					Ptr<GuiImageData>							GetSmallImage(int itemIndex)override;
					Ptr<GuiImageData>							GetLargeImage(int itemIndex)override;
					WString										GetText(int itemIndex)override;
					WString										GetSubItem(int itemIndex, int index)override;
				public:
					ListViewItemProvider();
					~ListViewItemProvider();

					Interface*									RequestView(const WString& identifier)override;
					void										ReleaseView(Interface* view)override;
				};
			}

/***********************************************************************
ListView Control
***********************************************************************/

			class GuiListView : public GuiSelectableListControl
			{
				friend class list::ListViewItemStyleProvider;
			public:
				class IStyleProvider : public virtual GuiSelectableListControl::IStyleProvider
				{
				public:
					virtual GuiSelectableButton::IStyleController*		CreateItemBackground()=0;
				};
			protected:
				list::ListViewItemProvider*						items;
				IStyleProvider*									styleProvider;
			public:
				GuiListView(IStyleProvider* _styleProvider);
				~GuiListView();
				
				Ptr<GuiListControl::IItemStyleProvider>			SetStyleProvider(Ptr<GuiListControl::IItemStyleProvider> value);
				list::ListViewItemProvider&						GetItems();
			};
		}
	}
}

#endif