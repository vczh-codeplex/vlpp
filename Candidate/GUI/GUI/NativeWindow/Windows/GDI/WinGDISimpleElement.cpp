#include "WinGDISimpleElement.h"
#include "WinGDIApplication.h"

namespace vl
{
	namespace presentation
	{
		namespace elements
		{
			namespace gdi_simple
			{
				using namespace vl::presentation::windows;

/***********************************************************************
Resources
***********************************************************************/

#define RESOURCE_NAME(NAME) const wchar_t* const Resources::NAME = L#NAME

				RESOURCE_NAME(PUSHABLE_BACKGROUND_NORMAL_PEN);
				RESOURCE_NAME(PUSHABLE_BACKGROUND_ACTIVE_PEN);
				RESOURCE_NAME(PUSHABLE_BACKGROUND_PRESSED_PEN);
				RESOURCE_NAME(PUSHABLE_BACKGROUND_DISABLED_PEN);

				RESOURCE_NAME(PUSHABLE_BACKGROUND_NORMAL_BRUSH);
				RESOURCE_NAME(PUSHABLE_BACKGROUND_ACTIVE_BRUSH);
				RESOURCE_NAME(PUSHABLE_BACKGROUND_PRESSED_BRUSH);
				RESOURCE_NAME(PUSHABLE_BACKGROUND_DISABLED_BRUSH);

				RESOURCE_NAME(SELECTABLE_BACKGROUND_NORMAL_PEN);
				RESOURCE_NAME(SELECTABLE_BACKGROUND_ACTIVE_PEN);
				RESOURCE_NAME(SELECTABLE_BACKGROUND_PRESSED_PEN);
				RESOURCE_NAME(SELECTABLE_BACKGROUND_DISABLED_PEN);

				RESOURCE_NAME(SELECTABLE_BACKGROUND_NORMAL_BRUSH);
				RESOURCE_NAME(SELECTABLE_BACKGROUND_ACTIVE_BRUSH);
				RESOURCE_NAME(SELECTABLE_BACKGROUND_PRESSED_BRUSH);
				RESOURCE_NAME(SELECTABLE_BACKGROUND_DISABLED_BRUSH);

#undef RESOURCE_NAME

				class ResourcesAutoInstaller : public INativeControllerListener
				{
				public:
					void NativeWindowCreated(INativeWindow* window)
					{
						WinGDIElementEnvironment* environment=GetNativeWindowGDIElementEnvironment(window);
						if(environment)
						{
							Resources::Install(environment);
						}
					}

					void NativeWindowDestroying(INativeWindow* window)
					{
						WinGDIElementEnvironment* environment=GetNativeWindowGDIElementEnvironment(window);
						if(environment)
						{
							Resources::Uninstall(environment);
						}
					}
				} ResourcesAutoInstaller;
			
				void Resources::RegisterAutoInstall()
				{
					GetCurrentController()->InstallListener(&ResourcesAutoInstaller);
				}

				void Resources::UnregisterAutoInstall()
				{
					GetCurrentController()->UninstallListener(&ResourcesAutoInstaller);
				}

				void Resources::Install(WinGDIElementEnvironment* environment)
				{
					environment->Resources().Add(PUSHABLE_BACKGROUND_NORMAL_PEN,			new WinPen(PS_SOLID, 1, RGB(161,161,161)));
					environment->Resources().Add(PUSHABLE_BACKGROUND_ACTIVE_PEN,			new WinPen(PS_SOLID, 1, RGB(161,161,161)));
					environment->Resources().Add(PUSHABLE_BACKGROUND_PRESSED_PEN,			new WinPen(PS_SOLID, 1, RGB(161,161,161)));
					environment->Resources().Add(PUSHABLE_BACKGROUND_DISABLED_PEN,			new WinPen(PS_SOLID, 1, RGB(161,161,161)));

					environment->Resources().Add(PUSHABLE_BACKGROUND_NORMAL_BRUSH,			new WinBrush(RGB(255, 255, 255)));
					environment->Resources().Add(PUSHABLE_BACKGROUND_ACTIVE_BRUSH,			new WinBrush(RGB(210, 235, 249)));
					environment->Resources().Add(PUSHABLE_BACKGROUND_PRESSED_BRUSH,			new WinBrush(RGB(160, 205, 229)));
					environment->Resources().Add(PUSHABLE_BACKGROUND_DISABLED_BRUSH,		new WinBrush(RGB(211, 211, 211)));

					environment->Resources().Add(SELECTABLE_BACKGROUND_NORMAL_PEN,			new WinPen(PS_SOLID, 1, RGB(255, 255, 255)));
					environment->Resources().Add(SELECTABLE_BACKGROUND_ACTIVE_PEN,			new WinPen(PS_SOLID, 1, RGB(161,161,161)));
					environment->Resources().Add(SELECTABLE_BACKGROUND_PRESSED_PEN,			new WinPen(PS_SOLID, 1, RGB(0, 96, 166)));
					environment->Resources().Add(SELECTABLE_BACKGROUND_DISABLED_PEN,		new WinPen(PS_SOLID, 1, RGB(211, 211, 211)));

					environment->Resources().Add(SELECTABLE_BACKGROUND_NORMAL_BRUSH,		new WinBrush(RGB(255, 255, 255)));
					environment->Resources().Add(SELECTABLE_BACKGROUND_ACTIVE_BRUSH,		new WinBrush(RGB(210, 235, 249)));
					environment->Resources().Add(SELECTABLE_BACKGROUND_PRESSED_BRUSH,		new WinBrush(RGB(0, 96, 166)));
					environment->Resources().Add(SELECTABLE_BACKGROUND_DISABLED_BRUSH,		new WinBrush(RGB(211, 211, 211)));
				}

