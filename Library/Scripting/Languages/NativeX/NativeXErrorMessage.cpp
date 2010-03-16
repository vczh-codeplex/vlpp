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

		}
	}
}
