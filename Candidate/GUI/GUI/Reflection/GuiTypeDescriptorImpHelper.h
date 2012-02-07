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
				Type*								constReferenceType;
				Type*								rvalueReferenceType;
				ArrayTypeMap						arrayTypes;
			protected:
				Type(TypeEnum _typeEnum, const WString& _name, TypeDescriptor* _typeDescriptor, TypeProvider* _owner);
				~Type();

			public:
				Type*								GetPointerType();
				Type*								GetSmartPointerType();
				Type*								GetReferenceType();
				Type*								GetConstReferenceType();
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

			class ParameterDescriptor : public Object, public IParameterDescriptor
			{
			protected:
				WString								name;
				Type*								type;

			public:
				ParameterDescriptor(const WString& _name, Type* _type);
				~ParameterDescriptor();

				WString								GetName()override;
				IType*								GetType()override;
			};

			class MethodDescriptor : public Object, public IMethodDescriptor
			{
				friend class TypeDescriptor;

				typedef collections::List<Ptr<ParameterDescriptor>>																	ParameterList;
				typedef Func<DescriptableValue(const DescriptableValue&, const collections::IReadonlyList<DescriptableValue>&)>		HandlerFuncType;
			protected:
				WString								name;
				MemberTypeEnum						memberTypeEnum;
				Type*								returnType;
				ParameterList						parameters;
				HandlerFuncType						handler;
				TypeDescriptor*						ownerTypeDescriptor;

			public:
				MethodDescriptor(const WString& _name, MemberTypeEnum _memberTypeEnum);
				~MethodDescriptor();

				MethodDescriptor*					ReturnType(Type* _returnType);
				MethodDescriptor*					Parameter(const WString& _name, Type* _type);
				MethodDescriptor*					Handler(const HandlerFuncType& _handler);

				WString								GetName()override;
				ITypeDescriptor*					GetOwnerTypeDescriptor()override;
				MemberTypeEnum						GetMemberTypeEnum()override;
				IType*								GetReturnType()override;
				int									GetParameterCount()override;
				IParameterDescriptor*				GetParameter(int index)override;
				DescriptableValue					Invoke(const DescriptableValue& thisObject, const collections::IReadonlyList<DescriptableValue>& parameters)override;
			};

			class PropertyDescriptor : public Object, public IPropertyDescriptor
			{
				friend class TypeDescriptor;
			protected:
				WString								name;
				MemberTypeEnum						memberTypeEnum;
				Type*								propertyType;
				Ptr<MethodDescriptor>				getter;
				Ptr<MethodDescriptor>				setter;
				TypeDescriptor*						ownerTypeDescriptor;

			public:
				PropertyDescriptor(const WString& _name, MemberTypeEnum _memberTypeEnum);
				~PropertyDescriptor();

				PropertyDescriptor*					PropertyType(Type* _propertyType);
				PropertyDescriptor*					Getter(Ptr<MethodDescriptor> _getter);
				PropertyDescriptor*					Setter(Ptr<MethodDescriptor> _setter);
				
				WString								GetName()override;
				ITypeDescriptor*					GetOwnerTypeDescriptor()override;
				MemberTypeEnum						GetMemberTypeEnum()override;
				bool								CanGet()override;
				bool								CanSet()override;
				IType*								GetPropertyType()override;
				IMethodDescriptor*					GetSetter()override;
				IMethodDescriptor*					GetGetter()override;
			};

			class TypeDescriptor : public Object, public ITypeDescriptor
			{
				friend class TypeProvider;

				typedef collections::List<Type*>										TypeList;
				typedef collections::List<Ptr<MethodDescriptor>>						MethodList;
				typedef collections::Dictionary<WString, Ptr<PropertyDescriptor>>		PropertyMap;
			private:
				Type*								type;
				bool								contentAvailable;
				IMethodDescriptor*					defaultConstructor;

				TypeList							baseTypeList;
				MethodList							constructorList;
				MethodList							methodList;
				PropertyMap							propertyMap;

				void								EnsureContentAvailable();
			protected:

				void								AddBaseType(Type* value);
				void								AddConstructor(Ptr<MethodDescriptor> value);
				void								AddMethod(Ptr<MethodDescriptor> value);
				void								AddProperty(Ptr<PropertyDescriptor> value);

				virtual void						FillTypeContent()=0;
			public:
				TypeDescriptor();
				~TypeDescriptor();

				IType*								GetType()override;
				int									GetBaseTypeCount()override;
				IType*								GetBaseType(int index)override;

				int									GetConstructorCount()override;
				IMethodDescriptor*					GetConstructor(int index)override;
				IMethodDescriptor*					GetDefaultConstructor()override;
				int									GetMethodCount()override;
				IMethodDescriptor*					GetMethod(int index)override;
				int									GetPropertyCount()override;
				IPropertyDescriptor*				GetProperty(int index)override;

				IMethodDescriptor*					FindMethod(const WString& name, bool searchParent, bool searchStatic, bool searchInstance)override;
				void								FindMethods(const WString& name, bool searchParent, bool searchStatic, bool searchInstance, collections::List<IMethodDescriptor*>& methods)override;
				IPropertyDescriptor*				FindProperty(const WString& name, bool searchParent, bool searchStatic, bool searchInstance)override;
			};

/***********************************************************************
ITypeProvider
***********************************************************************/

			class TypeProvider : public Object, public ITypeProvider
			{
				typedef collections::List<Ptr<Type>>				TypeList;
				typedef collections::List<Ptr<TypeDescriptor>>		TypeDescriptorList;
				typedef collections::Dictionary<WString, Type*>		NameTypeMap;
			protected:
				TypeList							createdTypes;
				TypeDescriptorList					createdTypeDescriptors;
				NameTypeMap							namedTypes;
				
				Type*								typeVoid;
				Type*								typeSInt8;
				Type*								typeUInt8;
				Type*								typeSInt16;
				Type*								typeUInt16;
				Type*								typeSInt32;
				Type*								typeUInt32;
				Type*								typeSInt64;
				Type*								typeUInt64;
				Type*								typeFloat;
				Type*								typeDouble;
				Type*								typeBool;
				Type*								typeChar;
				Type*								typeString;
			public:
				TypeProvider();
				~TypeProvider();

				Type*								CreateType(IType::TypeEnum _typeEnum, const WString& _name, TypeDescriptor* _typeDescriptor);
				IType*								Void()override;
				IType*								SInt8()override;
				IType*								UInt8()override;
				IType*								SInt16()override;
				IType*								UInt16()override;
				IType*								SInt32()override;
				IType*								UInt32()override;
				IType*								SInt64()override;
				IType*								UInt64()override;
				IType*								Float()override;
				IType*								Double()override;
				IType*								Bool()override;
				IType*								Char()override;
				IType*								String()override;
				IType*								Pointer(IType* elementType)override;
				IType*								SmartPointer(IType* elementType)override;
				IType*								Reference(IType* elementType)override;
				IType*								ConstReference(IType* elementType)override;
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