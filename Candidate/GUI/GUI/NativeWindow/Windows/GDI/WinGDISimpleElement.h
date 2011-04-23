/***********************************************************************
Vczh Library++ 3.0
Developer: ³Âè÷å«(vczh)
GUI::Windows GDI Provider

Interfaces:
***********************************************************************/

#ifndef VCZH_PRESENTATION_WINDOWS_GDI_WINGDISIMPLEELEMENT
#define VCZH_PRESENTATION_WINDOWS_GDI_WINGDISIMPLEELEMENT

#include "WinGDIElement.h"

namespace vl
{
	namespace presentation
	{
		namespace elements
		{

/***********************************************************************
×ÊÔ´
***********************************************************************/

			class WinGDISimpleResources
			{
			public:
				static void								RegisterAutoInstall();
				static void								UnregisterAutoInstall();
				static void								Install(WinGDIElementEnvironment* environment);
				static void								Uninstall(WinGDIElementEnvironment* environment);

				static Ptr<windows::WinPen>				GetPen(WinGDIElementEnvironment* environment, const WString& name);
				static Ptr<windows::WinBrush>			GetBrush(WinGDIElementEnvironment* environment, const WString& name);
			};

/***********************************************************************
ÔªËØ
***********************************************************************/
		}
	}
}

#endif