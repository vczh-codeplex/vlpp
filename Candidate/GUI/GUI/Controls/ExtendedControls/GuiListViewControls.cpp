#include "GuiListViewControls.h"

namespace vl
{
	namespace presentation
	{
		namespace controls
		{
			using namespace elements;

			namespace list
			{

/***********************************************************************
ListViewItemStyleProviderBase::TextItemStyleController
***********************************************************************/

				ListViewItemStyleProviderBase::ListViewItemStyleController::ListViewItemStyleController(ListViewItemStyleProviderBase* provider)
					:ItemStyleControllerBase(provider, 0)
					,backgroundButton(0)
					,listViewItemStyleProvider(provider)
				{
					backgroundButton=new GuiSelectableButton(listViewItemStyleProvider->listControl->GetListViewStyleProvider()->CreateItemBackground());
					backgroundButton->SetAutoSelection(false);
					Initialize(backgroundButton->GetBoundsComposition(), backgroundButton);
				}

				ListViewItemStyleProviderBase::ListViewItemStyleController::~ListViewItemStyleController()
				{
				}

				bool ListViewItemStyleProviderBase::ListViewItemStyleController::GetSelected()
				{
					return backgroundButton->GetSelected();
				}

				void ListViewItemStyleProviderBase::ListViewItemStyleController::SetSelected(bool value)
				{
					backgroundButton->SetSelected(value);
				}

/***********************************************************************
ListViewItemStyleProviderBase
***********************************************************************/

				ListViewItemStyleProviderBase::ListViewItemStyleProviderBase()
					:listControl(0)
				{
				}

				ListViewItemStyleProviderBase::~ListViewItemStyleProviderBase()
				{
				}

				void ListViewItemStyleProviderBase::AttachListControl(GuiListControl* value)
				{
					listControl=dynamic_cast<GuiListViewBase*>(value);
				}

				void ListViewItemStyleProviderBase::DetachListControl()
				{
					listControl=0;
				}

				int ListViewItemStyleProviderBase::GetItemStyleId(int itemIndex)
				{
					return 0;
				}

				void ListViewItemStyleProviderBase::SetStyleSelected(GuiListControl::IItemStyleController* style, bool value)
				{
					ListViewItemStyleController* textStyle=dynamic_cast<ListViewItemStyleController*>(style);
					textStyle->SetSelected(value);
				}
			}

/***********************************************************************
GuiListViewBase
***********************************************************************/

			GuiListViewBase::GuiListViewBase(IStyleProvider* _styleProvider, GuiListControl::IItemProvider* _itemProvider)
				:GuiSelectableListControl(_styleProvider, _itemProvider)
				,styleProvider(0)
			{
				styleProvider=dynamic_cast<IStyleProvider*>(styleController->GetStyleProvider());
			}

			GuiListViewBase::~GuiListViewBase()
			{
			}

			GuiListViewBase::IStyleProvider* GuiListViewBase::GetListViewStyleProvider()
			{
				return styleProvider;
			}

			Ptr<GuiListControl::IItemStyleProvider> GuiListViewBase::SetStyleProvider(Ptr<GuiListControl::IItemStyleProvider> value)
			{
				if(value.Cast<list::ListViewItemStyleProvider>())
				{
					return GuiSelectableListControl::SetStyleProvider(value);
				}
				else
				{
					return 0;
				}
			}

			namespace list
			{

/***********************************************************************
ListViewItemStyleProvider::ListViewContentItemStyleController
***********************************************************************/

				ListViewItemStyleProvider::ListViewContentItemStyleController::ListViewContentItemStyleController(ListViewItemStyleProvider* provider)
					:ListViewItemStyleController(provider)
					,listViewItemStyleProvider(provider)
				{
					content=listViewItemStyleProvider->listViewItemContentProvider->CreateItemContent(backgroundButton->GetFont());
					GuiBoundsComposition* composition=content->GetContentComposition();
					composition->SetAlignmentToParent(Margin(0, 0, 0, 0));
					backgroundButton->GetContainerComposition()->AddChild(composition);

					GuiBoundsComposition* decorator=content->GetBackgroundDecorator();
					if(decorator)
					{
						backgroundButton->GetBoundsComposition()->AddChild(decorator);
						backgroundButton->GetBoundsComposition()->MoveChild(decorator, 0);
					}
				}

				ListViewItemStyleProvider::ListViewContentItemStyleController::~ListViewContentItemStyleController()
				{
				}

				ListViewItemStyleProvider::IListViewItemContent* ListViewItemStyleProvider::ListViewContentItemStyleController::GetItemContent()
				{
					return content.Obj();
				}

				void ListViewItemStyleProvider::ListViewContentItemStyleController::Install(IListViewItemView* view, int itemIndex)
				{
					content->Install(listViewItemStyleProvider->listControl->GetListViewStyleProvider(), view, itemIndex);
				}

/***********************************************************************
ListViewItemStyleProvider
***********************************************************************/

				const wchar_t* ListViewItemStyleProvider::IListViewItemView::Identifier = L"vl::presentation::controls::list::ListViewItemStyleProvider::IListViewItemView";

				ListViewItemStyleProvider::ListViewItemStyleProvider(IListViewItemContentProvider* itemContentProvider)
					:listViewItemView(0)
					,listViewItemContentProvider(itemContentProvider)
				{
				}

				ListViewItemStyleProvider::~ListViewItemStyleProvider()
				{
				}

				void ListViewItemStyleProvider::AttachListControl(GuiListControl* value)
				{
					ListViewItemStyleProviderBase::AttachListControl(value);
					listViewItemView=dynamic_cast<IListViewItemView*>(value->GetItemProvider()->RequestView(IListViewItemView::Identifier));
					listViewItemContentProvider->AttachListControl(value);
				}

				void ListViewItemStyleProvider::DetachListControl()
				{
					listViewItemContentProvider->DetachListControl();
					listControl->GetItemProvider()->ReleaseView(listViewItemView);
					listViewItemView=0;
					ListViewItemStyleProviderBase::DetachListControl();
				}

				GuiListControl::IItemStyleController* ListViewItemStyleProvider::CreateItemStyle(int styleId)
				{
					ListViewContentItemStyleController* itemStyle=new ListViewContentItemStyleController(this);
					itemStyles.Add(itemStyle);
					return itemStyle;
				}

				void ListViewItemStyleProvider::DestroyItemStyle(GuiListControl::IItemStyleController* style)
				{
					ListViewContentItemStyleController* itemStyle=dynamic_cast<ListViewContentItemStyleController*>(style);
					if(itemStyle)
					{
						itemStyles.Remove(itemStyle);
						delete itemStyle;
					}
				}

