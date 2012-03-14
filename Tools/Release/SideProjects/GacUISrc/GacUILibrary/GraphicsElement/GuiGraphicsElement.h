/***********************************************************************
Vczh Library++ 3.0
Developer: ³Âè÷å«(vczh)
GacUI::Element System and Infrastructure Interfaces

Interfaces:
***********************************************************************/

#ifndef VCZH_PRESENTATION_ELEMENTS_GUIGRAPHICSELEMENT
#define VCZH_PRESENTATION_ELEMENTS_GUIGRAPHICSELEMENT

#include "..\NativeWindow\GuiNativeWindow.h"
#include "..\Reflection\GuiTypeDescriptor.h"

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

			/// <summary>
			/// This is the interface for graphics elements.
			/// Graphics elements usually contains some information and helper functions for visible things.
			/// An graphics elements should be created using ElementType::Create.
			/// </summary>
			class IGuiGraphicsElement : public virtual IDescriptable, public Description<IGuiGraphicsElement>
			{
			public:
				/// <summary>
				/// Access the <see cref="IGuiGraphicsElementFactory"></see> that is used to create this graphics elements.
				/// </summary>
				/// <returns>Returns the related factory.</returns>
				virtual IGuiGraphicsElementFactory*		GetFactory()=0;
				/// <summary>
				/// Access the associated <see cref="IGuiGraphicsRenderer"></see> for this graphics element.
				/// </summary>
				/// <returns>Returns the related renderer.</returns>
				virtual IGuiGraphicsRenderer*			GetRenderer()=0;
			};

			/// <summary>
			/// This is the interface for graphics element factories.
			/// Graphics element factories should be registered using [M:vl.presentation.elements.GuiGraphicsResourceManager.RegisterElementFactory].
			/// </summary>
			class IGuiGraphicsElementFactory : public Interface
			{
			public:
				/// <summary>
				/// Get the name representing the kind of graphics element to be created.
				/// </summary>
				/// <returns>Returns the name of graphics elements.</returns>
				virtual WString							GetElementTypeName()=0;
				/// <summary>
				/// Create a <see cref="IGuiGraphicsElement"></see>.
				/// </summary>
				/// <returns>Returns the created graphics elements.</returns>
				virtual IGuiGraphicsElement*			Create()=0;
			};

			/// <summary>
			/// This is the interface for graphics renderers.
			/// </summary>
			class IGuiGraphicsRenderer : public Interface
			{
			public:
				/// <summary>
				/// Access the graphics <see cref="IGuiGraphicsRendererFactory"></see> that is used to create this graphics renderer.
				/// </summary>
				/// <returns>Returns the related factory.</returns>
				virtual IGuiGraphicsRendererFactory*	GetFactory()=0;

				/// <summary>
				/// Initialize the grpahics renderer by binding a <see cref="IGuiGraphicsElement"></see> to it.
				/// </summary>
				/// <param name="element">The graphics element to bind.</param>
				virtual void							Initialize(IGuiGraphicsElement* element)=0;
				/// <summary>
				/// Release all resources that used by this renderer.
				/// </summary>
				virtual void							Finalize()=0;
				/// <summary>
				/// Set a <see cref="IGuiGraphicsRenderTarget"></see> to this element.
				/// </summary>
				/// <param name="renderTarget">The graphics render target. It can be NULL.</param>
				virtual void							SetRenderTarget(IGuiGraphicsRenderTarget* renderTarget)=0;
				/// <summary>
				/// Render the graphics element using a specified bounds.
				/// </summary>
				/// <param name="bounds">Bounds to decide the size and position of the binded graphics element.</param>
				virtual void							Render(Rect bounds)=0;
				/// <summary>
				/// Notify that the state in the binded graphics element is changed. This function is usually called by the element itself.
				/// </summary>
				virtual void							OnElementStateChanged()=0;
				/// <summary>
				/// Calculate the minimum size using the binded graphics element and its state.
				/// </summary>
				/// <returns>The minimum size.</returns>
				virtual Size							GetMinSize()=0;
			};

			/// <summary>
			/// This is the interface for graphics renderer factories.
			/// Graphics renderers should be registered using [M:vl.presentation.elements.GuiGraphicsResourceManager.RegisterRendererFactory]. 
			/// </summary>
			class IGuiGraphicsRendererFactory : public Interface
			{
			public:
				/// <summary>
				/// Create a <see cref="IGuiGraphicsRenderer"></see>.
				/// </summary>
				/// <returns>Returns the created graphics renderer.</returns>
				virtual IGuiGraphicsRenderer*			Create()=0;
			};

			/// <summary>
			/// This is the interface for graphics renderer targets.
			/// </summary>
			class IGuiGraphicsRenderTarget : public Interface
			{
			public:
				/// <summary>
				/// Notify the target to prepare for rendering.
				/// </summary>
				virtual void							StartRendering()=0;
				/// <summary>
				/// Notify the target to stop rendering.
				/// </summary>
				virtual void							StopRendering()=0;
				/// <summary>
				/// Apply a clipper to the render target.
				/// The result clipper is combined by all clippers in the clipper stack maintained by the render target.
				/// </summary>
				virtual void							PushClipper(Rect clipper)=0;
				/// <summary>
				/// Remove the last pushed clipper from the clipper stack.
				/// </summary>
				virtual void							PopClipper()=0;
				/// <summary>
				/// Get the combined clipper
				/// </summary>
				/// <returns>The combined clipper</returns>
				virtual Rect							GetClipper()=0;
				/// <summary>
				/// Test is the combined clipper is as large as the render target.
				/// </summary>
				/// <returns>Return true if the combined clipper is as large as the render target.</returns>
				virtual bool							IsClipperCoverWholeTarget()=0;
			};

