/***********************************************************************
Vczh Library++ 3.0
Developer: �����(vczh)
GUI::Windows Platform

Interfaces:
***********************************************************************/

#ifndef VCZH_PRESENTATION_ELEMENTS_GUIGRAPHICSHOST
#define VCZH_PRESENTATION_ELEMENTS_GUIGRAPHICSHOST

#include "GuiGraphicsComposition.h"

namespace vl
{
	namespace presentation
	{
		namespace elements
		{

/***********************************************************************
Animation
***********************************************************************/

			class IGuiGraphicsAnimation : public Interface
			{
			public:
				virtual int						GetTotalLength()=0;
				virtual int						GetCurrentPosition()=0;
				virtual void					Play(int currentPosition, int totalLength)=0;
				virtual void					Stop()=0;
			};

			class GuiGraphicsAnimationManager : public Object
			{
				typedef collections::List<Ptr<IGuiGraphicsAnimation>>		AnimationList;
			protected:
				AnimationList					playingAnimations;
			public:
				GuiGraphicsAnimationManager();
				~GuiGraphicsAnimationManager();

				void							AddAnimation(Ptr<IGuiGraphicsAnimation> animation);
				bool							HasAnimation();
				void							Play();
			};

/***********************************************************************
Host
***********************************************************************/

			class GuiGraphicsHost : public Object, private INativeWindowListener, private INativeControllerListener
			{
				typedef collections::List<GuiGraphicsComposition*>		CompositionList;
			protected:
				INativeWindow*					nativeWindow;
				GuiWindowComposition*			windowComposition;
				GuiGraphicsComposition*			focusedComposition;
				Size							previousClientSize;
				Size							minSize;

				GuiGraphicsAnimationManager		animationManager;
				GuiGraphicsComposition*			mouseCaptureComposition;
				CompositionList					mouseEnterCompositions;

				void							MouseCapture(const NativeWindowMouseInfo& info);
				void							MouseUncapture(const NativeWindowMouseInfo& info);
			private:
				void							RaiseMouseEvent(GuiMouseEventArgs& arguments, GuiGraphicsComposition* composition, GuiMouseEvent GuiGraphicsEventReceiver::* eventReceiverEvent);
				void							OnMouseInput(const NativeWindowMouseInfo& info, GuiMouseEvent GuiGraphicsEventReceiver::* eventReceiverEvent);

				void							Moving(Rect& bounds);
				void							Moved();

				void							LeftButtonDown(const NativeWindowMouseInfo& info);
				void							LeftButtonUp(const NativeWindowMouseInfo& info);
				void							LeftButtonDoubleClick(const NativeWindowMouseInfo& info);
				void							RightButtonDown(const NativeWindowMouseInfo& info);
				void							RightButtonUp(const NativeWindowMouseInfo& info);
				void							RightButtonDoubleClick(const NativeWindowMouseInfo& info);
				void							MiddleButtonDown(const NativeWindowMouseInfo& info);
				void							MiddleButtonUp(const NativeWindowMouseInfo& info);
				void							MiddleButtonDoubleClick(const NativeWindowMouseInfo& info);
				void							HorizontalWheel(const NativeWindowMouseInfo& info);
				void							VerticalWheel(const NativeWindowMouseInfo& info);
				void							MouseMoving(const NativeWindowMouseInfo& info);
				void							MouseEntered();
				void							MouseLeaved();

				void							KeyDown(int code, bool alt);
				void							KeyUp(int code, bool alt);
				void							SysKeyDown(int code, bool alt);
				void							SysKeyUp(int code, bool alt);
				void							Char(wchar_t keyChar);

				void							GlobalTimer();
			public:
				GuiGraphicsHost();
				~GuiGraphicsHost();

				INativeWindow*					GetNativeWindow();
				void							SetNativeWindow(INativeWindow* _nativeWindow);
				GuiGraphicsComposition*			GetMainComposition();
				void							Render();
				bool							SetFocus(GuiGraphicsComposition* composition);

				GuiGraphicsAnimationManager*	GetAnimationManager();
			};

/***********************************************************************
Helpers
***********************************************************************/

			class GuiTimeBasedAnimation : public IGuiGraphicsAnimation
			{
			protected:
				unsigned __int64				startTime;
				int								length;
			public:
				GuiTimeBasedAnimation(int totalMilliseconds);
				~GuiTimeBasedAnimation();

				void							Restart(int totalMilliseconds=-1);
				int								GetTotalLength();
				int								GetCurrentPosition();
			};
		}
	}
}

#endif