				void ListViewItemStyleProvider::Install(GuiListControl::IItemStyleController* style, int itemIndex)
				{
					ListViewContentItemStyleController* itemStyle=dynamic_cast<ListViewContentItemStyleController*>(style);
					itemStyle->Install(listViewItemView, itemIndex);
				}

				const ListViewItemStyleProvider::IItemStyleList& ListViewItemStyleProvider::GetCreatedItemStyles()
				{
					return itemStyles.Wrap();
				}

				bool ListViewItemStyleProvider::IsItemStyleAttachedToListView(GuiListControl::IItemStyleController* itemStyle)
				{
					return itemStyle && itemStyle->GetBoundsComposition()->GetParent();
				}
				
/***********************************************************************
ListViewBigIconContentProvider
***********************************************************************/

				ListViewBigIconContentProvider::ItemContent::ItemContent(Size iconSize, const FontProperties& font)
					:contentComposition(0)
				{
					contentComposition=new GuiBoundsComposition;
					contentComposition->SetMinSizeLimitation(GuiGraphicsComposition::LimitToElementAndChildren);

					GuiTableComposition* table=new GuiTableComposition;
					contentComposition->AddChild(table);
					table->SetRowsAndColumns(2, 3);
					table->SetRowOption(0, GuiCellOption::MinSizeOption());
					table->SetRowOption(1, GuiCellOption::MinSizeOption());
					table->SetColumnOption(0, GuiCellOption::PercentageOption(0.5));
					table->SetColumnOption(1, GuiCellOption::MinSizeOption());
					table->SetColumnOption(2, GuiCellOption::PercentageOption(0.5));
					table->SetAlignmentToParent(Margin(0, 0, 0, 0));
					table->SetCellPadding(5);
					{
						GuiCellComposition* cell=new GuiCellComposition;
						table->AddChild(cell);
						cell->SetSite(0, 1, 1, 1);
						cell->SetPreferredMinSize(iconSize);

						image=GuiImageFrameElement::Create();
						image->SetStretch(true);
						cell->SetOwnedElement(image);
					}
					{
						GuiCellComposition* cell=new GuiCellComposition;
						table->AddChild(cell);
						cell->SetMinSizeLimitation(GuiGraphicsComposition::NoLimit);
						cell->SetSite(1, 0, 1, 3);
						cell->SetPreferredMinSize(Size(64, 40));

						text=GuiSolidLabelElement::Create();
						text->SetAlignments(Alignment::Center, Alignment::Top);
						text->SetFont(font);
						text->SetWrapLine(true);
						text->SetEllipse(true);
						cell->SetOwnedElement(text);
					}
				}

				ListViewBigIconContentProvider::ItemContent::~ItemContent()
				{
				}

				elements::GuiBoundsComposition* ListViewBigIconContentProvider::ItemContent::GetContentComposition()
				{
					return contentComposition;
				}

				elements::GuiBoundsComposition* ListViewBigIconContentProvider::ItemContent::GetBackgroundDecorator()
				{
					return 0;
				}

				void ListViewBigIconContentProvider::ItemContent::Install(GuiListViewBase::IStyleProvider* styleProvider, ListViewItemStyleProvider::IListViewItemView* view, int itemIndex)
				{
					Ptr<GuiImageData> imageData=view->GetLargeImage(itemIndex);
					if(imageData)
					{
						image->SetImage(imageData->GetImage(), imageData->GetFrameIndex());
					}
					else
					{
						image->SetImage(0);
					}
					text->SetText(view->GetText(itemIndex));
					text->SetColor(styleProvider->GetPrimaryTextColor());
				}

				ListViewBigIconContentProvider::ListViewBigIconContentProvider(Size _iconSize)
					:iconSize(_iconSize)
				{
				}

				ListViewBigIconContentProvider::~ListViewBigIconContentProvider()
				{
				}

				GuiListControl::IItemCoordinateTransformer* ListViewBigIconContentProvider::CreatePreferredCoordinateTransformer()
				{
					return new DefaultItemCoordinateTransformer;
				}

				GuiListControl::IItemArranger* ListViewBigIconContentProvider::CreatePreferredArranger()
				{
					return new FixedSizeMultiColumnItemArranger;
				}

				ListViewItemStyleProvider::IListViewItemContent* ListViewBigIconContentProvider::CreateItemContent(const FontProperties& font)
				{
					return new ItemContent(iconSize, font);
				}

				void ListViewBigIconContentProvider::AttachListControl(GuiListControl* value)
				{
				}

				void ListViewBigIconContentProvider::DetachListControl()
				{
				}
				
/***********************************************************************
ListViewSmallIconContentProvider
***********************************************************************/

				ListViewSmallIconContentProvider::ItemContent::ItemContent(Size iconSize, const FontProperties& font)
					:contentComposition(0)
				{
					contentComposition=new GuiBoundsComposition;
					contentComposition->SetMinSizeLimitation(GuiGraphicsComposition::LimitToElementAndChildren);

					GuiTableComposition* table=new GuiTableComposition;
					contentComposition->AddChild(table);
					table->SetRowsAndColumns(3, 2);
					table->SetRowOption(0, GuiCellOption::PercentageOption(0.5));
					table->SetRowOption(1, GuiCellOption::MinSizeOption());
					table->SetRowOption(2, GuiCellOption::PercentageOption(0.5));
					table->SetColumnOption(0, GuiCellOption::MinSizeOption());
					table->SetColumnOption(1, GuiCellOption::MinSizeOption());
					table->SetAlignmentToParent(Margin(0, 0, 0, 0));
					table->SetCellPadding(2);
					{
						GuiCellComposition* cell=new GuiCellComposition;
						table->AddChild(cell);
						cell->SetSite(1, 0, 1, 1);
						cell->SetPreferredMinSize(iconSize);

						image=GuiImageFrameElement::Create();
						image->SetStretch(true);
						cell->SetOwnedElement(image);
					}
					{
						GuiCellComposition* cell=new GuiCellComposition;
						table->AddChild(cell);
						cell->SetSite(0, 1, 3, 1);
						cell->SetPreferredMinSize(Size(192, 0));

						text=GuiSolidLabelElement::Create();
						text->SetAlignments(Alignment::Left, Alignment::Center);
						text->SetFont(font);
						text->SetEllipse(true);
						cell->SetOwnedElement(text);
					}
				}

				ListViewSmallIconContentProvider::ItemContent::~ItemContent()
				{
				}

				elements::GuiBoundsComposition* ListViewSmallIconContentProvider::ItemContent::GetContentComposition()
				{
					return contentComposition;
				}

