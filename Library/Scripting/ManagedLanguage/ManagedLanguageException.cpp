#include "ManagedLanguageException.h"
#include "..\..\Collections\Operation.h"

namespace vl
{
	namespace scripting
	{
		namespace managedlanguage
		{
			using namespace collections;

/***********************************************************************
ManagedLanguageCodeException
***********************************************************************/

			ManagedLanguageCodeException::ManagedLanguageCodeException(ManagedLanguageElement* _element, ExceptionCode _exceptionCode, const collections::IReadonlyList<WString>& _parameters, ManagedTypeSymbol* _typeParameter, ManagedTypeSymbol* _memberTypeParameter, ManagedSymbolItem* _memberSymbolParameter)
				:element(_element)
				,typeParameter(_typeParameter)
				,memberTypeParameter(_memberTypeParameter)
				,memberSymbolParameter(_memberSymbolParameter)
				,exceptionCode(_exceptionCode)
			{
				CopyFrom(parameters.Wrap(), _parameters);
			}

			ManagedLanguageCodeException::ManagedLanguageCodeException(const ManagedLanguageCodeException& exception)
				:element(exception.element)
				,typeParameter(exception.typeParameter)
				,exceptionCode(exception.exceptionCode)
			{
				CopyFrom(parameters.Wrap(), exception.parameters.Wrap());
			}

			ManagedLanguageCodeException::~ManagedLanguageCodeException()
			{
			}

			ManagedLanguageElement* ManagedLanguageCodeException::GetManagedLanguageElement()const
			{
				return element;
			}

			ManagedTypeSymbol* ManagedLanguageCodeException::GetTypeParameter()const
			{
				return typeParameter;
			}

			ManagedTypeSymbol* ManagedLanguageCodeException::GetMemberTypeParameter()const
			{
				return memberTypeParameter;
			}

			ManagedSymbolItem* ManagedLanguageCodeException::GetMemberSymbolParameter()const
			{
				return memberSymbolParameter;
			}

			ManagedLanguageCodeException::ExceptionCode ManagedLanguageCodeException::GetExceptionCode()const
			{
				return exceptionCode;
			}

			const collections::IReadonlyList<WString>& ManagedLanguageCodeException::GetParameters()const
			{
				return parameters.Wrap();
			}

/***********************************************************************
ManagedLanguageCodeException::ExceptionCode::ScopeBuilding
***********************************************************************/

			Ptr<ManagedLanguageCodeException> ManagedLanguageCodeException::GetSymbolAlreadyDefined(ManagedLanguageElement* element, const WString& name)
			{
				Array<WString> parameters(1);
				parameters[0]=name;
				return new ManagedLanguageCodeException(element, SymbolAlreadyDefined, parameters.Wrap());
			}

			Ptr<ManagedLanguageCodeException> ManagedLanguageCodeException::GetNamespaceNotExists(ManagedUsingNamespaceDeclaration* element)
			{
				Array<WString> parameters(0);
				return new ManagedLanguageCodeException(element, NamespaceNotExists, parameters.Wrap());
			}

			Ptr<ManagedLanguageCodeException> ManagedLanguageCodeException::GetSystemTypeNotExists(ManagedLanguageElement* element, const WString& name)
			{
				Array<WString> parameters(1);
				parameters[0]=name;
				return new ManagedLanguageCodeException(element, SystemTypeNotExists, parameters.Wrap());
			}

			Ptr<ManagedLanguageCodeException> ManagedLanguageCodeException::GetSystemTypeDuplicated(ManagedLanguageElement* element, const WString& name)
			{
				Array<WString> parameters(1);
				parameters[0]=name;
				return new ManagedLanguageCodeException(element, SystemTypeDuplicated, parameters.Wrap());
			}

			Ptr<ManagedLanguageCodeException> ManagedLanguageCodeException::GetTypeNotExists(ManagedType* element)
			{
				Array<WString> parameters(0);
				return new ManagedLanguageCodeException(element, TypeNotExists, parameters.Wrap());
			}

