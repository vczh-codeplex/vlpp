#include "GuiNativeWindow.h"

namespace vl
{
	namespace presentation
	{
		INativeController* currentController=0;

		INativeController* GetCurrentController()
		{
			return currentController;
		}

		void SetCurrentController(INativeController* controller)
		{
			currentController=controller;
		}
	}
}