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
					SymbolAlreadyDefined,						// <all>								:name
					NamespaceNotExists,							// <ManagedUsingNamespaceDeclaration>
					SystemTypeNotExists,						// <all>								:name
					SystemTypeDuplicated,						// <all>								:name
					TypeNotExists,								// <ManagedType>
					TypeDuplicated,								// <ManagedType>
					IllegalAutoRefer,							// <ManagedAutoReferType>
					CannotDirectReferTypeRenameAsToplevel,		// <ManagedTypeRenameDeclaration>		:name
					IllegalNestedDeclaration,					// <ManagedTypeMember>
					TypeInvisible,								// <all>
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

				ManagedLanguageElement*											GetManagedLanguageElement()const;
				ManagedTypeSymbol*												GetTypeParameter()const;
				ExceptionCode													GetExceptionCode()const;
				const collections::IReadonlyList<WString>&						GetParameters()const;

				static Ptr<ManagedLanguageCodeException>						GetSymbolAlreadyDefined(ManagedLanguageElement* element, const WString& name);
				static Ptr<ManagedLanguageCodeException>						GetNamespaceNotExists(ManagedUsingNamespaceDeclaration* element);
				static Ptr<ManagedLanguageCodeException>						GetSystemTypeNotExists(ManagedLanguageElement* element, const WString& name);
				static Ptr<ManagedLanguageCodeException>						GetSystemTypeDuplicated(ManagedLanguageElement* element, const WString& name);
				static Ptr<ManagedLanguageCodeException>						GetTypeNotExists(ManagedType* element);
				static Ptr<ManagedLanguageCodeException>						GetTypeDuplicated(ManagedType* element);
				static Ptr<ManagedLanguageCodeException>						GetIllegalAutoRefer(ManagedAutoReferType* element);
				static Ptr<ManagedLanguageCodeException>						GetCannotDirectReferTypeRenameAsToplevel(ManagedTypeRenameDeclaration* element);
				static Ptr<ManagedLanguageCodeException>						GetIllegalNestedDeclaration(ManagedTypeMember* element);
				static Ptr<ManagedLanguageCodeException>						GetTypeInvisible(ManagedLanguageElement* element, ManagedTypeSymbol* type);
			};
		}
	}
}

#endif