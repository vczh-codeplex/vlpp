/***********************************************************************
Vczh Library++ 3.0
Developer: ³Âè÷å«(vczh)
GUI::Windows Platform

Interfaces:
***********************************************************************/

#ifndef VCZH_PRESENTATION_WINDOWS_GUIGRAPHICSELEMENT
#define VCZH_PRESENTATION_WINDOWS_GUIGRAPHICSELEMENT

#include "..\NativeWindow\GuiNativeWindow.h"
#include "..\..\..\..\Library\Pointer.h"
#include "..\..\..\..\Library\Collections\Dictionary.h"

namespace vl
{
	namespace presentation
	{
		namespace elements
		{
			class IGuiGraphicsElement;
			class IGuiGraphicsElementFactory;
			class IGuiGraphicsRenderer;
			class IGuiGraphicsRendererFactory;
			class IGuiGraphicsRenderTarget;

/***********************************************************************
Basic Construction
***********************************************************************/

			class IGuiGraphicsElement : public Interface
			{
			public:
				virtual IGuiGraphicsElementFactory*		GetFactory()=0;
				virtual IGuiGraphicsRenderer*			GetRenderer()=0;
			};

			class IGuiGraphicsElementFactory : public Interface
			{
			public:
				virtual WString							GetElementTypeName()=0;
				virtual IGuiGraphicsElement*			Create()=0;
			};

			class IGuiGraphicsRenderer : public Interface
			{
			public:
				virtual IGuiGraphicsRendererFactory*	GetFactory()=0;
				virtual void							Initialize(IGuiGraphicsElement* element)=0;
				virtual void							SetRenderTarget(IGuiGraphicsRenderTarget* renderTarget)=0;
				virtual void							Render(Rect bounds)=0;
				virtual void							OnElementStateChanged()=0;
				virtual Size							GetMinSize()=0;
			};

			class IGuiGraphicsRendererFactory : public Interface
			{
			public:
				virtual IGuiGraphicsRenderer*			Create()=0;
			};

			class IGuiGraphicsRenderTarget : public Interface
			{
			public:
				virtual void							StartRendering()=0;
				virtual void							StopRendering()=0;
				virtual void							PushClipper(Rect clipper)=0;
				virtual void							PopClipper()=0;
				virtual Rect							GetClipper()=0;
				virtual bool							IsClipperCoverWholeTarget()=0;
			};

/***********************************************************************
Resource Manager
***********************************************************************/

			class GuiGraphicsResourceManager : public Object
			{
				typedef collections::Dictionary<WString, Ptr<IGuiGraphicsElementFactory>>		elementFactoryMap;
				typedef collections::Dictionary<WString, Ptr<IGuiGraphicsRendererFactory>>		rendererFactoryMap;
			protected:
				elementFactoryMap						elementFactories;
				rendererFactoryMap						rendererFactories;
			public:
				GuiGraphicsResourceManager();
				~GuiGraphicsResourceManager();

				virtual bool							RegisterElementFactory(IGuiGraphicsElementFactory* factory);
				virtual bool							RegisterRendererFactory(const WString& elementTypeName, IGuiGraphicsRendererFactory* factory);
				virtual IGuiGraphicsElementFactory*		GetElementFactory(const WString& elementTypeName);
				virtual IGuiGraphicsRendererFactory*	GetRendererFactory(const WString& elementTypeName);
				virtual IGuiGraphicsRenderTarget*		GetRenderTarget(INativeWindow* window)=0;
			};

