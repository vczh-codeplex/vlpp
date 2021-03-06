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

			BasicLanguageCodeException::BasicLanguageCodeException(BasicLanguageElement* _element, ExceptionCode _exceptionCode, const collections::IReadonlyList<WString>& _parameters, BasicTypeRecord* _typeParameter)
				:Exception(L"����BasicLanguage�����������")
				,element(_element)
				,typeParameter(_typeParameter)
				,exceptionCode(_exceptionCode)
			{
				CopyFrom(parameters.Wrap(), _parameters);
			}

			BasicLanguageCodeException::BasicLanguageCodeException(const BasicLanguageCodeException& exception)
				:Exception(exception.Message())
				,element(exception.element)
				,typeParameter(exception.typeParameter)
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

			BasicTypeRecord* BasicLanguageCodeException::GetTypeParameter()const
			{
				return typeParameter;
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

			Ptr<BasicLanguageCodeException> BasicLanguageCodeException::GetStructureMemberNotExists(BasicOffsetofExpression* member)
			{
				Array<WString> parameters(1);
				parameters[0]=member->member;
				return new BasicLanguageCodeException(member, StructureMemberNotExists, parameters.Wrap());
			}

			Ptr<BasicLanguageCodeException> BasicLanguageCodeException::GetStructureMemberNotExists(BasicTypeofMemberType* member)
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

			Ptr<BasicLanguageCodeException> BasicLanguageCodeException::GetLeftOperandShouldBeStructure(BasicMemberExpression* expression)
			{
				Array<WString> parameters(0);
				return new BasicLanguageCodeException(expression, LeftOperandShouldBeStructure, parameters.Wrap());
			}

			Ptr<BasicLanguageCodeException> BasicLanguageCodeException::GetLeftOperandShouldBeStructure(BasicOffsetofExpression* expression)
			{
				Array<WString> parameters(0);
				return new BasicLanguageCodeException(expression, LeftOperandShouldBeStructure, parameters.Wrap());
			}

			Ptr<BasicLanguageCodeException> BasicLanguageCodeException::GetLeftOperandShouldBeStructure(BasicTypeofMemberType* type)
			{
				Array<WString> parameters(0);
				return new BasicLanguageCodeException(type, LeftOperandShouldBeStructure, parameters.Wrap());
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

			Ptr<BasicLanguageCodeException> BasicLanguageCodeException::GetGenericArgumentNotExists(BasicDeclaration* declaration, const WString& name)
			{
				Array<WString> parameters(1);
				parameters[0]=name;
				return new BasicLanguageCodeException(declaration, GenericArgumentNotExists, parameters.Wrap());
			}

			Ptr<BasicLanguageCodeException> BasicLanguageCodeException::GetCannotUseUninstantiatedGenericType(BasicType* type)
			{
				Array<WString> parameters(0);
				return new BasicLanguageCodeException(type, CannotUseUninstantiatedGenericType, parameters.Wrap());
			}
			
			Ptr<BasicLanguageCodeException> BasicLanguageCodeException::GetCannotUseUninstantiatedGenericVariable(BasicReferenceExpression* expression)
			{
				Array<WString> parameters(1);
				parameters[0]=expression->name;
				return new BasicLanguageCodeException(expression, CannotUseUninstantiatedGenericVariable, parameters.Wrap());
			}

			Ptr<BasicLanguageCodeException> BasicLanguageCodeException::GetCannotUseUninstantiatedGenericFunction(BasicReferenceExpression* expression)
			{
				Array<WString> parameters(1);
				parameters[0]=expression->name;
				return new BasicLanguageCodeException(expression, CannotUseUninstantiatedGenericFunction, parameters.Wrap());
			}

			Ptr<BasicLanguageCodeException> BasicLanguageCodeException::GetGenericArgumentNumberNotMatch(BasicType* type)
			{
				Array<WString> parameters(0);
				return new BasicLanguageCodeException(type, GenericArgumentNumberNotMatch, parameters.Wrap());
			}

			Ptr<BasicLanguageCodeException> BasicLanguageCodeException::GetGenericArgumentNumberNotMatch(BasicInstantiatedExpression* expression)
			{
				Array<WString> parameters(0);
				return new BasicLanguageCodeException(expression, GenericArgumentNumberNotMatch, parameters.Wrap());
			}

			Ptr<BasicLanguageCodeException> BasicLanguageCodeException::GetGenericArgumentCannotApplyToNonGenericType(BasicType* type)
			{
				Array<WString> parameters(0);
				return new BasicLanguageCodeException(type, GenericArgumentCannotApplyToNonGenericType, parameters.Wrap());
			}

			Ptr<BasicLanguageCodeException> BasicLanguageCodeException::GetGenericArgumentCannotApplyToNonGenericType(BasicInstantiatedExpression* expression)
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

			Ptr<BasicLanguageCodeException> BasicLanguageCodeException::GetConceptFunctionNotExists(BasicInstanceFunctionExpression* expression)
			{
				Array<WString> parameters(2);
				parameters[0]=expression->conceptName;
				parameters[1]=expression->functionName;
				return new BasicLanguageCodeException(expression, ConceptFunctionAlreadyExists, parameters.Wrap());
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

			Ptr<BasicLanguageCodeException> BasicLanguageCodeException::GetConceptNotExists(BasicDeclaration* declaration, const WString& conceptName)
			{
				Array<WString> parameters(1);
				parameters[0]=conceptName;
				return new BasicLanguageCodeException(declaration, ConceptNotExists, parameters.Wrap());
			}

			Ptr<BasicLanguageCodeException> BasicLanguageCodeException::GetInstanceTypeNotCorrect(BasicConceptInstanceDeclaration* declaration)
			{
				Array<WString> parameters(0);
				return new BasicLanguageCodeException(declaration, InstanceTypeNotCorrect, parameters.Wrap());
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

			Ptr<BasicLanguageCodeException> BasicLanguageCodeException::GetInstanceShouldBeDeclaredOnType(BasicType* typeExpression, BasicTypeRecord* type, const WString& conceptName)
			{
				Array<WString> parameters(1);
				parameters[0]=conceptName;
				return new BasicLanguageCodeException(typeExpression, InstanceShouldBeDeclaredOnType, parameters.Wrap(), type);
			}

			Ptr<BasicLanguageCodeException> BasicLanguageCodeException::GetCannotThrowVoidValue(BasicThrowStatement* statement)
			{
				Array<WString> parameters(0);
				return new BasicLanguageCodeException(statement, CannotThrowVoidValue, parameters.Wrap());
			}

			Ptr<BasicLanguageCodeException> BasicLanguageCodeException::GetForeignFunctionCannotBeGeneric(BasicFunctionDeclaration* declaration)
			{
				Array<WString> parameters(1);
				parameters[0]=declaration->name;
				return new BasicLanguageCodeException(declaration, ForeignFunctionCannotBeGeneric, parameters.Wrap());
			}

			Ptr<BasicLanguageCodeException> BasicLanguageCodeException::GetAttributeCannotApplyOnFunctionDeclaration(BasicFunctionDeclaration* declaration, const WString& attributeName)
			{
				Array<WString> parameters(2);
				parameters[0]=attributeName;
				parameters[1]=declaration->name;
				return new BasicLanguageCodeException(declaration, AttributeCannotApplyOnFunctionDeclaration, parameters.Wrap());
			}

			Ptr<BasicLanguageCodeException> BasicLanguageCodeException::GetAttributeCannotApplyOnStructureDeclaration(BasicStructureDeclaration* declaration, const WString& attributeName)
			{
				Array<WString> parameters(2);
				parameters[0]=attributeName;
				parameters[1]=declaration->name;
				return new BasicLanguageCodeException(declaration, AttributeCannotApplyOnStructureDeclaration, parameters.Wrap());
			}

			Ptr<BasicLanguageCodeException> BasicLanguageCodeException::GetAttributeCannotApplyOnVariableDeclaration(BasicVariableDeclaration* declaration, const WString& attributeName)
			{
				Array<WString> parameters(2);
				parameters[0]=attributeName;
				parameters[1]=declaration->name;
				return new BasicLanguageCodeException(declaration, AttributeCannotApplyOnVariableDeclaration, parameters.Wrap());
			}

			Ptr<BasicLanguageCodeException> BasicLanguageCodeException::GetAttributeCannotApplyOnTypeRenameDeclaration(BasicTypeRenameDeclaration* declaration, const WString& attributeName)
			{
				Array<WString> parameters(2);
				parameters[0]=attributeName;
				parameters[1]=declaration->name;
				return new BasicLanguageCodeException(declaration, AttributeCannotApplyOnTypeRenameDeclaration, parameters.Wrap());
			}

			Ptr<BasicLanguageCodeException> BasicLanguageCodeException::GetAttributeCannotApplyOnConceptDeclaration(BasicConceptBaseDeclaration* declaration, const WString& attributeName)
			{
				Array<WString> parameters(2);
				parameters[0]=attributeName;
				parameters[1]=declaration->name;
				return new BasicLanguageCodeException(declaration, AttributeCannotApplyOnConceptDeclaration, parameters.Wrap());
			}

			Ptr<BasicLanguageCodeException> BasicLanguageCodeException::GetAttributeCannotApplyOnInstanceDeclaration(BasicConceptInstanceDeclaration* declaration, const WString& attributeName)
			{
				Array<WString> parameters(2);
				parameters[0]=attributeName;
				parameters[1]=declaration->name;
				return new BasicLanguageCodeException(declaration, AttributeCannotApplyOnInstanceDeclaration, parameters.Wrap());
			}

			Ptr<BasicLanguageCodeException> BasicLanguageCodeException::GetPublicDeclarationCannotUseNonPublicType(BasicDeclaration* declaration, BasicTypeRecord* type)
			{
				Array<WString> parameters(1);
				parameters[0]=declaration->name;
				return new BasicLanguageCodeException(declaration, PublicDeclarationCannotUseNonPublicType, parameters.Wrap(), type);
			}

			Ptr<BasicLanguageCodeException> BasicLanguageCodeException::GetInstanceDeclaredOnPublicConceptShouldBePublic(BasicConceptInstanceDeclaration* declaration)
			{
				Array<WString> parameters(1);
				parameters[0]=declaration->name;
				return new BasicLanguageCodeException(declaration, InstanceDeclaredOnPublicConceptShouldBePublic, parameters.Wrap());
			}

			Ptr<BasicLanguageCodeException> BasicLanguageCodeException::GetInstanceDeclaredOnNonPublicConceptShouldBeNonPublic(BasicConceptInstanceDeclaration* declaration)
			{
				Array<WString> parameters(1);
				parameters[0]=declaration->name;
				return new BasicLanguageCodeException(declaration, InstanceDeclaredOnNonPublicConceptShouldBeNonPublic, parameters.Wrap());
			}

			Ptr<BasicLanguageCodeException> BasicLanguageCodeException::GetArrayElementCountShouldBeIntegerConstantExpression(BasicExpression* expression)
			{
				Array<WString> parameters(0);
				return new BasicLanguageCodeException(expression, ArrayElementCountShouldBeIntegerConstantExpression, parameters.Wrap());
			}

			Ptr<BasicLanguageCodeException> BasicLanguageCodeException::GetArrayElementCountShouldBePositive(BasicExpression* expression)
			{
				Array<WString> parameters(0);
				return new BasicLanguageCodeException(expression, ArrayElementCountShouldBePositive, parameters.Wrap());
			}

			Ptr<BasicLanguageCodeException> BasicLanguageCodeException::GetConstantInitializerShouldBeConstantExpression(BasicVariableDeclaration* declaration)
			{
				Array<WString> parameters(0);
				return new BasicLanguageCodeException(declaration, ConstantInitializerShouldBeConstantExpression, parameters.Wrap());
			}

			Ptr<BasicLanguageCodeException> BasicLanguageCodeException::GetConstantCannotBeGeneric(BasicVariableDeclaration* declaration)
			{
				Array<WString> parameters(0);
				return new BasicLanguageCodeException(declaration, ConstantCannotBeGeneric, parameters.Wrap());
			}

			Ptr<BasicLanguageCodeException> BasicLanguageCodeException::GetConstantCannotBeExternalSymbol(BasicVariableDeclaration* declaration)
			{
				Array<WString> parameters(0);
				return new BasicLanguageCodeException(declaration, ConstantCannotBeExternalSymbol, parameters.Wrap());
			}
		}
	}
}