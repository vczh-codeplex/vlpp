/***********************************************************************
Vczh Library++ 3.0
Developer: �����(vczh)
GUI::Windows Platform

Interfaces:
***********************************************************************/

#ifndef VCZH_PRESENTATION_WINDOWS_GUIGRAPHICSEVENTRECEIVER
#define VCZH_PRESENTATION_WINDOWS_GUIGRAPHICSEVENTRECEIVER

#include "..\NativeWindow\GuiNativeWindow.h"
#include "..\..\..\..\Library\Pointer.h"
#include "..\..\..\..\Library\Function.h"
#include "..\..\..\..\Library\Collections\List.h"

namespace vl
{
	namespace presentation
	{
		namespace elements
		{
			class GuiGraphicsComposition;

/***********************************************************************
Event
***********************************************************************/

			template<typename T>
			class GuiGraphicsEvent : public Object
			{
			public:
				class IHandler : public Interface
				{
				public:
					virtual void			Execute(GuiGraphicsComposition* sender, T& argument)=0;
				};
				
				class FunctionHandler : public Object, public IHandler
				{
				public:
					typedef Func<void(GuiGraphicsComposition*, T&)>		FunctionType;
				protected:
					FunctionType		handler;
				public:
					FunctionHandler(const FunctionType& _handler)
						:handler(_handler)
					{
					}

					void Execute(GuiGraphicsComposition* sender, T& argument)
					{
						handler(sender, argument);
					}
				};
			protected:
				struct HandlerNode
				{
					Ptr<IHandler>					handler;
					Ptr<HandlerNode>				next;
				};

				GuiGraphicsComposition*				sender;
				Ptr<HandlerNode>					handlers;
			public:
				GuiGraphicsEvent(GuiGraphicsComposition* _sender)
					:sender(_sender)
				{
				}

				~GuiGraphicsEvent()
				{
				}

				GuiGraphicsComposition* GetAssociatedComposition()
				{
					return sender;
				}

				bool Attach(Ptr<IHandler> handler)
				{
					Ptr<HandlerNode>* currentHandler=&handlers;
					while(*currentHandler)
					{
						if((*currentHandler)->handler==handler)
						{
							return false;
						}
						else
						{
							currentHandler=&(*currentHandler)->next;
						}
					}
					(*currentHandler)=new HandlerNode;
					(*currentHandler)->handler=handler;
					return true;
				}

				bool Detach(Ptr<IHandler> handler)
				{
					Ptr<HandlerNode>* currentHandler=&handlers;
					while(*currentHandler)
					{
						if((*currentHandler)->handler==handler)
						{
							(*currentHandler)=(*currentHandler)->next;
							return true;
						}
						else
						{
							currentHandler=&(*currentHandler)->next;
						}
					}
					return false;
				}

				void Execute(T& argument)
				{
					Ptr<HandlerNode>* currentHandler=&handlers;
					while(*currentHandler)
					{
						(*currentHandler)->handler->Execute(sender, argument);
						currentHandler=&(*currentHandler)->next;
					}
				}
			};

/***********************************************************************
Predefined Events
***********************************************************************/

			struct GuiEventArgs
			{
				GuiGraphicsComposition*		compositionSource;	// the deepest composition
				GuiGraphicsComposition*		eventSource;		// the deepest composition that contains an event receiver

				GuiEventArgs()
					:compositionSource(0)
					,eventSource(0)
				{
				}
			};

			struct GuiRequestEventArgs : public GuiEventArgs
			{
				bool		cancel;
			};

			struct GuiKeyEventArgs : public GuiEventArgs
			{
				int			code;
				bool		alt;
			};

			struct GuiCharEventArgs : public GuiEventArgs
			{
				wchar_t		value;
			};

			struct GuiMouseEventArgs : public GuiEventArgs, public NativeWindowMouseInfo
			{
			};

			typedef GuiGraphicsEvent<GuiEventArgs>				GuiNotifyEvent;
			typedef GuiGraphicsEvent<GuiRequestEventArgs>		GuiRequestEvent;
			typedef GuiGraphicsEvent<GuiKeyEventArgs>			GuiKeyEvent;
			typedef GuiGraphicsEvent<GuiCharEventArgs>			GuiCharEvent;
			typedef GuiGraphicsEvent<GuiMouseEventArgs>			GuiMouseEvent;

/***********************************************************************
Event Receiver
***********************************************************************/

			class GuiGraphicsEventReceiver : public Object
			{
			protected:
				GuiGraphicsComposition*			sender;
				bool							enabled;
			public:
				GuiGraphicsEventReceiver(GuiGraphicsComposition* _sender);
				~GuiGraphicsEventReceiver();

				bool							GetEnabled();
				void							SetEnabled(bool value);
				GuiGraphicsComposition*			GetAssociatedComposition();

				GuiMouseEvent					leftButtonDown;
				GuiMouseEvent					leftButtonUp;
				GuiMouseEvent					leftButtonDoubleClick;
				GuiMouseEvent					middleButtonDown;
				GuiMouseEvent					middleButtonUp;
				GuiMouseEvent					middleButtonDoubleClick;
				GuiMouseEvent					rightButtonDown;
				GuiMouseEvent					rightButtonUp;
				GuiMouseEvent					rightButtonDoubleClick;
				GuiMouseEvent					horizontalWheel;
				GuiMouseEvent					verticalWheel;
				GuiMouseEvent					mouseMove;
				GuiNotifyEvent					mouseEnter;
				GuiNotifyEvent					mouseLeave;

				GuiKeyEvent						keyDown;
				GuiKeyEvent						keyUp;
				GuiKeyEvent						systemKeyDown;
				GuiKeyEvent						systemKeyUp;
				GuiCharEvent					charInput;
			};
		}
	}
}

#endif