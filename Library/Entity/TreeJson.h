/***********************************************************************
Vczh Library++ 3.0
Developer: ³Âè÷å«(vczh)
Framework::Entity

Classes:
	JsonReader									£ºJson¶ÁÈ¡
	JsonWriter									£ºJsonÐ´Èë
	TreeJsonSerializer							£º¿É²éÑ¯Ê÷Json¶ÁÐ´
***********************************************************************/

#ifndef VCZH_ENTITY_TREEJSON
#define VCZH_ENTITY_TREEJSON

#include "TreeQuery.h"
#include "..\Stream\Accessor.h"
#include "..\Collections\List.h"

namespace vl
{
	namespace entities
	{

/***********************************************************************
Json
***********************************************************************/

		class JsonReader
		{
		public:
			enum ComponentType
			{
				ObjectOpening,
				ObjectClosing,
				Field,
				ArrayOpening,
				ArrayClosing,
				Bool,
				Int,
				Double,
				String,
				Null,

				BeginOfFile,
				EndOfFile,
				WrongFormat,
			};
		protected:
			enum ReaderState
			{
				RS_NOT_READ_YET,
				RS_FIELD_OR_OBJECT_CLOSING,
				RS_FIELD,
				RS_FIELD_VALUE,
				RS_COMMA_OR_OBJECT_CLOSING,
				RS_ELEMENT_OR_ARRAY_CLOSING,
				RS_ELEMENT,
				RS_COMMA_OR_ARRAY_CLOSING,
				RS_END_OF_FILE,
			};

			enum ReaderObject
			{
				RO_OBJECT,
				RO_ARRAY,
			};

			friend class collections::ReadonlyListEnumerator<ReaderObject>;
		protected:
			stream::TextReader*					reader;
			ComponentType						componentType;
			WString								value;
			
			ReaderState							readerState;
			collections::List<ReaderObject>		readerObjects;

			wchar_t								GetNextChar();
			wchar_t								GetNextCharSkipSpaces();
			WString								GetWord(wchar_t &leading);
			WString								GetText(wchar_t &leading);

			void								CloseObject(bool aggregationObject);
			bool								TransferToObjectOpening();
			bool								TransferToObjectClosing();
			bool								TransferToField(const WString& _value);
			bool								TransferToArrayOpening();
			bool								TransferToArrayClosing();
			bool								TransferToPrimitive(ComponentType _componentType, const WString& _value);
			bool								TransferToBool(const WString& _value);
			bool								TransferToInt(const WString& _value);
			bool								TransferToDouble(const WString& _value);
			bool								TransferToString(const WString& _value);
			bool								TransferToNull();
			bool								TransferToEndOfFile();
			bool								TransferToWrongFormat();
		public:
			JsonReader(stream::TextReader& _reader);
			~JsonReader();

			ComponentType						CurrentComponentType()const { return componentType; }
			const WString&						CurrentValue()const { return value; }
			bool								Next();
			bool								IsAvailable()const { return componentType!=EndOfFile && componentType!=WrongFormat; }
		};

		class JsonWriter
		{
		protected:
			enum WriterState
			{
				WS_OBJECT,
				WS_FIRST_FIELD,
				WS_NEXT_FIELD,
				WS_FIRST_ARRAY_ELEMENT,
				WS_NEXT_ARRAY_ELEMENT,
				WS_END_OF_FILE,
			};

			enum WriterObject
			{
				WO_OBJECT,
				WO_ARRAY,
			};

			friend class collections::ReadonlyListEnumerator<WriterObject>;
		protected:
			stream::TextWriter*					writer;
			bool								autoNewLine;
			WString								space;
			
			vint								indentation;
			WriterState							writerState;
			collections::List<WriterObject>		writerObjects;

			void								Indent(int deltaIndentation);
			void								WriteObjectPrefix();
			void								WriteEncodedString(const WString& value);
			void								CloseElement();
		public:
			JsonWriter(stream::TextWriter& _writer, bool _autoNewLine=true, const WString& _space=L"    ");
			~JsonWriter();

			bool								OpenObject();
			bool								CloseObject();
			bool								AddField(const WString& name);
			bool								OpenArray();
			bool								CloseArray();
			bool								WriteBool(bool value);
			bool								WriteInt(vint value);
			bool								WriteDouble(double value);
			bool								WriteString(const WString& value);
			bool								WriteNull();
		};

/***********************************************************************
TreeJson
***********************************************************************/
	}
}

#endif