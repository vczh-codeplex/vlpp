#ifdef GUI_GRAPHICS_RENDERER_DIRECT2D

#pragma comment(lib, "d2d1.lib")
#pragma comment(lib, "dwrite.lib")

#if _DEBUG
#define _CRTDBG_MAP_ALLOC
#include <stdlib.h>
#include <crtdbg.h>
#endif

#include "WinDirect2DApplication.h"
#include "..\..\..\..\..\..\Library\Pointer.h"
#include "..\..\..\..\..\..\Library\Collections\Dictionary.h"

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
				ID2D1HwndRenderTarget*			d2dRenderTarget;
				INativeWindow*					window;
				Size							previousSize;

				int DetermineBufferLength(int minSize, int minBound, int maxBound, int currentSize)
				{
					if(currentSize<minSize || currentSize>maxBound)
					{
						return minBound;
					}
					else
					{
						return currentSize;
					}
				}

				Size CalculateBufferSize()
				{
					Size windowSize=window->GetClientSize();
					Size minBounds(windowSize.x*5/4, windowSize.y*5/4);
					Size maxBounds(windowSize.x*3/2, windowSize.y*3/2);
					int newWidth=DetermineBufferLength(windowSize.x, minBounds.x, maxBounds.x, previousSize.x);
					int newHeight=DetermineBufferLength(windowSize.y, minBounds.y, maxBounds.y, previousSize.y);
					return Size(newWidth, newHeight);
				}

				void RebuildCanvas(Size size)
				{
					if(size.x<256)size.x=256;
					if(size.y<256)size.y=256;
					if(!d2dRenderTarget)
					{
						IWindowsForm* form=GetWindowsForm(window);
						HRESULT hr=d2dFactory->CreateHwndRenderTarget(
							D2D1::RenderTargetProperties(),
							D2D1::HwndRenderTargetProperties(
								form->GetWindowHandle(),
								D2D1::SizeU(size.x, size.y)
								),
							&d2dRenderTarget
							);
						if(FAILED(hr))
						{
							d2dRenderTarget=0;
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

				~Direct2DWindowsNativeWindowListener()
				{
					if(d2dRenderTarget)
					{
						d2dRenderTarget->Release();
					}
				}

				void Moved()
				{
					RebuildCanvas(CalculateBufferSize());
				}

				void Paint()
				{
					IWindowsForm* form=GetWindowsForm(window);
					form->GetGraphicsHandler()->RedrawContent();
				}

				ID2D1RenderTarget* GetDirect2DRenderTarget()
				{
					if(!d2dRenderTarget) Moved();
					return d2dRenderTarget;
				}
			};

			class Direct2DWindowsNativeControllerListener : public Object, public INativeControllerListener
			{
			public:
				Dictionary<INativeWindow*, Ptr<Direct2DWindowsNativeWindowListener>>		nativeWindowListeners;
				ID2D1Factory*																d2dFactory;

				Direct2DWindowsNativeControllerListener()
					:d2dFactory(0)
				{
					HRESULT hr=D2D1CreateFactory(D2D1_FACTORY_TYPE_SINGLE_THREADED, &d2dFactory);
					if(FAILED(hr))
					{
						d2dFactory=0;
					}
				}

				~Direct2DWindowsNativeControllerListener()
				{
					if(d2dFactory)
					{
						d2dFactory->Release();
					}
				}

				void NativeWindowCreated(INativeWindow* window)
				{
					Ptr<Direct2DWindowsNativeWindowListener> listener=new Direct2DWindowsNativeWindowListener(window, d2dFactory);
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
		}
	}
}

using namespace vl;
using namespace vl::presentation;
using namespace vl::presentation::windows;

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow)
{
	// create controller
	INativeController* controller=CreateWindowsNativeController(hInstance);
	SetCurrentController(controller);
	{
		// install listener
		Direct2DWindowsNativeControllerListener listener;
		controller->InstallListener(&listener);
		direct2DListener=&listener;
		// main
		NativeMain();
		// uninstall listener
		direct2DListener=0;
		controller->UninstallListener(&listener);
	}
	// destroy controller
	DestroyWindowsNativeController(controller);
#if _DEBUG
	_CrtDumpMemoryLeaks();
#endif
	return 0;
}

#endif