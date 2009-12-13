/***********************************************************************
Vczh Library++ 3.0
Developer: 陈梓瀚(vczh)
ObjectModel::ObjectType for class template

Classes:

本文件使用Vczh Functional Macro工具自动生成
***********************************************************************/
#ifndef VCZH_OBJECTMODEL_OBJECTTYPECLASSTEMPLATE
#define VCZH_OBJECTMODEL_OBJECTTYPECLASSTEMPLATE
#include "ObjectTypeClass.h"
namespace vl
{
	namespace objectmodel
	{
		template<typename T>
		class ObjectTypeClassTemplate : public ObjectTypeClass<T>
		{
			friend class ObjectTypeRegister<T>;
		protected:
			collections::List<ObjectType*>			typenames;
		public:
			ObjectTypeClassTemplate()
			{
			}
			~ObjectTypeClassTemplate()
			{
			}
			bool IsTemplateClass()const
			{
				return true;
			}
			WString TemplateName()const
			{
				return ObjectTypeInformation<T>::GetTemplateName();
			}
			const TypeList& Typenames()const
			{
				return typenames.Wrap();
			}
		};
	}
}
 
#define BEGIN_CLASS_TEMPLATE_1(NAME)\
namespace vl\
{\
	namespace objectmodel\
	{\
		template<typename T0>\
		class ObjectTypeInformation<NAME<T0>>\
		{\
		public:\
			static WString GetTemplateName()\
			{\
				return L#NAME;\
			}\
			static WString GetName()\
			{\
				return GetTemplateName()+L"<"+ObjectTypeInformation<T0>::GetName()+L">";\
			}\
			typedef ObjectTypeClassTemplate<NAME<T0>> ObjectTypeImplementation;\
		};\
		template<typename T0>\
		class ObjectTypeRegister<NAME<T0>>\
		{\
		private:\
			typedef NAME<T0> __class_type__;\
		public:\
			static void Register()\
			{\
				ObjectTypeClassTemplate<__class_type__>* __object_type_class__ = new ObjectTypeClassTemplate<__class_type__>;\
				RegisterObjectType(ObjectTypeInformation<__class_type__>::GetName(), __object_type_class__);\
				\
				__object_type_class__->typenames.Add(TypeOf<T0>());\
				ObjectMember* field=0;\
				ObjectMember* method=0;\
 
  
#define BEGIN_CLASS_TEMPLATE_2(NAME)\
namespace vl\
{\
	namespace objectmodel\
	{\
		template<typename T0,typename T1>\
		class ObjectTypeInformation<NAME<T0,T1>>\
		{\
		public:\
			static WString GetTemplateName()\
			{\
				return L#NAME;\
			}\
			static WString GetName()\
			{\
				return GetTemplateName()+L"<"+ObjectTypeInformation<T0>::GetName()+L","+ObjectTypeInformation<T1>::GetName()+L">";\
			}\
			typedef ObjectTypeClassTemplate<NAME<T0,T1>> ObjectTypeImplementation;\
		};\
		template<typename T0,typename T1>\
		class ObjectTypeRegister<NAME<T0,T1>>\
		{\
		private:\
			typedef NAME<T0,T1> __class_type__;\
		public:\
			static void Register()\
			{\
				ObjectTypeClassTemplate<__class_type__>* __object_type_class__ = new ObjectTypeClassTemplate<__class_type__>;\
				RegisterObjectType(ObjectTypeInformation<__class_type__>::GetName(), __object_type_class__);\
				\
				__object_type_class__->typenames.Add(TypeOf<T0>());\
				__object_type_class__->typenames.Add(TypeOf<T1>());\
				ObjectMember* field=0;\
				ObjectMember* method=0;\
 
  
#define BEGIN_CLASS_TEMPLATE_3(NAME)\
namespace vl\
{\
	namespace objectmodel\
	{\
		template<typename T0,typename T1,typename T2>\
		class ObjectTypeInformation<NAME<T0,T1,T2>>\
		{\
		public:\
			static WString GetTemplateName()\
			{\
				return L#NAME;\
			}\
			static WString GetName()\
			{\
				return GetTemplateName()+L"<"+ObjectTypeInformation<T0>::GetName()+L","+ObjectTypeInformation<T1>::GetName()+L","+ObjectTypeInformation<T2>::GetName()+L">";\
			}\
			typedef ObjectTypeClassTemplate<NAME<T0,T1,T2>> ObjectTypeImplementation;\
		};\
		template<typename T0,typename T1,typename T2>\
		class ObjectTypeRegister<NAME<T0,T1,T2>>\
		{\
		private:\
			typedef NAME<T0,T1,T2> __class_type__;\
		public:\
			static void Register()\
			{\
				ObjectTypeClassTemplate<__class_type__>* __object_type_class__ = new ObjectTypeClassTemplate<__class_type__>;\
				RegisterObjectType(ObjectTypeInformation<__class_type__>::GetName(), __object_type_class__);\
				\
				__object_type_class__->typenames.Add(TypeOf<T0>());\
				__object_type_class__->typenames.Add(TypeOf<T1>());\
				__object_type_class__->typenames.Add(TypeOf<T2>());\
				ObjectMember* field=0;\
				ObjectMember* method=0;\
 
  
#define BEGIN_CLASS_TEMPLATE_4(NAME)\
namespace vl\
{\
	namespace objectmodel\
	{\
		template<typename T0,typename T1,typename T2,typename T3>\
		class ObjectTypeInformation<NAME<T0,T1,T2,T3>>\
		{\
		public:\
			static WString GetTemplateName()\
			{\
				return L#NAME;\
			}\
			static WString GetName()\
			{\
				return GetTemplateName()+L"<"+ObjectTypeInformation<T0>::GetName()+L","+ObjectTypeInformation<T1>::GetName()+L","+ObjectTypeInformation<T2>::GetName()+L","+ObjectTypeInformation<T3>::GetName()+L">";\
			}\
			typedef ObjectTypeClassTemplate<NAME<T0,T1,T2,T3>> ObjectTypeImplementation;\
		};\
		template<typename T0,typename T1,typename T2,typename T3>\
		class ObjectTypeRegister<NAME<T0,T1,T2,T3>>\
		{\
		private:\
			typedef NAME<T0,T1,T2,T3> __class_type__;\
		public:\
			static void Register()\
			{\
				ObjectTypeClassTemplate<__class_type__>* __object_type_class__ = new ObjectTypeClassTemplate<__class_type__>;\
				RegisterObjectType(ObjectTypeInformation<__class_type__>::GetName(), __object_type_class__);\
				\
				__object_type_class__->typenames.Add(TypeOf<T0>());\
				__object_type_class__->typenames.Add(TypeOf<T1>());\
				__object_type_class__->typenames.Add(TypeOf<T2>());\
				__object_type_class__->typenames.Add(TypeOf<T3>());\
				ObjectMember* field=0;\
				ObjectMember* method=0;\
 
  
#define BEGIN_CLASS_TEMPLATE_5(NAME)\
namespace vl\
{\
	namespace objectmodel\
	{\
		template<typename T0,typename T1,typename T2,typename T3,typename T4>\
		class ObjectTypeInformation<NAME<T0,T1,T2,T3,T4>>\
		{\
		public:\
			static WString GetTemplateName()\
			{\
				return L#NAME;\
			}\
			static WString GetName()\
			{\
				return GetTemplateName()+L"<"+ObjectTypeInformation<T0>::GetName()+L","+ObjectTypeInformation<T1>::GetName()+L","+ObjectTypeInformation<T2>::GetName()+L","+ObjectTypeInformation<T3>::GetName()+L","+ObjectTypeInformation<T4>::GetName()+L">";\
			}\
			typedef ObjectTypeClassTemplate<NAME<T0,T1,T2,T3,T4>> ObjectTypeImplementation;\
		};\
		template<typename T0,typename T1,typename T2,typename T3,typename T4>\
		class ObjectTypeRegister<NAME<T0,T1,T2,T3,T4>>\
		{\
		private:\
			typedef NAME<T0,T1,T2,T3,T4> __class_type__;\
		public:\
			static void Register()\
			{\
				ObjectTypeClassTemplate<__class_type__>* __object_type_class__ = new ObjectTypeClassTemplate<__class_type__>;\
				RegisterObjectType(ObjectTypeInformation<__class_type__>::GetName(), __object_type_class__);\
				\
				__object_type_class__->typenames.Add(TypeOf<T0>());\
				__object_type_class__->typenames.Add(TypeOf<T1>());\
				__object_type_class__->typenames.Add(TypeOf<T2>());\
				__object_type_class__->typenames.Add(TypeOf<T3>());\
				__object_type_class__->typenames.Add(TypeOf<T4>());\
				ObjectMember* field=0;\
				ObjectMember* method=0;\
 
  
#define BEGIN_CLASS_TEMPLATE_6(NAME)\
namespace vl\
{\
	namespace objectmodel\
	{\
		template<typename T0,typename T1,typename T2,typename T3,typename T4,typename T5>\
		class ObjectTypeInformation<NAME<T0,T1,T2,T3,T4,T5>>\
		{\
		public:\
			static WString GetTemplateName()\
			{\
				return L#NAME;\
			}\
			static WString GetName()\
			{\
				return GetTemplateName()+L"<"+ObjectTypeInformation<T0>::GetName()+L","+ObjectTypeInformation<T1>::GetName()+L","+ObjectTypeInformation<T2>::GetName()+L","+ObjectTypeInformation<T3>::GetName()+L","+ObjectTypeInformation<T4>::GetName()+L","+ObjectTypeInformation<T5>::GetName()+L">";\
			}\
			typedef ObjectTypeClassTemplate<NAME<T0,T1,T2,T3,T4,T5>> ObjectTypeImplementation;\
		};\
		template<typename T0,typename T1,typename T2,typename T3,typename T4,typename T5>\
		class ObjectTypeRegister<NAME<T0,T1,T2,T3,T4,T5>>\
		{\
		private:\
			typedef NAME<T0,T1,T2,T3,T4,T5> __class_type__;\
		public:\
			static void Register()\
			{\
				ObjectTypeClassTemplate<__class_type__>* __object_type_class__ = new ObjectTypeClassTemplate<__class_type__>;\
				RegisterObjectType(ObjectTypeInformation<__class_type__>::GetName(), __object_type_class__);\
				\
				__object_type_class__->typenames.Add(TypeOf<T0>());\
				__object_type_class__->typenames.Add(TypeOf<T1>());\
				__object_type_class__->typenames.Add(TypeOf<T2>());\
				__object_type_class__->typenames.Add(TypeOf<T3>());\
				__object_type_class__->typenames.Add(TypeOf<T4>());\
				__object_type_class__->typenames.Add(TypeOf<T5>());\
				ObjectMember* field=0;\
				ObjectMember* method=0;\
 
  
#define BEGIN_CLASS_TEMPLATE_7(NAME)\
namespace vl\
{\
	namespace objectmodel\
	{\
		template<typename T0,typename T1,typename T2,typename T3,typename T4,typename T5,typename T6>\
		class ObjectTypeInformation<NAME<T0,T1,T2,T3,T4,T5,T6>>\
		{\
		public:\
			static WString GetTemplateName()\
			{\
				return L#NAME;\
			}\
			static WString GetName()\
			{\
				return GetTemplateName()+L"<"+ObjectTypeInformation<T0>::GetName()+L","+ObjectTypeInformation<T1>::GetName()+L","+ObjectTypeInformation<T2>::GetName()+L","+ObjectTypeInformation<T3>::GetName()+L","+ObjectTypeInformation<T4>::GetName()+L","+ObjectTypeInformation<T5>::GetName()+L","+ObjectTypeInformation<T6>::GetName()+L">";\
			}\
			typedef ObjectTypeClassTemplate<NAME<T0,T1,T2,T3,T4,T5,T6>> ObjectTypeImplementation;\
		};\
		template<typename T0,typename T1,typename T2,typename T3,typename T4,typename T5,typename T6>\
		class ObjectTypeRegister<NAME<T0,T1,T2,T3,T4,T5,T6>>\
		{\
		private:\
			typedef NAME<T0,T1,T2,T3,T4,T5,T6> __class_type__;\
		public:\
			static void Register()\
			{\
				ObjectTypeClassTemplate<__class_type__>* __object_type_class__ = new ObjectTypeClassTemplate<__class_type__>;\
				RegisterObjectType(ObjectTypeInformation<__class_type__>::GetName(), __object_type_class__);\
				\
				__object_type_class__->typenames.Add(TypeOf<T0>());\
				__object_type_class__->typenames.Add(TypeOf<T1>());\
				__object_type_class__->typenames.Add(TypeOf<T2>());\
				__object_type_class__->typenames.Add(TypeOf<T3>());\
				__object_type_class__->typenames.Add(TypeOf<T4>());\
				__object_type_class__->typenames.Add(TypeOf<T5>());\
				__object_type_class__->typenames.Add(TypeOf<T6>());\
				ObjectMember* field=0;\
				ObjectMember* method=0;\
 
  
#define BEGIN_CLASS_TEMPLATE_8(NAME)\
namespace vl\
{\
	namespace objectmodel\
	{\
		template<typename T0,typename T1,typename T2,typename T3,typename T4,typename T5,typename T6,typename T7>\
		class ObjectTypeInformation<NAME<T0,T1,T2,T3,T4,T5,T6,T7>>\
		{\
		public:\
			static WString GetTemplateName()\
			{\
				return L#NAME;\
			}\
			static WString GetName()\
			{\
				return GetTemplateName()+L"<"+ObjectTypeInformation<T0>::GetName()+L","+ObjectTypeInformation<T1>::GetName()+L","+ObjectTypeInformation<T2>::GetName()+L","+ObjectTypeInformation<T3>::GetName()+L","+ObjectTypeInformation<T4>::GetName()+L","+ObjectTypeInformation<T5>::GetName()+L","+ObjectTypeInformation<T6>::GetName()+L","+ObjectTypeInformation<T7>::GetName()+L">";\
			}\
			typedef ObjectTypeClassTemplate<NAME<T0,T1,T2,T3,T4,T5,T6,T7>> ObjectTypeImplementation;\
		};\
		template<typename T0,typename T1,typename T2,typename T3,typename T4,typename T5,typename T6,typename T7>\
		class ObjectTypeRegister<NAME<T0,T1,T2,T3,T4,T5,T6,T7>>\
		{\
		private:\
			typedef NAME<T0,T1,T2,T3,T4,T5,T6,T7> __class_type__;\
		public:\
			static void Register()\
			{\
				ObjectTypeClassTemplate<__class_type__>* __object_type_class__ = new ObjectTypeClassTemplate<__class_type__>;\
				RegisterObjectType(ObjectTypeInformation<__class_type__>::GetName(), __object_type_class__);\
				\
				__object_type_class__->typenames.Add(TypeOf<T0>());\
				__object_type_class__->typenames.Add(TypeOf<T1>());\
				__object_type_class__->typenames.Add(TypeOf<T2>());\
				__object_type_class__->typenames.Add(TypeOf<T3>());\
				__object_type_class__->typenames.Add(TypeOf<T4>());\
				__object_type_class__->typenames.Add(TypeOf<T5>());\
				__object_type_class__->typenames.Add(TypeOf<T6>());\
				__object_type_class__->typenames.Add(TypeOf<T7>());\
				ObjectMember* field=0;\
				ObjectMember* method=0;\
 
  
#define BEGIN_CLASS_TEMPLATE_9(NAME)\
namespace vl\
{\
	namespace objectmodel\
	{\
		template<typename T0,typename T1,typename T2,typename T3,typename T4,typename T5,typename T6,typename T7,typename T8>\
		class ObjectTypeInformation<NAME<T0,T1,T2,T3,T4,T5,T6,T7,T8>>\
		{\
		public:\
			static WString GetTemplateName()\
			{\
				return L#NAME;\
			}\
			static WString GetName()\
			{\
				return GetTemplateName()+L"<"+ObjectTypeInformation<T0>::GetName()+L","+ObjectTypeInformation<T1>::GetName()+L","+ObjectTypeInformation<T2>::GetName()+L","+ObjectTypeInformation<T3>::GetName()+L","+ObjectTypeInformation<T4>::GetName()+L","+ObjectTypeInformation<T5>::GetName()+L","+ObjectTypeInformation<T6>::GetName()+L","+ObjectTypeInformation<T7>::GetName()+L","+ObjectTypeInformation<T8>::GetName()+L">";\
			}\
			typedef ObjectTypeClassTemplate<NAME<T0,T1,T2,T3,T4,T5,T6,T7,T8>> ObjectTypeImplementation;\
		};\
		template<typename T0,typename T1,typename T2,typename T3,typename T4,typename T5,typename T6,typename T7,typename T8>\
		class ObjectTypeRegister<NAME<T0,T1,T2,T3,T4,T5,T6,T7,T8>>\
		{\
		private:\
			typedef NAME<T0,T1,T2,T3,T4,T5,T6,T7,T8> __class_type__;\
		public:\
			static void Register()\
			{\
				ObjectTypeClassTemplate<__class_type__>* __object_type_class__ = new ObjectTypeClassTemplate<__class_type__>;\
				RegisterObjectType(ObjectTypeInformation<__class_type__>::GetName(), __object_type_class__);\
				\
				__object_type_class__->typenames.Add(TypeOf<T0>());\
				__object_type_class__->typenames.Add(TypeOf<T1>());\
				__object_type_class__->typenames.Add(TypeOf<T2>());\
				__object_type_class__->typenames.Add(TypeOf<T3>());\
				__object_type_class__->typenames.Add(TypeOf<T4>());\
				__object_type_class__->typenames.Add(TypeOf<T5>());\
				__object_type_class__->typenames.Add(TypeOf<T6>());\
				__object_type_class__->typenames.Add(TypeOf<T7>());\
				__object_type_class__->typenames.Add(TypeOf<T8>());\
				ObjectMember* field=0;\
				ObjectMember* method=0;\
 
  
#define BEGIN_CLASS_TEMPLATE_10(NAME)\
namespace vl\
{\
	namespace objectmodel\
	{\
		template<typename T0,typename T1,typename T2,typename T3,typename T4,typename T5,typename T6,typename T7,typename T8,typename T9>\
		class ObjectTypeInformation<NAME<T0,T1,T2,T3,T4,T5,T6,T7,T8,T9>>\
		{\
		public:\
			static WString GetTemplateName()\
			{\
				return L#NAME;\
			}\
			static WString GetName()\
			{\
				return GetTemplateName()+L"<"+ObjectTypeInformation<T0>::GetName()+L","+ObjectTypeInformation<T1>::GetName()+L","+ObjectTypeInformation<T2>::GetName()+L","+ObjectTypeInformation<T3>::GetName()+L","+ObjectTypeInformation<T4>::GetName()+L","+ObjectTypeInformation<T5>::GetName()+L","+ObjectTypeInformation<T6>::GetName()+L","+ObjectTypeInformation<T7>::GetName()+L","+ObjectTypeInformation<T8>::GetName()+L","+ObjectTypeInformation<T9>::GetName()+L">";\
			}\
			typedef ObjectTypeClassTemplate<NAME<T0,T1,T2,T3,T4,T5,T6,T7,T8,T9>> ObjectTypeImplementation;\
		};\
		template<typename T0,typename T1,typename T2,typename T3,typename T4,typename T5,typename T6,typename T7,typename T8,typename T9>\
		class ObjectTypeRegister<NAME<T0,T1,T2,T3,T4,T5,T6,T7,T8,T9>>\
		{\
		private:\
			typedef NAME<T0,T1,T2,T3,T4,T5,T6,T7,T8,T9> __class_type__;\
		public:\
			static void Register()\
			{\
				ObjectTypeClassTemplate<__class_type__>* __object_type_class__ = new ObjectTypeClassTemplate<__class_type__>;\
				RegisterObjectType(ObjectTypeInformation<__class_type__>::GetName(), __object_type_class__);\
				\
				__object_type_class__->typenames.Add(TypeOf<T0>());\
				__object_type_class__->typenames.Add(TypeOf<T1>());\
				__object_type_class__->typenames.Add(TypeOf<T2>());\
				__object_type_class__->typenames.Add(TypeOf<T3>());\
				__object_type_class__->typenames.Add(TypeOf<T4>());\
				__object_type_class__->typenames.Add(TypeOf<T5>());\
				__object_type_class__->typenames.Add(TypeOf<T6>());\
				__object_type_class__->typenames.Add(TypeOf<T7>());\
				__object_type_class__->typenames.Add(TypeOf<T8>());\
				__object_type_class__->typenames.Add(TypeOf<T9>());\
				ObjectMember* field=0;\
				ObjectMember* method=0;\
 
 
#endif