#include <string.h>
#include "..\..\Library\UnitTest\UnitTest.h"
#include "..\..\Library\Combinator\Combinator.h"
#include "..\..\Library\Combinator\StringCombinator.h"
#include "..\..\Library\Collections\OperationString.h"

using namespace vl;
using namespace vl::collections;
using namespace vl::combinator;

/***********************************************************************
迭代器
***********************************************************************/

TEST_CASE(TestStringInput)
{
	WString string=L"vczh is genius!";

	StringInput<wchar_t> input=string;
	TEST_ASSERT(input.Length()==15);
	TEST_ASSERT(input.Available()==true);
	TEST_ASSERT(input.Index()==0);
	TEST_ASSERT(input.Current()==L'v');
	TEST_ASSERT(input.Next());
	TEST_ASSERT(input.Available()==true);
	TEST_ASSERT(input.Index()==1);
	TEST_ASSERT(input.Current()==L'c');
	TEST_ASSERT(input.Next());
	TEST_ASSERT(input.Available()==true);
	TEST_ASSERT(input.Index()==2);
	TEST_ASSERT(input.Current()==L'z');
	TEST_ASSERT(input.Next());
	TEST_ASSERT(input.Available()==true);
	TEST_ASSERT(input.Index()==3);
	TEST_ASSERT(input.Current()==L'h');

	StringInput<wchar_t> backup=input;
	wchar_t buffer[]=L" is genius!";
	for(int i=0;i<sizeof(buffer)/sizeof(*buffer)-1;i++)
	{
		TEST_ASSERT(backup.Next()==true);
		TEST_ASSERT(backup.Available()==true);
		TEST_ASSERT(backup.Index()==4+i);
		TEST_ASSERT(backup.Current()==buffer[i]);
	}
	TEST_ASSERT(backup.Next()==false);
	TEST_ASSERT(backup.Available()==false);

	input.Step(5);
	TEST_ASSERT(input.Index()==8);
	TEST_ASSERT(input.Current()==L'g');
	TEST_ASSERT(input.Available()==true);

	input.Step(100);
	TEST_ASSERT(input.Available()==false);
}

TEST_CASE(TestEnumerableInput)
{
	wchar_t original[]=L"vczh is genius!";
	List<wchar_t> list;
	for(int i=0;i<sizeof(original)/sizeof(*original)-1;i++)
	{
		list.Add(original[i]);
	}

	EnumerableInput<wchar_t> input=list.Wrap();
	TEST_ASSERT(input.Available()==true);
	TEST_ASSERT(input.Index()==0);
	TEST_ASSERT(input.Current()==L'v');
	TEST_ASSERT(input.Next());
	TEST_ASSERT(input.Available()==true);
	TEST_ASSERT(input.Index()==1);
	TEST_ASSERT(input.Current()==L'c');
	TEST_ASSERT(input.Next());
	TEST_ASSERT(input.Available()==true);
	TEST_ASSERT(input.Index()==2);
	TEST_ASSERT(input.Current()==L'z');
	TEST_ASSERT(input.Next());
	TEST_ASSERT(input.Available()==true);
	TEST_ASSERT(input.Index()==3);
	TEST_ASSERT(input.Current()==L'h');

	EnumerableInput<wchar_t> backup=input;
	wchar_t buffer[]=L" is genius!";
	for(int i=0;i<sizeof(buffer)/sizeof(*buffer)-1;i++)
	{
		TEST_ASSERT(backup.Next()==true);
		TEST_ASSERT(backup.Available()==true);
		TEST_ASSERT(backup.Index()==4+i);
		TEST_ASSERT(backup.Current()==buffer[i]);
	}
	TEST_ASSERT(backup.Next()==false);
	TEST_ASSERT(backup.Available()==false);

	for(int i=0;i<sizeof(buffer)/sizeof(*buffer)-1;i++)
	{
		TEST_ASSERT(input.Next()==true);
		TEST_ASSERT(input.Available()==true);
		TEST_ASSERT(input.Index()==4+i);
		TEST_ASSERT(input.Current()==buffer[i]);
	}
	TEST_ASSERT(input.Next()==false);
	TEST_ASSERT(input.Available()==false);
}

