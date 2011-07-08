#include "ManagedErrorMessageTranslator.h"
#include "ManagedErrorMessage.h"

namespace vl
{
	namespace scripting
	{
		namespace managedlanguage
		{
			using namespace collections;

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
						message=ManagedErrorMessage::NamespaceNotExists(SymbolsToString(declaration->namespaceFragments.Wrap()));
					}
					break;
				case ManagedLanguageCodeException::SystemTypeNotExists:
					{
						Array<WString> symbols(2);
						symbols[0]=L"System";
						symbols[1]=error->GetParameters()[0];
						message=ManagedErrorMessage::FullPathTypeNotExists(SymbolsToString(symbols.Wrap()));
					}
					break;
				case ManagedLanguageCodeException::SystemTypeDuplicated:
					{
						Array<WString> symbols(2);
						symbols[0]=L"System";
						symbols[1]=error->GetParameters()[0];
						message=ManagedErrorMessage::FullPathTypeDuplicated(SymbolsToString(symbols.Wrap()));
					}
					break;
				case ManagedLanguageCodeException::TypeNotExists:
					{
						message=ManagedErrorMessage::ScopedTypeNotExists(TypeToString(dynamic_cast<ManagedType*>(error->GetManagedLanguageElement())));
					}
					break;
				case ManagedLanguageCodeException::TypeDuplicated:
					{
						message=ManagedErrorMessage::ScopedTypeDuplicated(TypeToString(dynamic_cast<ManagedType*>(error->GetManagedLanguageElement())));
					}
					break;
				case ManagedLanguageCodeException::IllegalAutoRefer:
					{
						message=ManagedErrorMessage::IllegalAutoRefer(TypeToString(dynamic_cast<ManagedAutoReferType*>(error->GetManagedLanguageElement())));
					}
					break;
				case ManagedLanguageCodeException::CannotDirectReferTypeRenameAsToplevel:
					{
						message=ManagedErrorMessage::CannotDirectReferTypeRenameAsToplevel(error->GetParameters()[0]);
					}
					break;
				case ManagedLanguageCodeException::IllegalNestedDeclaration:
					{
						message=ManagedErrorMessage::IllegalNestedDeclaration();
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