#include "..\..\GUI\GraphicsElement\GuiGraphicsElement.h"
#include "..\..\GUI\GraphicsElement\GuiGraphicsComposition.h"

using namespace vl;
using namespace vl::presentation;
using namespace vl::presentation::elements;

GuiBoundsComposition* CreateBorder(GuiGraphicsComposition* parent, Color color, Rect bounds, bool internalMargin)
{
	GuiSolidBorderElement* element=GuiSolidBorderElement::Create();
	element->SetColor(color);

	GuiBoundsComposition* composition=new GuiBoundsComposition;
	composition->SetOwnedElement(element);
	composition->SetBounds(bounds);
	if(internalMargin)
	{
		composition->SetInternalMargin(Margin(1, 1, 1, 1));
	}
	parent->AddChild(composition);

	return composition;
}

GuiBoundsComposition* CreateLabel(GuiGraphicsComposition* parent, Color color, Point location, const WString& text)
{
	GuiSolidLabelElement* element=GuiSolidLabelElement::Create();
	{
		FontProperties font;
		font.size=24;
		font.fontFamily=L"Lucida Console";
		element->SetFont(font);
	}
	element->SetText(text);

	GuiBoundsComposition* composition=new GuiBoundsComposition;
	parent->AddChild(composition);
	composition->SetOwnedElement(element);
	composition->SetBounds(Rect(location, element->GetRenderer()->GetMinSize()));

	return composition;
}

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
		}
		{
			GuiBoundsComposition* redBorder=CreateBorder(&windowComposition, Color(255, 0, 0), windowComposition.GetBounds(), true);
			{
				GuiBoundsComposition* label=CreateLabel(redBorder, Color(0, 0, 0), Point(10, 10), L"Vczh GUI Demo");
				GuiBoundsComposition* border1=CreateBorder(redBorder, Color(255, 0, 0), Rect(Point(100, 200), Size(100, 100)), false);
				GuiBoundsComposition* border2=CreateBorder(border1, Color(0, 255, 0), Rect(Point(0, 20), Size(100, 60)), false);
				GuiBoundsComposition* border3=CreateBorder(border1, Color(0, 0, 255), Rect(Point(-1, 21), Size(102, 58)), false);
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