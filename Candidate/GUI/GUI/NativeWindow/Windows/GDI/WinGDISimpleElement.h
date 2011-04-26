/***********************************************************************
Vczh Library++ 3.0
Developer: 陈梓瀚(vczh)
GUI::Windows GDI Provider

Interfaces:
***********************************************************************/

#ifndef VCZH_PRESENTATION_WINDOWS_GDI_ELEMENT
#define VCZH_PRESENTATION_WINDOWS_GDI_ELEMENT

#include "WinGDIApplication.h"

namespace vl
{
	namespace presentation
	{
		namespace elements
		{
			namespace gdi_simple
			{

/***********************************************************************
资源
***********************************************************************/

#define RESOURCE_NAME(NAME) static const wchar_t* const NAME

				class Resources
				{
				public:

					//-----------------------------------------------------

					RESOURCE_NAME(PUSHABLE_BACKGROUND_NORMAL_PEN);
					RESOURCE_NAME(PUSHABLE_BACKGROUND_ACTIVE_PEN);
					RESOURCE_NAME(PUSHABLE_BACKGROUND_PRESSED_PEN);
					RESOURCE_NAME(PUSHABLE_BACKGROUND_DISABLED_PEN);

					RESOURCE_NAME(PUSHABLE_BACKGROUND_NORMAL_BRUSH);
					RESOURCE_NAME(PUSHABLE_BACKGROUND_ACTIVE_BRUSH);
					RESOURCE_NAME(PUSHABLE_BACKGROUND_PRESSED_BRUSH);
					RESOURCE_NAME(PUSHABLE_BACKGROUND_DISABLED_BRUSH);

					RESOURCE_NAME(PUSHABLE_LABEL_NORMAL_FONT);
					RESOURCE_NAME(PUSHABLE_LABEL_ACTIVE_FONT);
					RESOURCE_NAME(PUSHABLE_LABEL_PRESSED_FONT);
					RESOURCE_NAME(PUSHABLE_LABEL_DISABLED_FONT);

					RESOURCE_NAME(PUSHABLE_LABEL_NORMAL_COLOR);
					RESOURCE_NAME(PUSHABLE_LABEL_ACTIVE_COLOR);
					RESOURCE_NAME(PUSHABLE_LABEL_PRESSED_COLOR);
					RESOURCE_NAME(PUSHABLE_LABEL_DISABLED_COLOR);

					//-----------------------------------------------------

					RESOURCE_NAME(SELECTABLE_BACKGROUND_NORMAL_PEN);
					RESOURCE_NAME(SELECTABLE_BACKGROUND_ACTIVE_PEN);
					RESOURCE_NAME(SELECTABLE_BACKGROUND_PRESSED_PEN);
					RESOURCE_NAME(SELECTABLE_BACKGROUND_DISABLED_PEN);

					RESOURCE_NAME(SELECTABLE_BACKGROUND_NORMAL_BRUSH);
					RESOURCE_NAME(SELECTABLE_BACKGROUND_ACTIVE_BRUSH);
					RESOURCE_NAME(SELECTABLE_BACKGROUND_PRESSED_BRUSH);
					RESOURCE_NAME(SELECTABLE_BACKGROUND_DISABLED_BRUSH);

					RESOURCE_NAME(SELECTABLE_LABEL_NORMAL_FONT);
					RESOURCE_NAME(SELECTABLE_LABEL_ACTIVE_FONT);
					RESOURCE_NAME(SELECTABLE_LABEL_PRESSED_FONT);
					RESOURCE_NAME(SELECTABLE_LABEL_DISABLED_FONT);

					RESOURCE_NAME(SELECTABLE_LABEL_NORMAL_COLOR);
					RESOURCE_NAME(SELECTABLE_LABEL_ACTIVE_COLOR);
					RESOURCE_NAME(SELECTABLE_LABEL_PRESSED_COLOR);
					RESOURCE_NAME(SELECTABLE_LABEL_DISABLED_COLOR);

					//-----------------------------------------------------

					RESOURCE_NAME(FOCUSABLE_BACKGROUND_NORMAL_PEN);
					RESOURCE_NAME(FOCUSABLE_BACKGROUND_ACTIVE_PEN);
					RESOURCE_NAME(FOCUSABLE_BACKGROUND_PRESSED_PEN);
					RESOURCE_NAME(FOCUSABLE_BACKGROUND_DISABLED_PEN);
					RESOURCE_NAME(FOCUSABLE_BACKGROUND_PEN);
					RESOURCE_NAME(FOCUSABLE_BACKGROUND_BRUSH);

					//-----------------------------------------------------

					static void								RegisterAutoInstall();
					static void								UnregisterAutoInstall();
					static void								Install(WinGDIElementEnvironment* environment);
					static void								Uninstall(WinGDIElementEnvironment* environment);

					static Ptr<windows::WinPen>				GetPen(WinGDIElementEnvironment* environment, const WString& name);
					static Ptr<windows::WinBrush>			GetBrush(WinGDIElementEnvironment* environment, const WString& name);
					static Ptr<windows::WinFont>			GetFont(WinGDIElementEnvironment* environment, const WString& name);
					static COLORREF							GetColor(WinGDIElementEnvironment* environment, const WString& name);
				};

#undef RESOURCE_NAME

/***********************************************************************
元素
***********************************************************************/

				class StatefulObject : public WinGDIElement
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
				public:
					StatefulObject(WinGDIElementEnvironment* _environment);
				};

				class StatefulBackground : public StatefulObject
				{
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

				class StatefulLabel : public StatefulObject
				{
				protected:
					Ptr<windows::WinFont>					fonts[TotalCount];
					COLORREF								colors[TotalCount];
					WString									text;
					Point									position;
					State									state;

				public:
					StatefulLabel(const wchar_t** fontNames, const wchar_t** colorNames, WinGDIElementEnvironment* _environment);
					~StatefulLabel();

					WString									GetText();
					void									SetText(const WString& value);
					Point									GetPosition();
					void									SetPosition(Point value);
					State									GetState();
					void									SetState(State value);

					Size									GetSize();
					Rect									GetBounds();

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

				class FocusableBackground : public StatefulBackground
				{
				public:
					FocusableBackground(bool staticPen, WinGDIElementEnvironment* _environment);
					~FocusableBackground();
				};

				class PushableLabel : public StatefulLabel
				{
				public:
					PushableLabel(WinGDIElementEnvironment* _environment);
					~PushableLabel();
				};

				class SelectableLabel : public StatefulLabel
				{
				public:
					SelectableLabel(WinGDIElementEnvironment* _environment);
					~SelectableLabel();
				};

/***********************************************************************
控件风格
***********************************************************************/

				class WindowSkin : public WinGDISkin<GuiWindow::IGuiWindowSkin>
				{
					DEFINE_BUILDER(WindowSkin);
				protected:
					Ptr<FocusableBackground>				background;

					Ptr<WinGDIClipElement>					GetContainerElement();
					int										GetTopLevelElementCount();
					void									InsertElements(int index, Ptr<WinGDIClipElement> containerElement);
				public:
					WindowSkin(INativeWindow* window);
					~WindowSkin();

					void									SetBounds(Rect value);
					void									Install();
				};
			}
		}
	}
}

#endif