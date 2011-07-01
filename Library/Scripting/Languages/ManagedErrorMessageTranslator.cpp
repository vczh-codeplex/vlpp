#include "ManagedErrorMessageTranslator.h"
#include "ManagedErrorMessage.h"

namespace vl
{
	namespace scripting
	{
		namespace managedlanguage
		{

/***********************************************************************
ManagedErrorMessageTranslator
***********************************************************************/

			Ptr<LanguageException> ManagedErrorMessageTranslator::Translate(Ptr<ManagedLanguageCodeException> error, vint codeIndex)
			{
				WString message;
				switch(error->GetExceptionCode())
				{
				case ManagedLanguageCodeException::SymbolAlreadyDefined:
					{
						message=ManagedErrorMessage::SymbolAlreadyDefined(error->GetParameters()[0]);
					}
					break;
				default:
					return 0;
				}
				return new LanguageException(
					message,
					error->GetBasicLanguageElement()->position.lineIndex,
					error->GetBasicLanguageElement()->position.lineStart,
					error->GetBasicLanguageElement()->position.start,
					codeIndex
					);
			}
		}
	}
}