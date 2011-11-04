#include "GuiTextList.h"

namespace vl
{
	namespace presentation
	{
		namespace controls
		{
			using namespace collections;
			using namespace elements;

			namespace list
			{

/***********************************************************************
TextItem
***********************************************************************/

				TextItem::TextItem()
					:checked(false)
				{
				}

				TextItem::TextItem(const TextItem& item)
					:text(item.text)
					,checked(item.checked)
				{
				}

				TextItem::TextItem(const WString& _text, bool _checked)
					:text(_text)
					,checked(_checked)
				{
				}

				TextItem::TextItem(const wchar_t* _text, bool _checked)
					:text(_text)
					,checked(_checked)
				{
				}

				TextItem::~TextItem()
				{
				}

				bool TextItem::operator==(const TextItem& value)const
				{
					return text==value.text;
				}

				bool TextItem::operator!=(const TextItem& value)const
				{
					return text!=value.text;
				}

				const WString& TextItem::GetText()const
				{
					return text;
				}

				bool TextItem::GetChecked()const
				{
					return checked;
				}

/***********************************************************************
TextItemProvider
***********************************************************************/

				void TextItemProvider::SetCheckedSilently(int itemIndex, bool value)
				{
					list[itemIndex].checked=value;
				}

				TextItemProvider::TextItemProvider()
				{
				}

				TextItemProvider::~TextItemProvider()
				{
				}
					
				void TextItemProvider::SetText(int itemIndex, const WString& value)
				{
					list[itemIndex].text=value;
					InvokeOnItemModified(itemIndex, 1, 1);
				}

				void TextItemProvider::SetChecked(int itemIndex, bool value)
				{
					SetCheckedSilently(itemIndex, value);
					InvokeOnItemModified(itemIndex, 1, 1);
				}

/***********************************************************************
TextItemStyleProvider::TextItemStyleController
***********************************************************************/

				void TextItemStyleProvider::TextItemStyleController::OnBulletSelectedChanged(elements::GuiGraphicsComposition* sender, elements::GuiEventArgs& arguments)
				{
					textItemStyleProvider->OnStyleCheckedChanged(this);
				}

				TextItemStyleProvider::TextItemStyleController::TextItemStyleController(TextItemStyleProvider* provider)
					:ItemStyleControllerBase(provider, 0)
					,backgroundButton(0)
					,bulletButton(0)
					,textElement(0)
					,textItemStyleProvider(provider)
				{
					backgroundButton=new GuiSelectableButton(textItemStyleProvider->textItemStyleProvider->CreateBackgroundStyleController());
					backgroundButton->SetAutoSelection(false);
					
					textElement=GuiSolidLabelElement::Create();
					textElement->SetAlignments(Alignment::Left, Alignment::Center);
					textElement->SetFont(backgroundButton->GetFont());

					GuiBoundsComposition* textComposition=new GuiBoundsComposition;
					textComposition->SetOwnedElement(textElement);
					textComposition->SetMinSizeLimitation(GuiGraphicsComposition::LimitToElement);

					GuiSelectableButton::IStyleController* bulletStyleController=textItemStyleProvider->textItemStyleProvider->CreateBulletStyleController();
					if(bulletStyleController)
					{
						bulletButton=new GuiSelectableButton(bulletStyleController);
						bulletButton->GetBoundsComposition()->SetAlignmentToParent(Margin(0, 0, 0, 0));
						bulletButton->SelectedChanged.AttachMethod(this, &TextItemStyleController::OnBulletSelectedChanged);

						GuiTableComposition* table=new GuiTableComposition;
						backgroundButton->GetContainerComposition()->AddChild(table);
						table->SetAlignmentToParent(Margin(0, 0, 0, 0));
						table->SetMinSizeLimitation(GuiGraphicsComposition::LimitToElementAndChildren);
						table->SetRowsAndColumns(1, 2);
						table->SetRowOption(0, GuiCellOption::PercentageOption(1.0));
						table->SetColumnOption(0, GuiCellOption::MinSizeOption());
						table->SetColumnOption(1, GuiCellOption::PercentageOption(1.0));
						{
							GuiCellComposition* cell=new GuiCellComposition;
							table->AddChild(cell);
							cell->SetSite(0, 0, 1, 1);
							cell->AddChild(bulletButton->GetBoundsComposition());
						}
						{
							GuiCellComposition* cell=new GuiCellComposition;
							table->AddChild(cell);
							cell->SetSite(0, 1, 1, 1);
							cell->AddChild(textComposition);
							textComposition->SetAlignmentToParent(Margin(0, 0, 0, 0));
						}
					}
					else
					{
						backgroundButton->GetContainerComposition()->AddChild(textComposition);
						textComposition->SetAlignmentToParent(Margin(5, 0, 0, 0));
					}
					Initialize(backgroundButton->GetBoundsComposition(), backgroundButton);
				}

