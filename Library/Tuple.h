/***********************************************************************
Vczh Library++ 3.0
Developer: 陈梓瀚(vczh)
Framework::Tuple
	
本文件使用Vczh Functional Macro工具自动生成
***********************************************************************/
#ifndef VCZH_EVENT
#define VCZH_EVENT
namespace vl
{
	class TupleNullItem
	{
	};
	template<typename T0 = TupleNullItem,typename T1 = TupleNullItem,typename T2 = TupleNullItem,typename T3 = TupleNullItem,typename T4 = TupleNullItem,typename T5 = TupleNullItem,typename T6 = TupleNullItem,typename T7 = TupleNullItem,typename T8 = TupleNullItem,typename T9 = TupleNullItem,typename T10 = TupleNullItem>
	class Tuple
	{
	};
 
/***********************************************************************
vl::Tuple<T0>
***********************************************************************/
	template<typename T0>
	class Tuple<T0> : public Object
	{
	public:
		T0 f0;
	};
  
/***********************************************************************
vl::Tuple<T0,T1>
***********************************************************************/
	template<typename T0,typename T1>
	class Tuple<T0,T1> : public Object
	{
	public:
		T0 f0;T1 f1;
	};
  
/***********************************************************************
vl::Tuple<T0,T1,T2>
***********************************************************************/
	template<typename T0,typename T1,typename T2>
	class Tuple<T0,T1,T2> : public Object
	{
	public:
		T0 f0;T1 f1;T2 f2;
	};
  
/***********************************************************************
vl::Tuple<T0,T1,T2,T3>
***********************************************************************/
	template<typename T0,typename T1,typename T2,typename T3>
	class Tuple<T0,T1,T2,T3> : public Object
	{
	public:
		T0 f0;T1 f1;T2 f2;T3 f3;
	};
  
/***********************************************************************
vl::Tuple<T0,T1,T2,T3,T4>
***********************************************************************/
	template<typename T0,typename T1,typename T2,typename T3,typename T4>
	class Tuple<T0,T1,T2,T3,T4> : public Object
	{
	public:
		T0 f0;T1 f1;T2 f2;T3 f3;T4 f4;
	};
  
/***********************************************************************
vl::Tuple<T0,T1,T2,T3,T4,T5>
***********************************************************************/
	template<typename T0,typename T1,typename T2,typename T3,typename T4,typename T5>
	class Tuple<T0,T1,T2,T3,T4,T5> : public Object
	{
	public:
		T0 f0;T1 f1;T2 f2;T3 f3;T4 f4;T5 f5;
	};
  
/***********************************************************************
vl::Tuple<T0,T1,T2,T3,T4,T5,T6>
***********************************************************************/
	template<typename T0,typename T1,typename T2,typename T3,typename T4,typename T5,typename T6>
	class Tuple<T0,T1,T2,T3,T4,T5,T6> : public Object
	{
	public:
		T0 f0;T1 f1;T2 f2;T3 f3;T4 f4;T5 f5;T6 f6;
	};
  
/***********************************************************************
vl::Tuple<T0,T1,T2,T3,T4,T5,T6,T7>
***********************************************************************/
	template<typename T0,typename T1,typename T2,typename T3,typename T4,typename T5,typename T6,typename T7>
	class Tuple<T0,T1,T2,T3,T4,T5,T6,T7> : public Object
	{
	public:
		T0 f0;T1 f1;T2 f2;T3 f3;T4 f4;T5 f5;T6 f6;T7 f7;
	};
  
/***********************************************************************
vl::Tuple<T0,T1,T2,T3,T4,T5,T6,T7,T8>
***********************************************************************/
	template<typename T0,typename T1,typename T2,typename T3,typename T4,typename T5,typename T6,typename T7,typename T8>
	class Tuple<T0,T1,T2,T3,T4,T5,T6,T7,T8> : public Object
	{
	public:
		T0 f0;T1 f1;T2 f2;T3 f3;T4 f4;T5 f5;T6 f6;T7 f7;T8 f8;
	};
  
/***********************************************************************
vl::Tuple<T0,T1,T2,T3,T4,T5,T6,T7,T8,T9>
***********************************************************************/
	template<typename T0,typename T1,typename T2,typename T3,typename T4,typename T5,typename T6,typename T7,typename T8,typename T9>
	class Tuple<T0,T1,T2,T3,T4,T5,T6,T7,T8,T9> : public Object
	{
	public:
		T0 f0;T1 f1;T2 f2;T3 f3;T4 f4;T5 f5;T6 f6;T7 f7;T8 f8;T9 f9;
	};
 
}
#endif