/***********************************************************************
_Unit
***********************************************************************/

TEST_CASE(TestUnitCombinator)
{
	WString string=L"vczh";
	StringInput<wchar_t> input=string;
	Types<StringInput<wchar_t>>::GlobalInfo info;

	info.errors.Clear();
	_Unit<StringInput<wchar_t>> unit1(L'v', L"这里需要v。");
	StringInput<wchar_t> input1=input;
	ParsingResult<wchar_t> result1=unit1.Parse(input1, info);
	TEST_ASSERT(result1==true);
	TEST_ASSERT(result1.Value()==L'v');
	TEST_ASSERT(info.errors.Count()==0);
	TEST_ASSERT(input1.Current()==L'c');
	TEST_ASSERT(input1.Index()==1);

	info.errors.Clear();
	_Unit<StringInput<wchar_t>> unit2(L'c', L"这里需要c。");
	StringInput<wchar_t> input2=input;
	ParsingResult<wchar_t> result2=unit2.Parse(input2, info);
	TEST_ASSERT(result2==false);
	TEST_ASSERT(info.errors.Count()==1);
	TEST_ASSERT(info.errors[0]->Message()==L"这里需要c。");
	TEST_ASSERT(info.errors[0]->GetPosition().Index()==0);
	TEST_ASSERT(info.errors[0]->GetPosition().Current()==L'v');
	TEST_ASSERT(input2.Current()==L'v');
	TEST_ASSERT(input2.Index()==0);
}

/***********************************************************************
_Seq
***********************************************************************/

TEST_CASE(TestSeqCombinator)
{
	WString string=L"vczh";
	StringInput<wchar_t> source=string;
	Types<StringInput<wchar_t>>::GlobalInfo info;
	{
		_Seq<StringInput<wchar_t>, wchar_t, wchar_t> seq(
			new _Unit<StringInput<wchar_t>>(L'v', L"这里需要v。"),
			new _Unit<StringInput<wchar_t>>(L'c', L"这里需要c。")
			);
		StringInput<wchar_t> input=source;
		info.errors.Clear();
		ParsingResult<ParsingPair<wchar_t, wchar_t>> result=seq.Parse(input, info);

		TEST_ASSERT(result==true);
		TEST_ASSERT(result.Value().First()==L'v');
		TEST_ASSERT(result.Value().Second()==L'c');
		TEST_ASSERT(info.errors.Count()==0);
		TEST_ASSERT(input.Current()==L'z');
		TEST_ASSERT(input.Index()==2);
	}
	{
		_Seq<StringInput<wchar_t>, wchar_t, wchar_t> seq(
			new _Unit<StringInput<wchar_t>>(L'v', L"这里需要v。"),
			new _Unit<StringInput<wchar_t>>(L's', L"这里需要s。")
			);
		StringInput<wchar_t> input=source;
		info.errors.Clear();
		ParsingResult<ParsingPair<wchar_t, wchar_t>> result=seq.Parse(input, info);

		TEST_ASSERT(result==false);
		TEST_ASSERT(info.errors.Count()==1);
		TEST_ASSERT(info.errors[0]->Message()==L"这里需要s。");
		TEST_ASSERT(info.errors[0]->GetPosition().Index()==1);
		TEST_ASSERT(info.errors[0]->GetPosition().Current()==L'c');
		TEST_ASSERT(input.Current()==L'c');
		TEST_ASSERT(input.Index()==1);
	}
	{
		_Seq<StringInput<wchar_t>, wchar_t, wchar_t> seq(
			new _Unit<StringInput<wchar_t>>(L'x', L"这里需要x。"),
			new _Unit<StringInput<wchar_t>>(L'c', L"这里需要c。")
			);
		StringInput<wchar_t> input=source;
		info.errors.Clear();
		ParsingResult<ParsingPair<wchar_t, wchar_t>> result=seq.Parse(input, info);

		TEST_ASSERT(result==false);
		TEST_ASSERT(info.errors.Count()==1);
		TEST_ASSERT(info.errors[0]->Message()==L"这里需要x。");
		TEST_ASSERT(info.errors[0]->GetPosition().Index()==0);
		TEST_ASSERT(info.errors[0]->GetPosition().Current()==L'v');
		TEST_ASSERT(input.Current()==L'v');
		TEST_ASSERT(input.Index()==0);
	}
}

