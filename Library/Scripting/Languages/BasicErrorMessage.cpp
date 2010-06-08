#include "BasicErrorMessage.h"

namespace vl
{
	namespace scripting
	{
		namespace basiclanguage
		{
			WString BasicErrorMessage::TypeNameNotExists(const WString& type)
			{
				return L"Type "+type+L" does not exists.";
			}

			WString BasicErrorMessage::FunctionAlreadyExists(const WString& name)
			{
				return L"Function "+name+L" already exists.";
			}

			WString BasicErrorMessage::VariableAlreadyExists(const WString& name)
			{
				return L"Variable "+name+L" already exists.";
			}

			WString BasicErrorMessage::TypeAlreadyExists(const WString& name)
			{
				return L"Type "+name+L" already exists.";
			}

			WString BasicErrorMessage::StructureMemberAlreadyExists(const WString& name)
			{
				return L"Member "+name+L" already exists.";
			}

			WString BasicErrorMessage::VariableNotExists(const WString& name)
			{
				return L"Variable "+name+L" not exists.";
			}

			WString BasicErrorMessage::FailToCast(const WString& from, const WString& to)
			{
				return L"Fail to cast from "+from+L" to "+to+L".";
			}

			WString BasicErrorMessage::VoidFunctionNotHaveResult()
			{
				return L"Cannot access function result in a function without return value.";
			}

			WString BasicErrorMessage::GlobalNotHaveResult()
			{
				return L"Cannot access function result in global definitions.";
			}

			WString BasicErrorMessage::CannotInvokeNonFunctionValue(const WString& type)
			{
				return L"Cannot invoke a value of "+type+L".";
			}

			WString BasicErrorMessage::ArgumentNumnerNotMatch()
			{
				return L"Argument number should not be greater or less than the function required.";
			}

			WString BasicErrorMessage::ArgumentTypeNotMatch(const WString& index, const WString& from, const WString& to)
			{
				return L"Cannot implicitly cast argument "+index+L" from "+from+L" to "+to+L".";
			}

			WString BasicErrorMessage::StructureMemberNotExists(const WString& name)
			{
				return L"Member "+name+L" not exists.";
			}

			WString BasicErrorMessage::CannotConvertIndexToInt(const WString& from, const WString& to)
			{
				return L"Cannot implicit cast the index from "+from+L" to "+to+L".";
			}

			WString BasicErrorMessage::CannotSubscribe(const WString& type)
			{
				return L"Cannot subscribe a value of "+type+L".";
			}

			WString BasicErrorMessage::UnaryOperandShouldBeLeftValue()
			{
				return L"Operand should be left value.";
			}

			WString BasicErrorMessage::UnaryTypeNotMatch(const WString& op, const WString& type)
			{
				return L"Unary operator "+op+L" cannot apply to a value of "+type+L".";
			}

			WString BasicErrorMessage::BinaryLeftOperandShouldBeLeftValue(const WString& op)
			{
				return L"Left operand of binary operator "+op+L" should be left value.";
			}

			WString BasicErrorMessage::BinaryTypeNotMatch(const WString& op, const WString& left, const WString& right)
			{
				return L"Binary operator "+op+L" cannot apply to values of "+left+L" and "+right+L".";
			}

			WString BasicErrorMessage::ConditionCannotConvertToBool(const WString& from, const WString& boolean)
			{
				return L"Cannot convert the condition from "+from+L" to "+boolean+L".";
			}

			WString BasicErrorMessage::BreakShouldBeInLooping(const WString& breakStatement)
			{
				return breakStatement+L" should be used in a loop.";
			}

			WString BasicErrorMessage::ContinueShouldBeInLooping(const WString& continueStatement)
			{
				return continueStatement+L" should be used in a loop.";
			}

			WString BasicErrorMessage::InitializerTypeNotMatch(const WString& from, const WString& to)
			{
				return L"Cannot convert the variable initializer from "+from+L" to "+to+L".";
			}

			WString BasicErrorMessage::ParameterCountNotMatch()
			{
				return L"Parameter number should not be greater or less than the function required.";
			}

			WString BasicErrorMessage::ParameterAlreadyExists(const WString& name)
			{
				return L"Parameter "+name+L" already exists.";
			}

			WString BasicErrorMessage::StructureMemberCannotBeUndefinedType(const WString& name)
			{
				return L"Cannot refer to an undefined structure in member "+name+L".";
			}

			WString BasicErrorMessage::LeftOperandShouldBeStructure()
			{
				return L"Left operand should be a structure.";
			}

			WString BasicErrorMessage::LeftOperandShouldBePointerToStructure()
			{
				return L"Left operand should be a pointer to a structure.";
			}

			WString BasicErrorMessage::PredeclaredStructureShouldBeDefined(const WString& name)
			{
				return L"Predeclared structure "+name+L" should be defined.";
			}

			WString BasicErrorMessage::ILExceptionStackOverflow()
			{
				return L"Stack overflow.";
			}

			WString BasicErrorMessage::ILExceptionDividByZero()
			{
				return L"Divided by zero.";
			}

			WString BasicErrorMessage::ILExceptionAccessViolation()
			{
				return L"Access violation.";
			}

			WString BasicErrorMessage::ILExceptionInstructionIndexOutOfRange()
			{
				return L"Instruction index out of range.";
			}

			WString BasicErrorMessage::ILExceptionUnknownInstruction()
			{
				return L"Unknown instruction.";
			}

			WString BasicErrorMessage::ILExceptionBadInstructionArgument()
			{
				return L"Bad instruction argument.";
			}

			WString BasicErrorMessage::ILLinkerExceptionDuplicatedAssemblyName(const WString& assembly)
			{
				return L"Assembly "+assembly+L" exists. Cannot load an assembly that conflict with existing names.";
			}

			WString BasicErrorMessage::ILLinkerExceptionAssemblyNotExists(const WString& assembly)
			{
				return L"Assembly "+assembly+L" has not been loaded.";
			}

			WString BasicErrorMessage::ILLinkerExceptionDuplicatedSymbolName(const WString& assembly, const WString& symbol)
			{
				return L"Symbol "+symbol+L" in assembly "+assembly+L" exists. Assembly cannot have two symbols of the same name.";
			}

			WString BasicErrorMessage::ILLinkerExceptionSymbolNotExists(const WString& assembly, const WString& symbol)
			{
				return L"Cannot find symbol "+symbol+L" in assembly "+assembly+L".";
			}

		}
	}
}
