#include "..\..\GUI\GacUI.h"
#include "dia2.h"

#pragma comment(lib, "diaguids.lib")

using namespace vl;
using namespace vl::stream;
using namespace vl::entities;

namespace dumppdb
{
	extern void DumpPdbToXml(IDiaSymbol* exeSymbol, const wchar_t* xml);
}

extern tree::INodeRootProvider* CreateProviderFromDiaSymbol(IDiaSymbol* symbol);
extern tree::INodeRootProvider* CreateProviderFromXml(Ptr<TreeNode> node);

IDiaSymbol* CreateDiaSymbol()
{
    IDiaDataSource* pSource=0;
    IDiaSession* pSession=0;
    IDiaSymbol* pSymbol=0;
    CoInitialize(NULL);
    HRESULT hr = CoCreateInstance(
        CLSID_DiaSource,
        NULL,
        CLSCTX_INPROC_SERVER,
        IID_IDiaDataSource,
        (void**) &pSource
        );
    if(SUCCEEDED(hr))
	if(SUCCEEDED(pSource->loadDataFromPdb(L"..\\Debug\\GuiDemo.pdb")))
    if(SUCCEEDED(pSource->openSession(&pSession)))
	if(SUCCEEDED(pSession->get_globalScope(&pSymbol)))
	{
		return pSymbol;
	}
	return 0;
}

void GuiMain()
{
	GuiWindow window(new win7::Win7WindowStyle);
#ifdef GUI_GRAPHICS_RENDERER_GDI
	window.SetText(L"PDB Viewer (GDI)");
#endif
#ifdef GUI_GRAPHICS_RENDERER_DIRECT2D
	window.SetText(L"PDB Viewer (Direct2D)");
#endif
	{
		IDiaSymbol* diaSymbol=CreateDiaSymbol();
		GuiButton* buttonDump=0;
		GuiTab* tabControl=0;
		window.GetBoundsComposition()->SetMinSizeLimitation(GuiGraphicsComposition::LimitToElementAndChildren);

		GuiTableComposition* table=new GuiTableComposition;
		table->SetAlignmentToParent(Margin(2, 2, 2, 2));
		table->SetRowsAndColumns(2, 1);
		table->SetRowOption(0, GuiCellOption::MinSizeOption());
		table->SetRowOption(1, GuiCellOption::PercentageOption(1.0));
		table->SetColumnOption(0, GuiCellOption::PercentageOption(1.0));
		table->SetCellPadding(3);
		window.GetBoundsComposition()->AddChild(table);
		{
			GuiCellComposition* cell=new GuiCellComposition;
			table->AddChild(cell);
			cell->SetSite(0, 0, 1, 1);

			buttonDump=new GuiButton(new win7::Win7ButtonStyle);
			buttonDump->SetText(L"Dump PDB to GuiDemo.XML");
			cell->AddChild(buttonDump->GetBoundsComposition());
		}
		{
			GuiCellComposition* cell=new GuiCellComposition;
			table->AddChild(cell);
			cell->SetSite(1, 0, 1, 1);

			tabControl=new GuiTab(new win7::Win7TabStyle);
			tabControl->GetBoundsComposition()->SetPreferredMinSize(Size(100, 100));
			tabControl->GetBoundsComposition()->SetAlignmentToParent(Margin(0, 0, 0, 0));
			cell->AddChild(tabControl->GetBoundsComposition());
			{
				GuiTabPage* page=tabControl->CreatePage();
				page->SetText(L"PDB View");

				GuiTreeView* treeControl=new GuiTreeView(new win7::Win7TreeViewProvider, CreateProviderFromDiaSymbol(diaSymbol));
				treeControl->GetBoundsComposition()->SetAlignmentToParent(Margin(0, 0, 0, 0));
				treeControl->SetVerticalAlwaysVisible(false);
				treeControl->SetHorizontalAlwaysVisible(false);
				page->GetContainer()->GetContainerComposition()->AddChild(treeControl->GetBoundsComposition());
			}
			{
				GuiTabPage* page=tabControl->CreatePage();
				page->SetText(L"Class View");
				page->GetContainer()->GetBoundsComposition()->SetMinSizeLimitation(GuiGraphicsComposition::LimitToElementAndChildren);

				GuiSolidLabelElement* textElement=GuiSolidLabelElement::Create();
				textElement->SetText(L"Please dump pdb to enable the class view.");
				textElement->SetAlignments(Alignment::Center, Alignment::Center);
				textElement->SetFont(page->GetContainer()->GetFont());

				GuiBoundsComposition* textComposition=new GuiBoundsComposition;
				textComposition->SetAlignmentToParent(Margin(0, 0, 0, 0));
				textComposition->SetOwnedElement(textElement);
				textComposition->SetMinSizeLimitation(GuiGraphicsComposition::LimitToElementAndChildren);
				page->GetContainer()->GetContainerComposition()->AddChild(textComposition);
			}
		}

		buttonDump->Clicked.AttachLambda([=](GuiGraphicsComposition* sender, GuiEventArgs& arguments)
		{
			INativeController* controller=GetCurrentController();
			tabControl->GetPages()[0]->GetContainer()->SetEnabled(false);
			buttonDump->SetEnabled(false);
			buttonDump->SetText(L"Dumping...");
			buttonDump->GetRelatedControlHost()->GetBoundsComposition()->SetAssociatedCursor(controller->GetSystemCursor(INativeCursor::LargeWaiting));

			ThreadPoolLite::QueueLambda([=]()
			{
				dumppdb::DumpPdbToXml(diaSymbol, L"..\\Debug\\GuiDemo.xml");
				GetApplication()->InvokeLambdaInMainThread([=]()
				{
					tabControl->GetPages()[0]->GetContainer()->SetEnabled(true);
					tabControl->SetSelectedPage(tabControl->GetPages()[1]);
					buttonDump->SetText(L"Loading GuiDemo.xml in the class view...");

					ThreadPoolLite::QueueLambda([=]()
					{
						FileStream fileStream(L"..\\Debug\\GuiDemo.xml", FileStream::ReadOnly);
						BomDecoder decoder;
						DecoderStream decoderStream(fileStream, decoder);
						StreamReader reader(decoderStream);
						Ptr<TreeNode> xml=LoadXmlRawDocument(reader);

						GetApplication()->InvokeLambdaInMainThreadAndWait([=]()
						{
							buttonDump->SetText(L"GuiDemo.xml dumpped.");
							buttonDump->GetRelatedControlHost()->GetBoundsComposition()->SetAssociatedCursor(controller->GetDefaultSystemCursor());

							GuiTreeView* treeControl=new GuiTreeView(new win7::Win7TreeViewProvider);
							treeControl->GetBoundsComposition()->SetAlignmentToParent(Margin(0, 0, 0, 0));
							treeControl->SetVerticalAlwaysVisible(false);
							treeControl->SetHorizontalAlwaysVisible(false);
							tabControl->GetPages()[1]->GetContainer()->GetContainerComposition()->AddChild(treeControl->GetBoundsComposition());
						});
					});
				});
			});
		});
	}

	window.SetClientSize(Size(800, 600));
	INativeScreen* screen=window.GetRelatedScreen();
	Rect windowBounds=window.GetBounds();
	Rect screenBounds=screen->GetClientBounds();
	window.SetBounds(Rect(
		Point(
			screenBounds.Left()+(screenBounds.Width()-windowBounds.Width())/2,
			screenBounds.Top()+(screenBounds.Height()-windowBounds.Height())/2
			),
		windowBounds.GetSize()
		));

	GetApplication()->Run(&window);
}