				elements::GuiBoundsComposition* ListViewSmallIconContentProvider::ItemContent::GetBackgroundDecorator()
				{
					return 0;
				}

				void ListViewSmallIconContentProvider::ItemContent::Install(GuiListViewBase::IStyleProvider* styleProvider, ListViewItemStyleProvider::IListViewItemView* view, int itemIndex)
				{
					Ptr<GuiImageData> imageData=view->GetSmallImage(itemIndex);
					if(imageData)
					{
						image->SetImage(imageData->GetImage(), imageData->GetFrameIndex());
					}
					else
					{
						image->SetImage(0);
					}
					text->SetText(view->GetText(itemIndex));
					text->SetColor(styleProvider->GetPrimaryTextColor());
				}

				ListViewSmallIconContentProvider::ListViewSmallIconContentProvider(Size _iconSize)
					:iconSize(_iconSize)
				{
				}

				ListViewSmallIconContentProvider::~ListViewSmallIconContentProvider()
				{
				}

				GuiListControl::IItemCoordinateTransformer* ListViewSmallIconContentProvider::CreatePreferredCoordinateTransformer()
				{
					return new DefaultItemCoordinateTransformer;
				}

				GuiListControl::IItemArranger* ListViewSmallIconContentProvider::CreatePreferredArranger()
				{
					return new FixedSizeMultiColumnItemArranger;
				}

				ListViewItemStyleProvider::IListViewItemContent* ListViewSmallIconContentProvider::CreateItemContent(const FontProperties& font)
				{
					return new ItemContent(iconSize, font);
				}

				void ListViewSmallIconContentProvider::AttachListControl(GuiListControl* value)
				{
				}

				void ListViewSmallIconContentProvider::DetachListControl()
				{
				}
				
/***********************************************************************
ListViewListContentProvider
***********************************************************************/

				ListViewListContentProvider::ItemContent::ItemContent(Size iconSize, const FontProperties& font)
					:contentComposition(0)
				{
					contentComposition=new GuiBoundsComposition;
					contentComposition->SetMinSizeLimitation(GuiGraphicsComposition::LimitToElementAndChildren);

					GuiTableComposition* table=new GuiTableComposition;
					contentComposition->AddChild(table);
					table->SetRowsAndColumns(3, 2);
					table->SetRowOption(0, GuiCellOption::PercentageOption(0.5));
					table->SetRowOption(1, GuiCellOption::MinSizeOption());
					table->SetRowOption(2, GuiCellOption::PercentageOption(0.5));
					table->SetColumnOption(0, GuiCellOption::MinSizeOption());
					table->SetColumnOption(1, GuiCellOption::MinSizeOption());
					table->SetAlignmentToParent(Margin(0, 0, 0, 0));
					table->SetCellPadding(2);
					{
						GuiCellComposition* cell=new GuiCellComposition;
						table->AddChild(cell);
						cell->SetSite(1, 0, 1, 1);
						cell->SetPreferredMinSize(iconSize);

						image=GuiImageFrameElement::Create();
						image->SetStretch(true);
						cell->SetOwnedElement(image);
					}
					{
						GuiCellComposition* cell=new GuiCellComposition;
						table->AddChild(cell);
						cell->SetSite(0, 1, 3, 1);
						cell->SetMargin(Margin(0, 0, 16, 0));

						text=GuiSolidLabelElement::Create();
						text->SetAlignments(Alignment::Left, Alignment::Center);
						text->SetFont(font);
						cell->SetOwnedElement(text);
					}
				}

				ListViewListContentProvider::ItemContent::~ItemContent()
				{
				}

				elements::GuiBoundsComposition* ListViewListContentProvider::ItemContent::GetContentComposition()
				{
					return contentComposition;
				}

				elements::GuiBoundsComposition* ListViewListContentProvider::ItemContent::GetBackgroundDecorator()
				{
					return 0;
				}

				void ListViewListContentProvider::ItemContent::Install(GuiListViewBase::IStyleProvider* styleProvider, ListViewItemStyleProvider::IListViewItemView* view, int itemIndex)
				{
					Ptr<GuiImageData> imageData=view->GetSmallImage(itemIndex);
					if(imageData)
					{
						image->SetImage(imageData->GetImage(), imageData->GetFrameIndex());
					}
					else
					{
						image->SetImage(0);
					}
					text->SetText(view->GetText(itemIndex));
					text->SetColor(styleProvider->GetPrimaryTextColor());
				}

				ListViewListContentProvider::ListViewListContentProvider(Size _iconSize)
					:iconSize(_iconSize)
				{
				}

				ListViewListContentProvider::~ListViewListContentProvider()
				{
				}

				GuiListControl::IItemCoordinateTransformer* ListViewListContentProvider::CreatePreferredCoordinateTransformer()
				{
					return new DefaultItemCoordinateTransformer;
				}

				GuiListControl::IItemArranger* ListViewListContentProvider::CreatePreferredArranger()
				{
					return new FixedHeightMultiColumnItemArranger;
				}

				ListViewItemStyleProvider::IListViewItemContent* ListViewListContentProvider::CreateItemContent(const FontProperties& font)
				{
					return new ItemContent(iconSize, font);
				}

				void ListViewListContentProvider::AttachListControl(GuiListControl* value)
				{
				}

				void ListViewListContentProvider::DetachListControl()
				{
				}
				
/***********************************************************************
ListViewTileContentProvider
***********************************************************************/

				void ListViewTileContentProvider::ItemContent::RemoveTextElement(int textRow)
				{
					GuiCellComposition* cell=textTable->GetSitedCell(textRow+1, 0);
					textTable->RemoveChild(cell);
					delete cell;
				}

				elements::GuiSolidLabelElement* ListViewTileContentProvider::ItemContent::CreateTextElement(int textRow, const FontProperties& font)
				{
					GuiCellComposition* cell=new GuiCellComposition;
					textTable->AddChild(cell);
					cell->SetSite(textRow+1, 0, 1, 1);

					elements::GuiSolidLabelElement* textElement=GuiSolidLabelElement::Create();
					textElement->SetAlignments(Alignment::Left, Alignment::Center);
					textElement->SetFont(font);
					textElement->SetEllipse(true);
					cell->SetOwnedElement(textElement);
					return textElement;
				}

				void ListViewTileContentProvider::ItemContent::ResetTextTable(int textRows)
				{
					textTable->SetRowsAndColumns(textRows+2, 1);
					textTable->SetRowOption(0, GuiCellOption::PercentageOption(0.5));
					for(int i=0;i<textRows;i++)
					{
						textTable->SetRowOption(i+1, GuiCellOption::MinSizeOption());
					}
					textTable->SetRowOption(textRows+1, GuiCellOption::PercentageOption(0.5));
					textTable->SetColumnOption(0, GuiCellOption::PercentageOption(1.0));
				}

