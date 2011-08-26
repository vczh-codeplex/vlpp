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
					SymbolAlreadyDefined,						// <all>								:name
					NamespaceNotExists,							// <ManagedUsingNamespaceDeclaration>
					SystemTypeNotExists,						// <all>								:name
					SystemTypeDuplicated,						// <all>								:name
					TypeNotExists,								// <ManagedType>
					TypeDuplicated,								// <ManagedType>
					IllegalAutoRefer,							// <ManagedAutoReferType>
					CannotDirectReferTypeRenameAsToplevel,		// <ManagedTypeRenameDeclaration>		:name
					IllegalNestedDeclaration,					// <ManagedTypeMember>
					TypeInvisible,								// <all>
					TypeInvisibleOutSideOfAssembly,				// <all>
					CannotUseUninstantiatedGenericType,			// <all>
					GenericTypeArgumentCountNotMatches,			// <all>
					GenericTypeTypeConstraintNotSatisfied,		// <all>								:index(type parameters generic argument index)
					GenericTypeNewConstraintNotSatisfied,		// <all>								:index(type parameters generic argument index)

					TypeCannotDerivedFromItself,				// <ManagedTypeDeclaration>
					ClassIllegalBaseClass,						// <ManagedTypeDeclaration>
					StructureIllegalBaseClass,					// <ManagedTypeDeclaration>
					InterfaceIllegalBaseClass,					// <ManagedTypeDeclaration>
					ClassStructureTooMuchBaseClasses,			// <ManagedTypeDeclaration>
					ClassStructureIllegalInheritation,			// <ManagedTypeDeclaration>
					InterfaceIllegalInheritation,				// <ManagedTypeDeclaration>
					TypeIllegalAccessor,						// <ManagedTypeDeclaration>
					InterfaceIllegalMemberType,					// <ManagedMember>
					InterfaceIllegalMemberAccessor,				// <ManagedMember>
					InterfaceIllegalMemberMemberType,			// <ManagedMember>
					InterfaceIllegalMemberInheritation,			// <ManagedMember>
					SealedTypeMemberIllegalAccessor,			// <ManagedMember>
					SealedTypeMemberIllegalInheritance,			// <ManagedMember>
					NonAbstractTypeMemberIllegalInheritance,	// <ManagedMember>
					MemberImplementedInterfaceTypeNotExists,	// <ManagedMember>
					MemberOverridedTargetNotExists,				// <ManagedMember>
					MemberOverridedTargetIllegalAccessor,		// <ManagedMember>

					ExpressionResolvedToDuplicatedTargets,		// <ManagedExpression>
					ExpressionResolvingFailed,					// <ManagedExpression>					:name
					ExpressionCannotConvertToType,				// <ManagedExpression>
					IllegalAutoRefWithoutInitializer,			// <ManagedVariableStatement>			:name
					VariableAlreadyExists,						// <ManagedVariableStatement>			:name
					IllegalBreak,								// <ManagedBreakStatement>
					IllegalContinue,							// <ManagedContinueStatement>
					IllegalThrow,								// <ManagedThrowStatement>
					ExceptionTypeShouldDerivedFromException,	// <ManagedType | ManagedExpression>
					IllegalThis,								// <ManagedThisExpression>
					IllegalBase,								// <ManagedBaseExpression>
					ExpressionIsNotValue,						// <ManagedExpression>
					MatchedMethodNotExists,						// <ManagedExpression>
					MatchedMethodTooMuch,						// <ManagedExpression>
					IllegalNull,								// <ManagedNullExpression>
				};
			protected:
				ManagedLanguageElement*											element;
				ManagedTypeSymbol*												typeParameter;
				ExceptionCode													exceptionCode;
				collections::List<WString>										parameters;
			public:
				ManagedLanguageCodeException(ManagedLanguageElement* _element, ExceptionCode _exceptionCode, const collections::IReadonlyList<WString>& _parameters, ManagedTypeSymbol* _typeParameter=0);
				ManagedLanguageCodeException(const ManagedLanguageCodeException& exception);
				~ManagedLanguageCodeException();

				ManagedLanguageElement*											GetManagedLanguageElement()const;
				ManagedTypeSymbol*												GetTypeParameter()const;
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

				static Ptr<ManagedLanguageCodeException>						GetTypeCannotDerivedFromItself(ManagedTypeDeclaration* declaration);
				static Ptr<ManagedLanguageCodeException>						GetClassIllegalBaseClass(ManagedTypeDeclaration* declaration);
				static Ptr<ManagedLanguageCodeException>						GetStructureIllegalBaseClass(ManagedTypeDeclaration* declaration);
				static Ptr<ManagedLanguageCodeException>						GetInterfaceIllegalBaseClass(ManagedTypeDeclaration* declaration);
				static Ptr<ManagedLanguageCodeException>						GetClassStructureTooMuchBaseClasses(ManagedTypeDeclaration* declaration);
				static Ptr<ManagedLanguageCodeException>						GetClassStructureIllegalInheritation(ManagedTypeDeclaration* declaration);
				static Ptr<ManagedLanguageCodeException>						GetInterfaceIllegalInheritation(ManagedTypeDeclaration* declaration);
				static Ptr<ManagedLanguageCodeException>						GetTypeIllegalAccessor(ManagedTypeDeclaration* declaration);
				static Ptr<ManagedLanguageCodeException>						GetInterfaceIllegalMemberType(ManagedMember* member);
				static Ptr<ManagedLanguageCodeException>						GetInterfaceIllegalMemberAccessor(ManagedMember* member);
				static Ptr<ManagedLanguageCodeException>						GetInterfaceIllegalMemberMemberType(ManagedMember* member);
				static Ptr<ManagedLanguageCodeException>						GetInterfaceIllegalMemberInheritation(ManagedMember* member);
				static Ptr<ManagedLanguageCodeException>						GetSealedTypeMemberIllegalAccessor(ManagedMember* member);
				static Ptr<ManagedLanguageCodeException>						GetSealedTypeMemberIllegalInheritance(ManagedMember* member);
				static Ptr<ManagedLanguageCodeException>						GetNonAbstractTypeMemberIllegalInheritance(ManagedMember* member);
				static Ptr<ManagedLanguageCodeException>						GetMemberImplementedInterfaceTypeNotExists(ManagedMember* member);
				static Ptr<ManagedLanguageCodeException>						GetMemberOverridedTargetNotExists(ManagedMember* member);
				static Ptr<ManagedLanguageCodeException>						GetMemberOverridedTargetIllegalAccessor(ManagedMember* member);

				static Ptr<ManagedLanguageCodeException>						GetExpressionResolvedToDuplicatedTargets(ManagedExpression* expression, ManagedTypeSymbol* type);
				static Ptr<ManagedLanguageCodeException>						GetExpressionResolvingFailed(ManagedReferenceExpression* expression);
				static Ptr<ManagedLanguageCodeException>						GetExpressionResolvingFailed(ManagedMemberExpression* expression);
				static Ptr<ManagedLanguageCodeException>						GetExpressionCannotConvertToType(ManagedExpression* expression, ManagedTypeSymbol* type);
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
			};
		}
	}
}

#endif