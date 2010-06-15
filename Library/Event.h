/***********************************************************************
Vczh Library++ 3.0
Developer: 陈梓瀚(vczh)
Framework::Event

Classes:
	Event<function-type>									：事件对象
	
本文件使用Vczh Functional Macro工具自动生成
***********************************************************************/
#ifndef VCZH_EVENT
#define VCZH_EVENT
#include "Function.h"
#include "Collections\List.h"
namespace vl
{
	template<typename T>
	class Event
	{
	};
 
/***********************************************************************
vl::Event<void()>
***********************************************************************/
	template< >
	class Event<void()> : public Object, private NotCopyable
	{
	protected:
		collections::SortedList<Func<void()>>	functions;
	public:
		void Add(const Func<void()>& handler)
		{
			functions.Add(handler);
		}
		
		void Remove(const Func<void()>& handler)
		{
			functions.Remove(handler);
		}
		void operator()()const
		{
			for(vint i=0;i<functions.Count();i++)
			{
				functions.Get(i)();
			}
		}
	};
  
/***********************************************************************
vl::Event<void(T0)>
***********************************************************************/
	template< typename T0>
	class Event<void(T0)> : public Object, private NotCopyable
	{
	protected:
		collections::SortedList<Func<void(T0)>>	functions;
	public:
		void Add(const Func<void(T0)>& handler)
		{
			functions.Add(handler);
		}
		
		void Remove(const Func<void(T0)>& handler)
		{
			functions.Remove(handler);
		}
		void operator()(T0 p0)const
		{
			for(vint i=0;i<functions.Count();i++)
			{
				functions.Get(i)(p0);
			}
		}
	};
  
/***********************************************************************
vl::Event<void(T0,T1)>
***********************************************************************/
	template< typename T0,typename T1>
	class Event<void(T0,T1)> : public Object, private NotCopyable
	{
	protected:
		collections::SortedList<Func<void(T0,T1)>>	functions;
	public:
		void Add(const Func<void(T0,T1)>& handler)
		{
			functions.Add(handler);
		}
		
		void Remove(const Func<void(T0,T1)>& handler)
		{
			functions.Remove(handler);
		}
		void operator()(T0 p0,T1 p1)const
		{
			for(vint i=0;i<functions.Count();i++)
			{
				functions.Get(i)(p0,p1);
			}
		}
	};
  
/***********************************************************************
vl::Event<void(T0,T1,T2)>
***********************************************************************/
	template< typename T0,typename T1,typename T2>
	class Event<void(T0,T1,T2)> : public Object, private NotCopyable
	{
	protected:
		collections::SortedList<Func<void(T0,T1,T2)>>	functions;
	public:
		void Add(const Func<void(T0,T1,T2)>& handler)
		{
			functions.Add(handler);
		}
		
		void Remove(const Func<void(T0,T1,T2)>& handler)
		{
			functions.Remove(handler);
		}
		void operator()(T0 p0,T1 p1,T2 p2)const
		{
			for(vint i=0;i<functions.Count();i++)
			{
				functions.Get(i)(p0,p1,p2);
			}
		}
	};
  
/***********************************************************************
vl::Event<void(T0,T1,T2,T3)>
***********************************************************************/
	template< typename T0,typename T1,typename T2,typename T3>
	class Event<void(T0,T1,T2,T3)> : public Object, private NotCopyable
	{
	protected:
		collections::SortedList<Func<void(T0,T1,T2,T3)>>	functions;
	public:
		void Add(const Func<void(T0,T1,T2,T3)>& handler)
		{
			functions.Add(handler);
		}
		
		void Remove(const Func<void(T0,T1,T2,T3)>& handler)
		{
			functions.Remove(handler);
		}
		void operator()(T0 p0,T1 p1,T2 p2,T3 p3)const
		{
			for(vint i=0;i<functions.Count();i++)
			{
				functions.Get(i)(p0,p1,p2,p3);
			}
		}
	};
  
/***********************************************************************
vl::Event<void(T0,T1,T2,T3,T4)>
***********************************************************************/
	template< typename T0,typename T1,typename T2,typename T3,typename T4>
	class Event<void(T0,T1,T2,T3,T4)> : public Object, private NotCopyable
	{
	protected:
		collections::SortedList<Func<void(T0,T1,T2,T3,T4)>>	functions;
	public:
		void Add(const Func<void(T0,T1,T2,T3,T4)>& handler)
		{
			functions.Add(handler);
		}
		
		void Remove(const Func<void(T0,T1,T2,T3,T4)>& handler)
		{
			functions.Remove(handler);
		}
		void operator()(T0 p0,T1 p1,T2 p2,T3 p3,T4 p4)const
		{
			for(vint i=0;i<functions.Count();i++)
			{
				functions.Get(i)(p0,p1,p2,p3,p4);
			}
		}
	};
  
/***********************************************************************
vl::Event<void(T0,T1,T2,T3,T4,T5)>
***********************************************************************/
	template< typename T0,typename T1,typename T2,typename T3,typename T4,typename T5>
	class Event<void(T0,T1,T2,T3,T4,T5)> : public Object, private NotCopyable
	{
	protected:
		collections::SortedList<Func<void(T0,T1,T2,T3,T4,T5)>>	functions;
	public:
		void Add(const Func<void(T0,T1,T2,T3,T4,T5)>& handler)
		{
			functions.Add(handler);
		}
		
