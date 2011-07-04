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
				case ManagedLanguageCodeException::NamespaceNotExists:
					{
						ManagedUsingNamespaceDeclaration* declaration=dynamic_cast<ManagedUsingNamespaceDeclaration*>(error->GetManagedLanguageElement());
						message=ManagedErrorMessage::NamespaceNotExists(NamespaceToString(declaration->namespaceFragments.Wrap()));
					}
					break;
				case ManagedLanguageCodeException::SystemTypeNotExists:
					{
						message=ManagedErrorMessage::FullPathTypeNotExists(SystemTypeToString(error->GetParameters()[0]));
					}
					break;
				case ManagedLanguageCodeException::SystemTypeDuplicated:
					{
						message=ManagedErrorMessage::FullPathTypeDuplicated(SystemTypeToString(error->GetParameters()[0]));
					}
					break;
				default:
					return 0;
				}
				return new LanguageException(
					message,
					error->GetManagedLanguageElement()->position.lineIndex,
					error->GetManagedLanguageElement()->position.lineStart,
					error->GetManagedLanguageElement()->position.start,
					codeIndex
					);
			}
		}
	}
}