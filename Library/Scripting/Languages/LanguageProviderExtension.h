/***********************************************************************
Vczh Library++ 3.0
Developer: 陈梓瀚(vczh)
Scripting::Language Provider

Interfaces:
  IBasicLanguageProvider						：程序语言服务提供者
***********************************************************************/

#ifndef VCZH_SCRIPTING_LANGUAGES_LANGUAGEPROVIDEREXTENSION
#define VCZH_SCRIPTING_LANGUAGES_LANGUAGEPROVIDEREXTENSION

#include "LanguageProvider.h"
#include "..\BasicLanguage\BasicLanguageExpression.h"

namespace vl
{
	namespace scripting
	{
		class IBasicLanguageProvider : public Interface
		{
		public:
			virtual Ptr<basiclanguage::BasicProgram>	ParseProgram(
															const WString& code, 
															collections::IList<Ptr<LanguageException>>& errors
															)=0;

			virtual void								GenerateCode(
															Ptr<basiclanguage::BasicProgram> program,
															stream::TextWriter& writer
															)=0;
		};
	}
}

#endif