		void Remove(const Func<void(T0,T1,T2,T3,T4,T5)>& handler)
		{
			functions.Remove(handler);
		}
		void operator()(T0 p0,T1 p1,T2 p2,T3 p3,T4 p4,T5 p5)const
		{
			for(vint i=0;i<functions.Count();i++)
			{
				functions.Get(i)(p0,p1,p2,p3,p4,p5);
			}
		}
	};
  
/***********************************************************************
vl::Event<void(T0,T1,T2,T3,T4,T5,T6)>
***********************************************************************/
	template< typename T0,typename T1,typename T2,typename T3,typename T4,typename T5,typename T6>
	class Event<void(T0,T1,T2,T3,T4,T5,T6)> : public Object, private NotCopyable
	{
	protected:
		collections::SortedList<Func<void(T0,T1,T2,T3,T4,T5,T6)>>	functions;
	public:
		void Add(const Func<void(T0,T1,T2,T3,T4,T5,T6)>& handler)
		{
			functions.Add(handler);
		}
		
		void Remove(const Func<void(T0,T1,T2,T3,T4,T5,T6)>& handler)
		{
			functions.Remove(handler);
		}
		void operator()(T0 p0,T1 p1,T2 p2,T3 p3,T4 p4,T5 p5,T6 p6)const
		{
			for(vint i=0;i<functions.Count();i++)
			{
				functions.Get(i)(p0,p1,p2,p3,p4,p5,p6);
			}
		}
	};
  
/***********************************************************************
vl::Event<void(T0,T1,T2,T3,T4,T5,T6,T7)>
***********************************************************************/
	template< typename T0,typename T1,typename T2,typename T3,typename T4,typename T5,typename T6,typename T7>
	class Event<void(T0,T1,T2,T3,T4,T5,T6,T7)> : public Object, private NotCopyable
	{
	protected:
		collections::SortedList<Func<void(T0,T1,T2,T3,T4,T5,T6,T7)>>	functions;
	public:
		void Add(const Func<void(T0,T1,T2,T3,T4,T5,T6,T7)>& handler)
		{
			functions.Add(handler);
		}
		
		void Remove(const Func<void(T0,T1,T2,T3,T4,T5,T6,T7)>& handler)
		{
			functions.Remove(handler);
		}
		void operator()(T0 p0,T1 p1,T2 p2,T3 p3,T4 p4,T5 p5,T6 p6,T7 p7)const
		{
			for(vint i=0;i<functions.Count();i++)
			{
				functions.Get(i)(p0,p1,p2,p3,p4,p5,p6,p7);
			}
		}
	};
  
/***********************************************************************
vl::Event<void(T0,T1,T2,T3,T4,T5,T6,T7,T8)>
***********************************************************************/
	template< typename T0,typename T1,typename T2,typename T3,typename T4,typename T5,typename T6,typename T7,typename T8>
	class Event<void(T0,T1,T2,T3,T4,T5,T6,T7,T8)> : public Object, private NotCopyable
	{
	protected:
		collections::SortedList<Func<void(T0,T1,T2,T3,T4,T5,T6,T7,T8)>>	functions;
	public:
		void Add(const Func<void(T0,T1,T2,T3,T4,T5,T6,T7,T8)>& handler)
		{
			functions.Add(handler);
		}
		
		void Remove(const Func<void(T0,T1,T2,T3,T4,T5,T6,T7,T8)>& handler)
		{
			functions.Remove(handler);
		}
		void operator()(T0 p0,T1 p1,T2 p2,T3 p3,T4 p4,T5 p5,T6 p6,T7 p7,T8 p8)const
		{
			for(vint i=0;i<functions.Count();i++)
			{
				functions.Get(i)(p0,p1,p2,p3,p4,p5,p6,p7,p8);
			}
		}
	};
  
/***********************************************************************
vl::Event<void(T0,T1,T2,T3,T4,T5,T6,T7,T8,T9)>
***********************************************************************/
	template< typename T0,typename T1,typename T2,typename T3,typename T4,typename T5,typename T6,typename T7,typename T8,typename T9>
	class Event<void(T0,T1,T2,T3,T4,T5,T6,T7,T8,T9)> : public Object, private NotCopyable
	{
	protected:
		collections::SortedList<Func<void(T0,T1,T2,T3,T4,T5,T6,T7,T8,T9)>>	functions;
	public:
		void Add(const Func<void(T0,T1,T2,T3,T4,T5,T6,T7,T8,T9)>& handler)
		{
			functions.Add(handler);
		}
		
		void Remove(const Func<void(T0,T1,T2,T3,T4,T5,T6,T7,T8,T9)>& handler)
		{
			functions.Remove(handler);
		}
		void operator()(T0 p0,T1 p1,T2 p2,T3 p3,T4 p4,T5 p5,T6 p6,T7 p7,T8 p8,T9 p9)const
		{
			for(vint i=0;i<functions.Count();i++)
			{
				functions.Get(i)(p0,p1,p2,p3,p4,p5,p6,p7,p8,p9);
			}
		}
	};
 
}
#endif