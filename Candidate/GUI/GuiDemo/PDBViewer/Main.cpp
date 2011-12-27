#include "..\..\GUI\GacUI.h"
#include "dia2.h"

#pragma comment(lib, "diaguids.lib")

tree::INodeRootProvider* CreateProvider()
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
		window.GetBoundsComposition()->SetMinSizeLimitation(GuiGraphicsComposition::LimitToElementAndChildren);
		GuiTreeView* treeControl=new GuiTreeView(new win7::Win7TreeViewProvider, CreateProvider());
		treeControl->GetBoundsComposition()->SetPreferredMinSize(Size(400, 600));
		treeControl->GetBoundsComposition()->SetAlignmentToParent(Margin(3, 3, 3, 3));
		window.GetBoundsComposition()->AddChild(treeControl->GetBoundsComposition());
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