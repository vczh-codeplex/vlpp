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
				virtual Ptr<windows::WinPen>			CreateGdiPen(Color color)=0;
				virtual void							DestroyGdiPen(Color color)=0;
				virtual Ptr<windows::WinBrush>			CreateGdiBrush(Color color)=0;
				virtual void							DestroyGdiBrush(Color color)=0;
				virtual Ptr<windows::WinFont>			CreateGdiFont(const FontProperties& fontProperties)=0;
				virtual void							DestroyGdiFont(const FontProperties& fontProperties)=0;
			};

			extern IWindowsGDIResourceManager*			GetWindowsGDIResourceManager();
		}
	}
}

#endif