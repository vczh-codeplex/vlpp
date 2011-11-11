/***********************************************************************
Vczh Library++ 3.0
Developer: ³Âè÷å«(vczh)
GacUI::Graphics Composition Host

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
			public:
				static const unsigned __int64	CaretInterval=500;
			protected:
				INativeWindow*					nativeWindow;
				GuiWindowComposition*			windowComposition;
				GuiGraphicsComposition*			focusedComposition;
				Size							previousClientSize;
				Size							minSize;
				Point							caretPoint;
				unsigned __int64				lastCaretTime;

				GuiGraphicsAnimationManager		animationManager;
				GuiGraphicsComposition*			mouseCaptureComposition;
				CompositionList					mouseEnterCompositions;

				void							MouseCapture(const NativeWindowMouseInfo& info);
				void							MouseUncapture(const NativeWindowMouseInfo& info);
				void							OnCharInput(const NativeWindowCharInfo& info, GuiGraphicsComposition* composition, GuiCharEvent GuiGraphicsEventReceiver::* eventReceiverEvent);
				void							OnKeyInput(const NativeWindowKeyInfo& info, GuiGraphicsComposition* composition, GuiKeyEvent GuiGraphicsEventReceiver::* eventReceiverEvent);
				void							RaiseMouseEvent(GuiMouseEventArgs& arguments, GuiGraphicsComposition* composition, GuiMouseEvent GuiGraphicsEventReceiver::* eventReceiverEvent);
				void							OnMouseInput(const NativeWindowMouseInfo& info, GuiMouseEvent GuiGraphicsEventReceiver::* eventReceiverEvent);
				
			private:
				void							Moving(Rect& bounds, bool fixSizeOnly)override;
				void							Moved()override;

				void							LeftButtonDown(const NativeWindowMouseInfo& info)override;
				void							LeftButtonUp(const NativeWindowMouseInfo& info)override;
				void							LeftButtonDoubleClick(const NativeWindowMouseInfo& info)override;
				void							RightButtonDown(const NativeWindowMouseInfo& info)override;
				void							RightButtonUp(const NativeWindowMouseInfo& info)override;
				void							RightButtonDoubleClick(const NativeWindowMouseInfo& info)override;
				void							MiddleButtonDown(const NativeWindowMouseInfo& info)override;
				void							MiddleButtonUp(const NativeWindowMouseInfo& info)override;
				void							MiddleButtonDoubleClick(const NativeWindowMouseInfo& info)override;
				void							HorizontalWheel(const NativeWindowMouseInfo& info)override;
				void							VerticalWheel(const NativeWindowMouseInfo& info)override;
				void							MouseMoving(const NativeWindowMouseInfo& info)override;
				void							MouseEntered()override;
				void							MouseLeaved()override;

				void							KeyDown(const NativeWindowKeyInfo& info)override;
				void							KeyUp(const NativeWindowKeyInfo& info)override;
				void							SysKeyDown(const NativeWindowKeyInfo& info)override;
				void							SysKeyUp(const NativeWindowKeyInfo& info)override;
				void							Char(const NativeWindowCharInfo& info)override;

				void							GlobalTimer()override;
			public:
				GuiGraphicsHost();
				~GuiGraphicsHost();

				INativeWindow*					GetNativeWindow();
				void							SetNativeWindow(INativeWindow* _nativeWindow);
				GuiGraphicsComposition*			GetMainComposition();
				void							Render();

				bool							SetFocus(GuiGraphicsComposition* composition);
				GuiGraphicsComposition*			GetFocusedComposition();
				Point							GetCaretPoint();
				void							SetCaretPoint(Point value, GuiGraphicsComposition* referenceComposition=0);

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
				int								GetTotalLength()override;
				int								GetCurrentPosition()override;
			};
		}
	}
}

#endif