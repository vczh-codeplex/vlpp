#include "..\..\Library\UnitTest\UnitTest.h"
#include "..\..\Library\ObjectModel\ObjectModelIncludes.h"

using namespace vl;
using namespace vl::collections;
using namespace vl::objectmodel;

template<typename T>
void TestObjectTypeNumeric(const WString& name)
{
	ObjectType* type=TypeOf<T>();
	TEST_ASSERT(type==GetObjectType(name));
	TEST_ASSERT(type->Name()==name);
	TEST_ASSERT(type->Size()==sizeof(T));
	TEST_ASSERT(type->Category()==ObjectType::Numeric);
	TEST_ASSERT(type->IsPOD()==true);
}

#define TEST_NUMERIC(TYPE) TestObjectTypeNumeric<TYPE>(L#TYPE)

TEST_CASE(TestNumericType)
{
	EnsureObjectTypeInitialized();
	TEST_NUMERIC(signed __int8);
	TEST_NUMERIC(unsigned __int8);
	TEST_NUMERIC(signed __int16);
	TEST_NUMERIC(unsigned __int16);
	TEST_NUMERIC(signed __int32);
	TEST_NUMERIC(unsigned __int32);
	TEST_NUMERIC(signed __int64);
	TEST_NUMERIC(unsigned __int64);
	TEST_NUMERIC(char);
	TEST_NUMERIC(wchar_t);
	TEST_NUMERIC(bool);
}

TEST_CASE(TestDecoratorType)
{
	EnsureObjectTypeInitialized();

	ObjectType* cint=TypeOf<const int>();
	TEST_ASSERT(cint==GetObjectType(L"const(signed __int32)"));
	TEST_ASSERT(cint->Name()==L"const(signed __int32)");
	TEST_ASSERT(cint->Size()==sizeof(const int*));
	TEST_ASSERT(cint->Category()==ObjectType::Decorator);
	TEST_ASSERT(cint->IsPOD()==true);
	TEST_ASSERT(cint->ElementType()==TypeOf<int>());
	TEST_ASSERT(cint->IsConstDecorator()==true);
	TEST_ASSERT(cint->IsVolatileDecorator()==false);

	ObjectType* vint=TypeOf<volatile int>();
	TEST_ASSERT(vint==GetObjectType(L"volatile(signed __int32)"));
	TEST_ASSERT(vint->Name()==L"volatile(signed __int32)");
	TEST_ASSERT(vint->Size()==sizeof(volatile int*));
	TEST_ASSERT(vint->Category()==ObjectType::Decorator);
	TEST_ASSERT(vint->IsPOD()==true);
	TEST_ASSERT(vint->ElementType()==TypeOf<int>());
	TEST_ASSERT(vint->IsConstDecorator()==false);
	TEST_ASSERT(vint->IsVolatileDecorator()==true);

	ObjectType* cvint=TypeOf<const volatile int>();
	TEST_ASSERT(cvint==GetObjectType(L"const-volatile(signed __int32)"));
	TEST_ASSERT(cvint->Name()==L"const-volatile(signed __int32)");
	TEST_ASSERT(cvint->Size()==sizeof(const volatile int*));
	TEST_ASSERT(cvint->Category()==ObjectType::Decorator);
	TEST_ASSERT(cvint->IsPOD()==true);
	TEST_ASSERT(cvint->ElementType()==TypeOf<int>());
	TEST_ASSERT(cvint->IsConstDecorator()==true);
	TEST_ASSERT(cvint->IsVolatileDecorator()==true);
}

