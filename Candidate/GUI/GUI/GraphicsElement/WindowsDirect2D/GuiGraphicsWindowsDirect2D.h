/***********************************************************************
Vczh Library++ 3.0
Developer: ³Âè÷å«(vczh)
GacUI::Native Window::Direct2D Provider for Windows Implementation::Renderer

Interfaces:
***********************************************************************/

#ifndef VCZH_PRESENTATION_ELEMENTS_GUIGRAPHICSWINDOWSDIRECT2D
#define VCZH_PRESENTATION_ELEMENTS_GUIGRAPHICSWINDOWSDIRECT2D

#include "..\..\NativeWindow\Windows\Direct2D\WinDirect2DApplication.h"
#include "..\GuiGraphicsElement.h"
#include "..\GuiGraphicsComposition.h"

namespace vl
{
	namespace presentation
	{
		namespace elements
		{
			class IWindowsDirect2DRenderTarget : public IGuiGraphicsRenderTarget
			{
			public:
				virtual ID2D1RenderTarget*					GetDirect2DRenderTarget()=0;
				virtual ComPtr<ID2D1Bitmap>					GetBitmap(INativeImageFrame* frame)=0;
				virtual void								DestroyBitmapCache(INativeImageFrame* frame)=0;

				virtual ID2D1SolidColorBrush*				CreateDirect2DBrush(Color color)=0;
				virtual void								DestroyDirect2DBrush(Color color)=0;
				virtual ID2D1LinearGradientBrush*			CreateDirect2DLinearBrush(Color c1, Color c2)=0;
				virtual void								DestroyDirect2DLinearBrush(Color c1, Color c2)=0;
			};

			class IWindowsDirect2DResourceManager : public Interface
			{
			public:
				virtual IDWriteTextFormat*					CreateDirect2DTextFormat(const FontProperties& fontProperties)=0;
				virtual void								DestroyDirect2DTextFormat(const FontProperties& fontProperties)=0;
				virtual Ptr<elements::text::CharMeasurer>	CreateDirect2DCharMeasurer(const FontProperties& fontProperties)=0;
				virtual void								DestroyDirect2DCharMeasurer(const FontProperties& fontProperties)=0;
			};

			extern IWindowsDirect2DResourceManager*			GetWindowsDirect2DResourceManager();

			extern D2D1::ColorF								GetD2DColor(Color color);
		}
	}
}

#endif