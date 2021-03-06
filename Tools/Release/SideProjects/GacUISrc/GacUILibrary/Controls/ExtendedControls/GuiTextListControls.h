/***********************************************************************
Vczh Library++ 3.0
Developer: �����(vczh)
GacUI::Control System

Interfaces:
***********************************************************************/

#ifndef VCZH_PRESENTATION_CONTROLS_GUITEXTLISTCONTROLS
#define VCZH_PRESENTATION_CONTROLS_GUITEXTLISTCONTROLS

#include "..\GuiListControls.h"

namespace vl
{
	namespace presentation
	{
		namespace controls
		{
			class GuiVirtualTextList;

			namespace list
			{

/***********************************************************************
TextList Style Provider
***********************************************************************/

				class TextItemStyleProvider : public Object, public GuiSelectableListControl::IItemStyleProvider, public Description<TextItemStyleProvider>
				{
				public:
					class ITextItemStyleProvider : public virtual IDescriptable, public Description<ITextItemStyleProvider>
					{
					public:
						virtual GuiSelectableButton::IStyleController*		CreateBackgroundStyleController()=0;
						virtual GuiSelectableButton::IStyleController*		CreateBulletStyleController()=0;
					};

					class ITextItemView : public virtual GuiListControl::IItemPrimaryTextView, public Description<ITextItemView>
					{
					public:
						static const wchar_t* const				Identifier;

						virtual WString							GetText(int itemIndex)=0;
						virtual bool							GetChecked(int itemIndex)=0;
						virtual void							SetCheckedSilently(int itemIndex, bool value)=0;
					};

					class TextItemStyleController : public ItemStyleControllerBase, public Description<TextItemStyleController>
					{
					protected:
						GuiSelectableButton*					backgroundButton;
						GuiSelectableButton*					bulletButton;
						elements::GuiSolidLabelElement*			textElement;
						TextItemStyleProvider*					textItemStyleProvider;

						void									OnBulletSelectedChanged(elements::GuiGraphicsComposition* sender, elements::GuiEventArgs& arguments);
					public:
						TextItemStyleController(TextItemStyleProvider* provider);
						~TextItemStyleController();

						bool									GetSelected();
						void									SetSelected(bool value);
						bool									GetChecked();
						void									SetChecked(bool value);
						const WString&							GetText();
						void									SetText(const WString& value);
					};

				protected:
					Ptr<ITextItemStyleProvider>					textItemStyleProvider;
					ITextItemView*								textItemView;
					GuiListControl*								listControl;

					void										OnStyleCheckedChanged(TextItemStyleController* style);
				public:
					TextItemStyleProvider(ITextItemStyleProvider* _textItemStyleProvider);
					~TextItemStyleProvider();

					void										AttachListControl(GuiListControl* value)override;
					void										DetachListControl()override;
					int											GetItemStyleId(int itemIndex)override;
					GuiListControl::IItemStyleController*		CreateItemStyle(int styleId)override;
					void										DestroyItemStyle(GuiListControl::IItemStyleController* style)override;
					void										Install(GuiListControl::IItemStyleController* style, int itemIndex)override;
					void										SetStyleSelected(GuiListControl::IItemStyleController* style, bool value)override;
				};

/***********************************************************************
TextList Data Source
***********************************************************************/

				class TextItem
				{
					friend class TextItemProvider;
				protected:
					WString										text;
					bool										checked;
				public:
					TextItem();
					TextItem(const TextItem& item);
					TextItem(const WString& _text, bool _checked=false);
					TextItem(const wchar_t* _text, bool _checked=false);
					~TextItem();

					bool										operator==(const TextItem& value)const;
					bool										operator!=(const TextItem& value)const;

					const WString&								GetText()const;
					bool										GetChecked()const;
				};

				class TextItemProvider : public ListProvider<TextItem>, protected TextItemStyleProvider::ITextItemView, public Description<TextItemProvider>
				{
				protected:
					bool										ContainsPrimaryText(int itemIndex)override;
					WString										GetPrimaryTextViewText(int itemIndex)override;
					WString										GetText(int itemIndex)override;
					bool										GetChecked(int itemIndex)override;
					void										SetCheckedSilently(int itemIndex, bool value)override;
				public:
					TextItemProvider();
					~TextItemProvider();
					
					void										SetText(int itemIndex, const WString& value);
					void										SetChecked(int itemIndex, bool value);

					IDescriptable*								RequestView(const WString& identifier)override;
					void										ReleaseView(IDescriptable* view)override;
				};
			}

/***********************************************************************
TextList Control
***********************************************************************/

			class GuiVirtualTextList : public GuiSelectableListControl, public Description<GuiVirtualTextList>
			{
			public:
				GuiVirtualTextList(IStyleProvider* _styleProvider, list::TextItemStyleProvider::ITextItemStyleProvider* _itemStyleProvider, GuiListControl::IItemProvider* _itemProvider);
				~GuiVirtualTextList();
				
				Ptr<GuiListControl::IItemStyleProvider>			SetStyleProvider(Ptr<GuiListControl::IItemStyleProvider> value);
				Ptr<GuiListControl::IItemStyleProvider>			ChangeItemStyle(list::TextItemStyleProvider::ITextItemStyleProvider* itemStyleProvider);
			};

			class GuiTextList : public GuiVirtualTextList, public Description<GuiTextList>
			{
			protected:
				list::TextItemProvider*							items;
			public:
				GuiTextList(IStyleProvider* _styleProvider, list::TextItemStyleProvider::ITextItemStyleProvider* _itemStyleProvider);
				~GuiTextList();

				list::TextItemProvider&							GetItems();
			};
		}
	}
}

#endif