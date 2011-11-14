/***********************************************************************
Vczh Library++ 3.0
Developer: ³Âè÷å«(vczh)
GacUI::Application Framework

Interfaces:
***********************************************************************/

#ifndef VCZH_PRESENTATION_CONTROLS_GUIAPPLICATION
#define VCZH_PRESENTATION_CONTROLS_GUIAPPLICATION

#include "GuiWindowControls.h"

namespace vl
{
	namespace presentation
	{
		namespace controls
		{
			class GuiApplication : public Object, private INativeControllerListener
			{
				friend void GuiApplicationInitialize();
				friend class GuiWindow;
			private:
				void											LeftButtonDown(Point position)override;
				void											LeftButtonUp(Point position)override;
				void											RightButtonDown(Point position)override;
				void											RightButtonUp(Point position)override;
			protected:
				GuiWindow*										mainWindow;
				collections::List<GuiWindow*>					windows;

				GuiApplication();
				~GuiApplication();

				void											RegisterWindow(GuiWindow* window);
				void											UnregisterWindow(GuiWindow* window);
				void											OnMouseDown(Point location);
			public:
				void											Run(GuiWindow* _mainWindow);
				const collections::IReadonlyList<GuiWindow*>&	GetWindows();
				GuiWindow*										GetWindow(Point location);
			};

			extern GuiApplication*								GetApplication();
		}
	}
}

extern void GuiApplicationMain();

#endif