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
			stream::TextReader*					reader;
			ComponentType						componentType;
			WString								value;
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
			stream::TextWriter*					writer;
			bool								autoNewLine;
			WString								space;
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