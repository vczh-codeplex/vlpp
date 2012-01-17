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

			friend class DescriptableValue;
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
DescriptableValue
***********************************************************************/

		class IType;

		class DescriptableValue : public Object
		{
		protected:
			IType*								type;
			signed __int64						dataSInt;
			unsigned __int64					dataUInt;
			double								dataFloat;
			bool								dataBool;
			wchar_t								dataChar;
			WString								dataString;
			Ptr<DescriptableObject>				dataDescriptableObject;
			DescriptableObject*					dataDescriptableObjectPointer;
			Ptr<Object>							dataObject;
			Object*								dataObjectPointer;
			Ptr<Interface>						dataInterface;
			Interface*							dataInterfacePointer;
			void*								dataRawPointer;

			void								Initialize();
		public:
			static DescriptableValue			Null;

			DescriptableValue();									// null
			DescriptableValue(IType* type, signed __int64 value);	// enum
			DescriptableValue(signed __int64 value);				// SInt
			DescriptableValue(unsigned __int64 value);				// UInt
			DescriptableValue(double value);						// Float
			DescriptableValue(bool value);							// Bool
			DescriptableValue(wchar_t value);						// Char
			DescriptableValue(const WString& value);				// String
			DescriptableValue(Ptr<DescriptableObject> value);		// descriptable object
			DescriptableValue(DescriptableObject* value);			// descriptable object pointer
			DescriptableValue(IType* _type, Ptr<Object> value);		// object, ObjectBox-ed structure
			DescriptableValue(IType* _type, Object* value);			// object pointer
			DescriptableValue(IType* _type, Ptr<Interface> value);	// interface
			DescriptableValue(IType* _type, Interface* value);		// interface pointer
			DescriptableValue(IType* _type, void* value);			// pointer, pointer/reference to struct
			DescriptableValue(const DescriptableValue& value);		// copy

			IType*								GetType()const;
			signed __int64						GetSInt()const;
			unsigned __int64					GetUInt()const;
			double								GetFloat()const;
			bool								GetBool()const;
			wchar_t								GetChar()const;
			WString								GetString()const;
			
			Ptr<DescriptableObject>				GetDescriptableObject()const;
			DescriptableObject*					GetDescriptableObjectPoitner()const;
			Ptr<Object>							GetRawObject()const;
			Object*								GetRawObjectPointer()const;
			Ptr<Interface>						GetInterface()const;
			Interface*							GetInterfacePointer()const;
			void*								GetRawPointer()const;

			bool								CanDelete()const;
			void								Delete();
		};

/***********************************************************************
ITypeDescriptor
***********************************************************************/

		class IType : public virtual Interface
		{
		public:
			enum TypeEnum
			{
				SInt8,
				UInt8,
				SInt16,
				UInt16,
				SInt32,
				UInt32,
				SInt64,
				UInt64,
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

		class IPropertyDescriptor : public virtual IMemberDescriptor
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
			virtual void						FindMethods(const WString& name, bool searchParent, bool searchStatic, bool searchInstance, collections::List<IMethodDescriptor*>& methods)=0;
			virtual IPropertyDescriptor*		FindProperty(const WString& name, bool searchParent, bool searchStatic, bool searchInstance)=0;
		};

/***********************************************************************
Helper Functions
***********************************************************************/

		class ITypeProvider : public virtual Interface
		{
		public:
			virtual IType*			SInt8()=0;
			virtual IType*			UInt8()=0;
			virtual IType*			SInt16()=0;
			virtual IType*			UInt16()=0;
			virtual IType*			SInt32()=0;
			virtual IType*			UInt32()=0;
			virtual IType*			SInt64()=0;
			virtual IType*			UInt64()=0;
			virtual IType*			Float()=0;
			virtual IType*			Bool()=0;
			virtual IType*			Char()=0;
			virtual IType*			String()=0;
			virtual IType*			Pointer(IType* elementType)=0;
			virtual IType*			SmartPointer(IType* elementType)=0;
			virtual IType*			Reference(IType* elementType)=0;
			virtual IType*			RValueReference(IType* elementType)=0;
			virtual IType*			Array(IType* elementType, int length)=0;

			virtual int				GetDescriptableTypeCount()=0;
			virtual IType*			GetDescriptableType(int index)=0;
			virtual IType*			FindType(const WString& name)=0;
		};

		extern ITypeProvider*		GetTypeProvider();
		extern void					SetTypeProvider(ITypeProvider* value);
	}
}

#endif