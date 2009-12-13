#include "..\..\Library\UnitTest\UnitTest.h"
#include "..\..\Library\Function.h"
#include "..\..\Library\Event.h"
#include "..\..\Library\Collections\List.h"

using namespace vl;
using namespace vl::unittest;
using namespace vl::collections;

int Add1(int a, int b)
{
	return a+b;
};

class Add2
{
public:
	int operator()(int a, int b)
	{
		return a+b;
	}
};

void Inc1(int& a)
{
	a++;
}

class Inc2
{
public:
	void operator()(int& a)
	{
		a++;
	}
};

TEST_CASE(TestFunction)
{
	Add2 add;
	Func<int(int,int)> add1=Add1;
	Func<int(int,int)> add2(&add,&Add2::operator());
	Func<int(int,int)> add3=&add;
	Func<int(int,int)> add4=Ptr<Add2>(new Add2);
	Func<int(int,int)> add5=add;

	TEST_ASSERT(add1(1,2)==3);
	TEST_ASSERT(add2(1,2)==3);
	TEST_ASSERT(add3(1,2)==3);
	TEST_ASSERT(add4(1,2)==3);
	TEST_ASSERT(add5(1,2)==3);

	Func<int(int,int)> add6;
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

	List<Func<int(int,int)>> adds;
	adds.Add(add1);
	adds.Add(add2);
	adds.Add(add3);
	adds.Add(add4);
	adds.Add(add5);
	for(int i=0;i<adds.Count();i++)
	{
		TEST_ASSERT(adds[i](1,2)==3);
	}
	
	Inc2 inc;
	Func<void(int&)> inc1=Inc1;
	Func<void(int&)> inc2(&inc,&Inc2::operator());
	Func<void(int&)> inc3=&inc;
	Func<void(int&)> inc4=Ptr<Inc2>(new Inc2);
	Func<void(int&)> inc5=inc;

	int a=0;
	inc1(a);
	inc2(a);
	inc3(a);
	inc4(a);
	inc5(a);
	TEST_ASSERT(a==5);
}

int Add3(int a, int b, int c)
{
	return a+b+c;
}

void Add3r(int a, int b, int c, int& r)
{
	r=a+b+c;
}

TEST_CASE(TestCurrying)
{
	Func<int(int,int,int)> add=Add3;
	Func<int(int,int)> add_1=Curry(add)(1);
	Func<int(int)> add_1_2=Curry(add_1)(2);
	Func<int()> add_1_2_3=Curry(add_1_2)(3);

	TEST_ASSERT(add(1,2,3)==6);
	TEST_ASSERT(add_1(2,3)==6);
	TEST_ASSERT(add_1_2(3)==6);
	TEST_ASSERT(add_1_2_3()==6);

	int r=0;
	Func<void(int,int,int,int&)> addr=Add3r;
	Func<void(int,int,int&)> addr_1=Curry(addr)(1);
	Func<void(int,int&)> addr_1_2=Curry(addr_1)(2);
	Func<void(int&)> addr_1_2_3=Curry(addr_1_2)(3);
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

int MyAdd(int a, int b)
{
	return a+b;
}

int MyMul(int a, int b)
{
	return a*b;
}

int MySub(int a, int b)
{
	return a-b;
}

TEST_CASE(TestCombining)
{
	Func<int(int,int)> add=MyAdd;
	Func<int(int,int)> mul=MyMul;
	Func<int(int,int)> sub=MySub;
	Func<int(int,int)> func=Combine(sub, add, mul);

	TEST_ASSERT(add(3,4)==7);
	TEST_ASSERT(mul(3,4)==12);
	TEST_ASSERT(sub(3,4)==-1);
	TEST_ASSERT(func(3,4)==-5);
}

void EventHandler1(int& a)
{
	a+=1;
}

void EventHandler2(int& a)
{
	a+=2;
}

TEST_CASE(TestEvent)
{
	int a=0;
	Event<void(int&)> e;

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