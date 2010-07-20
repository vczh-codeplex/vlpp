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
				:Exception(L"发生BasicLanguage语义分析错误。")
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

			Ptr<BasicLanguageCodeException> BasicLanguageCodeException::GetFunctionAlreadyExists(BasicDeclaration* function)
			{
				Array<WString> parameters(1);
				parameters[0]=function->name;
				return new BasicLanguageCodeException(function, FunctionAlreadyExists, parameters.Wrap());
			}

			Ptr<BasicLanguageCodeException> BasicLanguageCodeException::GetVariableAlreadyExists(BasicDeclaration* variable)
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

			Ptr<BasicLanguageCodeException> BasicLanguageCodeException::GetTypeAlreadyExists(BasicDeclaration* type)
			{
				Array<WString> parameters(1);
				parameters[0]=type->name;
				return new BasicLanguageCodeException(type, TypeAlreadyExists, parameters.Wrap());
			}

			Ptr<BasicLanguageCodeException> BasicLanguageCodeException::GetStructureMemberAlreadyExists(BasicStructureDeclaration* type, vint memberIndex)
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

			Ptr<BasicLanguageCodeException> BasicLanguageCodeException::GetArgumentTypeNotMatch(BasicInvokeExpression* invoke, vint parameterIndex)
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

			Ptr<BasicLanguageCodeException> BasicLanguageCodeException::GetInitializerTypeNotMatch(BasicVariableDeclaration* declaration)
			{
				Array<WString> parameters(0);
				return new BasicLanguageCodeException(declaration, InitializerTypeNotMatch, parameters.Wrap());
			}

			Ptr<BasicLanguageCodeException> BasicLanguageCodeException::GetParameterCountNotMatch(BasicFunctionDeclaration* declaration)
			{
				Array<WString> parameters(0);
				return new BasicLanguageCodeException(declaration, ParameterCountNotMatch, parameters.Wrap());
			}

			Ptr<BasicLanguageCodeException> BasicLanguageCodeException::GetParameterAlreadyExists(BasicFunctionDeclaration* declaration, vint parameterIndex)
			{
				Array<WString> parameters(1);
				parameters[0]=itow(parameterIndex);
				return new BasicLanguageCodeException(declaration, ParameterAlreadyExists, parameters.Wrap());
			}

			Ptr<BasicLanguageCodeException> BasicLanguageCodeException::GetStructureMemberCannotBeUndefinedType(BasicStructureDeclaration* declaration, vint memberIndex)
			{
				Array<WString> parameters(1);
				parameters[0]=declaration->memberNames[memberIndex];
				return new BasicLanguageCodeException(declaration, StructureMemberCannotBeUndefinedType, parameters.Wrap());
			}

			Ptr<BasicLanguageCodeException> BasicLanguageCodeException::GetLeftOperandShouldBeStructure(BasicMemberExpression* declaration)
			{
				Array<WString> parameters(0);
				return new BasicLanguageCodeException(declaration, LeftOperandShouldBeStructure, parameters.Wrap());
			}

			Ptr<BasicLanguageCodeException> BasicLanguageCodeException::GetLeftOperandShouldBePointerToStructure(BasicMemberExpression* expression)
			{
				Array<WString> parameters(0);
				return new BasicLanguageCodeException(expression, LeftOperandShouldBePointerToStructure, parameters.Wrap());
			}

			Ptr<BasicLanguageCodeException> BasicLanguageCodeException::GetPredeclaredStructureShouldBeDefined(BasicStructureDeclaration* declaration)
			{
				Array<WString> parameters(0);
				return new BasicLanguageCodeException(declaration, PredeclaredStructureShouldBeDefined, parameters.Wrap());
			}

			Ptr<BasicLanguageCodeException> BasicLanguageCodeException::GetExternalFunctionCannotHaveStatement(BasicFunctionDeclaration* declaration)
			{
				Array<WString> parameters(1);
				parameters[0]=declaration->name;
				return new BasicLanguageCodeException(declaration, ExternalFunctionCannotHaveStatement, parameters.Wrap());
			}

			Ptr<BasicLanguageCodeException> BasicLanguageCodeException::GetLocalFunctionShouldHaveStatement(BasicFunctionDeclaration* declaration)
			{
				Array<WString> parameters(1);
				parameters[0]=declaration->name;
				return new BasicLanguageCodeException(declaration, LocalFunctionShouldHaveStatement, parameters.Wrap());
			}

			Ptr<BasicLanguageCodeException> BasicLanguageCodeException::GetExternalVariableCannotHaveInitializer(BasicVariableDeclaration* declaration)
			{
				Array<WString> parameters(1);
				parameters[0]=declaration->name;
				return new BasicLanguageCodeException(declaration, ExternalVariableCannotHaveInitializer, parameters.Wrap());
			}

			Ptr<BasicLanguageCodeException> BasicLanguageCodeException::GetExternalStructureShouldBeDefined(BasicStructureDeclaration* declaration)
			{
				Array<WString> parameters(1);
				parameters[0]=declaration->name;
				return new BasicLanguageCodeException(declaration, ExternalStructureShouldBeDefined, parameters.Wrap());
			}

			Ptr<BasicLanguageCodeException> BasicLanguageCodeException::GetGenericArgumentAlreadyExists(BasicDeclaration* declaration, const WString& name)
			{
				Array<WString> parameters(1);
				parameters[0]=name;
				return new BasicLanguageCodeException(declaration, GenericArgumentAlreadyExists, parameters.Wrap());
			}

			Ptr<BasicLanguageCodeException> BasicLanguageCodeException::GetCannotUseUninstanciatedGenericType(BasicType* type)
			{
				Array<WString> parameters(0);
				return new BasicLanguageCodeException(type, CannotUseUninstanciatedGenericType, parameters.Wrap());
			}
			
			Ptr<BasicLanguageCodeException> BasicLanguageCodeException::GetCannotUseUninstanciatedGenericVariable(BasicReferenceExpression* expression)
			{
				Array<WString> parameters(1);
				parameters[0]=expression->name;
				return new BasicLanguageCodeException(expression, CannotUseUninstanciatedGenericVariable, parameters.Wrap());
			}

			Ptr<BasicLanguageCodeException> BasicLanguageCodeException::GetCannotUseUninstanciatedGenericFunction(BasicReferenceExpression* expression)
			{
				Array<WString> parameters(1);
				parameters[0]=expression->name;
				return new BasicLanguageCodeException(expression, CannotUseUninstanciatedGenericFunction, parameters.Wrap());
			}

			Ptr<BasicLanguageCodeException> BasicLanguageCodeException::GetGenericArgumentNumberNotMatch(BasicType* type)
			{
				Array<WString> parameters(0);
				return new BasicLanguageCodeException(type, GenericArgumentNumberNotMatch, parameters.Wrap());
			}

			Ptr<BasicLanguageCodeException> BasicLanguageCodeException::GetGenericArgumentNumberNotMatch(BasicInstanciatedExpression* expression)
			{
				Array<WString> parameters(0);
				return new BasicLanguageCodeException(expression, GenericArgumentNumberNotMatch, parameters.Wrap());
			}

			Ptr<BasicLanguageCodeException> BasicLanguageCodeException::GetGenericArgumentCannotApplyToNonGenericType(BasicType* type)
			{
				Array<WString> parameters(0);
				return new BasicLanguageCodeException(type, GenericArgumentCannotApplyToNonGenericType, parameters.Wrap());
			}

			Ptr<BasicLanguageCodeException> BasicLanguageCodeException::GetGenericArgumentCannotApplyToNonGenericType(BasicInstanciatedExpression* expression)
			{
				Array<WString> parameters(0);
				return new BasicLanguageCodeException(expression, GenericArgumentCannotApplyToNonGenericType, parameters.Wrap());
			}

			Ptr<BasicLanguageCodeException> BasicLanguageCodeException::GetConceptFunctionAlreadyExists(BasicConceptBaseDeclaration* declaration, const WString& name)
			{
				Array<WString> parameters(1);
				parameters[0]=name;
				return new BasicLanguageCodeException(declaration, ConceptFunctionAlreadyExists, parameters.Wrap());
			}

			Ptr<BasicLanguageCodeException> BasicLanguageCodeException::GetConceptAlreadyExists(BasicDeclaration* declaration)
			{
				Array<WString> parameters(1);
				parameters[0]=declaration->name;
				return new BasicLanguageCodeException(declaration, ConceptAlreadyExists, parameters.Wrap());
			}

			Ptr<BasicLanguageCodeException> BasicLanguageCodeException::GetConceptNotExists(BasicConceptInstanceDeclaration* declaration)
			{
				Array<WString> parameters(1);
				parameters[0]=declaration->name;
				return new BasicLanguageCodeException(declaration, ConceptNotExists, parameters.Wrap());
			}

			Ptr<BasicLanguageCodeException> BasicLanguageCodeException::GetConceptFunctionTypeNotMatches(BasicConceptInstanceDeclaration* declaration, const WString& name)
			{
				Array<WString> parameters(1);
				parameters[0]=name;
				return new BasicLanguageCodeException(declaration, ConceptFunctionTypeNotMatches, parameters.Wrap());
			}

			Ptr<BasicLanguageCodeException> BasicLanguageCodeException::GetInstanceShouldHaveFunction(BasicConceptInstanceDeclaration* declaration, const WString& name)
			{
				Array<WString> parameters(1);
				parameters[0]=name;
				return new BasicLanguageCodeException(declaration, InstanceShouldHaveFunction, parameters.Wrap());
			}

			Ptr<BasicLanguageCodeException> BasicLanguageCodeException::GetInstanceShouldNotHaveFunction(BasicConceptInstanceDeclaration* declaration, const WString& name)
			{
				Array<WString> parameters(1);
				parameters[0]=name;
				return new BasicLanguageCodeException(declaration, InstanceShouldNotHaveFunction, parameters.Wrap());
			}
		}
	}
}