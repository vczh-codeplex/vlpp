/***********************************************************************
Vczh Library++ 3.0
Developer: ³Âè÷å«(vczh)
GacUI::Reflection

Interfaces:
***********************************************************************/

#ifndef VCZH_PRESENTATION_REFLECTION_GUITYPEDESCRIPTOR
#define VCZH_PRESENTATION_REFLECTION_GUITYPEDESCRIPTOR

#include "..\NativeWindow\GuiTypes.h"

namespace vl
{
	namespace presentation
	{

/***********************************************************************
Attribute
***********************************************************************/

		class ITypeDescriptor;

		class DescriptableObject
		{
			template<typename T>
			friend class Description;
		protected:
			size_t							objectSize;
			ITypeDescriptor**				typeDescriptor;
		public:
			DescriptableObject();
			virtual ~DescriptableObject();
		};

		class IDescriptable : public virtual Interface, public virtual DescriptableObject
		{
		public:
			~IDescriptable(){}
		};
		
		template<typename T>
		class Description : public virtual DescriptableObject
		{
			template<typename T>
			friend class DescriptionAssigner;
		protected:
			static ITypeDescriptor*			associatedTypeDescriptor;
		public:
			Description()
			{
				if(objectSize<sizeof(T))
				{
					objectSize=sizeof(T);
					typeDescriptor=&associatedTypeDescriptor;
				}
			}
		};

		template<typename T>
		ITypeDescriptor* Description<T>::associatedTypeDescriptor=0;

/***********************************************************************
ITypeDescriptor
***********************************************************************/

		class IType : public virtual Interface
		{
		public:
			enum TypeEnum
			{
				SInt,
				UInt,
				Float,
				Bool,
				Char,
				String,
				Enum,
				Struct,
				Class,

				Pointer,
				SmartPointer,
				Reference,
				RValueReference,
				Array,
				Const,
				Volatile,
				FunctionPointer,
			};

			virtual TypeEnum					GetTypeEnum()=0;
			virtual WString						GetName()=0;
			virtual ITypeDescriptor*			GetTypeDescriptor()=0;
			virtual bool						CanConvertTo(IType* targetType)=0;

			virtual IType*						GetElementType()=0;
			virtual int							GetArrayLength()=0;
			virtual IType*						GetReturnType()=0;
			virtual int							GetParameterCount()=0;
			virtual IType*						GetParameterType(int index)=0;
		};

		class DescriptableValue : public Object
		{
		public:
			static DescriptableValue			Null;

			DescriptableValue();
			DescriptableValue(signed __int64 value);
			DescriptableValue(unsigned __int64 value);
			DescriptableValue(double value);
			DescriptableValue(bool value);
			DescriptableValue(wchar_t value);
			DescriptableValue(const WString& value);
			DescriptableValue(DescriptableObject* value);
			DescriptableValue(Ptr<DescriptableObject> value);
			DescriptableValue(IType* type, Ptr<Object> data);
			DescriptableValue(const DescriptableValue& value);

			IType*								GetType()const;
			signed __int64						GetSInt()const;
			unsigned __int64					GetUInt()const;
			double								GetFloat()const;
			bool								GetBool()const;
			wchar_t								GetChar()const;
			WString								GetString()const;
			DescriptableObject*					GetObject()const;
			Ptr<Object>							GetData()const;

			DescriptableValue					Convert(IType* targetType)const;
			bool								NeedDelete()const;
			void								Delete();
		};

		class IMemberDescriptor : public virtual Interface
		{
		public:
			enum MemberTypeEnum
			{
				Static,
				Abstract,
				Virtual,
				Normal,
			};

			virtual WString						GetName()=0;
			virtual ITypeDescriptor*			GetOwnerTypeDescriptor()=0;
			virtual MemberTypeEnum				GetMemberTypeEnum()=0;
		};

		class IParameterDescriptor : public virtual Interface
		{
		public:
			virtual WString						GetName()=0;
			virtual IType*						GetType()=0;
		};

		class IMethodDescriptor : public virtual IMemberDescriptor
		{
		public:
			virtual IType*						GetReturnType()=0;
			virtual int							GetParameterCount()=0;
			virtual IParameterDescriptor*		GetParameter(int index)=0;
			virtual DescriptableValue			Invoke(const DescriptableValue& thisObject, const collections::IReadonlyList<DescriptableValue>& parameters)=0;
		}; 

		class IPropertyDescriptor : public virtual Interface
		{
		public:
			virtual bool						CanGet()=0;
			virtual bool						CanSet()=0;
			virtual IType*						GetPropertyType()=0;
			virtual IMethodDescriptor*			GetSetter()=0;
			virtual IMethodDescriptor*			GetGetter()=0;
		};

		class ITypeDescriptor : public virtual Interface
		{
		public:
			virtual IType*						GetType()=0;
			virtual int							GetBaseTypeCount()=0;
			virtual IType*						GetBaseType(int index)=0;

			virtual int							GetConstructorCount()=0;
			virtual IMethodDescriptor*			GetConstructor(int index)=0;
			virtual IMethodDescriptor*			GetDefaultConstructor()=0;
			virtual int							GetMethodCount()=0;
			virtual IMethodDescriptor*			GetMethod(int index)=0;
			virtual int							GetPropertyCount()=0;
			virtual IPropertyDescriptor*		GetProperty(int index)=0;

			virtual IMethodDescriptor*			FindMethod(const WString& name, bool searchParent, bool searchStatic, bool searchInstance)=0;
			virtual void						FindMethods(const WString& name, bool searchParent, bool searchStatic, bool searchInstance, collections::Array<IMethodDescriptor*>& methods)=0;
			virtual IPropertyDescriptor*		FindProperty(const WString& name, bool searchParent, bool searchStatic, bool searchInstance)=0;
		};

/***********************************************************************
Helper Functions
***********************************************************************/

		class ITypeProvider : public virtual Interface
		{
		public:
			virtual IType*			SInt()=0;
			virtual IType*			UInt()=0;
			virtual IType*			Float()=0;
			virtual IType*			Bool()=0;
			virtual IType*			Char()=0;
			virtual IType*			String()=0;
			virtual IType*			Pointer(IType* elementType)=0;
			virtual IType*			SmartPointer(IType* elementType)=0;
			virtual IType*			Reference(IType* elementType)=0;
			virtual IType*			RValueReference(IType* elementType)=0;
			virtual IType*			Array(IType* element, int length)=0;

			virtual int				GetDescriptableTypeCount()=0;
			virtual IType*			GetDescriptableType(int index)=0;
			virtual IType*			FindType(const WString& name)=0;
		};

		extern void					InitializeReflection();
		extern void					FinalizeReflection();
		extern ITypeProvider*		GetTypeProvider();
	}
}

#endif