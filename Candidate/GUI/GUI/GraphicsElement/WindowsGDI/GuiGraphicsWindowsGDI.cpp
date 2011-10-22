#ifdef GUI_GRAPHICS_RENDERER_GDI

#include "GuiGraphicsWindowsGDI.h"
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

			class WindowsGDIRenderTarget : public Object, public IWindowsGDIRenderTarget, public IWindowsFormGraphicsHandler
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
					return dc?dc:GetNativeWindowDC(window);
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
CachedResourceAllocator
***********************************************************************/

			class CachedPenAllocator
			{
				DEFINE_CACHED_RESOURCE_ALLOCATOR(Color, Ptr<WinPen>)
			public:
				Ptr<WinPen> CreateInternal(Color color)
				{
					return new WinPen(PS_SOLID, 1, RGB(color.r, color.g, color.b));
				}
			};

			class CachedBrushAllocator
			{
				DEFINE_CACHED_RESOURCE_ALLOCATOR(Color, Ptr<WinBrush>)
			public:
				Ptr<WinBrush> CreateInternal(Color color)
				{
					return color.a==0?new WinBrush:new WinBrush(RGB(color.r, color.g, color.b));
				}
			};

			class CachedFontAllocator
			{
				DEFINE_CACHED_RESOURCE_ALLOCATOR(FontProperties, Ptr<WinFont>)
			public:
				static Ptr<WinFont> CreateGdiFont(const FontProperties& value)
				{
					int size=value.size<0?value.size:-value.size;
					return new WinFont(value.fontFamily, size, 0, 0, 0, (value.bold?FW_BOLD:FW_NORMAL), value.italic, value.underline, value.strikeline, value.antialias);
				}

				Ptr<WinFont> CreateInternal(const FontProperties& value)
				{
					return CreateGdiFont(value);
				}
			};

			class CachedCharMeasurerAllocator
			{
				DEFINE_CACHED_RESOURCE_ALLOCATOR(FontProperties, Ptr<text::CharMeasurer>)

			protected:
				class GdiCharMeasurer : public text::CharMeasurer
				{
				protected:
					Ptr<WinFont>			font;

					int MeasureWidthInternal(wchar_t character, IGuiGraphicsRenderTarget* renderTarget)
					{
						if(renderTarget)
						{
							WindowsGDIRenderTarget* gdiRenderTarget=dynamic_cast<WindowsGDIRenderTarget*>(renderTarget);
							return gdiRenderTarget->GetDC()->MeasureBuffer(&character, 1, -1).cx;
						}
						else
						{
							return 0;
						}
					}
				public:
					GdiCharMeasurer(Ptr<WinFont> _font, int _size)
						:text::CharMeasurer(_size)
						,font(_font)
					{
					}
				};
			public:
				Ptr<text::CharMeasurer> CreateInternal(const FontProperties& value)
				{
					return new GdiCharMeasurer(CachedFontAllocator::CreateGdiFont(value), value.size);
				}
			};

/***********************************************************************
WindowsGDIResourceManager
***********************************************************************/

			class WindowsGDIResourceManager : public GuiGraphicsResourceManager, public IWindowsGDIResourceManager, public INativeControllerListener
			{
			protected:
				SortedList<Ptr<WindowsGDIRenderTarget>>		renderTargets;
				CachedPenAllocator							pens;
				CachedBrushAllocator						brushes;
				CachedFontAllocator							fonts;
				CachedCharMeasurerAllocator					charMeasurers;
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

				Ptr<windows::WinPen> CreateGdiPen(Color color)
				{
					return pens.Create(color);
				}

				void DestroyGdiPen(Color color)
				{
					pens.Destroy(color);
				}

				Ptr<windows::WinBrush> CreateGdiBrush(Color color)
				{
					return brushes.Create(color);
				}

				void DestroyGdiBrush(Color color)
				{
					brushes.Destroy(color);
				}

				Ptr<windows::WinFont> CreateGdiFont(const FontProperties& fontProperties)
				{
					return fonts.Create(fontProperties);
				}

				void DestroyGdiFont(const FontProperties& fontProperties)
				{
					fonts.Destroy(fontProperties);
				}

				Ptr<elements::text::CharMeasurer> CreateCharMeasurer(const FontProperties& fontProperties)
				{
					return charMeasurers.Create(fontProperties);
				}

				void DestroyCharMeasurer(const FontProperties& fontProperties)
				{
					charMeasurers.Destroy(fontProperties);
				}
			};
		}

		namespace elements
		{
			using namespace elements_windows_gdi;

			IWindowsGDIResourceManager* windowsGDIResourceManager=0;

			IWindowsGDIResourceManager* GetWindowsGDIResourceManager()
			{
				return windowsGDIResourceManager;
			}

			void SetWindowsGDIResourceManager(IWindowsGDIResourceManager* resourceManager)
			{
				windowsGDIResourceManager=resourceManager;
			}
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
	SetWindowsGDIResourceManager(&resourceManager);
	GetCurrentController()->InstallListener(&resourceManager);
	GetCurrentController()->StartTimer();

	GuiSolidBorderElementRenderer::Register();
	GuiRoundBorderElementRenderer::Register();
	Gui3DBorderElementRenderer::Register();
	GuiSolidBackgroundElementRenderer::Register();
	GuiGradientBackgroundElementRenderer::Register();
	GuiSolidLabelElementRenderer::Register();
	GuiColorizedTextElementRenderer::Register();

	GuiMain();
	SetWindowsGDIResourceManager(0);
	SetGuiGraphicsResourceManager(0);
}

#endif