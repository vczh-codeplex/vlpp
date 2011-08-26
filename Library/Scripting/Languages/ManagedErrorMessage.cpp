#include "ManagedErrorMessage.h"

namespace vl
{
	namespace scripting
	{
		namespace managedlanguage
		{
			WString ManagedErrorMessage::SymbolAlreadyDefined(const WString& name)
			{
				return L"Symbol "+name+L" already defined.";
			}

			WString ManagedErrorMessage::NamespaceNotExists(const WString& name)
			{
				return L"Namespace "+name+L" does not exists.";
			}

			WString ManagedErrorMessage::FullPathTypeNotExists(const WString& name)
			{
				return L"Type "+name+L" does not exist.";
			}

			WString ManagedErrorMessage::FullPathTypeDuplicated(const WString& name)
			{
				return L"Multiple types of "+name+L" found.";
			}

			WString ManagedErrorMessage::ScopedTypeNotExists(const WString& name)
			{
				return L"Type "+name+L" does not exist in the current scope.";
			}

			WString ManagedErrorMessage::ScopedTypeDuplicated(const WString& name)
			{
				return L"Multiple types of "+name+L" found in the current scope.";
			}

			WString ManagedErrorMessage::IllegalAutoRefer(const WString& autoRefType)
			{
				return L"The Auto refer type \""+autoRefType+L"\" can only be used in variable declaration statement, using statement, foreach statement and lambda expression parameter/return type.";
			}

			WString ManagedErrorMessage::CannotDirectReferTypeRenameAsToplevel(const WString& name)
			{
				return L"Type rename declaration "+name+L" cannot target to a type whose top level type is another type rename declaration.";
			}

			WString ManagedErrorMessage::IllegalNestedDeclaration()
			{
				return L"Only class, structure, interface, enumeration and type rename can be defined as a nested type.";
			}

			WString ManagedErrorMessage::TypeInvisible(const WString& name)
			{
				return L"Type "+name+L" is invisible at the current scope.";
			}

			WString ManagedErrorMessage::TypeInvisibleOutSideOfAssembly(const WString& name)
			{
				return L"The accessor of type "+name+L" is incompatible of the current declaration so that it is invisible out side of the assembly.";
			}

			WString ManagedErrorMessage::CannotUseUninstantiatedGenericType(const WString& name)
			{
				return L"Cannot use uninstantiated type "+name+L" as generic argument or to declare object.";
			}

			WString ManagedErrorMessage::GenericTypeArgumentCountNotMatches(const WString& name)
			{
				return L"Generic argument count of type "+name+L" do not match the declaration.";
			}

			WString ManagedErrorMessage::GenericTypeTypeConstraintNotSatisfied(const WString& name, const WString& index)
			{
				return L"The "+index+L"th generic argument of type "+name+L" does not satisfies the required type constraint.";
			}

			WString ManagedErrorMessage::GenericTypeNewConstraintNotSatisfied(const WString& name, const WString& index)
			{
				return L"The "+index+L"th generic argument of type "+name+L" does not satisfies the required new constraint.";
			}

			WString ManagedErrorMessage::TypeCannotDerivedFromItself(const WString& name)
			{
				return L"Type "+name+L" cannot derived from itself directly or indirectly.";
			}

			WString ManagedErrorMessage::ClassIllegalBaseClass(const WString& name)
			{
				return L"Class "+name+L" can only derived from a non-sealed class.";
			}

			WString ManagedErrorMessage::StructureIllegalBaseClass(const WString& name)
			{
				return L"Structure "+name+L" can only derived from Object or a non-sealed structure with StructBaseAttribute.";
			}

			WString ManagedErrorMessage::InterfaceIllegalBaseClass(const WString& name)
			{
				return L"Interface "+name+L" can only derived from other interfaces.";
			}

			WString ManagedErrorMessage::ClassStructureTooMuchBaseClasses(const WString& name)
			{
				return L"Type "+name+L" can only derived from one non-interface type.";
			}

			WString ManagedErrorMessage::ClassStructureIllegalInheritation(const WString& name)
			{
				return L"Type "+name+L" cannot be virtual or overrided.";
			}

			WString ManagedErrorMessage::InterfaceIllegalInheritation(const WString& name)
			{
				return L"Interface "+name+L" should be abstract.";
			}

			WString ManagedErrorMessage::TypeIllegalAccessor(const WString& name)
			{
				return L"Type "+name+L" cannot be protected or protected internal when it is not a sub type.";
			}

			WString ManagedErrorMessage::InterfaceIllegalMemberType(const WString& name, const WString& member)
			{
				return L"Member "+member+L" cannot be declared in interface "+name+L" because it is not a property, a method or a converter operator.";
			}

