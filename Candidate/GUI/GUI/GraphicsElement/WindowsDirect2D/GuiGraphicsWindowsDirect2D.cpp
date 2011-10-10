#ifdef GUI_GRAPHICS_RENDERER_DIRECT2D

#include "GuiGraphicsWindowsDirect2D.h"
#include "GuiGraphicsRenderersWindowsDirect2D.h"

namespace vl
{
	namespace presentation
	{
		namespace elements_windows_gdi
		{
			using namespace windows;
			using namespace elements;
			using namespace collections;

/***********************************************************************
WindiwsGDIRenderTarget
***********************************************************************/

			class WindowsDirect2DRenderTarget : public Object, public IWindowsDirect2DRenderTarget, public IWindowsFormGraphicsHandler
			{
			protected:
				INativeWindow*				window;
				ID2D1RenderTarget*			d2dRenderTarget;
				List<Rect>					clippers;
				int							clipperCoverWholeTargetCounter;

				void ApplyClipper()
				{
					if(clipperCoverWholeTargetCounter==0)
					{
						if(clippers.Count()==0)
						{
							//dc->RemoveClip();
						}
						else
						{
							Rect clipper=GetClipper();
							//dc->ClipRegion(new WinRegion(clipper.Left(), clipper.Top(), clipper.Right(), clipper.Bottom(), true));
						}
					}
				}
			public:
				WindowsDirect2DRenderTarget(INativeWindow* _window)
					:window(_window)
					,d2dRenderTarget(0)
					,clipperCoverWholeTargetCounter(0)
				{
				}

				ID2D1RenderTarget* GetDirect2DRenderTarget()
				{
					return d2dRenderTarget?d2dRenderTarget:GetNativeWindowDirect2DRenderTarget(window);
				}

				void StartRendering()
				{
					d2dRenderTarget=GetNativeWindowDirect2DRenderTarget(window);
					d2dRenderTarget->BeginDraw();
				}

				void StopRendering()
				{
					d2dRenderTarget->EndDraw();
					d2dRenderTarget=0;
				}

				void RedrawContent()
				{
				}

				void PushClipper(Rect clipper)
				{
					if(clipperCoverWholeTargetCounter>0)
					{
						clipperCoverWholeTargetCounter++;
					}
					else
					{
						Rect previousClipper=GetClipper();
						Rect currentClipper;

						currentClipper.x1=(previousClipper.x1>clipper.x1?previousClipper.x1:clipper.x1);
						currentClipper.y1=(previousClipper.y1>clipper.y1?previousClipper.y1:clipper.y1);
						currentClipper.x2=(previousClipper.x2<clipper.x2?previousClipper.x2:clipper.x2);
						currentClipper.y2=(previousClipper.y2<clipper.y2?previousClipper.y2:clipper.y2);

						if(currentClipper.x1<currentClipper.x2 && currentClipper.y1<currentClipper.y2)
						{
							clippers.Add(currentClipper);
						}
						else
						{
							clipperCoverWholeTargetCounter++;
						}
					}
					ApplyClipper();
				}

				void PopClipper()
				{
					if(clippers.Count()>0)
					{
						if(clipperCoverWholeTargetCounter>0)
						{
							clipperCoverWholeTargetCounter--;
						}
						else
						{
							clippers.RemoveAt(clippers.Count()-1);
						}
						ApplyClipper();
					}
				}

				Rect GetClipper()
				{
					if(clippers.Count()==0)
					{
						return Rect(Point(0, 0), window->GetClientSize());
					}
					else
					{
						return clippers[clippers.Count()-1];
					}
				}

				bool IsClipperCoverWholeTarget()
				{
					return clipperCoverWholeTargetCounter>0;
				}
			};

/***********************************************************************
WindowsGDIResourceManager
***********************************************************************/

			class WindowsDirect2DResourceManager : public GuiGraphicsResourceManager, public IWindowsDirect2DResourceManager, public INativeControllerListener
			{
			protected:
				SortedList<Ptr<WindowsDirect2DRenderTarget>>		renderTargets;
			public:
				IGuiGraphicsRenderTarget* GetRenderTarget(INativeWindow* window)
				{
					return dynamic_cast<IGuiGraphicsRenderTarget*>(GetWindowsForm(window)->GetGraphicsHandler());
				}

				void NativeWindowCreated(INativeWindow* window)
				{
					WindowsDirect2DRenderTarget* renderTarget=new WindowsDirect2DRenderTarget(window);
					renderTargets.Add(renderTarget);
					GetWindowsForm(window)->SetGraphicsHandler(renderTarget);
				}

				void NativeWindowDestroying(INativeWindow* window)
				{
					IWindowsForm* form=GetWindowsForm(window);
					WindowsDirect2DRenderTarget* renderTarget=dynamic_cast<WindowsDirect2DRenderTarget*>(form->GetGraphicsHandler());
					form->SetGraphicsHandler(0);
					renderTargets.Remove(renderTarget);
				}
			};
		}

		namespace elements
		{
			using namespace elements_windows_gdi;

			IWindowsDirect2DResourceManager* windowsDirect2DResourceManager=0;

			IWindowsDirect2DResourceManager* GetWindowsDirect2DResourceManager()
			{
				return windowsDirect2DResourceManager;
			}

			void SetWindowsDirect2DResourceManager(IWindowsDirect2DResourceManager* resourceManager)
			{
				windowsDirect2DResourceManager=resourceManager;
			}
		}
	}
}

/***********************************************************************
NativeMain
***********************************************************************/

using namespace vl::presentation;
using namespace vl::presentation::elements;
using namespace vl::presentation::elements_windows_d2d;

void NativeMain()
{
	WindowsDirect2DResourceManager resourceManager;
	SetGuiGraphicsResourceManager(&resourceManager);
	SetWindowsDirect2DResourceManager(&resourceManager);
	GetCurrentController()->InstallListener(&resourceManager);

	GuiSolidBorderElementRenderer::Register();
	GuiRoundBorderElementRenderer::Register();
	GuiSolidBackgroundElementRenderer::Register();
	GuiGradientBackgroundElementRenderer::Register();
	GuiSolidLabelElementRenderer::Register();

	GuiMain();
	SetWindowsDirect2DResourceManager(0);
	SetGuiGraphicsResourceManager(0);
}

#endif