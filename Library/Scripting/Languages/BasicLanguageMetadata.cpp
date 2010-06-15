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

		const BasicLanguageMetadata* BasicMetadataInfo::GetMetadata()const
		{
			return metadata;
		}

/***********************************************************************
BasicTypeInfo
***********************************************************************/

		ResourceHandle<BasicTypeLinkRes> BasicTypeInfo::GetComponent(vint index)const
		{
			if(components && index>=0 && index<components->Count())
			{
				return components->Get(index);
			}
			else
			{
				return ResourceHandle<BasicTypeLinkRes>();
			}
		}

		BasicTypeInfo::BasicTypeInfo(ResourceHandle<BasicTypeRes> _type, const BasicLanguageMetadata* metadata)
			:BasicMetadataInfo(metadata)
			,type(metadata->GetResourceStream()->ReadRecord(_type))
		{
			components=new ComponentHandleList;
			ResourceHandle<BasicTypeLinkRes> currentComponent=type->subTypes;
			while(currentComponent)
			{
				components->Add(currentComponent);
				currentComponent=metadata->GetResourceStream()->ReadRecord(currentComponent)->next;
			}
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
			return components?components->Count():0;
		}

		BasicTypeInfo BasicTypeInfo::GetComponentType(vint index)const
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

		WString BasicTypeInfo::GetComponentName(vint index)const
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

		vint BasicTypeInfo::GetComponentOffset(vint index)const
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
		
		ResourceHandle<BasicParameterRes> BasicDeclarationInfo::GetParameter(vint index)const
		{
			if(parameters && index>=0 && index<parameters->Count())
			{
				return parameters->Get(index);
			}
			else
			{
				return ResourceHandle<BasicParameterRes>();
			}
		}

		BasicDeclarationInfo::BasicDeclarationInfo(ResourceHandle<BasicDeclarationRes> _declaration, const BasicLanguageMetadata* metadata)
			:BasicMetadataInfo(metadata)
			,declaration(metadata->GetResourceStream()->ReadRecord(_declaration))
		{
			parameters=new ParameterHandleList;
			ResourceHandle<BasicParameterRes> currentParameter=declaration->parameterNames;
			while(currentParameter)
			{
				parameters->Add(currentParameter);
				currentParameter=metadata->GetResourceStream()->ReadRecord(currentParameter)->next;
			}
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

		vint BasicDeclarationInfo::GetParameterCount()const
		{
			return parameters?parameters->Count():0;
		}

		WString BasicDeclarationInfo::GetParameterName(vint index)const
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

/***********************************************************************
BasicLanguageMetadata
***********************************************************************/

		BasicLanguageMetadata::BasicLanguageMetadata(IMetadataProvider* metadataProvider)
			:LanguageMetadata(metadataProvider, basicil::BasicILResourceNames::BasicLanguageInterfaces)
		{
			entry=resources->ReadRootRecord<BasicEntryRes>();
		}

		vint BasicLanguageMetadata::GetDeclarationCount()const
		{
			vint count=0;
			ResourceHandle<BasicDeclarationLinkRes> currentDeclaration=entry->declarations;
			while(currentDeclaration)
			{
				currentDeclaration=resources->ReadRecord(currentDeclaration)->next;
				count++;
			}
			return count;
		}

		BasicDeclarationInfo BasicLanguageMetadata::GetDeclaration(vint index)const
		{
			vint count=0;
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