			Ptr<ManagedLanguageCodeException> ManagedLanguageCodeException::GetTypeDuplicated(ManagedType* element)
			{
				Array<WString> parameters(0);
				return new ManagedLanguageCodeException(element, TypeDuplicated, parameters.Wrap());
			}

			Ptr<ManagedLanguageCodeException> ManagedLanguageCodeException::GetIllegalAutoRefer(ManagedAutoReferType* element)
			{
				Array<WString> parameters(0);
				return new ManagedLanguageCodeException(element, IllegalAutoRefer, parameters.Wrap());
			}

			Ptr<ManagedLanguageCodeException> ManagedLanguageCodeException::GetCannotDirectReferTypeRenameAsToplevel(ManagedTypeRenameDeclaration* element)
			{
				Array<WString> parameters(1);
				parameters[0]=element->name;
				return new ManagedLanguageCodeException(element, CannotDirectReferTypeRenameAsToplevel, parameters.Wrap());
			}

			Ptr<ManagedLanguageCodeException> ManagedLanguageCodeException::GetIllegalNestedDeclaration(ManagedTypeMember* element)
			{
				Array<WString> parameters(0);
				return new ManagedLanguageCodeException(element, IllegalNestedDeclaration, parameters.Wrap());
			}

			Ptr<ManagedLanguageCodeException> ManagedLanguageCodeException::GetTypeInvisible(ManagedLanguageElement* element, ManagedTypeSymbol* type)
			{
				Array<WString> parameters(0);
				return new ManagedLanguageCodeException(element, TypeInvisible, parameters.Wrap(), type);
			}

			Ptr<ManagedLanguageCodeException> ManagedLanguageCodeException::GetTypeInvisibleOutSideOfAssembly(ManagedLanguageElement* element, ManagedTypeSymbol* type)
			{
				Array<WString> parameters(0);
				return new ManagedLanguageCodeException(element, TypeInvisibleOutSideOfAssembly, parameters.Wrap(), type);
			}

			Ptr<ManagedLanguageCodeException> ManagedLanguageCodeException::GetCannotUseUninstantiatedGenericType(ManagedLanguageElement* element, ManagedTypeSymbol* type)
			{
				Array<WString> parameters(0);
				return new ManagedLanguageCodeException(element, CannotUseUninstantiatedGenericType, parameters.Wrap(), type);
			}

			Ptr<ManagedLanguageCodeException> ManagedLanguageCodeException::GetGenericTypeArgumentCountNotMatches(ManagedLanguageElement* element, ManagedTypeSymbol* type)
			{
				Array<WString> parameters(0);
				return new ManagedLanguageCodeException(element, GenericTypeArgumentCountNotMatches, parameters.Wrap(), type);
			}

			Ptr<ManagedLanguageCodeException> ManagedLanguageCodeException::GetGenericTypeTypeConstraintNotSatisfied(ManagedLanguageElement* element, ManagedTypeSymbol* genericType, vint index)
			{
				Array<WString> parameters(1);
				parameters[0]=itow(index);
				return new ManagedLanguageCodeException(element, GenericTypeTypeConstraintNotSatisfied, parameters.Wrap(), genericType);
			}

			Ptr<ManagedLanguageCodeException> ManagedLanguageCodeException::GetGenericTypeNewConstraintNotSatisfied(ManagedLanguageElement* element, ManagedTypeSymbol* genericType, vint index)
			{
				Array<WString> parameters(1);
				parameters[0]=itow(index);
				return new ManagedLanguageCodeException(element, GenericTypeNewConstraintNotSatisfied, parameters.Wrap(), genericType);
			}

/***********************************************************************
ManagedLanguageCodeException::ExceptionCode::DeclarationTypeChecking
***********************************************************************/

