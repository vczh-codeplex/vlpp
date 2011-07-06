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
		}
	}
}