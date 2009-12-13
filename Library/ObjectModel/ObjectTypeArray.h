/***********************************************************************
Vczh Library++ 3.0
Developer: ³Âè÷å«(vczh)
ObjectModel::ObjectType for array

Classes:
***********************************************************************/

#ifndef VCZH_OBJECTMODEL_OBJECTTYPEARRAY
#define VCZH_OBJECTMODEL_OBJECTTYPEARRAY

#include "ObjectType.h"

namespace vl
{
	namespace objectmodel
	{
		template<typename T, int _Size>
		class ObjectTypeArray : public SpecifiedObjectType<T[_Size]>
		{
			friend class ObjectTypeRegister<T[_Size]>;
		protected:
			ObjectType*							elementType;
			collections::List<ObjectMember*>	constructors;

			static void* ctor()
			{
				return new T[_Size];
			}
		public:
			ObjectTypeArray()
			{
				elementType=TypeOf<T>();
			}

			~ObjectTypeArray()
			{
				for(int i=0;i<constructors.Count();i++)
				{
					delete constructors[i];
				}
			}

			ObjectCategory Category()const
			{
				return ObjectType::Array;
			}

			const ObjectType* ElementType()const
			{
				return elementType;
			}

			int ArraySize()const
			{
				return _Size;
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
				delete[] (T*)instance;
			}
		};

		template<typename T, int _Size>
		class ObjectTypeInformation<T[_Size]>
		{
		public:
			static WString GetName()
			{
				return ObjectTypeInformation<T>::GetName()+L"["+itow(_Size)+L"]";
			}

			typedef ObjectTypeArray<T, _Size> ObjectTypeImplementation;
		};

		template<typename T, int _Size>
		class ObjectTypeRegister<T[_Size]>
		{
		public:
			static void Register()
			{
				ObjectTypeArray<T, _Size>* type=new ObjectTypeArray<T, _Size>();
				RegisterObjectType(ObjectTypeInformation<T[_Size]>::GetName(), type);

				type->constructors.Add(new ObjectTypeArray<T, _Size>::Constructor<void*(*)()>(&ObjectTypeArray<T, _Size>::ctor));
			}
		};
	}
}

#endif