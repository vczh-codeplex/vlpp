/***********************************************************************
Vczh Library++ 3.0
Developer: ³Âè÷å«(vczh)
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

			const BasicLanguageMetadata*			GetMetadata()const;
		};

/***********************************************************************
Metadata
***********************************************************************/

		class BasicTypeInfo : public BasicMetadataInfo
		{
			friend class BasicDeclarationInfo;
		protected:
			ResourceRecord<BasicTypeRes>			type;

			BasicTypeInfo(ResourceHandle<BasicTypeRes> _type, const BasicLanguageMetadata* metadata);
		public:
			BasicTypeInfo();
			operator bool()const;
			bool									IsSameRecord(const BasicTypeInfo& typeInfo)const;

			bool									IsPrimitive()const;
			bool									IsPointer()const;
			bool									IsArray()const;
			bool									IsFunction()const;
			bool									IsStructure()const;
			bool									IsGenericArgument()const;
	
			vint									GetSize()const;
			BasicTypeRes::PrimitiveTypeEnum			GetPrimitive()const;
			BasicTypeInfo							GetElementType()const;
			vint									GetElementCount()const;
			vint									GetComponentCount()const;
			BasicTypeInfo							GetComponentType(vint index)const;
			WString									GetComponentName(vint index)const;
			vint									GetComponentOffset(vint index)const;
			WString									GetGenericArgumentName()const;
		};

		class BasicDeclarationInfo : public BasicMetadataInfo
		{
			friend class BasicLanguageMetadata;
		protected:
			ResourceRecord<BasicDeclarationRes>		declaration;

			BasicDeclarationInfo(ResourceHandle<BasicDeclarationRes> _declaration, const BasicLanguageMetadata* metadata);
		public:
			BasicDeclarationInfo();
			operator bool()const;
			bool									IsSameRecord(const BasicDeclarationInfo& declarationInfo)const;

			bool									IsFunction()const;
			bool									IsVariable()const;
			bool									IsStructure()const;
			bool									IsConcept()const;
			bool									IsInstance()const;

			BasicTypeInfo							GetType()const;
			WString									GetName()const;
			vint									GetParameterCount()const;
			WString									GetParameterName(vint index)const;
			vint									GetAddress()const;
			bool									IsLinkingSymbol()const;
			WString									GetLinkingAssemblyName()const;
			WString									GetLinkingSymbolName()const;
			vint									GetGenericArgumentCount()const;
			WString									GetGenericArgumentName(vint index)const;
			WString									GetInstanceConceptAssemblyName()const;
			WString									GetInstanceConceptSymbolName()const;
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

			vint									GetDeclarationCount()const;
			BasicDeclarationInfo					GetDeclaration(vint index)const;
		};
	}
}

#endif