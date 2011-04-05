#include "TreeXml.h"

namespace vl
{
	namespace entities
	{

/***********************************************************************
XmlReader
***********************************************************************/

		bool IsSpace(wchar_t c)
		{
			return c==L' ' || c==L'\t' || c==L'\r' || c==L'\n';
		}

		bool IsWord(wchar_t c)
		{
			return L'0'<=c && c<='9' || L'A'<=c && c<='Z' || L'a'<=c && c<='z' || c==L'.' || c==L'-' || c==L':' || c==L'_';
		}

		WString Unescape(const WString& value)
		{
			return value;
		}

		wchar_t XmlReader::GetNextChar()
		{
			if(reader->IsEnd())
			{
				return L'\0';
			}
			else
			{
				return reader->ReadChar();
			}
		}

		wchar_t XmlReader::GetNextCharSkipSpaces()
		{
			do
			{
				wchar_t c=GetNextChar();
				if(IsSpace(c))
					continue;
				return c;
			}while(true);
		}

		WString XmlReader::GetWord(wchar_t &leading)
		{
			WString value=L"";
			while(leading==L'<' || leading==L'\0')
			{
				value+=leading;
				leading=GetNextChar();
			}
			return value;
		}

		bool XmlReader::TransferToElementHeadOpening(const WString& _name)
		{
			componentType=ElementHeadOpening;
			name=_name;
			value=L"";
			currentElementNames.Add(_name);
			return IsAvailable();
		}

		bool XmlReader::TransferToElementHeadClosing()
		{
			if(currentElementNames.Count()>0)
			{
				componentType=ElementHeadClosing;
				name=L"";
				value=L"";
				return IsAvailable();
			}
			else
			{
				return TransferToWrongFormat();
			}
		}

		bool XmlReader::TransferToElementClosing(const WString& _name)
		{
			if(currentElementNames.Count()>0)
			{
				WString last=currentElementNames[currentElementNames.Count()-1];
				currentElementNames.RemoveAt(currentElementNames.Count()-1);
				if(last==_name)
				{
					componentType=ElementClosing;
					name=L"";
					value=L"";
					return IsAvailable();
				}
			}
			return TransferToWrongFormat();
		}

		bool XmlReader::TransferToElementClosing()
		{
			if(currentElementNames.Count()>0)
			{
				currentElementNames.RemoveAt(currentElementNames.Count()-1);
				componentType=ElementClosing;
				name=L"";
				value=L"";
				return IsAvailable();
			}
			return TransferToWrongFormat();
		}

		bool XmlReader::TransferToAttribute(const WString& _name, const WString& _value)
		{
			componentType=Attribute;
			name=_name;
			value=_value;
			return IsAvailable();
		}

		bool XmlReader::TransferToText(const WString& _value)
		{
			if(currentElementNames.Count()>0)
			{
				componentType=Text;
				name=L"";
				value=_value;
				return IsAvailable();
			}
			else
			{
				return TransferToWrongFormat();
			}
		}

		bool XmlReader::TransferToCData(const WString& _value)
		{
			if(currentElementNames.Count()>0)
			{
				componentType=CData;
				name=L"";
				value=_value;
				return IsAvailable();
			}
			else
			{
				return TransferToWrongFormat();
			}
		}

		bool XmlReader::TransferToComment(const WString& _value)
		{
			if(currentElementNames.Count()>0)
			{
				componentType=Comment;
				name=L"";
				value=_value;
				return IsAvailable();
			}
			else
			{
				return TransferToWrongFormat();
			}
		}

		bool XmlReader::TransferToEndOfFile()
		{
			if(currentElementNames.Count()==0 && readerState==RS_BEGIN)
			{
				componentType=EndOfFile;
				name=L"";
				value=L"";
				return IsAvailable();
			}
			else
			{
				return TransferToWrongFormat();
			}
		}

		bool XmlReader::TransferToWrongFormat()
		{
			componentType=WrongFormat;
			name=L"";
			value=L"";
			return IsAvailable();
		}

		XmlReader::XmlReader(stream::TextReader& _reader)
			:reader(&_reader)
			,componentType(BeginOfFile)
			,readerState(RS_NOT_READ_YET)
		{
		}

		XmlReader::~XmlReader()
		{
		}

