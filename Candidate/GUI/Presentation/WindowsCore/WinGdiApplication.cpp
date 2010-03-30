#include "WinGdiApplication.h"
#include "WinNativeCanvas.h"
#include "WinNativeWindow.h"
#include "..\..\..\..\Library\Collections\Dictionary.h"
#include "..\..\..\..\Library\Pointer.h"

namespace vl
{
	namespace presentation
	{
		namespace windows
		{
			using namespace vl::collections;

			HINSTANCE hInstance=0;

/***********************************************************************
GdiWindowPackage
***********************************************************************/

			class GdiWindowPackage : public Object, private INativeWindowListener
			{
			protected:
				INativeWindow*							nativeWindow;
				IWindowsForm*							windowsForm;

				INativeGraphics*						nativeGraphics;
				INativeBitmap*							nativeBitmap;
				INativeCanvas*							nativeCanvas;
				IGdiCanvas*								gdiCanvas;

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
					Size windowSize=nativeWindow->GetClientSize();
					Size minBounds(windowSize.x*5/4, windowSize.y*5/4);
					Size maxBounds(windowSize.x*3/2, windowSize.y*3/2);
					Size currentSize=nativeBitmap?Size(nativeBitmap->GetWidth(), nativeBitmap->GetHeight()):Size(0, 0);
					int newWidth=DetermineBufferLength(windowSize.x, minBounds.x, maxBounds.x, currentSize.x);
					int newHeight=DetermineBufferLength(windowSize.y, minBounds.y, maxBounds.y, currentSize.y);
					return Size(newWidth, newHeight);
				}

				void RebuildCanvas(Size size)
				{
					if(nativeBitmap)
					{
						if(nativeBitmap->GetWidth()!=size.x || nativeBitmap->GetHeight()!=size.y)
						{
							nativeGraphics->Destroy(nativeBitmap);
							nativeBitmap=0;
							nativeCanvas=0;
							gdiCanvas=0;
						}
					}
					if(!nativeBitmap)
					{
						nativeBitmap=nativeGraphics->CreateBitmap(size.x, size.y);
						nativeCanvas=nativeBitmap->Lock();
						gdiCanvas=GetGdiCanvas(nativeCanvas);
					}
				}

				void Moved()
				{
					RebuildCanvas(CalculateBufferSize());
				}

				void Paint()
				{
				}
			public:
				GdiWindowPackage(INativeWindow* window, INativeGraphics* graphics)
					:nativeWindow(window)
					,nativeGraphics(graphics)
					,nativeBitmap(0)
					,nativeCanvas(0)
					,gdiCanvas(0)
				{
					windowsForm=GetWindowsForm(nativeWindow);
					Moved();
				}

				~GdiWindowPackage()
				{
					nativeGraphics->Destroy(nativeBitmap);
				}

				void Install()
				{
					nativeWindow->InstallListener(this);
				}

				void Uninstall()
				{
					nativeWindow->UninstallListener(this);
				}

				INativeCanvas* GetCanvas()
				{
					return nativeCanvas;
				}
			};

/***********************************************************************
GdiApplication
***********************************************************************/

			class GdiApplication : public Object, public INativeApplication
			{
			protected:
				INativeController*									controller;
				INativeGraphics*									graphics;
				Dictionary<INativeWindow*, Ptr<GdiWindowPackage>>	windowPackages;
			public:
				GdiApplication()
				{
					controller=CreateWindowsNativeController(hInstance);
					graphics=CreateWindowsGdiGraphics();
					SetCurrentController(controller);
				}

				~GdiApplication()
				{
					windowPackages.Clear();
					DestroyWindowsGdiGraphics(graphics);
					DestroyWindowsNativeController(controller);
				}

				INativeWindow* CreateNativeWindow()
				{
					INativeWindow* window=controller->CreateNativeWindow();
					Ptr<GdiWindowPackage> package=new GdiWindowPackage(window, graphics);
					windowPackages.Add(window, package);
					package->Install();
					return window;
				}

				void DestroyNativeWindow(INativeWindow* window)
				{
					Ptr<GdiWindowPackage> package=windowPackages[window];
					package->Uninstall();
					windowPackages.Remove(window);
					controller->DestroyNativeWindow(window);
				}

				bool InstallListener(INativeControllerListener* listener)
				{
					return controller->InstallListener(listener);
				}

				bool UninstallListener(INativeControllerListener* listener)
				{
					return controller->UninstallListener(listener);
				}

				INativeGraphics* GetGraphics()
				{
					return graphics;
				}

				INativeCanvas* LockWindow(INativeWindow* window)
				{
					return windowPackages[window]->GetCanvas();
				}

				void UnlockWindow()
				{
				}

				void Run(INativeWindow* window)
				{
					controller->SetMainWindow(window);
					MSG message;
					while(GetMessage(&message, NULL, 0, 0))
					{
						TranslateMessage(&message);
						DispatchMessage(&message);
					}
				}
			};

/***********************************************************************
Windows GDI Application
***********************************************************************/

			INativeApplication* CreateGdiApplication()
			{
				return new GdiApplication();
			}

			void DestroyGdiApplication(INativeApplication* application)
			{
				delete application;
			}
		}
	}
}

/***********************************************************************
WinMain
***********************************************************************/
			
int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow)
{
	vl::presentation::windows::hInstance=hInstance;
	NativeMain();
	return 0;
}