/***********************************************************************
_Left
***********************************************************************/

TEST_CASE(TestLeftCombinator)
{
	WString string=L"vczh";
	StringInput<wchar_t> source=string;
	Types<StringInput<wchar_t>>::GlobalInfo info;
	{
		_Left<StringInput<wchar_t>, wchar_t, wchar_t> left(
			new _Unit<StringInput<wchar_t>>(L'v', L"这里需要v。"),
			new _Unit<StringInput<wchar_t>>(L'c', L"这里需要c。")
			);
		StringInput<wchar_t> input=source;
		info.errors.Clear();
		ParsingResult<wchar_t> result=left.Parse(input, info);

		TEST_ASSERT(result==true);
		TEST_ASSERT(result.Value()==L'v');
		TEST_ASSERT(info.errors.Count()==0);
		TEST_ASSERT(input.Current()==L'z');
		TEST_ASSERT(input.Index()==2);
	}
	{
		_Left<StringInput<wchar_t>, wchar_t, wchar_t> left(
			new _Unit<StringInput<wchar_t>>(L'v', L"这里需要v。"),
			new _Unit<StringInput<wchar_t>>(L's', L"这里需要s。")
			);
		StringInput<wchar_t> input=source;
		info.errors.Clear();
		ParsingResult<wchar_t> result=left.Parse(input, info);

		TEST_ASSERT(result==false);
		TEST_ASSERT(info.errors.Count()==1);
		TEST_ASSERT(info.errors[0]->Message()==L"这里需要s。");
		TEST_ASSERT(info.errors[0]->GetPosition().Index()==1);
		TEST_ASSERT(info.errors[0]->GetPosition().Current()==L'c');
		TEST_ASSERT(input.Current()==L'c');
		TEST_ASSERT(input.Index()==1);
	}
	{
		_Left<StringInput<wchar_t>, wchar_t, wchar_t> left(
			new _Unit<StringInput<wchar_t>>(L'x', L"这里需要x。"),
			new _Unit<StringInput<wchar_t>>(L'c', L"这里需要c。")
			);
		StringInput<wchar_t> input=source;
		info.errors.Clear();
		ParsingResult<wchar_t> result=left.Parse(input, info);

		TEST_ASSERT(result==false);
		TEST_ASSERT(info.errors.Count()==1);
		TEST_ASSERT(info.errors[0]->Message()==L"这里需要x。");
		TEST_ASSERT(info.errors[0]->GetPosition().Index()==0);
		TEST_ASSERT(info.errors[0]->GetPosition().Current()==L'v');
		TEST_ASSERT(input.Current()==L'v');
		TEST_ASSERT(input.Index()==0);
	}
}

/***********************************************************************
_Right
***********************************************************************/