TEST_CASE(TestPointerType)
{
	EnsureObjectTypeInitialized();

	ObjectType* pint=TypeOf<int*>();
	TEST_ASSERT(pint==GetObjectType(L"signed __int32*"));
	TEST_ASSERT(pint->Name()==L"signed __int32*");
	TEST_ASSERT(pint->Size()==sizeof(int*));
	TEST_ASSERT(pint->Category()==ObjectType::Pointer);
	TEST_ASSERT(pint->IsPOD()==true);
	TEST_ASSERT(pint->ElementType()==TypeOf<int>());

	ObjectType* ppint=TypeOf<int**>();
	TEST_ASSERT(ppint==GetObjectType(L"signed __int32**"));
	TEST_ASSERT(ppint->Name()==L"signed __int32**");
	TEST_ASSERT(ppint->Size()==sizeof(int**));
	TEST_ASSERT(ppint->Category()==ObjectType::Pointer);
	TEST_ASSERT(ppint->IsPOD()==true);
	TEST_ASSERT(ppint->ElementType()==TypeOf<int*>());
}

TEST_CASE(TestReferenceType)
{
	EnsureObjectTypeInitialized();

	ObjectType* rint=TypeOf<int&>();
	TEST_ASSERT(rint==GetObjectType(L"signed __int32&"));
	TEST_ASSERT(rint->Name()==L"signed __int32&");
	TEST_ASSERT(rint->Size()==sizeof(int*));
	TEST_ASSERT(rint->Category()==ObjectType::Reference);
	TEST_ASSERT(rint->IsPOD()==true);
	TEST_ASSERT(rint->ElementType()==TypeOf<int>());
}

TEST_CASE(TestArrayType)
{
	EnsureObjectTypeInitialized();

	ObjectType* pints=TypeOf<int*[10]>();
	TEST_ASSERT(pints==GetObjectType(L"signed __int32*[10]"));
	TEST_ASSERT(pints->Name()==L"signed __int32*[10]");
	TEST_ASSERT(pints->Size()==sizeof(int*[10]));
	TEST_ASSERT(pints->Category()==ObjectType::Array);
	TEST_ASSERT(pints->IsPOD()==true);
	TEST_ASSERT(pints->ElementType()==TypeOf<int*>());
	TEST_ASSERT(pints->ArraySize()==10);
}

namespace mynamespace
{
	enum MyEnum
	{
		Member1=1,
		Member2=2,
		Member3=4,
	};
}

namespace vl
{
	template<>
	struct POD<mynamespace::MyEnum>
	{
		static const bool Result=true;
	};
}

BEGIN_ENUM_TYPE(mynamespace::MyEnum)
	ENUM_MEMBER(mynamespace::Member1)
	ENUM_MEMBER(mynamespace::Member2)
	ENUM_MEMBER(mynamespace::Member3)
END_ENUM_TYPE

TEST_CASE(TestEnumType)
{
	using namespace mynamespace;
	EnsureObjectTypeInitialized();

	ObjectType* enumeration=TypeOf<MyEnum>();
	TEST_ASSERT(enumeration==GetObjectType(L"mynamespace::MyEnum"));
	TEST_ASSERT(enumeration->Name()==L"mynamespace::MyEnum");
	TEST_ASSERT(enumeration->Size()==sizeof(MyEnum));
	TEST_ASSERT(enumeration->Category()==ObjectType::Enum);
	TEST_ASSERT(enumeration->IsPOD()==true);

	const IReadonlyDictionary<WString, ObjectMember*>& members=enumeration->EnumMembers();
	TEST_ASSERT(members.Count()==3);

	TEST_ASSERT(members.Keys()[0]==L"mynamespace::Member1");
	TEST_ASSERT(members.Values()[0]->Type()==TypeOf<MyEnum>());
	TEST_ASSERT(members.Values()[0]->Name()==L"mynamespace::Member1");
	TEST_ASSERT(members.Values()[0]->EnumValue()==1);

	TEST_ASSERT(members.Keys()[1]==L"mynamespace::Member2");
	TEST_ASSERT(members.Values()[1]->Type()==TypeOf<MyEnum>());
	TEST_ASSERT(members.Values()[1]->Name()==L"mynamespace::Member2");
	TEST_ASSERT(members.Values()[1]->EnumValue()==2);

	TEST_ASSERT(members.Keys()[2]==L"mynamespace::Member3");
	TEST_ASSERT(members.Values()[2]->Type()==TypeOf<MyEnum>());
	TEST_ASSERT(members.Values()[2]->Name()==L"mynamespace::Member3");
	TEST_ASSERT(members.Values()[2]->EnumValue()==4);
}