				void Resources::Uninstall(WinGDIElementEnvironment* environment)
				{
					environment->Resources().Remove(PUSHABLE_BACKGROUND_NORMAL_PEN);
					environment->Resources().Remove(PUSHABLE_BACKGROUND_ACTIVE_PEN);
					environment->Resources().Remove(PUSHABLE_BACKGROUND_PRESSED_PEN);
					environment->Resources().Remove(PUSHABLE_BACKGROUND_DISABLED_PEN);

					environment->Resources().Remove(PUSHABLE_BACKGROUND_NORMAL_BRUSH);
					environment->Resources().Remove(PUSHABLE_BACKGROUND_ACTIVE_BRUSH);
					environment->Resources().Remove(PUSHABLE_BACKGROUND_PRESSED_BRUSH);
					environment->Resources().Remove(PUSHABLE_BACKGROUND_DISABLED_BRUSH);

					environment->Resources().Remove(SELECTABLE_BACKGROUND_NORMAL_PEN);
					environment->Resources().Remove(SELECTABLE_BACKGROUND_ACTIVE_PEN);
					environment->Resources().Remove(SELECTABLE_BACKGROUND_PRESSED_PEN);
					environment->Resources().Remove(SELECTABLE_BACKGROUND_DISABLED_PEN);

					environment->Resources().Remove(SELECTABLE_BACKGROUND_NORMAL_BRUSH);
					environment->Resources().Remove(SELECTABLE_BACKGROUND_ACTIVE_BRUSH);
					environment->Resources().Remove(SELECTABLE_BACKGROUND_PRESSED_BRUSH);
					environment->Resources().Remove(SELECTABLE_BACKGROUND_DISABLED_BRUSH);
				}

				Ptr<WinPen> Resources::GetPen(WinGDIElementEnvironment* environment, const WString& name)
				{
					return environment->Resource(name).Cast<WinPen>();
				}

				Ptr<WinBrush> Resources::GetBrush(WinGDIElementEnvironment* environment, const WString& name)
				{
					return environment->Resource(name).Cast<WinBrush>();
				}

/***********************************************************************
StatefulBackground
***********************************************************************/

				StatefulBackground::StatefulBackground(const wchar_t** penNames, const wchar_t** brushNames, WinGDIElementEnvironment* _environment)
					:WinGDIElement(_environment)
				{
					for(int i=0;i<TotalCount;i++)
					{
						pens[i]=Resources::GetPen(_environment, penNames[i]);
						brushes[i]=Resources::GetBrush(_environment, brushNames[i]);
					}
				}

				StatefulBackground::~StatefulBackground()
				{
				}

				Rect StatefulBackground::GetBounds()
				{
					return bounds;
				}

				void StatefulBackground::SetBounds(Rect value)
				{
					bounds=value;
				}

				StatefulBackground::State StatefulBackground::GetState()
				{
					return state;
				}

				void StatefulBackground::SetState(State value)
				{
					state=value;
				}

				void StatefulBackground::Paint(Size offset, windows::WinDC* dc)
				{
					int x=bounds.Left()+offset.x;
					int y=bounds.Top()+offset.y;
					int w=bounds.Width();
					int h=bounds.Height();
					dc->SetPen(pens[state]);
					dc->SetBrush(brushes[state]);
					dc->Rectangle(x, y, x+w-1, y+h-1);
				}

/***********************************************************************
PushableBackground
***********************************************************************/

				const wchar_t* PushableBackground_PenNames[]={
					Resources::PUSHABLE_BACKGROUND_NORMAL_PEN,
					Resources::PUSHABLE_BACKGROUND_ACTIVE_PEN,
					Resources::PUSHABLE_BACKGROUND_PRESSED_PEN,
					Resources::PUSHABLE_BACKGROUND_DISABLED_PEN,
				};

				const wchar_t* PushableBackground_BrushNames[]={
					Resources::PUSHABLE_BACKGROUND_NORMAL_BRUSH,
					Resources::PUSHABLE_BACKGROUND_ACTIVE_BRUSH,
					Resources::PUSHABLE_BACKGROUND_PRESSED_BRUSH,
					Resources::PUSHABLE_BACKGROUND_DISABLED_BRUSH,
				};

				PushableBackground::PushableBackground(WinGDIElementEnvironment* _environment)
					:StatefulBackground(PushableBackground_PenNames, PushableBackground_BrushNames, _environment)
				{
				}

				PushableBackground::~PushableBackground()
				{
				}

/***********************************************************************
SelectableBackground
***********************************************************************/

				const wchar_t* SelectableBackground_PenNames[]={
					Resources::SELECTABLE_BACKGROUND_NORMAL_PEN,
					Resources::SELECTABLE_BACKGROUND_ACTIVE_PEN,
					Resources::SELECTABLE_BACKGROUND_PRESSED_PEN,
					Resources::SELECTABLE_BACKGROUND_DISABLED_PEN,
				};

				const wchar_t* SelectableBackground_BrushNames[]={
					Resources::SELECTABLE_BACKGROUND_NORMAL_BRUSH,
					Resources::SELECTABLE_BACKGROUND_ACTIVE_BRUSH,
					Resources::SELECTABLE_BACKGROUND_PRESSED_BRUSH,
					Resources::SELECTABLE_BACKGROUND_DISABLED_BRUSH,
				};

				SelectableBackground::SelectableBackground(WinGDIElementEnvironment* _environment)
					:StatefulBackground(SelectableBackground_PenNames, SelectableBackground_BrushNames, _environment)
				{
				}

				SelectableBackground::~SelectableBackground()
				{
				}
			}
		}
	}
}