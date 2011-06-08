/***********************************************************************
Vczh Library++ 3.0
Developer: �����(vczh)
Scripting::Language Provider

Interfaces:
  IManagedLanguageProvider						���������Է����ṩ��
***********************************************************************/

#ifndef VCZH_SCRIPTING_LANGUAGES_LANGUAGEPROVIDEREXTENSION
#define VCZH_SCRIPTING_LANGUAGES_LANGUAGEPROVIDEREXTENSION

#include "LanguageProvider.h"
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

			virtual void									GenerateCode(
																Ptr<managedlanguage::ManagedProgram> program,
																Ptr<Object> inputExtra,
																stream::TextWriter& writer
																)=0;

			virtual bool									GenerateHeader(
																Ptr<LanguageAssembly> assembly,
																Ptr<Object> inputExtra,
																stream::TextWriter& writer
																)=0;
		};
	}
}

#endif