TEST_CASE(TestRightCombinator)
{
	WString string=L"vczh";
	StringInput<wchar_t> source=string;
	Types<StringInput<wchar_t>>::GlobalInfo info;
	{
		_Right<StringInput<wchar_t>, wchar_t, wchar_t> right(
			new _Unit<StringInput<wchar_t>>(L'v', L"这里需要v。"),
			new _Unit<StringInput<wchar_t>>(L'c', L"这里需要c。")
			);
		StringInput<wchar_t> input=source;
		info.errors.Clear();
		ParsingResult<wchar_t> result=right.Parse(input, info);

		TEST_ASSERT(result==true);
		TEST_ASSERT(result.Value()==L'c');
		TEST_ASSERT(info.errors.Count()==0);
		TEST_ASSERT(input.Current()==L'z');
		TEST_ASSERT(input.Index()==2);
	}
	{
		_Right<StringInput<wchar_t>, wchar_t, wchar_t> right(
			new _Unit<StringInput<wchar_t>>(L'v', L"这里需要v。"),
			new _Unit<StringInput<wchar_t>>(L's', L"这里需要s。")
			);
		StringInput<wchar_t> input=source;
		info.errors.Clear();
		ParsingResult<wchar_t> result=right.Parse(input, info);

		TEST_ASSERT(result==false);
		TEST_ASSERT(info.errors.Count()==1);
		TEST_ASSERT(info.errors[0]->Message()==L"这里需要s。");
		TEST_ASSERT(info.errors[0]->GetPosition().Index()==1);
		TEST_ASSERT(info.errors[0]->GetPosition().Current()==L'c');
		TEST_ASSERT(input.Current()==L'c');
		TEST_ASSERT(input.Index()==1);
	}
	{
		_Right<StringInput<wchar_t>, wchar_t, wchar_t> right(
			new _Unit<StringInput<wchar_t>>(L'x', L"这里需要x。"),
			new _Unit<StringInput<wchar_t>>(L'c', L"这里需要c。")
			);
		StringInput<wchar_t> input=source;
		info.errors.Clear();
		ParsingResult<wchar_t> result=right.Parse(input, info);

		TEST_ASSERT(result==false);
		TEST_ASSERT(info.errors.Count()==1);
		TEST_ASSERT(info.errors[0]->Message()==L"这里需要x。");
		TEST_ASSERT(info.errors[0]->GetPosition().Index()==0);
		TEST_ASSERT(info.errors[0]->GetPosition().Current()==L'v');
		TEST_ASSERT(input.Current()==L'v');
		TEST_ASSERT(input.Index()==0);
	}
}

/***********************************************************************
_Alt
***********************************************************************/