TEST_CASE(TestFunctionType)
{
	EnsureObjectTypeInitialized();

	ObjectType* pf1=TypeOf<int(*)(void*,double)>();
	TEST_ASSERT(pf1==GetObjectType(L"signed __int32(*)(void*,double)"));
	TEST_ASSERT(pf1->Name()==L"signed __int32(*)(void*,double)");
	TEST_ASSERT(pf1->Size()==sizeof(int(*)(void*,double)));
	TEST_ASSERT(pf1->Category()==ObjectType::Function);
	TEST_ASSERT(pf1->IsPOD()==true);

	TEST_ASSERT(pf1->ReturnType()==TypeOf<int>());
	TEST_ASSERT(pf1->ThisType()==0);
	TEST_ASSERT(pf1->IsConstFunction()==false);
	TEST_ASSERT(pf1->ParameterTypes().Count()==2);
	TEST_ASSERT(pf1->ParameterTypes()[0]==TypeOf<void*>());
	TEST_ASSERT(pf1->ParameterTypes()[1]==TypeOf<double>());
}

namespace mynamespace
{
	class MyBase : public Object
	{
	public:
		int field1;
		double field2;

		void Method1(int)const{}
		int Method2(int*,double*){return 0;}
	};

	class MyDerived : public MyBase
	{
	public:
		int* field3;
		double* field4;

		void Method3(int)const{}
		int Method3(int*,double*){return 0;}
	};

	template<typename F1, typename F2>
	class MyTemplate
	{
	public:
		F1 field1;
		F2 field2;
	};
}

BEGIN_CLASS_TYPE(mynamespace::MyBase)
	CLASS_CONSTRUCTOR_0
	CLASS_FIELD(field1)
	CLASS_FIELD(field2)
	CLASS_METHOD(Method1)
	CLASS_METHOD(Method2)
END_CLASS_TYPE

BEGIN_CLASS_TYPE(mynamespace::MyDerived)
	CLASS_BASE(mynamespace::MyBase)
	CLASS_CONSTRUCTOR_0
	CLASS_FIELD(field3)
	CLASS_FIELD(field4)
	CLASS_METHOD_OVERLOAD(Method3, void(mynamespace::MyDerived::*)(int)const)
	CLASS_METHOD_OVERLOAD(Method3, int(mynamespace::MyDerived::*)(int*,double*))
END_CLASS_TYPE

BEGIN_CLASS_TEMPLATE_2(mynamespace::MyTemplate)
	CLASS_CONSTRUCTOR_0
	CLASS_FIELD(field1)
	CLASS_FIELD(field2)
END_CLASS_TYPE

