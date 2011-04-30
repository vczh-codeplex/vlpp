#include "GuiContainer.h"

namespace vl
{
	namespace presentation
	{

/***********************************************************************
GuiPanel
***********************************************************************/

		const wchar_t* const GuiPanel::SkinBuilderName = L"vl::presentation::GuiPanel";

		WString GuiPanel::GetSkinBuilderName()
		{
			return SkinBuilderName;
		}

		void GuiPanel::NotifySkinChanged()
		{
			GuiControl::NotifySkinChanged();
			Ptr<IGuiPanelSkin> skin=GetSkin().Cast<IGuiPanelSkin>();
			if(skin)
			{
				UpdateSkinPanelState(skin);
				skin->SetBorderState(borderState);
			}
		}

		void GuiPanel::UpdateSkinPanelState(Ptr<IGuiPanelSkin> skin)
		{
			if(!skin)
			{
				skin=GetSkin().Cast<IGuiPanelSkin>();
			}
			if(skin)
			{
				skin->SetPanelState(
					IsFocusing()?Focused:
					entering?Active:Normal
					);
			}
		}
			
		void GuiPanel::NotifyMouseEntered()
		{
			GuiControl::NotifyMouseEntered();
			entering=true;
			UpdateSkinPanelState();
		}

		void GuiPanel::NotifyMouseLeaved()
		{
			GuiControl::NotifyMouseLeaved();
			entering=false;
			UpdateSkinPanelState();
		}

		GuiControl* GuiPanel::NotifyGotFocus()
		{
			GuiControl* actionRaiser=GuiControl::NotifyGotFocus();
			UpdateSkinPanelState();
			return actionRaiser;
		}

		GuiControl* GuiPanel::NotifyLostFocus()
		{
			GuiControl* actionRaiser=GuiControl::NotifyLostFocus();
			UpdateSkinPanelState();
			return actionRaiser;
		}

		GuiPanel::GuiPanel()
			:entering(false)
			,borderState(Dynamic)
		{
		}

		GuiPanel::~GuiPanel()
		{
		}

		GuiPanel::BorderState GuiPanel::GetBorderState()
		{
			return borderState;
		}

		void GuiPanel::SetBorderState(BorderState value)
		{
			borderState=value;
			Ptr<IGuiPanelSkin> skin=GetSkin().Cast<IGuiPanelSkin>();
			if(skin)
			{
				skin->SetBorderState(borderState);
			}
		}
	}
}