/***********************************************************************
Vczh Library++ 3.0
Developer: 陈梓瀚(vczh)
GUI::Windows GDI Provider

Interfaces:
***********************************************************************/
#ifndef VCZH_PRESENTATION_WINDOWS_GDI_WINGDIELEMENT
#define VCZH_PRESENTATION_WINDOWS_GDI_WINGDIELEMENT

#include "WinGDI.h"
#include "..\WinNativeWindow.h"
#include "..\..\..\..\..\..\Library\Collections\Dictionary.h"

namespace vl
{
	namespace presentation
	{
		namespace elements
		{
			class WinGDIElement;
			class WinGDIElementEnvironment;

/***********************************************************************
基础建设
***********************************************************************/

			class WinGDIElement : public Object
			{
			protected:
				WinGDIElementEnvironment*		environment;

			public:
				WinGDIElement(WinGDIElementEnvironment* _environment);
				~WinGDIElement();

				virtual void					Paint(Size offset, windows::WinDC* dc)=0;
			};

			class WinGDIElementEnvironment : public Object
			{
				typedef collections::Dictionary<WString, Ptr<Object>>	ResourceMap;
			protected:
				INativeWindow*					window;
				Ptr<WinGDIElement>				rootElement;
				ResourceMap						resources;
			public:
				WinGDIElementEnvironment(INativeWindow* _window);
				~WinGDIElementEnvironment();

				ResourceMap&					Resources();
				Ptr<Object>						Resource(const WString& name);
				windows::WinDC*					GetEnvironmentDC();
				Ptr<WinGDIElement>				GetRootElement();
				void							SetRootElement(Ptr<WinGDIElement> element);
				void							Paint();
			};

/***********************************************************************
基本元素
***********************************************************************/

			class WinGDIClipElement : public WinGDIElement
			{
				typedef collections::List<Ptr<WinGDIElement>>			ElementList;
			protected:
				ElementList						children;
				Rect							bounds;
			public:
				WinGDIClipElement(WinGDIElementEnvironment* _environment);
				~WinGDIClipElement();
				
				ElementList&					Children();
				Rect							GetBounds();
				void							SetBounds(Rect value);
				void							Paint(Size offset, windows::WinDC* dc);
			};
		}
	}
}

#endif