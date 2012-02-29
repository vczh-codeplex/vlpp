#include "GuiGraphicsElement.h"

namespace vl
{
	namespace presentation
	{
		namespace elements
		{
			using namespace collections;

/***********************************************************************
GuiGraphicsResourceManager
***********************************************************************/

			GuiGraphicsResourceManager::GuiGraphicsResourceManager()
			{
			}

			GuiGraphicsResourceManager::~GuiGraphicsResourceManager()
			{
			}

			bool GuiGraphicsResourceManager::RegisterElementFactory(IGuiGraphicsElementFactory* factory)
			{
				if(elementFactories.Keys().Contains(factory->GetElementTypeName()))
				{
					return false;
				}
				else
				{
					elementFactories.Add(factory->GetElementTypeName(), factory);
					return true;
				}
			}

			bool GuiGraphicsResourceManager::RegisterRendererFactory(const WString& elementTypeName, IGuiGraphicsRendererFactory* factory)
			{
				if(rendererFactories.Keys().Contains(elementTypeName))
				{
					return false;
				}
				else
				{
					rendererFactories.Add(elementTypeName, factory);
					return true;
				}
			}

			IGuiGraphicsElementFactory* GuiGraphicsResourceManager::GetElementFactory(const WString& elementTypeName)
			{
				int index=elementFactories.Keys().IndexOf(elementTypeName);
				return index==-1?0:elementFactories.Values()[index].Obj();
			}

			IGuiGraphicsRendererFactory* GuiGraphicsResourceManager::GetRendererFactory(const WString& elementTypeName)
			{
				int index=rendererFactories.Keys().IndexOf(elementTypeName);
				return index==-1?0:rendererFactories.Values()[index].Obj();
			}

			GuiGraphicsResourceManager* guiGraphicsResourceManager=0;

			GuiGraphicsResourceManager* GetGuiGraphicsResourceManager()
			{
				return guiGraphicsResourceManager;
			}

			void SetGuiGraphicsResourceManager(GuiGraphicsResourceManager* resourceManager)
			{
				guiGraphicsResourceManager=resourceManager;
			}

			bool RegisterFactories(IGuiGraphicsElementFactory* elementFactory, IGuiGraphicsRendererFactory* rendererFactory)
			{
				if(guiGraphicsResourceManager && elementFactory && rendererFactory)
				{
					if(guiGraphicsResourceManager->RegisterElementFactory(elementFactory))
					{
						if(guiGraphicsResourceManager->RegisterRendererFactory(elementFactory->GetElementTypeName(), rendererFactory))
						{
							return true;
						}
					}
				}
				return false;
			}

/***********************************************************************
GuiSolidBorderElement
***********************************************************************/

			GuiSolidBorderElement::GuiSolidBorderElement()
				:color(0, 0, 0)
				,shape(ElementShape::Rectangle)
			{
			}

			GuiSolidBorderElement::~GuiSolidBorderElement()
			{
				renderer->Finalize();
			}

			Color GuiSolidBorderElement::GetColor()
			{
				return color;
			}

			void GuiSolidBorderElement::SetColor(Color value)
			{
				if(color!=value)
				{
					color=value;
					renderer->OnElementStateChanged();
				}
			}
			
			ElementShape::Type GuiSolidBorderElement::GetShape()
			{
				return shape;
			}

			void GuiSolidBorderElement::SetShape(ElementShape::Type value)
			{
				shape=value;
			}

/***********************************************************************
GuiRoundBorderElement
***********************************************************************/

			GuiRoundBorderElement::GuiRoundBorderElement()
				:color(0, 0, 0)
				,radius(10)
			{
			}

			GuiRoundBorderElement::~GuiRoundBorderElement()
			{
				renderer->Finalize();
			}

			Color GuiRoundBorderElement::GetColor()
			{
				return color;
			}

			void GuiRoundBorderElement::SetColor(Color value)
			{
				if(color!=value)
				{
					color=value;
					renderer->OnElementStateChanged();
				}
			}

			int GuiRoundBorderElement::GetRadius()
			{
				return radius;
			}

			void GuiRoundBorderElement::SetRadius(int value)
			{
				if(radius!=value)
				{
					radius=value;
					renderer->OnElementStateChanged();
				}
			}

/***********************************************************************
Gui3DBorderElement
***********************************************************************/

			Gui3DBorderElement::Gui3DBorderElement()
			{
			}

			Gui3DBorderElement::~Gui3DBorderElement()
			{
				renderer->Finalize();
			}

			Color Gui3DBorderElement::GetColor1()
			{
				return color1;
			}

			Color Gui3DBorderElement::GetColor2()
			{
				return color2;
			}

