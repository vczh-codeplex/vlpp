/***********************************************************************
Vczh Library++ 3.0
Developer: 陈梓瀚(vczh)
ObjectModel::ObjectType for function

Classes:

本文件使用Vczh Functional Macro工具自动生成
***********************************************************************/
#ifndef VCZH_OBJECTMODEL_OBJECTTYPEFUNCTION
#define VCZH_OBJECTMODEL_OBJECTTYPEFUNCTION
#include "ObjectType.h"
namespace vl
{
	namespace objectmodel
	{
		template<typename T>
		class ObjectTypeFunction
		{
		public:
		};
		template<typename T>
		struct RemoveCVR
		{
			typedef T Type;
		};
		template<typename T>
		struct RemoveCVR<T&>
		{
			typedef typename RemoveCVR<T>::Type Type;
		};
		template<typename T>
		struct RemoveCVR<const T>
		{
			typedef typename RemoveCVR<T>::Type Type;
		};
		template<typename T>
		struct RemoveCVR<volatile T>
		{
			typedef typename RemoveCVR<T>::Type Type;
		};
		template<typename T>
		struct RemoveCVR<const volatile T>
		{
			typedef typename RemoveCVR<T>::Type Type;
		};
 
/***********************************************************************
vl::objectmodel::ObjectTypeFunction<R(*)() >
***********************************************************************/
		template<typename R >
		class ObjectTypeFunction<R(*)() > : public SpecifiedObjectType<R(*)() >
		{
			friend class ObjectTypeRegister<R(*)() >;
			typedef R(*FunctionPointer)() ;
		protected:
			static void* ctor()
			{
				return new FunctionPointer(0);
			}
			collections::List<ObjectType*>		parameterTypes;
			ObjectType*							returnType;
			ObjectType*							thisType;
			collections::List<ObjectMember*>	constructors;
		public:
			ObjectTypeFunction()
			{
				returnType=0;
				thisType=0;
			}
			~ObjectTypeFunction()
			{
				for(int i=0;i<constructors.Count();i++)
				{
					delete constructors[i];
				}
			}
			ObjectCategory Category()const
			{
				return ObjectType::Function;
			}
			const ObjectType* ReturnType()const
			{
				return returnType;
			}
			const TypeList& ParameterTypes()const
			{
				return parameterTypes.Wrap();
			}
			const ObjectType* ThisType()const
			{
				return thisType;
			}
			bool IsConstFunction()const
			{
				return false;
			}
			void* Invoke(void* object, void* funcPtrPtr, void** arguments)const
			{

				R result=(*((FunctionPointer*)funcPtrPtr))();
				return new R(result);
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
				delete (FunctionPointer*)instance;
			}
		};
		template<typename R >
		class ObjectTypeInformation<R(*)() >
		{
		public:
			static WString GetName()
			{
				return ObjectTypeInformation<R>::GetName()+L"(*)"+L"()" ;
			}
			typedef ObjectTypeFunction<R(*)() > ObjectTypeImplementation;
		};
		template<typename R >
		class ObjectTypeRegister<R(*)() >
		{
		public:
			static void Register()
			{
				ObjectTypeFunction<R(*)() >* type=new ObjectTypeFunction<R(*)() >;
				RegisterObjectType(ObjectTypeInformation<R(*)() >::GetName(), type);

				type->returnType=TypeOf<R>();
				type->thisType=0;
				type->constructors.Add(new ObjectTypeFunction<R(*)() >::Constructor<void*(*)()>(&ObjectTypeFunction<R(*)() >::ctor));
			}
		};
 
/***********************************************************************
vl::objectmodel::ObjectTypeFunction<void(*)() >
***********************************************************************/
		template< >
		class ObjectTypeFunction<void(*)() > : public SpecifiedObjectType<void(*)() >
		{
			friend class ObjectTypeRegister<void(*)() >;
			typedef void(*FunctionPointer)() ;
		protected:
			static void* ctor()
			{
				return new FunctionPointer(0);
			}
			collections::List<ObjectType*>		parameterTypes;
			ObjectType*							returnType;
			ObjectType*							thisType;
			collections::List<ObjectMember*>	constructors;
		public:
			ObjectTypeFunction()
			{
				returnType=0;
				thisType=0;
			}
			~ObjectTypeFunction()
			{
				for(int i=0;i<constructors.Count();i++)
				{
					delete constructors[i];
				}
			}
			ObjectCategory Category()const
			{
				return ObjectType::Function;
			}
			const ObjectType* ReturnType()const
			{
				return returnType;
			}
			const TypeList& ParameterTypes()const
			{
				return parameterTypes.Wrap();
			}
			const ObjectType* ThisType()const
			{
				return thisType;
			}
			bool IsConstFunction()const
			{
				return false;
			}
			void* Invoke(void* object, void* funcPtrPtr, void** arguments)const
			{

				 (*((FunctionPointer*)funcPtrPtr))();
				return 0;
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
				delete (FunctionPointer*)instance;
			}
		};
		template< >
		class ObjectTypeInformation<void(*)() >
		{
		public:
			static WString GetName()
			{
				return ObjectTypeInformation<void>::GetName()+L"(*)"+L"()" ;
			}
			typedef ObjectTypeFunction<void(*)() > ObjectTypeImplementation;
		};
		template< >
		class ObjectTypeRegister<void(*)() >
		{
		public:
			static void Register()
			{
				ObjectTypeFunction<void(*)() >* type=new ObjectTypeFunction<void(*)() >;
				RegisterObjectType(ObjectTypeInformation<void(*)() >::GetName(), type);

				type->returnType=TypeOf<void>();
				type->thisType=0;
				type->constructors.Add(new ObjectTypeFunction<void(*)() >::Constructor<void*(*)()>(&ObjectTypeFunction<void(*)() >::ctor));
			}
		};
 
/***********************************************************************
vl::objectmodel::ObjectTypeFunction<R(C::*)() >
***********************************************************************/
		template<typename C ,typename R >
		class ObjectTypeFunction<R(C::*)() > : public SpecifiedObjectType<R(C::*)() >
		{
			friend class ObjectTypeRegister<R(C::*)() >;
			typedef R(C::*FunctionPointer)() ;
		protected:
			static void* ctor()
			{
				return new FunctionPointer(0);
			}
			collections::List<ObjectType*>		parameterTypes;
			ObjectType*							returnType;
			ObjectType*							thisType;
			collections::List<ObjectMember*>	constructors;
		public:
			ObjectTypeFunction()
			{
				returnType=0;
				thisType=0;
			}
			~ObjectTypeFunction()
			{
				for(int i=0;i<constructors.Count();i++)
				{
					delete constructors[i];
				}
			}
			ObjectCategory Category()const
			{
				return ObjectType::Function;
			}
			const ObjectType* ReturnType()const
			{
				return returnType;
			}
			const TypeList& ParameterTypes()const
			{
				return parameterTypes.Wrap();
			}
			const ObjectType* ThisType()const
			{
				return thisType;
			}
			bool IsConstFunction()const
			{
				return false;
			}
			void* Invoke(void* object, void* funcPtrPtr, void** arguments)const
			{

				R result=(((C*)object)->*(*((FunctionPointer*)funcPtrPtr)))();
				return new R(result);
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
				delete (FunctionPointer*)instance;
			}
		};
		template<typename C ,typename R >
		class ObjectTypeInformation<R(C::*)() >
		{
		public:
			static WString GetName()
			{
				return ObjectTypeInformation<R>::GetName()+L"("+ObjectTypeInformation<C>::GetName()+L"::*)"+L"()" ;
			}
			typedef ObjectTypeFunction<R(C::*)() > ObjectTypeImplementation;
		};
		template<typename C ,typename R >
		class ObjectTypeRegister<R(C::*)() >
		{
		public:
			static void Register()
			{
				ObjectTypeFunction<R(C::*)() >* type=new ObjectTypeFunction<R(C::*)() >;
				RegisterObjectType(ObjectTypeInformation<R(C::*)() >::GetName(), type);

				type->returnType=TypeOf<R>();
				type->thisType=TypeOf<C>();
				type->constructors.Add(new ObjectTypeFunction<R(C::*)() >::Constructor<void*(*)()>(&ObjectTypeFunction<R(C::*)() >::ctor));
			}
		};
 
/***********************************************************************
vl::objectmodel::ObjectTypeFunction<void(C::*)() >
***********************************************************************/
		template<typename C   >
		class ObjectTypeFunction<void(C::*)() > : public SpecifiedObjectType<void(C::*)() >
		{
			friend class ObjectTypeRegister<void(C::*)() >;
			typedef void(C::*FunctionPointer)() ;
		protected:
			static void* ctor()
			{
				return new FunctionPointer(0);
			}
			collections::List<ObjectType*>		parameterTypes;
			ObjectType*							returnType;
			ObjectType*							thisType;
			collections::List<ObjectMember*>	constructors;
		public:
			ObjectTypeFunction()
			{
				returnType=0;
				thisType=0;
			}
			~ObjectTypeFunction()
			{
				for(int i=0;i<constructors.Count();i++)
				{
					delete constructors[i];
				}
			}
			ObjectCategory Category()const
			{
				return ObjectType::Function;
			}
			const ObjectType* ReturnType()const
			{
				return returnType;
			}
			const TypeList& ParameterTypes()const
			{
				return parameterTypes.Wrap();
			}
			const ObjectType* ThisType()const
			{
				return thisType;
			}
			bool IsConstFunction()const
			{
				return false;
			}
			void* Invoke(void* object, void* funcPtrPtr, void** arguments)const
			{

				 (((C*)object)->*(*((FunctionPointer*)funcPtrPtr)))();
				return 0;
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
				delete (FunctionPointer*)instance;
			}
		};
		template<typename C   >
		class ObjectTypeInformation<void(C::*)() >
		{
		public:
			static WString GetName()
			{
				return ObjectTypeInformation<void>::GetName()+L"("+ObjectTypeInformation<C>::GetName()+L"::*)"+L"()" ;
			}
			typedef ObjectTypeFunction<void(C::*)() > ObjectTypeImplementation;
		};
		template<typename C   >
		class ObjectTypeRegister<void(C::*)() >
		{
		public:
			static void Register()
			{
				ObjectTypeFunction<void(C::*)() >* type=new ObjectTypeFunction<void(C::*)() >;
				RegisterObjectType(ObjectTypeInformation<void(C::*)() >::GetName(), type);

				type->returnType=TypeOf<void>();
				type->thisType=TypeOf<C>();
				type->constructors.Add(new ObjectTypeFunction<void(C::*)() >::Constructor<void*(*)()>(&ObjectTypeFunction<void(C::*)() >::ctor));
			}
		};
 
/***********************************************************************
vl::objectmodel::ObjectTypeFunction<R(C::*)()const>
***********************************************************************/
		template<typename C ,typename R >
		class ObjectTypeFunction<R(C::*)()const> : public SpecifiedObjectType<R(C::*)()const>
		{
			friend class ObjectTypeRegister<R(C::*)()const>;
			typedef R(C::*FunctionPointer)()const;
		protected:
			static void* ctor()
			{
				return new FunctionPointer(0);
			}
			collections::List<ObjectType*>		parameterTypes;
			ObjectType*							returnType;
			ObjectType*							thisType;
			collections::List<ObjectMember*>	constructors;
		public:
			ObjectTypeFunction()
			{
				returnType=0;
				thisType=0;
			}
			~ObjectTypeFunction()
			{
				for(int i=0;i<constructors.Count();i++)
				{
					delete constructors[i];
				}
			}
			ObjectCategory Category()const
			{
				return ObjectType::Function;
			}
			const ObjectType* ReturnType()const
			{
				return returnType;
			}
			const TypeList& ParameterTypes()const
			{
				return parameterTypes.Wrap();
			}
			const ObjectType* ThisType()const
			{
				return thisType;
			}
			bool IsConstFunction()const
			{
				return true;
			}
			void* Invoke(void* object, void* funcPtrPtr, void** arguments)const
			{

				R result=(((C*)object)->*(*((FunctionPointer*)funcPtrPtr)))();
				return new R(result);
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
				delete (FunctionPointer*)instance;
			}
		};
		template<typename C ,typename R >
		class ObjectTypeInformation<R(C::*)()const>
		{
		public:
			static WString GetName()
			{
				return ObjectTypeInformation<R>::GetName()+L"("+ObjectTypeInformation<C>::GetName()+L"::*)"+L"()"+L"const";
			}
			typedef ObjectTypeFunction<R(C::*)()const> ObjectTypeImplementation;
		};
		template<typename C ,typename R >
		class ObjectTypeRegister<R(C::*)()const>
		{
		public:
			static void Register()
			{
				ObjectTypeFunction<R(C::*)()const>* type=new ObjectTypeFunction<R(C::*)()const>;
				RegisterObjectType(ObjectTypeInformation<R(C::*)()const>::GetName(), type);

				type->returnType=TypeOf<R>();
				type->thisType=TypeOf<C>();
				type->constructors.Add(new ObjectTypeFunction<R(C::*)()const>::Constructor<void*(*)()>(&ObjectTypeFunction<R(C::*)()const>::ctor));
			}
		};
 
/***********************************************************************
vl::objectmodel::ObjectTypeFunction<void(C::*)()const>
***********************************************************************/
		template<typename C   >
		class ObjectTypeFunction<void(C::*)()const> : public SpecifiedObjectType<void(C::*)()const>
		{
			friend class ObjectTypeRegister<void(C::*)()const>;
			typedef void(C::*FunctionPointer)()const;
		protected:
			static void* ctor()
			{
				return new FunctionPointer(0);
			}
			collections::List<ObjectType*>		parameterTypes;
			ObjectType*							returnType;
			ObjectType*							thisType;
			collections::List<ObjectMember*>	constructors;
		public:
			ObjectTypeFunction()
			{
				returnType=0;
				thisType=0;
			}
			~ObjectTypeFunction()
			{
				for(int i=0;i<constructors.Count();i++)
				{
					delete constructors[i];
				}
			}
			ObjectCategory Category()const
			{
				return ObjectType::Function;
			}
			const ObjectType* ReturnType()const
			{
				return returnType;
			}
			const TypeList& ParameterTypes()const
			{
				return parameterTypes.Wrap();
			}
			const ObjectType* ThisType()const
			{
				return thisType;
			}
			bool IsConstFunction()const
			{
				return true;
			}
			void* Invoke(void* object, void* funcPtrPtr, void** arguments)const
			{

				 (((C*)object)->*(*((FunctionPointer*)funcPtrPtr)))();
				return 0;
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
				delete (FunctionPointer*)instance;
			}
		};
		template<typename C   >
		class ObjectTypeInformation<void(C::*)()const>
		{
		public:
			static WString GetName()
			{
				return ObjectTypeInformation<void>::GetName()+L"("+ObjectTypeInformation<C>::GetName()+L"::*)"+L"()"+L"const";
			}
			typedef ObjectTypeFunction<void(C::*)()const> ObjectTypeImplementation;
		};
		template<typename C   >
		class ObjectTypeRegister<void(C::*)()const>
		{
		public:
			static void Register()
			{
				ObjectTypeFunction<void(C::*)()const>* type=new ObjectTypeFunction<void(C::*)()const>;
				RegisterObjectType(ObjectTypeInformation<void(C::*)()const>::GetName(), type);

				type->returnType=TypeOf<void>();
				type->thisType=TypeOf<C>();
				type->constructors.Add(new ObjectTypeFunction<void(C::*)()const>::Constructor<void*(*)()>(&ObjectTypeFunction<void(C::*)()const>::ctor));
			}
		};
  
/***********************************************************************
vl::objectmodel::ObjectTypeFunction<R(*)(T0) >
***********************************************************************/
		template<typename R,typename T0>
		class ObjectTypeFunction<R(*)(T0) > : public SpecifiedObjectType<R(*)(T0) >
		{
			friend class ObjectTypeRegister<R(*)(T0) >;
			typedef R(*FunctionPointer)(T0) ;
		protected:
			static void* ctor()
			{
				return new FunctionPointer(0);
			}
			collections::List<ObjectType*>		parameterTypes;
			ObjectType*							returnType;
			ObjectType*							thisType;
			collections::List<ObjectMember*>	constructors;
		public:
			ObjectTypeFunction()
			{
				returnType=0;
				thisType=0;
			}
			~ObjectTypeFunction()
			{
				for(int i=0;i<constructors.Count();i++)
				{
					delete constructors[i];
				}
			}
			ObjectCategory Category()const
			{
				return ObjectType::Function;
			}
			const ObjectType* ReturnType()const
			{
				return returnType;
			}
			const TypeList& ParameterTypes()const
			{
				return parameterTypes.Wrap();
			}
			const ObjectType* ThisType()const
			{
				return thisType;
			}
			bool IsConstFunction()const
			{
				return false;
			}
			void* Invoke(void* object, void* funcPtrPtr, void** arguments)const
			{
				typename RemoveCVR<T0>::Type* argument0 = (typename RemoveCVR<T0>::Type*)arguments[0];
 
				R result=(*((FunctionPointer*)funcPtrPtr))(*argument0);
				return new R(result);
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
				delete (FunctionPointer*)instance;
			}
		};
		template<typename R,typename T0>
		class ObjectTypeInformation<R(*)(T0) >
		{
		public:
			static WString GetName()
			{
				return ObjectTypeInformation<R>::GetName()+L"(*)"+L"("+ObjectTypeInformation<T0>::GetName()+L")" ;
			}
			typedef ObjectTypeFunction<R(*)(T0) > ObjectTypeImplementation;
		};
		template<typename R,typename T0>
		class ObjectTypeRegister<R(*)(T0) >
		{
		public:
			static void Register()
			{
				ObjectTypeFunction<R(*)(T0) >* type=new ObjectTypeFunction<R(*)(T0) >;
				RegisterObjectType(ObjectTypeInformation<R(*)(T0) >::GetName(), type);
				type->parameterTypes.Add(TypeOf<T0>());
 
				type->returnType=TypeOf<R>();
				type->thisType=0;
				type->constructors.Add(new ObjectTypeFunction<R(*)(T0) >::Constructor<void*(*)()>(&ObjectTypeFunction<R(*)(T0) >::ctor));
			}
		};
 
/***********************************************************************
vl::objectmodel::ObjectTypeFunction<void(*)(T0) >
***********************************************************************/
		template< typename T0>
		class ObjectTypeFunction<void(*)(T0) > : public SpecifiedObjectType<void(*)(T0) >
		{
			friend class ObjectTypeRegister<void(*)(T0) >;
			typedef void(*FunctionPointer)(T0) ;
		protected:
			static void* ctor()
			{
				return new FunctionPointer(0);
			}
			collections::List<ObjectType*>		parameterTypes;
			ObjectType*							returnType;
			ObjectType*							thisType;
			collections::List<ObjectMember*>	constructors;
		public:
			ObjectTypeFunction()
			{
				returnType=0;
				thisType=0;
			}
			~ObjectTypeFunction()
			{
				for(int i=0;i<constructors.Count();i++)
				{
					delete constructors[i];
				}
			}
			ObjectCategory Category()const
			{
				return ObjectType::Function;
			}
			const ObjectType* ReturnType()const
			{
				return returnType;
			}
			const TypeList& ParameterTypes()const
			{
				return parameterTypes.Wrap();
			}
			const ObjectType* ThisType()const
			{
				return thisType;
			}
			bool IsConstFunction()const
			{
				return false;
			}
			void* Invoke(void* object, void* funcPtrPtr, void** arguments)const
			{
				typename RemoveCVR<T0>::Type* argument0 = (typename RemoveCVR<T0>::Type*)arguments[0];
 
				 (*((FunctionPointer*)funcPtrPtr))(*argument0);
				return 0;
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
				delete (FunctionPointer*)instance;
			}
		};
		template< typename T0>
		class ObjectTypeInformation<void(*)(T0) >
		{
		public:
			static WString GetName()
			{
				return ObjectTypeInformation<void>::GetName()+L"(*)"+L"("+ObjectTypeInformation<T0>::GetName()+L")" ;
			}
			typedef ObjectTypeFunction<void(*)(T0) > ObjectTypeImplementation;
		};
		template< typename T0>
		class ObjectTypeRegister<void(*)(T0) >
		{
		public:
			static void Register()
			{
				ObjectTypeFunction<void(*)(T0) >* type=new ObjectTypeFunction<void(*)(T0) >;
				RegisterObjectType(ObjectTypeInformation<void(*)(T0) >::GetName(), type);
				type->parameterTypes.Add(TypeOf<T0>());
 
				type->returnType=TypeOf<void>();
				type->thisType=0;
				type->constructors.Add(new ObjectTypeFunction<void(*)(T0) >::Constructor<void*(*)()>(&ObjectTypeFunction<void(*)(T0) >::ctor));
			}
		};
 
/***********************************************************************
vl::objectmodel::ObjectTypeFunction<R(C::*)(T0) >
***********************************************************************/
		template<typename C ,typename R,typename T0>
		class ObjectTypeFunction<R(C::*)(T0) > : public SpecifiedObjectType<R(C::*)(T0) >
		{
			friend class ObjectTypeRegister<R(C::*)(T0) >;
			typedef R(C::*FunctionPointer)(T0) ;
		protected:
			static void* ctor()
			{
				return new FunctionPointer(0);
			}
			collections::List<ObjectType*>		parameterTypes;
			ObjectType*							returnType;
			ObjectType*							thisType;
			collections::List<ObjectMember*>	constructors;
		public:
			ObjectTypeFunction()
			{
				returnType=0;
				thisType=0;
			}
			~ObjectTypeFunction()
			{
				for(int i=0;i<constructors.Count();i++)
				{
					delete constructors[i];
				}
			}
			ObjectCategory Category()const
			{
				return ObjectType::Function;
			}
			const ObjectType* ReturnType()const
			{
				return returnType;
			}
			const TypeList& ParameterTypes()const
			{
				return parameterTypes.Wrap();
			}
			const ObjectType* ThisType()const
			{
				return thisType;
			}
			bool IsConstFunction()const
			{
				return false;
			}
			void* Invoke(void* object, void* funcPtrPtr, void** arguments)const
			{
				typename RemoveCVR<T0>::Type* argument0 = (typename RemoveCVR<T0>::Type*)arguments[0];
 
				R result=(((C*)object)->*(*((FunctionPointer*)funcPtrPtr)))(*argument0);
				return new R(result);
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
				delete (FunctionPointer*)instance;
			}
		};
		template<typename C ,typename R,typename T0>
		class ObjectTypeInformation<R(C::*)(T0) >
		{
		public:
			static WString GetName()
			{
				return ObjectTypeInformation<R>::GetName()+L"("+ObjectTypeInformation<C>::GetName()+L"::*)"+L"("+ObjectTypeInformation<T0>::GetName()+L")" ;
			}
			typedef ObjectTypeFunction<R(C::*)(T0) > ObjectTypeImplementation;
		};
		template<typename C ,typename R,typename T0>
		class ObjectTypeRegister<R(C::*)(T0) >
		{
		public:
			static void Register()
			{
				ObjectTypeFunction<R(C::*)(T0) >* type=new ObjectTypeFunction<R(C::*)(T0) >;
				RegisterObjectType(ObjectTypeInformation<R(C::*)(T0) >::GetName(), type);
				type->parameterTypes.Add(TypeOf<T0>());
 
				type->returnType=TypeOf<R>();
				type->thisType=TypeOf<C>();
				type->constructors.Add(new ObjectTypeFunction<R(C::*)(T0) >::Constructor<void*(*)()>(&ObjectTypeFunction<R(C::*)(T0) >::ctor));
			}
		};
 
/***********************************************************************
vl::objectmodel::ObjectTypeFunction<void(C::*)(T0) >
***********************************************************************/
		template<typename C  ,typename T0>
		class ObjectTypeFunction<void(C::*)(T0) > : public SpecifiedObjectType<void(C::*)(T0) >
		{
			friend class ObjectTypeRegister<void(C::*)(T0) >;
			typedef void(C::*FunctionPointer)(T0) ;
		protected:
			static void* ctor()
			{
				return new FunctionPointer(0);
			}
			collections::List<ObjectType*>		parameterTypes;
			ObjectType*							returnType;
			ObjectType*							thisType;
			collections::List<ObjectMember*>	constructors;
		public:
			ObjectTypeFunction()
			{
				returnType=0;
				thisType=0;
			}
			~ObjectTypeFunction()
			{
				for(int i=0;i<constructors.Count();i++)
				{
					delete constructors[i];
				}
			}
			ObjectCategory Category()const
			{
				return ObjectType::Function;
			}
			const ObjectType* ReturnType()const
			{
				return returnType;
			}
			const TypeList& ParameterTypes()const
			{
				return parameterTypes.Wrap();
			}
			const ObjectType* ThisType()const
			{
				return thisType;
			}
			bool IsConstFunction()const
			{
				return false;
			}
			void* Invoke(void* object, void* funcPtrPtr, void** arguments)const
			{
				typename RemoveCVR<T0>::Type* argument0 = (typename RemoveCVR<T0>::Type*)arguments[0];
 
				 (((C*)object)->*(*((FunctionPointer*)funcPtrPtr)))(*argument0);
				return 0;
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
				delete (FunctionPointer*)instance;
			}
		};
		template<typename C  ,typename T0>
		class ObjectTypeInformation<void(C::*)(T0) >
		{
		public:
			static WString GetName()
			{
				return ObjectTypeInformation<void>::GetName()+L"("+ObjectTypeInformation<C>::GetName()+L"::*)"+L"("+ObjectTypeInformation<T0>::GetName()+L")" ;
			}
			typedef ObjectTypeFunction<void(C::*)(T0) > ObjectTypeImplementation;
		};
		template<typename C  ,typename T0>
		class ObjectTypeRegister<void(C::*)(T0) >
		{
		public:
			static void Register()
			{
				ObjectTypeFunction<void(C::*)(T0) >* type=new ObjectTypeFunction<void(C::*)(T0) >;
				RegisterObjectType(ObjectTypeInformation<void(C::*)(T0) >::GetName(), type);
				type->parameterTypes.Add(TypeOf<T0>());
 
				type->returnType=TypeOf<void>();
				type->thisType=TypeOf<C>();
				type->constructors.Add(new ObjectTypeFunction<void(C::*)(T0) >::Constructor<void*(*)()>(&ObjectTypeFunction<void(C::*)(T0) >::ctor));
			}
		};
 
/***********************************************************************
vl::objectmodel::ObjectTypeFunction<R(C::*)(T0)const>
***********************************************************************/
		template<typename C ,typename R,typename T0>
		class ObjectTypeFunction<R(C::*)(T0)const> : public SpecifiedObjectType<R(C::*)(T0)const>
		{
			friend class ObjectTypeRegister<R(C::*)(T0)const>;
			typedef R(C::*FunctionPointer)(T0)const;
		protected:
			static void* ctor()
			{
				return new FunctionPointer(0);
			}
			collections::List<ObjectType*>		parameterTypes;
			ObjectType*							returnType;
			ObjectType*							thisType;
			collections::List<ObjectMember*>	constructors;
		public:
			ObjectTypeFunction()
			{
				returnType=0;
				thisType=0;
			}
			~ObjectTypeFunction()
			{
				for(int i=0;i<constructors.Count();i++)
				{
					delete constructors[i];
				}
			}
			ObjectCategory Category()const
			{
				return ObjectType::Function;
			}
			const ObjectType* ReturnType()const
			{
				return returnType;
			}
			const TypeList& ParameterTypes()const
			{
				return parameterTypes.Wrap();
			}
			const ObjectType* ThisType()const
			{
				return thisType;
			}
			bool IsConstFunction()const
			{
				return true;
			}
			void* Invoke(void* object, void* funcPtrPtr, void** arguments)const
			{
				typename RemoveCVR<T0>::Type* argument0 = (typename RemoveCVR<T0>::Type*)arguments[0];
 
				R result=(((C*)object)->*(*((FunctionPointer*)funcPtrPtr)))(*argument0);
				return new R(result);
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
				delete (FunctionPointer*)instance;
			}
		};
		template<typename C ,typename R,typename T0>
		class ObjectTypeInformation<R(C::*)(T0)const>
		{
		public:
			static WString GetName()
			{
				return ObjectTypeInformation<R>::GetName()+L"("+ObjectTypeInformation<C>::GetName()+L"::*)"+L"("+ObjectTypeInformation<T0>::GetName()+L")"+L"const";
			}
			typedef ObjectTypeFunction<R(C::*)(T0)const> ObjectTypeImplementation;
		};
		template<typename C ,typename R,typename T0>
		class ObjectTypeRegister<R(C::*)(T0)const>
		{
		public:
			static void Register()
			{
				ObjectTypeFunction<R(C::*)(T0)const>* type=new ObjectTypeFunction<R(C::*)(T0)const>;
				RegisterObjectType(ObjectTypeInformation<R(C::*)(T0)const>::GetName(), type);
				type->parameterTypes.Add(TypeOf<T0>());
 
				type->returnType=TypeOf<R>();
				type->thisType=TypeOf<C>();
				type->constructors.Add(new ObjectTypeFunction<R(C::*)(T0)const>::Constructor<void*(*)()>(&ObjectTypeFunction<R(C::*)(T0)const>::ctor));
			}
		};
 
/***********************************************************************
vl::objectmodel::ObjectTypeFunction<void(C::*)(T0)const>
***********************************************************************/
		template<typename C  ,typename T0>
		class ObjectTypeFunction<void(C::*)(T0)const> : public SpecifiedObjectType<void(C::*)(T0)const>
		{
			friend class ObjectTypeRegister<void(C::*)(T0)const>;
			typedef void(C::*FunctionPointer)(T0)const;
		protected:
			static void* ctor()
			{
				return new FunctionPointer(0);
			}
			collections::List<ObjectType*>		parameterTypes;
			ObjectType*							returnType;
			ObjectType*							thisType;
			collections::List<ObjectMember*>	constructors;
		public:
			ObjectTypeFunction()
			{
				returnType=0;
				thisType=0;
			}
			~ObjectTypeFunction()
			{
				for(int i=0;i<constructors.Count();i++)
				{
					delete constructors[i];
				}
			}
			ObjectCategory Category()const
			{
				return ObjectType::Function;
			}
			const ObjectType* ReturnType()const
			{
				return returnType;
			}
			const TypeList& ParameterTypes()const
			{
				return parameterTypes.Wrap();
			}
			const ObjectType* ThisType()const
			{
				return thisType;
			}
			bool IsConstFunction()const
			{
				return true;
			}
			void* Invoke(void* object, void* funcPtrPtr, void** arguments)const
			{
				typename RemoveCVR<T0>::Type* argument0 = (typename RemoveCVR<T0>::Type*)arguments[0];
 
				 (((C*)object)->*(*((FunctionPointer*)funcPtrPtr)))(*argument0);
				return 0;
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
				delete (FunctionPointer*)instance;
			}
		};
		template<typename C  ,typename T0>
		class ObjectTypeInformation<void(C::*)(T0)const>
		{
		public:
			static WString GetName()
			{
				return ObjectTypeInformation<void>::GetName()+L"("+ObjectTypeInformation<C>::GetName()+L"::*)"+L"("+ObjectTypeInformation<T0>::GetName()+L")"+L"const";
			}
			typedef ObjectTypeFunction<void(C::*)(T0)const> ObjectTypeImplementation;
		};
		template<typename C  ,typename T0>
		class ObjectTypeRegister<void(C::*)(T0)const>
		{
		public:
			static void Register()
			{
				ObjectTypeFunction<void(C::*)(T0)const>* type=new ObjectTypeFunction<void(C::*)(T0)const>;
				RegisterObjectType(ObjectTypeInformation<void(C::*)(T0)const>::GetName(), type);
				type->parameterTypes.Add(TypeOf<T0>());
 
				type->returnType=TypeOf<void>();
				type->thisType=TypeOf<C>();
				type->constructors.Add(new ObjectTypeFunction<void(C::*)(T0)const>::Constructor<void*(*)()>(&ObjectTypeFunction<void(C::*)(T0)const>::ctor));
			}
		};
  
/***********************************************************************
vl::objectmodel::ObjectTypeFunction<R(*)(T0,T1) >
***********************************************************************/
		template<typename R,typename T0,typename T1>
		class ObjectTypeFunction<R(*)(T0,T1) > : public SpecifiedObjectType<R(*)(T0,T1) >
		{
			friend class ObjectTypeRegister<R(*)(T0,T1) >;
			typedef R(*FunctionPointer)(T0,T1) ;
		protected:
			static void* ctor()
			{
				return new FunctionPointer(0);
			}
			collections::List<ObjectType*>		parameterTypes;
			ObjectType*							returnType;
			ObjectType*							thisType;
			collections::List<ObjectMember*>	constructors;
		public:
			ObjectTypeFunction()
			{
				returnType=0;
				thisType=0;
			}
			~ObjectTypeFunction()
			{
				for(int i=0;i<constructors.Count();i++)
				{
					delete constructors[i];
				}
			}
			ObjectCategory Category()const
			{
				return ObjectType::Function;
			}
			const ObjectType* ReturnType()const
			{
				return returnType;
			}
			const TypeList& ParameterTypes()const
			{
				return parameterTypes.Wrap();
			}
			const ObjectType* ThisType()const
			{
				return thisType;
			}
			bool IsConstFunction()const
			{
				return false;
			}
			void* Invoke(void* object, void* funcPtrPtr, void** arguments)const
			{
				typename RemoveCVR<T0>::Type* argument0 = (typename RemoveCVR<T0>::Type*)arguments[0];
 				typename RemoveCVR<T1>::Type* argument1 = (typename RemoveCVR<T1>::Type*)arguments[1];
 
				R result=(*((FunctionPointer*)funcPtrPtr))(*argument0,*argument1);
				return new R(result);
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
				delete (FunctionPointer*)instance;
			}
		};
		template<typename R,typename T0,typename T1>
		class ObjectTypeInformation<R(*)(T0,T1) >
		{
		public:
			static WString GetName()
			{
				return ObjectTypeInformation<R>::GetName()+L"(*)"+L"("+ObjectTypeInformation<T0>::GetName()+L","+ObjectTypeInformation<T1>::GetName()+L")" ;
			}
			typedef ObjectTypeFunction<R(*)(T0,T1) > ObjectTypeImplementation;
		};
		template<typename R,typename T0,typename T1>
		class ObjectTypeRegister<R(*)(T0,T1) >
		{
		public:
			static void Register()
			{
				ObjectTypeFunction<R(*)(T0,T1) >* type=new ObjectTypeFunction<R(*)(T0,T1) >;
				RegisterObjectType(ObjectTypeInformation<R(*)(T0,T1) >::GetName(), type);
				type->parameterTypes.Add(TypeOf<T0>());
 				type->parameterTypes.Add(TypeOf<T1>());
 
				type->returnType=TypeOf<R>();
				type->thisType=0;
				type->constructors.Add(new ObjectTypeFunction<R(*)(T0,T1) >::Constructor<void*(*)()>(&ObjectTypeFunction<R(*)(T0,T1) >::ctor));
			}
		};
 
/***********************************************************************
vl::objectmodel::ObjectTypeFunction<void(*)(T0,T1) >
***********************************************************************/
		template< typename T0,typename T1>
		class ObjectTypeFunction<void(*)(T0,T1) > : public SpecifiedObjectType<void(*)(T0,T1) >
		{
			friend class ObjectTypeRegister<void(*)(T0,T1) >;
			typedef void(*FunctionPointer)(T0,T1) ;
		protected:
			static void* ctor()
			{
				return new FunctionPointer(0);
			}
			collections::List<ObjectType*>		parameterTypes;
			ObjectType*							returnType;
			ObjectType*							thisType;
			collections::List<ObjectMember*>	constructors;
		public:
			ObjectTypeFunction()
			{
				returnType=0;
				thisType=0;
			}
			~ObjectTypeFunction()
			{
				for(int i=0;i<constructors.Count();i++)
				{
					delete constructors[i];
				}
			}
			ObjectCategory Category()const
			{
				return ObjectType::Function;
			}
			const ObjectType* ReturnType()const
			{
				return returnType;
			}
			const TypeList& ParameterTypes()const
			{
				return parameterTypes.Wrap();
			}
			const ObjectType* ThisType()const
			{
				return thisType;
			}
			bool IsConstFunction()const
			{
				return false;
			}
			void* Invoke(void* object, void* funcPtrPtr, void** arguments)const
			{
				typename RemoveCVR<T0>::Type* argument0 = (typename RemoveCVR<T0>::Type*)arguments[0];
 				typename RemoveCVR<T1>::Type* argument1 = (typename RemoveCVR<T1>::Type*)arguments[1];
 
				 (*((FunctionPointer*)funcPtrPtr))(*argument0,*argument1);
				return 0;
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
				delete (FunctionPointer*)instance;
			}
		};
		template< typename T0,typename T1>
		class ObjectTypeInformation<void(*)(T0,T1) >
		{
		public:
			static WString GetName()
			{
				return ObjectTypeInformation<void>::GetName()+L"(*)"+L"("+ObjectTypeInformation<T0>::GetName()+L","+ObjectTypeInformation<T1>::GetName()+L")" ;
			}
			typedef ObjectTypeFunction<void(*)(T0,T1) > ObjectTypeImplementation;
		};
		template< typename T0,typename T1>
		class ObjectTypeRegister<void(*)(T0,T1) >
		{
		public:
			static void Register()
			{
				ObjectTypeFunction<void(*)(T0,T1) >* type=new ObjectTypeFunction<void(*)(T0,T1) >;
				RegisterObjectType(ObjectTypeInformation<void(*)(T0,T1) >::GetName(), type);
				type->parameterTypes.Add(TypeOf<T0>());
 				type->parameterTypes.Add(TypeOf<T1>());
 
				type->returnType=TypeOf<void>();
				type->thisType=0;
				type->constructors.Add(new ObjectTypeFunction<void(*)(T0,T1) >::Constructor<void*(*)()>(&ObjectTypeFunction<void(*)(T0,T1) >::ctor));
			}
		};
 
/***********************************************************************
vl::objectmodel::ObjectTypeFunction<R(C::*)(T0,T1) >
***********************************************************************/
		template<typename C ,typename R,typename T0,typename T1>
		class ObjectTypeFunction<R(C::*)(T0,T1) > : public SpecifiedObjectType<R(C::*)(T0,T1) >
		{
			friend class ObjectTypeRegister<R(C::*)(T0,T1) >;
			typedef R(C::*FunctionPointer)(T0,T1) ;
		protected:
			static void* ctor()
			{
				return new FunctionPointer(0);
			}
			collections::List<ObjectType*>		parameterTypes;
			ObjectType*							returnType;
			ObjectType*							thisType;
			collections::List<ObjectMember*>	constructors;
		public:
			ObjectTypeFunction()
			{
				returnType=0;
				thisType=0;
			}
			~ObjectTypeFunction()
			{
				for(int i=0;i<constructors.Count();i++)
				{
					delete constructors[i];
				}
			}
			ObjectCategory Category()const
			{
				return ObjectType::Function;
			}
			const ObjectType* ReturnType()const
			{
				return returnType;
			}
			const TypeList& ParameterTypes()const
			{
				return parameterTypes.Wrap();
			}
			const ObjectType* ThisType()const
			{
				return thisType;
			}
			bool IsConstFunction()const
			{
				return false;
			}
			void* Invoke(void* object, void* funcPtrPtr, void** arguments)const
			{
				typename RemoveCVR<T0>::Type* argument0 = (typename RemoveCVR<T0>::Type*)arguments[0];
 				typename RemoveCVR<T1>::Type* argument1 = (typename RemoveCVR<T1>::Type*)arguments[1];
 
				R result=(((C*)object)->*(*((FunctionPointer*)funcPtrPtr)))(*argument0,*argument1);
				return new R(result);
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
				delete (FunctionPointer*)instance;
			}
		};
		template<typename C ,typename R,typename T0,typename T1>
		class ObjectTypeInformation<R(C::*)(T0,T1) >
		{
		public:
			static WString GetName()
			{
				return ObjectTypeInformation<R>::GetName()+L"("+ObjectTypeInformation<C>::GetName()+L"::*)"+L"("+ObjectTypeInformation<T0>::GetName()+L","+ObjectTypeInformation<T1>::GetName()+L")" ;
			}
			typedef ObjectTypeFunction<R(C::*)(T0,T1) > ObjectTypeImplementation;
		};
		template<typename C ,typename R,typename T0,typename T1>
		class ObjectTypeRegister<R(C::*)(T0,T1) >
		{
		public:
			static void Register()
			{
				ObjectTypeFunction<R(C::*)(T0,T1) >* type=new ObjectTypeFunction<R(C::*)(T0,T1) >;
				RegisterObjectType(ObjectTypeInformation<R(C::*)(T0,T1) >::GetName(), type);
				type->parameterTypes.Add(TypeOf<T0>());
 				type->parameterTypes.Add(TypeOf<T1>());
 
				type->returnType=TypeOf<R>();
				type->thisType=TypeOf<C>();
				type->constructors.Add(new ObjectTypeFunction<R(C::*)(T0,T1) >::Constructor<void*(*)()>(&ObjectTypeFunction<R(C::*)(T0,T1) >::ctor));
			}
		};
 
/***********************************************************************
vl::objectmodel::ObjectTypeFunction<void(C::*)(T0,T1) >
***********************************************************************/
		template<typename C  ,typename T0,typename T1>
		class ObjectTypeFunction<void(C::*)(T0,T1) > : public SpecifiedObjectType<void(C::*)(T0,T1) >
		{
			friend class ObjectTypeRegister<void(C::*)(T0,T1) >;
			typedef void(C::*FunctionPointer)(T0,T1) ;
		protected:
			static void* ctor()
			{
				return new FunctionPointer(0);
			}
			collections::List<ObjectType*>		parameterTypes;
			ObjectType*							returnType;
			ObjectType*							thisType;
			collections::List<ObjectMember*>	constructors;
		public:
			ObjectTypeFunction()
			{
				returnType=0;
				thisType=0;
			}
			~ObjectTypeFunction()
			{
				for(int i=0;i<constructors.Count();i++)
				{
					delete constructors[i];
				}
			}
			ObjectCategory Category()const
			{
				return ObjectType::Function;
			}
			const ObjectType* ReturnType()const
			{
				return returnType;
			}
			const TypeList& ParameterTypes()const
			{
				return parameterTypes.Wrap();
			}
			const ObjectType* ThisType()const
			{
				return thisType;
			}
			bool IsConstFunction()const
			{
				return false;
			}
			void* Invoke(void* object, void* funcPtrPtr, void** arguments)const
			{
				typename RemoveCVR<T0>::Type* argument0 = (typename RemoveCVR<T0>::Type*)arguments[0];
 				typename RemoveCVR<T1>::Type* argument1 = (typename RemoveCVR<T1>::Type*)arguments[1];
 
				 (((C*)object)->*(*((FunctionPointer*)funcPtrPtr)))(*argument0,*argument1);
				return 0;
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
				delete (FunctionPointer*)instance;
			}
		};
		template<typename C  ,typename T0,typename T1>
		class ObjectTypeInformation<void(C::*)(T0,T1) >
		{
		public:
			static WString GetName()
			{
				return ObjectTypeInformation<void>::GetName()+L"("+ObjectTypeInformation<C>::GetName()+L"::*)"+L"("+ObjectTypeInformation<T0>::GetName()+L","+ObjectTypeInformation<T1>::GetName()+L")" ;
			}
			typedef ObjectTypeFunction<void(C::*)(T0,T1) > ObjectTypeImplementation;
		};
		template<typename C  ,typename T0,typename T1>
		class ObjectTypeRegister<void(C::*)(T0,T1) >
		{
		public:
			static void Register()
			{
				ObjectTypeFunction<void(C::*)(T0,T1) >* type=new ObjectTypeFunction<void(C::*)(T0,T1) >;
				RegisterObjectType(ObjectTypeInformation<void(C::*)(T0,T1) >::GetName(), type);
				type->parameterTypes.Add(TypeOf<T0>());
 				type->parameterTypes.Add(TypeOf<T1>());
 
				type->returnType=TypeOf<void>();
				type->thisType=TypeOf<C>();
				type->constructors.Add(new ObjectTypeFunction<void(C::*)(T0,T1) >::Constructor<void*(*)()>(&ObjectTypeFunction<void(C::*)(T0,T1) >::ctor));
			}
		};
 
/***********************************************************************
vl::objectmodel::ObjectTypeFunction<R(C::*)(T0,T1)const>
***********************************************************************/
		template<typename C ,typename R,typename T0,typename T1>
		class ObjectTypeFunction<R(C::*)(T0,T1)const> : public SpecifiedObjectType<R(C::*)(T0,T1)const>
		{
			friend class ObjectTypeRegister<R(C::*)(T0,T1)const>;
			typedef R(C::*FunctionPointer)(T0,T1)const;
		protected:
			static void* ctor()
			{
				return new FunctionPointer(0);
			}
			collections::List<ObjectType*>		parameterTypes;
			ObjectType*							returnType;
			ObjectType*							thisType;
			collections::List<ObjectMember*>	constructors;
		public:
			ObjectTypeFunction()
			{
				returnType=0;
				thisType=0;
			}
			~ObjectTypeFunction()
			{
				for(int i=0;i<constructors.Count();i++)
				{
					delete constructors[i];
				}
			}
			ObjectCategory Category()const
			{
				return ObjectType::Function;
			}
			const ObjectType* ReturnType()const
			{
				return returnType;
			}
			const TypeList& ParameterTypes()const
			{
				return parameterTypes.Wrap();
			}
			const ObjectType* ThisType()const
			{
				return thisType;
			}
			bool IsConstFunction()const
			{
				return true;
			}
			void* Invoke(void* object, void* funcPtrPtr, void** arguments)const
			{
				typename RemoveCVR<T0>::Type* argument0 = (typename RemoveCVR<T0>::Type*)arguments[0];
 				typename RemoveCVR<T1>::Type* argument1 = (typename RemoveCVR<T1>::Type*)arguments[1];
 
				R result=(((C*)object)->*(*((FunctionPointer*)funcPtrPtr)))(*argument0,*argument1);
				return new R(result);
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
				delete (FunctionPointer*)instance;
			}
		};
		template<typename C ,typename R,typename T0,typename T1>
		class ObjectTypeInformation<R(C::*)(T0,T1)const>
		{
		public:
			static WString GetName()
			{
				return ObjectTypeInformation<R>::GetName()+L"("+ObjectTypeInformation<C>::GetName()+L"::*)"+L"("+ObjectTypeInformation<T0>::GetName()+L","+ObjectTypeInformation<T1>::GetName()+L")"+L"const";
			}
			typedef ObjectTypeFunction<R(C::*)(T0,T1)const> ObjectTypeImplementation;
		};
		template<typename C ,typename R,typename T0,typename T1>
		class ObjectTypeRegister<R(C::*)(T0,T1)const>
		{
		public:
			static void Register()
			{
				ObjectTypeFunction<R(C::*)(T0,T1)const>* type=new ObjectTypeFunction<R(C::*)(T0,T1)const>;
				RegisterObjectType(ObjectTypeInformation<R(C::*)(T0,T1)const>::GetName(), type);
				type->parameterTypes.Add(TypeOf<T0>());
 				type->parameterTypes.Add(TypeOf<T1>());
 
				type->returnType=TypeOf<R>();
				type->thisType=TypeOf<C>();
				type->constructors.Add(new ObjectTypeFunction<R(C::*)(T0,T1)const>::Constructor<void*(*)()>(&ObjectTypeFunction<R(C::*)(T0,T1)const>::ctor));
			}
		};
 
/***********************************************************************
vl::objectmodel::ObjectTypeFunction<void(C::*)(T0,T1)const>
***********************************************************************/
		template<typename C  ,typename T0,typename T1>
		class ObjectTypeFunction<void(C::*)(T0,T1)const> : public SpecifiedObjectType<void(C::*)(T0,T1)const>
		{
			friend class ObjectTypeRegister<void(C::*)(T0,T1)const>;
			typedef void(C::*FunctionPointer)(T0,T1)const;
		protected:
			static void* ctor()
			{
				return new FunctionPointer(0);
			}
			collections::List<ObjectType*>		parameterTypes;
			ObjectType*							returnType;
			ObjectType*							thisType;
			collections::List<ObjectMember*>	constructors;
		public:
			ObjectTypeFunction()
			{
				returnType=0;
				thisType=0;
			}
			~ObjectTypeFunction()
			{
				for(int i=0;i<constructors.Count();i++)
				{
					delete constructors[i];
				}
			}
			ObjectCategory Category()const
			{
				return ObjectType::Function;
			}
			const ObjectType* ReturnType()const
			{
				return returnType;
			}
			const TypeList& ParameterTypes()const
			{
				return parameterTypes.Wrap();
			}
			const ObjectType* ThisType()const
			{
				return thisType;
			}
			bool IsConstFunction()const
			{
				return true;
			}
			void* Invoke(void* object, void* funcPtrPtr, void** arguments)const
			{
				typename RemoveCVR<T0>::Type* argument0 = (typename RemoveCVR<T0>::Type*)arguments[0];
 				typename RemoveCVR<T1>::Type* argument1 = (typename RemoveCVR<T1>::Type*)arguments[1];
 
				 (((C*)object)->*(*((FunctionPointer*)funcPtrPtr)))(*argument0,*argument1);
				return 0;
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
				delete (FunctionPointer*)instance;
			}
		};
		template<typename C  ,typename T0,typename T1>
		class ObjectTypeInformation<void(C::*)(T0,T1)const>
		{
		public:
			static WString GetName()
			{
				return ObjectTypeInformation<void>::GetName()+L"("+ObjectTypeInformation<C>::GetName()+L"::*)"+L"("+ObjectTypeInformation<T0>::GetName()+L","+ObjectTypeInformation<T1>::GetName()+L")"+L"const";
			}
			typedef ObjectTypeFunction<void(C::*)(T0,T1)const> ObjectTypeImplementation;
		};
		template<typename C  ,typename T0,typename T1>
		class ObjectTypeRegister<void(C::*)(T0,T1)const>
		{
		public:
			static void Register()
			{
				ObjectTypeFunction<void(C::*)(T0,T1)const>* type=new ObjectTypeFunction<void(C::*)(T0,T1)const>;
				RegisterObjectType(ObjectTypeInformation<void(C::*)(T0,T1)const>::GetName(), type);
				type->parameterTypes.Add(TypeOf<T0>());
 				type->parameterTypes.Add(TypeOf<T1>());
 
				type->returnType=TypeOf<void>();
				type->thisType=TypeOf<C>();
				type->constructors.Add(new ObjectTypeFunction<void(C::*)(T0,T1)const>::Constructor<void*(*)()>(&ObjectTypeFunction<void(C::*)(T0,T1)const>::ctor));
			}
		};
  
/***********************************************************************
vl::objectmodel::ObjectTypeFunction<R(*)(T0,T1,T2) >
***********************************************************************/
		template<typename R,typename T0,typename T1,typename T2>
		class ObjectTypeFunction<R(*)(T0,T1,T2) > : public SpecifiedObjectType<R(*)(T0,T1,T2) >
		{
			friend class ObjectTypeRegister<R(*)(T0,T1,T2) >;
			typedef R(*FunctionPointer)(T0,T1,T2) ;
		protected:
			static void* ctor()
			{
				return new FunctionPointer(0);
			}
			collections::List<ObjectType*>		parameterTypes;
			ObjectType*							returnType;
			ObjectType*							thisType;
			collections::List<ObjectMember*>	constructors;
		public:
			ObjectTypeFunction()
			{
				returnType=0;
				thisType=0;
			}
			~ObjectTypeFunction()
			{
				for(int i=0;i<constructors.Count();i++)
				{
					delete constructors[i];
				}
			}
			ObjectCategory Category()const
			{
				return ObjectType::Function;
			}
			const ObjectType* ReturnType()const
			{
				return returnType;
			}
			const TypeList& ParameterTypes()const
			{
				return parameterTypes.Wrap();
			}
			const ObjectType* ThisType()const
			{
				return thisType;
			}
			bool IsConstFunction()const
			{
				return false;
			}
			void* Invoke(void* object, void* funcPtrPtr, void** arguments)const
			{
				typename RemoveCVR<T0>::Type* argument0 = (typename RemoveCVR<T0>::Type*)arguments[0];
 				typename RemoveCVR<T1>::Type* argument1 = (typename RemoveCVR<T1>::Type*)arguments[1];
 				typename RemoveCVR<T2>::Type* argument2 = (typename RemoveCVR<T2>::Type*)arguments[2];
 
				R result=(*((FunctionPointer*)funcPtrPtr))(*argument0,*argument1,*argument2);
				return new R(result);
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
				delete (FunctionPointer*)instance;
			}
		};
		template<typename R,typename T0,typename T1,typename T2>
		class ObjectTypeInformation<R(*)(T0,T1,T2) >
		{
		public:
			static WString GetName()
			{
				return ObjectTypeInformation<R>::GetName()+L"(*)"+L"("+ObjectTypeInformation<T0>::GetName()+L","+ObjectTypeInformation<T1>::GetName()+L","+ObjectTypeInformation<T2>::GetName()+L")" ;
			}
			typedef ObjectTypeFunction<R(*)(T0,T1,T2) > ObjectTypeImplementation;
		};
		template<typename R,typename T0,typename T1,typename T2>
		class ObjectTypeRegister<R(*)(T0,T1,T2) >
		{
		public:
			static void Register()
			{
				ObjectTypeFunction<R(*)(T0,T1,T2) >* type=new ObjectTypeFunction<R(*)(T0,T1,T2) >;
				RegisterObjectType(ObjectTypeInformation<R(*)(T0,T1,T2) >::GetName(), type);
				type->parameterTypes.Add(TypeOf<T0>());
 				type->parameterTypes.Add(TypeOf<T1>());
 				type->parameterTypes.Add(TypeOf<T2>());
 
				type->returnType=TypeOf<R>();
				type->thisType=0;
				type->constructors.Add(new ObjectTypeFunction<R(*)(T0,T1,T2) >::Constructor<void*(*)()>(&ObjectTypeFunction<R(*)(T0,T1,T2) >::ctor));
			}
		};
 
/***********************************************************************
vl::objectmodel::ObjectTypeFunction<void(*)(T0,T1,T2) >
***********************************************************************/
		template< typename T0,typename T1,typename T2>
		class ObjectTypeFunction<void(*)(T0,T1,T2) > : public SpecifiedObjectType<void(*)(T0,T1,T2) >
		{
			friend class ObjectTypeRegister<void(*)(T0,T1,T2) >;
			typedef void(*FunctionPointer)(T0,T1,T2) ;
		protected:
			static void* ctor()
			{
				return new FunctionPointer(0);
			}
			collections::List<ObjectType*>		parameterTypes;
			ObjectType*							returnType;
			ObjectType*							thisType;
			collections::List<ObjectMember*>	constructors;
		public:
			ObjectTypeFunction()
			{
				returnType=0;
				thisType=0;
			}
			~ObjectTypeFunction()
			{
				for(int i=0;i<constructors.Count();i++)
				{
					delete constructors[i];
				}
			}
			ObjectCategory Category()const
			{
				return ObjectType::Function;
			}
			const ObjectType* ReturnType()const
			{
				return returnType;
			}
			const TypeList& ParameterTypes()const
			{
				return parameterTypes.Wrap();
			}
			const ObjectType* ThisType()const
			{
				return thisType;
			}
			bool IsConstFunction()const
			{
				return false;
			}
			void* Invoke(void* object, void* funcPtrPtr, void** arguments)const
			{
				typename RemoveCVR<T0>::Type* argument0 = (typename RemoveCVR<T0>::Type*)arguments[0];
 				typename RemoveCVR<T1>::Type* argument1 = (typename RemoveCVR<T1>::Type*)arguments[1];
 				typename RemoveCVR<T2>::Type* argument2 = (typename RemoveCVR<T2>::Type*)arguments[2];
 
				 (*((FunctionPointer*)funcPtrPtr))(*argument0,*argument1,*argument2);
				return 0;
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
				delete (FunctionPointer*)instance;
			}
		};
		template< typename T0,typename T1,typename T2>
		class ObjectTypeInformation<void(*)(T0,T1,T2) >
		{
		public:
			static WString GetName()
			{
				return ObjectTypeInformation<void>::GetName()+L"(*)"+L"("+ObjectTypeInformation<T0>::GetName()+L","+ObjectTypeInformation<T1>::GetName()+L","+ObjectTypeInformation<T2>::GetName()+L")" ;
			}
			typedef ObjectTypeFunction<void(*)(T0,T1,T2) > ObjectTypeImplementation;
		};
		template< typename T0,typename T1,typename T2>
		class ObjectTypeRegister<void(*)(T0,T1,T2) >
		{
		public:
			static void Register()
			{
				ObjectTypeFunction<void(*)(T0,T1,T2) >* type=new ObjectTypeFunction<void(*)(T0,T1,T2) >;
				RegisterObjectType(ObjectTypeInformation<void(*)(T0,T1,T2) >::GetName(), type);
				type->parameterTypes.Add(TypeOf<T0>());
 				type->parameterTypes.Add(TypeOf<T1>());
 				type->parameterTypes.Add(TypeOf<T2>());
 
				type->returnType=TypeOf<void>();
				type->thisType=0;
				type->constructors.Add(new ObjectTypeFunction<void(*)(T0,T1,T2) >::Constructor<void*(*)()>(&ObjectTypeFunction<void(*)(T0,T1,T2) >::ctor));
			}
		};
 
/***********************************************************************
vl::objectmodel::ObjectTypeFunction<R(C::*)(T0,T1,T2) >
***********************************************************************/
		template<typename C ,typename R,typename T0,typename T1,typename T2>
		class ObjectTypeFunction<R(C::*)(T0,T1,T2) > : public SpecifiedObjectType<R(C::*)(T0,T1,T2) >
		{
			friend class ObjectTypeRegister<R(C::*)(T0,T1,T2) >;
			typedef R(C::*FunctionPointer)(T0,T1,T2) ;
		protected:
			static void* ctor()
			{
				return new FunctionPointer(0);
			}
			collections::List<ObjectType*>		parameterTypes;
			ObjectType*							returnType;
			ObjectType*							thisType;
			collections::List<ObjectMember*>	constructors;
		public:
			ObjectTypeFunction()
			{
				returnType=0;
				thisType=0;
			}
			~ObjectTypeFunction()
			{
				for(int i=0;i<constructors.Count();i++)
				{
					delete constructors[i];
				}
			}
			ObjectCategory Category()const
			{
				return ObjectType::Function;
			}
			const ObjectType* ReturnType()const
			{
				return returnType;
			}
			const TypeList& ParameterTypes()const
			{
				return parameterTypes.Wrap();
			}
			const ObjectType* ThisType()const
			{
				return thisType;
			}
			bool IsConstFunction()const
			{
				return false;
			}
			void* Invoke(void* object, void* funcPtrPtr, void** arguments)const
			{
				typename RemoveCVR<T0>::Type* argument0 = (typename RemoveCVR<T0>::Type*)arguments[0];
 				typename RemoveCVR<T1>::Type* argument1 = (typename RemoveCVR<T1>::Type*)arguments[1];
 				typename RemoveCVR<T2>::Type* argument2 = (typename RemoveCVR<T2>::Type*)arguments[2];
 
				R result=(((C*)object)->*(*((FunctionPointer*)funcPtrPtr)))(*argument0,*argument1,*argument2);
				return new R(result);
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
				delete (FunctionPointer*)instance;
			}
		};
		template<typename C ,typename R,typename T0,typename T1,typename T2>
		class ObjectTypeInformation<R(C::*)(T0,T1,T2) >
		{
		public:
			static WString GetName()
			{
				return ObjectTypeInformation<R>::GetName()+L"("+ObjectTypeInformation<C>::GetName()+L"::*)"+L"("+ObjectTypeInformation<T0>::GetName()+L","+ObjectTypeInformation<T1>::GetName()+L","+ObjectTypeInformation<T2>::GetName()+L")" ;
			}
			typedef ObjectTypeFunction<R(C::*)(T0,T1,T2) > ObjectTypeImplementation;
		};
		template<typename C ,typename R,typename T0,typename T1,typename T2>
		class ObjectTypeRegister<R(C::*)(T0,T1,T2) >
		{
		public:
			static void Register()
			{
				ObjectTypeFunction<R(C::*)(T0,T1,T2) >* type=new ObjectTypeFunction<R(C::*)(T0,T1,T2) >;
				RegisterObjectType(ObjectTypeInformation<R(C::*)(T0,T1,T2) >::GetName(), type);
				type->parameterTypes.Add(TypeOf<T0>());
 				type->parameterTypes.Add(TypeOf<T1>());
 				type->parameterTypes.Add(TypeOf<T2>());
 
				type->returnType=TypeOf<R>();
				type->thisType=TypeOf<C>();
				type->constructors.Add(new ObjectTypeFunction<R(C::*)(T0,T1,T2) >::Constructor<void*(*)()>(&ObjectTypeFunction<R(C::*)(T0,T1,T2) >::ctor));
			}
		};
 
/***********************************************************************
vl::objectmodel::ObjectTypeFunction<void(C::*)(T0,T1,T2) >
***********************************************************************/
		template<typename C  ,typename T0,typename T1,typename T2>
		class ObjectTypeFunction<void(C::*)(T0,T1,T2) > : public SpecifiedObjectType<void(C::*)(T0,T1,T2) >
		{
			friend class ObjectTypeRegister<void(C::*)(T0,T1,T2) >;
			typedef void(C::*FunctionPointer)(T0,T1,T2) ;
		protected:
			static void* ctor()
			{
				return new FunctionPointer(0);
			}
			collections::List<ObjectType*>		parameterTypes;
			ObjectType*							returnType;
			ObjectType*							thisType;
			collections::List<ObjectMember*>	constructors;
		public:
			ObjectTypeFunction()
			{
				returnType=0;
				thisType=0;
			}
			~ObjectTypeFunction()
			{
				for(int i=0;i<constructors.Count();i++)
				{
					delete constructors[i];
				}
			}
			ObjectCategory Category()const
			{
				return ObjectType::Function;
			}
			const ObjectType* ReturnType()const
			{
				return returnType;
			}
			const TypeList& ParameterTypes()const
			{
				return parameterTypes.Wrap();
			}
			const ObjectType* ThisType()const
			{
				return thisType;
			}
			bool IsConstFunction()const
			{
				return false;
			}
			void* Invoke(void* object, void* funcPtrPtr, void** arguments)const
			{
				typename RemoveCVR<T0>::Type* argument0 = (typename RemoveCVR<T0>::Type*)arguments[0];
 				typename RemoveCVR<T1>::Type* argument1 = (typename RemoveCVR<T1>::Type*)arguments[1];
 				typename RemoveCVR<T2>::Type* argument2 = (typename RemoveCVR<T2>::Type*)arguments[2];
 
				 (((C*)object)->*(*((FunctionPointer*)funcPtrPtr)))(*argument0,*argument1,*argument2);
				return 0;
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
				delete (FunctionPointer*)instance;
			}
		};
		template<typename C  ,typename T0,typename T1,typename T2>
		class ObjectTypeInformation<void(C::*)(T0,T1,T2) >
		{
		public:
			static WString GetName()
			{
				return ObjectTypeInformation<void>::GetName()+L"("+ObjectTypeInformation<C>::GetName()+L"::*)"+L"("+ObjectTypeInformation<T0>::GetName()+L","+ObjectTypeInformation<T1>::GetName()+L","+ObjectTypeInformation<T2>::GetName()+L")" ;
			}
			typedef ObjectTypeFunction<void(C::*)(T0,T1,T2) > ObjectTypeImplementation;
		};
		template<typename C  ,typename T0,typename T1,typename T2>
		class ObjectTypeRegister<void(C::*)(T0,T1,T2) >
		{
		public:
			static void Register()
			{
				ObjectTypeFunction<void(C::*)(T0,T1,T2) >* type=new ObjectTypeFunction<void(C::*)(T0,T1,T2) >;
				RegisterObjectType(ObjectTypeInformation<void(C::*)(T0,T1,T2) >::GetName(), type);
				type->parameterTypes.Add(TypeOf<T0>());
 				type->parameterTypes.Add(TypeOf<T1>());
 				type->parameterTypes.Add(TypeOf<T2>());
 
				type->returnType=TypeOf<void>();
				type->thisType=TypeOf<C>();
				type->constructors.Add(new ObjectTypeFunction<void(C::*)(T0,T1,T2) >::Constructor<void*(*)()>(&ObjectTypeFunction<void(C::*)(T0,T1,T2) >::ctor));
			}
		};
 
/***********************************************************************
vl::objectmodel::ObjectTypeFunction<R(C::*)(T0,T1,T2)const>
***********************************************************************/
		template<typename C ,typename R,typename T0,typename T1,typename T2>
		class ObjectTypeFunction<R(C::*)(T0,T1,T2)const> : public SpecifiedObjectType<R(C::*)(T0,T1,T2)const>
		{
			friend class ObjectTypeRegister<R(C::*)(T0,T1,T2)const>;
			typedef R(C::*FunctionPointer)(T0,T1,T2)const;
		protected:
			static void* ctor()
			{
				return new FunctionPointer(0);
			}
			collections::List<ObjectType*>		parameterTypes;
			ObjectType*							returnType;
			ObjectType*							thisType;
			collections::List<ObjectMember*>	constructors;
		public:
			ObjectTypeFunction()
			{
				returnType=0;
				thisType=0;
			}
			~ObjectTypeFunction()
			{
				for(int i=0;i<constructors.Count();i++)
				{
					delete constructors[i];
				}
			}
			ObjectCategory Category()const
			{
				return ObjectType::Function;
			}
			const ObjectType* ReturnType()const
			{
				return returnType;
			}
			const TypeList& ParameterTypes()const
			{
				return parameterTypes.Wrap();
			}
			const ObjectType* ThisType()const
			{
				return thisType;
			}
			bool IsConstFunction()const
			{
				return true;
			}
			void* Invoke(void* object, void* funcPtrPtr, void** arguments)const
			{
				typename RemoveCVR<T0>::Type* argument0 = (typename RemoveCVR<T0>::Type*)arguments[0];
 				typename RemoveCVR<T1>::Type* argument1 = (typename RemoveCVR<T1>::Type*)arguments[1];
 				typename RemoveCVR<T2>::Type* argument2 = (typename RemoveCVR<T2>::Type*)arguments[2];
 
				R result=(((C*)object)->*(*((FunctionPointer*)funcPtrPtr)))(*argument0,*argument1,*argument2);
				return new R(result);
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
				delete (FunctionPointer*)instance;
			}
		};
		template<typename C ,typename R,typename T0,typename T1,typename T2>
		class ObjectTypeInformation<R(C::*)(T0,T1,T2)const>
		{
		public:
			static WString GetName()
			{
				return ObjectTypeInformation<R>::GetName()+L"("+ObjectTypeInformation<C>::GetName()+L"::*)"+L"("+ObjectTypeInformation<T0>::GetName()+L","+ObjectTypeInformation<T1>::GetName()+L","+ObjectTypeInformation<T2>::GetName()+L")"+L"const";
			}
			typedef ObjectTypeFunction<R(C::*)(T0,T1,T2)const> ObjectTypeImplementation;
		};
		template<typename C ,typename R,typename T0,typename T1,typename T2>
		class ObjectTypeRegister<R(C::*)(T0,T1,T2)const>
		{
		public:
			static void Register()
			{
				ObjectTypeFunction<R(C::*)(T0,T1,T2)const>* type=new ObjectTypeFunction<R(C::*)(T0,T1,T2)const>;
				RegisterObjectType(ObjectTypeInformation<R(C::*)(T0,T1,T2)const>::GetName(), type);
				type->parameterTypes.Add(TypeOf<T0>());
 				type->parameterTypes.Add(TypeOf<T1>());
 				type->parameterTypes.Add(TypeOf<T2>());
 
				type->returnType=TypeOf<R>();
				type->thisType=TypeOf<C>();
				type->constructors.Add(new ObjectTypeFunction<R(C::*)(T0,T1,T2)const>::Constructor<void*(*)()>(&ObjectTypeFunction<R(C::*)(T0,T1,T2)const>::ctor));
			}
		};
 
/***********************************************************************
vl::objectmodel::ObjectTypeFunction<void(C::*)(T0,T1,T2)const>
***********************************************************************/
		template<typename C  ,typename T0,typename T1,typename T2>
		class ObjectTypeFunction<void(C::*)(T0,T1,T2)const> : public SpecifiedObjectType<void(C::*)(T0,T1,T2)const>
		{
			friend class ObjectTypeRegister<void(C::*)(T0,T1,T2)const>;
			typedef void(C::*FunctionPointer)(T0,T1,T2)const;
		protected:
			static void* ctor()
			{
				return new FunctionPointer(0);
			}
			collections::List<ObjectType*>		parameterTypes;
			ObjectType*							returnType;
			ObjectType*							thisType;
			collections::List<ObjectMember*>	constructors;
		public:
			ObjectTypeFunction()
			{
				returnType=0;
				thisType=0;
			}
			~ObjectTypeFunction()
			{
				for(int i=0;i<constructors.Count();i++)
				{
					delete constructors[i];
				}
			}
			ObjectCategory Category()const
			{
				return ObjectType::Function;
			}
			const ObjectType* ReturnType()const
			{
				return returnType;
			}
			const TypeList& ParameterTypes()const
			{
				return parameterTypes.Wrap();
			}
			const ObjectType* ThisType()const
			{
				return thisType;
			}
			bool IsConstFunction()const
			{
				return true;
			}
			void* Invoke(void* object, void* funcPtrPtr, void** arguments)const
			{
				typename RemoveCVR<T0>::Type* argument0 = (typename RemoveCVR<T0>::Type*)arguments[0];
 				typename RemoveCVR<T1>::Type* argument1 = (typename RemoveCVR<T1>::Type*)arguments[1];
 				typename RemoveCVR<T2>::Type* argument2 = (typename RemoveCVR<T2>::Type*)arguments[2];
 
				 (((C*)object)->*(*((FunctionPointer*)funcPtrPtr)))(*argument0,*argument1,*argument2);
				return 0;
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
				delete (FunctionPointer*)instance;
			}
		};
		template<typename C  ,typename T0,typename T1,typename T2>
		class ObjectTypeInformation<void(C::*)(T0,T1,T2)const>
		{
		public:
			static WString GetName()
			{
				return ObjectTypeInformation<void>::GetName()+L"("+ObjectTypeInformation<C>::GetName()+L"::*)"+L"("+ObjectTypeInformation<T0>::GetName()+L","+ObjectTypeInformation<T1>::GetName()+L","+ObjectTypeInformation<T2>::GetName()+L")"+L"const";
			}
			typedef ObjectTypeFunction<void(C::*)(T0,T1,T2)const> ObjectTypeImplementation;
		};
		template<typename C  ,typename T0,typename T1,typename T2>
		class ObjectTypeRegister<void(C::*)(T0,T1,T2)const>
		{
		public:
			static void Register()
			{
				ObjectTypeFunction<void(C::*)(T0,T1,T2)const>* type=new ObjectTypeFunction<void(C::*)(T0,T1,T2)const>;
				RegisterObjectType(ObjectTypeInformation<void(C::*)(T0,T1,T2)const>::GetName(), type);
				type->parameterTypes.Add(TypeOf<T0>());
 				type->parameterTypes.Add(TypeOf<T1>());
 				type->parameterTypes.Add(TypeOf<T2>());
 
				type->returnType=TypeOf<void>();
				type->thisType=TypeOf<C>();
				type->constructors.Add(new ObjectTypeFunction<void(C::*)(T0,T1,T2)const>::Constructor<void*(*)()>(&ObjectTypeFunction<void(C::*)(T0,T1,T2)const>::ctor));
			}
		};
  
/***********************************************************************
vl::objectmodel::ObjectTypeFunction<R(*)(T0,T1,T2,T3) >
***********************************************************************/
		template<typename R,typename T0,typename T1,typename T2,typename T3>
		class ObjectTypeFunction<R(*)(T0,T1,T2,T3) > : public SpecifiedObjectType<R(*)(T0,T1,T2,T3) >
		{
			friend class ObjectTypeRegister<R(*)(T0,T1,T2,T3) >;
			typedef R(*FunctionPointer)(T0,T1,T2,T3) ;
		protected:
			static void* ctor()
			{
				return new FunctionPointer(0);
			}
			collections::List<ObjectType*>		parameterTypes;
			ObjectType*							returnType;
			ObjectType*							thisType;
			collections::List<ObjectMember*>	constructors;
		public:
			ObjectTypeFunction()
			{
				returnType=0;
				thisType=0;
			}
			~ObjectTypeFunction()
			{
				for(int i=0;i<constructors.Count();i++)
				{
					delete constructors[i];
				}
			}
			ObjectCategory Category()const
			{
				return ObjectType::Function;
			}
			const ObjectType* ReturnType()const
			{
				return returnType;
			}
			const TypeList& ParameterTypes()const
			{
				return parameterTypes.Wrap();
			}
			const ObjectType* ThisType()const
			{
				return thisType;
			}
			bool IsConstFunction()const
			{
				return false;
			}
			void* Invoke(void* object, void* funcPtrPtr, void** arguments)const
			{
				typename RemoveCVR<T0>::Type* argument0 = (typename RemoveCVR<T0>::Type*)arguments[0];
 				typename RemoveCVR<T1>::Type* argument1 = (typename RemoveCVR<T1>::Type*)arguments[1];
 				typename RemoveCVR<T2>::Type* argument2 = (typename RemoveCVR<T2>::Type*)arguments[2];
 				typename RemoveCVR<T3>::Type* argument3 = (typename RemoveCVR<T3>::Type*)arguments[3];
 
				R result=(*((FunctionPointer*)funcPtrPtr))(*argument0,*argument1,*argument2,*argument3);
				return new R(result);
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
				delete (FunctionPointer*)instance;
			}
		};
		template<typename R,typename T0,typename T1,typename T2,typename T3>
		class ObjectTypeInformation<R(*)(T0,T1,T2,T3) >
		{
		public:
			static WString GetName()
			{
				return ObjectTypeInformation<R>::GetName()+L"(*)"+L"("+ObjectTypeInformation<T0>::GetName()+L","+ObjectTypeInformation<T1>::GetName()+L","+ObjectTypeInformation<T2>::GetName()+L","+ObjectTypeInformation<T3>::GetName()+L")" ;
			}
			typedef ObjectTypeFunction<R(*)(T0,T1,T2,T3) > ObjectTypeImplementation;
		};
		template<typename R,typename T0,typename T1,typename T2,typename T3>
		class ObjectTypeRegister<R(*)(T0,T1,T2,T3) >
		{
		public:
			static void Register()
			{
				ObjectTypeFunction<R(*)(T0,T1,T2,T3) >* type=new ObjectTypeFunction<R(*)(T0,T1,T2,T3) >;
				RegisterObjectType(ObjectTypeInformation<R(*)(T0,T1,T2,T3) >::GetName(), type);
				type->parameterTypes.Add(TypeOf<T0>());
 				type->parameterTypes.Add(TypeOf<T1>());
 				type->parameterTypes.Add(TypeOf<T2>());
 				type->parameterTypes.Add(TypeOf<T3>());
 
				type->returnType=TypeOf<R>();
				type->thisType=0;
				type->constructors.Add(new ObjectTypeFunction<R(*)(T0,T1,T2,T3) >::Constructor<void*(*)()>(&ObjectTypeFunction<R(*)(T0,T1,T2,T3) >::ctor));
			}
		};
 
/***********************************************************************
vl::objectmodel::ObjectTypeFunction<void(*)(T0,T1,T2,T3) >
***********************************************************************/
		template< typename T0,typename T1,typename T2,typename T3>
		class ObjectTypeFunction<void(*)(T0,T1,T2,T3) > : public SpecifiedObjectType<void(*)(T0,T1,T2,T3) >
		{
			friend class ObjectTypeRegister<void(*)(T0,T1,T2,T3) >;
			typedef void(*FunctionPointer)(T0,T1,T2,T3) ;
		protected:
			static void* ctor()
			{
				return new FunctionPointer(0);
			}
			collections::List<ObjectType*>		parameterTypes;
			ObjectType*							returnType;
			ObjectType*							thisType;
			collections::List<ObjectMember*>	constructors;
		public:
			ObjectTypeFunction()
			{
				returnType=0;
				thisType=0;
			}
			~ObjectTypeFunction()
			{
				for(int i=0;i<constructors.Count();i++)
				{
					delete constructors[i];
				}
			}
			ObjectCategory Category()const
			{
				return ObjectType::Function;
			}
			const ObjectType* ReturnType()const
			{
				return returnType;
			}
			const TypeList& ParameterTypes()const
			{
				return parameterTypes.Wrap();
			}
			const ObjectType* ThisType()const
			{
				return thisType;
			}
			bool IsConstFunction()const
			{
				return false;
			}
			void* Invoke(void* object, void* funcPtrPtr, void** arguments)const
			{
				typename RemoveCVR<T0>::Type* argument0 = (typename RemoveCVR<T0>::Type*)arguments[0];
 				typename RemoveCVR<T1>::Type* argument1 = (typename RemoveCVR<T1>::Type*)arguments[1];
 				typename RemoveCVR<T2>::Type* argument2 = (typename RemoveCVR<T2>::Type*)arguments[2];
 				typename RemoveCVR<T3>::Type* argument3 = (typename RemoveCVR<T3>::Type*)arguments[3];
 
				 (*((FunctionPointer*)funcPtrPtr))(*argument0,*argument1,*argument2,*argument3);
				return 0;
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
				delete (FunctionPointer*)instance;
			}
		};
		template< typename T0,typename T1,typename T2,typename T3>
		class ObjectTypeInformation<void(*)(T0,T1,T2,T3) >
		{
		public:
			static WString GetName()
			{
				return ObjectTypeInformation<void>::GetName()+L"(*)"+L"("+ObjectTypeInformation<T0>::GetName()+L","+ObjectTypeInformation<T1>::GetName()+L","+ObjectTypeInformation<T2>::GetName()+L","+ObjectTypeInformation<T3>::GetName()+L")" ;
			}
			typedef ObjectTypeFunction<void(*)(T0,T1,T2,T3) > ObjectTypeImplementation;
		};
		template< typename T0,typename T1,typename T2,typename T3>
		class ObjectTypeRegister<void(*)(T0,T1,T2,T3) >
		{
		public:
			static void Register()
			{
				ObjectTypeFunction<void(*)(T0,T1,T2,T3) >* type=new ObjectTypeFunction<void(*)(T0,T1,T2,T3) >;
				RegisterObjectType(ObjectTypeInformation<void(*)(T0,T1,T2,T3) >::GetName(), type);
				type->parameterTypes.Add(TypeOf<T0>());
 				type->parameterTypes.Add(TypeOf<T1>());
 				type->parameterTypes.Add(TypeOf<T2>());
 				type->parameterTypes.Add(TypeOf<T3>());
 
				type->returnType=TypeOf<void>();
				type->thisType=0;
				type->constructors.Add(new ObjectTypeFunction<void(*)(T0,T1,T2,T3) >::Constructor<void*(*)()>(&ObjectTypeFunction<void(*)(T0,T1,T2,T3) >::ctor));
			}
		};
 
/***********************************************************************
vl::objectmodel::ObjectTypeFunction<R(C::*)(T0,T1,T2,T3) >
***********************************************************************/
		template<typename C ,typename R,typename T0,typename T1,typename T2,typename T3>
		class ObjectTypeFunction<R(C::*)(T0,T1,T2,T3) > : public SpecifiedObjectType<R(C::*)(T0,T1,T2,T3) >
		{
			friend class ObjectTypeRegister<R(C::*)(T0,T1,T2,T3) >;
			typedef R(C::*FunctionPointer)(T0,T1,T2,T3) ;
		protected:
			static void* ctor()
			{
				return new FunctionPointer(0);
			}
			collections::List<ObjectType*>		parameterTypes;
			ObjectType*							returnType;
			ObjectType*							thisType;
			collections::List<ObjectMember*>	constructors;
		public:
			ObjectTypeFunction()
			{
				returnType=0;
				thisType=0;
			}
			~ObjectTypeFunction()
			{
				for(int i=0;i<constructors.Count();i++)
				{
					delete constructors[i];
				}
			}
			ObjectCategory Category()const
			{
				return ObjectType::Function;
			}
			const ObjectType* ReturnType()const
			{
				return returnType;
			}
			const TypeList& ParameterTypes()const
			{
				return parameterTypes.Wrap();
			}
			const ObjectType* ThisType()const
			{
				return thisType;
			}
			bool IsConstFunction()const
			{
				return false;
			}
			void* Invoke(void* object, void* funcPtrPtr, void** arguments)const
			{
				typename RemoveCVR<T0>::Type* argument0 = (typename RemoveCVR<T0>::Type*)arguments[0];
 				typename RemoveCVR<T1>::Type* argument1 = (typename RemoveCVR<T1>::Type*)arguments[1];
 				typename RemoveCVR<T2>::Type* argument2 = (typename RemoveCVR<T2>::Type*)arguments[2];
 				typename RemoveCVR<T3>::Type* argument3 = (typename RemoveCVR<T3>::Type*)arguments[3];
 
				R result=(((C*)object)->*(*((FunctionPointer*)funcPtrPtr)))(*argument0,*argument1,*argument2,*argument3);
				return new R(result);
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
				delete (FunctionPointer*)instance;
			}
		};
		template<typename C ,typename R,typename T0,typename T1,typename T2,typename T3>
		class ObjectTypeInformation<R(C::*)(T0,T1,T2,T3) >
		{
		public:
			static WString GetName()
			{
				return ObjectTypeInformation<R>::GetName()+L"("+ObjectTypeInformation<C>::GetName()+L"::*)"+L"("+ObjectTypeInformation<T0>::GetName()+L","+ObjectTypeInformation<T1>::GetName()+L","+ObjectTypeInformation<T2>::GetName()+L","+ObjectTypeInformation<T3>::GetName()+L")" ;
			}
			typedef ObjectTypeFunction<R(C::*)(T0,T1,T2,T3) > ObjectTypeImplementation;
		};
		template<typename C ,typename R,typename T0,typename T1,typename T2,typename T3>
		class ObjectTypeRegister<R(C::*)(T0,T1,T2,T3) >
		{
		public:
			static void Register()
			{
				ObjectTypeFunction<R(C::*)(T0,T1,T2,T3) >* type=new ObjectTypeFunction<R(C::*)(T0,T1,T2,T3) >;
				RegisterObjectType(ObjectTypeInformation<R(C::*)(T0,T1,T2,T3) >::GetName(), type);
				type->parameterTypes.Add(TypeOf<T0>());
 				type->parameterTypes.Add(TypeOf<T1>());
 				type->parameterTypes.Add(TypeOf<T2>());
 				type->parameterTypes.Add(TypeOf<T3>());
 
				type->returnType=TypeOf<R>();
				type->thisType=TypeOf<C>();
				type->constructors.Add(new ObjectTypeFunction<R(C::*)(T0,T1,T2,T3) >::Constructor<void*(*)()>(&ObjectTypeFunction<R(C::*)(T0,T1,T2,T3) >::ctor));
			}
		};
 
/***********************************************************************
vl::objectmodel::ObjectTypeFunction<void(C::*)(T0,T1,T2,T3) >
***********************************************************************/
		template<typename C  ,typename T0,typename T1,typename T2,typename T3>
		class ObjectTypeFunction<void(C::*)(T0,T1,T2,T3) > : public SpecifiedObjectType<void(C::*)(T0,T1,T2,T3) >
		{
			friend class ObjectTypeRegister<void(C::*)(T0,T1,T2,T3) >;
			typedef void(C::*FunctionPointer)(T0,T1,T2,T3) ;
		protected:
			static void* ctor()
			{
				return new FunctionPointer(0);
			}
			collections::List<ObjectType*>		parameterTypes;
			ObjectType*							returnType;
			ObjectType*							thisType;
			collections::List<ObjectMember*>	constructors;
		public:
			ObjectTypeFunction()
			{
				returnType=0;
				thisType=0;
			}
			~ObjectTypeFunction()
			{
				for(int i=0;i<constructors.Count();i++)
				{
					delete constructors[i];
				}
			}
			ObjectCategory Category()const
			{
				return ObjectType::Function;
			}
			const ObjectType* ReturnType()const
			{
				return returnType;
			}
			const TypeList& ParameterTypes()const
			{
				return parameterTypes.Wrap();
			}
			const ObjectType* ThisType()const
			{
				return thisType;
			}
			bool IsConstFunction()const
			{
				return false;
			}
			void* Invoke(void* object, void* funcPtrPtr, void** arguments)const
			{
				typename RemoveCVR<T0>::Type* argument0 = (typename RemoveCVR<T0>::Type*)arguments[0];
 				typename RemoveCVR<T1>::Type* argument1 = (typename RemoveCVR<T1>::Type*)arguments[1];
 				typename RemoveCVR<T2>::Type* argument2 = (typename RemoveCVR<T2>::Type*)arguments[2];
 				typename RemoveCVR<T3>::Type* argument3 = (typename RemoveCVR<T3>::Type*)arguments[3];
 
				 (((C*)object)->*(*((FunctionPointer*)funcPtrPtr)))(*argument0,*argument1,*argument2,*argument3);
				return 0;
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
				delete (FunctionPointer*)instance;
			}
		};
		template<typename C  ,typename T0,typename T1,typename T2,typename T3>
		class ObjectTypeInformation<void(C::*)(T0,T1,T2,T3) >
		{
		public:
			static WString GetName()
			{
				return ObjectTypeInformation<void>::GetName()+L"("+ObjectTypeInformation<C>::GetName()+L"::*)"+L"("+ObjectTypeInformation<T0>::GetName()+L","+ObjectTypeInformation<T1>::GetName()+L","+ObjectTypeInformation<T2>::GetName()+L","+ObjectTypeInformation<T3>::GetName()+L")" ;
			}
			typedef ObjectTypeFunction<void(C::*)(T0,T1,T2,T3) > ObjectTypeImplementation;
		};
		template<typename C  ,typename T0,typename T1,typename T2,typename T3>
		class ObjectTypeRegister<void(C::*)(T0,T1,T2,T3) >
		{
		public:
			static void Register()
			{
				ObjectTypeFunction<void(C::*)(T0,T1,T2,T3) >* type=new ObjectTypeFunction<void(C::*)(T0,T1,T2,T3) >;
				RegisterObjectType(ObjectTypeInformation<void(C::*)(T0,T1,T2,T3) >::GetName(), type);
				type->parameterTypes.Add(TypeOf<T0>());
 				type->parameterTypes.Add(TypeOf<T1>());
 				type->parameterTypes.Add(TypeOf<T2>());
 				type->parameterTypes.Add(TypeOf<T3>());
 
				type->returnType=TypeOf<void>();
				type->thisType=TypeOf<C>();
				type->constructors.Add(new ObjectTypeFunction<void(C::*)(T0,T1,T2,T3) >::Constructor<void*(*)()>(&ObjectTypeFunction<void(C::*)(T0,T1,T2,T3) >::ctor));
			}
		};
 
/***********************************************************************
vl::objectmodel::ObjectTypeFunction<R(C::*)(T0,T1,T2,T3)const>
***********************************************************************/
		template<typename C ,typename R,typename T0,typename T1,typename T2,typename T3>
		class ObjectTypeFunction<R(C::*)(T0,T1,T2,T3)const> : public SpecifiedObjectType<R(C::*)(T0,T1,T2,T3)const>
		{
			friend class ObjectTypeRegister<R(C::*)(T0,T1,T2,T3)const>;
			typedef R(C::*FunctionPointer)(T0,T1,T2,T3)const;
		protected:
			static void* ctor()
			{
				return new FunctionPointer(0);
			}
			collections::List<ObjectType*>		parameterTypes;
			ObjectType*							returnType;
			ObjectType*							thisType;
			collections::List<ObjectMember*>	constructors;
		public:
			ObjectTypeFunction()
			{
				returnType=0;
				thisType=0;
			}
			~ObjectTypeFunction()
			{
				for(int i=0;i<constructors.Count();i++)
				{
					delete constructors[i];
				}
			}
			ObjectCategory Category()const
			{
				return ObjectType::Function;
			}
			const ObjectType* ReturnType()const
			{
				return returnType;
			}
			const TypeList& ParameterTypes()const
			{
				return parameterTypes.Wrap();
			}
			const ObjectType* ThisType()const
			{
				return thisType;
			}
			bool IsConstFunction()const
			{
				return true;
			}
			void* Invoke(void* object, void* funcPtrPtr, void** arguments)const
			{
				typename RemoveCVR<T0>::Type* argument0 = (typename RemoveCVR<T0>::Type*)arguments[0];
 				typename RemoveCVR<T1>::Type* argument1 = (typename RemoveCVR<T1>::Type*)arguments[1];
 				typename RemoveCVR<T2>::Type* argument2 = (typename RemoveCVR<T2>::Type*)arguments[2];
 				typename RemoveCVR<T3>::Type* argument3 = (typename RemoveCVR<T3>::Type*)arguments[3];
 
				R result=(((C*)object)->*(*((FunctionPointer*)funcPtrPtr)))(*argument0,*argument1,*argument2,*argument3);
				return new R(result);
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
				delete (FunctionPointer*)instance;
			}
		};
		template<typename C ,typename R,typename T0,typename T1,typename T2,typename T3>
		class ObjectTypeInformation<R(C::*)(T0,T1,T2,T3)const>
		{
		public:
			static WString GetName()
			{
				return ObjectTypeInformation<R>::GetName()+L"("+ObjectTypeInformation<C>::GetName()+L"::*)"+L"("+ObjectTypeInformation<T0>::GetName()+L","+ObjectTypeInformation<T1>::GetName()+L","+ObjectTypeInformation<T2>::GetName()+L","+ObjectTypeInformation<T3>::GetName()+L")"+L"const";
			}
			typedef ObjectTypeFunction<R(C::*)(T0,T1,T2,T3)const> ObjectTypeImplementation;
		};
		template<typename C ,typename R,typename T0,typename T1,typename T2,typename T3>
		class ObjectTypeRegister<R(C::*)(T0,T1,T2,T3)const>
		{
		public:
			static void Register()
			{
				ObjectTypeFunction<R(C::*)(T0,T1,T2,T3)const>* type=new ObjectTypeFunction<R(C::*)(T0,T1,T2,T3)const>;
				RegisterObjectType(ObjectTypeInformation<R(C::*)(T0,T1,T2,T3)const>::GetName(), type);
				type->parameterTypes.Add(TypeOf<T0>());
 				type->parameterTypes.Add(TypeOf<T1>());
 				type->parameterTypes.Add(TypeOf<T2>());
 				type->parameterTypes.Add(TypeOf<T3>());
 
				type->returnType=TypeOf<R>();
				type->thisType=TypeOf<C>();
				type->constructors.Add(new ObjectTypeFunction<R(C::*)(T0,T1,T2,T3)const>::Constructor<void*(*)()>(&ObjectTypeFunction<R(C::*)(T0,T1,T2,T3)const>::ctor));
			}
		};
 
/***********************************************************************
vl::objectmodel::ObjectTypeFunction<void(C::*)(T0,T1,T2,T3)const>
***********************************************************************/
		template<typename C  ,typename T0,typename T1,typename T2,typename T3>
		class ObjectTypeFunction<void(C::*)(T0,T1,T2,T3)const> : public SpecifiedObjectType<void(C::*)(T0,T1,T2,T3)const>
		{
			friend class ObjectTypeRegister<void(C::*)(T0,T1,T2,T3)const>;
			typedef void(C::*FunctionPointer)(T0,T1,T2,T3)const;
		protected:
			static void* ctor()
			{
				return new FunctionPointer(0);
			}
			collections::List<ObjectType*>		parameterTypes;
			ObjectType*							returnType;
			ObjectType*							thisType;
			collections::List<ObjectMember*>	constructors;
		public:
			ObjectTypeFunction()
			{
				returnType=0;
				thisType=0;
			}
			~ObjectTypeFunction()
			{
				for(int i=0;i<constructors.Count();i++)
				{
					delete constructors[i];
				}
			}
			ObjectCategory Category()const
			{
				return ObjectType::Function;
			}
			const ObjectType* ReturnType()const
			{
				return returnType;
			}
			const TypeList& ParameterTypes()const
			{
				return parameterTypes.Wrap();
			}
			const ObjectType* ThisType()const
			{
				return thisType;
			}
			bool IsConstFunction()const
			{
				return true;
			}
			void* Invoke(void* object, void* funcPtrPtr, void** arguments)const
			{
				typename RemoveCVR<T0>::Type* argument0 = (typename RemoveCVR<T0>::Type*)arguments[0];
 				typename RemoveCVR<T1>::Type* argument1 = (typename RemoveCVR<T1>::Type*)arguments[1];
 				typename RemoveCVR<T2>::Type* argument2 = (typename RemoveCVR<T2>::Type*)arguments[2];
 				typename RemoveCVR<T3>::Type* argument3 = (typename RemoveCVR<T3>::Type*)arguments[3];
 
				 (((C*)object)->*(*((FunctionPointer*)funcPtrPtr)))(*argument0,*argument1,*argument2,*argument3);
				return 0;
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
				delete (FunctionPointer*)instance;
			}
		};
		template<typename C  ,typename T0,typename T1,typename T2,typename T3>
		class ObjectTypeInformation<void(C::*)(T0,T1,T2,T3)const>
		{
		public:
			static WString GetName()
			{
				return ObjectTypeInformation<void>::GetName()+L"("+ObjectTypeInformation<C>::GetName()+L"::*)"+L"("+ObjectTypeInformation<T0>::GetName()+L","+ObjectTypeInformation<T1>::GetName()+L","+ObjectTypeInformation<T2>::GetName()+L","+ObjectTypeInformation<T3>::GetName()+L")"+L"const";
			}
			typedef ObjectTypeFunction<void(C::*)(T0,T1,T2,T3)const> ObjectTypeImplementation;
		};
		template<typename C  ,typename T0,typename T1,typename T2,typename T3>
		class ObjectTypeRegister<void(C::*)(T0,T1,T2,T3)const>
		{
		public:
			static void Register()
			{
				ObjectTypeFunction<void(C::*)(T0,T1,T2,T3)const>* type=new ObjectTypeFunction<void(C::*)(T0,T1,T2,T3)const>;
				RegisterObjectType(ObjectTypeInformation<void(C::*)(T0,T1,T2,T3)const>::GetName(), type);
				type->parameterTypes.Add(TypeOf<T0>());
 				type->parameterTypes.Add(TypeOf<T1>());
 				type->parameterTypes.Add(TypeOf<T2>());
 				type->parameterTypes.Add(TypeOf<T3>());
 
				type->returnType=TypeOf<void>();
				type->thisType=TypeOf<C>();
				type->constructors.Add(new ObjectTypeFunction<void(C::*)(T0,T1,T2,T3)const>::Constructor<void*(*)()>(&ObjectTypeFunction<void(C::*)(T0,T1,T2,T3)const>::ctor));
			}
		};
  
/***********************************************************************
vl::objectmodel::ObjectTypeFunction<R(*)(T0,T1,T2,T3,T4) >
***********************************************************************/
		template<typename R,typename T0,typename T1,typename T2,typename T3,typename T4>
		class ObjectTypeFunction<R(*)(T0,T1,T2,T3,T4) > : public SpecifiedObjectType<R(*)(T0,T1,T2,T3,T4) >
		{
			friend class ObjectTypeRegister<R(*)(T0,T1,T2,T3,T4) >;
			typedef R(*FunctionPointer)(T0,T1,T2,T3,T4) ;
		protected:
			static void* ctor()
			{
				return new FunctionPointer(0);
			}
			collections::List<ObjectType*>		parameterTypes;
			ObjectType*							returnType;
			ObjectType*							thisType;
			collections::List<ObjectMember*>	constructors;
		public:
			ObjectTypeFunction()
			{
				returnType=0;
				thisType=0;
			}
			~ObjectTypeFunction()
			{
				for(int i=0;i<constructors.Count();i++)
				{
					delete constructors[i];
				}
			}
			ObjectCategory Category()const
			{
				return ObjectType::Function;
			}
			const ObjectType* ReturnType()const
			{
				return returnType;
			}
			const TypeList& ParameterTypes()const
			{
				return parameterTypes.Wrap();
			}
			const ObjectType* ThisType()const
			{
				return thisType;
			}
			bool IsConstFunction()const
			{
				return false;
			}
			void* Invoke(void* object, void* funcPtrPtr, void** arguments)const
			{
				typename RemoveCVR<T0>::Type* argument0 = (typename RemoveCVR<T0>::Type*)arguments[0];
 				typename RemoveCVR<T1>::Type* argument1 = (typename RemoveCVR<T1>::Type*)arguments[1];
 				typename RemoveCVR<T2>::Type* argument2 = (typename RemoveCVR<T2>::Type*)arguments[2];
 				typename RemoveCVR<T3>::Type* argument3 = (typename RemoveCVR<T3>::Type*)arguments[3];
 				typename RemoveCVR<T4>::Type* argument4 = (typename RemoveCVR<T4>::Type*)arguments[4];
 
				R result=(*((FunctionPointer*)funcPtrPtr))(*argument0,*argument1,*argument2,*argument3,*argument4);
				return new R(result);
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
				delete (FunctionPointer*)instance;
			}
		};
		template<typename R,typename T0,typename T1,typename T2,typename T3,typename T4>
		class ObjectTypeInformation<R(*)(T0,T1,T2,T3,T4) >
		{
		public:
			static WString GetName()
			{
				return ObjectTypeInformation<R>::GetName()+L"(*)"+L"("+ObjectTypeInformation<T0>::GetName()+L","+ObjectTypeInformation<T1>::GetName()+L","+ObjectTypeInformation<T2>::GetName()+L","+ObjectTypeInformation<T3>::GetName()+L","+ObjectTypeInformation<T4>::GetName()+L")" ;
			}
			typedef ObjectTypeFunction<R(*)(T0,T1,T2,T3,T4) > ObjectTypeImplementation;
		};
		template<typename R,typename T0,typename T1,typename T2,typename T3,typename T4>
		class ObjectTypeRegister<R(*)(T0,T1,T2,T3,T4) >
		{
		public:
			static void Register()
			{
				ObjectTypeFunction<R(*)(T0,T1,T2,T3,T4) >* type=new ObjectTypeFunction<R(*)(T0,T1,T2,T3,T4) >;
				RegisterObjectType(ObjectTypeInformation<R(*)(T0,T1,T2,T3,T4) >::GetName(), type);
				type->parameterTypes.Add(TypeOf<T0>());
 				type->parameterTypes.Add(TypeOf<T1>());
 				type->parameterTypes.Add(TypeOf<T2>());
 				type->parameterTypes.Add(TypeOf<T3>());
 				type->parameterTypes.Add(TypeOf<T4>());
 
				type->returnType=TypeOf<R>();
				type->thisType=0;
				type->constructors.Add(new ObjectTypeFunction<R(*)(T0,T1,T2,T3,T4) >::Constructor<void*(*)()>(&ObjectTypeFunction<R(*)(T0,T1,T2,T3,T4) >::ctor));
			}
		};
 
/***********************************************************************
vl::objectmodel::ObjectTypeFunction<void(*)(T0,T1,T2,T3,T4) >
***********************************************************************/
		template< typename T0,typename T1,typename T2,typename T3,typename T4>
		class ObjectTypeFunction<void(*)(T0,T1,T2,T3,T4) > : public SpecifiedObjectType<void(*)(T0,T1,T2,T3,T4) >
		{
			friend class ObjectTypeRegister<void(*)(T0,T1,T2,T3,T4) >;
			typedef void(*FunctionPointer)(T0,T1,T2,T3,T4) ;
		protected:
			static void* ctor()
			{
				return new FunctionPointer(0);
			}
			collections::List<ObjectType*>		parameterTypes;
			ObjectType*							returnType;
			ObjectType*							thisType;
			collections::List<ObjectMember*>	constructors;
		public:
			ObjectTypeFunction()
			{
				returnType=0;
				thisType=0;
			}
			~ObjectTypeFunction()
			{
				for(int i=0;i<constructors.Count();i++)
				{
					delete constructors[i];
				}
			}
			ObjectCategory Category()const
			{
				return ObjectType::Function;
			}
			const ObjectType* ReturnType()const
			{
				return returnType;
			}
			const TypeList& ParameterTypes()const
			{
				return parameterTypes.Wrap();
			}
			const ObjectType* ThisType()const
			{
				return thisType;
			}
			bool IsConstFunction()const
			{
				return false;
			}
			void* Invoke(void* object, void* funcPtrPtr, void** arguments)const
			{
				typename RemoveCVR<T0>::Type* argument0 = (typename RemoveCVR<T0>::Type*)arguments[0];
 				typename RemoveCVR<T1>::Type* argument1 = (typename RemoveCVR<T1>::Type*)arguments[1];
 				typename RemoveCVR<T2>::Type* argument2 = (typename RemoveCVR<T2>::Type*)arguments[2];
 				typename RemoveCVR<T3>::Type* argument3 = (typename RemoveCVR<T3>::Type*)arguments[3];
 				typename RemoveCVR<T4>::Type* argument4 = (typename RemoveCVR<T4>::Type*)arguments[4];
 
				 (*((FunctionPointer*)funcPtrPtr))(*argument0,*argument1,*argument2,*argument3,*argument4);
				return 0;
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
				delete (FunctionPointer*)instance;
			}
		};
		template< typename T0,typename T1,typename T2,typename T3,typename T4>
		class ObjectTypeInformation<void(*)(T0,T1,T2,T3,T4) >
		{
		public:
			static WString GetName()
			{
				return ObjectTypeInformation<void>::GetName()+L"(*)"+L"("+ObjectTypeInformation<T0>::GetName()+L","+ObjectTypeInformation<T1>::GetName()+L","+ObjectTypeInformation<T2>::GetName()+L","+ObjectTypeInformation<T3>::GetName()+L","+ObjectTypeInformation<T4>::GetName()+L")" ;
			}
			typedef ObjectTypeFunction<void(*)(T0,T1,T2,T3,T4) > ObjectTypeImplementation;
		};
		template< typename T0,typename T1,typename T2,typename T3,typename T4>
		class ObjectTypeRegister<void(*)(T0,T1,T2,T3,T4) >
		{
		public:
			static void Register()
			{
				ObjectTypeFunction<void(*)(T0,T1,T2,T3,T4) >* type=new ObjectTypeFunction<void(*)(T0,T1,T2,T3,T4) >;
				RegisterObjectType(ObjectTypeInformation<void(*)(T0,T1,T2,T3,T4) >::GetName(), type);
				type->parameterTypes.Add(TypeOf<T0>());
 				type->parameterTypes.Add(TypeOf<T1>());
 				type->parameterTypes.Add(TypeOf<T2>());
 				type->parameterTypes.Add(TypeOf<T3>());
 				type->parameterTypes.Add(TypeOf<T4>());
 
				type->returnType=TypeOf<void>();
				type->thisType=0;
				type->constructors.Add(new ObjectTypeFunction<void(*)(T0,T1,T2,T3,T4) >::Constructor<void*(*)()>(&ObjectTypeFunction<void(*)(T0,T1,T2,T3,T4) >::ctor));
			}
		};
 
/***********************************************************************
vl::objectmodel::ObjectTypeFunction<R(C::*)(T0,T1,T2,T3,T4) >
***********************************************************************/
		template<typename C ,typename R,typename T0,typename T1,typename T2,typename T3,typename T4>
		class ObjectTypeFunction<R(C::*)(T0,T1,T2,T3,T4) > : public SpecifiedObjectType<R(C::*)(T0,T1,T2,T3,T4) >
		{
			friend class ObjectTypeRegister<R(C::*)(T0,T1,T2,T3,T4) >;
			typedef R(C::*FunctionPointer)(T0,T1,T2,T3,T4) ;
		protected:
			static void* ctor()
			{
				return new FunctionPointer(0);
			}
			collections::List<ObjectType*>		parameterTypes;
			ObjectType*							returnType;
			ObjectType*							thisType;
			collections::List<ObjectMember*>	constructors;
		public:
			ObjectTypeFunction()
			{
				returnType=0;
				thisType=0;
			}
			~ObjectTypeFunction()
			{
				for(int i=0;i<constructors.Count();i++)
				{
					delete constructors[i];
				}
			}
			ObjectCategory Category()const
			{
				return ObjectType::Function;
			}
			const ObjectType* ReturnType()const
			{
				return returnType;
			}
			const TypeList& ParameterTypes()const
			{
				return parameterTypes.Wrap();
			}
			const ObjectType* ThisType()const
			{
				return thisType;
			}
			bool IsConstFunction()const
			{
				return false;
			}
			void* Invoke(void* object, void* funcPtrPtr, void** arguments)const
			{
				typename RemoveCVR<T0>::Type* argument0 = (typename RemoveCVR<T0>::Type*)arguments[0];
 				typename RemoveCVR<T1>::Type* argument1 = (typename RemoveCVR<T1>::Type*)arguments[1];
 				typename RemoveCVR<T2>::Type* argument2 = (typename RemoveCVR<T2>::Type*)arguments[2];
 				typename RemoveCVR<T3>::Type* argument3 = (typename RemoveCVR<T3>::Type*)arguments[3];
 				typename RemoveCVR<T4>::Type* argument4 = (typename RemoveCVR<T4>::Type*)arguments[4];
 
				R result=(((C*)object)->*(*((FunctionPointer*)funcPtrPtr)))(*argument0,*argument1,*argument2,*argument3,*argument4);
				return new R(result);
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
				delete (FunctionPointer*)instance;
			}
		};
		template<typename C ,typename R,typename T0,typename T1,typename T2,typename T3,typename T4>
		class ObjectTypeInformation<R(C::*)(T0,T1,T2,T3,T4) >
		{
		public:
			static WString GetName()
			{
				return ObjectTypeInformation<R>::GetName()+L"("+ObjectTypeInformation<C>::GetName()+L"::*)"+L"("+ObjectTypeInformation<T0>::GetName()+L","+ObjectTypeInformation<T1>::GetName()+L","+ObjectTypeInformation<T2>::GetName()+L","+ObjectTypeInformation<T3>::GetName()+L","+ObjectTypeInformation<T4>::GetName()+L")" ;
			}
			typedef ObjectTypeFunction<R(C::*)(T0,T1,T2,T3,T4) > ObjectTypeImplementation;
		};
		template<typename C ,typename R,typename T0,typename T1,typename T2,typename T3,typename T4>
		class ObjectTypeRegister<R(C::*)(T0,T1,T2,T3,T4) >
		{
		public:
			static void Register()
			{
				ObjectTypeFunction<R(C::*)(T0,T1,T2,T3,T4) >* type=new ObjectTypeFunction<R(C::*)(T0,T1,T2,T3,T4) >;
				RegisterObjectType(ObjectTypeInformation<R(C::*)(T0,T1,T2,T3,T4) >::GetName(), type);
				type->parameterTypes.Add(TypeOf<T0>());
 				type->parameterTypes.Add(TypeOf<T1>());
 				type->parameterTypes.Add(TypeOf<T2>());
 				type->parameterTypes.Add(TypeOf<T3>());
 				type->parameterTypes.Add(TypeOf<T4>());
 
				type->returnType=TypeOf<R>();
				type->thisType=TypeOf<C>();
				type->constructors.Add(new ObjectTypeFunction<R(C::*)(T0,T1,T2,T3,T4) >::Constructor<void*(*)()>(&ObjectTypeFunction<R(C::*)(T0,T1,T2,T3,T4) >::ctor));
			}
		};
 
/***********************************************************************
vl::objectmodel::ObjectTypeFunction<void(C::*)(T0,T1,T2,T3,T4) >
***********************************************************************/
		template<typename C  ,typename T0,typename T1,typename T2,typename T3,typename T4>
		class ObjectTypeFunction<void(C::*)(T0,T1,T2,T3,T4) > : public SpecifiedObjectType<void(C::*)(T0,T1,T2,T3,T4) >
		{
			friend class ObjectTypeRegister<void(C::*)(T0,T1,T2,T3,T4) >;
			typedef void(C::*FunctionPointer)(T0,T1,T2,T3,T4) ;
		protected:
			static void* ctor()
			{
				return new FunctionPointer(0);
			}
			collections::List<ObjectType*>		parameterTypes;
			ObjectType*							returnType;
			ObjectType*							thisType;
			collections::List<ObjectMember*>	constructors;
		public:
			ObjectTypeFunction()
			{
				returnType=0;
				thisType=0;
			}
			~ObjectTypeFunction()
			{
				for(int i=0;i<constructors.Count();i++)
				{
					delete constructors[i];
				}
			}
			ObjectCategory Category()const
			{
				return ObjectType::Function;
			}
			const ObjectType* ReturnType()const
			{
				return returnType;
			}
			const TypeList& ParameterTypes()const
			{
				return parameterTypes.Wrap();
			}
			const ObjectType* ThisType()const
			{
				return thisType;
			}
			bool IsConstFunction()const
			{
				return false;
			}
			void* Invoke(void* object, void* funcPtrPtr, void** arguments)const
			{
				typename RemoveCVR<T0>::Type* argument0 = (typename RemoveCVR<T0>::Type*)arguments[0];
 				typename RemoveCVR<T1>::Type* argument1 = (typename RemoveCVR<T1>::Type*)arguments[1];
 				typename RemoveCVR<T2>::Type* argument2 = (typename RemoveCVR<T2>::Type*)arguments[2];
 				typename RemoveCVR<T3>::Type* argument3 = (typename RemoveCVR<T3>::Type*)arguments[3];
 				typename RemoveCVR<T4>::Type* argument4 = (typename RemoveCVR<T4>::Type*)arguments[4];
 
				 (((C*)object)->*(*((FunctionPointer*)funcPtrPtr)))(*argument0,*argument1,*argument2,*argument3,*argument4);
				return 0;
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
				delete (FunctionPointer*)instance;
			}
		};
		template<typename C  ,typename T0,typename T1,typename T2,typename T3,typename T4>
		class ObjectTypeInformation<void(C::*)(T0,T1,T2,T3,T4) >
		{
		public:
			static WString GetName()
			{
				return ObjectTypeInformation<void>::GetName()+L"("+ObjectTypeInformation<C>::GetName()+L"::*)"+L"("+ObjectTypeInformation<T0>::GetName()+L","+ObjectTypeInformation<T1>::GetName()+L","+ObjectTypeInformation<T2>::GetName()+L","+ObjectTypeInformation<T3>::GetName()+L","+ObjectTypeInformation<T4>::GetName()+L")" ;
			}
			typedef ObjectTypeFunction<void(C::*)(T0,T1,T2,T3,T4) > ObjectTypeImplementation;
		};
		template<typename C  ,typename T0,typename T1,typename T2,typename T3,typename T4>
		class ObjectTypeRegister<void(C::*)(T0,T1,T2,T3,T4) >
		{
		public:
			static void Register()
			{
				ObjectTypeFunction<void(C::*)(T0,T1,T2,T3,T4) >* type=new ObjectTypeFunction<void(C::*)(T0,T1,T2,T3,T4) >;
				RegisterObjectType(ObjectTypeInformation<void(C::*)(T0,T1,T2,T3,T4) >::GetName(), type);
				type->parameterTypes.Add(TypeOf<T0>());
 				type->parameterTypes.Add(TypeOf<T1>());
 				type->parameterTypes.Add(TypeOf<T2>());
 				type->parameterTypes.Add(TypeOf<T3>());
 				type->parameterTypes.Add(TypeOf<T4>());
 
				type->returnType=TypeOf<void>();
				type->thisType=TypeOf<C>();
				type->constructors.Add(new ObjectTypeFunction<void(C::*)(T0,T1,T2,T3,T4) >::Constructor<void*(*)()>(&ObjectTypeFunction<void(C::*)(T0,T1,T2,T3,T4) >::ctor));
			}
		};
 
/***********************************************************************
vl::objectmodel::ObjectTypeFunction<R(C::*)(T0,T1,T2,T3,T4)const>
***********************************************************************/
		template<typename C ,typename R,typename T0,typename T1,typename T2,typename T3,typename T4>
		class ObjectTypeFunction<R(C::*)(T0,T1,T2,T3,T4)const> : public SpecifiedObjectType<R(C::*)(T0,T1,T2,T3,T4)const>
		{
			friend class ObjectTypeRegister<R(C::*)(T0,T1,T2,T3,T4)const>;
			typedef R(C::*FunctionPointer)(T0,T1,T2,T3,T4)const;
		protected:
			static void* ctor()
			{
				return new FunctionPointer(0);
			}
			collections::List<ObjectType*>		parameterTypes;
			ObjectType*							returnType;
			ObjectType*							thisType;
			collections::List<ObjectMember*>	constructors;
		public:
			ObjectTypeFunction()
			{
				returnType=0;
				thisType=0;
			}
			~ObjectTypeFunction()
			{
				for(int i=0;i<constructors.Count();i++)
				{
					delete constructors[i];
				}
			}
			ObjectCategory Category()const
			{
				return ObjectType::Function;
			}
			const ObjectType* ReturnType()const
			{
				return returnType;
			}
			const TypeList& ParameterTypes()const
			{
				return parameterTypes.Wrap();
			}
			const ObjectType* ThisType()const
			{
				return thisType;
			}
			bool IsConstFunction()const
			{
				return true;
			}
			void* Invoke(void* object, void* funcPtrPtr, void** arguments)const
			{
				typename RemoveCVR<T0>::Type* argument0 = (typename RemoveCVR<T0>::Type*)arguments[0];
 				typename RemoveCVR<T1>::Type* argument1 = (typename RemoveCVR<T1>::Type*)arguments[1];
 				typename RemoveCVR<T2>::Type* argument2 = (typename RemoveCVR<T2>::Type*)arguments[2];
 				typename RemoveCVR<T3>::Type* argument3 = (typename RemoveCVR<T3>::Type*)arguments[3];
 				typename RemoveCVR<T4>::Type* argument4 = (typename RemoveCVR<T4>::Type*)arguments[4];
 
				R result=(((C*)object)->*(*((FunctionPointer*)funcPtrPtr)))(*argument0,*argument1,*argument2,*argument3,*argument4);
				return new R(result);
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
				delete (FunctionPointer*)instance;
			}
		};
		template<typename C ,typename R,typename T0,typename T1,typename T2,typename T3,typename T4>
		class ObjectTypeInformation<R(C::*)(T0,T1,T2,T3,T4)const>
		{
		public:
			static WString GetName()
			{
				return ObjectTypeInformation<R>::GetName()+L"("+ObjectTypeInformation<C>::GetName()+L"::*)"+L"("+ObjectTypeInformation<T0>::GetName()+L","+ObjectTypeInformation<T1>::GetName()+L","+ObjectTypeInformation<T2>::GetName()+L","+ObjectTypeInformation<T3>::GetName()+L","+ObjectTypeInformation<T4>::GetName()+L")"+L"const";
			}
			typedef ObjectTypeFunction<R(C::*)(T0,T1,T2,T3,T4)const> ObjectTypeImplementation;
		};
		template<typename C ,typename R,typename T0,typename T1,typename T2,typename T3,typename T4>
		class ObjectTypeRegister<R(C::*)(T0,T1,T2,T3,T4)const>
		{
		public:
			static void Register()
			{
				ObjectTypeFunction<R(C::*)(T0,T1,T2,T3,T4)const>* type=new ObjectTypeFunction<R(C::*)(T0,T1,T2,T3,T4)const>;
				RegisterObjectType(ObjectTypeInformation<R(C::*)(T0,T1,T2,T3,T4)const>::GetName(), type);
				type->parameterTypes.Add(TypeOf<T0>());
 				type->parameterTypes.Add(TypeOf<T1>());
 				type->parameterTypes.Add(TypeOf<T2>());
 				type->parameterTypes.Add(TypeOf<T3>());
 				type->parameterTypes.Add(TypeOf<T4>());
 
				type->returnType=TypeOf<R>();
				type->thisType=TypeOf<C>();
				type->constructors.Add(new ObjectTypeFunction<R(C::*)(T0,T1,T2,T3,T4)const>::Constructor<void*(*)()>(&ObjectTypeFunction<R(C::*)(T0,T1,T2,T3,T4)const>::ctor));
			}
		};
 
/***********************************************************************
vl::objectmodel::ObjectTypeFunction<void(C::*)(T0,T1,T2,T3,T4)const>
***********************************************************************/
		template<typename C  ,typename T0,typename T1,typename T2,typename T3,typename T4>
		class ObjectTypeFunction<void(C::*)(T0,T1,T2,T3,T4)const> : public SpecifiedObjectType<void(C::*)(T0,T1,T2,T3,T4)const>
		{
			friend class ObjectTypeRegister<void(C::*)(T0,T1,T2,T3,T4)const>;
			typedef void(C::*FunctionPointer)(T0,T1,T2,T3,T4)const;
		protected:
			static void* ctor()
			{
				return new FunctionPointer(0);
			}
			collections::List<ObjectType*>		parameterTypes;
			ObjectType*							returnType;
			ObjectType*							thisType;
			collections::List<ObjectMember*>	constructors;
		public:
			ObjectTypeFunction()
			{
				returnType=0;
				thisType=0;
			}
			~ObjectTypeFunction()
			{
				for(int i=0;i<constructors.Count();i++)
				{
					delete constructors[i];
				}
			}
			ObjectCategory Category()const
			{
				return ObjectType::Function;
			}
			const ObjectType* ReturnType()const
			{
				return returnType;
			}
			const TypeList& ParameterTypes()const
			{
				return parameterTypes.Wrap();
			}
			const ObjectType* ThisType()const
			{
				return thisType;
			}
			bool IsConstFunction()const
			{
				return true;
			}
			void* Invoke(void* object, void* funcPtrPtr, void** arguments)const
			{
				typename RemoveCVR<T0>::Type* argument0 = (typename RemoveCVR<T0>::Type*)arguments[0];
 				typename RemoveCVR<T1>::Type* argument1 = (typename RemoveCVR<T1>::Type*)arguments[1];
 				typename RemoveCVR<T2>::Type* argument2 = (typename RemoveCVR<T2>::Type*)arguments[2];
 				typename RemoveCVR<T3>::Type* argument3 = (typename RemoveCVR<T3>::Type*)arguments[3];
 				typename RemoveCVR<T4>::Type* argument4 = (typename RemoveCVR<T4>::Type*)arguments[4];
 
				 (((C*)object)->*(*((FunctionPointer*)funcPtrPtr)))(*argument0,*argument1,*argument2,*argument3,*argument4);
				return 0;
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
				delete (FunctionPointer*)instance;
			}
		};
		template<typename C  ,typename T0,typename T1,typename T2,typename T3,typename T4>
		class ObjectTypeInformation<void(C::*)(T0,T1,T2,T3,T4)const>
		{
		public:
			static WString GetName()
			{
				return ObjectTypeInformation<void>::GetName()+L"("+ObjectTypeInformation<C>::GetName()+L"::*)"+L"("+ObjectTypeInformation<T0>::GetName()+L","+ObjectTypeInformation<T1>::GetName()+L","+ObjectTypeInformation<T2>::GetName()+L","+ObjectTypeInformation<T3>::GetName()+L","+ObjectTypeInformation<T4>::GetName()+L")"+L"const";
			}
			typedef ObjectTypeFunction<void(C::*)(T0,T1,T2,T3,T4)const> ObjectTypeImplementation;
		};
		template<typename C  ,typename T0,typename T1,typename T2,typename T3,typename T4>
		class ObjectTypeRegister<void(C::*)(T0,T1,T2,T3,T4)const>
		{
		public:
			static void Register()
			{
				ObjectTypeFunction<void(C::*)(T0,T1,T2,T3,T4)const>* type=new ObjectTypeFunction<void(C::*)(T0,T1,T2,T3,T4)const>;
				RegisterObjectType(ObjectTypeInformation<void(C::*)(T0,T1,T2,T3,T4)const>::GetName(), type);
				type->parameterTypes.Add(TypeOf<T0>());
 				type->parameterTypes.Add(TypeOf<T1>());
 				type->parameterTypes.Add(TypeOf<T2>());
 				type->parameterTypes.Add(TypeOf<T3>());
 				type->parameterTypes.Add(TypeOf<T4>());
 
				type->returnType=TypeOf<void>();
				type->thisType=TypeOf<C>();
				type->constructors.Add(new ObjectTypeFunction<void(C::*)(T0,T1,T2,T3,T4)const>::Constructor<void*(*)()>(&ObjectTypeFunction<void(C::*)(T0,T1,T2,T3,T4)const>::ctor));
			}
		};
  
/***********************************************************************
vl::objectmodel::ObjectTypeFunction<R(*)(T0,T1,T2,T3,T4,T5) >
***********************************************************************/
		template<typename R,typename T0,typename T1,typename T2,typename T3,typename T4,typename T5>
		class ObjectTypeFunction<R(*)(T0,T1,T2,T3,T4,T5) > : public SpecifiedObjectType<R(*)(T0,T1,T2,T3,T4,T5) >
		{
			friend class ObjectTypeRegister<R(*)(T0,T1,T2,T3,T4,T5) >;
			typedef R(*FunctionPointer)(T0,T1,T2,T3,T4,T5) ;
		protected:
			static void* ctor()
			{
				return new FunctionPointer(0);
			}
			collections::List<ObjectType*>		parameterTypes;
			ObjectType*							returnType;
			ObjectType*							thisType;
			collections::List<ObjectMember*>	constructors;
		public:
			ObjectTypeFunction()
			{
				returnType=0;
				thisType=0;
			}
			~ObjectTypeFunction()
			{
				for(int i=0;i<constructors.Count();i++)
				{
					delete constructors[i];
				}
			}
			ObjectCategory Category()const
			{
				return ObjectType::Function;
			}
			const ObjectType* ReturnType()const
			{
				return returnType;
			}
			const TypeList& ParameterTypes()const
			{
				return parameterTypes.Wrap();
			}
			const ObjectType* ThisType()const
			{
				return thisType;
			}
			bool IsConstFunction()const
			{
				return false;
			}
			void* Invoke(void* object, void* funcPtrPtr, void** arguments)const
			{
				typename RemoveCVR<T0>::Type* argument0 = (typename RemoveCVR<T0>::Type*)arguments[0];
 				typename RemoveCVR<T1>::Type* argument1 = (typename RemoveCVR<T1>::Type*)arguments[1];
 				typename RemoveCVR<T2>::Type* argument2 = (typename RemoveCVR<T2>::Type*)arguments[2];
 				typename RemoveCVR<T3>::Type* argument3 = (typename RemoveCVR<T3>::Type*)arguments[3];
 				typename RemoveCVR<T4>::Type* argument4 = (typename RemoveCVR<T4>::Type*)arguments[4];
 				typename RemoveCVR<T5>::Type* argument5 = (typename RemoveCVR<T5>::Type*)arguments[5];
 
				R result=(*((FunctionPointer*)funcPtrPtr))(*argument0,*argument1,*argument2,*argument3,*argument4,*argument5);
				return new R(result);
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
				delete (FunctionPointer*)instance;
			}
		};
		template<typename R,typename T0,typename T1,typename T2,typename T3,typename T4,typename T5>
		class ObjectTypeInformation<R(*)(T0,T1,T2,T3,T4,T5) >
		{
		public:
			static WString GetName()
			{
				return ObjectTypeInformation<R>::GetName()+L"(*)"+L"("+ObjectTypeInformation<T0>::GetName()+L","+ObjectTypeInformation<T1>::GetName()+L","+ObjectTypeInformation<T2>::GetName()+L","+ObjectTypeInformation<T3>::GetName()+L","+ObjectTypeInformation<T4>::GetName()+L","+ObjectTypeInformation<T5>::GetName()+L")" ;
			}
			typedef ObjectTypeFunction<R(*)(T0,T1,T2,T3,T4,T5) > ObjectTypeImplementation;
		};
		template<typename R,typename T0,typename T1,typename T2,typename T3,typename T4,typename T5>
		class ObjectTypeRegister<R(*)(T0,T1,T2,T3,T4,T5) >
		{
		public:
			static void Register()
			{
				ObjectTypeFunction<R(*)(T0,T1,T2,T3,T4,T5) >* type=new ObjectTypeFunction<R(*)(T0,T1,T2,T3,T4,T5) >;
				RegisterObjectType(ObjectTypeInformation<R(*)(T0,T1,T2,T3,T4,T5) >::GetName(), type);
				type->parameterTypes.Add(TypeOf<T0>());
 				type->parameterTypes.Add(TypeOf<T1>());
 				type->parameterTypes.Add(TypeOf<T2>());
 				type->parameterTypes.Add(TypeOf<T3>());
 				type->parameterTypes.Add(TypeOf<T4>());
 				type->parameterTypes.Add(TypeOf<T5>());
 
				type->returnType=TypeOf<R>();
				type->thisType=0;
				type->constructors.Add(new ObjectTypeFunction<R(*)(T0,T1,T2,T3,T4,T5) >::Constructor<void*(*)()>(&ObjectTypeFunction<R(*)(T0,T1,T2,T3,T4,T5) >::ctor));
			}
		};
 
/***********************************************************************
vl::objectmodel::ObjectTypeFunction<void(*)(T0,T1,T2,T3,T4,T5) >
***********************************************************************/
		template< typename T0,typename T1,typename T2,typename T3,typename T4,typename T5>
		class ObjectTypeFunction<void(*)(T0,T1,T2,T3,T4,T5) > : public SpecifiedObjectType<void(*)(T0,T1,T2,T3,T4,T5) >
		{
			friend class ObjectTypeRegister<void(*)(T0,T1,T2,T3,T4,T5) >;
			typedef void(*FunctionPointer)(T0,T1,T2,T3,T4,T5) ;
		protected:
			static void* ctor()
			{
				return new FunctionPointer(0);
			}
			collections::List<ObjectType*>		parameterTypes;
			ObjectType*							returnType;
			ObjectType*							thisType;
			collections::List<ObjectMember*>	constructors;
		public:
			ObjectTypeFunction()
			{
				returnType=0;
				thisType=0;
			}
			~ObjectTypeFunction()
			{
				for(int i=0;i<constructors.Count();i++)
				{
					delete constructors[i];
				}
			}
			ObjectCategory Category()const
			{
				return ObjectType::Function;
			}
			const ObjectType* ReturnType()const
			{
				return returnType;
			}
			const TypeList& ParameterTypes()const
			{
				return parameterTypes.Wrap();
			}
			const ObjectType* ThisType()const
			{
				return thisType;
			}
			bool IsConstFunction()const
			{
				return false;
			}
			void* Invoke(void* object, void* funcPtrPtr, void** arguments)const
			{
				typename RemoveCVR<T0>::Type* argument0 = (typename RemoveCVR<T0>::Type*)arguments[0];
 				typename RemoveCVR<T1>::Type* argument1 = (typename RemoveCVR<T1>::Type*)arguments[1];
 				typename RemoveCVR<T2>::Type* argument2 = (typename RemoveCVR<T2>::Type*)arguments[2];
 				typename RemoveCVR<T3>::Type* argument3 = (typename RemoveCVR<T3>::Type*)arguments[3];
 				typename RemoveCVR<T4>::Type* argument4 = (typename RemoveCVR<T4>::Type*)arguments[4];
 				typename RemoveCVR<T5>::Type* argument5 = (typename RemoveCVR<T5>::Type*)arguments[5];
 
				 (*((FunctionPointer*)funcPtrPtr))(*argument0,*argument1,*argument2,*argument3,*argument4,*argument5);
				return 0;
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
				delete (FunctionPointer*)instance;
			}
		};
		template< typename T0,typename T1,typename T2,typename T3,typename T4,typename T5>
		class ObjectTypeInformation<void(*)(T0,T1,T2,T3,T4,T5) >
		{
		public:
			static WString GetName()
			{
				return ObjectTypeInformation<void>::GetName()+L"(*)"+L"("+ObjectTypeInformation<T0>::GetName()+L","+ObjectTypeInformation<T1>::GetName()+L","+ObjectTypeInformation<T2>::GetName()+L","+ObjectTypeInformation<T3>::GetName()+L","+ObjectTypeInformation<T4>::GetName()+L","+ObjectTypeInformation<T5>::GetName()+L")" ;
			}
			typedef ObjectTypeFunction<void(*)(T0,T1,T2,T3,T4,T5) > ObjectTypeImplementation;
		};
		template< typename T0,typename T1,typename T2,typename T3,typename T4,typename T5>
		class ObjectTypeRegister<void(*)(T0,T1,T2,T3,T4,T5) >
		{
		public:
			static void Register()
			{
				ObjectTypeFunction<void(*)(T0,T1,T2,T3,T4,T5) >* type=new ObjectTypeFunction<void(*)(T0,T1,T2,T3,T4,T5) >;
				RegisterObjectType(ObjectTypeInformation<void(*)(T0,T1,T2,T3,T4,T5) >::GetName(), type);
				type->parameterTypes.Add(TypeOf<T0>());
 				type->parameterTypes.Add(TypeOf<T1>());
 				type->parameterTypes.Add(TypeOf<T2>());
 				type->parameterTypes.Add(TypeOf<T3>());
 				type->parameterTypes.Add(TypeOf<T4>());
 				type->parameterTypes.Add(TypeOf<T5>());
 
				type->returnType=TypeOf<void>();
				type->thisType=0;
				type->constructors.Add(new ObjectTypeFunction<void(*)(T0,T1,T2,T3,T4,T5) >::Constructor<void*(*)()>(&ObjectTypeFunction<void(*)(T0,T1,T2,T3,T4,T5) >::ctor));
			}
		};
 
/***********************************************************************
vl::objectmodel::ObjectTypeFunction<R(C::*)(T0,T1,T2,T3,T4,T5) >
***********************************************************************/
		template<typename C ,typename R,typename T0,typename T1,typename T2,typename T3,typename T4,typename T5>
		class ObjectTypeFunction<R(C::*)(T0,T1,T2,T3,T4,T5) > : public SpecifiedObjectType<R(C::*)(T0,T1,T2,T3,T4,T5) >
		{
			friend class ObjectTypeRegister<R(C::*)(T0,T1,T2,T3,T4,T5) >;
			typedef R(C::*FunctionPointer)(T0,T1,T2,T3,T4,T5) ;
		protected:
			static void* ctor()
			{
				return new FunctionPointer(0);
			}
			collections::List<ObjectType*>		parameterTypes;
			ObjectType*							returnType;
			ObjectType*							thisType;
			collections::List<ObjectMember*>	constructors;
		public:
			ObjectTypeFunction()
			{
				returnType=0;
				thisType=0;
			}
			~ObjectTypeFunction()
			{
				for(int i=0;i<constructors.Count();i++)
				{
					delete constructors[i];
				}
			}
			ObjectCategory Category()const
			{
				return ObjectType::Function;
			}
			const ObjectType* ReturnType()const
			{
				return returnType;
			}
			const TypeList& ParameterTypes()const
			{
				return parameterTypes.Wrap();
			}
			const ObjectType* ThisType()const
			{
				return thisType;
			}
			bool IsConstFunction()const
			{
				return false;
			}
			void* Invoke(void* object, void* funcPtrPtr, void** arguments)const
			{
				typename RemoveCVR<T0>::Type* argument0 = (typename RemoveCVR<T0>::Type*)arguments[0];
 				typename RemoveCVR<T1>::Type* argument1 = (typename RemoveCVR<T1>::Type*)arguments[1];
 				typename RemoveCVR<T2>::Type* argument2 = (typename RemoveCVR<T2>::Type*)arguments[2];
 				typename RemoveCVR<T3>::Type* argument3 = (typename RemoveCVR<T3>::Type*)arguments[3];
 				typename RemoveCVR<T4>::Type* argument4 = (typename RemoveCVR<T4>::Type*)arguments[4];
 				typename RemoveCVR<T5>::Type* argument5 = (typename RemoveCVR<T5>::Type*)arguments[5];
 
				R result=(((C*)object)->*(*((FunctionPointer*)funcPtrPtr)))(*argument0,*argument1,*argument2,*argument3,*argument4,*argument5);
				return new R(result);
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
				delete (FunctionPointer*)instance;
			}
		};
		template<typename C ,typename R,typename T0,typename T1,typename T2,typename T3,typename T4,typename T5>
		class ObjectTypeInformation<R(C::*)(T0,T1,T2,T3,T4,T5) >
		{
		public:
			static WString GetName()
			{
				return ObjectTypeInformation<R>::GetName()+L"("+ObjectTypeInformation<C>::GetName()+L"::*)"+L"("+ObjectTypeInformation<T0>::GetName()+L","+ObjectTypeInformation<T1>::GetName()+L","+ObjectTypeInformation<T2>::GetName()+L","+ObjectTypeInformation<T3>::GetName()+L","+ObjectTypeInformation<T4>::GetName()+L","+ObjectTypeInformation<T5>::GetName()+L")" ;
			}
			typedef ObjectTypeFunction<R(C::*)(T0,T1,T2,T3,T4,T5) > ObjectTypeImplementation;
		};
		template<typename C ,typename R,typename T0,typename T1,typename T2,typename T3,typename T4,typename T5>
		class ObjectTypeRegister<R(C::*)(T0,T1,T2,T3,T4,T5) >
		{
		public:
			static void Register()
			{
				ObjectTypeFunction<R(C::*)(T0,T1,T2,T3,T4,T5) >* type=new ObjectTypeFunction<R(C::*)(T0,T1,T2,T3,T4,T5) >;
				RegisterObjectType(ObjectTypeInformation<R(C::*)(T0,T1,T2,T3,T4,T5) >::GetName(), type);
				type->parameterTypes.Add(TypeOf<T0>());
 				type->parameterTypes.Add(TypeOf<T1>());
 				type->parameterTypes.Add(TypeOf<T2>());
 				type->parameterTypes.Add(TypeOf<T3>());
 				type->parameterTypes.Add(TypeOf<T4>());
 				type->parameterTypes.Add(TypeOf<T5>());
 
				type->returnType=TypeOf<R>();
				type->thisType=TypeOf<C>();
				type->constructors.Add(new ObjectTypeFunction<R(C::*)(T0,T1,T2,T3,T4,T5) >::Constructor<void*(*)()>(&ObjectTypeFunction<R(C::*)(T0,T1,T2,T3,T4,T5) >::ctor));
			}
		};
 
/***********************************************************************
vl::objectmodel::ObjectTypeFunction<void(C::*)(T0,T1,T2,T3,T4,T5) >
***********************************************************************/
		template<typename C  ,typename T0,typename T1,typename T2,typename T3,typename T4,typename T5>
		class ObjectTypeFunction<void(C::*)(T0,T1,T2,T3,T4,T5) > : public SpecifiedObjectType<void(C::*)(T0,T1,T2,T3,T4,T5) >
		{
			friend class ObjectTypeRegister<void(C::*)(T0,T1,T2,T3,T4,T5) >;
			typedef void(C::*FunctionPointer)(T0,T1,T2,T3,T4,T5) ;
		protected:
			static void* ctor()
			{
				return new FunctionPointer(0);
			}
			collections::List<ObjectType*>		parameterTypes;
			ObjectType*							returnType;
			ObjectType*							thisType;
			collections::List<ObjectMember*>	constructors;
		public:
			ObjectTypeFunction()
			{
				returnType=0;
				thisType=0;
			}
			~ObjectTypeFunction()
			{
				for(int i=0;i<constructors.Count();i++)
				{
					delete constructors[i];
				}
			}
			ObjectCategory Category()const
			{
				return ObjectType::Function;
			}
			const ObjectType* ReturnType()const
			{
				return returnType;
			}
			const TypeList& ParameterTypes()const
			{
				return parameterTypes.Wrap();
			}
			const ObjectType* ThisType()const
			{
				return thisType;
			}
			bool IsConstFunction()const
			{
				return false;
			}
			void* Invoke(void* object, void* funcPtrPtr, void** arguments)const
			{
				typename RemoveCVR<T0>::Type* argument0 = (typename RemoveCVR<T0>::Type*)arguments[0];
 				typename RemoveCVR<T1>::Type* argument1 = (typename RemoveCVR<T1>::Type*)arguments[1];
 				typename RemoveCVR<T2>::Type* argument2 = (typename RemoveCVR<T2>::Type*)arguments[2];
 				typename RemoveCVR<T3>::Type* argument3 = (typename RemoveCVR<T3>::Type*)arguments[3];
 				typename RemoveCVR<T4>::Type* argument4 = (typename RemoveCVR<T4>::Type*)arguments[4];
 				typename RemoveCVR<T5>::Type* argument5 = (typename RemoveCVR<T5>::Type*)arguments[5];
 
				 (((C*)object)->*(*((FunctionPointer*)funcPtrPtr)))(*argument0,*argument1,*argument2,*argument3,*argument4,*argument5);
				return 0;
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
				delete (FunctionPointer*)instance;
			}
		};
		template<typename C  ,typename T0,typename T1,typename T2,typename T3,typename T4,typename T5>
		class ObjectTypeInformation<void(C::*)(T0,T1,T2,T3,T4,T5) >
		{
		public:
			static WString GetName()
			{
				return ObjectTypeInformation<void>::GetName()+L"("+ObjectTypeInformation<C>::GetName()+L"::*)"+L"("+ObjectTypeInformation<T0>::GetName()+L","+ObjectTypeInformation<T1>::GetName()+L","+ObjectTypeInformation<T2>::GetName()+L","+ObjectTypeInformation<T3>::GetName()+L","+ObjectTypeInformation<T4>::GetName()+L","+ObjectTypeInformation<T5>::GetName()+L")" ;
			}
			typedef ObjectTypeFunction<void(C::*)(T0,T1,T2,T3,T4,T5) > ObjectTypeImplementation;
		};
		template<typename C  ,typename T0,typename T1,typename T2,typename T3,typename T4,typename T5>
		class ObjectTypeRegister<void(C::*)(T0,T1,T2,T3,T4,T5) >
		{
		public:
			static void Register()
			{
				ObjectTypeFunction<void(C::*)(T0,T1,T2,T3,T4,T5) >* type=new ObjectTypeFunction<void(C::*)(T0,T1,T2,T3,T4,T5) >;
				RegisterObjectType(ObjectTypeInformation<void(C::*)(T0,T1,T2,T3,T4,T5) >::GetName(), type);
				type->parameterTypes.Add(TypeOf<T0>());
 				type->parameterTypes.Add(TypeOf<T1>());
 				type->parameterTypes.Add(TypeOf<T2>());
 				type->parameterTypes.Add(TypeOf<T3>());
 				type->parameterTypes.Add(TypeOf<T4>());
 				type->parameterTypes.Add(TypeOf<T5>());
 
				type->returnType=TypeOf<void>();
				type->thisType=TypeOf<C>();
				type->constructors.Add(new ObjectTypeFunction<void(C::*)(T0,T1,T2,T3,T4,T5) >::Constructor<void*(*)()>(&ObjectTypeFunction<void(C::*)(T0,T1,T2,T3,T4,T5) >::ctor));
			}
		};
 
/***********************************************************************
vl::objectmodel::ObjectTypeFunction<R(C::*)(T0,T1,T2,T3,T4,T5)const>
***********************************************************************/
		template<typename C ,typename R,typename T0,typename T1,typename T2,typename T3,typename T4,typename T5>
		class ObjectTypeFunction<R(C::*)(T0,T1,T2,T3,T4,T5)const> : public SpecifiedObjectType<R(C::*)(T0,T1,T2,T3,T4,T5)const>
		{
			friend class ObjectTypeRegister<R(C::*)(T0,T1,T2,T3,T4,T5)const>;
			typedef R(C::*FunctionPointer)(T0,T1,T2,T3,T4,T5)const;
		protected:
			static void* ctor()
			{
				return new FunctionPointer(0);
			}
			collections::List<ObjectType*>		parameterTypes;
			ObjectType*							returnType;
			ObjectType*							thisType;
			collections::List<ObjectMember*>	constructors;
		public:
			ObjectTypeFunction()
			{
				returnType=0;
				thisType=0;
			}
			~ObjectTypeFunction()
			{
				for(int i=0;i<constructors.Count();i++)
				{
					delete constructors[i];
				}
			}
			ObjectCategory Category()const
			{
				return ObjectType::Function;
			}
			const ObjectType* ReturnType()const
			{
				return returnType;
			}
			const TypeList& ParameterTypes()const
			{
				return parameterTypes.Wrap();
			}
			const ObjectType* ThisType()const
			{
				return thisType;
			}
			bool IsConstFunction()const
			{
				return true;
			}
			void* Invoke(void* object, void* funcPtrPtr, void** arguments)const
			{
				typename RemoveCVR<T0>::Type* argument0 = (typename RemoveCVR<T0>::Type*)arguments[0];
 				typename RemoveCVR<T1>::Type* argument1 = (typename RemoveCVR<T1>::Type*)arguments[1];
 				typename RemoveCVR<T2>::Type* argument2 = (typename RemoveCVR<T2>::Type*)arguments[2];
 				typename RemoveCVR<T3>::Type* argument3 = (typename RemoveCVR<T3>::Type*)arguments[3];
 				typename RemoveCVR<T4>::Type* argument4 = (typename RemoveCVR<T4>::Type*)arguments[4];
 				typename RemoveCVR<T5>::Type* argument5 = (typename RemoveCVR<T5>::Type*)arguments[5];
 
				R result=(((C*)object)->*(*((FunctionPointer*)funcPtrPtr)))(*argument0,*argument1,*argument2,*argument3,*argument4,*argument5);
				return new R(result);
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
				delete (FunctionPointer*)instance;
			}
		};
		template<typename C ,typename R,typename T0,typename T1,typename T2,typename T3,typename T4,typename T5>
		class ObjectTypeInformation<R(C::*)(T0,T1,T2,T3,T4,T5)const>
		{
		public:
			static WString GetName()
			{
				return ObjectTypeInformation<R>::GetName()+L"("+ObjectTypeInformation<C>::GetName()+L"::*)"+L"("+ObjectTypeInformation<T0>::GetName()+L","+ObjectTypeInformation<T1>::GetName()+L","+ObjectTypeInformation<T2>::GetName()+L","+ObjectTypeInformation<T3>::GetName()+L","+ObjectTypeInformation<T4>::GetName()+L","+ObjectTypeInformation<T5>::GetName()+L")"+L"const";
			}
			typedef ObjectTypeFunction<R(C::*)(T0,T1,T2,T3,T4,T5)const> ObjectTypeImplementation;
		};
		template<typename C ,typename R,typename T0,typename T1,typename T2,typename T3,typename T4,typename T5>
		class ObjectTypeRegister<R(C::*)(T0,T1,T2,T3,T4,T5)const>
		{
		public:
			static void Register()
			{
				ObjectTypeFunction<R(C::*)(T0,T1,T2,T3,T4,T5)const>* type=new ObjectTypeFunction<R(C::*)(T0,T1,T2,T3,T4,T5)const>;
				RegisterObjectType(ObjectTypeInformation<R(C::*)(T0,T1,T2,T3,T4,T5)const>::GetName(), type);
				type->parameterTypes.Add(TypeOf<T0>());
 				type->parameterTypes.Add(TypeOf<T1>());
 				type->parameterTypes.Add(TypeOf<T2>());
 				type->parameterTypes.Add(TypeOf<T3>());
 				type->parameterTypes.Add(TypeOf<T4>());
 				type->parameterTypes.Add(TypeOf<T5>());
 
				type->returnType=TypeOf<R>();
				type->thisType=TypeOf<C>();
				type->constructors.Add(new ObjectTypeFunction<R(C::*)(T0,T1,T2,T3,T4,T5)const>::Constructor<void*(*)()>(&ObjectTypeFunction<R(C::*)(T0,T1,T2,T3,T4,T5)const>::ctor));
			}
		};
 
/***********************************************************************
vl::objectmodel::ObjectTypeFunction<void(C::*)(T0,T1,T2,T3,T4,T5)const>
***********************************************************************/
		template<typename C  ,typename T0,typename T1,typename T2,typename T3,typename T4,typename T5>
		class ObjectTypeFunction<void(C::*)(T0,T1,T2,T3,T4,T5)const> : public SpecifiedObjectType<void(C::*)(T0,T1,T2,T3,T4,T5)const>
		{
			friend class ObjectTypeRegister<void(C::*)(T0,T1,T2,T3,T4,T5)const>;
			typedef void(C::*FunctionPointer)(T0,T1,T2,T3,T4,T5)const;
		protected:
			static void* ctor()
			{
				return new FunctionPointer(0);
			}
			collections::List<ObjectType*>		parameterTypes;
			ObjectType*							returnType;
			ObjectType*							thisType;
			collections::List<ObjectMember*>	constructors;
		public:
			ObjectTypeFunction()
			{
				returnType=0;
				thisType=0;
			}
			~ObjectTypeFunction()
			{
				for(int i=0;i<constructors.Count();i++)
				{
					delete constructors[i];
				}
			}
			ObjectCategory Category()const
			{
				return ObjectType::Function;
			}
			const ObjectType* ReturnType()const
			{
				return returnType;
			}
			const TypeList& ParameterTypes()const
			{
				return parameterTypes.Wrap();
			}
			const ObjectType* ThisType()const
			{
				return thisType;
			}
			bool IsConstFunction()const
			{
				return true;
			}
			void* Invoke(void* object, void* funcPtrPtr, void** arguments)const
			{
				typename RemoveCVR<T0>::Type* argument0 = (typename RemoveCVR<T0>::Type*)arguments[0];
 				typename RemoveCVR<T1>::Type* argument1 = (typename RemoveCVR<T1>::Type*)arguments[1];
 				typename RemoveCVR<T2>::Type* argument2 = (typename RemoveCVR<T2>::Type*)arguments[2];
 				typename RemoveCVR<T3>::Type* argument3 = (typename RemoveCVR<T3>::Type*)arguments[3];
 				typename RemoveCVR<T4>::Type* argument4 = (typename RemoveCVR<T4>::Type*)arguments[4];
 				typename RemoveCVR<T5>::Type* argument5 = (typename RemoveCVR<T5>::Type*)arguments[5];
 
				 (((C*)object)->*(*((FunctionPointer*)funcPtrPtr)))(*argument0,*argument1,*argument2,*argument3,*argument4,*argument5);
				return 0;
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
				delete (FunctionPointer*)instance;
			}
		};
		template<typename C  ,typename T0,typename T1,typename T2,typename T3,typename T4,typename T5>
		class ObjectTypeInformation<void(C::*)(T0,T1,T2,T3,T4,T5)const>
		{
		public:
			static WString GetName()
			{
				return ObjectTypeInformation<void>::GetName()+L"("+ObjectTypeInformation<C>::GetName()+L"::*)"+L"("+ObjectTypeInformation<T0>::GetName()+L","+ObjectTypeInformation<T1>::GetName()+L","+ObjectTypeInformation<T2>::GetName()+L","+ObjectTypeInformation<T3>::GetName()+L","+ObjectTypeInformation<T4>::GetName()+L","+ObjectTypeInformation<T5>::GetName()+L")"+L"const";
			}
			typedef ObjectTypeFunction<void(C::*)(T0,T1,T2,T3,T4,T5)const> ObjectTypeImplementation;
		};
		template<typename C  ,typename T0,typename T1,typename T2,typename T3,typename T4,typename T5>
		class ObjectTypeRegister<void(C::*)(T0,T1,T2,T3,T4,T5)const>
		{
		public:
			static void Register()
			{
				ObjectTypeFunction<void(C::*)(T0,T1,T2,T3,T4,T5)const>* type=new ObjectTypeFunction<void(C::*)(T0,T1,T2,T3,T4,T5)const>;
				RegisterObjectType(ObjectTypeInformation<void(C::*)(T0,T1,T2,T3,T4,T5)const>::GetName(), type);
				type->parameterTypes.Add(TypeOf<T0>());
 				type->parameterTypes.Add(TypeOf<T1>());
 				type->parameterTypes.Add(TypeOf<T2>());
 				type->parameterTypes.Add(TypeOf<T3>());
 				type->parameterTypes.Add(TypeOf<T4>());
 				type->parameterTypes.Add(TypeOf<T5>());
 
				type->returnType=TypeOf<void>();
				type->thisType=TypeOf<C>();
				type->constructors.Add(new ObjectTypeFunction<void(C::*)(T0,T1,T2,T3,T4,T5)const>::Constructor<void*(*)()>(&ObjectTypeFunction<void(C::*)(T0,T1,T2,T3,T4,T5)const>::ctor));
			}
		};
  
/***********************************************************************
vl::objectmodel::ObjectTypeFunction<R(*)(T0,T1,T2,T3,T4,T5,T6) >
***********************************************************************/
		template<typename R,typename T0,typename T1,typename T2,typename T3,typename T4,typename T5,typename T6>
		class ObjectTypeFunction<R(*)(T0,T1,T2,T3,T4,T5,T6) > : public SpecifiedObjectType<R(*)(T0,T1,T2,T3,T4,T5,T6) >
		{
			friend class ObjectTypeRegister<R(*)(T0,T1,T2,T3,T4,T5,T6) >;
			typedef R(*FunctionPointer)(T0,T1,T2,T3,T4,T5,T6) ;
		protected:
			static void* ctor()
			{
				return new FunctionPointer(0);
			}
			collections::List<ObjectType*>		parameterTypes;
			ObjectType*							returnType;
			ObjectType*							thisType;
			collections::List<ObjectMember*>	constructors;
		public:
			ObjectTypeFunction()
			{
				returnType=0;
				thisType=0;
			}
			~ObjectTypeFunction()
			{
				for(int i=0;i<constructors.Count();i++)
				{
					delete constructors[i];
				}
			}
			ObjectCategory Category()const
			{
				return ObjectType::Function;
			}
			const ObjectType* ReturnType()const
			{
				return returnType;
			}
			const TypeList& ParameterTypes()const
			{
				return parameterTypes.Wrap();
			}
			const ObjectType* ThisType()const
			{
				return thisType;
			}
			bool IsConstFunction()const
			{
				return false;
			}
			void* Invoke(void* object, void* funcPtrPtr, void** arguments)const
			{
				typename RemoveCVR<T0>::Type* argument0 = (typename RemoveCVR<T0>::Type*)arguments[0];
 				typename RemoveCVR<T1>::Type* argument1 = (typename RemoveCVR<T1>::Type*)arguments[1];
 				typename RemoveCVR<T2>::Type* argument2 = (typename RemoveCVR<T2>::Type*)arguments[2];
 				typename RemoveCVR<T3>::Type* argument3 = (typename RemoveCVR<T3>::Type*)arguments[3];
 				typename RemoveCVR<T4>::Type* argument4 = (typename RemoveCVR<T4>::Type*)arguments[4];
 				typename RemoveCVR<T5>::Type* argument5 = (typename RemoveCVR<T5>::Type*)arguments[5];
 				typename RemoveCVR<T6>::Type* argument6 = (typename RemoveCVR<T6>::Type*)arguments[6];
 
				R result=(*((FunctionPointer*)funcPtrPtr))(*argument0,*argument1,*argument2,*argument3,*argument4,*argument5,*argument6);
				return new R(result);
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
				delete (FunctionPointer*)instance;
			}
		};
		template<typename R,typename T0,typename T1,typename T2,typename T3,typename T4,typename T5,typename T6>
		class ObjectTypeInformation<R(*)(T0,T1,T2,T3,T4,T5,T6) >
		{
		public:
			static WString GetName()
			{
				return ObjectTypeInformation<R>::GetName()+L"(*)"+L"("+ObjectTypeInformation<T0>::GetName()+L","+ObjectTypeInformation<T1>::GetName()+L","+ObjectTypeInformation<T2>::GetName()+L","+ObjectTypeInformation<T3>::GetName()+L","+ObjectTypeInformation<T4>::GetName()+L","+ObjectTypeInformation<T5>::GetName()+L","+ObjectTypeInformation<T6>::GetName()+L")" ;
			}
			typedef ObjectTypeFunction<R(*)(T0,T1,T2,T3,T4,T5,T6) > ObjectTypeImplementation;
		};
		template<typename R,typename T0,typename T1,typename T2,typename T3,typename T4,typename T5,typename T6>
		class ObjectTypeRegister<R(*)(T0,T1,T2,T3,T4,T5,T6) >
		{
		public:
			static void Register()
			{
				ObjectTypeFunction<R(*)(T0,T1,T2,T3,T4,T5,T6) >* type=new ObjectTypeFunction<R(*)(T0,T1,T2,T3,T4,T5,T6) >;
				RegisterObjectType(ObjectTypeInformation<R(*)(T0,T1,T2,T3,T4,T5,T6) >::GetName(), type);
				type->parameterTypes.Add(TypeOf<T0>());
 				type->parameterTypes.Add(TypeOf<T1>());
 				type->parameterTypes.Add(TypeOf<T2>());
 				type->parameterTypes.Add(TypeOf<T3>());
 				type->parameterTypes.Add(TypeOf<T4>());
 				type->parameterTypes.Add(TypeOf<T5>());
 				type->parameterTypes.Add(TypeOf<T6>());
 
				type->returnType=TypeOf<R>();
				type->thisType=0;
				type->constructors.Add(new ObjectTypeFunction<R(*)(T0,T1,T2,T3,T4,T5,T6) >::Constructor<void*(*)()>(&ObjectTypeFunction<R(*)(T0,T1,T2,T3,T4,T5,T6) >::ctor));
			}
		};
 
/***********************************************************************
vl::objectmodel::ObjectTypeFunction<void(*)(T0,T1,T2,T3,T4,T5,T6) >
***********************************************************************/
		template< typename T0,typename T1,typename T2,typename T3,typename T4,typename T5,typename T6>
		class ObjectTypeFunction<void(*)(T0,T1,T2,T3,T4,T5,T6) > : public SpecifiedObjectType<void(*)(T0,T1,T2,T3,T4,T5,T6) >
		{
			friend class ObjectTypeRegister<void(*)(T0,T1,T2,T3,T4,T5,T6) >;
			typedef void(*FunctionPointer)(T0,T1,T2,T3,T4,T5,T6) ;
		protected:
			static void* ctor()
			{
				return new FunctionPointer(0);
			}
			collections::List<ObjectType*>		parameterTypes;
			ObjectType*							returnType;
			ObjectType*							thisType;
			collections::List<ObjectMember*>	constructors;
		public:
			ObjectTypeFunction()
			{
				returnType=0;
				thisType=0;
			}
			~ObjectTypeFunction()
			{
				for(int i=0;i<constructors.Count();i++)
				{
					delete constructors[i];
				}
			}
			ObjectCategory Category()const
			{
				return ObjectType::Function;
			}
			const ObjectType* ReturnType()const
			{
				return returnType;
			}
			const TypeList& ParameterTypes()const
			{
				return parameterTypes.Wrap();
			}
			const ObjectType* ThisType()const
			{
				return thisType;
			}
			bool IsConstFunction()const
			{
				return false;
			}
			void* Invoke(void* object, void* funcPtrPtr, void** arguments)const
			{
				typename RemoveCVR<T0>::Type* argument0 = (typename RemoveCVR<T0>::Type*)arguments[0];
 				typename RemoveCVR<T1>::Type* argument1 = (typename RemoveCVR<T1>::Type*)arguments[1];
 				typename RemoveCVR<T2>::Type* argument2 = (typename RemoveCVR<T2>::Type*)arguments[2];
 				typename RemoveCVR<T3>::Type* argument3 = (typename RemoveCVR<T3>::Type*)arguments[3];
 				typename RemoveCVR<T4>::Type* argument4 = (typename RemoveCVR<T4>::Type*)arguments[4];
 				typename RemoveCVR<T5>::Type* argument5 = (typename RemoveCVR<T5>::Type*)arguments[5];
 				typename RemoveCVR<T6>::Type* argument6 = (typename RemoveCVR<T6>::Type*)arguments[6];
 
				 (*((FunctionPointer*)funcPtrPtr))(*argument0,*argument1,*argument2,*argument3,*argument4,*argument5,*argument6);
				return 0;
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
				delete (FunctionPointer*)instance;
			}
		};
		template< typename T0,typename T1,typename T2,typename T3,typename T4,typename T5,typename T6>
		class ObjectTypeInformation<void(*)(T0,T1,T2,T3,T4,T5,T6) >
		{
		public:
			static WString GetName()
			{
				return ObjectTypeInformation<void>::GetName()+L"(*)"+L"("+ObjectTypeInformation<T0>::GetName()+L","+ObjectTypeInformation<T1>::GetName()+L","+ObjectTypeInformation<T2>::GetName()+L","+ObjectTypeInformation<T3>::GetName()+L","+ObjectTypeInformation<T4>::GetName()+L","+ObjectTypeInformation<T5>::GetName()+L","+ObjectTypeInformation<T6>::GetName()+L")" ;
			}
			typedef ObjectTypeFunction<void(*)(T0,T1,T2,T3,T4,T5,T6) > ObjectTypeImplementation;
		};
		template< typename T0,typename T1,typename T2,typename T3,typename T4,typename T5,typename T6>
		class ObjectTypeRegister<void(*)(T0,T1,T2,T3,T4,T5,T6) >
		{
		public:
			static void Register()
			{
				ObjectTypeFunction<void(*)(T0,T1,T2,T3,T4,T5,T6) >* type=new ObjectTypeFunction<void(*)(T0,T1,T2,T3,T4,T5,T6) >;
				RegisterObjectType(ObjectTypeInformation<void(*)(T0,T1,T2,T3,T4,T5,T6) >::GetName(), type);
				type->parameterTypes.Add(TypeOf<T0>());
 				type->parameterTypes.Add(TypeOf<T1>());
 				type->parameterTypes.Add(TypeOf<T2>());
 				type->parameterTypes.Add(TypeOf<T3>());
 				type->parameterTypes.Add(TypeOf<T4>());
 				type->parameterTypes.Add(TypeOf<T5>());
 				type->parameterTypes.Add(TypeOf<T6>());
 
				type->returnType=TypeOf<void>();
				type->thisType=0;
				type->constructors.Add(new ObjectTypeFunction<void(*)(T0,T1,T2,T3,T4,T5,T6) >::Constructor<void*(*)()>(&ObjectTypeFunction<void(*)(T0,T1,T2,T3,T4,T5,T6) >::ctor));
			}
		};
 
/***********************************************************************
vl::objectmodel::ObjectTypeFunction<R(C::*)(T0,T1,T2,T3,T4,T5,T6) >
***********************************************************************/
		template<typename C ,typename R,typename T0,typename T1,typename T2,typename T3,typename T4,typename T5,typename T6>
		class ObjectTypeFunction<R(C::*)(T0,T1,T2,T3,T4,T5,T6) > : public SpecifiedObjectType<R(C::*)(T0,T1,T2,T3,T4,T5,T6) >
		{
			friend class ObjectTypeRegister<R(C::*)(T0,T1,T2,T3,T4,T5,T6) >;
			typedef R(C::*FunctionPointer)(T0,T1,T2,T3,T4,T5,T6) ;
		protected:
			static void* ctor()
			{
				return new FunctionPointer(0);
			}
			collections::List<ObjectType*>		parameterTypes;
			ObjectType*							returnType;
			ObjectType*							thisType;
			collections::List<ObjectMember*>	constructors;
		public:
			ObjectTypeFunction()
			{
				returnType=0;
				thisType=0;
			}
			~ObjectTypeFunction()
			{
				for(int i=0;i<constructors.Count();i++)
				{
					delete constructors[i];
				}
			}
			ObjectCategory Category()const
			{
				return ObjectType::Function;
			}
			const ObjectType* ReturnType()const
			{
				return returnType;
			}
			const TypeList& ParameterTypes()const
			{
				return parameterTypes.Wrap();
			}
			const ObjectType* ThisType()const
			{
				return thisType;
			}
			bool IsConstFunction()const
			{
				return false;
			}
			void* Invoke(void* object, void* funcPtrPtr, void** arguments)const
			{
				typename RemoveCVR<T0>::Type* argument0 = (typename RemoveCVR<T0>::Type*)arguments[0];
 				typename RemoveCVR<T1>::Type* argument1 = (typename RemoveCVR<T1>::Type*)arguments[1];
 				typename RemoveCVR<T2>::Type* argument2 = (typename RemoveCVR<T2>::Type*)arguments[2];
 				typename RemoveCVR<T3>::Type* argument3 = (typename RemoveCVR<T3>::Type*)arguments[3];
 				typename RemoveCVR<T4>::Type* argument4 = (typename RemoveCVR<T4>::Type*)arguments[4];
 				typename RemoveCVR<T5>::Type* argument5 = (typename RemoveCVR<T5>::Type*)arguments[5];
 				typename RemoveCVR<T6>::Type* argument6 = (typename RemoveCVR<T6>::Type*)arguments[6];
 
				R result=(((C*)object)->*(*((FunctionPointer*)funcPtrPtr)))(*argument0,*argument1,*argument2,*argument3,*argument4,*argument5,*argument6);
				return new R(result);
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
				delete (FunctionPointer*)instance;
			}
		};
		template<typename C ,typename R,typename T0,typename T1,typename T2,typename T3,typename T4,typename T5,typename T6>
		class ObjectTypeInformation<R(C::*)(T0,T1,T2,T3,T4,T5,T6) >
		{
		public:
			static WString GetName()
			{
				return ObjectTypeInformation<R>::GetName()+L"("+ObjectTypeInformation<C>::GetName()+L"::*)"+L"("+ObjectTypeInformation<T0>::GetName()+L","+ObjectTypeInformation<T1>::GetName()+L","+ObjectTypeInformation<T2>::GetName()+L","+ObjectTypeInformation<T3>::GetName()+L","+ObjectTypeInformation<T4>::GetName()+L","+ObjectTypeInformation<T5>::GetName()+L","+ObjectTypeInformation<T6>::GetName()+L")" ;
			}
			typedef ObjectTypeFunction<R(C::*)(T0,T1,T2,T3,T4,T5,T6) > ObjectTypeImplementation;
		};
		template<typename C ,typename R,typename T0,typename T1,typename T2,typename T3,typename T4,typename T5,typename T6>
		class ObjectTypeRegister<R(C::*)(T0,T1,T2,T3,T4,T5,T6) >
		{
		public:
			static void Register()
			{
				ObjectTypeFunction<R(C::*)(T0,T1,T2,T3,T4,T5,T6) >* type=new ObjectTypeFunction<R(C::*)(T0,T1,T2,T3,T4,T5,T6) >;
				RegisterObjectType(ObjectTypeInformation<R(C::*)(T0,T1,T2,T3,T4,T5,T6) >::GetName(), type);
				type->parameterTypes.Add(TypeOf<T0>());
 				type->parameterTypes.Add(TypeOf<T1>());
 				type->parameterTypes.Add(TypeOf<T2>());
 				type->parameterTypes.Add(TypeOf<T3>());
 				type->parameterTypes.Add(TypeOf<T4>());
 				type->parameterTypes.Add(TypeOf<T5>());
 				type->parameterTypes.Add(TypeOf<T6>());
 
				type->returnType=TypeOf<R>();
				type->thisType=TypeOf<C>();
				type->constructors.Add(new ObjectTypeFunction<R(C::*)(T0,T1,T2,T3,T4,T5,T6) >::Constructor<void*(*)()>(&ObjectTypeFunction<R(C::*)(T0,T1,T2,T3,T4,T5,T6) >::ctor));
			}
		};
 
/***********************************************************************
vl::objectmodel::ObjectTypeFunction<void(C::*)(T0,T1,T2,T3,T4,T5,T6) >
***********************************************************************/
		template<typename C  ,typename T0,typename T1,typename T2,typename T3,typename T4,typename T5,typename T6>
		class ObjectTypeFunction<void(C::*)(T0,T1,T2,T3,T4,T5,T6) > : public SpecifiedObjectType<void(C::*)(T0,T1,T2,T3,T4,T5,T6) >
		{
			friend class ObjectTypeRegister<void(C::*)(T0,T1,T2,T3,T4,T5,T6) >;
			typedef void(C::*FunctionPointer)(T0,T1,T2,T3,T4,T5,T6) ;
		protected:
			static void* ctor()
			{
				return new FunctionPointer(0);
			}
			collections::List<ObjectType*>		parameterTypes;
			ObjectType*							returnType;
			ObjectType*							thisType;
			collections::List<ObjectMember*>	constructors;
		public:
			ObjectTypeFunction()
			{
				returnType=0;
				thisType=0;
			}
			~ObjectTypeFunction()
			{
				for(int i=0;i<constructors.Count();i++)
				{
					delete constructors[i];
				}
			}
			ObjectCategory Category()const
			{
				return ObjectType::Function;
			}
			const ObjectType* ReturnType()const
			{
				return returnType;
			}
			const TypeList& ParameterTypes()const
			{
				return parameterTypes.Wrap();
			}
			const ObjectType* ThisType()const
			{
				return thisType;
			}
			bool IsConstFunction()const
			{
				return false;
			}
			void* Invoke(void* object, void* funcPtrPtr, void** arguments)const
			{
				typename RemoveCVR<T0>::Type* argument0 = (typename RemoveCVR<T0>::Type*)arguments[0];
 				typename RemoveCVR<T1>::Type* argument1 = (typename RemoveCVR<T1>::Type*)arguments[1];
 				typename RemoveCVR<T2>::Type* argument2 = (typename RemoveCVR<T2>::Type*)arguments[2];
 				typename RemoveCVR<T3>::Type* argument3 = (typename RemoveCVR<T3>::Type*)arguments[3];
 				typename RemoveCVR<T4>::Type* argument4 = (typename RemoveCVR<T4>::Type*)arguments[4];
 				typename RemoveCVR<T5>::Type* argument5 = (typename RemoveCVR<T5>::Type*)arguments[5];
 				typename RemoveCVR<T6>::Type* argument6 = (typename RemoveCVR<T6>::Type*)arguments[6];
 
				 (((C*)object)->*(*((FunctionPointer*)funcPtrPtr)))(*argument0,*argument1,*argument2,*argument3,*argument4,*argument5,*argument6);
				return 0;
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
				delete (FunctionPointer*)instance;
			}
		};
		template<typename C  ,typename T0,typename T1,typename T2,typename T3,typename T4,typename T5,typename T6>
		class ObjectTypeInformation<void(C::*)(T0,T1,T2,T3,T4,T5,T6) >
		{
		public:
			static WString GetName()
			{
				return ObjectTypeInformation<void>::GetName()+L"("+ObjectTypeInformation<C>::GetName()+L"::*)"+L"("+ObjectTypeInformation<T0>::GetName()+L","+ObjectTypeInformation<T1>::GetName()+L","+ObjectTypeInformation<T2>::GetName()+L","+ObjectTypeInformation<T3>::GetName()+L","+ObjectTypeInformation<T4>::GetName()+L","+ObjectTypeInformation<T5>::GetName()+L","+ObjectTypeInformation<T6>::GetName()+L")" ;
			}
			typedef ObjectTypeFunction<void(C::*)(T0,T1,T2,T3,T4,T5,T6) > ObjectTypeImplementation;
		};
		template<typename C  ,typename T0,typename T1,typename T2,typename T3,typename T4,typename T5,typename T6>
		class ObjectTypeRegister<void(C::*)(T0,T1,T2,T3,T4,T5,T6) >
		{
		public:
			static void Register()
			{
				ObjectTypeFunction<void(C::*)(T0,T1,T2,T3,T4,T5,T6) >* type=new ObjectTypeFunction<void(C::*)(T0,T1,T2,T3,T4,T5,T6) >;
				RegisterObjectType(ObjectTypeInformation<void(C::*)(T0,T1,T2,T3,T4,T5,T6) >::GetName(), type);
				type->parameterTypes.Add(TypeOf<T0>());
 				type->parameterTypes.Add(TypeOf<T1>());
 				type->parameterTypes.Add(TypeOf<T2>());
 				type->parameterTypes.Add(TypeOf<T3>());
 				type->parameterTypes.Add(TypeOf<T4>());
 				type->parameterTypes.Add(TypeOf<T5>());
 				type->parameterTypes.Add(TypeOf<T6>());
 
				type->returnType=TypeOf<void>();
				type->thisType=TypeOf<C>();
				type->constructors.Add(new ObjectTypeFunction<void(C::*)(T0,T1,T2,T3,T4,T5,T6) >::Constructor<void*(*)()>(&ObjectTypeFunction<void(C::*)(T0,T1,T2,T3,T4,T5,T6) >::ctor));
			}
		};
 
/***********************************************************************
vl::objectmodel::ObjectTypeFunction<R(C::*)(T0,T1,T2,T3,T4,T5,T6)const>
***********************************************************************/
		template<typename C ,typename R,typename T0,typename T1,typename T2,typename T3,typename T4,typename T5,typename T6>
		class ObjectTypeFunction<R(C::*)(T0,T1,T2,T3,T4,T5,T6)const> : public SpecifiedObjectType<R(C::*)(T0,T1,T2,T3,T4,T5,T6)const>
		{
			friend class ObjectTypeRegister<R(C::*)(T0,T1,T2,T3,T4,T5,T6)const>;
			typedef R(C::*FunctionPointer)(T0,T1,T2,T3,T4,T5,T6)const;
		protected:
			static void* ctor()
			{
				return new FunctionPointer(0);
			}
			collections::List<ObjectType*>		parameterTypes;
			ObjectType*							returnType;
			ObjectType*							thisType;
			collections::List<ObjectMember*>	constructors;
		public:
			ObjectTypeFunction()
			{
				returnType=0;
				thisType=0;
			}
			~ObjectTypeFunction()
			{
				for(int i=0;i<constructors.Count();i++)
				{
					delete constructors[i];
				}
			}
			ObjectCategory Category()const
			{
				return ObjectType::Function;
			}
			const ObjectType* ReturnType()const
			{
				return returnType;
			}
			const TypeList& ParameterTypes()const
			{
				return parameterTypes.Wrap();
			}
			const ObjectType* ThisType()const
			{
				return thisType;
			}
			bool IsConstFunction()const
			{
				return true;
			}
			void* Invoke(void* object, void* funcPtrPtr, void** arguments)const
			{
				typename RemoveCVR<T0>::Type* argument0 = (typename RemoveCVR<T0>::Type*)arguments[0];
 				typename RemoveCVR<T1>::Type* argument1 = (typename RemoveCVR<T1>::Type*)arguments[1];
 				typename RemoveCVR<T2>::Type* argument2 = (typename RemoveCVR<T2>::Type*)arguments[2];
 				typename RemoveCVR<T3>::Type* argument3 = (typename RemoveCVR<T3>::Type*)arguments[3];
 				typename RemoveCVR<T4>::Type* argument4 = (typename RemoveCVR<T4>::Type*)arguments[4];
 				typename RemoveCVR<T5>::Type* argument5 = (typename RemoveCVR<T5>::Type*)arguments[5];
 				typename RemoveCVR<T6>::Type* argument6 = (typename RemoveCVR<T6>::Type*)arguments[6];
 
				R result=(((C*)object)->*(*((FunctionPointer*)funcPtrPtr)))(*argument0,*argument1,*argument2,*argument3,*argument4,*argument5,*argument6);
				return new R(result);
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
				delete (FunctionPointer*)instance;
			}
		};
		template<typename C ,typename R,typename T0,typename T1,typename T2,typename T3,typename T4,typename T5,typename T6>
		class ObjectTypeInformation<R(C::*)(T0,T1,T2,T3,T4,T5,T6)const>
		{
		public:
			static WString GetName()
			{
				return ObjectTypeInformation<R>::GetName()+L"("+ObjectTypeInformation<C>::GetName()+L"::*)"+L"("+ObjectTypeInformation<T0>::GetName()+L","+ObjectTypeInformation<T1>::GetName()+L","+ObjectTypeInformation<T2>::GetName()+L","+ObjectTypeInformation<T3>::GetName()+L","+ObjectTypeInformation<T4>::GetName()+L","+ObjectTypeInformation<T5>::GetName()+L","+ObjectTypeInformation<T6>::GetName()+L")"+L"const";
			}
			typedef ObjectTypeFunction<R(C::*)(T0,T1,T2,T3,T4,T5,T6)const> ObjectTypeImplementation;
		};
		template<typename C ,typename R,typename T0,typename T1,typename T2,typename T3,typename T4,typename T5,typename T6>
		class ObjectTypeRegister<R(C::*)(T0,T1,T2,T3,T4,T5,T6)const>
		{
		public:
			static void Register()
			{
				ObjectTypeFunction<R(C::*)(T0,T1,T2,T3,T4,T5,T6)const>* type=new ObjectTypeFunction<R(C::*)(T0,T1,T2,T3,T4,T5,T6)const>;
				RegisterObjectType(ObjectTypeInformation<R(C::*)(T0,T1,T2,T3,T4,T5,T6)const>::GetName(), type);
				type->parameterTypes.Add(TypeOf<T0>());
 				type->parameterTypes.Add(TypeOf<T1>());
 				type->parameterTypes.Add(TypeOf<T2>());
 				type->parameterTypes.Add(TypeOf<T3>());
 				type->parameterTypes.Add(TypeOf<T4>());
 				type->parameterTypes.Add(TypeOf<T5>());
 				type->parameterTypes.Add(TypeOf<T6>());
 
				type->returnType=TypeOf<R>();
				type->thisType=TypeOf<C>();
				type->constructors.Add(new ObjectTypeFunction<R(C::*)(T0,T1,T2,T3,T4,T5,T6)const>::Constructor<void*(*)()>(&ObjectTypeFunction<R(C::*)(T0,T1,T2,T3,T4,T5,T6)const>::ctor));
			}
		};
 
/***********************************************************************
vl::objectmodel::ObjectTypeFunction<void(C::*)(T0,T1,T2,T3,T4,T5,T6)const>
***********************************************************************/
		template<typename C  ,typename T0,typename T1,typename T2,typename T3,typename T4,typename T5,typename T6>
		class ObjectTypeFunction<void(C::*)(T0,T1,T2,T3,T4,T5,T6)const> : public SpecifiedObjectType<void(C::*)(T0,T1,T2,T3,T4,T5,T6)const>
		{
			friend class ObjectTypeRegister<void(C::*)(T0,T1,T2,T3,T4,T5,T6)const>;
			typedef void(C::*FunctionPointer)(T0,T1,T2,T3,T4,T5,T6)const;
		protected:
			static void* ctor()
			{
				return new FunctionPointer(0);
			}
			collections::List<ObjectType*>		parameterTypes;
			ObjectType*							returnType;
			ObjectType*							thisType;
			collections::List<ObjectMember*>	constructors;
		public:
			ObjectTypeFunction()
			{
				returnType=0;
				thisType=0;
			}
			~ObjectTypeFunction()
			{
				for(int i=0;i<constructors.Count();i++)
				{
					delete constructors[i];
				}
			}
			ObjectCategory Category()const
			{
				return ObjectType::Function;
			}
			const ObjectType* ReturnType()const
			{
				return returnType;
			}
			const TypeList& ParameterTypes()const
			{
				return parameterTypes.Wrap();
			}
			const ObjectType* ThisType()const
			{
				return thisType;
			}
			bool IsConstFunction()const
			{
				return true;
			}
			void* Invoke(void* object, void* funcPtrPtr, void** arguments)const
			{
				typename RemoveCVR<T0>::Type* argument0 = (typename RemoveCVR<T0>::Type*)arguments[0];
 				typename RemoveCVR<T1>::Type* argument1 = (typename RemoveCVR<T1>::Type*)arguments[1];
 				typename RemoveCVR<T2>::Type* argument2 = (typename RemoveCVR<T2>::Type*)arguments[2];
 				typename RemoveCVR<T3>::Type* argument3 = (typename RemoveCVR<T3>::Type*)arguments[3];
 				typename RemoveCVR<T4>::Type* argument4 = (typename RemoveCVR<T4>::Type*)arguments[4];
 				typename RemoveCVR<T5>::Type* argument5 = (typename RemoveCVR<T5>::Type*)arguments[5];
 				typename RemoveCVR<T6>::Type* argument6 = (typename RemoveCVR<T6>::Type*)arguments[6];
 
				 (((C*)object)->*(*((FunctionPointer*)funcPtrPtr)))(*argument0,*argument1,*argument2,*argument3,*argument4,*argument5,*argument6);
				return 0;
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
				delete (FunctionPointer*)instance;
			}
		};
		template<typename C  ,typename T0,typename T1,typename T2,typename T3,typename T4,typename T5,typename T6>
		class ObjectTypeInformation<void(C::*)(T0,T1,T2,T3,T4,T5,T6)const>
		{
		public:
			static WString GetName()
			{
				return ObjectTypeInformation<void>::GetName()+L"("+ObjectTypeInformation<C>::GetName()+L"::*)"+L"("+ObjectTypeInformation<T0>::GetName()+L","+ObjectTypeInformation<T1>::GetName()+L","+ObjectTypeInformation<T2>::GetName()+L","+ObjectTypeInformation<T3>::GetName()+L","+ObjectTypeInformation<T4>::GetName()+L","+ObjectTypeInformation<T5>::GetName()+L","+ObjectTypeInformation<T6>::GetName()+L")"+L"const";
			}
			typedef ObjectTypeFunction<void(C::*)(T0,T1,T2,T3,T4,T5,T6)const> ObjectTypeImplementation;
		};
		template<typename C  ,typename T0,typename T1,typename T2,typename T3,typename T4,typename T5,typename T6>
		class ObjectTypeRegister<void(C::*)(T0,T1,T2,T3,T4,T5,T6)const>
		{
		public:
			static void Register()
			{
				ObjectTypeFunction<void(C::*)(T0,T1,T2,T3,T4,T5,T6)const>* type=new ObjectTypeFunction<void(C::*)(T0,T1,T2,T3,T4,T5,T6)const>;
				RegisterObjectType(ObjectTypeInformation<void(C::*)(T0,T1,T2,T3,T4,T5,T6)const>::GetName(), type);
				type->parameterTypes.Add(TypeOf<T0>());
 				type->parameterTypes.Add(TypeOf<T1>());
 				type->parameterTypes.Add(TypeOf<T2>());
 				type->parameterTypes.Add(TypeOf<T3>());
 				type->parameterTypes.Add(TypeOf<T4>());
 				type->parameterTypes.Add(TypeOf<T5>());
 				type->parameterTypes.Add(TypeOf<T6>());
 
				type->returnType=TypeOf<void>();
				type->thisType=TypeOf<C>();
				type->constructors.Add(new ObjectTypeFunction<void(C::*)(T0,T1,T2,T3,T4,T5,T6)const>::Constructor<void*(*)()>(&ObjectTypeFunction<void(C::*)(T0,T1,T2,T3,T4,T5,T6)const>::ctor));
			}
		};
  
/***********************************************************************
vl::objectmodel::ObjectTypeFunction<R(*)(T0,T1,T2,T3,T4,T5,T6,T7) >
***********************************************************************/
		template<typename R,typename T0,typename T1,typename T2,typename T3,typename T4,typename T5,typename T6,typename T7>
		class ObjectTypeFunction<R(*)(T0,T1,T2,T3,T4,T5,T6,T7) > : public SpecifiedObjectType<R(*)(T0,T1,T2,T3,T4,T5,T6,T7) >
		{
			friend class ObjectTypeRegister<R(*)(T0,T1,T2,T3,T4,T5,T6,T7) >;
			typedef R(*FunctionPointer)(T0,T1,T2,T3,T4,T5,T6,T7) ;
		protected:
			static void* ctor()
			{
				return new FunctionPointer(0);
			}
			collections::List<ObjectType*>		parameterTypes;
			ObjectType*							returnType;
			ObjectType*							thisType;
			collections::List<ObjectMember*>	constructors;
		public:
			ObjectTypeFunction()
			{
				returnType=0;
				thisType=0;
			}
			~ObjectTypeFunction()
			{
				for(int i=0;i<constructors.Count();i++)
				{
					delete constructors[i];
				}
			}
			ObjectCategory Category()const
			{
				return ObjectType::Function;
			}
			const ObjectType* ReturnType()const
			{
				return returnType;
			}
			const TypeList& ParameterTypes()const
			{
				return parameterTypes.Wrap();
			}
			const ObjectType* ThisType()const
			{
				return thisType;
			}
			bool IsConstFunction()const
			{
				return false;
			}
			void* Invoke(void* object, void* funcPtrPtr, void** arguments)const
			{
				typename RemoveCVR<T0>::Type* argument0 = (typename RemoveCVR<T0>::Type*)arguments[0];
 				typename RemoveCVR<T1>::Type* argument1 = (typename RemoveCVR<T1>::Type*)arguments[1];
 				typename RemoveCVR<T2>::Type* argument2 = (typename RemoveCVR<T2>::Type*)arguments[2];
 				typename RemoveCVR<T3>::Type* argument3 = (typename RemoveCVR<T3>::Type*)arguments[3];
 				typename RemoveCVR<T4>::Type* argument4 = (typename RemoveCVR<T4>::Type*)arguments[4];
 				typename RemoveCVR<T5>::Type* argument5 = (typename RemoveCVR<T5>::Type*)arguments[5];
 				typename RemoveCVR<T6>::Type* argument6 = (typename RemoveCVR<T6>::Type*)arguments[6];
 				typename RemoveCVR<T7>::Type* argument7 = (typename RemoveCVR<T7>::Type*)arguments[7];
 
				R result=(*((FunctionPointer*)funcPtrPtr))(*argument0,*argument1,*argument2,*argument3,*argument4,*argument5,*argument6,*argument7);
				return new R(result);
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
				delete (FunctionPointer*)instance;
			}
		};
		template<typename R,typename T0,typename T1,typename T2,typename T3,typename T4,typename T5,typename T6,typename T7>
		class ObjectTypeInformation<R(*)(T0,T1,T2,T3,T4,T5,T6,T7) >
		{
		public:
			static WString GetName()
			{
				return ObjectTypeInformation<R>::GetName()+L"(*)"+L"("+ObjectTypeInformation<T0>::GetName()+L","+ObjectTypeInformation<T1>::GetName()+L","+ObjectTypeInformation<T2>::GetName()+L","+ObjectTypeInformation<T3>::GetName()+L","+ObjectTypeInformation<T4>::GetName()+L","+ObjectTypeInformation<T5>::GetName()+L","+ObjectTypeInformation<T6>::GetName()+L","+ObjectTypeInformation<T7>::GetName()+L")" ;
			}
			typedef ObjectTypeFunction<R(*)(T0,T1,T2,T3,T4,T5,T6,T7) > ObjectTypeImplementation;
		};
		template<typename R,typename T0,typename T1,typename T2,typename T3,typename T4,typename T5,typename T6,typename T7>
		class ObjectTypeRegister<R(*)(T0,T1,T2,T3,T4,T5,T6,T7) >
		{
		public:
			static void Register()
			{
				ObjectTypeFunction<R(*)(T0,T1,T2,T3,T4,T5,T6,T7) >* type=new ObjectTypeFunction<R(*)(T0,T1,T2,T3,T4,T5,T6,T7) >;
				RegisterObjectType(ObjectTypeInformation<R(*)(T0,T1,T2,T3,T4,T5,T6,T7) >::GetName(), type);
				type->parameterTypes.Add(TypeOf<T0>());
 				type->parameterTypes.Add(TypeOf<T1>());
 				type->parameterTypes.Add(TypeOf<T2>());
 				type->parameterTypes.Add(TypeOf<T3>());
 				type->parameterTypes.Add(TypeOf<T4>());
 				type->parameterTypes.Add(TypeOf<T5>());
 				type->parameterTypes.Add(TypeOf<T6>());
 				type->parameterTypes.Add(TypeOf<T7>());
 
				type->returnType=TypeOf<R>();
				type->thisType=0;
				type->constructors.Add(new ObjectTypeFunction<R(*)(T0,T1,T2,T3,T4,T5,T6,T7) >::Constructor<void*(*)()>(&ObjectTypeFunction<R(*)(T0,T1,T2,T3,T4,T5,T6,T7) >::ctor));
			}
		};
 
/***********************************************************************
vl::objectmodel::ObjectTypeFunction<void(*)(T0,T1,T2,T3,T4,T5,T6,T7) >
***********************************************************************/
		template< typename T0,typename T1,typename T2,typename T3,typename T4,typename T5,typename T6,typename T7>
		class ObjectTypeFunction<void(*)(T0,T1,T2,T3,T4,T5,T6,T7) > : public SpecifiedObjectType<void(*)(T0,T1,T2,T3,T4,T5,T6,T7) >
		{
			friend class ObjectTypeRegister<void(*)(T0,T1,T2,T3,T4,T5,T6,T7) >;
			typedef void(*FunctionPointer)(T0,T1,T2,T3,T4,T5,T6,T7) ;
		protected:
			static void* ctor()
			{
				return new FunctionPointer(0);
			}
			collections::List<ObjectType*>		parameterTypes;
			ObjectType*							returnType;
			ObjectType*							thisType;
			collections::List<ObjectMember*>	constructors;
		public:
			ObjectTypeFunction()
			{
				returnType=0;
				thisType=0;
			}
			~ObjectTypeFunction()
			{
				for(int i=0;i<constructors.Count();i++)
				{
					delete constructors[i];
				}
			}
			ObjectCategory Category()const
			{
				return ObjectType::Function;
			}
			const ObjectType* ReturnType()const
			{
				return returnType;
			}
			const TypeList& ParameterTypes()const
			{
				return parameterTypes.Wrap();
			}
			const ObjectType* ThisType()const
			{
				return thisType;
			}
			bool IsConstFunction()const
			{
				return false;
			}
			void* Invoke(void* object, void* funcPtrPtr, void** arguments)const
			{
				typename RemoveCVR<T0>::Type* argument0 = (typename RemoveCVR<T0>::Type*)arguments[0];
 				typename RemoveCVR<T1>::Type* argument1 = (typename RemoveCVR<T1>::Type*)arguments[1];
 				typename RemoveCVR<T2>::Type* argument2 = (typename RemoveCVR<T2>::Type*)arguments[2];
 				typename RemoveCVR<T3>::Type* argument3 = (typename RemoveCVR<T3>::Type*)arguments[3];
 				typename RemoveCVR<T4>::Type* argument4 = (typename RemoveCVR<T4>::Type*)arguments[4];
 				typename RemoveCVR<T5>::Type* argument5 = (typename RemoveCVR<T5>::Type*)arguments[5];
 				typename RemoveCVR<T6>::Type* argument6 = (typename RemoveCVR<T6>::Type*)arguments[6];
 				typename RemoveCVR<T7>::Type* argument7 = (typename RemoveCVR<T7>::Type*)arguments[7];
 
				 (*((FunctionPointer*)funcPtrPtr))(*argument0,*argument1,*argument2,*argument3,*argument4,*argument5,*argument6,*argument7);
				return 0;
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
				delete (FunctionPointer*)instance;
			}
		};
		template< typename T0,typename T1,typename T2,typename T3,typename T4,typename T5,typename T6,typename T7>
		class ObjectTypeInformation<void(*)(T0,T1,T2,T3,T4,T5,T6,T7) >
		{
		public:
			static WString GetName()
			{
				return ObjectTypeInformation<void>::GetName()+L"(*)"+L"("+ObjectTypeInformation<T0>::GetName()+L","+ObjectTypeInformation<T1>::GetName()+L","+ObjectTypeInformation<T2>::GetName()+L","+ObjectTypeInformation<T3>::GetName()+L","+ObjectTypeInformation<T4>::GetName()+L","+ObjectTypeInformation<T5>::GetName()+L","+ObjectTypeInformation<T6>::GetName()+L","+ObjectTypeInformation<T7>::GetName()+L")" ;
			}
			typedef ObjectTypeFunction<void(*)(T0,T1,T2,T3,T4,T5,T6,T7) > ObjectTypeImplementation;
		};
		template< typename T0,typename T1,typename T2,typename T3,typename T4,typename T5,typename T6,typename T7>
		class ObjectTypeRegister<void(*)(T0,T1,T2,T3,T4,T5,T6,T7) >
		{
		public:
			static void Register()
			{
				ObjectTypeFunction<void(*)(T0,T1,T2,T3,T4,T5,T6,T7) >* type=new ObjectTypeFunction<void(*)(T0,T1,T2,T3,T4,T5,T6,T7) >;
				RegisterObjectType(ObjectTypeInformation<void(*)(T0,T1,T2,T3,T4,T5,T6,T7) >::GetName(), type);
				type->parameterTypes.Add(TypeOf<T0>());
 				type->parameterTypes.Add(TypeOf<T1>());
 				type->parameterTypes.Add(TypeOf<T2>());
 				type->parameterTypes.Add(TypeOf<T3>());
 				type->parameterTypes.Add(TypeOf<T4>());
 				type->parameterTypes.Add(TypeOf<T5>());
 				type->parameterTypes.Add(TypeOf<T6>());
 				type->parameterTypes.Add(TypeOf<T7>());
 
				type->returnType=TypeOf<void>();
				type->thisType=0;
				type->constructors.Add(new ObjectTypeFunction<void(*)(T0,T1,T2,T3,T4,T5,T6,T7) >::Constructor<void*(*)()>(&ObjectTypeFunction<void(*)(T0,T1,T2,T3,T4,T5,T6,T7) >::ctor));
			}
		};
 
/***********************************************************************
vl::objectmodel::ObjectTypeFunction<R(C::*)(T0,T1,T2,T3,T4,T5,T6,T7) >
***********************************************************************/
		template<typename C ,typename R,typename T0,typename T1,typename T2,typename T3,typename T4,typename T5,typename T6,typename T7>
		class ObjectTypeFunction<R(C::*)(T0,T1,T2,T3,T4,T5,T6,T7) > : public SpecifiedObjectType<R(C::*)(T0,T1,T2,T3,T4,T5,T6,T7) >
		{
			friend class ObjectTypeRegister<R(C::*)(T0,T1,T2,T3,T4,T5,T6,T7) >;
			typedef R(C::*FunctionPointer)(T0,T1,T2,T3,T4,T5,T6,T7) ;
		protected:
			static void* ctor()
			{
				return new FunctionPointer(0);
			}
			collections::List<ObjectType*>		parameterTypes;
			ObjectType*							returnType;
			ObjectType*							thisType;
			collections::List<ObjectMember*>	constructors;
		public:
			ObjectTypeFunction()
			{
				returnType=0;
				thisType=0;
			}
			~ObjectTypeFunction()
			{
				for(int i=0;i<constructors.Count();i++)
				{
					delete constructors[i];
				}
			}
			ObjectCategory Category()const
			{
				return ObjectType::Function;
			}
			const ObjectType* ReturnType()const
			{
				return returnType;
			}
			const TypeList& ParameterTypes()const
			{
				return parameterTypes.Wrap();
			}
			const ObjectType* ThisType()const
			{
				return thisType;
			}
			bool IsConstFunction()const
			{
				return false;
			}
			void* Invoke(void* object, void* funcPtrPtr, void** arguments)const
			{
				typename RemoveCVR<T0>::Type* argument0 = (typename RemoveCVR<T0>::Type*)arguments[0];
 				typename RemoveCVR<T1>::Type* argument1 = (typename RemoveCVR<T1>::Type*)arguments[1];
 				typename RemoveCVR<T2>::Type* argument2 = (typename RemoveCVR<T2>::Type*)arguments[2];
 				typename RemoveCVR<T3>::Type* argument3 = (typename RemoveCVR<T3>::Type*)arguments[3];
 				typename RemoveCVR<T4>::Type* argument4 = (typename RemoveCVR<T4>::Type*)arguments[4];
 				typename RemoveCVR<T5>::Type* argument5 = (typename RemoveCVR<T5>::Type*)arguments[5];
 				typename RemoveCVR<T6>::Type* argument6 = (typename RemoveCVR<T6>::Type*)arguments[6];
 				typename RemoveCVR<T7>::Type* argument7 = (typename RemoveCVR<T7>::Type*)arguments[7];
 
				R result=(((C*)object)->*(*((FunctionPointer*)funcPtrPtr)))(*argument0,*argument1,*argument2,*argument3,*argument4,*argument5,*argument6,*argument7);
				return new R(result);
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
				delete (FunctionPointer*)instance;
			}
		};
		template<typename C ,typename R,typename T0,typename T1,typename T2,typename T3,typename T4,typename T5,typename T6,typename T7>
		class ObjectTypeInformation<R(C::*)(T0,T1,T2,T3,T4,T5,T6,T7) >
		{
		public:
			static WString GetName()
			{
				return ObjectTypeInformation<R>::GetName()+L"("+ObjectTypeInformation<C>::GetName()+L"::*)"+L"("+ObjectTypeInformation<T0>::GetName()+L","+ObjectTypeInformation<T1>::GetName()+L","+ObjectTypeInformation<T2>::GetName()+L","+ObjectTypeInformation<T3>::GetName()+L","+ObjectTypeInformation<T4>::GetName()+L","+ObjectTypeInformation<T5>::GetName()+L","+ObjectTypeInformation<T6>::GetName()+L","+ObjectTypeInformation<T7>::GetName()+L")" ;
			}
			typedef ObjectTypeFunction<R(C::*)(T0,T1,T2,T3,T4,T5,T6,T7) > ObjectTypeImplementation;
		};
		template<typename C ,typename R,typename T0,typename T1,typename T2,typename T3,typename T4,typename T5,typename T6,typename T7>
		class ObjectTypeRegister<R(C::*)(T0,T1,T2,T3,T4,T5,T6,T7) >
		{
		public:
			static void Register()
			{
				ObjectTypeFunction<R(C::*)(T0,T1,T2,T3,T4,T5,T6,T7) >* type=new ObjectTypeFunction<R(C::*)(T0,T1,T2,T3,T4,T5,T6,T7) >;
				RegisterObjectType(ObjectTypeInformation<R(C::*)(T0,T1,T2,T3,T4,T5,T6,T7) >::GetName(), type);
				type->parameterTypes.Add(TypeOf<T0>());
 				type->parameterTypes.Add(TypeOf<T1>());
 				type->parameterTypes.Add(TypeOf<T2>());
 				type->parameterTypes.Add(TypeOf<T3>());
 				type->parameterTypes.Add(TypeOf<T4>());
 				type->parameterTypes.Add(TypeOf<T5>());
 				type->parameterTypes.Add(TypeOf<T6>());
 				type->parameterTypes.Add(TypeOf<T7>());
 
				type->returnType=TypeOf<R>();
				type->thisType=TypeOf<C>();
				type->constructors.Add(new ObjectTypeFunction<R(C::*)(T0,T1,T2,T3,T4,T5,T6,T7) >::Constructor<void*(*)()>(&ObjectTypeFunction<R(C::*)(T0,T1,T2,T3,T4,T5,T6,T7) >::ctor));
			}
		};
 
/***********************************************************************
vl::objectmodel::ObjectTypeFunction<void(C::*)(T0,T1,T2,T3,T4,T5,T6,T7) >
***********************************************************************/
		template<typename C  ,typename T0,typename T1,typename T2,typename T3,typename T4,typename T5,typename T6,typename T7>
		class ObjectTypeFunction<void(C::*)(T0,T1,T2,T3,T4,T5,T6,T7) > : public SpecifiedObjectType<void(C::*)(T0,T1,T2,T3,T4,T5,T6,T7) >
		{
			friend class ObjectTypeRegister<void(C::*)(T0,T1,T2,T3,T4,T5,T6,T7) >;
			typedef void(C::*FunctionPointer)(T0,T1,T2,T3,T4,T5,T6,T7) ;
		protected:
			static void* ctor()
			{
				return new FunctionPointer(0);
			}
			collections::List<ObjectType*>		parameterTypes;
			ObjectType*							returnType;
			ObjectType*							thisType;
			collections::List<ObjectMember*>	constructors;
		public:
			ObjectTypeFunction()
			{
				returnType=0;
				thisType=0;
			}
			~ObjectTypeFunction()
			{
				for(int i=0;i<constructors.Count();i++)
				{
					delete constructors[i];
				}
			}
			ObjectCategory Category()const
			{
				return ObjectType::Function;
			}
			const ObjectType* ReturnType()const
			{
				return returnType;
			}
			const TypeList& ParameterTypes()const
			{
				return parameterTypes.Wrap();
			}
			const ObjectType* ThisType()const
			{
				return thisType;
			}
			bool IsConstFunction()const
			{
				return false;
			}
			void* Invoke(void* object, void* funcPtrPtr, void** arguments)const
			{
				typename RemoveCVR<T0>::Type* argument0 = (typename RemoveCVR<T0>::Type*)arguments[0];
 				typename RemoveCVR<T1>::Type* argument1 = (typename RemoveCVR<T1>::Type*)arguments[1];
 				typename RemoveCVR<T2>::Type* argument2 = (typename RemoveCVR<T2>::Type*)arguments[2];
 				typename RemoveCVR<T3>::Type* argument3 = (typename RemoveCVR<T3>::Type*)arguments[3];
 				typename RemoveCVR<T4>::Type* argument4 = (typename RemoveCVR<T4>::Type*)arguments[4];
 				typename RemoveCVR<T5>::Type* argument5 = (typename RemoveCVR<T5>::Type*)arguments[5];
 				typename RemoveCVR<T6>::Type* argument6 = (typename RemoveCVR<T6>::Type*)arguments[6];
 				typename RemoveCVR<T7>::Type* argument7 = (typename RemoveCVR<T7>::Type*)arguments[7];
 
				 (((C*)object)->*(*((FunctionPointer*)funcPtrPtr)))(*argument0,*argument1,*argument2,*argument3,*argument4,*argument5,*argument6,*argument7);
				return 0;
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
				delete (FunctionPointer*)instance;
			}
		};
		template<typename C  ,typename T0,typename T1,typename T2,typename T3,typename T4,typename T5,typename T6,typename T7>
		class ObjectTypeInformation<void(C::*)(T0,T1,T2,T3,T4,T5,T6,T7) >
		{
		public:
			static WString GetName()
			{
				return ObjectTypeInformation<void>::GetName()+L"("+ObjectTypeInformation<C>::GetName()+L"::*)"+L"("+ObjectTypeInformation<T0>::GetName()+L","+ObjectTypeInformation<T1>::GetName()+L","+ObjectTypeInformation<T2>::GetName()+L","+ObjectTypeInformation<T3>::GetName()+L","+ObjectTypeInformation<T4>::GetName()+L","+ObjectTypeInformation<T5>::GetName()+L","+ObjectTypeInformation<T6>::GetName()+L","+ObjectTypeInformation<T7>::GetName()+L")" ;
			}
			typedef ObjectTypeFunction<void(C::*)(T0,T1,T2,T3,T4,T5,T6,T7) > ObjectTypeImplementation;
		};
		template<typename C  ,typename T0,typename T1,typename T2,typename T3,typename T4,typename T5,typename T6,typename T7>
		class ObjectTypeRegister<void(C::*)(T0,T1,T2,T3,T4,T5,T6,T7) >
		{
		public:
			static void Register()
			{
				ObjectTypeFunction<void(C::*)(T0,T1,T2,T3,T4,T5,T6,T7) >* type=new ObjectTypeFunction<void(C::*)(T0,T1,T2,T3,T4,T5,T6,T7) >;
				RegisterObjectType(ObjectTypeInformation<void(C::*)(T0,T1,T2,T3,T4,T5,T6,T7) >::GetName(), type);
				type->parameterTypes.Add(TypeOf<T0>());
 				type->parameterTypes.Add(TypeOf<T1>());
 				type->parameterTypes.Add(TypeOf<T2>());
 				type->parameterTypes.Add(TypeOf<T3>());
 				type->parameterTypes.Add(TypeOf<T4>());
 				type->parameterTypes.Add(TypeOf<T5>());
 				type->parameterTypes.Add(TypeOf<T6>());
 				type->parameterTypes.Add(TypeOf<T7>());
 
				type->returnType=TypeOf<void>();
				type->thisType=TypeOf<C>();
				type->constructors.Add(new ObjectTypeFunction<void(C::*)(T0,T1,T2,T3,T4,T5,T6,T7) >::Constructor<void*(*)()>(&ObjectTypeFunction<void(C::*)(T0,T1,T2,T3,T4,T5,T6,T7) >::ctor));
			}
		};
 
/***********************************************************************
vl::objectmodel::ObjectTypeFunction<R(C::*)(T0,T1,T2,T3,T4,T5,T6,T7)const>
***********************************************************************/
		template<typename C ,typename R,typename T0,typename T1,typename T2,typename T3,typename T4,typename T5,typename T6,typename T7>
		class ObjectTypeFunction<R(C::*)(T0,T1,T2,T3,T4,T5,T6,T7)const> : public SpecifiedObjectType<R(C::*)(T0,T1,T2,T3,T4,T5,T6,T7)const>
		{
			friend class ObjectTypeRegister<R(C::*)(T0,T1,T2,T3,T4,T5,T6,T7)const>;
			typedef R(C::*FunctionPointer)(T0,T1,T2,T3,T4,T5,T6,T7)const;
		protected:
			static void* ctor()
			{
				return new FunctionPointer(0);
			}
			collections::List<ObjectType*>		parameterTypes;
			ObjectType*							returnType;
			ObjectType*							thisType;
			collections::List<ObjectMember*>	constructors;
		public:
			ObjectTypeFunction()
			{
				returnType=0;
				thisType=0;
			}
			~ObjectTypeFunction()
			{
				for(int i=0;i<constructors.Count();i++)
				{
					delete constructors[i];
				}
			}
			ObjectCategory Category()const
			{
				return ObjectType::Function;
			}
			const ObjectType* ReturnType()const
			{
				return returnType;
			}
			const TypeList& ParameterTypes()const
			{
				return parameterTypes.Wrap();
			}
			const ObjectType* ThisType()const
			{
				return thisType;
			}
			bool IsConstFunction()const
			{
				return true;
			}
			void* Invoke(void* object, void* funcPtrPtr, void** arguments)const
			{
				typename RemoveCVR<T0>::Type* argument0 = (typename RemoveCVR<T0>::Type*)arguments[0];
 				typename RemoveCVR<T1>::Type* argument1 = (typename RemoveCVR<T1>::Type*)arguments[1];
 				typename RemoveCVR<T2>::Type* argument2 = (typename RemoveCVR<T2>::Type*)arguments[2];
 				typename RemoveCVR<T3>::Type* argument3 = (typename RemoveCVR<T3>::Type*)arguments[3];
 				typename RemoveCVR<T4>::Type* argument4 = (typename RemoveCVR<T4>::Type*)arguments[4];
 				typename RemoveCVR<T5>::Type* argument5 = (typename RemoveCVR<T5>::Type*)arguments[5];
 				typename RemoveCVR<T6>::Type* argument6 = (typename RemoveCVR<T6>::Type*)arguments[6];
 				typename RemoveCVR<T7>::Type* argument7 = (typename RemoveCVR<T7>::Type*)arguments[7];
 
				R result=(((C*)object)->*(*((FunctionPointer*)funcPtrPtr)))(*argument0,*argument1,*argument2,*argument3,*argument4,*argument5,*argument6,*argument7);
				return new R(result);
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
				delete (FunctionPointer*)instance;
			}
		};
		template<typename C ,typename R,typename T0,typename T1,typename T2,typename T3,typename T4,typename T5,typename T6,typename T7>
		class ObjectTypeInformation<R(C::*)(T0,T1,T2,T3,T4,T5,T6,T7)const>
		{
		public:
			static WString GetName()
			{
				return ObjectTypeInformation<R>::GetName()+L"("+ObjectTypeInformation<C>::GetName()+L"::*)"+L"("+ObjectTypeInformation<T0>::GetName()+L","+ObjectTypeInformation<T1>::GetName()+L","+ObjectTypeInformation<T2>::GetName()+L","+ObjectTypeInformation<T3>::GetName()+L","+ObjectTypeInformation<T4>::GetName()+L","+ObjectTypeInformation<T5>::GetName()+L","+ObjectTypeInformation<T6>::GetName()+L","+ObjectTypeInformation<T7>::GetName()+L")"+L"const";
			}
			typedef ObjectTypeFunction<R(C::*)(T0,T1,T2,T3,T4,T5,T6,T7)const> ObjectTypeImplementation;
		};
		template<typename C ,typename R,typename T0,typename T1,typename T2,typename T3,typename T4,typename T5,typename T6,typename T7>
		class ObjectTypeRegister<R(C::*)(T0,T1,T2,T3,T4,T5,T6,T7)const>
		{
		public:
			static void Register()
			{
				ObjectTypeFunction<R(C::*)(T0,T1,T2,T3,T4,T5,T6,T7)const>* type=new ObjectTypeFunction<R(C::*)(T0,T1,T2,T3,T4,T5,T6,T7)const>;
				RegisterObjectType(ObjectTypeInformation<R(C::*)(T0,T1,T2,T3,T4,T5,T6,T7)const>::GetName(), type);
				type->parameterTypes.Add(TypeOf<T0>());
 				type->parameterTypes.Add(TypeOf<T1>());
 				type->parameterTypes.Add(TypeOf<T2>());
 				type->parameterTypes.Add(TypeOf<T3>());
 				type->parameterTypes.Add(TypeOf<T4>());
 				type->parameterTypes.Add(TypeOf<T5>());
 				type->parameterTypes.Add(TypeOf<T6>());
 				type->parameterTypes.Add(TypeOf<T7>());
 
				type->returnType=TypeOf<R>();
				type->thisType=TypeOf<C>();
				type->constructors.Add(new ObjectTypeFunction<R(C::*)(T0,T1,T2,T3,T4,T5,T6,T7)const>::Constructor<void*(*)()>(&ObjectTypeFunction<R(C::*)(T0,T1,T2,T3,T4,T5,T6,T7)const>::ctor));
			}
		};
 
/***********************************************************************
vl::objectmodel::ObjectTypeFunction<void(C::*)(T0,T1,T2,T3,T4,T5,T6,T7)const>
***********************************************************************/
		template<typename C  ,typename T0,typename T1,typename T2,typename T3,typename T4,typename T5,typename T6,typename T7>
		class ObjectTypeFunction<void(C::*)(T0,T1,T2,T3,T4,T5,T6,T7)const> : public SpecifiedObjectType<void(C::*)(T0,T1,T2,T3,T4,T5,T6,T7)const>
		{
			friend class ObjectTypeRegister<void(C::*)(T0,T1,T2,T3,T4,T5,T6,T7)const>;
			typedef void(C::*FunctionPointer)(T0,T1,T2,T3,T4,T5,T6,T7)const;
		protected:
			static void* ctor()
			{
				return new FunctionPointer(0);
			}
			collections::List<ObjectType*>		parameterTypes;
			ObjectType*							returnType;
			ObjectType*							thisType;
			collections::List<ObjectMember*>	constructors;
		public:
			ObjectTypeFunction()
			{
				returnType=0;
				thisType=0;
			}
			~ObjectTypeFunction()
			{
				for(int i=0;i<constructors.Count();i++)
				{
					delete constructors[i];
				}
			}
			ObjectCategory Category()const
			{
				return ObjectType::Function;
			}
			const ObjectType* ReturnType()const
			{
				return returnType;
			}
			const TypeList& ParameterTypes()const
			{
				return parameterTypes.Wrap();
			}
			const ObjectType* ThisType()const
			{
				return thisType;
			}
			bool IsConstFunction()const
			{
				return true;
			}
			void* Invoke(void* object, void* funcPtrPtr, void** arguments)const
			{
				typename RemoveCVR<T0>::Type* argument0 = (typename RemoveCVR<T0>::Type*)arguments[0];
 				typename RemoveCVR<T1>::Type* argument1 = (typename RemoveCVR<T1>::Type*)arguments[1];
 				typename RemoveCVR<T2>::Type* argument2 = (typename RemoveCVR<T2>::Type*)arguments[2];
 				typename RemoveCVR<T3>::Type* argument3 = (typename RemoveCVR<T3>::Type*)arguments[3];
 				typename RemoveCVR<T4>::Type* argument4 = (typename RemoveCVR<T4>::Type*)arguments[4];
 				typename RemoveCVR<T5>::Type* argument5 = (typename RemoveCVR<T5>::Type*)arguments[5];
 				typename RemoveCVR<T6>::Type* argument6 = (typename RemoveCVR<T6>::Type*)arguments[6];
 				typename RemoveCVR<T7>::Type* argument7 = (typename RemoveCVR<T7>::Type*)arguments[7];
 
				 (((C*)object)->*(*((FunctionPointer*)funcPtrPtr)))(*argument0,*argument1,*argument2,*argument3,*argument4,*argument5,*argument6,*argument7);
				return 0;
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
				delete (FunctionPointer*)instance;
			}
		};
		template<typename C  ,typename T0,typename T1,typename T2,typename T3,typename T4,typename T5,typename T6,typename T7>
		class ObjectTypeInformation<void(C::*)(T0,T1,T2,T3,T4,T5,T6,T7)const>
		{
		public:
			static WString GetName()
			{
				return ObjectTypeInformation<void>::GetName()+L"("+ObjectTypeInformation<C>::GetName()+L"::*)"+L"("+ObjectTypeInformation<T0>::GetName()+L","+ObjectTypeInformation<T1>::GetName()+L","+ObjectTypeInformation<T2>::GetName()+L","+ObjectTypeInformation<T3>::GetName()+L","+ObjectTypeInformation<T4>::GetName()+L","+ObjectTypeInformation<T5>::GetName()+L","+ObjectTypeInformation<T6>::GetName()+L","+ObjectTypeInformation<T7>::GetName()+L")"+L"const";
			}
			typedef ObjectTypeFunction<void(C::*)(T0,T1,T2,T3,T4,T5,T6,T7)const> ObjectTypeImplementation;
		};
		template<typename C  ,typename T0,typename T1,typename T2,typename T3,typename T4,typename T5,typename T6,typename T7>
		class ObjectTypeRegister<void(C::*)(T0,T1,T2,T3,T4,T5,T6,T7)const>
		{
		public:
			static void Register()
			{
				ObjectTypeFunction<void(C::*)(T0,T1,T2,T3,T4,T5,T6,T7)const>* type=new ObjectTypeFunction<void(C::*)(T0,T1,T2,T3,T4,T5,T6,T7)const>;
				RegisterObjectType(ObjectTypeInformation<void(C::*)(T0,T1,T2,T3,T4,T5,T6,T7)const>::GetName(), type);
				type->parameterTypes.Add(TypeOf<T0>());
 				type->parameterTypes.Add(TypeOf<T1>());
 				type->parameterTypes.Add(TypeOf<T2>());
 				type->parameterTypes.Add(TypeOf<T3>());
 				type->parameterTypes.Add(TypeOf<T4>());
 				type->parameterTypes.Add(TypeOf<T5>());
 				type->parameterTypes.Add(TypeOf<T6>());
 				type->parameterTypes.Add(TypeOf<T7>());
 
				type->returnType=TypeOf<void>();
				type->thisType=TypeOf<C>();
				type->constructors.Add(new ObjectTypeFunction<void(C::*)(T0,T1,T2,T3,T4,T5,T6,T7)const>::Constructor<void*(*)()>(&ObjectTypeFunction<void(C::*)(T0,T1,T2,T3,T4,T5,T6,T7)const>::ctor));
			}
		};
  
/***********************************************************************
vl::objectmodel::ObjectTypeFunction<R(*)(T0,T1,T2,T3,T4,T5,T6,T7,T8) >
***********************************************************************/
		template<typename R,typename T0,typename T1,typename T2,typename T3,typename T4,typename T5,typename T6,typename T7,typename T8>
		class ObjectTypeFunction<R(*)(T0,T1,T2,T3,T4,T5,T6,T7,T8) > : public SpecifiedObjectType<R(*)(T0,T1,T2,T3,T4,T5,T6,T7,T8) >
		{
			friend class ObjectTypeRegister<R(*)(T0,T1,T2,T3,T4,T5,T6,T7,T8) >;
			typedef R(*FunctionPointer)(T0,T1,T2,T3,T4,T5,T6,T7,T8) ;
		protected:
			static void* ctor()
			{
				return new FunctionPointer(0);
			}
			collections::List<ObjectType*>		parameterTypes;
			ObjectType*							returnType;
			ObjectType*							thisType;
			collections::List<ObjectMember*>	constructors;
		public:
			ObjectTypeFunction()
			{
				returnType=0;
				thisType=0;
			}
			~ObjectTypeFunction()
			{
				for(int i=0;i<constructors.Count();i++)
				{
					delete constructors[i];
				}
			}
			ObjectCategory Category()const
			{
				return ObjectType::Function;
			}
			const ObjectType* ReturnType()const
			{
				return returnType;
			}
			const TypeList& ParameterTypes()const
			{
				return parameterTypes.Wrap();
			}
			const ObjectType* ThisType()const
			{
				return thisType;
			}
			bool IsConstFunction()const
			{
				return false;
			}
			void* Invoke(void* object, void* funcPtrPtr, void** arguments)const
			{
				typename RemoveCVR<T0>::Type* argument0 = (typename RemoveCVR<T0>::Type*)arguments[0];
 				typename RemoveCVR<T1>::Type* argument1 = (typename RemoveCVR<T1>::Type*)arguments[1];
 				typename RemoveCVR<T2>::Type* argument2 = (typename RemoveCVR<T2>::Type*)arguments[2];
 				typename RemoveCVR<T3>::Type* argument3 = (typename RemoveCVR<T3>::Type*)arguments[3];
 				typename RemoveCVR<T4>::Type* argument4 = (typename RemoveCVR<T4>::Type*)arguments[4];
 				typename RemoveCVR<T5>::Type* argument5 = (typename RemoveCVR<T5>::Type*)arguments[5];
 				typename RemoveCVR<T6>::Type* argument6 = (typename RemoveCVR<T6>::Type*)arguments[6];
 				typename RemoveCVR<T7>::Type* argument7 = (typename RemoveCVR<T7>::Type*)arguments[7];
 				typename RemoveCVR<T8>::Type* argument8 = (typename RemoveCVR<T8>::Type*)arguments[8];
 
				R result=(*((FunctionPointer*)funcPtrPtr))(*argument0,*argument1,*argument2,*argument3,*argument4,*argument5,*argument6,*argument7,*argument8);
				return new R(result);
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
				delete (FunctionPointer*)instance;
			}
		};
		template<typename R,typename T0,typename T1,typename T2,typename T3,typename T4,typename T5,typename T6,typename T7,typename T8>
		class ObjectTypeInformation<R(*)(T0,T1,T2,T3,T4,T5,T6,T7,T8) >
		{
		public:
			static WString GetName()
			{
				return ObjectTypeInformation<R>::GetName()+L"(*)"+L"("+ObjectTypeInformation<T0>::GetName()+L","+ObjectTypeInformation<T1>::GetName()+L","+ObjectTypeInformation<T2>::GetName()+L","+ObjectTypeInformation<T3>::GetName()+L","+ObjectTypeInformation<T4>::GetName()+L","+ObjectTypeInformation<T5>::GetName()+L","+ObjectTypeInformation<T6>::GetName()+L","+ObjectTypeInformation<T7>::GetName()+L","+ObjectTypeInformation<T8>::GetName()+L")" ;
			}
			typedef ObjectTypeFunction<R(*)(T0,T1,T2,T3,T4,T5,T6,T7,T8) > ObjectTypeImplementation;
		};
		template<typename R,typename T0,typename T1,typename T2,typename T3,typename T4,typename T5,typename T6,typename T7,typename T8>
		class ObjectTypeRegister<R(*)(T0,T1,T2,T3,T4,T5,T6,T7,T8) >
		{
		public:
			static void Register()
			{
				ObjectTypeFunction<R(*)(T0,T1,T2,T3,T4,T5,T6,T7,T8) >* type=new ObjectTypeFunction<R(*)(T0,T1,T2,T3,T4,T5,T6,T7,T8) >;
				RegisterObjectType(ObjectTypeInformation<R(*)(T0,T1,T2,T3,T4,T5,T6,T7,T8) >::GetName(), type);
				type->parameterTypes.Add(TypeOf<T0>());
 				type->parameterTypes.Add(TypeOf<T1>());
 				type->parameterTypes.Add(TypeOf<T2>());
 				type->parameterTypes.Add(TypeOf<T3>());
 				type->parameterTypes.Add(TypeOf<T4>());
 				type->parameterTypes.Add(TypeOf<T5>());
 				type->parameterTypes.Add(TypeOf<T6>());
 				type->parameterTypes.Add(TypeOf<T7>());
 				type->parameterTypes.Add(TypeOf<T8>());
 
				type->returnType=TypeOf<R>();
				type->thisType=0;
				type->constructors.Add(new ObjectTypeFunction<R(*)(T0,T1,T2,T3,T4,T5,T6,T7,T8) >::Constructor<void*(*)()>(&ObjectTypeFunction<R(*)(T0,T1,T2,T3,T4,T5,T6,T7,T8) >::ctor));
			}
		};
 
/***********************************************************************
vl::objectmodel::ObjectTypeFunction<void(*)(T0,T1,T2,T3,T4,T5,T6,T7,T8) >
***********************************************************************/
		template< typename T0,typename T1,typename T2,typename T3,typename T4,typename T5,typename T6,typename T7,typename T8>
		class ObjectTypeFunction<void(*)(T0,T1,T2,T3,T4,T5,T6,T7,T8) > : public SpecifiedObjectType<void(*)(T0,T1,T2,T3,T4,T5,T6,T7,T8) >
		{
			friend class ObjectTypeRegister<void(*)(T0,T1,T2,T3,T4,T5,T6,T7,T8) >;
			typedef void(*FunctionPointer)(T0,T1,T2,T3,T4,T5,T6,T7,T8) ;
		protected:
			static void* ctor()
			{
				return new FunctionPointer(0);
			}
			collections::List<ObjectType*>		parameterTypes;
			ObjectType*							returnType;
			ObjectType*							thisType;
			collections::List<ObjectMember*>	constructors;
		public:
			ObjectTypeFunction()
			{
				returnType=0;
				thisType=0;
			}
			~ObjectTypeFunction()
			{
				for(int i=0;i<constructors.Count();i++)
				{
					delete constructors[i];
				}
			}
			ObjectCategory Category()const
			{
				return ObjectType::Function;
			}
			const ObjectType* ReturnType()const
			{
				return returnType;
			}
			const TypeList& ParameterTypes()const
			{
				return parameterTypes.Wrap();
			}
			const ObjectType* ThisType()const
			{
				return thisType;
			}
			bool IsConstFunction()const
			{
				return false;
			}
			void* Invoke(void* object, void* funcPtrPtr, void** arguments)const
			{
				typename RemoveCVR<T0>::Type* argument0 = (typename RemoveCVR<T0>::Type*)arguments[0];
 				typename RemoveCVR<T1>::Type* argument1 = (typename RemoveCVR<T1>::Type*)arguments[1];
 				typename RemoveCVR<T2>::Type* argument2 = (typename RemoveCVR<T2>::Type*)arguments[2];
 				typename RemoveCVR<T3>::Type* argument3 = (typename RemoveCVR<T3>::Type*)arguments[3];
 				typename RemoveCVR<T4>::Type* argument4 = (typename RemoveCVR<T4>::Type*)arguments[4];
 				typename RemoveCVR<T5>::Type* argument5 = (typename RemoveCVR<T5>::Type*)arguments[5];
 				typename RemoveCVR<T6>::Type* argument6 = (typename RemoveCVR<T6>::Type*)arguments[6];
 				typename RemoveCVR<T7>::Type* argument7 = (typename RemoveCVR<T7>::Type*)arguments[7];
 				typename RemoveCVR<T8>::Type* argument8 = (typename RemoveCVR<T8>::Type*)arguments[8];
 
				 (*((FunctionPointer*)funcPtrPtr))(*argument0,*argument1,*argument2,*argument3,*argument4,*argument5,*argument6,*argument7,*argument8);
				return 0;
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
				delete (FunctionPointer*)instance;
			}
		};
		template< typename T0,typename T1,typename T2,typename T3,typename T4,typename T5,typename T6,typename T7,typename T8>
		class ObjectTypeInformation<void(*)(T0,T1,T2,T3,T4,T5,T6,T7,T8) >
		{
		public:
			static WString GetName()
			{
				return ObjectTypeInformation<void>::GetName()+L"(*)"+L"("+ObjectTypeInformation<T0>::GetName()+L","+ObjectTypeInformation<T1>::GetName()+L","+ObjectTypeInformation<T2>::GetName()+L","+ObjectTypeInformation<T3>::GetName()+L","+ObjectTypeInformation<T4>::GetName()+L","+ObjectTypeInformation<T5>::GetName()+L","+ObjectTypeInformation<T6>::GetName()+L","+ObjectTypeInformation<T7>::GetName()+L","+ObjectTypeInformation<T8>::GetName()+L")" ;
			}
			typedef ObjectTypeFunction<void(*)(T0,T1,T2,T3,T4,T5,T6,T7,T8) > ObjectTypeImplementation;
		};
		template< typename T0,typename T1,typename T2,typename T3,typename T4,typename T5,typename T6,typename T7,typename T8>
		class ObjectTypeRegister<void(*)(T0,T1,T2,T3,T4,T5,T6,T7,T8) >
		{
		public:
			static void Register()
			{
				ObjectTypeFunction<void(*)(T0,T1,T2,T3,T4,T5,T6,T7,T8) >* type=new ObjectTypeFunction<void(*)(T0,T1,T2,T3,T4,T5,T6,T7,T8) >;
				RegisterObjectType(ObjectTypeInformation<void(*)(T0,T1,T2,T3,T4,T5,T6,T7,T8) >::GetName(), type);
				type->parameterTypes.Add(TypeOf<T0>());
 				type->parameterTypes.Add(TypeOf<T1>());
 				type->parameterTypes.Add(TypeOf<T2>());
 				type->parameterTypes.Add(TypeOf<T3>());
 				type->parameterTypes.Add(TypeOf<T4>());
 				type->parameterTypes.Add(TypeOf<T5>());
 				type->parameterTypes.Add(TypeOf<T6>());
 				type->parameterTypes.Add(TypeOf<T7>());
 				type->parameterTypes.Add(TypeOf<T8>());
 
				type->returnType=TypeOf<void>();
				type->thisType=0;
				type->constructors.Add(new ObjectTypeFunction<void(*)(T0,T1,T2,T3,T4,T5,T6,T7,T8) >::Constructor<void*(*)()>(&ObjectTypeFunction<void(*)(T0,T1,T2,T3,T4,T5,T6,T7,T8) >::ctor));
			}
		};
 
/***********************************************************************
vl::objectmodel::ObjectTypeFunction<R(C::*)(T0,T1,T2,T3,T4,T5,T6,T7,T8) >
***********************************************************************/
		template<typename C ,typename R,typename T0,typename T1,typename T2,typename T3,typename T4,typename T5,typename T6,typename T7,typename T8>
		class ObjectTypeFunction<R(C::*)(T0,T1,T2,T3,T4,T5,T6,T7,T8) > : public SpecifiedObjectType<R(C::*)(T0,T1,T2,T3,T4,T5,T6,T7,T8) >
		{
			friend class ObjectTypeRegister<R(C::*)(T0,T1,T2,T3,T4,T5,T6,T7,T8) >;
			typedef R(C::*FunctionPointer)(T0,T1,T2,T3,T4,T5,T6,T7,T8) ;
		protected:
			static void* ctor()
			{
				return new FunctionPointer(0);
			}
			collections::List<ObjectType*>		parameterTypes;
			ObjectType*							returnType;
			ObjectType*							thisType;
			collections::List<ObjectMember*>	constructors;
		public:
			ObjectTypeFunction()
			{
				returnType=0;
				thisType=0;
			}
			~ObjectTypeFunction()
			{
				for(int i=0;i<constructors.Count();i++)
				{
					delete constructors[i];
				}
			}
			ObjectCategory Category()const
			{
				return ObjectType::Function;
			}
			const ObjectType* ReturnType()const
			{
				return returnType;
			}
			const TypeList& ParameterTypes()const
			{
				return parameterTypes.Wrap();
			}
			const ObjectType* ThisType()const
			{
				return thisType;
			}
			bool IsConstFunction()const
			{
				return false;
			}
			void* Invoke(void* object, void* funcPtrPtr, void** arguments)const
			{
				typename RemoveCVR<T0>::Type* argument0 = (typename RemoveCVR<T0>::Type*)arguments[0];
 				typename RemoveCVR<T1>::Type* argument1 = (typename RemoveCVR<T1>::Type*)arguments[1];
 				typename RemoveCVR<T2>::Type* argument2 = (typename RemoveCVR<T2>::Type*)arguments[2];
 				typename RemoveCVR<T3>::Type* argument3 = (typename RemoveCVR<T3>::Type*)arguments[3];
 				typename RemoveCVR<T4>::Type* argument4 = (typename RemoveCVR<T4>::Type*)arguments[4];
 				typename RemoveCVR<T5>::Type* argument5 = (typename RemoveCVR<T5>::Type*)arguments[5];
 				typename RemoveCVR<T6>::Type* argument6 = (typename RemoveCVR<T6>::Type*)arguments[6];
 				typename RemoveCVR<T7>::Type* argument7 = (typename RemoveCVR<T7>::Type*)arguments[7];
 				typename RemoveCVR<T8>::Type* argument8 = (typename RemoveCVR<T8>::Type*)arguments[8];
 
				R result=(((C*)object)->*(*((FunctionPointer*)funcPtrPtr)))(*argument0,*argument1,*argument2,*argument3,*argument4,*argument5,*argument6,*argument7,*argument8);
				return new R(result);
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
				delete (FunctionPointer*)instance;
			}
		};
		template<typename C ,typename R,typename T0,typename T1,typename T2,typename T3,typename T4,typename T5,typename T6,typename T7,typename T8>
		class ObjectTypeInformation<R(C::*)(T0,T1,T2,T3,T4,T5,T6,T7,T8) >
		{
		public:
			static WString GetName()
			{
				return ObjectTypeInformation<R>::GetName()+L"("+ObjectTypeInformation<C>::GetName()+L"::*)"+L"("+ObjectTypeInformation<T0>::GetName()+L","+ObjectTypeInformation<T1>::GetName()+L","+ObjectTypeInformation<T2>::GetName()+L","+ObjectTypeInformation<T3>::GetName()+L","+ObjectTypeInformation<T4>::GetName()+L","+ObjectTypeInformation<T5>::GetName()+L","+ObjectTypeInformation<T6>::GetName()+L","+ObjectTypeInformation<T7>::GetName()+L","+ObjectTypeInformation<T8>::GetName()+L")" ;
			}
			typedef ObjectTypeFunction<R(C::*)(T0,T1,T2,T3,T4,T5,T6,T7,T8) > ObjectTypeImplementation;
		};
		template<typename C ,typename R,typename T0,typename T1,typename T2,typename T3,typename T4,typename T5,typename T6,typename T7,typename T8>
		class ObjectTypeRegister<R(C::*)(T0,T1,T2,T3,T4,T5,T6,T7,T8) >
		{
		public:
			static void Register()
			{
				ObjectTypeFunction<R(C::*)(T0,T1,T2,T3,T4,T5,T6,T7,T8) >* type=new ObjectTypeFunction<R(C::*)(T0,T1,T2,T3,T4,T5,T6,T7,T8) >;
				RegisterObjectType(ObjectTypeInformation<R(C::*)(T0,T1,T2,T3,T4,T5,T6,T7,T8) >::GetName(), type);
				type->parameterTypes.Add(TypeOf<T0>());
 				type->parameterTypes.Add(TypeOf<T1>());
 				type->parameterTypes.Add(TypeOf<T2>());
 				type->parameterTypes.Add(TypeOf<T3>());
 				type->parameterTypes.Add(TypeOf<T4>());
 				type->parameterTypes.Add(TypeOf<T5>());
 				type->parameterTypes.Add(TypeOf<T6>());
 				type->parameterTypes.Add(TypeOf<T7>());
 				type->parameterTypes.Add(TypeOf<T8>());
 
				type->returnType=TypeOf<R>();
				type->thisType=TypeOf<C>();
				type->constructors.Add(new ObjectTypeFunction<R(C::*)(T0,T1,T2,T3,T4,T5,T6,T7,T8) >::Constructor<void*(*)()>(&ObjectTypeFunction<R(C::*)(T0,T1,T2,T3,T4,T5,T6,T7,T8) >::ctor));
			}
		};
 
/***********************************************************************
vl::objectmodel::ObjectTypeFunction<void(C::*)(T0,T1,T2,T3,T4,T5,T6,T7,T8) >
***********************************************************************/
		template<typename C  ,typename T0,typename T1,typename T2,typename T3,typename T4,typename T5,typename T6,typename T7,typename T8>
		class ObjectTypeFunction<void(C::*)(T0,T1,T2,T3,T4,T5,T6,T7,T8) > : public SpecifiedObjectType<void(C::*)(T0,T1,T2,T3,T4,T5,T6,T7,T8) >
		{
			friend class ObjectTypeRegister<void(C::*)(T0,T1,T2,T3,T4,T5,T6,T7,T8) >;
			typedef void(C::*FunctionPointer)(T0,T1,T2,T3,T4,T5,T6,T7,T8) ;
		protected:
			static void* ctor()
			{
				return new FunctionPointer(0);
			}
			collections::List<ObjectType*>		parameterTypes;
			ObjectType*							returnType;
			ObjectType*							thisType;
			collections::List<ObjectMember*>	constructors;
		public:
			ObjectTypeFunction()
			{
				returnType=0;
				thisType=0;
			}
			~ObjectTypeFunction()
			{
				for(int i=0;i<constructors.Count();i++)
				{
					delete constructors[i];
				}
			}
			ObjectCategory Category()const
			{
				return ObjectType::Function;
			}
			const ObjectType* ReturnType()const
			{
				return returnType;
			}
			const TypeList& ParameterTypes()const
			{
				return parameterTypes.Wrap();
			}
			const ObjectType* ThisType()const
			{
				return thisType;
			}
			bool IsConstFunction()const
			{
				return false;
			}
			void* Invoke(void* object, void* funcPtrPtr, void** arguments)const
			{
				typename RemoveCVR<T0>::Type* argument0 = (typename RemoveCVR<T0>::Type*)arguments[0];
 				typename RemoveCVR<T1>::Type* argument1 = (typename RemoveCVR<T1>::Type*)arguments[1];
 				typename RemoveCVR<T2>::Type* argument2 = (typename RemoveCVR<T2>::Type*)arguments[2];
 				typename RemoveCVR<T3>::Type* argument3 = (typename RemoveCVR<T3>::Type*)arguments[3];
 				typename RemoveCVR<T4>::Type* argument4 = (typename RemoveCVR<T4>::Type*)arguments[4];
 				typename RemoveCVR<T5>::Type* argument5 = (typename RemoveCVR<T5>::Type*)arguments[5];
 				typename RemoveCVR<T6>::Type* argument6 = (typename RemoveCVR<T6>::Type*)arguments[6];
 				typename RemoveCVR<T7>::Type* argument7 = (typename RemoveCVR<T7>::Type*)arguments[7];
 				typename RemoveCVR<T8>::Type* argument8 = (typename RemoveCVR<T8>::Type*)arguments[8];
 
				 (((C*)object)->*(*((FunctionPointer*)funcPtrPtr)))(*argument0,*argument1,*argument2,*argument3,*argument4,*argument5,*argument6,*argument7,*argument8);
				return 0;
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
				delete (FunctionPointer*)instance;
			}
		};
		template<typename C  ,typename T0,typename T1,typename T2,typename T3,typename T4,typename T5,typename T6,typename T7,typename T8>
		class ObjectTypeInformation<void(C::*)(T0,T1,T2,T3,T4,T5,T6,T7,T8) >
		{
		public:
			static WString GetName()
			{
				return ObjectTypeInformation<void>::GetName()+L"("+ObjectTypeInformation<C>::GetName()+L"::*)"+L"("+ObjectTypeInformation<T0>::GetName()+L","+ObjectTypeInformation<T1>::GetName()+L","+ObjectTypeInformation<T2>::GetName()+L","+ObjectTypeInformation<T3>::GetName()+L","+ObjectTypeInformation<T4>::GetName()+L","+ObjectTypeInformation<T5>::GetName()+L","+ObjectTypeInformation<T6>::GetName()+L","+ObjectTypeInformation<T7>::GetName()+L","+ObjectTypeInformation<T8>::GetName()+L")" ;
			}
			typedef ObjectTypeFunction<void(C::*)(T0,T1,T2,T3,T4,T5,T6,T7,T8) > ObjectTypeImplementation;
		};
		template<typename C  ,typename T0,typename T1,typename T2,typename T3,typename T4,typename T5,typename T6,typename T7,typename T8>
		class ObjectTypeRegister<void(C::*)(T0,T1,T2,T3,T4,T5,T6,T7,T8) >
		{
		public:
			static void Register()
			{
				ObjectTypeFunction<void(C::*)(T0,T1,T2,T3,T4,T5,T6,T7,T8) >* type=new ObjectTypeFunction<void(C::*)(T0,T1,T2,T3,T4,T5,T6,T7,T8) >;
				RegisterObjectType(ObjectTypeInformation<void(C::*)(T0,T1,T2,T3,T4,T5,T6,T7,T8) >::GetName(), type);
				type->parameterTypes.Add(TypeOf<T0>());
 				type->parameterTypes.Add(TypeOf<T1>());
 				type->parameterTypes.Add(TypeOf<T2>());
 				type->parameterTypes.Add(TypeOf<T3>());
 				type->parameterTypes.Add(TypeOf<T4>());
 				type->parameterTypes.Add(TypeOf<T5>());
 				type->parameterTypes.Add(TypeOf<T6>());
 				type->parameterTypes.Add(TypeOf<T7>());
 				type->parameterTypes.Add(TypeOf<T8>());
 
				type->returnType=TypeOf<void>();
				type->thisType=TypeOf<C>();
				type->constructors.Add(new ObjectTypeFunction<void(C::*)(T0,T1,T2,T3,T4,T5,T6,T7,T8) >::Constructor<void*(*)()>(&ObjectTypeFunction<void(C::*)(T0,T1,T2,T3,T4,T5,T6,T7,T8) >::ctor));
			}
		};
 
/***********************************************************************
vl::objectmodel::ObjectTypeFunction<R(C::*)(T0,T1,T2,T3,T4,T5,T6,T7,T8)const>
***********************************************************************/
		template<typename C ,typename R,typename T0,typename T1,typename T2,typename T3,typename T4,typename T5,typename T6,typename T7,typename T8>
		class ObjectTypeFunction<R(C::*)(T0,T1,T2,T3,T4,T5,T6,T7,T8)const> : public SpecifiedObjectType<R(C::*)(T0,T1,T2,T3,T4,T5,T6,T7,T8)const>
		{
			friend class ObjectTypeRegister<R(C::*)(T0,T1,T2,T3,T4,T5,T6,T7,T8)const>;
			typedef R(C::*FunctionPointer)(T0,T1,T2,T3,T4,T5,T6,T7,T8)const;
		protected:
			static void* ctor()
			{
				return new FunctionPointer(0);
			}
			collections::List<ObjectType*>		parameterTypes;
			ObjectType*							returnType;
			ObjectType*							thisType;
			collections::List<ObjectMember*>	constructors;
		public:
			ObjectTypeFunction()
			{
				returnType=0;
				thisType=0;
			}
			~ObjectTypeFunction()
			{
				for(int i=0;i<constructors.Count();i++)
				{
					delete constructors[i];
				}
			}
			ObjectCategory Category()const
			{
				return ObjectType::Function;
			}
			const ObjectType* ReturnType()const
			{
				return returnType;
			}
			const TypeList& ParameterTypes()const
			{
				return parameterTypes.Wrap();
			}
			const ObjectType* ThisType()const
			{
				return thisType;
			}
			bool IsConstFunction()const
			{
				return true;
			}
			void* Invoke(void* object, void* funcPtrPtr, void** arguments)const
			{
				typename RemoveCVR<T0>::Type* argument0 = (typename RemoveCVR<T0>::Type*)arguments[0];
 				typename RemoveCVR<T1>::Type* argument1 = (typename RemoveCVR<T1>::Type*)arguments[1];
 				typename RemoveCVR<T2>::Type* argument2 = (typename RemoveCVR<T2>::Type*)arguments[2];
 				typename RemoveCVR<T3>::Type* argument3 = (typename RemoveCVR<T3>::Type*)arguments[3];
 				typename RemoveCVR<T4>::Type* argument4 = (typename RemoveCVR<T4>::Type*)arguments[4];
 				typename RemoveCVR<T5>::Type* argument5 = (typename RemoveCVR<T5>::Type*)arguments[5];
 				typename RemoveCVR<T6>::Type* argument6 = (typename RemoveCVR<T6>::Type*)arguments[6];
 				typename RemoveCVR<T7>::Type* argument7 = (typename RemoveCVR<T7>::Type*)arguments[7];
 				typename RemoveCVR<T8>::Type* argument8 = (typename RemoveCVR<T8>::Type*)arguments[8];
 
				R result=(((C*)object)->*(*((FunctionPointer*)funcPtrPtr)))(*argument0,*argument1,*argument2,*argument3,*argument4,*argument5,*argument6,*argument7,*argument8);
				return new R(result);
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
				delete (FunctionPointer*)instance;
			}
		};
		template<typename C ,typename R,typename T0,typename T1,typename T2,typename T3,typename T4,typename T5,typename T6,typename T7,typename T8>
		class ObjectTypeInformation<R(C::*)(T0,T1,T2,T3,T4,T5,T6,T7,T8)const>
		{
		public:
			static WString GetName()
			{
				return ObjectTypeInformation<R>::GetName()+L"("+ObjectTypeInformation<C>::GetName()+L"::*)"+L"("+ObjectTypeInformation<T0>::GetName()+L","+ObjectTypeInformation<T1>::GetName()+L","+ObjectTypeInformation<T2>::GetName()+L","+ObjectTypeInformation<T3>::GetName()+L","+ObjectTypeInformation<T4>::GetName()+L","+ObjectTypeInformation<T5>::GetName()+L","+ObjectTypeInformation<T6>::GetName()+L","+ObjectTypeInformation<T7>::GetName()+L","+ObjectTypeInformation<T8>::GetName()+L")"+L"const";
			}
			typedef ObjectTypeFunction<R(C::*)(T0,T1,T2,T3,T4,T5,T6,T7,T8)const> ObjectTypeImplementation;
		};
		template<typename C ,typename R,typename T0,typename T1,typename T2,typename T3,typename T4,typename T5,typename T6,typename T7,typename T8>
		class ObjectTypeRegister<R(C::*)(T0,T1,T2,T3,T4,T5,T6,T7,T8)const>
		{
		public:
			static void Register()
			{
				ObjectTypeFunction<R(C::*)(T0,T1,T2,T3,T4,T5,T6,T7,T8)const>* type=new ObjectTypeFunction<R(C::*)(T0,T1,T2,T3,T4,T5,T6,T7,T8)const>;
				RegisterObjectType(ObjectTypeInformation<R(C::*)(T0,T1,T2,T3,T4,T5,T6,T7,T8)const>::GetName(), type);
				type->parameterTypes.Add(TypeOf<T0>());
 				type->parameterTypes.Add(TypeOf<T1>());
 				type->parameterTypes.Add(TypeOf<T2>());
 				type->parameterTypes.Add(TypeOf<T3>());
 				type->parameterTypes.Add(TypeOf<T4>());
 				type->parameterTypes.Add(TypeOf<T5>());
 				type->parameterTypes.Add(TypeOf<T6>());
 				type->parameterTypes.Add(TypeOf<T7>());
 				type->parameterTypes.Add(TypeOf<T8>());
 
				type->returnType=TypeOf<R>();
				type->thisType=TypeOf<C>();
				type->constructors.Add(new ObjectTypeFunction<R(C::*)(T0,T1,T2,T3,T4,T5,T6,T7,T8)const>::Constructor<void*(*)()>(&ObjectTypeFunction<R(C::*)(T0,T1,T2,T3,T4,T5,T6,T7,T8)const>::ctor));
			}
		};
 
/***********************************************************************
vl::objectmodel::ObjectTypeFunction<void(C::*)(T0,T1,T2,T3,T4,T5,T6,T7,T8)const>
***********************************************************************/
		template<typename C  ,typename T0,typename T1,typename T2,typename T3,typename T4,typename T5,typename T6,typename T7,typename T8>
		class ObjectTypeFunction<void(C::*)(T0,T1,T2,T3,T4,T5,T6,T7,T8)const> : public SpecifiedObjectType<void(C::*)(T0,T1,T2,T3,T4,T5,T6,T7,T8)const>
		{
			friend class ObjectTypeRegister<void(C::*)(T0,T1,T2,T3,T4,T5,T6,T7,T8)const>;
			typedef void(C::*FunctionPointer)(T0,T1,T2,T3,T4,T5,T6,T7,T8)const;
		protected:
			static void* ctor()
			{
				return new FunctionPointer(0);
			}
			collections::List<ObjectType*>		parameterTypes;
			ObjectType*							returnType;
			ObjectType*							thisType;
			collections::List<ObjectMember*>	constructors;
		public:
			ObjectTypeFunction()
			{
				returnType=0;
				thisType=0;
			}
			~ObjectTypeFunction()
			{
				for(int i=0;i<constructors.Count();i++)
				{
					delete constructors[i];
				}
			}
			ObjectCategory Category()const
			{
				return ObjectType::Function;
			}
			const ObjectType* ReturnType()const
			{
				return returnType;
			}
			const TypeList& ParameterTypes()const
			{
				return parameterTypes.Wrap();
			}
			const ObjectType* ThisType()const
			{
				return thisType;
			}
			bool IsConstFunction()const
			{
				return true;
			}
			void* Invoke(void* object, void* funcPtrPtr, void** arguments)const
			{
				typename RemoveCVR<T0>::Type* argument0 = (typename RemoveCVR<T0>::Type*)arguments[0];
 				typename RemoveCVR<T1>::Type* argument1 = (typename RemoveCVR<T1>::Type*)arguments[1];
 				typename RemoveCVR<T2>::Type* argument2 = (typename RemoveCVR<T2>::Type*)arguments[2];
 				typename RemoveCVR<T3>::Type* argument3 = (typename RemoveCVR<T3>::Type*)arguments[3];
 				typename RemoveCVR<T4>::Type* argument4 = (typename RemoveCVR<T4>::Type*)arguments[4];
 				typename RemoveCVR<T5>::Type* argument5 = (typename RemoveCVR<T5>::Type*)arguments[5];
 				typename RemoveCVR<T6>::Type* argument6 = (typename RemoveCVR<T6>::Type*)arguments[6];
 				typename RemoveCVR<T7>::Type* argument7 = (typename RemoveCVR<T7>::Type*)arguments[7];
 				typename RemoveCVR<T8>::Type* argument8 = (typename RemoveCVR<T8>::Type*)arguments[8];
 
				 (((C*)object)->*(*((FunctionPointer*)funcPtrPtr)))(*argument0,*argument1,*argument2,*argument3,*argument4,*argument5,*argument6,*argument7,*argument8);
				return 0;
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
				delete (FunctionPointer*)instance;
			}
		};
		template<typename C  ,typename T0,typename T1,typename T2,typename T3,typename T4,typename T5,typename T6,typename T7,typename T8>
		class ObjectTypeInformation<void(C::*)(T0,T1,T2,T3,T4,T5,T6,T7,T8)const>
		{
		public:
			static WString GetName()
			{
				return ObjectTypeInformation<void>::GetName()+L"("+ObjectTypeInformation<C>::GetName()+L"::*)"+L"("+ObjectTypeInformation<T0>::GetName()+L","+ObjectTypeInformation<T1>::GetName()+L","+ObjectTypeInformation<T2>::GetName()+L","+ObjectTypeInformation<T3>::GetName()+L","+ObjectTypeInformation<T4>::GetName()+L","+ObjectTypeInformation<T5>::GetName()+L","+ObjectTypeInformation<T6>::GetName()+L","+ObjectTypeInformation<T7>::GetName()+L","+ObjectTypeInformation<T8>::GetName()+L")"+L"const";
			}
			typedef ObjectTypeFunction<void(C::*)(T0,T1,T2,T3,T4,T5,T6,T7,T8)const> ObjectTypeImplementation;
		};
		template<typename C  ,typename T0,typename T1,typename T2,typename T3,typename T4,typename T5,typename T6,typename T7,typename T8>
		class ObjectTypeRegister<void(C::*)(T0,T1,T2,T3,T4,T5,T6,T7,T8)const>
		{
		public:
			static void Register()
			{
				ObjectTypeFunction<void(C::*)(T0,T1,T2,T3,T4,T5,T6,T7,T8)const>* type=new ObjectTypeFunction<void(C::*)(T0,T1,T2,T3,T4,T5,T6,T7,T8)const>;
				RegisterObjectType(ObjectTypeInformation<void(C::*)(T0,T1,T2,T3,T4,T5,T6,T7,T8)const>::GetName(), type);
				type->parameterTypes.Add(TypeOf<T0>());
 				type->parameterTypes.Add(TypeOf<T1>());
 				type->parameterTypes.Add(TypeOf<T2>());
 				type->parameterTypes.Add(TypeOf<T3>());
 				type->parameterTypes.Add(TypeOf<T4>());
 				type->parameterTypes.Add(TypeOf<T5>());
 				type->parameterTypes.Add(TypeOf<T6>());
 				type->parameterTypes.Add(TypeOf<T7>());
 				type->parameterTypes.Add(TypeOf<T8>());
 
				type->returnType=TypeOf<void>();
				type->thisType=TypeOf<C>();
				type->constructors.Add(new ObjectTypeFunction<void(C::*)(T0,T1,T2,T3,T4,T5,T6,T7,T8)const>::Constructor<void*(*)()>(&ObjectTypeFunction<void(C::*)(T0,T1,T2,T3,T4,T5,T6,T7,T8)const>::ctor));
			}
		};
  
/***********************************************************************
vl::objectmodel::ObjectTypeFunction<R(*)(T0,T1,T2,T3,T4,T5,T6,T7,T8,T9) >
***********************************************************************/
		template<typename R,typename T0,typename T1,typename T2,typename T3,typename T4,typename T5,typename T6,typename T7,typename T8,typename T9>
		class ObjectTypeFunction<R(*)(T0,T1,T2,T3,T4,T5,T6,T7,T8,T9) > : public SpecifiedObjectType<R(*)(T0,T1,T2,T3,T4,T5,T6,T7,T8,T9) >
		{
			friend class ObjectTypeRegister<R(*)(T0,T1,T2,T3,T4,T5,T6,T7,T8,T9) >;
			typedef R(*FunctionPointer)(T0,T1,T2,T3,T4,T5,T6,T7,T8,T9) ;
		protected:
			static void* ctor()
			{
				return new FunctionPointer(0);
			}
			collections::List<ObjectType*>		parameterTypes;
			ObjectType*							returnType;
			ObjectType*							thisType;
			collections::List<ObjectMember*>	constructors;
		public:
			ObjectTypeFunction()
			{
				returnType=0;
				thisType=0;
			}
			~ObjectTypeFunction()
			{
				for(int i=0;i<constructors.Count();i++)
				{
					delete constructors[i];
				}
			}
			ObjectCategory Category()const
			{
				return ObjectType::Function;
			}
			const ObjectType* ReturnType()const
			{
				return returnType;
			}
			const TypeList& ParameterTypes()const
			{
				return parameterTypes.Wrap();
			}
			const ObjectType* ThisType()const
			{
				return thisType;
			}
			bool IsConstFunction()const
			{
				return false;
			}
			void* Invoke(void* object, void* funcPtrPtr, void** arguments)const
			{
				typename RemoveCVR<T0>::Type* argument0 = (typename RemoveCVR<T0>::Type*)arguments[0];
 				typename RemoveCVR<T1>::Type* argument1 = (typename RemoveCVR<T1>::Type*)arguments[1];
 				typename RemoveCVR<T2>::Type* argument2 = (typename RemoveCVR<T2>::Type*)arguments[2];
 				typename RemoveCVR<T3>::Type* argument3 = (typename RemoveCVR<T3>::Type*)arguments[3];
 				typename RemoveCVR<T4>::Type* argument4 = (typename RemoveCVR<T4>::Type*)arguments[4];
 				typename RemoveCVR<T5>::Type* argument5 = (typename RemoveCVR<T5>::Type*)arguments[5];
 				typename RemoveCVR<T6>::Type* argument6 = (typename RemoveCVR<T6>::Type*)arguments[6];
 				typename RemoveCVR<T7>::Type* argument7 = (typename RemoveCVR<T7>::Type*)arguments[7];
 				typename RemoveCVR<T8>::Type* argument8 = (typename RemoveCVR<T8>::Type*)arguments[8];
 				typename RemoveCVR<T9>::Type* argument9 = (typename RemoveCVR<T9>::Type*)arguments[9];
 
				R result=(*((FunctionPointer*)funcPtrPtr))(*argument0,*argument1,*argument2,*argument3,*argument4,*argument5,*argument6,*argument7,*argument8,*argument9);
				return new R(result);
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
				delete (FunctionPointer*)instance;
			}
		};
		template<typename R,typename T0,typename T1,typename T2,typename T3,typename T4,typename T5,typename T6,typename T7,typename T8,typename T9>
		class ObjectTypeInformation<R(*)(T0,T1,T2,T3,T4,T5,T6,T7,T8,T9) >
		{
		public:
			static WString GetName()
			{
				return ObjectTypeInformation<R>::GetName()+L"(*)"+L"("+ObjectTypeInformation<T0>::GetName()+L","+ObjectTypeInformation<T1>::GetName()+L","+ObjectTypeInformation<T2>::GetName()+L","+ObjectTypeInformation<T3>::GetName()+L","+ObjectTypeInformation<T4>::GetName()+L","+ObjectTypeInformation<T5>::GetName()+L","+ObjectTypeInformation<T6>::GetName()+L","+ObjectTypeInformation<T7>::GetName()+L","+ObjectTypeInformation<T8>::GetName()+L","+ObjectTypeInformation<T9>::GetName()+L")" ;
			}
			typedef ObjectTypeFunction<R(*)(T0,T1,T2,T3,T4,T5,T6,T7,T8,T9) > ObjectTypeImplementation;
		};
		template<typename R,typename T0,typename T1,typename T2,typename T3,typename T4,typename T5,typename T6,typename T7,typename T8,typename T9>
		class ObjectTypeRegister<R(*)(T0,T1,T2,T3,T4,T5,T6,T7,T8,T9) >
		{
		public:
			static void Register()
			{
				ObjectTypeFunction<R(*)(T0,T1,T2,T3,T4,T5,T6,T7,T8,T9) >* type=new ObjectTypeFunction<R(*)(T0,T1,T2,T3,T4,T5,T6,T7,T8,T9) >;
				RegisterObjectType(ObjectTypeInformation<R(*)(T0,T1,T2,T3,T4,T5,T6,T7,T8,T9) >::GetName(), type);
				type->parameterTypes.Add(TypeOf<T0>());
 				type->parameterTypes.Add(TypeOf<T1>());
 				type->parameterTypes.Add(TypeOf<T2>());
 				type->parameterTypes.Add(TypeOf<T3>());
 				type->parameterTypes.Add(TypeOf<T4>());
 				type->parameterTypes.Add(TypeOf<T5>());
 				type->parameterTypes.Add(TypeOf<T6>());
 				type->parameterTypes.Add(TypeOf<T7>());
 				type->parameterTypes.Add(TypeOf<T8>());
 				type->parameterTypes.Add(TypeOf<T9>());
 
				type->returnType=TypeOf<R>();
				type->thisType=0;
				type->constructors.Add(new ObjectTypeFunction<R(*)(T0,T1,T2,T3,T4,T5,T6,T7,T8,T9) >::Constructor<void*(*)()>(&ObjectTypeFunction<R(*)(T0,T1,T2,T3,T4,T5,T6,T7,T8,T9) >::ctor));
			}
		};
 
/***********************************************************************
vl::objectmodel::ObjectTypeFunction<void(*)(T0,T1,T2,T3,T4,T5,T6,T7,T8,T9) >
***********************************************************************/
		template< typename T0,typename T1,typename T2,typename T3,typename T4,typename T5,typename T6,typename T7,typename T8,typename T9>
		class ObjectTypeFunction<void(*)(T0,T1,T2,T3,T4,T5,T6,T7,T8,T9) > : public SpecifiedObjectType<void(*)(T0,T1,T2,T3,T4,T5,T6,T7,T8,T9) >
		{
			friend class ObjectTypeRegister<void(*)(T0,T1,T2,T3,T4,T5,T6,T7,T8,T9) >;
			typedef void(*FunctionPointer)(T0,T1,T2,T3,T4,T5,T6,T7,T8,T9) ;
		protected:
			static void* ctor()
			{
				return new FunctionPointer(0);
			}
			collections::List<ObjectType*>		parameterTypes;
			ObjectType*							returnType;
			ObjectType*							thisType;
			collections::List<ObjectMember*>	constructors;
		public:
			ObjectTypeFunction()
			{
				returnType=0;
				thisType=0;
			}
			~ObjectTypeFunction()
			{
				for(int i=0;i<constructors.Count();i++)
				{
					delete constructors[i];
				}
			}
			ObjectCategory Category()const
			{
				return ObjectType::Function;
			}
			const ObjectType* ReturnType()const
			{
				return returnType;
			}
			const TypeList& ParameterTypes()const
			{
				return parameterTypes.Wrap();
			}
			const ObjectType* ThisType()const
			{
				return thisType;
			}
			bool IsConstFunction()const
			{
				return false;
			}
			void* Invoke(void* object, void* funcPtrPtr, void** arguments)const
			{
				typename RemoveCVR<T0>::Type* argument0 = (typename RemoveCVR<T0>::Type*)arguments[0];
 				typename RemoveCVR<T1>::Type* argument1 = (typename RemoveCVR<T1>::Type*)arguments[1];
 				typename RemoveCVR<T2>::Type* argument2 = (typename RemoveCVR<T2>::Type*)arguments[2];
 				typename RemoveCVR<T3>::Type* argument3 = (typename RemoveCVR<T3>::Type*)arguments[3];
 				typename RemoveCVR<T4>::Type* argument4 = (typename RemoveCVR<T4>::Type*)arguments[4];
 				typename RemoveCVR<T5>::Type* argument5 = (typename RemoveCVR<T5>::Type*)arguments[5];
 				typename RemoveCVR<T6>::Type* argument6 = (typename RemoveCVR<T6>::Type*)arguments[6];
 				typename RemoveCVR<T7>::Type* argument7 = (typename RemoveCVR<T7>::Type*)arguments[7];
 				typename RemoveCVR<T8>::Type* argument8 = (typename RemoveCVR<T8>::Type*)arguments[8];
 				typename RemoveCVR<T9>::Type* argument9 = (typename RemoveCVR<T9>::Type*)arguments[9];
 
				 (*((FunctionPointer*)funcPtrPtr))(*argument0,*argument1,*argument2,*argument3,*argument4,*argument5,*argument6,*argument7,*argument8,*argument9);
				return 0;
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
				delete (FunctionPointer*)instance;
			}
		};
		template< typename T0,typename T1,typename T2,typename T3,typename T4,typename T5,typename T6,typename T7,typename T8,typename T9>
		class ObjectTypeInformation<void(*)(T0,T1,T2,T3,T4,T5,T6,T7,T8,T9) >
		{
		public:
			static WString GetName()
			{
				return ObjectTypeInformation<void>::GetName()+L"(*)"+L"("+ObjectTypeInformation<T0>::GetName()+L","+ObjectTypeInformation<T1>::GetName()+L","+ObjectTypeInformation<T2>::GetName()+L","+ObjectTypeInformation<T3>::GetName()+L","+ObjectTypeInformation<T4>::GetName()+L","+ObjectTypeInformation<T5>::GetName()+L","+ObjectTypeInformation<T6>::GetName()+L","+ObjectTypeInformation<T7>::GetName()+L","+ObjectTypeInformation<T8>::GetName()+L","+ObjectTypeInformation<T9>::GetName()+L")" ;
			}
			typedef ObjectTypeFunction<void(*)(T0,T1,T2,T3,T4,T5,T6,T7,T8,T9) > ObjectTypeImplementation;
		};
		template< typename T0,typename T1,typename T2,typename T3,typename T4,typename T5,typename T6,typename T7,typename T8,typename T9>
		class ObjectTypeRegister<void(*)(T0,T1,T2,T3,T4,T5,T6,T7,T8,T9) >
		{
		public:
			static void Register()
			{
				ObjectTypeFunction<void(*)(T0,T1,T2,T3,T4,T5,T6,T7,T8,T9) >* type=new ObjectTypeFunction<void(*)(T0,T1,T2,T3,T4,T5,T6,T7,T8,T9) >;
				RegisterObjectType(ObjectTypeInformation<void(*)(T0,T1,T2,T3,T4,T5,T6,T7,T8,T9) >::GetName(), type);
				type->parameterTypes.Add(TypeOf<T0>());
 				type->parameterTypes.Add(TypeOf<T1>());
 				type->parameterTypes.Add(TypeOf<T2>());
 				type->parameterTypes.Add(TypeOf<T3>());
 				type->parameterTypes.Add(TypeOf<T4>());
 				type->parameterTypes.Add(TypeOf<T5>());
 				type->parameterTypes.Add(TypeOf<T6>());
 				type->parameterTypes.Add(TypeOf<T7>());
 				type->parameterTypes.Add(TypeOf<T8>());
 				type->parameterTypes.Add(TypeOf<T9>());
 
				type->returnType=TypeOf<void>();
				type->thisType=0;
				type->constructors.Add(new ObjectTypeFunction<void(*)(T0,T1,T2,T3,T4,T5,T6,T7,T8,T9) >::Constructor<void*(*)()>(&ObjectTypeFunction<void(*)(T0,T1,T2,T3,T4,T5,T6,T7,T8,T9) >::ctor));
			}
		};
 
/***********************************************************************
vl::objectmodel::ObjectTypeFunction<R(C::*)(T0,T1,T2,T3,T4,T5,T6,T7,T8,T9) >
***********************************************************************/
		template<typename C ,typename R,typename T0,typename T1,typename T2,typename T3,typename T4,typename T5,typename T6,typename T7,typename T8,typename T9>
		class ObjectTypeFunction<R(C::*)(T0,T1,T2,T3,T4,T5,T6,T7,T8,T9) > : public SpecifiedObjectType<R(C::*)(T0,T1,T2,T3,T4,T5,T6,T7,T8,T9) >
		{
			friend class ObjectTypeRegister<R(C::*)(T0,T1,T2,T3,T4,T5,T6,T7,T8,T9) >;
			typedef R(C::*FunctionPointer)(T0,T1,T2,T3,T4,T5,T6,T7,T8,T9) ;
		protected:
			static void* ctor()
			{
				return new FunctionPointer(0);
			}
			collections::List<ObjectType*>		parameterTypes;
			ObjectType*							returnType;
			ObjectType*							thisType;
			collections::List<ObjectMember*>	constructors;
		public:
			ObjectTypeFunction()
			{
				returnType=0;
				thisType=0;
			}
			~ObjectTypeFunction()
			{
				for(int i=0;i<constructors.Count();i++)
				{
					delete constructors[i];
				}
			}
			ObjectCategory Category()const
			{
				return ObjectType::Function;
			}
			const ObjectType* ReturnType()const
			{
				return returnType;
			}
			const TypeList& ParameterTypes()const
			{
				return parameterTypes.Wrap();
			}
			const ObjectType* ThisType()const
			{
				return thisType;
			}
			bool IsConstFunction()const
			{
				return false;
			}
			void* Invoke(void* object, void* funcPtrPtr, void** arguments)const
			{
				typename RemoveCVR<T0>::Type* argument0 = (typename RemoveCVR<T0>::Type*)arguments[0];
 				typename RemoveCVR<T1>::Type* argument1 = (typename RemoveCVR<T1>::Type*)arguments[1];
 				typename RemoveCVR<T2>::Type* argument2 = (typename RemoveCVR<T2>::Type*)arguments[2];
 				typename RemoveCVR<T3>::Type* argument3 = (typename RemoveCVR<T3>::Type*)arguments[3];
 				typename RemoveCVR<T4>::Type* argument4 = (typename RemoveCVR<T4>::Type*)arguments[4];
 				typename RemoveCVR<T5>::Type* argument5 = (typename RemoveCVR<T5>::Type*)arguments[5];
 				typename RemoveCVR<T6>::Type* argument6 = (typename RemoveCVR<T6>::Type*)arguments[6];
 				typename RemoveCVR<T7>::Type* argument7 = (typename RemoveCVR<T7>::Type*)arguments[7];
 				typename RemoveCVR<T8>::Type* argument8 = (typename RemoveCVR<T8>::Type*)arguments[8];
 				typename RemoveCVR<T9>::Type* argument9 = (typename RemoveCVR<T9>::Type*)arguments[9];
 
				R result=(((C*)object)->*(*((FunctionPointer*)funcPtrPtr)))(*argument0,*argument1,*argument2,*argument3,*argument4,*argument5,*argument6,*argument7,*argument8,*argument9);
				return new R(result);
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
				delete (FunctionPointer*)instance;
			}
		};
		template<typename C ,typename R,typename T0,typename T1,typename T2,typename T3,typename T4,typename T5,typename T6,typename T7,typename T8,typename T9>
		class ObjectTypeInformation<R(C::*)(T0,T1,T2,T3,T4,T5,T6,T7,T8,T9) >
		{
		public:
			static WString GetName()
			{
				return ObjectTypeInformation<R>::GetName()+L"("+ObjectTypeInformation<C>::GetName()+L"::*)"+L"("+ObjectTypeInformation<T0>::GetName()+L","+ObjectTypeInformation<T1>::GetName()+L","+ObjectTypeInformation<T2>::GetName()+L","+ObjectTypeInformation<T3>::GetName()+L","+ObjectTypeInformation<T4>::GetName()+L","+ObjectTypeInformation<T5>::GetName()+L","+ObjectTypeInformation<T6>::GetName()+L","+ObjectTypeInformation<T7>::GetName()+L","+ObjectTypeInformation<T8>::GetName()+L","+ObjectTypeInformation<T9>::GetName()+L")" ;
			}
			typedef ObjectTypeFunction<R(C::*)(T0,T1,T2,T3,T4,T5,T6,T7,T8,T9) > ObjectTypeImplementation;
		};
		template<typename C ,typename R,typename T0,typename T1,typename T2,typename T3,typename T4,typename T5,typename T6,typename T7,typename T8,typename T9>
		class ObjectTypeRegister<R(C::*)(T0,T1,T2,T3,T4,T5,T6,T7,T8,T9) >
		{
		public:
			static void Register()
			{
				ObjectTypeFunction<R(C::*)(T0,T1,T2,T3,T4,T5,T6,T7,T8,T9) >* type=new ObjectTypeFunction<R(C::*)(T0,T1,T2,T3,T4,T5,T6,T7,T8,T9) >;
				RegisterObjectType(ObjectTypeInformation<R(C::*)(T0,T1,T2,T3,T4,T5,T6,T7,T8,T9) >::GetName(), type);
				type->parameterTypes.Add(TypeOf<T0>());
 				type->parameterTypes.Add(TypeOf<T1>());
 				type->parameterTypes.Add(TypeOf<T2>());
 				type->parameterTypes.Add(TypeOf<T3>());
 				type->parameterTypes.Add(TypeOf<T4>());
 				type->parameterTypes.Add(TypeOf<T5>());
 				type->parameterTypes.Add(TypeOf<T6>());
 				type->parameterTypes.Add(TypeOf<T7>());
 				type->parameterTypes.Add(TypeOf<T8>());
 				type->parameterTypes.Add(TypeOf<T9>());
 
				type->returnType=TypeOf<R>();
				type->thisType=TypeOf<C>();
				type->constructors.Add(new ObjectTypeFunction<R(C::*)(T0,T1,T2,T3,T4,T5,T6,T7,T8,T9) >::Constructor<void*(*)()>(&ObjectTypeFunction<R(C::*)(T0,T1,T2,T3,T4,T5,T6,T7,T8,T9) >::ctor));
			}
		};
 
/***********************************************************************
vl::objectmodel::ObjectTypeFunction<void(C::*)(T0,T1,T2,T3,T4,T5,T6,T7,T8,T9) >
***********************************************************************/
		template<typename C  ,typename T0,typename T1,typename T2,typename T3,typename T4,typename T5,typename T6,typename T7,typename T8,typename T9>
		class ObjectTypeFunction<void(C::*)(T0,T1,T2,T3,T4,T5,T6,T7,T8,T9) > : public SpecifiedObjectType<void(C::*)(T0,T1,T2,T3,T4,T5,T6,T7,T8,T9) >
		{
			friend class ObjectTypeRegister<void(C::*)(T0,T1,T2,T3,T4,T5,T6,T7,T8,T9) >;
			typedef void(C::*FunctionPointer)(T0,T1,T2,T3,T4,T5,T6,T7,T8,T9) ;
		protected:
			static void* ctor()
			{
				return new FunctionPointer(0);
			}
			collections::List<ObjectType*>		parameterTypes;
			ObjectType*							returnType;
			ObjectType*							thisType;
			collections::List<ObjectMember*>	constructors;
		public:
			ObjectTypeFunction()
			{
				returnType=0;
				thisType=0;
			}
			~ObjectTypeFunction()
			{
				for(int i=0;i<constructors.Count();i++)
				{
					delete constructors[i];
				}
			}
			ObjectCategory Category()const
			{
				return ObjectType::Function;
			}
			const ObjectType* ReturnType()const
			{
				return returnType;
			}
			const TypeList& ParameterTypes()const
			{
				return parameterTypes.Wrap();
			}
			const ObjectType* ThisType()const
			{
				return thisType;
			}
			bool IsConstFunction()const
			{
				return false;
			}
			void* Invoke(void* object, void* funcPtrPtr, void** arguments)const
			{
				typename RemoveCVR<T0>::Type* argument0 = (typename RemoveCVR<T0>::Type*)arguments[0];
 				typename RemoveCVR<T1>::Type* argument1 = (typename RemoveCVR<T1>::Type*)arguments[1];
 				typename RemoveCVR<T2>::Type* argument2 = (typename RemoveCVR<T2>::Type*)arguments[2];
 				typename RemoveCVR<T3>::Type* argument3 = (typename RemoveCVR<T3>::Type*)arguments[3];
 				typename RemoveCVR<T4>::Type* argument4 = (typename RemoveCVR<T4>::Type*)arguments[4];
 				typename RemoveCVR<T5>::Type* argument5 = (typename RemoveCVR<T5>::Type*)arguments[5];
 				typename RemoveCVR<T6>::Type* argument6 = (typename RemoveCVR<T6>::Type*)arguments[6];
 				typename RemoveCVR<T7>::Type* argument7 = (typename RemoveCVR<T7>::Type*)arguments[7];
 				typename RemoveCVR<T8>::Type* argument8 = (typename RemoveCVR<T8>::Type*)arguments[8];
 				typename RemoveCVR<T9>::Type* argument9 = (typename RemoveCVR<T9>::Type*)arguments[9];
 
				 (((C*)object)->*(*((FunctionPointer*)funcPtrPtr)))(*argument0,*argument1,*argument2,*argument3,*argument4,*argument5,*argument6,*argument7,*argument8,*argument9);
				return 0;
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
				delete (FunctionPointer*)instance;
			}
		};
		template<typename C  ,typename T0,typename T1,typename T2,typename T3,typename T4,typename T5,typename T6,typename T7,typename T8,typename T9>
		class ObjectTypeInformation<void(C::*)(T0,T1,T2,T3,T4,T5,T6,T7,T8,T9) >
		{
		public:
			static WString GetName()
			{
				return ObjectTypeInformation<void>::GetName()+L"("+ObjectTypeInformation<C>::GetName()+L"::*)"+L"("+ObjectTypeInformation<T0>::GetName()+L","+ObjectTypeInformation<T1>::GetName()+L","+ObjectTypeInformation<T2>::GetName()+L","+ObjectTypeInformation<T3>::GetName()+L","+ObjectTypeInformation<T4>::GetName()+L","+ObjectTypeInformation<T5>::GetName()+L","+ObjectTypeInformation<T6>::GetName()+L","+ObjectTypeInformation<T7>::GetName()+L","+ObjectTypeInformation<T8>::GetName()+L","+ObjectTypeInformation<T9>::GetName()+L")" ;
			}
			typedef ObjectTypeFunction<void(C::*)(T0,T1,T2,T3,T4,T5,T6,T7,T8,T9) > ObjectTypeImplementation;
		};
		template<typename C  ,typename T0,typename T1,typename T2,typename T3,typename T4,typename T5,typename T6,typename T7,typename T8,typename T9>
		class ObjectTypeRegister<void(C::*)(T0,T1,T2,T3,T4,T5,T6,T7,T8,T9) >
		{
		public:
			static void Register()
			{
				ObjectTypeFunction<void(C::*)(T0,T1,T2,T3,T4,T5,T6,T7,T8,T9) >* type=new ObjectTypeFunction<void(C::*)(T0,T1,T2,T3,T4,T5,T6,T7,T8,T9) >;
				RegisterObjectType(ObjectTypeInformation<void(C::*)(T0,T1,T2,T3,T4,T5,T6,T7,T8,T9) >::GetName(), type);
				type->parameterTypes.Add(TypeOf<T0>());
 				type->parameterTypes.Add(TypeOf<T1>());
 				type->parameterTypes.Add(TypeOf<T2>());
 				type->parameterTypes.Add(TypeOf<T3>());
 				type->parameterTypes.Add(TypeOf<T4>());
 				type->parameterTypes.Add(TypeOf<T5>());
 				type->parameterTypes.Add(TypeOf<T6>());
 				type->parameterTypes.Add(TypeOf<T7>());
 				type->parameterTypes.Add(TypeOf<T8>());
 				type->parameterTypes.Add(TypeOf<T9>());
 
				type->returnType=TypeOf<void>();
				type->thisType=TypeOf<C>();
				type->constructors.Add(new ObjectTypeFunction<void(C::*)(T0,T1,T2,T3,T4,T5,T6,T7,T8,T9) >::Constructor<void*(*)()>(&ObjectTypeFunction<void(C::*)(T0,T1,T2,T3,T4,T5,T6,T7,T8,T9) >::ctor));
			}
		};
 
/***********************************************************************
vl::objectmodel::ObjectTypeFunction<R(C::*)(T0,T1,T2,T3,T4,T5,T6,T7,T8,T9)const>
***********************************************************************/
		template<typename C ,typename R,typename T0,typename T1,typename T2,typename T3,typename T4,typename T5,typename T6,typename T7,typename T8,typename T9>
		class ObjectTypeFunction<R(C::*)(T0,T1,T2,T3,T4,T5,T6,T7,T8,T9)const> : public SpecifiedObjectType<R(C::*)(T0,T1,T2,T3,T4,T5,T6,T7,T8,T9)const>
		{
			friend class ObjectTypeRegister<R(C::*)(T0,T1,T2,T3,T4,T5,T6,T7,T8,T9)const>;
			typedef R(C::*FunctionPointer)(T0,T1,T2,T3,T4,T5,T6,T7,T8,T9)const;
		protected:
			static void* ctor()
			{
				return new FunctionPointer(0);
			}
			collections::List<ObjectType*>		parameterTypes;
			ObjectType*							returnType;
			ObjectType*							thisType;
			collections::List<ObjectMember*>	constructors;
		public:
			ObjectTypeFunction()
			{
				returnType=0;
				thisType=0;
			}
			~ObjectTypeFunction()
			{
				for(int i=0;i<constructors.Count();i++)
				{
					delete constructors[i];
				}
			}
			ObjectCategory Category()const
			{
				return ObjectType::Function;
			}
			const ObjectType* ReturnType()const
			{
				return returnType;
			}
			const TypeList& ParameterTypes()const
			{
				return parameterTypes.Wrap();
			}
			const ObjectType* ThisType()const
			{
				return thisType;
			}
			bool IsConstFunction()const
			{
				return true;
			}
			void* Invoke(void* object, void* funcPtrPtr, void** arguments)const
			{
				typename RemoveCVR<T0>::Type* argument0 = (typename RemoveCVR<T0>::Type*)arguments[0];
 				typename RemoveCVR<T1>::Type* argument1 = (typename RemoveCVR<T1>::Type*)arguments[1];
 				typename RemoveCVR<T2>::Type* argument2 = (typename RemoveCVR<T2>::Type*)arguments[2];
 				typename RemoveCVR<T3>::Type* argument3 = (typename RemoveCVR<T3>::Type*)arguments[3];
 				typename RemoveCVR<T4>::Type* argument4 = (typename RemoveCVR<T4>::Type*)arguments[4];
 				typename RemoveCVR<T5>::Type* argument5 = (typename RemoveCVR<T5>::Type*)arguments[5];
 				typename RemoveCVR<T6>::Type* argument6 = (typename RemoveCVR<T6>::Type*)arguments[6];
 				typename RemoveCVR<T7>::Type* argument7 = (typename RemoveCVR<T7>::Type*)arguments[7];
 				typename RemoveCVR<T8>::Type* argument8 = (typename RemoveCVR<T8>::Type*)arguments[8];
 				typename RemoveCVR<T9>::Type* argument9 = (typename RemoveCVR<T9>::Type*)arguments[9];
 
				R result=(((C*)object)->*(*((FunctionPointer*)funcPtrPtr)))(*argument0,*argument1,*argument2,*argument3,*argument4,*argument5,*argument6,*argument7,*argument8,*argument9);
				return new R(result);
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
				delete (FunctionPointer*)instance;
			}
		};
		template<typename C ,typename R,typename T0,typename T1,typename T2,typename T3,typename T4,typename T5,typename T6,typename T7,typename T8,typename T9>
		class ObjectTypeInformation<R(C::*)(T0,T1,T2,T3,T4,T5,T6,T7,T8,T9)const>
		{
		public:
			static WString GetName()
			{
				return ObjectTypeInformation<R>::GetName()+L"("+ObjectTypeInformation<C>::GetName()+L"::*)"+L"("+ObjectTypeInformation<T0>::GetName()+L","+ObjectTypeInformation<T1>::GetName()+L","+ObjectTypeInformation<T2>::GetName()+L","+ObjectTypeInformation<T3>::GetName()+L","+ObjectTypeInformation<T4>::GetName()+L","+ObjectTypeInformation<T5>::GetName()+L","+ObjectTypeInformation<T6>::GetName()+L","+ObjectTypeInformation<T7>::GetName()+L","+ObjectTypeInformation<T8>::GetName()+L","+ObjectTypeInformation<T9>::GetName()+L")"+L"const";
			}
			typedef ObjectTypeFunction<R(C::*)(T0,T1,T2,T3,T4,T5,T6,T7,T8,T9)const> ObjectTypeImplementation;
		};
		template<typename C ,typename R,typename T0,typename T1,typename T2,typename T3,typename T4,typename T5,typename T6,typename T7,typename T8,typename T9>
		class ObjectTypeRegister<R(C::*)(T0,T1,T2,T3,T4,T5,T6,T7,T8,T9)const>
		{
		public:
			static void Register()
			{
				ObjectTypeFunction<R(C::*)(T0,T1,T2,T3,T4,T5,T6,T7,T8,T9)const>* type=new ObjectTypeFunction<R(C::*)(T0,T1,T2,T3,T4,T5,T6,T7,T8,T9)const>;
				RegisterObjectType(ObjectTypeInformation<R(C::*)(T0,T1,T2,T3,T4,T5,T6,T7,T8,T9)const>::GetName(), type);
				type->parameterTypes.Add(TypeOf<T0>());
 				type->parameterTypes.Add(TypeOf<T1>());
 				type->parameterTypes.Add(TypeOf<T2>());
 				type->parameterTypes.Add(TypeOf<T3>());
 				type->parameterTypes.Add(TypeOf<T4>());
 				type->parameterTypes.Add(TypeOf<T5>());
 				type->parameterTypes.Add(TypeOf<T6>());
 				type->parameterTypes.Add(TypeOf<T7>());
 				type->parameterTypes.Add(TypeOf<T8>());
 				type->parameterTypes.Add(TypeOf<T9>());
 
				type->returnType=TypeOf<R>();
				type->thisType=TypeOf<C>();
				type->constructors.Add(new ObjectTypeFunction<R(C::*)(T0,T1,T2,T3,T4,T5,T6,T7,T8,T9)const>::Constructor<void*(*)()>(&ObjectTypeFunction<R(C::*)(T0,T1,T2,T3,T4,T5,T6,T7,T8,T9)const>::ctor));
			}
		};
 
/***********************************************************************
vl::objectmodel::ObjectTypeFunction<void(C::*)(T0,T1,T2,T3,T4,T5,T6,T7,T8,T9)const>
***********************************************************************/
		template<typename C  ,typename T0,typename T1,typename T2,typename T3,typename T4,typename T5,typename T6,typename T7,typename T8,typename T9>
		class ObjectTypeFunction<void(C::*)(T0,T1,T2,T3,T4,T5,T6,T7,T8,T9)const> : public SpecifiedObjectType<void(C::*)(T0,T1,T2,T3,T4,T5,T6,T7,T8,T9)const>
		{
			friend class ObjectTypeRegister<void(C::*)(T0,T1,T2,T3,T4,T5,T6,T7,T8,T9)const>;
			typedef void(C::*FunctionPointer)(T0,T1,T2,T3,T4,T5,T6,T7,T8,T9)const;
		protected:
			static void* ctor()
			{
				return new FunctionPointer(0);
			}
			collections::List<ObjectType*>		parameterTypes;
			ObjectType*							returnType;
			ObjectType*							thisType;
			collections::List<ObjectMember*>	constructors;
		public:
			ObjectTypeFunction()
			{
				returnType=0;
				thisType=0;
			}
			~ObjectTypeFunction()
			{
				for(int i=0;i<constructors.Count();i++)
				{
					delete constructors[i];
				}
			}
			ObjectCategory Category()const
			{
				return ObjectType::Function;
			}
			const ObjectType* ReturnType()const
			{
				return returnType;
			}
			const TypeList& ParameterTypes()const
			{
				return parameterTypes.Wrap();
			}
			const ObjectType* ThisType()const
			{
				return thisType;
			}
			bool IsConstFunction()const
			{
				return true;
			}
			void* Invoke(void* object, void* funcPtrPtr, void** arguments)const
			{
				typename RemoveCVR<T0>::Type* argument0 = (typename RemoveCVR<T0>::Type*)arguments[0];
 				typename RemoveCVR<T1>::Type* argument1 = (typename RemoveCVR<T1>::Type*)arguments[1];
 				typename RemoveCVR<T2>::Type* argument2 = (typename RemoveCVR<T2>::Type*)arguments[2];
 				typename RemoveCVR<T3>::Type* argument3 = (typename RemoveCVR<T3>::Type*)arguments[3];
 				typename RemoveCVR<T4>::Type* argument4 = (typename RemoveCVR<T4>::Type*)arguments[4];
 				typename RemoveCVR<T5>::Type* argument5 = (typename RemoveCVR<T5>::Type*)arguments[5];
 				typename RemoveCVR<T6>::Type* argument6 = (typename RemoveCVR<T6>::Type*)arguments[6];
 				typename RemoveCVR<T7>::Type* argument7 = (typename RemoveCVR<T7>::Type*)arguments[7];
 				typename RemoveCVR<T8>::Type* argument8 = (typename RemoveCVR<T8>::Type*)arguments[8];
 				typename RemoveCVR<T9>::Type* argument9 = (typename RemoveCVR<T9>::Type*)arguments[9];
 
				 (((C*)object)->*(*((FunctionPointer*)funcPtrPtr)))(*argument0,*argument1,*argument2,*argument3,*argument4,*argument5,*argument6,*argument7,*argument8,*argument9);
				return 0;
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
				delete (FunctionPointer*)instance;
			}
		};
		template<typename C  ,typename T0,typename T1,typename T2,typename T3,typename T4,typename T5,typename T6,typename T7,typename T8,typename T9>
		class ObjectTypeInformation<void(C::*)(T0,T1,T2,T3,T4,T5,T6,T7,T8,T9)const>
		{
		public:
			static WString GetName()
			{
				return ObjectTypeInformation<void>::GetName()+L"("+ObjectTypeInformation<C>::GetName()+L"::*)"+L"("+ObjectTypeInformation<T0>::GetName()+L","+ObjectTypeInformation<T1>::GetName()+L","+ObjectTypeInformation<T2>::GetName()+L","+ObjectTypeInformation<T3>::GetName()+L","+ObjectTypeInformation<T4>::GetName()+L","+ObjectTypeInformation<T5>::GetName()+L","+ObjectTypeInformation<T6>::GetName()+L","+ObjectTypeInformation<T7>::GetName()+L","+ObjectTypeInformation<T8>::GetName()+L","+ObjectTypeInformation<T9>::GetName()+L")"+L"const";
			}
			typedef ObjectTypeFunction<void(C::*)(T0,T1,T2,T3,T4,T5,T6,T7,T8,T9)const> ObjectTypeImplementation;
		};
		template<typename C  ,typename T0,typename T1,typename T2,typename T3,typename T4,typename T5,typename T6,typename T7,typename T8,typename T9>
		class ObjectTypeRegister<void(C::*)(T0,T1,T2,T3,T4,T5,T6,T7,T8,T9)const>
		{
		public:
			static void Register()
			{
				ObjectTypeFunction<void(C::*)(T0,T1,T2,T3,T4,T5,T6,T7,T8,T9)const>* type=new ObjectTypeFunction<void(C::*)(T0,T1,T2,T3,T4,T5,T6,T7,T8,T9)const>;
				RegisterObjectType(ObjectTypeInformation<void(C::*)(T0,T1,T2,T3,T4,T5,T6,T7,T8,T9)const>::GetName(), type);
				type->parameterTypes.Add(TypeOf<T0>());
 				type->parameterTypes.Add(TypeOf<T1>());
 				type->parameterTypes.Add(TypeOf<T2>());
 				type->parameterTypes.Add(TypeOf<T3>());
 				type->parameterTypes.Add(TypeOf<T4>());
 				type->parameterTypes.Add(TypeOf<T5>());
 				type->parameterTypes.Add(TypeOf<T6>());
 				type->parameterTypes.Add(TypeOf<T7>());
 				type->parameterTypes.Add(TypeOf<T8>());
 				type->parameterTypes.Add(TypeOf<T9>());
 
				type->returnType=TypeOf<void>();
				type->thisType=TypeOf<C>();
				type->constructors.Add(new ObjectTypeFunction<void(C::*)(T0,T1,T2,T3,T4,T5,T6,T7,T8,T9)const>::Constructor<void*(*)()>(&ObjectTypeFunction<void(C::*)(T0,T1,T2,T3,T4,T5,T6,T7,T8,T9)const>::ctor));
			}
		};
 
	}
}
#endif