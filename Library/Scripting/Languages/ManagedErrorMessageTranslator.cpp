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
				case ManagedLanguageCodeException::TypeInvisible:
					{
						message=ManagedErrorMessage::TypeInvisible(TypeToString(error->GetTypeParameter()));
					}
					break;
				case ManagedLanguageCodeException::TypeInvisibleOutSideOfAssembly:
					{
						message=ManagedErrorMessage::TypeInvisibleOutSideOfAssembly(TypeToString(error->GetTypeParameter()));
					}
					break;
				case ManagedLanguageCodeException::CannotUseUninstantiatedGenericType:
					{
						message=ManagedErrorMessage::CannotUseUninstantiatedGenericType(TypeToString(error->GetTypeParameter()));
					}
					break;
				case ManagedLanguageCodeException::GenericTypeArgumentCountNotMatches:
					{
						message=ManagedErrorMessage::GenericTypeArgumentCountNotMatches(TypeToString(error->GetTypeParameter()));
					}
					break;
				case ManagedLanguageCodeException::GenericTypeTypeConstraintNotSatisfied:
					{
						vint index=wtoi(error->GetParameters()[0])+1;
						message=ManagedErrorMessage::GenericTypeTypeConstraintNotSatisfied(TypeToString(error->GetTypeParameter()), itow(index));
					}
					break;
				case ManagedLanguageCodeException::GenericTypeNewConstraintNotSatisfied:
					{
						vint index=wtoi(error->GetParameters()[0])+1;
						message=ManagedErrorMessage::GenericTypeNewConstraintNotSatisfied(TypeToString(error->GetTypeParameter()), itow(index));
					}
					break;
				case ManagedLanguageCodeException::ExpressionCannotConvertToType:
					{
						message=ManagedErrorMessage::ExpressionCannotConvertToType(TypeToString(error->GetTypeParameter()));
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