				TextItemStyleProvider::TextItemStyleController::~TextItemStyleController()
				{
				}

				bool TextItemStyleProvider::TextItemStyleController::GetSelected()
				{
					return backgroundButton->GetSelected();
				}

				void TextItemStyleProvider::TextItemStyleController::SetSelected(bool value)
				{
					backgroundButton->SetSelected(value);
				}

				bool TextItemStyleProvider::TextItemStyleController::GetChecked()
				{
					return bulletButton?bulletButton->GetSelected():false;
				}

				void TextItemStyleProvider::TextItemStyleController::SetChecked(bool value)
				{
					if(bulletButton) bulletButton->SetSelected(value);
				}
				
				const WString& TextItemStyleProvider::TextItemStyleController::GetText()
				{
					return textElement->GetText();
				}

				void TextItemStyleProvider::TextItemStyleController::SetText(const WString& value)
				{
					textElement->SetText(value);
				}

/***********************************************************************
TextItemStyleProvider
***********************************************************************/

				void TextItemStyleProvider::OnStyleCheckedChanged(TextItemStyleController* style)
				{
					int index=listControl->GetArranger()->GetVisibleIndex(style);
					if(index!=-1)
					{
						textItemProvider->SetCheckedSilently(index, style->GetChecked());
					}
				}

				TextItemStyleProvider::TextItemStyleProvider(ITextItemStyleProvider* _textItemStyleProvider)
					:textItemStyleProvider(_textItemStyleProvider)
					,textItemProvider(0)
					,listControl(0)
				{
				}

				TextItemStyleProvider::~TextItemStyleProvider()
				{
				}

				void TextItemStyleProvider::AttachListControl(GuiListControl* value)
				{
					listControl=value;
					textItemProvider=dynamic_cast<TextItemProvider*>(value->GetItemProvider());
				}

				void TextItemStyleProvider::DetachListControl()
				{
					listControl=0;
				}

				int TextItemStyleProvider::GetItemStyleId(int itemIndex)
				{
					return 0;
				}

				GuiListControl::IItemStyleController* TextItemStyleProvider::CreateItemStyle(int styleId)
				{
					return new TextItemStyleController(this);
				}

				void TextItemStyleProvider::DestroyItemStyle(GuiListControl::IItemStyleController* style)
				{
					delete dynamic_cast<TextItemStyleController*>(style);
				}

				void TextItemStyleProvider::Install(GuiListControl::IItemStyleController* style, int itemIndex)
				{
					TextItemStyleController* textStyle=dynamic_cast<TextItemStyleController*>(style);
					const TextItem& item=textItemProvider->Get(itemIndex);
					textStyle->SetText(item.GetText());
					textStyle->SetChecked(item.GetChecked());
				}

				void TextItemStyleProvider::SetStyleSelected(GuiListControl::IItemStyleController* style, bool value)
				{
					TextItemStyleController* textStyle=dynamic_cast<TextItemStyleController*>(style);
					textStyle->SetSelected(value);
				}
			}

/***********************************************************************
GuiTextList
***********************************************************************/

			GuiTextList::GuiTextList(IStyleProvider* _styleProvider, list::TextItemStyleProvider::ITextItemStyleProvider* _itemStyleProvider)
				:GuiSelectableListControl(_styleProvider, new list::TextItemProvider)
				,items(0)
			{
				items=dynamic_cast<list::TextItemProvider*>(itemProvider.Obj());
				ChangeItemStyle(_itemStyleProvider);
				SetArranger(new list::FixedHeightItemArranger);
			}

			GuiTextList::~GuiTextList()
			{
			}

			Ptr<GuiListControl::IItemStyleProvider> GuiTextList::SetStyleProvider(Ptr<GuiListControl::IItemStyleProvider> value)
			{
				if(value.Cast<list::TextItemStyleProvider>())
				{
					return GuiSelectableListControl::SetStyleProvider(value);
				}
				else
				{
					return 0;
				}
			}

			Ptr<GuiListControl::IItemStyleProvider> GuiTextList::ChangeItemStyle(list::TextItemStyleProvider::ITextItemStyleProvider* itemStyleProvider)
			{
				if(itemStyleProvider)
				{
					return SetStyleProvider(new list::TextItemStyleProvider(itemStyleProvider));
				}
				else
				{
					return 0;
				}
			}

			list::TextItemProvider& GuiTextList::GetItems()
			{
				return *items;
			}
		}
	}
}