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

			namespace text
			{

/***********************************************************************
text::TextLine
***********************************************************************/

				TextLine::TextLine()
					:text(0)
					,att(0)
					,availableOffsetCount(0)
					,bufferLength(0)
					,dataLength(0)
				{
				}

				TextLine::~TextLine()
				{
				}

				int TextLine::CalculateBufferLength(int dataLength)
				{
					if(dataLength<1)dataLength=1;
					int bufferLength=dataLength-dataLength%BlockSize;
					if(bufferLength<dataLength)
					{
						bufferLength+=BlockSize;
					}
					return bufferLength;
				}

				void TextLine::Initialize()
				{
					Finalize();
					text=new wchar_t[BlockSize];
					att=new CharAtt[BlockSize];
					bufferLength=BlockSize;

					memset(text, 0, sizeof(wchar_t)*bufferLength);
					memset(att, 0, sizeof(CharAtt)*bufferLength);
				}

				void TextLine::Finalize()
				{
					if(text)
					{
						delete[] text;
						text=0;
					}
					if(att)
					{
						delete[] att;
						att=0;
					}
					availableOffsetCount=0;
					bufferLength=0;
					dataLength=0;
				}

				bool TextLine::IsReady()
				{
					return text && att;
				}

				bool TextLine::Modify(int start, int count, const wchar_t* input, int inputCount)
				{
					if(!text || !att || start<0 || count<0 || start+count>dataLength || inputCount<0) return false;

					int newDataLength=dataLength-count+inputCount;
					int newBufferLength=CalculateBufferLength(newDataLength);
					if(newBufferLength!=bufferLength)
					{
						wchar_t* newText=new wchar_t[newBufferLength];
						memcpy(newText, text, start);
						memcpy(newText+start, input, inputCount);
						memcpy(newText+start+inputCount, text+start+count, dataLength-start-count);

						CharAtt* newAtt=new CharAtt[newBufferLength];
						memcpy(newAtt, att, start);
						memset(newAtt+start, 0, sizeof(inputCount)*sizeof(CharAtt));
						memcpy(newAtt+start+inputCount, att+start+count, dataLength-start-count);

						delete[] text;
						delete[] att;
						text=newText;
						att=newAtt;
					}
					else
					{
						memmove(text+start+inputCount, text+start+count, dataLength-start-count);
						memmove(att+start+inputCount, att+start+count, dataLength-start-count);
						memcpy(text+start, input, inputCount);
						memset(att+start, 0, sizeof(inputCount)*sizeof(CharAtt));
					}
					dataLength=newDataLength;
					bufferLength=newBufferLength;
					if(availableOffsetCount>start)
					{
						availableOffsetCount=start;
					}

					return true;
				}

				TextLine TextLine::Split(int index)
				{
					if(index<0 || index>dataLength) return TextLine();
					TextLine line;
					int count=dataLength-index;
					line.Modify(0, 0, text+index, count);
					memcpy(line.att, att+index, count);
					Modify(index, count, L"", 0);
					return line;
				}

				void TextLine::AppendAndFinalize(TextLine& line)
				{
					Modify(dataLength, 0, line.text, line.dataLength);
					memcpy(att+dataLength, line.att, line.dataLength);
					line.Finalize();
				}

/***********************************************************************
text::TextLines
***********************************************************************/

				TextLines::TextLines()
				{
					TextLine line;
					line.Initialize();
					lines.Add(line);
				}

				TextLines::~TextLines()
				{
					RemoveLines(0, lines.Count());
				}

				bool TextLines::RemoveLines(int start, int count)
				{
					if(start<0 || count<0 || start+count>lines.Count()) return false;
					for(int i=start+count-1;i>=start;i--)
					{
						lines[i].Finalize();
					}
					lines.RemoveRange(start, count);
					return true;
				}

				bool TextLines::IsAvailable(TextPos pos)
				{
					return 0<=pos.row && pos.row<lines.Count() && 0<=pos.column && pos.column<lines[pos.row].dataLength;
				}

				bool TextLines::Modify(TextPos start, TextPos end, const wchar_t** inputs, int* inputCounts, int rows)
				{
					if(!IsAvailable(start) || !IsAvailable(end) || start>end) return false;

					if(rows==1)
					{
						if(start.row==end.row)
						{
							lines[start.row].Modify(start.column, end.column, inputs[0], inputCounts[0]);
						}
						else
						{
							if(end.row-start.row>1)
							{
								RemoveLines(start.row+1, end.row-start.row-1);
							}
							int modifyCount=lines[start.row].dataLength-start.column+end.column;
							lines[start.row].AppendAndFinalize(lines[start.row+1]);
							lines.RemoveAt(start.row+1);
							lines[start.row].Modify(start.column, modifyCount, inputs[0], inputCounts[0]);
						}
						return true;
					}

					if(start.row==end.row)
					{
						TextLine newLine=lines[start.row].Split(end.column);
						lines.Insert(start.row+1, newLine);
						end=TextPos(start.row+1, 0);
					}

					int oldMiddleLines=end.row-start.row-1;
					int newMiddleLines=rows-2;
					if(oldMiddleLines<newMiddleLines)
					{
						for(int i=oldMiddleLines;i<newMiddleLines;i++)
						{
							TextLine line;
							line.Initialize();
							lines.Insert(end.row, line);
						}
					}
					else if(oldMiddleLines>newMiddleLines)
					{
						RemoveLines(start.row+newMiddleLines+1, oldMiddleLines-newMiddleLines);
					}
					end.row+=newMiddleLines-oldMiddleLines;

					lines[start.row].Modify(start.column, lines[start.row].dataLength-start.column, inputs[0], inputCounts[0]);
					lines[end.row].Modify(0, end.column, inputs[rows-1], inputCounts[rows-1]);
					for(int i=1;i<rows-1;i++)
					{
						lines[start.row+i].Modify(0, lines[start.row+1].dataLength, inputs[i], inputCounts[i]);
					}
					return true;
				}

				bool TextLines::Modify(TextPos start, TextPos end, const wchar_t* input, int inputCount)
				{
					List<const wchar_t*> inputs;
					List<int> inputCounts;
					const wchar_t* previous=input;
					const wchar_t* current=input;

					do
					{
						if(*current==L'\0')
						{
							inputs.Add(previous);
							inputCounts.Add(current-previous);
						}
						else if(*current==L'\r' || *current==L'\n')
						{
							inputs.Add(previous);
							inputCounts.Add(current-previous);
							previous=current+(current[1]==L'\n'?1:0);
							current=previous;
						}
						else
						{
							current++;
						}
					}while(*current);

					return Modify(start, end, &inputs[0], &inputCounts[0], inputs.Count());
				}

				bool TextLines::Modify(TextPos start, TextPos end, const wchar_t* input)
				{
					return Modify(start, end, input, wcslen(input));
				}

				bool TextLines::Modify(TextPos start, TextPos end, const WString& input)
				{
					return Modify(start, end, input.Buffer(), input.Length());
				}
			}
		}
	}
}