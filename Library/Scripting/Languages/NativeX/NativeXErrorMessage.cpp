#include "NativeXErrorMessage.h"

namespace vl
{
	namespace scripting
	{
		namespace language_nativex
		{
			WString NativeXErrorMessage::UnrecognizedToken(const WString& token)
			{
				return L"Unrecognized Token \""+token+L".\"";
			}

			WString NativeXErrorMessage::UnitAlreadyExists(const WString& name)
			{
				return L"Unit "+name+L" already exists.";
			}

			WString NativeXErrorMessage::UnitNotExists(const WString& name)
			{
				return L"Unit "+name+L" not exists.";
			}

			WString NativeXErrorMessage::UnitCircularReferenced(const WString& name)
			{
				return L"Unit "+name+L" has circular unit imports.";
			}

		}
	}
}
