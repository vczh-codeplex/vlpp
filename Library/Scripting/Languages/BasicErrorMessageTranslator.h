/***********************************************************************
Vczh Library++ 3.0
Developer: ³Âè÷å«(vczh)
Scripting::Language Provider

Classes:
***********************************************************************/

#ifndef VCZH_SCRIPTING_LANGUAGES_BASICERRORMESSAGETRANSLATOR
#define VCZH_SCRIPTING_LANGUAGES_BASICERRORMESSAGETRANSLATOR

#include "LanguageProvider.h"
#include "BasicErrorMessage.h"
#include "..\BasicLanguage\BasicLanguageExpression.h"
#include "..\BasicLanguage\BasicLanguageException.h"
#include "..\BasicLanguage\BasicLanguageTypeManager.h"

namespace vl
{
	namespace scripting
	{
		namespace basiclanguage
		{
			class BasicErrorMessageTranslator : public Object
			{
			protected:
				virtual BasicTypeRecord*		GetExpressionType(BasicExpression* expression)=0;
				virtual WString					ToString(BasicType* type)=0;
				virtual WString					ToString(BasicTypeRecord* type)=0;
				virtual WString					OpToString(BasicUnaryExpression* expression)=0;
				virtual WString					OpToString(BasicBinaryExpression* expression)=0;
				virtual WString					BreakStatementToString()=0;
				virtual WString					ContinueStatementToString()=0;
				virtual BasicTypeRecord*		GetIntegerType()=0;
				virtual BasicTypeRecord*		GetBooleanType()=0;
			public:
				Ptr<LanguageException>			Translate(Ptr<BasicLanguageCodeException> error, int codeIndex);
			};
		}
	}
}

#endif