/***********************************************************************
Vczh Library++ 3.0
Developer: ³Âè÷å«(vczh)
GUI::Windows Platform

Interfaces:
***********************************************************************/

#ifndef VCZH_PRESENTATION_WINDOWS_GUIGRAPHICSHOST
#define VCZH_PRESENTATION_WINDOWS_GUIGRAPHICSHOST

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
			protected:
				INativeWindow*					nativeWindow;
				GuiWindowComposition*			windowComposition;
				Size							previousClientSize;
				Size							minSize;

				GuiGraphicsAnimationManager		animationManager;

			private:
				void							Moving(Rect& bounds);
				void							Moved();

				void							GlobalTimer();
			public:
				GuiGraphicsHost();
				~GuiGraphicsHost();

				INativeWindow*					GetNativeWindow();
				void							SetNativeWindow(INativeWindow* _nativeWindow);
				GuiGraphicsComposition*			GetMainComposition();
				void							Render();

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

				void							Restart();
				int								GetTotalLength();
				int								GetCurrentPosition();
			};
		}
	}
}

#endif