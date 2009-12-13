#include <string.h>
#include "..\..\Library\UnitTest\UnitTest.h"

using namespace vl;

void CheckString(const WString& string, const wchar_t* buffer)
{
	size_t len=wcslen(buffer);
	TEST_ASSERT(string.Length()==len);
	for(size_t i=0;i<len;i++)
	{
		TEST_ASSERT(string[i]==buffer[i]);
	}
	TEST_ASSERT(wcscmp(string.Buffer(),buffer)==0);
}

TEST_CASE(StringDefaultConstructor)
{
	WString string;
	TEST_ASSERT(*string.Buffer()==0);
	CheckString(string,L"");
}

TEST_CASE(StringNonCopyConstructor)
{
	wchar_t* buffer=L"vczh";
	WString string(buffer,false);
	TEST_ASSERT(string.Buffer()==buffer);
	CheckString(string,L"vczh");
}

TEST_CASE(StringBufferCopyConstructor)
{
	wchar_t* buffer=L"vczh";
	WString string(buffer,true);
	TEST_ASSERT(string.Buffer()!=buffer);
	CheckString(string,L"vczh");
}

TEST_CASE(StringCharConstructor)
{
	WString string=L'v';
	CheckString(string,L"v");
}

TEST_CASE(StringCopyConstructor)
{
	{
		WString string;
		CheckString(WString(string),L"");
	}
	{
		WString string(L"vczh",false);
		CheckString(WString(string),L"vczh");
	}
	{
		WString string(L"vczh",true);
		CheckString(WString(string),L"vczh");
	}
	{
		WString string=L'v';
		CheckString(WString(string),L"v");
	}
}

TEST_CASE(StringLeftRightSub)
{
	WString string=L"vczh is genius";
	CheckString(string.Left(4),L"vczh");
	CheckString(string.Sub(5,2),L"is");
	CheckString(string.Right(6),L"genius");
	CheckString(string.Sub(4,4).Left(2),L" i");
	CheckString(string.Sub(4,4).Sub(1,2),L"is");
	CheckString(string.Sub(4,4).Right(2),L"s ");
}

TEST_CASE(StringConcat)
{
	WString string=L"vczh is genius";
	CheckString(string.Left(7)+string.Right(7),L"vczh is genius");
}

TEST_CASE(StringConcatFriend)
{
	WString string=L"vczh is genius";
	CheckString(L"vczh is"+string.Right(7),L"vczh is genius");
}

TEST_CASE(StringComparison)
{
	WString full=L"VCZHgeniusvczh";
	WString string[3]={full.Left(4), full.Sub(4, 6), full.Right(4)};
	for(int i=0;i<3;i++)
	{
		for(int j=0;j<3;j++)
		{
			TEST_ASSERT((string[i]==string[j])==(i==j));
			TEST_ASSERT((string[i]!=string[j])==(i!=j));
			TEST_ASSERT((string[i]<string[j])==(i<j));
			TEST_ASSERT((string[i]<=string[j])==(i<=j));
			TEST_ASSERT((string[i]>string[j])==(i>j));
			TEST_ASSERT((string[i]>=string[j])==(i>=j));
		}
	}
}

TEST_CASE(StringComparisonFriend)
{
	WString full=L"VCZHgeniusvczh";
	const wchar_t* left[3]={L"VCZH", L"genius", L"vczh"};
	WString right[3]={full.Left(4), full.Sub(4, 6), full.Right(4)};
	for(int i=0;i<3;i++)
	{
		for(int j=0;j<3;j++)
		{
			TEST_ASSERT((left[i]==right[j])==(i==j));
			TEST_ASSERT((left[i]!=right[j])==(i!=j));
			TEST_ASSERT((left[i]<right[j])==(i<j));
			TEST_ASSERT((left[i]<=right[j])==(i<=j));
			TEST_ASSERT((left[i]>right[j])==(i>j));
			TEST_ASSERT((left[i]>=right[j])==(i>=j));
		}
	}
}

TEST_CASE(StringInsertRemove)
{
	CheckString(WString(L"vczh is genius").Remove(4,4),L"vczhgenius");
	CheckString(WString(L"vczh is genius").Insert(8,L"a "),L"vczh is a genius");
	CheckString(WString(L"vczh is genius").Remove(4,4).Insert(4,L" a "),L"vczh a genius");
	CheckString(WString(L"vczh is genius").Insert(8,L" a ").Remove(4,4),L"vczh a genius");
	CheckString(WString(L"[vczh is genius]").Sub(1,14).Remove(4,4),L"vczhgenius");
	CheckString(WString(L"[vczh is genius]").Sub(1,14).Insert(8,L"a "),L"vczh is a genius");
	CheckString(WString(L"[vczh is genius]").Sub(1,14).Remove(4,4).Insert(4,L" a "),L"vczh a genius");
	CheckString(WString(L"[vczh is genius]").Sub(1,14).Insert(8,L" a ").Remove(4,4),L"vczh a genius");
}