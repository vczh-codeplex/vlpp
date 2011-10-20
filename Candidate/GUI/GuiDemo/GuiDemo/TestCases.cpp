#include "..\..\GUI\GraphicsElement\GuiGraphicsElement.h"

using namespace vl;
using namespace vl::presentation;
using namespace vl::presentation::elements;
using namespace vl::presentation::elements::text;

#define TEST_CASE(NAME)\
	void TestExecution_##NAME(void);\
	class TestCase_##NAME\
	{\
	public:\
		TestCase_##NAME()\
		{\
			TestExecution_##NAME();\
		}\
	} instance_##NAME;\
	void TestExecution_##NAME(void)

#define TEST_ASSERT(CONDITION) do{if(!(CONDITION)) throw 0;}while(false)

void AssertEmpty(TextLine line)
{
	TEST_ASSERT(line.text==0);
	TEST_ASSERT(line.att==0);
	TEST_ASSERT(line.bufferLength==0);
	TEST_ASSERT(line.dataLength==0);
	TEST_ASSERT(line.availableOffsetCount==0);
}

void AssertText(TextLine line, const WString& text)
{
	int length=text.Length();
	int buffer=0;
	if(length%TextLine::BlockSize)
	{
		buffer=(length/TextLine::BlockSize+1)*TextLine::BlockSize;
	}
	if(buffer==0)
	{
		buffer=TextLine::BlockSize;
	}

	TEST_ASSERT(wcsncmp(line.text,text.Buffer(), length)==0);
	TEST_ASSERT(line.att!=0);
	TEST_ASSERT(line.bufferLength==buffer);
	TEST_ASSERT(line.dataLength==length);
	TEST_ASSERT(line.availableOffsetCount==0);
}

TEST_CASE(SingleTextLine)
{
	TextLine line;
	AssertEmpty(line);

	line.Initialize();
	AssertText(line, L"");

	TEST_ASSERT(line.Modify(-1, -1, 0, 0)==false);
	TEST_ASSERT(line.Modify(-1, 0, 0, 0)==false);
	TEST_ASSERT(line.Modify(-1, 1, 0, 0)==false);
	TEST_ASSERT(line.Modify(0, -1, 0, 0)==false);
	TEST_ASSERT(line.Modify(0, 0, 0, 0)==true);
	TEST_ASSERT(line.Modify(0, 1, 0, 0)==false);
	TEST_ASSERT(line.Modify(1, -1, 0, 0)==false);
	TEST_ASSERT(line.Modify(1, 0, 0, 0)==false);
	TEST_ASSERT(line.Modify(1, 1, 0, 0)==false);
	AssertText(line, L"");

	TEST_ASSERT(line.Modify(0, 0, L"ABCDEFGHIJKLMNOPQRSTUVWXYZ", 26)==true);
	AssertText(line, L"ABCDEFGHIJKLMNOPQRSTUVWXYZ");

	TEST_ASSERT(line.Modify(13, 0, L"abcdefghijklmnopqrstuvwxyz", 26));
	AssertText(line, L"ABCDEFGHIJKLMabcdefghijklmnopqrstuvwxyzNOPQRSTUVWXYZ");

	TEST_ASSERT(line.Modify(13, 26, 0, 0));
	AssertText(line, L"ABCDEFGHIJKLMNOPQRSTUVWXYZ");

	TEST_ASSERT(line.Modify(0, 7, 0, 0));
	AssertText(line, L"HIJKLMNOPQRSTUVWXYZ");

	TEST_ASSERT(line.Modify(13, 6, 0, 0));
	AssertText(line, L"HIJKLMNOPQRST");

	TEST_ASSERT(line.Modify(0, 13, 0, 0));
	AssertText(line, L"");

	line.Finalize();
	AssertEmpty(line);
}

TEST_CASE(MultipleTextLine)
{
	{
		TextLine line1, line2;
		line1.Initialize();
		line2.Initialize();
		line1.Modify(0, 0, L"ABCDEFG", 7);
		line2.Modify(0, 0, L"HIJKLMN", 7);
		line1.AppendAndFinalize(line2);
		AssertText(line1, L"ABCDEFGHIJKLMN");
		AssertEmpty(line2);
		line1.Finalize();
	}
	{
		TextLine line1;
		line1.Initialize();
		line1.Modify(0, 0, L"ABCDEFGHIJKLMN", 14);
		TextLine line2=line1.Split(7);
		AssertText(line1, L"ABCDEFG");
		AssertText(line2, L"HIJKLMN");
		line1.Finalize();
		line2.Finalize();
	}
}

