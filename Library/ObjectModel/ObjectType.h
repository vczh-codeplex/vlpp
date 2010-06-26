/***********************************************************************
Vczh Library++ 3.0
Developer: 陈梓瀚(vczh)
ObjectModel::ObjectType

Classes:
***********************************************************************/

#ifndef VCZH_OBJECTMODEL_OBJECTTYPE
#define VCZH_OBJECTMODEL_OBJECTTYPE

#include "..\String.h"
#include "..\Pointer.h"
#include "..\Collections\Interfaces.h"
#include "..\Collections\List.h"

namespace vl
{
	namespace objectmodel
	{
/***********************************************************************
元对象
***********************************************************************/

		class ObjectMember;
		class ObjectType;

		class ObjectValue : public Object
		{
		protected:
			class InternalValue
			{
			public:
				const ObjectType*					objectType;
				void*								objectValue;
				bool								needToDestroy;

				InternalValue(const ObjectType* _objectType);
				InternalValue(const ObjectType* _objectType, void* _objectValue, bool _needToDestroy);
				~InternalValue();
			};

			Ptr<InternalValue>						internalValue;
			const ObjectType*						objectType;
			void*									objectValue;

			bool									InvokeMethod(const ObjectType* type, void* object, const WString& name, const collections::IReadonlyList<ObjectValue>& arguments, ObjectValue& result)const;
		public:
			ObjectValue();
			ObjectValue(const ObjectValue& value);
			ObjectValue(const ObjectValue& value, const ObjectType* _objectType, void* _objectValue);
			ObjectValue(const ObjectType* _objectType);
			ObjectValue(const ObjectType* _objectType, void* _objectValue, bool needToDestroy);
			~ObjectValue();

			operator bool()const;
			bool									operator!()const;
			bool									operator==(const ObjectValue& objectValue)const;
			bool									operator!=(const ObjectValue& objectValue)const;
			bool									operator==(bool value)const;
			bool									operator!=(bool value)const;
			ObjectType*								GetType()const;
			void*									GetValue()const;

			ObjectValue								CastToBaseClass(const ObjectType* type)const;
			ObjectValue								CastToSubClass(const ObjectType* type)const;
			ObjectValue								CastToClass(const ObjectType* type)const;
			ObjectValue								ImplicitlyConvertTo(const ObjectType* type)const;
			ObjectValue								GetField(const WString& name)const;
			bool									SetField(const WString& name, const ObjectValue& value)const;
			bool									InvokeMethod(const WString& name, const collections::IReadonlyList<ObjectValue>& arguments, ObjectValue& result)const;
		};

		extern bool									FindBaseClass(const ObjectType* sub, const ObjectType* base, collections::List<vint>& baseToSubPositions, collections::List<const ObjectType*>& baseToSubTypes);
		extern ObjectMember*						FindClassField(const ObjectType* type, const WString& fieldName);
		extern ObjectType*							ObjectTypeRemoveCVR(const ObjectType* type);
		extern bool									InvokeMethod(const collections::IReadonlyList<ObjectMember*>& methods, void* object, const collections::IReadonlyList<ObjectValue>& arguments, ObjectValue& result, ObjectType* resultType=0);

/***********************************************************************
类型对象
***********************************************************************/

		class ObjectType : public Object, private NotCopyable
		{
		public:
			typedef collections::IReadonlyList<ObjectType*>						TypeList;
			typedef collections::IReadonlyDictionary<WString, ObjectMember*>	EnumMemberMap;
			typedef collections::IReadonlyDictionary<WString, ObjectMember*>	FieldMap;
			typedef collections::IReadonlyGroup<WString, ObjectMember*>			MethodGroup;
			typedef collections::IReadonlyList<ObjectMember*>					ConstructorList;

			enum ObjectCategory
			{
				Void,
				Decorator,
				Numeric,
				Pointer,
				Reference,
				Array,
				Enum,
				Function,
				Class
			};

			virtual vint								Size()const=0;
			virtual WString							Name()const=0;
			virtual ObjectCategory					Category()const=0;
			virtual bool							IsPOD()const=0;
			virtual bool							IsConstDecorator()const=0;
			virtual bool							IsVolatileDecorator()const=0;

