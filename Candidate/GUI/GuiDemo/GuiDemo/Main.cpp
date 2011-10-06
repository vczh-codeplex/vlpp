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

		GuiSolidLabelElement* label=GuiSolidLabelElement::Create();
		{
			FontProperties font;
			font.size=24;
			font.fontFamily=L"Lucida Console";
			label->SetFont(font);
		}
		label->SetText(L"Vczh GUI Demo");

		IGuiGraphicsRenderTarget* renderTarget=GetGuiGraphicsResourceManager()->GetRenderTarget(window);
		background->GetRenderer()->SetRenderTarget(renderTarget);
		border->GetRenderer()->SetRenderTarget(renderTarget);
		label->GetRenderer()->SetRenderTarget(renderTarget);

		renderTarget->StartRendering();
		{
			Rect clientBounds(Point(0, 0), window->GetClientSize());
			background->GetRenderer()->Render(clientBounds);
			border->GetRenderer()->Render(clientBounds);

			Rect textBounds(Point(20, 20), label->GetRenderer()->GetMinSize());
			label->GetRenderer()->Render(textBounds);
		}
		renderTarget->StopRendering();

		delete background;
		delete border;
		delete label;
	}

	GetCurrentController()->Run(window);
}