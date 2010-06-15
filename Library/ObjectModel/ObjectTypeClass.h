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
			ObjectMember*										defaultConstructor;
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
				vint EnumValue()const
				{
					return 0;
				}
				void* GetValue(void* instance)const
				{
					return &(((T*)instance)->*offset);
				}
				void SetValue(void* instance, void* value)const
				{
					(((T*)instance)->*offset)=*(F*)value;
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
					return type->Invoke(object, (void*)&function, arguments);
				}
			};
		protected:
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
			static void* Constructor0 ()
			{
				return new T();
			}
 
			template<typename T0>
			static void* Constructor1 (T0 p0)
			{
				return new T(p0);
			} 
			template<typename T0,typename T1>
			static void* Constructor2 (T0 p0,T1 p1)
			{
				return new T(p0,p1);
			} 
			template<typename T0,typename T1,typename T2>
			static void* Constructor3 (T0 p0,T1 p1,T2 p2)
			{
				return new T(p0,p1,p2);
			} 
			template<typename T0,typename T1,typename T2,typename T3>
			static void* Constructor4 (T0 p0,T1 p1,T2 p2,T3 p3)
			{
				return new T(p0,p1,p2,p3);
			} 
			template<typename T0,typename T1,typename T2,typename T3,typename T4>
			static void* Constructor5 (T0 p0,T1 p1,T2 p2,T3 p3,T4 p4)
			{
				return new T(p0,p1,p2,p3,p4);
			} 
			template<typename T0,typename T1,typename T2,typename T3,typename T4,typename T5>
			static void* Constructor6 (T0 p0,T1 p1,T2 p2,T3 p3,T4 p4,T5 p5)
			{
				return new T(p0,p1,p2,p3,p4,p5);
			} 
			template<typename T0,typename T1,typename T2,typename T3,typename T4,typename T5,typename T6>
			static void* Constructor7 (T0 p0,T1 p1,T2 p2,T3 p3,T4 p4,T5 p5,T6 p6)
			{
				return new T(p0,p1,p2,p3,p4,p5,p6);
			} 
			template<typename T0,typename T1,typename T2,typename T3,typename T4,typename T5,typename T6,typename T7>
			static void* Constructor8 (T0 p0,T1 p1,T2 p2,T3 p3,T4 p4,T5 p5,T6 p6,T7 p7)
			{
				return new T(p0,p1,p2,p3,p4,p5,p6,p7);
			} 
			template<typename T0,typename T1,typename T2,typename T3,typename T4,typename T5,typename T6,typename T7,typename T8>
			static void* Constructor9 (T0 p0,T1 p1,T2 p2,T3 p3,T4 p4,T5 p5,T6 p6,T7 p7,T8 p8)
			{
				return new T(p0,p1,p2,p3,p4,p5,p6,p7,p8);
			} 
			template<typename T0,typename T1,typename T2,typename T3,typename T4,typename T5,typename T6,typename T7,typename T8,typename T9>
			static void* Constructor10 (T0 p0,T1 p1,T2 p2,T3 p3,T4 p4,T5 p5,T6 p6,T7 p7,T8 p8,T9 p9)
			{
				return new T(p0,p1,p2,p3,p4,p5,p6,p7,p8,p9);
			}
		public:
			ObjectTypeClass()
				:defaultConstructor(0)
			{
			}
			~ObjectTypeClass()
			{
				for(vint i=0;i<fields.Count();i++)
				{
					delete fields.Values()[i];
				}
				for(vint i=0;i<methods.Keys().Count();i++)
				{
					const IReadonlyList<ObjectMember*>& members=methods.GetByIndex(i);
					for(vint j=0;j<members.Count();j++)
					{
						delete members[j];
					}
				}
				for(vint i=0;i<constructors.Count();i++)
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
				return defaultConstructor!=0;
			}
			void* CreateInstance()const
			{
				return defaultConstructor==0?0:defaultConstructor->Invoke(0,0);
			}
			void DestroyInstance(void* instance)const
			{
				delete (T*)instance;
			}
			void* CastToBaseClass(vint baseClassIndex, void* instance)const
			{
				return upCasters.Get(baseClassIndex)(instance);
			}
			void* CastToSubClass(vint baseClassIndex, void* instance)const
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
				ObjectMember* constructor=0;\
 
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
 
