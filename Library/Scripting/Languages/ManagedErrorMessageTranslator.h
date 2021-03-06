/***********************************************************************
Vczh Library++ 3.0
Developer: �����(vczh)
Scripting::Language Provider

Classes:
***********************************************************************/

#ifndef VCZH_SCRIPTING_LANGUAGES_MANAGEDERRORMESSAGETRANSLATOR
#define VCZH_SCRIPTING_LANGUAGES_MANAGEDERRORMESSAGETRANSLATOR

#include "LanguageProvider.h"
#include "..\ManagedLanguage\ManagedLanguageException.h"

namespace vl
{
	namespace scripting
	{
		namespace managedlanguage
		{
			class ManagedErrorMessageTranslator : public Object
			{
			protected:
				virtual WString					SymbolsToString(const collections::IReadonlyList<WString>& symbols)=0;
				virtual WString					TypeToString(ManagedType* type)=0;
				virtual WString					TypeToString(ManagedTypeSymbol* type)=0;
				virtual ManagedTypeSymbol*		ExpressionToType(ManagedExpression* expression)=0;
				virtual WString					ExpressionToString(ManagedExpression* expression)=0;
				virtual WString					MemberContainingTypeName(ManagedMember* member)=0;
				virtual WString					MemberName(ManagedMember* member)=0;
				virtual WString					MemberHeader(ManagedSymbolItem* member)=0;
			public:
				Ptr<LanguageException>			Translate(Ptr<ManagedLanguageCodeException> error, vint codeIndex);
			};
		}
	}
}

#endif