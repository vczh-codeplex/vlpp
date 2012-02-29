/***********************************************************************
Vczh Library++ 3.0
Developer: 陈梓瀚(vczh)
GacUI::Native Window

Interfaces:
  INativeWindow							：窗口适配器
  INativeWindowListener					：窗口事件监听器
  INativeController						：全局控制器
  INativeControllerListener				：全局事件监听器

Renderers:
  GUI_GRAPHICS_RENDERER_GDI
  GUI_GRAPHICS_RENDERER_DIRECT2D
***********************************************************************/

#ifndef VCZH_PRESENTATION_GUINATIVEWINDOW
#define VCZH_PRESENTATION_GUINATIVEWINDOW

#include "GuiTypes.h"

namespace vl
{
	namespace presentation
	{
		class INativeWindow;
		class INativeWindowListener;
		class INativeController;
		class INativeControllerListener;

/***********************************************************************
System Object
***********************************************************************/

		class INativeScreen : public Interface
		{
		public:
			virtual Rect				GetBounds()=0;
			virtual Rect				GetClientBounds()=0;
			virtual WString				GetName()=0;
			virtual bool				IsPrimary()=0;
		};

		class INativeCursor : public Interface
		{
		public:
			enum SystemCursorType
			{
				SmallWaiting,
				LargeWaiting,
				Arrow,
				Cross,
				Hand,
				Help,
				IBeam,
				SizeAll,
				SizeNESW,
				SizeNS,
				SizeNWSE,
				SizeWE,
				LastSystemCursor=SizeWE,
			};

			static const int			SystemCursorCount=LastSystemCursor+1;
		public:
			virtual bool				IsSystemCursor()=0;
			virtual SystemCursorType	GetSystemCursorType()=0;
		};

/***********************************************************************
Image Object
***********************************************************************/

		class INativeImageService;
		class INativeImage;
		class INativeImageFrame;

		class INativeImageFrameCache : public Interface
		{
		public:
			virtual void						OnAttach(INativeImageFrame* frame)=0;
			virtual void						OnDetach(INativeImageFrame* frame)=0;
		};

		class INativeImageFrame : public Interface
		{
		public:
			virtual INativeImage*				GetImage()=0;
			virtual Size						GetSize()=0;

			virtual bool						SetCache(void* key, Ptr<INativeImageFrameCache> cache)=0;
			virtual Ptr<INativeImageFrameCache>	GetCache(void* key)=0;
			virtual Ptr<INativeImageFrameCache>	RemoveCache(void* key)=0;
		};

		class INativeImage : public Interface
		{
		public:
			enum FormatType
			{
				Bmp,
				Gif,
				Icon,
				Jpeg,
				Png,
				Tiff,
				Wmp,
				Unknown,
			};

			virtual INativeImageService*		GetImageService()=0;
			virtual FormatType					GetFormat()=0;
			virtual int							GetFrameCount()=0;
			virtual INativeImageFrame*			GetFrame(int index)=0;
		};

		class INativeImageService : public Interface
		{
		public:
			virtual Ptr<INativeImage>			CreateImageFromFile(const WString& path)=0;
		};

/***********************************************************************
Native Window
***********************************************************************/

		class INativeWindow : public Interface
		{
		public:
			virtual Rect				GetBounds()=0;
			virtual void				SetBounds(const Rect& bounds)=0;
			virtual Size				GetClientSize()=0;
			virtual void				SetClientSize(Size size)=0;
			virtual Rect				GetClientBoundsInScreen()=0;

			virtual WString				GetTitle()=0;
			virtual void				SetTitle(WString title)=0;
			virtual INativeCursor*		GetWindowCursor()=0;
			virtual void				SetWindowCursor(INativeCursor* cursor)=0;
			virtual Point				GetCaretPoint()=0;
			virtual void				SetCaretPoint(Point point)=0;

			virtual INativeWindow*		GetParent()=0;
			virtual void				SetParent(INativeWindow* parent)=0;
			virtual bool				GetAlwaysPassFocusToParent()=0;
			virtual void				SetAlwaysPassFocusToParent(bool value)=0;

			virtual void				Show()=0;
			virtual void				ShowDeactivated()=0;
			virtual void				ShowRestored()=0;
			virtual void				ShowMaximized()=0;
			virtual void				ShowMinimized()=0;
			virtual void				Hide()=0;
			virtual bool				IsVisible()=0;

