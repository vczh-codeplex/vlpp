#include "WinGdiApplication.h"
#include "WinNativeCanvas.h"
#include "WinNativeWindow.h"

namespace vl
{
	namespace presentation
	{
		namespace windows
		{
			HINSTANCE hInstance=0;

/***********************************************************************
GdiApplication
***********************************************************************/

			class GdiApplication : public Object, public INativeApplication
			{
			protected:
				INativeController*				controller;
				INativeGraphics*				graphics;

			public:
				GdiApplication()
				{
					controller=CreateWindowsNativeController(hInstance);
					graphics=CreateWindowsGdiGraphics();
					SetCurrentController(controller);
				}

				~GdiApplication()
				{
					DestroyWindowsGdiGraphics(graphics);
					DestroyWindowsNativeController(controller);
				}

				INativeWindow* CreateNativeWindow()
				{
					return controller->CreateNativeWindow();
				}

				void DestroyNativeWindow(INativeWindow* window)
				{
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
					return 0;
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