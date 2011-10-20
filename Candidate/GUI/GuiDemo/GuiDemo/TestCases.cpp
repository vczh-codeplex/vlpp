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

TEST_CASE(SingleTextLine)
{
	TextLine line;
	{
		TEST_ASSERT(line.text==0);
		TEST_ASSERT(line.att==0);
		TEST_ASSERT(line.bufferLength==0);
		TEST_ASSERT(line.dataLength==0);
		TEST_ASSERT(line.availableOffsetCount==0);
	}

	line.Initialize();
	{
		TEST_ASSERT(line.text!=0);
		TEST_ASSERT(line.att!=0);
		TEST_ASSERT(line.bufferLength==TextLine::BlockSize);
		TEST_ASSERT(line.dataLength==0);
		TEST_ASSERT(line.availableOffsetCount==0);
	}

	TEST_ASSERT(line.Modify(-1, -1, 0, 0)==false);
	TEST_ASSERT(line.Modify(-1, 0, 0, 0)==false);
	TEST_ASSERT(line.Modify(-1, 1, 0, 0)==false);
	TEST_ASSERT(line.Modify(0, -1, 0, 0)==false);
	TEST_ASSERT(line.Modify(0, 0, 0, 0)==true);
	TEST_ASSERT(line.Modify(0, 1, 0, 0)==false);
	TEST_ASSERT(line.Modify(1, -1, 0, 0)==false);
	TEST_ASSERT(line.Modify(1, 0, 0, 0)==false);
	TEST_ASSERT(line.Modify(1, 1, 0, 0)==false);
	{
		TEST_ASSERT(line.text!=0);
		TEST_ASSERT(line.att!=0);
		TEST_ASSERT(line.bufferLength==TextLine::BlockSize);
		TEST_ASSERT(line.dataLength==0);
		TEST_ASSERT(line.availableOffsetCount==0);
	}

	TEST_ASSERT(line.Modify(0, 0, L"ABCDEFGHIJKLMNOPQRSTUVWXYZ", 26)==true);
	{
		TEST_ASSERT(wcsncmp(line.text, L"ABCDEFGHIJKLMNOPQRSTUVWXYZ", 26)==0);
		TEST_ASSERT(line.att!=0);
		TEST_ASSERT(line.bufferLength==TextLine::BlockSize);
		TEST_ASSERT(line.dataLength==26);
		TEST_ASSERT(line.availableOffsetCount==0);
	}

	TEST_ASSERT(line.Modify(13, 0, L"abcdefghijklmnopqrstuvwxyz", 26));
	{
		TEST_ASSERT(wcsncmp(line.text, L"ABCDEFGHIJKLMabcdefghijklmnopqrstuvwxyzNOPQRSTUVWXYZ", 52)==0);
		TEST_ASSERT(line.att!=0);
		TEST_ASSERT(line.bufferLength==TextLine::BlockSize*2);
		TEST_ASSERT(line.dataLength==52);
		TEST_ASSERT(line.availableOffsetCount==0);
	}

	TEST_ASSERT(line.Modify(13, 26, 0, 0));
	{
		TEST_ASSERT(wcsncmp(line.text, L"ABCDEFGHIJKLMNOPQRSTUVWXYZ", 26)==0);
		TEST_ASSERT(line.att!=0);
		TEST_ASSERT(line.bufferLength==TextLine::BlockSize);
		TEST_ASSERT(line.dataLength==26);
		TEST_ASSERT(line.availableOffsetCount==0);
	}

	TEST_ASSERT(line.Modify(0, 7, 0, 0));
	{
		TEST_ASSERT(wcsncmp(line.text, L"HIJKLMNOPQRSTUVWXYZ", 19)==0);
		TEST_ASSERT(line.att!=0);
		TEST_ASSERT(line.bufferLength==TextLine::BlockSize);
		TEST_ASSERT(line.dataLength==19);
		TEST_ASSERT(line.availableOffsetCount==0);
	}

	TEST_ASSERT(line.Modify(13, 6, 0, 0));
	{
		TEST_ASSERT(wcsncmp(line.text, L"HIJKLMNOPQRST", 13)==0);
		TEST_ASSERT(line.att!=0);
		TEST_ASSERT(line.bufferLength==TextLine::BlockSize);
		TEST_ASSERT(line.dataLength==13);
		TEST_ASSERT(line.availableOffsetCount==0);
	}

	TEST_ASSERT(line.Modify(0, 13, 0, 0));
	{
		TEST_ASSERT(line.text!=0);
		TEST_ASSERT(line.att!=0);
		TEST_ASSERT(line.bufferLength==TextLine::BlockSize);
		TEST_ASSERT(line.dataLength==0);
		TEST_ASSERT(line.availableOffsetCount==0);
	}

	line.Finalize();
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
		{
			TEST_ASSERT(wcsncmp(line1.text, L"ABCDEFGHIJKLMN", 14)==0);
			TEST_ASSERT(line1.att!=0);
			TEST_ASSERT(line1.bufferLength==TextLine::BlockSize);
			TEST_ASSERT(line1.dataLength==14);
			TEST_ASSERT(line1.availableOffsetCount==0);
		}
		{
			TEST_ASSERT(line2.text==0);
			TEST_ASSERT(line2.att==0);
			TEST_ASSERT(line2.bufferLength==0);
			TEST_ASSERT(line2.dataLength==0);
			TEST_ASSERT(line2.availableOffsetCount==0);
		}
		line1.Finalize();
	}
	{
		TextLine line1;
		line1.Initialize();
		line1.Modify(0, 0, L"ABCDEFGHIJKLMN", 14);
		TextLine line2=line1.Split(7);
		{
			TEST_ASSERT(wcsncmp(line1.text, L"ABCDEFG", 7)==0);
			TEST_ASSERT(line1.att!=0);
			TEST_ASSERT(line1.bufferLength==TextLine::BlockSize);
			TEST_ASSERT(line1.dataLength==7);
			TEST_ASSERT(line1.availableOffsetCount==0);
		}
		{
			TEST_ASSERT(wcsncmp(line2.text, L"HIJKLMN", 7)==0);
			TEST_ASSERT(line2.att!=0);
			TEST_ASSERT(line2.bufferLength==TextLine::BlockSize);
			TEST_ASSERT(line2.dataLength==7);
			TEST_ASSERT(line2.availableOffsetCount==0);
		}
		line1.Finalize();
		line2.Finalize();
	}
}

TEST_CASE(TextLines)
{
}