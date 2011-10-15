#ifdef GUI_GRAPHICS_RENDERER_DIRECT2D

#include "GuiGraphicsWindowsDirect2D.h"
#include "GuiGraphicsRenderersWindowsDirect2D.h"

namespace vl
{
	namespace presentation
	{
		namespace elements
		{
			using namespace windows;
			using namespace elements;
			using namespace collections;

			D2D1::ColorF GetD2DColor(Color color)
			{
				return D2D1::ColorF(color.r/255.0f, color.g/255.0f, color.b/255.0f, color.a/255.0f);
			}

/***********************************************************************
CachedResourceAllocator
***********************************************************************/

			class CachedSolidBrushAllocator
			{
				DEFINE_CACHED_RESOURCE_ALLOCATOR(Color, ComPtr<ID2D1SolidColorBrush>)

				IWindowsDirect2DRenderTarget*	guiRenderTarget;
			public:
				CachedSolidBrushAllocator()
				{
				}

				void SetRenderTarget(IWindowsDirect2DRenderTarget* _guiRenderTarget)
				{
					guiRenderTarget=_guiRenderTarget;
				}

				ComPtr<ID2D1SolidColorBrush> CreateInternal(Color color)
				{
					ID2D1SolidColorBrush* brush=0;
					ID2D1RenderTarget* renderTarget=guiRenderTarget->GetDirect2DRenderTarget();
					HRESULT hr=renderTarget->CreateSolidColorBrush(GetD2DColor(color), &brush);
					if(!FAILED(hr))
					{
						return brush;
					}
					else
					{
						return 0;
					}
				}
			};

			class CachedLinearBrushAllocator
			{
				typedef Pair<Color, Color> ColorPair;
				DEFINE_CACHED_RESOURCE_ALLOCATOR(ColorPair, ComPtr<ID2D1LinearGradientBrush>)

				IWindowsDirect2DRenderTarget*	guiRenderTarget;
			public:
				CachedLinearBrushAllocator()
				{
				}

				void SetRenderTarget(IWindowsDirect2DRenderTarget* _guiRenderTarget)
				{
					guiRenderTarget=_guiRenderTarget;
				}

				ComPtr<ID2D1LinearGradientBrush> CreateInternal(ColorPair colors)
				{
					ID2D1RenderTarget* renderTarget=guiRenderTarget->GetDirect2DRenderTarget();
					ID2D1GradientStopCollection* stopCollection=0;
					{
						D2D1_GRADIENT_STOP stops[2];
						stops[0].color=GetD2DColor(colors.key);
						stops[0].position=0.0f;
						stops[1].color=GetD2DColor(colors.value);
						stops[1].position=1.0f;

						HRESULT hr=renderTarget->CreateGradientStopCollection(
							stops,
							2,
							D2D1_GAMMA_2_2,
							D2D1_EXTEND_MODE_CLAMP,
							&stopCollection);
						if(FAILED(hr)) return 0;
					}

					ID2D1LinearGradientBrush* brush=0;
					{
						D2D1_POINT_2F points[2]={{0, 0}, {0, 0}};
						HRESULT hr=renderTarget->CreateLinearGradientBrush(
							D2D1::LinearGradientBrushProperties(points[0], points[1]),
							stopCollection,
							&brush);
						stopCollection->Release();
						if(FAILED(hr)) return 0;
					}
					return brush;
				}
			};

			class CachedTextFormatAllocator
			{
				DEFINE_CACHED_RESOURCE_ALLOCATOR(FontProperties, ComPtr<IDWriteTextFormat>)
			public:
				CachedTextFormatAllocator()
				{
				}

				ComPtr<IDWriteTextFormat> CreateInternal(const FontProperties& fontProperties)
				{
					IDWriteFactory* dwriteFactory=GetDirectWriteFactory();
					IDWriteTextFormat* format=0;
					HRESULT hr=dwriteFactory->CreateTextFormat(
						fontProperties.fontFamily.Buffer(),
						NULL,
						(fontProperties.bold?DWRITE_FONT_WEIGHT_BOLD:DWRITE_FONT_WEIGHT_NORMAL),
						(fontProperties.italic?DWRITE_FONT_STYLE_ITALIC:DWRITE_FONT_STYLE_NORMAL),
						DWRITE_FONT_STRETCH_NORMAL,
						(FLOAT)fontProperties.size,
						L"",
						&format);
					if(!FAILED(hr))
					{
						format->SetWordWrapping(DWRITE_WORD_WRAPPING_NO_WRAP);
						return format;
					}
					else
					{
						return 0;
					}
				}
			};

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

				CachedSolidBrushAllocator	solidBrushes;
				CachedLinearBrushAllocator	linearBrushes;
			public:
				WindowsDirect2DRenderTarget(INativeWindow* _window)
					:window(_window)
					,d2dRenderTarget(0)
					,clipperCoverWholeTargetCounter(0)
				{
					solidBrushes.SetRenderTarget(this);
					linearBrushes.SetRenderTarget(this);
				}

				ID2D1RenderTarget* GetDirect2DRenderTarget()
				{
					return d2dRenderTarget?d2dRenderTarget:GetNativeWindowDirect2DRenderTarget(window);
				}

				void StartRendering()
				{
					d2dRenderTarget=GetNativeWindowDirect2DRenderTarget(window);
					d2dRenderTarget->BeginDraw();
					d2dRenderTarget->Clear(D2D1::ColorF(D2D1::ColorF::Black));
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
							d2dRenderTarget->PushAxisAlignedClip(
								D2D1::RectF((FLOAT)currentClipper.x1, (FLOAT)currentClipper.y1, (FLOAT)currentClipper.x2, (FLOAT)currentClipper.y2),
								D2D1_ANTIALIAS_MODE_PER_PRIMITIVE
								);
						}
						else
						{
							clipperCoverWholeTargetCounter++;
						}
					}
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
							d2dRenderTarget->PopAxisAlignedClip();
						}
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

				ID2D1SolidColorBrush* CreateDirect2DBrush(Color color)
				{
					return solidBrushes.Create(color).Obj();
				}

				void DestroyDirect2DBrush(Color color)
				{
					solidBrushes.Destroy(color);
				}

				ID2D1LinearGradientBrush* CreateDirect2DLinearBrush(Color c1, Color c2)
				{
					return linearBrushes.Create(Pair<Color, Color>(c1, c2)).Obj();
				}

				void DestroyDirect2DLinearBrush(Color c1, Color c2)
				{
					linearBrushes.Destroy(Pair<Color, Color>(c1, c2));
				}
			};

/***********************************************************************
WindowsGDIResourceManager
***********************************************************************/

			class WindowsDirect2DResourceManager : public GuiGraphicsResourceManager, public IWindowsDirect2DResourceManager, public INativeControllerListener
			{
			protected:
				SortedList<Ptr<WindowsDirect2DRenderTarget>>		renderTargets;

				CachedTextFormatAllocator							textFormats;
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

				IDWriteTextFormat* CreateDirect2DTextFormat(const FontProperties& fontProperties)
				{
					return textFormats.Create(fontProperties).Obj();
				}

				void DestroyDirect2DTextFormat(const FontProperties& fontProperties)
				{
					textFormats.Destroy(fontProperties);
				}
			};
		}

		namespace elements
		{
			using namespace elements_windows_d2d;

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
	GetCurrentController()->StartTimer();

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