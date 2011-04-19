#if _DEBUG
#define _CRTDBG_MAP_ALLOC
#include <stdlib.h>
#include <crtdbg.h>
#endif

#include "..\WinNativeWindow.h"
#include "WinGDI.h"
#include "..\..\..\..\..\..\Library\Collections\Dictionary.h"

namespace vl
{
	namespace presentation
	{
		namespace windows
		{
			using namespace vl::collections;
			using namespace vl::windows;

			class GdiWindowsNativeWindowListener : public Object, public INativeWindowListener
			{
			protected:
				Ptr<WinBitmap>					buffer;
				INativeWindow*					window;

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
					Size currentSize=buffer?Size(buffer->GetWidth(), buffer->GetHeight()):Size(0, 0);
					int newWidth=DetermineBufferLength(windowSize.x, minBounds.x, maxBounds.x, currentSize.x);
					int newHeight=DetermineBufferLength(windowSize.y, minBounds.y, maxBounds.y, currentSize.y);
					return Size(newWidth, newHeight);
				}

				void RebuildCanvas(Size size)
				{
					if(size.x<256)size.x=256;
					if(size.y<256)size.y=256;
					if(buffer)
					{
						if(buffer->GetWidth()!=size.x || buffer->GetHeight()!=size.y)
						{
							buffer=0;
						}
					}
					if(!buffer)
					{
						buffer=new WinBitmap(size.x, size.y, WinBitmap::vbb32Bits, true);
					}
				}
			public:
				GdiWindowsNativeWindowListener(INativeWindow* _window)
					:window(_window)
				{
				}

				void Moved()
				{
					RebuildCanvas(CalculateBufferSize());
				}

				void Paint()
				{
					IWindowsForm* form=GetWindowsForm(window);
					WinControlDC controlDC(form->GetWindowHandle());
					controlDC.Draw(0, 0, buffer);
				}

				WinDC* GetWinDC()
				{
					return buffer->GetWinDC();
				}
			};

			class GdiWindowsNativeControllerListener : public Object, public INativeControllerListener
			{
			public:
				Dictionary<INativeWindow*, Ptr<GdiWindowsNativeWindowListener>>		nativeWindowListeners;

				void NativeWindowCreated(INativeWindow* window)
				{
					Ptr<GdiWindowsNativeWindowListener> listener=new GdiWindowsNativeWindowListener(window);
					window->InstallListener(listener.Obj());
					nativeWindowListeners.Add(window, listener);
				}

				void NativeWindowDestroying(INativeWindow* window)
				{
					Ptr<GdiWindowsNativeWindowListener> listener=nativeWindowListeners[window];
					nativeWindowListeners.Remove(window);
					window->UninstallListener(listener.Obj());
				}
			};

			GdiWindowsNativeControllerListener* gdiListener=0;

			WinDC* GetNativeWindowDC(INativeWindow* window)
			{
				return gdiListener->nativeWindowListeners[window]->GetWinDC();
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
		GdiWindowsNativeControllerListener listener;
		controller->InstallListener(&listener);
		gdiListener=&listener;
		// main
		NativeMain();
		// uninstall listener
		gdiListener=0;
		controller->UninstallListener(&listener);
	}
	// destroy controller
	DestroyWindowsNativeController(controller);
#if _DEBUG
	_CrtDumpMemoryLeaks();
#endif
	return 0;
}