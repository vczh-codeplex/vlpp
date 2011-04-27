#include "GuiWindow.h"

namespace vl
{
	namespace presentation
	{

/***********************************************************************
GuiWindow
***********************************************************************/

		const wchar_t* const GuiWindow::SkinBuilderName = L"vl::presentation::GuiWindow";

		WString GuiWindow::GetSkinBuilderName()
		{
			return SkinBuilderName;
		}

		void GuiWindow::NotifySkinChanged()
		{
			GuiWindowBase::NotifySkinChanged();
			Ptr<IGuiWindowSkin> skin=GetSkin().Cast<IGuiWindowSkin>();
			if(skin)
			{
				skin->Install();
			}
		}

		GuiWindow::GuiWindow()
		{
			InitializeWindow();
		}

		GuiWindow::~GuiWindow()
		{
			FinalizeWindow();
		}

		WString GuiWindow::GetTitle()
		{
			return GetContainingNativeWindow()->GetTitle();
		}

		void GuiWindow::SetTitle(const WString& value)
		{
			GetContainingNativeWindow()->SetTitle(value);
		}

		void GuiWindow::MoveToScreenCenter()
		{
			INativeWindow* window=GetContainingNativeWindow();
			INativeScreen* screen=GetCurrentController()->GetScreen(window);
			if(screen)
			{
				Rect client=screen->GetClientBounds();
				Rect bounds=window->GetBounds();
				int x=(client.Width()-bounds.Width())/2+client.Left();
				int y=(client.Height()-bounds.Height())/2+client.Top();
				window->SetBounds(Rect(Point(x, y), bounds.GetSize()));
			}
		}

		void GuiWindow::Show()
		{
			GetContainingNativeWindow()->Show();
		}

		void GuiWindow::Close()
		{
			GetContainingNativeWindow()->Hide();
		}

		Rect GuiWindow::GetBounds()
		{
			return GetContainingNativeWindow()->GetBounds();
		}

		void GuiWindow::SetBounds(Rect value)
		{
			GetContainingNativeWindow()->SetBounds(value);
		}
		
		Size GuiWindow::GetClientSize()
		{
			return GetContainingNativeWindow()->GetClientSize();
		}

		void GuiWindow::SetClientSize(Size value)
		{
			GetContainingNativeWindow()->SetClientSize(value);
		}
	}
}