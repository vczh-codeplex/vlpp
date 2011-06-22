#include "ManagedXErrorMessage.h"

namespace vl
{
	namespace scripting
	{
		namespace language_managedx
		{
			WString ManagedXErrorMessage::UnrecognizedToken(const WString& token)
			{
				return L"Unrecognized Token \""+token+L"\".";
			}

			WString ManagedXErrorMessage::NeedId()
			{
				return L"Here needs an identifier.";
			}

			WString ManagedXErrorMessage::NeedWhile()
			{
				return L"Here needs \"while\".";
			}

			WString ManagedXErrorMessage::NeedAs()
			{
				return L"Here needs \"as\".";
			}

			WString ManagedXErrorMessage::NeedInOut()
			{
				return L"Here needs \"in\", \"out\" or \"in out\".";
			}

			WString ManagedXErrorMessage::NeedIn()
			{
				return L"Here needs \"in\".";
			}

			WString ManagedXErrorMessage::NeedBase()
			{
				return L"Here needs \"base\".";
			}

			WString ManagedXErrorMessage::NeedOperator()
			{
				return L"Here needs an operator.";
			}

			WString ManagedXErrorMessage::NeedDot()
			{
				return L"Here needs \".\".";
			}

			WString ManagedXErrorMessage::NeedColon()
			{
				return L"Here needs \":\".";
			}

			WString ManagedXErrorMessage::NeedSemicolon()
			{
				return L"Here needs \";\".";
			}

			WString ManagedXErrorMessage::NeedLambda()
			{
				return L"Here needs \"=>\".";
			}

			WString ManagedXErrorMessage::NeedEq()
			{
				return L"Here needs \"=\".";
			}

			WString ManagedXErrorMessage::NeedLt()
			{
				return L"Here needs \"<\".";
			}

			WString ManagedXErrorMessage::NeedGt()
			{
				return L"Here needs \">\".";
			}

			WString ManagedXErrorMessage::NeedOpenDeclBrace()
			{
				return L"Here needs \"{\".";
			}

			WString ManagedXErrorMessage::NeedCloseDeclBrace()
			{
				return L"Here needs \"}\".";
			}

			WString ManagedXErrorMessage::NeedOpenArrayBrace()
			{
				return L"Here needs \"]\".";
			}

			WString ManagedXErrorMessage::NeedCloseArrayBrace()
			{
				return L"Here needs \"]\".";
			}

			WString ManagedXErrorMessage::NeedOpenExpBrace()
			{
				return L"Here needs \"(\".";
			}

			WString ManagedXErrorMessage::NeedCloseExpBrace()
			{
				return L"Here needs \")\".";
			}

			WString ManagedXErrorMessage::NeedExpression()
			{
				return L"Here needs an expression.";
			}

			WString ManagedXErrorMessage::NeedStatement()
			{
				return L"Here needs a statement.";
			}

		}
	}
}
