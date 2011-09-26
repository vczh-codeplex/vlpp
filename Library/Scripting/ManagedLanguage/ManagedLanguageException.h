/***********************************************************************
Vczh Library++ 3.0
Developer: ³Âè÷å«(vczh)
Scripting::ManagedLanguage

Classes:
***********************************************************************/

#ifndef VCZH_SCRIPTING_MANAGEDLANGUAGE_MANAGEDLANGUAGEEXCEPTION
#define VCZH_SCRIPTING_MANAGEDLANGUAGE_MANAGEDLANGUAGEEXCEPTION

#include "ManagedLanguageSymbolManager.h"
#include "..\..\Exception.h"

namespace vl
{
	namespace scripting
	{
		namespace managedlanguage
		{
			class ManagedLanguageCodeException : public Exception
			{
			public:
				enum ExceptionCode
				{
					SymbolAlreadyDefined,								// <all>								:name
					NamespaceNotExists,									// <ManagedUsingNamespaceDeclaration>
					SystemTypeNotExists,								// <all>								:name
					SystemTypeDuplicated,								// <all>								:name
					TypeNotExists,										// <ManagedType>
					TypeDuplicated,										// <ManagedType>
					IllegalAutoRefer,									// <ManagedAutoReferType>
					CannotDirectReferTypeRenameAsToplevel,				// <ManagedTypeRenameDeclaration>		:name
					IllegalNestedDeclaration,							// <ManagedTypeMember>
					TypeInvisible,										// <all>
					TypeInvisibleOutSideOfAssembly,						// <all>
					CannotUseUninstantiatedGenericType,					// <all>
					GenericTypeArgumentCountNotMatches,					// <all>
					GenericTypeTypeConstraintNotSatisfied,				// <all>								:index(type parameters generic argument index)
					GenericTypeNewConstraintNotSatisfied,				// <all>								:index(type parameters generic argument index)
					
					IllegalTypeConstraint,								// <ManagedTypeDeclaration>				:name
					TypeCannotDerivedFromItself,						// <ManagedTypeDeclaration>
					ClassIllegalBaseClass,								// <ManagedTypeDeclaration>
					StructureIllegalBaseClass,							// <ManagedTypeDeclaration>
					InterfaceIllegalBaseClass,							// <ManagedTypeDeclaration>
					StaticTypeIllegalBaseClass,							// <ManagedTypeDeclaration>
					ClassStructureTooMuchBaseClasses,					// <ManagedTypeDeclaration>
					ClassStructureIllegalInheritation,					// <ManagedTypeDeclaration>
					InterfaceIllegalInheritation,						// <ManagedTypeDeclaration>
					InterfaceIllegalMemberType,							// <ManagedTypeDeclaration>
					StaticTypeIllegalInheritance,						// <ManagedTypeDeclaration>
					TypeIllegalAccessor,								// <ManagedTypeDeclaration>
					InterfaceMemberNotImplemented,						// <ManagedTypeDeclaration>
					AbstractMemberNotImplemented,						// <ManagedTypeDeclaration>

					InterfaceMemberIllegalType,							// <ManagedMember>
					InterfaceMemberIllegalAccessor,						// <ManagedMember>
					InterfaceMemberIllegalMemberType,					// <ManagedMember>
					InterfaceMemberIllegalInheritation,					// <ManagedMember>
					InterfaceMemberIllegalImplementedInterface,			// <ManagedMember>
					SealedTypeMemberIllegalAccessor,					// <ManagedMember>
					SealedTypeMemberIllegalInheritance,					// <ManagedMember>
					StaticTypeMemberIllegalAccessor,					// <ManagedMember>
					StaticTypeMemberIllegalInheritance,					// <ManagedMember>
					StaticTypeMemberIllegalMemberType,					// <ManagedMember>
					NonAbstractTypeMemberIllegalInheritance,			// <ManagedMember>
					InterfaceImplementedMemberIllegalMemberType,		// <ManagedMember>
					InterfaceImplementedMemberIllegalInheritation,		// <ManagedMember>
					MemberImplementedInterfaceTypeNotExists,			// <ManagedMember>
					MemberOverridedTargetNotExists,						// <ManagedMember>
					MemberOverridedTargetIllegalAccessor,				// <ManagedMember>
					MethodWithGenericParametersCannotBeVirtual,			// <ManagedMember>

