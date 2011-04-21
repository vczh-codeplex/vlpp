/***********************************************************************
Vczh Library++ 3.0
Developer: ³Âè÷å«(vczh)
GUI::Windows Platform

Interfaces:
***********************************************************************/
#ifndef VCZH_PRESENTATION_WINDOWS_GDI_WINGDIELEMENT
#define VCZH_PRESENTATION_WINDOWS_GDI_WINGDIELEMENT

#include "WinGDI.h"
#include "..\WinNativeWindow.h"

namespace vl
{
	namespace presentation
	{
		namespace windows
		{
			class WinGDIElement;
			class WinGDIElementEnvironment;

/***********************************************************************
Environment
***********************************************************************/

			class WinGDIElementEnvironment : public Object
			{
			protected:
				INativeWindow*					window;
			public:
				WinGDIElementEnvironment(INativeWindow* _window);
				~WinGDIElementEnvironment();

				WinDC*							GetEnvironmentDC();
			};

/***********************************************************************
Element
***********************************************************************/

			class WinGDIElement : public Object
			{
			protected:
				WinGDIElementEnvironment*		environment;

			public:
				WinGDIElement(WinGDIElementEnvironment* _environment);
				~WinGDIElement();

				virtual void					Paint()=0;
			};
		}
	}
}

#endif