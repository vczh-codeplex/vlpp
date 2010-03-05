#include "Strings.h"

namespace vl
{
	namespace scripting
	{
		namespace basiclanguage
		{
			WString ErrorMessages::VariableAlreadyExists(const WString& variable)
			{
				return L"Variable \""+variable+L"\" already exists.";
			}

			WString ErrorMessages::VoidFunctionDoesNotHaveResult()
			{
				return L"Cannot use \"result\" in a function that returns void.";
			}

		}
	}
}