			extern GuiGraphicsResourceManager*			GetGuiGraphicsResourceManager();
			extern void									SetGuiGraphicsResourceManager(GuiGraphicsResourceManager* resourceManager);
			extern bool									RegisterFactories(IGuiGraphicsElementFactory* elementFactory, IGuiGraphicsRendererFactory* rendererFactory);

/***********************************************************************
Helpers
***********************************************************************/

#define DEFINE_GUI_GRAPHICS_ELEMENT(TELEMENT, ELEMENT_TYPE_NAME)\
			public:\
				class Factory : public Object, public IGuiGraphicsElementFactory\
				{\
				public:\
					WString GetElementTypeName()\
					{\
						return TELEMENT::GetElementTypeName();\
					}\
					IGuiGraphicsElement* Create()\
					{\
						TELEMENT* element=new TELEMENT;\
						element->factory=this;\
						element->renderer=GetGuiGraphicsResourceManager()->GetRendererFactory(GetElementTypeName())->Create();\
						element->renderer->Initialize(element);\
						return element;\
					}\
				};\
			protected:\
				IGuiGraphicsElementFactory*		factory;\
				Ptr<IGuiGraphicsRenderer>		renderer;\
			public:\
				static WString GetElementTypeName()\
				{\
					return ELEMENT_TYPE_NAME;\
				}\
				static TELEMENT* Create()\
				{\
					return dynamic_cast<TELEMENT*>(GetGuiGraphicsResourceManager()->GetElementFactory(TELEMENT::GetElementTypeName())->Create());\
				}\
				IGuiGraphicsElementFactory* GetFactory()\
				{\
					return factory;\
				}\
				IGuiGraphicsRenderer* GetRenderer()\
				{\
					return renderer.Obj();\
				}\

#define DEFINE_GUI_GRAPHICS_RENDERER(TELEMENT, TRENDERER, TTARGET)\
			public:\
				class Factory : public Object, public IGuiGraphicsRendererFactory\
				{\
				public:\
					IGuiGraphicsRenderer* Create()\
					{\
						TRENDERER* renderer=new TRENDERER;\
						renderer->factory=this;\
						renderer->element=0;\
						renderer->renderTarget=0;\
						return renderer;\
					}\
				};\
			protected:\
				IGuiGraphicsRendererFactory*	factory;\
				TELEMENT*						element;\
				TTARGET*						renderTarget;\
				Size							minSize;\
			public:\
				static void Register()\
				{\
					RegisterFactories(new TELEMENT::Factory, new TRENDERER::Factory);\
				}\
				IGuiGraphicsRendererFactory* GetFactory()\
				{\
					return factory;\
				}\
				void Initialize(IGuiGraphicsElement* _element)\
				{\
					element=dynamic_cast<TELEMENT*>(_element);\
					InitializeInternal();\
				}\
				void SetRenderTarget(IGuiGraphicsRenderTarget* _renderTarget)\
				{\
					renderTarget=dynamic_cast<TTARGET*>(_renderTarget);\
					RenderTargetChangedInternal();\
				}\
				Size GetMinSize()\
				{\
					return minSize;\
				}\

/***********************************************************************
Elements
***********************************************************************/

			class GuiSolidBorderElement : public Object, public IGuiGraphicsElement
			{
				DEFINE_GUI_GRAPHICS_ELEMENT(GuiSolidBorderElement, L"SolidBorder")
			protected:
				Color					color;
			public:
				GuiSolidBorderElement();
				~GuiSolidBorderElement();

				Color					GetColor();
				void					SetColor(Color value);
			};

			class GuiSolidBackgroundElement : public Object, public IGuiGraphicsElement
			{
				DEFINE_GUI_GRAPHICS_ELEMENT(GuiSolidBackgroundElement, L"SolidBackground")
			protected:
				Color					color;
			public:
				GuiSolidBackgroundElement();
				~GuiSolidBackgroundElement();

				Color					GetColor();
				void					SetColor(Color value);
			};

			class GuiSolidLabelElement : public Object, public IGuiGraphicsElement
			{
				DEFINE_GUI_GRAPHICS_ELEMENT(GuiSolidLabelElement, L"SolidLabel");
			protected:
				Color					color;
				FontProperties			fontProperties;
				WString					text;
				Alignment::Type			hAlignment;
				Alignment::Type			vAlignment;
			public:
				GuiSolidLabelElement();
				~GuiSolidLabelElement();

				Color					GetColor();
				void					SetColor(Color value);

				const FontProperties&	GetFont();
				void					SetFont(const FontProperties& value);

				const WString&			GetText();
				void					SetText(const WString& value);

				Alignment::Type			GetHorizontalAlignment();
				Alignment::Type			GetVerticalAlignment();
				void					SetAlignments(Alignment::Type horizontal, Alignment::Type vertical);
			};
		}
	}
}

extern void GuiMain();

#endif