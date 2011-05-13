#include "UnitTest.h"
#include "..\Console.h"
#include "..\Threading.h"

namespace vl
{
	namespace unittest
	{
		using namespace vl::console;

/***********************************************************************
UnitTest
***********************************************************************/

		SpinLock spinLockUnitTest;

		void UnitTest::PrintMessage(const WString& string)
		{
			SpinLock::Scope scope(spinLockUnitTest);
			Console::SetColor(false, true, false, true);
			Console::WriteLine(string);
			Console::SetColor(true, true, true, false);
		}

		void UnitTest::PrintInfo(const WString& string)
		{
			SpinLock::Scope scope(spinLockUnitTest);
			Console::SetColor(true, true, true, true);
			Console::WriteLine(string);
			Console::SetColor(true, true, true, false);
		}

		void UnitTest::PrintError(const WString& string)
		{
			SpinLock::Scope scope(spinLockUnitTest);
			Console::SetColor(true, false, false, true);
			Console::WriteLine(string);
			Console::SetColor(true, true, true, false);
		}
	}
}