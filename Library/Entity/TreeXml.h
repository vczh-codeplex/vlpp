/***********************************************************************
Vczh Library++ 3.0
Developer: ³Âè÷å«(vczh)
Framework::Entity

Classes:
	XmlReader									£ºXML¶ÁÈ¡
	XmlWriter									£ºXMLÐ´Èë
***********************************************************************/

#ifndef VCZH_ENTITY_TREEXML
#define VCZH_ENTITY_TREEXML

#include "..\Stream\Accessor.h"
#include "..\Collections\List.h"

namespace vl
{
	namespace entities
	{

/***********************************************************************
Xml
***********************************************************************/

		class XmlReader
		{
		public:
			enum ComponentType
			{
				ElementHeadOpening,		// name
				ElementHeadClosing,		//
				ElementClosing,			//
				Attribute,				// name, value
				Text,					// value
				CData,					// value
				Comment,				// value

				BeginOfFile,
				EndOfFile,
				WrongFormat,
			};

		protected:
			enum ReaderState
			{
				RS_NOT_READ_YET,
				RS_BEGIN,
				RS_ELEMENT_LEADING,
				RS_ELEMENT_CLOSING,
				RS_ELEMENT_COMMENT_OR_CDATA,
				RS_ELEMENT_COMMENT,
				RS_ELEMENT_CDATA,
				RS_ELEMENT_ATTRIBUTE_OR_HEAD_CLOSING,
				RS_ELEMENT_PRE_NO_CHILDREN_HEAD_CLOSING,
				RS_ELEMENT_NO_CHILDREN_HEAD_CLOSING,
				RS_ELEMENT_HEAD_CLOSING,
			};

			stream::TextReader*					reader;
			ComponentType						componentType;
			WString								name;
			WString								value;

			ReaderState							readerState;
			collections::List<WString>			currentElementNames;

			wchar_t								GetNextChar();
			wchar_t								GetNextCharSkipSpaces();
			WString								GetWord(wchar_t &leading);
			WString								GetText(wchar_t &leading);

			bool								TransferToElementHeadOpening(const WString& _name);
			bool								TransferToElementHeadClosing();
			bool								TransferToElementClosing(const WString& _name);
			bool								TransferToElementClosing();
			bool								TransferToAttribute(const WString& _name, const WString& _value);
			bool								TransferToText(const WString& _value);
			bool								TransferToCData(const WString& _value);
			bool								TransferToComment(const WString& _value);

			bool								TransferToEndOfFile();
			bool								TransferToWrongFormat();
		public:
			XmlReader(stream::TextReader& _reader);
			~XmlReader();

			ComponentType						CurrentComponentType()const { return componentType; }
			const WString&						CurrentName()const { return name; }
			const WString&						CurrentValue()const { return value; }
			bool								Next();
			bool								IsAvailable()const { return componentType!=EndOfFile && componentType!=WrongFormat; }
		};

		class XmlWriter
		{
		protected:
			stream::TextWriter*					writer;
			bool								autoNewLine;
			WString								space;

			vint								indentation;
			bool								writingElementHeader;
			collections::List<WString>			currentElementNames;
			bool								writingXml;

			void								Indent(vint deltaIndentation);
			bool								NewNode(vint deltaIndentation);
			void								WriteEncodedText(const WString& value);
		public:
			XmlWriter(stream::TextWriter& _writer, bool _autoNewLine=true, const WString& _space=L"    ");
			~XmlWriter();

			bool								OpenElement(const WString& name);
			bool								CloseElement();
			bool								WriteElement(const WString& name, const WString& value);
			bool								WriteAttribute(const WString& name, const WString& value);
			bool								WriteText(const WString& value);
			bool								WriteCData(const WString& value);
			bool								WriteComment(const WString& value);
		};
	}
}

#endif