#include "ManagedLanguageException.h"
#include "..\..\Collections\Operation.h"

namespace vl
{
	namespace scripting
	{
		namespace managedlanguage
		{
			using namespace collections;

/***********************************************************************
ManagedLanguageCodeException
***********************************************************************/

			ManagedLanguageCodeException::ManagedLanguageCodeException(ManagedLanguageElement* _element, ExceptionCode _exceptionCode, const collections::IReadonlyList<WString>& _parameters, ManagedTypeSymbol* _typeParameter)
				:element(_element)
				,typeParameter(_typeParameter)
				,exceptionCode(_exceptionCode)
			{
				CopyFrom(parameters.Wrap(), _parameters);
			}

			ManagedLanguageCodeException::ManagedLanguageCodeException(const ManagedLanguageCodeException& exception)
				:element(exception.element)
				,typeParameter(exception.typeParameter)
				,exceptionCode(exception.exceptionCode)
			{
				CopyFrom(parameters.Wrap(), exception.parameters.Wrap());
			}

			ManagedLanguageCodeException::~ManagedLanguageCodeException()
			{
			}

			ManagedLanguageElement* ManagedLanguageCodeException::GetManagedLanguageElement()const
			{
				return element;
			}

			ManagedTypeSymbol* ManagedLanguageCodeException::GetTypeParameter()const
			{
				return typeParameter;
			}

			ManagedLanguageCodeException::ExceptionCode ManagedLanguageCodeException::GetExceptionCode()const
			{
				return exceptionCode;
			}

			const collections::IReadonlyList<WString>& ManagedLanguageCodeException::GetParameters()const
			{
				return parameters.Wrap();
			}

/***********************************************************************
ManagedLanguageCodeException::ExceptionCode
***********************************************************************/

			Ptr<ManagedLanguageCodeException> ManagedLanguageCodeException::GetSymbolAlreadyDefined(ManagedLanguageElement* element, const WString& name)
			{
				Array<WString> parameters(1);
				parameters[0]=name;
				return new ManagedLanguageCodeException(element, SymbolAlreadyDefined, parameters.Wrap());
			}

			Ptr<ManagedLanguageCodeException> ManagedLanguageCodeException::GetNamespaceNotExists(ManagedUsingNamespaceDeclaration* element)
			{
				Array<WString> parameters(0);
				return new ManagedLanguageCodeException(element, NamespaceNotExists, parameters.Wrap());
			}

			Ptr<ManagedLanguageCodeException> ManagedLanguageCodeException::GetSystemTypeNotExists(ManagedLanguageElement* element, const WString& name)
			{
				Array<WString> parameters(1);
				parameters[0]=name;
				return new ManagedLanguageCodeException(element, SystemTypeNotExists, parameters.Wrap());
			}

			Ptr<ManagedLanguageCodeException> ManagedLanguageCodeException::GetSystemTypeDuplicated(ManagedLanguageElement* element, const WString& name)
			{
				Array<WString> parameters(1);
				parameters[0]=name;
				return new ManagedLanguageCodeException(element, SystemTypeDuplicated, parameters.Wrap());
			}

			Ptr<ManagedLanguageCodeException> ManagedLanguageCodeException::GetTypeNotExists(ManagedType* element)
			{
				Array<WString> parameters(0);
				return new ManagedLanguageCodeException(element, TypeNotExists, parameters.Wrap());
			}

			Ptr<ManagedLanguageCodeException> ManagedLanguageCodeException::GetTypeDuplicated(ManagedType* element)
			{
				Array<WString> parameters(0);
				return new ManagedLanguageCodeException(element, TypeDuplicated, parameters.Wrap());
			}

			Ptr<ManagedLanguageCodeException> ManagedLanguageCodeException::GetIllegalAutoRefer(ManagedAutoReferType* element)
			{
				Array<WString> parameters(0);
				return new ManagedLanguageCodeException(element, IllegalAutoRefer, parameters.Wrap());
			}

			Ptr<ManagedLanguageCodeException> ManagedLanguageCodeException::GetCannotDirectReferTypeRenameAsToplevel(ManagedTypeRenameDeclaration* element)
			{
				Array<WString> parameters(1);
				parameters[0]=element->name;
				return new ManagedLanguageCodeException(element, CannotDirectReferTypeRenameAsToplevel, parameters.Wrap());
			}

			Ptr<ManagedLanguageCodeException> ManagedLanguageCodeException::GetIllegalNestedDeclaration(ManagedTypeMember* element)
			{
				Array<WString> parameters(0);
				return new ManagedLanguageCodeException(element, IllegalNestedDeclaration, parameters.Wrap());
			}

			Ptr<ManagedLanguageCodeException> ManagedLanguageCodeException::GetTypeInvisible(ManagedLanguageElement* element, ManagedTypeSymbol* type)
			{
				Array<WString> parameters(0);
				return new ManagedLanguageCodeException(element, TypeInvisible, parameters.Wrap(), type);
			}