TEST_CASE(TestClassType)
{
	using namespace mynamespace;
	EnsureObjectTypeInitialized();

	ObjectType* base=TypeOf<MyBase>();
	TEST_ASSERT(base==GetObjectType(L"mynamespace::MyBase"));
	TEST_ASSERT(base->Name()==L"mynamespace::MyBase");
	TEST_ASSERT(base->Size()==sizeof(MyBase));
	TEST_ASSERT(base->Category()==ObjectType::Class);
	TEST_ASSERT(base->IsPOD()==false);

	TEST_ASSERT(base->BaseClasses().Count()==0);
	TEST_ASSERT(base->Fields().Count()==2);
	TEST_ASSERT(base->Fields().Keys()[0]==L"field1");
	TEST_ASSERT(base->Fields().Values()[0]->Name()==L"field1");
	TEST_ASSERT(base->Fields().Values()[0]->Type()==TypeOf<int>());
	TEST_ASSERT(base->Fields().Keys()[1]==L"field2");
	TEST_ASSERT(base->Fields().Values()[1]->Name()==L"field2");
	TEST_ASSERT(base->Fields().Values()[1]->Type()==TypeOf<double>());
	TEST_ASSERT(base->Methods().Keys().Count()==2);
	TEST_ASSERT(base->Methods().Keys()[0]==L"Method1");
	TEST_ASSERT(base->Methods().GetByIndex(0).Count()==1);
	TEST_ASSERT(base->Methods().GetByIndex(0)[0]->Name()==L"Method1");
	TEST_ASSERT(base->Methods().GetByIndex(0)[0]->Type()==TypeOf<void(MyBase::*)(int)const>());
	TEST_ASSERT(base->Methods().Keys()[1]==L"Method2");
	TEST_ASSERT(base->Methods().GetByIndex(1).Count()==1);
	TEST_ASSERT(base->Methods().GetByIndex(1)[0]->Name()==L"Method2");
	TEST_ASSERT(base->Methods().GetByIndex(1)[0]->Type()==TypeOf<int(MyBase::*)(int*,double*)>());

	ObjectType* derived=TypeOf<MyDerived>();
	TEST_ASSERT(derived==GetObjectType(L"mynamespace::MyDerived"));
	TEST_ASSERT(derived->Name()==L"mynamespace::MyDerived");
	TEST_ASSERT(derived->Size()==sizeof(MyDerived));
	TEST_ASSERT(derived->Category()==ObjectType::Class);
	TEST_ASSERT(derived->IsPOD()==false);

	TEST_ASSERT(derived->BaseClasses().Count()==1);
	TEST_ASSERT(derived->BaseClasses()[0]==TypeOf<MyBase>());
	TEST_ASSERT(derived->Fields().Count()==2);
	TEST_ASSERT(derived->Fields().Keys()[0]==L"field3");
	TEST_ASSERT(derived->Fields().Values()[0]->Name()==L"field3");
	TEST_ASSERT(derived->Fields().Values()[0]->Type()==TypeOf<int*>());
	TEST_ASSERT(derived->Fields().Keys()[1]==L"field4");
	TEST_ASSERT(derived->Fields().Values()[1]->Name()==L"field4");
	TEST_ASSERT(derived->Fields().Values()[1]->Type()==TypeOf<double*>());
	TEST_ASSERT(derived->Methods().Keys().Count()==1);
	TEST_ASSERT(derived->Methods().Keys()[0]==L"Method3");
	TEST_ASSERT(derived->Methods().GetByIndex(0).Count()==2);
	TEST_ASSERT(derived->Methods().GetByIndex(0)[0]->Name()==L"Method3");
	TEST_ASSERT(derived->Methods().GetByIndex(0)[0]->Type()==TypeOf<void(MyDerived::*)(int)const>());
	TEST_ASSERT(derived->Methods().GetByIndex(0)[1]->Name()==L"Method3");
	TEST_ASSERT(derived->Methods().GetByIndex(0)[1]->Type()==TypeOf<int(MyDerived::*)(int*,double*)>());
}