				ListViewTileContentProvider::ItemContent::ItemContent(Size iconSize, const FontProperties& font)
					:contentComposition(0)
				{
					contentComposition=new GuiBoundsComposition;
					contentComposition->SetMinSizeLimitation(GuiGraphicsComposition::LimitToElementAndChildren);

					GuiTableComposition* table=new GuiTableComposition;
					contentComposition->AddChild(table);
					table->SetRowsAndColumns(3, 2);
					table->SetRowOption(0, GuiCellOption::PercentageOption(0.5));
					table->SetRowOption(1, GuiCellOption::MinSizeOption());
					table->SetRowOption(2, GuiCellOption::PercentageOption(0.5));
					table->SetColumnOption(0, GuiCellOption::MinSizeOption());
					table->SetColumnOption(1, GuiCellOption::MinSizeOption());
					table->SetAlignmentToParent(Margin(0, 0, 0, 0));
					table->SetCellPadding(4);
					{
						GuiCellComposition* cell=new GuiCellComposition;
						table->AddChild(cell);
						cell->SetSite(1, 0, 1, 1);
						cell->SetPreferredMinSize(iconSize);

						image=GuiImageFrameElement::Create();
						image->SetStretch(true);
						cell->SetOwnedElement(image);
					}
					{
						GuiCellComposition* cell=new GuiCellComposition;
						table->AddChild(cell);
						cell->SetSite(0, 1, 3, 1);
						cell->SetPreferredMinSize(Size(224, 0));

						textTable=new GuiTableComposition;
						textTable->SetCellPadding(1);
						ResetTextTable(1);
						textTable->SetAlignmentToParent(Margin(0, 0, 0, 0));
						cell->AddChild(textTable);
						{
							text=CreateTextElement(0, font);
						}
					}
				}

				ListViewTileContentProvider::ItemContent::~ItemContent()
				{
				}

				elements::GuiBoundsComposition* ListViewTileContentProvider::ItemContent::GetContentComposition()
				{
					return contentComposition;
				}

				elements::GuiBoundsComposition* ListViewTileContentProvider::ItemContent::GetBackgroundDecorator()
				{
					return 0;
				}

				void ListViewTileContentProvider::ItemContent::Install(GuiListViewBase::IStyleProvider* styleProvider, ListViewItemStyleProvider::IListViewItemView* view, int itemIndex)
				{
					Ptr<GuiImageData> imageData=view->GetLargeImage(itemIndex);
					if(imageData)
					{
						image->SetImage(imageData->GetImage(), imageData->GetFrameIndex());
					}
					else
					{
						image->SetImage(0);
					}
					text->SetText(view->GetText(itemIndex));
					text->SetColor(styleProvider->GetPrimaryTextColor());

					for(int i=0;i<dataTexts.Count();i++)
					{
						RemoveTextElement(i+1);
					}
					int dataColumnCount=view->GetDataColumnCount();
					ResetTextTable(dataColumnCount+1);
					dataTexts.Resize(dataColumnCount);
					for(int i=0;i<dataColumnCount;i++)
					{
						dataTexts[i]=CreateTextElement(i+1, text->GetFont());
						dataTexts[i]->SetText(view->GetSubItem(itemIndex, view->GetDataColumn(i)));
						dataTexts[i]->SetColor(styleProvider->GetSecondaryTextColor());
					}
				}

				ListViewTileContentProvider::ListViewTileContentProvider(Size _iconSize)
					:iconSize(_iconSize)
				{
				}

				ListViewTileContentProvider::~ListViewTileContentProvider()
				{
				}

				GuiListControl::IItemCoordinateTransformer* ListViewTileContentProvider::CreatePreferredCoordinateTransformer()
				{
					return new DefaultItemCoordinateTransformer;
				}

				GuiListControl::IItemArranger* ListViewTileContentProvider::CreatePreferredArranger()
				{
					return new FixedSizeMultiColumnItemArranger;
				}

				ListViewItemStyleProvider::IListViewItemContent* ListViewTileContentProvider::CreateItemContent(const FontProperties& font)
				{
					return new ItemContent(iconSize, font);
				}

				void ListViewTileContentProvider::AttachListControl(GuiListControl* value)
				{
				}

				void ListViewTileContentProvider::DetachListControl()
				{
				}
				
/***********************************************************************
ListViewInformationContentProvider
***********************************************************************/

				ListViewInformationContentProvider::ItemContent::ItemContent(Size iconSize, const FontProperties& font)
					:contentComposition(0)
					,baselineFont(font)
				{
					contentComposition=new GuiBoundsComposition;
					contentComposition->SetMinSizeLimitation(GuiGraphicsComposition::LimitToElementAndChildren);
					{
						bottomLine=GuiSolidBackgroundElement::Create();
						bottomLineComposition=new GuiBoundsComposition;
						bottomLineComposition->SetOwnedElement(bottomLine);
						bottomLineComposition->SetAlignmentToParent(Margin(8, -1, 8, 0));
						bottomLineComposition->SetPreferredMinSize(Size(0, 1));
					}

					GuiTableComposition* table=new GuiTableComposition;
					contentComposition->AddChild(table);
					table->SetRowsAndColumns(3, 3);
					table->SetRowOption(0, GuiCellOption::PercentageOption(0.5));
					table->SetRowOption(1, GuiCellOption::MinSizeOption());
					table->SetRowOption(2, GuiCellOption::PercentageOption(0.5));
					table->SetColumnOption(0, GuiCellOption::MinSizeOption());
					table->SetColumnOption(1, GuiCellOption::PercentageOption(1.0));
					table->SetColumnOption(2, GuiCellOption::MinSizeOption());
					table->SetAlignmentToParent(Margin(0, 0, 0, 0));
					table->SetCellPadding(4);
					{
						GuiCellComposition* cell=new GuiCellComposition;
						table->AddChild(cell);
						cell->SetSite(1, 0, 1, 1);
						cell->SetPreferredMinSize(iconSize);

						image=GuiImageFrameElement::Create();
						image->SetStretch(true);
						cell->SetOwnedElement(image);
					}
					{
						GuiCellComposition* cell=new GuiCellComposition;
						table->AddChild(cell);
						cell->SetSite(0, 1, 3, 1);

						FontProperties textFont=font;
						textFont.size=(int)(textFont.size*1.2);

						text=GuiSolidLabelElement::Create();
						text->SetFont(textFont);
						text->SetEllipse(true);
						cell->SetOwnedElement(text);
					}
					{
						GuiCellComposition* cell=new GuiCellComposition;
						table->AddChild(cell);
						cell->SetSite(0, 2, 3, 1);
						cell->SetPreferredMinSize(Size(224, 0));

						textTable=new GuiTableComposition;
						textTable->SetCellPadding(4);
						textTable->SetAlignmentToParent(Margin(0, 0, 0, 0));
						cell->AddChild(textTable);
					}
				}

