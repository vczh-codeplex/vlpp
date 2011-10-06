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

			template<typename TElement>
			class GuiGraphicsElement : public Object, public IGuiGraphicsElement
			{
			public:
				class Factory : public Object, public IGuiGraphicsElementFactory
				{
				public:
					WString GetElementTypeName()
					{
						return TElement::GetElementTypeName();
					}

					IGuiGraphicsElement* Create()
					{
						GuiGraphicsElement<TElement>* element=new TElement;
						element->factory=this;
						element->renderer=GetGuiGraphicsResourceManager()->GetRendererFactory(GetElementTypeName())->Create();
						element->renderer->Initialize(element);
						return element;
					}
				};
			protected:
				IGuiGraphicsElementFactory*		factory;
				Ptr<IGuiGraphicsRenderer>		renderer;
			public:
				GuiGraphicsElement()
					:factory(0)
				{
				}

				static TElement* Create()
				{
					return dynamic_cast<TElement*>(GetGuiGraphicsResourceManager()->GetElementFactory(TElement::GetElementTypeName())->Create());
				}

				IGuiGraphicsElementFactory* GetFactory()
				{
					return factory;
				}

				IGuiGraphicsRenderer* GetRenderer()
				{
					return renderer.Obj();
				}
			};

			template<typename TElement, typename TRenderer, typename TTarget>
			class GuiGraphicsRenderer : public Object, public IGuiGraphicsRenderer
			{
			public:
				class Factory : public Object, public IGuiGraphicsRendererFactory
				{
				public:
					IGuiGraphicsRenderer* Create()
					{
						GuiGraphicsRenderer<TElement, TRenderer, TTarget>* renderer=new TRenderer;
						renderer->factory=this;
						return renderer;
					}
				};
			protected:
				IGuiGraphicsRendererFactory*	factory;
				TElement*						element;
				TTarget*						renderTarget;
			public:
				GuiGraphicsRenderer()
					:factory(0)
					,element(0)
					,renderTarget(0)
				{
				}

				static void Register()
				{
					RegisterFactories(new TElement::Factory, new TRenderer::Factory);
				}

				IGuiGraphicsRendererFactory* GetFactory()
				{
					return factory;
				}

				void Initialize(IGuiGraphicsElement* _element)
				{
					element=dynamic_cast<TElement*>(_element);
				}

				void SetRenderTarget(IGuiGraphicsRenderTarget* _renderTarget)
				{
					renderTarget=dynamic_cast<TTarget*>(_renderTarget);
				}
			};

/***********************************************************************
Elements
***********************************************************************/

			class GuiSolidBorderElement : public GuiGraphicsElement<GuiSolidBorderElement>
			{
			protected:
				Color				color;
			public:
				GuiSolidBorderElement();
				~GuiSolidBorderElement();

				static WString		GetElementTypeName();

				Color				GetColor();
				void				SetColor(Color value);
			};

			class GuiSolidBackgroundElement : public GuiGraphicsElement<GuiSolidBackgroundElement>
			{
			protected:
				Color				color;
			public:
				GuiSolidBackgroundElement();
				~GuiSolidBackgroundElement();

				static WString		GetElementTypeName();

				Color				GetColor();
				void				SetColor(Color value);
			};
		}
	}
}

extern void GuiMain();

#endif