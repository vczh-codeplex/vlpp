#include "UnitTest.h"
#include "..\Console.h"

namespace vl
{
	namespace unittest
	{
		using namespace vl::console;

/***********************************************************************
UnitTest
***********************************************************************/

		void UnitTest::PrintMessage(const WString& string)
		{
			Console::SetColor(false, true, false, true);
			Console::WriteLine(string);
			Console::SetColor(true, true, true, false);
		}

		void UnitTest::PrintInfo(const WString& string)
		{
			Console::SetColor(true, true, true, true);
			Console::WriteLine(string);
			Console::SetColor(true, true, true, false);
		}

		void UnitTest::PrintError(const WString& string)
		{
			Console::SetColor(true, false, false, true);
			Console::WriteLine(string);
			Console::SetColor(true, true, true, false);
		}
	}
}