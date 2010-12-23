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

			WString BasicErrorMessage::ExternalFunctionCannotHaveStatement(const WString& name)
			{
				return L"External function "+name+L" cannot have a statement.";
			}

			WString BasicErrorMessage::LocalFunctionShouldHaveStatement(const WString& name)
			{
				return L"Local function "+name+L" should have a statement.";
			}

			WString BasicErrorMessage::ExternalVariableCannotHaveInitializer(const WString& name)
			{
				return L"External variable "+name+L" cannot have an initializer.";
			}

			WString BasicErrorMessage::ExternalStructureShouldBeDefined(const WString& name)
			{
				return L"External structure "+name+L" should be defined.";
			}

			WString BasicErrorMessage::GenericArgumentAlreadyExists(const WString& declaration, const WString& name)
			{
				return L"Generic argument "+name+L" duplicated in declaration "+declaration+L".";
			}

			WString BasicErrorMessage::GenericArgumentNotExists(const WString& declaration, const WString& name)
			{
				return L"Generic argument "+name+L" does not exist in declaration "+declaration+L".";
			}

			WString BasicErrorMessage::CannotUseUninstanciatedGenericType()
			{
				return L"Cannot use uninstanciated generic types.";
			}

			WString BasicErrorMessage::CannotUseUninstanciatedGenericVariable(const WString& name)
			{
				return L"Cannot use uninstanciated generic variable "+name+L".";
			}

			WString BasicErrorMessage::CannotUseUninstanciatedGenericFunction(const WString& name)
			{
				return L"Cannot use uninstanciated generic function "+name+L".";
			}

			WString BasicErrorMessage::GenericArgumentNumberNotMatch()
			{
				return L"Generic argument number should not be greater or less than the generic type required.";
			}

			WString BasicErrorMessage::GenericArgumentCannotApplyToNonGenericType()
			{
				return L"Generic argument cannot apply to non-generic type.";
			}

			WString BasicErrorMessage::ConceptFunctionAlreadyExists(const WString& name, const WString& function)
			{
				return L"Concept "+name+L" already have a function called "+function+L".";
			}

			WString BasicErrorMessage::ConceptFunctionNotExists(const WString& name, const WString& function)
			{
				return L"Concept "+name+L" does not have a function called "+function+L".";
			}

			WString BasicErrorMessage::ConceptAlreadyExists(const WString& name)
			{
				return L"Concept "+name+L" already exists;";
			}

			WString BasicErrorMessage::ConceptNotExists(const WString& name)
			{
				return L"Concept "+name+L" not exists.";
			}

			WString BasicErrorMessage::InstanceTypeNotCorrect(const WString& name)
			{
				return L"Instance "+name+L" can only apply to primitive types or raw generic types.";
			}

			WString BasicErrorMessage::ConceptFunctionTypeNotMatches(const WString& name, const WString& function)
			{
				return L"Concept "+name+L" has a function "+function+L" with a different type.";
			}

			WString BasicErrorMessage::InstanceShouldHaveFunction(const WString& name, const WString& function)
			{
				return L"Instance "+name+L" should have a function "+function+L".";
			}

			WString BasicErrorMessage::InstanceShouldNotHaveFunction(const WString& name, const WString& function)
			{
				return L"Concept "+name+L" should not have a function "+function+L".";
			}

			WString BasicErrorMessage::InstanceShouldBeDeclaredOnType(const WString& type, const WString& conceptName)
			{
				return L"An instance of concept "+conceptName+L" on type "+type+L" is needed.";
			}

			WString BasicErrorMessage::CannotThrowVoidValue()
			{
				return L"Cannot throw an expression that returns no value.";
			}

			WString BasicErrorMessage::ForeignFunctionCannotBeGeneric(const WString& name)
			{
				return L"Foreign function "+name+L" cannot be generic.";
			}

			WString BasicErrorMessage::AttributeCannotApplyOnFunctionDeclaration(const WString& attribute, const WString& name)
			{
				return L"Attribute "+attribute+L" cannot apply on function declaration "+name+L".";
			}

			WString BasicErrorMessage::AttributeCannotApplyOnStructureDeclaration(const WString& attribute, const WString& name)
			{
				return L"Attribute "+attribute+L" cannot apply on structure declaration "+name+L".";
			}

			WString BasicErrorMessage::AttributeCannotApplyOnVariableDeclaration(const WString& attribute, const WString& name)
			{
				return L"Attribute "+attribute+L" cannot apply on variable declaration "+name+L".";
			}

			WString BasicErrorMessage::AttributeCannotApplyOnTypeRenameDeclaration(const WString& attribute, const WString& name)
			{
				return L"Attribute "+attribute+L" cannot apply on type rename declaration "+name+L".";
			}

			WString BasicErrorMessage::AttributeCannotApplyOnConceptDeclaration(const WString& attribute, const WString& name)
			{
				return L"Attribute "+attribute+L" cannot apply on concept declaration "+name+L".";
			}

			WString BasicErrorMessage::AttributeCannotApplyOnInstanceDeclaration(const WString& attribute, const WString& name)
			{
				return L"Attribute "+attribute+L" cannot apply on instance declaration "+name+L".";
			}

			WString BasicErrorMessage::PublicDeclarationCannotUseNonPublicType(const WString& name, const WString& type)
			{
				return L"Public declaration "+name+L" cannot use non public type "+type+L".";
			}

			WString BasicErrorMessage::InstanceDeclaredOnPublicConceptShouldBePublic(const WString& concept)
			{
				return L"Instance declared on public concept "+concept+L" should be public.";
			}

			WString BasicErrorMessage::InstanceDeclaredOnNonPublicConceptShouldBeNonPublic(const WString& concept)
			{
				return L"Instance declared on non public concept "+concept+L" should be non public.";
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

			WString BasicErrorMessage::ILExceptionUnhandledException()
			{
				return L"Unhandled exception raised.";
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

			WString BasicErrorMessage::ILLinkerExceptionSymbolNotALabel(const WString& assembly, const WString& symbol)
			{
				return L"Symbol "+symbol+L" in assembly "+assembly+L" does not associated with an instruction entry.";
			}

			WString BasicErrorMessage::ILLinkerExceptionDuplicatedInstance(const WString& name)
			{
				return L"Instance "+name+L" already exists.";
			}

			WString BasicErrorMessage::ILLinkerExceptionForeignFunctionNotExists(const WString& assembly, const WString& symbol)
			{
				return L"Cannot find foreign function "+assembly+L"::"+symbol+L".";
			}

		}
	}
}
