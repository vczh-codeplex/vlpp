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
				for(int i=playingAnimations.Count();i>=0;i--)
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

			void GuiGraphicsHost::GlobalTimer()
			{
				if(animationManager.HasAnimation())
				{
					animationManager.Play();
					Render();
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