			Ptr<ManagedLanguageCodeException> ManagedLanguageCodeException::GetTypeCannotDerivedFromItself(ManagedTypeDeclaration* declaration)
			{
				Array<WString> parameters(0);
				return new ManagedLanguageCodeException(declaration, TypeCannotDerivedFromItself, parameters.Wrap());
			}

			Ptr<ManagedLanguageCodeException> ManagedLanguageCodeException::GetClassIllegalBaseClass(ManagedTypeDeclaration* declaration)
			{
				Array<WString> parameters(0);
				return new ManagedLanguageCodeException(declaration, ClassIllegalBaseClass, parameters.Wrap());
			}

			Ptr<ManagedLanguageCodeException> ManagedLanguageCodeException::GetStructureIllegalBaseClass(ManagedTypeDeclaration* declaration)
			{
				Array<WString> parameters(0);
				return new ManagedLanguageCodeException(declaration, StructureIllegalBaseClass, parameters.Wrap());
			}

			Ptr<ManagedLanguageCodeException> ManagedLanguageCodeException::GetInterfaceIllegalBaseClass(ManagedTypeDeclaration* declaration)
			{
				Array<WString> parameters(0);
				return new ManagedLanguageCodeException(declaration, InterfaceIllegalBaseClass, parameters.Wrap());
			}

			Ptr<ManagedLanguageCodeException> ManagedLanguageCodeException::GetStaticTypeIllegalBaseClass(ManagedTypeDeclaration* declaration)
			{
				Array<WString> parameters(0);
				return new ManagedLanguageCodeException(declaration, StaticTypeIllegalBaseClass, parameters.Wrap());
			}

			Ptr<ManagedLanguageCodeException> ManagedLanguageCodeException::GetClassStructureTooMuchBaseClasses(ManagedTypeDeclaration* declaration)
			{
				Array<WString> parameters(0);
				return new ManagedLanguageCodeException(declaration, ClassStructureTooMuchBaseClasses, parameters.Wrap());
			}

			Ptr<ManagedLanguageCodeException> ManagedLanguageCodeException::GetClassStructureIllegalInheritation(ManagedTypeDeclaration* declaration)
			{
				Array<WString> parameters(0);
				return new ManagedLanguageCodeException(declaration, ClassStructureIllegalInheritation, parameters.Wrap());
			}

			Ptr<ManagedLanguageCodeException> ManagedLanguageCodeException::GetInterfaceIllegalInheritation(ManagedTypeDeclaration* declaration)
			{
				Array<WString> parameters(0);
				return new ManagedLanguageCodeException(declaration, InterfaceIllegalInheritation, parameters.Wrap());
			}

			Ptr<ManagedLanguageCodeException> ManagedLanguageCodeException::GetInterfaceIllegalMemberType(ManagedTypeDeclaration* declaration)
			{
				Array<WString> parameters(0);
				return new ManagedLanguageCodeException(declaration, InterfaceIllegalMemberType, parameters.Wrap());
			}

			Ptr<ManagedLanguageCodeException> ManagedLanguageCodeException::GetStaticTypeIllegalInheritance(ManagedTypeDeclaration* declaration)
			{
				Array<WString> parameters(0);
				return new ManagedLanguageCodeException(declaration, StaticTypeIllegalInheritance, parameters.Wrap());
			}

			Ptr<ManagedLanguageCodeException> ManagedLanguageCodeException::GetTypeIllegalAccessor(ManagedTypeDeclaration* declaration)
			{
				Array<WString> parameters(0);
				return new ManagedLanguageCodeException(declaration, TypeIllegalAccessor, parameters.Wrap());
			}
			
			Ptr<ManagedLanguageCodeException> ManagedLanguageCodeException::GetInterfaceMemberNotImplemented(ManagedTypeDeclaration* declaration, ManagedTypeSymbol* abstractMemberType, ManagedSymbolItem* abstractMemberSymbol)
			{
				Array<WString> parameters(0);
				return new ManagedLanguageCodeException(declaration, InterfaceMemberNotImplemented, parameters.Wrap(), 0, abstractMemberType, abstractMemberSymbol);
			}

