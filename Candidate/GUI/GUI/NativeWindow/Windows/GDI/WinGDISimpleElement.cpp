#include "WinGDISimpleElement.h"

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
					GetCurrentApplication()->SetSkinBuilder(GuiWindow::SkinBuilderName, new BUILDER_OF_SKIN(WindowSkin));
					GetCurrentApplication()->SetSkinBuilder(GuiTextButton::SkinBuilderName, new BUILDER_OF_SKIN(TextButtonSkin));
					GetCurrentApplication()->SetSkinBuilder(GuiPanel::SkinBuilderName, new BUILDER_OF_SKIN(PanelSkin));
				}

				void Resources::UnregisterAutoInstall()
				{
					GetCurrentController()->UninstallListener(&ResourcesAutoInstaller);
				}

				void Resources::Install(WinGDIElementEnvironment* environment)
				{
					Ptr<WinFont> defaultFont=GetDefaultResourceService()->GetDefaultFont();

					//-----------------------------------------------------

					environment->Resources().Add(PUSHABLE_BACKGROUND_NORMAL_PEN,			new WinPen(PS_SOLID, 1, RGB(161, 161, 161)));
					environment->Resources().Add(PUSHABLE_BACKGROUND_ACTIVE_PEN,			new WinPen(PS_SOLID, 1, RGB(161, 161, 161)));
					environment->Resources().Add(PUSHABLE_BACKGROUND_PRESSED_PEN,			new WinPen(PS_SOLID, 1, RGB(161, 161, 161)));
					environment->Resources().Add(PUSHABLE_BACKGROUND_DISABLED_PEN,			new WinPen(PS_SOLID, 1, RGB(161, 161, 161)));

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
					environment->Resources().Add(PUSHABLE_LABEL_DISABLED_COLOR,				new ObjectBox<COLORREF>(RGB(161, 161, 161)));

					//-----------------------------------------------------

					environment->Resources().Add(SELECTABLE_BACKGROUND_NORMAL_PEN,			new WinPen(PS_SOLID, 1, RGB(255, 255, 255)));
					environment->Resources().Add(SELECTABLE_BACKGROUND_ACTIVE_PEN,			new WinPen(PS_SOLID, 1, RGB(161, 161, 161)));
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
					environment->Resources().Add(SELECTABLE_LABEL_DISABLED_COLOR,			new ObjectBox<COLORREF>(RGB(161, 161, 161)));

					//-----------------------------------------------------

					environment->Resources().Add(FOCUSABLE_BACKGROUND_NORMAL_PEN,			new WinPen(PS_SOLID, 1, RGB(161, 161, 161)));
					environment->Resources().Add(FOCUSABLE_BACKGROUND_ACTIVE_PEN,			new WinPen(PS_SOLID, 1, RGB(160, 205, 229)));
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
					ResetResource(penNames, brushNames);
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

				void StatefulBackground::ResetResource(const wchar_t** penNames, const wchar_t** brushNames)
				{
					for(int i=0;i<TotalCount;i++)
					{
						pens[i]=Resources::GetPen(environment, penNames[i]);
						brushes[i]=Resources::GetBrush(environment, brushNames[i]);
					}
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
					ResetResource(fontNames, colorNames);
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

				void StatefulLabel::ResetResource(const wchar_t** fontNames, const wchar_t** colorNames)
				{
					for(int i=0;i<TotalCount;i++)
					{
						fonts[i]=Resources::GetFont(environment, fontNames[i]);
						colors[i]=Resources::GetColor(environment, colorNames[i]);
					}
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
FocusableBackground
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
					,isStaticPen(staticPen)
				{
				}

				FocusableBackground::~FocusableBackground()
				{
				}

				void FocusableBackground::SetStaticPen(bool value)
				{
					isStaticPen=value;
					ResetResource((isStaticPen?FocusableBackground_EmptyPenNames:FocusableBackground_PenNames), FocusableBackground_BrushNames);
				}

				bool FocusableBackground::IsStaticPen()
				{
					return isStaticPen;
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
FocusedRectangle
***********************************************************************/

				FocusedRectangle::FocusedRectangle(WinGDIElementEnvironment* _environment)
					:WinGDIElement(_environment)
					,focusing(false)
				{
				}

				FocusedRectangle::~FocusedRectangle()
				{
				}
					
				Rect FocusedRectangle::GetBounds()
				{
					return bounds;
				}

				void FocusedRectangle::SetBounds(Rect value)
				{
					bounds=value;
				}

				bool FocusedRectangle::GetFocus()
				{
					return focusing;
				}

				void FocusedRectangle::SetFocus(bool value)
				{
					focusing=value;
				}

				void FocusedRectangle::Paint(Size offset, windows::WinDC* dc)
				{
					if(focusing)
					{
						int x=offset.x+bounds.Left()+3;
						int y=offset.y+bounds.Top()+3;
						int w=bounds.Width()-6;
						int h=bounds.Height()-6;
						dc->FocusRectangle(x, y, x+w, y+h);
					}
				}

/***********************************************************************
WindowSkin
***********************************************************************/

				Ptr<WinGDIClipElement> WindowSkin::GetContainerElement()
				{
					return containerElement;
				}

				int WindowSkin::GetTopLevelElementCount()
				{
					return 1;
				}

				void WindowSkin::InsertElements(int index, Ptr<WinGDIClipElement> _containerElement)
				{
					_containerElement->Children().Insert(index, root);
				}

				WindowSkin::WindowSkin(INativeWindow* window)
					:WinGDISkin(window)
				{
					background=new FocusableBackground(true, environment);
					containerElement=new WinGDIClipElement(environment);
					root=new WinGDIClipElement(environment);

					root->Children().Add(background);
					root->Children().Add(containerElement);
				}

				WindowSkin::~WindowSkin()
				{
				}
				
				Rect WindowSkin::GetBounds()
				{
					return root->GetBounds();
				}

				void WindowSkin::SetBounds(Rect value)
				{
					root->SetBounds(value);
					background->SetBounds(Rect(Point(0, 0), value.GetSize()));
					containerElement->SetBounds(Rect(Point(0, 0), value.GetSize()));
					skinListener->RequireRedraw();
				}

				void WindowSkin::Install()
				{
					environment->SetRootElement(root);
				}

/***********************************************************************
TextButtonSkin
***********************************************************************/

				Ptr<WinGDIClipElement> TextButtonSkin::GetContainerElement()
				{
					return containerElement;
				}

				int TextButtonSkin::GetTopLevelElementCount()
				{
					return 1;
				}

				void TextButtonSkin::InsertElements(int index, Ptr<WinGDIClipElement> _containerElement)
				{
					_containerElement->Children().Insert(index, clipBorder);
				}

				void TextButtonSkin::AdjustLabel()
				{
					Size buttonSize=background->GetBounds().GetSize();
					Size labelSize=label->GetSize();
					int offset=
						label->GetState()==StatefulObject::Active?-1:
						label->GetState()==StatefulObject::Pressed?1:
						0;
					label->SetPosition(Point((buttonSize.x-labelSize.x)/2+offset, (buttonSize.y-labelSize.y)/2+offset));
				}

				TextButtonSkin::TextButtonSkin(INativeWindow* window)
					:WinGDISkin(window)
				{
					clipBorder=new WinGDIClipElement(environment);
					background=new PushableBackground(environment);
					label=new PushableLabel(environment);
					focusedRectangle=new FocusedRectangle(environment);
					containerElement=new WinGDIClipElement(environment);

					clipBorder->Children().Add(background);
					clipBorder->Children().Add(label);
					clipBorder->Children().Add(focusedRectangle);
					clipBorder->Children().Add(containerElement);
				}

				TextButtonSkin::~TextButtonSkin()
				{
				}
				
				Rect TextButtonSkin::GetBounds()
				{
					return clipBorder->GetBounds();
				}

				void TextButtonSkin::SetBounds(Rect value)
				{
					clipBorder->SetBounds(value);
					background->SetBounds(Rect(Point(0, 0), value.GetSize()));
					focusedRectangle->SetBounds(Rect(Point(0, 0), value.GetSize()));
					containerElement->SetBounds(Rect(Point(0, 0), value.GetSize()));
					AdjustLabel();
					skinListener->RequireRedraw();
				}

				void TextButtonSkin::SetState(GuiButtonBase::ButtonState style)
				{
					StatefulObject::State state=StatefulObject::Normal;
					switch(style)
					{
					case GuiButtonBase::Normal:
						state=StatefulObject::Normal;
						break;
					case GuiButtonBase::Active:
						state=StatefulObject::Active;
						break;
					case GuiButtonBase::Pressed:
						state=StatefulObject::Pressed;
						break;
					default:
						return;
					}
					background->SetState(state);
					label->SetState(state);
					AdjustLabel();
					skinListener->RequireRedraw();
				}

				void TextButtonSkin::SetText(const WString& text)
				{
					label->SetText(text);
					AdjustLabel();
					skinListener->RequireRedraw();
				}

				void TextButtonSkin::SetFocus(bool focusing)
				{
					focusedRectangle->SetFocus(focusing);
					skinListener->RequireRedraw();
				}

/***********************************************************************
PanelSkin
***********************************************************************/

				Ptr<WinGDIClipElement> PanelSkin::GetContainerElement()
				{
					return containerElement;
				}

				int PanelSkin::GetTopLevelElementCount()
				{
					return 2;
				}

				void PanelSkin::InsertElements(int index, Ptr<WinGDIClipElement> _containerElement)
				{
					_containerElement->Children().Insert(index, containerElement);
					_containerElement->Children().Insert(index, background);
				}

				void PanelSkin::UpdateState()
				{
					bool staticPen=borderState==GuiPanel::Normal;
					if(background->IsStaticPen()!=staticPen)
					{
						background->SetStaticPen(staticPen);
					}
					if(!staticPen)
					{
						if(borderState==GuiPanel::Static)
						{
							background->SetState(StatefulObject::Normal);
						}
						else switch(panelState)
						{
						case GuiPanel::Normal:
							background->SetState(StatefulObject::Normal);
							break;
						case GuiPanel::Active:
							background->SetState(StatefulObject::Active);
							break;
						case GuiPanel::Focused:
							background->SetState(StatefulObject::Pressed);
							break;
						}
					}
					skinListener->RequireRedraw();
				}

				PanelSkin::PanelSkin(INativeWindow* window)
					:WinGDISkin(window)
					,panelState(GuiPanel::Normal)
					,borderState(GuiPanel::Dynamic)
				{
					background=new FocusableBackground(false, environment);
					containerElement=new WinGDIClipElement(environment);
				}

				PanelSkin::~PanelSkin()
				{
				}
					
				Rect PanelSkin::GetBounds()
				{
					return background->GetBounds();
				}

				void PanelSkin::SetBounds(Rect value)
				{
					background->SetBounds(value);
					containerElement->SetBounds(Rect(value.LeftTop()+Size(1, 1), value.GetSize()-Size(2, 2)));
				}

				void PanelSkin::SetPanelState(GuiPanel::PanelState value)
				{
					panelState=value;
					UpdateState();
				}

				void PanelSkin::SetBorderState(GuiPanel::BorderState value)
				{
					borderState=value;
					UpdateState();
				}
			}
		}
	}
}