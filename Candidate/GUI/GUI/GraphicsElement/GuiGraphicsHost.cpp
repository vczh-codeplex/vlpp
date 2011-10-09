#include "GuiGraphicsHost.h"

namespace vl
{
	namespace presentation
	{
		namespace elements
		{

/***********************************************************************
GuiGraphicsAnimationManager
***********************************************************************/

			GuiGraphicsAnimationManager::GuiGraphicsAnimationManager()
			{
			}

			GuiGraphicsAnimationManager::~GuiGraphicsAnimationManager()
			{
			}

			void GuiGraphicsAnimationManager::AddAnimation(Ptr<IGuiGraphicsAnimation> animation)
			{
				playingAnimations.Add(animation);
			}

			bool GuiGraphicsAnimationManager::HasAnimation()
			{
				return playingAnimations.Count()>0;
			}

			void GuiGraphicsAnimationManager::Play()
			{
				for(int i=playingAnimations.Count()-1;i>=0;i--)
				{
					Ptr<IGuiGraphicsAnimation> animation=playingAnimations[i];
					int totalLength=animation->GetTotalLength();
					int currentPosition=animation->GetCurrentPosition();
					animation->Play(currentPosition, totalLength);
					if(currentPosition>=totalLength)
					{
						playingAnimations.RemoveAt(i);
						animation->Stop();
					}
				}
			}

/***********************************************************************
GuiGraphicsHost
***********************************************************************/

			void GuiGraphicsHost::RaiseMouseEvent(GuiMouseEventArgs& arguments, GuiGraphicsComposition* composition, GuiMouseEvent GuiGraphicsEventReceiver::* eventReceiverEvent)
			{
				arguments.compositionSource=composition;
				arguments.eventSource=0;
				int x=arguments.x;
				int y=arguments.y;

				while(composition)
				{
					if(composition->HasEventReceiver())
					{
						if(!arguments.eventSource)
						{
							arguments.eventSource=composition;
						}
						GuiGraphicsEventReceiver* eventReceiver=composition->GetEventReceiver();
						if(eventReceiver->GetEnabled())
						{
							(eventReceiver->*eventReceiverEvent).Execute(arguments);
						}
					}

					GuiGraphicsComposition* parent=composition->GetParent();
					if(parent)
					{
						Rect parentBounds=parent->GetBounds();
						Rect clientArea=parent->GetClientArea();
						Rect childBounds=composition->GetBounds();

						x+=childBounds.x1+(clientArea.x1-parentBounds.x1);
						y+=childBounds.y1+(clientArea.y1-parentBounds.y1);
						arguments.x=x;
						arguments.y=y;
					}
					composition=parent;
				}
			}

			void GuiGraphicsHost::OnMouseInput(const NativeWindowMouseInfo& info, GuiMouseEvent GuiGraphicsEventReceiver::* eventReceiverEvent)
			{
				Rect bounds;
				GuiGraphicsComposition* composition=windowComposition->FindComposition(Point(info.x, info.y), bounds);
				if(composition)
				{
					GuiMouseEventArgs arguments;
					(NativeWindowMouseInfo&)arguments=info;
					arguments.x-=bounds.x1;
					arguments.y-=bounds.y1;
					RaiseMouseEvent(arguments, composition, eventReceiverEvent);
				}
			}

			void GuiGraphicsHost::Moving(Rect& bounds)
			{
				Rect oldBounds=nativeWindow->GetBounds();
				Size minWindowSize=minSize+(oldBounds.GetSize()-nativeWindow->GetClientSize());
				if(bounds.Width()<minWindowSize.x)
				{
					if(oldBounds.x1!=bounds.x1)
					{
						bounds.x1=oldBounds.x2-minWindowSize.x;
					}
					else if(oldBounds.x2!=bounds.x2)
					{
						bounds.x2=oldBounds.x1+minWindowSize.x;
					}
				}
				if(bounds.Height()<minWindowSize.y)
				{
					if(oldBounds.y1!=bounds.y1)
					{
						bounds.y1=oldBounds.y2-minWindowSize.y;
					}
					else if(oldBounds.y2!=bounds.y2)
					{
						bounds.y2=oldBounds.y1+minWindowSize.y;
					}
				}
			}

			void GuiGraphicsHost::Moved()
			{
				Size size=nativeWindow->GetClientSize();
				if(previousClientSize!=size)
				{
					previousClientSize=size;
					minSize=windowComposition->GetMinNecessaryBounds().GetSize();
					Render();
				}
			}

			void GuiGraphicsHost::LeftButtonDown(const NativeWindowMouseInfo& info)
			{
				OnMouseInput(info, &GuiGraphicsEventReceiver::leftButtonDown);
			}

			void GuiGraphicsHost::LeftButtonUp(const NativeWindowMouseInfo& info)
			{
				OnMouseInput(info, &GuiGraphicsEventReceiver::leftButtonUp);
			}