/***********************************************************************
Resource Manager
***********************************************************************/

			/// <summary>
			/// This is a class for managing grpahics element factories and graphics renderer factories
			/// </summary>
			class GuiGraphicsResourceManager : public Object
			{
				typedef collections::Dictionary<WString, Ptr<IGuiGraphicsElementFactory>>		elementFactoryMap;
				typedef collections::Dictionary<WString, Ptr<IGuiGraphicsRendererFactory>>		rendererFactoryMap;
			protected:
				elementFactoryMap						elementFactories;
				rendererFactoryMap						rendererFactories;
			public:
				/// <summary>
				/// Create a graphics resource manager without any predefined factories
				/// </summary>
				GuiGraphicsResourceManager();
				~GuiGraphicsResourceManager();

				/// <summary>
				/// Register a <see cref="IGuiGraphicsElementFactory"></see> using the element type from <see cref="IGuiGraphicsElementFactory::GetElementTypeName"></see>.
				/// </summary>
				/// <param name="factory">The instance of the graphics element factory to register.</param>
				/// <returns>Returns true if this operations succeeded.</returns>
				virtual bool							RegisterElementFactory(IGuiGraphicsElementFactory* factory);
				/// <summary>
				/// Register a <see cref="IGuiGraphicsRendererFactory"></see> and bind it to a registered <see cref="IGuiGraphicsElementFactory"></see>.
				/// </summary>
				/// <param name="elementTypeName">The element type to represent a graphics element factory.</param>
				/// <param name="factory">The instance of the graphics renderer factory to register.</param>
				/// <returns>Returns true if this operations succeeded.</returns>
				virtual bool							RegisterRendererFactory(const WString& elementTypeName, IGuiGraphicsRendererFactory* factory);
				/// <summary>
				/// Get the instance of a registered <see cref="IGuiGraphicsElementFactory"></see> that is binded to a specified element type.
				/// </summary>
				/// <param name="elementTypeName">The element type to get a corresponding graphics element factory.</param>
				virtual IGuiGraphicsElementFactory*		GetElementFactory(const WString& elementTypeName);
				/// <summary>
				/// Get the instance of a registered <see cref="IGuiGraphicsRendererFactory"></see> that is binded to a specified element type.
				/// </summary>
				/// <param name="elementTypeName">The element type to get a corresponding graphics renderer factory.</param>
				virtual IGuiGraphicsRendererFactory*	GetRendererFactory(const WString& elementTypeName);
				/// <summary>
				/// Get the instance of a <see cref="IGuiGraphicsRenderTarget"></see> that is binded to an <see cref="INativeWindow"></see>.
				/// </summary>
				virtual IGuiGraphicsRenderTarget*		GetRenderTarget(INativeWindow* window)=0;
			};

			/// <summary>
			/// Get the current <see cref="GuiGraphicsResourceManager"></see>.
			/// </summary>
			/// <returns>Returns the current resource manager.</returns>
			extern GuiGraphicsResourceManager*			GetGuiGraphicsResourceManager();
			/// <summary>
			/// Set the current <see cref="GuiGraphicsResourceManager"></see>.
			/// </summary>
			/// <params name="resourceManager">The resource manager to set.</params>
			extern void									SetGuiGraphicsResourceManager(GuiGraphicsResourceManager* resourceManager);
			/// <summary>
			/// Helper function to register a <see cref="IGuiGraphicsElementFactory"></see> with a <see cref="IGuiGraphicsRendererFactory"></see> and bind them together.
			/// </summary>
			/// <returns>Returns true if this operations succeeded.</returns>
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
				IGuiGraphicsElementFactory* GetFactory()override\
				{\
					return factory;\
				}\
				IGuiGraphicsRenderer* GetRenderer()override\
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
				IGuiGraphicsRendererFactory* GetFactory()override\
				{\
					return factory;\
				}\
				void Initialize(IGuiGraphicsElement* _element)override\
				{\
					element=dynamic_cast<TELEMENT*>(_element);\
					InitializeInternal();\
				}\
				void Finalize()override\
				{\
					FinalizeInternal();\
				}\
				void SetRenderTarget(IGuiGraphicsRenderTarget* _renderTarget)override\
				{\
					TTARGET* oldRenderTarget=renderTarget;\
					renderTarget=dynamic_cast<TTARGET*>(_renderTarget);\
					RenderTargetChangedInternal(oldRenderTarget, renderTarget);\
				}\
				Size GetMinSize()override\
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
				/// <summary>
				/// Defines an shape for some <see cref="IGuiGraphicsElement"></see>.
				/// </summary>
				enum Type
				{
					/// <summary>[T:vl.presentation.elements.ElementShape.Type]Rectangle shape.</summary>
					Rectangle,
					/// <summary>[T:vl.presentation.elements.ElementShape.Type]Ellipse shape.</summary>
					Ellipse,
				};
			};

			/// <summary>
			/// Defines a border element with a thickness of one pixel.
			/// </summary>
			class GuiSolidBorderElement : public Object, public IGuiGraphicsElement, public Description<GuiSolidBorderElement>
			{
				DEFINE_GUI_GRAPHICS_ELEMENT(GuiSolidBorderElement, L"SolidBorder")
			protected:
				Color					color;
				ElementShape::Type		shape;

				GuiSolidBorderElement();
			public:
				~GuiSolidBorderElement();

				/// <summary>
				/// Get the border color.
				/// </summary>
				/// <returns>The border color.</returns>
				Color					GetColor();
				/// <summary>
				/// Set the border color.
				/// </summary>
				/// <param name="value">The new border color.</param>
				void					SetColor(Color value);
				/// <summary>
				/// Get the shape.
				/// </summary>
				/// <returns>The shape.</returns>
				ElementShape::Type		GetShape();
				/// <summary>
				/// Set the shape.
				/// </summary>
				/// <param name="value">The new shape.</param>
				void					SetShape(ElementShape::Type value);
			};
			
			/// <summary>
			/// Defines a rectangle border element with round corners and a thickness of one pixel.
			/// </summary>
			class GuiRoundBorderElement : public Object, public IGuiGraphicsElement, public Description<GuiRoundBorderElement>
			{
				DEFINE_GUI_GRAPHICS_ELEMENT(GuiRoundBorderElement, L"RoundBorder")
			protected:
				Color					color;
				int						radius;

				GuiRoundBorderElement();
			public:
				~GuiRoundBorderElement();
				
				/// <summary>
				/// Get the border color.
				/// </summary>
				/// <returns>The border color.</returns>
				Color					GetColor();
				/// <summary>
				/// Set the border color.
				/// </summary>
				/// <param name="value">The new border color.</param>
				void					SetColor(Color value);
				
				/// <summary>
				/// Get the radius of round corners.
				/// </summary>
				/// <returns>The radius.</returns>
				int						GetRadius();
				/// <summary>
				/// Set the radius of round corners.
				/// </summary>
				/// <param name="value">The new radius.</param>
				void					SetRadius(int value);
			};
			
			/// <summary>
			/// Defines a 3D-like rectangle element with a thickness of two pixels.
			/// </summary>
			class Gui3DBorderElement : public Object, public IGuiGraphicsElement, public Description<Gui3DBorderElement>
			{
				DEFINE_GUI_GRAPHICS_ELEMENT(Gui3DBorderElement, L"3DBorder")
			protected:
				Color					color1;
				Color					color2;

				Gui3DBorderElement();
			public:
				~Gui3DBorderElement();
				
				/// <summary>
				/// Get the left-top color.
				/// </summary>
				/// <returns>The left-top color.</returns>
				Color					GetColor1();
				/// <summary>
				/// Get the right-bottom color.
				/// </summary>
				/// <returns>The right-bottom color.</returns>
				Color					GetColor2();
				/// <summary>
				/// Set colors of the element.
				/// </summary>
				/// <param name="value1">The new left-top color.</param>
				/// <param name="value2">The new right bottom color.</param>
				void					SetColors(Color value1, Color value2);
			};
			
			/// <summary>
			/// Defines a 3D-like splitter element with a thickness of two pixels.
			/// </summary>
			class Gui3DSplitterElement : public Object, public IGuiGraphicsElement, public Description<Gui3DSplitterElement>
			{
				DEFINE_GUI_GRAPHICS_ELEMENT(Gui3DSplitterElement, L"3DSplitter")
			public:
				/// <summary>
				/// Defines a direction of the <see cref="Gui3DSplitterElement"></see>.
				/// </summary>
				enum Direction
				{
					/// <summary>[T:vl.presentation.elements.Gui3DSplitterElement.Direction]Horizontal direction.</summary>
					Horizontal,
					/// <summary>[T:vl.presentation.elements.Gui3DSplitterElement.Direction]Vertical direction.</summary>
					Vertical,
				};
			protected:
				Color					color1;
				Color					color2;
				Direction				direction;

				Gui3DSplitterElement();
			public:
				~Gui3DSplitterElement();
				
				/// <summary>
				/// Get the left-top color.
				/// </summary>
				/// <returns>The left-top color.</returns>
				Color					GetColor1();
				/// <summary>
				/// Get the right-bottom color.
				/// </summary>
				/// <returns>The right-bottom color.</returns>
				Color					GetColor2();
				/// <summary>
				/// Set colors of the element.
				/// </summary>
				/// <param name="value1">The new left-top color.</param>
				/// <param name="value2">The new right bottom color.</param>
				void					SetColors(Color value1, Color value2);
				
				/// <summary>
				/// Get the direction.
				/// </summary>
				/// <returns>The direction.</returns>
				Direction				GetDirection();
				/// <summary>
				/// Set the direction.
				/// </summary>
				/// <param name="value">The new direction.</param>
				void					SetDirection(Direction value);
			};

			/// <summary>
			/// Defines a color-filled element without border.
			/// </summary>
			class GuiSolidBackgroundElement : public Object, public IGuiGraphicsElement, public Description<GuiSolidBackgroundElement>
			{
				DEFINE_GUI_GRAPHICS_ELEMENT(GuiSolidBackgroundElement, L"SolidBackground")
			protected:
				Color					color;
				ElementShape::Type		shape;

				GuiSolidBackgroundElement();
			public:
				~GuiSolidBackgroundElement();
				
				/// <summary>
				/// Get the border color.
				/// </summary>
				/// <returns>The border color.</returns>
				Color					GetColor();
				/// <summary>
				/// Set the border color.
				/// </summary>
				/// <param name="value">The new border color.</param>
				void					SetColor(Color value);
				/// <summary>
				/// Get the shape.
				/// </summary>
				/// <returns>The shape.</returns>
				ElementShape::Type		GetShape();
				/// <summary>
				/// Set the shape.
				/// </summary>
				/// <param name="value">The new shape.</param>
				void					SetShape(ElementShape::Type value);
			};
			
			/// <summary>
			/// Defines a color-filled gradient element without border.
			/// </summary>
			class GuiGradientBackgroundElement : public Object, public IGuiGraphicsElement, public Description<GuiGradientBackgroundElement>
			{
				DEFINE_GUI_GRAPHICS_ELEMENT(GuiGradientBackgroundElement, L"GradientBackground")
			public:
				/// <summary>
				/// Defines a direction of the <see cref="Gui3DSplitterElement"></see>.
				/// </summary>
				enum Direction
				{
					/// <summary>[T:vl.presentation.elements.GuiGradientBackgroundElement.Direction]Horizontal direction.</summary>
					Horizontal,
					/// <summary>[T:vl.presentation.elements.GuiGradientBackgroundElement.Direction]vertical direction.</summary>
					Vertical,
					/// <summary>[T:vl.presentation.elements.GuiGradientBackgroundElement.Direction]Slash direction.</summary>
					Slash,
					/// <summary>[T:vl.presentation.elements.GuiGradientBackgroundElement.Direction]Back slash direction.</summary>
					Backslash,
				};
			protected:
				Color					color1, color2;
				Direction				direction;
				ElementShape::Type		shape;

				GuiGradientBackgroundElement();
			public:
				~GuiGradientBackgroundElement();
				
				/// <summary>
				/// Get the left-top color.
				/// </summary>
				/// <returns>The left-top color.</returns>
				Color					GetColor1();
				/// <summary>
				/// Get the right bottom color.
				/// </summary>
				/// <returns>The right-bottom color.</returns>
				Color					GetColor2();
				/// <summary>
				/// Set colors of the element.
				/// </summary>
				/// <param name="value1">The new left-top color.</param>
				/// <param name="value2">The new right bottom color.</param>
				void					SetColors(Color value1, Color value2);
				
				/// <summary>
				/// Get the direction.
				/// </summary>
				/// <returns>The direction.</returns>
				Direction				GetDirection();
				/// <summary>
				/// Set the direction.
				/// </summary>
				/// <param name="value">The new direction.</param>
				void					SetDirection(Direction value);
				/// <summary>
				/// Get the shape.
				/// </summary>
				/// <returns>The shape.</returns>
				ElementShape::Type		GetShape();
				/// <summary>
				/// Set the shape.
				/// </summary>
				/// <param name="value">The new shape.</param>
				void					SetShape(ElementShape::Type value);
			};
			
			/// <summary>
			/// Defines an element of a plain text.
			/// </summary>
			class GuiSolidLabelElement : public Object, public IGuiGraphicsElement, public Description<GuiSolidLabelElement>
			{
				DEFINE_GUI_GRAPHICS_ELEMENT(GuiSolidLabelElement, L"SolidLabel");
			protected:
				Color					color;
				FontProperties			fontProperties;
				WString					text;
				Alignment::Type			hAlignment;
				Alignment::Type			vAlignment;
				bool					wrapLine;
				bool					ellipse;
				bool					multiline;

				GuiSolidLabelElement();
			public:
				~GuiSolidLabelElement();
				
				/// <summary>
				/// Get the text color.
				/// </summary>
				/// <returns>The text color.</returns>
				Color					GetColor();
				/// <summary>
				/// Set the text color.
				/// </summary>
				/// <param name="value">The new text color.</param>
				void					SetColor(Color value);
				
				/// <summary>
				/// Get the text font.
				/// </summary>
				/// <returns>The text font.</returns>
				const FontProperties&	GetFont();
				/// <summary>
				/// Set the text font.
				/// </summary>
				/// <param name="value">The new text font.</param>
				void					SetFont(const FontProperties& value);
				
				/// <summary>
				/// Get the text.
				/// </summary>
				/// <returns>The text.</returns>
				const WString&			GetText();
				/// <summary>
				/// Set the text.
				/// </summary>
				/// <param name="value">The new text.</param>
				void					SetText(const WString& value);
				
				/// <summary>
				/// Get the horizontal alignment of the text.
				/// </summary>
				/// <returns>The horizontal alignment of the text.</returns>
				Alignment::Type			GetHorizontalAlignment();
				/// <summary>
				/// Get the vertical alignment of the text.
				/// </summary>
				/// <returns>The vertical alignment of the text.</returns>
				Alignment::Type			GetVerticalAlignment();
				/// <summary>
				/// Set alignments in both directions of the text.
				/// </summary>
				/// <param name="horizontal">The new horizontal alignment of the text.</param>
				/// <param name="vertical">The vertical alignment of the text.</param>
				void					SetAlignments(Alignment::Type horizontal, Alignment::Type vertical);
				
				/// <summary>
				/// Get if line auto-wrapping is enabled for this text.
				/// </summary>
				/// <returns>Return true if line auto-wrapping is enabled for this text.</returns>
				bool					GetWrapLine();
				/// <summary>
				/// Set if line auto-wrapping is enabled for this text.
				/// </summary>
				/// <param name="value">True if line auto-wrapping is enabled for this text.</param>
				void					SetWrapLine(bool value);
				
				/// <summary>
				/// Get if ellipse is enabled for this text. Ellipse will appear when the text is clipped.
				/// </summary>
				/// <returns>Return true if ellipse is enabled for this text.</returns>
				bool					GetEllipse();
				/// <summary>
				/// Set if ellipse is enabled for this text. Ellipse will appear when the text is clipped.
				/// </summary>
				/// <param name="value">True if ellipse is enabled for this text.</param>
				void					SetEllipse(bool value);
				
				/// <summary>
				/// Get if multiple lines is enabled for this text.
				/// </summary>
				/// <returns>Return true if multiple lines is enabled for this text.</returns>
				bool					GetMultiline();
				/// <summary>
				/// Set if multiple lines is enabled for this text.
				/// </summary>
				/// <param name="value">True if multiple lines is enabled for this text.</param>
				void					SetMultiline(bool value);
			};

			/// <summary>
			/// Defines an element containing an image.
			/// </summary>
			class GuiImageFrameElement : public Object, public IGuiGraphicsElement, public Description<GuiImageFrameElement>
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

				/// <summary>
				/// Get the containing image.
				/// </summary>
				/// <returns>The contining picture.</returns>
				Ptr<INativeImage>		GetImage();
				/// <summary>
				/// Get the index of the frame in the containing image.
				/// </summary>
				/// <returns>The index of the frame in the containing image</returns>
				int						GetFrameIndex();
				/// <summary>
				/// Set the containing image and the frame index.
				/// </summary>
				/// <param name="_image">The new containing image.</param>
				/// <param name="_frameIndex">The new frameIndex.</param>
				void					SetImage(Ptr<INativeImage> _image, int _frameIndex=0);
				
				/// <summary>
				/// Get the horizontal alignment of the image.
				/// </summary>
				/// <returns>The horizontal alignment of the image.</returns>
				Alignment::Type			GetHorizontalAlignment();
				/// <summary>
				/// Get the vertical alignment of the image.
				/// </summary>
				/// <returns>The vertical alignment of the image.</returns>
				Alignment::Type			GetVerticalAlignment();
				/// <summary>
				/// Set alignments in both directions of the image.
				/// </summary>
				/// <param name="horizontal">The new horizontal alignment of the image.</param>
				/// <param name="vertical">The vertical alignment of the image.</param>
				void					SetAlignments(Alignment::Type horizontal, Alignment::Type vertical);

				/// <summary>
				/// Get if stretching is enabled for this image.
				/// </summary>
				/// <returns>Return true if stretching is enabled for this image.</returns>
				bool					GetStretch();
				/// <summary>
				/// Set if stretching is enabled for this image.
				/// </summary>
				/// <param name="value">True if stretching is enabled for this image.</param>
				void					SetStretch(bool value);
			};

			/// <summary>
			/// Defines a polygon element with a thickness of one pixel.
			/// </summary>
			class GuiPolygonElement : public Object, public IGuiGraphicsElement, public Description<GuiPolygonElement>
			{
				DEFINE_GUI_GRAPHICS_ELEMENT(GuiPolygonElement, L"Polygon");
			protected:
				Size							size;
				collections::Array<Point>		points;
				Color							borderColor;
				Color							backgroundColor;

				GuiPolygonElement();
			public:
				~GuiPolygonElement();

				/// <summary>
				/// Get a suggested size. The polygon element will be layouted to the center of the required bounds using this size.
				/// </summary>
				/// <returns>The suggested size.</returns>
				Size					GetSize();
				/// <summary>
				/// Set a suggested size. The polygon element will be layouted to the center of the required bounds using this size.
				/// </summary>
				/// <param name="value">The new size.</param>
				void					SetSize(Size value);

				/// <summary>
				/// Get a point of the polygon element using an index.
				/// </summary>
				/// <param name="index">The index to access a point.</param>
				/// <returns>The point of the polygon element associated with the index.</returns>
				const Point&			GetPoint(int index);
				/// <summary>
				/// Get the number of points
				/// </summary>
				/// <returns>The number of points.</returns>
				int						GetPointCount();
				/// <summary>
				/// Set all points to the polygon element.
				/// </summary>
				/// <param name="p">A pointer to a buffer that stores all points.</param>
				/// <param name="index">The number of points.</param>
				void					SetPoints(const Point* p, int count);
				
				/// <summary>
				/// Get the border color.
				/// </summary>
				/// <returns>The border color.</returns>
				Color					GetBorderColor();
				/// <summary>
				/// Set the border color.
				/// </summary>
				/// <param name="value">The new border color.</param>
				void					SetBorderColor(Color value);
				/// <summary>
				/// Get the background color.
				/// </summary>
				/// <returns>The background color.</returns>
				Color					GetBackgroundColor();
				/// <summary>
				/// Set the background color.
				/// </summary>
				/// <param name="value">The new background color.</param>
				void					SetBackgroundColor(Color value);
			};
		}
	}
}

#endif