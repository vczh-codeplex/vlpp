/***********************************************************************
Vczh Library++ 3.0
Developer: ³Âè÷å«(vczh)
GUI::Windows Platform

Interfaces:
***********************************************************************/

#ifndef VCZH_PRESENTATION_WINDOWS_GUIGRAPHICSWINDOWSGDI
#define VCZH_PRESENTATION_WINDOWS_GUIGRAPHICSWINDOWSGDI

#include "..\..\NativeWindow\Windows\GDI\WinGDIApplication.h"
#include "..\GuiGraphicsElement.h"
#include "..\GuiGraphicsComposition.h"

namespace vl
{
	namespace presentation
	{
		namespace elements
		{
			class IWindowsGDIRenderTarget : public IGuiGraphicsRenderTarget
			{
			public:
				virtual windows::WinDC*					GetDC()=0;
			};

			class IWindowsGDIResourceManager : public Interface
			{
			public:
				virtual Ptr<windows::WinPen>			CreatePen(Color color)=0;
				virtual void							DestroyPen(Color color)=0;
				virtual Ptr<windows::WinBrush>			CreateBrush(Color color)=0;
				virtual void							DestroyBrush(Color color)=0;
				virtual Ptr<windows::WinFont>			CreateFont(const FontProperties& fontProperties)=0;
				virtual void							DestroyFont(const FontProperties& fontProperties)=0;
			};

			extern IWindowsGDIResourceManager*			GetWindowsGDIResourceManager();
		}
	}
}

#endif