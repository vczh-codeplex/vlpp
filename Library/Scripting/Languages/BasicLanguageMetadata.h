/***********************************************************************
Vczh Library++ 3.0
Developer: �����(vczh)
Scripting::Language Provider

Classes:
***********************************************************************/

#ifndef VCZH_SCRIPTING_LANGUAGES_BASICLANGUAGEMETADATA
#define VCZH_SCRIPTING_LANGUAGES_BASICLANGUAGEMETADATA

#include "LanguageMetadata.h"
#include "..\BasicLanguage\BasicLanguageResource.h"

namespace vl
{
	namespace scripting
	{
		class BasicLanguageMetadata;

		class BasicMetadataInfo : public Object
		{
		protected:
			const BasicLanguageMetadata*			metadata;

			BasicMetadataInfo(const BasicLanguageMetadata* _metadata);
		public:
		};

/***********************************************************************
Metadata
***********************************************************************/

		class BasicTypeInfo : public BasicMetadataInfo
		{
			friend class BasicDeclarationInfo;
		protected:
			ResourceRecord<BasicTypeRes>			type;

			ResourceHandle<BasicTypeLinkRes>		GetComponent(int index)const;
			BasicTypeInfo(ResourceHandle<BasicTypeRes> _type, const BasicLanguageMetadata* metadata);
		public:
			BasicTypeInfo();
			operator bool()const;

			bool									IsPrimitive()const;
			bool									IsPointer()const;
			bool									IsArray()const;
			bool									IsFunction()const;
			bool									IsStructure()const;
	
			int										GetSize()const;
			BasicTypeRes::PrimitiveTypeEnum			GetPrimitive()const;
			BasicTypeInfo							GetElementType()const;
			int										GetElementCount()const;
			int										GetComponentCount()const;
			BasicTypeInfo							GetComponentType(int index)const;
			WString									GetComponentName(int index)const;
			int										GetComponentOffset(int index)const;
		};

		class BasicDeclarationInfo : public BasicMetadataInfo
		{
			friend class BasicLanguageMetadata;
		protected:
			ResourceRecord<BasicDeclarationRes>		declaration;

			ResourceHandle<BasicParameterRes>		GetParameter(int index)const;
			BasicDeclarationInfo(ResourceHandle<BasicDeclarationRes> _declaration, const BasicLanguageMetadata* metadata);
		public:
			BasicDeclarationInfo();
			operator bool()const;

			bool									IsFunction()const;
			bool									IsVariable()const;
			bool									IsStructure()const;
			BasicTypeInfo							GetType()const;
			WString									GetName()const;
			int										GetParameterCount()const;
			WString									GetParameterName(int index)const;
			int										GetAddress()const;
		};

/***********************************************************************
BasicLanguageMetadata
***********************************************************************/

		class BasicLanguageMetadata : public LanguageMetadata
		{
		protected:
			ResourceRecord<BasicEntryRes>			entry;
		public:
			BasicLanguageMetadata(IMetadataProvider* metadataProvider);

			int										GetDeclarationCount()const;
			BasicDeclarationInfo					GetDeclaration(int index)const;
		};
	}
}

#endif