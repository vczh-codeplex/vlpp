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

/***********************************************************************
ManagedLanguageCodeException::ExceptionCode::ScopeBuilding
***********************************************************************/

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

/***********************************************************************
ManagedLanguageCodeException::ExceptionCode::DeclarationTypeChecking
***********************************************************************/
					
				case ManagedLanguageCodeException::IllegalTypeConstraint:
					{
						message=ManagedErrorMessage::IllegalTypeConstraint(error->GetParameters()[0]);
					}
					break;
				case ManagedLanguageCodeException::TypeCannotDerivedFromItself:
					{
						ManagedTypeDeclaration* declaration=dynamic_cast<ManagedTypeDeclaration*>(error->GetManagedLanguageElement());
						message=ManagedErrorMessage::TypeCannotDerivedFromItself(declaration->name);
					}
					break;
				case ManagedLanguageCodeException::ClassIllegalBaseClass:
					{
						ManagedTypeDeclaration* declaration=dynamic_cast<ManagedTypeDeclaration*>(error->GetManagedLanguageElement());
						message=ManagedErrorMessage::ClassIllegalBaseClass(declaration->name);
					}
					break;
				case ManagedLanguageCodeException::StructureIllegalBaseClass:
					{
						ManagedTypeDeclaration* declaration=dynamic_cast<ManagedTypeDeclaration*>(error->GetManagedLanguageElement());
						message=ManagedErrorMessage::StructureIllegalBaseClass(declaration->name);
					}
					break;
				case ManagedLanguageCodeException::InterfaceIllegalBaseClass:
					{
						ManagedTypeDeclaration* declaration=dynamic_cast<ManagedTypeDeclaration*>(error->GetManagedLanguageElement());
						message=ManagedErrorMessage::InterfaceIllegalBaseClass(declaration->name);
					}
					break;
				case ManagedLanguageCodeException::StaticTypeIllegalBaseClass:
					{
						ManagedTypeDeclaration* declaration=dynamic_cast<ManagedTypeDeclaration*>(error->GetManagedLanguageElement());
						message=ManagedErrorMessage::StaticTypeIllegalBaseClass(declaration->name);
					}
					break;
				case ManagedLanguageCodeException::ClassStructureTooMuchBaseClasses:
					{
						ManagedTypeDeclaration* declaration=dynamic_cast<ManagedTypeDeclaration*>(error->GetManagedLanguageElement());
						message=ManagedErrorMessage::ClassStructureTooMuchBaseClasses(declaration->name);
					}
					break;
				case ManagedLanguageCodeException::ClassStructureIllegalInheritation:
					{
						ManagedTypeDeclaration* declaration=dynamic_cast<ManagedTypeDeclaration*>(error->GetManagedLanguageElement());
						message=ManagedErrorMessage::ClassStructureIllegalInheritation(declaration->name);
					}
					break;
				case ManagedLanguageCodeException::InterfaceIllegalInheritation:
					{
						ManagedTypeDeclaration* declaration=dynamic_cast<ManagedTypeDeclaration*>(error->GetManagedLanguageElement());
						message=ManagedErrorMessage::InterfaceIllegalInheritation(declaration->name);
					}
					break;
				case ManagedLanguageCodeException::InterfaceIllegalMemberType:
					{
						ManagedTypeDeclaration* declaration=dynamic_cast<ManagedTypeDeclaration*>(error->GetManagedLanguageElement());
						message=ManagedErrorMessage::InterfaceIllegalMemberType(declaration->name);
					}
					break;
				case ManagedLanguageCodeException::StaticTypeIllegalInheritance:
					{
						ManagedTypeDeclaration* declaration=dynamic_cast<ManagedTypeDeclaration*>(error->GetManagedLanguageElement());
						message=ManagedErrorMessage::StaticTypeIllegalInheritance(declaration->name);
					}
					break;
				case ManagedLanguageCodeException::TypeIllegalAccessor:
					{
						ManagedTypeDeclaration* declaration=dynamic_cast<ManagedTypeDeclaration*>(error->GetManagedLanguageElement());
						message=ManagedErrorMessage::TypeIllegalAccessor(declaration->name);
					}
					break;
				case ManagedLanguageCodeException::InterfaceMemberNotImplemented:
					{
						ManagedTypeDeclaration* declaration=dynamic_cast<ManagedTypeDeclaration*>(error->GetManagedLanguageElement());
						ManagedTypeSymbol* memberType=error->GetMemberTypeParameter();
						ManagedSymbolItem* memberSymbol=error->GetMemberSymbolParameter();
						message=ManagedErrorMessage::InterfaceMemberNotImplemented(declaration->name, TypeToString(memberType), MemberHeader(memberSymbol));
					}
					break;
				case ManagedLanguageCodeException::AbstractMemberNotImplemented:
					{
						ManagedTypeDeclaration* declaration=dynamic_cast<ManagedTypeDeclaration*>(error->GetManagedLanguageElement());
						ManagedTypeSymbol* memberType=error->GetMemberTypeParameter();
						ManagedSymbolItem* memberSymbol=error->GetMemberSymbolParameter();
						message=ManagedErrorMessage::AbstractMemberNotImplemented(declaration->name, TypeToString(memberType), MemberHeader(memberSymbol));
					}
					break;