			virtual const ObjectType*				ElementType()const=0;
			virtual vint								ArraySize()const=0;
			virtual const EnumMemberMap&			EnumMembers()const=0;
			virtual const TypeList&					BaseClasses()const=0;
			virtual const FieldMap&					Fields()const=0;
			virtual const MethodGroup&				Methods()const=0;
			
			virtual const ObjectType*				ReturnType()const=0;
			virtual const TypeList&					ParameterTypes()const=0;
			virtual const ObjectType*				ThisType()const=0;
			virtual bool							IsConstFunction()const=0;
			virtual void*							Invoke(void* object, void* funcPtrPtr, void** arguments)const=0;

			virtual bool							IsTemplateClass()const=0;
			virtual WString							TemplateName()const=0;
			virtual const TypeList&					Typenames()const=0;

			virtual const ConstructorList&			Constructors()const=0;
			virtual bool							CanCreateInstance()const=0;
			virtual void*							CreateInstance()const=0;
			virtual void							DestroyInstance(void* instance)const=0;
			virtual void*							CastToBaseClass(vint baseClassIndex, void* instance)const=0;
			virtual void*							CastToSubClass(vint baseClassIndex, void* instance)const=0;
		};

		class ObjectMember : public Object, private NotCopyable
		{
		public:
			virtual const ObjectType*				OwnerType()const=0;
			virtual const ObjectType*				Type()const=0;
			virtual WString							Name()const=0;
			virtual vint								EnumValue()const=0;
			virtual void*							GetValue(void* instance)const=0;
			virtual void							SetValue(void* instance, void* value)const=0;
			virtual void*							Invoke(void* object, void** arguments)const=0;
		};

/***********************************************************************
结构支撑
***********************************************************************/

		extern void									RegisterObjectType(const WString& name, ObjectType* type);
		extern ObjectType*							GetObjectType(const WString& name);
		extern bool									IsObjectTypeRegistered(const WString& name);
		extern void									EnsureObjectTypeInitialized();

		template<typename T>
		class ObjectTypeInformation
		{
		public:
			/*
			static WString GetName();
			typedef X<T> ObjectTypeImplementation;
			*/
		};

		template<typename T>
		class ObjectTypeRegister
		{
		public:
			static void Register()
			{
				RegisterObjectType(ObjectTypeInformation<T>::GetName(), new ObjectTypeInformation<T>::ObjectTypeImplementation);
			}
		};

		template<typename T>
		void RegisterObjectType()
		{
			ObjectTypeRegister<T>::Register();
		}

		template<typename T>
		ObjectType* TypeOf()
		{
			if(!IsObjectTypeRegistered(ObjectTypeInformation<T>::GetName()))
			{
				RegisterObjectType<T>();
			}
			return GetObjectType(ObjectTypeInformation<T>::GetName());
		}

		template<typename T>
		ObjectValue New()
		{
			return TypeOf<T>();
		}

		template<typename T>
		ObjectValue NewReference(T& value)
		{
			return ObjectValue(TypeOf<T>(), &value, false);
		}

		template<typename T>
		ObjectValue NewValue(const T& value)
		{
			return ObjectValue(TypeOf<T>(), new T(value), true);
		}

/***********************************************************************
缺省实现
***********************************************************************/

		template<typename T>
		class SpecifiedObjectType : public ObjectType
		{
		protected:
			template<typename F>
			class Constructor : public ObjectMember
			{
			protected:
				F						function;
				ObjectType*				ownerType;
				ObjectType*				type;
			public:
				Constructor(F _function)
					:function(_function)
					,ownerType(TypeOf<T>())
					,type(TypeOf<F>())
				{
				}

				const ObjectType* OwnerType()const
				{
					return ownerType;
				}

				const ObjectType* Type()const
				{
					return type;
				}

				WString Name()const
				{
					return L".ctor";
				}

				vint EnumValue()const
				{
					return 0;
				}

				void* GetValue(void* instance)const
				{
					return 0;
				}

				void SetValue(void* instance, void* value)const
				{
				}

				void* Invoke(void* object, void** arguments)const
				{
					void** result=(void**)type->Invoke(0, (void*)&function, arguments);
					if(result==0)
					{
						return 0;
					}
					else
					{
						void* realResult=*result;
						delete result;
						return realResult;
					}
				}
			};
		public:
			vint Size()const
			{
				return sizeof(T);
			}

