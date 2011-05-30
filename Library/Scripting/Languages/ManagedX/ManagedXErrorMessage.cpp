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

			WString ManagedXErrorMessage::NeedId()
			{
				return L"Here needs an identifier.";
			}

			WString ManagedXErrorMessage::NeedOpenDeclBrace()
			{
				return L"Here needs \"{\".";
			}

			WString ManagedXErrorMessage::NeedCloseDeclBrace()
			{
				return L"Here needs \"}\".";
			}

			WString ManagedXErrorMessage::NeedSemicolon()
			{
				return L"Here needs \";\".";
			}

		}
	}
}