				ListViewInformationContentProvider::ItemContent::~ItemContent()
				{
				}

				elements::GuiBoundsComposition* ListViewInformationContentProvider::ItemContent::GetContentComposition()
				{
					return contentComposition;
				}

				elements::GuiBoundsComposition* ListViewInformationContentProvider::ItemContent::GetBackgroundDecorator()
				{
					return bottomLineComposition;
				}

				void ListViewInformationContentProvider::ItemContent::Install(GuiListViewBase::IStyleProvider* styleProvider, ListViewItemStyleProvider::IListViewItemView* view, int itemIndex)
				{
					Ptr<GuiImageData> imageData=view->GetLargeImage(itemIndex);
					if(imageData)
					{
						image->SetImage(imageData->GetImage(), imageData->GetFrameIndex());
					}
					else
					{
						image->SetImage(0);
					}
					text->SetText(view->GetText(itemIndex));
					text->SetColor(styleProvider->GetPrimaryTextColor());
					bottomLine->SetColor(styleProvider->GetItemSeparatorColor());

					for(int i=0;i<dataTexts.Count();i++)
					{
						GuiCellComposition* cell=textTable->GetSitedCell(i, 0);
						textTable->RemoveChild(cell);
						delete cell;
					}

					int dataColumnCount=view->GetDataColumnCount();
					dataTexts.Resize(dataColumnCount);
					textTable->SetRowsAndColumns(dataColumnCount, 1);
					textTable->SetColumnOption(0, GuiCellOption::PercentageOption(1.0));
					for(int i=0;i<dataColumnCount;i++)
					{
						textTable->SetRowOption(i, GuiCellOption::MinSizeOption());
					}
					
					for(int i=0;i<dataColumnCount;i++)
					{
						GuiCellComposition* cell=new GuiCellComposition;
						textTable->AddChild(cell);
						cell->SetSite(i, 0, 1, 1);

						GuiTableComposition* dataTable=new GuiTableComposition;
						dataTable->SetRowsAndColumns(1, 2);
						dataTable->SetRowOption(0, GuiCellOption::MinSizeOption());
						dataTable->SetColumnOption(0, GuiCellOption::MinSizeOption());
						dataTable->SetColumnOption(1, GuiCellOption::PercentageOption(1.0));
						dataTable->SetAlignmentToParent(Margin(0, 0, 0, 0));
						cell->AddChild(dataTable);
						{
							GuiCellComposition* cell=new GuiCellComposition;
							dataTable->AddChild(cell);
							cell->SetSite(0, 0, 1, 1);

							GuiSolidLabelElement* textColumn=GuiSolidLabelElement::Create();
							textColumn->SetFont(baselineFont);
							textColumn->SetText(view->GetColumnText(view->GetDataColumn(i)+1)+L": ");
							textColumn->SetColor(styleProvider->GetSecondaryTextColor());
							cell->SetOwnedElement(textColumn);
						}
						{
							GuiCellComposition* cell=new GuiCellComposition;
							dataTable->AddChild(cell);
							cell->SetSite(0, 1, 1, 1);

							GuiSolidLabelElement* textData=GuiSolidLabelElement::Create();
							textData->SetFont(baselineFont);
							textData->SetEllipse(true);
							textData->SetText(view->GetSubItem(itemIndex, view->GetDataColumn(i)));
							textData->SetColor(styleProvider->GetPrimaryTextColor());
							cell->SetOwnedElement(textData);
						}
					}
				}

				ListViewInformationContentProvider::ListViewInformationContentProvider(Size _iconSize)
					:iconSize(_iconSize)
				{
				}

				ListViewInformationContentProvider::~ListViewInformationContentProvider()
				{
				}

				GuiListControl::IItemCoordinateTransformer* ListViewInformationContentProvider::CreatePreferredCoordinateTransformer()
				{
					return new DefaultItemCoordinateTransformer;
				}

				GuiListControl::IItemArranger* ListViewInformationContentProvider::CreatePreferredArranger()
				{
					return new FixedHeightItemArranger;
				}

				ListViewItemStyleProvider::IListViewItemContent* ListViewInformationContentProvider::CreateItemContent(const FontProperties& font)
				{
					return new ItemContent(iconSize, font);
				}

				void ListViewInformationContentProvider::AttachListControl(GuiListControl* value)
				{
				}

				void ListViewInformationContentProvider::DetachListControl()
				{
				}
				
/***********************************************************************
ListViewColumnItemArranger::ColumnItemViewCallback
***********************************************************************/

				ListViewColumnItemArranger::ColumnItemViewCallback::ColumnItemViewCallback(ListViewColumnItemArranger* _arranger)
					:arranger(_arranger)
				{
				}

				ListViewColumnItemArranger::ColumnItemViewCallback::~ColumnItemViewCallback()
				{
				}

				void ListViewColumnItemArranger::ColumnItemViewCallback::OnColumnChanged()
				{
					arranger->RebuildColumns();
				}

				void ListViewColumnItemArranger::ColumnItemViewCallback::OnColumnSizeChanged(int index)
				{
					arranger->UpdateColumnSize(index);
				}
				
/***********************************************************************
ListViewColumnItemArranger
***********************************************************************/

				const wchar_t* ListViewColumnItemArranger::IColumnItemView::Identifier = L"vl::presentation::controls::list::ListViewColumnItemArranger::IColumnItemView";

				void ListViewColumnItemArranger::ColumnHeaderSplitterLeftButtonDown(elements::GuiGraphicsComposition* sender, elements::GuiMouseEventArgs& arguments)
				{
					if(listView->GetVisuallyEnabled())
					{
						arguments.handled=true;
						splitterDragging=true;
						splitterLatestX=arguments.x;
					}
				}

				void ListViewColumnItemArranger::ColumnHeaderSplitterLeftButtonUp(elements::GuiGraphicsComposition* sender, elements::GuiMouseEventArgs& arguments)
				{
					if(listView->GetVisuallyEnabled())
					{
						arguments.handled=true;
						splitterDragging=false;
						splitterLatestX=0;
					}
				}

