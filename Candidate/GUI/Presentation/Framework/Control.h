/***********************************************************************
Vczh Library++ 3.0
Developer: 陈梓瀚(vczh)
GUI::Framework

Classes:
  Control								：控件基类
  Grid									：排版基类
***********************************************************************/

#ifndef VCZH_PRESENTATION_FRAMEWORK_CONTROL
#define VCZH_PRESENTATION_FRAMEWORK_CONTROL

#include "..\Layout\FreeLayout.h"
#include "..\Layout\DockLayout.h"
#include "..\Layout\TableLayout.h"
#include "..\Layout\StackLayout.h"
#include "..\..\..\..\Library\Event.h"

namespace vl
{
	namespace presentation
	{
		using namespace collections;

		class Control;
		class Grid;

/***********************************************************************
Events
***********************************************************************/

		template<typename T>
		class EventArgs
		{
		public:
			typedef Event<void(Control*, T)>	Handlers;
		};

		class NotifyEventArgs : public EventArgs<NotifyEventArgs>
		{
		public:
		};

		class CancelEventArgs : public EventArgs<CancelEventArgs>
		{
		public:
			bool								cancel;
		};

		class MouseEventArgs : public EventArgs<MouseEventArgs>
		{
		public:
			int									x;
			int									y;
			bool								left;
			bool								middle;
			bool								right;
			int									wheel;
			bool								ctrl;
			bool								shift;
		};

		class KeyEventArgs : public EventArgs<KeyEventArgs>
		{
		public:
			int									keyCode;
		};

		class CharEventArgs : public EventArgs<CharEventArgs>
		{
		public:
			wchar_t								character;
		};

		class BoundsEventArgs : public EventArgs<BoundsEventArgs>
		{
		public:
			Rect								bounds;
		};

/***********************************************************************
Framework
***********************************************************************/

		class Control : public Object
		{
			friend class Grid;
		private:
			LayoutHost									layoutHost;
			Point										visualLocation;
			Size										visualSize;

			Control*									parent;
			Grid*										grid;
			WString										name;
			WString										text;
			bool										autoSizeMode;

			bool										isVisible;
			bool										isEnabled;
			bool										isFocused;
			bool										isMouseOver;
			bool										isTabStopEnabled;
			bool										isConsumeTab;
			int											altShortcutKey;

			bool										updatingGrid;
		protected:
			void										RequestRefresh();
			void										UpdateGrid();
			void										UpdateHost();
			bool										RequestFocus();

		public:
			Control();
			~Control();

			virtual void								ProcessMessage(int message, void* arguments);

			// 控件关系
			virtual Control*							GetParent();
			virtual Grid*								GetGrid();
			virtual void								SetGrid(Grid* value);

			virtual WString								GetText();
			virtual void								SetText(const WString& value);
			virtual WString								GetName();
			virtual void								SetName(const WString& value);

			// 控件排版信息
			virtual Size								GetMinSize();
			virtual void								SetMinSize(Size value);
			virtual Size								GetMaxSize();
			virtual void								SetMaxSize(Size value);
			virtual Point								GetExpectedLocation();
			virtual void								SetExpectedLocation(Point value);
			virtual Size								GetExpectedSize();
			virtual void								SetExpectedSize(Size value);
			virtual Point								GetExpectedClientLocation();
			virtual Size								GetExpectedClientSize();
			virtual void								SetExpectedClientSize(Size value);
			virtual Margin								GetMargin();
			virtual void								SetMargin(Margin value);
			virtual bool								GetAutoSizeMode();
			virtual void								SetAutoSizeMode(bool value);
			virtual Point								GetLocation();
			virtual Size								GetSize();
			virtual Size								GetClientSize();

			// 控件状态
			virtual bool								GetVisible();
			virtual void								SetVisible(bool value);
			virtual bool								GetEnabled();
			virtual void								SetEnabled(bool value);
			virtual bool								GetFocused();
			virtual void								SetFocused(bool value);

			virtual bool								GetTabStopEnabled();
			virtual void								SetTabStopEnabled(bool value);
			virtual bool								GetConsumeTab();
			virtual void								SetConsumeTab(bool value);
			virtual int									GetAltShortcutKey();
			virtual void								SetAltShortcutKey(int value);

			virtual bool								IsMouseOver();
			virtual bool								IsVisuallyVisible();
			virtual bool								IsVisuallyEnabled();

			// 控件事件
			NotifyEventArgs::Handlers					ParentChanged;
			NotifyEventArgs::Handlers					GridChanged;
			NotifyEventArgs::Handlers					TextChanged;
			NotifyEventArgs::Handlers					NameChanged;

			NotifyEventArgs::Handlers					MinSizeChanged;
			NotifyEventArgs::Handlers					MaxSizeChanged;
			NotifyEventArgs::Handlers					ExpectedLocationChanged;
			NotifyEventArgs::Handlers					ExpectedSizeChanged;
			NotifyEventArgs::Handlers					MarginChanged;
			NotifyEventArgs::Handlers					AutoSizeModeChanged;
			NotifyEventArgs::Handlers					LocationChanged;
			NotifyEventArgs::Handlers					SizeChanged;

			NotifyEventArgs::Handlers					VisibleChanged;
			NotifyEventArgs::Handlers					EnabledChanged;
			NotifyEventArgs::Handlers					FocusedChanged;

			NotifyEventArgs::Handlers					TabStopEnabledChanged;
			NotifyEventArgs::Handlers					ConsumeTabChanged;
			NotifyEventArgs::Handlers					AltShortcutKeyChanged;

			NotifyEventArgs::Handlers					MouseOver;
			NotifyEventArgs::Handlers					MouseLeave;
			MouseEventArgs::Handlers					MouseMove;
			MouseEventArgs::Handlers					MouseDown;
			MouseEventArgs::Handlers					MouseUp;
			MouseEventArgs::Handlers					MouseClicked;
			MouseEventArgs::Handlers					MouseDoubleClicked;
			MouseEventArgs::Handlers					VerticalWheel;
			MouseEventArgs::Handlers					HorizontalWheel;
			KeyEventArgs::Handlers						KeyDown;
			KeyEventArgs::Handlers						KeyUp;
			CharEventArgs::Handlers						CharInput;
		};

		class Grid : public Object
		{
			friend class Control;
		protected:
			LayoutBase*									layout;
			Control*									hostControl;
			List<Control*>								children;

			LayoutHost*									GetControlLayoutHost(Control* control);
		public:
			Grid();
			~Grid();

			virtual Control*							GetHostControl();
			virtual const IReadonlyList<Control*>&		GetControls();
			virtual Size								GetMinSize();
			virtual Size								GetMaxSize();
			virtual Size								GetSize();
			virtual void								SetSize(Size value);

			virtual void								UpdateMinMax();
			virtual void								ApplyLayout();
		};

/***********************************************************************
Layout
***********************************************************************/
	}
}

#endif