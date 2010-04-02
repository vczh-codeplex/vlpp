#include "BasicLanguageMetadata.h"
#include "..\BasicIL\BasicILDefinition.h"

namespace vl
{
	namespace scripting
	{

/***********************************************************************
BasicMetadataInfo
***********************************************************************/

		BasicMetadataInfo::BasicMetadataInfo(const BasicLanguageMetadata* _metadata)
			:metadata(_metadata)
		{
		}

/***********************************************************************
BasicTypeInfo
***********************************************************************/

		ResourceHandle<BasicTypeLinkRes> BasicTypeInfo::GetComponent(int index)const
		{
			int count=0;
			ResourceHandle<BasicTypeLinkRes> currentComponent=type->subTypes;
			while(currentComponent && count!=index)
			{
				currentComponent=metadata->GetResourceStream()->ReadRecord(currentComponent)->next;
				count++;
			}
			return currentComponent;
		}

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

		int BasicTypeInfo::GetSize()const
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

		int BasicTypeInfo::GetElementCount()const
		{
			return type->elementCount;
		}

		int BasicTypeInfo::GetComponentCount()const
		{
			int count=0;
			ResourceHandle<BasicTypeLinkRes> currentComponent=type->subTypes;
			while(currentComponent)
			{
				currentComponent=metadata->GetResourceStream()->ReadRecord(currentComponent)->next;
				count++;
			}
			return count;
		}

		BasicTypeInfo BasicTypeInfo::GetComponentType(int index)const
		{
			ResourceHandle<BasicTypeLinkRes> currentComponent=GetComponent(index);
			if(currentComponent)
			{
				return BasicTypeInfo(metadata->GetResourceStream()->ReadRecord(currentComponent)->type, metadata);
			}
			else
			{
				return BasicTypeInfo();
			}
		}

		WString BasicTypeInfo::GetComponentName(int index)const
		{
			ResourceHandle<BasicTypeLinkRes> currentComponent=GetComponent(index);
			if(currentComponent)
			{
				return metadata->GetResourceStream()->ReadString(metadata->GetResourceStream()->ReadRecord(currentComponent)->name);
			}
			else
			{
				return L"";
			}
		}

		int BasicTypeInfo::GetComponentOffset(int index)const
		{
			ResourceHandle<BasicTypeLinkRes> currentComponent=GetComponent(index);
			if(currentComponent)
			{
				return metadata->GetResourceStream()->ReadRecord(currentComponent)->offset;
			}
			else
			{
				return -1;
			}
		}

/***********************************************************************
BasicDeclarationInfo
***********************************************************************/
		
		ResourceHandle<BasicParameterRes> BasicDeclarationInfo::GetParameter(int index)const
		{
			int count=0;
			ResourceHandle<BasicParameterRes> currentParameter=declaration->parameterNames;
			while(currentParameter && count!=index)
			{
				currentParameter=metadata->GetResourceStream()->ReadRecord(currentParameter)->next;
				count++;
			}
			return currentParameter;
		}

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

		BasicTypeInfo BasicDeclarationInfo::GetType()const
		{
			return BasicTypeInfo(declaration->declarationType, metadata);
		}

		WString BasicDeclarationInfo::GetName()const
		{
			return metadata->GetResourceStream()->ReadString(declaration->name);
		}

		int BasicDeclarationInfo::GetParameterCount()const
		{
			int count=0;
			ResourceHandle<BasicParameterRes> currentParameter=declaration->parameterNames;
			while(currentParameter)
			{
				currentParameter=metadata->GetResourceStream()->ReadRecord(currentParameter)->next;
				count++;
			}
			return count;
		}

		WString BasicDeclarationInfo::GetParameterName(int index)const
		{
			ResourceHandle<BasicParameterRes> parameter=GetParameter(index);
			if(parameter)
			{
				return metadata->GetResourceStream()->ReadString(metadata->GetResourceStream()->ReadRecord(parameter)->name);
			}
			else
			{
				return L"";
			}
		}

		int BasicDeclarationInfo::GetAddress()const
		{
			return declaration->address;
		}

/***********************************************************************
BasicLanguageMetadata
***********************************************************************/

		BasicLanguageMetadata::BasicLanguageMetadata(IMetadataProvider* metadataProvider)
			:LanguageMetadata(metadataProvider, basicil::BasicILResourceNames::BasicLanguageInterfaces)
		{
			entry=resources->ReadRootRecord<BasicEntryRes>();
		}

		int BasicLanguageMetadata::GetDeclarationCount()const
		{
			int count=0;
			ResourceHandle<BasicDeclarationLinkRes> currentDeclaration=entry->declarations;
			while(currentDeclaration)
			{
				currentDeclaration=resources->ReadRecord(currentDeclaration)->next;
				count++;
			}
			return count;
		}

		BasicDeclarationInfo BasicLanguageMetadata::GetDeclaration(int index)const
		{
			int count=0;
			ResourceHandle<BasicDeclarationLinkRes> currentDeclaration=entry->declarations;
			while(currentDeclaration && count!=index)
			{
				currentDeclaration=resources->ReadRecord(currentDeclaration)->next;
				count++;
			}
			if(currentDeclaration)
			{
				return BasicDeclarationInfo(resources->ReadRecord(currentDeclaration)->declaration, this);
			}
			else
			{
				return BasicDeclarationInfo();
			}
		}
	}
}