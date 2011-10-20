#include "GuiGraphicsTextElement.h"

namespace vl
{
	namespace presentation
	{
		namespace elements
		{
			using namespace collections;

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
						memcpy(newText, text, start*sizeof(wchar_t));
						memcpy(newText+start, input, inputCount*sizeof(wchar_t));
						memcpy(newText+start+inputCount, text+start+count, (dataLength-start-count)*sizeof(wchar_t));

						CharAtt* newAtt=new CharAtt[newBufferLength];
						memcpy(newAtt, att, start*sizeof(CharAtt));
						memset(newAtt+start, 0, inputCount*sizeof(CharAtt));
						memcpy(newAtt+start+inputCount, att+start+count, (dataLength-start-count)*sizeof(CharAtt));

						delete[] text;
						delete[] att;
						text=newText;
						att=newAtt;
					}
					else
					{
						memmove(text+start+inputCount, text+start+count, (dataLength-start-count)*sizeof(wchar_t));
						memmove(att+start+inputCount, att+start+count, (dataLength-start-count)*sizeof(CharAtt));
						memcpy(text+start, input, inputCount*sizeof(wchar_t));
						memset(att+start, 0, inputCount*sizeof(CharAtt));
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
					int count=dataLength-index;
					TextLine line;
					line.Initialize();
					line.Modify(0, 0, text+index, count);
					memcpy(line.att, att+index, count*sizeof(CharAtt));
					Modify(index, count, L"", 0);
					return line;
				}

				void TextLine::AppendAndFinalize(TextLine& line)
				{
					Modify(dataLength, 0, line.text, line.dataLength);
					memcpy(att+dataLength, line.att, line.dataLength*sizeof(CharAtt));
					line.Finalize();
				}

/***********************************************************************
text::TextLines
***********************************************************************/

				TextLines::TextLines()
					:charMeasurer(0)
				{
					TextLine line;
					line.Initialize();
					lines.Add(line);
				}

				TextLines::~TextLines()
				{
					RemoveLines(0, lines.Count());
				}

				//--------------------------------------------------------

				int TextLines::GetCount()
				{
					return lines.Count();
				}

				TextLine& TextLines::GetLine(int row)
				{
					return lines[row];
				}

				ICharMeasurer* TextLines::GetCharMeasurer()
				{
					return charMeasurer;
				}

				void TextLines::SetCharMeasurer(ICharMeasurer* value)
				{
					charMeasurer=value;
				}

				WString TextLines::GetText(TextPos start, TextPos end)
				{
					if(!IsAvailable(start) || !IsAvailable(end) || start>end) return L"";

					if(start.row==end.row)
					{
						return WString(lines[start.row].text+start.column, end.column-start.column);
					}

					int count=0;
					for(int i=start.row+1;i<end.row;i++)
					{
						count+=lines[i].dataLength;
					}
					count+=lines[start.row].dataLength-start.column;
					count+=end.column;

					Array<wchar_t> buffer;
					buffer.Resize(count+(end.row-start.row)*2);
					wchar_t* writing=&buffer[0];

					for(int i=start.row;i<=end.row;i++)
					{
						wchar_t* text=lines[i].text;
						int chars=0;
						if(i==start.row)
						{
							text+=start.column;
							chars=lines[i].dataLength-start.column;
						}
						else if(i==end.row)
						{
							chars=end.column;
						}
						else
						{
							chars=lines[i].dataLength;
						}

						if(i!=start.row)
						{
							*writing++=L'\r';
							*writing++=L'\n';
						}
						memcpy(writing, text, chars*sizeof(wchar_t));
						writing+=chars;
					}
					return WString(&buffer[0], buffer.Count());
				}

				WString TextLines::GetText()
				{
					return GetText(TextPos(0, 0), TextPos(lines.Count()-1, lines[lines.Count()-1].dataLength));
				}

				//--------------------------------------------------------

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
					return 0<=pos.row && pos.row<lines.Count() && 0<=pos.column && pos.column<=lines[pos.row].dataLength;
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
						lines[start.row+i].Modify(0, lines[start.row+i].dataLength, inputs[i], inputCounts[i]);
					}
					return true;
				}

				bool TextLines::Modify(TextPos start, TextPos end, const wchar_t* input, int inputCount)
				{
					List<const wchar_t*> inputs;
					List<int> inputCounts;
					const wchar_t* previous=input;
					const wchar_t* current=input;

					while(true)
					{
						if(current==input+inputCount)
						{
							inputs.Add(previous);
							inputCounts.Add(current-previous);
							break;
						}
						else if(*current==L'\r' || *current==L'\n')
						{
							inputs.Add(previous);
							inputCounts.Add(current-previous);
							previous=current+(current[1]==L'\n'?2:1);
							current=previous;
						}
						else
						{
							current++;
						}
					}

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

				void TextLines::Clear()
				{
					RemoveLines(0, lines.Count());
					TextLine line;
					line.Initialize();
					lines.Add(line);
				}

				//--------------------------------------------------------
			}
		}
	}
}