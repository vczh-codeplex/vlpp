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
元素基类
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
					void									ResetResource(const wchar_t** penNames, const wchar_t** brushNames);

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
					void									ResetResource(const wchar_t** fontNames, const wchar_t** colorNames);

					Size									GetSize();
					Rect									GetBounds();

					void									Paint(Size offset, windows::WinDC* dc);
				};

/***********************************************************************
元素
***********************************************************************/

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
				protected:
					bool									isStaticPen;
				public:
					FocusableBackground(bool staticPen, WinGDIElementEnvironment* _environment);
					~FocusableBackground();

					void									SetStaticPen(bool value);
					bool									IsStaticPen();
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

				class FocusedRectangle : public WinGDIElement
				{
				protected:
					Rect									bounds;
					bool									focusing;
				public:
					FocusedRectangle(WinGDIElementEnvironment* _environment);
					~FocusedRectangle();
					
					Rect									GetBounds();
					void									SetBounds(Rect value);
					bool									GetFocus();
					void									SetFocus(bool value);
					void									Paint(Size offset, windows::WinDC* dc);
				};

/***********************************************************************
控件风格
***********************************************************************/

				class WindowSkin : public WinGDISkin<GuiWindow::IGuiWindowSkin>
				{
					DEFINE_BUILDER(WindowSkin);
				protected:
					Ptr<WinGDIClipElement>					root;
					Ptr<FocusableBackground>				background;
					Ptr<WinGDIClipElement>					containerElement;

					Ptr<WinGDIClipElement>					GetContainerElement();
					int										GetTopLevelElementCount();
					void									InsertElements(int index, Ptr<WinGDIClipElement> _containerElement);
				public:
					WindowSkin(INativeWindow* window);
					~WindowSkin();

					Rect									GetBounds();
					void									SetBounds(Rect value);
					void									Install();
				};

				class TextButtonSkin : public WinGDISkin<GuiTextButton::IGuiTextButtonSkin>
				{
					DEFINE_BUILDER(TextButtonSkin);
				protected:
					Ptr<WinGDIClipElement>					clipBorder;
					Ptr<PushableBackground>					background;
					Ptr<PushableLabel>						label;
					Ptr<FocusedRectangle>					focusedRectangle;
					Ptr<WinGDIClipElement>					containerElement;

					Ptr<WinGDIClipElement>					GetContainerElement();
					int										GetTopLevelElementCount();
					void									InsertElements(int index, Ptr<WinGDIClipElement> _containerElement);
					void									AdjustLabel();
				public:
					TextButtonSkin(INativeWindow* window);
					~TextButtonSkin();
					
					Rect									GetBounds();
					void									SetBounds(Rect value);
					void									SetState(GuiButtonBase::ButtonState style);
					void									SetText(const WString& text);
					void									SetFocus(bool focusing);
				};

				class PanelSkin : public WinGDISkin<GuiPanel::IGuiPanelSkin>
				{
					DEFINE_BUILDER(PanelSkin);
				protected:
					Ptr<FocusableBackground>				background;
					Ptr<WinGDIClipElement>					containerElement;
					GuiPanel::PanelState					panelState;
					GuiPanel::BorderState					borderState;

					Ptr<WinGDIClipElement>					GetContainerElement();
					int										GetTopLevelElementCount();
					void									InsertElements(int index, Ptr<WinGDIClipElement> _containerElement);
					void									UpdateState();
				public:
					PanelSkin(INativeWindow* window);
					~PanelSkin();
					
					Rect									GetBounds();
					void									SetBounds(Rect value);
					void									SetPanelState(GuiPanel::PanelState value);
					void									SetBorderState(GuiPanel::BorderState value);
				};
			}
		}
	}
}

#endif