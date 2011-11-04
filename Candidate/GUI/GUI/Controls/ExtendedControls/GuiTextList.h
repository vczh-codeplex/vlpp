/***********************************************************************
Vczh Library++ 3.0
Developer: ³Âè÷å«(vczh)
GacUI::Control System

Interfaces:
***********************************************************************/

#ifndef VCZH_PRESENTATION_CONTROLS_GUITEXTLIST
#define VCZH_PRESENTATION_CONTROLS_GUITEXTLIST

#include "..\GuiListControls.h"

namespace vl
{
	namespace presentation
	{
		namespace controls
		{

/***********************************************************************
TextList Data Source
***********************************************************************/

			namespace list
			{
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

				class TextItemProvider : public ListProvider<TextItem>
				{
					friend class TextItemStyleProvider;
				protected:
					void										SetCheckedSilently(int itemIndex, bool value);
				public:
					TextItemProvider();
					~TextItemProvider();
					
					void										SetText(int itemIndex, const WString& value);
					void										SetChecked(int itemIndex, bool value);
				};

/***********************************************************************
TextList Style Provider
***********************************************************************/

				class TextItemStyleProvider : public Object, public GuiSelectableListControl::IItemStyleProvider
				{
				public:
					class ITextItemStyleProvider : public Interface
					{
					public:
						virtual GuiSelectableButton::IStyleController*		CreateBackgroundStyleController()=0;
						virtual GuiSelectableButton::IStyleController*		CreateBulletStyleController()=0;
					};
				protected:
					class TextItemStyleController : public ItemStyleControllerBase
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
					
					friend class collections::ReadonlyListEnumerator<TextItemStyleController*>;

					Ptr<ITextItemStyleProvider>					textItemStyleProvider;
					TextItemProvider*							textItemProvider;
					GuiListControl*								listControl;

					void										OnStyleCheckedChanged(TextItemStyleController* style);
				public:
					TextItemStyleProvider(ITextItemStyleProvider* _textItemStyleProvider);
					~TextItemStyleProvider();

					void										AttachListControl(GuiListControl* value);
					void										DetachListControl();
					int											GetItemStyleId(int itemIndex);
					GuiListControl::IItemStyleController*		CreateItemStyle(int styleId);
					void										DestroyItemStyle(GuiListControl::IItemStyleController* style);
					void										Install(GuiListControl::IItemStyleController* style, int itemIndex);
					void										SetStyleSelected(GuiListControl::IItemStyleController* style, bool value);
				};
			}

/***********************************************************************
TextList Control
***********************************************************************/

			class GuiTextList : public GuiSelectableListControl
			{
			protected:
				list::TextItemProvider*							items;
			public:
				GuiTextList(IStyleProvider* _styleProvider, list::TextItemStyleProvider::ITextItemStyleProvider* _itemStyleProvider);
				~GuiTextList();
				
				Ptr<GuiListControl::IItemStyleProvider>			SetStyleProvider(Ptr<GuiListControl::IItemStyleProvider> value);
				Ptr<GuiListControl::IItemStyleProvider>			ChangeItemStyle(list::TextItemStyleProvider::ITextItemStyleProvider* itemStyleProvider);
				list::TextItemProvider&							GetItems();
			};
		}
	}
}

#endif