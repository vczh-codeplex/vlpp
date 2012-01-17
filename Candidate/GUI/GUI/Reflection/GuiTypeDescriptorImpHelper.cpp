#include "GuiTypeDescriptorImpHelper.h"

namespace vl
{
	namespace presentation
	{
		namespace reflection_implementation
		{

/***********************************************************************
IType
***********************************************************************/

			Type::Type(TypeEnum _typeEnum, const WString& _name, TypeDescriptor* _typeDescriptor, TypeProvider* _owner)
				:typeEnum(_typeEnum)
				,name(_name)
				,typeDescriptor(_typeDescriptor)
				,owner(_owner)
				,elementType(0)
				,arrayLength(0)
				,pointerType(0)
				,smartPointerType(0)
				,referenceType(0)
				,rvalueReferenceType(0)
			{
			}

			Type::~Type()
			{
			}

			Type* Type::GetPointerType()
			{
				if(!pointerType)
				{
					pointerType=owner->CreateType(IType::Pointer, name+L"*", 0);
					pointerType->elementType=this;
				}
				return pointerType;
			}

			Type* Type::GetSmartPointerType()
			{
				if(!smartPointerType)
				{
					smartPointerType=owner->CreateType(IType::SmartPointer, name+L"*", 0);
					pointerType->elementType=this;
				}
				return smartPointerType;
			}

			Type* Type::GetReferenceType()
			{
				if(!referenceType)
				{
					referenceType=owner->CreateType(IType::Reference, name+L"&", 0);
					pointerType->elementType=this;
				}
				return referenceType;
			}

			Type* Type::GetRValueReferenceType()
			{
				if(!rvalueReferenceType)
				{
					rvalueReferenceType=owner->CreateType(IType::RValueReference, name+L"&&", 0);
					pointerType->elementType=this;
				}
				return rvalueReferenceType;
			}

			Type* Type::GetArrayType(int length)
			{
				int index=arrayTypes.Keys().IndexOf(length);
				if(index==-1)
				{
					Type* type=owner->CreateType(IType::Array, name+L"["+itow(length)+L"]", 0);
					type->elementType=this;
					type->arrayLength=length;
					arrayTypes.Add(length, type);
					return type;
				}
				else
				{
					return arrayTypes.Values()[index];
				}
			}

			IType::TypeEnum Type::GetTypeEnum()
			{
				return typeEnum;
			}

			WString Type::GetName()
			{
				return name;
			}

			ITypeDescriptor* Type::GetTypeDescriptor()
			{
				return typeDescriptor;
			}

			bool Type::CanConvertTo(IType* targetType)
			{
				if(this==targetType) return true;
				if(typeDescriptor && targetType->GetTypeDescriptor())
				{
					int baseTypes=typeDescriptor->GetBaseTypeCount();
					for(int i=0;i<baseTypes;i++)
					{
						if(typeDescriptor->GetBaseType(i)->CanConvertTo(targetType))
						{
							return true;
						}
					}
				}
				return false;
			}

			IType* Type::GetElementType()
			{
				return elementType;
			}

			int Type::GetArrayLength()
			{
				return arrayLength;
			}

			IType* Type::GetReturnType()
			{
				return 0;
			}

			int Type::GetParameterCount()
			{
				return 0;
			}

			IType* Type::GetParameterType(int index)
			{
				return 0;
			}

/***********************************************************************
TypeProvider
***********************************************************************/

			TypeProvider::TypeProvider()
			{
			}

			TypeProvider::~TypeProvider()
			{
			}

			Type* TypeProvider::CreateType(IType::TypeEnum _typeEnum, const WString& _name, TypeDescriptor* _typeDescriptor)
			{
				Type* type=new Type(_typeEnum, _name, _typeDescriptor, this);
				createdTypes.Add(type);
				if(_typeDescriptor)
				{
					_typeDescriptor->type=type;
					namedTypes.Add(_typeDescriptor->typeName, type);
				}
				return type;
			}

			IType* TypeProvider::SInt8()
			{
				return typeSInt8;
			}

			IType* TypeProvider::UInt8()
			{
				return typeUInt8;
			}

			IType* TypeProvider::SInt16()
			{
				return typeSInt16;
			}

			IType* TypeProvider::UInt16()
			{
				return typeUInt16;
			}

			IType* TypeProvider::SInt32()
			{
				return typeSInt32;
			}

			IType* TypeProvider::UInt32()
			{
				return typeUInt32;
			}

			IType* TypeProvider::SInt64()
			{
				return typeSInt64;
			}

			IType* TypeProvider::UInt64()
			{
				return typeUInt64;
			}

			IType* TypeProvider::Float()
			{
				return typeFloat;
			}

			IType* TypeProvider::Bool()
			{
				return typeBool;
			}

			IType* TypeProvider::Char()
			{
				return typeChar;
			}

			IType* TypeProvider::String()
			{
				return typeString;
			}

			IType* TypeProvider::Pointer(IType* elementType)
			{
				Type* type=dynamic_cast<Type*>(elementType);
				if(type)
				{
					return type->GetPointerType();
				}
				return 0;
			}

			IType* TypeProvider::SmartPointer(IType* elementType)
			{
				Type* type=dynamic_cast<Type*>(elementType);
				if(type)
				{
					return type->GetSmartPointerType();
				}
				return 0;
			}

			IType* TypeProvider::Reference(IType* elementType)
			{
				Type* type=dynamic_cast<Type*>(elementType);
				if(type)
				{
					return type->GetReferenceType();
				}
				return 0;
			}

			IType* TypeProvider::RValueReference(IType* elementType)
			{
				Type* type=dynamic_cast<Type*>(elementType);
				if(type)
				{
					return type->GetRValueReferenceType();
				}
				return 0;
			}

			IType* TypeProvider::Array(IType* elementType, int length)
			{
				Type* type=dynamic_cast<Type*>(elementType);
				if(type)
				{
					return type->GetArrayType(length);
				}
				return 0;
			}

			int TypeProvider::GetDescriptableTypeCount()
			{
				return namedTypes.Count();
			}

			IType* TypeProvider::GetDescriptableType(int index)
			{
				return namedTypes.Values()[index];
			}

			IType* TypeProvider::FindType(const WString& name)
			{
				int index=namedTypes.Keys().IndexOf(name);
				return index==-1?0:namedTypes.Values()[index];
			}
		}
	}
}