TEST_CASE(TestClassTemplateType)
{
	using namespace mynamespace;
	EnsureObjectTypeInitialized();

	ObjectType* temp=TypeOf<MyTemplate<int,double>>();
	TEST_ASSERT(temp==GetObjectType(L"mynamespace::MyTemplate<signed __int32,double>"));
	TEST_ASSERT(temp->Name()==L"mynamespace::MyTemplate<signed __int32,double>");
	TEST_ASSERT(temp->Size()==sizeof(MyTemplate<int,double>));
	TEST_ASSERT(temp->Category()==ObjectType::Class);
	TEST_ASSERT(temp->IsPOD()==false);
	TEST_ASSERT(temp->IsTemplateClass()==true);
	TEST_ASSERT(temp->TemplateName()==L"mynamespace::MyTemplate");
	TEST_ASSERT(temp->Typenames().Count()==2);
	TEST_ASSERT(temp->Typenames()[0]==TypeOf<int>());
	TEST_ASSERT(temp->Typenames()[1]==TypeOf<double>());

	TEST_ASSERT(temp->BaseClasses().Count()==0);
	TEST_ASSERT(temp->Fields().Count()==2);
	TEST_ASSERT(temp->Fields().Keys()[0]==L"field1");
	TEST_ASSERT(temp->Fields().Values()[0]->Name()==L"field1");
	TEST_ASSERT(temp->Fields().Values()[0]->Type()==TypeOf<int>());
	TEST_ASSERT(temp->Fields().Keys()[1]==L"field2");
	TEST_ASSERT(temp->Fields().Values()[1]->Name()==L"field2");
	TEST_ASSERT(temp->Fields().Values()[1]->Type()==TypeOf<double>());
	TEST_ASSERT(temp->Methods().Keys().Count()==0);
}

namespace mynamespace
{
	class Foo
	{
	public:
		static int		used;
		int				value;

		Foo()
		{
			value=used++;
		}
	};
	int Foo::used=0;
};

BEGIN_CLASS_TYPE(mynamespace::Foo)
	CLASS_CONSTRUCTOR_0
	CLASS_FIELD(value);
END_CLASS_TYPE

TEST_CASE(TestCreateInstance)
{
	using namespace mynamespace;
	{
		ObjectValue v=New<int>();
		TEST_ASSERT(v==true);
		TEST_ASSERT(!v==false);
		TEST_ASSERT(v.GetType()==TypeOf<int>());
		TEST_ASSERT(*(int*)v.GetValue()==0);
	}
	{
		ObjectValue v=New<const int>();
		TEST_ASSERT(v==true);
		TEST_ASSERT(!v==false);
		TEST_ASSERT(v.GetType()==TypeOf<const int>());
		TEST_ASSERT(*(const int*)v.GetValue()==0);
	}
	{
		ObjectValue v=New<volatile int&>();
		TEST_ASSERT(v==false);
	}
	{
		ObjectValue v=New<int*>();
		TEST_ASSERT(v==true);
		TEST_ASSERT(!v==false);
		TEST_ASSERT(v.GetType()==TypeOf<int*>());
		TEST_ASSERT(*(int**)v.GetValue()==0);
	}
	{
		ObjectValue v=New<Foo>();
		TEST_ASSERT(v==true);
		TEST_ASSERT(!v==false);
		TEST_ASSERT(v.GetType()==TypeOf<Foo>());
		TEST_ASSERT(((Foo*)v.GetValue())->value==0);
		TEST_ASSERT(Foo::used==1);
	}
	{
		ObjectValue v=New<Foo[3]>();
		TEST_ASSERT(v==true);
		TEST_ASSERT(!v==false);
		TEST_ASSERT(v.GetType()==TypeOf<Foo[3]>());

		Foo* foo=(Foo*)v.GetValue();
		TEST_ASSERT(foo[0].value==1);
		TEST_ASSERT(foo[1].value==2);
		TEST_ASSERT(foo[2].value==3);
		TEST_ASSERT(Foo::used==4);
	}
	{
		ObjectValue v=New<MyEnum>();
		TEST_ASSERT(v==true);
		TEST_ASSERT(!v==false);
		TEST_ASSERT(v.GetType()==TypeOf<MyEnum>());
		TEST_ASSERT(*(MyEnum*)v.GetValue()==0);
	}
	{
		ObjectValue v=New<int(*)(int,double)>();
		TEST_ASSERT(v==true);
		TEST_ASSERT(!v==false);
		TEST_ASSERT(v.GetType()==TypeOf<int(*)(int,double)>());
		TEST_ASSERT(*(int(**)(int,double))v.GetValue()==0);
	}
	{
		int i=100;
		ObjectValue v=NewReference(i);
		TEST_ASSERT(v==true);
		TEST_ASSERT(!v==false);
		TEST_ASSERT(v.GetType()==TypeOf<int>());
		TEST_ASSERT((int*)v.GetValue()==&i);
		TEST_ASSERT(*(int*)v.GetValue()==100);
	}
	{
		int i=100;
		ObjectValue v=NewValue(i);
		TEST_ASSERT(v==true);
		TEST_ASSERT(!v==false);
		TEST_ASSERT(v.GetType()==TypeOf<int>());
		TEST_ASSERT((int*)v.GetValue()!=&i);
		TEST_ASSERT(*(int*)v.GetValue()==100);
	}
}

