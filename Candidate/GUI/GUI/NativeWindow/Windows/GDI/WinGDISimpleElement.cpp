#include "WinGDISimpleElement.h"
#include "WinGDIApplication.h"

namespace vl
{
	namespace presentation
	{
		namespace elements
		{
			using namespace vl::presentation::windows;

/***********************************************************************
WinGDISimpleResources
***********************************************************************/

			class WinGDISimpleResourcesAutoInstaller : public INativeControllerListener
			{
			public:
				void NativeWindowCreated(INativeWindow* window)
				{
					WinGDIElementEnvironment* environment=GetNativeWindowGDIElementEnvironment(window);
					if(environment)
					{
						WinGDISimpleResources::Install(environment);
					}
				}

				void NativeWindowDestroying(INativeWindow* window)
				{
					WinGDIElementEnvironment* environment=GetNativeWindowGDIElementEnvironment(window);
					if(environment)
					{
						WinGDISimpleResources::Uninstall(environment);
					}
				}
			} winGDISimpleResourcesAutoInstaller;
			
			void WinGDISimpleResources::RegisterAutoInstall()
			{
				GetCurrentController()->InstallListener(&winGDISimpleResourcesAutoInstaller);
			}

			void WinGDISimpleResources::UnregisterAutoInstall()
			{
				GetCurrentController()->UninstallListener(&winGDISimpleResourcesAutoInstaller);
			}

			void WinGDISimpleResources::Install(WinGDIElementEnvironment* environment)
			{
			}

			void WinGDISimpleResources::Uninstall(WinGDIElementEnvironment* environment)
			{
			}

			Ptr<WinPen> WinGDISimpleResources::GetPen(WinGDIElementEnvironment* environment, const WString& name)
			{
				return environment->Resource(name).Cast<WinPen>();
			}

			Ptr<WinBrush> WinGDISimpleResources::GetBrush(WinGDIElementEnvironment* environment, const WString& name)
			{
				return environment->Resource(name).Cast<WinBrush>();
			}
		}
	}
}