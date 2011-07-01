#include "ManagedErrorMessage.h"

namespace vl
{
	namespace scripting
	{
		namespace managedlanguage
		{
			WString ManagedErrorMessage::SymbolAlreadyDefined(const WString& name)
			{
				return L"Symbol "+name+L" already defined.";
			}

		}
	}
}