			void Gui3DBorderElement::SetColors(Color value1, Color value2)
			{
				if(color1!=value1 || color2!=value2)
				{
					color1=value1;
					color2=value2;
					renderer->OnElementStateChanged();
				}
			}

/***********************************************************************
Gui3DSplitterElement
***********************************************************************/

			Gui3DSplitterElement::Gui3DSplitterElement()
				:direction(Horizontal)
			{
			}

			Gui3DSplitterElement::~Gui3DSplitterElement()
			{
				renderer->Finalize();
			}

			Color Gui3DSplitterElement::GetColor1()
			{
				return color1;
			}

			Color Gui3DSplitterElement::GetColor2()
			{
				return color2;
			}

			void Gui3DSplitterElement::SetColors(Color value1, Color value2)
			{
				if(color1!=value1 || color2!=value2)
				{
					color1=value1;
					color2=value2;
					renderer->OnElementStateChanged();
				}
			}

			Gui3DSplitterElement::Direction Gui3DSplitterElement::GetDirection()
			{
				return direction;
			}

			void Gui3DSplitterElement::SetDirection(Direction value)
			{
				if(direction!=value)
				{
					direction=value;
					renderer->OnElementStateChanged();
				}
			}

/***********************************************************************
GuiSolidBackgroundElement
***********************************************************************/

			GuiSolidBackgroundElement::GuiSolidBackgroundElement()
				:color(255, 255, 255)
				,shape(ElementShape::Rectangle)
			{
			}

			GuiSolidBackgroundElement::~GuiSolidBackgroundElement()
			{
				renderer->Finalize();
			}

			Color GuiSolidBackgroundElement::GetColor()
			{
				return color;
			}

			void GuiSolidBackgroundElement::SetColor(Color value)
			{
				if(color!=value)
				{
					color=value;
					renderer->OnElementStateChanged();
				}
			}
			
			ElementShape::Type GuiSolidBackgroundElement::GetShape()
			{
				return shape;
			}

			void GuiSolidBackgroundElement::SetShape(ElementShape::Type value)
			{
				shape=value;
			}

/***********************************************************************
GuiGradientBackgroundElement
***********************************************************************/

			GuiGradientBackgroundElement::GuiGradientBackgroundElement()
				:direction(Horizontal)
				,shape(ElementShape::Rectangle)
			{
			}

			GuiGradientBackgroundElement::~GuiGradientBackgroundElement()
			{
				renderer->Finalize();
			}

			Color GuiGradientBackgroundElement::GetColor1()
			{
				return color1;
			}

			Color GuiGradientBackgroundElement::GetColor2()
			{
				return color2;
			}

			void GuiGradientBackgroundElement::SetColors(Color value1, Color value2)
			{
				if(color1!=value1 || color2!=value2)
				{
					color1=value1;
					color2=value2;
					renderer->OnElementStateChanged();
				}
			}

			GuiGradientBackgroundElement::Direction GuiGradientBackgroundElement::GetDirection()
			{
				return direction;
			}

			void GuiGradientBackgroundElement::SetDirection(Direction value)
			{
				if(direction!=value)
				{
					direction=value;
					renderer->OnElementStateChanged();
				}
			}
			
			ElementShape::Type GuiGradientBackgroundElement::GetShape()
			{
				return shape;
			}

			void GuiGradientBackgroundElement::SetShape(ElementShape::Type value)
			{
				shape=value;
			}

/***********************************************************************
GuiSolidLabelElement
***********************************************************************/

			GuiSolidLabelElement::GuiSolidLabelElement()
				:color(0, 0, 0)
				,hAlignment(Alignment::Left)
				,vAlignment(Alignment::Top)
				,wrapLine(false)
				,ellipse(false)
				,multiline(false)
			{
				fontProperties.fontFamily=L"Lucida Console";
				fontProperties.size=12;
			}

			GuiSolidLabelElement::~GuiSolidLabelElement()
			{
				renderer->Finalize();
			}

			Color GuiSolidLabelElement::GetColor()
			{
				return color;
			}

			void GuiSolidLabelElement::SetColor(Color value)
			{
				if(color!=value)
				{
					color=value;
					renderer->OnElementStateChanged();
				}
			}

			const FontProperties& GuiSolidLabelElement::GetFont()
			{
				return fontProperties;
			}

			void GuiSolidLabelElement::SetFont(const FontProperties& value)
			{
				if(fontProperties!=value)
				{
					fontProperties=value;
					renderer->OnElementStateChanged();
				}
			}

			const WString& GuiSolidLabelElement::GetText()
			{
				return text;
			}

			void GuiSolidLabelElement::SetText(const WString& value)
			{
				if(text!=value)
				{
					text=value;
					renderer->OnElementStateChanged();
				}
			}

