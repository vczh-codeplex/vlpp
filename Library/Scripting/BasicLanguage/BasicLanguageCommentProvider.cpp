#include "BasicLanguageCommentProvider.h"

namespace vl
{
	namespace scripting
	{
		namespace basiclanguage
		{
			using namespace collections;
			using namespace basicil;
			using namespace stream;

			BasicLanguageCommentProvider::BasicLanguageCommentProvider()
			{
				memoryStream=new MemoryStream();
				streamWriter=0;
				lastLine=-2;
			}

			BasicLanguageCommentProvider::~BasicLanguageCommentProvider()
			{
				if(streamWriter)
				{
					delete streamWriter;
				}
				delete memoryStream;
			}

			stream::TextWriter* BasicLanguageCommentProvider::OpenWriter()
			{
				if(!streamWriter)
				{
					memoryStream->SeekFromBegin(0);
					streamWriter=new StreamWriter(*memoryStream);
				}
				return streamWriter;
			}

			void BasicLanguageCommentProvider::CloseWriter()
			{
				if(streamWriter)
				{
					memoryStream->SeekFromBegin(0);
					delete streamWriter;
					streamWriter=0;

					lines.Clear();
					StreamReader reader(*memoryStream);
					while(!reader.IsEnd())
					{
						WString line=reader.ReadLine();
						int spaceCount=0;
						for(int i=0;i<line.Length();i++)
						{
							if(line[i]==L' ')
							{
								spaceCount++;
							}
							else
							{
								break;
							}
						}
						lines.Add(line.Right(line.Length()-spaceCount));
					}

					lastLine=-2;
				}
			}

			bool BasicLanguageCommentProvider::IsOpening()
			{
				return streamWriter!=0;
			}

			void BasicLanguageCommentProvider::AppendComment(stream::TextWriter& writer, void* userData)
			{
				if(userData)
				{
					BasicLanguageElement* element=(BasicLanguageElement*)userData;
					if(element->position.lineIndex!=lastLine)
					{
						lastLine=element->position.lineIndex;
						if(lastLine>=0 && lastLine<lines.Count())
						{
							writer.WriteString(L"// ");
							writer.WriteLine(lines[lastLine]);
						}
						else
						{
							writer.WriteLine(L"// UNKNOWN LINE");
						}
					}
				}
			}
		}
	}
}