			WString ManagedErrorMessage::InterfaceIllegalMemberAccessor(const WString& name, const WString& member)
			{
				return L"Member "+member+L" in interface "+name+L" should be public.";
			}

			WString ManagedErrorMessage::InterfaceIllegalMemberMemberType(const WString& name, const WString& member)
			{
				return L"Member "+member+L" in interface "+name+L" cannot be static.";
			}

			WString ManagedErrorMessage::InterfaceIllegalMemberInheritation(const WString& name, const WString& member)
			{
				return L"Member "+member+L" in interface "+name+L" should be abstract.";
			}

			WString ManagedErrorMessage::SealedTypeMemberIllegalAccessor(const WString& name, const WString& member)
			{
				return L"Member "+member+L" in sealed type "+name+L" can only be private, internal or public.";
			}

			WString ManagedErrorMessage::SealedTypeMemberIllegalInheritance(const WString& name, const WString& member)
			{
				return L"Member "+member+L" in sealed type "+name+L" cannot be abstract, virtual or override.";
			}

			WString ManagedErrorMessage::NonAbstractTypeMemberIllegalInheritance(const WString& name, const WString& member)
			{
				return L"Abstract member "+member+L" cannot be declared in non-abstract type "+name+L".";
			}

			WString ManagedErrorMessage::MemberImplementedInterfaceTypeNotExists(const WString& name, const WString& member)
			{
				return L"The interface type specified in member "+member+L" is not derived by type "+L"{type}"+L".";
			}

			WString ManagedErrorMessage::MemberOverridedTargetNotExists(const WString& name, const WString& member)
			{
				return L"Member "+member+L" in type "+L"{type}"+L" cannot find a correct member to override.";
			}

			WString ManagedErrorMessage::MemberOverridedTargetIllegalAccessor(const WString& name, const WString& member)
			{
				return L"Member "+member+L" in type "+L"{type}"+L" find a correct member to override, but the accessor doesn't match.";
			}

			WString ManagedErrorMessage::ExpressionResolvedToDuplicatedTargets()
			{
				return L"Expression resolved to duplicated targets.";
			}

			WString ManagedErrorMessage::ExpressionResolvedToDuplicatedTargets(const WString& type)
			{
				return L"Expression resolved to duplicated targets which converts to type "+type+L".";
			}

			WString ManagedErrorMessage::ExpressionResolvingFailed(const WString& name)
			{
				return L"Failed to resolve symbol "+name+L".";
			}

			WString ManagedErrorMessage::ExpressionCannotConvertToType(const WString& type)
			{
				return L"Expression cannot convert to type "+type+L".";
			}

			WString ManagedErrorMessage::IllegalAutoRefWithoutInitializer(const WString& name, const WString& autoRefType)
			{
				return L"Variable type cannot be \""+autoRefType+L"\" if it doesn't have an initializer.";
			}

			WString ManagedErrorMessage::VariableAlreadyExists(const WString& name)
			{
				return L"Variable "+name+L" already exists.";
			}

			WString ManagedErrorMessage::IllegalBreak()
			{
				return L"Break statement can only be used in loop or switch statement.";
			}

			WString ManagedErrorMessage::IllegalContinue()
			{
				return L"Continue statement can only be used in loop statement.";
			}

			WString ManagedErrorMessage::IllegalThrow()
			{
				return L"Rethrow statement can only be used in catch clause.";
			}

			WString ManagedErrorMessage::ExceptionTypeShouldDerivedFromException(const WString& type, const WString& exceptionType)
			{
				return L"Exceptions should derived from "+exceptionType+L", but "+type+L" does not.";
			}

			WString ManagedErrorMessage::IllegalThis(const WString& expr)
			{
				return L"Expression "+expr+L" is only accessible in non-static methods, non-static converter operators and constructors.";
			}

			WString ManagedErrorMessage::IllegalBase(const WString& expr)
			{
				return L"Expression "+expr+L" is only accessible in non-static methods, non-static converter operators and constructors of a class or structure that has a base class or structure.";
			}

			WString ManagedErrorMessage::ExpressionIsNotValue(const WString& expr)
			{
				return L"Expression \""+expr+L"\" cannot be evaluated to a value.";
			}

			WString ManagedErrorMessage::MatchedMethodNotExists(const WString& expr)
			{
				return L"Cannot find a matched method or operator of expression \""+expr+L"\".";
			}

			WString ManagedErrorMessage::MatchedMethodTooMuch(const WString& expr)
			{
				return L"Matched method or operator of expression \""+expr+L"\" too much.";
			}

			WString ManagedErrorMessage::IllegalNull(const WString& expr)
			{
				return L"Cannot use "+expr+L" without context.";
			}

		}
	}
}
