/***********************************************************************
Vczh Library++ 3.0
Developer: ³Âè÷å«(vczh)
Scripting::ManagedLanguage

Classes:
***********************************************************************/

#ifndef VCZH_SCRIPTING_MANAGEDLANGUAGE_MANAGEDLANGUAGEEXCEPTION
#define VCZH_SCRIPTING_MANAGEDLANGUAGE_MANAGEDLANGUAGEEXCEPTION

#include "ManagedLanguageSymbolManager.h"
#include "..\..\Exception.h"

namespace vl
{
	namespace scripting
	{
		namespace managedlanguage
		{
			class ManagedLanguageCodeException : public Exception
			{
			public:
				enum ExceptionCode
				{
					SymbolAlreadyDefined,	// name
				};
			protected:
				ManagedLanguageElement*											element;
				ManagedTypeSymbol*												typeParameter;
				ExceptionCode													exceptionCode;
				collections::List<WString>										parameters;
			public:
				ManagedLanguageCodeException(ManagedLanguageElement* _element, ExceptionCode _exceptionCode, const collections::IReadonlyList<WString>& _parameters, ManagedTypeSymbol* _typeParameter=0);
				ManagedLanguageCodeException(const ManagedLanguageCodeException& exception);
				~ManagedLanguageCodeException();

				ManagedLanguageElement*											GetBasicLanguageElement()const;
				ManagedTypeSymbol*												GetTypeParameter()const;
				ExceptionCode													GetExceptionCode()const;
				const collections::IReadonlyList<WString>&						GetParameters()const;

				static Ptr<ManagedLanguageCodeException>						GetSymbolAlreadyDefined(ManagedLanguageElement* element, const WString& name);
			};
		}
	}
}

#endif