TEST_CASE(TestAltCombinator)
{
	WString string=L"vczh";
	StringInput<wchar_t> source=string;
	Types<StringInput<wchar_t>>::GlobalInfo info;
	{
		_Alt<StringInput<wchar_t>, wchar_t> alt(
			new _Unit<StringInput<wchar_t>>(L'v', L"这里需要v。"),
			new _Unit<StringInput<wchar_t>>(L'c', L"这里需要c。")
			);
		StringInput<wchar_t> input=source;
		info.errors.Clear();
		ParsingResult<wchar_t> result=alt.Parse(input, info);

		TEST_ASSERT(result==true);
		TEST_ASSERT(result.Value()==L'v');
		TEST_ASSERT(input.Index()==1);
		TEST_ASSERT(input.Current()==L'c');
		TEST_ASSERT(input.Index()==1);
	}
	{
		_Alt<StringInput<wchar_t>, wchar_t> alt(
			new _Unit<StringInput<wchar_t>>(L'c', L"这里需要c。"),
			new _Unit<StringInput<wchar_t>>(L'v', L"这里需要v。")
			);
		StringInput<wchar_t> input=source;
		info.errors.Clear();
		ParsingResult<wchar_t> result=alt.Parse(input, info);

		TEST_ASSERT(result==true);
		TEST_ASSERT(result.Value()==L'v');
		TEST_ASSERT(input.Index()==1);
		TEST_ASSERT(input.Current()==L'c');
		TEST_ASSERT(input.Index()==1);
	}
	{
		_Alt<StringInput<wchar_t>, wchar_t> alt(
			new _Unit<StringInput<wchar_t>>(L'c', L"这里需要c。"),
			new _Unit<StringInput<wchar_t>>(L's', L"这里需要s。")
			);
		StringInput<wchar_t> input=source;
		info.errors.Clear();
		ParsingResult<wchar_t> result=alt.Parse(input, info);

		TEST_ASSERT(result==false);
		TEST_ASSERT(info.errors.Count()==1);
		TEST_ASSERT(info.errors[0]->Message()==L"这里需要c。");
		TEST_ASSERT(info.errors[0]->GetPosition().Index()==0);
		TEST_ASSERT(info.errors[0]->GetPosition().Current()==L'v');
		TEST_ASSERT(input.Current()==L'v');
		TEST_ASSERT(input.Index()==0);
	}
	{
		_Alt<StringInput<wchar_t>, wchar_t> alt(
			new _Right<StringInput<wchar_t>, wchar_t, wchar_t>(
				new _Unit<StringInput<wchar_t>>(L'v', L"这里需要v。"),
				new _Unit<StringInput<wchar_t>>(L'x', L"这里需要x。")
				),
			new _Unit<StringInput<wchar_t>>(L's', L"这里需要s。")
			);
		StringInput<wchar_t> input=source;
		info.errors.Clear();
		ParsingResult<wchar_t> result=alt.Parse(input, info);

		TEST_ASSERT(result==false);
		TEST_ASSERT(info.errors.Count()==1);
		TEST_ASSERT(info.errors[0]->Message()==L"这里需要x。");
		TEST_ASSERT(info.errors[0]->GetPosition().Index()==1);
		TEST_ASSERT(info.errors[0]->GetPosition().Current()==L'c');
		TEST_ASSERT(input.Current()==L'c');
		TEST_ASSERT(input.Index()==1);
	}
	{
		_Alt<StringInput<wchar_t>, wchar_t> alt(
			new _Unit<StringInput<wchar_t>>(L's', L"这里需要s。"),
			new _Right<StringInput<wchar_t>, wchar_t, wchar_t>(
				new _Unit<StringInput<wchar_t>>(L'v', L"这里需要v。"),
				new _Unit<StringInput<wchar_t>>(L'x', L"这里需要x。")
				)
			);
		StringInput<wchar_t> input=source;
		info.errors.Clear();
		ParsingResult<wchar_t> result=alt.Parse(input, info);

		TEST_ASSERT(result==false);
		TEST_ASSERT(info.errors.Count()==1);
		TEST_ASSERT(info.errors[0]->Message()==L"这里需要x。");
		TEST_ASSERT(info.errors[0]->GetPosition().Index()==1);
		TEST_ASSERT(info.errors[0]->GetPosition().Current()==L'c');
		TEST_ASSERT(input.Current()==L'c');
		TEST_ASSERT(input.Index()==1);
	}
}

/***********************************************************************
_Loop
***********************************************************************/

