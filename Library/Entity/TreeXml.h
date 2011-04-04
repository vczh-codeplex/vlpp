/***********************************************************************
Vczh Library++ 3.0
Developer: ³Âè÷å«(vczh)
Framework::Entity

Classes:
	XmlReader									£ºXML¶ÁÈ¡
	XmlWriter									£ºXMLÐ´Èë
	TreeXmlSerializer							£º¿É²éÑ¯Ê÷XML¶ÁÐ´
***********************************************************************/

#ifndef VCZH_ENTITY_TREEXML
#define VCZH_ENTITY_TREEXML

#include "TreeQuery.h"
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
				Comment,
				ElementHeadOpening,
				ElementHeadClosing,
				ElementClosing,
				Attribute,
				Text,
				CData,

				BeginOfFile,
				EndOfFile,
				WrongFormat,
			};

		protected:
			stream::TextReader*					reader;
			ComponentType						componentType;
			WString								name;
			WString								value;
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

/***********************************************************************
TreeXml
***********************************************************************/
	}
}

#endif