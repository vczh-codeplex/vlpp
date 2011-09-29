/***********************************************************************
Vczh Library++ 3.0
Developer: 陈梓瀚(vczh)
Scripting::Language Provider

Interfaces:
  IManagedLanguageProvider						：程序语言服务提供者
***********************************************************************/

#ifndef VCZH_SCRIPTING_LANGUAGES_LANGUAGEPROVIDERMANAGEDEXTENSION
#define VCZH_SCRIPTING_LANGUAGES_LANGUAGEPROVIDERMANAGEDEXTENSION

#include "LanguageProvider.h"
#include "..\BasicLanguage\BasicLanguageExpression.h"
#include "..\ManagedLanguage\ManagedLanguageExpression.h"

namespace vl
{
	namespace scripting
	{
		class IManagedLanguageProvider : public Interface
		{
		public:
			virtual Ptr<managedlanguage::ManagedProgram>	ParseProgram(
																const WString& code, 
																Ptr<Object>& outputExtra,
																collections::IList<Ptr<LanguageException>>& errors
																)=0;

			virtual Ptr<basiclanguage::BasicProgram>		CompileToBasicProgram(
																collections::IReadonlyList<Ptr<LanguageAssembly>>& references,
																collections::IReadonlyList<WString>& codes, 
																collections::IList<Ptr<LanguageException>>& errors
																)=0;

			virtual void									GenerateCode(
																Ptr<managedlanguage::ManagedProgram> program,
																Ptr<Object> inputExtra,
																stream::TextWriter& writer
																)=0;
		};
	}
}

#endif