			virtual void				Enable()=0;
			virtual void				Disable()=0;
			virtual bool				IsEnabled()=0;

			virtual void				SetFocus()=0;
			virtual bool				IsFocused()=0;
			virtual void				SetActivate()=0;
			virtual bool				IsActivated()=0;

			virtual void				ShowInTaskBar()=0;
			virtual void				HideInTaskBar()=0;
			virtual bool				IsAppearedInTaskBar()=0;

			virtual void				EnableActivate()=0;
			virtual void				DisableActivate()=0;
			virtual bool				IsEnabledActivate()=0;

			virtual bool				RequireCapture()=0;
			virtual bool				ReleaseCapture()=0;

			virtual bool				GetMaximizedBox()=0;
			virtual void				SetMaximizedBox(bool visible)=0;
			virtual bool				GetMinimizedBox()=0;
			virtual void				SetMinimizedBox(bool visible)=0;
			virtual bool				GetBorder()=0;
			virtual void				SetBorder(bool visible)=0;
			virtual bool				GetSizeBox()=0;
			virtual void				SetSizeBox(bool visible)=0;
			virtual bool				GetIconVisible()=0;
			virtual void				SetIconVisible(bool visible)=0;
			virtual bool				GetTitleBar()=0;
			virtual void				SetTitleBar(bool visible)=0;
			virtual bool				GetTopMost()=0;
			virtual void				SetTopMost(bool topmost)=0;

			virtual bool				InstallListener(INativeWindowListener* listener)=0;
			virtual bool				UninstallListener(INativeWindowListener* listener)=0;
			virtual void				RedrawContent()=0;
		};

		struct NativeWindowMouseInfo
		{
			bool						ctrl;
			bool						shift;
			bool						left;
			bool						middle;
			bool						right;
			int							x;
			int							y;
			int							wheel;
		};

		struct NativeWindowKeyInfo
		{
			int							code;
			bool						ctrl;
			bool						shift;
			bool						alt;
			bool						capslock;
		};

		struct NativeWindowCharInfo
		{
			wchar_t						code;
			bool						ctrl;
			bool						shift;
			bool						alt;
			bool						capslock;
		};

		class INativeWindowListener : public Interface
		{
		public:
			virtual void				Moving(Rect& bounds, bool fixSizeOnly);
			virtual void				Moved();
			virtual void				Enabled();
			virtual void				Disabled();
			virtual void				GotFocus();
			virtual void				LostFocus();
			virtual void				Activated();
			virtual void				Deactivated();
			virtual void				Opened();
			virtual void				Closing(bool& cancel);
			virtual void				Closed();
			virtual void				Paint();
			virtual void				Destroying();
			virtual void				Destroyed();

			virtual void				LeftButtonDown(const NativeWindowMouseInfo& info);
			virtual void				LeftButtonUp(const NativeWindowMouseInfo& info);
			virtual void				LeftButtonDoubleClick(const NativeWindowMouseInfo& info);
			virtual void				RightButtonDown(const NativeWindowMouseInfo& info);
			virtual void				RightButtonUp(const NativeWindowMouseInfo& info);
			virtual void				RightButtonDoubleClick(const NativeWindowMouseInfo& info);
			virtual void				MiddleButtonDown(const NativeWindowMouseInfo& info);
			virtual void				MiddleButtonUp(const NativeWindowMouseInfo& info);
			virtual void				MiddleButtonDoubleClick(const NativeWindowMouseInfo& info);
			virtual void				HorizontalWheel(const NativeWindowMouseInfo& info);
			virtual void				VerticalWheel(const NativeWindowMouseInfo& info);
			virtual void				MouseMoving(const NativeWindowMouseInfo& info);
			virtual void				MouseEntered();
			virtual void				MouseLeaved();

			virtual void				KeyDown(const NativeWindowKeyInfo& info);
			virtual void				KeyUp(const NativeWindowKeyInfo& info);
			virtual void				SysKeyDown(const NativeWindowKeyInfo& info);
			virtual void				SysKeyUp(const NativeWindowKeyInfo& info);
			virtual void				Char(const NativeWindowCharInfo& info);
		};

/***********************************************************************
Native Window Services
***********************************************************************/

		class INativeResourceService : public virtual Interface
		{
		public:
			virtual INativeCursor*			GetSystemCursor(INativeCursor::SystemCursorType type)=0;
			virtual INativeCursor*			GetDefaultSystemCursor()=0;

