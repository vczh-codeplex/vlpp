#include "BasicLanguageMetadata.h"
#include "..\BasicIL\BasicILDefinition.h"

namespace vl
{
	namespace scripting
	{
		using namespace basiclanguage;

/***********************************************************************
BasicMetadataInfo
***********************************************************************/

		BasicMetadataInfo::BasicMetadataInfo(const BasicLanguageMetadata* _metadata)
			:metadata(_metadata)
		{
		}

		const BasicLanguageMetadata* BasicMetadataInfo::GetMetadata()const
		{
			return metadata;
		}

/***********************************************************************
BasicTypeInfo
***********************************************************************/

		BasicTypeInfo::BasicTypeInfo(ResourceHandle<BasicTypeRes> _type, const BasicLanguageMetadata* metadata)
			:BasicMetadataInfo(metadata)
			,type(metadata->GetResourceStream()->ReadRecord(_type))
		{
		}

		BasicTypeInfo::BasicTypeInfo()
			:BasicMetadataInfo(0)
		{
		}

		BasicTypeInfo::operator bool()const
		{
			return type;
		}

		bool BasicTypeInfo::IsSameRecord(const BasicTypeInfo& typeInfo)const
		{
			return type.Pointer()==typeInfo.type.Pointer() && metadata==typeInfo.metadata;
		}

		bool BasicTypeInfo::IsPrimitive()const
		{
			return type->type==BasicTypeRes::Primitive;
		}

		bool BasicTypeInfo::IsPointer()const
		{
			return type->type==BasicTypeRes::Pointer;
		}

		bool BasicTypeInfo::IsArray()const
		{
			return type->type==BasicTypeRes::Array;
		}

		bool BasicTypeInfo::IsFunction()const
		{
			return type->type==BasicTypeRes::Function;
		}

		bool BasicTypeInfo::IsStructure()const
		{
			return type->type==BasicTypeRes::Structure;
		}

		bool BasicTypeInfo::IsGenericArgument()const
		{
			return type->type==BasicTypeRes::GenericArgument;
		}

		vint BasicTypeInfo::GetSize()const
		{
			return type->size;
		}

		BasicTypeRes::PrimitiveTypeEnum BasicTypeInfo::GetPrimitive()const
		{
			return type->primitiveType;
		}

		BasicTypeInfo BasicTypeInfo::GetElementType()const
		{
			return BasicTypeInfo(type->elementType, metadata);
		}

		vint BasicTypeInfo::GetElementCount()const
		{
			return type->elementCount;
		}

		vint BasicTypeInfo::GetComponentCount()const
		{
			if(type->subTypes)
			{
				ResourceArrayRecord<BasicSubTypeRes> subTypes=metadata->GetResourceStream()->ReadArrayRecord<BasicSubTypeRes>(type->subTypes);
				return subTypes.Count();
			}
			else
			{
				return 0;
			}
		}

		BasicTypeInfo BasicTypeInfo::GetComponentType(vint index)const
		{
			ResourceArrayRecord<BasicSubTypeRes> subTypes=metadata->GetResourceStream()->ReadArrayRecord<BasicSubTypeRes>(type->subTypes);
			return BasicTypeInfo(subTypes.Get(index)->type, metadata);
		}

		WString BasicTypeInfo::GetComponentName(vint index)const
		{
			ResourceArrayRecord<BasicSubTypeRes> subTypes=metadata->GetResourceStream()->ReadArrayRecord<BasicSubTypeRes>(type->subTypes);
			return metadata->GetResourceStream()->ReadString(subTypes.Get(index)->name);
		}

		vint BasicTypeInfo::GetComponentOffset(vint index)const
		{
			ResourceArrayRecord<BasicSubTypeRes> subTypes=metadata->GetResourceStream()->ReadArrayRecord<BasicSubTypeRes>(type->subTypes);
			return subTypes.Get(index)->offset;
		}

		WString BasicTypeInfo::GetGenericArgumentName()const
		{
			return metadata->GetResourceStream()->ReadString(type->genericArgumentName);
		}

/***********************************************************************
BasicDeclarationInfo
***********************************************************************/

		BasicDeclarationInfo::BasicDeclarationInfo(ResourceHandle<BasicDeclarationRes> _declaration, const BasicLanguageMetadata* metadata)
			:BasicMetadataInfo(metadata)
			,declaration(metadata->GetResourceStream()->ReadRecord(_declaration))
		{
		}

		BasicDeclarationInfo::BasicDeclarationInfo()
			:BasicMetadataInfo(0)
		{
		}

		BasicDeclarationInfo::operator bool()const
		{
			return declaration;
		}

		bool BasicDeclarationInfo::IsSameRecord(const BasicDeclarationInfo& declarationInfo)const
		{
			return declaration.Pointer()==declarationInfo.declaration.Pointer() && metadata==declarationInfo.metadata;
		}