		bool XmlReader::Next()
		{
			if(!IsAvailable()) return false;
			while(true)
			{
				switch(readerState)
				{
				case RS_NOT_READ_YET:
				case RS_BEGIN:
					{
						wchar_t leading=GetNextCharSkipSpaces();
						if(leading==L'\0')
						{
							return TransferToEndOfFile();
						}
						else if(leading==L'<')
						{
							readerState=RS_ELEMENT_LEADING;
						}
						else
						{
							readerState=RS_BEGIN;
							return TransferToText(GetWord(leading));
						}
					}
					break;
				case RS_ELEMENT_LEADING:
					{
						wchar_t leading=GetNextCharSkipSpaces();
						if(leading==L'\0')
						{
							return TransferToEndOfFile();
						}
						else if(leading==L'/')
						{
							readerState=RS_ELEMENT_CLOSING;
						}
						else if(leading==L'!')
						{
							readerState=RS_ELEMENT_COMMENT_OR_CDATA;
						}
						else if(IsWord(leading))
						{
							WString name=GetWord(leading);
							if(leading==L'/')
							{
								readerState=RS_ELEMENT_PRE_NO_CHILDREN_HEAD_CLOSING;
							}
							else
							{
								readerState=RS_ELEMENT_ATTRIBUTE_OR_HEAD_CLOSING;
							}
							return TransferToElementHeadOpening(name);
						}
						else
						{
							return TransferToWrongFormat();
						}
					}
					break;
				case RS_ELEMENT_CLOSING:
					{
						wchar_t leading=GetNextCharSkipSpaces();
						if(leading==L'\0')
						{
							return TransferToEndOfFile();
						}
						else if(IsWord(leading))
						{
							WString name=GetWord(leading);
							if(IsSpace(leading))
								leading=GetNextCharSkipSpaces();
							if(leading!=L'>')
								return TransferToWrongFormat();
							readerState=RS_BEGIN;
							return TransferToElementClosing(name);
						}
						else
						{
							return TransferToWrongFormat();
						}
					}
					break;
				case RS_ELEMENT_COMMENT_OR_CDATA:
					{
						wchar_t leading=GetNextCharSkipSpaces();
						if(leading==L'\0')
						{
							return TransferToEndOfFile();
						}
						else if(leading==L'-')
						{
							readerState=RS_ELEMENT_COMMENT;
						}
						else if(leading==L'[')
						{
							readerState=RS_ELEMENT_CDATA;
						}
						else
						{
							return TransferToWrongFormat();
						}
					}
					break;
				case RS_ELEMENT_COMMENT:
					{
						wchar_t leading=GetNextCharSkipSpaces();
						if(leading==L'\0')
						{
							return TransferToEndOfFile();
						}
						else if(leading==L'-')
						{
							vint slashCount=0;
							WString value;
							while(leading=GetNextChar())
							{
								switch(leading)
								{
								case L'-':
									if(slashCount<2) slashCount=2;
									break;
								case L'>':
									switch(slashCount)
									{
									case 0:
										value+=L">";
										break;
									case 1:
										value+=L"->";
										slashCount=0;
										break;
									case 2:
										readerState=RS_BEGIN;
										return TransferToComment(value);
									}
									break;
								default:
									switch(slashCount)
									{
									case 1:
										value+=L"-";
										slashCount=0;
										break;
									case 2:
										value+=L"--";
										slashCount=0;
										break;
									}
									value+=leading;
								}
							}
							return TransferToEndOfFile();
						}
						else
						{
							return TransferToWrongFormat();
						}
					}
					break;
				case RS_ELEMENT_CDATA:
					{
						if(GetNextChar()!=L'C')return TransferToWrongFormat();
						if(GetNextChar()!=L'D')return TransferToWrongFormat();
						if(GetNextChar()!=L'A')return TransferToWrongFormat();
						if(GetNextChar()!=L'T')return TransferToWrongFormat();
						if(GetNextChar()!=L'A')return TransferToWrongFormat();
						if(GetNextChar()!=L'[')return TransferToWrongFormat();

						vint slashCount=0;
						WString value;
						while(wchar_t leading=GetNextChar())
						{
							switch(leading)
							{
							case L']':
								if(slashCount<2) slashCount=2;
								break;
							case L'>':
								switch(slashCount)
								{
								case 0:
									value+=L">";
									break;
								case 1:
									value+=L"]>";
									slashCount=0;
									break;
								case 2:
									readerState=RS_BEGIN;
									return TransferToCData(value);
								}
								break;
							default:
								switch(slashCount)
								{
								case 1:
									value+=L"]";
									slashCount=0;
									break;
								case 2:
									value+=L"]]";
									slashCount=0;
									break;
								}
								value+=leading;
							}
						}
						return TransferToEndOfFile();
					}
					break;
				case RS_ELEMENT_ATTRIBUTE_OR_HEAD_CLOSING:
					{
						wchar_t leading=GetNextCharSkipSpaces();
						if(leading==L'\0')
						{
							return TransferToEndOfFile();
						}
						else if(IsWord(leading))
						{
							WString name=GetWord(leading);
							if(IsSpace(leading)) leading=GetNextCharSkipSpaces();
							if(leading!=L'=') return TransferToWrongFormat();
							if(GetNextCharSkipSpaces()!=L'"') return TransferToWrongFormat();

							WString value;
							while(leading=GetNextChar())
							{
								if(leading==L'"')
								{
									return TransferToAttribute(name, Unescape(value));
								}
								else
								{
									value+=leading;
								}
							}
							return TransferToEndOfFile();
						}
						else if(leading==L'/')
						{
							readerState=RS_ELEMENT_PRE_NO_CHILDREN_HEAD_CLOSING;
						}
						else if(leading==L'>')
						{
							readerState=RS_BEGIN;
							return TransferToElementHeadClosing();
						}
						else
						{
							return TransferToWrongFormat();
						}
					}
					break;
				case RS_ELEMENT_PRE_NO_CHILDREN_HEAD_CLOSING:
					{
						readerState=RS_ELEMENT_NO_CHILDREN_HEAD_CLOSING;
						return TransferToElementHeadClosing();
					}
					break;
				case RS_ELEMENT_NO_CHILDREN_HEAD_CLOSING:
					{
						readerState=RS_BEGIN;
						if(GetNextCharSkipSpaces()==L'>')
						{
							return TransferToElementClosing();
						}
						else
						{
							return TransferToWrongFormat();
						}
					}
					break;
				}
			}
		}

/***********************************************************************
XmlWriter
***********************************************************************/