			Ptr<ManagedLanguageCodeException> ManagedLanguageCodeException::GetTypeInvisibleOutSideOfAssembly(ManagedLanguageElement* element, ManagedTypeSymbol* type)
			{
				Array<WString> parameters(0);
				return new ManagedLanguageCodeException(element, TypeInvisibleOutSideOfAssembly, parameters.Wrap(), type);
			}

			Ptr<ManagedLanguageCodeException> ManagedLanguageCodeException::GetCannotUseUninstantiatedGenericType(ManagedLanguageElement* element, ManagedTypeSymbol* type)
			{
				Array<WString> parameters(0);
				return new ManagedLanguageCodeException(element, CannotUseUninstantiatedGenericType, parameters.Wrap(), type);
			}

			Ptr<ManagedLanguageCodeException> ManagedLanguageCodeException::GetGenericTypeArgumentCountNotMatches(ManagedLanguageElement* element, ManagedTypeSymbol* type)
			{
				Array<WString> parameters(0);
				return new ManagedLanguageCodeException(element, GenericTypeArgumentCountNotMatches, parameters.Wrap(), type);
			}

			Ptr<ManagedLanguageCodeException> ManagedLanguageCodeException::GetGenericTypeTypeConstraintNotSatisfied(ManagedLanguageElement* element, ManagedTypeSymbol* genericType, vint index)
			{
				Array<WString> parameters(1);
				parameters[0]=itow(index);
				return new ManagedLanguageCodeException(element, GenericTypeTypeConstraintNotSatisfied, parameters.Wrap(), genericType);
			}

			Ptr<ManagedLanguageCodeException> ManagedLanguageCodeException::GetGenericTypeNewConstraintNotSatisfied(ManagedLanguageElement* element, ManagedTypeSymbol* genericType, vint index)
			{
				Array<WString> parameters(1);
				parameters[0]=itow(index);
				return new ManagedLanguageCodeException(element, GenericTypeNewConstraintNotSatisfied, parameters.Wrap(), genericType);
			}

			Ptr<ManagedLanguageCodeException> ManagedLanguageCodeException::GetExpressionResolvedToDuplicatedTargets(ManagedExpression* expression, ManagedTypeSymbol* type)
			{
				Array<WString> parameters(0);
				return new ManagedLanguageCodeException(expression, ExpressionResolvedToDuplicatedTargets, parameters.Wrap(), type);
			}

			Ptr<ManagedLanguageCodeException> ManagedLanguageCodeException::GetExpressionCannotConvertToType(ManagedExpression* expression, ManagedTypeSymbol* type)
			{
				Array<WString> parameters(0);
				return new ManagedLanguageCodeException(expression, ExpressionCannotConvertToType, parameters.Wrap(), type);
			}

			Ptr<ManagedLanguageCodeException> ManagedLanguageCodeException::GetIllegalAutoRefWithoutInitializer(ManagedVariableStatement* statement)
			{
				Array<WString> parameters(1);
				parameters[0]=statement->name;
				return new ManagedLanguageCodeException(statement, IllegalAutoRefWithoutInitializer, parameters.Wrap());
			}

			Ptr<ManagedLanguageCodeException> ManagedLanguageCodeException::GetVariableAlreadyExists(ManagedVariableStatement* statement)
			{
				Array<WString> parameters(1);
				parameters[0]=statement->name;
				return new ManagedLanguageCodeException(statement, VariableAlreadyExists, parameters.Wrap());
			}

			Ptr<ManagedLanguageCodeException> ManagedLanguageCodeException::GetIllegalBreak(ManagedBreakStatement* statement)
			{
				Array<WString> parameters(0);
				return new ManagedLanguageCodeException(statement, IllegalBreak, parameters.Wrap());
			}

			Ptr<ManagedLanguageCodeException> ManagedLanguageCodeException::GetIllegalContinue(ManagedContinueStatement* statement)
			{
				Array<WString> parameters(0);
				return new ManagedLanguageCodeException(statement, IllegalContinue, parameters.Wrap());
			}

			Ptr<ManagedLanguageCodeException> ManagedLanguageCodeException::GetIllegalThrow(ManagedThrowStatement* statement)
			{
				Array<WString> parameters(0);
				return new ManagedLanguageCodeException(statement, IllegalThrow, parameters.Wrap());
			}

			Ptr<ManagedLanguageCodeException> ManagedLanguageCodeException::GetExceptionTypeShouldDerivedFromException(ManagedType* type, ManagedTypeSymbol* exceptionType)
			{
				Array<WString> parameters(0);
				return new ManagedLanguageCodeException(type, ExceptionTypeShouldDerivedFromException, parameters.Wrap(), exceptionType);
			}

			Ptr<ManagedLanguageCodeException> ManagedLanguageCodeException::GetExceptionTypeShouldDerivedFromException(ManagedExpression* expression, ManagedTypeSymbol* exceptionType)
			{
				Array<WString> parameters(0);
				return new ManagedLanguageCodeException(expression, ExceptionTypeShouldDerivedFromException, parameters.Wrap(), exceptionType);
			}
		}
	}
}