			Alignment::Type GuiSolidLabelElement::GetHorizontalAlignment()
			{
				return hAlignment;
			}

			Alignment::Type GuiSolidLabelElement::GetVerticalAlignment()
			{
				return vAlignment;
			}

			void GuiSolidLabelElement::SetAlignments(Alignment::Type horizontal, Alignment::Type vertical)
			{
				if(hAlignment!=horizontal || vAlignment!=vertical)
				{
					hAlignment=horizontal;
					vAlignment=vertical;
					renderer->OnElementStateChanged();
				}
			}

			bool GuiSolidLabelElement::GetWrapLine()
			{
				return wrapLine;
			}

			void GuiSolidLabelElement::SetWrapLine(bool value)
			{
				if(wrapLine!=value)
				{
					wrapLine=value;
					renderer->OnElementStateChanged();
				}
			}

			bool GuiSolidLabelElement::GetEllipse()
			{
				return ellipse;
			}

			void GuiSolidLabelElement::SetEllipse(bool value)
			{
				if(ellipse!=value)
				{
					ellipse=value;
					renderer->OnElementStateChanged();
				}
			}

			bool GuiSolidLabelElement::GetMultiline()
			{
				return multiline;
			}

			void GuiSolidLabelElement::SetMultiline(bool value)
			{
				if(multiline!=value)
				{
					multiline=value;
					renderer->OnElementStateChanged();
				}
			}

/***********************************************************************
GuiImageFrameElement
***********************************************************************/

			GuiImageFrameElement::GuiImageFrameElement()
				:frameIndex(0)
				,hAlignment(Alignment::Left)
				,vAlignment(Alignment::Top)
				,stretch(false)
			{
			}

			GuiImageFrameElement::~GuiImageFrameElement()
			{
				renderer->Finalize();
			}

			Ptr<INativeImage> GuiImageFrameElement::GetImage()
			{
				return image;
			}

			int GuiImageFrameElement::GetFrameIndex()
			{
				return frameIndex;
			}

			void GuiImageFrameElement::SetImage(Ptr<INativeImage> _image, int _frameIndex)
			{
				if(image!=_image || frameIndex!=_frameIndex)
				{
					if(0<=_frameIndex && _frameIndex<_image->GetFrameCount())
					{
						image=_image;
						frameIndex=_frameIndex;
						renderer->OnElementStateChanged();
					}
				}
			}

			Alignment::Type GuiImageFrameElement::GetHorizontalAlignment()
			{
				return hAlignment;
			}

			Alignment::Type GuiImageFrameElement::GetVerticalAlignment()
			{
				return vAlignment;
			}

			void GuiImageFrameElement::SetAlignments(Alignment::Type horizontal, Alignment::Type vertical)
			{
				if(hAlignment!=horizontal || vAlignment!=vertical)
				{
					hAlignment=horizontal;
					vAlignment=vertical;
					renderer->OnElementStateChanged();
				}
			}

			bool GuiImageFrameElement::GetStretch()
			{
				return stretch;
			}

			void GuiImageFrameElement::SetStretch(bool value)
			{
				if(stretch!=value)
				{
					stretch=value;
					renderer->OnElementStateChanged();
				}
			}

/***********************************************************************
GuiPolygonElement
***********************************************************************/

			GuiPolygonElement::GuiPolygonElement()
			{
			}

			GuiPolygonElement::~GuiPolygonElement()
			{
			}

			Size GuiPolygonElement::GetSize()
			{
				return size;
			}

			void GuiPolygonElement::SetSize(Size value)
			{
				if(size!=value)
				{
					size=value;
					renderer->OnElementStateChanged();
				}
			}

			const Point& GuiPolygonElement::GetPoint(int index)
			{
				return points[index];
			}

			int GuiPolygonElement::GetPointCount()
			{
				return points.Count();
			}

			void GuiPolygonElement::SetPoints(const Point* p, int count)
			{
				points.Resize(count);
				memcpy(&points[0], p, sizeof(*p)*count);
				renderer->OnElementStateChanged();
			}

			Color GuiPolygonElement::GetBorderColor()
			{
				return borderColor;
			}

			void GuiPolygonElement::SetBorderColor(Color value)
			{
				if(borderColor!=value)
				{
					borderColor=value;
					renderer->OnElementStateChanged();
				}
			}

			Color GuiPolygonElement::GetBackgroundColor()
			{
				return backgroundColor;
			}

			void GuiPolygonElement::SetBackgroundColor(Color value)
			{
				if(backgroundColor!=value)
				{
					backgroundColor=value;
					renderer->OnElementStateChanged();
				}
			}
		}
	}
}