			void GuiGraphicsHost::LeftButtonDoubleClick(const NativeWindowMouseInfo& info)
			{
				OnMouseInput(info, &GuiGraphicsEventReceiver::leftButtonDoubleClick);
			}

			void GuiGraphicsHost::RightButtonDown(const NativeWindowMouseInfo& info)
			{
				OnMouseInput(info, &GuiGraphicsEventReceiver::rightButtonDown);
			}

			void GuiGraphicsHost::RightButtonUp(const NativeWindowMouseInfo& info)
			{
				OnMouseInput(info, &GuiGraphicsEventReceiver::rightButtonUp);
			}

			void GuiGraphicsHost::RightButtonDoubleClick(const NativeWindowMouseInfo& info)
			{
				OnMouseInput(info, &GuiGraphicsEventReceiver::rightButtonDoubleClick);
			}

			void GuiGraphicsHost::MiddleButtonDown(const NativeWindowMouseInfo& info)
			{
				OnMouseInput(info, &GuiGraphicsEventReceiver::middleButtonDown);
			}

			void GuiGraphicsHost::MiddleButtonUp(const NativeWindowMouseInfo& info)
			{
				OnMouseInput(info, &GuiGraphicsEventReceiver::middleButtonUp);
			}

			void GuiGraphicsHost::MiddleButtonDoubleClick(const NativeWindowMouseInfo& info)
			{
				OnMouseInput(info, &GuiGraphicsEventReceiver::middleButtonDoubleClick);
			}

			void GuiGraphicsHost::HorizontalWheel(const NativeWindowMouseInfo& info)
			{
			}

			void GuiGraphicsHost::VerticalWheel(const NativeWindowMouseInfo& info)
			{
			}

			void GuiGraphicsHost::MouseMoving(const NativeWindowMouseInfo& info)
			{
				OnMouseInput(info, &GuiGraphicsEventReceiver::mouseMove);
			}

			void GuiGraphicsHost::MouseEntered()
			{
			}

			void GuiGraphicsHost::MouseLeaved()
			{
			}

			void GuiGraphicsHost::KeyDown(int code, bool alt)
			{
			}

			void GuiGraphicsHost::KeyUp(int code, bool alt)
			{
			}

			void GuiGraphicsHost::SysKeyDown(int code, bool alt)
			{
			}

			void GuiGraphicsHost::SysKeyUp(int code, bool alt)
			{
			}

			void GuiGraphicsHost::Char(wchar_t keyChar)
			{
			}

			void GuiGraphicsHost::GlobalTimer()
			{
				if(animationManager.HasAnimation())
				{
					animationManager.Play();
					Render();
					nativeWindow->RedrawContent();
				}
			}

			GuiGraphicsHost::GuiGraphicsHost()
				:nativeWindow(0)
				,windowComposition(0)
			{
				windowComposition=new GuiWindowComposition;
				windowComposition->SetMinSizeLimitation(GuiGraphicsComposition::LimitToElementAndChildren);
			}

			GuiGraphicsHost::~GuiGraphicsHost()
			{
				delete windowComposition;
			}

			INativeWindow* GuiGraphicsHost::GetNativeWindow()
			{
				return nativeWindow;
			}

			void GuiGraphicsHost::SetNativeWindow(INativeWindow* _nativeWindow)
			{
				if(nativeWindow!=_nativeWindow)
				{
					if(nativeWindow)
					{
						GetCurrentController()->UninstallListener(this);
						nativeWindow->UninstallListener(this);
					}
					nativeWindow=_nativeWindow;
					windowComposition->SetAttachedWindow(nativeWindow);
					if(nativeWindow)
					{
						nativeWindow->InstallListener(this);
						GetCurrentController()->InstallListener(this);
						previousClientSize=nativeWindow->GetClientSize();
						minSize=windowComposition->GetMinNecessaryBounds().GetSize();
						Render();
					}
				}
			}

			GuiGraphicsComposition* GuiGraphicsHost::GetMainComposition()
			{
				return windowComposition;
			}

			void GuiGraphicsHost::Render()
			{
				windowComposition->GetRenderTarget()->StartRendering();
				windowComposition->Render(Size());
				windowComposition->GetRenderTarget()->StopRendering();
			}

			GuiGraphicsAnimationManager* GuiGraphicsHost::GetAnimationManager()
			{
				return &animationManager;
			}

/***********************************************************************
GuiTimeBasedAnimation
***********************************************************************/

			GuiTimeBasedAnimation::GuiTimeBasedAnimation(int totalMilliseconds)
				:startTime(0)
				,length(totalMilliseconds)
			{
				Restart();
			}

			GuiTimeBasedAnimation::~GuiTimeBasedAnimation()
			{
			}

			void GuiTimeBasedAnimation::Restart()
			{
				startTime=DateTime::LocalTime().totalMilliseconds;
			}

			int GuiTimeBasedAnimation::GetTotalLength()
			{
				return length;
			}

			int GuiTimeBasedAnimation::GetCurrentPosition()
			{
				return (int)(DateTime::LocalTime().totalMilliseconds-startTime);
			}
		}
	}
}