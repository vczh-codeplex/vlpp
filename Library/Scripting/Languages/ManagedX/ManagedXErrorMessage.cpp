#include "ManagedXErrorMessage.h"

namespace vl
{
	namespace scripting
	{
		namespace language_managedx
		{
			WString ManagedXErrorMessage::UnrecognizedToken(const WString& token)
			{
				return L"Unrecognized Token \""+token+L"\".";
			}

		}
	}
}
