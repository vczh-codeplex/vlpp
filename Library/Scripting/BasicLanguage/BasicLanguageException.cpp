#include "BasicLanguageException.h"
#include "..\..\Collections\OperationCopyFrom.h"

namespace vl
{
	namespace scripting
	{
		namespace basiclanguage
		{
			using namespace collections;

/***********************************************************************
BasicLanguageCodeException
***********************************************************************/

			BasicLanguageCodeException::BasicLanguageCodeException(BasicLanguageElement* _element, ExceptionCode _exceptionCode, const collections::IReadonlyList<WString>& _parameters)
				:Exception(L"����BasicLanguage�����������")
				,element(_element)
				,exceptionCode(_exceptionCode)
			{
				CopyFrom(parameters.Wrap(), _parameters);
			}

			BasicLanguageCodeException::BasicLanguageCodeException(const BasicLanguageCodeException& exception)
				:Exception(exception.Message())
				,element(exception.element)
				,exceptionCode(exception.exceptionCode)
			{
				CopyFrom(parameters.Wrap(), exception.parameters.Wrap());
			}

			BasicLanguageCodeException::~BasicLanguageCodeException()
			{
			}

			BasicLanguageElement* BasicLanguageCodeException::GetBasicLanguageElement()const
			{
				return element;
			}

			BasicLanguageCodeException::ExceptionCode BasicLanguageCodeException::GetExceptionCode()const
			{
				return exceptionCode;
			}

			const collections::IReadonlyList<WString>& BasicLanguageCodeException::GetParameters()const
			{
				return parameters.Wrap();
			}

			Ptr<BasicLanguageCodeException> BasicLanguageCodeException::GetTypeNameNotExists(BasicReferenceType* type)
			{
				Array<WString> parameters(1);
				parameters[0]=type->name;
				return new BasicLanguageCodeException(type, TypeNameNotExists, parameters.Wrap());
			}

			Ptr<BasicLanguageCodeException> BasicLanguageCodeException::GetFunctionAlreadyExists(BasicFunctionDeclaration* function)
			{
				Array<WString> parameters(1);
				parameters[0]=function->name;
				return new BasicLanguageCodeException(function, FunctionAlreadyExists, parameters.Wrap());
			}

			Ptr<BasicLanguageCodeException> BasicLanguageCodeException::GetVariableAlreadyExists(BasicVariableDeclaration* variable)
			{
				Array<WString> parameters(1);
				parameters[0]=variable->name;
				return new BasicLanguageCodeException(variable, VariableAlreadyExists, parameters.Wrap());
			}

			Ptr<BasicLanguageCodeException> BasicLanguageCodeException::GetVariableAlreadyExists(BasicVariableStatement* statement)
			{
				Array<WString> parameters(1);
				parameters[0]=statement->name;
				return new BasicLanguageCodeException(statement, VariableAlreadyExists, parameters.Wrap());
			}

			Ptr<BasicLanguageCodeException> BasicLanguageCodeException::GetTypeAlreadyExists(BasicStructureDeclaration* type)
			{
				Array<WString> parameters(1);
				parameters[0]=type->name;
				return new BasicLanguageCodeException(type, TypeAlreadyExists, parameters.Wrap());
			}

			Ptr<BasicLanguageCodeException> BasicLanguageCodeException::GetTypeAlreadyExists(BasicTypeRenameDeclaration* type)
			{
				Array<WString> parameters(1);
				parameters[0]=type->name;
				return new BasicLanguageCodeException(type, TypeAlreadyExists, parameters.Wrap());
			}

			Ptr<BasicLanguageCodeException> BasicLanguageCodeException::GetStructureMemberAlreadyExists(BasicStructureDeclaration* type, int memberIndex)
			{
				Array<WString> parameters(1);
				parameters[0]=type->memberNames[memberIndex];
				return new BasicLanguageCodeException(type, TypeAlreadyExists, parameters.Wrap());
			}

			Ptr<BasicLanguageCodeException> BasicLanguageCodeException::GetVariableNotExists(BasicReferenceExpression* variable)
			{
				Array<WString> parameters(1);
				parameters[0]=variable->name;
				return new BasicLanguageCodeException(variable, VariableNotExists, parameters.Wrap());
			}

			Ptr<BasicLanguageCodeException> BasicLanguageCodeException::GetFailToCast(BasicCastingExpression* casting)
			{
				Array<WString> parameters(0);
				return new BasicLanguageCodeException(casting, FailToCast, parameters.Wrap());
			}

			Ptr<BasicLanguageCodeException> BasicLanguageCodeException::GetVoidFunctionNotHaveResult(BasicFunctionResultExpression* result)
			{
				Array<WString> parameters(0);
				return new BasicLanguageCodeException(result, VoidFunctionNotHaveResult, parameters.Wrap());
			}

			Ptr<BasicLanguageCodeException> BasicLanguageCodeException::GetGlobalNotHaveResult(BasicFunctionResultExpression* result)
			{
				Array<WString> parameters(0);
				return new BasicLanguageCodeException(result, GlobalNotHaveResult, parameters.Wrap());
			}

