#include "GuiGraphicsRenderersWindowsGDI.h"

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

			class WindowsGDIRenderTarget : public Object, public IGuiGraphicsRenderTarget, public IWindowsFormGraphicsHandler
			{
			protected:
				INativeWindow*				window;
				WinDC*						dc;
				List<Rect>					clippers;
				int							clipperCoverWholeTargetCounter;

				void ApplyClipper()
				{
					if(clipperCoverWholeTargetCounter==0)
					{
						if(clippers.Count()==0)
						{
							dc->RemoveClip();
						}
						else
						{
							Rect clipper=GetClipper();
							dc->ClipRegion(new WinRegion(clipper.Left(), clipper.Top(), clipper.Right(), clipper.Bottom(), true));
						}
					}
				}
			public:
				WindowsGDIRenderTarget(INativeWindow* _window)
					:window(_window)
					,dc(0)
					,clipperCoverWholeTargetCounter(0)
				{
				}

				WinDC* GetDC()
				{
					return dc;
				}

				void StartRendering()
				{
					dc=GetNativeWindowDC(window);
				}

				void StopRendering()
				{
					dc=0;
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
						currentClipper.x2=(previousClipper.x2>clipper.x2?previousClipper.x2:clipper.x2);
						currentClipper.y2=(previousClipper.y2>clipper.y2?previousClipper.y2:clipper.y2);

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

			class WindowsGDIResourceManager : public GuiGraphicsResourceManager, public INativeControllerListener
			{
			protected:
				SortedList<Ptr<WindowsGDIRenderTarget>>		renderTargets;
			public:
				IGuiGraphicsRenderTarget* GetRenderTarget(INativeWindow* window)
				{
					return dynamic_cast<IGuiGraphicsRenderTarget*>(GetWindowsForm(window)->GetGraphicsHandler());
				}

				void NativeWindowCreated(INativeWindow* window)
				{
					WindowsGDIRenderTarget* renderTarget=new WindowsGDIRenderTarget(window);
					renderTargets.Add(renderTarget);
					GetWindowsForm(window)->SetGraphicsHandler(renderTarget);
				}

				void NativeWindowDestroying(INativeWindow* window)
				{
					IWindowsForm* form=GetWindowsForm(window);
					WindowsGDIRenderTarget* renderTarget=dynamic_cast<WindowsGDIRenderTarget*>(form->GetGraphicsHandler());
					form->SetGraphicsHandler(0);
					renderTargets.Remove(renderTarget);
				}
			};

/***********************************************************************
Renderers
***********************************************************************/

			class GuiSolidBorderElementRenderer
				: public GuiGraphicsRenderer<GuiSolidBorderElement, GuiSolidBorderElementRenderer, WindowsGDIRenderTarget>
			{
			public:
				void Render(Rect bounds)
				{
					Color color=element->GetColor();
					if(color.a>0)
					{
						// TODO: cache gdi resource
						Ptr<WinBrush> brush=new WinBrush;
						Ptr<WinPen> pen=new WinPen(PS_SOLID, 1, RGB(color.r, color.g, color.b));
						renderTarget->GetDC()->SetBrush(brush);
						renderTarget->GetDC()->SetPen(pen);
						renderTarget->GetDC()->Rectangle(bounds.Left(), bounds.Top(), bounds.Right()-1, bounds.Bottom()-1);
					}
				}

				void OnElementStateChanged()
				{
				}
			};

			class GuiSolidBackgroundElementRenderer
				: public GuiGraphicsRenderer<GuiSolidBackgroundElement, GuiSolidBackgroundElementRenderer, WindowsGDIRenderTarget>
			{
			public:
				void Render(Rect bounds)
				{
					Color color=element->GetColor();
					if(color.a>0)
					{
						// TODO: cache gdi resource
						Ptr<WinBrush> brush=new WinBrush(RGB(color.r, color.g, color.b));
						renderTarget->GetDC()->SetBrush(brush);
						renderTarget->GetDC()->FillRect(bounds.Left(), bounds.Top(), bounds.Right(), bounds.Bottom());
					}
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

using namespace vl::presentation;
using namespace vl::presentation::elements;
using namespace vl::presentation::elements_windows_gdi;

void NativeMain()
{
	WindowsGDIResourceManager resourceManager;
	SetGuiGraphicsResourceManager(&resourceManager);
	GetCurrentController()->InstallListener(&resourceManager);

	GuiSolidBorderElementRenderer::Register();
	GuiSolidBackgroundElementRenderer::Register();

	GuiMain();
}