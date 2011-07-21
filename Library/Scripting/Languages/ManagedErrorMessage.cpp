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

		}
	}
}
