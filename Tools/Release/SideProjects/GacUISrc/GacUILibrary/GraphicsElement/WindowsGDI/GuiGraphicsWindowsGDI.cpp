#include "GuiGraphicsWindowsGDI.h"
#include "GuiGraphicsRenderersWindowsGDI.h"
#include "..\..\Controls\GuiApplication.h"

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

				WinDC* GetDC()override
				{
					return dc?dc:GetNativeWindowDC(window);
				}

				void StartRendering()override
				{
					dc=GetNativeWindowDC(window);
				}

				void StopRendering()override
				{
					dc=0;
				}

				void RedrawContent()override
				{
				}

				void PushClipper(Rect clipper)override
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

				void PopClipper()override
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

				Rect GetClipper()override
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

				bool IsClipperCoverWholeTarget()override
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
					int						size;

					Size MeasureInternal(wchar_t character, IGuiGraphicsRenderTarget* renderTarget)
					{
						if(renderTarget)
						{
							WindowsGDIRenderTarget* gdiRenderTarget=dynamic_cast<WindowsGDIRenderTarget*>(renderTarget);
							WinDC* dc=gdiRenderTarget->GetDC();
							dc->SetFont(font);
							SIZE size=dc->MeasureBuffer(&character, 1, -1);
							return Size(size.cx, size.cy);
						}
						else
						{
							return Size(0, 0);
						}
					}

					int MeasureWidthInternal(wchar_t character, IGuiGraphicsRenderTarget* renderTarget)
					{
						return MeasureInternal(character, renderTarget).x;
					}

					int GetRowHeightInternal(IGuiGraphicsRenderTarget* renderTarget)
					{
						if(renderTarget)
						{
							return MeasureInternal(L' ', renderTarget).y;
						}
						else
						{
							return size;
						}
					}
				public:
					GdiCharMeasurer(Ptr<WinFont> _font, int _size)
						:text::CharMeasurer(_size)
						,size(_size)
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

			class WindowsGDIImageFrameCache : public Object, public INativeImageFrameCache
			{
			protected:
				IWindowsGDIResourceManager*			resourceManager;
				INativeImageFrame*					cachedFrame;
				Ptr<WinBitmap>						bitmap;
			public:
				WindowsGDIImageFrameCache(IWindowsGDIResourceManager* _resourceManager)
					:resourceManager(_resourceManager)
				{
				}

				~WindowsGDIImageFrameCache()
				{
				}

				void OnAttach(INativeImageFrame* frame)override
				{
					cachedFrame=frame;
					Size size=frame->GetSize();
					bitmap=new WinBitmap(size.x, size.y, WinBitmap::vbb32Bits, true);

					IWICBitmap* wicBitmap=GetWICBitmap(frame);
					WICRect rect;
					rect.X=0;
					rect.Y=0;
					rect.Width=size.x;
					rect.Height=size.y;
					wicBitmap->CopyPixels(&rect, bitmap->GetLineBytes(), bitmap->GetLineBytes()*size.y, (BYTE*)bitmap->GetScanLines()[0]);

					bitmap->BuildAlphaChannel(false);
				}
				
				void OnDetach(INativeImageFrame* frame)override
				{
					resourceManager->DestroyBitmapCache(cachedFrame);
				}

				INativeImageFrame* GetFrame()
				{
					return cachedFrame;
				}

				Ptr<WinBitmap> GetBitmap()
				{
					return bitmap;
				}
			};

			class WindowsGDIResourceManager : public GuiGraphicsResourceManager, public IWindowsGDIResourceManager, public INativeControllerListener
			{
				typedef SortedList<Ptr<WindowsGDIImageFrameCache>> ImageCacheList;
			protected:
				SortedList<Ptr<WindowsGDIRenderTarget>>		renderTargets;
				CachedPenAllocator							pens;
				CachedBrushAllocator						brushes;
				CachedFontAllocator							fonts;
				CachedCharMeasurerAllocator					charMeasurers;
				ImageCacheList								imageCaches;
			public:
				IGuiGraphicsRenderTarget* GetRenderTarget(INativeWindow* window)
				{
					return dynamic_cast<IGuiGraphicsRenderTarget*>(GetWindowsForm(window)->GetGraphicsHandler());
				}

				void NativeWindowCreated(INativeWindow* window)override
				{
					WindowsGDIRenderTarget* renderTarget=new WindowsGDIRenderTarget(window);
					renderTargets.Add(renderTarget);
					GetWindowsForm(window)->SetGraphicsHandler(renderTarget);
				}

				void NativeWindowDestroying(INativeWindow* window)override
				{
					IWindowsForm* form=GetWindowsForm(window);
					WindowsGDIRenderTarget* renderTarget=dynamic_cast<WindowsGDIRenderTarget*>(form->GetGraphicsHandler());
					form->SetGraphicsHandler(0);
					renderTargets.Remove(renderTarget);
				}

				Ptr<windows::WinPen> CreateGdiPen(Color color)override
				{
					return pens.Create(color);
				}

				void DestroyGdiPen(Color color)override
				{
					pens.Destroy(color);
				}

				Ptr<windows::WinBrush> CreateGdiBrush(Color color)override
				{
					return brushes.Create(color);
				}

				void DestroyGdiBrush(Color color)override
				{
					brushes.Destroy(color);
				}

				Ptr<windows::WinFont> CreateGdiFont(const FontProperties& fontProperties)override
				{
					return fonts.Create(fontProperties);
				}

				void DestroyGdiFont(const FontProperties& fontProperties)override
				{
					fonts.Destroy(fontProperties);
				}

				Ptr<elements::text::CharMeasurer> CreateCharMeasurer(const FontProperties& fontProperties)override
				{
					return charMeasurers.Create(fontProperties);
				}

				void DestroyCharMeasurer(const FontProperties& fontProperties)override
				{
					charMeasurers.Destroy(fontProperties);
				}

				Ptr<windows::WinBitmap> GetBitmap(INativeImageFrame* frame)override
				{
					Ptr<INativeImageFrameCache> cache=frame->GetCache(this);
					if(cache)
					{
						return cache.Cast<WindowsGDIImageFrameCache>()->GetBitmap();
					}
					else
					{
						WindowsGDIImageFrameCache* gdiCache=new WindowsGDIImageFrameCache(this);
						if(frame->SetCache(this, gdiCache))
						{
							return gdiCache->GetBitmap();
						}
						else
						{
							return 0;
						}
					}
				}

				void DestroyBitmapCache(INativeImageFrame* frame)override
				{
					WindowsGDIImageFrameCache* cache=frame->GetCache(this).Cast<WindowsGDIImageFrameCache>().Obj();
					imageCaches.Remove(cache);
				}
			};
		}

		namespace elements_windows_gdi
		{
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

void RendererMainGDI()
{
	elements_windows_gdi::WindowsGDIResourceManager resourceManager;
	SetGuiGraphicsResourceManager(&resourceManager);
	elements_windows_gdi::SetWindowsGDIResourceManager(&resourceManager);
	GetCurrentController()->CallbackService()->InstallListener(&resourceManager);

	elements_windows_gdi::GuiSolidBorderElementRenderer::Register();
	elements_windows_gdi::GuiRoundBorderElementRenderer::Register();
	elements_windows_gdi::Gui3DBorderElementRenderer::Register();
	elements_windows_gdi::Gui3DSplitterElementRenderer::Register();
	elements_windows_gdi::GuiSolidBackgroundElementRenderer::Register();
	elements_windows_gdi::GuiGradientBackgroundElementRenderer::Register();
	elements_windows_gdi::GuiSolidLabelElementRenderer::Register();
	elements_windows_gdi::GuiImageFrameElementRenderer::Register();
	elements_windows_gdi::GuiPolygonElementRenderer::Register();
	elements_windows_gdi::GuiColorizedTextElementRenderer::Register();

	GuiApplicationMain();
	elements_windows_gdi::SetWindowsGDIResourceManager(0);
	SetGuiGraphicsResourceManager(0);
}

int SetupWindowsGDIRenderer()
{
	HINSTANCE hInstance=(HINSTANCE)GetModuleHandle(NULL);
	return WinMainGDI(hInstance, &RendererMainGDI);
}