		void XmlWriter::Indent(vint deltaIndentation)
		{
			if(autoNewLine)
			{
				if(deltaIndentation<0)
				{
					indentation+=deltaIndentation;
				}

				writer->WriteLine(L"");
				for(vint i=0;i<indentation;i++)
				{
					writer->WriteString(space);
				}

				if(deltaIndentation>=0)
				{
					indentation+=deltaIndentation;
				}
			}
		}

		bool XmlWriter::NewNode(vint deltaIndentation)
		{
			if(writingXml)
			{
				if(indentation==0)
				{
					return false;
				}
				if(writingElementHeader)
				{
					writer->WriteString(L">");
					writingElementHeader=false;
				}
				Indent(deltaIndentation);
			}
			else
			{
				indentation+=deltaIndentation;
			}
			return true;
		}

		void XmlWriter::WriteEncodedText(const WString& value)
		{
			const wchar_t* reading=value.Buffer();
			while(wchar_t c=*reading++)
			{
				switch(c)
				{
				case L'<':
					writer->WriteString(L"&lt;");
					break;
				case L'>':
					writer->WriteString(L"&gt;");
					break;
				case L'\'':
					writer->WriteString(L"&apos;");
					break;
				case L'\"':
					writer->WriteString(L"&quot;");
					break;
				case L'&':
					writer->WriteString(L"&amp;");
					break;
				default:
					writer->WriteChar(c);
				}
			}
		}

		XmlWriter::XmlWriter(stream::TextWriter& _writer, bool _autoNewLine, const WString& _space)
			:writer(&_writer)
			,autoNewLine(_autoNewLine)
			,space(_space)
			,indentation(0)
			,writingElementHeader(false)
			,writingXml(false)
		{
		}

		XmlWriter::~XmlWriter()
		{
		}

		bool XmlWriter::OpenElement(const WString& name)
		{
			if(!NewNode(1)) return false;
			writingXml=true;
			writingElementHeader=true;
			currentElementNames.Add(name);
			writer->WriteString(L"<");
			writer->WriteString(name);
			return true;
		}

		bool XmlWriter::CloseElement()
		{
			if(indentation<=0) return false;
			if(writingElementHeader)
			{
				writer->WriteString(L"/>");
			}
			else
			{
				Indent(-1);
				writer->WriteString(L"</");
				writer->WriteString(currentElementNames[currentElementNames.Count()-1]);
				writer->WriteString(L">");
			}
			currentElementNames.RemoveAt(currentElementNames.Count()-1);
			return true;
		}

		bool XmlWriter::WriteElement(const WString& name, const WString& value)
		{
			if(!NewNode(0)) return false;
			writer->WriteString(L"<");
			writer->WriteString(name);
			writer->WriteString(L">");
			WriteEncodedText(value);
			writer->WriteString(L"</");
			writer->WriteString(name);
			writer->WriteString(L">");
			return true;
		}

		bool XmlWriter::WriteAttribute(const WString& name, const WString& value)
		{
			if(!writingElementHeader) return false;
			writer->WriteString(L" ");
			writer->WriteString(name);
			writer->WriteString(L" = \"");
			WriteEncodedText(value);
			writer->WriteString(L"\"");
			return true;
		}

		bool XmlWriter::WriteText(const WString& value)
		{
			if(!NewNode(0)) return false;
			WriteEncodedText(value);
			return true;
		}

		bool XmlWriter::WriteCData(const WString& value)
		{
			if(writingXml)
			{
				if(indentation==0)
				{
					return false;
				}
				if(writingElementHeader)
				{
					writer->WriteString(L">");
					writingElementHeader=false;
				}
			}
			writer->WriteString(L"<![CDATA[");
			writer->WriteString(value);
			writer->WriteString(L"]]>");
			return true;
		}

		bool XmlWriter::WriteComment(const WString& value)
		{
			if(!NewNode(0)) return false;
			writer->WriteString(L"<!--");
			WriteEncodedText(value);
			writer->WriteString(L"-->");
			return true;
		}
	}
}