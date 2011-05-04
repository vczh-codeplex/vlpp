/***********************************************************************
Vczh Library++ 3.0
Developer: �����(vczh)
GUI::Control Library

Interfaces:
***********************************************************************/

#ifndef VCZH_PRESENTATION_GUICONTAINER
#define VCZH_PRESENTATION_GUICONTAINER

#include "GuiControl.h"

namespace vl
{
	namespace presentation
	{

/***********************************************************************
��ͨ����
***********************************************************************/

		class GuiPanel : public GuiControl
		{
		public:
			enum PanelState
			{
				Normal,
				Active,
				Focused,
				Disabled,
			};

			enum BorderState
			{
				None,
				Static,
				Dynamic,
			};

			static const wchar_t* const		SkinBuilderName;

			class IGuiPanelSkin : public virtual IGuiSkin
			{
			public:
				virtual void					SetPanelState(PanelState value)=0;
				virtual void					SetBorderState(BorderState value)=0;
			};
		protected:
			bool								entering;
			BorderState							borderState;

			WString								GetSkinBuilderName();
			void								NotifySkinChanged();
			void								UpdateSkinPanelState(Ptr<IGuiPanelSkin> skin=0);
			
			void								NotifyMouseEntered();
			void								NotifyMouseLeaved();
			GuiControl*							NotifyGotFocus();
			GuiControl*							NotifyLostFocus();
			void								NotifyVisuallyEnabled();
			void								NotifyVisuallyDisabled();
		public:
			GuiPanel();
			~GuiPanel();

			BorderState							GetBorderState();
			void								SetBorderState(BorderState value);
		};
	}
}

#endif