				void ListViewColumnItemArranger::ColumnHeaderSplitterMouseMove(elements::GuiGraphicsComposition* sender, elements::GuiMouseEventArgs& arguments)
				{
					if(splitterDragging)
					{
						int offset=arguments.x-splitterLatestX;
						int index=columnHeaderSplitters.IndexOf(dynamic_cast<GuiBoundsComposition*>(sender));
						if(index!=-1)
						{
							GuiBoundsComposition* buttonBounds=columnHeaderButtons[index]->GetBoundsComposition();
							Rect bounds=buttonBounds->GetBounds();
							Rect newBounds(bounds.LeftTop(), Size(bounds.Width()+offset, bounds.Height()));
							buttonBounds->SetBounds(newBounds);

							int finalSize=buttonBounds->GetBounds().Width();
							columnItemView->SetColumnSize(index, finalSize);
						}
					}
				}

				void ListViewColumnItemArranger::RearrangeItemBounds()
				{
					FixedHeightItemArranger::RearrangeItemBounds();
					int count=columnHeaders->GetParent()->Children().Count();
					columnHeaders->GetParent()->MoveChild(columnHeaders, count-1);
					columnHeaders->SetBounds(Rect(Point(-viewBounds.Left(), 0), Size(0, 0)));
				}

				int ListViewColumnItemArranger::GetWidth()
				{
					int width=columnHeaders->GetBounds().Width()-SplitterWidth;
					if(width<SplitterWidth)
					{
						width=SplitterWidth;
					}
					return width;
				}

				int ListViewColumnItemArranger::GetYOffset()
				{
					return columnHeaders->GetBounds().Height();
				}

				Size ListViewColumnItemArranger::OnCalculateTotalSize()
				{
					Size size=FixedHeightItemArranger::OnCalculateTotalSize();
					size.x+=SplitterWidth;
					return size;
				}

				void ListViewColumnItemArranger::DeleteColumnButtons()
				{
					for(int i=columnHeaders->GetStackItems().Count()-1;i>=0;i--)
					{
						GuiStackItemComposition* item=columnHeaders->GetStackItems()[i];
						columnHeaders->RemoveChild(item);

						GuiControl* button=item->Children()[0]->GetAssociatedControl();
						if(button)
						{
							item->RemoveChild(button->GetBoundsComposition());
							delete button;
						}
						delete item;
					}
					columnHeaderButtons.Clear();
					columnHeaderSplitters.Clear();
				}

				void ListViewColumnItemArranger::RebuildColumns()
				{
					DeleteColumnButtons();
					if(columnItemView)
					{
						for(int i=0;i<columnItemView->GetColumnCount();i++)
						{
							GuiBoundsComposition* splitterComposition=new GuiBoundsComposition;
							splitterComposition->SetAlignmentToParent(Margin(0, 0, 0, 0));
							splitterComposition->SetAssociatedCursor(GetCurrentController()->GetSystemCursor(INativeCursor::SizeWE));
							splitterComposition->SetAlignmentToParent(Margin(0, 0, -1, 0));
							splitterComposition->SetPreferredMinSize(Size(SplitterWidth, 0));
							columnHeaderSplitters.Add(splitterComposition);

							splitterComposition->GetEventReceiver()->leftButtonDown.AttachMethod(this, &ListViewColumnItemArranger::ColumnHeaderSplitterLeftButtonDown);
							splitterComposition->GetEventReceiver()->leftButtonUp.AttachMethod(this, &ListViewColumnItemArranger::ColumnHeaderSplitterLeftButtonUp);
							splitterComposition->GetEventReceiver()->mouseMove.AttachMethod(this, &ListViewColumnItemArranger::ColumnHeaderSplitterMouseMove);
						}
						for(int i=0;i<columnItemView->GetColumnCount();i++)
						{
							GuiButton* button=new GuiButton(styleProvider->CreateColumnStyle());
							button->SetText(columnItemView->GetColumnText(i));
							button->GetBoundsComposition()->SetBounds(Rect(Point(0, 0), Size(columnItemView->GetColumnSize(i), 0)));
							columnHeaderButtons.Add(button);
							if(i>0)
							{
								button->GetContainerComposition()->AddChild(columnHeaderSplitters[i-1]);
							}

							GuiStackItemComposition* item=new GuiStackItemComposition;
							item->SetMinSizeLimitation(GuiGraphicsComposition::LimitToElementAndChildren);
							item->AddChild(button->GetBoundsComposition());
							columnHeaders->AddChild(item);
						}
						if(columnItemView->GetColumnCount()>0)
						{
							GuiBoundsComposition* splitterComposition=columnHeaderSplitters[columnItemView->GetColumnCount()-1];

							GuiStackItemComposition* item=new GuiStackItemComposition;
							item->SetMinSizeLimitation(GuiGraphicsComposition::LimitToElementAndChildren);
							item->AddChild(splitterComposition);
							columnHeaders->AddChild(item);
						}
					}
					callback->OnTotalSizeChanged();
				}

				void ListViewColumnItemArranger::UpdateColumnSize(int index)
				{
					if(index>=0 && index<columnHeaderButtons.Count())
					{
						int size=columnItemView->GetColumnSize(index);
						GuiBoundsComposition* buttonBounds=columnHeaderButtons[index]->GetBoundsComposition();
						Rect bounds=buttonBounds->GetBounds();
						Rect newBounds(bounds.LeftTop(), Size(size, bounds.Height()));
						buttonBounds->SetBounds(newBounds);
						callback->OnTotalSizeChanged();
					}
				}

				ListViewColumnItemArranger::ListViewColumnItemArranger()
					:listView(0)
					,styleProvider(0)
					,columnItemView(0)
					,splitterDragging(false)
					,splitterLatestX(0)
				{
					columnHeaders=new GuiStackComposition;
					columnHeaders->SetMinSizeLimitation(GuiGraphicsComposition::LimitToElementAndChildren);
					columnItemViewCallback=new ColumnItemViewCallback(this);
				}

				ListViewColumnItemArranger::~ListViewColumnItemArranger()
				{
					if(!columnHeaders->GetParent())
					{
						DeleteColumnButtons();
						delete columnHeaders;
					}
				}

				void ListViewColumnItemArranger::AttachListControl(GuiListControl* value)
				{
					FixedHeightItemArranger::AttachListControl(value);
					listView=dynamic_cast<GuiListViewBase*>(value);
					if(listView)
					{
						styleProvider=listView->GetListViewStyleProvider();
						listView->GetContainerComposition()->AddChild(columnHeaders);
						columnItemView=dynamic_cast<IColumnItemView*>(listView->GetItemProvider()->RequestView(IColumnItemView::Identifier));
						if(columnItemView)
						{
							columnItemView->AttachCallback(columnItemViewCallback.Obj());
							RebuildColumns();
						}
					}
				}

