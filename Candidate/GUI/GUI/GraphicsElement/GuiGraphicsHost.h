/***********************************************************************
Vczh Library++ 3.0
Developer: ³Âè÷å«(vczh)
GUI::Windows Platform

Interfaces:
***********************************************************************/

#ifndef VCZH_PRESENTATION_WINDOWS_GUIGRAPHICSHOST
#define VCZH_PRESENTATION_WINDOWS_GUIGRAPHICSHOST

#include "GuiGraphicsComposition.h"

namespace vl
{
	namespace presentation
	{
		namespace elements
		{

/***********************************************************************
Host
***********************************************************************/

			class GuiGraphicsHost : public Object, private INativeWindowListener
			{
			protected:
				INativeWindow*					nativeWindow;
				GuiWindowComposition*			windowComposition;
				Size							previousClientSize;
				Size							minSize;

			private:
				void							Moving(Rect& bounds);
				void							Moved();
			public:
				GuiGraphicsHost();
				~GuiGraphicsHost();

				INativeWindow*					GetNativeWindow();
				void							SetNativeWindow(INativeWindow* _nativeWindow);
				GuiGraphicsComposition*			GetMainComposition();
				void							Render();
			};

/***********************************************************************
Animation
***********************************************************************/

/***********************************************************************
Message Receiver
***********************************************************************/
		}
	}
}

#endif