/***********************************************************************
Vczh Library++ 3.0
Developer: ³Âè÷å«(vczh)
ObjectModel::ObjectType for enumeration type

Classes:
***********************************************************************/

#ifndef VCZH_OBJECTMODEL_OBJECTTYPEENUM
#define VCZH_OBJECTMODEL_OBJECTTYPEENUM

#include "ObjectType.h"
#include "..\Collections\Dictionary.h"

namespace vl
{
	namespace objectmodel
	{
		template<typename T>
		class ObjectTypeEnum : public SpecifiedObjectType<T>
		{
			friend class ObjectTypeRegister<T>;
		protected:

			class Member : public ObjectMember
			{
			protected:
				WString				name;
				vint					value;
				ObjectType*			type;
			public:
				Member(const WString& _name, vint _value, ObjectType* _type)
					:name(_name)
					,value(_value)
					,type(_type)
				{
				}

				const ObjectType* OwnerType()const
				{
					return type;
				}

				const ObjectType* Type()const
				{
					return type;
				}

				WString Name()const
				{
					return name;
				}

				vint EnumValue()const
				{
					return value;
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
					return 0;
				}
			};

			static void* ctor()
			{
				return new T((T)0);
			}

			static void* ctor(const T& value)
			{
				return new T(value);
			}

			static void* ctor(vint value)
			{
				return new T((T)value);
			}

			collections::Dictionary<WString, ObjectMember*>	enumMembers;
			collections::List<ObjectMember*>				constructors;
		public:

			ObjectTypeEnum()
			{
			}

			~ObjectTypeEnum()
			{
				for(vint i=0;i<enumMembers.Count();i++)
				{
					delete enumMembers.Values()[i];
				}
				for(vint i=0;i<constructors.Count();i++)
				{
					delete constructors[i];
				}
			}

			ObjectCategory Category()const
			{
				return ObjectType::Enum;
			}

			const EnumMemberMap& EnumMembers()const
			{
				return enumMembers.Wrap();
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
	}
}

#define BEGIN_ENUM_TYPE(NAME)\
namespace vl\
{\
	namespace objectmodel\
	{\
		template<>\
		class ObjectTypeInformation<NAME>\
		{\
		public:\
			static WString GetName()\
			{\
				return L#NAME;\
			}\
			typedef ObjectTypeEnum<NAME> ObjectTypeImplementation;\
		};\
		template<>\
		class ObjectTypeRegister<NAME>\
		{\
		private:\
			typedef NAME __enum_type__;\
		public:\
			static void Register()\
			{\
				ObjectTypeEnum<__enum_type__>* __object_type_enum__ = new ObjectTypeEnum<__enum_type__>;\
				RegisterObjectType(ObjectTypeInformation<__enum_type__>::GetName(), __object_type_enum__);\
				__object_type_enum__->constructors.Add(new ObjectTypeEnum<__enum_type__>::Constructor<void*(*)()>(&ObjectTypeEnum<__enum_type__>::ctor));\
				__object_type_enum__->constructors.Add(new ObjectTypeEnum<__enum_type__>::Constructor<void*(*)(const NAME&)>(&ObjectTypeEnum<__enum_type__>::ctor));\
				__object_type_enum__->constructors.Add(new ObjectTypeEnum<__enum_type__>::Constructor<void*(*)(vint)>(&ObjectTypeEnum<__enum_type__>::ctor));\
				ObjectMember* member=0;\

#define ENUM_MEMBER(NAME)\
				member=new ObjectTypeEnum<__enum_type__>::Member(L#NAME, (vint)NAME, __object_type_enum__);\
				__object_type_enum__->enumMembers.Add(L#NAME, member);\

#define END_ENUM_TYPE\
			}\
		};\
	}\
}\

#endif