#include "GuiGraphicsEventReceiver.h"

namespace vl
{
	namespace presentation
	{
		namespace elements
		{

/***********************************************************************
Event Receiver
***********************************************************************/

			GuiGraphicsEventReceiver::GuiGraphicsEventReceiver(GuiGraphicsComposition* _sender)
				:sender(_sender)
				,leftButtonDown(_sender)
				,leftButtonUp(_sender)
				,leftButtonDoubleClick(_sender)
				,middleButtonDown(_sender)
				,middleButtonUp(_sender)
				,middleButtonDoubleClick(_sender)
				,rightButtonDown(_sender)
				,rightButtonUp(_sender)
				,rightButtonDoubleClick(_sender)
				,horizontalWheel(_sender)
				,verticalWheel(_sender)
				,mouseMove(_sender)
				,mouseEnter(_sender)
				,mouseLeave(_sender)
				,keyDown(_sender)
				,keyUp(_sender)
				,systemKeyDown(_sender)
				,systemKeyUp(_sender)
				,charInput(_sender)
			{
			}

			GuiGraphicsEventReceiver::~GuiGraphicsEventReceiver()
			{
			}

			GuiGraphicsComposition* GuiGraphicsEventReceiver::GetAssociatedComposition()
			{
				return sender;
			}
		}
	}
}