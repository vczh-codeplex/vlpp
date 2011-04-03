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
				ElementOpening,
				ElementClosing,
				Attribute,
				Text,
				CData,
				EndOfFile
			};

		protected:
			stream::TextReader*					reader;
			ComponentType						componentType;
			WString								name;
			WString								value;
		public:
			XmlReader(stream::TextReader& _reader);
			~XmlReader();

			ComponentType						CurrentComponentType()const;
			const WString&						CurrentName()const;
			const WString&						CurrentValue()const;
			bool								Next();
			bool								IsAvailable()const;
		};

		class XmlWriter
		{
		protected:
			stream::TextWriter*					writer;
		public:
			XmlWriter(stream::TextWriter& _writer);
			~XmlWriter();

			bool								OpenElement(const WString& name);
			bool								CloseElement(const WString& name);
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