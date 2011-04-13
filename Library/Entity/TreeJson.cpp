#include "TreeJson.h"
#include <stdlib.h>

namespace vl
{
	namespace entities
	{
		using namespace collections;

/***********************************************************************
JsonReader
***********************************************************************/

		namespace JsonReaderHelper
		{
			bool IsSpace(wchar_t c)
			{
				return c==L' ' || c==L'\t' || c==L'\r' || c==L'\n';
			}

			bool IsWord(wchar_t c)
			{
				return L'0'<=c && c<='9' || L'A'<=c && c<='Z' || L'a'<=c && c<='z' || c==L'.';
			}

			WString Unescape(const WString& value)
			{
				Array<wchar_t> buffer(value.Length()+1);
				const wchar_t* reading=value.Buffer();
				wchar_t* writing=&buffer[0];

				while(wchar_t c=*reading++)
				{
					if(c==L'\\')
					{
						switch(c=*reading++)
						{
						case L'\"':
							*writing++=L'\"';
							break;
						case L'\\':
							*writing++=L'\\';
							break;
						case L'/':
							*writing++=L'/';
							break;
						case L'\f':
							*writing++=L'\f';
							break;
						case L'\n':
							*writing++=L'\n';
							break;
						case L'\r':
							*writing++=L'\r';
							break;
						case L'\t':
							*writing++=L'\t';
							break;
						}
					}
					else
					{
						*writing++=c;
					}
				}
				*writing=L'\0';
				return &buffer[0];
			}
		}
		using namespace JsonReaderHelper;

		wchar_t JsonReader::GetNextChar()
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

		wchar_t JsonReader::GetNextCharSkipSpaces()
		{
			do
			{
				wchar_t c=GetNextChar();
				if(IsSpace(c))
					continue;
				return c;
			}while(true);
		}

		WString JsonReader::GetWord(wchar_t &leading)
		{
			WString value=L"";
			while(IsWord(leading))
			{
				value+=leading;
				leading=GetNextChar();
			}
			return value;
		}

		WString JsonReader::GetText(wchar_t &leading)
		{
			WString value=L"";
			bool escaping=false;
			while(leading=GetNextChar())
			{
				switch(leading)
				{
				case L'\\':
					escaping=true;
					value+=L'\\';
					break;
				case L'\"':
					if(!escaping)
					{
						return Unescape(value);
					}
				default:
					value+=leading;
					escaping=false;
				}
			}
			return L"";
		}

		void JsonReader::CloseObject(bool aggregationObject)
		{
			if(aggregationObject)
			{
				readerObjects.RemoveAt(readerObjects.Count()-1);
			}
			if(readerObjects.Count()==0)
			{
				readerState=RS_END_OF_FILE;
			}
			else switch(readerObjects[readerObjects.Count()-1])
			{
			case RO_OBJECT:
				readerState=RS_COMMA_OR_OBJECT_CLOSING;
				break;
			case RO_ARRAY:
				readerState=RS_COMMA_OR_ARRAY_CLOSING;
				break;
			}
		}

		bool JsonReader::TransferToObjectOpening()
		{
			if(readerState==RS_NOT_READ_YET || readerState==RS_FIELD_VALUE || readerState==RS_ELEMENT_OR_ARRAY_CLOSING || readerState==RS_ELEMENT)
			{
				componentType=ObjectOpening;
				value=L"";
				readerObjects.Add(RO_OBJECT);
				readerState=RS_FIELD_OR_OBJECT_CLOSING;
				return true;
			}
			return TransferToWrongFormat();
		}

		bool JsonReader::TransferToObjectClosing()
		{
			if(readerObjects.Count()>0 && readerObjects[readerObjects.Count()-1]==RO_OBJECT)
			{
				componentType=ObjectClosing;
				value=L"";
				CloseObject(true);
				return true;
			}
			return TransferToWrongFormat();
		}

		bool JsonReader::TransferToField(const WString& _value)
		{
			if(readerState==RS_FIELD_OR_OBJECT_CLOSING || readerState==RS_FIELD)
			{
				componentType=Field;
				value=value;
				readerState=RS_FIELD_VALUE;
				return true;
			}
			return TransferToWrongFormat();
		}

		bool JsonReader::TransferToArrayOpening()
		{
			if(readerState==RS_NOT_READ_YET || readerState==RS_FIELD_VALUE || readerState==RS_ELEMENT_OR_ARRAY_CLOSING || readerState==RS_ELEMENT)
			{
				componentType=ArrayOpening;
				value=L"";
				readerObjects.Add(RO_ARRAY);
				readerState=RS_ELEMENT_OR_ARRAY_CLOSING;
				return true;
			}
			return TransferToWrongFormat();
		}

