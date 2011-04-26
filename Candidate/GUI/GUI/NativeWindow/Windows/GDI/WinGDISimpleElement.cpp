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
					GetCurrentApplication()->SetSkinBuilder(GuiWindow::SkinBuilderName, new WindowSkin::Builder);
				}

				void Resources::UnregisterAutoInstall()
				{
					GetCurrentController()->UninstallListener(&ResourcesAutoInstaller);
				}

				void Resources::Install(WinGDIElementEnvironment* environment)
				{
					Ptr<WinFont> defaultFont=GetDefaultResourceService()->GetDefaultFont();

					//-----------------------------------------------------

					environment->Resources().Add(PUSHABLE_BACKGROUND_NORMAL_PEN,			new WinPen(PS_SOLID, 1, RGB(161,161,161)));
					environment->Resources().Add(PUSHABLE_BACKGROUND_ACTIVE_PEN,			new WinPen(PS_SOLID, 1, RGB(161,161,161)));
					environment->Resources().Add(PUSHABLE_BACKGROUND_PRESSED_PEN,			new WinPen(PS_SOLID, 1, RGB(161,161,161)));
					environment->Resources().Add(PUSHABLE_BACKGROUND_DISABLED_PEN,			new WinPen(PS_SOLID, 1, RGB(161,161,161)));

					environment->Resources().Add(PUSHABLE_BACKGROUND_NORMAL_BRUSH,			new WinBrush(RGB(255, 255, 255)));
					environment->Resources().Add(PUSHABLE_BACKGROUND_ACTIVE_BRUSH,			new WinBrush(RGB(210, 235, 249)));
					environment->Resources().Add(PUSHABLE_BACKGROUND_PRESSED_BRUSH,			new WinBrush(RGB(160, 205, 229)));
					environment->Resources().Add(PUSHABLE_BACKGROUND_DISABLED_BRUSH,		new WinBrush(RGB(211, 211, 211)));

					environment->Resources().Add(PUSHABLE_LABEL_NORMAL_FONT,				defaultFont);
					environment->Resources().Add(PUSHABLE_LABEL_ACTIVE_FONT,				defaultFont);
					environment->Resources().Add(PUSHABLE_LABEL_PRESSED_FONT,				defaultFont);
					environment->Resources().Add(PUSHABLE_LABEL_DISABLED_FONT,				defaultFont);

					environment->Resources().Add(PUSHABLE_LABEL_NORMAL_COLOR,				new ObjectBox<COLORREF>(RGB(102, 96, 181)));
					environment->Resources().Add(PUSHABLE_LABEL_ACTIVE_COLOR,				new ObjectBox<COLORREF>(RGB(102, 96, 181)));
					environment->Resources().Add(PUSHABLE_LABEL_PRESSED_COLOR,				new ObjectBox<COLORREF>(RGB(102, 96, 181)));
					environment->Resources().Add(PUSHABLE_LABEL_DISABLED_COLOR,				new ObjectBox<COLORREF>(RGB(161,161,161)));

					//-----------------------------------------------------

					environment->Resources().Add(SELECTABLE_BACKGROUND_NORMAL_PEN,			new WinPen(PS_SOLID, 1, RGB(255, 255, 255)));
					environment->Resources().Add(SELECTABLE_BACKGROUND_ACTIVE_PEN,			new WinPen(PS_SOLID, 1, RGB(161,161,161)));
					environment->Resources().Add(SELECTABLE_BACKGROUND_PRESSED_PEN,			new WinPen(PS_SOLID, 1, RGB(0, 96, 166)));
					environment->Resources().Add(SELECTABLE_BACKGROUND_DISABLED_PEN,		new WinPen(PS_SOLID, 1, RGB(211, 211, 211)));

					environment->Resources().Add(SELECTABLE_BACKGROUND_NORMAL_BRUSH,		new WinBrush(RGB(255, 255, 255)));
					environment->Resources().Add(SELECTABLE_BACKGROUND_ACTIVE_BRUSH,		new WinBrush(RGB(210, 235, 249)));
					environment->Resources().Add(SELECTABLE_BACKGROUND_PRESSED_BRUSH,		new WinBrush(RGB(0, 96, 166)));
					environment->Resources().Add(SELECTABLE_BACKGROUND_DISABLED_BRUSH,		new WinBrush(RGB(211, 211, 211)));

					environment->Resources().Add(SELECTABLE_LABEL_NORMAL_FONT,				defaultFont);
					environment->Resources().Add(SELECTABLE_LABEL_ACTIVE_FONT,				defaultFont);
					environment->Resources().Add(SELECTABLE_LABEL_PRESSED_FONT,				defaultFont);
					environment->Resources().Add(SELECTABLE_LABEL_DISABLED_FONT,			defaultFont);

					environment->Resources().Add(SELECTABLE_LABEL_NORMAL_COLOR,				new ObjectBox<COLORREF>(RGB(102, 96, 181)));
					environment->Resources().Add(SELECTABLE_LABEL_ACTIVE_COLOR,				new ObjectBox<COLORREF>(RGB(102, 96, 181)));
					environment->Resources().Add(SELECTABLE_LABEL_PRESSED_COLOR,			new ObjectBox<COLORREF>(RGB(255, 255, 255)));
					environment->Resources().Add(SELECTABLE_LABEL_DISABLED_COLOR,			new ObjectBox<COLORREF>(RGB(161,161,161)));

					//-----------------------------------------------------

					environment->Resources().Add(FOCUSABLE_BACKGROUND_NORMAL_PEN,			new WinPen(PS_SOLID, 1, RGB(211, 211, 211)));
					environment->Resources().Add(FOCUSABLE_BACKGROUND_ACTIVE_PEN,			new WinPen(PS_SOLID, 1, RGB(210, 235, 249)));
					environment->Resources().Add(FOCUSABLE_BACKGROUND_PRESSED_PEN,			new WinPen(PS_SOLID, 1, RGB(0, 96, 166)));
					environment->Resources().Add(FOCUSABLE_BACKGROUND_DISABLED_PEN,			new WinPen(PS_SOLID, 1, RGB(211, 211, 211)));
					environment->Resources().Add(FOCUSABLE_BACKGROUND_PEN,					new WinPen(PS_SOLID, 1, RGB(255, 255, 255)));
					environment->Resources().Add(FOCUSABLE_BACKGROUND_BRUSH,				new WinBrush(RGB(255, 255, 255)));

					//-----------------------------------------------------
				}

				void Resources::Uninstall(WinGDIElementEnvironment* environment)
				{

					//-----------------------------------------------------

					environment->Resources().Remove(PUSHABLE_BACKGROUND_NORMAL_PEN);
					environment->Resources().Remove(PUSHABLE_BACKGROUND_ACTIVE_PEN);
					environment->Resources().Remove(PUSHABLE_BACKGROUND_PRESSED_PEN);
					environment->Resources().Remove(PUSHABLE_BACKGROUND_DISABLED_PEN);

					environment->Resources().Remove(PUSHABLE_BACKGROUND_NORMAL_BRUSH);
					environment->Resources().Remove(PUSHABLE_BACKGROUND_ACTIVE_BRUSH);
					environment->Resources().Remove(PUSHABLE_BACKGROUND_PRESSED_BRUSH);
					environment->Resources().Remove(PUSHABLE_BACKGROUND_DISABLED_BRUSH);

					environment->Resources().Remove(PUSHABLE_LABEL_NORMAL_FONT);
					environment->Resources().Remove(PUSHABLE_LABEL_ACTIVE_FONT);
					environment->Resources().Remove(PUSHABLE_LABEL_PRESSED_FONT);
					environment->Resources().Remove(PUSHABLE_LABEL_DISABLED_FONT);

					environment->Resources().Remove(PUSHABLE_LABEL_NORMAL_COLOR);
					environment->Resources().Remove(PUSHABLE_LABEL_ACTIVE_COLOR);
					environment->Resources().Remove(PUSHABLE_LABEL_PRESSED_COLOR);
					environment->Resources().Remove(PUSHABLE_LABEL_DISABLED_COLOR);

					//-----------------------------------------------------

					environment->Resources().Remove(SELECTABLE_BACKGROUND_NORMAL_PEN);
					environment->Resources().Remove(SELECTABLE_BACKGROUND_ACTIVE_PEN);
					environment->Resources().Remove(SELECTABLE_BACKGROUND_PRESSED_PEN);
					environment->Resources().Remove(SELECTABLE_BACKGROUND_DISABLED_PEN);

					environment->Resources().Remove(SELECTABLE_BACKGROUND_NORMAL_BRUSH);
					environment->Resources().Remove(SELECTABLE_BACKGROUND_ACTIVE_BRUSH);
					environment->Resources().Remove(SELECTABLE_BACKGROUND_PRESSED_BRUSH);
					environment->Resources().Remove(SELECTABLE_BACKGROUND_DISABLED_BRUSH);

					environment->Resources().Remove(SELECTABLE_LABEL_NORMAL_FONT);
					environment->Resources().Remove(SELECTABLE_LABEL_ACTIVE_FONT);
					environment->Resources().Remove(SELECTABLE_LABEL_PRESSED_FONT);
					environment->Resources().Remove(SELECTABLE_LABEL_DISABLED_FONT);

					environment->Resources().Remove(SELECTABLE_LABEL_NORMAL_COLOR);
					environment->Resources().Remove(SELECTABLE_LABEL_ACTIVE_COLOR);
					environment->Resources().Remove(SELECTABLE_LABEL_PRESSED_COLOR);
					environment->Resources().Remove(SELECTABLE_LABEL_DISABLED_COLOR);

					//-----------------------------------------------------

					environment->Resources().Remove(FOCUSABLE_BACKGROUND_NORMAL_PEN);
					environment->Resources().Remove(FOCUSABLE_BACKGROUND_ACTIVE_PEN);
					environment->Resources().Remove(FOCUSABLE_BACKGROUND_PRESSED_PEN);
					environment->Resources().Remove(FOCUSABLE_BACKGROUND_DISABLED_PEN);
					environment->Resources().Remove(FOCUSABLE_BACKGROUND_PEN);
					environment->Resources().Remove(FOCUSABLE_BACKGROUND_BRUSH);

					//-----------------------------------------------------
				}

				Ptr<WinPen> Resources::GetPen(WinGDIElementEnvironment* environment, const WString& name)
				{
					return environment->Resource(name).Cast<WinPen>();
				}

				Ptr<WinBrush> Resources::GetBrush(WinGDIElementEnvironment* environment, const WString& name)
				{
					return environment->Resource(name).Cast<WinBrush>();
				}

				Ptr<windows::WinFont> Resources::GetFont(WinGDIElementEnvironment* environment, const WString& name)
				{
					return environment->Resource(name).Cast<WinFont>();
				}

				COLORREF Resources::GetColor(WinGDIElementEnvironment* environment, const WString& name)
				{
					return environment->Resource(name).Cast<ObjectBox<COLORREF>>()->Unbox();
				}

