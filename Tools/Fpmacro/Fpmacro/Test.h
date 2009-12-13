/***********************************************************************
Vczh Library++ 3.0
Developer: 陈梓瀚(vczh)
Framework::Function

Classes:
	Func<function-type>									：函数对象

Functions:
	Curry :: (A->B) -> A -> B							：参数拆分
	Combine :: (A->B) -> (A->C) -> (B->C->D) -> (A->D)	：函数组合
	
本文件使用Vczh Functional Macro工具自动生成
***********************************************************************/
#ifndef VCZH_FUNCTION
#define VCZH_FUNCTION
#include "Basic.h"
#include "Pointer.h"
namespace vl
{
	template<typename T>
	class Func
	{
	};
 
/***********************************************************************
vl::Func<R()>
***********************************************************************/
	template<typename R >
	class Func<R()> : public Object
	{
	protected:
		class Invoker : public Object
		{
		public:
			virtual R Invoke()=0;
		};
		class StaticInvoker : public Invoker
		{
		protected:
			R(*function)();
		public:
			StaticInvoker(R(*_function)())
				:function(_function)
			{
			}
			virtual R Invoke()
			{
				return function();
			}
		};
		template<typename C>
		class MemberInvoker : public Invoker
		{
		protected:
			C*			sender;
			R(C::*function)();
		public:
			MemberInvoker(C* _sender, R(C::*_function)())
				:sender(_sender)
				,function(_function)
			{
			}
			virtual R Invoke()
			{
				return (sender->*function)();
			}
		};
		template<typename C>
		class PointerInvoker : public Invoker
		{
		protected:
			C*			function;
		public:
			PointerInvoker(C* _function)
				:function(_function)
			{
			}
			virtual R Invoke()
			{
				return function->operator()();
			}
		};
		template<typename C>
		class SmartPointerInvoker : public Invoker
		{
		protected:
			Ptr<C>		function;
		public:
			SmartPointerInvoker(const Ptr<C>& _function)
				:function(_function)
			{
			}
			virtual R Invoke()
			{
				return function->operator()();
			}
		};
		template<typename C>
		class ObjectInvoker : public Invoker
		{
		protected:
			C			function;
		public:
			ObjectInvoker(const C& _function)
				:function(_function)
			{
			}
			virtual R Invoke()
			{
				return function();
			}
		};
	protected:
		Ptr<Invoker>			invoker;
	public:
		typedef R FunctionType();
		typedef R ResultType;
		Func()
		{
		}
		Func(R(*function)())
		{
			invoker=new StaticInvoker(function);
		}
		template<typename C>
		Func(C* sender, R(C::*function)())
		{
			invoker=new MemberInvoker<C>(sender, function);
		}
		template<typename C>
		Func(C* function)
		{
			invoker=new PointerInvoker<C>(function);
		}
		template<typename C>
		Func(const Ptr<C>& function)
		{
			invoker=new SmartPointerInvoker<C>(function);
		}
		template<typename C>
		Func(const C& function)
		{
			invoker=new ObjectInvoker<C>(function);
		}
		R operator()()const
		{
			return invoker->Invoke();
		}
		bool operator==(const Func<R()>& function)const
		{
			return invoker==function.invoker;
		}
		bool operator!=(const Func<R()>& function)const
		{
			return invoker!=function.invoker;
		}
		bool operator<(const Func<R()>& function)const
		{
			return invoker<function.invoker;
		}
		bool operator<=(const Func<R()>& function)const
		{
			return invoker<=function.invoker;
		}
		bool operator>(const Func<R()>& function)const
		{
			return invoker>function.invoker;
		}
		bool operator>=(const Func<R()>& function)const
		{
			return invoker>=function.invoker;
		}
		operator bool()const
		{
			return invoker;
		}
	};
 
/***********************************************************************
vl::Func<void()>
***********************************************************************/
	template< >
	class Func<void()> : public Object
	{
	protected:
		class Invoker : public Object
		{
		public:
			virtual void Invoke()=0;
		};
		class StaticInvoker : public Invoker
		{
		protected:
			void(*function)();
		public:
			StaticInvoker(void(*_function)())
				:function(_function)
			{
			}
			virtual void Invoke()
			{
				  function();
			}
		};
		template<typename C>
		class MemberInvoker : public Invoker
		{
		protected:
			C*			sender;
			void(C::*function)();
		public:
			MemberInvoker(C* _sender, void(C::*_function)())
				:sender(_sender)
				,function(_function)
			{
			}
			virtual void Invoke()
			{
				  (sender->*function)();
			}
		};
		template<typename C>
		class PointerInvoker : public Invoker
		{
		protected:
			C*			function;
		public:
			PointerInvoker(C* _function)
				:function(_function)
			{
			}
			virtual void Invoke()
			{
				  function->operator()();
			}
		};
		template<typename C>
		class SmartPointerInvoker : public Invoker
		{
		protected:
			Ptr<C>		function;
		public:
			SmartPointerInvoker(const Ptr<C>& _function)
				:function(_function)
			{
			}
			virtual void Invoke()
			{
				  function->operator()();
			}
		};
		template<typename C>
		class ObjectInvoker : public Invoker
		{
		protected:
			C			function;
		public:
			ObjectInvoker(const C& _function)
				:function(_function)
			{
			}
			virtual void Invoke()
			{
				  function();
			}
		};
	protected:
		Ptr<Invoker>			invoker;
	public:
		typedef void FunctionType();
		typedef void ResultType;
		Func()
		{
		}
		Func(void(*function)())
		{
			invoker=new StaticInvoker(function);
		}
		template<typename C>
		Func(C* sender, void(C::*function)())
		{
			invoker=new MemberInvoker<C>(sender, function);
		}
		template<typename C>
		Func(C* function)
		{
			invoker=new PointerInvoker<C>(function);
		}
		template<typename C>
		Func(const Ptr<C>& function)
		{
			invoker=new SmartPointerInvoker<C>(function);
		}
		template<typename C>
		Func(const C& function)
		{
			invoker=new ObjectInvoker<C>(function);
		}
		void operator()()const
		{
			  invoker->Invoke();
		}
		bool operator==(const Func<void()>& function)const
		{
			return invoker==function.invoker;
		}
		bool operator!=(const Func<void()>& function)const
		{
			return invoker!=function.invoker;
		}
		bool operator<(const Func<void()>& function)const
		{
			return invoker<function.invoker;
		}
		bool operator<=(const Func<void()>& function)const
		{
			return invoker<=function.invoker;
		}
		bool operator>(const Func<void()>& function)const
		{
			return invoker>function.invoker;
		}
		bool operator>=(const Func<void()>& function)const
		{
			return invoker>=function.invoker;
		}
		operator bool()const
		{
			return invoker;
		}
	};
  
/***********************************************************************
vl::Func<R(T0)>
***********************************************************************/
	template<typename R,typename T0>
	class Func<R(T0)> : public Object
	{
	protected:
		class Invoker : public Object
		{
		public:
			virtual R Invoke(T0 p0)=0;
		};
		class StaticInvoker : public Invoker
		{
		protected:
			R(*function)(T0);
		public:
			StaticInvoker(R(*_function)(T0))
				:function(_function)
			{
			}
			virtual R Invoke(T0 p0)
			{
				return function(p0);
			}
		};
		template<typename C>
		class MemberInvoker : public Invoker
		{
		protected:
			C*			sender;
			R(C::*function)(T0);
		public:
			MemberInvoker(C* _sender, R(C::*_function)(T0))
				:sender(_sender)
				,function(_function)
			{
			}
			virtual R Invoke(T0 p0)
			{
				return (sender->*function)(p0);
			}
		};
		template<typename C>
		class PointerInvoker : public Invoker
		{
		protected:
			C*			function;
		public:
			PointerInvoker(C* _function)
				:function(_function)
			{
			}
			virtual R Invoke(T0 p0)
			{
				return function->operator()(p0);
			}
		};
		template<typename C>
		class SmartPointerInvoker : public Invoker
		{
		protected:
			Ptr<C>		function;
		public:
			SmartPointerInvoker(const Ptr<C>& _function)
				:function(_function)
			{
			}
			virtual R Invoke(T0 p0)
			{
				return function->operator()(p0);
			}
		};
		template<typename C>
		class ObjectInvoker : public Invoker
		{
		protected:
			C			function;
		public:
			ObjectInvoker(const C& _function)
				:function(_function)
			{
			}
			virtual R Invoke(T0 p0)
			{
				return function(p0);
			}
		};
	protected:
		Ptr<Invoker>			invoker;
	public:
		typedef R FunctionType(T0);
		typedef R ResultType;
		Func()
		{
		}
		Func(R(*function)(T0))
		{
			invoker=new StaticInvoker(function);
		}
		template<typename C>
		Func(C* sender, R(C::*function)(T0))
		{
			invoker=new MemberInvoker<C>(sender, function);
		}
		template<typename C>
		Func(C* function)
		{
			invoker=new PointerInvoker<C>(function);
		}
		template<typename C>
		Func(const Ptr<C>& function)
		{
			invoker=new SmartPointerInvoker<C>(function);
		}
		template<typename C>
		Func(const C& function)
		{
			invoker=new ObjectInvoker<C>(function);
		}
		R operator()(T0 p0)const
		{
			return invoker->Invoke(p0);
		}
		bool operator==(const Func<R(T0)>& function)const
		{
			return invoker==function.invoker;
		}
		bool operator!=(const Func<R(T0)>& function)const
		{
			return invoker!=function.invoker;
		}
		bool operator<(const Func<R(T0)>& function)const
		{
			return invoker<function.invoker;
		}
		bool operator<=(const Func<R(T0)>& function)const
		{
			return invoker<=function.invoker;
		}
		bool operator>(const Func<R(T0)>& function)const
		{
			return invoker>function.invoker;
		}
		bool operator>=(const Func<R(T0)>& function)const
		{
			return invoker>=function.invoker;
		}
		operator bool()const
		{
			return invoker;
		}
	};
 
/***********************************************************************
vl::Func<void(T0)>
***********************************************************************/
	template< typename T0>
	class Func<void(T0)> : public Object
	{
	protected:
		class Invoker : public Object
		{
		public:
			virtual void Invoke(T0 p0)=0;
		};
		class StaticInvoker : public Invoker
		{
		protected:
			void(*function)(T0);
		public:
			StaticInvoker(void(*_function)(T0))
				:function(_function)
			{
			}
			virtual void Invoke(T0 p0)
			{
				  function(p0);
			}
		};
		template<typename C>
		class MemberInvoker : public Invoker
		{
		protected:
			C*			sender;
			void(C::*function)(T0);
		public:
			MemberInvoker(C* _sender, void(C::*_function)(T0))
				:sender(_sender)
				,function(_function)
			{
			}
			virtual void Invoke(T0 p0)
			{
				  (sender->*function)(p0);
			}
		};
		template<typename C>
		class PointerInvoker : public Invoker
		{
		protected:
			C*			function;
		public:
			PointerInvoker(C* _function)
				:function(_function)
			{
			}
			virtual void Invoke(T0 p0)
			{
				  function->operator()(p0);
			}
		};
		template<typename C>
		class SmartPointerInvoker : public Invoker
		{
		protected:
			Ptr<C>		function;
		public:
			SmartPointerInvoker(const Ptr<C>& _function)
				:function(_function)
			{
			}
			virtual void Invoke(T0 p0)
			{
				  function->operator()(p0);
			}
		};
		template<typename C>
		class ObjectInvoker : public Invoker
		{
		protected:
			C			function;
		public:
			ObjectInvoker(const C& _function)
				:function(_function)
			{
			}
			virtual void Invoke(T0 p0)
			{
				  function(p0);
			}
		};
	protected:
		Ptr<Invoker>			invoker;
	public:
		typedef void FunctionType(T0);
		typedef void ResultType;
		Func()
		{
		}
		Func(void(*function)(T0))
		{
			invoker=new StaticInvoker(function);
		}
		template<typename C>
		Func(C* sender, void(C::*function)(T0))
		{
			invoker=new MemberInvoker<C>(sender, function);
		}
		template<typename C>
		Func(C* function)
		{
			invoker=new PointerInvoker<C>(function);
		}
		template<typename C>
		Func(const Ptr<C>& function)
		{
			invoker=new SmartPointerInvoker<C>(function);
		}
		template<typename C>
		Func(const C& function)
		{
			invoker=new ObjectInvoker<C>(function);
		}
		void operator()(T0 p0)const
		{
			  invoker->Invoke(p0);
		}
		bool operator==(const Func<void(T0)>& function)const
		{
			return invoker==function.invoker;
		}
		bool operator!=(const Func<void(T0)>& function)const
		{
			return invoker!=function.invoker;
		}
		bool operator<(const Func<void(T0)>& function)const
		{
			return invoker<function.invoker;
		}
		bool operator<=(const Func<void(T0)>& function)const
		{
			return invoker<=function.invoker;
		}
		bool operator>(const Func<void(T0)>& function)const
		{
			return invoker>function.invoker;
		}
		bool operator>=(const Func<void(T0)>& function)const
		{
			return invoker>=function.invoker;
		}
		operator bool()const
		{
			return invoker;
		}
	};
  
/***********************************************************************
vl::Func<R(T0,T1)>
***********************************************************************/
	template<typename R,typename T0,typename T1>
	class Func<R(T0,T1)> : public Object
	{
	protected:
		class Invoker : public Object
		{
		public:
			virtual R Invoke(T0 p0,T1 p1)=0;
		};
		class StaticInvoker : public Invoker
		{
		protected:
			R(*function)(T0,T1);
		public:
			StaticInvoker(R(*_function)(T0,T1))
				:function(_function)
			{
			}
			virtual R Invoke(T0 p0,T1 p1)
			{
				return function(p0,p1);
			}
		};
		template<typename C>
		class MemberInvoker : public Invoker
		{
		protected:
			C*			sender;
			R(C::*function)(T0,T1);
		public:
			MemberInvoker(C* _sender, R(C::*_function)(T0,T1))
				:sender(_sender)
				,function(_function)
			{
			}
			virtual R Invoke(T0 p0,T1 p1)
			{
				return (sender->*function)(p0,p1);
			}
		};
		template<typename C>
		class PointerInvoker : public Invoker
		{
		protected:
			C*			function;
		public:
			PointerInvoker(C* _function)
				:function(_function)
			{
			}
			virtual R Invoke(T0 p0,T1 p1)
			{
				return function->operator()(p0,p1);
			}
		};
		template<typename C>
		class SmartPointerInvoker : public Invoker
		{
		protected:
			Ptr<C>		function;
		public:
			SmartPointerInvoker(const Ptr<C>& _function)
				:function(_function)
			{
			}
			virtual R Invoke(T0 p0,T1 p1)
			{
				return function->operator()(p0,p1);
			}
		};
		template<typename C>
		class ObjectInvoker : public Invoker
		{
		protected:
			C			function;
		public:
			ObjectInvoker(const C& _function)
				:function(_function)
			{
			}
			virtual R Invoke(T0 p0,T1 p1)
			{
				return function(p0,p1);
			}
		};
	protected:
		Ptr<Invoker>			invoker;
	public:
		typedef R FunctionType(T0,T1);
		typedef R ResultType;
		Func()
		{
		}
		Func(R(*function)(T0,T1))
		{
			invoker=new StaticInvoker(function);
		}
		template<typename C>
		Func(C* sender, R(C::*function)(T0,T1))
		{
			invoker=new MemberInvoker<C>(sender, function);
		}
		template<typename C>
		Func(C* function)
		{
			invoker=new PointerInvoker<C>(function);
		}
		template<typename C>
		Func(const Ptr<C>& function)
		{
			invoker=new SmartPointerInvoker<C>(function);
		}
		template<typename C>
		Func(const C& function)
		{
			invoker=new ObjectInvoker<C>(function);
		}
		R operator()(T0 p0,T1 p1)const
		{
			return invoker->Invoke(p0,p1);
		}
		bool operator==(const Func<R(T0,T1)>& function)const
		{
			return invoker==function.invoker;
		}
		bool operator!=(const Func<R(T0,T1)>& function)const
		{
			return invoker!=function.invoker;
		}
		bool operator<(const Func<R(T0,T1)>& function)const
		{
			return invoker<function.invoker;
		}
		bool operator<=(const Func<R(T0,T1)>& function)const
		{
			return invoker<=function.invoker;
		}
		bool operator>(const Func<R(T0,T1)>& function)const
		{
			return invoker>function.invoker;
		}
		bool operator>=(const Func<R(T0,T1)>& function)const
		{
			return invoker>=function.invoker;
		}
		operator bool()const
		{
			return invoker;
		}
	};
 
/***********************************************************************
vl::Func<void(T0,T1)>
***********************************************************************/
	template< typename T0,typename T1>
	class Func<void(T0,T1)> : public Object
	{
	protected:
		class Invoker : public Object
		{
		public:
			virtual void Invoke(T0 p0,T1 p1)=0;
		};
		class StaticInvoker : public Invoker
		{
		protected:
			void(*function)(T0,T1);
		public:
			StaticInvoker(void(*_function)(T0,T1))
				:function(_function)
			{
			}
			virtual void Invoke(T0 p0,T1 p1)
			{
				  function(p0,p1);
			}
		};
		template<typename C>
		class MemberInvoker : public Invoker
		{
		protected:
			C*			sender;
			void(C::*function)(T0,T1);
		public:
			MemberInvoker(C* _sender, void(C::*_function)(T0,T1))
				:sender(_sender)
				,function(_function)
			{
			}
			virtual void Invoke(T0 p0,T1 p1)
			{
				  (sender->*function)(p0,p1);
			}
		};
		template<typename C>
		class PointerInvoker : public Invoker
		{
		protected:
			C*			function;
		public:
			PointerInvoker(C* _function)
				:function(_function)
			{
			}
			virtual void Invoke(T0 p0,T1 p1)
			{
				  function->operator()(p0,p1);
			}
		};
		template<typename C>
		class SmartPointerInvoker : public Invoker
		{
		protected:
			Ptr<C>		function;
		public:
			SmartPointerInvoker(const Ptr<C>& _function)
				:function(_function)
			{
			}
			virtual void Invoke(T0 p0,T1 p1)
			{
				  function->operator()(p0,p1);
			}
		};
		template<typename C>
		class ObjectInvoker : public Invoker
		{
		protected:
			C			function;
		public:
			ObjectInvoker(const C& _function)
				:function(_function)
			{
			}
			virtual void Invoke(T0 p0,T1 p1)
			{
				  function(p0,p1);
			}
		};
	protected:
		Ptr<Invoker>			invoker;
	public:
		typedef void FunctionType(T0,T1);
		typedef void ResultType;
		Func()
		{
		}
		Func(void(*function)(T0,T1))
		{
			invoker=new StaticInvoker(function);
		}
		template<typename C>
		Func(C* sender, void(C::*function)(T0,T1))
		{
			invoker=new MemberInvoker<C>(sender, function);
		}
		template<typename C>
		Func(C* function)
		{
			invoker=new PointerInvoker<C>(function);
		}
		template<typename C>
		Func(const Ptr<C>& function)
		{
			invoker=new SmartPointerInvoker<C>(function);
		}
		template<typename C>
		Func(const C& function)
		{
			invoker=new ObjectInvoker<C>(function);
		}
		void operator()(T0 p0,T1 p1)const
		{
			  invoker->Invoke(p0,p1);
		}
		bool operator==(const Func<void(T0,T1)>& function)const
		{
			return invoker==function.invoker;
		}
		bool operator!=(const Func<void(T0,T1)>& function)const
		{
			return invoker!=function.invoker;
		}
		bool operator<(const Func<void(T0,T1)>& function)const
		{
			return invoker<function.invoker;
		}
		bool operator<=(const Func<void(T0,T1)>& function)const
		{
			return invoker<=function.invoker;
		}
		bool operator>(const Func<void(T0,T1)>& function)const
		{
			return invoker>function.invoker;
		}
		bool operator>=(const Func<void(T0,T1)>& function)const
		{
			return invoker>=function.invoker;
		}
		operator bool()const
		{
			return invoker;
		}
	};
  
/***********************************************************************
vl::Func<R(T0,T1,T2)>
***********************************************************************/
	template<typename R,typename T0,typename T1,typename T2>
	class Func<R(T0,T1,T2)> : public Object
	{
	protected:
		class Invoker : public Object
		{
		public:
			virtual R Invoke(T0 p0,T1 p1,T2 p2)=0;
		};
		class StaticInvoker : public Invoker
		{
		protected:
			R(*function)(T0,T1,T2);
		public:
			StaticInvoker(R(*_function)(T0,T1,T2))
				:function(_function)
			{
			}
			virtual R Invoke(T0 p0,T1 p1,T2 p2)
			{
				return function(p0,p1,p2);
			}
		};
		template<typename C>
		class MemberInvoker : public Invoker
		{
		protected:
			C*			sender;
			R(C::*function)(T0,T1,T2);
		public:
			MemberInvoker(C* _sender, R(C::*_function)(T0,T1,T2))
				:sender(_sender)
				,function(_function)
			{
			}
			virtual R Invoke(T0 p0,T1 p1,T2 p2)
			{
				return (sender->*function)(p0,p1,p2);
			}
		};
		template<typename C>
		class PointerInvoker : public Invoker
		{
		protected:
			C*			function;
		public:
			PointerInvoker(C* _function)
				:function(_function)
			{
			}
			virtual R Invoke(T0 p0,T1 p1,T2 p2)
			{
				return function->operator()(p0,p1,p2);
			}
		};
		template<typename C>
		class SmartPointerInvoker : public Invoker
		{
		protected:
			Ptr<C>		function;
		public:
			SmartPointerInvoker(const Ptr<C>& _function)
				:function(_function)
			{
			}
			virtual R Invoke(T0 p0,T1 p1,T2 p2)
			{
				return function->operator()(p0,p1,p2);
			}
		};
		template<typename C>
		class ObjectInvoker : public Invoker
		{
		protected:
			C			function;
		public:
			ObjectInvoker(const C& _function)
				:function(_function)
			{
			}
			virtual R Invoke(T0 p0,T1 p1,T2 p2)
			{
				return function(p0,p1,p2);
			}
		};
	protected:
		Ptr<Invoker>			invoker;
	public:
		typedef R FunctionType(T0,T1,T2);
		typedef R ResultType;
		Func()
		{
		}
		Func(R(*function)(T0,T1,T2))
		{
			invoker=new StaticInvoker(function);
		}
		template<typename C>
		Func(C* sender, R(C::*function)(T0,T1,T2))
		{
			invoker=new MemberInvoker<C>(sender, function);
		}
		template<typename C>
		Func(C* function)
		{
			invoker=new PointerInvoker<C>(function);
		}
		template<typename C>
		Func(const Ptr<C>& function)
		{
			invoker=new SmartPointerInvoker<C>(function);
		}
		template<typename C>
		Func(const C& function)
		{
			invoker=new ObjectInvoker<C>(function);
		}
		R operator()(T0 p0,T1 p1,T2 p2)const
		{
			return invoker->Invoke(p0,p1,p2);
		}
		bool operator==(const Func<R(T0,T1,T2)>& function)const
		{
			return invoker==function.invoker;
		}
		bool operator!=(const Func<R(T0,T1,T2)>& function)const
		{
			return invoker!=function.invoker;
		}
		bool operator<(const Func<R(T0,T1,T2)>& function)const
		{
			return invoker<function.invoker;
		}
		bool operator<=(const Func<R(T0,T1,T2)>& function)const
		{
			return invoker<=function.invoker;
		}
		bool operator>(const Func<R(T0,T1,T2)>& function)const
		{
			return invoker>function.invoker;
		}
		bool operator>=(const Func<R(T0,T1,T2)>& function)const
		{
			return invoker>=function.invoker;
		}
		operator bool()const
		{
			return invoker;
		}
	};
 
/***********************************************************************
vl::Func<void(T0,T1,T2)>
***********************************************************************/
	template< typename T0,typename T1,typename T2>
	class Func<void(T0,T1,T2)> : public Object
	{
	protected:
		class Invoker : public Object
		{
		public:
			virtual void Invoke(T0 p0,T1 p1,T2 p2)=0;
		};
		class StaticInvoker : public Invoker
		{
		protected:
			void(*function)(T0,T1,T2);
		public:
			StaticInvoker(void(*_function)(T0,T1,T2))
				:function(_function)
			{
			}
			virtual void Invoke(T0 p0,T1 p1,T2 p2)
			{
				  function(p0,p1,p2);
			}
		};
		template<typename C>
		class MemberInvoker : public Invoker
		{
		protected:
			C*			sender;
			void(C::*function)(T0,T1,T2);
		public:
			MemberInvoker(C* _sender, void(C::*_function)(T0,T1,T2))
				:sender(_sender)
				,function(_function)
			{
			}
			virtual void Invoke(T0 p0,T1 p1,T2 p2)
			{
				  (sender->*function)(p0,p1,p2);
			}
		};
		template<typename C>
		class PointerInvoker : public Invoker
		{
		protected:
			C*			function;
		public:
			PointerInvoker(C* _function)
				:function(_function)
			{
			}
			virtual void Invoke(T0 p0,T1 p1,T2 p2)
			{
				  function->operator()(p0,p1,p2);
			}
		};
		template<typename C>
		class SmartPointerInvoker : public Invoker
		{
		protected:
			Ptr<C>		function;
		public:
			SmartPointerInvoker(const Ptr<C>& _function)
				:function(_function)
			{
			}
			virtual void Invoke(T0 p0,T1 p1,T2 p2)
			{
				  function->operator()(p0,p1,p2);
			}
		};
		template<typename C>
		class ObjectInvoker : public Invoker
		{
		protected:
			C			function;
		public:
			ObjectInvoker(const C& _function)
				:function(_function)
			{
			}
			virtual void Invoke(T0 p0,T1 p1,T2 p2)
			{
				  function(p0,p1,p2);
			}
		};
	protected:
		Ptr<Invoker>			invoker;
	public:
		typedef void FunctionType(T0,T1,T2);
		typedef void ResultType;
		Func()
		{
		}
		Func(void(*function)(T0,T1,T2))
		{
			invoker=new StaticInvoker(function);
		}
		template<typename C>
		Func(C* sender, void(C::*function)(T0,T1,T2))
		{
			invoker=new MemberInvoker<C>(sender, function);
		}
		template<typename C>
		Func(C* function)
		{
			invoker=new PointerInvoker<C>(function);
		}
		template<typename C>
		Func(const Ptr<C>& function)
		{
			invoker=new SmartPointerInvoker<C>(function);
		}
		template<typename C>
		Func(const C& function)
		{
			invoker=new ObjectInvoker<C>(function);
		}
		void operator()(T0 p0,T1 p1,T2 p2)const
		{
			  invoker->Invoke(p0,p1,p2);
		}
		bool operator==(const Func<void(T0,T1,T2)>& function)const
		{
			return invoker==function.invoker;
		}
		bool operator!=(const Func<void(T0,T1,T2)>& function)const
		{
			return invoker!=function.invoker;
		}
		bool operator<(const Func<void(T0,T1,T2)>& function)const
		{
			return invoker<function.invoker;
		}
		bool operator<=(const Func<void(T0,T1,T2)>& function)const
		{
			return invoker<=function.invoker;
		}
		bool operator>(const Func<void(T0,T1,T2)>& function)const
		{
			return invoker>function.invoker;
		}
		bool operator>=(const Func<void(T0,T1,T2)>& function)const
		{
			return invoker>=function.invoker;
		}
		operator bool()const
		{
			return invoker;
		}
	};
  
/***********************************************************************
vl::Func<R(T0,T1,T2,T3)>
***********************************************************************/
	template<typename R,typename T0,typename T1,typename T2,typename T3>
	class Func<R(T0,T1,T2,T3)> : public Object
	{
	protected:
		class Invoker : public Object
		{
		public:
			virtual R Invoke(T0 p0,T1 p1,T2 p2,T3 p3)=0;
		};
		class StaticInvoker : public Invoker
		{
		protected:
			R(*function)(T0,T1,T2,T3);
		public:
			StaticInvoker(R(*_function)(T0,T1,T2,T3))
				:function(_function)
			{
			}
			virtual R Invoke(T0 p0,T1 p1,T2 p2,T3 p3)
			{
				return function(p0,p1,p2,p3);
			}
		};
		template<typename C>
		class MemberInvoker : public Invoker
		{
		protected:
			C*			sender;
			R(C::*function)(T0,T1,T2,T3);
		public:
			MemberInvoker(C* _sender, R(C::*_function)(T0,T1,T2,T3))
				:sender(_sender)
				,function(_function)
			{
			}
			virtual R Invoke(T0 p0,T1 p1,T2 p2,T3 p3)
			{
				return (sender->*function)(p0,p1,p2,p3);
			}
		};
		template<typename C>
		class PointerInvoker : public Invoker
		{
		protected:
			C*			function;
		public:
			PointerInvoker(C* _function)
				:function(_function)
			{
			}
			virtual R Invoke(T0 p0,T1 p1,T2 p2,T3 p3)
			{
				return function->operator()(p0,p1,p2,p3);
			}
		};
		template<typename C>
		class SmartPointerInvoker : public Invoker
		{
		protected:
			Ptr<C>		function;
		public:
			SmartPointerInvoker(const Ptr<C>& _function)
				:function(_function)
			{
			}
			virtual R Invoke(T0 p0,T1 p1,T2 p2,T3 p3)
			{
				return function->operator()(p0,p1,p2,p3);
			}
		};
		template<typename C>
		class ObjectInvoker : public Invoker
		{
		protected:
			C			function;
		public:
			ObjectInvoker(const C& _function)
				:function(_function)
			{
			}
			virtual R Invoke(T0 p0,T1 p1,T2 p2,T3 p3)
			{
				return function(p0,p1,p2,p3);
			}
		};
	protected:
		Ptr<Invoker>			invoker;
	public:
		typedef R FunctionType(T0,T1,T2,T3);
		typedef R ResultType;
		Func()
		{
		}
		Func(R(*function)(T0,T1,T2,T3))
		{
			invoker=new StaticInvoker(function);
		}
		template<typename C>
		Func(C* sender, R(C::*function)(T0,T1,T2,T3))
		{
			invoker=new MemberInvoker<C>(sender, function);
		}
		template<typename C>
		Func(C* function)
		{
			invoker=new PointerInvoker<C>(function);
		}
		template<typename C>
		Func(const Ptr<C>& function)
		{
			invoker=new SmartPointerInvoker<C>(function);
		}
		template<typename C>
		Func(const C& function)
		{
			invoker=new ObjectInvoker<C>(function);
		}
		R operator()(T0 p0,T1 p1,T2 p2,T3 p3)const
		{
			return invoker->Invoke(p0,p1,p2,p3);
		}
		bool operator==(const Func<R(T0,T1,T2,T3)>& function)const
		{
			return invoker==function.invoker;
		}
		bool operator!=(const Func<R(T0,T1,T2,T3)>& function)const
		{
			return invoker!=function.invoker;
		}
		bool operator<(const Func<R(T0,T1,T2,T3)>& function)const
		{
			return invoker<function.invoker;
		}
		bool operator<=(const Func<R(T0,T1,T2,T3)>& function)const
		{
			return invoker<=function.invoker;
		}
		bool operator>(const Func<R(T0,T1,T2,T3)>& function)const
		{
			return invoker>function.invoker;
		}
		bool operator>=(const Func<R(T0,T1,T2,T3)>& function)const
		{
			return invoker>=function.invoker;
		}
		operator bool()const
		{
			return invoker;
		}
	};
 
/***********************************************************************
vl::Func<void(T0,T1,T2,T3)>
***********************************************************************/
	template< typename T0,typename T1,typename T2,typename T3>
	class Func<void(T0,T1,T2,T3)> : public Object
	{
	protected:
		class Invoker : public Object
		{
		public:
			virtual void Invoke(T0 p0,T1 p1,T2 p2,T3 p3)=0;
		};
		class StaticInvoker : public Invoker
		{
		protected:
			void(*function)(T0,T1,T2,T3);
		public:
			StaticInvoker(void(*_function)(T0,T1,T2,T3))
				:function(_function)
			{
			}
			virtual void Invoke(T0 p0,T1 p1,T2 p2,T3 p3)
			{
				  function(p0,p1,p2,p3);
			}
		};
		template<typename C>
		class MemberInvoker : public Invoker
		{
		protected:
			C*			sender;
			void(C::*function)(T0,T1,T2,T3);
		public:
			MemberInvoker(C* _sender, void(C::*_function)(T0,T1,T2,T3))
				:sender(_sender)
				,function(_function)
			{
			}
			virtual void Invoke(T0 p0,T1 p1,T2 p2,T3 p3)
			{
				  (sender->*function)(p0,p1,p2,p3);
			}
		};
		template<typename C>
		class PointerInvoker : public Invoker
		{
		protected:
			C*			function;
		public:
			PointerInvoker(C* _function)
				:function(_function)
			{
			}
			virtual void Invoke(T0 p0,T1 p1,T2 p2,T3 p3)
			{
				  function->operator()(p0,p1,p2,p3);
			}
		};
		template<typename C>
		class SmartPointerInvoker : public Invoker
		{
		protected:
			Ptr<C>		function;
		public:
			SmartPointerInvoker(const Ptr<C>& _function)
				:function(_function)
			{
			}
			virtual void Invoke(T0 p0,T1 p1,T2 p2,T3 p3)
			{
				  function->operator()(p0,p1,p2,p3);
			}
		};
		template<typename C>
		class ObjectInvoker : public Invoker
		{
		protected:
			C			function;
		public:
			ObjectInvoker(const C& _function)
				:function(_function)
			{
			}
			virtual void Invoke(T0 p0,T1 p1,T2 p2,T3 p3)
			{
				  function(p0,p1,p2,p3);
			}
		};
	protected:
		Ptr<Invoker>			invoker;
	public:
		typedef void FunctionType(T0,T1,T2,T3);
		typedef void ResultType;
		Func()
		{
		}
		Func(void(*function)(T0,T1,T2,T3))
		{
			invoker=new StaticInvoker(function);
		}
		template<typename C>
		Func(C* sender, void(C::*function)(T0,T1,T2,T3))
		{
			invoker=new MemberInvoker<C>(sender, function);
		}
		template<typename C>
		Func(C* function)
		{
			invoker=new PointerInvoker<C>(function);
		}
		template<typename C>
		Func(const Ptr<C>& function)
		{
			invoker=new SmartPointerInvoker<C>(function);
		}
		template<typename C>
		Func(const C& function)
		{
			invoker=new ObjectInvoker<C>(function);
		}
		void operator()(T0 p0,T1 p1,T2 p2,T3 p3)const
		{
			  invoker->Invoke(p0,p1,p2,p3);
		}
		bool operator==(const Func<void(T0,T1,T2,T3)>& function)const
		{
			return invoker==function.invoker;
		}
		bool operator!=(const Func<void(T0,T1,T2,T3)>& function)const
		{
			return invoker!=function.invoker;
		}
		bool operator<(const Func<void(T0,T1,T2,T3)>& function)const
		{
			return invoker<function.invoker;
		}
		bool operator<=(const Func<void(T0,T1,T2,T3)>& function)const
		{
			return invoker<=function.invoker;
		}
		bool operator>(const Func<void(T0,T1,T2,T3)>& function)const
		{
			return invoker>function.invoker;
		}
		bool operator>=(const Func<void(T0,T1,T2,T3)>& function)const
		{
			return invoker>=function.invoker;
		}
		operator bool()const
		{
			return invoker;
		}
	};
  
/***********************************************************************
vl::Func<R(T0,T1,T2,T3,T4)>
***********************************************************************/
	template<typename R,typename T0,typename T1,typename T2,typename T3,typename T4>
	class Func<R(T0,T1,T2,T3,T4)> : public Object
	{
	protected:
		class Invoker : public Object
		{
		public:
			virtual R Invoke(T0 p0,T1 p1,T2 p2,T3 p3,T4 p4)=0;
		};
		class StaticInvoker : public Invoker
		{
		protected:
			R(*function)(T0,T1,T2,T3,T4);
		public:
			StaticInvoker(R(*_function)(T0,T1,T2,T3,T4))
				:function(_function)
			{
			}
			virtual R Invoke(T0 p0,T1 p1,T2 p2,T3 p3,T4 p4)
			{
				return function(p0,p1,p2,p3,p4);
			}
		};
		template<typename C>
		class MemberInvoker : public Invoker
		{
		protected:
			C*			sender;
			R(C::*function)(T0,T1,T2,T3,T4);
		public:
			MemberInvoker(C* _sender, R(C::*_function)(T0,T1,T2,T3,T4))
				:sender(_sender)
				,function(_function)
			{
			}
			virtual R Invoke(T0 p0,T1 p1,T2 p2,T3 p3,T4 p4)
			{
				return (sender->*function)(p0,p1,p2,p3,p4);
			}
		};
		template<typename C>
		class PointerInvoker : public Invoker
		{
		protected:
			C*			function;
		public:
			PointerInvoker(C* _function)
				:function(_function)
			{
			}
			virtual R Invoke(T0 p0,T1 p1,T2 p2,T3 p3,T4 p4)
			{
				return function->operator()(p0,p1,p2,p3,p4);
			}
		};
		template<typename C>
		class SmartPointerInvoker : public Invoker
		{
		protected:
			Ptr<C>		function;
		public:
			SmartPointerInvoker(const Ptr<C>& _function)
				:function(_function)
			{
			}
			virtual R Invoke(T0 p0,T1 p1,T2 p2,T3 p3,T4 p4)
			{
				return function->operator()(p0,p1,p2,p3,p4);
			}
		};
		template<typename C>
		class ObjectInvoker : public Invoker
		{
		protected:
			C			function;
		public:
			ObjectInvoker(const C& _function)
				:function(_function)
			{
			}
			virtual R Invoke(T0 p0,T1 p1,T2 p2,T3 p3,T4 p4)
			{
				return function(p0,p1,p2,p3,p4);
			}
		};
	protected:
		Ptr<Invoker>			invoker;
	public:
		typedef R FunctionType(T0,T1,T2,T3,T4);
		typedef R ResultType;
		Func()
		{
		}
		Func(R(*function)(T0,T1,T2,T3,T4))
		{
			invoker=new StaticInvoker(function);
		}
		template<typename C>
		Func(C* sender, R(C::*function)(T0,T1,T2,T3,T4))
		{
			invoker=new MemberInvoker<C>(sender, function);
		}
		template<typename C>
		Func(C* function)
		{
			invoker=new PointerInvoker<C>(function);
		}
		template<typename C>
		Func(const Ptr<C>& function)
		{
			invoker=new SmartPointerInvoker<C>(function);
		}
		template<typename C>
		Func(const C& function)
		{
			invoker=new ObjectInvoker<C>(function);
		}
		R operator()(T0 p0,T1 p1,T2 p2,T3 p3,T4 p4)const
		{
			return invoker->Invoke(p0,p1,p2,p3,p4);
		}
		bool operator==(const Func<R(T0,T1,T2,T3,T4)>& function)const
		{
			return invoker==function.invoker;
		}
		bool operator!=(const Func<R(T0,T1,T2,T3,T4)>& function)const
		{
			return invoker!=function.invoker;
		}
		bool operator<(const Func<R(T0,T1,T2,T3,T4)>& function)const
		{
			return invoker<function.invoker;
		}
		bool operator<=(const Func<R(T0,T1,T2,T3,T4)>& function)const
		{
			return invoker<=function.invoker;
		}
		bool operator>(const Func<R(T0,T1,T2,T3,T4)>& function)const
		{
			return invoker>function.invoker;
		}
		bool operator>=(const Func<R(T0,T1,T2,T3,T4)>& function)const
		{
			return invoker>=function.invoker;
		}
		operator bool()const
		{
			return invoker;
		}
	};
 
/***********************************************************************
vl::Func<void(T0,T1,T2,T3,T4)>
***********************************************************************/
	template< typename T0,typename T1,typename T2,typename T3,typename T4>
	class Func<void(T0,T1,T2,T3,T4)> : public Object
	{
	protected:
		class Invoker : public Object
		{
		public:
			virtual void Invoke(T0 p0,T1 p1,T2 p2,T3 p3,T4 p4)=0;
		};
		class StaticInvoker : public Invoker
		{
		protected:
			void(*function)(T0,T1,T2,T3,T4);
		public:
			StaticInvoker(void(*_function)(T0,T1,T2,T3,T4))
				:function(_function)
			{
			}
			virtual void Invoke(T0 p0,T1 p1,T2 p2,T3 p3,T4 p4)
			{
				  function(p0,p1,p2,p3,p4);
			}
		};
		template<typename C>
		class MemberInvoker : public Invoker
		{
		protected:
			C*			sender;
			void(C::*function)(T0,T1,T2,T3,T4);
		public:
			MemberInvoker(C* _sender, void(C::*_function)(T0,T1,T2,T3,T4))
				:sender(_sender)
				,function(_function)
			{
			}
			virtual void Invoke(T0 p0,T1 p1,T2 p2,T3 p3,T4 p4)
			{
				  (sender->*function)(p0,p1,p2,p3,p4);
			}
		};
		template<typename C>
		class PointerInvoker : public Invoker
		{
		protected:
			C*			function;
		public:
			PointerInvoker(C* _function)
				:function(_function)
			{
			}
			virtual void Invoke(T0 p0,T1 p1,T2 p2,T3 p3,T4 p4)
			{
				  function->operator()(p0,p1,p2,p3,p4);
			}
		};
		template<typename C>
		class SmartPointerInvoker : public Invoker
		{
		protected:
			Ptr<C>		function;
		public:
			SmartPointerInvoker(const Ptr<C>& _function)
				:function(_function)
			{
			}
			virtual void Invoke(T0 p0,T1 p1,T2 p2,T3 p3,T4 p4)
			{
				  function->operator()(p0,p1,p2,p3,p4);
			}
		};
		template<typename C>
		class ObjectInvoker : public Invoker
		{
		protected:
			C			function;
		public:
			ObjectInvoker(const C& _function)
				:function(_function)
			{
			}
			virtual void Invoke(T0 p0,T1 p1,T2 p2,T3 p3,T4 p4)
			{
				  function(p0,p1,p2,p3,p4);
			}
		};
	protected:
		Ptr<Invoker>			invoker;
	public:
		typedef void FunctionType(T0,T1,T2,T3,T4);
		typedef void ResultType;
		Func()
		{
		}
		Func(void(*function)(T0,T1,T2,T3,T4))
		{
			invoker=new StaticInvoker(function);
		}
		template<typename C>
		Func(C* sender, void(C::*function)(T0,T1,T2,T3,T4))
		{
			invoker=new MemberInvoker<C>(sender, function);
		}
		template<typename C>
		Func(C* function)
		{
			invoker=new PointerInvoker<C>(function);
		}
		template<typename C>
		Func(const Ptr<C>& function)
		{
			invoker=new SmartPointerInvoker<C>(function);
		}
		template<typename C>
		Func(const C& function)
		{
			invoker=new ObjectInvoker<C>(function);
		}
		void operator()(T0 p0,T1 p1,T2 p2,T3 p3,T4 p4)const
		{
			  invoker->Invoke(p0,p1,p2,p3,p4);
		}
		bool operator==(const Func<void(T0,T1,T2,T3,T4)>& function)const
		{
			return invoker==function.invoker;
		}
		bool operator!=(const Func<void(T0,T1,T2,T3,T4)>& function)const
		{
			return invoker!=function.invoker;
		}
		bool operator<(const Func<void(T0,T1,T2,T3,T4)>& function)const
		{
			return invoker<function.invoker;
		}
		bool operator<=(const Func<void(T0,T1,T2,T3,T4)>& function)const
		{
			return invoker<=function.invoker;
		}
		bool operator>(const Func<void(T0,T1,T2,T3,T4)>& function)const
		{
			return invoker>function.invoker;
		}
		bool operator>=(const Func<void(T0,T1,T2,T3,T4)>& function)const
		{
			return invoker>=function.invoker;
		}
		operator bool()const
		{
			return invoker;
		}
	};
  
/***********************************************************************
vl::Func<R(T0,T1,T2,T3,T4,T5)>
***********************************************************************/
	template<typename R,typename T0,typename T1,typename T2,typename T3,typename T4,typename T5>
	class Func<R(T0,T1,T2,T3,T4,T5)> : public Object
	{
	protected:
		class Invoker : public Object
		{
		public:
			virtual R Invoke(T0 p0,T1 p1,T2 p2,T3 p3,T4 p4,T5 p5)=0;
		};
		class StaticInvoker : public Invoker
		{
		protected:
			R(*function)(T0,T1,T2,T3,T4,T5);
		public:
			StaticInvoker(R(*_function)(T0,T1,T2,T3,T4,T5))
				:function(_function)
			{
			}
			virtual R Invoke(T0 p0,T1 p1,T2 p2,T3 p3,T4 p4,T5 p5)
			{
				return function(p0,p1,p2,p3,p4,p5);
			}
		};
		template<typename C>
		class MemberInvoker : public Invoker
		{
		protected:
			C*			sender;
			R(C::*function)(T0,T1,T2,T3,T4,T5);
		public:
			MemberInvoker(C* _sender, R(C::*_function)(T0,T1,T2,T3,T4,T5))
				:sender(_sender)
				,function(_function)
			{
			}
			virtual R Invoke(T0 p0,T1 p1,T2 p2,T3 p3,T4 p4,T5 p5)
			{
				return (sender->*function)(p0,p1,p2,p3,p4,p5);
			}
		};
		template<typename C>
		class PointerInvoker : public Invoker
		{
		protected:
			C*			function;
		public:
			PointerInvoker(C* _function)
				:function(_function)
			{
			}
			virtual R Invoke(T0 p0,T1 p1,T2 p2,T3 p3,T4 p4,T5 p5)
			{
				return function->operator()(p0,p1,p2,p3,p4,p5);
			}
		};
		template<typename C>
		class SmartPointerInvoker : public Invoker
		{
		protected:
			Ptr<C>		function;
		public:
			SmartPointerInvoker(const Ptr<C>& _function)
				:function(_function)
			{
			}
			virtual R Invoke(T0 p0,T1 p1,T2 p2,T3 p3,T4 p4,T5 p5)
			{
				return function->operator()(p0,p1,p2,p3,p4,p5);
			}
		};
		template<typename C>
		class ObjectInvoker : public Invoker
		{
		protected:
			C			function;
		public:
			ObjectInvoker(const C& _function)
				:function(_function)
			{
			}
			virtual R Invoke(T0 p0,T1 p1,T2 p2,T3 p3,T4 p4,T5 p5)
			{
				return function(p0,p1,p2,p3,p4,p5);
			}
		};
	protected:
		Ptr<Invoker>			invoker;
	public:
		typedef R FunctionType(T0,T1,T2,T3,T4,T5);
		typedef R ResultType;
		Func()
		{
		}
		Func(R(*function)(T0,T1,T2,T3,T4,T5))
		{
			invoker=new StaticInvoker(function);
		}
		template<typename C>
		Func(C* sender, R(C::*function)(T0,T1,T2,T3,T4,T5))
		{
			invoker=new MemberInvoker<C>(sender, function);
		}
		template<typename C>
		Func(C* function)
		{
			invoker=new PointerInvoker<C>(function);
		}
		template<typename C>
		Func(const Ptr<C>& function)
		{
			invoker=new SmartPointerInvoker<C>(function);
		}
		template<typename C>
		Func(const C& function)
		{
			invoker=new ObjectInvoker<C>(function);
		}
		R operator()(T0 p0,T1 p1,T2 p2,T3 p3,T4 p4,T5 p5)const
		{
			return invoker->Invoke(p0,p1,p2,p3,p4,p5);
		}
		bool operator==(const Func<R(T0,T1,T2,T3,T4,T5)>& function)const
		{
			return invoker==function.invoker;
		}
		bool operator!=(const Func<R(T0,T1,T2,T3,T4,T5)>& function)const
		{
			return invoker!=function.invoker;
		}
		bool operator<(const Func<R(T0,T1,T2,T3,T4,T5)>& function)const
		{
			return invoker<function.invoker;
		}
		bool operator<=(const Func<R(T0,T1,T2,T3,T4,T5)>& function)const
		{
			return invoker<=function.invoker;
		}
		bool operator>(const Func<R(T0,T1,T2,T3,T4,T5)>& function)const
		{
			return invoker>function.invoker;
		}
		bool operator>=(const Func<R(T0,T1,T2,T3,T4,T5)>& function)const
		{
			return invoker>=function.invoker;
		}
		operator bool()const
		{
			return invoker;
		}
	};
 
/***********************************************************************
vl::Func<void(T0,T1,T2,T3,T4,T5)>
***********************************************************************/
	template< typename T0,typename T1,typename T2,typename T3,typename T4,typename T5>
	class Func<void(T0,T1,T2,T3,T4,T5)> : public Object
	{
	protected:
		class Invoker : public Object
		{
		public:
			virtual void Invoke(T0 p0,T1 p1,T2 p2,T3 p3,T4 p4,T5 p5)=0;
		};
		class StaticInvoker : public Invoker
		{
		protected:
			void(*function)(T0,T1,T2,T3,T4,T5);
		public:
			StaticInvoker(void(*_function)(T0,T1,T2,T3,T4,T5))
				:function(_function)
			{
			}
			virtual void Invoke(T0 p0,T1 p1,T2 p2,T3 p3,T4 p4,T5 p5)
			{
				  function(p0,p1,p2,p3,p4,p5);
			}
		};
		template<typename C>
		class MemberInvoker : public Invoker
		{
		protected:
			C*			sender;
			void(C::*function)(T0,T1,T2,T3,T4,T5);
		public:
			MemberInvoker(C* _sender, void(C::*_function)(T0,T1,T2,T3,T4,T5))
				:sender(_sender)
				,function(_function)
			{
			}
			virtual void Invoke(T0 p0,T1 p1,T2 p2,T3 p3,T4 p4,T5 p5)
			{
				  (sender->*function)(p0,p1,p2,p3,p4,p5);
			}
		};
		template<typename C>
		class PointerInvoker : public Invoker
		{
		protected:
			C*			function;
		public:
			PointerInvoker(C* _function)
				:function(_function)
			{
			}
			virtual void Invoke(T0 p0,T1 p1,T2 p2,T3 p3,T4 p4,T5 p5)
			{
				  function->operator()(p0,p1,p2,p3,p4,p5);
			}
		};
		template<typename C>
		class SmartPointerInvoker : public Invoker
		{
		protected:
			Ptr<C>		function;
		public:
			SmartPointerInvoker(const Ptr<C>& _function)
				:function(_function)
			{
			}
			virtual void Invoke(T0 p0,T1 p1,T2 p2,T3 p3,T4 p4,T5 p5)
			{
				  function->operator()(p0,p1,p2,p3,p4,p5);
			}
		};
		template<typename C>
		class ObjectInvoker : public Invoker
		{
		protected:
			C			function;
		public:
			ObjectInvoker(const C& _function)
				:function(_function)
			{
			}
			virtual void Invoke(T0 p0,T1 p1,T2 p2,T3 p3,T4 p4,T5 p5)
			{
				  function(p0,p1,p2,p3,p4,p5);
			}
		};
	protected:
		Ptr<Invoker>			invoker;
	public:
		typedef void FunctionType(T0,T1,T2,T3,T4,T5);
		typedef void ResultType;
		Func()
		{
		}
		Func(void(*function)(T0,T1,T2,T3,T4,T5))
		{
			invoker=new StaticInvoker(function);
		}
		template<typename C>
		Func(C* sender, void(C::*function)(T0,T1,T2,T3,T4,T5))
		{
			invoker=new MemberInvoker<C>(sender, function);
		}
		template<typename C>
		Func(C* function)
		{
			invoker=new PointerInvoker<C>(function);
		}
		template<typename C>
		Func(const Ptr<C>& function)
		{
			invoker=new SmartPointerInvoker<C>(function);
		}
		template<typename C>
		Func(const C& function)
		{
			invoker=new ObjectInvoker<C>(function);
		}
		void operator()(T0 p0,T1 p1,T2 p2,T3 p3,T4 p4,T5 p5)const
		{
			  invoker->Invoke(p0,p1,p2,p3,p4,p5);
		}
		bool operator==(const Func<void(T0,T1,T2,T3,T4,T5)>& function)const
		{
			return invoker==function.invoker;
		}
		bool operator!=(const Func<void(T0,T1,T2,T3,T4,T5)>& function)const
		{
			return invoker!=function.invoker;
		}
		bool operator<(const Func<void(T0,T1,T2,T3,T4,T5)>& function)const
		{
			return invoker<function.invoker;
		}
		bool operator<=(const Func<void(T0,T1,T2,T3,T4,T5)>& function)const
		{
			return invoker<=function.invoker;
		}
		bool operator>(const Func<void(T0,T1,T2,T3,T4,T5)>& function)const
		{
			return invoker>function.invoker;
		}
		bool operator>=(const Func<void(T0,T1,T2,T3,T4,T5)>& function)const
		{
			return invoker>=function.invoker;
		}
		operator bool()const
		{
			return invoker;
		}
	};
  
/***********************************************************************
vl::Func<R(T0,T1,T2,T3,T4,T5,T6)>
***********************************************************************/
	template<typename R,typename T0,typename T1,typename T2,typename T3,typename T4,typename T5,typename T6>
	class Func<R(T0,T1,T2,T3,T4,T5,T6)> : public Object
	{
	protected:
		class Invoker : public Object
		{
		public:
			virtual R Invoke(T0 p0,T1 p1,T2 p2,T3 p3,T4 p4,T5 p5,T6 p6)=0;
		};
		class StaticInvoker : public Invoker
		{
		protected:
			R(*function)(T0,T1,T2,T3,T4,T5,T6);
		public:
			StaticInvoker(R(*_function)(T0,T1,T2,T3,T4,T5,T6))
				:function(_function)
			{
			}
			virtual R Invoke(T0 p0,T1 p1,T2 p2,T3 p3,T4 p4,T5 p5,T6 p6)
			{
				return function(p0,p1,p2,p3,p4,p5,p6);
			}
		};
		template<typename C>
		class MemberInvoker : public Invoker
		{
		protected:
			C*			sender;
			R(C::*function)(T0,T1,T2,T3,T4,T5,T6);
		public:
			MemberInvoker(C* _sender, R(C::*_function)(T0,T1,T2,T3,T4,T5,T6))
				:sender(_sender)
				,function(_function)
			{
			}
			virtual R Invoke(T0 p0,T1 p1,T2 p2,T3 p3,T4 p4,T5 p5,T6 p6)
			{
				return (sender->*function)(p0,p1,p2,p3,p4,p5,p6);
			}
		};
		template<typename C>
		class PointerInvoker : public Invoker
		{
		protected:
			C*			function;
		public:
			PointerInvoker(C* _function)
				:function(_function)
			{
			}
			virtual R Invoke(T0 p0,T1 p1,T2 p2,T3 p3,T4 p4,T5 p5,T6 p6)
			{
				return function->operator()(p0,p1,p2,p3,p4,p5,p6);
			}
		};
		template<typename C>
		class SmartPointerInvoker : public Invoker
		{
		protected:
			Ptr<C>		function;
		public:
			SmartPointerInvoker(const Ptr<C>& _function)
				:function(_function)
			{
			}
			virtual R Invoke(T0 p0,T1 p1,T2 p2,T3 p3,T4 p4,T5 p5,T6 p6)
			{
				return function->operator()(p0,p1,p2,p3,p4,p5,p6);
			}
		};
		template<typename C>
		class ObjectInvoker : public Invoker
		{
		protected:
			C			function;
		public:
			ObjectInvoker(const C& _function)
				:function(_function)
			{
			}
			virtual R Invoke(T0 p0,T1 p1,T2 p2,T3 p3,T4 p4,T5 p5,T6 p6)
			{
				return function(p0,p1,p2,p3,p4,p5,p6);
			}
		};
	protected:
		Ptr<Invoker>			invoker;
	public:
		typedef R FunctionType(T0,T1,T2,T3,T4,T5,T6);
		typedef R ResultType;
		Func()
		{
		}
		Func(R(*function)(T0,T1,T2,T3,T4,T5,T6))
		{
			invoker=new StaticInvoker(function);
		}
		template<typename C>
		Func(C* sender, R(C::*function)(T0,T1,T2,T3,T4,T5,T6))
		{
			invoker=new MemberInvoker<C>(sender, function);
		}
		template<typename C>
		Func(C* function)
		{
			invoker=new PointerInvoker<C>(function);
		}
		template<typename C>
		Func(const Ptr<C>& function)
		{
			invoker=new SmartPointerInvoker<C>(function);
		}
		template<typename C>
		Func(const C& function)
		{
			invoker=new ObjectInvoker<C>(function);
		}
		R operator()(T0 p0,T1 p1,T2 p2,T3 p3,T4 p4,T5 p5,T6 p6)const
		{
			return invoker->Invoke(p0,p1,p2,p3,p4,p5,p6);
		}
		bool operator==(const Func<R(T0,T1,T2,T3,T4,T5,T6)>& function)const
		{
			return invoker==function.invoker;
		}
		bool operator!=(const Func<R(T0,T1,T2,T3,T4,T5,T6)>& function)const
		{
			return invoker!=function.invoker;
		}
		bool operator<(const Func<R(T0,T1,T2,T3,T4,T5,T6)>& function)const
		{
			return invoker<function.invoker;
		}
		bool operator<=(const Func<R(T0,T1,T2,T3,T4,T5,T6)>& function)const
		{
			return invoker<=function.invoker;
		}
		bool operator>(const Func<R(T0,T1,T2,T3,T4,T5,T6)>& function)const
		{
			return invoker>function.invoker;
		}
		bool operator>=(const Func<R(T0,T1,T2,T3,T4,T5,T6)>& function)const
		{
			return invoker>=function.invoker;
		}
		operator bool()const
		{
			return invoker;
		}
	};
 
/***********************************************************************
vl::Func<void(T0,T1,T2,T3,T4,T5,T6)>
***********************************************************************/
	template< typename T0,typename T1,typename T2,typename T3,typename T4,typename T5,typename T6>
	class Func<void(T0,T1,T2,T3,T4,T5,T6)> : public Object
	{
	protected:
		class Invoker : public Object
		{
		public:
			virtual void Invoke(T0 p0,T1 p1,T2 p2,T3 p3,T4 p4,T5 p5,T6 p6)=0;
		};
		class StaticInvoker : public Invoker
		{
		protected:
			void(*function)(T0,T1,T2,T3,T4,T5,T6);
		public:
			StaticInvoker(void(*_function)(T0,T1,T2,T3,T4,T5,T6))
				:function(_function)
			{
			}
			virtual void Invoke(T0 p0,T1 p1,T2 p2,T3 p3,T4 p4,T5 p5,T6 p6)
			{
				  function(p0,p1,p2,p3,p4,p5,p6);
			}
		};
		template<typename C>
		class MemberInvoker : public Invoker
		{
		protected:
			C*			sender;
			void(C::*function)(T0,T1,T2,T3,T4,T5,T6);
		public:
			MemberInvoker(C* _sender, void(C::*_function)(T0,T1,T2,T3,T4,T5,T6))
				:sender(_sender)
				,function(_function)
			{
			}
			virtual void Invoke(T0 p0,T1 p1,T2 p2,T3 p3,T4 p4,T5 p5,T6 p6)
			{
				  (sender->*function)(p0,p1,p2,p3,p4,p5,p6);
			}
		};
		template<typename C>
		class PointerInvoker : public Invoker
		{
		protected:
			C*			function;
		public:
			PointerInvoker(C* _function)
				:function(_function)
			{
			}
			virtual void Invoke(T0 p0,T1 p1,T2 p2,T3 p3,T4 p4,T5 p5,T6 p6)
			{
				  function->operator()(p0,p1,p2,p3,p4,p5,p6);
			}
		};
		template<typename C>
		class SmartPointerInvoker : public Invoker
		{
		protected:
			Ptr<C>		function;
		public:
			SmartPointerInvoker(const Ptr<C>& _function)
				:function(_function)
			{
			}
			virtual void Invoke(T0 p0,T1 p1,T2 p2,T3 p3,T4 p4,T5 p5,T6 p6)
			{
				  function->operator()(p0,p1,p2,p3,p4,p5,p6);
			}
		};
		template<typename C>
		class ObjectInvoker : public Invoker
		{
		protected:
			C			function;
		public:
			ObjectInvoker(const C& _function)
				:function(_function)
			{
			}
			virtual void Invoke(T0 p0,T1 p1,T2 p2,T3 p3,T4 p4,T5 p5,T6 p6)
			{
				  function(p0,p1,p2,p3,p4,p5,p6);
			}
		};
	protected:
		Ptr<Invoker>			invoker;
	public:
		typedef void FunctionType(T0,T1,T2,T3,T4,T5,T6);
		typedef void ResultType;
		Func()
		{
		}
		Func(void(*function)(T0,T1,T2,T3,T4,T5,T6))
		{
			invoker=new StaticInvoker(function);
		}
		template<typename C>
		Func(C* sender, void(C::*function)(T0,T1,T2,T3,T4,T5,T6))
		{
			invoker=new MemberInvoker<C>(sender, function);
		}
		template<typename C>
		Func(C* function)
		{
			invoker=new PointerInvoker<C>(function);
		}
		template<typename C>
		Func(const Ptr<C>& function)
		{
			invoker=new SmartPointerInvoker<C>(function);
		}
		template<typename C>
		Func(const C& function)
		{
			invoker=new ObjectInvoker<C>(function);
		}
		void operator()(T0 p0,T1 p1,T2 p2,T3 p3,T4 p4,T5 p5,T6 p6)const
		{
			  invoker->Invoke(p0,p1,p2,p3,p4,p5,p6);
		}
		bool operator==(const Func<void(T0,T1,T2,T3,T4,T5,T6)>& function)const
		{
			return invoker==function.invoker;
		}
		bool operator!=(const Func<void(T0,T1,T2,T3,T4,T5,T6)>& function)const
		{
			return invoker!=function.invoker;
		}
		bool operator<(const Func<void(T0,T1,T2,T3,T4,T5,T6)>& function)const
		{
			return invoker<function.invoker;
		}
		bool operator<=(const Func<void(T0,T1,T2,T3,T4,T5,T6)>& function)const
		{
			return invoker<=function.invoker;
		}
		bool operator>(const Func<void(T0,T1,T2,T3,T4,T5,T6)>& function)const
		{
			return invoker>function.invoker;
		}
		bool operator>=(const Func<void(T0,T1,T2,T3,T4,T5,T6)>& function)const
		{
			return invoker>=function.invoker;
		}
		operator bool()const
		{
			return invoker;
		}
	};
  
/***********************************************************************
vl::Func<R(T0,T1,T2,T3,T4,T5,T6,T7)>
***********************************************************************/
	template<typename R,typename T0,typename T1,typename T2,typename T3,typename T4,typename T5,typename T6,typename T7>
	class Func<R(T0,T1,T2,T3,T4,T5,T6,T7)> : public Object
	{
	protected:
		class Invoker : public Object
		{
		public:
			virtual R Invoke(T0 p0,T1 p1,T2 p2,T3 p3,T4 p4,T5 p5,T6 p6,T7 p7)=0;
		};
		class StaticInvoker : public Invoker
		{
		protected:
			R(*function)(T0,T1,T2,T3,T4,T5,T6,T7);
		public:
			StaticInvoker(R(*_function)(T0,T1,T2,T3,T4,T5,T6,T7))
				:function(_function)
			{
			}
			virtual R Invoke(T0 p0,T1 p1,T2 p2,T3 p3,T4 p4,T5 p5,T6 p6,T7 p7)
			{
				return function(p0,p1,p2,p3,p4,p5,p6,p7);
			}
		};
		template<typename C>
		class MemberInvoker : public Invoker
		{
		protected:
			C*			sender;
			R(C::*function)(T0,T1,T2,T3,T4,T5,T6,T7);
		public:
			MemberInvoker(C* _sender, R(C::*_function)(T0,T1,T2,T3,T4,T5,T6,T7))
				:sender(_sender)
				,function(_function)
			{
			}
			virtual R Invoke(T0 p0,T1 p1,T2 p2,T3 p3,T4 p4,T5 p5,T6 p6,T7 p7)
			{
				return (sender->*function)(p0,p1,p2,p3,p4,p5,p6,p7);
			}
		};
		template<typename C>
		class PointerInvoker : public Invoker
		{
		protected:
			C*			function;
		public:
			PointerInvoker(C* _function)
				:function(_function)
			{
			}
			virtual R Invoke(T0 p0,T1 p1,T2 p2,T3 p3,T4 p4,T5 p5,T6 p6,T7 p7)
			{
				return function->operator()(p0,p1,p2,p3,p4,p5,p6,p7);
			}
		};
		template<typename C>
		class SmartPointerInvoker : public Invoker
		{
		protected:
			Ptr<C>		function;
		public:
			SmartPointerInvoker(const Ptr<C>& _function)
				:function(_function)
			{
			}
			virtual R Invoke(T0 p0,T1 p1,T2 p2,T3 p3,T4 p4,T5 p5,T6 p6,T7 p7)
			{
				return function->operator()(p0,p1,p2,p3,p4,p5,p6,p7);
			}
		};
		template<typename C>
		class ObjectInvoker : public Invoker
		{
		protected:
			C			function;
		public:
			ObjectInvoker(const C& _function)
				:function(_function)
			{
			}
			virtual R Invoke(T0 p0,T1 p1,T2 p2,T3 p3,T4 p4,T5 p5,T6 p6,T7 p7)
			{
				return function(p0,p1,p2,p3,p4,p5,p6,p7);
			}
		};
	protected:
		Ptr<Invoker>			invoker;
	public:
		typedef R FunctionType(T0,T1,T2,T3,T4,T5,T6,T7);
		typedef R ResultType;
		Func()
		{
		}
		Func(R(*function)(T0,T1,T2,T3,T4,T5,T6,T7))
		{
			invoker=new StaticInvoker(function);
		}
		template<typename C>
		Func(C* sender, R(C::*function)(T0,T1,T2,T3,T4,T5,T6,T7))
		{
			invoker=new MemberInvoker<C>(sender, function);
		}
		template<typename C>
		Func(C* function)
		{
			invoker=new PointerInvoker<C>(function);
		}
		template<typename C>
		Func(const Ptr<C>& function)
		{
			invoker=new SmartPointerInvoker<C>(function);
		}
		template<typename C>
		Func(const C& function)
		{
			invoker=new ObjectInvoker<C>(function);
		}
		R operator()(T0 p0,T1 p1,T2 p2,T3 p3,T4 p4,T5 p5,T6 p6,T7 p7)const
		{
			return invoker->Invoke(p0,p1,p2,p3,p4,p5,p6,p7);
		}
		bool operator==(const Func<R(T0,T1,T2,T3,T4,T5,T6,T7)>& function)const
		{
			return invoker==function.invoker;
		}
		bool operator!=(const Func<R(T0,T1,T2,T3,T4,T5,T6,T7)>& function)const
		{
			return invoker!=function.invoker;
		}
		bool operator<(const Func<R(T0,T1,T2,T3,T4,T5,T6,T7)>& function)const
		{
			return invoker<function.invoker;
		}
		bool operator<=(const Func<R(T0,T1,T2,T3,T4,T5,T6,T7)>& function)const
		{
			return invoker<=function.invoker;
		}
		bool operator>(const Func<R(T0,T1,T2,T3,T4,T5,T6,T7)>& function)const
		{
			return invoker>function.invoker;
		}
		bool operator>=(const Func<R(T0,T1,T2,T3,T4,T5,T6,T7)>& function)const
		{
			return invoker>=function.invoker;
		}
		operator bool()const
		{
			return invoker;
		}
	};
 
/***********************************************************************
vl::Func<void(T0,T1,T2,T3,T4,T5,T6,T7)>
***********************************************************************/
	template< typename T0,typename T1,typename T2,typename T3,typename T4,typename T5,typename T6,typename T7>
	class Func<void(T0,T1,T2,T3,T4,T5,T6,T7)> : public Object
	{
	protected:
		class Invoker : public Object
		{
		public:
			virtual void Invoke(T0 p0,T1 p1,T2 p2,T3 p3,T4 p4,T5 p5,T6 p6,T7 p7)=0;
		};
		class StaticInvoker : public Invoker
		{
		protected:
			void(*function)(T0,T1,T2,T3,T4,T5,T6,T7);
		public:
			StaticInvoker(void(*_function)(T0,T1,T2,T3,T4,T5,T6,T7))
				:function(_function)
			{
			}
			virtual void Invoke(T0 p0,T1 p1,T2 p2,T3 p3,T4 p4,T5 p5,T6 p6,T7 p7)
			{
				  function(p0,p1,p2,p3,p4,p5,p6,p7);
			}
		};
		template<typename C>
		class MemberInvoker : public Invoker
		{
		protected:
			C*			sender;
			void(C::*function)(T0,T1,T2,T3,T4,T5,T6,T7);
		public:
			MemberInvoker(C* _sender, void(C::*_function)(T0,T1,T2,T3,T4,T5,T6,T7))
				:sender(_sender)
				,function(_function)
			{
			}
			virtual void Invoke(T0 p0,T1 p1,T2 p2,T3 p3,T4 p4,T5 p5,T6 p6,T7 p7)
			{
				  (sender->*function)(p0,p1,p2,p3,p4,p5,p6,p7);
			}
		};
		template<typename C>
		class PointerInvoker : public Invoker
		{
		protected:
			C*			function;
		public:
			PointerInvoker(C* _function)
				:function(_function)
			{
			}
			virtual void Invoke(T0 p0,T1 p1,T2 p2,T3 p3,T4 p4,T5 p5,T6 p6,T7 p7)
			{
				  function->operator()(p0,p1,p2,p3,p4,p5,p6,p7);
			}
		};
		template<typename C>
		class SmartPointerInvoker : public Invoker
		{
		protected:
			Ptr<C>		function;
		public:
			SmartPointerInvoker(const Ptr<C>& _function)
				:function(_function)
			{
			}
			virtual void Invoke(T0 p0,T1 p1,T2 p2,T3 p3,T4 p4,T5 p5,T6 p6,T7 p7)
			{
				  function->operator()(p0,p1,p2,p3,p4,p5,p6,p7);
			}
		};
		template<typename C>
		class ObjectInvoker : public Invoker
		{
		protected:
			C			function;
		public:
			ObjectInvoker(const C& _function)
				:function(_function)
			{
			}
			virtual void Invoke(T0 p0,T1 p1,T2 p2,T3 p3,T4 p4,T5 p5,T6 p6,T7 p7)
			{
				  function(p0,p1,p2,p3,p4,p5,p6,p7);
			}
		};
	protected:
		Ptr<Invoker>			invoker;
	public:
		typedef void FunctionType(T0,T1,T2,T3,T4,T5,T6,T7);
		typedef void ResultType;
		Func()
		{
		}
		Func(void(*function)(T0,T1,T2,T3,T4,T5,T6,T7))
		{
			invoker=new StaticInvoker(function);
		}
		template<typename C>
		Func(C* sender, void(C::*function)(T0,T1,T2,T3,T4,T5,T6,T7))
		{
			invoker=new MemberInvoker<C>(sender, function);
		}
		template<typename C>
		Func(C* function)
		{
			invoker=new PointerInvoker<C>(function);
		}
		template<typename C>
		Func(const Ptr<C>& function)
		{
			invoker=new SmartPointerInvoker<C>(function);
		}
		template<typename C>
		Func(const C& function)
		{
			invoker=new ObjectInvoker<C>(function);
		}
		void operator()(T0 p0,T1 p1,T2 p2,T3 p3,T4 p4,T5 p5,T6 p6,T7 p7)const
		{
			  invoker->Invoke(p0,p1,p2,p3,p4,p5,p6,p7);
		}
		bool operator==(const Func<void(T0,T1,T2,T3,T4,T5,T6,T7)>& function)const
		{
			return invoker==function.invoker;
		}
		bool operator!=(const Func<void(T0,T1,T2,T3,T4,T5,T6,T7)>& function)const
		{
			return invoker!=function.invoker;
		}
		bool operator<(const Func<void(T0,T1,T2,T3,T4,T5,T6,T7)>& function)const
		{
			return invoker<function.invoker;
		}
		bool operator<=(const Func<void(T0,T1,T2,T3,T4,T5,T6,T7)>& function)const
		{
			return invoker<=function.invoker;
		}
		bool operator>(const Func<void(T0,T1,T2,T3,T4,T5,T6,T7)>& function)const
		{
			return invoker>function.invoker;
		}
		bool operator>=(const Func<void(T0,T1,T2,T3,T4,T5,T6,T7)>& function)const
		{
			return invoker>=function.invoker;
		}
		operator bool()const
		{
			return invoker;
		}
	};
  
/***********************************************************************
vl::Func<R(T0,T1,T2,T3,T4,T5,T6,T7,T8)>
***********************************************************************/
	template<typename R,typename T0,typename T1,typename T2,typename T3,typename T4,typename T5,typename T6,typename T7,typename T8>
	class Func<R(T0,T1,T2,T3,T4,T5,T6,T7,T8)> : public Object
	{
	protected:
		class Invoker : public Object
		{
		public:
			virtual R Invoke(T0 p0,T1 p1,T2 p2,T3 p3,T4 p4,T5 p5,T6 p6,T7 p7,T8 p8)=0;
		};
		class StaticInvoker : public Invoker
		{
		protected:
			R(*function)(T0,T1,T2,T3,T4,T5,T6,T7,T8);
		public:
			StaticInvoker(R(*_function)(T0,T1,T2,T3,T4,T5,T6,T7,T8))
				:function(_function)
			{
			}
			virtual R Invoke(T0 p0,T1 p1,T2 p2,T3 p3,T4 p4,T5 p5,T6 p6,T7 p7,T8 p8)
			{
				return function(p0,p1,p2,p3,p4,p5,p6,p7,p8);
			}
		};
		template<typename C>
		class MemberInvoker : public Invoker
		{
		protected:
			C*			sender;
			R(C::*function)(T0,T1,T2,T3,T4,T5,T6,T7,T8);
		public:
			MemberInvoker(C* _sender, R(C::*_function)(T0,T1,T2,T3,T4,T5,T6,T7,T8))
				:sender(_sender)
				,function(_function)
			{
			}
			virtual R Invoke(T0 p0,T1 p1,T2 p2,T3 p3,T4 p4,T5 p5,T6 p6,T7 p7,T8 p8)
			{
				return (sender->*function)(p0,p1,p2,p3,p4,p5,p6,p7,p8);
			}
		};
		template<typename C>
		class PointerInvoker : public Invoker
		{
		protected:
			C*			function;
		public:
			PointerInvoker(C* _function)
				:function(_function)
			{
			}
			virtual R Invoke(T0 p0,T1 p1,T2 p2,T3 p3,T4 p4,T5 p5,T6 p6,T7 p7,T8 p8)
			{
				return function->operator()(p0,p1,p2,p3,p4,p5,p6,p7,p8);
			}
		};
		template<typename C>
		class SmartPointerInvoker : public Invoker
		{
		protected:
			Ptr<C>		function;
		public:
			SmartPointerInvoker(const Ptr<C>& _function)
				:function(_function)
			{
			}
			virtual R Invoke(T0 p0,T1 p1,T2 p2,T3 p3,T4 p4,T5 p5,T6 p6,T7 p7,T8 p8)
			{
				return function->operator()(p0,p1,p2,p3,p4,p5,p6,p7,p8);
			}
		};
		template<typename C>
		class ObjectInvoker : public Invoker
		{
		protected:
			C			function;
		public:
			ObjectInvoker(const C& _function)
				:function(_function)
			{
			}
			virtual R Invoke(T0 p0,T1 p1,T2 p2,T3 p3,T4 p4,T5 p5,T6 p6,T7 p7,T8 p8)
			{
				return function(p0,p1,p2,p3,p4,p5,p6,p7,p8);
			}
		};
	protected:
		Ptr<Invoker>			invoker;
	public:
		typedef R FunctionType(T0,T1,T2,T3,T4,T5,T6,T7,T8);
		typedef R ResultType;
		Func()
		{
		}
		Func(R(*function)(T0,T1,T2,T3,T4,T5,T6,T7,T8))
		{
			invoker=new StaticInvoker(function);
		}
		template<typename C>
		Func(C* sender, R(C::*function)(T0,T1,T2,T3,T4,T5,T6,T7,T8))
		{
			invoker=new MemberInvoker<C>(sender, function);
		}
		template<typename C>
		Func(C* function)
		{
			invoker=new PointerInvoker<C>(function);
		}
		template<typename C>
		Func(const Ptr<C>& function)
		{
			invoker=new SmartPointerInvoker<C>(function);
		}
		template<typename C>
		Func(const C& function)
		{
			invoker=new ObjectInvoker<C>(function);
		}
		R operator()(T0 p0,T1 p1,T2 p2,T3 p3,T4 p4,T5 p5,T6 p6,T7 p7,T8 p8)const
		{
			return invoker->Invoke(p0,p1,p2,p3,p4,p5,p6,p7,p8);
		}
		bool operator==(const Func<R(T0,T1,T2,T3,T4,T5,T6,T7,T8)>& function)const
		{
			return invoker==function.invoker;
		}
		bool operator!=(const Func<R(T0,T1,T2,T3,T4,T5,T6,T7,T8)>& function)const
		{
			return invoker!=function.invoker;
		}
		bool operator<(const Func<R(T0,T1,T2,T3,T4,T5,T6,T7,T8)>& function)const
		{
			return invoker<function.invoker;
		}
		bool operator<=(const Func<R(T0,T1,T2,T3,T4,T5,T6,T7,T8)>& function)const
		{
			return invoker<=function.invoker;
		}
		bool operator>(const Func<R(T0,T1,T2,T3,T4,T5,T6,T7,T8)>& function)const
		{
			return invoker>function.invoker;
		}
		bool operator>=(const Func<R(T0,T1,T2,T3,T4,T5,T6,T7,T8)>& function)const
		{
			return invoker>=function.invoker;
		}
		operator bool()const
		{
			return invoker;
		}
	};
 
/***********************************************************************
vl::Func<void(T0,T1,T2,T3,T4,T5,T6,T7,T8)>
***********************************************************************/
	template< typename T0,typename T1,typename T2,typename T3,typename T4,typename T5,typename T6,typename T7,typename T8>
	class Func<void(T0,T1,T2,T3,T4,T5,T6,T7,T8)> : public Object
	{
	protected:
		class Invoker : public Object
		{
		public:
			virtual void Invoke(T0 p0,T1 p1,T2 p2,T3 p3,T4 p4,T5 p5,T6 p6,T7 p7,T8 p8)=0;
		};
		class StaticInvoker : public Invoker
		{
		protected:
			void(*function)(T0,T1,T2,T3,T4,T5,T6,T7,T8);
		public:
			StaticInvoker(void(*_function)(T0,T1,T2,T3,T4,T5,T6,T7,T8))
				:function(_function)
			{
			}
			virtual void Invoke(T0 p0,T1 p1,T2 p2,T3 p3,T4 p4,T5 p5,T6 p6,T7 p7,T8 p8)
			{
				  function(p0,p1,p2,p3,p4,p5,p6,p7,p8);
			}
		};
		template<typename C>
		class MemberInvoker : public Invoker
		{
		protected:
			C*			sender;
			void(C::*function)(T0,T1,T2,T3,T4,T5,T6,T7,T8);
		public:
			MemberInvoker(C* _sender, void(C::*_function)(T0,T1,T2,T3,T4,T5,T6,T7,T8))
				:sender(_sender)
				,function(_function)
			{
			}
			virtual void Invoke(T0 p0,T1 p1,T2 p2,T3 p3,T4 p4,T5 p5,T6 p6,T7 p7,T8 p8)
			{
				  (sender->*function)(p0,p1,p2,p3,p4,p5,p6,p7,p8);
			}
		};
		template<typename C>
		class PointerInvoker : public Invoker
		{
		protected:
			C*			function;
		public:
			PointerInvoker(C* _function)
				:function(_function)
			{
			}
			virtual void Invoke(T0 p0,T1 p1,T2 p2,T3 p3,T4 p4,T5 p5,T6 p6,T7 p7,T8 p8)
			{
				  function->operator()(p0,p1,p2,p3,p4,p5,p6,p7,p8);
			}
		};
		template<typename C>
		class SmartPointerInvoker : public Invoker
		{
		protected:
			Ptr<C>		function;
		public:
			SmartPointerInvoker(const Ptr<C>& _function)
				:function(_function)
			{
			}
			virtual void Invoke(T0 p0,T1 p1,T2 p2,T3 p3,T4 p4,T5 p5,T6 p6,T7 p7,T8 p8)
			{
				  function->operator()(p0,p1,p2,p3,p4,p5,p6,p7,p8);
			}
		};
		template<typename C>
		class ObjectInvoker : public Invoker
		{
		protected:
			C			function;
		public:
			ObjectInvoker(const C& _function)
				:function(_function)
			{
			}
			virtual void Invoke(T0 p0,T1 p1,T2 p2,T3 p3,T4 p4,T5 p5,T6 p6,T7 p7,T8 p8)
			{
				  function(p0,p1,p2,p3,p4,p5,p6,p7,p8);
			}
		};
	protected:
		Ptr<Invoker>			invoker;
	public:
		typedef void FunctionType(T0,T1,T2,T3,T4,T5,T6,T7,T8);
		typedef void ResultType;
		Func()
		{
		}
		Func(void(*function)(T0,T1,T2,T3,T4,T5,T6,T7,T8))
		{
			invoker=new StaticInvoker(function);
		}
		template<typename C>
		Func(C* sender, void(C::*function)(T0,T1,T2,T3,T4,T5,T6,T7,T8))
		{
			invoker=new MemberInvoker<C>(sender, function);
		}
		template<typename C>
		Func(C* function)
		{
			invoker=new PointerInvoker<C>(function);
		}
		template<typename C>
		Func(const Ptr<C>& function)
		{
			invoker=new SmartPointerInvoker<C>(function);
		}
		template<typename C>
		Func(const C& function)
		{
			invoker=new ObjectInvoker<C>(function);
		}
		void operator()(T0 p0,T1 p1,T2 p2,T3 p3,T4 p4,T5 p5,T6 p6,T7 p7,T8 p8)const
		{
			  invoker->Invoke(p0,p1,p2,p3,p4,p5,p6,p7,p8);
		}
		bool operator==(const Func<void(T0,T1,T2,T3,T4,T5,T6,T7,T8)>& function)const
		{
			return invoker==function.invoker;
		}
		bool operator!=(const Func<void(T0,T1,T2,T3,T4,T5,T6,T7,T8)>& function)const
		{
			return invoker!=function.invoker;
		}
		bool operator<(const Func<void(T0,T1,T2,T3,T4,T5,T6,T7,T8)>& function)const
		{
			return invoker<function.invoker;
		}
		bool operator<=(const Func<void(T0,T1,T2,T3,T4,T5,T6,T7,T8)>& function)const
		{
			return invoker<=function.invoker;
		}
		bool operator>(const Func<void(T0,T1,T2,T3,T4,T5,T6,T7,T8)>& function)const
		{
			return invoker>function.invoker;
		}
		bool operator>=(const Func<void(T0,T1,T2,T3,T4,T5,T6,T7,T8)>& function)const
		{
			return invoker>=function.invoker;
		}
		operator bool()const
		{
			return invoker;
		}
	};
  
/***********************************************************************
vl::Func<R(T0,T1,T2,T3,T4,T5,T6,T7,T8,T9)>
***********************************************************************/
	template<typename R,typename T0,typename T1,typename T2,typename T3,typename T4,typename T5,typename T6,typename T7,typename T8,typename T9>
	class Func<R(T0,T1,T2,T3,T4,T5,T6,T7,T8,T9)> : public Object
	{
	protected:
		class Invoker : public Object
		{
		public:
			virtual R Invoke(T0 p0,T1 p1,T2 p2,T3 p3,T4 p4,T5 p5,T6 p6,T7 p7,T8 p8,T9 p9)=0;
		};
		class StaticInvoker : public Invoker
		{
		protected:
			R(*function)(T0,T1,T2,T3,T4,T5,T6,T7,T8,T9);
		public:
			StaticInvoker(R(*_function)(T0,T1,T2,T3,T4,T5,T6,T7,T8,T9))
				:function(_function)
			{
			}
			virtual R Invoke(T0 p0,T1 p1,T2 p2,T3 p3,T4 p4,T5 p5,T6 p6,T7 p7,T8 p8,T9 p9)
			{
				return function(p0,p1,p2,p3,p4,p5,p6,p7,p8,p9);
			}
		};
		template<typename C>
		class MemberInvoker : public Invoker
		{
		protected:
			C*			sender;
			R(C::*function)(T0,T1,T2,T3,T4,T5,T6,T7,T8,T9);
		public:
			MemberInvoker(C* _sender, R(C::*_function)(T0,T1,T2,T3,T4,T5,T6,T7,T8,T9))
				:sender(_sender)
				,function(_function)
			{
			}
			virtual R Invoke(T0 p0,T1 p1,T2 p2,T3 p3,T4 p4,T5 p5,T6 p6,T7 p7,T8 p8,T9 p9)
			{
				return (sender->*function)(p0,p1,p2,p3,p4,p5,p6,p7,p8,p9);
			}
		};
		template<typename C>
		class PointerInvoker : public Invoker
		{
		protected:
			C*			function;
		public:
			PointerInvoker(C* _function)
				:function(_function)
			{
			}
			virtual R Invoke(T0 p0,T1 p1,T2 p2,T3 p3,T4 p4,T5 p5,T6 p6,T7 p7,T8 p8,T9 p9)
			{
				return function->operator()(p0,p1,p2,p3,p4,p5,p6,p7,p8,p9);
			}
		};
		template<typename C>
		class SmartPointerInvoker : public Invoker
		{
		protected:
			Ptr<C>		function;
		public:
			SmartPointerInvoker(const Ptr<C>& _function)
				:function(_function)
			{
			}
			virtual R Invoke(T0 p0,T1 p1,T2 p2,T3 p3,T4 p4,T5 p5,T6 p6,T7 p7,T8 p8,T9 p9)
			{
				return function->operator()(p0,p1,p2,p3,p4,p5,p6,p7,p8,p9);
			}
		};
		template<typename C>
		class ObjectInvoker : public Invoker
		{
		protected:
			C			function;
		public:
			ObjectInvoker(const C& _function)
				:function(_function)
			{
			}
			virtual R Invoke(T0 p0,T1 p1,T2 p2,T3 p3,T4 p4,T5 p5,T6 p6,T7 p7,T8 p8,T9 p9)
			{
				return function(p0,p1,p2,p3,p4,p5,p6,p7,p8,p9);
			}
		};
	protected:
		Ptr<Invoker>			invoker;
	public:
		typedef R FunctionType(T0,T1,T2,T3,T4,T5,T6,T7,T8,T9);
		typedef R ResultType;
		Func()
		{
		}
		Func(R(*function)(T0,T1,T2,T3,T4,T5,T6,T7,T8,T9))
		{
			invoker=new StaticInvoker(function);
		}
		template<typename C>
		Func(C* sender, R(C::*function)(T0,T1,T2,T3,T4,T5,T6,T7,T8,T9))
		{
			invoker=new MemberInvoker<C>(sender, function);
		}
		template<typename C>
		Func(C* function)
		{
			invoker=new PointerInvoker<C>(function);
		}
		template<typename C>
		Func(const Ptr<C>& function)
		{
			invoker=new SmartPointerInvoker<C>(function);
		}
		template<typename C>
		Func(const C& function)
		{
			invoker=new ObjectInvoker<C>(function);
		}
		R operator()(T0 p0,T1 p1,T2 p2,T3 p3,T4 p4,T5 p5,T6 p6,T7 p7,T8 p8,T9 p9)const
		{
			return invoker->Invoke(p0,p1,p2,p3,p4,p5,p6,p7,p8,p9);
		}
		bool operator==(const Func<R(T0,T1,T2,T3,T4,T5,T6,T7,T8,T9)>& function)const
		{
			return invoker==function.invoker;
		}
		bool operator!=(const Func<R(T0,T1,T2,T3,T4,T5,T6,T7,T8,T9)>& function)const
		{
			return invoker!=function.invoker;
		}
		bool operator<(const Func<R(T0,T1,T2,T3,T4,T5,T6,T7,T8,T9)>& function)const
		{
			return invoker<function.invoker;
		}
		bool operator<=(const Func<R(T0,T1,T2,T3,T4,T5,T6,T7,T8,T9)>& function)const
		{
			return invoker<=function.invoker;
		}
		bool operator>(const Func<R(T0,T1,T2,T3,T4,T5,T6,T7,T8,T9)>& function)const
		{
			return invoker>function.invoker;
		}
		bool operator>=(const Func<R(T0,T1,T2,T3,T4,T5,T6,T7,T8,T9)>& function)const
		{
			return invoker>=function.invoker;
		}
		operator bool()const
		{
			return invoker;
		}
	};
 
/***********************************************************************
vl::Func<void(T0,T1,T2,T3,T4,T5,T6,T7,T8,T9)>
***********************************************************************/
	template< typename T0,typename T1,typename T2,typename T3,typename T4,typename T5,typename T6,typename T7,typename T8,typename T9>
	class Func<void(T0,T1,T2,T3,T4,T5,T6,T7,T8,T9)> : public Object
	{
	protected:
		class Invoker : public Object
		{
		public:
			virtual void Invoke(T0 p0,T1 p1,T2 p2,T3 p3,T4 p4,T5 p5,T6 p6,T7 p7,T8 p8,T9 p9)=0;
		};
		class StaticInvoker : public Invoker
		{
		protected:
			void(*function)(T0,T1,T2,T3,T4,T5,T6,T7,T8,T9);
		public:
			StaticInvoker(void(*_function)(T0,T1,T2,T3,T4,T5,T6,T7,T8,T9))
				:function(_function)
			{
			}
			virtual void Invoke(T0 p0,T1 p1,T2 p2,T3 p3,T4 p4,T5 p5,T6 p6,T7 p7,T8 p8,T9 p9)
			{
				  function(p0,p1,p2,p3,p4,p5,p6,p7,p8,p9);
			}
		};
		template<typename C>
		class MemberInvoker : public Invoker
		{
		protected:
			C*			sender;
			void(C::*function)(T0,T1,T2,T3,T4,T5,T6,T7,T8,T9);
		public:
			MemberInvoker(C* _sender, void(C::*_function)(T0,T1,T2,T3,T4,T5,T6,T7,T8,T9))
				:sender(_sender)
				,function(_function)
			{
			}
			virtual void Invoke(T0 p0,T1 p1,T2 p2,T3 p3,T4 p4,T5 p5,T6 p6,T7 p7,T8 p8,T9 p9)
			{
				  (sender->*function)(p0,p1,p2,p3,p4,p5,p6,p7,p8,p9);
			}
		};
		template<typename C>
		class PointerInvoker : public Invoker
		{
		protected:
			C*			function;
		public:
			PointerInvoker(C* _function)
				:function(_function)
			{
			}
			virtual void Invoke(T0 p0,T1 p1,T2 p2,T3 p3,T4 p4,T5 p5,T6 p6,T7 p7,T8 p8,T9 p9)
			{
				  function->operator()(p0,p1,p2,p3,p4,p5,p6,p7,p8,p9);
			}
		};
		template<typename C>
		class SmartPointerInvoker : public Invoker
		{
		protected:
			Ptr<C>		function;
		public:
			SmartPointerInvoker(const Ptr<C>& _function)
				:function(_function)
			{
			}
			virtual void Invoke(T0 p0,T1 p1,T2 p2,T3 p3,T4 p4,T5 p5,T6 p6,T7 p7,T8 p8,T9 p9)
			{
				  function->operator()(p0,p1,p2,p3,p4,p5,p6,p7,p8,p9);
			}
		};
		template<typename C>
		class ObjectInvoker : public Invoker
		{
		protected:
			C			function;
		public:
			ObjectInvoker(const C& _function)
				:function(_function)
			{
			}
			virtual void Invoke(T0 p0,T1 p1,T2 p2,T3 p3,T4 p4,T5 p5,T6 p6,T7 p7,T8 p8,T9 p9)
			{
				  function(p0,p1,p2,p3,p4,p5,p6,p7,p8,p9);
			}
		};
	protected:
		Ptr<Invoker>			invoker;
	public:
		typedef void FunctionType(T0,T1,T2,T3,T4,T5,T6,T7,T8,T9);
		typedef void ResultType;
		Func()
		{
		}
		Func(void(*function)(T0,T1,T2,T3,T4,T5,T6,T7,T8,T9))
		{
			invoker=new StaticInvoker(function);
		}
		template<typename C>
		Func(C* sender, void(C::*function)(T0,T1,T2,T3,T4,T5,T6,T7,T8,T9))
		{
			invoker=new MemberInvoker<C>(sender, function);
		}
		template<typename C>
		Func(C* function)
		{
			invoker=new PointerInvoker<C>(function);
		}
		template<typename C>
		Func(const Ptr<C>& function)
		{
			invoker=new SmartPointerInvoker<C>(function);
		}
		template<typename C>
		Func(const C& function)
		{
			invoker=new ObjectInvoker<C>(function);
		}
		void operator()(T0 p0,T1 p1,T2 p2,T3 p3,T4 p4,T5 p5,T6 p6,T7 p7,T8 p8,T9 p9)const
		{
			  invoker->Invoke(p0,p1,p2,p3,p4,p5,p6,p7,p8,p9);
		}
		bool operator==(const Func<void(T0,T1,T2,T3,T4,T5,T6,T7,T8,T9)>& function)const
		{
			return invoker==function.invoker;
		}
		bool operator!=(const Func<void(T0,T1,T2,T3,T4,T5,T6,T7,T8,T9)>& function)const
		{
			return invoker!=function.invoker;
		}
		bool operator<(const Func<void(T0,T1,T2,T3,T4,T5,T6,T7,T8,T9)>& function)const
		{
			return invoker<function.invoker;
		}
		bool operator<=(const Func<void(T0,T1,T2,T3,T4,T5,T6,T7,T8,T9)>& function)const
		{
			return invoker<=function.invoker;
		}
		bool operator>(const Func<void(T0,T1,T2,T3,T4,T5,T6,T7,T8,T9)>& function)const
		{
			return invoker>function.invoker;
		}
		bool operator>=(const Func<void(T0,T1,T2,T3,T4,T5,T6,T7,T8,T9)>& function)const
		{
			return invoker>=function.invoker;
		}
		operator bool()const
		{
			return invoker;
		}
	};
 