namespace mynamespace
{
	class ClassA : public Object
	{
	public:
		int a;

		ClassA()
		{
			a=1;
		}
	};

	class ClassB : public Object
	{
	public:
		int b;

		ClassB()
		{
			b=2;
		}
	};

	class ClassC : public Object
	{
	public:
		int c;

		ClassC()
		{
			c=3;
		}
	};

	class ClassD : public Object
	{
	public:
		int d;

		ClassD()
		{
			d=4;
		}
	};

	class ClassAB : public ClassA , public ClassB
	{
	public:
		int ab;

		ClassAB()
		{
			ab=5;
		}
	};

	class ClassCD : public ClassC , public ClassD
	{
	public:
		int cd;

		ClassCD()
		{
			cd=6;
		}
	};

	class ClassABCD : public ClassAB , public ClassCD
	{
	public:
		int abcd;

		ClassABCD()
		{
			abcd=7;
		}

		ClassABCD(int _abcd)
		{
			abcd=_abcd;
		}

		int Add(int x, int y)
		{
			return x + y + a + b + c + d + ab + cd + abcd;
		};
	};
}

BEGIN_CLASS_TYPE(mynamespace::ClassA)
	CLASS_CONSTRUCTOR_0
	CLASS_FIELD(a)
END_CLASS_TYPE

BEGIN_CLASS_TYPE(mynamespace::ClassB)
	CLASS_CONSTRUCTOR_0
	CLASS_FIELD(b)
END_CLASS_TYPE

BEGIN_CLASS_TYPE(mynamespace::ClassC)
	CLASS_CONSTRUCTOR_0
	CLASS_FIELD(c)
END_CLASS_TYPE

BEGIN_CLASS_TYPE(mynamespace::ClassD)
	CLASS_CONSTRUCTOR_0
	CLASS_FIELD(d)
END_CLASS_TYPE

BEGIN_CLASS_TYPE(mynamespace::ClassAB)
	CLASS_BASE(mynamespace::ClassA)
	CLASS_BASE(mynamespace::ClassB)
	CLASS_CONSTRUCTOR_0
	CLASS_FIELD(ab)
END_CLASS_TYPE

BEGIN_CLASS_TYPE(mynamespace::ClassCD)
	CLASS_BASE(mynamespace::ClassC)
	CLASS_BASE(mynamespace::ClassD)
	CLASS_CONSTRUCTOR_0
	CLASS_FIELD(cd)
END_CLASS_TYPE

BEGIN_CLASS_TYPE(mynamespace::ClassABCD)
	CLASS_BASE(mynamespace::ClassAB)
	CLASS_BASE(mynamespace::ClassCD)
	CLASS_CONSTRUCTOR_0
	CLASS_CONSTRUCTOR_1(int)
	CLASS_FIELD(abcd)
	CLASS_METHOD(Add)
END_CLASS_TYPE

void TestClassFieldValue(const ObjectValue& value, const WString& name, int expected)
{
	ObjectValue field=value.GetField(name);
	TEST_ASSERT(field);
	TEST_ASSERT(field.GetType()==TypeOf<int>());
	TEST_ASSERT(*((int*)field.GetValue())==expected);
}