#define CLASS_CONSTRUCTOR_0\
				constructor=new ObjectTypeClass<__class_type__>::Constructor<void*(*)()>(&ObjectTypeClass<__class_type__>::Constructor0);\
				__object_type_class__->constructors.Add(constructor);\
				__object_type_class__->defaultConstructor=constructor;\
 
 
#define CLASS_CONSTRUCTOR_1(T0)\
				constructor=new ObjectTypeClass<__class_type__>::Constructor<void*(*)(T0)>(&ObjectTypeClass<__class_type__>::Constructor1<T0>);\
				__object_type_class__->constructors.Add(constructor);\
  
#define CLASS_CONSTRUCTOR_2(T0,T1)\
				constructor=new ObjectTypeClass<__class_type__>::Constructor<void*(*)(T0,T1)>(&ObjectTypeClass<__class_type__>::Constructor2<T0,T1>);\
				__object_type_class__->constructors.Add(constructor);\
  
#define CLASS_CONSTRUCTOR_3(T0,T1,T2)\
				constructor=new ObjectTypeClass<__class_type__>::Constructor<void*(*)(T0,T1,T2)>(&ObjectTypeClass<__class_type__>::Constructor3<T0,T1,T2>);\
				__object_type_class__->constructors.Add(constructor);\
  
#define CLASS_CONSTRUCTOR_4(T0,T1,T2,T3)\
				constructor=new ObjectTypeClass<__class_type__>::Constructor<void*(*)(T0,T1,T2,T3)>(&ObjectTypeClass<__class_type__>::Constructor4<T0,T1,T2,T3>);\
				__object_type_class__->constructors.Add(constructor);\
  
#define CLASS_CONSTRUCTOR_5(T0,T1,T2,T3,T4)\
				constructor=new ObjectTypeClass<__class_type__>::Constructor<void*(*)(T0,T1,T2,T3,T4)>(&ObjectTypeClass<__class_type__>::Constructor5<T0,T1,T2,T3,T4>);\
				__object_type_class__->constructors.Add(constructor);\
  
#define CLASS_CONSTRUCTOR_6(T0,T1,T2,T3,T4,T5)\
				constructor=new ObjectTypeClass<__class_type__>::Constructor<void*(*)(T0,T1,T2,T3,T4,T5)>(&ObjectTypeClass<__class_type__>::Constructor6<T0,T1,T2,T3,T4,T5>);\
				__object_type_class__->constructors.Add(constructor);\
  
#define CLASS_CONSTRUCTOR_7(T0,T1,T2,T3,T4,T5,T6)\
				constructor=new ObjectTypeClass<__class_type__>::Constructor<void*(*)(T0,T1,T2,T3,T4,T5,T6)>(&ObjectTypeClass<__class_type__>::Constructor7<T0,T1,T2,T3,T4,T5,T6>);\
				__object_type_class__->constructors.Add(constructor);\
  
#define CLASS_CONSTRUCTOR_8(T0,T1,T2,T3,T4,T5,T6,T7)\
				constructor=new ObjectTypeClass<__class_type__>::Constructor<void*(*)(T0,T1,T2,T3,T4,T5,T6,T7)>(&ObjectTypeClass<__class_type__>::Constructor8<T0,T1,T2,T3,T4,T5,T6,T7>);\
				__object_type_class__->constructors.Add(constructor);\
  
#define CLASS_CONSTRUCTOR_9(T0,T1,T2,T3,T4,T5,T6,T7,T8)\
				constructor=new ObjectTypeClass<__class_type__>::Constructor<void*(*)(T0,T1,T2,T3,T4,T5,T6,T7,T8)>(&ObjectTypeClass<__class_type__>::Constructor9<T0,T1,T2,T3,T4,T5,T6,T7,T8>);\
				__object_type_class__->constructors.Add(constructor);\
  
#define CLASS_CONSTRUCTOR_10(T0,T1,T2,T3,T4,T5,T6,T7,T8,T9)\
				constructor=new ObjectTypeClass<__class_type__>::Constructor<void*(*)(T0,T1,T2,T3,T4,T5,T6,T7,T8,T9)>(&ObjectTypeClass<__class_type__>::Constructor10<T0,T1,T2,T3,T4,T5,T6,T7,T8,T9>);\
				__object_type_class__->constructors.Add(constructor);\
 
 
#define END_CLASS_TYPE\
			}\
		};\
	}\
}\
 
#endif