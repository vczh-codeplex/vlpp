/***********************************************************************
Vczh Library++ 3.0
Developer: ³Âè÷å«(vczh)
ObjectModel::ObjectType for const/volatile decorator

Classes:
***********************************************************************/

#ifndef VCZH_OBJECTMODEL_OBJECTTYPEDECORATOR
#define VCZH_OBJECTMODEL_OBJECTTYPEDECORATOR

#include "ObjectType.h"

namespace vl
{
	namespace objectmodel
	{
		template<typename T>
		class ObjectTypeDecorator : public SpecifiedObjectType<T>
		{
		};

/***********************************************************************
const
***********************************************************************/

		template<typename T>
		class ObjectTypeDecorator<const T> : public SpecifiedObjectType<const T>
		{
		protected:
			ObjectType*							elementType;
		public:
			ObjectTypeDecorator()
			{
				elementType=TypeOf<T>();
			}

			ObjectCategory Category()const
			{
				return ObjectType::Decorator;
			}

			bool IsConstDecorator()const
			{
				return true;
			}

			const ObjectType* ElementType()const
			{
				return elementType;
			}

			const ConstructorList& Constructors()const
			{
				return elementType->Constructors();
			}

			bool CanCreateInstance()const
			{
				return elementType->CanCreateInstance();
			}

			void* CreateInstance()const
			{
				return elementType->CreateInstance();
			}

			void DestroyInstance(void* instance)const
			{
				elementType->DestroyInstance(instance);
			}
		};

/***********************************************************************
volatile
***********************************************************************/

		template<typename T>
		class ObjectTypeDecorator<volatile T> : public SpecifiedObjectType<volatile T>
		{
		protected:
			ObjectType*							elementType;
		public:
			ObjectTypeDecorator()
			{
				elementType=TypeOf<T>();
			}

			~ObjectTypeDecorator()
			{
			}

			ObjectCategory Category()const
			{
				return ObjectType::Decorator;
			}

			bool IsVolatileDecorator()const
			{
				return true;
			}

			const ObjectType* ElementType()const
			{
				return elementType;
			}

			const ConstructorList& Constructors()const
			{
				return elementType->Constructors();
			}

			bool CanCreateInstance()const
			{
				return elementType->CanCreateInstance();
			}

			void* CreateInstance()const
			{
				return elementType->CreateInstance();
			}

			void DestroyInstance(void* instance)const
			{
				elementType->DestroyInstance(instance);
			}
		};

/***********************************************************************
const volatile
***********************************************************************/

		template<typename T>
		class ObjectTypeDecorator<const volatile T> : public SpecifiedObjectType<const volatile T>
		{
		protected:
			ObjectType*							elementType;
		public:
			ObjectTypeDecorator()
			{
				elementType=TypeOf<T>();
			}

			ObjectCategory Category()const
			{
				return ObjectType::Decorator;
			}

			bool IsConstDecorator()const
			{
				return true;
			}

			bool IsVolatileDecorator()const
			{
				return true;
			}

			const ObjectType* ElementType()const
			{
				return elementType;
			}

			const ConstructorList& Constructors()const
			{
				return elementType->Constructors();
			}

			bool CanCreateInstance()const
			{
				return elementType->CanCreateInstance();
			}

			void* CreateInstance()const
			{
				return elementType->CreateInstance();
			}

			void DestroyInstance(void* instance)const
			{
				elementType->DestroyInstance(instance);
			}
		};

/***********************************************************************
ObjectTypeInformation
***********************************************************************/

		template<typename T>
		class ObjectTypeInformation<const T>
		{
		public:
			static WString GetName()
			{
				return L"const("+ObjectTypeInformation<T>::GetName()+L")";
			}

			typedef ObjectTypeDecorator<const T> ObjectTypeImplementation;
		};

		template<typename T>
		class ObjectTypeInformation<volatile T>
		{
		public:
			static WString GetName()
			{
				return L"volatile("+ObjectTypeInformation<T>::GetName()+L")";
			}

			typedef ObjectTypeDecorator<volatile T> ObjectTypeImplementation;
		};

		template<typename T>
		class ObjectTypeInformation<const volatile T>
		{
		public:
			static WString GetName()
			{
				return L"const-volatile("+ObjectTypeInformation<T>::GetName()+L")";
			}

			typedef ObjectTypeDecorator<const volatile T> ObjectTypeImplementation;
		};
	}
}

#endif