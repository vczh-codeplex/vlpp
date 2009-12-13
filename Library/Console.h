/***********************************************************************
Vczh Library++ 3.0
Developer: �����(vczh)
UI::Console

***********************************************************************/

#ifndef VCZH_CONSOLE
#define VCZH_CONSOLE

#include "String.h"

namespace vl
{
	namespace console
	{
		class Console abstract
		{
		public:
			static void Write(const WString& string);
			static void WriteLine(const WString& string);
			static WString Read();
			static void SetColor(bool red, bool green, bool blue, bool light);
			static void SetTitle(const WString& string);
		};
	}
}

#endif