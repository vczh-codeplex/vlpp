/***********************************************************************
Vczh Library++ 3.0
Developer: �����(vczh)
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
			namespace list
			{

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

					class ITextItemView : public Interface
					{
					public:
						virtual const WString&					GetText(int itemIndex)=0;
						virtual bool							GetChecked(int itemIndex)=0;
						virtual void							SetCheckedSilently(int itemIndex, bool value)=0;
					};

					static const wchar_t*						ITextItemViewIdentifier;
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
					ITextItemView*								textItemView;
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

				class TextItemProvider : public ListProvider<TextItem>, protected TextItemStyleProvider::ITextItemView
				{
				protected:
					const WString&								GetText(int itemIndex);
					bool										GetChecked(int itemIndex);
					void										SetCheckedSilently(int itemIndex, bool value);
				public:
					TextItemProvider();
					~TextItemProvider();
					
					void										SetText(int itemIndex, const WString& value);
					void										SetChecked(int itemIndex, bool value);

					Interface*									RequestView(const WString& identifier);
					void										ReleaseView(Interface* view);
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