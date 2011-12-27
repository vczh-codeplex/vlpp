#include "..\..\GUI\GacUI.h"

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
		GuiTreeView* treeControl=new GuiTreeView(new win7::Win7TreeViewProvider);
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