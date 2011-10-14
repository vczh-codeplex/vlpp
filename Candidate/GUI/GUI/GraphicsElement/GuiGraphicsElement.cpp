#include "GuiGraphicsElement.h"

namespace vl
{
	namespace presentation
	{
		namespace elements
		{

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
				hAlignment=horizontal;
				vAlignment=vertical;
				renderer->OnElementStateChanged();
			}
		}
	}
}