TEST_CASE(TestLoopCombinator)
{
	WString string=L"vvv";
	StringInput<wchar_t> source=string;
	Types<StringInput<wchar_t>>::GlobalInfo info;
	{
		_Loop<StringInput<wchar_t>, wchar_t> loop(
			new _Unit<StringInput<wchar_t>>(L'v', L"这里需要v。"),
			1,
			-1);
		StringInput<wchar_t> input=source;
		info.errors.Clear();
		ParsingResult<ParsingList<wchar_t>> result=loop.Parse(input, info);

		TEST_ASSERT(result);
		TEST_ASSERT(result.Value().Count()==3);
		ParsingList<wchar_t>::Node::Ref current=result.Value().Head();
		for(int i=0;i<3;i++)
		{
			TEST_ASSERT(current);
			TEST_ASSERT(current->Value()==L'v');
			current=current->Next();
		}
		TEST_ASSERT(!current);
		TEST_ASSERT(input.Available()==false);
	}
	{
		_Loop<StringInput<wchar_t>, wchar_t> loop(
			new _Unit<StringInput<wchar_t>>(L'v', L"这里需要v。"),
			0,
			2);
		StringInput<wchar_t> input=source;
		info.errors.Clear();
		ParsingResult<ParsingList<wchar_t>> result=loop.Parse(input, info);

		TEST_ASSERT(result);
		TEST_ASSERT(result.Value().Count()==2);
		ParsingList<wchar_t>::Node::Ref current=result.Value().Head();
		for(int i=0;i<2;i++)
		{
			TEST_ASSERT(current);
			TEST_ASSERT(current->Value()==L'v');
			current=current->Next();
		}
		TEST_ASSERT(!current);
		TEST_ASSERT(input.Current()==L'v');
		TEST_ASSERT(input.Index()==2);
	}
	{
		_Loop<StringInput<wchar_t>, wchar_t> loop(
			new _Unit<StringInput<wchar_t>>(L'v', L"这里需要v。"),
			0,
			3);
		StringInput<wchar_t> input=source;
		info.errors.Clear();
		ParsingResult<ParsingList<wchar_t>> result=loop.Parse(input, info);

		TEST_ASSERT(result);
		TEST_ASSERT(result.Value().Count()==3);
		ParsingList<wchar_t>::Node::Ref current=result.Value().Head();
		for(int i=0;i<3;i++)
		{
			TEST_ASSERT(current);
			TEST_ASSERT(current->Value()==L'v');
			current=current->Next();
		}
		TEST_ASSERT(!current);
		TEST_ASSERT(input.Available()==false);
	}
	{
		_Loop<StringInput<wchar_t>, wchar_t> loop(
			new _Unit<StringInput<wchar_t>>(L'v', L"这里需要v。"),
			4,
			-1);
		StringInput<wchar_t> input=source;
		info.errors.Clear();
		ParsingResult<ParsingList<wchar_t>> result=loop.Parse(input, info);

		TEST_ASSERT(result==false);
		TEST_ASSERT(info.errors[0]->Message()==L"这里需要v。");
		TEST_ASSERT(info.errors[0]->GetPosition().Available()==false);
		TEST_ASSERT(input.Available()==false);
	}
}

TEST_CASE(TestLoopCombinator2)
{
	WString string=L"vvvx";
	StringInput<wchar_t> source=string;
	Types<StringInput<wchar_t>>::GlobalInfo info;
	{
		_Loop<StringInput<wchar_t>, wchar_t> loop(
			new _Unit<StringInput<wchar_t>>(L'v', L"这里需要v。"),
			1,
			-1);
		StringInput<wchar_t> input=source;
		info.errors.Clear();
		ParsingResult<ParsingList<wchar_t>> result=loop.Parse(input, info);

		TEST_ASSERT(result);
		TEST_ASSERT(result.Value().Count()==3);
		ParsingList<wchar_t>::Node::Ref current=result.Value().Head();
		for(int i=0;i<3;i++)
		{
			TEST_ASSERT(current);
			TEST_ASSERT(current->Value()==L'v');
			current=current->Next();
		}
		TEST_ASSERT(!current);
		TEST_ASSERT(input.Current()==L'x');
		TEST_ASSERT(input.Index()==3);
	}
	{
		_Loop<StringInput<wchar_t>, wchar_t> loop(
			new _Unit<StringInput<wchar_t>>(L'v', L"这里需要v。"),
			0,
			2);
		StringInput<wchar_t> input=source;
		info.errors.Clear();
		ParsingResult<ParsingList<wchar_t>> result=loop.Parse(input, info);

		TEST_ASSERT(result);
		TEST_ASSERT(result.Value().Count()==2);
		ParsingList<wchar_t>::Node::Ref current=result.Value().Head();
		for(int i=0;i<2;i++)
		{
			TEST_ASSERT(current);
			TEST_ASSERT(current->Value()==L'v');
			current=current->Next();
		}
		TEST_ASSERT(!current);
		TEST_ASSERT(input.Current()==L'v');
		TEST_ASSERT(input.Index()==2);
	}
	{
		_Loop<StringInput<wchar_t>, wchar_t> loop(
			new _Unit<StringInput<wchar_t>>(L'v', L"这里需要v。"),
			0,
			3);
		StringInput<wchar_t> input=source;
		info.errors.Clear();
		ParsingResult<ParsingList<wchar_t>> result=loop.Parse(input, info);

		TEST_ASSERT(result);
		TEST_ASSERT(result.Value().Count()==3);
		ParsingList<wchar_t>::Node::Ref current=result.Value().Head();
		for(int i=0;i<3;i++)
		{
			TEST_ASSERT(current);
			TEST_ASSERT(current->Value()==L'v');
			current=current->Next();
		}
		TEST_ASSERT(!current);
		TEST_ASSERT(input.Current()==L'x');
		TEST_ASSERT(input.Index()==3);
	}
	{
		_Loop<StringInput<wchar_t>, wchar_t> loop(
			new _Unit<StringInput<wchar_t>>(L'v', L"这里需要v。"),
			4,
			-1);
		StringInput<wchar_t> input=source;
		info.errors.Clear();
		ParsingResult<ParsingList<wchar_t>> result=loop.Parse(input, info);

		TEST_ASSERT(result==false);
		TEST_ASSERT(info.errors[0]->Message()==L"这里需要v。");
		TEST_ASSERT(info.errors[0]->GetPosition().Current()==L'x');
		TEST_ASSERT(info.errors[0]->GetPosition().Index()==3);
		TEST_ASSERT(input.Current()==L'x');
		TEST_ASSERT(input.Index()==3);
	}
}

