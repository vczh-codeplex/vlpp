/***********************************************************************
Vczh Library++ 3.0
Developer: 陈梓瀚(vczh)
GUI::Windows GDI Provider

Interfaces:
***********************************************************************/
#ifndef VCZH_PRESENTATION_WINDOWS_GDI_WINGDIAPPLICATION
#define VCZH_PRESENTATION_WINDOWS_GDI_WINGDIAPPLICATION

#include "WinGDI.h"
#include "..\WinNativeWindow.h"
#include "..\..\..\..\..\..\Library\Collections\Dictionary.h"
#include "..\..\..\GuiApplication.h"

namespace vl
{
	namespace presentation
	{
		namespace elements
		{
			class WinGDIElement;
			class WinGDIElementEnvironment;
		}

		namespace windows
		{
			extern WinDC*									GetNativeWindowDC(INativeWindow* window);
			extern elements::WinGDIElementEnvironment*		GetNativeWindowGDIElementEnvironment(INativeWindow* window);
		}

		namespace elements
		{

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

/***********************************************************************
基本皮肤
***********************************************************************/

			template<typename ISkinInterface, typename SkinImplementation>
			class WinGDISkin : public Object, public ISkinInterface
			{
			public:
				class Builder : public Object, public IGuiSkinBuilder
				{
				public:
					Ptr<IGuiSkin> Build(INativeWindow* window)
					{
						return new SkinImplementation(window);
					}
				};
			protected:
				INativeWindow*							window;
				WinGDIElementEnvironment*				environment;
				IGuiSkinListener*						skinListener;
			public:
				WinGDISkin(INativeWindow* _window)
					:window(_window)
					,environment(windows::GetNativeWindowGDIElementEnvironment(_window))
					,skinListener(0)
				{
				}

				void AttachListener(IGuiSkinListener* listener)
				{
					skinListener=listener;
				}

				void RemoveChild(IGuiSkin* child)
				{
				}

				void InsertChild(int index, IGuiSkin* child)
				{
				}

				int ChildCount()
				{
					return 0;
				}
			};
		}
	}
}

#endif