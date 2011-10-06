#include "..\..\GUI\GraphicsElement\GuiGraphicsElement.h"

using namespace vl;
using namespace vl::presentation;
using namespace vl::presentation::elements;

void GuiMain()
{
	INativeWindow* window=GetCurrentController()->CreateNativeWindow();
	window->SetSizeBox(false);
	window->SetMaximizedBox(false);
	window->SetMinimizedBox(false);
	window->SetTitle(L"Vczh GUI Demo");
	window->SetClientSize(Size(800, 600));

	INativeScreen* screen=GetCurrentController()->GetScreen(0);
	Rect windowBounds=window->GetBounds();
	Rect screenBounds=screen->GetClientBounds();
	window->SetBounds(Rect(
		Point(
			screenBounds.Left()+(screenBounds.Width()-windowBounds.Width())/2,
			screenBounds.Top()+(screenBounds.Height()-windowBounds.Height())/2
			),
		windowBounds.GetSize()
		));

	{
		GuiSolidBackgroundElement* background=GuiSolidBackgroundElement::Create();
		background->SetColor(Color(255, 255, 255));

		GuiSolidBorderElement* border=GuiSolidBorderElement::Create();
		border->SetColor(Color(255, 0, 0));

		IGuiGraphicsRenderTarget* renderTarget=GetGuiGraphicsResourceManager()->GetRenderTarget(window);
		background->GetRenderer()->SetRenderTarget(renderTarget);
		border->GetRenderer()->SetRenderTarget(renderTarget);

		renderTarget->StartRendering();
		Rect bounds(Point(0, 0), window->GetClientSize());
		background->GetRenderer()->Render(bounds);
		border->GetRenderer()->Render(bounds);
		renderTarget->StopRendering();
	}

	window->Show();
	Rect x=window->GetBounds();

	GetCurrentController()->Run(window);
}