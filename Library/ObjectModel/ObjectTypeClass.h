/***********************************************************************
Vczh Library++ 3.0
Developer: ³Âè÷å«(vczh)
ObjectModel::ObjectType for class

Classes:
***********************************************************************/

#ifndef VCZH_OBJECTMODEL_OBJECTTYPECLASS
#define VCZH_OBJECTMODEL_OBJECTTYPECLASS

#include "ObjectType.h"
#include "ObjectTypeFunction.h"
#include "..\Collections\Dictionary.h"

namespace vl
{
	namespace objectmodel
	{
		template<typename T>
		class ObjectTypeClass : public SpecifiedObjectType<T>
		{
			friend class ObjectTypeRegister<T>;
		protected:
			collections::List<ObjectType*>						baseClasses;
			collections::Dictionary<WString, ObjectMember*>		fields;
			collections::Group<WString, ObjectMember*>			methods;
			collections::List<void*(*)(void*)>					upCasters;
			collections::List<void*(*)(void*)>					downCasters;
			collections::List<ObjectMember*>					constructors;

			template<typename T, typename S>
			static void* Caster(void* source)
			{
				return dynamic_cast<T*>((S*)source);
			}

			template<typename F>
			class Field : public ObjectMember
			{
			protected:
				F T::*											offset;
				ObjectType*										ownerType;
				ObjectType*										type;
				WString											name;
			public:
				Field(F T::* _offset, const WString& _name)
					:offset(_offset)
					,name(_name)
					,type(TypeOf<F>())
					,ownerType(TypeOf<T>())
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
					return name;
				}

				int EnumValue()const
				{
					return 0;
				}

				void* GetValue(void* instance)const
				{
					return &(((T*)instance)->*offset);
				}

				void* Invoke(void* object, void** arguments)const
				{
					return 0;
				}
			};

			template<typename F>
			class Method : public ObjectMember
			{
			protected:
				F												function;
				ObjectType*										ownerType;
				ObjectType*										type;
				WString											name;
			public:
				Method(F _function, const WString& _name)
					:function(_function)
					,name(_name)
					,type(TypeOf<F>())
					,ownerType(TypeOf<T>())
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
					return name;
				}

				int EnumValue()const
				{
					return 0;
				}

				void* GetValue(void* instance)const
				{
					return 0;
				}

				void* Invoke(void* object, void** arguments)const
				{
					return type->Invoke(object, (void*)&function, arguments);
				}
			};

		public:
			template<typename F>
			static ObjectMember* GetField(F T::* offset, const WString& name)
			{
				return new Field<F>(offset, name);
			}

			template<typename F>
			static ObjectMember* GetMethod(F offset, const WString& name)
			{
				return new Method<F>(offset, name);
			}
		public:
			ObjectTypeClass()
			{
			}

			~ObjectTypeClass()
			{
				for(int i=0;i<fields.Count();i++)
				{
					delete fields.Values()[i];
				}
				for(int i=0;i<methods.Keys().Count();i++)
				{
					const IReadonlyList<ObjectMember*>& members=methods.GetByIndex(i);
					for(int j=0;j<members.Count();j++)
					{
						delete members[j];
					}
				}
				for(int i=0;i<constructors.Count();i++)
				{
					delete constructors[i];
				}
			}

			ObjectCategory Category()const
			{
				return ObjectType::Class;
			}

			const TypeList& BaseClasses()const
			{
				return baseClasses.Wrap();
			}

			const FieldMap& Fields()const
			{
				return fields.Wrap();
			}

			const MethodGroup& Methods()const
			{
				return methods.Wrap();
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
				return new T;
			}

			void DestroyInstance(void* instance)const
			{
				delete (T*)instance;
			}

			void* CastToBaseClass(int baseClassIndex, void* instance)const
			{
				return upCasters.Get(baseClassIndex)(instance);
			}

			void* CastToSubClass(int baseClassIndex, void* instance)const
			{
				return downCasters.Get(baseClassIndex)(instance);
			}
		};
	}
}

#define BEGIN_CLASS_TYPE(NAME)\
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
			typedef ObjectTypeClass<NAME> ObjectTypeImplementation;\
		};\
		template<>\
		class ObjectTypeRegister<NAME>\
		{\
		private:\
			typedef NAME __class_type__;\
		public:\
			static void Register()\
			{\
				ObjectTypeClass<__class_type__>* __object_type_class__ = new ObjectTypeClass<__class_type__>;\
				RegisterObjectType(ObjectTypeInformation<__class_type__>::GetName(), __object_type_class__);\
				ObjectMember* field=0;\
				ObjectMember* method=0;\

#define CLASS_BASE(NAME)\
				__object_type_class__->baseClasses.Add(TypeOf<NAME>());\
				__object_type_class__->upCasters.Add(&ObjectTypeClass<__class_type__>::Caster<NAME, __class_type__>);\
				__object_type_class__->downCasters.Add(&ObjectTypeClass<__class_type__>::Caster<__class_type__, NAME>);\

#define CLASS_FIELD(NAME)\
				field=ObjectTypeClass<__class_type__>::GetField(&__class_type__::NAME, L#NAME);\
				__object_type_class__->fields.Add(L#NAME, field);\

#define CLASS_METHOD(NAME)\
				method=ObjectTypeClass<__class_type__>::GetMethod(&__class_type__::NAME, L#NAME);\
				__object_type_class__->methods.Add(L#NAME, method);\

#define CLASS_METHOD_OVERLOAD(NAME,TYPE)\
				method=ObjectTypeClass<__class_type__>::GetMethod<TYPE>(&__class_type__::NAME, L#NAME);\
				__object_type_class__->methods.Add(L#NAME, method);\

#define END_CLASS_TYPE\
			}\
		};\
	}\
}\

#endif