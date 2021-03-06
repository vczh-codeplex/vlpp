/***********************************************************************
Vczh Library++ 3.0
Developer: �����(vczh)
GacUI::Event Model

Interfaces:
***********************************************************************/

#ifndef VCZH_PRESENTATION_ELEMENTS_GUIGRAPHICSEVENTRECEIVER
#define VCZH_PRESENTATION_ELEMENTS_GUIGRAPHICSEVENTRECEIVER

#include "..\NativeWindow\GuiNativeWindow.h"
#include "..\Reflection\GuiTypeDescriptor.h"

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
			class GuiGraphicsEvent : public Object, public Description<GuiGraphicsEvent<T>>
			{
			public:
				typedef void(RawFunctionType)(GuiGraphicsComposition*, T&);
				typedef Func<RawFunctionType>						FunctionType;

				class IHandler : public virtual IDescriptable, public Description<IHandler>
				{
				public:
					virtual void			Execute(GuiGraphicsComposition* sender, T& argument)=0;
				};
				
				class FunctionHandler : public Object, public IHandler
				{
				protected:
					FunctionType		handler;
				public:
					FunctionHandler(const FunctionType& _handler)
						:handler(_handler)
					{
					}

					void Execute(GuiGraphicsComposition* sender, T& argument)override
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
				GuiGraphicsEvent(GuiGraphicsComposition* _sender=0)
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

				void SetAssociatedComposition(GuiGraphicsComposition* _sender)
				{
					sender=_sender;
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

				template<typename TClass, typename TMethod>
				Ptr<IHandler> AttachMethod(TClass* receiver, TMethod TClass::* method)
				{
					Ptr<IHandler> handler=new FunctionHandler(FunctionType(receiver, method));
					Attach(handler);
					return handler;
				}

				Ptr<IHandler> AttachFunction(RawFunctionType* function)
				{
					Ptr<IHandler> handler=new FunctionHandler(FunctionType(function));
					Attach(handler);
					return handler;
				}

				template<typename T>
				Ptr<IHandler> AttachLambda(const T& lambda)
				{
					Ptr<IHandler> handler=new FunctionHandler(FunctionType(lambda));
					Attach(handler);
					return handler;
				}

				bool Detach(Ptr<IHandler> handler)
				{
					Ptr<HandlerNode>* currentHandler=&handlers;
					while(*currentHandler)
					{
						if((*currentHandler)->handler==handler)
						{
							Ptr<HandlerNode> next=(*currentHandler)->next;
							(*currentHandler)=next;
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
				bool						handled;

				GuiEventArgs(GuiGraphicsComposition* composition)
					:compositionSource(composition)
					,eventSource(composition)
					,handled(false)
				{
				}

				GuiEventArgs()
					:compositionSource(0)
					,eventSource(0)
					,handled(false)
				{
				}
			};

			struct GuiRequestEventArgs : public GuiEventArgs
			{
				bool		cancel;

				GuiRequestEventArgs()
					:cancel(false)
				{
				}

				GuiRequestEventArgs(GuiGraphicsComposition* composition)
					:GuiEventArgs(composition)
					,cancel(false)
				{
				}
			};

			struct GuiKeyEventArgs : public GuiEventArgs, public NativeWindowKeyInfo
			{
				GuiKeyEventArgs()
				{
				}

				GuiKeyEventArgs(GuiGraphicsComposition* composition)
					:GuiEventArgs(composition)
				{
				}
			};

			struct GuiCharEventArgs : public GuiEventArgs, public NativeWindowCharInfo
			{
				GuiCharEventArgs()
				{
				}

				GuiCharEventArgs(GuiGraphicsComposition* composition)
					:GuiEventArgs(composition)
				{
				}
			};

			struct GuiMouseEventArgs : public GuiEventArgs, public NativeWindowMouseInfo
			{
				GuiMouseEventArgs()
				{
				}

				GuiMouseEventArgs(GuiGraphicsComposition* composition)
					:GuiEventArgs(composition)
				{
				}
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
			public:
				GuiGraphicsEventReceiver(GuiGraphicsComposition* _sender);
				~GuiGraphicsEventReceiver();

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

				GuiKeyEvent						previewKey;
				GuiKeyEvent						keyDown;
				GuiKeyEvent						keyUp;
				GuiKeyEvent						systemKeyDown;
				GuiKeyEvent						systemKeyUp;
				GuiCharEvent					previewCharInput;
				GuiCharEvent					charInput;
				GuiNotifyEvent					gotFocus;
				GuiNotifyEvent					lostFocus;
				GuiNotifyEvent					caretNotify;
			};
		}
	}
}

#endif