		bool BasicDeclarationInfo::IsForeignFunction()const
		{
			return declaration->type==BasicDeclarationRes::ForeignFunction;
		}

		bool BasicDeclarationInfo::IsFunction()const
		{
			return declaration->type==BasicDeclarationRes::Function;
		}

		bool BasicDeclarationInfo::IsVariable()const
		{
			return declaration->type==BasicDeclarationRes::Variable;
		}

		bool BasicDeclarationInfo::IsStructure()const
		{
			return declaration->type==BasicDeclarationRes::Structure;
		}

		bool BasicDeclarationInfo::IsConcept()const
		{
			return declaration->type==BasicDeclarationRes::Concept;
		}

		bool BasicDeclarationInfo::IsInstance()const
		{
			return declaration->type==BasicDeclarationRes::Instance;
		}

		bool BasicDeclarationInfo::IsTypeRename()const
		{
			return declaration->type==BasicDeclarationRes::TypeRename;
		}

		bool BasicDeclarationInfo::IsConstant()const
		{
			return declaration->type==BasicDeclarationRes::Constant;
		}

		BasicTypeInfo BasicDeclarationInfo::GetType()const
		{
			return BasicTypeInfo(declaration->declarationType, metadata);
		}

		WString BasicDeclarationInfo::GetName()const
		{
			return metadata->GetResourceStream()->ReadString(declaration->name);
		}

		vint BasicDeclarationInfo::GetParameterCount()const
		{
			if(declaration->parameterNames)
			{
				ResourceArrayRecord<BasicParameterRes> parameters=metadata->GetResourceStream()->ReadArrayRecord(declaration->parameterNames);
				return parameters.Count();
			}
			else
			{
				return 0;
			}
		}

		WString BasicDeclarationInfo::GetParameterName(vint index)const
		{
			ResourceArrayRecord<BasicParameterRes> parameters=metadata->GetResourceStream()->ReadArrayRecord(declaration->parameterNames);
			return metadata->GetResourceStream()->ReadString(parameters.Get(index)->name);
		}

		vint BasicDeclarationInfo::GetAddress()const
		{
			return declaration->address;
		}

		bool BasicDeclarationInfo::IsLinkingSymbol()const
		{
			return declaration->linkingAssemblyName && declaration->linkingSymbolName;
		}

		WString BasicDeclarationInfo::GetLinkingAssemblyName()const
		{
			return metadata->GetResourceStream()->ReadString(declaration->linkingAssemblyName);
		}

		WString BasicDeclarationInfo::GetLinkingSymbolName()const
		{
			return metadata->GetResourceStream()->ReadString(declaration->linkingSymbolName);
		}

		vint BasicDeclarationInfo::GetGenericArgumentCount()const
		{
			if(declaration->genericArgumentNames)
			{
				ResourceArrayRecord<BasicParameterRes> parameters=metadata->GetResourceStream()->ReadArrayRecord(declaration->genericArgumentNames);
				return parameters.Count();
			}
			else
			{
				return 0;
			}
		}

		WString BasicDeclarationInfo::GetGenericArgumentName(vint index)const
		{
			ResourceArrayRecord<BasicParameterRes> parameters=metadata->GetResourceStream()->ReadArrayRecord(declaration->genericArgumentNames);
			return metadata->GetResourceStream()->ReadString(parameters.Get(index)->name);
		}

		WString BasicDeclarationInfo::GetInstanceConceptAssemblyName()const
		{
			return metadata->GetResourceStream()->ReadString(declaration->instanceConceptAssemblyName);
		}

		WString BasicDeclarationInfo::GetInstanceConceptSymbolName()const
		{
			return metadata->GetResourceStream()->ReadString(declaration->instanceConceptSymbolName);
		}

/***********************************************************************
BasicLanguageMetadata
***********************************************************************/

		BasicLanguageMetadata::BasicLanguageMetadata(IMetadataProvider* metadataProvider)
			:LanguageMetadata(metadataProvider, basicil::BasicILResourceNames::BasicLanguageInterfaces)
		{
			if(resources)
			{
				entry=resources->ReadRootRecord<BasicEntryRes>();
			}
		}

		vint BasicLanguageMetadata::GetDeclarationCount()const
		{
			if(entry->declarations)
			{
				ResourceArrayRecord<BasicDeclarationRes> declarations=GetResourceStream()->ReadArrayRecord(entry->declarations);
				return declarations.Count();
			}
			else
			{
				return 0;
			}
		}

		BasicDeclarationInfo BasicLanguageMetadata::GetDeclaration(vint index)const
		{
			ResourceArrayRecord<BasicDeclarationRes> declarations=GetResourceStream()->ReadArrayRecord(entry->declarations);
			return BasicDeclarationInfo(declarations.Get(index), this);
		}
	}
}