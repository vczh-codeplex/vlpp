/***********************************************************************
Vczh Library++ 3.0
Developer: ³Âè÷å«(vczh)
ObjectModel::ObjectType for numeric

Classes:
***********************************************************************/

#ifndef VCZH_OBJECTMODEL_OBJECTTYPENUMERIC
#define VCZH_OBJECTMODEL_OBJECTTYPENUMERIC

#include "ObjectType.h"

namespace vl
{
	namespace objectmodel
	{
		template<typename T>
		class ObjectTypeNumeric : public SpecifiedObjectType<T>
		{
			friend class ObjectTypeRegister<T>;
		protected:
			static void* ctor()
			{
				return new T(0);
			}

			static void* ctor(T value)
			{
				return new T(value);
			}

			collections::List<ObjectMember*>	constructors;
		public:
			~ObjectTypeNumeric()
			{
				for(int i=0;i<constructors.Count();i++)
				{
					delete constructors[i];
				}
			}

			ObjectCategory Category()const
			{
				return ObjectType::Numeric;
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
				delete (T*)instance;
			}
		};

#define DEFINE_NUMERIC_OBJECT_TYPE(TYPE)\
		template<>\
		class ObjectTypeInformation<TYPE>\
		{\
		public:\
			static WString GetName()\
			{\
				return L#TYPE;\
			}\
			typedef ObjectTypeNumeric<TYPE> ObjectTypeImplementation;\
		};\
		template<>\
		class ObjectTypeRegister<TYPE>\
		{\
		public:\
			static void Register()\
			{\
				ObjectTypeNumeric<TYPE>* type=new ObjectTypeNumeric<TYPE>;\
				RegisterObjectType(ObjectTypeInformation<TYPE>::GetName(), type);\
				type->constructors.Add(new ObjectTypeNumeric<TYPE>::Constructor<void*(*)()>(&ObjectTypeNumeric<TYPE>::ctor));\
				type->constructors.Add(new ObjectTypeNumeric<TYPE>::Constructor<void*(*)(TYPE)>(&ObjectTypeNumeric<TYPE>::ctor));\
			}\
		}

		DEFINE_NUMERIC_OBJECT_TYPE(signed __int8);
		DEFINE_NUMERIC_OBJECT_TYPE(unsigned __int8);
		DEFINE_NUMERIC_OBJECT_TYPE(signed __int16);
		DEFINE_NUMERIC_OBJECT_TYPE(unsigned __int16);
		DEFINE_NUMERIC_OBJECT_TYPE(signed __int32);
		DEFINE_NUMERIC_OBJECT_TYPE(unsigned __int32);
		DEFINE_NUMERIC_OBJECT_TYPE(signed __int64);
		DEFINE_NUMERIC_OBJECT_TYPE(unsigned __int64);
		DEFINE_NUMERIC_OBJECT_TYPE(char);
		DEFINE_NUMERIC_OBJECT_TYPE(wchar_t);
		DEFINE_NUMERIC_OBJECT_TYPE(bool);
		DEFINE_NUMERIC_OBJECT_TYPE(float);
		DEFINE_NUMERIC_OBJECT_TYPE(double);

#undef DEFINE_NUMERIC_OBJECT_TYPE
	}
}

#endif