			Ptr<ManagedLanguageCodeException> ManagedLanguageCodeException::GetAbstractMemberNotImplemented(ManagedTypeDeclaration* declaration, ManagedTypeSymbol* abstractMemberType, ManagedSymbolItem* abstractMemberSymbol)
			{
				Array<WString> parameters(0);
				return new ManagedLanguageCodeException(declaration, AbstractMemberNotImplemented, parameters.Wrap(), 0, abstractMemberType, abstractMemberSymbol);
			}

/***********************************************************************
ManagedLanguageCodeException::ExceptionCode::DeclarationMemberChecking
***********************************************************************/

			Ptr<ManagedLanguageCodeException> ManagedLanguageCodeException::GetInterfaceMemberIllegalType(ManagedMember* member)
			{
				Array<WString> parameters(0);
				return new ManagedLanguageCodeException(member, InterfaceMemberIllegalType, parameters.Wrap());
			}

			Ptr<ManagedLanguageCodeException> ManagedLanguageCodeException::GetInterfaceMemberIllegalAccessor(ManagedMember* member)
			{
				Array<WString> parameters(0);
				return new ManagedLanguageCodeException(member, InterfaceMemberIllegalAccessor, parameters.Wrap());
			}

			Ptr<ManagedLanguageCodeException> ManagedLanguageCodeException::GetInterfaceMemberIllegalMemberType(ManagedMember* member)
			{
				Array<WString> parameters(0);
				return new ManagedLanguageCodeException(member, InterfaceMemberIllegalMemberType, parameters.Wrap());
			}

			Ptr<ManagedLanguageCodeException> ManagedLanguageCodeException::GetInterfaceMemberIllegalInheritation(ManagedMember* member)
			{
				Array<WString> parameters(0);
				return new ManagedLanguageCodeException(member, InterfaceMemberIllegalInheritation, parameters.Wrap());
			}

			Ptr<ManagedLanguageCodeException> ManagedLanguageCodeException::GetInterfaceMemberIllegalImplementedInterface(ManagedMember* member)
			{
				Array<WString> parameters(0);
				return new ManagedLanguageCodeException(member, InterfaceMemberIllegalImplementedInterface, parameters.Wrap());
			}

			Ptr<ManagedLanguageCodeException> ManagedLanguageCodeException::GetSealedTypeMemberIllegalAccessor(ManagedMember* member)
			{
				Array<WString> parameters(0);
				return new ManagedLanguageCodeException(member, SealedTypeMemberIllegalAccessor, parameters.Wrap());
			}

			Ptr<ManagedLanguageCodeException> ManagedLanguageCodeException::GetSealedTypeMemberIllegalInheritance(ManagedMember* member)
			{
				Array<WString> parameters(0);
				return new ManagedLanguageCodeException(member, SealedTypeMemberIllegalInheritance, parameters.Wrap());
			}

			Ptr<ManagedLanguageCodeException> ManagedLanguageCodeException::GetStaticTypeMemberIllegalAccessor(ManagedMember* member)
			{
				Array<WString> parameters(0);
				return new ManagedLanguageCodeException(member, StaticTypeMemberIllegalAccessor, parameters.Wrap());
			}

			Ptr<ManagedLanguageCodeException> ManagedLanguageCodeException::GetStaticTypeMemberIllegalInheritance(ManagedMember* member)
			{
				Array<WString> parameters(0);
				return new ManagedLanguageCodeException(member, StaticTypeMemberIllegalInheritance, parameters.Wrap());
			}

			Ptr<ManagedLanguageCodeException> ManagedLanguageCodeException::GetStaticTypeMemberIllegalMemberType(ManagedMember* member)
			{
				Array<WString> parameters(0);
				return new ManagedLanguageCodeException(member, StaticTypeMemberIllegalMemberType, parameters.Wrap());
			}

