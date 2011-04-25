/***********************************************************************
Vczh Library++ 3.0
Developer: �����(vczh)
GUI::Control Library

Interfaces:
***********************************************************************/

#ifndef VCZH_PRESENTATION_GUIWINDOW
#define VCZH_PRESENTATION_GUIWINDOW

#include "GuiControl.h"

namespace vl
{
	namespace presentation
	{

/***********************************************************************
���㴰��
***********************************************************************/

		class GuiWindow : public GuiWindowBase
		{
		public:
			static const wchar_t* const		SkinBuilderName;

			class IGuiWindowSkin : public IGuiSkin
			{
			public:
				virtual void				Install(INativeWindow* window)=0;
			};
		protected:

			WString							GetSkinBuilderName();
			void							NotifySkinChanged();
		public:
			GuiWindow();
			~GuiWindow();

			WString							GetTitle();
			void							SetTitle(const WString& value);
			void							MoveToScreenCenter();
			void							Show();
			void							Close();

			Rect							GetBounds();
			void							SetBounds(Rect value);
			Size							GetClientSize();
			void							SetClientSize(Size value);
		};
	}
}

#endif