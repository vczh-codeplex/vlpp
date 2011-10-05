/***********************************************************************
Vczh Library++ 3.0
Developer: ³Âè÷å«(vczh)
GUI::Windows Platform

Interfaces:
***********************************************************************/

#ifndef VCZH_PRESENTATION_WINDOWS_GUIGRAPHICSELEMENT
#define VCZH_PRESENTATION_WINDOWS_GUIGRAPHICSELEMENT

#include "..\NativeWindow\GuiNativeWindow.h"

namespace vl
{
	namespace presentation
	{
		namespace windows
		{
			class IGuiGraphicsElement;
			class IGuiGraphicsElementFactory;
			class IGuiGraphicsRenderer;
			class IGuiGraphicsRendererFactory;
			class IGuiGraphicsRenderTarget;
			class IGuiGraphicsResourceManager;

/***********************************************************************
Basic Construction
***********************************************************************/

			class IGuiGraphicsElement : public Object
			{
			public:
				virtual int								GetElementTypeId()=0;
				virtual IGuiGraphicsElementFactory*		GetFactory()=0;
				virtual IGuiGraphicsRenderer*			GetRenderer()=0;
			};

			class IGuiGraphicsElementFactory : public Object
			{
			public:
				virtual WString							GetElementTypeName()=0;
				virtual IGuiGraphicsElement*			Create()=0;
			};

			class IGuiGraphicsRenderer : public Object
			{
			public:
				virtual IGuiGraphicsRendererFactory*	GetFactory()=0;
				virtual void							Initialize(IGuiGraphicsElement* element)=0;
				virtual void							SetRenderTarget(IGuiGraphicsRenderTarget* renderTarget)=0;
				virtual void							Render(Rect bounds)=0;
				virtual void							OnElementStateChanged()=0;
			};

			class IGuiGraphicsRendererFactory : public Object
			{
			public:
				virtual IGuiGraphicsRenderer*			Create()=0;
			};

			class IGuiGraphicsRenderTarget : public Object
			{
			};

/***********************************************************************
Resource Manager
***********************************************************************/

			class IGuiGraphicsResourceManager : public Object
			{
			public:
				virtual int								RegisterElementFactory(IGuiGraphicsElementFactory* factory)=0;
				virtual void							RegisterRendererFactory(int elementTypeId, IGuiGraphicsRendererFactory* factory)=0;
				virtual IGuiGraphicsElementFactory*		GetElementFactory(const WString& elementTypeName)=0;
				virtual int								GetElementTypeId(IGuiGraphicsElementFactory* factory)=0;
				virtual IGuiGraphicsRendererFactory*	GetRendererFactory(int elementTypeId)=0;
			};

			extern IGuiGraphicsResourceManager*			GetGuiGraphicsResourceManager();
			extern void									SetGuiGraphicsResourceManager(IGuiGraphicsResourceManager* resourceManager);

/***********************************************************************
Elements
***********************************************************************/
		}
	}
}

#endif