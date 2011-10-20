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

TEST_CASE(TextLine)
{
}

TEST_CASE(TextLines)
{
}