			virtual FontProperties			GetDefaultFont()=0;
			virtual void					SetDefaultFont(const FontProperties& value)=0;
		};

		class INativeAsyncService : public virtual Interface
		{
		public:
			typedef void (AsyncTaskProc)(void* arguments);

			virtual bool					IsInMainThread()=0;
			virtual void					InvokeInMainThread(AsyncTaskProc* proc, void* argument)=0;
			virtual bool					InvokeInMainThreadAndWait(AsyncTaskProc* proc, void* argument, int milliseconds=-1)=0;
		};

		class INativeClipboardService : public virtual Interface
		{
		public:
			virtual bool					ContainsText()=0;
			virtual WString					GetText()=0;
			virtual bool					SetText(const WString& value)=0;
		};

		class INativeScreenService : public virtual Interface
		{
		public:
			virtual int						GetScreenCount()=0;
			virtual INativeScreen*			GetScreen(int index)=0;
			virtual INativeScreen*			GetScreen(INativeWindow* window)=0;
		};

		class INativeWindowService : public virtual Interface
		{
		public:
			virtual INativeWindow*			CreateNativeWindow()=0;
			virtual void					DestroyNativeWindow(INativeWindow* window)=0;
			virtual INativeWindow*			GetMainWindow()=0;
			virtual INativeWindow*			GetWindow(Point location)=0;
			virtual void					Run(INativeWindow* window)=0;
		};

		class INativeInputService : public virtual Interface
		{
		public:
			virtual void					StartHookMouse()=0;
			virtual void					StopHookMouse()=0;
			virtual bool					IsHookingMouse()=0;

			virtual void					StartTimer()=0;
			virtual void					StopTimer()=0;
			virtual bool					IsTimerEnabled()=0;

			virtual bool					IsKeyPressing(int code)=0;
			virtual bool					IsKeyToggled(int code)=0;
		};

		class INativeCallbackService : public virtual Interface
		{
		public:
			virtual bool					InstallListener(INativeControllerListener* listener)=0;
			virtual bool					UninstallListener(INativeControllerListener* listener)=0;
		};

/***********************************************************************
Native Window Controller
***********************************************************************/

		class INativeController : public virtual Interface
		{
		public:
			virtual INativeCallbackService*			CallbackService()=0;
			virtual INativeResourceService*			ResourceService()=0;
			virtual INativeAsyncService*			AsyncService()=0;
			virtual INativeClipboardService*		ClipboardService()=0;
			virtual INativeImageService*			ImageService()=0;
			virtual INativeScreenService*			ScreenService()=0;
			virtual INativeWindowService*			WindowService()=0;
			virtual INativeInputService*			InputService()=0;
		};

		class INativeControllerListener : public Interface
		{
		public:
			virtual void					LeftButtonDown(Point position);
			virtual void					LeftButtonUp(Point position);
			virtual void					RightButtonDown(Point position);
			virtual void					RightButtonUp(Point position);
			virtual void					MouseMoving(Point position);
			virtual void					GlobalTimer();
			virtual void					ClipboardUpdated();
			virtual void					NativeWindowCreated(INativeWindow* window);
			virtual void					NativeWindowDestroying(INativeWindow* window);
		};

		extern								INativeController* GetCurrentController();
		extern void							SetCurrentController(INativeController* controller);
	}
}

/***********************************************************************
Native Window Provider
***********************************************************************/

/*
 * Virtual Keys, Standard Set
 */
#define VKEY_LBUTTON        0x01
#define VKEY_RBUTTON        0x02
#define VKEY_CANCEL         0x03
#define VKEY_MBUTTON        0x04    /* NOT contiguous with L & RBUTTON */

#define VKEY_XBUTTON1       0x05    /* NOT contiguous with L & RBUTTON */
#define VKEY_XBUTTON2       0x06    /* NOT contiguous with L & RBUTTON */

/*
 * 0x07 : unassigned
 */

#define VKEY_BACK           0x08
#define VKEY_TAB            0x09

/*
 * 0x0A - 0x0B : reserved
 */

#define VKEY_CLEAR          0x0C
#define VKEY_RETURN         0x0D

#define VKEY_SHIFT          0x10
#define VKEY_CONTROL        0x11
#define VKEY_MENU           0x12
#define VKEY_PAUSE          0x13
#define VKEY_CAPITAL        0x14