/***********************************************************************
ManagedLanguageCodeException::ExceptionCode::DeclarationMemberChecking
***********************************************************************/

				case ManagedLanguageCodeException::InterfaceMemberIllegalType:
					{
						ManagedMember* member=dynamic_cast<ManagedMember*>(error->GetManagedLanguageElement());
						message=ManagedErrorMessage::InterfaceMemberIllegalInheritation(MemberContainingTypeName(member), MemberName(member));
					}
					break;
				case ManagedLanguageCodeException::InterfaceMemberIllegalAccessor:
					{
						ManagedMember* member=dynamic_cast<ManagedMember*>(error->GetManagedLanguageElement());
						message=ManagedErrorMessage::InterfaceMemberIllegalAccessor(MemberContainingTypeName(member), MemberName(member));
					}
					break;
				case ManagedLanguageCodeException::InterfaceMemberIllegalMemberType:
					{
						ManagedMember* member=dynamic_cast<ManagedMember*>(error->GetManagedLanguageElement());
						message=ManagedErrorMessage::InterfaceMemberIllegalMemberType(MemberContainingTypeName(member), MemberName(member));
					}
					break;
				case ManagedLanguageCodeException::InterfaceMemberIllegalInheritation:
					{
						ManagedMember* member=dynamic_cast<ManagedMember*>(error->GetManagedLanguageElement());
						message=ManagedErrorMessage::InterfaceMemberIllegalInheritation(MemberContainingTypeName(member), MemberName(member));
					}
					break;
				case ManagedLanguageCodeException::InterfaceMemberIllegalImplementedInterface:
					{
						ManagedMember* member=dynamic_cast<ManagedMember*>(error->GetManagedLanguageElement());
						message=ManagedErrorMessage::InterfaceMemberIllegalImplementedInterface(MemberContainingTypeName(member), MemberName(member));
					}
					break;
				case ManagedLanguageCodeException::SealedTypeMemberIllegalAccessor:
					{
						ManagedMember* member=dynamic_cast<ManagedMember*>(error->GetManagedLanguageElement());
						message=ManagedErrorMessage::SealedTypeMemberIllegalAccessor(MemberContainingTypeName(member), MemberName(member));
					}
					break;
				case ManagedLanguageCodeException::SealedTypeMemberIllegalInheritance:
					{
						ManagedMember* member=dynamic_cast<ManagedMember*>(error->GetManagedLanguageElement());
						message=ManagedErrorMessage::SealedTypeMemberIllegalInheritance(MemberContainingTypeName(member), MemberName(member));
					}
					break;
				case ManagedLanguageCodeException::StaticTypeMemberIllegalAccessor:
					{
						ManagedMember* member=dynamic_cast<ManagedMember*>(error->GetManagedLanguageElement());
						message=ManagedErrorMessage::StaticTypeMemberIllegalAccessor(MemberContainingTypeName(member), MemberName(member));
					}
					break;
				case ManagedLanguageCodeException::StaticTypeMemberIllegalInheritance:
					{
						ManagedMember* member=dynamic_cast<ManagedMember*>(error->GetManagedLanguageElement());
						message=ManagedErrorMessage::StaticTypeMemberIllegalInheritance(MemberContainingTypeName(member), MemberName(member));
					}
					break;
				case ManagedLanguageCodeException::StaticTypeMemberIllegalMemberType:
					{
						ManagedMember* member=dynamic_cast<ManagedMember*>(error->GetManagedLanguageElement());
						message=ManagedErrorMessage::StaticTypeMemberIllegalMemberType(MemberContainingTypeName(member), MemberName(member));
					}
					break;
				case ManagedLanguageCodeException::NonAbstractTypeMemberIllegalInheritance:
					{
						ManagedMember* member=dynamic_cast<ManagedMember*>(error->GetManagedLanguageElement());
						message=ManagedErrorMessage::NonAbstractTypeMemberIllegalInheritance(MemberContainingTypeName(member), MemberName(member));
					}
					break;
				case ManagedLanguageCodeException::InterfaceImplementedMemberIllegalInheritation:
					{
						ManagedMember* member=dynamic_cast<ManagedMember*>(error->GetManagedLanguageElement());
						message=ManagedErrorMessage::InterfaceImplementedMemberIllegalInheritation(MemberContainingTypeName(member), MemberName(member));
					}
					break;
				case ManagedLanguageCodeException::InterfaceImplementedMemberIllegalMemberType:
					{
						ManagedMember* member=dynamic_cast<ManagedMember*>(error->GetManagedLanguageElement());
						message=ManagedErrorMessage::InterfaceImplementedMemberIllegalMemberType(MemberContainingTypeName(member), MemberName(member));
					}
					break;
				case ManagedLanguageCodeException::MemberImplementedInterfaceTypeNotExists:
					{
						ManagedMember* member=dynamic_cast<ManagedMember*>(error->GetManagedLanguageElement());
						message=ManagedErrorMessage::MemberImplementedInterfaceTypeNotExists(MemberContainingTypeName(member), MemberName(member));
					}
					break;
				case ManagedLanguageCodeException::MemberOverridedTargetNotExists:
					{
						ManagedMember* member=dynamic_cast<ManagedMember*>(error->GetManagedLanguageElement());
						message=ManagedErrorMessage::MemberOverridedTargetNotExists(MemberContainingTypeName(member), MemberName(member));
					}
					break;
				case ManagedLanguageCodeException::MemberOverridedTargetIllegalAccessor:
					{
						ManagedMember* member=dynamic_cast<ManagedMember*>(error->GetManagedLanguageElement());
						message=ManagedErrorMessage::MemberOverridedTargetIllegalAccessor(MemberContainingTypeName(member), MemberName(member));
					}
					break;
				case ManagedLanguageCodeException::MethodWithGenericParametersCannotBeVirtual:
					{
						ManagedMember* member=dynamic_cast<ManagedMember*>(error->GetManagedLanguageElement());
						message=ManagedErrorMessage::MethodWithGenericParametersCannotBeVirtual(MemberContainingTypeName(member), MemberName(member));
					}
					break;

