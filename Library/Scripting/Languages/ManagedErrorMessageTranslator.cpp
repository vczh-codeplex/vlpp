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
				case ManagedLanguageCodeException::ExpressionResolvedToDuplicatedTargets:
					{
						ManagedTypeSymbol* type=error->GetTypeParameter();
						if(type)
						{
							message=ManagedErrorMessage::ExpressionResolvedToDuplicatedTargets(TypeToString(type));
						}
						else
						{
							message=ManagedErrorMessage::ExpressionResolvedToDuplicatedTargets();
						}
					}
					break;
				case ManagedLanguageCodeException::ExpressionResolvingFailed:
					{
						message=ManagedErrorMessage::ExpressionResolvingFailed(error->GetParameters()[0]);
					}
					break;
				case ManagedLanguageCodeException::ExpressionCannotConvertToType:
					{
						message=ManagedErrorMessage::ExpressionCannotConvertToType(TypeToString(error->GetTypeParameter()));
					}
					break;
				case ManagedLanguageCodeException::IllegalAutoRefWithoutInitializer:
					{
						WString name=error->GetParameters()[0];
						WString autoref=TypeToString(dynamic_cast<ManagedVariableStatement*>(error->GetManagedLanguageElement())->type.Obj());
						message=ManagedErrorMessage::IllegalAutoRefWithoutInitializer(name, autoref);
					}
					break;
				case ManagedLanguageCodeException::VariableAlreadyExists:
					{
						WString name=error->GetParameters()[0];
						message=ManagedErrorMessage::VariableAlreadyExists(name);
					}
					break;
				case ManagedLanguageCodeException::IllegalBreak:
					{
						message=ManagedErrorMessage::IllegalBreak();
					}
					break;
				case ManagedLanguageCodeException::IllegalContinue:
					{
						message=ManagedErrorMessage::IllegalContinue();
					}
					break;
				case ManagedLanguageCodeException::IllegalThrow:
					{
						message=ManagedErrorMessage::IllegalThrow();
					}
					break;
				case ManagedLanguageCodeException::ExceptionTypeShouldDerivedFromException:
					{
						WString exceptionType;
						if(ManagedType* type=dynamic_cast<ManagedType*>(error->GetManagedLanguageElement()))
						{
							exceptionType=TypeToString(type);
						}
						else if(ManagedExpression* expression=dynamic_cast<ManagedExpression*>(error->GetManagedLanguageElement()))
						{
							exceptionType=TypeToString(ExpressionToType(expression));
						}
						message=ManagedErrorMessage::ExceptionTypeShouldDerivedFromException(exceptionType, TypeToString(error->GetTypeParameter()));
					}
					break;
				case ManagedLanguageCodeException::IllegalThis:
					{
						message=ManagedErrorMessage::IllegalThis(ExpressionToString(dynamic_cast<ManagedExpression*>(error->GetManagedLanguageElement())));
					}
					break;
				case ManagedLanguageCodeException::IllegalBase:
					{
						message=ManagedErrorMessage::IllegalBase(ExpressionToString(dynamic_cast<ManagedExpression*>(error->GetManagedLanguageElement())));
					}
					break;
				case ManagedLanguageCodeException::ExpressionIsNotValue:
					{
						message=ManagedErrorMessage::ExpressionIsNotValue(ExpressionToString(dynamic_cast<ManagedExpression*>(error->GetManagedLanguageElement())));
					}
					break;
				case ManagedLanguageCodeException::MatchedMethodNotExists:
					{
						message=ManagedErrorMessage::MatchedMethodNotExists(ExpressionToString(dynamic_cast<ManagedExpression*>(error->GetManagedLanguageElement())));
					}
					break;
				case ManagedLanguageCodeException::MatchedMethodTooMuch:
					{
						message=ManagedErrorMessage::MatchedMethodTooMuch(ExpressionToString(dynamic_cast<ManagedExpression*>(error->GetManagedLanguageElement())));
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