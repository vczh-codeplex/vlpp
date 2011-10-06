#include "GuiGraphicsRenderersWindowsGDI.h"

namespace vl
{
	namespace presentation
	{
		namespace elements_windows_gdi
		{
			using namespace elements;

/***********************************************************************
WindiwsGDIRenderTarget
***********************************************************************/

			class WindiwsGDIRenderTarget : public Object, public IGuiGraphicsRenderTarget
			{
			};

/***********************************************************************
WindowsGDIResourceManager
***********************************************************************/

			class WindowsGDIResourceManager : public GuiGraphicsResourceManager
			{
			public:
				IGuiGraphicsRenderTarget* GetRenderTarget(INativeWindow* window)
				{
					return 0;
				}
			};

/***********************************************************************
Renderers
***********************************************************************/

			class GuiSolidBorderElementRenderer
				: public GuiGraphicsRenderer<GuiSolidBorderElement, GuiSolidBorderElementRenderer, WindiwsGDIRenderTarget>
			{
			public:
				void Render(Rect bounds)
				{
				}

				void OnElementStateChanged()
				{
				}
			};

			class GuiSolidBackgroundElementRenderer
				: public GuiGraphicsRenderer<GuiSolidBackgroundElement, GuiSolidBorderElementRenderer, WindiwsGDIRenderTarget>
			{
			public:
				void Render(Rect bounds)
				{
				}

				void OnElementStateChanged()
				{
				}
			};
		}
	}
}

/***********************************************************************
NativeMain
***********************************************************************/

using namespace vl::presentation::elements;
using namespace vl::presentation::elements_windows_gdi;

void NativeMain()
{
	WindowsGDIResourceManager resourceManager;
	SetGuiGraphicsResourceManager(&resourceManager);

	GuiSolidBorderElementRenderer::Register();
	GuiSolidBackgroundElementRenderer::Register();

	GuiMain();
}