#pragma comment(lib, "d2d1.lib")
#pragma comment(lib, "dwrite.lib")

#include "WinDirect2DApplication.h"

namespace vl
{
	namespace presentation
	{
		namespace windows
		{
			using namespace vl::collections;

			class Direct2DWindowsNativeWindowListener : public Object, public INativeWindowListener
			{
			protected:
				ID2D1Factory*					d2dFactory;
				ComPtr<ID2D1HwndRenderTarget>	d2dRenderTarget;
				INativeWindow*					window;
				Size							previousSize;

				void RebuildCanvas(Size size)
				{
					if(size.x==0) size.x=1;
					if(size.y==0) size.y=1;
					if(!d2dRenderTarget)
					{
						ID2D1HwndRenderTarget* renderTarget=0;
						IWindowsForm* form=GetWindowsForm(window);
						HRESULT hr=d2dFactory->CreateHwndRenderTarget(
							D2D1::RenderTargetProperties(),
							D2D1::HwndRenderTargetProperties(
								form->GetWindowHandle(),
								D2D1::SizeU(size.x, size.y)
								),
							&renderTarget
							);
						if(!FAILED(hr))
						{
							d2dRenderTarget=renderTarget;
							d2dRenderTarget->SetTextAntialiasMode(D2D1_TEXT_ANTIALIAS_MODE_CLEARTYPE);

							IDWriteFactory* dwriteFactory=GetDirectWriteFactory();
							IDWriteRenderingParams* defaultParams=0;
							hr=dwriteFactory->CreateRenderingParams(&defaultParams);
							if(!FAILED(hr))
							{
								IDWriteRenderingParams* bidirectionalTextParams=0;
								FLOAT gamma=defaultParams->GetGamma();
								FLOAT enhancedContrast=defaultParams->GetEnhancedContrast();
								FLOAT clearTypeLevel=defaultParams->GetClearTypeLevel();
								DWRITE_PIXEL_GEOMETRY pixelGeometry=defaultParams->GetPixelGeometry();
								DWRITE_RENDERING_MODE renderingMode=DWRITE_RENDERING_MODE_CLEARTYPE_NATURAL_SYMMETRIC;
								hr=dwriteFactory->CreateCustomRenderingParams(
									gamma,
									enhancedContrast,
									clearTypeLevel,
									pixelGeometry,
									renderingMode,
									&bidirectionalTextParams);
								if(!FAILED(hr))
								{
									d2dRenderTarget->SetTextRenderingParams(bidirectionalTextParams);
									bidirectionalTextParams->Release();
								}
								defaultParams->Release();
							}
						}
					}
					else if(previousSize!=size)
					{
						d2dRenderTarget->Resize(D2D1::SizeU(size.x, size.y));
					}
					previousSize=size;
				}
			public:
				Direct2DWindowsNativeWindowListener(INativeWindow* _window, ID2D1Factory* _d2dFactory)
					:window(_window)
					,d2dFactory(_d2dFactory)
					,d2dRenderTarget(0)
				{
				}

				void Moved()
				{
					RebuildCanvas(window->GetClientSize());
				}

				void Paint()
				{
					IWindowsForm* form=GetWindowsForm(window);
					form->GetGraphicsHandler()->RedrawContent();
				}

				ID2D1RenderTarget* GetDirect2DRenderTarget()
				{
					if(!d2dRenderTarget) Moved();
					return d2dRenderTarget.Obj();
				}
			};

			class Direct2DWindowsNativeControllerListener : public Object, public INativeControllerListener
			{
			public:
				Dictionary<INativeWindow*, Ptr<Direct2DWindowsNativeWindowListener>>		nativeWindowListeners;
				ComPtr<ID2D1Factory>														d2dFactory;
				ComPtr<IDWriteFactory>														dwrite;

				Direct2DWindowsNativeControllerListener()
				{
					{
						ID2D1Factory* factory=0;
						HRESULT hr=D2D1CreateFactory(D2D1_FACTORY_TYPE_SINGLE_THREADED, &factory);
						if(!FAILED(hr))
						{
							d2dFactory=factory;
						}
					}
					{
						IDWriteFactory* factory=0;
						HRESULT hr=DWriteCreateFactory(DWRITE_FACTORY_TYPE_ISOLATED, __uuidof(IDWriteFactory), reinterpret_cast<IUnknown**>(&factory));
						if(!FAILED(hr))
						{
							dwrite=factory;
						}
					}
				}

				void NativeWindowCreated(INativeWindow* window)
				{
					Ptr<Direct2DWindowsNativeWindowListener> listener=new Direct2DWindowsNativeWindowListener(window, d2dFactory.Obj());
					window->InstallListener(listener.Obj());
					nativeWindowListeners.Add(window, listener);
				}

				void NativeWindowDestroying(INativeWindow* window)
				{
					Ptr<Direct2DWindowsNativeWindowListener> listener=nativeWindowListeners[window];
					nativeWindowListeners.Remove(window);
					window->UninstallListener(listener.Obj());
				}
			};

			Direct2DWindowsNativeControllerListener* direct2DListener=0;

			ID2D1RenderTarget* GetNativeWindowDirect2DRenderTarget(INativeWindow* window)
			{
				int index=direct2DListener->nativeWindowListeners.Keys().IndexOf(window);
				return index==-1?0:direct2DListener->nativeWindowListeners.Values()[index]->GetDirect2DRenderTarget();
			}

			ID2D1Factory* GetDirect2DFactory()
			{
				return direct2DListener->d2dFactory.Obj();
			}

			IDWriteFactory* GetDirectWriteFactory()
			{
				return direct2DListener->dwrite.Obj();
			}
		}
	}
}

using namespace vl;
using namespace vl::presentation;
using namespace vl::presentation::windows;

int WinMainDirect2D(HINSTANCE hInstance, void(*RendererMain)())
{
	CoInitializeEx(NULL, COINIT_MULTITHREADED);
	// create controller
	INativeController* controller=CreateWindowsNativeController(hInstance);
	SetCurrentController(controller);
	{
		// install listener
		Direct2DWindowsNativeControllerListener listener;
		controller->CallbackService()->InstallListener(&listener);
		direct2DListener=&listener;
		// main
		RendererMain();
		// uninstall listener
		direct2DListener=0;
		controller->CallbackService()->UninstallListener(&listener);
	}
	// destroy controller
	DestroyWindowsNativeController(controller);
	return 0;
}