				void ListViewColumnItemArranger::DetachListControl()
				{
					if(listView)
					{
						if(columnItemView)
						{
							columnItemView->DetachCallback(columnItemViewCallback.Obj());
							listView->GetItemProvider()->ReleaseView(columnItemView);
							columnItemView=0;
						}
						listView->GetContainerComposition()->RemoveChild(columnHeaders);
						styleProvider=0;
						listView=0;
					}
					FixedHeightItemArranger::DetachListControl();
				}
				
/***********************************************************************
ListViewDetailContentProvider
***********************************************************************/

				ListViewDetailContentProvider::ItemContent::ItemContent(Size iconSize, const FontProperties& font, GuiListControl::IItemProvider* _itemProvider)
					:contentComposition(0)
					,itemProvider(_itemProvider)
				{
					columnItemView=dynamic_cast<ListViewColumnItemArranger::IColumnItemView*>(itemProvider->RequestView(ListViewColumnItemArranger::IColumnItemView::Identifier));
					contentComposition=new GuiBoundsComposition;
					contentComposition->SetMinSizeLimitation(GuiGraphicsComposition::LimitToElementAndChildren);

					textTable=new GuiTableComposition;
					textTable->SetAlignmentToParent(Margin(0, 0, 0, 0));
					textTable->SetRowsAndColumns(1, 1);
					textTable->SetRowOption(0, GuiCellOption::MinSizeOption());
					textTable->SetColumnOption(0, GuiCellOption::AbsoluteOption(0));
					contentComposition->AddChild(textTable);
					{
						GuiCellComposition* cell=new GuiCellComposition;
						textTable->AddChild(cell);
						cell->SetSite(0, 0, 1, 1);

						GuiTableComposition* table=new GuiTableComposition;
						cell->AddChild(table);
						table->SetRowsAndColumns(3, 2);
						table->SetRowOption(0, GuiCellOption::PercentageOption(0.5));
						table->SetRowOption(1, GuiCellOption::MinSizeOption());
						table->SetRowOption(2, GuiCellOption::PercentageOption(0.5));
						table->SetColumnOption(0, GuiCellOption::MinSizeOption());
						table->SetColumnOption(1, GuiCellOption::PercentageOption(1.0));
						table->SetAlignmentToParent(Margin(0, 0, 0, 0));
						table->SetCellPadding(2);
						{
							GuiCellComposition* cell=new GuiCellComposition;
							table->AddChild(cell);
							cell->SetSite(1, 0, 1, 1);
							cell->SetPreferredMinSize(iconSize);

							image=GuiImageFrameElement::Create();
							image->SetStretch(true);
							cell->SetOwnedElement(image);
						}
						{
							GuiCellComposition* cell=new GuiCellComposition;
							table->AddChild(cell);
							cell->SetSite(0, 1, 3, 1);
							cell->SetMargin(Margin(0, 0, 8, 0));

							text=GuiSolidLabelElement::Create();
							text->SetFont(font);
							text->SetEllipse(true);
							cell->SetOwnedElement(text);
						}
					}
				}

				ListViewDetailContentProvider::ItemContent::~ItemContent()
				{
					if(columnItemView)
					{
						itemProvider->ReleaseView(columnItemView);
					}
				}

				elements::GuiBoundsComposition* ListViewDetailContentProvider::ItemContent::GetContentComposition()
				{
					return contentComposition;
				}

				elements::GuiBoundsComposition* ListViewDetailContentProvider::ItemContent::GetBackgroundDecorator()
				{
					return 0;
				}

				void ListViewDetailContentProvider::ItemContent::UpdateSubItemSize()
				{
					int columnCount=columnItemView->GetColumnCount();
					for(int i=0;i<columnCount;i++)
					{
						textTable->SetColumnOption(i, GuiCellOption::AbsoluteOption(columnItemView->GetColumnSize(i)));
					}
					textTable->UpdateCellBounds();
				}

				void ListViewDetailContentProvider::ItemContent::Install(GuiListViewBase::IStyleProvider* styleProvider, ListViewItemStyleProvider::IListViewItemView* view, int itemIndex)
				{
					for(int i=1;i<textTable->GetColumns();i++)
					{
						GuiCellComposition* cell=textTable->GetSitedCell(0, i);
						textTable->RemoveChild(cell);
						delete cell;
					}

					Ptr<GuiImageData> imageData=view->GetSmallImage(itemIndex);
					if(imageData)
					{
						image->SetImage(imageData->GetImage(), imageData->GetFrameIndex());
					}
					else
					{
						image->SetImage(0);
					}
					text->SetText(view->GetText(itemIndex));
					text->SetColor(styleProvider->GetPrimaryTextColor());

					int columnCount=columnItemView->GetColumnCount();
					textTable->SetRowsAndColumns(1, columnCount);
					for(int i=1;i<columnCount;i++)
					{
						GuiCellComposition* cell=new GuiCellComposition;
						textTable->AddChild(cell);
						cell->SetSite(0, i, 1, 1);
						cell->SetMargin(Margin(8, 0, 8, 0));

						GuiSolidLabelElement* subText=GuiSolidLabelElement::Create();
						subText->SetAlignments(Alignment::Left, Alignment::Center);
						subText->SetFont(text->GetFont());
						subText->SetEllipse(true);
						subText->SetText(view->GetSubItem(itemIndex, i-1));
						subText->SetColor(styleProvider->GetSecondaryTextColor());
						cell->SetOwnedElement(subText);
					}
					UpdateSubItemSize();
				}

				void ListViewDetailContentProvider::OnColumnChanged()
				{
				}

				void ListViewDetailContentProvider::OnColumnSizeChanged(int index)
				{
					int itemCount=listViewItemStyleProvider->GetCreatedItemStyles().Count();
					for(int i=0;i<itemCount;i++)
					{
						ListViewItemStyleProvider::ListViewContentItemStyleController* itemStyle
							=dynamic_cast<ListViewItemStyleProvider::ListViewContentItemStyleController*>(
								listViewItemStyleProvider->GetCreatedItemStyles()[i]
								);
						if(itemStyle && listViewItemStyleProvider->IsItemStyleAttachedToListView(itemStyle))
						{
							ItemContent* itemContent=dynamic_cast<ItemContent*>(itemStyle->GetItemContent());
							if(itemContent)
							{
								itemContent->UpdateSubItemSize();
							}
						}
					}
				}

				ListViewDetailContentProvider::ListViewDetailContentProvider(Size _iconSize)
					:iconSize(_iconSize)
					,itemProvider(0)
					,columnItemView(0)
					,listViewItemStyleProvider(0)
				{
				}

				ListViewDetailContentProvider::~ListViewDetailContentProvider()
				{
				}