TEST_CASE(TestClassFieldRetriving)
{
	using namespace mynamespace;
	{
		ObjectValue abcd=New<ClassABCD>();
		ObjectValue ab=abcd.CastToBaseClass(TypeOf<ClassAB>());
		ObjectValue cd=abcd.CastToBaseClass(TypeOf<ClassCD>());
		ObjectValue a=ab.CastToBaseClass(TypeOf<ClassA>());
		ObjectValue b=ab.CastToBaseClass(TypeOf<ClassB>());
		ObjectValue c=cd.CastToBaseClass(TypeOf<ClassC>());
		ObjectValue d=cd.CastToBaseClass(TypeOf<ClassD>());

		TEST_ASSERT(a.GetType()==TypeOf<ClassA>());
		TEST_ASSERT(((ClassA*)a.GetValue())->a==1);

		TEST_ASSERT(b.GetType()==TypeOf<ClassB>());
		TEST_ASSERT(((ClassB*)b.GetValue())->b==2);

		TEST_ASSERT(c.GetType()==TypeOf<ClassC>());
		TEST_ASSERT(((ClassC*)c.GetValue())->c==3);

		TEST_ASSERT(d.GetType()==TypeOf<ClassD>());
		TEST_ASSERT(((ClassD*)d.GetValue())->d==4);

		TEST_ASSERT(ab.GetType()==TypeOf<ClassAB>());
		TEST_ASSERT(((ClassAB*)ab.GetValue())->ab==5);

		TEST_ASSERT(cd.GetType()==TypeOf<ClassCD>());
		TEST_ASSERT(((ClassCD*)cd.GetValue())->cd==6);

		TEST_ASSERT(abcd.GetType()==TypeOf<ClassABCD>());
		TEST_ASSERT(((ClassABCD*)abcd.GetValue())->abcd==7);

		TEST_ASSERT(abcd.CastToBaseClass(TypeOf<ClassA>()).GetValue()==a.GetValue());
		TEST_ASSERT(abcd.CastToBaseClass(TypeOf<ClassB>()).GetValue()==b.GetValue());
		TEST_ASSERT(abcd.CastToBaseClass(TypeOf<ClassC>()).GetValue()==c.GetValue());
		TEST_ASSERT(abcd.CastToBaseClass(TypeOf<ClassD>()).GetValue()==d.GetValue());

		TEST_ASSERT(a.CastToSubClass(TypeOf<ClassAB>()).GetValue()==ab.GetValue());
		TEST_ASSERT(b.CastToSubClass(TypeOf<ClassAB>()).GetValue()==ab.GetValue());
		TEST_ASSERT(c.CastToSubClass(TypeOf<ClassCD>()).GetValue()==cd.GetValue());
		TEST_ASSERT(d.CastToSubClass(TypeOf<ClassCD>()).GetValue()==cd.GetValue());

		TEST_ASSERT(a.CastToSubClass(TypeOf<ClassABCD>()).GetValue()==abcd.GetValue());
		TEST_ASSERT(b.CastToSubClass(TypeOf<ClassABCD>()).GetValue()==abcd.GetValue());
		TEST_ASSERT(c.CastToSubClass(TypeOf<ClassABCD>()).GetValue()==abcd.GetValue());
		TEST_ASSERT(d.CastToSubClass(TypeOf<ClassABCD>()).GetValue()==abcd.GetValue());

		TestClassFieldValue(a, L"a", 1);
		TestClassFieldValue(b, L"b", 2);
		TestClassFieldValue(c, L"c", 3);
		TestClassFieldValue(d, L"d", 4);

		TestClassFieldValue(ab, L"a", 1);
		TestClassFieldValue(ab, L"b", 2);
		TestClassFieldValue(ab, L"ab", 5);

		TestClassFieldValue(cd, L"c", 3);
		TestClassFieldValue(cd, L"d", 4);
		TestClassFieldValue(cd, L"cd", 6);

		TestClassFieldValue(abcd, L"a", 1);
		TestClassFieldValue(abcd, L"b", 2);
		TestClassFieldValue(abcd, L"c", 3);
		TestClassFieldValue(abcd, L"d", 4);
		TestClassFieldValue(abcd, L"ab", 5);
		TestClassFieldValue(abcd, L"cd", 6);
		TestClassFieldValue(abcd, L"abcd", 7);

		abcd.SetField(L"a", NewValue<int>(100));
		abcd.SetField(L"b", NewValue<int>(200));
		abcd.SetField(L"c", NewValue<int>(300));
		abcd.SetField(L"d", NewValue<int>(400));
		abcd.SetField(L"ab", NewValue<int>(500));
		abcd.SetField(L"cd", NewValue<int>(600));
		abcd.SetField(L"abcd", NewValue<int>(700));

		TestClassFieldValue(abcd, L"a", 100);
		TestClassFieldValue(abcd, L"b", 200);
		TestClassFieldValue(abcd, L"c", 300);
		TestClassFieldValue(abcd, L"d", 400);
		TestClassFieldValue(abcd, L"ab", 500);
		TestClassFieldValue(abcd, L"cd", 600);
		TestClassFieldValue(abcd, L"abcd", 700);
	}
}