			Ptr<ManagedLanguageCodeException> ManagedLanguageCodeException::GetNonAbstractTypeMemberIllegalInheritance(ManagedMember* member)
			{
				Array<WString> parameters(0);
				return new ManagedLanguageCodeException(member, NonAbstractTypeMemberIllegalInheritance, parameters.Wrap());
			}

			Ptr<ManagedLanguageCodeException> ManagedLanguageCodeException::GetMemberImplementedInterfaceTypeNotExists(ManagedMember* member)
			{
				Array<WString> parameters(0);
				return new ManagedLanguageCodeException(member, MemberImplementedInterfaceTypeNotExists, parameters.Wrap());
			}

			Ptr<ManagedLanguageCodeException> ManagedLanguageCodeException::GetInterfaceImplementedMemberIllegalMemberType(ManagedMember* member)
			{
				Array<WString> parameters(0);
				return new ManagedLanguageCodeException(member, InterfaceImplementedMemberIllegalMemberType, parameters.Wrap());
			}

			Ptr<ManagedLanguageCodeException> ManagedLanguageCodeException::GetInterfaceImplementedMemberIllegalInheritation(ManagedMember* member)
			{
				Array<WString> parameters(0);
				return new ManagedLanguageCodeException(member, InterfaceImplementedMemberIllegalInheritation, parameters.Wrap());
			}

			Ptr<ManagedLanguageCodeException> ManagedLanguageCodeException::GetMemberOverridedTargetNotExists(ManagedMember* member)
			{
				Array<WString> parameters(0);
				return new ManagedLanguageCodeException(member, MemberOverridedTargetNotExists, parameters.Wrap());
			}

			Ptr<ManagedLanguageCodeException> ManagedLanguageCodeException::GetMemberOverridedTargetIllegalAccessor(ManagedMember* member)
			{
				Array<WString> parameters(0);
				return new ManagedLanguageCodeException(member, MemberOverridedTargetIllegalAccessor, parameters.Wrap());
			}

			Ptr<ManagedLanguageCodeException> ManagedLanguageCodeException::GetMethodWithGenericParametersCannotBeVirtual(ManagedMethod* member)
			{
				Array<WString> parameters(0);
				return new ManagedLanguageCodeException(member, MethodWithGenericParametersCannotBeVirtual, parameters.Wrap());
			}

/***********************************************************************
ManagedLanguageCodeException::ExceptionCode::SymbolResolving
***********************************************************************/

			Ptr<ManagedLanguageCodeException> ManagedLanguageCodeException::GetExpressionResolvedToDuplicatedTargets(ManagedExpression* expression, ManagedTypeSymbol* type)
			{
				Array<WString> parameters(0);
				return new ManagedLanguageCodeException(expression, ExpressionResolvedToDuplicatedTargets, parameters.Wrap(), type);
			}

			Ptr<ManagedLanguageCodeException> ManagedLanguageCodeException::GetExpressionResolvingFailed(ManagedReferenceExpression* expression)
			{
				Array<WString> parameters(1);
				parameters[0]=expression->name;
				return new ManagedLanguageCodeException(expression, ExpressionResolvingFailed, parameters.Wrap());
			}

			Ptr<ManagedLanguageCodeException> ManagedLanguageCodeException::GetExpressionResolvingFailed(ManagedMemberExpression* expression)
			{
				Array<WString> parameters(1);
				parameters[0]=expression->member;
				return new ManagedLanguageCodeException(expression, ExpressionResolvingFailed, parameters.Wrap());
			}

			Ptr<ManagedLanguageCodeException> ManagedLanguageCodeException::GetExpressionCannotConvertToType(ManagedExpression* expression, ManagedTypeSymbol* type)
			{
				Array<WString> parameters(0);
				return new ManagedLanguageCodeException(expression, ExpressionCannotConvertToType, parameters.Wrap(), type);
			}