				GuiListControl::IItemCoordinateTransformer* ListViewDetailContentProvider::CreatePreferredCoordinateTransformer()
				{
					return new DefaultItemCoordinateTransformer;
				}

				GuiListControl::IItemArranger* ListViewDetailContentProvider::CreatePreferredArranger()
				{
					return new ListViewColumnItemArranger;
				}

				ListViewItemStyleProvider::IListViewItemContent* ListViewDetailContentProvider::CreateItemContent(const FontProperties& font)
				{
					return new ItemContent(iconSize, font, itemProvider);
				}

				void ListViewDetailContentProvider::AttachListControl(GuiListControl* value)
				{
					listViewItemStyleProvider=dynamic_cast<ListViewItemStyleProvider*>(value->GetStyleProvider());
					itemProvider=value->GetItemProvider();
					columnItemView=dynamic_cast<ListViewColumnItemArranger::IColumnItemView*>(itemProvider->RequestView(ListViewColumnItemArranger::IColumnItemView::Identifier));
					if(columnItemView)
					{
						columnItemView->AttachCallback(this);
					}
				}

				void ListViewDetailContentProvider::DetachListControl()
				{
					if(columnItemView)
					{
						columnItemView->DetachCallback(this);
						itemProvider->ReleaseView(columnItemView);
					}
					itemProvider=0;
					listViewItemStyleProvider=0;
				}

/***********************************************************************
ListViewItemProvider
***********************************************************************/

				ListViewColumn::ListViewColumn(const WString& _text, int _size)
					:text(_text)
					,size(_size)
				{
				}

				Ptr<GuiImageData> ListViewItemProvider::GetSmallImage(int itemIndex)
				{
					return Get(itemIndex)->smallImage;
				}

				Ptr<GuiImageData> ListViewItemProvider::GetLargeImage(int itemIndex)
				{
					return Get(itemIndex)->largeImage;
				}

				WString ListViewItemProvider::GetText(int itemIndex)
				{
					return Get(itemIndex)->text;
				}

				WString ListViewItemProvider::GetSubItem(int itemIndex, int index)
				{
					Ptr<ListViewItem> item=Get(itemIndex);
					if(index<0 || index>=item->subItems.Count())
					{
						return L"";
					}
					else
					{
						return item->subItems[index];
					}
				}

				int ListViewItemProvider::GetDataColumnCount()
				{
					return dataColumns.Count();
				}

				int ListViewItemProvider::GetDataColumn(int index)
				{
					return dataColumns[index];
				}

				bool ListViewItemProvider::AttachCallback(ListViewColumnItemArranger::IColumnItemViewCallback* value)
				{
					if(columnItemViewCallbacks.Contains(value))
					{
						return false;
					}
					else
					{
						columnItemViewCallbacks.Add(value);
						return true;
					}
				}

				bool ListViewItemProvider::DetachCallback(ListViewColumnItemArranger::IColumnItemViewCallback* value)
				{
					int index=columnItemViewCallbacks.IndexOf(value);
					if(index==-1)
					{
						return false;
					}
					else
					{
						columnItemViewCallbacks.Remove(value);
						return true;
					}
				}

				int ListViewItemProvider::GetColumnCount()
				{
					return columns.Count();
				}

				WString ListViewItemProvider::GetColumnText(int index)
				{
					if(index<0 || index>=columns.Count())
					{
						return L"";
					}
					else
					{
						return columns[index]->text;
					}
				}

				int ListViewItemProvider::GetColumnSize(int index)
				{
					if(index<0 || index>=columns.Count())
					{
						return 0;
					}
					else
					{
						return columns[index]->size;
					}
				}

				void ListViewItemProvider::SetColumnSize(int index, int value)
				{
					if(index>=0 && index<columns.Count())
					{
						columns[index]->size=value;
						for(int i=0;i<columnItemViewCallbacks.Count();i++)
						{
							columnItemViewCallbacks[i]->OnColumnSizeChanged(index);
						}
					}
				}

				ListViewItemProvider::ListViewItemProvider()
				{
				}

				ListViewItemProvider::~ListViewItemProvider()
				{
				}

				Interface* ListViewItemProvider::RequestView(const WString& identifier)
				{
					if(identifier==ListViewItemStyleProvider::IListViewItemView::Identifier)
					{
						return (ListViewItemStyleProvider::IListViewItemView*)this;
					}
					else if(identifier==ListViewColumnItemArranger::IColumnItemView::Identifier)
					{
						return (ListViewColumnItemArranger::IColumnItemView*)this;
					}
					else
					{
						return 0;
					}
				}

				void ListViewItemProvider::ReleaseView(Interface* view)
				{
				}

				collections::IList<int>& ListViewItemProvider::GetDataColumns()
				{
					return dataColumns.Wrap();
				}

				void ListViewItemProvider::NotifyDataColumnsUpdated()
				{
					NotifyUpdate(0, Count());
				}

				collections::IList<Ptr<ListViewColumn>>& ListViewItemProvider::GetColumns()
				{
					return columns.Wrap();
				}

				void ListViewItemProvider::NotifyColumnsUpdated()
				{
					for(int i=0;i<columnItemViewCallbacks.Count();i++)
					{
						columnItemViewCallbacks[i]->OnColumnChanged();
					}
				}
			}

/***********************************************************************
GuiListView
***********************************************************************/

			GuiVirtualListView::GuiVirtualListView(IStyleProvider* _styleProvider, GuiListControl::IItemProvider* _itemProvider)
				:GuiListViewBase(_styleProvider, _itemProvider)
			{
				ChangeItemStyle(new list::ListViewBigIconContentProvider);
			}

			GuiVirtualListView::~GuiVirtualListView()
			{
			}

			void GuiVirtualListView::ChangeItemStyle(list::ListViewItemStyleProvider::IListViewItemContentProvider* contentProvider)
			{
				SetStyleProvider(0);
				SetArranger(0);
				SetCoordinateTransformer(contentProvider->CreatePreferredCoordinateTransformer());
				SetStyleProvider(new list::ListViewItemStyleProvider(contentProvider));
				SetArranger(contentProvider->CreatePreferredArranger());
			}

/***********************************************************************
GuiListView
***********************************************************************/

			GuiListView::GuiListView(IStyleProvider* _styleProvider)
				:GuiVirtualListView(_styleProvider, new list::ListViewItemProvider)
			{
				items=dynamic_cast<list::ListViewItemProvider*>(itemProvider.Obj());
			}

			GuiListView::~GuiListView()
			{
			}

			list::ListViewItemProvider& GuiListView::GetItems()
			{
				return *items;
			}
		}
	}
}