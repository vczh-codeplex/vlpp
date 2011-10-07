#include "..\..\GUI\GraphicsElement\GuiGraphicsElement.h"
#include "..\..\GUI\GraphicsElement\GuiGraphicsComposition.h"

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
		GuiWindowComposition windowComposition;
		windowComposition.SetAttachedWindow(window);
		{
			GuiSolidBackgroundElement* background=GuiSolidBackgroundElement::Create();
			background->SetColor(Color(255, 255, 255));
			windowComposition.SetOwnedElement(background);

			GuiBoundsComposition* labelComposition=new GuiBoundsComposition;
			{
				GuiSolidLabelElement* label=GuiSolidLabelElement::Create();
				{
					FontProperties font;
					font.size=24;
					font.fontFamily=L"Lucida Console";
					label->SetFont(font);
				}
				label->SetText(L"Vczh GUI Demo");

				windowComposition.AddChild(labelComposition);
				labelComposition->SetOwnedElement(label);
				labelComposition->SetBounds(Rect(Point(10, 10), label->GetRenderer()->GetMinSize()));
			}
		}
		{
			windowComposition.GetRenderTarget()->StartRendering();
			windowComposition.Render(Size());
			windowComposition.GetRenderTarget()->StopRendering();
		}

		//GuiSolidBorderElement* border=GuiSolidBorderElement::Create();
		//border->SetColor(Color(255, 0, 0));

	}

	GetCurrentController()->Run(window);
}