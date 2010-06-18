#include "NativeXErrorMessage.h"

namespace vl
{
	namespace scripting
	{
		namespace language_nativex
		{
			WString NativeXErrorMessage::UnrecognizedToken(const WString& token)
			{
				return L"Unrecognized Token \""+token+L".\"";
			}

			WString NativeXErrorMessage::UnitAlreadyExists(const WString& name)
			{
				return L"Unit "+name+L" already exists.";
			}

			WString NativeXErrorMessage::UnitNotExists(const WString& name)
			{
				return L"Unit "+name+L" not exists.";
			}

			WString NativeXErrorMessage::UnitCircularReferenced(const WString& name)
			{
				return L"Unit "+name+L" has circular unit imports.";
			}

			WString NativeXErrorMessage::NeedExpression()
			{
				return L"Here needs an expression.";
			}

			WString NativeXErrorMessage::NeedType()
			{
				return L"Here needs a type.";
			}

			WString NativeXErrorMessage::NeedStatement()
			{
				return L"Here needs a statement.";
			}

			WString NativeXErrorMessage::NeedDeclaration()
			{
				return L"Here needs a declaration.";
			}

			WString NativeXErrorMessage::NeedID()
			{
				return L"Here needs a name.";
			}

			WString NativeXErrorMessage::NeedLt()
			{
				return L"Here needs an open angle bracket \"<\".";
			}

			WString NativeXErrorMessage::NeedGt()
			{
				return L"Here needs a close angle bracket \">\".";
			}

			WString NativeXErrorMessage::NeedOpenBrace()
			{
				return L"Here needs an open brace \"(\".";
			}

			WString NativeXErrorMessage::NeedCloseBrace()
			{
				return L"Here needs a close brace \")\".";
			}

			WString NativeXErrorMessage::NeedCloseArray()
			{
				return L"Here needs a close square bracket \"]\".";
			}

			WString NativeXErrorMessage::NeedComma()
			{
				return L"Here needs a comma \",\".";
			}

			WString NativeXErrorMessage::NeedSemicolon()
			{
				return L"Here needs a semicolon \";\".";
			}

			WString NativeXErrorMessage::NeedCloseStat()
			{
				return L"Here needs a close statement bracket \"}\".";
			}

			WString NativeXErrorMessage::NeedAssign()
			{
				return L"Here needs an assignment operator \"=\".";
			}

			WString NativeXErrorMessage::NeedOpenStruct()
			{
				return L"Here needs a open structure bracket \"{\".";
			}

			WString NativeXErrorMessage::NeedCloseStruct()
			{
				return L"Here needs a close structure bracket \"}\".";
			}

			WString NativeXErrorMessage::NeedWhile()
			{
				return L"Here needs \"while\".";
			}

			WString NativeXErrorMessage::NeedWhen()
			{
				return L"Here needs \"when\".";
			}

			WString NativeXErrorMessage::NeedWith()
			{
				return L"Here needs \"with\".";
			}

			WString NativeXErrorMessage::NeedDo()
			{
				return L"Here needs \"do\".";
			}

			WString NativeXErrorMessage::NeedUnit()
			{
				return L"Here needs \"unit\".";
			}

			WString NativeXErrorMessage::OperatorShouldNotBe(const WString& right, const WString& wrong)
			{
				return L"Operator "+right+L" should not be "+wrong+L".";
			}

		}
	}
}
