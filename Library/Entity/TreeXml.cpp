#include "TreeXml.h"

namespace vl
{
	namespace entities
	{

/***********************************************************************
XmlReader
***********************************************************************/

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
			while(wchar_t c=*reading)
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
			writingElementHeader=true;
			currentElementName=name;
			writer->WriteString(L"<");
			writer->WriteString(name);
			return true;
		}

		bool XmlWriter::CloseElement(const WString& name)
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
				writer->WriteString(currentElementName);
				writer->WriteString(L">");
			}
			return true;
		}

		bool XmlWriter::WriteElement(const WString& name, const WString& value)
		{
			if(!NewNode(1)) return false;
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
				}
			}
			writer->WriteString(L"<![CDATA]");
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