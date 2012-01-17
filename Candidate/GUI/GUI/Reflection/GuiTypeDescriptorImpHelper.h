/***********************************************************************
Vczh Library++ 3.0
Developer: ³Âè÷å«(vczh)
GacUI::Reflection

Interfaces:
***********************************************************************/

#ifndef VCZH_PRESENTATION_REFLECTION_GUITYPEDESCRIPTORIMPHELPER
#define VCZH_PRESENTATION_REFLECTION_GUITYPEDESCRIPTORIMPHELPER

#include "GuiTypeDescriptor.h"

namespace vl
{
	namespace presentation
	{
		namespace reflection_implementation
		{

/***********************************************************************
IType
***********************************************************************/

			class TypeDescriptor;
			class TypeProvider;

			class Type : public Object, public IType
			{
				friend class TypeProvider;
				friend class Ptr<Type>;

				typedef collections::Dictionary<int, Type*>			ArrayTypeMap;
			protected:
				TypeEnum							typeEnum;
				WString								name;
				TypeDescriptor*						typeDescriptor;
				TypeProvider*						owner;
				Type*								elementType;
				int									arrayLength;

				Type*								pointerType;
				Type*								smartPointerType;
				Type*								referenceType;
				Type*								rvalueReferenceType;
				ArrayTypeMap						arrayTypes;
			protected:
				Type(TypeEnum _typeEnum, const WString& _name, TypeDescriptor* _typeDescriptor, TypeProvider* _owner);
				~Type();

				Type*								GetPointerType();
				Type*								GetSmartPointerType();
				Type*								GetReferenceType();
				Type*								GetRValueReferenceType();
				Type*								GetArrayType(int length);

				TypeEnum							GetTypeEnum()override;
				WString								GetName()override;
				ITypeDescriptor*					GetTypeDescriptor()override;
				bool								CanConvertTo(IType* targetType)override;

				IType*								GetElementType()override;
				int									GetArrayLength()override;
				IType*								GetReturnType()override;
				int									GetParameterCount()override;
				IType*								GetParameterType(int index)override;
			};

/***********************************************************************
ITypeDescriptor
***********************************************************************/

			class TypeDescriptor : public Object, public ITypeDescriptor
			{
				friend class TypeProvider;
			protected:
				Type*								type;
				WString								typeName;

			public:
				TypeDescriptor(const WString& typeName);
				~TypeDescriptor();
			};

/***********************************************************************
ITypeProvider
***********************************************************************/

			class TypeProvider : public Object, public ITypeProvider
			{
				typedef collections::List<Ptr<Type>>				TypeList;
				typedef collections::Dictionary<WString, Type*>		NameTypeMap;
			protected:
				TypeList							createdTypes;
				NameTypeMap							namedTypes;

				Type*								typeSInt8;
				Type*								typeUInt8;
				Type*								typeSInt16;
				Type*								typeUInt16;
				Type*								typeSInt32;
				Type*								typeUInt32;
				Type*								typeSInt64;
				Type*								typeUInt64;
				Type*								typeFloat;
				Type*								typeBool;
				Type*								typeChar;
				Type*								typeString;
			public:
				TypeProvider();
				~TypeProvider();

				Type*								CreateType(IType::TypeEnum _typeEnum, const WString& _name, TypeDescriptor* _typeDescriptor);
				IType*								SInt8()override;
				IType*								UInt8()override;
				IType*								SInt16()override;
				IType*								UInt16()override;
				IType*								SInt32()override;
				IType*								UInt32()override;
				IType*								SInt64()override;
				IType*								UInt64()override;
				IType*								Float()override;
				IType*								Bool()override;
				IType*								Char()override;
				IType*								String()override;
				IType*								Pointer(IType* elementType)override;
				IType*								SmartPointer(IType* elementType)override;
				IType*								Reference(IType* elementType)override;
				IType*								RValueReference(IType* elementType)override;
				IType*								Array(IType* elementType, int length)override;

				int									GetDescriptableTypeCount()override;
				IType*								GetDescriptableType(int index)override;
				IType*								FindType(const WString& name)override;
			};
		}
	}
}

#endif