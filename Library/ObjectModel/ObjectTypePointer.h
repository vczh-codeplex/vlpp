/***********************************************************************
Vczh Library++ 3.0
Developer: ³Âè÷å«(vczh)
ObjectModel::ObjectType for pointer

Classes:
***********************************************************************/

#ifndef VCZH_OBJECTMODEL_OBJECTTYPEPOINTER
#define VCZH_OBJECTMODEL_OBJECTTYPEPOINTER

#include "ObjectType.h"

namespace vl
{
	namespace objectmodel
	{
		template<typename T>
		class ObjectTypePointer : public SpecifiedObjectType<T*>
		{
			friend class ObjectTypeRegister<T*>;
		protected:
			static void* ctor()
			{
				return new T*(0);
			}

			static void*ctor(T* value)
			{
				return new T*(value);
			}

			ObjectType*							elementType;
			collections::List<ObjectMember*>	constructors;
		public:
			ObjectTypePointer()
			{
				elementType=TypeOf<T>();
			}

			~ObjectTypePointer()
			{
				for(vint i=0;i<constructors.Count();i++)
				{
					delete constructors[i];
				}
			}

			ObjectCategory Category()const
			{
				return ObjectType::Pointer;
			}

			const ObjectType* ElementType()const
			{
				return elementType;
			}

			const ConstructorList& Constructors()const
			{
				return constructors.Wrap();
			}

			bool CanCreateInstance()const
			{
				return true;
			}

			void* CreateInstance()const
			{
				return ctor();
			}

			void DestroyInstance(void* instance)const
			{
				delete (T**)instance;
			}
		};

		template<typename T>
		class ObjectTypeInformation<T*>
		{
		public:
			static WString GetName()
			{
				return ObjectTypeInformation<T>::GetName()+L"*";
			}

			typedef ObjectTypePointer<T> ObjectTypeImplementation;
		};

		template<typename T>
		class ObjectTypeRegister<T*>
		{
		public:
			static void Register()
			{
				ObjectTypePointer<T>* type=new ObjectTypePointer<T>;
				RegisterObjectType(ObjectTypeInformation<T*>::GetName(), type);

				type->constructors.Add(new ObjectTypePointer<T>::Constructor<void*(*)()>(&ObjectTypePointer<T>::ctor));
				type->constructors.Add(new ObjectTypePointer<T>::Constructor<void*(*)(T*)>(&ObjectTypePointer<T>::ctor));
			}
		};
	}
}

#endif