/***********************************************************************
_Using
***********************************************************************/

int _f(const wchar_t& w)
{
	return w+1;
}

TEST_CASE(TestUsingCombinator)
{
	WString string=L"vczh";
	StringInput<wchar_t> input=string;
	Types<StringInput<wchar_t>>::GlobalInfo info;

	info.errors.Clear();
	_Using<StringInput<wchar_t>, wchar_t, int> use1(
		new _Unit<StringInput<wchar_t>>(L'v', L"这里需要v。"),
		_f
		);
	StringInput<wchar_t> input1=input;
	ParsingResult<int> result1=use1.Parse(input1, info);
	TEST_ASSERT(result1==true);
	TEST_ASSERT(result1.Value()==L'w');
	TEST_ASSERT(info.errors.Count()==0);
	TEST_ASSERT(input1.Current()==L'c');
	TEST_ASSERT(input1.Index()==1);

	info.errors.Clear();
	_Using<StringInput<wchar_t>, wchar_t, int> use2(
		new _Unit<StringInput<wchar_t>>(L'c', L"这里需要c。"),
		_f
		);
	StringInput<wchar_t> input2=input;
	ParsingResult<int> result2=use2.Parse(input2, info);
	TEST_ASSERT(result2==false);
	TEST_ASSERT(info.errors.Count()==1);
	TEST_ASSERT(info.errors[0]->Message()==L"这里需要c。");
	TEST_ASSERT(info.errors[0]->GetPosition().Index()==0);
	TEST_ASSERT(info.errors[0]->GetPosition().Current()==L'v');
	TEST_ASSERT(input2.Current()==L'v');
	TEST_ASSERT(input2.Index()==0);
}

/***********************************************************************
_Error
***********************************************************************/

ParsingResult<wchar_t> _e1(StringInput<wchar_t>& input, Types<StringInput<wchar_t>>::GlobalInfo& globalInfo)
{
	return ParsingResult<wchar_t>();
}

ParsingResult<wchar_t> _e2(StringInput<wchar_t>& input, Types<StringInput<wchar_t>>::GlobalInfo& globalInfo)
{
	globalInfo.errors.Clear();
	globalInfo.errors.Add(new CombinatorError<StringInput<wchar_t>>(L"囧。", input));
	input.Next();
	return ParsingResult<wchar_t>(L'c');
}

