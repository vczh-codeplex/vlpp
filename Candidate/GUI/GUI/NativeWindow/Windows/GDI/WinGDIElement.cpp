#include "WinGDIElement.h"
#include "WinGDIApplication.h"

namespace vl
{
	namespace presentation
	{
		namespace windows
		{

/***********************************************************************
WinGDIElementEnvironment
***********************************************************************/

			WinGDIElementEnvironment::WinGDIElementEnvironment(INativeWindow* _window)
				:window(_window)
			{
			}

			WinGDIElementEnvironment::~WinGDIElementEnvironment()
			{
			}

			WinDC* WinGDIElementEnvironment::GetEnvironmentDC()
			{
				return GetNativeWindowDC(window);
			}

/***********************************************************************
WinGDIElement
***********************************************************************/

			WinGDIElement::WinGDIElement(WinGDIElementEnvironment* _environment)
				:environment(_environment)
			{
			}

			WinGDIElement::~WinGDIElement()
			{
			}
		}
	}
}