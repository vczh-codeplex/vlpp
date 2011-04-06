#include "TreeJson.h"

namespace vl
{
	namespace entities
	{

/***********************************************************************
JsonReader
***********************************************************************/

/***********************************************************************
JsonWriter
***********************************************************************/

		void JsonWriter::Indent(int deltaIndentation)
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

		void JsonWriter::WriteObjectPrefix()
		{
			switch(writerState)
			{
			case WS_OBJECT:
				if(writerObjects.Count()>0)
				{
					writer->WriteString(L" : ");
				}
				break;
			case WS_NEXT_ARRAY_ELEMENT:
				writer->WriteString(L", ");
				break;
			}
		}

		void JsonWriter::WriteEncodedString(const WString& value)
		{
			const wchar_t* reading=value.Buffer();
			writer->WriteString(L"\"");
			while(wchar_t c=*reading++)
			{
				switch(c)
				{
				case L'\"':
					writer->WriteString(L"\\\"");
					break;
				case L'\\':
					writer->WriteString(L"\\\\");
					break;
				case L'/':
					writer->WriteString(L"\\/");
					break;
				case L'\f':
					writer->WriteString(L"\\f");
					break;
				case L'\n':
					writer->WriteString(L"\\n");
					break;
				case L'\r':
					writer->WriteString(L"\\r");
					break;
				case L'\t':
					writer->WriteString(L"\\t");
					break;
				default:
					writer->WriteChar(c);
				}
			}
			writer->WriteString(L"\"");
		}

		void JsonWriter::CloseElement()
		{
			if(writerObjects.Count()==0)
			{
				writerState=WS_END_OF_FILE;
			}
			else switch(writerObjects[writerObjects.Count()-1])
			{
			case WO_OBJECT:
				writerState=WS_NEXT_FIELD;
				break;
			case WO_ARRAY:
				writerState=WS_NEXT_ARRAY_ELEMENT;
				break;
			}
		}

		JsonWriter::JsonWriter(stream::TextWriter& _writer, bool _autoNewLine, const WString& _space)
			:writer(&_writer)
			,autoNewLine(_autoNewLine)
			,space(_space)
			,indentation(0)
			,writerState(WS_OBJECT)
		{
		}

		JsonWriter::~JsonWriter()
		{
		}

		bool JsonWriter::OpenObject()
		{
			if(writerState==WS_OBJECT || writerState==WS_FIRST_ARRAY_ELEMENT || writerState==WS_NEXT_ARRAY_ELEMENT)
			{
				WriteObjectPrefix();
				writer->WriteString(L"{");
				indentation++;
				writerObjects.Add(WO_OBJECT);
				writerState=WS_FIRST_FIELD;
				return true;
			}
			return false;
		}

		bool JsonWriter::CloseObject()
		{
			if((writerState==WS_FIRST_FIELD || writerState==WS_NEXT_FIELD) && writerObjects.Count()>0)
			{
				WriterObject wo=writerObjects[writerObjects.Count()-1];
				if(wo==WO_OBJECT)
				{
					writerObjects.RemoveAt(writerObjects.Count()-1);
					Indent(-1);
					writer->WriteString(L"}");
					CloseElement();
					return true;
				}
			}
			return false;
		}

		bool JsonWriter::AddField(const WString& name)
		{
			if(writerState==WS_FIRST_FIELD || writerState==WS_NEXT_FIELD)
			{
				if(writerState==WS_NEXT_FIELD)
				{
					writer->WriteString(L",");
				}
				Indent(0);
				WriteEncodedString(name);
				writerState=WS_OBJECT;
			}
			return false;
		}

		bool JsonWriter::OpenArray()
		{
			if(writerState==WS_OBJECT || writerState==WS_FIRST_ARRAY_ELEMENT || writerState==WS_NEXT_ARRAY_ELEMENT)
			{
				WriteObjectPrefix();
				writer->WriteString(L"[");
				writerObjects.Add(WO_ARRAY);
				writerState=WS_FIRST_ARRAY_ELEMENT;
				return true;
			}
			return false;
		}

		bool JsonWriter::CloseArray()
		{
			if((writerState==WS_FIRST_ARRAY_ELEMENT || writerState==WS_NEXT_ARRAY_ELEMENT) && writerObjects.Count()>0)
			{
				WriterObject wo=writerObjects[writerObjects.Count()-1];
				if(wo==WO_ARRAY)
				{
					writerObjects.RemoveAt(writerObjects.Count()-1);
					writer->WriteString(L"]");
					CloseElement();
					return true;
				}
			}
			return false;
		}

		bool JsonWriter::WriteBool(bool value)
		{
			if(writerState==WS_OBJECT || writerState==WS_FIRST_ARRAY_ELEMENT || writerState==WS_NEXT_ARRAY_ELEMENT)
			{
				WriteObjectPrefix();
				writer->WriteString(value?L"true":L"false");
				CloseElement();
				return true;
			}
			return false;
		}

		bool JsonWriter::WriteInt(vint value)
		{
			if(writerState==WS_OBJECT || writerState==WS_FIRST_ARRAY_ELEMENT || writerState==WS_NEXT_ARRAY_ELEMENT)
			{
				WriteObjectPrefix();
				writer->WriteString(itow(value));
				CloseElement();
				return true;
			}
			return false;
		}

		bool JsonWriter::WriteDouble(double value)
		{
			if(writerState==WS_OBJECT || writerState==WS_FIRST_ARRAY_ELEMENT || writerState==WS_NEXT_ARRAY_ELEMENT)
			{
				WriteObjectPrefix();
				writer->WriteString(ftow(value));
				CloseElement();
				return true;
			}
			return false;
		}

		bool JsonWriter::WriteString(const WString& value)
		{
			if(writerState==WS_OBJECT || writerState==WS_FIRST_ARRAY_ELEMENT || writerState==WS_NEXT_ARRAY_ELEMENT)
			{
				WriteObjectPrefix();
				WriteEncodedString(value);
				CloseElement();
				return true;
			}
			return false;
		}

		bool JsonWriter::WriteNull()
		{
			if(writerState==WS_OBJECT || writerState==WS_FIRST_ARRAY_ELEMENT || writerState==WS_NEXT_ARRAY_ELEMENT)
			{
				WriteObjectPrefix();
				writer->WriteString(L"null");
				CloseElement();
				return true;
			}
			return false;
		}
	}
}