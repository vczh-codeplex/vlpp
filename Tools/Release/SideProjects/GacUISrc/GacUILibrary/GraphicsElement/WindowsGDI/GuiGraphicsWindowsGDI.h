/***********************************************************************
Vczh Library++ 3.0
Developer: �����(vczh)
GacUI::Native Window::GDI Provider for Windows Implementation::Renderer

Interfaces:
***********************************************************************/

#ifndef VCZH_PRESENTATION_ELEMENTS_GUIGRAPHICSWINDOWSGDI
#define VCZH_PRESENTATION_ELEMENTS_GUIGRAPHICSWINDOWSGDI

#include "..\..\NativeWindow\Windows\GDI\WinGDIApplication.h"
#include "..\GuiGraphicsElement.h"
#include "..\GuiGraphicsTextElement.h"
#include "..\GuiGraphicsComposition.h"

namespace vl
{
	namespace presentation
	{
		namespace elements_windows_gdi
		{
			class IWindowsGDIRenderTarget : public elements::IGuiGraphicsRenderTarget
			{
			public:
				virtual windows::WinDC*					GetDC()=0;
			};

			class IWindowsGDIResourceManager : public Interface
			{
			public:
				virtual Ptr<windows::WinPen>				CreateGdiPen(Color color)=0;
				virtual void								DestroyGdiPen(Color color)=0;
				virtual Ptr<windows::WinBrush>				CreateGdiBrush(Color color)=0;
				virtual void								DestroyGdiBrush(Color color)=0;
				virtual Ptr<windows::WinFont>				CreateGdiFont(const FontProperties& fontProperties)=0;
				virtual void								DestroyGdiFont(const FontProperties& fontProperties)=0;
				virtual Ptr<elements::text::CharMeasurer>	CreateCharMeasurer(const FontProperties& fontProperties)=0;
				virtual void								DestroyCharMeasurer(const FontProperties& fontProperties)=0;

				virtual Ptr<windows::WinBitmap>				GetBitmap(INativeImageFrame* frame)=0;
				virtual void								DestroyBitmapCache(INativeImageFrame* frame)=0;
			};

			extern IWindowsGDIResourceManager*			GetWindowsGDIResourceManager();
		}
	}
}

#endif