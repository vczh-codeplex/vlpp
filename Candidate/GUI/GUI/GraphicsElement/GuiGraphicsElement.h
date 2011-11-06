/***********************************************************************
Vczh Library++ 3.0
Developer: ³Âè÷å«(vczh)
GacUI::Element System and Infrastructure Interfaces

Interfaces:
***********************************************************************/

#ifndef VCZH_PRESENTATION_ELEMENTS_GUIGRAPHICSELEMENT
#define VCZH_PRESENTATION_ELEMENTS_GUIGRAPHICSELEMENT

#include "..\NativeWindow\GuiNativeWindow.h"

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
				virtual void							Finalize()=0;
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
				void Finalize()\
				{\
					FinalizeInternal();\
				}\
				void SetRenderTarget(IGuiGraphicsRenderTarget* _renderTarget)\
				{\
					TTARGET* oldRenderTarget=renderTarget;\
					renderTarget=dynamic_cast<TTARGET*>(_renderTarget);\
					RenderTargetChangedInternal(oldRenderTarget, renderTarget);\
				}\
				Size GetMinSize()\
				{\
					return minSize;\
				}\

#define DEFINE_CACHED_RESOURCE_ALLOCATOR(TKEY, TVALUE)\
			public:\
				static const int DeadPackageMax=32;\
				struct Package\
				{\
					TVALUE							resource;\
					int								counter;\
					bool operator==(const Package& package)const{return false;}\
					bool operator!=(const Package& package)const{return true;}\
				};\
				struct DeadPackage\
				{\
					TKEY							key;\
					TVALUE							value;\
					bool operator==(const DeadPackage& package)const{return false;}\
					bool operator!=(const DeadPackage& package)const{return true;}\
				};\
				Dictionary<TKEY, Package>			aliveResources;\
				List<DeadPackage>					deadResources;\
			public:\
				TVALUE Create(const TKEY& key)\
				{\
					int index=aliveResources.Keys().IndexOf(key);\
					if(index!=-1)\
					{\
						Package package=aliveResources.Values()[index];\
						package.counter++;\
						aliveResources.Set(key, package);\
						return package.resource;\
					}\
					TVALUE resource;\
					for(int i=0;i<deadResources.Count();i++)\
					{\
						if(deadResources[i].key==key)\
						{\
							DeadPackage deadPackage=deadResources[i];\
							deadResources.RemoveAt(i);\
							resource=deadPackage.value;\
							break;\
						}\
					}\
					if(!resource)\
					{\
						resource=CreateInternal(key);\
					}\
					Package package;\
					package.resource=resource;\
					package.counter=1;\
					aliveResources.Add(key, package);\
					return package.resource;\
				}\
				void Destroy(const TKEY& key)\
				{\
					int index=aliveResources.Keys().IndexOf(key);\
					if(index!=-1)\
					{\
						Package package=aliveResources.Values()[index];\
						package.counter--;\
						if(package.counter==0)\
						{\
							aliveResources.Remove(key);\
							if(deadResources.Count()==DeadPackageMax)\
							{\
								deadResources.RemoveAt(DeadPackageMax-1);\
							}\
							DeadPackage deadPackage;\
							deadPackage.key=key;\
							deadPackage.value=package.resource;\
							deadResources.Insert(0, deadPackage);\
						}\
						else\
						{\
							aliveResources.Set(key, package);\
						}\
					}\
				}\

/***********************************************************************
Elements
***********************************************************************/

			namespace ElementShape
			{
				enum Type
				{
					Rectangle,
					Ellipse,
				};
			};

			class GuiSolidBorderElement : public Object, public IGuiGraphicsElement
			{
				DEFINE_GUI_GRAPHICS_ELEMENT(GuiSolidBorderElement, L"SolidBorder")
			protected:
				Color					color;
				ElementShape::Type		shape;

				GuiSolidBorderElement();
			public:
				~GuiSolidBorderElement();

				Color					GetColor();
				void					SetColor(Color value);
				ElementShape::Type		GetShape();
				void					SetShape(ElementShape::Type value);
			};

			class GuiRoundBorderElement : public Object, public IGuiGraphicsElement
			{
				DEFINE_GUI_GRAPHICS_ELEMENT(GuiRoundBorderElement, L"RoundBorder")
			protected:
				Color					color;
				int						radius;

				GuiRoundBorderElement();
			public:
				~GuiRoundBorderElement();

				Color					GetColor();
				void					SetColor(Color value);

				int						GetRadius();
				void					SetRadius(int value);
			};

			class Gui3DBorderElement : public Object, public IGuiGraphicsElement
			{
				DEFINE_GUI_GRAPHICS_ELEMENT(Gui3DBorderElement, L"3DBorder")
			protected:
				Color					color1;
				Color					color2;

				Gui3DBorderElement();
			public:
				~Gui3DBorderElement();

				Color					GetColor1();
				Color					GetColor2();
				void					SetColors(Color value1, Color value2);
			};

			class GuiSolidBackgroundElement : public Object, public IGuiGraphicsElement
			{
				DEFINE_GUI_GRAPHICS_ELEMENT(GuiSolidBackgroundElement, L"SolidBackground")
			protected:
				Color					color;
				ElementShape::Type		shape;

				GuiSolidBackgroundElement();
			public:
				~GuiSolidBackgroundElement();

				Color					GetColor();
				void					SetColor(Color value);
				ElementShape::Type		GetShape();
				void					SetShape(ElementShape::Type value);
			};

			class GuiGradientBackgroundElement : public Object, public IGuiGraphicsElement
			{
				DEFINE_GUI_GRAPHICS_ELEMENT(GuiGradientBackgroundElement, L"GradientBackground")
			public:
				enum Direction
				{
					Horizontal,
					Vertical,
					Slash,
					Backslash,
				};
			protected:
				Color					color1, color2;
				Direction				direction;
				ElementShape::Type		shape;

				GuiGradientBackgroundElement();
			public:
				~GuiGradientBackgroundElement();

				Color					GetColor1();
				Color					GetColor2();
				void					SetColors(Color value1, Color value2);

				Direction				GetDirection();
				void					SetDirection(Direction value);
				ElementShape::Type		GetShape();
				void					SetShape(ElementShape::Type value);
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

				GuiSolidLabelElement();
			public:
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

			class GuiImageFrameElement : public Object, public IGuiGraphicsElement
			{
				DEFINE_GUI_GRAPHICS_ELEMENT(GuiImageFrameElement, L"ImageFrame");
			protected:
				Ptr<INativeImage>		image;
				int						frameIndex;
				Alignment::Type			hAlignment;
				Alignment::Type			vAlignment;
				bool					stretch;

				GuiImageFrameElement();
			public:
				~GuiImageFrameElement();

				Ptr<INativeImage>		GetImage();
				int						GetFrameIndex();
				void					SetImage(Ptr<INativeImage> _image, int _frameIndex=0);

				Alignment::Type			GetHorizontalAlignment();
				Alignment::Type			GetVerticalAlignment();
				void					SetAlignments(Alignment::Type horizontal, Alignment::Type vertical);

				bool					GetStretch();
				void					SetStretch(bool value);
			};
		}
	}
}

extern void GuiMain();

#endif