		bool JsonReader::TransferToArrayClosing()
		{
			if(readerObjects.Count()>0 && readerObjects[readerObjects.Count()-1]==RO_ARRAY)
			{
				componentType=ArrayClosing;
				value=L"";
				CloseObject(true);
				return true;
			}
			return TransferToWrongFormat();
		}

		bool JsonReader::TransferToPrimitive(ComponentType _componentType, const WString& _value)
		{
			if(readerState==RS_NOT_READ_YET || readerState==RS_FIELD_VALUE || readerState==RS_ELEMENT_OR_ARRAY_CLOSING || readerState==RS_ELEMENT)
			{
				componentType=_componentType;
				value=_value;
				CloseObject(false);
				return true;
			}
			return TransferToWrongFormat();
		}

		bool JsonReader::TransferToBool(const WString& _value)
		{
			return TransferToPrimitive(Bool, _value);
		}

		bool JsonReader::TransferToInt(const WString& _value)
		{
			return TransferToPrimitive(Int, _value);
		}

		bool JsonReader::TransferToDouble(const WString& _value)
		{
			return TransferToPrimitive(Double, _value);
		}

		bool JsonReader::TransferToString(const WString& _value)
		{
			return TransferToPrimitive(String, _value);
		}

		bool JsonReader::TransferToNull()
		{
			return TransferToPrimitive(Null, L"");
		}

		bool JsonReader::TransferToEndOfFile()
		{
			if(readerState==RS_END_OF_FILE)
			{
				componentType=EndOfFile;
				value=L"";
				return true;
			}
			return TransferToWrongFormat();
		}

		bool JsonReader::TransferToWrongFormat()
		{
			componentType=WrongFormat;
			value=L"";
			return false;
		}

		JsonReader::JsonReader(stream::TextReader& _reader)
			:reader(&_reader)
			,componentType(BeginOfFile)
			,readerState(RS_NOT_READ_YET)
		{
		}

		JsonReader::~JsonReader()
		{
		}

		bool JsonReader::Next()
		{
			if(!IsAvailable()) return false;
			while(true)
			{
				switch(readerState)
				{
				case RS_NOT_READ_YET:
				case RS_FIELD_VALUE:
				case RS_ELEMENT_OR_ARRAY_CLOSING:
				case RS_ELEMENT:
					if(wchar_t leading=GetNextCharSkipSpaces())
					{
						switch(leading)
						{
						case L'{':
							return TransferToObjectOpening();
						case L'[':
							return TransferToArrayOpening();
						case L']':
							return TransferToArrayClosing();
						case L'\"':
							{
								WString stringValue=GetText(leading);
								if(leading==L'\0')
								{
									return TransferToEndOfFile();
								}
								return TransferToString(stringValue);
							}
						default:
							if(IsWord(leading))
							{
								WString wordValue=GetWord(leading);
								if(wordValue==L"null")
								{
									return TransferToNull();
								}
								else if(wordValue==L"true")
								{
									return TransferToBool(L"true");
								}
								else if(wordValue==L"false")
								{
									return TransferToBool(L"false");
								}
								else
								{
									const wchar_t* reading=wordValue.Buffer();
									wchar_t* endptr=0;
									wcstod(reading, &endptr);
									if(*endptr==L'\0')
									{
										return TransferToDouble(wordValue);
									}

									wcstol(reading, &endptr, 10);
									if(*endptr==L'\0')
									{
										return TransferToInt(wordValue);
									}

									return TransferToWrongFormat();
								}
							}
							else
							{
								return TransferToWrongFormat();
							}
						}
					}
					else
					{
						return TransferToEndOfFile();
					}
					break;
				case RS_FIELD_OR_OBJECT_CLOSING:
				case RS_FIELD:
					if(wchar_t leading=GetNextCharSkipSpaces())
					{
						if(leading==L'\"')
						{
							WString fieldName=GetText(leading);
							if(leading==L'\0')
							{
								return TransferToEndOfFile();
							}
							return TransferToField(fieldName);
						}
						else if(leading==L'}')
						{
							return TransferToObjectClosing();
						}
						else
						{
							return TransferToWrongFormat();
						}
					}
					else
					{
						return TransferToEndOfFile();
					}
				case RS_COMMA_OR_OBJECT_CLOSING:
				case RS_COMMA_OR_ARRAY_CLOSING:
					if(wchar_t leading=GetNextCharSkipSpaces())
					{
						if(leading==L',')
						{
							readerState=(readerState==RS_COMMA_OR_OBJECT_CLOSING?RS_FIELD:RS_ELEMENT);
						}
						else if(leading==L'}')
						{
							return TransferToObjectClosing();
						}
						else if(leading==L']')
						{
							return TransferToArrayClosing();
						}
						else
						{
							return TransferToWrongFormat();
						}
					}
					else
					{
						return TransferToEndOfFile();
					}
				case RS_END_OF_FILE:
					return TransferToEndOfFile();
				}
			}
		}

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