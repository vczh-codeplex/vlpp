/***********************************************************************
Vczh Library++ 3.0
Developer: ³Âè÷å«(vczh)
GUI::Windows GDI Provider

Interfaces:
***********************************************************************/

#ifndef VCZH_PRESENTATION_WINDOWS_GDI_ELEMENT
#define VCZH_PRESENTATION_WINDOWS_GDI_ELEMENT

#include "WinGDIElement.h"

namespace vl
{
	namespace presentation
	{
		namespace elements
		{
			namespace gdi_simple
			{

/***********************************************************************
×ÊÔ´
***********************************************************************/

#define RESOURCE_NAME(NAME) static const wchar_t* const NAME

				class Resources
				{
				public:
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

					static void								RegisterAutoInstall();
					static void								UnregisterAutoInstall();
					static void								Install(WinGDIElementEnvironment* environment);
					static void								Uninstall(WinGDIElementEnvironment* environment);

					static Ptr<windows::WinPen>				GetPen(WinGDIElementEnvironment* environment, const WString& name);
					static Ptr<windows::WinBrush>			GetBrush(WinGDIElementEnvironment* environment, const WString& name);
				};

#undef RESOURCE_NAME

/***********************************************************************
ÔªËØ
***********************************************************************/

				class StatefulBackground : public WinGDIElement
				{
				public:
					enum State
					{
						Normal,
						Active,
						Pressed,
						Disabled,

						TotalCount,
					};
				protected:
					Ptr<windows::WinPen>					pens[TotalCount];
					Ptr<windows::WinBrush>					brushes[TotalCount];
					Rect									bounds;
					State									state;

				public:
					StatefulBackground(const wchar_t** penNames, const wchar_t** brushNames, WinGDIElementEnvironment* _environment);
					~StatefulBackground();

					Rect									GetBounds();
					void									SetBounds(Rect value);
					State									GetState();
					void									SetState(State value);

					void									Paint(Size offset, windows::WinDC* dc);
				};

				class PushableBackground : public StatefulBackground
				{
				public:
					PushableBackground(WinGDIElementEnvironment* _environment);
					~PushableBackground();
				};

				class SelectableBackground : public StatefulBackground
				{
				public:
					SelectableBackground(WinGDIElementEnvironment* _environment);
					~SelectableBackground();
				};
			}
		}
	}
}

#endif