					ExpressionResolvedToDuplicatedTargets,				// <ManagedExpression>
					ExpressionResolvingFailed,							// <ManagedExpression>					:name
					ExpressionCannotConvertToType,						// <ManagedExpression>
					ExpressionCannotCastToType,							// <ManagedExpression>
					IllegalAutoRefWithoutInitializer,					// <ManagedVariableStatement>			:name
					VariableAlreadyExists,								// <ManagedVariableStatement>			:name
					IllegalBreak,										// <ManagedBreakStatement>
					IllegalContinue,									// <ManagedContinueStatement>
					IllegalThrow,										// <ManagedThrowStatement>
					ExceptionTypeShouldDerivedFromException,			// <ManagedType | ManagedExpression>
					IllegalThis,										// <ManagedThisExpression>
					IllegalBase,										// <ManagedBaseExpression>
					ExpressionIsNotValue,								// <ManagedExpression>
					MatchedMethodNotExists,								// <ManagedExpression>
					MatchedMethodTooMuch,								// <ManagedExpression>
					IllegalNull,										// <ManagedNullExpression>
					CannotDetermineContainerElementType,				// <ManagedForEachStatement>
					CannotDisposeNonIDisposableType,					// <ManagedUsingStatement>
					IllegalFunctionResult,								// <ManagedFunctionResultExpression>
					IllegalInstanciation,								// <ManagedInstantiatedExpression>
					InstanciationFailedGenericArgumentCountNotMatched,	// <ManagedInstantiatedExpression>
					CannotAccessMembersOfNull,							// <ManagedExpression>
				};
			protected:
				ManagedLanguageElement*											element;
				ManagedTypeSymbol*												typeParameter;
				ManagedTypeSymbol*												memberTypeParameter;
				ManagedSymbolItem*												memberSymbolParameter;
				ExceptionCode													exceptionCode;
				collections::List<WString>										parameters;
			public:
				ManagedLanguageCodeException(ManagedLanguageElement* _element, ExceptionCode _exceptionCode, const collections::IReadonlyList<WString>& _parameters, ManagedTypeSymbol* _typeParameter=0, ManagedTypeSymbol* _memberTypeParameter=0, ManagedSymbolItem* _memberSymbolParameter=0);
				ManagedLanguageCodeException(const ManagedLanguageCodeException& exception);
				~ManagedLanguageCodeException();

				ManagedLanguageElement*											GetManagedLanguageElement()const;
				ManagedTypeSymbol*												GetTypeParameter()const;
				ManagedTypeSymbol*												GetMemberTypeParameter()const;
				ManagedSymbolItem*												GetMemberSymbolParameter()const;
				ExceptionCode													GetExceptionCode()const;
				const collections::IReadonlyList<WString>&						GetParameters()const;

				static Ptr<ManagedLanguageCodeException>						GetSymbolAlreadyDefined(ManagedLanguageElement* element, const WString& name);
				static Ptr<ManagedLanguageCodeException>						GetNamespaceNotExists(ManagedUsingNamespaceDeclaration* element);
				static Ptr<ManagedLanguageCodeException>						GetSystemTypeNotExists(ManagedLanguageElement* element, const WString& name);
				static Ptr<ManagedLanguageCodeException>						GetSystemTypeDuplicated(ManagedLanguageElement* element, const WString& name);
				static Ptr<ManagedLanguageCodeException>						GetTypeNotExists(ManagedType* element);
				static Ptr<ManagedLanguageCodeException>						GetTypeDuplicated(ManagedType* element);
				static Ptr<ManagedLanguageCodeException>						GetIllegalAutoRefer(ManagedAutoReferType* element);
				static Ptr<ManagedLanguageCodeException>						GetCannotDirectReferTypeRenameAsToplevel(ManagedTypeRenameDeclaration* element);
				static Ptr<ManagedLanguageCodeException>						GetIllegalNestedDeclaration(ManagedTypeMember* element);
				static Ptr<ManagedLanguageCodeException>						GetTypeInvisible(ManagedLanguageElement* element, ManagedTypeSymbol* type);
				static Ptr<ManagedLanguageCodeException>						GetTypeInvisibleOutSideOfAssembly(ManagedLanguageElement* element, ManagedTypeSymbol* type);
				static Ptr<ManagedLanguageCodeException>						GetCannotUseUninstantiatedGenericType(ManagedLanguageElement* element, ManagedTypeSymbol* type);
				static Ptr<ManagedLanguageCodeException>						GetGenericTypeArgumentCountNotMatches(ManagedLanguageElement* element, ManagedTypeSymbol* type);
				static Ptr<ManagedLanguageCodeException>						GetGenericTypeTypeConstraintNotSatisfied(ManagedLanguageElement* element, ManagedTypeSymbol* genericType, vint index);
				static Ptr<ManagedLanguageCodeException>						GetGenericTypeNewConstraintNotSatisfied(ManagedLanguageElement* element, ManagedTypeSymbol* genericType, vint index);