TEST_CASE(TestMethodInvokingNative)
{
	using namespace mynamespace;
	ObjectValue abcd=New<ClassABCD>();
	{
		ObjectMember* method=abcd.GetType()->Methods()[L"Add"][0];

		int x=100;
		int y=200;
		void* arguments[]={&x, &y};
		int* result=(int*)method->Invoke(abcd.GetValue(), arguments);
		TEST_ASSERT(result);
		TEST_ASSERT(*result==328);
		delete result;

		ObjectMember* constructor=TypeOf<int>()->Constructors()[1];
		result=(int*)constructor->Invoke(0, arguments);
		TEST_ASSERT(result);
		TEST_ASSERT(*result==100);
		delete result;
	}
	{
		ObjectValue result;
		Array<ObjectValue> arguments;
		arguments.Resize(2);
		arguments[0]=NewValue<int>(100);
		arguments[1]=NewValue<int>(200);
		TEST_ASSERT(abcd.InvokeMethod(L"Add", arguments.Wrap(), result)==true);
		TEST_ASSERT(result);
		TEST_ASSERT(result.GetType()==TypeOf<int>());
		TEST_ASSERT(*((int*)result.GetValue())==328);
	}
	{
		ObjectValue result;
		Array<ObjectValue> arguments;
		arguments.Resize(1);
		arguments[0]=NewValue<int>(100);
		TEST_ASSERT(InvokeMethod(TypeOf<ClassABCD>()->Constructors(), 0, arguments.Wrap(), result, TypeOf<ClassABCD>()));
		TEST_ASSERT(result);
		TestClassFieldValue(result, L"abcd", 100);
	}
}

TEST_CASE(TestImplicitlyConversion)
{
	using namespace mynamespace;

	ObjectValue abcd=New<ClassABCD>();
	ObjectValue b=abcd.ImplicitlyConvertTo(TypeOf<ClassB>());
	TEST_ASSERT(b);
	TEST_ASSERT(((ClassB*)b.GetValue())->b==2);

	ClassABCD* abcd_ptr=(ClassABCD*)abcd.GetValue();
	ObjectValue pabcd(TypeOf<volatile ClassABCD* const>(), &abcd_ptr, false);
	ObjectValue pcd=pabcd.ImplicitlyConvertTo(TypeOf<ClassCD*>());
	TEST_ASSERT(pcd);
	ClassCD* cd_ptr=*((ClassCD**)pcd.GetValue());
	TEST_ASSERT(cd_ptr->cd==6);

	TEST_ASSERT(pcd.ImplicitlyConvertTo(TypeOf<ClassABCD*>())==false);
	TEST_ASSERT(b.ImplicitlyConvertTo(TypeOf<int>())==false);
}