			WString Name()const
			{
				return ObjectTypeInformation<T>::GetName();
			}

			bool IsPOD()const
			{
				return POD<T>::Result;
			}

			bool IsConstDecorator()const
			{
				return false;
			}

			bool IsVolatileDecorator()const
			{
				return false;
			}

			const ObjectType* ElementType()const
			{
				return 0;
			}

			vint ArraySize()const
			{
				return 0;
			}

			const EnumMemberMap& EnumMembers()const
			{
				return *(EnumMemberMap*)0;
			}

			const TypeList& BaseClasses()const
			{
				return *(TypeList*)0;
			}

			const FieldMap& Fields()const
			{
				return *(FieldMap*)0;
			}

			const MethodGroup& Methods()const
			{
				return *(MethodGroup*)0;
			}

			const ObjectType* ReturnType()const
			{
				return 0;
			}

			const TypeList& ParameterTypes()const
			{
				return *(TypeList*)0;
			}

			const ObjectType* ThisType()const
			{
				return 0;
			}

			bool IsConstFunction()const
			{
				return false;
			}

			void* Invoke(void* object, void* funcPtrPtr, void** arguments)const
			{
				return 0;
			}

			bool IsTemplateClass()const
			{
				return false;
			}

			WString TemplateName()const
			{
				return WString::Empty;
			}

			const TypeList&	 Typenames()const
			{
				return *(TypeList*)0;
			}

			const ConstructorList& Constructors()const
			{
				return *(ConstructorList*)0;
			}

			bool CanCreateInstance()const
			{
				return false;
			}

			void* CreateInstance()const
			{
				return 0;
			}

			void DestroyInstance(void* instance)const
			{
			}
			
			void* CastToBaseClass(vint baseClassIndex, void* instance)const
			{
				return 0;
			}

			void* CastToSubClass(vint baseClassIndex, void* instance)const
			{
				return 0;
			}
		};

/***********************************************************************
void支持
***********************************************************************/

		template<>
		class ObjectTypeInformation<void>
		{
		public:
			static WString GetName()
			{
				return L"void";
			}

			typedef SpecifiedObjectType<void> ObjectTypeImplementation;
		};

		template<>
		class SpecifiedObjectType<void> : public ObjectType
		{
		public:
			vint Size()const
			{
				return 0;
			}

			WString Name()const
			{
				return ObjectTypeInformation<void>::GetName();
			}

			ObjectCategory Category()const
			{
				return ObjectType::Void;
			}

			bool IsPOD()const
			{
				return true;
			}

			bool IsConstDecorator()const
			{
				return false;
			}

			bool IsVolatileDecorator()const
			{
				return false;
			}

			const ObjectType* ElementType()const
			{
				return 0;
			}

			vint ArraySize()const
			{
				return 0;
			}

			const EnumMemberMap& EnumMembers()const
			{
				return *(EnumMemberMap*)0;
			}

			const TypeList& BaseClasses()const
			{
				return *(TypeList*)0;
			}

			const FieldMap& Fields()const
			{
				return *(FieldMap*)0;
			}

			const MethodGroup& Methods()const
			{
				return *(MethodGroup*)0;
			}

			const ObjectType* ReturnType()const
			{
				return 0;
			}

			const TypeList& ParameterTypes()const
			{
				return *(TypeList*)0;
			}

			const ObjectType* ThisType()const
			{
				return 0;
			}

			bool IsConstFunction()const
			{
				return false;
			}

			void* Invoke(void* object, void* funcPtrPtr, void** arguments)const
			{
				return 0;
			}

			bool IsTemplateClass()const
			{
				return false;
			}

			WString TemplateName()const
			{
				return WString::Empty;
			}

			const TypeList&	 Typenames()const
			{
				return *(TypeList*)0;
			}

			const ConstructorList& Constructors()const
			{
				return *(ConstructorList*)0;
			}

			bool CanCreateInstance()const
			{
				return false;
			}

			void* CreateInstance()const
			{
				return 0;
			}

			void DestroyInstance(void* instance)const
			{
			}
			
			void* CastToBaseClass(vint baseClassIndex, void* instance)const
			{
				return 0;
			}

			void* CastToSubClass(vint baseClassIndex, void* instance)const
			{
				return 0;
			}
		};
	}
}

#endif