TEST_CASE(TestErrorCombinator)
{
	WString string=L"vczh";
	StringInput<wchar_t> source=string;
	Types<StringInput<wchar_t>>::GlobalInfo info;
	{
		info.errors.Clear();
		_Error<StringInput<wchar_t>, wchar_t> error(
			new _Unit<StringInput<wchar_t>>(L'v', L"这里需要v。"),
			_e1
			);
		StringInput<wchar_t> input=source;
		ParsingResult<wchar_t> result=error.Parse(input, info);
		TEST_ASSERT(result==true);
		TEST_ASSERT(result.Value()==L'v');
		TEST_ASSERT(info.errors.Count()==0);
		TEST_ASSERT(input.Current()==L'c');
		TEST_ASSERT(input.Index()==1);
	}
	{
		info.errors.Clear();
		_Error<StringInput<wchar_t>, wchar_t> error(
			new _Unit<StringInput<wchar_t>>(L'v', L"这里需要v。"),
			_e2
			);
		StringInput<wchar_t> input=source;
		ParsingResult<wchar_t> result=error.Parse(input, info);
		TEST_ASSERT(result==true);
		TEST_ASSERT(result.Value()==L'v');
		TEST_ASSERT(info.errors.Count()==0);
		TEST_ASSERT(input.Current()==L'c');
		TEST_ASSERT(input.Index()==1);
	}
	{
		info.errors.Clear();
		_Error<StringInput<wchar_t>, wchar_t> error(
			new _Unit<StringInput<wchar_t>>(L'c', L"这里需要c。"),
			_e1
			);
		StringInput<wchar_t> input=source;
		ParsingResult<wchar_t> result=error.Parse(input, info);
		TEST_ASSERT(result==false);
		TEST_ASSERT(info.errors.Count()==1);
		TEST_ASSERT(info.errors[0]->Message()==L"这里需要c。");
		TEST_ASSERT(info.errors[0]->GetPosition().Index()==0);
		TEST_ASSERT(info.errors[0]->GetPosition().Current()==L'v');
		TEST_ASSERT(input.Current()==L'v');
		TEST_ASSERT(input.Index()==0);
	}
	{
		info.errors.Clear();
		_Error<StringInput<wchar_t>, wchar_t> error(
			new _Unit<StringInput<wchar_t>>(L'c', L"这里需要c。"),
			_e2
			);
		StringInput<wchar_t> input=source;
		ParsingResult<wchar_t> result=error.Parse(input, info);
		TEST_ASSERT(result==true);
		TEST_ASSERT(result.Value()==L'c');
		TEST_ASSERT(info.errors.Count()==1);
		TEST_ASSERT(info.errors[0]->Message()==L"囧。");
		TEST_ASSERT(info.errors[0]->GetPosition().Index()==0);
		TEST_ASSERT(info.errors[0]->GetPosition().Current()==L'v');
		TEST_ASSERT(input.Current()==L'c');
		TEST_ASSERT(input.Index()==1);
	}
}

/***********************************************************************
Combinator
***********************************************************************/

int cal(const int& first, const ParsingPair<wchar_t, int>& second)
{
	int result=first;
	switch(second.First())
	{
		case L'+':
			result+=second.Second();
			break;
		case L'-':
			result-=second.Second();
			break;
		case L'*':
			result*=second.Second();
			break;
		case L'/':
			result/=second.Second();
			break;
	}
	return result;
}

TEST_CASE(TestCombinator)
{
	typedef Rule<StringInput<wchar_t>, int> _Rule;
	typedef Node<StringInput<wchar_t>, int> _Node;

	_Rule FACTOR, TERM, EXP;
	FACTOR = rgx(L"/d+")[wtoi] | (ch(L'(')>>EXP<<ch(L')'));
	TERM = lrec(FACTOR + *(chs(L"*/") + FACTOR), cal);
	EXP = lrec(TERM + *(chs(L"+-") + TERM), cal);

	{
		Types<StringInput<wchar_t>>::GlobalInfo info;
		StringInput<wchar_t> input=L"(1+2)*(3+4)";
		ParsingResult<int> result=EXP.Parse(input, info);
		TEST_ASSERT(result);
		TEST_ASSERT(result.Value()==21);
		TEST_ASSERT(info.errors.Count()==0);
	}
	{
		TEST_ASSERT(EXP.Parse(L"(10+20)*(30+40)", false)==2100);
	}
}