/***********************************************************************
ManagedLanguageCodeException::ExceptionCode::SymbolResolving
***********************************************************************/
					
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
				case ManagedLanguageCodeException::ExpressionCannotCastToType:
					{
						message=ManagedErrorMessage::ExpressionCannotCastToType(TypeToString(error->GetTypeParameter()));
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
				case ManagedLanguageCodeException::IllegalNull:
					{
						message=ManagedErrorMessage::IllegalNull(ExpressionToString(dynamic_cast<ManagedExpression*>(error->GetManagedLanguageElement())));
					}
					break;
				case ManagedLanguageCodeException::CannotDetermineContainerElementType:
					{
						message=ManagedErrorMessage::CannotDetermineContainerElementType(TypeToString(error->GetTypeParameter()));
					}
					break;
				case ManagedLanguageCodeException::CannotDisposeNonIDisposableType:
					{
						message=ManagedErrorMessage::CannotDisposeNonIDisposableType(TypeToString(error->GetTypeParameter()));
					}
					break;
				case ManagedLanguageCodeException::IllegalFunctionResult:
					{
						message=ManagedErrorMessage::IllegalFunctionResult();
					}
					break;
				case ManagedLanguageCodeException::IllegalInstanciation:
					{
						message=ManagedErrorMessage::IllegalInstanciation();
					}
					break;
				case ManagedLanguageCodeException::InstanciationFailedGenericArgumentCountNotMatched:
					{
						message=ManagedErrorMessage::InstanciationFailedGenericArgumentCountNotMatched();
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