				static Ptr<ManagedLanguageCodeException>						GetIllegalTypeConstraint(ManagedLanguageElement* element, const WString& name);
				static Ptr<ManagedLanguageCodeException>						GetTypeCannotDerivedFromItself(ManagedTypeDeclaration* declaration);
				static Ptr<ManagedLanguageCodeException>						GetClassIllegalBaseClass(ManagedTypeDeclaration* declaration);
				static Ptr<ManagedLanguageCodeException>						GetStructureIllegalBaseClass(ManagedTypeDeclaration* declaration);
				static Ptr<ManagedLanguageCodeException>						GetInterfaceIllegalBaseClass(ManagedTypeDeclaration* declaration);
				static Ptr<ManagedLanguageCodeException>						GetStaticTypeIllegalBaseClass(ManagedTypeDeclaration* declaration);
				static Ptr<ManagedLanguageCodeException>						GetClassStructureTooMuchBaseClasses(ManagedTypeDeclaration* declaration);
				static Ptr<ManagedLanguageCodeException>						GetClassStructureIllegalInheritation(ManagedTypeDeclaration* declaration);
				static Ptr<ManagedLanguageCodeException>						GetInterfaceIllegalInheritation(ManagedTypeDeclaration* declaration);
				static Ptr<ManagedLanguageCodeException>						GetInterfaceIllegalMemberType(ManagedTypeDeclaration* declaration);
				static Ptr<ManagedLanguageCodeException>						GetStaticTypeIllegalInheritance(ManagedTypeDeclaration* declaration);
				static Ptr<ManagedLanguageCodeException>						GetTypeIllegalAccessor(ManagedTypeDeclaration* declaration);
				static Ptr<ManagedLanguageCodeException>						GetInterfaceMemberNotImplemented(ManagedTypeDeclaration* declaration, ManagedTypeSymbol* abstractMemberType, ManagedSymbolItem* abstractMemberSymbol);
				static Ptr<ManagedLanguageCodeException>						GetAbstractMemberNotImplemented(ManagedTypeDeclaration* declaration, ManagedTypeSymbol* abstractMemberType, ManagedSymbolItem* abstractMemberSymbol);

				static Ptr<ManagedLanguageCodeException>						GetInterfaceMemberIllegalType(ManagedMember* member);
				static Ptr<ManagedLanguageCodeException>						GetInterfaceMemberIllegalAccessor(ManagedMember* member);
				static Ptr<ManagedLanguageCodeException>						GetInterfaceMemberIllegalMemberType(ManagedMember* member);
				static Ptr<ManagedLanguageCodeException>						GetInterfaceMemberIllegalInheritation(ManagedMember* member);
				static Ptr<ManagedLanguageCodeException>						GetInterfaceMemberIllegalImplementedInterface(ManagedMember* member);
				static Ptr<ManagedLanguageCodeException>						GetSealedTypeMemberIllegalAccessor(ManagedMember* member);
				static Ptr<ManagedLanguageCodeException>						GetSealedTypeMemberIllegalInheritance(ManagedMember* member);
				static Ptr<ManagedLanguageCodeException>						GetStaticTypeMemberIllegalAccessor(ManagedMember* member);
				static Ptr<ManagedLanguageCodeException>						GetStaticTypeMemberIllegalInheritance(ManagedMember* member);
				static Ptr<ManagedLanguageCodeException>						GetStaticTypeMemberIllegalMemberType(ManagedMember* member);
				static Ptr<ManagedLanguageCodeException>						GetNonAbstractTypeMemberIllegalInheritance(ManagedMember* member);
				static Ptr<ManagedLanguageCodeException>						GetInterfaceImplementedMemberIllegalMemberType(ManagedMember* member);
				static Ptr<ManagedLanguageCodeException>						GetInterfaceImplementedMemberIllegalInheritation(ManagedMember* member);
				static Ptr<ManagedLanguageCodeException>						GetMemberImplementedInterfaceTypeNotExists(ManagedMember* member);
				static Ptr<ManagedLanguageCodeException>						GetMemberOverridedTargetNotExists(ManagedMember* member);
				static Ptr<ManagedLanguageCodeException>						GetMemberOverridedTargetIllegalAccessor(ManagedMember* member);
				static Ptr<ManagedLanguageCodeException>						GetMethodWithGenericParametersCannotBeVirtual(ManagedMember* member);