TEST_CASE(TextLines)
{
	TextLines lines;
	{
		TEST_ASSERT(lines.lines.Count()==1);
		AssertText(lines.lines[0], L"");
	}

	TEST_ASSERT(lines.Modify(TextPos(0, 0), TextPos(0, 0), L"")==true);
	{
		TEST_ASSERT(lines.lines.Count()==1);
		AssertText(lines.lines[0], L"");
	}

	TEST_ASSERT(lines.Modify(TextPos(0, 0), TextPos(0, 0), L"ABCDEFG")==true);
	{
		TEST_ASSERT(lines.lines.Count()==1);
		AssertText(lines.lines[0], L"ABCDEFG");
	}

	TEST_ASSERT(lines.Modify(TextPos(0, 7), TextPos(0, 7), L"\r\nHIJKLMN")==true);
	{
		TEST_ASSERT(lines.lines.Count()==2);
		AssertText(lines.lines[0], L"ABCDEFG");
		AssertText(lines.lines[1], L"HIJKLMN");
	}

	TEST_ASSERT(lines.Modify(TextPos(1, 7), TextPos(1, 7), L"\r\n")==true);
	{
		TEST_ASSERT(lines.lines.Count()==3);
		AssertText(lines.lines[0], L"ABCDEFG");
		AssertText(lines.lines[1], L"HIJKLMN");
		AssertText(lines.lines[2], L"");
	}

	TEST_ASSERT(lines.Modify(TextPos(2, 0), TextPos(2, 0), L"OPQRST\r\nUVWXYZ")==true);
	{
		TEST_ASSERT(lines.lines.Count()==4);
		AssertText(lines.lines[0], L"ABCDEFG");
		AssertText(lines.lines[1], L"HIJKLMN");
		AssertText(lines.lines[2], L"OPQRST");
		AssertText(lines.lines[3], L"UVWXYZ");
	}

	TEST_ASSERT(lines.Modify(TextPos(1, 3), TextPos(2, 3), L"vczh"));
	{
		TEST_ASSERT(lines.lines.Count()==3);
		AssertText(lines.lines[0], L"ABCDEFG");
		AssertText(lines.lines[1], L"HIJvczhRST");
		AssertText(lines.lines[2], L"UVWXYZ");
	}

	TEST_ASSERT(lines.Modify(TextPos(1, 3), TextPos(1, 7), L"KLMN\r\nOPQ"));
	{
		TEST_ASSERT(lines.lines.Count()==4);
		AssertText(lines.lines[0], L"ABCDEFG");
		AssertText(lines.lines[1], L"HIJKLMN");
		AssertText(lines.lines[2], L"OPQRST");
		AssertText(lines.lines[3], L"UVWXYZ");
	}

	TEST_ASSERT(lines.Modify(TextPos(0, 3), TextPos(3, 3), L"vczh"));
	{
		TEST_ASSERT(lines.lines.Count()==1);
		AssertText(lines.lines[0], L"ABCvczhXYZ");
	}

	TEST_ASSERT(lines.Modify(TextPos(0, 0), TextPos(0, 10), L"ABCDEFG\r\nHIJKLMN\r\nOPQRST\r\nUVWXYZ"));
	{
		TEST_ASSERT(lines.lines.Count()==4);
		AssertText(lines.lines[0], L"ABCDEFG");
		AssertText(lines.lines[1], L"HIJKLMN");
		AssertText(lines.lines[2], L"OPQRST");
		AssertText(lines.lines[3], L"UVWXYZ");
	}

	TEST_ASSERT(lines.Modify(TextPos(0, 0), TextPos(3, 6), L"12345\r\n67890"));
	{
		TEST_ASSERT(lines.lines.Count()==2);
		AssertText(lines.lines[0], L"12345");
		AssertText(lines.lines[1], L"67890");
	}

	lines.Clear();
	{
		TEST_ASSERT(lines.lines.Count()==1);
		AssertText(lines.lines[0], L"");
	}
}