/***********************************************************************
StatefulObject
***********************************************************************/

				StatefulObject::StatefulObject(WinGDIElementEnvironment* _environment)
					:WinGDIElement(_environment)
				{
				}

/***********************************************************************
StatefulBackground
***********************************************************************/

				StatefulBackground::StatefulBackground(const wchar_t** penNames, const wchar_t** brushNames, WinGDIElementEnvironment* _environment)
					:StatefulObject(_environment)
					,state(StatefulObject::Normal)
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

				StatefulObject::State StatefulBackground::GetState()
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
StatefulLabel
***********************************************************************/
				
				StatefulLabel::StatefulLabel(const wchar_t** fontNames, const wchar_t** colorNames, WinGDIElementEnvironment* _environment)
					:StatefulObject(_environment)
					,state(StatefulObject::Normal)
				{
					for(int i=0;i<TotalCount;i++)
					{
						fonts[i]=Resources::GetFont(_environment, fontNames[i]);
						colors[i]=Resources::GetColor(_environment, colorNames[i]);
					}
				}

				StatefulLabel::~StatefulLabel()
				{
				}

				WString StatefulLabel::GetText()
				{
					return text;
				}

				void StatefulLabel::SetText(const WString& value)
				{
					text=value;
				}

				Point StatefulLabel::GetPosition()
				{
					return position;
				}

				void StatefulLabel::SetPosition(Point value)
				{
					position=value;
				}

				StatefulObject::State StatefulLabel::GetState()
				{
					return state;
				}

				void StatefulLabel::SetState(State value)
				{
					state=value;
				}

				Size StatefulLabel::GetSize()
				{
					WinDC* dc=environment->GetEnvironmentDC();
					dc->SetFont(fonts[state]);
					SIZE size=environment->GetEnvironmentDC()->MeasureString(text);
					return Size(size.cx, size.cy);
				}

				Rect StatefulLabel::GetBounds()
				{
					return Rect(position, GetSize());
				}

				void StatefulLabel::Paint(Size offset, windows::WinDC* dc)
				{
					dc->SetFont(fonts[state]);
					dc->SetTextColor(colors[state]);
					dc->SetBackTransparent(true);
					dc->DrawString(position.x+offset.x, position.y+offset.y, text);
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

/***********************************************************************
SelectableBackground
***********************************************************************/

				const wchar_t* FocusableBackground_PenNames[]={
					Resources::FOCUSABLE_BACKGROUND_NORMAL_PEN,
					Resources::FOCUSABLE_BACKGROUND_ACTIVE_PEN,
					Resources::FOCUSABLE_BACKGROUND_PRESSED_PEN,
					Resources::FOCUSABLE_BACKGROUND_DISABLED_PEN,
				};

				const wchar_t* FocusableBackground_EmptyPenNames[]={
					Resources::FOCUSABLE_BACKGROUND_PEN,
					Resources::FOCUSABLE_BACKGROUND_PEN,
					Resources::FOCUSABLE_BACKGROUND_PEN,
					Resources::FOCUSABLE_BACKGROUND_PEN,
				};

				const wchar_t* FocusableBackground_BrushNames[]={
					Resources::FOCUSABLE_BACKGROUND_BRUSH,
					Resources::FOCUSABLE_BACKGROUND_BRUSH,
					Resources::FOCUSABLE_BACKGROUND_BRUSH,
					Resources::FOCUSABLE_BACKGROUND_BRUSH,
				};

				FocusableBackground::FocusableBackground(bool staticPen, WinGDIElementEnvironment* _environment)
					:StatefulBackground((staticPen?FocusableBackground_EmptyPenNames:FocusableBackground_PenNames), FocusableBackground_BrushNames, _environment)
				{
				}

				FocusableBackground::~FocusableBackground()
				{
				}

/***********************************************************************
PushableBackground
***********************************************************************/

				const wchar_t* PushableLabel_FontNames[]={
					Resources::PUSHABLE_LABEL_NORMAL_FONT,
					Resources::PUSHABLE_LABEL_ACTIVE_FONT,
					Resources::PUSHABLE_LABEL_PRESSED_FONT,
					Resources::PUSHABLE_LABEL_DISABLED_FONT,
				};

				const wchar_t* PushableLabel_ColorNames[]={
					Resources::PUSHABLE_LABEL_NORMAL_COLOR,
					Resources::PUSHABLE_LABEL_ACTIVE_COLOR,
					Resources::PUSHABLE_LABEL_PRESSED_COLOR,
					Resources::PUSHABLE_LABEL_DISABLED_COLOR,
				};

				PushableLabel::PushableLabel(WinGDIElementEnvironment* _environment)
					:StatefulLabel(PushableLabel_FontNames, PushableLabel_ColorNames, _environment)
				{
				}

				PushableLabel::~PushableLabel()
				{
				}

/***********************************************************************
SelectableLabel
***********************************************************************/

				const wchar_t* SelectableLabel_FontNames[]={
					Resources::SELECTABLE_LABEL_NORMAL_FONT,
					Resources::SELECTABLE_LABEL_ACTIVE_FONT,
					Resources::SELECTABLE_LABEL_PRESSED_FONT,
					Resources::SELECTABLE_LABEL_DISABLED_FONT,
				};

				const wchar_t* SelectableLabel_ColorNames[]={
					Resources::SELECTABLE_LABEL_NORMAL_COLOR,
					Resources::SELECTABLE_LABEL_ACTIVE_COLOR,
					Resources::SELECTABLE_LABEL_PRESSED_COLOR,
					Resources::SELECTABLE_LABEL_DISABLED_COLOR,
				};

				SelectableLabel::SelectableLabel(WinGDIElementEnvironment* _environment)
					:StatefulLabel(SelectableLabel_FontNames, SelectableLabel_ColorNames, _environment)
				{
				}

				SelectableLabel::~SelectableLabel()
				{
				}

/***********************************************************************
WindowSkin
***********************************************************************/

				WindowSkin::WindowSkin(INativeWindow* window)
					:WinGDISkin(window)
				{
					background=new FocusableBackground(true, environment);
				}

				WindowSkin::~WindowSkin()
				{
				}

				void WindowSkin::SetBounds(Rect value)
				{
					background->SetBounds(value);
					skinListener->RequireRedraw();
				}

				void WindowSkin::Install()
				{
					environment->SetRootElement(background);
				}
			}
		}
	}
}