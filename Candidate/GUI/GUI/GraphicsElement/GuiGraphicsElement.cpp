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
			{
			}

			GuiSolidBorderElement::~GuiSolidBorderElement()
			{
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

/***********************************************************************
GuiSolidBackgroundElement
***********************************************************************/

			GuiSolidBackgroundElement::GuiSolidBackgroundElement()
				:color(255, 255, 255)
			{
			}

			GuiSolidBackgroundElement::~GuiSolidBackgroundElement()
			{
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

/***********************************************************************
GuiSolidLabelElement
***********************************************************************/

			GuiSolidLabelElement::GuiSolidLabelElement()
				:color(0, 0, 0)
			{
				fontProperties.fontFamily=L"Lucida Console";
				fontProperties.size=12;
			}

			GuiSolidLabelElement::~GuiSolidLabelElement()
			{
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
		}
	}
}