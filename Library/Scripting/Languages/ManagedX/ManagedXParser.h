/***********************************************************************
Vczh Library++ 3.0
Developer: ³Âè÷å«(vczh)
Scripting::Managed X

Classes:
***********************************************************************/

#ifndef VCZH_SCRIPTING_LANGUAGES_MANAGEDX_MANAGEDXPARSER
#define VCZH_SCRIPTING_LANGUAGES_MANAGEDX_MANAGEDXPARSER

#include "ManagedX.h"
#include "..\..\ManagedLanguage\ManagedLanguageExpression.h"

namespace vl
{
	namespace scripting
	{
		namespace language_managedx
		{
			using namespace collections;
			using namespace managedlanguage;

			class ManagedXUnit : public Object
			{
			public:
				Ptr<ManagedProgram>				program;
				vint							codeIndex;
			};

			class ManagedXParser : public Object
			{
			public:
				static Ptr<ManagedXParser>		Create();

				virtual Ptr<ManagedXUnit>		Parse(const WString& code, vint codeIndex, IList<Ptr<LanguageException>>& errors)=0;
			};
		}
	}
}

#endif