	namespace function_binding
	{
		template<typename T>
		struct Binding
		{
		};
		template<typename T>
		struct CR{typedef const T& Type;};
		template<typename T>
		struct CR<T&>{typedef T& Type;};
		template<typename T>
		struct CR<const T>{typedef const T& Type;};
		template<typename T>
		struct CR<const T&>{typedef const T& Type;};
		
 
/***********************************************************************
vl::function_binding::Binding<R(T0)>
***********************************************************************/
		template<typename R,typename T0>
		struct Binding<R(T0)>
		{
			typedef R FunctionType(T0);
			typedef R CurriedType();
			typedef T0 ParameterType;
			class Binder : public Object
			{
			protected:
				Func<FunctionType>		target;
				T0						p0;
			public:
				Binder(const Func<FunctionType>& _target, typename CR<T0>::Type _p0)
					:target(_target)
					,p0(_p0)
				{
				}
				R operator()()const
				{
					return target(p0);
				}
			};
			class Currier : public Object
			{
			protected:
				Func<FunctionType>		target;
			public:
				Currier(const Func<FunctionType>& _target)
					:target(_target)
				{
				}
				Func<CurriedType> operator()(typename CR<T0>::Type argument)const
				{
					return Ptr<Binder>(new Binder(target, argument));
				}
			};
		};
 
/***********************************************************************
vl::function_binding::Binding<void(T0)>
***********************************************************************/
		template< typename T0>
		struct Binding<void(T0)>
		{
			typedef void FunctionType(T0);
			typedef void CurriedType();
			typedef T0 ParameterType;
			class Binder : public Object
			{
			protected:
				Func<FunctionType>		target;
				T0						p0;
			public:
				Binder(const Func<FunctionType>& _target, typename CR<T0>::Type _p0)
					:target(_target)
					,p0(_p0)
				{
				}
				void operator()()const
				{
					  target(p0);
				}
			};
			class Currier : public Object
			{
			protected:
				Func<FunctionType>		target;
			public:
				Currier(const Func<FunctionType>& _target)
					:target(_target)
				{
				}
				Func<CurriedType> operator()(typename CR<T0>::Type argument)const
				{
					return Ptr<Binder>(new Binder(target, argument));
				}
			};
		};
  
/***********************************************************************
vl::function_binding::Binding<R(T0,T1)>
***********************************************************************/
		template<typename R,typename T0,typename T1>
		struct Binding<R(T0,T1)>
		{
			typedef R FunctionType(T0,T1);
			typedef R CurriedType(T1);
			typedef T0 ParameterType;
			class Binder : public Object
			{
			protected:
				Func<FunctionType>		target;
				T0						p0;
			public:
				Binder(const Func<FunctionType>& _target, typename CR<T0>::Type _p0)
					:target(_target)
					,p0(_p0)
				{
				}
				R operator()(T1 p1)const
				{
					return target(p0,p1);
				}
			};
			class Currier : public Object
			{
			protected:
				Func<FunctionType>		target;
			public:
				Currier(const Func<FunctionType>& _target)
					:target(_target)
				{
				}
				Func<CurriedType> operator()(typename CR<T0>::Type argument)const
				{
					return Ptr<Binder>(new Binder(target, argument));
				}
			};
		};
 
/***********************************************************************
vl::function_binding::Binding<void(T0,T1)>
***********************************************************************/
		template< typename T0,typename T1>
		struct Binding<void(T0,T1)>
		{
			typedef void FunctionType(T0,T1);
			typedef void CurriedType(T1);
			typedef T0 ParameterType;
			class Binder : public Object
			{
			protected:
				Func<FunctionType>		target;
				T0						p0;
			public:
				Binder(const Func<FunctionType>& _target, typename CR<T0>::Type _p0)
					:target(_target)
					,p0(_p0)
				{
				}
				void operator()(T1 p1)const
				{
					  target(p0,p1);
				}
			};
			class Currier : public Object
			{
			protected:
				Func<FunctionType>		target;
			public:
				Currier(const Func<FunctionType>& _target)
					:target(_target)
				{
				}
				Func<CurriedType> operator()(typename CR<T0>::Type argument)const
				{
					return Ptr<Binder>(new Binder(target, argument));
				}
			};
		};
  
/***********************************************************************
vl::function_binding::Binding<R(T0,T1,T2)>
***********************************************************************/
		template<typename R,typename T0,typename T1,typename T2>
		struct Binding<R(T0,T1,T2)>
		{
			typedef R FunctionType(T0,T1,T2);
			typedef R CurriedType(T1,T2);
			typedef T0 ParameterType;
			class Binder : public Object
			{
			protected:
				Func<FunctionType>		target;
				T0						p0;
			public:
				Binder(const Func<FunctionType>& _target, typename CR<T0>::Type _p0)
					:target(_target)
					,p0(_p0)
				{
				}
				R operator()(T1 p1,T2 p2)const
				{
					return target(p0,p1,p2);
				}
			};
			class Currier : public Object
			{
			protected:
				Func<FunctionType>		target;
			public:
				Currier(const Func<FunctionType>& _target)
					:target(_target)
				{
				}
				Func<CurriedType> operator()(typename CR<T0>::Type argument)const
				{
					return Ptr<Binder>(new Binder(target, argument));
				}
			};
		};
 
/***********************************************************************
vl::function_binding::Binding<void(T0,T1,T2)>
***********************************************************************/
		template< typename T0,typename T1,typename T2>
		struct Binding<void(T0,T1,T2)>
		{
			typedef void FunctionType(T0,T1,T2);
			typedef void CurriedType(T1,T2);
			typedef T0 ParameterType;
			class Binder : public Object
			{
			protected:
				Func<FunctionType>		target;
				T0						p0;
			public:
				Binder(const Func<FunctionType>& _target, typename CR<T0>::Type _p0)
					:target(_target)
					,p0(_p0)
				{
				}
				void operator()(T1 p1,T2 p2)const
				{
					  target(p0,p1,p2);
				}
			};
			class Currier : public Object
			{
			protected:
				Func<FunctionType>		target;
			public:
				Currier(const Func<FunctionType>& _target)
					:target(_target)
				{
				}
				Func<CurriedType> operator()(typename CR<T0>::Type argument)const
				{
					return Ptr<Binder>(new Binder(target, argument));
				}
			};
		};
  
/***********************************************************************
vl::function_binding::Binding<R(T0,T1,T2,T3)>
***********************************************************************/
		template<typename R,typename T0,typename T1,typename T2,typename T3>
		struct Binding<R(T0,T1,T2,T3)>
		{
			typedef R FunctionType(T0,T1,T2,T3);
			typedef R CurriedType(T1,T2,T3);
			typedef T0 ParameterType;
			class Binder : public Object
			{
			protected:
				Func<FunctionType>		target;
				T0						p0;
			public:
				Binder(const Func<FunctionType>& _target, typename CR<T0>::Type _p0)
					:target(_target)
					,p0(_p0)
				{
				}
				R operator()(T1 p1,T2 p2,T3 p3)const
				{
					return target(p0,p1,p2,p3);
				}
			};
			class Currier : public Object
			{
			protected:
				Func<FunctionType>		target;
			public:
				Currier(const Func<FunctionType>& _target)
					:target(_target)
				{
				}
				Func<CurriedType> operator()(typename CR<T0>::Type argument)const
				{
					return Ptr<Binder>(new Binder(target, argument));
				}
			};
		};
 
/***********************************************************************
vl::function_binding::Binding<void(T0,T1,T2,T3)>
***********************************************************************/
		template< typename T0,typename T1,typename T2,typename T3>
		struct Binding<void(T0,T1,T2,T3)>
		{
			typedef void FunctionType(T0,T1,T2,T3);
			typedef void CurriedType(T1,T2,T3);
			typedef T0 ParameterType;
			class Binder : public Object
			{
			protected:
				Func<FunctionType>		target;
				T0						p0;
			public:
				Binder(const Func<FunctionType>& _target, typename CR<T0>::Type _p0)
					:target(_target)
					,p0(_p0)
				{
				}
				void operator()(T1 p1,T2 p2,T3 p3)const
				{
					  target(p0,p1,p2,p3);
				}
			};
			class Currier : public Object
			{
			protected:
				Func<FunctionType>		target;
			public:
				Currier(const Func<FunctionType>& _target)
					:target(_target)
				{
				}
				Func<CurriedType> operator()(typename CR<T0>::Type argument)const
				{
					return Ptr<Binder>(new Binder(target, argument));
				}
			};
		};
  
/***********************************************************************
vl::function_binding::Binding<R(T0,T1,T2,T3,T4)>
***********************************************************************/
		template<typename R,typename T0,typename T1,typename T2,typename T3,typename T4>
		struct Binding<R(T0,T1,T2,T3,T4)>
		{
			typedef R FunctionType(T0,T1,T2,T3,T4);
			typedef R CurriedType(T1,T2,T3,T4);
			typedef T0 ParameterType;
			class Binder : public Object
			{
			protected:
				Func<FunctionType>		target;
				T0						p0;
			public:
				Binder(const Func<FunctionType>& _target, typename CR<T0>::Type _p0)
					:target(_target)
					,p0(_p0)
				{
				}
				R operator()(T1 p1,T2 p2,T3 p3,T4 p4)const
				{
					return target(p0,p1,p2,p3,p4);
				}
			};
			class Currier : public Object
			{
			protected:
				Func<FunctionType>		target;
			public:
				Currier(const Func<FunctionType>& _target)
					:target(_target)
				{
				}
				Func<CurriedType> operator()(typename CR<T0>::Type argument)const
				{
					return Ptr<Binder>(new Binder(target, argument));
				}
			};
		};
 
/***********************************************************************
vl::function_binding::Binding<void(T0,T1,T2,T3,T4)>
***********************************************************************/
		template< typename T0,typename T1,typename T2,typename T3,typename T4>
		struct Binding<void(T0,T1,T2,T3,T4)>
		{
			typedef void FunctionType(T0,T1,T2,T3,T4);
			typedef void CurriedType(T1,T2,T3,T4);
			typedef T0 ParameterType;
			class Binder : public Object
			{
			protected:
				Func<FunctionType>		target;
				T0						p0;
			public:
				Binder(const Func<FunctionType>& _target, typename CR<T0>::Type _p0)
					:target(_target)
					,p0(_p0)
				{
				}
				void operator()(T1 p1,T2 p2,T3 p3,T4 p4)const
				{
					  target(p0,p1,p2,p3,p4);
				}
			};
			class Currier : public Object
			{
			protected:
				Func<FunctionType>		target;
			public:
				Currier(const Func<FunctionType>& _target)
					:target(_target)
				{
				}
				Func<CurriedType> operator()(typename CR<T0>::Type argument)const
				{
					return Ptr<Binder>(new Binder(target, argument));
				}
			};
		};
  
/***********************************************************************
vl::function_binding::Binding<R(T0,T1,T2,T3,T4,T5)>
***********************************************************************/
		template<typename R,typename T0,typename T1,typename T2,typename T3,typename T4,typename T5>
		struct Binding<R(T0,T1,T2,T3,T4,T5)>
		{
			typedef R FunctionType(T0,T1,T2,T3,T4,T5);
			typedef R CurriedType(T1,T2,T3,T4,T5);
			typedef T0 ParameterType;
			class Binder : public Object
			{
			protected:
				Func<FunctionType>		target;
				T0						p0;
			public:
				Binder(const Func<FunctionType>& _target, typename CR<T0>::Type _p0)
					:target(_target)
					,p0(_p0)
				{
				}
				R operator()(T1 p1,T2 p2,T3 p3,T4 p4,T5 p5)const
				{
					return target(p0,p1,p2,p3,p4,p5);
				}
			};
			class Currier : public Object
			{
			protected:
				Func<FunctionType>		target;
			public:
				Currier(const Func<FunctionType>& _target)
					:target(_target)
				{
				}
				Func<CurriedType> operator()(typename CR<T0>::Type argument)const
				{
					return Ptr<Binder>(new Binder(target, argument));
				}
			};
		};
 
/***********************************************************************
vl::function_binding::Binding<void(T0,T1,T2,T3,T4,T5)>
***********************************************************************/
		template< typename T0,typename T1,typename T2,typename T3,typename T4,typename T5>
		struct Binding<void(T0,T1,T2,T3,T4,T5)>
		{
			typedef void FunctionType(T0,T1,T2,T3,T4,T5);
			typedef void CurriedType(T1,T2,T3,T4,T5);
			typedef T0 ParameterType;
			class Binder : public Object
			{
			protected:
				Func<FunctionType>		target;
				T0						p0;
			public:
				Binder(const Func<FunctionType>& _target, typename CR<T0>::Type _p0)
					:target(_target)
					,p0(_p0)
				{
				}
				void operator()(T1 p1,T2 p2,T3 p3,T4 p4,T5 p5)const
				{
					  target(p0,p1,p2,p3,p4,p5);
				}
			};
			class Currier : public Object
			{
			protected:
				Func<FunctionType>		target;
			public:
				Currier(const Func<FunctionType>& _target)
					:target(_target)
				{
				}
				Func<CurriedType> operator()(typename CR<T0>::Type argument)const
				{
					return Ptr<Binder>(new Binder(target, argument));
				}
			};
		};
  
/***********************************************************************
vl::function_binding::Binding<R(T0,T1,T2,T3,T4,T5,T6)>
***********************************************************************/
		template<typename R,typename T0,typename T1,typename T2,typename T3,typename T4,typename T5,typename T6>
		struct Binding<R(T0,T1,T2,T3,T4,T5,T6)>
		{
			typedef R FunctionType(T0,T1,T2,T3,T4,T5,T6);
			typedef R CurriedType(T1,T2,T3,T4,T5,T6);
			typedef T0 ParameterType;
			class Binder : public Object
			{
			protected:
				Func<FunctionType>		target;
				T0						p0;
			public:
				Binder(const Func<FunctionType>& _target, typename CR<T0>::Type _p0)
					:target(_target)
					,p0(_p0)
				{
				}
				R operator()(T1 p1,T2 p2,T3 p3,T4 p4,T5 p5,T6 p6)const
				{
					return target(p0,p1,p2,p3,p4,p5,p6);
				}
			};
			class Currier : public Object
			{
			protected:
				Func<FunctionType>		target;
			public:
				Currier(const Func<FunctionType>& _target)
					:target(_target)
				{
				}
				Func<CurriedType> operator()(typename CR<T0>::Type argument)const
				{
					return Ptr<Binder>(new Binder(target, argument));
				}
			};
		};
 
/***********************************************************************
vl::function_binding::Binding<void(T0,T1,T2,T3,T4,T5,T6)>
***********************************************************************/
		template< typename T0,typename T1,typename T2,typename T3,typename T4,typename T5,typename T6>
		struct Binding<void(T0,T1,T2,T3,T4,T5,T6)>
		{
			typedef void FunctionType(T0,T1,T2,T3,T4,T5,T6);
			typedef void CurriedType(T1,T2,T3,T4,T5,T6);
			typedef T0 ParameterType;
			class Binder : public Object
			{
			protected:
				Func<FunctionType>		target;
				T0						p0;
			public:
				Binder(const Func<FunctionType>& _target, typename CR<T0>::Type _p0)
					:target(_target)
					,p0(_p0)
				{
				}
				void operator()(T1 p1,T2 p2,T3 p3,T4 p4,T5 p5,T6 p6)const
				{
					  target(p0,p1,p2,p3,p4,p5,p6);
				}
			};
			class Currier : public Object
			{
			protected:
				Func<FunctionType>		target;
			public:
				Currier(const Func<FunctionType>& _target)
					:target(_target)
				{
				}
				Func<CurriedType> operator()(typename CR<T0>::Type argument)const
				{
					return Ptr<Binder>(new Binder(target, argument));
				}
			};
		};
  
/***********************************************************************
vl::function_binding::Binding<R(T0,T1,T2,T3,T4,T5,T6,T7)>
***********************************************************************/
		template<typename R,typename T0,typename T1,typename T2,typename T3,typename T4,typename T5,typename T6,typename T7>
		struct Binding<R(T0,T1,T2,T3,T4,T5,T6,T7)>
		{
			typedef R FunctionType(T0,T1,T2,T3,T4,T5,T6,T7);
			typedef R CurriedType(T1,T2,T3,T4,T5,T6,T7);
			typedef T0 ParameterType;
			class Binder : public Object
			{
			protected:
				Func<FunctionType>		target;
				T0						p0;
			public:
				Binder(const Func<FunctionType>& _target, typename CR<T0>::Type _p0)
					:target(_target)
					,p0(_p0)
				{
				}
				R operator()(T1 p1,T2 p2,T3 p3,T4 p4,T5 p5,T6 p6,T7 p7)const
				{
					return target(p0,p1,p2,p3,p4,p5,p6,p7);
				}
			};
			class Currier : public Object
			{
			protected:
				Func<FunctionType>		target;
			public:
				Currier(const Func<FunctionType>& _target)
					:target(_target)
				{
				}
				Func<CurriedType> operator()(typename CR<T0>::Type argument)const
				{
					return Ptr<Binder>(new Binder(target, argument));
				}
			};
		};
 
/***********************************************************************
vl::function_binding::Binding<void(T0,T1,T2,T3,T4,T5,T6,T7)>
***********************************************************************/
		template< typename T0,typename T1,typename T2,typename T3,typename T4,typename T5,typename T6,typename T7>
		struct Binding<void(T0,T1,T2,T3,T4,T5,T6,T7)>
		{
			typedef void FunctionType(T0,T1,T2,T3,T4,T5,T6,T7);
			typedef void CurriedType(T1,T2,T3,T4,T5,T6,T7);
			typedef T0 ParameterType;
			class Binder : public Object
			{
			protected:
				Func<FunctionType>		target;
				T0						p0;
			public:
				Binder(const Func<FunctionType>& _target, typename CR<T0>::Type _p0)
					:target(_target)
					,p0(_p0)
				{
				}
				void operator()(T1 p1,T2 p2,T3 p3,T4 p4,T5 p5,T6 p6,T7 p7)const
				{
					  target(p0,p1,p2,p3,p4,p5,p6,p7);
				}
			};
			class Currier : public Object
			{
			protected:
				Func<FunctionType>		target;
			public:
				Currier(const Func<FunctionType>& _target)
					:target(_target)
				{
				}
				Func<CurriedType> operator()(typename CR<T0>::Type argument)const
				{
					return Ptr<Binder>(new Binder(target, argument));
				}
			};
		};
  
/***********************************************************************
vl::function_binding::Binding<R(T0,T1,T2,T3,T4,T5,T6,T7,T8)>
***********************************************************************/
		template<typename R,typename T0,typename T1,typename T2,typename T3,typename T4,typename T5,typename T6,typename T7,typename T8>
		struct Binding<R(T0,T1,T2,T3,T4,T5,T6,T7,T8)>
		{
			typedef R FunctionType(T0,T1,T2,T3,T4,T5,T6,T7,T8);
			typedef R CurriedType(T1,T2,T3,T4,T5,T6,T7,T8);
			typedef T0 ParameterType;
			class Binder : public Object
			{
			protected:
				Func<FunctionType>		target;
				T0						p0;
			public:
				Binder(const Func<FunctionType>& _target, typename CR<T0>::Type _p0)
					:target(_target)
					,p0(_p0)
				{
				}
				R operator()(T1 p1,T2 p2,T3 p3,T4 p4,T5 p5,T6 p6,T7 p7,T8 p8)const
				{
					return target(p0,p1,p2,p3,p4,p5,p6,p7,p8);
				}
			};
			class Currier : public Object
			{
			protected:
				Func<FunctionType>		target;
			public:
				Currier(const Func<FunctionType>& _target)
					:target(_target)
				{
				}
				Func<CurriedType> operator()(typename CR<T0>::Type argument)const
				{
					return Ptr<Binder>(new Binder(target, argument));
				}
			};
		};
 
/***********************************************************************
vl::function_binding::Binding<void(T0,T1,T2,T3,T4,T5,T6,T7,T8)>
***********************************************************************/
		template< typename T0,typename T1,typename T2,typename T3,typename T4,typename T5,typename T6,typename T7,typename T8>
		struct Binding<void(T0,T1,T2,T3,T4,T5,T6,T7,T8)>
		{
			typedef void FunctionType(T0,T1,T2,T3,T4,T5,T6,T7,T8);
			typedef void CurriedType(T1,T2,T3,T4,T5,T6,T7,T8);
			typedef T0 ParameterType;
			class Binder : public Object
			{
			protected:
				Func<FunctionType>		target;
				T0						p0;
			public:
				Binder(const Func<FunctionType>& _target, typename CR<T0>::Type _p0)
					:target(_target)
					,p0(_p0)
				{
				}
				void operator()(T1 p1,T2 p2,T3 p3,T4 p4,T5 p5,T6 p6,T7 p7,T8 p8)const
				{
					  target(p0,p1,p2,p3,p4,p5,p6,p7,p8);
				}
			};
			class Currier : public Object
			{
			protected:
				Func<FunctionType>		target;
			public:
				Currier(const Func<FunctionType>& _target)
					:target(_target)
				{
				}
				Func<CurriedType> operator()(typename CR<T0>::Type argument)const
				{
					return Ptr<Binder>(new Binder(target, argument));
				}
			};
		};
  
/***********************************************************************
vl::function_binding::Binding<R(T0,T1,T2,T3,T4,T5,T6,T7,T8,T9)>
***********************************************************************/
		template<typename R,typename T0,typename T1,typename T2,typename T3,typename T4,typename T5,typename T6,typename T7,typename T8,typename T9>
		struct Binding<R(T0,T1,T2,T3,T4,T5,T6,T7,T8,T9)>
		{
			typedef R FunctionType(T0,T1,T2,T3,T4,T5,T6,T7,T8,T9);
			typedef R CurriedType(T1,T2,T3,T4,T5,T6,T7,T8,T9);
			typedef T0 ParameterType;
			class Binder : public Object
			{
			protected:
				Func<FunctionType>		target;
				T0						p0;
			public:
				Binder(const Func<FunctionType>& _target, typename CR<T0>::Type _p0)
					:target(_target)
					,p0(_p0)
				{
				}
				R operator()(T1 p1,T2 p2,T3 p3,T4 p4,T5 p5,T6 p6,T7 p7,T8 p8,T9 p9)const
				{
					return target(p0,p1,p2,p3,p4,p5,p6,p7,p8,p9);
				}
			};
			class Currier : public Object
			{
			protected:
				Func<FunctionType>		target;
			public:
				Currier(const Func<FunctionType>& _target)
					:target(_target)
				{
				}
				Func<CurriedType> operator()(typename CR<T0>::Type argument)const
				{
					return Ptr<Binder>(new Binder(target, argument));
				}
			};
		};
 
/***********************************************************************
vl::function_binding::Binding<void(T0,T1,T2,T3,T4,T5,T6,T7,T8,T9)>
***********************************************************************/
		template< typename T0,typename T1,typename T2,typename T3,typename T4,typename T5,typename T6,typename T7,typename T8,typename T9>
		struct Binding<void(T0,T1,T2,T3,T4,T5,T6,T7,T8,T9)>
		{
			typedef void FunctionType(T0,T1,T2,T3,T4,T5,T6,T7,T8,T9);
			typedef void CurriedType(T1,T2,T3,T4,T5,T6,T7,T8,T9);
			typedef T0 ParameterType;
			class Binder : public Object
			{
			protected:
				Func<FunctionType>		target;
				T0						p0;
			public:
				Binder(const Func<FunctionType>& _target, typename CR<T0>::Type _p0)
					:target(_target)
					,p0(_p0)
				{
				}
				void operator()(T1 p1,T2 p2,T3 p3,T4 p4,T5 p5,T6 p6,T7 p7,T8 p8,T9 p9)const
				{
					  target(p0,p1,p2,p3,p4,p5,p6,p7,p8,p9);
				}
			};
			class Currier : public Object
			{
			protected:
				Func<FunctionType>		target;
			public:
				Currier(const Func<FunctionType>& _target)
					:target(_target)
				{
				}
				Func<CurriedType> operator()(typename CR<T0>::Type argument)const
				{
					return Ptr<Binder>(new Binder(target, argument));
				}
			};
		};
 
	}
	template<typename T>
	Func<Func<typename function_binding::Binding<T>::CurriedType>(typename function_binding::Binding<T>::ParameterType)>
	Curry(T* function)
	{
		return Ptr<typename function_binding::Binding<T>::Currier>(new typename function_binding::Binding<T>::Currier(function));
	}
	template<typename T>
	Func<Func<typename function_binding::Binding<T>::CurriedType>(typename function_binding::Binding<T>::ParameterType)>
	Curry(const Func<T>& function)
	{
		return Ptr<typename function_binding::Binding<T>::Currier>(new typename function_binding::Binding<T>::Currier(function));
	}
	namespace function_combining
	{
		template<typename A, typename B, typename C>
		class Combining
		{
		};
		
 
/***********************************************************************
vl::function_combining::Combining<R1(), R2(), R(R1,R2)>
***********************************************************************/
		template<typename R1, typename R2, typename R >
		class Combining<R1(), R2(), R(R1,R2)> : public Object
		{
		protected:
			Func<R1()>			function1;
			Func<R2()>			function2;
			Func<R(R1,R2)>							converter;
		public:
			typedef R1 FirstFunctionType();
			typedef R2 SecondFunctionType();
			typedef R ConverterFunctionType(R1,R2);
			typedef R FinalFunctionType();
			Combining(const Func<R1()>& _function1, const Func<R2()>& _function2, const Func<R(R1,R2)>& _converter)
				:function1(_function1)
				,function2(_function2)
				,converter(_converter)
			{
			}
			R operator()()const
			{
				return converter(function1(), function2());
			}
		}; 
/***********************************************************************
vl::function_combining::Combining<R1(T0), R2(T0), R(R1,R2)>
***********************************************************************/
		template<typename R1, typename R2, typename R,typename T0>
		class Combining<R1(T0), R2(T0), R(R1,R2)> : public Object
		{
		protected:
			Func<R1(T0)>			function1;
			Func<R2(T0)>			function2;
			Func<R(R1,R2)>							converter;
		public:
			typedef R1 FirstFunctionType(T0);
			typedef R2 SecondFunctionType(T0);
			typedef R ConverterFunctionType(R1,R2);
			typedef R FinalFunctionType(T0);
			Combining(const Func<R1(T0)>& _function1, const Func<R2(T0)>& _function2, const Func<R(R1,R2)>& _converter)
				:function1(_function1)
				,function2(_function2)
				,converter(_converter)
			{
			}
			R operator()(T0 p0)const
			{
				return converter(function1(p0), function2(p0));
			}
		}; 
/***********************************************************************
vl::function_combining::Combining<R1(T0,T1), R2(T0,T1), R(R1,R2)>
***********************************************************************/
		template<typename R1, typename R2, typename R,typename T0,typename T1>
		class Combining<R1(T0,T1), R2(T0,T1), R(R1,R2)> : public Object
		{
		protected:
			Func<R1(T0,T1)>			function1;
			Func<R2(T0,T1)>			function2;
			Func<R(R1,R2)>							converter;
		public:
			typedef R1 FirstFunctionType(T0,T1);
			typedef R2 SecondFunctionType(T0,T1);
			typedef R ConverterFunctionType(R1,R2);
			typedef R FinalFunctionType(T0,T1);
			Combining(const Func<R1(T0,T1)>& _function1, const Func<R2(T0,T1)>& _function2, const Func<R(R1,R2)>& _converter)
				:function1(_function1)
				,function2(_function2)
				,converter(_converter)
			{
			}
			R operator()(T0 p0,T1 p1)const
			{
				return converter(function1(p0,p1), function2(p0,p1));
			}
		}; 
/***********************************************************************
vl::function_combining::Combining<R1(T0,T1,T2), R2(T0,T1,T2), R(R1,R2)>
***********************************************************************/
		template<typename R1, typename R2, typename R,typename T0,typename T1,typename T2>
		class Combining<R1(T0,T1,T2), R2(T0,T1,T2), R(R1,R2)> : public Object
		{
		protected:
			Func<R1(T0,T1,T2)>			function1;
			Func<R2(T0,T1,T2)>			function2;
			Func<R(R1,R2)>							converter;
		public:
			typedef R1 FirstFunctionType(T0,T1,T2);
			typedef R2 SecondFunctionType(T0,T1,T2);
			typedef R ConverterFunctionType(R1,R2);
			typedef R FinalFunctionType(T0,T1,T2);
			Combining(const Func<R1(T0,T1,T2)>& _function1, const Func<R2(T0,T1,T2)>& _function2, const Func<R(R1,R2)>& _converter)
				:function1(_function1)
				,function2(_function2)
				,converter(_converter)
			{
			}
			R operator()(T0 p0,T1 p1,T2 p2)const
			{
				return converter(function1(p0,p1,p2), function2(p0,p1,p2));
			}
		}; 
/***********************************************************************
vl::function_combining::Combining<R1(T0,T1,T2,T3), R2(T0,T1,T2,T3), R(R1,R2)>
***********************************************************************/
		template<typename R1, typename R2, typename R,typename T0,typename T1,typename T2,typename T3>
		class Combining<R1(T0,T1,T2,T3), R2(T0,T1,T2,T3), R(R1,R2)> : public Object
		{
		protected:
			Func<R1(T0,T1,T2,T3)>			function1;
			Func<R2(T0,T1,T2,T3)>			function2;
			Func<R(R1,R2)>							converter;
		public:
			typedef R1 FirstFunctionType(T0,T1,T2,T3);
			typedef R2 SecondFunctionType(T0,T1,T2,T3);
			typedef R ConverterFunctionType(R1,R2);
			typedef R FinalFunctionType(T0,T1,T2,T3);
			Combining(const Func<R1(T0,T1,T2,T3)>& _function1, const Func<R2(T0,T1,T2,T3)>& _function2, const Func<R(R1,R2)>& _converter)
				:function1(_function1)
				,function2(_function2)
				,converter(_converter)
			{
			}
			R operator()(T0 p0,T1 p1,T2 p2,T3 p3)const
			{
				return converter(function1(p0,p1,p2,p3), function2(p0,p1,p2,p3));
			}
		}; 
/***********************************************************************
vl::function_combining::Combining<R1(T0,T1,T2,T3,T4), R2(T0,T1,T2,T3,T4), R(R1,R2)>
***********************************************************************/
		template<typename R1, typename R2, typename R,typename T0,typename T1,typename T2,typename T3,typename T4>
		class Combining<R1(T0,T1,T2,T3,T4), R2(T0,T1,T2,T3,T4), R(R1,R2)> : public Object
		{
		protected:
			Func<R1(T0,T1,T2,T3,T4)>			function1;
			Func<R2(T0,T1,T2,T3,T4)>			function2;
			Func<R(R1,R2)>							converter;
		public:
			typedef R1 FirstFunctionType(T0,T1,T2,T3,T4);
			typedef R2 SecondFunctionType(T0,T1,T2,T3,T4);
			typedef R ConverterFunctionType(R1,R2);
			typedef R FinalFunctionType(T0,T1,T2,T3,T4);
			Combining(const Func<R1(T0,T1,T2,T3,T4)>& _function1, const Func<R2(T0,T1,T2,T3,T4)>& _function2, const Func<R(R1,R2)>& _converter)
				:function1(_function1)
				,function2(_function2)
				,converter(_converter)
			{
			}
			R operator()(T0 p0,T1 p1,T2 p2,T3 p3,T4 p4)const
			{
				return converter(function1(p0,p1,p2,p3,p4), function2(p0,p1,p2,p3,p4));
			}
		}; 
/***********************************************************************
vl::function_combining::Combining<R1(T0,T1,T2,T3,T4,T5), R2(T0,T1,T2,T3,T4,T5), R(R1,R2)>
***********************************************************************/
		template<typename R1, typename R2, typename R,typename T0,typename T1,typename T2,typename T3,typename T4,typename T5>
		class Combining<R1(T0,T1,T2,T3,T4,T5), R2(T0,T1,T2,T3,T4,T5), R(R1,R2)> : public Object
		{
		protected:
			Func<R1(T0,T1,T2,T3,T4,T5)>			function1;
			Func<R2(T0,T1,T2,T3,T4,T5)>			function2;
			Func<R(R1,R2)>							converter;
		public:
			typedef R1 FirstFunctionType(T0,T1,T2,T3,T4,T5);
			typedef R2 SecondFunctionType(T0,T1,T2,T3,T4,T5);
			typedef R ConverterFunctionType(R1,R2);
			typedef R FinalFunctionType(T0,T1,T2,T3,T4,T5);
			Combining(const Func<R1(T0,T1,T2,T3,T4,T5)>& _function1, const Func<R2(T0,T1,T2,T3,T4,T5)>& _function2, const Func<R(R1,R2)>& _converter)
				:function1(_function1)
				,function2(_function2)
				,converter(_converter)
			{
			}
			R operator()(T0 p0,T1 p1,T2 p2,T3 p3,T4 p4,T5 p5)const
			{
				return converter(function1(p0,p1,p2,p3,p4,p5), function2(p0,p1,p2,p3,p4,p5));
			}
		}; 
/***********************************************************************
vl::function_combining::Combining<R1(T0,T1,T2,T3,T4,T5,T6), R2(T0,T1,T2,T3,T4,T5,T6), R(R1,R2)>
***********************************************************************/
		template<typename R1, typename R2, typename R,typename T0,typename T1,typename T2,typename T3,typename T4,typename T5,typename T6>
		class Combining<R1(T0,T1,T2,T3,T4,T5,T6), R2(T0,T1,T2,T3,T4,T5,T6), R(R1,R2)> : public Object
		{
		protected:
			Func<R1(T0,T1,T2,T3,T4,T5,T6)>			function1;
			Func<R2(T0,T1,T2,T3,T4,T5,T6)>			function2;
			Func<R(R1,R2)>							converter;
		public:
			typedef R1 FirstFunctionType(T0,T1,T2,T3,T4,T5,T6);
			typedef R2 SecondFunctionType(T0,T1,T2,T3,T4,T5,T6);
			typedef R ConverterFunctionType(R1,R2);
			typedef R FinalFunctionType(T0,T1,T2,T3,T4,T5,T6);
			Combining(const Func<R1(T0,T1,T2,T3,T4,T5,T6)>& _function1, const Func<R2(T0,T1,T2,T3,T4,T5,T6)>& _function2, const Func<R(R1,R2)>& _converter)
				:function1(_function1)
				,function2(_function2)
				,converter(_converter)
			{
			}
			R operator()(T0 p0,T1 p1,T2 p2,T3 p3,T4 p4,T5 p5,T6 p6)const
			{
				return converter(function1(p0,p1,p2,p3,p4,p5,p6), function2(p0,p1,p2,p3,p4,p5,p6));
			}
		}; 
/***********************************************************************
vl::function_combining::Combining<R1(T0,T1,T2,T3,T4,T5,T6,T7), R2(T0,T1,T2,T3,T4,T5,T6,T7), R(R1,R2)>
***********************************************************************/
		template<typename R1, typename R2, typename R,typename T0,typename T1,typename T2,typename T3,typename T4,typename T5,typename T6,typename T7>
		class Combining<R1(T0,T1,T2,T3,T4,T5,T6,T7), R2(T0,T1,T2,T3,T4,T5,T6,T7), R(R1,R2)> : public Object
		{
		protected:
			Func<R1(T0,T1,T2,T3,T4,T5,T6,T7)>			function1;
			Func<R2(T0,T1,T2,T3,T4,T5,T6,T7)>			function2;
			Func<R(R1,R2)>							converter;
		public:
			typedef R1 FirstFunctionType(T0,T1,T2,T3,T4,T5,T6,T7);
			typedef R2 SecondFunctionType(T0,T1,T2,T3,T4,T5,T6,T7);
			typedef R ConverterFunctionType(R1,R2);
			typedef R FinalFunctionType(T0,T1,T2,T3,T4,T5,T6,T7);
			Combining(const Func<R1(T0,T1,T2,T3,T4,T5,T6,T7)>& _function1, const Func<R2(T0,T1,T2,T3,T4,T5,T6,T7)>& _function2, const Func<R(R1,R2)>& _converter)
				:function1(_function1)
				,function2(_function2)
				,converter(_converter)
			{
			}
			R operator()(T0 p0,T1 p1,T2 p2,T3 p3,T4 p4,T5 p5,T6 p6,T7 p7)const
			{
				return converter(function1(p0,p1,p2,p3,p4,p5,p6,p7), function2(p0,p1,p2,p3,p4,p5,p6,p7));
			}
		}; 
/***********************************************************************
vl::function_combining::Combining<R1(T0,T1,T2,T3,T4,T5,T6,T7,T8), R2(T0,T1,T2,T3,T4,T5,T6,T7,T8), R(R1,R2)>
***********************************************************************/
		template<typename R1, typename R2, typename R,typename T0,typename T1,typename T2,typename T3,typename T4,typename T5,typename T6,typename T7,typename T8>
		class Combining<R1(T0,T1,T2,T3,T4,T5,T6,T7,T8), R2(T0,T1,T2,T3,T4,T5,T6,T7,T8), R(R1,R2)> : public Object
		{
		protected:
			Func<R1(T0,T1,T2,T3,T4,T5,T6,T7,T8)>			function1;
			Func<R2(T0,T1,T2,T3,T4,T5,T6,T7,T8)>			function2;
			Func<R(R1,R2)>							converter;
		public:
			typedef R1 FirstFunctionType(T0,T1,T2,T3,T4,T5,T6,T7,T8);
			typedef R2 SecondFunctionType(T0,T1,T2,T3,T4,T5,T6,T7,T8);
			typedef R ConverterFunctionType(R1,R2);
			typedef R FinalFunctionType(T0,T1,T2,T3,T4,T5,T6,T7,T8);
			Combining(const Func<R1(T0,T1,T2,T3,T4,T5,T6,T7,T8)>& _function1, const Func<R2(T0,T1,T2,T3,T4,T5,T6,T7,T8)>& _function2, const Func<R(R1,R2)>& _converter)
				:function1(_function1)
				,function2(_function2)
				,converter(_converter)
			{
			}
			R operator()(T0 p0,T1 p1,T2 p2,T3 p3,T4 p4,T5 p5,T6 p6,T7 p7,T8 p8)const
			{
				return converter(function1(p0,p1,p2,p3,p4,p5,p6,p7,p8), function2(p0,p1,p2,p3,p4,p5,p6,p7,p8));
			}
		}; 
/***********************************************************************
vl::function_combining::Combining<R1(T0,T1,T2,T3,T4,T5,T6,T7,T8,T9), R2(T0,T1,T2,T3,T4,T5,T6,T7,T8,T9), R(R1,R2)>
***********************************************************************/
		template<typename R1, typename R2, typename R,typename T0,typename T1,typename T2,typename T3,typename T4,typename T5,typename T6,typename T7,typename T8,typename T9>
		class Combining<R1(T0,T1,T2,T3,T4,T5,T6,T7,T8,T9), R2(T0,T1,T2,T3,T4,T5,T6,T7,T8,T9), R(R1,R2)> : public Object
		{
		protected:
			Func<R1(T0,T1,T2,T3,T4,T5,T6,T7,T8,T9)>			function1;
			Func<R2(T0,T1,T2,T3,T4,T5,T6,T7,T8,T9)>			function2;
			Func<R(R1,R2)>							converter;
		public:
			typedef R1 FirstFunctionType(T0,T1,T2,T3,T4,T5,T6,T7,T8,T9);
			typedef R2 SecondFunctionType(T0,T1,T2,T3,T4,T5,T6,T7,T8,T9);
			typedef R ConverterFunctionType(R1,R2);
			typedef R FinalFunctionType(T0,T1,T2,T3,T4,T5,T6,T7,T8,T9);
			Combining(const Func<R1(T0,T1,T2,T3,T4,T5,T6,T7,T8,T9)>& _function1, const Func<R2(T0,T1,T2,T3,T4,T5,T6,T7,T8,T9)>& _function2, const Func<R(R1,R2)>& _converter)
				:function1(_function1)
				,function2(_function2)
				,converter(_converter)
			{
			}
			R operator()(T0 p0,T1 p1,T2 p2,T3 p3,T4 p4,T5 p5,T6 p6,T7 p7,T8 p8,T9 p9)const
			{
				return converter(function1(p0,p1,p2,p3,p4,p5,p6,p7,p8,p9), function2(p0,p1,p2,p3,p4,p5,p6,p7,p8,p9));
			}
		};
	}
	template<typename F1, typename F2, typename C>
	Func<typename function_combining::Combining<F1, F2, C>::FinalFunctionType>
	Combine(Func<C> converter, Func<F1> function1, Func<F2> function2)
	{
		return Ptr<function_combining::Combining<F1, F2, C>>(new function_combining::Combining<F1, F2, C>(function1, function2, converter));
	}
	template<typename T>
	Func<Func<T>(Func<T>,Func<T>)> Combiner(const Func<typename Func<T>::ResultType(typename Func<T>::ResultType,typename Func<T>::ResultType)>& converter)
	{
		typedef typename Func<T>::ResultType R;
		return Curry<Func<T>(Func<R(R,R)>,Func<T>,Func<T>)>(Combine)(converter);
	}
}
#endif