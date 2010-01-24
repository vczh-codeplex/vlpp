/***********************************************************************
Vczh Library++ 3.0
Developer: ³Âè÷å«(vczh)
UI::Console

***********************************************************************/

#ifndef VCZH_UNITTEST
#define VCZH_UNITTEST

#include "..\String.h"

class UnitTestError
{
};

namespace vl
{
	namespace unittest
	{
		class UnitTest abstract
		{
		public:
			static void PrintMessage(const WString& string);
			static void PrintInfo(const WString& string);
			static void PrintError(const WString& string);
		};

#define TEST_ASSERT(CONDITION) do{CHECK_ERROR(CONDITION,L"");}while(0)
#define TEST_ERROR(CONDITION) do{try{CONDITION;throw UnitTestError();}catch(const Error&){}catch(const UnitTestError&){CHECK_ERROR(false,L"");}}while(0)
#define TEST_CASE(NAME)\
		extern void TESTCASE_##NAME();								\
		namespace vl_unittest_executors								\
		{															\
			class TESTCASE_RUNNER_##NAME							\
			{														\
			public:													\
				TESTCASE_RUNNER_##NAME()							\
				{													\
					vl::unittest::UnitTest::PrintMessage(L#NAME);	\
					TESTCASE_##NAME();								\
				}													\
			} TESTCASE_RUNNER_##NAME##_INSTANCE;					\
		}															\
		void TESTCASE_##NAME()
#define TEST_PRINT(x) vl::unittest::UnitTest::PrintMessage(x)
#define TEST_EXCEPTION(STATEMENT,EXCEPTION,ASSERT_FUNCTION) try{STATEMENT; TEST_ASSERT(false);}catch(const EXCEPTION& e){ASSERT_FUNCTION(e);}
	}
}

#endif