			Ptr<ManagedLanguageCodeException> ManagedLanguageCodeException::GetIllegalAutoRefWithoutInitializer(ManagedVariableStatement* statement)
			{
				Array<WString> parameters(1);
				parameters[0]=statement->name;
				return new ManagedLanguageCodeException(statement, IllegalAutoRefWithoutInitializer, parameters.Wrap());
			}

			Ptr<ManagedLanguageCodeException> ManagedLanguageCodeException::GetVariableAlreadyExists(ManagedVariableStatement* statement)
			{
				Array<WString> parameters(1);
				parameters[0]=statement->name;
				return new ManagedLanguageCodeException(statement, VariableAlreadyExists, parameters.Wrap());
			}

			Ptr<ManagedLanguageCodeException> ManagedLanguageCodeException::GetIllegalBreak(ManagedBreakStatement* statement)
			{
				Array<WString> parameters(0);
				return new ManagedLanguageCodeException(statement, IllegalBreak, parameters.Wrap());
			}

			Ptr<ManagedLanguageCodeException> ManagedLanguageCodeException::GetIllegalContinue(ManagedContinueStatement* statement)
			{
				Array<WString> parameters(0);
				return new ManagedLanguageCodeException(statement, IllegalContinue, parameters.Wrap());
			}

			Ptr<ManagedLanguageCodeException> ManagedLanguageCodeException::GetIllegalThrow(ManagedThrowStatement* statement)
			{
				Array<WString> parameters(0);
				return new ManagedLanguageCodeException(statement, IllegalThrow, parameters.Wrap());
			}

			Ptr<ManagedLanguageCodeException> ManagedLanguageCodeException::GetExceptionTypeShouldDerivedFromException(ManagedType* type, ManagedTypeSymbol* exceptionType)
			{
				Array<WString> parameters(0);
				return new ManagedLanguageCodeException(type, ExceptionTypeShouldDerivedFromException, parameters.Wrap(), exceptionType);
			}

			Ptr<ManagedLanguageCodeException> ManagedLanguageCodeException::GetExceptionTypeShouldDerivedFromException(ManagedExpression* expression, ManagedTypeSymbol* exceptionType)
			{
				Array<WString> parameters(0);
				return new ManagedLanguageCodeException(expression, ExceptionTypeShouldDerivedFromException, parameters.Wrap(), exceptionType);
			}

			Ptr<ManagedLanguageCodeException> ManagedLanguageCodeException::GetIllegalThis(ManagedThisExpression* expression)
			{
				Array<WString> parameters(0);
				return new ManagedLanguageCodeException(expression, IllegalThis, parameters.Wrap());
			}

			Ptr<ManagedLanguageCodeException> ManagedLanguageCodeException::GetIllegalBase(ManagedBaseExpression* expression)
			{
				Array<WString> parameters(0);
				return new ManagedLanguageCodeException(expression, IllegalBase, parameters.Wrap());
			}

			Ptr<ManagedLanguageCodeException> ManagedLanguageCodeException::GetExpressionIsNotValue(ManagedExpression* expression)
			{
				Array<WString> parameters(0);
				return new ManagedLanguageCodeException(expression, ExpressionIsNotValue, parameters.Wrap());
			}

			Ptr<ManagedLanguageCodeException> ManagedLanguageCodeException::GetMatchedMethodNotExists(ManagedExpression* expression)
			{
				Array<WString> parameters(0);
				return new ManagedLanguageCodeException(expression, MatchedMethodNotExists, parameters.Wrap());
			}

			Ptr<ManagedLanguageCodeException> ManagedLanguageCodeException::GetMatchedMethodTooMuch(ManagedExpression* expression)
			{
				Array<WString> parameters(0);
				return new ManagedLanguageCodeException(expression, MatchedMethodTooMuch, parameters.Wrap());
			}

			Ptr<ManagedLanguageCodeException> ManagedLanguageCodeException::GetIllegalNull(ManagedNullExpression* expression)
			{
				Array<WString> parameters(0);
				return new ManagedLanguageCodeException(expression, IllegalNull, parameters.Wrap());
			}
		}
	}
}