#include "GuiTypeDescriptorImpHelper.h"

namespace vl
{
	namespace presentation
	{
		namespace reflection_implementation
		{
			using namespace collections;

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
				,constReferenceType(0)
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

			Type* Type::GetConstReferenceType()
			{
				if(!constReferenceType)
				{
					constReferenceType=owner->CreateType(IType::ConstReference, name+L" const&", 0);
					pointerType->elementType=this;
				}
				return constReferenceType;
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
ParameterDescriptor
***********************************************************************/

			ParameterDescriptor::ParameterDescriptor(const WString& _name, Type* _type)
				:name(_name)
				,type(_type)
			{
			}

			ParameterDescriptor::~ParameterDescriptor()
			{
			}

			WString ParameterDescriptor::GetName()
			{
				return name;
			}

			IType* ParameterDescriptor::GetType()
			{
				return type;
			}

/***********************************************************************
MethodDescriptor
***********************************************************************/

			MethodDescriptor::MethodDescriptor(const WString& _name, MemberTypeEnum _memberTypeEnum)
				:name(_name)
				,memberTypeEnum(_memberTypeEnum)
				,returnType(0)
				,ownerTypeDescriptor(0)
			{
			}

			MethodDescriptor::~MethodDescriptor()
			{
			}

			MethodDescriptor* MethodDescriptor::ReturnType(Type* _returnType)
			{
				returnType=_returnType;
				return this;
			}

			MethodDescriptor* MethodDescriptor::Parameter(const WString& _name, Type* _type)
			{
				parameters.Add(new ParameterDescriptor(_name, _type));
				return this;
			}

			MethodDescriptor* MethodDescriptor::Handler(const HandlerFuncType& _handler)
			{
				handler=_handler;
				return this;
			}

			WString MethodDescriptor::GetName()
			{
				return name;
			}

			ITypeDescriptor* MethodDescriptor::GetOwnerTypeDescriptor()
			{
				return ownerTypeDescriptor;
			}

			IMemberDescriptor::MemberTypeEnum MethodDescriptor::GetMemberTypeEnum()
			{
				return memberTypeEnum;
			}

			IType* MethodDescriptor::GetReturnType()
			{
				return returnType;
			}

			int MethodDescriptor::GetParameterCount()
			{
				return parameters.Count();
			}

			IParameterDescriptor* MethodDescriptor::GetParameter(int index)
			{
				if(0<=index && index<parameters.Count())
				{
					return parameters[index].Obj();
				}
				else
				{
					return 0;
				}
			}

			DescriptableValue MethodDescriptor::Invoke(const DescriptableValue& thisObject, const collections::IReadonlyList<DescriptableValue>& parameters)
			{
				return handler(thisObject, parameters);
			}

/***********************************************************************
PropertyDescriptor
***********************************************************************/

			PropertyDescriptor::PropertyDescriptor(const WString& _name, MemberTypeEnum _memberTypeEnum)
				:name(_name)
				,memberTypeEnum(_memberTypeEnum)
				,propertyType(0)
				,ownerTypeDescriptor(0)
			{
			}

			PropertyDescriptor::~PropertyDescriptor()
			{
			}

			PropertyDescriptor* PropertyDescriptor::PropertyType(Type* _propertyType)
			{
				propertyType=_propertyType;
				return this;
			}

			PropertyDescriptor* PropertyDescriptor::Getter(Ptr<MethodDescriptor> _getter)
			{
				getter=_getter;
				return this;
			}

			PropertyDescriptor* PropertyDescriptor::Setter(Ptr<MethodDescriptor> _setter)
			{
				setter=_setter;
				return this;
			}

			WString PropertyDescriptor::GetName()
			{
				return name;
			}

			ITypeDescriptor* PropertyDescriptor::GetOwnerTypeDescriptor()
			{
				return ownerTypeDescriptor;
			}

			IMemberDescriptor::MemberTypeEnum PropertyDescriptor::GetMemberTypeEnum()
			{
				return memberTypeEnum;
			}

			bool PropertyDescriptor::CanGet()
			{
				return getter;
			}

			bool PropertyDescriptor::CanSet()
			{
				return setter;
			}

			IType* PropertyDescriptor::GetPropertyType()
			{
				return propertyType;
			}

			IMethodDescriptor* PropertyDescriptor::GetSetter()
			{
				return getter.Obj();
			}

			IMethodDescriptor* PropertyDescriptor::GetGetter()
			{
				return setter.Obj();
			}

/***********************************************************************
TypeDescriptor
***********************************************************************/

			void TypeDescriptor::EnsureContentAvailable()
			{
				if(!contentAvailable)
				{
					contentAvailable=true;
					FillTypeContent();
					for(int i=0;i<constructorList.Count();i++)
					{
						Ptr<MethodDescriptor> method=constructorList[i];
						method->ownerTypeDescriptor=this;
						if(method->GetParameterCount()==0)
						{
							defaultConstructor=method.Obj();
						}
					}
					for(int i=0;i<methodList.Count();i++)
					{
						Ptr<MethodDescriptor> method=methodList[i];
						method->ownerTypeDescriptor=this;
					}
					for(int i=0;i<propertyMap.Count();i++)
					{
						Ptr<PropertyDescriptor> prop=propertyMap.Values()[i];
						prop->ownerTypeDescriptor=this;
						if(prop->getter) prop->getter->ownerTypeDescriptor=this;
						if(prop->setter) prop->setter->ownerTypeDescriptor=this;
					}
				}
			}

			void TypeDescriptor::AddBaseType(Type* value)
			{
				baseTypeList.Add(value);
			}

			void TypeDescriptor::AddConstructor(Ptr<MethodDescriptor> value)
			{
				constructorList.Add(value);
			}

			void TypeDescriptor::AddMethod(Ptr<MethodDescriptor> value)
			{
				methodList.Add(value);
			}

			void TypeDescriptor::AddProperty(Ptr<PropertyDescriptor> value)
			{
				propertyMap.Set(value->GetName(), value);
			}

			TypeDescriptor::TypeDescriptor()
				:type(0)
				,contentAvailable(false)
				,defaultConstructor(0)
			{
			}

			TypeDescriptor::~TypeDescriptor()
			{
			}

			IType* TypeDescriptor::GetType()
			{
				return type;
			}

			int TypeDescriptor::GetBaseTypeCount()
			{
				EnsureContentAvailable();
				return baseTypeList.Count();
			}

			IType* TypeDescriptor::GetBaseType(int index)
			{
				EnsureContentAvailable();
				if(0<=index && index<baseTypeList.Count())
				{
					return baseTypeList[index];
				}
				return 0;
			}

			int TypeDescriptor::GetConstructorCount()
			{
				EnsureContentAvailable();
				return constructorList.Count();
			}

			IMethodDescriptor* TypeDescriptor::GetConstructor(int index)
			{
				EnsureContentAvailable();
				if(0<=index && index<constructorList.Count())
				{
					return constructorList[index].Obj();
				}
				return 0;
			}

			IMethodDescriptor* TypeDescriptor::GetDefaultConstructor()
			{
				EnsureContentAvailable();
				return defaultConstructor;
			}

			int TypeDescriptor::GetMethodCount()
			{
				EnsureContentAvailable();
				return methodList.Count();
			}

			IMethodDescriptor* TypeDescriptor::GetMethod(int index)
			{
				EnsureContentAvailable();
				if(0<=index && index<methodList.Count())
				{
					return methodList[index].Obj();
				}
				return 0;
			}

			int TypeDescriptor::GetPropertyCount()
			{
				EnsureContentAvailable();
				return propertyMap.Count();
			}

			IPropertyDescriptor* TypeDescriptor::GetProperty(int index)
			{
				EnsureContentAvailable();
				if(0<=index && index<propertyMap.Count())
				{
					return propertyMap.Values()[index].Obj();
				}
				return 0;
			}

			IMethodDescriptor* TypeDescriptor::FindMethod(const WString& name, bool searchParent, bool searchStatic, bool searchInstance)
			{
				List<IMethodDescriptor*> methods;
				FindMethods(name, searchParent, searchStatic, searchInstance, methods);
				return methods.Count()==1?methods[0]:0;
			}

			void TypeDescriptor::FindMethods(const WString& name, bool searchParent, bool searchStatic, bool searchInstance, collections::List<IMethodDescriptor*>& methods)
			{
				EnsureContentAvailable();
				for(int i=0;i<baseTypeList.Count();i++)
				{
					baseTypeList[i]->GetTypeDescriptor()->FindMethods(name, searchParent, searchStatic, searchInstance, methods);
				}

				for(int i=0;i<methodList.Count();i++)
				{
					MethodDescriptor* method=methodList[i].Obj();
					if(method->GetName()==name)
					{
						if(searchStatic && method->GetMemberTypeEnum()==IMemberDescriptor::Static)
						{
							methods.Add(method);
						}
						else if(searchInstance && method->GetMemberTypeEnum()!=IMemberDescriptor::Static)
						{
							methods.Add(method);
						}
					}
				}
			}

			IPropertyDescriptor* TypeDescriptor::FindProperty(const WString& name, bool searchParent, bool searchStatic, bool searchInstance)
			{
				EnsureContentAvailable();
				int index=propertyMap.Keys().IndexOf(name);
				if(index!=-1)
				{
					PropertyDescriptor* prop=propertyMap.Values()[index].Obj();
					if(searchStatic && prop->GetMemberTypeEnum()==IMemberDescriptor::Static)
					{
						return prop;
					}
					else if(searchInstance && prop->GetMemberTypeEnum()!=IMemberDescriptor::Static)
					{
						return prop;
					}
				}
				if(searchParent)
				{
					for(int i=0;i<baseTypeList.Count();i++)
					{
						IPropertyDescriptor* prop=baseTypeList[i]->GetTypeDescriptor()->FindProperty(name, searchParent, searchStatic, searchInstance);
						if(prop) return prop;
					}
				}
				return 0;
			}

/***********************************************************************
TypeProvider
***********************************************************************/

			TypeProvider::TypeProvider()
			{
				typeSInt8=CreateType(IType::SInt8, L"signed __int8", 0);
				typeSInt16=CreateType(IType::SInt16, L"signed __int16", 0);
				typeSInt32=CreateType(IType::SInt32, L"signed __int32", 0);
				typeSInt64=CreateType(IType::SInt64, L"signed __int64", 0);
				
				typeUInt8=CreateType(IType::UInt8, L"unsigned __int8", 0);
				typeUInt16=CreateType(IType::UInt16, L"unsigned __int16", 0);
				typeUInt32=CreateType(IType::UInt32, L"unsigned __int32", 0);
				typeUInt64=CreateType(IType::UInt64, L"unsigned __int64", 0);
				
				typeFloat=CreateType(IType::Float, L"float", 0);
				typeDouble=CreateType(IType::Double, L"double", 0);
				typeBool=CreateType(IType::Bool, L"bool", 0);
				typeChar=CreateType(IType::Char, L"wchar_t", 0);
				typeString=CreateType(IType::String, L"wstring", 0);
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
					namedTypes.Add(_name, type);
					createdTypeDescriptors.Add(_typeDescriptor);
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

			IType* TypeProvider::Double()
			{
				return typeDouble;
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

			IType* TypeProvider::ConstReference(IType* elementType)
			{
				Type* type=dynamic_cast<Type*>(elementType);
				if(type)
				{
					return type->GetConstReferenceType();
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