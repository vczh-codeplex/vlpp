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

		}
	}
}