			Ptr<BasicLanguageCodeException> BasicLanguageCodeException::GetCannotInvokeNonFunctionValue(BasicInvokeExpression* invoke)
			{
				Array<WString> parameters(0);
				return new BasicLanguageCodeException(invoke, CannotInvokeNonFunctionValue, parameters.Wrap());
			}

			Ptr<BasicLanguageCodeException> BasicLanguageCodeException::GetArgumentNumberNotMatch(BasicInvokeExpression* invoke)
			{
				Array<WString> parameters(0);
				return new BasicLanguageCodeException(invoke, ArgumentNumberNotMatch, parameters.Wrap());
			}

			Ptr<BasicLanguageCodeException> BasicLanguageCodeException::GetArgumentTypeNotMatch(BasicInvokeExpression* invoke, int parameterIndex)
			{
				Array<WString> parameters(1);
				parameters[0]=itow(parameterIndex);
				return new BasicLanguageCodeException(invoke, ArgumentTypeNotMatch, parameters.Wrap());
			}

			Ptr<BasicLanguageCodeException> BasicLanguageCodeException::GetStructureMemberNotExists(BasicMemberExpression* member)
			{
				Array<WString> parameters(1);
				parameters[0]=member->member;
				return new BasicLanguageCodeException(member, StructureMemberNotExists, parameters.Wrap());
			}

			Ptr<BasicLanguageCodeException> BasicLanguageCodeException::GetCannotConvertIndexToInt(BasicSubscribeExpression* subscribe)
			{
				Array<WString> parameters(0);
				return new BasicLanguageCodeException(subscribe, CannotConvertIndexToInt, parameters.Wrap());
			}

			Ptr<BasicLanguageCodeException> BasicLanguageCodeException::GetCannotSubscribe(BasicSubscribeExpression* subscribe)
			{
				Array<WString> parameters(0);
				return new BasicLanguageCodeException(subscribe, CannotSubscribe, parameters.Wrap());
			}

			Ptr<BasicLanguageCodeException> BasicLanguageCodeException::GetUnaryOperandShouldBeLeftValue(BasicUnaryExpression* unary)
			{
				Array<WString> parameters(0);
				return new BasicLanguageCodeException(unary, UnaryOperandShouldBeLeftValue, parameters.Wrap());
			}

			Ptr<BasicLanguageCodeException> BasicLanguageCodeException::GetUnaryTypeNotMatch(BasicUnaryExpression* unary)
			{
				Array<WString> parameters(0);
				return new BasicLanguageCodeException(unary, UnaryTypeNotMatch, parameters.Wrap());
			}

			Ptr<BasicLanguageCodeException> BasicLanguageCodeException::GetBinaryLeftOperandShouldBeLeftValue(BasicBinaryExpression* binary)
			{
				Array<WString> parameters(0);
				return new BasicLanguageCodeException(binary, BinaryLeftOperandShouldBeLeftValue, parameters.Wrap());
			}

			Ptr<BasicLanguageCodeException> BasicLanguageCodeException::GetBinaryTypeNotMatch(BasicBinaryExpression* binary)
			{
				Array<WString> parameters(0);
				return new BasicLanguageCodeException(binary, BinaryTypeNotMatch, parameters.Wrap());
			}

			Ptr<BasicLanguageCodeException> BasicLanguageCodeException::GetConditionCannotConvertToBool(BasicExpression* expression)
			{
				Array<WString> parameters(0);
				return new BasicLanguageCodeException(expression, ConditionCannotConvertToBool, parameters.Wrap());
			}

			Ptr<BasicLanguageCodeException> BasicLanguageCodeException::GetBreakShouldBeInLooping(BasicBreakStatement* statement)
			{
				Array<WString> parameters(0);
				return new BasicLanguageCodeException(statement, BreakShouldBeInLooping, parameters.Wrap());
			}

			Ptr<BasicLanguageCodeException> BasicLanguageCodeException::GetContinueShouldBeInLooping(BasicContinueStatement* statement)
			{
				Array<WString> parameters(0);
				return new BasicLanguageCodeException(statement, ContinueShouldBeInLooping, parameters.Wrap());
			}

			Ptr<BasicLanguageCodeException> BasicLanguageCodeException::GetInitializerTypeNotMatch(BasicVariableStatement* statement)
			{
				Array<WString> parameters(0);
				return new BasicLanguageCodeException(statement, InitializerTypeNotMatch, parameters.Wrap());
			}

			Ptr<BasicLanguageCodeException> BasicLanguageCodeException::GetParameterCountNotMatch(BasicFunctionDeclaration* declaration)
			{
				Array<WString> parameters(0);
				return new BasicLanguageCodeException(declaration, ParameterCountNotMatch, parameters.Wrap());
			}

			Ptr<BasicLanguageCodeException> BasicLanguageCodeException::GetParameterAlreadyExists(BasicFunctionDeclaration* declaration, int parameterIndex)
			{
				Array<WString> parameters(1);
				parameters[0]=itow(parameterIndex);
				return new BasicLanguageCodeException(declaration, ParameterAlreadyExists, parameters.Wrap());
			}
		}
	}
}