#define VKEY_KANA           0x15
#define VKEY_HANGEUL        0x15  /* old name - should be here for compatibility */
#define VKEY_HANGUL         0x15
#define VKEY_JUNJA          0x17
#define VKEY_FINAL          0x18
#define VKEY_HANJA          0x19
#define VKEY_KANJI          0x19

#define VKEY_ESCAPE         0x1B

#define VKEY_CONVERT        0x1C
#define VKEY_NONCONVERT     0x1D
#define VKEY_ACCEPT         0x1E
#define VKEY_MODECHANGE     0x1F

#define VKEY_SPACE          0x20
#define VKEY_PRIOR          0x21
#define VKEY_NEXT           0x22
#define VKEY_END            0x23
#define VKEY_HOME           0x24
#define VKEY_LEFT           0x25
#define VKEY_UP             0x26
#define VKEY_RIGHT          0x27
#define VKEY_DOWN           0x28
#define VKEY_SELECT         0x29
#define VKEY_PRINT          0x2A
#define VKEY_EXECUTE        0x2B
#define VKEY_SNAPSHOT       0x2C
#define VKEY_INSERT         0x2D
#define VKEY_DELETE         0x2E
#define VKEY_HELP           0x2F

/*
 * VKEY_0 - VKEY_9 are the same as ASCII '0' - '9' (0x30 - 0x39)
 * 0x40 : unassigned
 * VKEY_A - VKEY_Z are the same as ASCII 'A' - 'Z' (0x41 - 0x5A)
 */

#define VKEY_LWIN           0x5B
#define VKEY_RWIN           0x5C
#define VKEY_APPS           0x5D

/*
 * 0x5E : reserved
 */

#define VKEY_SLEEP          0x5F

#define VKEY_NUMPAD0        0x60
#define VKEY_NUMPAD1        0x61
#define VKEY_NUMPAD2        0x62
#define VKEY_NUMPAD3        0x63
#define VKEY_NUMPAD4        0x64
#define VKEY_NUMPAD5        0x65
#define VKEY_NUMPAD6        0x66
#define VKEY_NUMPAD7        0x67
#define VKEY_NUMPAD8        0x68
#define VKEY_NUMPAD9        0x69
#define VKEY_MULTIPLY       0x6A
#define VKEY_ADD            0x6B
#define VKEY_SEPARATOR      0x6C
#define VKEY_SUBTRACT       0x6D
#define VKEY_DECIMAL        0x6E
#define VKEY_DIVIDE         0x6F
#define VKEY_F1             0x70
#define VKEY_F2             0x71
#define VKEY_F3             0x72
#define VKEY_F4             0x73
#define VKEY_F5             0x74
#define VKEY_F6             0x75
#define VKEY_F7             0x76
#define VKEY_F8             0x77
#define VKEY_F9             0x78
#define VKEY_F10            0x79
#define VKEY_F11            0x7A
#define VKEY_F12            0x7B
#define VKEY_F13            0x7C
#define VKEY_F14            0x7D
#define VKEY_F15            0x7E
#define VKEY_F16            0x7F
#define VKEY_F17            0x80
#define VKEY_F18            0x81
#define VKEY_F19            0x82
#define VKEY_F20            0x83
#define VKEY_F21            0x84
#define VKEY_F22            0x85
#define VKEY_F23            0x86
#define VKEY_F24            0x87

/*
 * 0x88 - 0x8F : unassigned
 */

#define VKEY_NUMLOCK        0x90
#define VKEY_SCROLL         0x91

/*
 * NEC PC-9800 kbd definitions
 */
#define VKEY_OEM_NEC_EQUAL  0x92   // '=' key on numpad

/*
 * Fujitsu/OASYS kbd definitions
 */
#define VKEY_OEM_FJ_JISHO   0x92   // 'Dictionary' key
#define VKEY_OEM_FJ_MASSHOU 0x93   // 'Unregister word' key
#define VKEY_OEM_FJ_TOUROKU 0x94   // 'Register word' key
#define VKEY_OEM_FJ_LOYA    0x95   // 'Left OYAYUBI' key
#define VKEY_OEM_FJ_ROYA    0x96   // 'Right OYAYUBI' key

/*
 * 0x97 - 0x9F : unassigned
 */

