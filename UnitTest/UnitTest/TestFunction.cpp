#include "..\..\Library\UnitTest\UnitTest.h"
#include "..\..\Library\Function.h"
#include "..\..\Library\Event.h"
#include "..\..\Library\Collections\List.h"

using namespace vl;
using namespace vl::unittest;
using namespace vl::collections;

vint Add1(vint a, vint b)
{
	return a+b;
};

class Add2
{
public:
	vint operator()(vint a, vint b)
	{
		return a+b;
	}
};

void Inc1(vint& a)
{
	a++;
}

class Inc2
{
public:
	void operator()(vint& a)
	{
		a++;
	}
};

TEST_CASE(TestFunction)
{
	Add2 add;
	Func<vint(vint,vint)> add1=Add1;
	Func<vint(vint,vint)> add2(&add,&Add2::operator());
	Func<vint(vint,vint)> add3=&add;
	Func<vint(vint,vint)> add4=Ptr<Add2>(new Add2);
	Func<vint(vint,vint)> add5=add;

	TEST_ASSERT(add1(1,2)==3);
	TEST_ASSERT(add2(1,2)==3);
	TEST_ASSERT(add3(1,2)==3);
	TEST_ASSERT(add4(1,2)==3);
	TEST_ASSERT(add5(1,2)==3);

	Func<vint(vint,vint)> add6;
	add6=add1;
	TEST_ASSERT(add6(1,2)==3);
	add6=add2;
	TEST_ASSERT(add6(1,2)==3);
	add6=add3;
	TEST_ASSERT(add6(1,2)==3);
	add6=add4;
	TEST_ASSERT(add6(1,2)==3);
	add6=add5;
	TEST_ASSERT(add6(1,2)==3);

	List<Func<vint(vint,vint)>> adds;
	adds.Add(add1);
	adds.Add(add2);
	adds.Add(add3);
	adds.Add(add4);
	adds.Add(add5);
	for(vint i=0;i<adds.Count();i++)
	{
		TEST_ASSERT(adds[i](1,2)==3);
	}
	
	Inc2 inc;
	Func<void(vint&)> inc1=Inc1;
	Func<void(vint&)> inc2(&inc,&Inc2::operator());
	Func<void(vint&)> inc3=&inc;
	Func<void(vint&)> inc4=Ptr<Inc2>(new Inc2);
	Func<void(vint&)> inc5=inc;

	vint a=0;
	inc1(a);
	inc2(a);
	inc3(a);
	inc4(a);
	inc5(a);
	TEST_ASSERT(a==5);
}

vint Add3(vint a, vint b, vint c)
{
	return a+b+c;
}

void Add3r(vint a, vint b, vint c, vint& r)
{
	r=a+b+c;
}

TEST_CASE(TestCurrying)
{
	Func<vint(vint,vint,vint)> add=Add3;
	Func<vint(vint,vint)> add_1=Curry(add)(1);
	Func<vint(vint)> add_1_2=Curry(add_1)(2);
	Func<vint()> add_1_2_3=Curry(add_1_2)(3);

	TEST_ASSERT(add(1,2,3)==6);
	TEST_ASSERT(add_1(2,3)==6);
	TEST_ASSERT(add_1_2(3)==6);
	TEST_ASSERT(add_1_2_3()==6);

	vint r=0;
	Func<void(vint,vint,vint,vint&)> addr=Add3r;
	Func<void(vint,vint,vint&)> addr_1=Curry(addr)(1);
	Func<void(vint,vint&)> addr_1_2=Curry(addr_1)(2);
	Func<void(vint&)> addr_1_2_3=Curry(addr_1_2)(3);
	Func<void()> addr_1_2_3_4=Curry(addr_1_2_3)(r);

	r=0;
	addr(1,2,3,r);
	TEST_ASSERT(r==6);
	
	r=0;
	addr_1(2,3,r);
	TEST_ASSERT(r==6);

	r=0;
	addr_1_2(3,r);
	TEST_ASSERT(r==6);

	r=0;
	addr_1_2_3(r);
	TEST_ASSERT(r==6);

	r=0;
	addr_1_2_3_4();
	TEST_ASSERT(r==6);
}

vint MyAdd(vint a, vint b)
{
	return a+b;
}

vint MyMul(vint a, vint b)
{
	return a*b;
}

vint MySub(vint a, vint b)
{
	return a-b;
}

TEST_CASE(TestCombining)
{
	Func<vint(vint,vint)> add=MyAdd;
	Func<vint(vint,vint)> mul=MyMul;
	Func<vint(vint,vint)> sub=MySub;
	Func<vint(vint,vint)> func=Combine(sub, add, mul);

	TEST_ASSERT(add(3,4)==7);
	TEST_ASSERT(mul(3,4)==12);
	TEST_ASSERT(sub(3,4)==-1);
	TEST_ASSERT(func(3,4)==-5);
}

void EventHandler1(vint& a)
{
	a+=1;
}

void EventHandler2(vint& a)
{
	a+=2;
}

TEST_CASE(TestEvent)
{
	vint a=0;
	Event<void(vint&)> e;

	e.Add(EventHandler1);
	e.Add(EventHandler1);
	e(a);
	TEST_ASSERT(a==2);

	e.Add(EventHandler2);
	e.Add(EventHandler2);
	e(a);
	TEST_ASSERT(a==8);

	e.Remove(EventHandler1);
	e.Remove(EventHandler1);
	e(a);
	TEST_ASSERT(a==12);

	e.Remove(EventHandler2);
	e.Remove(EventHandler2);
	e(a);
	TEST_ASSERT(a==12);
}