				static Ptr<ManagedLanguageCodeException>						GetExpressionResolvedToDuplicatedTargets(ManagedExpression* expression, ManagedTypeSymbol* type);
				static Ptr<ManagedLanguageCodeException>						GetExpressionResolvingFailed(ManagedReferenceExpression* expression);
				static Ptr<ManagedLanguageCodeException>						GetExpressionResolvingFailed(ManagedMemberExpression* expression);
				static Ptr<ManagedLanguageCodeException>						GetExpressionCannotConvertToType(ManagedExpression* expression, ManagedTypeSymbol* type);
				static Ptr<ManagedLanguageCodeException>						GetExpressionCannotCastToType(ManagedExpression* expression, ManagedTypeSymbol* type);
				static Ptr<ManagedLanguageCodeException>						GetIllegalAutoRefWithoutInitializer(ManagedVariableStatement* statement);
				static Ptr<ManagedLanguageCodeException>						GetVariableAlreadyExists(ManagedVariableStatement* statement);
				static Ptr<ManagedLanguageCodeException>						GetIllegalBreak(ManagedBreakStatement* statement);
				static Ptr<ManagedLanguageCodeException>						GetIllegalContinue(ManagedContinueStatement* statement);
				static Ptr<ManagedLanguageCodeException>						GetIllegalThrow(ManagedThrowStatement* statement);
				static Ptr<ManagedLanguageCodeException>						GetExceptionTypeShouldDerivedFromException(ManagedType* type, ManagedTypeSymbol* exceptionType);
				static Ptr<ManagedLanguageCodeException>						GetExceptionTypeShouldDerivedFromException(ManagedExpression* expression, ManagedTypeSymbol* exceptionType);
				static Ptr<ManagedLanguageCodeException>						GetIllegalThis(ManagedThisExpression* expression);
				static Ptr<ManagedLanguageCodeException>						GetIllegalBase(ManagedBaseExpression* expression);
				static Ptr<ManagedLanguageCodeException>						GetExpressionIsNotValue(ManagedExpression* expression);
				static Ptr<ManagedLanguageCodeException>						GetMatchedMethodNotExists(ManagedExpression* expression);
				static Ptr<ManagedLanguageCodeException>						GetMatchedMethodTooMuch(ManagedExpression* expression);
				static Ptr<ManagedLanguageCodeException>						GetIllegalNull(ManagedNullExpression* expression);
				static Ptr<ManagedLanguageCodeException>						GetCannotDetermineContainerElementType(ManagedForEachStatement* statement, ManagedTypeSymbol* containerType);
				static Ptr<ManagedLanguageCodeException>						GetCannotDisposeNonIDisposableType(ManagedUsingStatement* statement, ManagedTypeSymbol* containerType);
				static Ptr<ManagedLanguageCodeException>						GetIllegalFunctionResult(ManagedFunctionResultExpression* expression);
				static Ptr<ManagedLanguageCodeException>						GetIllegalInstanciation(ManagedInstantiatedExpression* expression);
				static Ptr<ManagedLanguageCodeException>						GetInstanciationFailedGenericArgumentCountNotMatched(ManagedInstantiatedExpression* expression);
				static Ptr<ManagedLanguageCodeException>						GetCannotAccessMembersOfNull(ManagedExpression* expression);
			};
		}
	}
}

#endif