/*
 * VKEY_L* & VKEY_R* - left and right Alt, Ctrl and Shift virtual keys.
 * Used only as parameters to GetAsyncKeyState() and GetKeyState().
 * No other API or message will distinguish left and right keys in this way.
 */
#define VKEY_LSHIFT         0xA0
#define VKEY_RSHIFT         0xA1
#define VKEY_LCONTROL       0xA2
#define VKEY_RCONTROL       0xA3
#define VKEY_LMENU          0xA4
#define VKEY_RMENU          0xA5

#define VKEY_BROWSER_BACK        0xA6
#define VKEY_BROWSER_FORWARD     0xA7
#define VKEY_BROWSER_REFRESH     0xA8
#define VKEY_BROWSER_STOP        0xA9
#define VKEY_BROWSER_SEARCH      0xAA
#define VKEY_BROWSER_FAVORITES   0xAB
#define VKEY_BROWSER_HOME        0xAC

#define VKEY_VOLUME_MUTE         0xAD
#define VKEY_VOLUME_DOWN         0xAE
#define VKEY_VOLUME_UP           0xAF
#define VKEY_MEDIA_NEXT_TRACK    0xB0
#define VKEY_MEDIA_PREV_TRACK    0xB1
#define VKEY_MEDIA_STOP          0xB2
#define VKEY_MEDIA_PLAY_PAUSE    0xB3
#define VKEY_LAUNCH_MAIL         0xB4
#define VKEY_LAUNCH_MEDIA_SELECT 0xB5
#define VKEY_LAUNCH_APP1         0xB6
#define VKEY_LAUNCH_APP2         0xB7

/*
 * 0xB8 - 0xB9 : reserved
 */

#define VKEY_OEM_1          0xBA   // ';:' for US
#define VKEY_OEM_PLUS       0xBB   // '+' any country
#define VKEY_OEM_COMMA      0xBC   // ',' any country
#define VKEY_OEM_MINUS      0xBD   // '-' any country
#define VKEY_OEM_PERIOD     0xBE   // '.' any country
#define VKEY_OEM_2          0xBF   // '/?' for US
#define VKEY_OEM_3          0xC0   // '`~' for US

/*
 * 0xC1 - 0xD7 : reserved
 */

/*
 * 0xD8 - 0xDA : unassigned
 */

#define VKEY_OEM_4          0xDB  //  '[{' for US
#define VKEY_OEM_5          0xDC  //  '\|' for US
#define VKEY_OEM_6          0xDD  //  ']}' for US
#define VKEY_OEM_7          0xDE  //  ''"' for US
#define VKEY_OEM_8          0xDF

/*
 * 0xE0 : reserved
 */

/*
 * Various extended or enhanced keyboards
 */

#define VKEY_OEM_AX         0xE1  //  'AX' key on Japanese AX kbd
#define VKEY_OEM_102        0xE2  //  "<>" or "\|" on RT 102-key kbd.
#define VKEY_ICO_HELP       0xE3  //  Help key on ICO
#define VKEY_ICO_00         0xE4  //  00 key on ICO

#define VKEY_PROCESSKEY     0xE5
#define VKEY_ICO_CLEAR      0xE6
#define VKEY_PACKET         0xE7

/*
 * 0xE8 : unassigned
 */

/*
 * Nokia/Ericsson definitions
 */
#define VKEY_OEM_RESET      0xE9
#define VKEY_OEM_JUMP       0xEA
#define VKEY_OEM_PA1        0xEB
#define VKEY_OEM_PA2        0xEC
#define VKEY_OEM_PA3        0xED
#define VKEY_OEM_WSCTRL     0xEE
#define VKEY_OEM_CUSEL      0xEF
#define VKEY_OEM_ATTN       0xF0
#define VKEY_OEM_FINISH     0xF1
#define VKEY_OEM_COPY       0xF2
#define VKEY_OEM_AUTO       0xF3
#define VKEY_OEM_ENLW       0xF4
#define VKEY_OEM_BACKTAB    0xF5

#define VKEY_ATTN           0xF6
#define VKEY_CRSEL          0xF7
#define VKEY_EXSEL          0xF8
#define VKEY_EREOF          0xF9
#define VKEY_PLAY           0xFA
#define VKEY_ZOOM           0xFB
#define VKEY_NONAME         0xFC
#define VKEY_PA1            0xFD
#define VKEY_OEM_CLEAR      0xFE

#endif